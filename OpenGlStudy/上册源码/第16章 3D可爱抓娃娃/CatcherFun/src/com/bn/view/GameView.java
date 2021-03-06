package com.bn.view;

import static com.bn.constant.SourceConstant.*;
import java.util.ArrayList;
import java.util.List;
import javax.microedition.khronos.opengles.GL10;
import javax.vecmath.Vector3f;
import com.bn.MatrixState.MatrixState2D;
import com.bn.MatrixState.MatrixState3D;
import com.bn.addRigidBody.BoxRigidBody;
import com.bn.addRigidBody.Camera;
import com.bn.addRigidBody.Car;
import com.bn.addRigidBody.Claw;
import com.bn.addRigidBody.Doll;
import com.bn.addRigidBody.Niu;
import com.bn.addRigidBody.Parrot;
import com.bn.addRigidBody.Phone;
import com.bn.addRigidBody.Robot;
import com.bn.addRigidBody.Tv;
import com.bn.catcherFun.MainActivity;
import com.bn.catcherFun.MySurfaceView;
import com.bn.constant.Constant;
import com.bn.constant.SourceConstant;
import com.bn.object.BN2DObject;
import com.bn.object.TexFloor;
import com.bn.thread.HoleThread;
import com.bn.thread.KeyThread;
import com.bn.thread.MoneyThread;
import com.bn.thread.PhysicsThread;
import com.bn.util.DrawNumber;
import com.bn.util.RigidBodyHelper;
import com.bn.util.SliderHelper;
import com.bn.util.manager.ShaderManager;
import com.bn.util.manager.TextureManager;
import com.bulletphysics.collision.broadphase.AxisSweep3;
import com.bulletphysics.collision.dispatch.CollisionConfiguration;
import com.bulletphysics.collision.dispatch.CollisionDispatcher;
import com.bulletphysics.collision.dispatch.DefaultCollisionConfiguration;
import com.bulletphysics.collision.shapes.BoxShape;
import com.bulletphysics.collision.shapes.CapsuleShape;
import com.bulletphysics.collision.shapes.CollisionShape;
import com.bulletphysics.collision.shapes.CylinderShape;
import com.bulletphysics.collision.shapes.StaticPlaneShape;
import com.bulletphysics.dynamics.DiscreteDynamicsWorld;
import com.bulletphysics.dynamics.RigidBody;
import com.bulletphysics.dynamics.constraintsolver.SequentialImpulseConstraintSolver;
import android.annotation.SuppressLint;
import android.opengl.GLES30; 
import android.view.MotionEvent;

@SuppressLint("UseSparseArrays")
public class GameView extends BNAbstractView
{
	public PhysicsThread pt;//????????
	public MySurfaceView viewManager;
	public SliderHelper sliderhelper;
	DrawNumber score;
	public CatchSucceedView catchview;
	public DiscreteDynamicsWorld dynamicsWorld;//????????
	private final float TOUCH_SCALE_FACTOR = 180.0f/320;//????????????
	public float xAngle=0;
	private float mPreviousX;//??????????????X????
	public boolean ismoneyout=false;
	public boolean isGrab=false;//????????????????
	public boolean isGrabOver=false;//??????????????
	public boolean isSuccess=false;
	public int successId=0;
	public boolean isdown=false;
	CollisionShape boxShapeLR;
	CollisionShape boxShapefb;
	CollisionShape boxShapefloor;

	RigidBody boxRigidBodyL;
	RigidBody boxRigidBodyR;
	RigidBody boxRigidBodyF;
	RigidBody boxRigidBodyB;
	RigidBody boxRigidBodyD;
	CollisionShape planeShape;//????????????
	CollisionShape capsuleShape3;//????????
	
	CollisionShape cylinderShape0;
	CollisionShape cylinderShape1;
	CollisionShape cylinderShape2;
	CollisionShape[]  csa=new CollisionShape[3];
	
