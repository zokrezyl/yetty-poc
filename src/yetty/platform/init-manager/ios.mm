#include <yetty/platform/init-manager.h>

#if YETTY_IOS

#include <yetty/yetty.h>
#include <yetty/platform/event-loop.h>
#include <yetty/base/event-queue.h>
#include <yetty/base/event.h>
#include "../shared/ios-app-singleton.h"
#include <ytrace/ytrace.hpp>
#include <uv.h>
#include <thread>
#include <atomic>

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <QuartzCore/CAMetalLayer.h>
#import <Metal/Metal.h>

namespace yetty {
class IosInitManager;
}

// Global state
static yetty::IosInitManager* g_initManager = nullptr;
static yetty::Yetty::Ptr g_yetty = nullptr;

// =============================================================================
// YettyMetalView - UIView backed by CAMetalLayer
// =============================================================================
@interface YettyMetalView : UIView
@property (nonatomic, readonly) CAMetalLayer* metalLayer;
@end

@implementation YettyMetalView

+ (Class)layerClass {
    return [CAMetalLayer class];
}

- (CAMetalLayer*)metalLayer {
    return (CAMetalLayer*)self.layer;
}

- (instancetype)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        self.metalLayer.device = MTLCreateSystemDefaultDevice();
        self.metalLayer.pixelFormat = MTLPixelFormatBGRA8Unorm;
        self.metalLayer.framebufferOnly = YES;
        self.metalLayer.contentsScale = [UIScreen mainScreen].scale;
        self.backgroundColor = [UIColor blackColor];
    }
    return self;
}

- (void)layoutSubviews {
    [super layoutSubviews];
    CGSize drawableSize = self.bounds.size;
    drawableSize.width *= self.metalLayer.contentsScale;
    drawableSize.height *= self.metalLayer.contentsScale;
    self.metalLayer.drawableSize = drawableSize;

    // Update singleton
    if (auto singleton = yetty::IosAppSingleton::instance(); singleton) {
        (*singleton)->setDrawableSize(static_cast<int>(drawableSize.width),
                                      static_cast<int>(drawableSize.height));
    }
}

@end

// =============================================================================
// YettyViewController
// =============================================================================
@interface YettyViewController : UIViewController
@property (nonatomic, strong) YettyMetalView* metalView;
@property (nonatomic, strong) CADisplayLink* displayLink;
@end

@implementation YettyViewController

- (void)viewDidLoad {
    [super viewDidLoad];

    self.metalView = [[YettyMetalView alloc] initWithFrame:self.view.bounds];
    self.metalView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
    [self.view addSubview:self.metalView];

    // Update singleton with initial state
    if (auto singleton = yetty::IosAppSingleton::instance(); singleton) {
        (*singleton)->setMetalLayer(self.metalView.metalLayer);
        (*singleton)->setContentScale(self.metalView.metalLayer.contentsScale);

        CGSize size = self.metalView.metalLayer.drawableSize;
        (*singleton)->setDrawableSize(static_cast<int>(size.width),
                                      static_cast<int>(size.height));
    }
}

- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
    [self startRenderLoop];
}

- (void)startRenderLoop {
    if (self.displayLink) return;

    self.displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(renderFrame)];
    [self.displayLink addToRunLoop:[NSRunLoop mainRunLoop] forMode:NSDefaultRunLoopMode];
    yinfo("iOS: Render loop started");
}

- (void)stopRenderLoop {
    [self.displayLink invalidate];
    self.displayLink = nil;
}

- (void)renderFrame {
    // Process libuv events for async I/O (telnet, etc.)
    uv_run(uv_default_loop(), UV_RUN_NOWAIT);

    // Dispatch frame event to trigger rendering
    if (auto loopResult = yetty::base::EventLoop::instance(); loopResult) {
        (*loopResult)->dispatch(yetty::base::Event::screenUpdateEvent());
    }
}

- (void)viewDidLayoutSubviews {
    [super viewDidLayoutSubviews];

    // Dispatch resize event
    if (auto queueResult = yetty::base::EventQueue::instance(); queueResult) {
        CGSize size = self.metalView.metalLayer.drawableSize;
        (*queueResult)->push(yetty::base::Event::resizeEvent(
            static_cast<int>(size.width), static_cast<int>(size.height)));
    }
}

