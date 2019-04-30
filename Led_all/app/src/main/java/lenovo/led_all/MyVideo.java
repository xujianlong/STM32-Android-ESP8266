package lenovo.led_all;

import android.app.Activity;
import android.os.Bundle;
import android.os.StrictMode;
import android.util.DisplayMetrics;
import android.view.MotionEvent;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;

import java.io.IOException;
import java.io.OutputStream;
import java.net.Socket;
import java.net.UnknownHostException;

import static lenovo.led_all.MainActivity.IP;

public class MyVideo extends Activity
{
	private int ksangle1=0x96;   //舵机1的初始角度150度
	private int ksangle2=0x5a;	//舵机2的初始角度90度
	private int maxangle=0xb4;
	public float startX; //初始横坐标
	public float startY; //初始纵坐标
	private int  dqangle1;//当前角度
	private int  dqangle2;//当前角度
	private Button fanhui;
	private Button TakePhotos;
	private Button ViewPhotos;


	private Socket socket;
	OutputStream socketWriter ;


	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);

		this.requestWindowFeature(Window.FEATURE_NO_TITLE);//隐去标题（应用的名字必须要写在setContentView之前，否则会有异常）
		this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
		setContentView(R.layout.help);

		if (android.os.Build.VERSION.SDK_INT > 9)
		{
			StrictMode.ThreadPolicy policy = new StrictMode.ThreadPolicy.Builder().permitAll().build();
			StrictMode.setThreadPolicy(policy);
		}
		InitSocket();

	}


	//使用了Activity的ontouchEvent方法监听手指点击事件
	@Override
	public  boolean onTouchEvent( MotionEvent event)
	{
		// TODO Auto-generated method stub
		super.onTouchEvent(event);
		//detector.onTouchEvent(event);
		//setContentView(R.layout.myvideo);

		switch(event.getAction())
		{
			case MotionEvent.ACTION_DOWN://手指按下屏幕的时候回调该方法
				startY=event.getRawY();//记录开始按下时 的坐标
				startX=event.getRawX();
				dqangle1=ksangle1;
				dqangle2=ksangle2;
				break;

			case MotionEvent.ACTION_MOVE://手指在屏幕上滑动回调该方法,来到新的坐标
				DisplayMetrics dm = new DisplayMetrics(); //获取屏幕信息(宽高)
				getWindowManager().getDefaultDisplay().getMetrics(dm);
				int screenHeight = dm.heightPixels;
				int screenWidth = dm.widthPixels;
				float endY=event.getRawY();//记录此时 的坐标
				float endX=event.getRawX();//记录此时 的坐标

				float distanceY=endY-startY;//计算滑动距离
				float distanceX=endX-startX;//计算滑动距离

				int delatangle1=(int) (maxangle*distanceY/screenHeight); //改变的角度:最大角度=滑动距离：屏幕高度
				int angle1= Math.min(Math.max(delatangle1+dqangle1, 3), maxangle); //具体角度=当前角度+改变角度

				int delatangle2=(int) (maxangle*distanceX/screenWidth); //改变的角度:最大角度=滑动距离：屏幕高度
				int angle2= Math.min(Math.max(delatangle2+dqangle2, 3), maxangle); //具体角度=当前角度+改变角度

				int dqangle2=angle2;//更新角度

				try {

					socketWriter.write(new byte[]{
							(byte)0x02,(byte)0x01,(byte)0x02,(byte)(dqangle2),(byte)(delatangle1)});
					socketWriter.flush();//强制将输出流缓冲区的数据送出
				}
				catch (IOException e)
				{
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				ksangle2=dqangle2;//更新开始角度

				int dqangle1=angle1;//更新角度

				try
				{
					socketWriter.write(new byte[]{(byte)0xff,(byte)0x01,(byte)0x07,(byte)(dqangle1),(byte)0xff});
					socketWriter.flush();
				}
				catch (IOException e)
				{
					e.printStackTrace();
				}
				ksangle1=dqangle1;//更新开始角度
				break;
			//case MotionEvent.ACTION_UP://手指离开屏幕回调该方法
			//break;
		}
		return true;
	}


	public void InitSocket()
	{
		new Thread()
		{
			@Override
			public void  run()
			{
				try {
					socket = new Socket(IP,9999);
				} catch (UnknownHostException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				try {
					socketWriter= socket.getOutputStream();

				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}.start();

	}


	public void onDestroy()
	{
		super.onDestroy();
	}

}
