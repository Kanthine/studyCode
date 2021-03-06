package com.bn.Sample11_6;//声明包

import java.io.ByteArrayOutputStream;
import java.io.InputStream;
import android.content.res.Resources;
import android.opengl.GLES31;
import android.util.Log;

//加载着色器的工具类
public class ShaderUtil 
{
   //加载指定shader的方法
   public static int loadShader
   (
		 int shaderType, //shader的类型  GLES31.GL_VERTEX_SHADER(顶点)   GLES31.GL_FRAGMENT_SHADER(片元)   GLES31.GL_COMPUTE_SHADER(计算)
		 String source   //shader的脚本字符串
   ) 
   {
	    //创建一个新shader
        int shader = GLES31.glCreateShader(shaderType);
        //若创建成功则加载shader
        if (shader != 0) 
        {
        	//加载shader的源代码
        	GLES31.glShaderSource(shader, source);
            //编译shader
        	GLES31.glCompileShader(shader);
            //存放编译成功shader数量的数组
            int[] compiled = new int[1];
            //获取Shader的编译情况
            GLES31.glGetShaderiv(shader, GLES31.GL_COMPILE_STATUS, compiled, 0);
            if (compiled[0] == 0) 
            {//若编译失败则显示错误日志并删除此shader
                Log.e("ES31_ERROR", "Could not compile shader " + shaderType + ":");
                Log.e("ES31_ERROR", GLES31.glGetShaderInfoLog(shader));
                GLES31.glDeleteShader(shader);
                shader = 0;      
            }  
        }
        return shader;
    }
   
   //创建计算着色器程序的方法
   public static int createComputeProgram(String source) 
   {
	    //加载计算着色器
        int computeShader = loadShader(GLES31.GL_COMPUTE_SHADER, source);
        if (computeShader == 0) 
        {//计算着色器加载失败时
            return 0;//返回0
        }

        //创建程序
        int program = GLES31.glCreateProgram();
        //若程序创建成功则向程序中加入计算着色器
        if (program != 0) 
        {
        	//向程序中加入计算着色器
            GLES31.glAttachShader(program, computeShader);
            checkGlError("glAttachShader");
            //链接程序
            GLES31.glLinkProgram(program);
            //存放链接成功program数量的数组
            int[] linkStatus = new int[1];
            //获取program的链接情况
            GLES31.glGetProgramiv(program, GLES31.GL_LINK_STATUS, linkStatus, 0);
            //若链接失败则报错并删除程序
            if (linkStatus[0] != GLES31.GL_TRUE) 
            {
                Log.e("ES31_ERROR", "Could not link program: ");
                Log.e("ES31_ERROR", GLES31.glGetProgramInfoLog(program));
                GLES31.glDeleteProgram(program);//删除程序
                program = 0;
            }
        }
        return program;//返回结果
    }
    
   //创建绘制shader程序的方法
   public static int createRenderProgram(String vertexSource, String fragmentSource) 
   {
	    //加载顶点着色器
        int vertexShader = loadShader(GLES31.GL_VERTEX_SHADER, vertexSource);
        if (vertexShader == 0) 
        {
            return 0;
        }
        
        //加载片元着色器
        int pixelShader = loadShader(GLES31.GL_FRAGMENT_SHADER, fragmentSource);
        if (pixelShader == 0) 
        {
            return 0;
        }

        //创建程序
        int program = GLES31.glCreateProgram();
        //若程序创建成功则向程序中加入顶点着色器与片元着色器
        if (program != 0) 
        {
        	//向程序中加入顶点着色器
            GLES31.glAttachShader(program, vertexShader);
            checkGlError("glAttachShader");
            //向程序中加入片元着色器
            GLES31.glAttachShader(program, pixelShader);
            checkGlError("glAttachShader");
            //链接程序
            GLES31.glLinkProgram(program);
            //存放链接成功program数量的数组
            int[] linkStatus = new int[1];
            //获取program的链接情况
            GLES31.glGetProgramiv(program, GLES31.GL_LINK_STATUS, linkStatus, 0);
            //若链接失败则报错并删除程序
            if (linkStatus[0] != GLES31.GL_TRUE) 
            {
                Log.e("ES31_ERROR", "Could not link program: ");
                Log.e("ES31_ERROR", GLES31.glGetProgramInfoLog(program));
                GLES31.glDeleteProgram(program);
                program = 0;
            }
        }
        return program;
    }
    
   //检查每一步操作是否有错误的方法
   public static void checkGlError(String op) 
   {
        int error;
        while ((error = GLES31.glGetError()) != GLES31.GL_NO_ERROR) 
        {
            Log.e("ES31_ERROR", op + ": glError " + error);
            throw new RuntimeException(op + ": glError " + error);
        }
   }
   
   //从sh脚本中加载shader内容的方法
   public static String loadFromAssetsFile(String fname,Resources r)
   {
   	String result=null;    	
   	try
   	{
   		InputStream in=r.getAssets().open(fname);
			int ch=0;
		    ByteArrayOutputStream baos = new ByteArrayOutputStream();
		    while((ch=in.read())!=-1)
		    {
		      	baos.write(ch);
		    }      
		    byte[] buff=baos.toByteArray();
		    baos.close();
		    in.close();
   		result=new String(buff,"UTF-8"); 
   		result=result.replaceAll("\\r\\n","\n");
   	}
   	catch(Exception e)
   	{
   		e.printStackTrace();
   	}    	
   	return result;
   }
}
