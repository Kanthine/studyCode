#ifndef __FileUtil_H__
#define __FileUtil_H__

#include "android/asset_manager.h"
#include "android/asset_manager_jni.h"
#include <string>
#include "jni.h"

#include <vector>
using namespace std;//指定使用的命名空间
class FileUtil
{
  public:
	static AAssetManager* aam;//指向AAssetManager对象的指针
	static void setAAssetManager(AAssetManager* aamIn);//初始化AAssetManager对象
	static string loadShaderStr(string fname);//加载着色器
	static void loadFromFile(string fname);	//加载obj模型

  private:
	static void analyze(char* data,int size);
	static std::string assemblyString(char *data);
	//切割字符串
	static void stringSplit(std::string aString,const char* split);
	static void saveVerticesData(std::string aString);
	static void saveTextureData(std::string aString);
	static void saveNormalData(std::string aString);
	static void saveVerticesIndexData();
	static void VertorToFloatpoint();
	static void clearAllAtrri();
  private:
	static int currCount;
	static std::vector<int>* resultVec;
	//储存原始顶点坐标数据的向量
	static std::vector<float>* verticesVec ;
	//储存原始顶点索引数据的向量
	static std::vector<int>* verticesIndexVec ;
	//储存原始纹理坐标数据的向量
	static std::vector<float>* texVec ;
	//储存原始纹理坐标索引数据的向量
	static std::vector<int>* texIndexVec ;
	//储存原始法向量坐标数据的向量
	static std::vector<float>* normalVec ;
	//储存原始法向量坐标索引数据的向量
	static std::vector<int>* normalIndexVec ;
  public:
	static float* m_vertices;		//顶点数据
	static int* m_verticesIndices;	//顶点索引数据
	static float* m_tex;			//纹理数据
	static int* m_texIndices;		//纹理索引数据
	static float* m_normal;			//法向量数据
	static int* m_normalIndices;	//法向量索引数据
	//顶点数目
	static int numVertices;
	//顶点索引数目
	static int numVerticesIndex;
	//纹理数目
	static int numTex;
	//纹理索引数目
	static int numTexIndex;
	//法向量数目
	static int numNormal;
	//法向量索引数目
	static int numNormalIndex;


	static std::string SDCardPath;
	static void setSDCardPath(std::string SDCardPathTemp);
	static void loadFromSDCardFile(std::string sdCardFilePath);			//从SDCard文件加载
	static void spliteStrin(std::string aString,const char* split);
};
#endif
