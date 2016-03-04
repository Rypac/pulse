#import "AppController.h"
#import "platform/ios/CCEAGLView-ios.h"
#import "cocos2d.h"
#import "AppDelegate.h"
#import "RootViewController.h"

@implementation AppController

static AppDelegate s_sharedApplication;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    auto app = cocos2d::Application::getInstance();
    app->initGLContextAttrs();
    cocos2d::GLViewImpl::convertAttrs();

    window = [[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]];

    auto eaglView = [CCEAGLView viewWithFrame:[window bounds]
                                  pixelFormat:(NSString*)cocos2d::GLViewImpl::_pixelFormat
                                  depthFormat:cocos2d::GLViewImpl::_depthFormat
                           preserveBackbuffer:NO
                                   sharegroup:nil
                                multiSampling:NO
                              numberOfSamples:0];
    [eaglView setMultipleTouchEnabled:NO];

    _viewController = [[RootViewController alloc] initWithNibName:nil bundle:nil];
    _viewController.wantsFullScreenLayout = YES;
    _viewController.view = eaglView;

    [window setRootViewController:_viewController];
    [window makeKeyAndVisible];

    [[UIApplication sharedApplication] setStatusBarHidden:true];

    auto glview = cocos2d::GLViewImpl::createWithEAGLView(eaglView);
    cocos2d::Director::getInstance()->setOpenGLView(glview);

    app->run();

    return YES;
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    cocos2d::Application::getInstance()->applicationDidEnterBackground();
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    cocos2d::Application::getInstance()->applicationWillEnterForeground();
}

@end
