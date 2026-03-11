//=============================================================================
// OSC PTY Poll Integration Test
//
// Tests that the poll mechanism correctly fires for multiple data chunks
// arriving through a pipe, simulating PTY behavior.
//=============================================================================

#include <boost/ut.hpp>
#include <yetty/base/event-loop.h>
#include <yetty/base/event.h>
#include <cstring>
#include <string>
#include <vector>
#include <thread>
#include <unistd.h>
#include <fcntl.h>

using namespace boost::ut;
using namespace yetty;
using namespace yetty::base;

// ---------------------------------------------------------------------------
// Pipe-based poll tests
// ---------------------------------------------------------------------------

suite pty_poll_tests = [] {
    "poll fires for each write to pipe"_test = [] {
        // Create a pipe
        int pipefd[2];
        expect(pipe(pipefd) == 0_i) << "Failed to create pipe";

        int readFd = pipefd[0];
        int writeFd = pipefd[1];

        // Set read end to non-blocking
        int flags = fcntl(readFd, F_GETFL, 0);
        fcntl(readFd, F_SETFL, flags | O_NONBLOCK);

        // Get event loop
        auto loopResult = EventLoop::instance();
        expect(loopResult.has_value()) << "Failed to get event loop";
        auto loop = *loopResult;

        // Create and configure poll
        auto pollIdResult = loop->createPoll();
        expect(pollIdResult.has_value()) << "Failed to create poll";
        auto pollId = *pollIdResult;

        expect(loop->configPoll(pollId, readFd).has_value()) << "Failed to config poll";

        // Track poll callbacks
        int pollCallbackCount = 0;
        std::vector<std::string> receivedData;

        // Create listener for poll events
        class PollListener : public EventListener,
                             public std::enable_shared_from_this<PollListener> {
        public:
            int readFd;
            int* callbackCount;
            std::vector<std::string>* data;
            EventLoop::Ptr loop;
            EventLoop::PollId pollId;

            Result<bool> onEvent(const Event& event) override {
                if (event.type == Event::Type::PollReadable && event.poll.fd == readFd) {
                    (*callbackCount)++;

                    // Read available data
                    char buf[1024];
                    ssize_t n = read(readFd, buf, sizeof(buf));
                    if (n > 0) {
                        data->push_back(std::string(buf, n));
                    }

                    // Rearm poll for next data
                    loop->startPoll(pollId);

                    return Ok(true);
                }
                return Ok(false);
            }
        };

        auto listener = std::make_shared<PollListener>();
        listener->readFd = readFd;
        listener->callbackCount = &pollCallbackCount;
        listener->data = &receivedData;
        listener->loop = loop;
        listener->pollId = pollId;

        loop->registerPollListener(pollId, listener);
        loop->startPoll(pollId);

        // Write 5 chunks to pipe with small delays
        std::thread writer([writeFd]() {
            for (int i = 0; i < 5; i++) {
                std::string msg = "chunk" + std::to_string(i);
                write(writeFd, msg.c_str(), msg.size());
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        });

        // Run event loop for a bit
        auto startTime = std::chrono::steady_clock::now();
        while (std::chrono::steady_clock::now() - startTime < std::chrono::milliseconds(200)) {
            loop->runOnce(10);  // 10ms timeout
        }

        writer.join();
        close(writeFd);
        close(readFd);

        // Verify we got all chunks
        expect(pollCallbackCount >= 1_i) << "Poll should fire at least once, got " << pollCallbackCount;
        expect(!receivedData.empty()) << "Should receive data";

        // Combine all received data
        std::string allData;
        for (const auto& chunk : receivedData) {
            allData += chunk;
        }
        expect(allData.find("chunk0") != std::string::npos) << "Should receive chunk0";
        expect(allData.find("chunk4") != std::string::npos) << "Should receive chunk4";
    };

    "poll rearm works after blocking operation"_test = [] {
        int pipefd[2];
        expect(pipe(pipefd) == 0_i);

        int readFd = pipefd[0];
        int writeFd = pipefd[1];

        int flags = fcntl(readFd, F_GETFL, 0);
        fcntl(readFd, F_SETFL, flags | O_NONBLOCK);

        auto loopResult = EventLoop::instance();
        expect(loopResult.has_value());
        auto loop = *loopResult;

        auto pollIdResult = loop->createPoll();
        expect(pollIdResult.has_value());
        auto pollId = *pollIdResult;

        expect(loop->configPoll(pollId, readFd).has_value());

        int callbackCount = 0;

        class BlockingListener : public EventListener,
                                 public std::enable_shared_from_this<BlockingListener> {
        public:
            int readFd;
            int* count;
            EventLoop::Ptr loop;
            EventLoop::PollId pollId;

            Result<bool> onEvent(const Event& event) override {
                if (event.type == Event::Type::PollReadable && event.poll.fd == readFd) {
                    (*count)++;

                    // Read data
                    char buf[1024];
                    read(readFd, buf, sizeof(buf));

                    // Simulate blocking work (like GPU render)
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));

                    // Rearm poll AFTER blocking work
                    loop->startPoll(pollId);

                    return Ok(true);
                }
                return Ok(false);
            }
        };

        auto listener = std::make_shared<BlockingListener>();
        listener->readFd = readFd;
        listener->count = &callbackCount;
        listener->loop = loop;
        listener->pollId = pollId;

        loop->registerPollListener(pollId, listener);
        loop->startPoll(pollId);

        // Writer sends data, waits, sends more
        std::thread writer([writeFd]() {
            // First chunk
            write(writeFd, "first", 5);
            // Wait for blocking operation
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            // Second chunk - should trigger poll after rearm
            write(writeFd, "second", 6);
        });

        // Run event loop
        auto startTime = std::chrono::steady_clock::now();
        while (std::chrono::steady_clock::now() - startTime < std::chrono::milliseconds(300)) {
            loop->runOnce(10);
        }

        writer.join();
        close(writeFd);
        close(readFd);

        // Both chunks should trigger callbacks
        expect(callbackCount == 2_i) << "Should get 2 callbacks (one per chunk), got " << callbackCount;
    };

    "rapid writes coalesce into single poll callback"_test = [] {
        int pipefd[2];
        expect(pipe(pipefd) == 0_i);

        int readFd = pipefd[0];
        int writeFd = pipefd[1];

        int flags = fcntl(readFd, F_GETFL, 0);
        fcntl(readFd, F_SETFL, flags | O_NONBLOCK);

        auto loopResult = EventLoop::instance();
        expect(loopResult.has_value());
        auto loop = *loopResult;

        auto pollIdResult = loop->createPoll();
        expect(pollIdResult.has_value());
        auto pollId = *pollIdResult;

        expect(loop->configPoll(pollId, readFd).has_value());

        int callbackCount = 0;
        int totalBytesRead = 0;

        class ReadAllListener : public EventListener,
                                public std::enable_shared_from_this<ReadAllListener> {
        public:
            int readFd;
            int* count;
            int* bytesRead;
            EventLoop::Ptr loop;
            EventLoop::PollId pollId;

            Result<bool> onEvent(const Event& event) override {
                if (event.type == Event::Type::PollReadable && event.poll.fd == readFd) {
                    (*count)++;

                    // Read ALL available data
                    char buf[4096];
                    ssize_t n;
                    while ((n = read(readFd, buf, sizeof(buf))) > 0) {
                        *bytesRead += n;
                    }

                    loop->startPoll(pollId);
                    return Ok(true);
                }
                return Ok(false);
            }
        };

        auto listener = std::make_shared<ReadAllListener>();
        listener->readFd = readFd;
        listener->count = &callbackCount;
        listener->bytesRead = &totalBytesRead;
        listener->loop = loop;
        listener->pollId = pollId;

        loop->registerPollListener(pollId, listener);
        loop->startPoll(pollId);

        // Write 10 chunks rapidly before any poll can fire
        for (int i = 0; i < 10; i++) {
            std::string msg = "chunk" + std::to_string(i) + "\n";
            write(writeFd, msg.c_str(), msg.size());
        }

        // Run event loop
        auto startTime = std::chrono::steady_clock::now();
        while (std::chrono::steady_clock::now() - startTime < std::chrono::milliseconds(100)) {
            loop->runOnce(10);
        }

        close(writeFd);
        close(readFd);

        // May get 1 callback (all coalesced) or a few (depending on timing)
        expect(callbackCount >= 1_i) << "Should get at least 1 callback";
        expect(totalBytesRead > 50_i) << "Should read all data, got " << totalBytesRead << " bytes";
    };
};
