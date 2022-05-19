#ifndef _Ball_H_
#define _Ball_H_

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

class Ball
{
    GLuint mProgram;				//自定义渲染管线程序id

	//获取程序中顶点位置属性引用id
    GLuint maPositionHandle;
	//获取程序中顶点纹理坐标属性引用id
    GLuint maTexCoorHandle;
	//获取程序中总变换矩阵引用id
    GLuint muMVPMatrixHandle;
	//获取位置、旋转变换矩阵引用id
    GLuint muMMatrixHandle;
	//获取程序中顶点法向量属性引用id
    GLuint maNormalHandle;
	//获取程序中光源位置引用id
    GLuint maLightLocationHandle;
	//获取程序中摄像机位置引用id
    GLuint maCameraHandle;
	//获取程序中是否绘制阴影属性引用id
    GLuint muIsShadow;
    GLuint muIsShadowFrag;
	//获取是否绘制篮板上阴影属性应用ID
    GLuint muIsLanBanShdow;
	//获取程序中摄像机矩阵引用id
    GLuint muCameraMatrixHandle;
	//获取程序中投影矩阵引用id
    GLuint muProjMatrixHandle;
	//获取桌球纹理属性引用id
    GLuint muBallTexHandle;
	//获取程序中平面法向量引用id;
    GLuint muPlaneN;
	//获取程序中平面上的点的引用的Id
    GLuint muPlaneV;

    int isLanbanYy;
    int isShadow;

    const GLvoid* mVertexBuffer;	//顶点坐标数据缓冲
    const GLvoid* mTextureBuffer;	//顶点纹理坐标数据缓冲
    const GLvoid* mNormalBuffer;	//顶点法向量数据缓冲

    float r;
    int texId;						//纹理id
    int vCount;						//顶点总数 有重复
public:
	Ball(
			int texIdIn,
			float r,//半径
			GLuint programIn
			);
//	~Ball();

    void initShader(GLuint programIn);
    void drawSelf(int texIdIn,int isShadow,int planeId,int isLanbanYy);

    void initVertexData();
    float toRadians(float angle);
    void generateTexCoor(int bw,int bh,float* tex);
};

#endif