	PhysicsThread  pThread;
	KeyThread  KThread;
	MoneyThread mThread;
	
	TexFloor floor;//????????1	
	TexFloor floor1;//????????1
	int floorid;
	public Claw claw;
	Doll  pig0;
	Doll pig1;
	Niu niurg;
	public Phone phone;
	Tv tvrg;
	public BoxRigidBody holeboxrg;
	public int keyState = 0;
	List<BN2DObject> button=new ArrayList<BN2DObject>();//????BNObject????
	public List<BN2DObject> menulist=new ArrayList<BN2DObject>();//????BNObject????
	
    public boolean isMenu=false;
    boolean MGStart=false;//??????????????????
    boolean gamestart=false;
    int dance=0;
    public static boolean isdsMoney=false;//??????????????????????????
    boolean isStart=false;//????????????????????????
    boolean isSX=false;//??????????????????
    boolean isSXMoney=false;
    int danceSX=0;
    int SpecialJS=0;
    BN2DObject bn;
	public GameView(MySurfaceView viewManager)
	{
		this.viewManager=viewManager;
		initView();
	}
	@Override
	public boolean onTouchEvent(MotionEvent e)
	{
       
		float x=Constant.fromRealScreenXToStandardScreenX(e.getX());//????????????????
		float y=Constant.fromRealScreenYToStandardScreenY(e.getY());
        switch (e.getAction() & MotionEvent.ACTION_MASK) 
        {
    	case MotionEvent.ACTION_DOWN:
    		
    		if(x>menu_left&&x<menu_right&&y>menu_top&&y<menu_bottom)
    		{
    			  isMenu=true;	
    		}
    		if(!isSuccess)
    		{	
    		if(!isMenu&&!isCollection&&!ismoneyout)
    		{
		    		if(x>MGstart_TOUCH_LEFT_x&&x<MGstart_TOUCH_RIGHT_x&&
							y>MGstart_TOUCH_TOP_y&&y<MGstart_TOUCH_BOTTOM_y&&!isGrab&&!gamestart)
		    		{
		    			MGStart=true;
		    			gamestart=true;
		    			if(moneycount<1)
		    			{
		    				ismoneyout=true;	
		    				isStart=false;
		    				MGStart=false;
		    			}else
		    			{
		    				moneycount=moneycount-1;
		    				MainActivity. editor.putString("count", Integer.toString(moneycount));
		    				MainActivity.editor.commit();
		    			}
		    		}
		    		if(x>shuaxin_TOUCH_LEFT_x&&x<shuaxin_TOUCH_RIGHT_x&&
							y>shuaxin_TOUCH_TOP_y&&y<shuaxin_TOUCH_BOTTOM_y&&!gamestart)
		    		{
		    			if(moneycount<3)
		    			  {
		    				isSX=false;
		    				isSXMoney=false;
		    			  }else
		    			  {
		    				    isSX=true;
		    				    isupdate=true;
		    					moneycount=moneycount-3;
		    					MainActivity.editor.putString("count", Integer.toString(moneycount));
			    				MainActivity.editor.commit();
		    			  }
		    		}
		    		if(isdsMoney){
		    			if(xAngle>ANGLE_MIN+10&&xAngle<ANGLE_MAX-10)
		    			{
		        			setkeyState(x,y,0x04,0x08,0x02,0x01);
		        			
		    			}
		    			if(xAngle<ANGLE_MIN+10)
		    			{
		    				xAngle=ANGLE_MIN;
		    				setkeyState(x,y,0x02,0x01,0x08,0x04);
		    				
		    				
		    			}
		    			if(xAngle>ANGLE_MAX-10)
		    			{
		    				setkeyState(x,y,0x01,0x02,0x04,0x08);
		    				
		    			}
	    		    }
    		    }
	    		if(x>=tol_left&&x<=tor_left&&y>=tou_top&&y<=tod_bottom)
	    		{
	    			isdown=true;
	    		}
    		}else
    		{
    			return catchview.onTouchEvent(e);
    		}
    		break;
    	case MotionEvent.ACTION_MOVE:    	
	    		float dx = x - mPreviousX;//??????????X????
	    		if(isCollection&&!isSuccess&&isdown)
	    		{
	    			return false;
	    			
	    		}
	    		System.out.println(keyState);
	    		if(keyState==0){
					if(Math.abs(dx)<=0.04f&&isdown){
						break;
					}
				
					xAngle -= dx*TOUCH_SCALE_FACTOR;
					
					if(xAngle<ANGLE_MIN)
					{
						xAngle=ANGLE_MIN;
			    		
					}
					else if(xAngle>ANGLE_MAX)
					{
						xAngle=ANGLE_MAX;
					}		
					calculateMainAndMirrorCamera(xAngle);
	    		}
	    		break;
    	case MotionEvent.ACTION_UP:
    		if(x>MGstart_TOUCH_LEFT_x&&x<MGstart_TOUCH_RIGHT_x&&
					y>MGstart_TOUCH_TOP_y&&y<MGstart_TOUCH_BOTTOM_y)
    		{
    			
    			MGStart=false;
    			
    		}
    		if(x>shuaxin_TOUCH_LEFT_x&&x<shuaxin_TOUCH_RIGHT_x&&
					y>shuaxin_TOUCH_TOP_y&&y<shuaxin_TOUCH_BOTTOM_y)
    		{
    			isSX=false;
    		}
    		keyState = 0;
    		break;
        }
        mPreviousX = x;//??????????????
		if(isMenu)
		{
			 return viewManager.menuview.onTouchEvent(e);
		}
    	return true;
	}
	public void setkeyState(float x,float y,int tolState,int torState,int touState,int todState)
	{
		if(x>=catch_left&&x<=catch_right&&y>=catch_top&&y<=catch_bottom)
		{
			isdown=true;
			if(!isGrab)
			 {
				  isGrab=true;
				  allcount++;//??????????
				  KThread=new KeyThread(GameView.this);
				  KThread.start();
			 }
			
		}
		if(x>=tol_left&&x<=tol_right&&y>=tol_top&&y<=tol_bottom)
		{
			
			if(!isGrab)
			{
				
				keyState = tolState;
				
			}
		}
		
		if(x>=tor_left&&x<=tor_right&&y>=tor_top&&y<=tor_bottom)
		{
			if(!isGrab)
			{
				
				keyState = torState;
			}
			
		}
		
		if(x>=tou_left&&x<=tou_right&&y>=tou_top&&y<=tou_bottom)
		{
			
			if(!isGrab)
			{
				keyState = touState;
			}
			
		}
		
		if(x>=tod_left&&x<=tod_right&&y>=tod_top&&y<=tod_bottom)
		{
			if(!isGrab)
			{
				keyState = todState;
			}
			
		}
		
	}
	@Override
	public void initView() {
		 initButton();
		 initWorld();
		 if(hb!=null){
			 HoleThread ht=new HoleThread();
			 ht.start();
		 }
		 update();
         score=new DrawNumber(this.viewManager);
         catchview=new CatchSucceedView(this.viewManager);
         pThread=new PhysicsThread(GameView.this);
         mThread=new MoneyThread(GameView.this);
         mThread.start();
         pThread.start();         
	}
	public void update()//????????
	{	
		for(int i=0;i<9;i++)
		{
			int random=(int) (Math.random()*9);
			switch(random)
			{
				case 0:
					updatedoll.add(new Niu(niuId,dynamicsWorld,niu,new Vector3f(dollinitx+i%4*spanx,1,dollinitz+i/4*spanz),0));
					break;
				case 1:
					updatedoll.add(new Doll(doll0Id,dynamicsWorld,doll0,new Vector3f(dollinitx+i%4*spanx,2,dollinitz+i/4*spanz),1));
					break;
				case 2:
	               updatedoll.add(new Phone(doll2Id,dynamicsWorld,doll2,new Vector3f(dollinitx+i%4*spanx,1.5f,dollinitz+i/4*spanz),2));
	               break;
				case 3:
				   updatedoll.add(new Parrot(parrotId,dynamicsWorld,ParrotMd,new Vector3f(dollinitx+i%4*spanx,1,dollinitz+i/4*spanz),3));
				   break;
				case 4:
					if(CollectionView.islock)
					{
					 updatedoll.add(new Tv(tvId,dynamicsWorld,tvmodle,new Vector3f(dollinitx+i%4*spanx,1,dollinitz+i/4*spanz),6));
					}else
					{
					 updatedoll.add(new Robot(robotId,dynamicsWorld,RobotMD,new Vector3f(dollinitx+i%4*spanx,1,dollinitz+i/4*spanz),6));
					}
					break;
				case 5:
					
					 updatedoll.add(new Car(CarId,dynamicsWorld,CarMD,new Vector3f(dollinitx+i%4*spanx,2,dollinitz+i/4*spanz),5));
					 break;	
				case 6:
					 updatedoll.add(new Tv(tvId,dynamicsWorld,tvmodle,new Vector3f(dollinitx+i%4*spanx,1,dollinitz+i/4*spanz),6));
					 break;	
				case 7:
					 updatedoll.add(new Doll(doll1Id,dynamicsWorld,doll1,new Vector3f(dollinitx+i%4*spanx,1.5f,dollinitz+i/4*spanz),7));
					 break;	
				case 8:
					 updatedoll.add(new Camera(CameraId,dynamicsWorld,Camera,new Vector3f(dollinitx+i%4*spanx,2,dollinitz+i/4*spanz),8));
					 break;				
			}
		}
	}
	public void initButton()
	{
		
		button.add(new BN2DObject(920, 1700, 200, 200, TextureManager.getTextures("catch.png"), 
				ShaderManager.getShader(2)));//0
		
		button.add(new BN2DObject(200, 1600, 150, 150, TextureManager.getTextures("tou.png"), 
				ShaderManager.getShader(2)));//1
		button.add(new BN2DObject(200, 1820, 150, 150, TextureManager.getTextures("tod.png"), 
				ShaderManager.getShader(2)));//2
		button.add(new BN2DObject(80, 1710, 150, 150, TextureManager.getTextures("tol.png"), 
				ShaderManager.getShader(2)));//3
		button.add(new BN2DObject(320, 1710, 150, 150, TextureManager.getTextures("tor.png"), 
				ShaderManager.getShader(2)));//4
		button.add(new BN2DObject(Box_x,Box_y,Box_SIZE_x,Box_SIZE_y,Box1Id,
				ShaderManager.getShader(2)));//5??????????????
		button.add(new BN2DObject(MGstart_x,MGstart_y,MGstart_SIZE_x,MGstart_SIZE_y,MGstartId,
				ShaderManager.getShader(2)));//6????????????????????????
		button.add(new BN2DObject(MGstart_x,MGstart_y,MGstart_SIZE_x,MGstart_SIZE_y,MGstartDownId,
				ShaderManager.getShader(2)));//7??????????????????????????????????
		button.add(new BN2DObject(Money_x,Money_y,Money_SIZE_x,Money_SIZE_y,MoneyBoxId,
				ShaderManager.getShader(2)));//8????????????????????????????????????????????
		button.add(new BN2DObject(Money_x,Money_y,Money_SIZE_x,Money_SIZE_y,MoneyId,
				ShaderManager.getShader(2)));//9??????????????????????????????????
		button.add(new BN2DObject(shuaxin_x,shuaxin_y,shuaxin_SIZE_x,shuaxin_SIZE_y,shuaxinId,
				ShaderManager.getShader(2)));//10 ????????????????????????
		button.add(new BN2DObject(shuaxin_x,shuaxin_y,shuaxin_SIZE_x,shuaxin_SIZE_y,shuaxinDownId,
				ShaderManager.getShader(2)));//11 ??????????????  ??????????????????
		button.add(new BN2DObject(1000, 110, 180, 80, TextureManager.getTextures("menu.png"), 
				ShaderManager.getShader(2)));//12
		bn=new BN2DObject(550, 900, 800, 600, TextureManager.getTextures("message.png"), 
				ShaderManager.getShader(2));
		button.add(new BN2DObject(Box2_x,Box2_y,Box2_SIZE_x,Box2_SIZE_y,Box2Id,
				ShaderManager.getShader(2)));//13????????????????????
	}
	public void initWorld()
	{
		//????????????????????????
		CollisionConfiguration collisionConfiguration = new DefaultCollisionConfiguration();		
		//????????????????????????????????????????????????????????????????????????????????????????
		CollisionDispatcher dispatcher = new CollisionDispatcher(collisionConfiguration);		
		//??????????????????????????
		Vector3f worldAabbMin = new Vector3f(-10000, -10000, -10000);
		Vector3f worldAabbMax = new Vector3f(10000, 10000, 10000);
		int maxProxies = 1024;
		//??????????????????????????????????
		AxisSweep3 overlappingPairCache =new AxisSweep3(worldAabbMin, worldAabbMax, maxProxies);
		//??????????????????????
		SequentialImpulseConstraintSolver solver = new SequentialImpulseConstraintSolver();
		//????????????????
		dynamicsWorld = new DiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver,collisionConfiguration);
		//??????????????
		dynamicsWorld.setGravity(new Vector3f(0, -10, 0));
		
