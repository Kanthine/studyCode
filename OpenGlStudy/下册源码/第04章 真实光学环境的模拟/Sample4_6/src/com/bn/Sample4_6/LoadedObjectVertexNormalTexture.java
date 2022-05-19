package com.bn.Sample4_6;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import android.opengl.GLES30;

//加载后的物体——仅携带顶点信息，颜色随机
public class LoadedObjectVertexNormalTexture
{	
	int mProgram;//自定义渲染管线着色器程序id  
    int muMVPMatrixHandle;//总变换矩阵引用
    int muMMatrixHandle;//位置、旋转变换矩阵
    int maPositionHandle; //顶点位置属性引用  
    int maNormalHandle; //顶点法向量属性引用  
    int maTangentHandle; //顶点切向量属性引用  
    int maLightLocationHandle;//光源位置属性引用  
    int maCameraHandle; //摄像机位置属性引用 
    int maTexCoorHandle; //顶点纹理坐标属性引用  
    String mVertexShader;//顶点着色器代码脚本    	 
    String mFragmentShader;//片元着色器代码脚本
	
	FloatBuffer   mVertexBuffer;//顶点坐标数据缓冲  
	FloatBuffer   mNormalBuffer;//顶点法向量数据缓冲
	FloatBuffer   mTangentBuffer;//顶点切向量数据缓冲
	FloatBuffer   mTexCoorBuffer;//顶点纹理坐标数据缓冲
    int vCount=0;  
    
    int uTexHandle;//外观纹理属性引用  
    int uNormalTexHandle;//法线纹理属性引用  
    
    public LoadedObjectVertexNormalTexture(MySurfaceView mv,float[] vertices,float[] normals,float[] texCoors,float[] tangent)
    {//带有凹凸贴图的物体    	
    	//初始化顶点数据
    	initVertexData(vertices,normals,texCoors,tangent);
    	//初始化着色器       
    	initShader(mv);
    }
    public LoadedObjectVertexNormalTexture(MySurfaceView mv,float[] vertices,float[] normals,float[] texCoors)
    {//普通物体    	
    	//初始化顶点数据
    	initVertexDataN(vertices,normals,texCoors);
    	//初始化着色器       
    	initShaderN(mv);
    }
    //初始化普通物体顶点数据的方法
    public void initVertexDataN(float[] vertices,float[] normals,float texCoors[])
    {
    	//顶点坐标数据的初始化================begin============================
    	vCount=vertices.length/3;   
		
        //创建顶点坐标数据缓冲
        //vertices.length*4是因为一个整数四个字节
        ByteBuffer vbb = ByteBuffer.allocateDirect(vertices.length*4);
        vbb.order(ByteOrder.nativeOrder());//设置字节顺序
        mVertexBuffer = vbb.asFloatBuffer();//转换为Float型缓冲
        mVertexBuffer.put(vertices);//向缓冲区中放入顶点坐标数据
        mVertexBuffer.position(0);//设置缓冲区起始位置
        //特别提示：由于不同平台字节顺序不同数据单元不是字节的一定要经过ByteBuffer
        //转换，关键是要通过ByteOrder设置nativeOrder()，否则有可能会出问题
        //顶点坐标数据的初始化================end============================
        
        //顶点法向量数据的初始化================begin============================  
        ByteBuffer cbb = ByteBuffer.allocateDirect(normals.length*4);
        cbb.order(ByteOrder.nativeOrder());//设置字节顺序
        mNormalBuffer = cbb.asFloatBuffer();//转换为Float型缓冲
        mNormalBuffer.put(normals);//向缓冲区中放入顶点法向量数据
        mNormalBuffer.position(0);//设置缓冲区起始位置
        //特别提示：由于不同平台字节顺序不同数据单元不是字节的一定要经过ByteBuffer
        //转换，关键是要通过ByteOrder设置nativeOrder()，否则有可能会出问题
        //顶点着色数据的初始化================end============================
        
        //顶点纹理坐标数据的初始化================begin============================  
        ByteBuffer tbb = ByteBuffer.allocateDirect(texCoors.length*4);
        tbb.order(ByteOrder.nativeOrder());//设置字节顺序
        mTexCoorBuffer = tbb.asFloatBuffer();//转换为Float型缓冲
        mTexCoorBuffer.put(texCoors);//向缓冲区中放入顶点纹理坐标数据
        mTexCoorBuffer.position(0);//设置缓冲区起始位置
        //特别提示：由于不同平台字节顺序不同数据单元不是字节的一定要经过ByteBuffer
        //转换，关键是要通过ByteOrder设置nativeOrder()，否则有可能会出问题
        //顶点纹理坐标数据的初始化================end============================
    }
    
