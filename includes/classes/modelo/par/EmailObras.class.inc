<?php

/**
 * @author Jair Foro <jairsanto@mec.gov.br>
 */
class EmailObras extends Modelo {

    /**
     * Nome do Remetente do E-mail
     */
    const NOME_REMETENTE = 'EQUIPE ' SIGLA_SISTEMA;

    /**
     * E-mail de envio
     */
    const EMAIL_REMETENTE = EMAIL_SISTEMA_NOREPLY;

    /**
     * Array de Invervalo de Envio
     * @var array 
     */
    protected $intervalo;

    /**
     * Obten o valor de intervalo
     * @return \EmailObra.class
     */
    public function getIntervalo() {
        return $this->intervalo;
    }

    /**
     * Atribui o valor de intervalo
     * @param type $intervalo
     * @return \EmailObras
     */
    public function setIntervalo($intervalo) {
        $this->intervalo = $intervalo;
        return $this;
    }

    public function __construct($arrIntervalo = array(60, 30, 5)) {
        foreach ($arrIntervalo as $intervalo) {
            $this->setIntervalo($intervalo);
            $this->enviarEmailObrasPar();
        }
    }

    public function enviarEmailObrasPar() {
        $arrObras = $this->buscarObras();
        if ($arrObras) {
            $arrObraAgrupada = array();
            $intTotal = count($arrObras);
            foreach ($arrObras as $key => $obra) {
                $arrCurrent = current($arrObras);
                $arrObraAgrupada[] = $obra;
                
                if (($obra['preesfera']=='M' && $obra['muncod'] != $arrCurrent['muncod']) || ($obra['preesfera']=='M' && (($key+1) == $intTotal))){
                    $this->enviarEmailObra($obra, $arrObraAgrupada);
                }
                if (($obra['preesfera']=='E' && $obra['estuf'] != $arrCurrent['estuf']) || ($obra['preesfera']=='E' && (($key+1) == $intTotal))){
                    $this->enviarEmailObra($obra, $arrObraAgrupada);
                }
            }
        }
    }

    public function buscarEmailEntidade($obra) {
        $strWhere = "";
        
        if ($obra['preesfera'] == 'E') { # Obra estadual
            $strWhere .="WHERE iu.estuf ='{$obra['estuf']}'
            AND en.dutid = ".DUTID_SECRETARIA_ESTADUAL;
        } else { #Obra Municipal
            $strWhere .="WHERE iu.muncod ='{$obra['muncod']}'
            AND en.dutid = ".DUTID_PREFEITURA;
        }
        
        $sql = "SELECT 
                en.entnome,
                en.entemail
            FROM par.instrumentounidade iu 
            INNER JOIN par.entidade en ON iu.inuid=en.inuid
            {$strWhere}";
        $arrEmail = $this->carregar($sql);
        return reset($arrEmail);
    }

    public function buscarObras() {

        $sql = "            		
			SELECT 
				foo.preid,
				foo.preesfera,
				foo.estuf,
				foo.muncod,    
				foo.predescricao,
				foo.prazo
			FROM (
            	SELECT
                	pre.preid,
                    pre.preesfera,
                    pre.estuf,
                    pre.muncod,    
                    pre.predescricao,
                    CASE WHEN (popdataprazoaprovado::date - now()::date) > ((MIN(pag.pagdatapagamentosiafi) + 720)::date - now()::date) THEN (popdataprazoaprovado::date - now()::date)
                    	ELSE ((MIN(pag.pagdatapagamentosiafi) + 720)::date - now()::date)
                    END as prazo
                FROM
                	obras.preobra pre
                INNER JOIN par.pagamentoobra po ON pre.preid = po.preid
                INNER JOIN par.pagamento pag ON pag.pagid = po.pagid AND pag.pagstatus = 'A'
                LEFT  JOIN obras.preobraprorrogacao pop ON pop.preid = po.preid AND popstatus = 'A'
                
        		WHERE
					pre.prestatus = 'A'
                    AND pre.presistema = '23'
                    AND pre.preidpai IS NULL 
                GROUP BY pre.estuf, pre.preesfera, pre.muncod, pre.predescricao,pre.preid, popdataprazoaprovado
				UNION           
        		SELECT
                	pre.preid,
                    pre.preesfera,
                    pre.estuf,
                    pre.muncod,    
                    pre.predescricao,
               	CASE WHEN (popdataprazoaprovado::date - now()::date) > ((MIN(pag.pagdatapagamentosiafi) + 720)::date - now()::date) THEN (popdataprazoaprovado::date - now()::date)
                	ELSE ((MIN(pag.pagdatapagamentosiafi) + 720)::date - now()::date)
               	END as prazo
                    FROM
                    	obras.preobra pre
                    INNER JOIN par.pagamentoobrapar po ON pre.preid = po.preid
                    INNER JOIN par.pagamento pag ON pag.pagid = po.pagid AND pag.pagstatus = 'A'
                    LEFT  JOIN obras.preobraprorrogacao pop ON pop.preid = po.preid AND popstatus = 'A'
                    WHERE
						pre.prestatus = 'A'
                        AND pre.presistema = '23'
                        AND pre.preidpai IS NULL 
                   GROUP BY pre.estuf, pre.preesfera, pre.muncod, pre.predescricao,pre.preid, popdataprazoaprovado
	) foo
	GROUP BY          foo.preid,
		foo.preesfera,
	    foo.estuf,
	    foo.muncod,
	    foo.predescricao,
	    foo.prazo
	HAVING prazo = ". $this->getIntervalo()." AND prazo > 0
    order by foo.estuf";

        $arrObras = $this->carregar($sql);
        return $arrObras;
    }
	
