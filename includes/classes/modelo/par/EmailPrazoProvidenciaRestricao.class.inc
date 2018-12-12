<?php

/**
 * @author Jair Foro <jairsanto@mec.gov.br>
 */
class EmailPrazoProvidenciaRestricao extends Modelo{
    
    const NOME_REMETENTE = 'EQUIPE ' SIGLA_SISTEMA;
    const EMAIL_REMETENTE = EMAIL_SISTEMA_NOREPLY;

    public function enviarEmailPrazoProvidenciaRestricao(){
        $arrCpf = $this->buscarCpfRestricao();
        if ($arrCpf) {
            foreach ($arrCpf as $cpf) {
                $arrRestricao = $this->buscarRestricaoVencida($cpf['usucpf']);
                $arrInfo = reset($this->buscarInfoUsuario($cpf['usucpf']));
                
                if ($arrRestricao) {
                    $strHtml = $this->getHtmlEmail($arrRestricao);
                    enviar_email(
                        array(
                            "nome" => self::NOME_REMETENTE, 
                            "email" => self::EMAIL_REMETENTE
                        ), 
                        $arrInfo['usuemail'],
                        "[SIMEC-PAR] Restrições com prazo da Providência Vencidas", 
                        $strHtml
                    );
                }
            }
        }
    }
    
    public function buscarInfoUsuario($usucpf){
        $sql = "
        SELECT
            usunome,
            usuemail
        FROM
            seguranca.usuario
        WHERE
            usucpf = '{$usucpf}'";

        $arrInfo = $this->carregar($sql);        
        return $arrInfo;
    }
    
    public function buscarCpfRestricao() {
        $sql = "SELECT DISTINCT 
                    usucpf 
                FROM par.restricaofnde res  
                WHERE res.restemporestricao = true 
                AND res.ressuperado = 'N'
                AND res.resdatafim < CURRENT_DATE";

        $arrCpf = $this->carregar($sql);        
        return $arrCpf;
    }
    
    public function buscarRestricaoVencida($usucpf){
        $sql = "SELECT 
                    res.resdescricao,
                    to_char(res.resdatafim, 'dd/mm/YYYY') as datafim,
                    to_char(res.resdatainicio, 'dd/mm/YYYY') as datainicio 
                FROM par.restricaofnde res 
                WHERE res.restemporestricao = true 
                AND res.ressuperado = 'N'
                AND res.resdatafim < CURRENT_DATE
                AND res.usucpf = '{$usucpf}'
                ORDER BY res.usucpf, res.resdatafim asc";

        $arrRestricao = $this->carregar($sql);        
        return $arrRestricao;
    }
    
    public function getHtmlEmail($arrRestricao) {
        $strHtml = '<html>
                        <head>
                            <title></title>
                            <link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
                            <link rel="stylesheet" type="text/css" href="../includes/listagem.css">
                        </head>
                        <body>
                            <table style="width: 100%;">
                                <tbody>
                                    <tr>
                                        <td style="padding:20px 0 20px 0;">
                                          Assunto: <b>Existe(m) '.count($arrRestricao).' restrição(ões) com prazo de providência vencida(s) e não superada(s):</b>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td style="line-height: 15px; text-align:justify">
                                            <table style="width: 80%;">
                                                <thead>
                                                    <tr>
                                                        <th>Restrição</th>
                                                        <th>Data de Vencimento</th>
                                                    </tr>
                                                </thead>
                                                <tbody>';
                                                    foreach ($arrRestricao as $restricao){
                                                        $strHtml .= '
                                                            <tr>
                                                                <td>'.$restricao['resdescricao'].'</td>
                                                                <td>'.$restricao['datainicio'].' à '.$restricao['datafim'].'</td>
                                                            </tr>
                                                        ';
                                                    }
                                                $strHtml .= '</tbody>
                                            </table>
                                        </td>
                                    </tr>';
                                   
                                 $strHtml .= '</tbody>
                            </table>
                        </body>
                    </html>';
        
        return $strHtml;
    }
    
    
}