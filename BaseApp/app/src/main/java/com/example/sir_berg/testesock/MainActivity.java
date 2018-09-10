package com.example.sir_berg.testesock;

import android.content.Context;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

public class MainActivity extends AppCompatActivity {

    private Button btConectar;
    private Button btEnviar;
    private EditText etTexto;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        btConectar = (Button) findViewById(R.id.btConectar);
        btEnviar = (Button) findViewById(R.id.btEnviar);
        etTexto = (EditText) findViewById(R.id.etMensagem);
        //btEnviar.setEnabled(false);
    }

    public void btEnviarOnClick(View view){
        /**
         * Cria o Cliente TCP como uma tarefa em background.
         **/

        Thread threadClient = new Thread(new RunClient("172.20.220.247", 12345, this, etTexto.getText().toString()));
        threadClient.start();

    }

    public void btConectarOnClick(View view) {

        /**
         * Uma vez conectado, mantem conectado e desativa.
         */
        btConectar.setEnabled(false);
        btEnviar.setEnabled(true);
    }

    public Button getBtConectar() {
        return btConectar;
    }

    class RunClient implements Runnable{
        private String ip;
        private int porta;
        private MainActivity context;
        private ClientTCP cli;
        private String msg;

        public RunClient(String ip, int porta, MainActivity context, String msg){
            this.ip = ip;
            this.porta = porta;
            this.context = context;
            this.msg = msg;
        }

        @Override
        public void run() {
            cli = new ClientTCP(this.ip, this.porta, context, this.msg);
            cli.ClientClose(context);
        }
    }
}
