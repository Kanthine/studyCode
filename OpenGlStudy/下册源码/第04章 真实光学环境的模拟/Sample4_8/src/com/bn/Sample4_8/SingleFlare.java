package com.bn.Sample4_8;//声明包

public class SingleFlare {
	public int texture;//所用纹理
	public float distance;//距离（沿光源到屏幕中心直线的成比例的距离）
	public float size;//原始尺寸
	public float bSize;//变换后的尺寸
	public float[] color;//颜色数组
	public float px;//绘制位置x坐标
	public float py;//绘制位置y坐标
	public SingleFlare(int texture,float size,float distance,float[] color)
	{
		this.texture=texture;//初始化纹理
		this.distance=distance;//初始化距离值
		this.size=size;//初始化原始尺寸值
		this.bSize=size;//初始化变换后的尺寸值
		this.color=color;//初始化颜色数组
		this.px=0;//初始化绘制位置x坐标
		this.py=0;//初始化绘制位置y坐标
	}
}
