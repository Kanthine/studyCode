#include "FileUtil.h"

using namespace std;

#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>


AAssetManager* FileUtil::aam;

int FileUtil::currCount = 0;

//储存原始顶点坐标数据的向量
std::vector<float>* FileUtil::verticesVec = new std::vector<float>();
//储存原始顶点索引数据的向量
std::vector<int>* FileUtil::verticesIndexVec = new std::vector<int>();

//储存原始纹理坐标数据的向量
std::vector<float>* FileUtil::texVec = new std::vector<float>();
//储存原始纹理坐标索引数据的向量
std::vector<int>* FileUtil::texIndexVec = new std::vector<int>();

//储存原始法向量坐标数据的向量
std::vector<float>* FileUtil::normalVec = new std::vector<float>();
//储存原始法向量坐标索引数据的向量
std::vector<int>* FileUtil::normalIndexVec = new std::vector<int>();
//储存临时结果的向量
std::vector<int>* FileUtil::resultVec = new std::vector<int>();

//顶点数目
int FileUtil::numVertices = 0;
//顶点索引数目
int FileUtil::numVerticesIndex = 0;

//纹理数目
int FileUtil::numTex = 0;
//纹理索引数目
int FileUtil::numTexIndex = 0;

//法向量数目
int FileUtil::numNormalIndex = 0;
//法向量索引数目
int FileUtil::numNormal = 0;

float* FileUtil::m_vertices = NULL;//顶点数据
int* FileUtil::m_verticesIndices = NULL;//顶点索引数据
float* FileUtil::m_tex = NULL;//纹理数据
int* FileUtil::m_texIndices = NULL;//顶点索引数据
float* FileUtil::m_normal = NULL;//法向量数据
int* FileUtil::m_normalIndices = NULL;//顶点索引数据

