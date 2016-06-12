#import "RootViewController.h"
#import "cocos2d.h"
#import "platform/ios/CCEAGLView-ios.h"

@interface RootViewController ()

@property (strong, nonatomic, readonly) CCEAGLView *eaGlView;

@end

@implementation RootViewController

@synthesize eaGlView = _eaGlView;

- (CCEAGLView *)eaGlView {
    if (!_eaGlView) {
        _eaGlView = [CCEAGLView viewWithFrame:[UIScreen mainScreen].bounds
                                  pixelFormat:(NSString *)cocos2d::GLViewImpl::_pixelFormat
                                  depthFormat:cocos2d::GLViewImpl::_depthFormat
                           preserveBackbuffer:NO
                                   sharegroup:nil
                                multiSampling:NO
                              numberOfSamples:0];
        _eaGlView.multipleTouchEnabled = NO;
    }
    return _eaGlView;
}

- (void)loadView {
    self.view = self.eaGlView;
}

- (void)viewDidLoad {
    [super viewDidLoad];

    cocos2d::Application *app = cocos2d::Application::getInstance();
    app->initGLContextAttrs();
    cocos2d::GLViewImpl::convertAttrs();
    cocos2d::GLView *glView = cocos2d::GLViewImpl::createWithEAGLView(self.eaGlView);
    cocos2d::Director::getInstance()->setOpenGLView(glView);
    app->run();
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

    CGSize size = CGSizeMake([self.eaGlView getWidth], [self.eaGlView getHeight]);
    cocos2d::Application::getInstance()->applicationScreenSizeChanged((int)size.width, (int)size.height);
}

@end
