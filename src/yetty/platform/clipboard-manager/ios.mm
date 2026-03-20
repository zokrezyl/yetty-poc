#include <yetty/platform/clipboard-manager.h>

#if YETTY_IOS

#include <ytrace/ytrace.hpp>

#import <UIKit/UIKit.h>

namespace yetty {

class IosClipboardManager : public ClipboardManager {
public:
    std::string getText() const override {
        @autoreleasepool {
            UIPasteboard* pb = [UIPasteboard generalPasteboard];
            if (pb.string) {
                return std::string([pb.string UTF8String]);
            }
        }
        return "";
    }

    void setText(const std::string& text) override {
        @autoreleasepool {
            [UIPasteboard generalPasteboard].string = [NSString stringWithUTF8String:text.c_str()];
        }
    }
};

// Factory
Result<ClipboardManager::Ptr> ClipboardManager::createImpl() {
    return Ok(Ptr(new IosClipboardManager()));
}

} // namespace yetty

#endif // YETTY_IOS