    //初始化带有凹凸贴图的物体顶点数据的方法
    public void initVertexData(float[] vertices,float[] normals,float texCoors[],float[] tangent)
    {
    	//顶点坐标数据的初始化================begin============================
    	vCount=vertices.length/3;   
		
        //创建顶点坐标数据缓冲
        //vertices.length*4是因为一个整数四个字节
        ByteBuffer vbb = ByteBuffer.allocateDirect(vertices.length*4);
        vbb.order(ByteOrder.nativeOrder());//设置字节顺序
        mVertexBuffer = vbb.asFloatBuffer();//转换为Float型缓冲
        mVertexBuffer.put(vertices);//向缓冲区中放入顶点坐标数据
        mVertexBuffer.position(0);//设置缓冲区起始位置
        //特别提示：由于不同平台字节顺序不同数据单元不是字节的一定要经过ByteBuffer
        //转换，关键是要通过ByteOrder设置nativeOrder()，否则有可能会出问题
        //顶点坐标数据的初始化================end============================
        
        //顶点法向量数据的初始化================begin============================  
        ByteBuffer cbb = ByteBuffer.allocateDirect(normals.length*4);
        cbb.order(ByteOrder.nativeOrder());//设置字节顺序
        mNormalBuffer = cbb.asFloatBuffer();//转换为Float型缓冲
        mNormalBuffer.put(normals);//向缓冲区中放入顶点法向量数据
        mNormalBuffer.position(0);//设置缓冲区起始位置
        //特别提示：由于不同平台字节顺序不同数据单元不是字节的一定要经过ByteBuffer
        //转换，关键是要通过ByteOrder设置nativeOrder()，否则有可能会出问题
        //顶点着色数据的初始化================end============================
        
        //顶点切向量数据的初始化================begin============================  
        ByteBuffer tnbb = ByteBuffer.allocateDirect(tangent.length*4);
        tnbb.order(ByteOrder.nativeOrder());//设置字节顺序
        mTangentBuffer = tnbb.asFloatBuffer();//转换为Float型缓冲
        mTangentBuffer.put(tangent);//向缓冲区中放入顶点切向量数据
        mTangentBuffer.position(0);//设置缓冲区起始位置
        //特别提示：由于不同平台字节顺序不同数据单元不是字节的一定要经过ByteBuffer
        //转换，关键是要通过ByteOrder设置nativeOrder()，否则有可能会出问题
        //顶点着色数据的初始化================end============================
        
        
        //顶点纹理坐标数据的初始化================begin============================  
        ByteBuffer tbb = ByteBuffer.allocateDirect(texCoors.length*4);
        tbb.order(ByteOrder.nativeOrder());//设置字节顺序
        mTexCoorBuffer = tbb.asFloatBuffer();//转换为Float型缓冲
        mTexCoorBuffer.put(texCoors);//向缓冲区中放入顶点纹理坐标数据
        mTexCoorBuffer.position(0);//设置缓冲区起始位置
        //特别提示：由于不同平台字节顺序不同数据单元不是字节的一定要经过ByteBuffer
        //转换，关键是要通过ByteOrder设置nativeOrder()，否则有可能会出问题
        //顶点纹理坐标数据的初始化================end============================
    }

