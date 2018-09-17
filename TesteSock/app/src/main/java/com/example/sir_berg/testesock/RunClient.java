package com.example.sir_berg.testesock;

import com.example.sir_berg.testesock.ClientTCP;
import com.example.sir_berg.testesock.MainActivity;

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
        cli = new ClientTCP(this.ip, this.porta, context);

        final String resp = cli.ClientSendReq(this.msg, this.context);

        context.runOnUiThread(new Thread() {
            public void run(){
                context.setTvLogServer(resp);
                context.getEtTexto().setText("");
                context.getEtTexto().requestFocus();
            }
        });

        cli.ClientClose(context);
    }

    /**
     *  Adicionar o Interpretador aqui!!!
     *  o context funciona como se fosse um callback.
     *  E esse callback direto para Ui deve ser executado em uma thread especifica.
     *  context.runOnUiThread()--->
     **/
}