    public function getHtmlEmail($arrObraAgrupada) {
        $arrPrazo = reset($arrObraAgrupada);
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
                                                <p>Prezado(a) Dirigente, </p>
                                                <p>Existe(m) '.count($arrObraAgrupada).' Obra(s) com vencimento em '.$arrPrazo['prazo'].' dias conforme relação abaixo: </p>
                                                <table class="table">
                                                    <thead>
                                                        <tr>
                                                            <th class="th">N°</th>
                                                            <th class="th">Obra</th>
                                                            <th class="th">Tipo</th>
                                                            <th class="th">Endereço</th>
                                                            <th class="th">Valor</th>
                                                        </tr>
                                                    </thead>
                                                    <tbody>';
                                                    $intCount = 1;
                                                    foreach ($arrObraAgrupada as $key => $value){
                                                        $sql = "SELECT 
                                                                    preid || ' - ' || predescricao as obra,
                                                                    prelogradouro as logradouro,
                                                                    precomplemento as complemento,
                                                                    ptodescricao as tipoobra,
                                                                    prevalorobra as vlrobra
                                                                FROM 
                                                                    obras.preobra pre
                                                                INNER JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid
                                                                WHERE
                                                                    preid = " . $value['preid'];
                                                        $dado = $this->pegaLinha($sql);
                                                        $strHtml .='<tr>
                                                            <td class="td">'.$intCount++.'</td>
                                                            <td class="td">'.$dado['obra'].'</td>
                                                            <td class="td">'.$dado['tipoobra'].'</td>
                                                            <td class="td">'.$dado['logradouro'].'<br />'.$dado['complemento'].'</td>
                                                            <td class="td">R$: '.number_format($dado['vlrobra'], 2, ',', '.') .'</td>
                                                        </tr>';
                                                    }
                                                $strHtml .='
                                                    </tbody>
                                                </table>
                                                <p>Solicitamos ao ente federado atentar-se para a(s) vigência(s) da(s) Obra(s) objeto(s) do(s) Termo(s) de Compromisso(s) do seu Estado/Município.</p>
                                                <p>Caso a(s) obras() objeto(s) do(s)  Termo(s)  de Compromisso(s)  não tenha sido executado integralmente, deverão ser solicitadas as devidas prorrogações, sessenta dias antes do prazo final da vigência, considerando que a data de encerramento é de 720 (setecentos e vinte) dias a contar do recebimento da primeira parcela da(s) obra(s).</p>
                                                <p>Esclarecemos que se a solicitação não for feita em tempo hábil, sua(s) respectiva(s) obra(s) serão automaticamente finalizados, e os recursos recebidos deverão ser devolvidos à conta do Tesouro Nacional. </p>
                                                <p>Ressaltamos, ainda, que o pedido deverá ser solicitado no próprio SIMEC, na Aba PAR > Árvore> Lista de Obras> ícone Solicitar Prorrogação de vigência.</p>
                                                <p>Estamos à disposição para esclarecimentos e orientações que se fizerem necessárias.<br />
                                                Equipe Técnica da COVEN - Coordenação de Convênios:<br />
                                                -Telefones: (61)2022-4904/4905/5041/5038/4745.<br />
                                                - E-mail: coven@fnde.gov.br</p>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="padding: 10px 0 0 0;">
                                                    Atenciosamente,
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="text-align: left; padding: 10px 0 0 0;">
                                                <p style="font-weight:bold">Equipe Técnica da Coordenação de Convênios &ndash; COVEN<br />
                                                Fundo Nacional de Desenvolvimento da Educação <br />
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
    
    public function enviarEmailObra($obra, $arrObraAgrupada){
        $arrEmail = $this->buscarEmailEntidade($obra);
        $strHtml = $this->getHtmlEmail($arrObraAgrupada);
        enviar_email(
            array(
                "nome" => self::NOME_REMETENTE,
                "email" => self::EMAIL_REMETENTE
            ), 
            $arrEmail['entemail'], 
            "Atenção, Obra(s) com vencimento em {$obra['prazo']} dias", $strHtml
        );
        echo " - enviado para {$arrEmail['entemail']}: <br />";
    }

}
