package com.bn.Sample3_13;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import android.opengl.GLES30;

public class Mountion
{
	//单位长度
	float UNIT_SIZE=3.0f;
	//体积雾产生者平面的高度
	float TJ_GOG_SLAB_Y=8f;
	//扰动起始角
	float startAngle=0;

	//自定义渲染管线的id
	int mProgram;
	//总变化矩阵引用的id
	int muMVPMatrixHandle;
	//基本变换阵引用的id
	int muMMatrixHandle;
	//摄像机位置引用的id
	int muCamaraLocationHandle;
	//体积雾产生者平面高度引用的id
	int slabYHandle;
	//体积雾高度扰动起始角引用的id
	int startAngleHandle;
	//顶点位置属性引用id
	int maPositionHandle;
	//顶点纹理坐标属性引用id
	int maTexCoorHandle;

	//草地的id
	int sTextureGrassHandle;
	//石头的id
	int sTextureRockHandle;
	//起始x值
	int landStartYYHandle;
	//长度
	int landYSpanHandle;

	//顶点数据缓冲和纹理坐标数据缓冲
	FloatBuffer mVertexBuffer;
	FloatBuffer mTexCoorBuffer;
	//顶点数量
	int vCount=0;

	public Mountion(MySurfaceView mv,float[][] yArray,int rows,int cols)
	{
		initVertexData(yArray,rows,cols);
		initShader(mv);
	}
	//初始化顶点坐标与着色数据的方法
    public void initVertexData(float[][] yArray,int rows,int cols)
    {
    	//顶点坐标数据的初始化
    	vCount=cols*rows*2*3;//每个格子两个三角形，每个三角形3个顶点
        float vertices[]=new float[vCount*3];//每个顶点xyz三个坐标
        int count=0;//顶点计数器
        for(int j=0;j<rows;j++)
        {
        	for(int i=0;i<cols;i++)
        	{
        		//计算当前格子左上侧点坐标
        		float zsx=-UNIT_SIZE*cols/2+i*UNIT_SIZE;
        		float zsz=-UNIT_SIZE*rows/2+j*UNIT_SIZE;

        		vertices[count++]=zsx;
        		vertices[count++]=yArray[j][i];
        		vertices[count++]=zsz;

        		vertices[count++]=zsx;
        		vertices[count++]=yArray[j+1][i];
        		vertices[count++]=zsz+UNIT_SIZE;

        		vertices[count++]=zsx+UNIT_SIZE;
        		vertices[count++]=yArray[j][i+1];
        		vertices[count++]=zsz;

        		vertices[count++]=zsx+UNIT_SIZE;
        		vertices[count++]=yArray[j][i+1];
        		vertices[count++]=zsz;

        		vertices[count++]=zsx;
        		vertices[count++]=yArray[j+1][i];
        		vertices[count++]=zsz+UNIT_SIZE;

        		vertices[count++]=zsx+UNIT_SIZE;
        		vertices[count++]=yArray[j+1][i+1];
        		vertices[count++]=zsz+UNIT_SIZE;
        	}
        }

        //创建顶点坐标数据缓冲
        ByteBuffer vbb = ByteBuffer.allocateDirect(vertices.length*4);
        vbb.order(ByteOrder.nativeOrder());//设置字节顺序
        mVertexBuffer = vbb.asFloatBuffer();//转换为Float型缓冲
        mVertexBuffer.put(vertices);//向缓冲区中放入顶点坐标数据
        mVertexBuffer.position(0);//设置缓冲区起始位置

        //顶点纹理坐标数据的初始化
        float[] texCoor=generateTexCoor(cols,rows);
        //创建顶点纹理坐标数据缓冲
        ByteBuffer cbb = ByteBuffer.allocateDirect(texCoor.length*4);
        cbb.order(ByteOrder.nativeOrder());//设置字节顺序
        mTexCoorBuffer = cbb.asFloatBuffer();//转换为Float型缓冲
        mTexCoorBuffer.put(texCoor);//向缓冲区中放入顶点着色数据
        mTexCoorBuffer.position(0);//设置缓冲区起始位置
    }

