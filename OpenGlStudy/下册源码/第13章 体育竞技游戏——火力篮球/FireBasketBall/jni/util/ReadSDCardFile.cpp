#include "ReadSDCardFile.h"

#include <iostream>
#include <fstream>


void ReadSDCardFile::readByLine(std::string sdCardFilePath)
{
	char buffer[256];
	//输入流
	fstream inFileStream;
	inFileStream.open("/sdcard/" + sdCardFilePath,ios::in);//打开文件
	while(!inFileStream.eof())
	{
		inFileStream.getline(buffer,256,'\n');			//按行获取
		string s(&buffer[0],&buffer[strlen(buffer)]);
	}
	//关闭输入流
	inFileStream.close();
}
void ReadSDCardFile::readByChar(std::string sdCardFilePath)
{
	//文件输出流
	fstream inFileStream;
	//储存读取的字符
	char c;
	//打开文件
	inFileStream.open("/sdcard/" + sdCardFilePath,ios::in);
	while(!inFileStream.eof())
	{
		inFileStream>>c;							//按字符获取
	}
	inFileStream.close();
}
