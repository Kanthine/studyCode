package com.bn.ms3d.io;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
public class BitmapReader{	
	public static Bitmap load(String fileName,Resources res){
		Bitmap bitmap = null;
		try{
			bitmap = BitmapFactory.decodeStream(res.getAssets().open(fileName));
		} 
		catch (Exception e){
			throw new RuntimeException(fileName + " not found!");
		}
		return bitmap;  
	}}
