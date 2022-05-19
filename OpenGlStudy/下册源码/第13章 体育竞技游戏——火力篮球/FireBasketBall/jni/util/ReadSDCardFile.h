#ifndef _ReadSDCardFile_H_
#define _ReadSDCardFile_H_

using namespace std;
#include <string>

class ReadSDCardFile
{
public:
	static void readByLine(std::string sdCardFilePath);
	static void readByChar(std::string sdCardFilePath);
};

#endif
