#include "http-fetcher.h"
#include <cpr/cpr.h>
#include <curl/curl.h>
#include <ytrace/ytrace.hpp>
#include <cstdlib>
#include <filesystem>

namespace yetty::card {

//=============================================================================
// HttpFetcherImpl
//=============================================================================

class HttpFetcherImpl : public HttpFetcher {
public:
    HttpFetcherImpl() {
        _session.SetUserAgent(cpr::UserAgent{"yetty/1.0"});
        _session.SetTimeout(cpr::Timeout{10000});
        _session.SetRedirect(cpr::Redirect{10L});

        // Enable persistent cookie storage in XDG cache directory
        _cookiePath = cookiePath();
        if (!_cookiePath.empty()) {
            auto holder = _session.GetCurlHolder();
            curl_easy_setopt(holder->handle, CURLOPT_COOKIEFILE, _cookiePath.c_str());
            curl_easy_setopt(holder->handle, CURLOPT_COOKIEJAR, _cookiePath.c_str());
        }
    }
    ~HttpFetcherImpl() override = default;

    void setBaseUrl(const std::string& baseUrl) override {
        _baseUrl = baseUrl;
        // Strip to directory: "https://x.com/path/page.html" → "https://x.com/path/"
        auto pos = _baseUrl.rfind('/');
        if (pos != std::string::npos && pos > 8) {
            _baseUrl = _baseUrl.substr(0, pos + 1);
        } else if (!_baseUrl.empty() && _baseUrl.back() != '/') {
            _baseUrl += '/';
        }
    }

    const std::string& baseUrl() const override { return _baseUrl; }

    std::string resolveUrl(const std::string& url) const override {
        if (isUrl(url)) return url;
        if (url.empty()) return _baseUrl;
        if (url[0] == '/' && !_baseUrl.empty()) {
            // Absolute path — combine with origin
            auto schemeEnd = _baseUrl.find("://");
            if (schemeEnd != std::string::npos) {
                auto hostEnd = _baseUrl.find('/', schemeEnd + 3);
                if (hostEnd != std::string::npos) {
                    return _baseUrl.substr(0, hostEnd) + url;
                }
            }
        }
        // Relative path
        return _baseUrl + url;
    }

    std::optional<std::string> fetch(const std::string& url) const override {
        std::string resolved = resolveUrl(url);
        if (resolved.empty()) return std::nullopt;

        yinfo("HttpFetcher::fetch: {}", resolved);

        _session.SetUrl(cpr::Url{resolved});
        cpr::Response r = _session.Get();

        if (r.status_code == 0) {
            yerror("HttpFetcher::fetch: connection failed: {}", r.error.message);
            return std::nullopt;
        }
        if (r.status_code >= 400) {
            ywarn("HttpFetcher::fetch: HTTP {}: {}", r.status_code, resolved);
            return std::nullopt;
        }

        yinfo("HttpFetcher::fetch: OK {} ({} bytes)", r.status_code, r.text.size());
        return r.text;
    }

    std::optional<std::string> post(const std::string& url,
                                    const std::string& formData) const override {
        std::string resolved = resolveUrl(url);
        if (resolved.empty()) return std::nullopt;

        yinfo("HttpFetcher::post: {}", resolved);

        _session.SetUrl(cpr::Url{resolved});
        _session.SetBody(cpr::Body{formData});
        _session.SetHeader(cpr::Header{{"Content-Type", "application/x-www-form-urlencoded"}});
        cpr::Response r = _session.Post();

        // Clear body for subsequent GET requests
        _session.SetBody(cpr::Body{});
        _session.SetHeader(cpr::Header{});

        if (r.status_code == 0) {
            yerror("HttpFetcher::post: connection failed: {}", r.error.message);
            return std::nullopt;
        }
        if (r.status_code >= 400) {
            ywarn("HttpFetcher::post: HTTP {}: {}", r.status_code, resolved);
            return std::nullopt;
        }

        yinfo("HttpFetcher::post: OK {} ({} bytes)", r.status_code, r.text.size());
        return r.text;
    }

private:
    std::string _baseUrl;
    std::string _cookiePath;
    mutable cpr::Session _session;

    static std::string cookiePath() {
        const char* home = std::getenv("HOME");
        if (!home) return {};

        std::filesystem::path dir = std::filesystem::path(home) / ".cache" / "yetty" / "yhtml";
        std::error_code ec;
        std::filesystem::create_directories(dir, ec);
        if (ec) return {};

        return (dir / "cookies.txt").string();
    }
};

//=============================================================================
// Static helpers
//=============================================================================

bool HttpFetcher::isUrl(const std::string& str) {
    return str.size() > 8 &&
           (str.substr(0, 7) == "http://" || str.substr(0, 8) == "https://");
}

//=============================================================================
// Factory
//=============================================================================

Result<HttpFetcher::Ptr> HttpFetcher::createImpl() {
    return Ok(Ptr(new HttpFetcherImpl()));
}

} // namespace yetty::card