		cylinderShape0=new CylinderShape(new Vector3f(2.1f,2.1f,2.1f));
		cylinderShape1=new CylinderShape(new Vector3f(0.7f,5.6f,0.7f));
		cylinderShape2=new CylinderShape(new Vector3f(2.1f,2.1f,2.1f));
		
		csa[0]=cylinderShape0;
		csa[1]=cylinderShape1;
		csa[2]=cylinderShape2;
		
		//????????????????????
		//??????????????????
		planeShape=new StaticPlaneShape(new Vector3f(0, 1, 0), 0);	
		capsuleShape3=new CapsuleShape(0.77f*2,1.5f*2);
		boxShapeLR=new BoxShape(new Vector3f(0.05f,4f,3.088f));
		boxShapefb=new BoxShape(new Vector3f(3.075f,4f,0.05f));
		boxShapefloor=new BoxShape(new Vector3f(3.075f,0.05f,3.088f));
		floor=new TexFloor(ShaderManager.getShader(0),
         		80*SourceConstant.UNIT_SIZE,0,-SourceConstant.UNIT_SIZE,0,planeShape,dynamicsWorld,0);
	
         claw = new Claw(clawId,ganId,dunId,viewManager,dynamicsWorld,bodyForDraws,ShaderManager.getShader(0));
        
