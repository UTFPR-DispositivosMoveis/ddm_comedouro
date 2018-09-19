package com.example.sir_berg.testesock;

import android.content.Context;
import android.widget.Button;
import android.widget.Toast;

import java.io.DataOutputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.util.concurrent.Semaphore;

public class ClientTCP{
    private Semaphore control;
    private String buffer;
    private Socket sock; // Descritor de socket.
    private OutputStream out; // Saida de bytes

    public ClientTCP(String ip, int porta, final MainActivity context, String buffer){
        try {
            this.sock = new Socket(ip, porta);
            out = new DataOutputStream(sock.getOutputStream());
            this.buffer = buffer;
            out.write(this.buffer.getBytes());
            out.flush();
        }catch (Exception ex) {
            context.runOnUiThread( new Thread() {
                public void run() {
                    Toast.makeText(context, "Deu erro desconhecido!!", Toast.LENGTH_SHORT).show();
                }
            });
        }
    }

    public void ClientClose(final MainActivity context){
        try {
            this.sock.close();
        }catch (Exception e){
            context.runOnUiThread( new Thread() {
                public void run() {
                    Toast.makeText(context, "Deu erro desconhecido!!", Toast.LENGTH_SHORT).show();
//                    Button bt = context.getBtConectar();
//                    bt.setEnabled(true);
                }
            });
        }
    }

    public void flush(final MainActivity context){
        try {
            out.write(buffer.getBytes());
        }catch (Exception ex){
            context.runOnUiThread( new Thread() {
                public void run() {
                    Toast.makeText(context, "Deu erro desconhecido!!", Toast.LENGTH_SHORT).show();
//                    Button bt = context.getBtConectar();
//                    bt.setEnabled(true);
                }
            });
        }
    }

    public void setBuffer(String buffer){
        this.buffer = buffer;
    }
}
