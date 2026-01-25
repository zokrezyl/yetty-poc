#pragma once

#include <yetty/result.hpp>
#include <memory>

namespace yetty {

class NewYetty {
public:
    using Ptr = std::shared_ptr<NewYetty>;

    virtual ~NewYetty() = default;

    static Result<Ptr> create(int argc, char* argv[]) noexcept;

    virtual int run() noexcept = 0;

protected:
    NewYetty() = default;
};

} // namespace yetty
