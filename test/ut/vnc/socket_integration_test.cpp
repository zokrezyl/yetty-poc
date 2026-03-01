#include <boost/ut.hpp>
#include "yetty/vnc/protocol.h"
#include <cstring>
#include <vector>
#include <thread>
#include <atomic>
#include <chrono>
#include <functional>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

using namespace boost::ut;
using namespace yetty::vnc;

// Test server that receives input and dispatches to callbacks
class TestVncServer {
public:
    std::function<void(int16_t, int16_t)> onMouseMove;
    std::function<void(int16_t, int16_t, MouseButton, bool)> onMouseButton;
    std::function<void(uint32_t, uint32_t, uint8_t)> onKeyDown;
    std::function<void(uint32_t, uint32_t, uint8_t)> onKeyUp;
    std::function<void(const std::string&)> onTextInput;

    int serverFd = -1;
    int clientFd = -1;
    uint16_t port = 0;
    std::atomic<bool> running{false};

    bool start(uint16_t p) {
        serverFd = socket(AF_INET, SOCK_STREAM, 0);
        if (serverFd < 0) return false;

        int opt = 1;
        setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        struct sockaddr_in addr = {};
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(p);

        if (bind(serverFd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            close(serverFd);
            serverFd = -1;
            return false;
        }

        if (listen(serverFd, 1) < 0) {
            close(serverFd);
            serverFd = -1;
            return false;
        }

        port = p;
        running = true;
        return true;
    }

    bool acceptClient(int timeoutMs = 1000) {
        struct pollfd pfd = {serverFd, POLLIN, 0};
        if (poll(&pfd, 1, timeoutMs) <= 0) return false;

        struct sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);
        clientFd = accept(serverFd, (struct sockaddr*)&clientAddr, &clientLen);
        if (clientFd < 0) return false;

        int flag = 1;
        setsockopt(clientFd, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag));
        return true;
    }

    void processInput(int timeoutMs = 100) {
        if (clientFd < 0) return;

        struct pollfd pfd = {clientFd, POLLIN, 0};
        while (poll(&pfd, 1, timeoutMs) > 0) {
            InputHeader hdr;
            ssize_t n = recv(clientFd, &hdr, sizeof(hdr), MSG_WAITALL);
            if (n != sizeof(hdr)) break;

            std::vector<uint8_t> data(hdr.data_size);
            if (hdr.data_size > 0) {
                n = recv(clientFd, data.data(), hdr.data_size, MSG_WAITALL);
                if (n != hdr.data_size) break;
            }

            dispatchInput(hdr, data.empty() ? nullptr : data.data());
        }
    }

    void dispatchInput(const InputHeader& hdr, const uint8_t* data) {
        switch (static_cast<InputType>(hdr.type)) {
            case InputType::MOUSE_MOVE:
                if (data && hdr.data_size >= sizeof(MouseMoveEvent) && onMouseMove) {
                    const MouseMoveEvent* m = reinterpret_cast<const MouseMoveEvent*>(data);
                    onMouseMove(m->x, m->y);
                }
                break;
            case InputType::MOUSE_BUTTON:
                if (data && hdr.data_size >= sizeof(MouseButtonEvent) && onMouseButton) {
                    const MouseButtonEvent* m = reinterpret_cast<const MouseButtonEvent*>(data);
                    onMouseButton(m->x, m->y, static_cast<MouseButton>(m->button), m->pressed != 0);
                }
                break;
            case InputType::KEY_DOWN:
                if (data && hdr.data_size >= sizeof(KeyEvent) && onKeyDown) {
                    const KeyEvent* k = reinterpret_cast<const KeyEvent*>(data);
                    onKeyDown(k->keycode, k->scancode, k->mods);
                }
                break;
            case InputType::KEY_UP:
                if (data && hdr.data_size >= sizeof(KeyEvent) && onKeyUp) {
                    const KeyEvent* k = reinterpret_cast<const KeyEvent*>(data);
                    onKeyUp(k->keycode, k->scancode, k->mods);
                }
                break;
            case InputType::TEXT_INPUT:
                if (data && hdr.data_size > 0 && onTextInput) {
                    onTextInput(std::string(reinterpret_cast<const char*>(data), hdr.data_size));
                }
                break;
            default:
                break;
        }
    }

    void stop() {
        running = false;
        if (clientFd >= 0) { close(clientFd); clientFd = -1; }
        if (serverFd >= 0) { close(serverFd); serverFd = -1; }
    }

    ~TestVncServer() { stop(); }
};