         holeboxrg=new BoxRigidBody(holeboxId,dynamicsWorld,holebox,new Vector3f(holeboxx,holeboxy,holeboxz));
         sliderhelper=new SliderHelper(ganboxId,viewManager,dynamicsWorld);
         boxRigidBodyL = RigidBodyHelper.addRigidBody(0,boxShapeLR, -2.7f,4,13,dynamicsWorld,false);
         boxRigidBodyR = RigidBodyHelper.addRigidBody(0,boxShapeLR,2.7f,4,13,dynamicsWorld,false);      
         boxRigidBodyF=RigidBodyHelper.addRigidBody(0,boxShapefb,0f,3,10f,dynamicsWorld,false);
         boxRigidBodyB=RigidBodyHelper.addRigidBody(0,boxShapefb,0f,3,16.2f,dynamicsWorld,false);
         boxRigidBodyD=RigidBodyHelper.addRigidBody(0,boxShapefloor,0f,0.5f,13f,dynamicsWorld,false);
       
	}
	@Override
	public void drawView(GL10 gl) {
		 //??????????????????????
		GLES30.glClear(GL10.GL_COLOR_BUFFER_BIT | GL10.GL_DEPTH_BUFFER_BIT);   
		if(!isCollection)
		{
         MatrixState3D.setCamera( 
         		EYE_X,   //??????????X
         		EYE_Y, 	//??????????Y
         		EYE_Z,   //??????????Z
         		TARGET_X, 	//????????????X
         		TARGET_Y,   //????????????Y
         		TARGET_Z,   //????????????Z
         		0, 
         		1, 
         		0);
		}
         GLES30.glEnable(GLES30.GL_DEPTH_TEST);
      
      
         //????????
         MatrixState3D.pushMatrix();
         floor.drawSelf(floorTextureId);
         MatrixState3D.popMatrix();       
         MatrixState3D.pushMatrix();
         claw.drawSelf();
         MatrixState3D.popMatrix();
         MatrixState3D.pushMatrix();
         sliderhelper.drawSelf();
         MatrixState3D.popMatrix();
         

         for(int i=0;i<updatedoll.size();i++)
         {
        	 updatedoll.get(i).drawSelf();
         }
            
         
         holeboxrg.drawSelf();
         drawdoll();
         GLES30.glDisable(GLES30.GL_DEPTH_TEST);
         
         draw2DObject();
       
         
         GLES30.glDisable(GLES30.GL_DEPTH_TEST);  
         if(isupdate){//????????????
        	 SpecialJS=2;
         }
         if(SpecialJS>1){
             if(SpecialJS>30){
            	 Special.drawSpecial(5);//????????????????
            	 if(SpecialJS>130){//??????????
                	 SpecialJS=0;
            	 }
             }
             SpecialJS++;
         }

         if(isSuccess)
         {
           catchview.drawView(successId);
           Special.drawSpecial(2);//????????????????
         }
         GLES30.glEnable(GLES30.GL_DEPTH_TEST); 
       
	}
	public void drawdoll()
	{        
   
	      MatrixState3D.pushMatrix();
		  MatrixState3D.translate(0,-4,13);
		  dollbox.drawSelf(dollboxId);
	      MatrixState3D.popMatrix();
	      
	      
	      MatrixState3D.pushMatrix();
		  MatrixState3D.translate(0, 1, 10);
		  hole.drawSelf(holeId);
	      MatrixState3D.popMatrix();
	      
	      GLES30.glEnable(GLES30.GL_BLEND);//????????
    	  //????????????
		  GLES30.glBlendFunc(GLES30.GL_SRC_ALPHA,GLES30.GL_ONE_MINUS_SRC_ALPHA);
          GLES30.glDisable(GL10.GL_CULL_FACE);
	      MatrixState3D.pushMatrix();
		  MatrixState3D.translate(1.4f,0.5f,14.8f);
		  hb.drawSelf(HBId);//??????????????????????
	      MatrixState3D.popMatrix();
	      GLES30.glEnable(GL10.GL_CULL_FACE);
	      GLES30.glDisable(GLES30.GL_BLEND);  
	      
	}
	public void drawMoney()
	{
		initdatax=180;
		initdatay=100;
		score.drawnumber(moneycount);
	}
   
    public void draw2DObject()
    {
    	if(ismoneyout)
		{
			bn.drawSelf();
		}
    	if(isMenu)
        {
           viewManager.menuview.drawView();
        }
    	if(!isMenu)
	    {
    	    button.get(12).drawSelf();
    	    button.get(8).drawSelf();
    	    drawMoney();
    	    drawDropMoney();
	    }
    }
    public void OneMoney()
    {
    	if(dance<70){//??????????????????????????
			dance++;
			if(dance==62){
				if(!effictOff){
    				MainActivity.sound.playMusic(SOUND_DropMoney,0);
    			}
			}
			button.get(9).drawSelf(Money_x+dance*3f, Money_y+dance*28);
		}else{
			dance++;

			if(dance>80){
				isdsMoney=true;//??????????????????????????????????????????????
			}
		}
    }
    public void SanMoney()//??????????????????????????????????????????????
    {
    	if(danceSX<70){
			danceSX++;
			if(danceSX==62){
				if(!effictOff){
    				MainActivity.sound.playMusic(SOUND_DropMoney,0);
    			}
			}
			button.get(9).drawSelf(Money_x+danceSX*3f, Money_y+danceSX*28);
			button.get(9).drawSelf(Money_x+danceSX*3f, Money_y+danceSX*30);
			button.get(9).drawSelf(Money_x+danceSX*3f, Money_y+danceSX*34);
		}else{
			danceSX++;
			if(danceSX>80){
				isSXMoney=false;
				danceSX=0;
			}
			
		}
    }
    public void drawDropMoney()
    {
    	boolean tempLeft = isleft;
    	boolean tempRight = isright;
    	boolean tempTop = istop;
    	boolean tempBottom = isbottom;
    	if(xAngle<ANGLE_MIN+10)
		{

			 tempLeft = istop;
	    	 tempRight = isbottom;
	    	 tempTop = isright;
	    	 tempBottom = isleft;
			
		}
		if(xAngle>ANGLE_MAX-10)
		{

			
			 tempLeft = isbottom;
	    	 tempRight = istop;
	    	 tempTop = isleft;
	    	 tempBottom = isright;
		}
    	if(isdsMoney)//??????????????????????????????????????????
    	{
    		MatrixState2D.pushMatrix();//???????? 
            
          	  button.get(0).drawSelf();
           if(!tempLeft)
           {
        	   button.get(3).drawSelf();
           }
           if(!tempRight)
           {
        	   button.get(4).drawSelf();
           }
           if(!tempTop)
           {
        	   button.get(1).drawSelf();
           }
           if(!tempBottom)
           {
        	   button.get(2).drawSelf();
           }
  	        MatrixState2D.popMatrix();//????????
    	}else{
    	    //?????????? ??????????????????????????????????????????????????????????
	    	button.get(13).drawSelf();//??????????????

	    	if(isStart){//??????????????
	    		OneMoney();
	    	}
	    	if(isSXMoney){//????????????????
	    		SanMoney();
	    	}
	    	button.get(5).drawSelf();//??????????????
	    	
	    	//??????????????????????????????????????
	    	if(MGStart){//????????????????????
	    		button.get(7).drawSelf();//??????????????
	    		isStart=true;//??????????????????????????????????
	    		
	    	}else{
	    		button.get(6).drawSelf();//????????
	    	}
	    	if(isSX){
    			button.get(11).drawSelf();//????????????
    			isSXMoney=true;//????????????????
	    	}else{
	    		button.get(10).drawSelf();//????????
	    	}
	    	
    	}
    }
    public void reData()
    {
    	  isMenu=false;
    	  MGStart=false;//??????????????
    	  dance=0;
    	  isdsMoney=false;//??????????????????????????
    	  isStart=false;//????????????????????????
    	  isSX=false;//??????????????????
    	  isSXMoney=false;
    	  danceSX=0;
    	  isCollection=false;
    	  isGrab=false;
    	  gamestart=false;
    	
    }
    public static void calculateMainAndMirrorCamera(float angle)
	{		
		//????????????????????????
    	
    	EYE_X = (float) (r*Math.sin(Math.toRadians(angle)));
    	EYE_Y=4f;
    	EYE_Z = (float) (r*Math.cos(Math.toRadians(angle)))+TARGET_Z;
    
	}
}
