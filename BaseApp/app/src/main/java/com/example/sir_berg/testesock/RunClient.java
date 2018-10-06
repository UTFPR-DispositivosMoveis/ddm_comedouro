package com.example.sir_berg.testesock;

import android.app.Activity;
import android.widget.Toast;

class RunClient implements Runnable{
    private String ip;
    private int porta;
    private Activity context;
    private ClientTCP cli;
    private String requi;

    public RunClient(String ip, int porta, Activity context, String requi){
        this.ip = ip;
        this.porta = porta;
        this.context = context;
        this.requi = requi;
    }

    /**
     * Todas as Activities irao chamar a mesma funcao para enviar.
     *
     * Cada Activity ira possuir um codigo antes da funcao que e preciso enviar pela requisicao.
     * Esse codigo e um "sigla" do nome dela. Como exemplo, MainActivity sera representada por "ma".
     * Entao, as requisicoes que vierem dessa activity devem voltar para aquela activity especifica, para
     * isso, e necessario como parametro de entrada um valor generico para todas as activities e
     * na string de requisicao ser especificado quem fez o que.
     *
     **/
    @Override
    public void run() {

        // Primeiramente, tratando a String de entrada, e removendo o primeiro termo dela.
        String act = null, func = null;

        act = requi.split(" ")[0];
        func = requi.split(" ")[1];

        // Removendo o prefixo do nome da Activity, pois o servidor nao precisa interpretar.
        this.requi = this.requi.replaceFirst(act + " ", "");

        // Criando um objeto ClientTCP, e enviando a requisicao.
        cli = new ClientTCP(this.ip, this.porta, context);

        String aux = "";

        if(!cli.getCon().equals("failCon")) {
            aux = cli.ClientSendReq(this.requi);
        }else{
            aux = "fail";
        }

        final String resp = aux;

        /**, this.context
         * A nao ser que a requisicao tenha sido um status de toda a aplicacao,
         * as demais funcoes e de alteracao de alguma coisa, ou possui um retorno.
         *
         * Com isso, e necessario interpretar esses retornos e analisar se deu boa ou nao.
         *
         * Requisicoes de cada lugar:
         *
         * - MainActivity:
         *      --> getstatus
         *          hora1 hora2 hora3 alturaRacao nivelBateria
         *
         * - Configuracao:
         *      --> SetHora: hora <hora> <indice>
         *
         * - Manual:
         *      --> Abrir: abrir <time>
         *      --> Buzzer: buzz <time>
         **/
        switch (act){
            case "ma": // MainActivity
                final MainActivity auxMA = (MainActivity)context;
                auxMA.runOnUiThread(new Thread(){
                    public void run(){
                        String aux[] = resp.split(" ");
                        if(!aux[0].equals("fail")) {
                            auxMA.setStatus(Integer.valueOf(aux[0]), Integer.valueOf(aux[1]), Integer.valueOf(aux[2]), Integer.valueOf(aux[3]), Integer.valueOf(aux[4]));
                        }else{
                            auxMA.setStatusNotConnect();
                        }
                    }
                });
                break;

            case "cf": // Configuracao.
                /**
                 * É preciso indentificar qual hora foi acionada.
                 *  hora <hora> <indice>
                 *  requi[2]: pega o indice
                 **/
                final Configuracao auxC = (Configuracao)context;
                final int indice = Integer.valueOf(requi.split(" ")[2]);
                final int hora = Integer.valueOf(requi.split(" ")[1]);
                auxC.runOnUiThread(new Thread(){
                    public void run(){
                        if(resp.equals("fail")){
                            auxC.setHoraErro();
                        }else{
                            if(resp.equals("ok")) {
                                auxC.setHora(hora, indice);
                            }
                        }
                    }
                });

                break;

            case "man":
                final Manual auxMan = (Manual)context;

                break;

            default:
                break;
        }

        cli.ClientClose();
    }
}