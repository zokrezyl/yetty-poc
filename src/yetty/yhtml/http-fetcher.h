#pragma once

#include <yetty/base/object.h>
#include <yetty/base/factory.h>
#include <string>
#include <optional>

namespace yetty::card {

//=============================================================================
// HttpFetcher - HTTP client using cpr (libcurl wrapper)
//
// Fetches URLs and returns body as string. Used by YHtml to download
// pages, CSS, and images referenced in HTML.
//=============================================================================
class HttpFetcher : public base::Object,
                    public base::ObjectFactory<HttpFetcher> {
public:
    using Ptr = std::shared_ptr<HttpFetcher>;

    static Result<Ptr> createImpl();

    ~HttpFetcher() override = default;

    // Set base URL for resolving relative URLs
    virtual void setBaseUrl(const std::string& baseUrl) = 0;
    virtual const std::string& baseUrl() const = 0;

    // Resolve a possibly-relative URL against the base
    virtual std::string resolveUrl(const std::string& url) const = 0;

    // Fetch a URL, returns body or nullopt on failure
    virtual std::optional<std::string> fetch(const std::string& url) const = 0;

    // Returns true if the string looks like a URL (http:// or https://)
    static bool isUrl(const std::string& str);

    const char* typeName() const override { return "HttpFetcher"; }

protected:
    HttpFetcher() = default;
};

} // namespace yetty::card
