#import "AppController.h"
#import "cocos2d.h"
#import "pulse/AppDelegate.hpp"
#import "RootViewController.h"

@implementation AppController

static AppDelegate s_sharedApplication;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    [UIApplication sharedApplication].statusBarHidden = YES;

    _viewController = [RootViewController new];
    window = [[UIWindow alloc] initWithFrame:[UIScreen mainScreen].bounds];
    window.rootViewController = _viewController;
    [window makeKeyAndVisible];

    return YES;
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    cocos2d::Application::getInstance()->applicationDidEnterBackground();
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    cocos2d::Application::getInstance()->applicationWillEnterForeground();
}

@end
