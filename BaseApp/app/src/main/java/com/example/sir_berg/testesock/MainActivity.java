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
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {

    private Button btConfigurar;
    private Button btManual;
    private EditText etTexto;
    private TextView tvBateria;
    private TextView tvRacao;
    private TextView tvHora;

    private Integer alturaRacao = 0;
    private Integer nivelBateria = 0;

    private Integer hora1 = -1;
    private Integer hora2 = -1;
    private Integer hora3 = -1;

    private String ip = "192.168.43.130";
    private int porta = 12345;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        btConfigurar = (Button) findViewById(R.id.btConfigurar);
        btManual = (Button) findViewById(R.id.btManual);

        tvBateria = (TextView) findViewById(R.id.tvBateria);
        tvRacao = (TextView) findViewById(R.id.tvRacao);
        tvHora = (TextView) findViewById(R.id.tvHora);

        Thread thread = new Thread(new RunClient(this.ip, this.porta, this, "ma getstatus"));
        thread.start();
    }

    public void btConfigurarOnClick(View view){
        Intent i = new Intent(this, Configuracao.class);
        i.putExtra( "hora1", hora1);
        i.putExtra( "hora2", hora2);
        i.putExtra( "hora3", hora3);
        i.putExtra("ip", this.ip);
        i.putExtra("porta", this.porta);
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
        i.putExtra("ip", this.ip);
        i.putExtra("porta", this.porta);
        startActivity(i);
    }

    public void btAtualizarOnClick(View view){
        Thread thread = new Thread(new RunClient(this.ip, this.porta, this, "ma getstatus"));
        thread.start();
    }

    /**
     * getstatus: Ordem de recebimento
     * hora1 hora2 hora3 alturaRacao nivelBateria
     **/
    public void setStatus(int hora1, int hora2, int hora3, int alturaRacao, int nivelBateria){
        this.hora1 = hora1;
        this.hora2 = hora2;
        this.hora3 = hora3;
        this.alturaRacao = alturaRacao;
        this.nivelBateria = nivelBateria;

        tvBateria.setText("Bateria: " + String.format("%02d", nivelBateria) + "%");
        tvRacao.setText("Nível Ração: " + String.format("%02d", alturaRacao) + "%");
        tvHora.setText(makeHora());
    }

    public void setStatusNotConnect(){
        Toast.makeText(this, "Servidor não disponível!!", Toast.LENGTH_SHORT).show();
        tvHora.setText(makeHora());
    }

    public Button getBtConfigurar() {
        return btConfigurar;
    }
}
