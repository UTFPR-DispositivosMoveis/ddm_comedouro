package com.example.sir_berg.testesock;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.TimePicker;
import android.widget.Toast;

import java.util.Date;

public class Configuracao extends AppCompatActivity {

    private TextView tvHora1, tvHora2, tvHora3;
    private int hora, hora1, hora2, hora3;
    private Spinner spHora;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_configuracao);

        tvHora1 = (TextView) findViewById(R.id.tvHora1);
        tvHora2 = (TextView) findViewById(R.id.tvHora2);
        tvHora3 = (TextView) findViewById(R.id.tvHora3);
        spHora = (Spinner) findViewById(R.id.spHora);

        final String horas[] = {"Desativar","00", "01", "02", "03", "04", "05", "06",
            "07", "08", "09", "10", "11", "12", "13", "14", "15", "16",
            "17", "18", "19", "20", "21", "22", "23"};

        ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, horas);

        spHora.setAdapter(adapter);

        spHora.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                hora = position-1;
                Toast.makeText(Configuracao.this, horas[position], Toast.LENGTH_LONG).show();
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {
                Toast.makeText(Configuracao.this, "Nehuma hora selecionada!", Toast.LENGTH_LONG).show();
            }
        });

        Intent i = getIntent();

        hora1 = i.getIntExtra("hora1", -1);
        hora2 = i.getIntExtra("hora2", -1);
        hora3 = i.getIntExtra("hora3", -1);

        tvHora1.setText(makeHora(hora1,1));
        tvHora2.setText(makeHora(hora2,2));
        tvHora3.setText(makeHora(hora3,3));
    }

    private String makeHora(int hora, int indice){
        String aux="";
        if(hora == -1){
            aux = aux + "Tratar "+indice+": desativado!";
        }else{
            aux = aux + "Tratar "+indice+": " + String.format("%02d", hora)+ "h00";
        }
        return aux;
    }

    public void btSalvar(View view) {

        String horaFormatada = hora1 +"h00 \nTratar: " + hora2 +"h00 \nTratar: " +hora3+"h00 \n";
        Intent i = getIntent();

        // enviar para rede e validar o recebimento
        // usar função do wall
        // mostrar o resultado
        Toast.makeText(this, horaFormatada,
                Toast.LENGTH_LONG).show();
        i.putExtra( "hora1", hora1);
        i.putExtra( "hora2", hora2);
        i.putExtra( "hora3", hora3);
        setResult( 0, i );
        finish();
    }

    public void btSet1(View view) {
        if(hora == -1){
            hora1 = hora;
            tvHora1.setText("Tratar 1: desativado!");
            return;
        }else{
            if(hora != hora2 && hora != hora3) {
                hora1 = hora;
                tvHora1.setText("Tratar 1: " + hora + "h00");
            }else{
                Toast.makeText(this, "Hora já selecionada!",
                        Toast.LENGTH_LONG).show();
            }
        }
    }
    public void btSet2(View view) {
        if(hora==-1){
            hora2 = hora;
            tvHora2.setText("Tratar 2: desativado!");
            return;
        }else{
            if(hora!=hora1 && hora!=hora3) {
                hora2 = hora;
                tvHora2.setText("Tratar 2: " + hora + "h00");
            }else{
                Toast.makeText(this, "Hora já selecionada!",
                        Toast.LENGTH_LONG).show();
            }
        }
    }
    public void btSet3(View view) {
        if(hora==-1){
            hora3 = hora;
            tvHora3.setText("Tratar 3: desativado!");
            return;
        }else{
            if(hora != hora2 && hora != hora1) {
                hora3 = hora;
                tvHora3.setText("Tratar 3: " + hora + "h00");
            }else{
                Toast.makeText(this, "Hora já selecionada!",
                        Toast.LENGTH_LONG).show();
            }
        }
    }
}
