package com.example.android.streamv;

import android.os.AsyncTask;
import android.util.Log;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
public class CFS extends AsyncTask<Void,Void,Void> {
    byte[] s;
    String si;
    String sp;

    CFS(byte[] s, String si) {
        //Log.e("TAG", "HERE ");
        this.s = s;
        this.si = si;
        Log.e("TAG", s.length+"sss"+s.toString()+" s "+si);

    }
    /*
    CFS(String s, String si) {
        //Log.e("TAG", "HERE ");
        this.sp = s;
        this.si = si;
        Log.e("TAG", s+" sss");

    }*/
    protected Void doInBackground(Void... voids) {
        if (si != "") {
            try {//"192.168.0.16"
               // si="192.168.0.18";
                Log.e("TAG",si+" server ip");
                InetAddress ips = InetAddress.getByName(si);
                DatagramSocket ds = new DatagramSocket();
                byte buf[] = null;
                //Thread.sleep(10000);
                //buf = s.getBytes();
                DatagramPacket DpSend =
                        new DatagramPacket(s, s.length, ips, 8080);
                ds.send(DpSend);
                ds.close();
            } catch (Exception e) {
                Log.e("TAG", "EXCEPTION CAUGHT WHILE EXECUTING DATA TRANSACTION " + e);
            }
        }
        return null;
    }//com/example/android/picappa/CFS.java
}