// Test client that sends input
class TestVncClient {
public:
    int sockFd = -1;

    bool connect(uint16_t port, int timeoutMs = 1000) {
        sockFd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockFd < 0) return false;

        struct sockaddr_in addr = {};
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        addr.sin_port = htons(port);

        if (::connect(sockFd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            close(sockFd);
            sockFd = -1;
            return false;
        }

        int flag = 1;
        setsockopt(sockFd, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag));
        return true;
    }

    bool sendData(const void* data, size_t size) {
        if (sockFd < 0) return false;
        const uint8_t* ptr = static_cast<const uint8_t*>(data);
        size_t remaining = size;
        while (remaining > 0) {
            ssize_t sent = send(sockFd, ptr, remaining, MSG_NOSIGNAL);
            if (sent <= 0) return false;
            ptr += sent;
            remaining -= sent;
        }
        return true;
    }

    bool sendMouseMove(int16_t x, int16_t y) {
        InputHeader hdr = {};
        hdr.type = static_cast<uint8_t>(InputType::MOUSE_MOVE);
        hdr.data_size = sizeof(MouseMoveEvent);

        MouseMoveEvent evt = {};
        evt.x = x;
        evt.y = y;

        uint8_t buf[sizeof(hdr) + sizeof(evt)];
        std::memcpy(buf, &hdr, sizeof(hdr));
        std::memcpy(buf + sizeof(hdr), &evt, sizeof(evt));
        return sendData(buf, sizeof(buf));
    }

    bool sendMouseButton(int16_t x, int16_t y, MouseButton button, bool pressed) {
        InputHeader hdr = {};
        hdr.type = static_cast<uint8_t>(InputType::MOUSE_BUTTON);
        hdr.data_size = sizeof(MouseButtonEvent);

        MouseButtonEvent evt = {};
        evt.x = x;
        evt.y = y;
        evt.button = static_cast<uint8_t>(button);
        evt.pressed = pressed ? 1 : 0;

        uint8_t buf[sizeof(hdr) + sizeof(evt)];
        std::memcpy(buf, &hdr, sizeof(hdr));
        std::memcpy(buf + sizeof(hdr), &evt, sizeof(evt));
        return sendData(buf, sizeof(buf));
    }

    bool sendKeyDown(uint32_t keycode, uint32_t scancode, uint8_t mods) {
        InputHeader hdr = {};
        hdr.type = static_cast<uint8_t>(InputType::KEY_DOWN);
        hdr.data_size = sizeof(KeyEvent);

        KeyEvent evt = {};
        evt.keycode = keycode;
        evt.scancode = scancode;
        evt.mods = mods;

        uint8_t buf[sizeof(hdr) + sizeof(evt)];
        std::memcpy(buf, &hdr, sizeof(hdr));
        std::memcpy(buf + sizeof(hdr), &evt, sizeof(evt));
        return sendData(buf, sizeof(buf));
    }

    bool sendKeyUp(uint32_t keycode, uint32_t scancode, uint8_t mods) {
        InputHeader hdr = {};
        hdr.type = static_cast<uint8_t>(InputType::KEY_UP);
        hdr.data_size = sizeof(KeyEvent);

        KeyEvent evt = {};
        evt.keycode = keycode;
        evt.scancode = scancode;
        evt.mods = mods;

        uint8_t buf[sizeof(hdr) + sizeof(evt)];
        std::memcpy(buf, &hdr, sizeof(hdr));
        std::memcpy(buf + sizeof(hdr), &evt, sizeof(evt));
        return sendData(buf, sizeof(buf));
    }

    bool sendTextInput(const std::string& text) {
        InputHeader hdr = {};
        hdr.type = static_cast<uint8_t>(InputType::TEXT_INPUT);
        hdr.data_size = static_cast<uint16_t>(text.size());

        std::vector<uint8_t> buf(sizeof(hdr) + text.size());
        std::memcpy(buf.data(), &hdr, sizeof(hdr));
        std::memcpy(buf.data() + sizeof(hdr), text.data(), text.size());
        return sendData(buf.data(), buf.size());
    }

    void disconnect() {
        if (sockFd >= 0) { close(sockFd); sockFd = -1; }
    }

    ~TestVncClient() { disconnect(); }
};