	//初始化Shader的方法
	public void initShader(MySurfaceView mv)
	{
		String mVertexShader=ShaderUtil.loadFromAssetsFile("vertex.sh", mv.getResources());
		String mFragmentShader=ShaderUtil.loadFromAssetsFile("frag.sh", mv.getResources());
		//基于顶点着色器与片元着色器创建程序
        mProgram = ShaderUtil.createProgram(mVertexShader, mFragmentShader);
        //获取程序中顶点位置属性引用id
        maPositionHandle = GLES30.glGetAttribLocation(mProgram, "aPosition");
        //获取程序中顶点纹理坐标属性引用id
        maTexCoorHandle= GLES30.glGetAttribLocation(mProgram, "aTexCoor");
        //获取程序中总变换矩阵引用的id
        muMVPMatrixHandle = GLES30.glGetUniformLocation(mProgram, "uMVPMatrix");
        //获取程序中基本变换矩阵引用的id
        muMMatrixHandle=GLES30.glGetUniformLocation(mProgram, "uMMatrix");
        //获取程序中摄像机位置引用的id
        muCamaraLocationHandle=GLES30.glGetUniformLocation(mProgram, "uCamaraLocation");
        //获取程序中体积雾产生者平面高度引用的id
        slabYHandle=GLES30.glGetUniformLocation(mProgram, "slabY");
        //获取程序中体积雾高度扰动起始角引用的id
        startAngleHandle=GLES30.glGetUniformLocation(mProgram, "startAngle");

        //纹理
		//草地
		sTextureGrassHandle=GLES30.glGetUniformLocation(mProgram, "sTextureGrass");
		//石头
		sTextureRockHandle=GLES30.glGetUniformLocation(mProgram, "sTextureRock");
		//x位置
		landStartYYHandle=GLES30.glGetUniformLocation(mProgram, "landStartY");
		//x最大
		landYSpanHandle=GLES30.glGetUniformLocation(mProgram, "landYSpan");
	}

	//自定义的绘制方法drawSelf
	public void drawSelf(int texId,int rock_textId)
	{
		//指定使用某套着色器程序
   	 	GLES30.glUseProgram(mProgram);
        //将最终变换矩阵传入渲染管线
        GLES30.glUniformMatrix4fv(muMVPMatrixHandle, 1, false, MatrixState.getFinalMatrix(), 0);
        //将基本变换矩阵传入渲染管线
        GLES30.glUniformMatrix4fv(muMMatrixHandle, 1, false, MatrixState.getMMatrix(), 0);
        //将摄像机位置传入渲染管线
        GLES30.glUniform3fv(muCamaraLocationHandle, 1,MatrixState.cameraFB);


        //将体积雾的雾平面高度传入渲染管线
        GLES30.glUniform1f(slabYHandle, TJ_GOG_SLAB_Y);
        //将体积雾扰动起始角传入渲染管线
        GLES30.glUniform1f(startAngleHandle, (float) Math.toRadians(startAngle));
        //修改扰动角的值，每次加3，取值范围永远在0~360的范围内
        startAngle=(startAngle+3f)%360.0f;

        //传送顶点位置数据
		GLES30.glVertexAttribPointer
		(
			maPositionHandle,
			3,
			GLES30.GL_FLOAT,
			false,
			3*4,
			mVertexBuffer
		);
		//传送顶点纹理坐标数据
		GLES30.glVertexAttribPointer
		(
			maTexCoorHandle,
			2,
			GLES30.GL_FLOAT,
			false,
			2*4,
			mTexCoorBuffer
		);
		//允许顶点位置数据数组
        GLES30.glEnableVertexAttribArray(maPositionHandle);
        GLES30.glEnableVertexAttribArray(maTexCoorHandle);

        //绑定纹理
        GLES30.glActiveTexture(GLES30.GL_TEXTURE0);
        GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, texId);
        GLES30.glActiveTexture(GLES30.GL_TEXTURE1);
		GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, rock_textId);
		GLES30.glUniform1i(sTextureGrassHandle, 0);//使用0号纹理
        GLES30.glUniform1i(sTextureRockHandle, 1); //使用1号纹理

        //传送相应的x参数
        GLES30.glUniform1f(landStartYYHandle, 0);
        GLES30.glUniform1f(landYSpanHandle, 50);

        //绘制纹理矩形
        GLES30.glDrawArrays(GLES30.GL_TRIANGLES, 0, vCount);
	}
	//自动切分纹理产生纹理数组的方法
    public float[] generateTexCoor(int bw,int bh)
    {
    	float[] result=new float[bw*bh*6*2];
    	float sizew=16.0f/bw;//列数
    	float sizeh=16.0f/bh;//行数
    	int c=0;
    	for(int i=0;i<bh;i++)
    	{
    		for(int j=0;j<bw;j++)
    		{
    			//每行列一个矩形，由两个三角形构成，共六个点，12个纹理坐标
    			float s=j*sizew;
    			float t=i*sizeh;

    			result[c++]=s;
    			result[c++]=t;

    			result[c++]=s;
    			result[c++]=t+sizeh;

    			result[c++]=s+sizew;
    			result[c++]=t;

    			result[c++]=s+sizew;
    			result[c++]=t;

    			result[c++]=s;
    			result[c++]=t+sizeh;

    			result[c++]=s+sizew;
    			result[c++]=t+sizeh;
    		}
    	}
    	return result;
    }
}
