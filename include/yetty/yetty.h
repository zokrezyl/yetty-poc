#pragma once

#include <yetty/result.hpp>
#include <memory>

namespace yetty {

class Yetty {
public:
    using Ptr = std::shared_ptr<Yetty>;

    virtual ~Yetty() = default;

    static Result<Ptr> create(int argc, char* argv[]) noexcept;

    virtual int run() noexcept = 0;

protected:
    Yetty() = default;
};

} // namespace yetty