suite socket_integration_tests = [] {
    "client connects to server"_test = [] {
        TestVncServer server;
        expect(server.start(15900) == true);

        std::thread serverThread([&]() {
            server.acceptClient(2000);
        });

        TestVncClient client;
        expect(client.connect(15900) == true);

        serverThread.join();
        expect(server.clientFd >= 0);
    };

    "client sends mouse move to server"_test = [] {
        TestVncServer server;
        expect(server.start(15901) == true);

        int16_t receivedX = 0, receivedY = 0;
        server.onMouseMove = [&](int16_t x, int16_t y) {
            receivedX = x;
            receivedY = y;
        };

        std::thread serverThread([&]() {
            server.acceptClient(2000);
            server.processInput(500);
        });

        TestVncClient client;
        expect(client.connect(15901) == true);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        expect(client.sendMouseMove(123, 456) == true);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        serverThread.join();

        expect(receivedX == 123_i);
        expect(receivedY == 456_i);
    };

    "client sends mouse button to server"_test = [] {
        TestVncServer server;
        expect(server.start(15902) == true);

        int16_t rx = 0, ry = 0;
        MouseButton rBtn = MouseButton::LEFT;
        bool rPressed = false;

        server.onMouseButton = [&](int16_t x, int16_t y, MouseButton btn, bool pressed) {
            rx = x; ry = y; rBtn = btn; rPressed = pressed;
        };

        std::thread serverThread([&]() {
            server.acceptClient(2000);
            server.processInput(500);
        });

        TestVncClient client;
        expect(client.connect(15902) == true);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        expect(client.sendMouseButton(100, 200, MouseButton::RIGHT, true) == true);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        serverThread.join();

        expect(rx == 100_i);
        expect(ry == 200_i);
        expect(rBtn == MouseButton::RIGHT);
        expect(rPressed == true);
    };

    "client sends key down to server"_test = [] {
        TestVncServer server;
        expect(server.start(15903) == true);

        uint32_t rKeycode = 0, rScancode = 0;
        uint8_t rMods = 0;

        server.onKeyDown = [&](uint32_t keycode, uint32_t scancode, uint8_t mods) {
            rKeycode = keycode;
            rScancode = scancode;
            rMods = mods;
        };

        std::thread serverThread([&]() {
            server.acceptClient(2000);
            server.processInput(500);
        });

        TestVncClient client;
        expect(client.connect(15903) == true);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        expect(client.sendKeyDown(65, 30, MOD_SHIFT) == true);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        serverThread.join();

        expect(rKeycode == 65_i);
        expect(rScancode == 30_i);
        expect(rMods == MOD_SHIFT);
    };

    "client sends key up to server"_test = [] {
        TestVncServer server;
        expect(server.start(15904) == true);

        uint32_t rKeycode = 0;
        bool keyUpCalled = false;

        server.onKeyUp = [&](uint32_t keycode, uint32_t, uint8_t) {
            rKeycode = keycode;
            keyUpCalled = true;
        };

        std::thread serverThread([&]() {
            server.acceptClient(2000);
            server.processInput(500);
        });

        TestVncClient client;
        expect(client.connect(15904) == true);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        expect(client.sendKeyUp(66, 31, 0) == true);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        serverThread.join();

        expect(keyUpCalled == true);
        expect(rKeycode == 66_i);
    };

    "client sends text input to server"_test = [] {
        TestVncServer server;
        expect(server.start(15905) == true);

        std::string receivedText;

        server.onTextInput = [&](const std::string& text) {
            receivedText = text;
        };

        std::thread serverThread([&]() {
            server.acceptClient(2000);
            server.processInput(500);
        });

        TestVncClient client;
        expect(client.connect(15905) == true);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        expect(client.sendTextInput("Hello World!") == true);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        serverThread.join();

        expect(receivedText == "Hello World!");
    };

    "client sends multiple events to server"_test = [] {
        TestVncServer server;
        expect(server.start(15906) == true);

        int mouseMoveCnt = 0;
        int keyDownCnt = 0;

        server.onMouseMove = [&](int16_t, int16_t) { mouseMoveCnt++; };
        server.onKeyDown = [&](uint32_t, uint32_t, uint8_t) { keyDownCnt++; };

        std::thread serverThread([&]() {
            server.acceptClient(2000);
            server.processInput(500);
        });

        TestVncClient client;
        expect(client.connect(15906) == true);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        // Send multiple events
        for (int i = 0; i < 5; i++) {
            client.sendMouseMove(i * 10, i * 20);
            client.sendKeyDown(65 + i, 30 + i, 0);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        serverThread.join();

        expect(mouseMoveCnt == 5_i);
        expect(keyDownCnt == 5_i);
    };

    "typing sequence test"_test = [] {
        TestVncServer server;
        expect(server.start(15907) == true);

        std::vector<uint32_t> keysPressed;
        std::vector<uint32_t> keysReleased;

        server.onKeyDown = [&](uint32_t keycode, uint32_t, uint8_t) {
            keysPressed.push_back(keycode);
        };
        server.onKeyUp = [&](uint32_t keycode, uint32_t, uint8_t) {
            keysReleased.push_back(keycode);
        };

        std::thread serverThread([&]() {
            server.acceptClient(2000);
            server.processInput(500);
        });

        TestVncClient client;
        expect(client.connect(15907) == true);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        // Type "Hi" - H down, H up, I down, I up
        client.sendKeyDown(72, 35, MOD_SHIFT);  // H (shift+h)
        client.sendKeyUp(72, 35, MOD_SHIFT);
        client.sendKeyDown(73, 23, 0);  // i
        client.sendKeyUp(73, 23, 0);

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        serverThread.join();

        expect(keysPressed.size() == 2_i);
        expect(keysReleased.size() == 2_i);
        expect(keysPressed[0] == 72_i);  // H
        expect(keysPressed[1] == 73_i);  // I
    };

    "click sequence test"_test = [] {
        TestVncServer server;
        expect(server.start(15908) == true);

        std::vector<std::pair<bool, MouseButton>> clicks;

        server.onMouseButton = [&](int16_t, int16_t, MouseButton btn, bool pressed) {
            clicks.push_back({pressed, btn});
        };

        std::thread serverThread([&]() {
            server.acceptClient(2000);
            server.processInput(500);
        });

        TestVncClient client;
        expect(client.connect(15908) == true);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        // Click: press, release
        client.sendMouseButton(100, 100, MouseButton::LEFT, true);
        client.sendMouseButton(100, 100, MouseButton::LEFT, false);

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        serverThread.join();

        expect(clicks.size() == 2_i);
        expect(clicks[0].first == true);   // pressed
        expect(clicks[0].second == MouseButton::LEFT);
        expect(clicks[1].first == false);  // released
    };

    // Test using real VncServer non-blocking pattern (MSG_DONTWAIT)
    "non-blocking receive pattern"_test = [] {
        TestVncServer server;
        expect(server.start(15909) == true);

        int keyCount = 0;
        server.onKeyDown = [&](uint32_t, uint32_t, uint8_t) { keyCount++; };

        // Accept client first
        std::thread clientThread([&]() {
            TestVncClient client;
            expect(client.connect(15909) == true);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            // Send multiple key events
            for (int i = 0; i < 10; i++) {
                client.sendKeyDown(65 + i, 30 + i, 0);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        });

        expect(server.acceptClient(2000) == true);

        // Poll multiple times like render loop does
        for (int i = 0; i < 20; i++) {
            server.processInput(50);
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }

        clientThread.join();

        expect(keyCount == 10_i);
    };

    "rapid input stress test"_test = [] {
        TestVncServer server;
        expect(server.start(15910) == true);

        std::atomic<int> eventCount{0};
        server.onMouseMove = [&](int16_t, int16_t) { eventCount++; };
        server.onKeyDown = [&](uint32_t, uint32_t, uint8_t) { eventCount++; };

        std::thread clientThread([&]() {
            TestVncClient client;
            expect(client.connect(15910) == true);
            std::this_thread::sleep_for(std::chrono::milliseconds(50));

            // Rapid fire events
            for (int i = 0; i < 100; i++) {
                client.sendMouseMove(i, i * 2);
                client.sendKeyDown(65, 30, 0);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        });

        expect(server.acceptClient(2000) == true);

        // Process input multiple times
        for (int i = 0; i < 30; i++) {
            server.processInput(50);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        clientThread.join();

        expect(eventCount.load() == 200_i);
    };
};
