package com.example.sir_berg.testesock;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Spinner;
import android.widget.Toast;

public class Manual extends AppCompatActivity {

    private Spinner spTempo;
    final String tempo[] = {"5s", "10s", "15s", "20s"};
    private Integer time;

    private String ip;
    private int porta;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_manual);
        spTempo = (Spinner) findViewById(R.id.spTempo);

        ArrayAdapter<String> adapter = new ArrayAdapter<>(this, android.R.layout.simple_list_item_1, tempo);
        spTempo.setAdapter(adapter);

        Intent i = getIntent();
        ip = i.getStringExtra("ip");
        porta = i.getIntExtra("porta", 12345);

        spTempo.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                time = (position+1)*5;
                Toast.makeText(Manual.this, "Tempo de Abertura: " + time, Toast.LENGTH_SHORT).show();
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {
                Toast.makeText(Manual.this, "Selecione alguma coisa" + time, Toast.LENGTH_SHORT).show();
            }
        });

    }

    public void btTratar(View view){
        // funcao para abrir valvula de racao para tratar peixes
        // mostrar o resultado
        Thread thread = new Thread(new RunClient(this.ip, this.porta, this, "man tratar " + time));
        thread.start();
    }

    public void tratarOk(){
        Toast.makeText(this, "Tratamento Ok!!", Toast.LENGTH_SHORT).show();
    }

    public void tratarNotOk(){
        Toast.makeText(this, "Comedouro ocupado!!", Toast.LENGTH_SHORT).show();
    }

    public void btBuzzer(View view){
        // funcao para ligar alerta sonoro
        Thread thread = new Thread(new RunClient(this.ip, this.porta, this, "man buzzer " + time));
        thread.start();
    }

    public void buzzerOk(){
        Toast.makeText(this, "Buzzer Ligado!!", Toast.LENGTH_SHORT).show();
    }

    public void buzzerNotOk(){
        Toast.makeText(this, "Buzzer ocupado!!", Toast.LENGTH_SHORT).show();
    }

}
