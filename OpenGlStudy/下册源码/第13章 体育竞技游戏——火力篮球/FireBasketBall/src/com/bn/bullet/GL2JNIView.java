package com.bn.bullet;

import java.io.IOException;
import java.io.InputStream;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.GLES30;
import android.opengl.GLSurfaceView;
import android.opengl.GLUtils;
import android.opengl.Matrix;
import android.view.MotionEvent;

import static com.bn.bullet.BasketBulletActivity.*;

@SuppressLint({ "NewApi", "ClickableViewAccessibility" })
public class GL2JNIView extends GLSurfaceView
{
	SceneRenderer mRenderer;
	public GL2JNIView(Context context)
	{
		super(context);
		this.setEGLContextClientVersion(3);
		mRenderer=new SceneRenderer();
		this.setRenderer(mRenderer);
		this.setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
	}
	private class SceneRenderer implements GLSurfaceView.Renderer
	{
		int atemp =0;
		public void onDrawFrame(GL10 gl)
		{
			if(atemp<4)
			{
				System.out.println("onDrawFrame = "+atemp++);
			}			
			JNIPort.step();
		}
		public void onSurfaceChanged(GL10 gl, int width, int height)
		{
			System.out.println("onSurfaceChanged = "+atemp++);
			JNIPort.onSurfaceChanged(width, height);
		}
		public void onSurfaceCreated(GL10 gl, EGLConfig config)
		{
	        JNIPort.onSurfaceCreated(GL2JNIView.this);
		}
	}
	@Override
	public boolean onTouchEvent(MotionEvent event)
	{
		float touchX =event.getX();
		float touchY =event.getY();
		
		switch(event.getAction())
		{
		case MotionEvent.ACTION_DOWN:
			JNIPort.onTouchBegan(touchX,touchY);
			break;
		case MotionEvent.ACTION_UP:
			JNIPort.onTouchEnded(touchX,touchY);
			break;
		case MotionEvent.ACTION_MOVE:
			JNIPort.onTouchMoved(touchX,touchY);
			break;
		}
		return true;
	}
	//加载纹理的方法
	public static int initTextureRepeat(GLSurfaceView gsv,String pname)
	{
		//生成纹理ID
		int[] textures = new int[1];
		GLES30.glGenTextures
		(
				1,          //产生的纹理id的数量
				textures,   //纹理id的数组
				0           //偏移量
		);
		int textureId = textures[0];
		GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, textureId);
					
		GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_MIN_FILTER,GLES30.GL_NEAREST);
		GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D,GLES30.GL_TEXTURE_MAG_FILTER,GLES30.GL_LINEAR);
		GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_S,GLES30.GL_REPEAT);
		GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_T,GLES30.GL_REPEAT);
		
           
        //通过输入流加载图片===============begin===================
		InputStream is = null;
		try {
			is = gsv.getResources().getAssets().open(pname);
		} catch (IOException e1) {
			e1.printStackTrace();
		}
        Bitmap bitmapTmp;
        try {
        	bitmapTmp = BitmapFactory.decodeStream(is);
        } 
        finally {
            try {
                is.close();
            } 
            catch(IOException e) {
                e.printStackTrace();
            }
        }
        //实际加载纹理
        GLUtils.texImage2D
        (
        		GLES30.GL_TEXTURE_2D,   //纹理类型，在OpenGL ES中必须为GL10.GL_TEXTURE_2D
        		0, 					  	//纹理的层次，0表示基本图像层，可以理解为直接贴图
        		bitmapTmp, 			  	//纹理图像
        		0					  	//纹理边框尺寸
        );
        bitmapTmp.recycle(); 		  	//纹理加载成功后释放图片 
        return textureId;
	}
	static BasketBulletActivity hba;
    //播放声音
    static void shengyinBoFang(int sound,int loop)
    {
    	hba.shengyinBoFang(sound, loop);
    }
    static void recordCurrViewIndex(int currViewIn)
    {
    	//记录当前的view编号
    	Constant.VIEW_CUR_GL_INDEX = currViewIn;
    }
}