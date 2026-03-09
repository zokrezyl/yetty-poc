// Simple HTTP fetcher using libcurl
#pragma once

#include <string>
#include <optional>
#include <curl/curl.h>

namespace ydraw {

class HttpFetcher {
public:
    HttpFetcher() {
        curl_global_init(CURL_GLOBAL_DEFAULT);
    }

    ~HttpFetcher() {
        curl_global_cleanup();
    }

    void setBaseUrl(const std::string& url) {
        _baseUrl = url;
        // Extract base path
        size_t pos = url.rfind('/');
        if (pos != std::string::npos && pos > 8) { // after "https://"
            _basePath = url.substr(0, pos);
        } else {
            _basePath = url;
        }
    }

    const std::string& baseUrl() const { return _baseUrl; }

    std::string resolveUrl(const std::string& url) const {
        if (url.find("://") != std::string::npos) {
            return url; // Already absolute
        }
        if (url.empty()) return _baseUrl;
        if (url[0] == '/') {
            // Root-relative
            size_t pos = _baseUrl.find("://");
            if (pos != std::string::npos) {
                size_t end = _baseUrl.find('/', pos + 3);
                if (end != std::string::npos) {
                    return _baseUrl.substr(0, end) + url;
                }
            }
            return _baseUrl + url;
        }
        // Relative to current path
        return _basePath + "/" + url;
    }

    std::optional<std::string> fetch(const std::string& url) const {
        CURL* curl = curl_easy_init();
        if (!curl) return std::nullopt;

        std::string resolved = resolveUrl(url);
        std::string response;

        curl_easy_setopt(curl, CURLOPT_URL, resolved.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "YDraw-HTML/1.0");

        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            return std::nullopt;
        }

        return response;
    }

private:
    static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
        size_t totalSize = size * nmemb;
        auto* str = static_cast<std::string*>(userp);
        str->append(static_cast<char*>(contents), totalSize);
        return totalSize;
    }

    std::string _baseUrl;
    std::string _basePath;
};

} // namespace ydraw
