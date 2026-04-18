// yhtml-machine: HTTP fetch state machine for testing yhtml pipeline
//
// Fetches URLs using HttpFetcher (with persistent cookies), detects
// consent/redirect walls, auto-submits forms, and prints results.
// No rendering — tests the HTTP layer only.

#include "http-fetcher.h"
#include <args.hxx>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <regex>
#include <string>

using namespace yetty::card;

// Extract <title>...</title> from HTML
static std::string extractTitle(const std::string& html) {
    auto start = html.find("<title>");
    if (start == std::string::npos) start = html.find("<title ");
    if (start == std::string::npos) return "";
    start = html.find('>', start) + 1;
    auto end = html.find("</title>", start);
    if (end == std::string::npos) return "";
    return html.substr(start, end - start);
}

// Extract hidden input fields from an HTML form
// Returns URL-encoded form data string
static std::string extractFormFields(const std::string& formHtml) {
    std::string result;
    std::regex inputRe(R"xx(<input\s+type="hidden"\s+name="([^"]+)"\s+value="([^"]*)")xx");
    auto begin = std::sregex_iterator(formHtml.begin(), formHtml.end(), inputRe);
    auto end = std::sregex_iterator();
    for (auto it = begin; it != end; ++it) {
        if (!result.empty()) result += '&';
        result += (*it)[1].str() + '=' + (*it)[2].str();
    }
    return result;
}

// Detect Google consent page and extract the "Reject all" form
// Returns {action_url, form_data} or empty strings if not a consent page
static std::pair<std::string, std::string> detectConsentForm(const std::string& html) {
    if (html.find("Before you continue") == std::string::npos)
        return {};

    // Find the form with "Reject all" button (set_eom=true)
    auto pos = html.find("set_eom\" value=\"true\"");
    if (pos == std::string::npos) return {};

    // Walk back to find the <form> tag
    auto formStart = html.rfind("<form", pos);
    if (formStart == std::string::npos) return {};

    auto formEnd = html.find("</form>", pos);
    if (formEnd == std::string::npos) return {};

    std::string formHtml = html.substr(formStart, formEnd - formStart);

    // Extract action URL
    std::regex actionRe(R"xx(action="([^"]+)")xx");
    std::smatch match;
    std::string action;
    if (std::regex_search(formHtml, match, actionRe)) {
        action = match[1].str();
    }

    auto fields = extractFormFields(formHtml);
    return {action, fields};
}

// Strip HTML tags, collapse whitespace, return plain text
static std::string htmlToText(const std::string& html) {
    std::string text;
    text.reserve(html.size() / 4);
    bool inTag = false;
    bool lastSpace = true;
    for (char c : html) {
        if (c == '<') { inTag = true; continue; }
        if (c == '>') { inTag = false; continue; }
        if (inTag) continue;
        if (c == '\n' || c == '\r' || c == '\t') c = ' ';
        if (c == ' ' && lastSpace) continue;
        text += c;
        lastSpace = (c == ' ');
    }
    return text;
}

int main(int argc, char** argv) {
    args::ArgumentParser parser("yhtml-machine - HTTP fetch state machine");
    args::HelpFlag help(parser, "help", "Show help", {'h', "help"});
    args::Flag rawFlag(parser, "raw", "Print raw HTML instead of text", {"raw"});
    args::Flag verboseFlag(parser, "verbose", "Verbose output", {'v', "verbose"});
    args::Positional<std::string> urlArg(parser, "url", "URL to fetch");

    try {
        parser.ParseCLI(argc, argv);
    } catch (const args::Help&) {
        std::cout << parser;
        return 0;
    } catch (const args::ParseError& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    if (!urlArg) {
        std::cerr << "Error: no URL specified\n";
        return 1;
    }

    auto fetcherResult = HttpFetcher::create();
    if (!fetcherResult) {
        std::cerr << "Error: failed to create HTTP fetcher\n";
        return 1;
    }
    auto fetcher = std::move(*fetcherResult);

    std::string url = args::get(urlArg);
    fetcher->setBaseUrl(url);

    // Step 1: fetch the URL
    auto body = fetcher->fetch(url);
    if (!body) {
        std::cerr << "Error: failed to fetch: " << url << "\n";
        return 1;
    }

    std::string title = extractTitle(*body);
    if (verboseFlag) {
        std::cerr << "Step 1: " << body->size() << " bytes, title: " << title << "\n";
    }

    // Step 2: detect consent form and auto-submit
    auto [action, formData] = detectConsentForm(*body);
    if (!action.empty()) {
        if (verboseFlag) {
            std::cerr << "Consent detected, POSTing to: " << action << "\n";
        }

        auto postResult = fetcher->post(action, formData);
        if (postResult) {
            // POST may return the final page directly (via redirect)
            body = std::move(postResult);
            title = extractTitle(*body);
            if (verboseFlag) {
                std::cerr << "Step 2 (post): " << body->size() << " bytes, title: " << title << "\n";
            }
        } else {
            // POST failed or returned redirect — re-fetch original URL
            if (verboseFlag) {
                std::cerr << "Step 2 (post): failed, re-fetching original URL\n";
            }
            body = fetcher->fetch(url);
            if (!body) {
                std::cerr << "Error: failed to re-fetch: " << url << "\n";
                return 1;
            }
            title = extractTitle(*body);
            if (verboseFlag) {
                std::cerr << "Step 2 (refetch): " << body->size() << " bytes, title: " << title << "\n";
            }
        }
    }

    // Step 3: output
    std::cout << "URL: " << url << "\n";
    std::cout << "Title: " << title << "\n";
    std::cout << "Size: " << body->size() << " bytes\n";

    // Show cookie file
    const char* home = std::getenv("HOME");
    if (home) {
        auto cookiePath = std::filesystem::path(home) / ".cache" / "yetty" / "yhtml" / "cookies.txt";
        if (std::filesystem::exists(cookiePath)) {
            auto sz = std::filesystem::file_size(cookiePath);
            std::cout << "Cookies: " << cookiePath.string() << " (" << sz << " bytes)\n";
        }
    }

    std::cout << "---\n";
    if (rawFlag) {
        std::cout << *body << "\n";
    } else {
        std::string text = htmlToText(*body);
        if (text.size() > 4000) text.resize(4000);
        std::cout << text << "\n";
    }

    return 0;
}