- (BOOL)prefersStatusBarHidden {
    return YES;
}

- (BOOL)prefersHomeIndicatorAutoHidden {
    return YES;
}

- (void)dealloc {
    [self stopRenderLoop];
}

@end

// =============================================================================
// YettyAppDelegate
// =============================================================================
@interface YettyAppDelegate : UIResponder <UIApplicationDelegate>
@property (nonatomic, strong) UIWindow* window;
@property (nonatomic, strong) YettyViewController* viewController;
@end

@implementation YettyAppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    (void)application;
    (void)launchOptions;

    yinfo("iOS: App launching...");

    self.window = [[UIWindow alloc] initWithFrame:[UIScreen mainScreen].bounds];
    self.viewController = [[YettyViewController alloc] init];
    self.window.rootViewController = self.viewController;
    [self.window makeKeyAndVisible];

    yinfo("iOS: Window created, size: {:.0f}x{:.0f}, scale: {:.1f}",
          self.window.bounds.size.width, self.window.bounds.size.height,
          [UIScreen mainScreen].scale);

    // Initialize Yetty on next run loop iteration (after Metal view is ready)
    dispatch_async(dispatch_get_main_queue(), ^{
        [self initializeYetty];
    });

    return YES;
}

- (void)initializeYetty {
    yinfo("iOS: Initializing Yetty...");

    int fakeArgc = 1;
    const char* fakeArgv[] = {"yetty", nullptr};

    auto result = yetty::Yetty::create(fakeArgc, const_cast<char**>(fakeArgv));
    if (!result) {
        yerror("iOS: Failed to initialize Yetty: {}", result.error().message());

        UIAlertController* alert = [UIAlertController
            alertControllerWithTitle:@"Initialization Failed"
            message:[NSString stringWithFormat:@"%s", result.error().message().c_str()]
            preferredStyle:UIAlertControllerStyleAlert];

        [alert addAction:[UIAlertAction actionWithTitle:@"OK"
            style:UIAlertActionStyleDefault handler:nil]];

        [self.viewController presentViewController:alert animated:YES completion:nil];
        return;
    }

    g_yetty = *result;
    yinfo("iOS: Yetty initialized successfully");
}

- (void)applicationWillTerminate:(UIApplication *)application {
    (void)application;
    yinfo("iOS: Terminating...");

    if (g_yetty) {
        (void)g_yetty->shutdown();
        g_yetty.reset();
    }
}

@end

// =============================================================================
// C++ InitManager Implementation
// =============================================================================
namespace yetty {

class IosInitManager : public InitManager {
public:
    IosInitManager() {
        g_initManager = this;
    }

    ~IosInitManager() override {
        if (g_initManager == this) {
            g_initManager = nullptr;
        }
    }

    Result<void> init() {
        // Initialize IosAppSingleton
        auto singletonResult = IosAppSingleton::instance();
        if (!singletonResult) {
            return Err<void>("Failed to create IosAppSingleton", singletonResult);
        }

        ydebug("IosInitManager: initialized");
        return Ok();
    }

    void run(int argc, char** argv) override {
        (void)argc; (void)argv;  // iOS ignores argc/argv - UIApplicationMain handles everything

        ydebug("IosInitManager::run - starting UIApplicationMain");

        // This never returns
        @autoreleasepool {
            // Enable ytrace logging
            setenv("YTRACE_DEFAULT_ON", "yes", 1);

            int fakeArgc = 1;
            char* fakeArgv[] = {(char*)"yetty", nullptr};
            UIApplicationMain(fakeArgc, fakeArgv, nil, NSStringFromClass([YettyAppDelegate class]));
        }
    }
};

// Factory
Result<InitManager::Ptr> InitManager::createImpl() {
    auto mgr = new IosInitManager();
    if (auto res = mgr->init(); !res) {
        delete mgr;
        return Err<Ptr>("IosInitManager init failed", res);
    }
    return Ok(Ptr(mgr));
}

} // namespace yetty

// =============================================================================
// iOS main entry point
// =============================================================================
extern "C" int main(int argc, char* argv[]) {
    auto initResult = yetty::InitManager::create();
    if (!initResult) {
        NSLog(@"Yetty: Failed to create InitManager");
        return 1;
    }

    (*initResult)->run(argc, argv);
    return 0;
}

#endif // YETTY_IOS
