<?php

/**
 * @author Jair Foro <jairsanto@mec.gov.br>
 */
class AtualizarEmpenhoPagamento extends Modelo {
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
            $this->AtualizarEmpenho();
            $this->AtualizarPagamento();
            if ($this->commit()) {
                $this->enviarEmailConfirmacao();
            }
        } catch (Exception $exc) {
            $this->rollback();
            echo '<h1>Não foi possível Atualizar os empenhos e pagamentos. Erro: </h1>';
            echo '<pre>';
                var_dump($ex);
            echo '</pre>';
        }
    }

    public function AtualizarEmpenho() {
        $sql = "UPDATE 
                    par.empenho e 
                SET empsituacao = (
                    CASE 
                        WHEN es.ems_situacao_do_empenho ILIKE '%EFETIVADO%' THEN '2 - EFETIVADO'
                        WHEN es.ems_situacao_do_empenho ILIKE '%SOLICITAÇÃO APROVADA%' THEN '8 - SOLICITAÇÃO APROVADA'
                        WHEN es.ems_situacao_do_empenho ILIKE '%AUTORIZADO%' THEN '0 - AUTORIZADO'
                        ELSE es.ems_situacao_do_empenho 
                    END
                ),
        		empnumero 			= es.ems_ano_do_empenho||'NE'||es.ems_numero_do_empenho,
                empnumerooriginal 	= cast(es.ems_numero_do_empenho as numeric(6)),  
                empanoconvenio 		= cast(es.ems_ano_do_empenho as numeric(4)),
                empdataatualizacao 	= es.ems_data_atualizacao_rotina
                --SELECT to_char(es.ems_data_atualizacao_rotina, 'YYYY-MM-DD'), ems_data_atualizacao_rotina
                FROM par.empenhosigef es
                WHERE CAST(TO_CHAR(es.ems_data_atualizacao_rotina, 'YYYY-MM-DD') AS date) = (CAST(TO_CHAR(now(), 'YYYY-MM-DD') AS date) )
                AND es.empid = e.empid
                AND es.ems_numero_processo = e.empnumeroprocesso";
        $this->executar($sql);
        $this->commit();
    }
    
    public function AtualizarPagamento() {
        $sql = "UPDATE 
                    par.pagamento e 
                SET pagsituacaopagamento = (
                    CASE 
                        WHEN es.ds_situacao_doc_siafi ILIKE '%EFETIVADO%' THEN '2 - EFETIVADO'
                        WHEN es.ds_situacao_doc_siafi ILIKE '%SOLICITAÇÃO APROVADA%' THEN '8 - SOLICITAÇÃO APROVADA'
                        WHEN es.ds_situacao_doc_siafi ILIKE '%AUTORIZADO%' THEN '0 - AUTORIZADO'                                            
                        WHEN es.ds_situacao_doc_siafi ILIKE '%CANCELADO%' THEN '9 - CANCELADO'
                        WHEN es.ds_situacao_doc_siafi ILIKE '%VALA SIAFI%' THEN '6 - VALA SIAFI'
                        WHEN es.ds_situacao_doc_siafi ILIKE '%DEVOLVIDO%' THEN '11 - DEVOLVIDO'
                        WHEN es.ds_situacao_doc_siafi ILIKE '%DEVOLVIDA/ESTORNO -E%' THEN '34 - DEVOLVIDA/ESTORNO -E'
                        ELSE es.ds_situacao_doc_siafi 
                    END
                ),
                pagdataatualizacao = es.data_atualizacao_rotina,
                pagdatapagamentosiafi = es.dt_movimento
               -- SELECT TO_CHAR(es.data_atualizacao_rotina, 'YYYY-MM-DD'), data_atualizacao_rotina, es.dt_movimento
                FROM par.historicopagamentosigef es
                WHERE CAST(TO_CHAR(es.data_atualizacao_rotina, 'YYYY-MM-DD') AS date) = (CAST(TO_CHAR(now(), 'YYYY-MM-DD') AS date) )                   
                 AND CAST(es.nu_seq_mov_pag as integer) = CAST(e.parnumseqob as integer)
                    AND es.empid = e.empid
                    AND es.pagid = e.pagid";
        $this->executar($sql);
        $this->commit();
        /*
        $sql = "UPDATE par.pagamento SET pagstatus = 'I', pagjustificaitiva = 'Pagamento inativado pela carga' 
				WHERE cast(parnumseqob as varchar) not in (SELECT es.nu_seq_mov_pag FROM par.historicopagamentosigef es) 
					and pagstatus = 'A'";
        $this->executar($sql);
         
         */
    }
    
    public function enviarEmailConfirmacao(){
        $arrEmail = array(array('usuemail'=> $_SESSION['email_sistema'], 'usunome'=> SIGLA_SISTEMA));
        $strHtml = $this->getHtmlEmail();
        enviar_email(
            array(
                "nome" => self::NOME_REMETENTE,
                "email" => self::EMAIL_REMETENTE
            ), 
            $arrEmail, 
            "Script Atualizar Empenho e Pagamento Executado", $strHtml
        );
    }
    
    public function getHtmlEmail() {
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
                                    <thead>
                                        <tr>
                                            <td style="text-align: center;">
                                                <p><img  src="data:image/png;base64,' . base64_encode(file_get_contents(APPRAIZ . '/www/' . 'imagens/brasao.gif')) . '" width="70"/><br/>
                                                <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                                FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/><br/>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="text-align: right; padding: 40px 0 0 0;">
                                                ' . date('d/m/Y G:i:s') . '
                                            </td>
                                        </tr>
                                    </thead>
                                    <tbody>
                                        <tr>
                                            <td style="line-height: 15px; text-align:justify">
                                                <p>Prezado(a), </p>
                                                <p>O Script de Atualização dos Empenhos e Pagamentos foram executados com sucesso as '.date('d/m/Y H:i:s').'</p>
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
                                    <tfoot>
                                    </tfoot>
                                </table>
                            </body>
                        </html>';

        return $strHtml;
    }
}
