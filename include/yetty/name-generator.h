#pragma once

#include <yetty/result.hpp>
#include <yetty/base/factory.h>
#include <string>
#include <cstdint>

namespace yetty {

/**
 * NameGenerator - Thread-local singleton for generating random names.
 *
 * Generates Docker-style names: "adjective_scientist" (e.g., "happy_turing").
 * Uses word lists from Docker/Moby project (Apache 2.0 License).
 */
class NameGenerator : public base::ThreadSingleton<NameGenerator> {
public:
    using Ptr = std::shared_ptr<NameGenerator>;

    static Result<Ptr> createImpl() noexcept;

    virtual ~NameGenerator() = default;

    // Generate a random name like "happy_turing"
    virtual std::string generate() = 0;

    // Generate with optional numeric suffix for uniqueness (e.g., "happy_turing3")
    virtual std::string generate(int retry) = 0;

protected:
    NameGenerator() = default;
};

} // namespace yetty
