#import "RootViewController.h"
#import "cocos2d.h"
#import "platform/ios/CCEAGLView-ios.h"

@implementation RootViewController

- (void)loadView {
    cocos2d::Application::getInstance()->initGLContextAttrs();
    cocos2d::GLViewImpl::convertAttrs();

    self.view = [CCEAGLView viewWithFrame:[self currentScreenBoundsDependOnOrientation]
                              pixelFormat:(NSString *)cocos2d::GLViewImpl::_pixelFormat
                              depthFormat:cocos2d::GLViewImpl::_depthFormat
                       preserveBackbuffer:NO
                               sharegroup:nil
                            multiSampling:NO
                          numberOfSamples:0];
    self.view.multipleTouchEnabled = NO;
}

- (void)viewDidLoad {
    [super viewDidLoad];

    cocos2d::GLView *view = cocos2d::GLViewImpl::createWithEAGLView((__bridge void *)self.view);
    cocos2d::Director::getInstance()->setOpenGLView(view);
    cocos2d::Application::getInstance()->run();
}

- (NSUInteger)supportedInterfaceOrientations {
    return UIInterfaceOrientationMaskLandscape;
}

- (BOOL)prefersStatusBarHidden {
    return YES;
}

- (BOOL)shouldAutorotate {
    return YES;
}

- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation {
    [super didRotateFromInterfaceOrientation:fromInterfaceOrientation];

    CCEAGLView *view = (__bridge CCEAGLView *)cocos2d::Director::getInstance()->getOpenGLView()->getEAGLView();
    cocos2d::Application::getInstance()->applicationScreenSizeChanged([view getWidth], [view getHeight]);
}

- (CGRect)currentScreenBoundsDependOnOrientation {
    CGRect bounds = [UIScreen mainScreen].bounds;
    if (NSFoundationVersionNumber > NSFoundationVersionNumber_iOS_7_1) {
        return bounds;
    }

    UIInterfaceOrientation orientation = [UIApplication sharedApplication].statusBarOrientation;
    if (UIInterfaceOrientationIsPortrait(orientation)) {
        bounds.size = CGSizeMake(bounds.size.width, bounds.size.height);
    } else if (UIInterfaceOrientationIsLandscape(orientation)) {
        bounds.size = CGSizeMake(bounds.size.height, bounds.size.width);
    }
    return bounds;
}

@end
