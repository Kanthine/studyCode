package com.bn.Sample1_3;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import android.opengl.GLES30;

//UBO
public class LoadedObjectVertexNormalTexture
{	
	static final int BYTES_PER_FLOAT=4;//每个浮点数的字节数
	
	int mProgram;//自定义渲染管线着色器程序id  
    int muMVPMatrixHandle;//总变换矩阵引用
    int muMMatrixHandle;//基本变换矩阵
    int maPositionHandle; //顶点位置属性引用  
    int maNormalHandle; //顶点法向量属性引用  
    int maTexCoorHandle; //顶点纹理坐标属性引用  
      
    int uboHandle;//一致块缓冲对象id    
    int blockIndex;//一致块的索引
    
    String mVertexShader;//顶点着色器代码脚本    	 
    String mFragmentShader;//片元着色器代码脚本    
    
    FloatBuffer   mVertexBuffer;//顶点坐标数据缓冲
	FloatBuffer   mNormalBuffer;//顶点法向量数据缓冲
	FloatBuffer  mTexCoorBuffer;//顶点纹理数据缓冲
    
    int vCount=0;  
    
    public LoadedObjectVertexNormalTexture(MySurfaceView mv,float[] vertices,float[] normals,float texCoors[])
    {  
    	//初始化顶点数据的方法
    	initVertexData(vertices,normals,texCoors); 
    	//初始化着色器的方法        
    	initShader(mv);    	
    }
    
    //初始化顶点数据的方法
    public void initVertexData(float[] vertices,float[] normals,float texCoors[])
    {
    	vCount=vertices.length/3;   
        //创建顶点坐标数据缓冲
        ByteBuffer vbb = ByteBuffer.allocateDirect(vertices.length*4);
        vbb.order(ByteOrder.nativeOrder());//设置字节顺序
        mVertexBuffer = vbb.asFloatBuffer();//转换为Float型缓冲
        mVertexBuffer.put(vertices);//向缓冲区中放入顶点坐标数据
        mVertexBuffer.position(0);//设置缓冲区起始位置
     
        ByteBuffer cbb = ByteBuffer.allocateDirect(normals.length*4);
        cbb.order(ByteOrder.nativeOrder());//设置字节顺序
        mNormalBuffer = cbb.asFloatBuffer();//转换为Float型缓冲
        mNormalBuffer.put(normals);//向缓冲区中放入顶点法向量数据
        mNormalBuffer.position(0);//设置缓冲区起始位置

        ByteBuffer tbb = ByteBuffer.allocateDirect(texCoors.length*4);
        tbb.order(ByteOrder.nativeOrder());//设置字节顺序
        mTexCoorBuffer = tbb.asFloatBuffer();//转换为Float型缓冲
        mTexCoorBuffer.put(texCoors);//向缓冲区中放入顶点纹理坐标数据
        mTexCoorBuffer.position(0);//设置缓冲区起始位置
    }
    
    //初始化一致缓冲
    public void initUBO()
    {
        //获取一致块的索引
        blockIndex=GLES30.glGetUniformBlockIndex(mProgram, "MyDataBlock"); 
        //获取一致块的尺寸
        int[] blockSizes=new int[1];        
        GLES30.glGetActiveUniformBlockiv(mProgram, blockIndex, GLES30.GL_UNIFORM_BLOCK_DATA_SIZE, blockSizes, 0);
        int blockSize=blockSizes[0];           
        //声明一致块内的成员名称数组
        String[] names={"MyDataBlock.uLightLocation","MyDataBlock.uCamera"};
        //声明对应的成员索引数组
        int[] uIndices=new int[names.length];
        //获取一致块内的成员索引
        GLES30.glGetUniformIndices(mProgram, names, uIndices, 0);
        //获取一致块内的成员偏移量        
        int[] offset=new int[names.length];
        GLES30.glGetActiveUniformsiv(mProgram, 2,uIndices,0, GLES30.GL_UNIFORM_OFFSET, offset,0);
        //用于存储一致缓冲对象编号的数组
        int[] uboHandles=new int[1];  
        //创建一致缓冲对象
        GLES30.glGenBuffers(1, uboHandles, 0);
        //获取一致缓冲对象编号
        uboHandle=uboHandles[0];        
        //将一致缓冲对象绑定到一致块  
        GLES30.glBindBufferBase(GLES30.GL_UNIFORM_BUFFER,blockIndex,uboHandle);
        //开辟存放一致缓冲所需数据的内存缓冲
        ByteBuffer ubb = ByteBuffer.allocateDirect(blockSize);
        ubb.order(ByteOrder.nativeOrder());//设置字节顺序
        FloatBuffer  uBlockBuffer = ubb.asFloatBuffer();     //转换为Float型缓冲
        //将光源位置数据送入内存缓冲
   	    float[] data=MatrixState.lightLocation;
   	    uBlockBuffer.position(offset[0]/BYTES_PER_FLOAT);
        uBlockBuffer.put(data);
        //将摄像机位置数据送入内存缓冲
        float[] data1=MatrixState.cameraLocation;
        uBlockBuffer.position(offset[1]/BYTES_PER_FLOAT);
        uBlockBuffer.put(data1);
        //设置缓冲起始偏移量
        uBlockBuffer.position(0);   
        //将光源位置、摄像机位置总数据内存缓冲中的数据送入一致缓冲               
        GLES30.glBufferData(GLES30.GL_UNIFORM_BUFFER,blockSize,uBlockBuffer,GLES30.GL_DYNAMIC_DRAW);  
    }
    
