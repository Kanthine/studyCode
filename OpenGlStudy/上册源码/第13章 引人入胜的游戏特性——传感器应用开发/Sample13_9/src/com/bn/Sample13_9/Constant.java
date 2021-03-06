package com.bn.Sample13_9;
//常量类
public class Constant 
{
	public static final float  SCALE=1;	
	
	//黄金长方形长边的一半
	public static final float AHALF=1;
	
	//给定木块凹槽的数据
	public static final float CUBE_LENGTH=18;	//矩形平面的长度
	public static final float CUBE_HEIGHT=1;	//墙的高度
	public static final float CUBE_WIDTH=12;	//矩形平面的宽度
	public static final float WALL_WIDTH=1f;	//墙的厚度
	//----------------------------数据给定结束-----------------------------------------
	
	public static float D3_CUBE_LENGTH=CUBE_LENGTH*SCALE;	//三维空间中底面矩形的真正长度
	public static float D3_CUBE_WIDTH=CUBE_WIDTH*SCALE;	//三维空间中底面矩形的真正宽度
	public static float D3_WALL_WIDTH=WALL_WIDTH*SCALE;	//三维空间中墙的真正厚度
	public static float BALLR=(float) Math.sqrt(SCALE*AHALF*SCALE*AHALF+SCALE*AHALF*0.618034f*SCALE*AHALF*0.618034f);		//球的半径
	
	//底面凹槽的边界,均为正值
	public static float XBOUNDARY=D3_CUBE_LENGTH/2-D3_WALL_WIDTH-BALLR;	//x方向上的边界
	public static float ZBOUNDARY=D3_CUBE_WIDTH/2-D3_WALL_WIDTH-BALLR;	//z方向上的边界
	
	
	public static float XOFFSET=0;	//球位置坐标
	public static float ZOFFSET=0;
	
	public static float SPANX=0;	//球的步进
	public static float SPANZ=0;
}
