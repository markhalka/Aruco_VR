package com.example.android.streamv;

import android.net.wifi.WifiManager;
import android.os.AsyncTask;
import android.text.format.Formatter;
import android.util.Log;
import android.widget.Toast;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.util.Arrays;

public class CSL extends AsyncTask<String,String,String> {
    byte[] s;
    String ip;

    CSL() {
        //String Ip
        //ip=Ip;
        //Log.e("TAG", "HERE ");
        //this.s = s;
        //this.si = si;
        Log.e("TAG", "sss");

    }
    protected String doInBackground(String... strings) {
       // if (si != "") {
        Log.e("TAG","we're in");


        try {//"192.168.0.16"
                //InetAddress ips = InetAddress.getByName(si);
                //DatagramSocket ds = new DatagramSocket();
               // byte buf[] = null;
                //Thread.sleep(10000);
                //buf = s.getBytes();
                //DatagramPacket DpSend =                        new DatagramPacket(s, s.length, ips, 8600);
               // ds.send(DpSend);

               // ds.close();

                String lText="";
                DatagramSocket dsocket = new DatagramSocket(8080);

                //String ip = dsocket.getInetAddress().toString();
                Log.e("UDP packet received", ip+ " is my ip");

                byte[] buffer = new byte[2048];
                DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
                while(true) {

                    dsocket.receive(packet);
                    lText = new String(buffer, 0, packet.getLength());
                    Log.e("TAG", lText);
                    //data.setText(lText);

                    packet.setLength(buffer.length);

                    new CFS((strings[0]+"ip").getBytes(), lText).execute();
                    publishProgress(lText);
                    return  lText;
                }



            } catch (Exception e) {
                Log.e("TAG", "EXCEPTION CAUGHT WHILE EXECUTING DATA TRANSACTION " + e);
            }

        return null;
    }//com/example/android/picappa/CFS.java

    protected void onProgressUpdate(String... strings) { //though this guy can...
        //setProgressPercent(progress[0]);
        Log.e("TAG",strings+" progress"); //this method can interact with the main thread
    }


    protected void onPostExecute(String result) { //This will never get executed with while(true)
        Log.e("TAG","done "+ result);
       // MainActivity.
        //showDialog("Downloaded " + result + " bytes");
    }
}

    //save from back when it was all void
/*
package com.example.android.streamv;

import android.net.wifi.WifiManager;
import android.os.AsyncTask;
import android.text.format.Formatter;
import android.util.Log;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.util.Arrays;

public class CSL extends AsyncTask<Void,Void,Void> {
    byte[] s;
    String ip;

    CSL(String Ip) {
        ip=Ip;
        //Log.e("TAG", "HERE ");
        //this.s = s;
        //this.si = si;
       // Log.e("TAG", s.length+"sss");

    }
    protected Void doInBackground(Void... voids) {
       // if (si != "") {
            try {//"192.168.0.16"
                //InetAddress ips = InetAddress.getByName(si);
                //DatagramSocket ds = new DatagramSocket();
               // byte buf[] = null;
                //Thread.sleep(10000);
                //buf = s.getBytes();
                //DatagramPacket DpSend =                        new DatagramPacket(s, s.length, ips, 8600);
               // ds.send(DpSend);

               // ds.close();


                String lText="";
                DatagramSocket dsocket = new DatagramSocket(8080);

                //String ip = dsocket.getInetAddress().toString();
                Log.e("UDP packet received", ip+ " is my ip");

                byte[] buffer = new byte[2048];
                DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
                while(true) {

                    dsocket.receive(packet);
                    lText = new String(buffer, 0, packet.getLength());
                    Log.e("UDP packet received", lText);
                    //data.setText(lText);

                    packet.setLength(buffer.length);

                    //new CFS((ip+"ip").getBytes(), lText).execute();


                }

            } catch (Exception e) {
                Log.e("TAG", "EXCEPTION CAUGHT WHILE EXECUTING DATA TRANSACTION " + e);
            }

        return null;
    }//com/example/android/picappa/CFS.java
}
 */
