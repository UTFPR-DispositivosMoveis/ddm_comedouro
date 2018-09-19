package com.example.sir_berg.testesock;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;
import android.widget.TimePicker;
import android.widget.Toast;

import java.util.Date;

public class Configuracao extends AppCompatActivity {

    private TimePicker tpHora;
    private TextView tvHora;
    private int hora;
    private int minuto;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_configuracao);
        tpHora = (TimePicker) findViewById(R.id.tpHora);
        tvHora = (TextView) findViewById(R.id.tvHora);

        Intent i = getIntent();

        String hora = i.getStringExtra("hora");
        tvHora.setText( hora);

    }

    public void btDefinirHora(View view){
        int hora = tpHora.getCurrentHour();
        int minuto = tpHora.getCurrentMinute();

        String horaMinuto = hora + "h" + minuto;

        Toast.makeText(this, horaMinuto, Toast.LENGTH_LONG).show();
    }

    public void btPegarHora(View view) {
        this.hora = tpHora.getCurrentHour();
        this.minuto = tpHora.getCurrentMinute();

        String horaFormatada = "";
        Intent i = getIntent();
        if(hora<=9){
            horaFormatada = "0" + hora;
        }else{
            horaFormatada = String.valueOf(hora);
        }
        if(minuto <=9){
            horaFormatada += ":0" + minuto;
        }else{
            horaFormatada += ":" + String.valueOf(minuto);
        }

        // enviar para rede e validar o recebimento

        // mostrar o resultado
        Toast.makeText(this, horaFormatada,
                Toast.LENGTH_LONG).show();
        i.putExtra( "hora", horaFormatada);
        setResult( 0, i );
        finish();

    }

    public void btSalvar(View view){
        // salvar conteudo e enviar para rasp;
    }
}
