#include "MyBullet/Digital.h"
#include "util/Constant.h"

#include "stdlib.h"
#include <string>

#include "util/MatrixState.h"

using namespace std;

#define INTTOSTR(I) #I   //可以使用宏定义

Digital::Digital(GLuint mprogram)
{
	//创建纹理矩形--并指定着色器
	for(int i=0;i<10;i++)
	{
		int count = 0;
		float* texCoord = new float[12];

		texCoord[count++] = 0.1f*i;
		texCoord[count++] = 0;

		texCoord[count++] = 0.1f*i;
		texCoord[count++] = 1;

		texCoord[count++] = 0.1f*(i+1);
		texCoord[count++] = 1;

		texCoord[count++] = 0.1f*i;
		texCoord[count++] = 0;

		texCoord[count++] = 0.1f*(i+1);
		texCoord[count++] = 1;

		texCoord[count++] = 0.1f*(i+1);
		texCoord[count++] = 0;

		textureRectArray[i] = new TextureRect(
				Constant::SHUZI_KUANDU/2,
				Constant::SHUZI_GAODU/2,
//				0.3,0.3,
				0,1,mprogram,texCoord
		);
	}
}
void Digital::drawSelf(int score,GLuint texId)//传入数字和纹理坐标
{
    //开启混合
    glEnable(GL_BLEND);
    //设置混合因子
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//数字转换成字符串
	std::string scoreStr ;
	//储存转换后的字符
	char index[10];
	 //数字转换为字符串
	sprintf(index,"%d",score);
	scoreStr = index;
	const char* charArray = scoreStr.c_str();

	for(int i=0;i<scoreStr.length();i++)
	{//将得分中的每个数字字符绘制

		char c=charArray[i];

		 MatrixState::pushMatrix();
		 MatrixState::translate(i*Constant::SHUZI_KUANDU, 0, 0);
		 MatrixState::rotate(90,0,0,1);
		 textureRectArray[c-'0']->drawSelf(texId);
		 MatrixState::popMatrix();
	}
	glDisable(GL_BLEND);
}
