package com.bn.addRigidBody;

import javax.vecmath.Vector3f;
import com.bn.MatrixState.MatrixState3D;
import com.bn.catcherFun.MySurfaceView;
import com.bn.object.BNAbstractDoll;
import com.bn.object.LoadedObjectVertexNormalTexture;
import com.bn.util.RigidBodyHelper;
import com.bulletphysics.collision.shapes.BoxShape;
import com.bulletphysics.collision.shapes.CollisionShape;
import com.bulletphysics.collision.shapes.CompoundShape;
import com.bulletphysics.dynamics.DiscreteDynamicsWorld;
import com.bulletphysics.linearmath.Transform;
import static com.bn.constant.SourceConstant.*;
public class Niu extends BNAbstractDoll{
	int texId;
	DiscreteDynamicsWorld dynamicsWorld;
	Vector3f position;
//	LoadedObjectVertexNormalTexture lovo;
	CollisionShape[] niushape=new CollisionShape[5];
	CollisionShape niujn;

	
	
	//这是将刚体位置画出来的一个长方体类
	MySurfaceView mv;
	public Niu(int texId,DiscreteDynamicsWorld dynamicsWorld,LoadedObjectVertexNormalTexture lovo,
			Vector3f  position,int bianhao)
	{
		this.texId=texId;
		this.lovo=lovo;            
		this.position=position;
		this.dynamicsWorld=dynamicsWorld;   
		this.bianhao=bianhao;
		initRigidBodys();
	}
	public void initRigidBodys()
	{
		niushape[0]=new BoxShape(new Vector3f(niubodyx,niubodyy,niubodyz));//这是牛的身体
		niushape[1]=new BoxShape(new Vector3f(niufootx,niufooty,niufootz));
		niushape[2]=new BoxShape(new Vector3f(niuadd1x,niuadd1y,niuadd1z));
		niushape[3]=new BoxShape(new Vector3f(niuadd2x,niuadd2y,niuadd2z));
		
		niushape[4]=new BoxShape(new Vector3f(niuadd3x,niuadd3y,niuadd3z));
		niujn=addChild(niushape);
		RigidBodydoll=RigidBodyHelper.addRigidBody(1,niujn,position.x,position.y,position.z,dynamicsWorld,false);
	}
	//胶囊的组装
  	public CompoundShape addChild(CollisionShape[] shape)//组装出所需的胶囊
  	{
  		CompoundShape comShape=new CompoundShape(); //创建组合形状
  		Transform localTransform = new Transform();//创建变换对象
  		
  		
  		localTransform.setIdentity();//初始化变换
  		localTransform.origin.set(new Vector3f(0,niubodyy+niufooty*2,0));//设置变换的起点
  		comShape.addChildShape(localTransform, shape[0]);//添加子形状----胶囊
  		
  		localTransform.setIdentity();//初始化变换
  		localTransform.origin.set(new Vector3f(0,niubodyy*2+niufooty*2,0));//设置变换的起点
  		comShape.addChildShape(localTransform, shape[2]);//添加子形状----胶囊
  		
  		localTransform.setIdentity();//初始化变换
  		localTransform.origin.set(new Vector3f(0,niubodyy+niufooty*2,niubodyz));//设置变换的起点
  		comShape.addChildShape(localTransform, shape[3]);//添加子形状----胶囊
  		
  		localTransform.setIdentity();//初始化变换
  		localTransform.origin.set(new Vector3f(0,niubodyy+niufooty*2,-niubodyz));//设置变换的起点
  		comShape.addChildShape(localTransform, shape[3]);//添加子形状----胶囊
  		
  		localTransform.setIdentity();//初始化变换
  		localTransform.origin.set(new Vector3f(niubodyx,niuadd3y+niufooty*2,0));//设置变换的起点
  		comShape.addChildShape(localTransform, shape[4]);//添加子形状----胶囊
  		
  		localTransform.setIdentity();//初始化变换
  		localTransform.origin.set(new Vector3f(niubodyx-niufootx*2,niufooty,niubodyz-niufootz));//设置变换的起点
  		comShape.addChildShape(localTransform, shape[1]);//添加子形状----胶囊
  		
  		localTransform.setIdentity();//初始化变换
  		localTransform.origin.set(new Vector3f(niubodyx-niufootx*2,niufooty,-niubodyz+niufootz));//设置变换的起点
  		comShape.addChildShape(localTransform, shape[1]);//添加子形状----胶囊
  		
  		localTransform.setIdentity();//初始化变换
  		localTransform.origin.set(new Vector3f(-niubodyx+niufootx*2,niufooty,-niubodyz+niufootz));//设置变换的起点
  		comShape.addChildShape(localTransform, shape[1]);//添加子形状----胶囊
  		
  		localTransform.setIdentity();//初始化变换
  		localTransform.origin.set(new Vector3f(-niubodyx+niufootx*2,niufooty,niubodyz-niufootz));//设置变换的起点
  		comShape.addChildShape(localTransform, shape[1]);//添加子形状----胶囊
  		return comShape;
  	}
  	public void drawSelf()
  	{
  		
	  		MatrixState3D.pushMatrix();
			Transform trans=RigidBodydoll.getMotionState().getWorldTransform(new Transform());//获取这个物体的变换信息对象
			MatrixState3D.translate(trans.origin.x,trans.origin.y-speed, trans.origin.z);//进行移位变换
			trans.getOpenGLMatrix(MatrixState3D.getMMatrix());
			
			MatrixState3D.pushMatrix();
			MatrixState3D.scale(niubz,niubz,niubz);
			lovo.drawSelf(texId);
			MatrixState3D.popMatrix();
			MatrixState3D.popMatrix();
  	
  	}
}
