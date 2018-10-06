package com.example.sir_berg.testesock;

import android.content.Context;
import android.content.Intent;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    private Button btConfigurar;
    private Button btManual;
    private EditText etTexto;
    private TextView tvBateria;
    private TextView tvRacao;
    private TextView tvHora;

    private Integer alturaRacao;
    private Integer nivelBateria;

    private Integer hora1 = -1;
    private Integer hora2 = -1;
    private Integer hora3 = -1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        btConfigurar = (Button) findViewById(R.id.btConfigurar);
        btManual = (Button) findViewById(R.id.btManual);
        tvHora = (TextView) findViewById(R.id.tvHora);

        tvHora.setText(this.makeHora());
    }

    public void btConfigurarOnClick(View view){
        Intent i = new Intent(this, Configuracao.class);
        i.putExtra( "hora1", hora1);
        i.putExtra( "hora2", hora2);
        i.putExtra( "hora3", hora3);
        startActivityForResult(i, 0);
    }


    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        if ( data != null){
            hora1 = data.getIntExtra("hora1", -1);
            hora2 = data.getIntExtra("hora2", -1);
            hora3 = data.getIntExtra("hora3", -1);
            tvHora.setText(makeHora());
        }
    }

    private String makeHora(){
        String aux="";
        if(hora1 == -1){
            aux = aux + "Tratar 1: desativado!\n";
        }else{
            aux = aux + "Tratar 1: " + String.format("%02d", hora1)+ "h00\n";
        }

        if(hora2 == -1){
            aux = aux + "Tratar 2: desativado!\n";
        }else{
            aux = aux + "Tratar 2: " + String.format("%02d", hora2)+ "h00\n";
        }

        if(hora3 == -1){
            aux = aux + "Tratar 3: desativado!\n";
        }else{
            aux = aux + "Tratar 3: " + String.format("%02d", hora3)+ "h00";
        }
        return aux;
    }

    public void btManualOnClick(View view) {
        Intent i = new Intent(this, Manual.class);
        startActivity(i);
    }

    public void btAtualizarOnClick(View view){

        /*Cria o Cliente TCP como uma tarefa em background.*/
//        Thread threadClient = new Thread(new RunClient("172.20.220.247", 12345, this, etTexto.getText().toString()));
//        threadClient.start();
    }

    public Button getBtConfigurar() {
        return btConfigurar;
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
            //  ok = send
            // string = recive
            cli.ClientClose(context);
        }
    }
}
