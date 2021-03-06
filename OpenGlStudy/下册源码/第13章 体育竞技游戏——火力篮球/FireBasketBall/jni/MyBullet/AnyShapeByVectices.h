#ifndef _AnyShapeByVectices_H_
#define _AnyShapeByVectices_H_

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include "../Bullet/LinearMath/btVector3.h"

class AnyShapeByVectices
{

	GLuint mProgram;//自定义渲染管线程序id
	GLuint muMVPMatrixHandle;//总变换矩阵id
	GLuint muMMatrixHandle;//位置、旋转变换矩阵
	GLuint muTexHandle;//外观纹理属性id
	GLuint muLightLocationHandle;//光源位置属性id
	GLuint muCameraHandle;//摄像机位置属性id

	GLuint maPositionHandle;//顶点位置属性id
	GLuint maTexCoorHandle;//顶点纹理坐标属性id
	GLuint maNormalHandle;//顶点法向量属性id

	const GLvoid* mVertexBuffer;//顶点坐标数据缓冲
	const GLvoid* mNormalBuffer;//顶点法向量数据缓冲
	const GLvoid* mTextureBuffer;//顶点纹理数据缓冲


	int m_vCount;//顶点总数 有重复
	int m_vCount_noRepeat;//顶点总数 无重复
	float* m_normals;
	int m_num_normals;
	float* m_vertices;
	float* m_vertices_noRepeat;
	btVector3* m_vertices_bt;
	int* m_indices;//索引数组
	float m_lightFlag;// 为1.0f表示顺时针     为-1.0f表示逆时针  	 一般为-1.0f

	float* m_texs ;

	bool update;//当物体的顶点不断发生变换时，是否进行更新

public:

	AnyShapeByVectices(
			float* vertices_fl,//顶点*3数组	无重复值
			int numsVerticesBt,//顶点*3总数（无重复值）
			int* indices,//索引数组
			int numsIndices,//索引总数
			float* m_texs,
			int m_nums_tex,
			float* m_normals,
			int m_num_normals,
			bool update,
			float lightFlag,// 为1表示顺时针     为-1表示逆时针
			GLuint mprogram
			);//三角形

	AnyShapeByVectices(
			float* vertices_fl,//顶点数组	无重复值
			int numsVerticesBt,//顶点总数（无重复值）
			int* indices,//索引数组
			int numsIndices,//索引总数
			float* m_texs,
			int m_nums_tex,
			bool update,//形状需要发生变换时，为true，形状不需要发生变换时，为false
			float lightFlag,// 为1表示顺时针     为-1表示逆时针
			GLuint mprogram
			);//三角形

    void initShader(GLuint mprogram);
    void drawSelf(int texId);
    void initIndices();

    float* method(
    		btVector3* vertices,//顶点数组(有重复值的)
    		int nums_vertices,//顶点数组的长度
    		int* index_vertices,//顶点数组的索引数组
    		int nums_indexVertices//顶点数组的索引数组的长度	注意：应该与nums_vertices相同
    		);

	void initVNData();

	float* btVector3ToFloat(btVector3* vertex, int nums);//顶点数组，顶点总数 返回数组长度 nums*3

	btVector3* floatToBtVector3(float* vertex, int nums);//顶点数组，顶点总数  返回数组长度 nums/3
};

#endif
