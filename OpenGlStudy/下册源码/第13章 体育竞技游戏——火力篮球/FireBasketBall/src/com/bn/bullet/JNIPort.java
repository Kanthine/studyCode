package com.bn.bullet;

import java.io.IOException;
import java.io.InputStream;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.GLES30;
import android.opengl.GLSurfaceView;
import android.opengl.GLUtils;

import java.lang.Integer;
import java.lang.String;;
public class JNIPort 
{
    static 
    {
        System.loadLibrary("BNbullet");
    }
    public static BasketBulletActivity hba;
//	public static native float calJNI();
    public static native void step();
    public static native void onSurfaceChanged(int width,int height);
    public static native void onSurfaceCreated(GLSurfaceView gsv);
    public static native void nativeSetAssetManager(AssetManager am);
    
    public static native void sendWidthAndHeight(float width,float height);
    //触摸回调方法
    public static native void onTouchBegan(float touchX,float touchY);
    public static native void onTouchMoved(float touchX,float touchY);
    public static native void onTouchEnded(float touchX,float touchY);
    
    public static native void toMenuView();
    public static native void sendSDCardPath(String SDCardPathin);
    public static native void setJNIEnv();
    static void NDKCallExit()
    {   
    	System.out.println("EXIT SUCCESS!!!");
    	System.exit(0);
    }
    //播放声音
    static void shengyinBoFang(int sound,int loop)
    {
    	System.out.println("shengyinBoFang!!!"+sound+"::"+loop);
    	hba.shengyinBoFang(sound, loop);
    }
    static void playBackMusic(int flag)
    {
    	System.out.println("playBackMusic = flag = " + flag);
    	if(flag != 0){
    		hba.beijingyinyue.start();
    	}
    }
    static void recordCurrViewIndex(int currViewIn)
    {
    	//记录当前的view编号
    	Constant.VIEW_CUR_GL_INDEX = currViewIn;
    	System.out.println("currView = "+Constant.VIEW_CUR_GL_INDEX);
    }
}