    //初始化凹凸贴图的着色器
    public void initShader(MySurfaceView mv)
    {
    	//加载顶点着色器的脚本内容
        mVertexShader=ShaderUtil.loadFromAssetsFile("vertex_ut.sh", mv.getResources());
        //加载片元着色器的脚本内容
        mFragmentShader=ShaderUtil.loadFromAssetsFile("frag_ut.sh", mv.getResources());  
        //基于顶点着色器与片元着色器创建程序
        mProgram = ShaderUtil.createProgram(mVertexShader, mFragmentShader);
        //获取程序中顶点位置属性引用  
        maPositionHandle = GLES30.glGetAttribLocation(mProgram, "aPosition");
        //获取程序中顶点法向量属性引用  
        maNormalHandle= GLES30.glGetAttribLocation(mProgram, "aNormal");
        //获取程序中顶点切向量属性引用  
        maTangentHandle= GLES30.glGetAttribLocation(mProgram, "tNormal");
        //获取程序中总变换矩阵引用
        muMVPMatrixHandle = GLES30.glGetUniformLocation(mProgram, "uMVPMatrix");  
        //获取位置、旋转变换矩阵引用
        muMMatrixHandle = GLES30.glGetUniformLocation(mProgram, "uMMatrix"); 
        //获取程序中光源位置引用
        maLightLocationHandle=GLES30.glGetUniformLocation(mProgram, "uLightLocationSun");
        //获取程序中顶点纹理坐标属性引用  
        maTexCoorHandle= GLES30.glGetAttribLocation(mProgram, "aTexCoor"); 
        //获取程序中摄像机位置引用
        maCameraHandle=GLES30.glGetUniformLocation(mProgram, "uCamera"); 
        //获取外观、法线两个纹理引用
        uTexHandle=GLES30.glGetUniformLocation(mProgram, "sTextureWg");  
        uNormalTexHandle=GLES30.glGetUniformLocation(mProgram, "sTextureNormal");  
    }
    //初始化普通物体的着色器
    public void initShaderN(MySurfaceView mv)
    {
    	//加载顶点着色器的脚本内容
        mVertexShader=ShaderUtil.loadFromAssetsFile("vertex.sh", mv.getResources());
        //加载片元着色器的脚本内容
        mFragmentShader=ShaderUtil.loadFromAssetsFile("frag.sh", mv.getResources());  
        //基于顶点着色器与片元着色器创建程序
        mProgram = ShaderUtil.createProgram(mVertexShader, mFragmentShader);
        //获取程序中顶点位置属性引用  
        maPositionHandle = GLES30.glGetAttribLocation(mProgram, "aPosition");
        //获取程序中顶点法向量属性引用  
        maNormalHandle= GLES30.glGetAttribLocation(mProgram, "aNormal");
        //获取程序中总变换矩阵引用
        muMVPMatrixHandle = GLES30.glGetUniformLocation(mProgram, "uMVPMatrix");  
        //获取位置、旋转变换矩阵引用
        muMMatrixHandle = GLES30.glGetUniformLocation(mProgram, "uMMatrix"); 
        //获取程序中光源位置引用
        maLightLocationHandle=GLES30.glGetUniformLocation(mProgram, "uLightLocation");
        //获取程序中顶点纹理坐标属性引用  
        maTexCoorHandle= GLES30.glGetAttribLocation(mProgram, "aTexCoor"); 
        //获取程序中摄像机位置引用
        maCameraHandle=GLES30.glGetUniformLocation(mProgram, "uCamera");
    }
    //绘制带有凹凸贴图的物体
    public void drawSelf(int texId,int texIdNormal)
    {        
    	 //指定使用某套着色器程序
    	 GLES30.glUseProgram(mProgram);
         //将最终变换矩阵传入渲染管线
         GLES30.glUniformMatrix4fv(muMVPMatrixHandle, 1, false, MatrixState.getFinalMatrix(), 0); 
         //将位置、旋转变换矩阵传入渲染管线
         GLES30.glUniformMatrix4fv(muMMatrixHandle, 1, false, MatrixState.getMMatrix(), 0);   
         //将光源位置传入渲染管线   
         GLES30.glUniform3fv(maLightLocationHandle, 1, MatrixState.lightPositionFB);
         //将摄像机位置传入渲染管线
         GLES30.glUniform3fv(maCameraHandle, 1, MatrixState.cameraFB);
         //将顶点位置数据传入渲染管线
         GLES30.glVertexAttribPointer  
         (
         		maPositionHandle,   
         		3, 
         		GLES30.GL_FLOAT, 
         		false,
                3*4,   
                mVertexBuffer
         );       
         //将顶点法向量数据传入渲染管线
         GLES30.glVertexAttribPointer  
         (
        		maNormalHandle, 
         		3,   
         		GLES30.GL_FLOAT, 
         		false,
                3*4,   
                mNormalBuffer
         );   
        //将顶点切向量数据传入渲染管线
         GLES30.glVertexAttribPointer  
         (
        		maTangentHandle, 
         		3,   
         		GLES30.GL_FLOAT, 
         		false,
                3*4,   
                mTangentBuffer
         );   
         //将顶点纹理坐标数据传入渲染管线
         GLES30.glVertexAttribPointer  
         (
        		maTexCoorHandle, 
         		2, 
         		GLES30.GL_FLOAT, 
         		false,
                2*4,   
                mTexCoorBuffer
         );   
         GLES30.glEnableVertexAttribArray(maPositionHandle);   //启用顶点位置数据数组
         GLES30.glEnableVertexAttribArray(maNormalHandle);   //启用法向量、纹理坐标数据数组
         GLES30.glEnableVertexAttribArray(maTangentHandle);  //启用切向量数据数组
         GLES30.glEnableVertexAttribArray(maTexCoorHandle);   //启用纹理坐标数据数组
         
         GLES30.glActiveTexture(GLES30.GL_TEXTURE0);//启用0号纹理
         GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, texId);    //绑定外观纹理
         GLES30.glActiveTexture(GLES30.GL_TEXTURE1);//启用1号纹理
         GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, texIdNormal);      //绑定法向量纹理  
         
         GLES30.glUniform1i(uTexHandle, 0);//通过引用指定外观纹理
         GLES30.glUniform1i(uNormalTexHandle, 1);    //通过引用指定法向量纹理
         GLES30.glDrawArrays(GLES30.GL_TRIANGLES, 0, vCount);      //绘制加载的物体 
    }
    //绘制普通物体
    public void drawSelfN(int texId)
    {        
    	 //指定使用某套着色器程序
    	 GLES30.glUseProgram(mProgram);
         //将最终变换矩阵传入渲染管线
         GLES30.glUniformMatrix4fv(muMVPMatrixHandle, 1, false, MatrixState.getFinalMatrix(), 0); 
         //将位置、旋转变换矩阵传入渲染管线
         GLES30.glUniformMatrix4fv(muMMatrixHandle, 1, false, MatrixState.getMMatrix(), 0);   
         //将光源位置传入渲染管线 
         GLES30.glUniform3fv(maLightLocationHandle, 1, MatrixState.lightPositionFB);
         //将摄像机位置传入渲染管线 
         GLES30.glUniform3fv(maCameraHandle, 1, MatrixState.cameraFB);
         //将顶点位置数据传入渲染管线
         GLES30.glVertexAttribPointer  
         (
         		maPositionHandle,   
         		3, 
         		GLES30.GL_FLOAT, 
         		false,
                3*4,   
                mVertexBuffer
         );       
         //将顶点法向量数据传入渲染管线
         GLES30.glVertexAttribPointer  
         (
        		maNormalHandle, 
         		3,   
         		GLES30.GL_FLOAT, 
         		false,
                3*4,   
                mNormalBuffer
         ); 
         //将顶点纹理坐标数据传入渲染管线
         GLES30.glVertexAttribPointer  
         (
        		maTexCoorHandle, 
         		2, 
         		GLES30.GL_FLOAT, 
         		false,
                2*4,   
                mTexCoorBuffer
         );   
         //允许顶点位置、法向量、纹理坐标数据数组
         GLES30.glEnableVertexAttribArray(maPositionHandle);  
         GLES30.glEnableVertexAttribArray(maNormalHandle);
         GLES30.glEnableVertexAttribArray(maTexCoorHandle);  
         //绑定纹理  
         GLES30.glActiveTexture(GLES30.GL_TEXTURE0);
         GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, texId);    
         //绘制加载的物体
         GLES30.glDrawArrays(GLES30.GL_TRIANGLES, 0, vCount);   
    }
}