    //初始化着色器
    public void initShader(MySurfaceView mv)
    {
    	//加载顶点着色器的脚本内容
        mVertexShader=ShaderUtil.loadFromAssetsFile("vertex.sh", mv.getResources());
        //加载片元着色器的脚本内容
        mFragmentShader=ShaderUtil.loadFromAssetsFile("frag.sh", mv.getResources());  
        //基于顶点着色器与片元着色器创建程序
        mProgram = ShaderUtil.createProgram(mVertexShader, mFragmentShader);
        //获取程序中顶点位置属性引用  
        maPositionHandle = GLES30.glGetAttribLocation(mProgram, "aPosition");
        //获取程序中顶点颜色属性引用  
        maNormalHandle= GLES30.glGetAttribLocation(mProgram, "aNormal");
        //获取程序中顶点纹理坐标属性引用  
        maTexCoorHandle= GLES30.glGetAttribLocation(mProgram, "aTexCoor"); 
        //获取总变换矩阵引用
        muMVPMatrixHandle=GLES30.glGetUniformLocation(mProgram, "uMVPMatrix");
        //获取基本变换矩阵引用
        muMMatrixHandle=GLES30.glGetUniformLocation(mProgram, "uMMatrix");
        //初始化一致缓冲
        initUBO();
    }
    
    public void drawSelf(int texId)
    {        
    	 //指定使用某套着色器程序
    	 GLES30.glUseProgram(mProgram);

         //将总变换矩阵传入渲染管线 
         GLES30.glUniformMatrix4fv(muMVPMatrixHandle, 1, false, MatrixState.getFinalMatrix(), 0);  
         //将位置、旋转变换矩阵传入渲染管线
         GLES30.glUniformMatrix4fv(muMMatrixHandle, 1, false,MatrixState.getMMatrix(),0);
         //为一致块绑定一致缓冲
         GLES30.glBindBufferBase(GLES30.GL_UNIFORM_BUFFER,blockIndex,uboHandle);         
         GLES30.glEnableVertexAttribArray(maPositionHandle);   //启用顶点位置数据数组
         GLES30.glEnableVertexAttribArray(maNormalHandle);      //启用顶点法向量数据数组
         GLES30.glEnableVertexAttribArray(maTexCoorHandle);     //启用顶纹理坐标数据数组
         //将顶点位置数据送入渲染管线
         GLES30.glVertexAttribPointer  
         (
         		maPositionHandle,   
         		3, 
         		GLES30.GL_FLOAT, 
         		false,
                3*4,   
                mVertexBuffer
         );  
         //将顶点法向量送入渲染管线
         GLES30.glVertexAttribPointer  
         (
        		maNormalHandle, 
         		3,   
         		GLES30.GL_FLOAT, 
         		false,
                3*4,   
                mNormalBuffer
         );   
         //将顶点纹理数据送入渲染管线
         GLES30.glVertexAttribPointer  
         (
        		maTexCoorHandle, 
         		2, 
         		GLES30.GL_FLOAT, 
         		false,
                2*4,   
                mTexCoorBuffer
         );        
    
         GLES30.glActiveTexture(GLES30.GL_TEXTURE0);//激活纹理
         GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, texId);     //绑定纹理
         //绘制加载的物体
         GLES30.glDrawArrays(GLES30.GL_TRIANGLES, 0, vCount); 
    }
}
