package com.example.android.streamv;

import android.net.wifi.WifiManager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.format.Formatter;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import java.util.Arrays;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Button btnM = (Button) findViewById(R.id.btn_begin);
        assert btnM != null;
        btnM.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                listen();
            }
        });

        listen();
    }
    public String tip;
    public void listen(){
       // while(true){
        Log.e("TAG","listen begin");
        WifiManager wifiManager = (WifiManager) getApplicationContext().getSystemService(WIFI_SERVICE);
        String ipAddress = Formatter.formatIpAddress(wifiManager.getConnectionInfo().getIpAddress());
            new CSL().execute(ipAddress);
       // }
    }

    public void send (byte[] s, String si) {
        //String st="";
        /*
        for (byte var:s) {
            st+=var+"";
        }
        */
        for (int i = 0; i < s.length / 65000; i++) {
            new CFS(Arrays.copyOfRange(s, i * 65000, (i + 1) * 65000), si).execute();
        }
    }

}
