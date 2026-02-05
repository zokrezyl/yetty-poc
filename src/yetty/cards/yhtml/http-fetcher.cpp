#include "http-fetcher.h"
#include <cpr/cpr.h>
#include <ytrace/ytrace.hpp>

namespace yetty::card {

//=============================================================================
// HttpFetcherImpl
//=============================================================================

class HttpFetcherImpl : public HttpFetcher {
public:
    HttpFetcherImpl() = default;
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

        cpr::Response r = cpr::Get(
            cpr::Url{resolved},
            cpr::Header{{"User-Agent", "yetty/1.0"}},
            cpr::Timeout{10000},
            cpr::Redirect{10L}
        );

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

private:
    std::string _baseUrl;
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
