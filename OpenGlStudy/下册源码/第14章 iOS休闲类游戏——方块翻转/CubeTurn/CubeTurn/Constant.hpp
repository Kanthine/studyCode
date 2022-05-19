#ifndef Sample14_4_Constant_hpp
#define Sample14_4_Constant_hpp
#include <math.h>


class Constant{
public:
    constexpr const static float CubeSize=10.0f;//方块大小
    constexpr const static float HalfCubeSize=CubeSize/2;
    constexpr const static float RADIUS=(float) (1.414f*HalfCubeSize);
    constexpr const static float PI=3.1415926f/180.0f;
    constexpr const static float ONE_STEP_ANGLE_PER_FRAME=5;//只走一步5度一帧
    constexpr const static float TWO_STEP_ANGLE_PER_FRAME=10;//走两步10度一帧
    //0--地面方格编号 1--周围边框 2--黑边角
    constexpr const  static  int CUBEID=0;
    constexpr const  static  int FRAMEID=1;
    constexpr const  static  int CORNERID=2;
    constexpr const  static  int SMALL_CUBEID=3;//旋转的小立方体
    constexpr const  static  int THIN_CUBEID=4;//薄的小长方体
    constexpr const  static  int RED_CUBEID=5;//红色可移动小长方体
    constexpr const  static  int GREEN_CUBEID=6;//绿色可移动小长方体
    constexpr const  static  int BLUE_CUBEID=7;//蓝色可移动小长方体
    constexpr const  static  int BLOCK_CUBEID=8;//边框小长方体
    
    constexpr const static float TOUCH_SCALE_FACTOR = 180.0/360;
    constexpr const static float upCubeTransStep = -0.5f;
    constexpr const static float upCubeScaleStep = -0.09f;
    
    constexpr const static float ICON_WIDTH=0.18f;
    constexpr const static float ICON_HEIGHT=0.18f;
    
    const static int touch_span=7;//触控阈值
    const static int MAIN_VIEW_ID = 0;//主界面的id
    const static int GAME_VIEW_ID = 1;//游戏界面的id
    const static int CHOOSE_VIEW_ID=2;//选择关卡界面id
    const static int HELP_VIEW_ID=3;//帮助界面id
    const static int RIGHT_KEY=0;//触控方向
    const static int LEFT_KEY=1;
    const static int UP_KEY=2;
    const static int DOWN_KEY=3;
    
    const static int LEVEL_FIRST=0;//关卡
    const static int LEVEL_SEC=1;
    const static int LEVEL_THIRD=2;
    const static int LEVEL_FOUTH=3;
    
    static double toRadians(double degrees)
    {
        return (PI*degrees);
    }
    
};
#endif
