#import "ViewController.h"
#import "GLView.h"

@interface ViewController ()

@property (nonatomic, strong) GLView *glView;
@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)viewWillAppear:(BOOL)animated{
    [super viewWillAppear:animated];
    [self glView];
}

- (GLView *)glView{
    if (!_glView) {
        _glView=[[GLView alloc] initWithFrame:self.view.bounds];
        [self.view addSubview:_glView];
        //双手指扩大或者缩小
        UIPinchGestureRecognizer *twoFingersPinch = [[UIPinchGestureRecognizer alloc]initWithTarget:_glView action:@selector(twoFingerPinch:)];
        [_glView addGestureRecognizer:twoFingersPinch];
        //单手指双击两次手势
        UITapGestureRecognizer *oneFingerTwoTaps = [[UITapGestureRecognizer alloc] initWithTarget:_glView action:@selector(oneFingerTwoTaps)];
        [oneFingerTwoTaps setNumberOfTapsRequired:2];
        [oneFingerTwoTaps setNumberOfTouchesRequired:1];
        [_glView addGestureRecognizer:oneFingerTwoTaps];
        //双手指双击两次手势
        UITapGestureRecognizer *twoFingersTwoTaps = [[UITapGestureRecognizer alloc] initWithTarget:_glView action:@selector(twoFingersTwoTaps)];
        [twoFingersTwoTaps setNumberOfTapsRequired:2];
        [twoFingersTwoTaps setNumberOfTouchesRequired:2];
        [_glView addGestureRecognizer:twoFingersTwoTaps];
    }
    return _glView;
}
@end
