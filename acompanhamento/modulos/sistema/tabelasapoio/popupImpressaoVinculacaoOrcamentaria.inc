<?php 
    if(!isAjax()){
?> 
<link rel="stylesheet" href="../library/bootstrap-3.0.0/css/bootstrap.css">
<script src="../library/jquery/jquery-1.10.2.js" type="text/javascript" charset="ISO-8895-1"></script>
<script src="../library/bootstrap-3.0.0/js/bootstrap.min.js" type="text/javascript" charset="utf-8"></script>
<script language="javascript" src="/includes/Highcharts-3.0.0/js/highcharts.js"></script>
<script language="javascript" src="/estrutura/js/funcoes.js"></script>
<?  }
?>
<script>
    $(document).ready(function() {
        $('[data-toggle="popover"]').popover({trigger: 'hover'});

    });
</script>
<div class="col-md-12">
    <?php
    /**
     * Funções comuns aos sistemas SPO.
     * @see funcoesspo.php
     */
    require_once (APPRAIZ . 'includes/funcoesspo.php');
    /**
     * Componente para listagens.
     */
    require_once APPRAIZ . 'includes/library/simec/Listagem.php';
    /**
     * Componente para Graficos.
     */
    include_once APPRAIZ . "includes/library/simec/Grafico.php";
    /**
     * Funções Vinculações orçamentárias.
     */
    include_once APPRAIZ . "www/planacomorc/_funcoesVinculacaoOrcamentaria.php";

    /* Formatar o Comentário */
    function formataComentario($comentario, $dados) {
        return '<span class="glyphicon glyphicon-info-sign" data-toggle="popover" data-placement="bottom" title="Composição dos valores para ' . $dados['ano'] . '" data-content="' . $comentario . '"></span>';
    }

    /* Pesquisar chave no Array */
    function searcharray($value, $key, $array) {
        foreach ($array as $k => $val) {
            if ($val[$key] == $value) {
                return $k;
            }
        }
        return null;
    }

    $vacid = $_REQUEST['vacid'];
    if (isset($vacid)) {

        $sql = <<<DML
            SELECT
                aet.comentario,
                aet.ano,
                aet.empenhado,
                aet.pago,
                aet.rap_npp,
                aet.rap_pp
            FROM planacomorc.vwacaoestrategicatotal aet
            WHERE vacid = {$vacid}
DML;

        $execucaoTotal = $db->carregar($sql);

        $saidaTabela[0]['tipo'] = 'Despesas Empenhadas (R$)';
        $saidaTabela[0]['2012'] = $execucaoTotal[searcharray('2012', 'ano', $execucaoTotal)]['empenhado'];
        $saidaTabela[0]['doismiletreze'] = $execucaoTotal[searcharray('2013', 'ano', $execucaoTotal)]['empenhado'];
        $saidaTabela[0]['doismilequatorze'] = $execucaoTotal[searcharray('2014', 'ano', $execucaoTotal)]['empenhado'];
        $saidaTabela[0]['doismilequinze'] = $execucaoTotal[searcharray('2015', 'ano', $execucaoTotal)]['empenhado'];

        $saidaTabela[1]['tipo'] = 'Valores Pagos (R$)';
        $saidaTabela[1]['2012'] = $execucaoTotal[searcharray('2012', 'ano', $execucaoTotal)]['pago'];
        $saidaTabela[1]['doismiletreze'] = $execucaoTotal[searcharray('2013', 'ano', $execucaoTotal)]['pago'];
        $saidaTabela[1]['doismilequatorze'] = $execucaoTotal[searcharray('2014', 'ano', $execucaoTotal)]['pago'];
        $saidaTabela[1]['doismilequinze'] = $execucaoTotal[searcharray('2015', 'ano', $execucaoTotal)]['pago'];

        $saidaTabela[2]['tipo'] = 'RAP não Processado Pago (R$)';
        $saidaTabela[2]['2012'] = $execucaoTotal[searcharray('2012', 'ano', $execucaoTotal)]['rap_npp'];
        $saidaTabela[2]['doismiletreze'] = $execucaoTotal[searcharray('2013', 'ano', $execucaoTotal)]['rap_npp'];
        $saidaTabela[2]['doismilequatorze'] = $execucaoTotal[searcharray('2014', 'ano', $execucaoTotal)]['rap_npp'];
        $saidaTabela[2]['doismilequinze'] = $execucaoTotal[searcharray('2015', 'ano', $execucaoTotal)]['rap_npp'];

        $saidaTabela[3]['tipo'] = 'RAP Processado Pago (R$)';
        $saidaTabela[3]['2012'] = $execucaoTotal[searcharray('2012', 'ano', $execucaoTotal)]['rap_pp'];
        $saidaTabela[3]['doismiletreze'] = $execucaoTotal[searcharray('2013', 'ano', $execucaoTotal)]['rap_pp'];
        $saidaTabela[3]['doismilequatorze'] = $execucaoTotal[searcharray('2014', 'ano', $execucaoTotal)]['rap_pp'];
        $saidaTabela[3]['doismilequinze'] = $execucaoTotal[searcharray('2015', 'ano', $execucaoTotal)]['rap_pp'];
                
        $comentario['2012'] = $execucaoTotal[searcharray('2012', 'ano', $execucaoTotal)]['comentario'];
        $comentario['2013'] = $execucaoTotal[searcharray('2013', 'ano', $execucaoTotal)]['comentario'];
        $comentario['2014'] = $execucaoTotal[searcharray('2014', 'ano', $execucaoTotal)]['comentario'];
        $comentario['2015'] = $execucaoTotal[searcharray('2015', 'ano', $execucaoTotal)]['comentario'];

        $colunas = array(
            '',
            '2012 ' . formataComentario($comentario['2012'], array('ano' => '2012')),
            '2013 ' . formataComentario($comentario['2013'], array('ano' => '2013')),
            '2014 ' . formataComentario($comentario['2014'], array('ano' => '2014')),
            '2015 ' . formataComentario($comentario['2015'], array('ano' => '2015'))
        );        
        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
        $listagem->setCabecalho($colunas)            
            ->addCallbackDeCampo(array('2012', 'doismiletreze', 'doismilequatorze','doismilequinze'), 'mascaraMoeda')
            ->addCallbackDeCampo('tipo', 'alinhaParaEsquerda')
            ->esconderColunas('comentario')
            ->setDados($saidaTabela)
            ->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA,array('2012', 'doismiletreze', 'doismilequatorze','doismilequinze'))
            ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
        
        /* Grafico */        
        global $db;
        $dados = $db->carregar($sql);
        $dadosGrafico[] = array('descricao' => "Despesas Empenhadas (R$)", 'categoria' => $dados[2]['ano'], 'valor' => $dados[2]['empenhado']);
        $dadosGrafico[] = array('descricao' => "Despesas Empenhadas (R$)", 'categoria' => $dados[1]['ano'], 'valor' => $dados[1]['empenhado']);
        $dadosGrafico[] = array('descricao' => "Despesas Empenhadas (R$)", 'categoria' => $dados[0]['ano'], 'valor' => $dados[0]['empenhado']);
        $dadosGrafico[] = array('descricao' => "Valores Pagos (R$)", 'categoria' => $dados[2]['ano'], 'valor' => $dados[2]['pago']);
        $dadosGrafico[] = array('descricao' => "Valores Pagos (R$)", 'categoria' => $dados[1]['ano'], 'valor' => $dados[1]['pago']);
        $dadosGrafico[] = array('descricao' => "Valores Pagos (R$)", 'categoria' => $dados[0]['ano'], 'valor' => $dados[0]['pago']);
        $dadosGrafico[] = array('descricao' => "RAP não Processado Pago (R$)", 'categoria' => $dados[2]['ano'], 'valor' => $dados[2]['rap_npp']);
        $dadosGrafico[] = array('descricao' => "RAP não Processado Pago (R$)", 'categoria' => $dados[1]['ano'], 'valor' => $dados[1]['rap_npp']);
        $dadosGrafico[] = array('descricao' => "RAP não Processado Pago (R$)", 'categoria' => $dados[0]['ano'], 'valor' => $dados[0]['rap_npp']);
        $dadosGrafico[] = array('descricao' => "RAP Processado Pago (R$)", 'categoria' => $dados[2]['ano'], 'valor' => $dados[2]['rap_pp']);
        $dadosGrafico[] = array('descricao' => "RAP Processado Pago (R$)", 'categoria' => $dados[1]['ano'], 'valor' => $dados[1]['rappp']);
        $dadosGrafico[] = array('descricao' => "RAP Processado Pago (R$)", 'categoria' => $dados[0]['ano'], 'valor' => $dados[0]['rap_pp']);
        
        $grafico = new Grafico(null, false);
        echo <<<HTML
        <div class="row">
            <div class="col-md-12">
HTML;
        echo $grafico->setTipo(Grafico::K_TIPO_COLUNA)->setId('graficoExecucao')->setTitulo('')
            ->setWidth('100%')
            ->gerarGrafico($dadosGrafico);
        echo <<<HTML
            </div>
        </div>
HTML;
        die();
    }
    ?>
</div>
