#ifndef _ShaderManager_H_
#define _ShaderManager_H_
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

#include <string>

using namespace std;

class ShaderManager {
public:
	ShaderManager();
	~ShaderManager() {
	}
	static const int shaderCount;
	static const string shaderName[3][2];
	static GLuint* program;
public:
	static void compileShader();
	static GLuint getCommTextureShaderProgram();
	static GLuint getShadowshaderProgram();
	static GLuint getLigntAndTexturehaderProgram();

};
#endif
