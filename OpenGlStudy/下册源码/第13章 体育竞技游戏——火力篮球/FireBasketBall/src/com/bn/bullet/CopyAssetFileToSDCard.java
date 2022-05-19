package com.bn.bullet;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;

import com.bn.bullet7111.R;

import android.R.string;
import android.content.Context;
import android.content.res.AssetManager;
import android.os.Environment;
import android.util.Log;



public class CopyAssetFileToSDCard {
	private Context context;
	public  CopyAssetFileToSDCard(Context context)
	{
		this.context = context;
	}
	public  void copy(String assetFilePath,String SDcardFilePath,String copyFileName) throws IOException {
		//1、先判断sdcard是否挂载，然后在卡上建立需要的文件夹及文件
		File sdcardDir = Environment.getExternalStorageDirectory();
		File data = null;
		if(Environment.MEDIA_MOUNTED.equals(Environment.getExternalStorageState()))
		{
			//得到一个路径，内容是sdcard的文件夹路径和名字
			String path = sdcardDir.getPath()+SDcardFilePath;//得到SDCard/profitData的路径
			File folder = new File(path);//创建文件夹
			data = new File(path,copyFileName);//创建xml文件
			if(!folder.exists())
			{
				//若不存在，创建目录，可以在应用启动的时候创建
				folder.mkdirs();
			}
			if(!data.exists())
			{
				data.createNewFile();
			}
		}
		
		//2、以流的方法从项目里取得文件		
		InputStream myInput;
		OutputStream myOutput = new FileOutputStream(data);
		myInput = context.getAssets().open(assetFilePath);
		byte[] buffer = new byte[1024];
		int length = myInput.read(buffer);
		while(length > 0)
		{
			myOutput.write(buffer,0,length);
			length = myInput.read(buffer);
		}
		myOutput.flush();
		myInput.close();
		myOutput.close();
	}
}
