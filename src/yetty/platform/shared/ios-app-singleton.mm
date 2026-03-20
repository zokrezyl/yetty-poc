#include "ios-app-singleton.h"

#if YETTY_IOS

#import <Foundation/Foundation.h>

namespace yetty {

void IosAppSingleton::setMetalLayer(CAMetalLayer* layer) {
    _metalLayer.store(layer, std::memory_order_release);
}

void IosAppSingleton::setContentScale(float scale) {
    _contentScale.store(scale, std::memory_order_release);
}

void IosAppSingleton::setDrawableSize(int width, int height) {
    _width.store(width, std::memory_order_release);
    _height.store(height, std::memory_order_release);
}

CAMetalLayer* IosAppSingleton::getMetalLayer() const {
    return _metalLayer.load(std::memory_order_acquire);
}

float IosAppSingleton::getContentScale() const {
    return _contentScale.load(std::memory_order_acquire);
}

void IosAppSingleton::getDrawableSize(int& width, int& height) const {
    width = _width.load(std::memory_order_acquire);
    height = _height.load(std::memory_order_acquire);
}

std::string IosAppSingleton::getCachesDir() const {
    @autoreleasepool {
        NSArray* paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
        if (paths.count > 0) {
            NSString* cachesDir = [paths objectAtIndex:0];
            return std::string([cachesDir UTF8String]) + "/yetty";
        }
    }
    return "";
}

std::string IosAppSingleton::getBundleResourcePath() const {
    @autoreleasepool {
        NSString* resourcePath = [[NSBundle mainBundle] resourcePath];
        if (resourcePath) {
            return std::string([resourcePath UTF8String]);
        }
    }
    return "";
}

Result<IosAppSingleton::Ptr> IosAppSingleton::createImpl() {
    return Ok(Ptr(new IosAppSingleton()));
}

} // namespace yetty

#endif // YETTY_IOS
