package lenovo.led_all;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.util.DisplayMetrics;
import android.view.GestureDetector;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.SeekBar;
import android.widget.Space;
import android.widget.TextView;
import android.widget.Toast;

import java.util.Timer;
import java.util.TimerTask;

public class MainActivity extends AppCompatActivity implements
        GestureDetector.OnGestureListener, View.OnClickListener {
    Button window_control, beep_control;
    TextView temperature, water;
    Button reset, up, down, left, right;
    Space gesture;
    SeekBar red, green, blue;
    Handler handler;
    ClientThread clientThread;
    int c1 = 0, c2 = 0, c3 = 0, c4 = 0, c = 0;
    static String IP = "xujianlong.iok.la";
    int DHT11 = 0, RECEIVE;
    static int value;
    String TI, TD, HI, HD;
    int ti = 0, i = 1, hi = 0;
    Timer mTimer;
    TimerTask mTimerTask;
    int mCounter = 3;
    int mTime = 1000;
    static byte red_dat = 0x01, green_dat = 0x01, blue_dat = 0x01;
    byte command_0, command_1, command_2, command_3, command_4;
    byte[] command;
    // 定义手势检测器实例
    GestureDetector detector;

    private int ksangle1 = 0x96;   //舵机1的初始角度150度
    private int ksangle2 = 0x5a;    //舵机2的初始角度90度
    private int maxangle = 0xb4;
    public float startX; //初始横坐标
    public float startY; //初始纵坐标
    private int dqangle1;//当前角度
    private int dqangle2;//当前角度
    public static boolean thread_flag = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        red = (SeekBar) findViewById(R.id.RED);
        green = (SeekBar) findViewById(R.id.GREEN);
        blue = (SeekBar) findViewById(R.id.BLUE);
        reset = (Button) findViewById(R.id.reset);
        temperature = (TextView) findViewById(R.id.temperature);
        water = (TextView) findViewById(R.id.water);
        window_control = (Button) findViewById(R.id.window_control);
        beep_control = (Button) findViewById(R.id.beep_control);

      /*  up = (Button) findViewById(R.id.up);
        down = (Button) findViewById(R.id.down);
        left = (Button) findViewById(R.id.left);
        right = (Button) findViewById(R.id.right);*/

        // 创建手势检测器
        detector = new GestureDetector(MainActivity.this, (GestureDetector.OnGestureListener) MainActivity.this);
        /*red.setOnSeekBarChangeListener(this);
        green.setOnSeekBarChangeListener(this);
        blue.setOnSeekBarChangeListener(this);*/
        window_control.setOnClickListener(this);
        beep_control.setOnClickListener(this);
        /*window_control.setOnCheckedListener(this);
        beep_control.setOnCheckedListener(this);*/

        red.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {

            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {

                command_0 = 0x04;
                red_dat = (byte) (progress + 1);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                try {
                    Message msg = new Message();
                    msg.what = 0x456;
                    command = new byte[]{command_0, red_dat, green_dat, blue_dat};
                    msg.obj = command;
                    clientThread.revHandler.sendMessage(msg);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        });

        green.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                command_0 = 0x04;
                green_dat = (byte) (progress + 1);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                try {
                    Message msg = new Message();
                    msg.what = 0x456;
                    command = new byte[]{command_0, red_dat, green_dat, blue_dat};
                    msg.obj = command;
                    clientThread.revHandler.sendMessage(msg);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        });
        blue.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                command_0 = 0x04;
                blue_dat = (byte) (progress + 1);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                try {
                    Message msg = new Message();
                    msg.what = 0x456;
                    command = new byte[]{command_0, red_dat, green_dat, blue_dat};
                    msg.obj = command;
                    clientThread.revHandler.sendMessage(msg);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        });

        reset.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Message msg = new Message();
                msg.what = 0x456;
                try {
                    command_0 = 0x04;
                    c++;
                    if ((c % 2) == 0) {
                        command_1 = 0x0;
                        command_2 = 0x0;
                        command_3 = 0x0;

                        red.setProgress(0);
                        green.setProgress(0);
                        blue.setProgress(0);
                        reset.setBackgroundResource(R.drawable.set);
                    } else {
                        command_1 = (byte) 0xff;
                        command_2 = (byte) 0xff;
                        command_3 = (byte) 0xff;

                        red.setProgress(255);
                        green.setProgress(255);
                        blue.setProgress(255);
                        reset.setBackgroundResource(R.drawable.reset);
                    }

                    command = new byte[]{command_0, command_1, command_2, command_3};
                    msg.obj = command;
                    clientThread.revHandler.sendMessage(msg);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        });


        handler = new Handler() {  //在UI线程中的
            @Override
            public void handleMessage(Message msg) {
                if (msg.what == 0x123) {

                    RECEIVE = Integer.parseInt(String.valueOf(msg.obj));
                    if ((RECEIVE / 1000) != 0) {
                        hi = RECEIVE / 10000;
                        ti = (RECEIVE % 1000) / 10;
                        temperature.setText(String.valueOf(ti) + ".0");
                        water.setText(String.valueOf(hi) + ".0");
                    }
                    switch (RECEIVE) {
                        case 16:
                            window_control.setText("手动");
                            break;
                        case 31:
                            window_control.setText("自动");
                            break;
                        case 96:
                            beep_control.setText("关");
                            break;
                        case 111:
                            beep_control.setText("开");
                            break;
                        default:
                            break;
                    }

                }
            }
        };
       clientThread = new ClientThread(handler);
        new Thread(clientThread).start();
        dingshi();
    }

    public void dingshi() {
        mTimer = new Timer();
        mTimerTask = new TimerTask() {
            @Override
            public void run() {
                Message msg = new Message();
                msg.what = 0x456;
                command_0 = 0x03;
                command_1 = 0;
                command_2 = 0;
                command_3 = 0;
                command_4 = 0;
                command = new byte[]{command_0, command_1, command_2, command_3};
                msg.obj = command;
                System.out.println("222");
                if (thread_flag) {
                    clientThread.revHandler.sendMessage(msg);
                }
                //clientThread.revHandler.sendMessage(msg);
            }
        };
        //mTime = 1000;
        mTimer.schedule(mTimerTask, mCounter * mTime, mCounter * mTime);
    }

    @Override
    public void onClick(View v) {
        Message msg = new Message();
        msg.what = 0x456;
        switch (v.getId()) {
            case R.id.window_control:
                c1++;
                if (c1 % 2 == 1) {
                    command_1 = (byte) 0x0f;
                } else {
                    command_1 = (byte) 0xf0;
                }
                try {
                    command_0 = 0x01;
                    command_2 = command_3 = command_4 = 0;
                    command = new byte[]{command_0, command_1, command_2, command_3};
                    msg.obj = command;
                    clientThread.revHandler.sendMessage(msg);
                } catch (Exception e) {
                    e.printStackTrace();
                }
                break;
            case R.id.beep_control:
                c2++;
                if (c2 % 2 == 1) {
                    command_1 = (byte) 0xf0;
                } else {
                    command_1 = 0x0f;
                }
                try {
                    command_0 = 0x06;
                    command_2 = command_3 = command_4 = 0;
                    command = new byte[]{command_0, command_1, command_2, command_3};
                    msg.obj = command;
                    clientThread.revHandler.sendMessage(msg);
                } catch (Exception e) {
                    e.printStackTrace();
                }
                break;
            default:
                break;
        }
    }

    /*
   * 以下为按一下返回键后，提示“再按一次退出程序”
   */
    private long exitTime = 0;

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (keyCode == KeyEvent.KEYCODE_BACK && event.getAction() == KeyEvent.ACTION_DOWN) {

            if ((System.currentTimeMillis() - exitTime) > 2000)
            //System.currentTimeMillis()无论何时调用，肯定大于2000
            {
                Toast.makeText(getApplicationContext(), "再按一次退出程序",
                        Toast.LENGTH_SHORT).show();
                exitTime = System.currentTimeMillis();
            } else {
                super.onDestroy();
                System.exit(0);
            }

            return true;
        }
        return super.onKeyDown(keyCode, event);
    }

   /* */

    /**
     * 设置返回键不关闭应用,回到桌面
     *
     * @return
     *//*
    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {

        if (keyCode == KeyEvent.KEYCODE_BACK && event.getAction() == KeyEvent.ACTION_DOWN) {
            //启动一个意图,回到桌面
            Intent backHome = new Intent(Intent.ACTION_MAIN);
            backHome.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            backHome.addCategory(Intent.CATEGORY_HOME);
            startActivity(backHome);
            return true;
        }
        return super.onKeyDown(keyCode, event);
    }
    */
    /*@Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // -------------向menu中添加子菜单-------------
        SubMenu prog = menu.addSubMenu("菜单");
        // 设置菜单头的图标
        prog.setHeaderIcon(R.drawable.tools);
        // 设置菜单头的标题
        prog.setHeaderTitle("菜单");
        // 添加菜单项
        MenuItem item = prog.add("查看帮助文档");
        //为菜单项设置关联的Activity
        item.setIntent(new Intent(this, MyVideo.class));
        return super.onCreateOptionsMenu(menu);
    }
*/
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflator = new MenuInflater(this);
        // 状态R.menu.context对应的菜单，并添加到menu中
        inflator.inflate(R.menu.my_menu, menu);
        return super.onCreateOptionsMenu(menu);
    }

    @Override
    // 选项菜单的菜单项被单击后的回调方法
    public boolean onOptionsItemSelected(MenuItem mi) {
        if (mi.isCheckable()) {
            mi.setChecked(true);
        }
        // 判断单击的是哪个菜单项，并针对性的作出响应。
        switch (mi.getItemId()) {
            case R.id.plain_item:
                Intent intent=new Intent(MainActivity.this,MyVideo.class);
                startActivity(intent);
                break;
        }
        return true;
    }


    @Override
    public boolean onTouchEvent(MotionEvent event) {
        // 将该Activity上的触碰事件交给GestureDetector处理
        return detector.onTouchEvent(event);
    }

    @Override
    public boolean onDown(MotionEvent e) {
        return false;
    }

    @Override
    public void onShowPress(MotionEvent e) {

    }

    @Override
    public boolean onSingleTapUp(MotionEvent e) {
        return false;
    }

    @Override
    public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY) {
        return false;
    }

    @Override
    public void onLongPress(MotionEvent e) {

    }

    @Override
    public boolean onFling(MotionEvent e1, MotionEvent e2,
                           float velocityX, float velocityY) {
        Message msg = new Message();
        msg.what = 0x456;
        DisplayMetrics dm = new DisplayMetrics(); //获取屏幕信息(宽高)
        getWindowManager().getDefaultDisplay().getMetrics(dm);
        int screenWidth = dm.widthPixels;
        float distanceX = e1.getX() - e2.getX();
        int new_atangleX = (int) (maxangle * distanceX / screenWidth); //改变的角度:最大角度=滑动距离：屏幕高度
        int angleX = Math.min(Math.max(new_atangleX + dqangle2, 3), maxangle); //具体角度=当前角度+改变角度
        dqangle2 = angleX;//更新角度
        try {
            command_0 = 0x02;
            command_1 = 0x01;
            command_2 = 0x02;
            command_3 = (byte) dqangle2;
            command_4 = (byte) dqangle2;
            command = new byte[]{command_0, command_1, command_2, command_3, command_4};
            msg.obj = command;
            clientThread.revHandler.sendMessage(msg);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return false;
    }







/*
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
                Message msg = new Message();
                msg.what = 0x456;
                DisplayMetrics dm = new DisplayMetrics(); //获取屏幕信息(宽高)
                getWindowManager().getDefaultDisplay().getMetrics(dm);
                int screenHeight = dm.heightPixels;
                int screenWidth = dm.widthPixels;
                float endY=event.getRawY();//记录此时 的坐标
                float endX=event.getRawX();//记录此时 的坐标

                float distanceY=endY-startY;//计算滑动距离
                float distanceX=endX-startX;//计算滑动距离

                int delatangle1=(int) (maxangle*distanceY/screenHeight); //改变的角度:最大角度=滑动距离：屏幕高度
                int angle1=Math.min(Math.max(delatangle1+dqangle1, 3), maxangle); //具体角度=当前角度+改变角度

                int delatangle2=(int) (maxangle*distanceX/screenWidth); //改变的角度:最大角度=滑动距离：屏幕高度
                int angle2=Math.min(Math.max(delatangle2+dqangle2, 3), maxangle); //具体角度=当前角度+改变角度

                int dqangle2=angle2;//更新角度

                command_0=0x02;
                command_1=0x01;
                command_2=0x02;
                command_3= (byte) dqangle2;
                command_4= (byte) dqangle1;
                command = new byte[]{command_0, command_1, command_2, command_3,command_4};
                msg.obj = command;
                clientThread.revHandler.sendMessage(msg);

                    *//*socketWriter.write(new byte[]{(byte)0xff,(byte)0x01,(byte)0x08,(byte)(dqangle2),(byte)0xff});
                    socketWriter.flush();//强制将输出流缓冲区的数据送出*//*
                ksangle2=dqangle2;//更新开始角度
                ksangle1=dqangle1;//更新开始角度
                int dqangle1=angle1;//更新角度
                ksangle1=dqangle1;//更新开始角度
                break;
            //case MotionEvent.ACTION_UP://手指离开屏幕回调该方法
            //break;
        }
        return true;
    }*/


}









