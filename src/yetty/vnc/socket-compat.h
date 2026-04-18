#pragma once

// Cross-platform socket compatibility layer
// Provides unified API for POSIX sockets (Linux/macOS) and Winsock2 (Windows)

#ifdef _WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

// Map POSIX names to Winsock equivalents
#ifndef MSG_NOSIGNAL
#define MSG_NOSIGNAL 0
#endif
#ifndef MSG_DONTWAIT
#define MSG_DONTWAIT 0
#endif
#ifndef SHUT_RDWR
#define SHUT_RDWR SD_BOTH
#endif

using ssize_t = intptr_t;

namespace yetty::vnc::sock {

inline bool init() {
    WSADATA wsaData;
    return WSAStartup(MAKEWORD(2, 2), &wsaData) == 0;
}

inline void cleanup() {
    WSACleanup();
}

inline int close(int fd) {
    return ::closesocket(static_cast<SOCKET>(fd));
}

inline bool set_nonblocking(int fd) {
    u_long mode = 1;
    return ioctlsocket(static_cast<SOCKET>(fd), FIONBIO, &mode) == 0;
}

inline bool would_block() {
    int err = WSAGetLastError();
    return err == WSAEWOULDBLOCK || err == WSAEINPROGRESS;
}

inline bool connect_in_progress() {
    int err = WSAGetLastError();
    return err == WSAEWOULDBLOCK;
}

inline int last_error() {
    return WSAGetLastError();
}

inline std::string error_string(int err) {
    char buf[256];
    FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, err, 0, buf, sizeof(buf), nullptr);
    return std::string(buf);
}

inline std::string last_error_string() {
    return error_string(last_error());
}

// Windows setsockopt takes const char* instead of const void*
template<typename T>
inline int setsockopt(int fd, int level, int optname, const T& optval) {
    return ::setsockopt(static_cast<SOCKET>(fd), level, optname,
                        reinterpret_cast<const char*>(&optval), sizeof(T));
}

// Windows getsockopt takes char* instead of void*
template<typename T>
inline int getsockopt(int fd, int level, int optname, T& optval) {
    int len = sizeof(T);
    return ::getsockopt(static_cast<SOCKET>(fd), level, optname,
                        reinterpret_cast<char*>(&optval), &len);
}

// Windows recv/send take char* buffers
inline ssize_t recv(int fd, void* buf, size_t len, int flags) {
    // Strip MSG_DONTWAIT - on Windows sockets are already non-blocking
    flags &= ~MSG_DONTWAIT;
    return ::recv(static_cast<SOCKET>(fd), static_cast<char*>(buf), static_cast<int>(len), flags);
}

inline ssize_t send(int fd, const void* buf, size_t len, int flags) {
    // Strip MSG_NOSIGNAL and MSG_DONTWAIT - not needed on Windows
    flags &= ~(MSG_NOSIGNAL | MSG_DONTWAIT);
    return ::send(static_cast<SOCKET>(fd), static_cast<const char*>(buf), static_cast<int>(len), flags);
}

} // namespace yetty::vnc::sock

#else // POSIX

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <cerrno>

namespace yetty::vnc::sock {

inline bool init() { return true; }
inline void cleanup() {}

inline int close(int fd) {
    return ::close(fd);
}

inline bool set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK) >= 0;
}

inline bool would_block() {
    return errno == EAGAIN || errno == EWOULDBLOCK;
}

inline bool connect_in_progress() {
    return errno == EINPROGRESS;
}

inline int last_error() {
    return errno;
}

inline std::string error_string(int err) {
    return strerror(err);
}

inline std::string last_error_string() {
    return strerror(errno);
}

template<typename T>
inline int setsockopt(int fd, int level, int optname, const T& optval) {
    return ::setsockopt(fd, level, optname, &optval, sizeof(T));
}

template<typename T>
inline int getsockopt(int fd, int level, int optname, T& optval) {
    socklen_t len = sizeof(T);
    return ::getsockopt(fd, level, optname, &optval, &len);
}

inline ssize_t recv(int fd, void* buf, size_t len, int flags) {
    return ::recv(fd, buf, len, flags);
}

inline ssize_t send(int fd, const void* buf, size_t len, int flags) {
    return ::send(fd, buf, len, flags);
}

} // namespace yetty::vnc::sock

#endif // _WIN32