std::string FileUtil::SDCardPath = "";
void FileUtil::setSDCardPath(std::string SDCardPathTemp)
{
	SDCardPath = SDCardPathTemp;
}
void FileUtil::setAAssetManager(AAssetManager* aamIn)//初始化AAssetManager对象
{
	aam=aamIn;//给AAssetManager对象赋值
}
void FileUtil::clearAllAtrri()
{
	currCount = 0;

	//储存原始顶点坐标数据的向量
	verticesVec->clear();
	//储存原始顶点索引数据的向量
	verticesIndexVec->clear();
	//储存原始纹理坐标数据的向量
	texVec->clear();
	//储存原始纹理坐标索引数据的向量
	texIndexVec->clear();
	//储存原始法向量坐标数据的向量
	normalVec->clear();
	//储存原始法向量坐标索引数据的向量
	normalIndexVec->clear();
	//储存临时结果的向量
	resultVec->clear();

	//顶点数目
	numVertices = 0;
	//顶点索引数目
	numVerticesIndex = 0;
	//纹理数目
	numTex = 0;
	//纹理索引数目
	numTexIndex = 0;
	//法向量数目
	numNormalIndex = 0;
	//法向量索引数目
	numNormal = 0;

	delete m_vertices;
	m_vertices = NULL ;//顶点数据
	delete m_verticesIndices;
	m_verticesIndices = NULL ;//顶点索引数据
	delete m_tex;
	m_tex = NULL;//纹理数据
	delete m_texIndices;
	m_texIndices = NULL;//顶点索引数据
	delete m_normal;
	m_normal = NULL;//法向量数据
	delete m_normalIndices;
	m_normalIndices = NULL;//顶点索引数据
}
string FileUtil::loadShaderStr(string fname)//加载着色器文件
{
	AAsset* asset =AAssetManager_open(aam,fname.c_str(),AASSET_MODE_UNKNOWN);//创建AAsset对象
	off_t fileSize = AAsset_getLength(asset);//获取AAsset对象的长度
	unsigned char* data = (unsigned char*) malloc(fileSize + 1);//分配内存
	data[fileSize] = '\0';//初始化
	int size = AAsset_read(asset, (void*)data, fileSize);//读取文件大小
	std::string resultStr((const char*)data);//获得结果字符串
	return resultStr;
}
void FileUtil::loadFromFile(string fname)//加载obj模型
{
	clearAllAtrri();
	AAsset* asset =AAssetManager_open(aam,fname.c_str(),AASSET_MODE_UNKNOWN);//创建AAsset对象
	off_t fileSize = AAsset_getLength(asset);//获取AAsset对象的长度
	char *data = new char[fileSize + 1];//分配内存
	data[fileSize] = '\0';//初始化
	int size = AAsset_read(asset, (void*)data, fileSize);//读取文件大小
	std::string resultStr((const char*)data);//获得结果字符串
	analyze(data,size);
}
void FileUtil::loadFromSDCardFile(std::string sdCardFilePath)
{
	//清空数据
	clearAllAtrri();
	fstream inFileStream;
	inFileStream.open(SDCardPath + sdCardFilePath,ios::in);
	char *buffer = new char[256];
	const char *splitVTNF = " ";
	const char *splitF = "/";
	while(!inFileStream.eof())
	{
		inFileStream.getline(buffer,256,'\n');			//按行获取---获取一行
		string afString = &buffer[0];
		//按空格切分字符串
		char *tempsa = strtok((char*)afString.c_str(),splitVTNF);
		if(tempsa != NULL)
		{
			string tempString = &tempsa[0];
			if(tempString == "f")
			{
				std::string* stringArray = new std::string[3];
				int index = 0;
				for(int i=0;i<3;i++)
				{
					//先用空格分割
					tempsa = strtok(NULL,splitVTNF);
					std::string temps((const char*)tempsa);//获得结果字符串
					stringArray[index++] = temps;
				}
				//对第一个字符串解析
				stringSplit(stringArray[0],splitF);
				saveVerticesIndexData();
				//对第二个字符串解析
				stringSplit(stringArray[1],splitF);
				saveVerticesIndexData();
				//对第三个字符串解析
				stringSplit(stringArray[2],splitF);
				saveVerticesIndexData();
			}else if(tempString == "v")
			{
				while(tempsa != NULL)
				{
					tempsa = strtok(NULL,splitVTNF);
					if(tempsa != NULL)
					{
						std::string aString = &tempsa[0];
						saveVerticesData(aString);
					}
				}
			}else if(tempString == "vt")
			{
				while(tempsa != NULL)
				{
					tempsa = strtok(NULL,splitVTNF);
					if(tempsa != NULL)
					{
						std::string aString = &tempsa[0];
						saveTextureData(aString);
					}
				}
			}else if(tempString == "vn")
			{
				while(tempsa != NULL)
				{
					tempsa = strtok(NULL,splitVTNF);
					if(tempsa != NULL)
					{
						std::string aString = &tempsa[0];
						saveNormalData(aString);
					}
				}
			}
		}
	}
	//关闭输入流
	inFileStream.close();
	VertorToFloatpoint();
}
void FileUtil::spliteStrin(std::string aString,const char* split)
{
	char *p;
	p = strtok((char*)aString.c_str(),split);
	while(p != NULL)
	{
		int it = 0;
		p = strtok(NULL,split);
	}
}
void FileUtil::analyze(char *data,int size)//加载obj模型
{
	FileUtil::currCount = 0;
	const char *split = "/";
	while(FileUtil::currCount < size)
	{
		std::string aString = assemblyString(data);//获取下个字符串
		if(aString == "f")
		{
			std::string afString = assemblyString(data);//获取下个字符串
			stringSplit(afString,split);
			saveVerticesIndexData();

			afString = assemblyString(data);//获取下个字符串
			stringSplit(afString,split);
			saveVerticesIndexData();

			afString = assemblyString(data);//获取下个字符串
			stringSplit(afString,split);
			saveVerticesIndexData();
		}else if(aString == "v")
		{
			for(int i=0;i<3;i++)
			{
				std::string afString = assemblyString(data);//获取下个字符串
				saveVerticesData(afString);
			}
		}else if(aString == "vt")
		{
			for(int i=0;i<3;i++)
			{
				std::string afString = assemblyString(data);//获取下个字符串
				saveTextureData(afString);
			}
		}else if(aString == "vn")
		{
			for(int i=0;i<3;i++)
			{
			std::string afString = assemblyString(data);//获取下个字符串
			saveNormalData(afString);
			}
		}
	}
	VertorToFloatpoint();
}
void FileUtil::VertorToFloatpoint()
{
	//记录顶点数目
	numVertices = verticesVec->size();
	if(m_vertices != NULL)
	{
		delete m_vertices;
		m_vertices = nullptr;
	}
	m_vertices = new float[numVertices];
	for(int i=0;i<numVertices;i++)
	{
		m_vertices[i] = verticesVec->at(i);
	}

	//记录顶点索引数目
	numVerticesIndex = verticesIndexVec->size();
	if(m_verticesIndices != NULL)
	{
		delete m_verticesIndices;
		m_verticesIndices = nullptr;
	}
	m_verticesIndices = new int[numVerticesIndex];
	for(int i=0;i<numVerticesIndex;i++)
	{
		m_verticesIndices[i] = verticesIndexVec->at(i)-1;
	}

	//记录纹理数目
	numTex = texVec->size();
	float* m_tex_temp = new float[numTex];
	for(int i=0;i<numTex;i++)
	{
		m_tex_temp[i] = texVec->at(i);
	}

	//记录纹理索引数目
	numTexIndex = texIndexVec->size();
	if(m_texIndices != NULL)
	{
		delete m_texIndices;
		m_texIndices = nullptr;
	}
	m_texIndices = new int[numTexIndex];
	for(int i=0;i<numTexIndex;i++)
	{
		m_texIndices[i] = texIndexVec->at(i);
	}

	//组装纹理数据
	numTex = numTexIndex*2;
	if(m_tex != NULL)
	{
		delete m_tex;
		m_tex = nullptr;
	}
	m_tex = new float[numTex];
	for(int i=0;i<numTexIndex;i++)
	{
		int index = m_texIndices[i] - 1;
    	m_tex[i*2+0] = m_tex_temp[index*3];
    	m_tex[i*2+1] = m_tex_temp[index*3+1];
	}

	//记录法向量数目
	numNormal = normalVec->size();
	float *m_normal_temp = new float[numNormal];
	for(int i=0;i<numNormal;i++)
	{
		m_normal_temp[i] = normalVec->at(i);
	}
	//记录法向量索引数目
	numNormalIndex = normalIndexVec->size();
	if(m_normalIndices != NULL)
	{
		delete m_normalIndices;
		m_normalIndices = nullptr;
	}
	m_normalIndices = new int[numNormalIndex];
	for(int i=0;i<numNormalIndex;i++)
	{
		m_normalIndices[i] = normalIndexVec->at(i);
	}

	numNormal = numNormalIndex * 3;
	if(m_normal != NULL)
	{
		delete m_normal;
		m_normal = nullptr;
	}
	m_normal = new float[numNormal];
	for(int i=0;i<numNormalIndex;i++)
	{
		int index = m_normalIndices[i] - 1;
		m_normal[i*3+0] = m_normal_temp[index*3+0];
		m_normal[i*3+1] = m_normal_temp[index*3+1];
		m_normal[i*3+2] = m_normal_temp[index*3+2];
	}
}
void FileUtil::saveVerticesIndexData()
{
	//储存原始顶点索引
	verticesIndexVec->push_back(resultVec->at(0));
	//储存原始纹理索引
	texIndexVec->push_back(resultVec->at(1));
	//储存原始法向量索引
	normalIndexVec->push_back(resultVec->at(2));
}
void FileUtil::saveVerticesData(std::string aString)
{
	//字符串转数字======为什么有小误差
	float verticestf = atof(aString.c_str());
	verticesVec->push_back(verticestf);
}
void FileUtil::saveTextureData(std::string aString)
{
	float textf = atof(aString.c_str());
	texVec->push_back(textf);
}
void FileUtil::saveNormalData(std::string aString)
{
	float normaltf = atof(aString.c_str());
	normalVec->push_back(normaltf);
}

void FileUtil::stringSplit(std::string aString,const char* split)
{
	//清空向量
	resultVec->clear();
	int resultIndex = 0;
	char *p;
	p = strtok((char*)aString.c_str(),split);
	while(p != NULL)
	{
		int it = 0;
		sscanf(p,"%d",&it);		//字符转整型
		resultVec->push_back(it);
		p = strtok(NULL,split);
	}
}
#define KONGGE 32
#define JINHAO 35
#define HUICHE 13
#define HUANHANG 10
std::string FileUtil::assemblyString(char *data)
{
	std::string result = "";
	bool flag = true;
	while(flag)
	{
		char c = data[FileUtil::currCount];//读取一个字符
		int ci = (int)c;
		if(ci == KONGGE || ci == HUANHANG)
		{
			FileUtil::currCount ++;
			if(result.size() != 0)
			{
				flag = false;
				return result;
			}
		}else if(ci == JINHAO || ci == HUICHE)
		{
			FileUtil::currCount++;
		}else
		{
			result = result+c;
			FileUtil::currCount++;
		}
	}
	return result;
}
