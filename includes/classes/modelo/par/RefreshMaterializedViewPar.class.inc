<?php

/**
 * @author Jair Foro <jairsanto@mec.gov.br>
 */
class RefreshMaterializedViewPar extends Modelo {
    /**
     * Nome do Remetente do E-mail
     */
    const NOME_REMETENTE = 'EQUIPE ' SIGLA_SISTEMA;

    /**
     * E-mail de envio
     */
    const EMAIL_REMETENTE = EMAIL_SISTEMA_NOREPLY;

    public function __construct() {
        try {
            $msg = $this->refreshMaterializedViewPar();
            if ($this->commit()) {
                $msgCompleta = 'SUCESSO: O Script de Atualização das Views Materializadas do PAR foi executado com sucesso as '.date('d/m/Y H:i:s'). '<br /><br />';
                $msgCompleta .= $msg;
                $this->enviarEmail('Views Materializadas do PAR Atualizadas com Sucesso', $msgCompleta);
                echo $msgCompleta;
            }
        } catch (Exception $exc) {
            $this->rollback();
            $msg = '<strong>ERRO: Houve falha de atualização das Views Materializadas do PAR '. date('d/m/Y H:i:s').'</strong><br /><br /><pre>';
            $msg .= var_dump($exc);
            $msg .= '</pre>';
            $this->enviarEmail('ERRO: Houve falha de atualização das Views Materializadas do PAR', $msg);
            echo $msg;
        }
    }

    public function refreshMaterializedViewPar() {
        $sql = "SELECT DISTINCT schemaname, matviewname FROM PG_MATVIEWS where schemaname = 'par'";
        $arrMaterializedViewPar = $this->carregar($sql);

        $msg = "";
        foreach ($arrMaterializedViewPar as $materializedViewPar) {
            $sql = "REFRESH MATERIALIZED VIEW {$materializedViewPar['schemaname']}.{$materializedViewPar['matviewname']}";
            $this->executar($sql);
            $msg .= "{$sql}<br />";
        }
        return $msg;
    }
    
    public function enviarEmail($assunto = '', $msg = ''){
        $arrEmail = array(array('usuemail'=> $_SESSION['email_sistema'], 'usunome'=> SIGLA_SISTEMA));
        $strHtml = $this->getHtmlEmail($msg);
        enviar_email(
            array(
                "nome" => self::NOME_REMETENTE,
                "email" => self::EMAIL_REMETENTE
            ), 
            $arrEmail,
            $assunto, $strHtml
        );
    }
    
    public function getHtmlEmail($msg) {

        $strHtml = '<html>
                        <head>
                            <title></title>
                            <link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
                            <link rel="stylesheet" type="text/css" href="../includes/listagem.css">
                            <style type="text/css">
                                .table
                                {
                                  font-family:"Trebuchet MS", Arial, Helvetica, sans-serif;
                                  width:100%;
                                  border-collapse:collapse;
                                }
                                .td, .th
                                {
                                  border:1px solid #DEDFE0;
                                  padding:3px 7px 2px 7px;
                                }
                                .th
                                {
                                  text-align:left;
                                  padding-top:5px;
                                  padding-bottom:4px;
                                  background-color:#DEDFE0;
                                }

                            </style>
                        </head>
                        <body>
                            <table style="width: 100%;">
                                <tbody>
                                    <tr>
                                        <td style="line-height: 15px; text-align:justify">
                                            <p>Prezado(a), </p>
                                            <p>'.$msg.'</p>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td style="padding: 10px 0 0 0;">
                                                Atenciosamente,
                                        </td>
                                    </tr>
                                    <tr>
                                        <td style="text-align: left; padding: 10px 0 0 0;">
                                            <p style="font-weight:bold">Equipe PAR<br />
                                            MINISTÉRIO DA EDUCAÇÃO </p>
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                        </body>
                    </html>';

        return $strHtml;
    }
}
