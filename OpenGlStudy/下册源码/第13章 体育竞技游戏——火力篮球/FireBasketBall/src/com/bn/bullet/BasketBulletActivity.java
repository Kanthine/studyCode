package com.bn.bullet;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.util.HashMap;

import com.bn.bullet.GL2JNIView;
import com.bn.bullet7111.R;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Context;
import android.content.pm.ActivityInfo;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.SoundPool;
import android.opengl.GLES30;
import android.opengl.GLSurfaceView;
import android.opengl.GLUtils;
import android.os.Bundle;
import android.os.Environment;
import android.util.DisplayMetrics;
import android.view.KeyEvent;
import android.view.Window;
import android.view.WindowManager;

public class BasketBulletActivity extends Activity {
	GL2JNIView mView;
	// 屏幕对应的宽度和高度
	static float WIDTH;
	static float HEIGHT;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		this.requestWindowFeature(Window.FEATURE_NO_TITLE);// 去掉标题
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
				WindowManager.LayoutParams.FLAG_FULLSCREEN);// 去掉标头
		// this.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);//强制横屏
		this.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);// 强制竖屏
		// 获得系统的宽度以及高度
		DisplayMetrics dm = new DisplayMetrics();
		getWindowManager().getDefaultDisplay().getMetrics(dm);
		if (dm.widthPixels > dm.heightPixels) {
			WIDTH = dm.heightPixels;
			HEIGHT = dm.widthPixels;
		} else {
			WIDTH = dm.widthPixels;
			HEIGHT = dm.heightPixels;
		}
		mView = new GL2JNIView(this);
		GL2JNIView.hba = this;
		mView.requestFocus(); // 获取焦点
		mView.setFocusableInTouchMode(true); // 设置为可触控
		setContentView(mView);

		JNIPort.sendWidthAndHeight(WIDTH, HEIGHT);
		JNIPort.nativeSetAssetManager(this.getAssets());// 初始化

		chushihuaSounds();
		JNIPort.hba = this;
		
		//1、先判断sdcard是否挂载，然后在卡上建立需要的文件夹及文件
		File sdcardDir = Environment.getExternalStorageDirectory();
		//获取SDCard路径
		String SDCardPath = sdcardDir.getPath();
		//发送SDcard路径
		JNIPort.sendSDCardPath(SDCardPath);
		
		CopyAssetFileToSDCard cztsd = new CopyAssetFileToSDCard(this);
		try {
			cztsd.copy("obj/wang.obj", Constant.SDCardPath,"wang.obj");
			cztsd.copy("obj/lankuang.obj", Constant.SDCardPath,"lankuang.obj");
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
//		JNIPort.setJNIEnv();
	}

	MediaPlayer beijingyinyue;// 游戏背景音乐播放器
	SoundPool shengyinChi;// 声音池
	HashMap<Integer, Integer> soundIdMap;// 声音池中声音ID与自定义声音ID的Map

	// 创建声音的方法
	@SuppressLint("UseSparseArrays")
	public void chushihuaSounds() {
		beijingyinyue = MediaPlayer.create(this, R.raw.beijingyingyu);
		beijingyinyue.setLooping(true);// 是否循环
		beijingyinyue.setVolume(0.2f, 0.2f);// 声音大小
		shengyinChi = new SoundPool(4, AudioManager.STREAM_MUSIC, 100);
		soundIdMap = new HashMap<Integer, Integer>();
		soundIdMap = new HashMap<Integer, Integer>();
		soundIdMap.put(1, shengyinChi.load(this, R.raw.pengzhuang, 1));// 碰撞声音
		soundIdMap.put(2, shengyinChi.load(this, R.raw.levelend, 1));// 游戏时间结束声音
		soundIdMap.put(3, shengyinChi.load(this, R.raw.shoot, 1));// 进球声音
	}

	// 播放声音的方法
	public int shengyinBoFang(int sound, int loop) {
		AudioManager mgr = (AudioManager) this
				.getSystemService(Context.AUDIO_SERVICE);
		float streamVolumeCurrent = mgr
				.getStreamVolume(AudioManager.STREAM_MUSIC);
		float streamVolumeMax = mgr
				.getStreamMaxVolume(AudioManager.STREAM_MUSIC);
		float volume = streamVolumeCurrent / streamVolumeMax;
		shengyinChi.play(soundIdMap.get(sound), volume, volume, 1, loop, 0.5f);

		return 0;
	}
    static void recordCurrViewIndex(int currViewIn)
    {
    	//记录当前的view编号
    	Constant.VIEW_CUR_GL_INDEX = currViewIn;
    	System.out.println("currView = "+Constant.VIEW_CUR_GL_INDEX);
    }
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent e)
	{ 
		if(keyCode!= 4)
		{
			return false;
		}
		if(Constant.VIEW_CUR_GL_INDEX == Constant.VIEW_ABOUT_INDEX | Constant.VIEW_CUR_GL_INDEX == Constant.VIEW_GAME_INDEX
				| Constant.VIEW_CUR_GL_INDEX == Constant.VIEW_RECORD_INDEX | Constant.VIEW_CUR_GL_INDEX == Constant.VIEW_SET_INDEX 
				| Constant.MENU_CURR_MENUINDEX == Constant.MENU_GAMEVIEW_END
				){
			//如果是设置界面,关于界面，记录界面,帮助界面,游戏结束界面
			//返回到菜单界面
			System.out.println("回到菜单界面------");
			beijingyinyue.pause();
			JNIPort.toMenuView();
			return true;
		}
		if(Constant.VIEW_CUR_GL_INDEX == Constant.VIEW_MENU_INDEX){//菜单界面
			System.out.println("退出------");
			System.exit(0);
			return true;
		}
		return true;
	}	
	@Override
	protected void onPause() {
		super.onPause();
		mView.onPause();
	}

	@Override
	protected void onResume() {
		super.onResume();
		mView.onResume();
	}
}