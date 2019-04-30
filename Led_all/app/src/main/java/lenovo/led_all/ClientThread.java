package lenovo.led_all;

import android.os.Handler;
import android.os.Looper;
import android.os.Message;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.Socket;
import java.net.SocketTimeoutException;

import static lenovo.led_all.MainActivity.IP;

/**
 * Created by Lenovo on 2017/3/27.
 */

public class ClientThread implements Runnable {

    private Socket s;
    private Handler handler;
    public Handler revHandler;

    BufferedReader br = null;
    OutputStream os = null;

    public ClientThread(Handler handler) {
        this.handler = handler;
    }

    public void run() {
        try {
            s = new Socket(IP, 9999);
            br = new BufferedReader(new InputStreamReader(s.getInputStream()));
            os = s.getOutputStream();
            new Thread() {
                @Override
                public void run() {
                    MainActivity.thread_flag=true;
                    int content = 0;
                    try {
                        while (((content = Integer.parseInt(br.readLine())) != 0)) {
                            Message msg = new Message();
                            msg.what = 0x123;
                            msg.obj = content;
                            handler.sendMessage(msg);
                        }
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }.start();
            Looper.prepare();
            revHandler = new Handler() {
                @Override
                public void handleMessage(Message msg) {
                    if (msg.what == 0x456) {
                        try {
                            os.write((byte[]) msg.obj);
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }
                }
            };
            Looper.loop();
        } catch (SocketTimeoutException el) {
            System.out.println("网络连接超时！！");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
