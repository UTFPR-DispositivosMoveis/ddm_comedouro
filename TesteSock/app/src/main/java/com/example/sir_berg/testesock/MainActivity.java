package com.example.sir_berg.testesock;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {

    private Button btConectar;
    private Button btEnviar;
    private EditText etTexto;
    private EditText etIpServer;
    private TextView tvLogServer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        btConectar = (Button) findViewById(R.id.btConectar);
        btEnviar = (Button) findViewById(R.id.btEnviar);
        etTexto = (EditText) findViewById(R.id.etMensagem);
        etIpServer = (EditText) findViewById(R.id.etIpServer);
        etIpServer.setText("172.20.220.<UTF>");

        tvLogServer = (TextView) findViewById(R.id.tvRespostaServer);
    }

    public EditText getEtTexto() {
        return etTexto;
    }

    public void btEnviarOnClick(View view){
        /**
         * Cria o Cliente TCP como uma tarefa em background.
         **/
        Thread threadClient = new Thread(new RunClient(etIpServer.getText().toString(), 12345, this, etTexto.getText().toString()));
        threadClient.start();

    }

    public void btConectarOnClick(View view) {

        /**
         * Uma vez conectado, mantem conectado e desativa.
         */
        btConectar.setEnabled(false);
        btEnviar.setEnabled(true);
    }

    public void setTvLogServer(String str){
        tvLogServer.setText(tvLogServer.getText().toString()+"\n"+str);
        Toast.makeText(this, "Novo dado na coleira", Toast.LENGTH_SHORT).show();
    }

}
