<?php

    /**
     * Tela de detalhes do PI.
     *
     * @filesource
     */
    $pi = new Spo_Model_Planointerno();
    $select = 't1.*, t2.eqdcod, t2.eqddsc, t3.neecod, t3.needsc, t4.capcod, t4.capdsc,';
    $select .= 't5.mdecod, t5.mdedsc, t6.unicod, t6.unidsc, t7.ungcod, t7.ungdsc,';
    $select .= 't8.sbacod, t8.sbadsc';

    $options = array(
        'join' => array('eqdid', 'neeid', 'capid'),
        'leftjoin' => array('mdeid', 'unicod', 'ungcod', 'sbaid')
    );
    $dadosPi = $pi->recuperarTodos($select, array("t1.pliid = {$_REQUEST['pliid']}"), '', $options);

    if (!empty($dadosPi)) {
        $dadosPi = current($dadosPi);
    }
    // -- Tratamento da suba��o
    if (empty($dadosPi['sbaid'])) {
        $sbacod = "C�digo livre - {$dadosPi['plicodsubacao']}";
    } else {
        $sbacod = "{$dadosPi['sbacod']} - {$dadosPi['sbatitulo']}";
    }

    // -- Tratamento da unidade or�ament�ria/unidade gestora
    if (!empty($dadosPi['unicod'])) {
        $unicod = "{$dadosPi['unicod']} - {$dadosPi['unidsc']}";
    } elseif (!empty($dadosPi['ungcod'])) {
        $dadosUnicod = $db->pegaLinha(Spo_Model_Unidade::getQueryUnidadeDaUngestora($dadosPi['ungcod']));
        $unicod = "{$dadosUnicod['unicod']} - {$dadosUnicod['unidsc']}";
    } else {
        $unicod = ' - ';
    }

    echo <<<HTML
<style>
    .modal-dialog{width:80%}
    button{cursor:pointer}
    table.horizontal th{text-align:right;background-color:#EFEFEF!important}
</style>
<div style="width:50%;float:left">
    <table class="tabela table table-striped table-bordered table-hover text-center horizontal">
        <tr>
            <th>C�digo do PI:</th>
            <td><b>{$dadosPi['plicod']}</b></td>
        </tr>
        <tr>
            <th>Enquadramento da Despesa:</th>
            <td>{$dadosPi['eqdcod']} - {$dadosPi['eqddsc']}</td>
        </tr>
        <tr>
            <th>Segmento Cultural:</th>
            <td>{$dadosPi['neecod']} - {$dadosPi['needsc']}</td>
        </tr>
        <tr>
            <th>Modalidade de Pactua��o:</th>
            <td>{$dadosPi['capcod']} - {$dadosPi['capdsc']}</td>
        </tr>
<!--
        <tr>
            <th>Codifica��o da Unidade(livre):</th>
            <td>{$dadosPi['plilivre']}</td>
        </tr>
!-->
        <tr>
            <th>�rea Cultural:</th>
            <td>{$dadosPi['mdecod']} - {$dadosPi['mdedsc']}</td>
        </tr>
        <tr>
            <th>T�tulo:</th>
            <td>{$dadosPi['plititulo']}</td>
        </tr>
        <tr>
            <th>Descri��o do PI:</th>
            <td>{$dadosPi['plidsc']}</td>
        </tr>
        <tr>
            <th>Unidade Or�ament�ria:</th>
            <td>{$unicod}</td>
        </tr>
        <tr>
            <th>Unidade Gestora:</th>
            <td>{$dadosPi['ungcod']} - {$dadosPi['ungdsc']}</td>
        </tr>
    </table>
</div>
HTML;
    // -- Grafico financeiro do PI
    echo <<<HTML
<div id="graficoDestino" style="width:50%;float:left;overflow:hidden">
HTML;
    $params = array(
        'exercicio' => $_SESSION['exercicio'],
        'unicod' => $dadosPi['unicod'],
        'plicod' => $dadosPi['plicod']
    );
    $grafico = new Grafico(null, false);
    $grafico->setTipo(Grafico::K_TIPO_COLUNA)
        ->setId('graficoPi')
        ->setWidth('450px')
        ->setTitulo('Execu��o Or�ament�ria')
        ->gerarGrafico(Spo_Model_Planointerno::queryGraficoFinanceiro($params));
    echo <<<HTML
</div>
<div style="clear:both" />
HTML;
    $params = array(
        'exercicio' => $_SESSION['exercicio'],
        'pliid' => $dadosPi['pliid']
    );
    $list = new Simec_Listagem();
    $list->setQuery(Spo_Model_Planointerno::queryAcoes($params))
        ->setTitulo('Detalhamento Or�ament�rio')
        ->setId('list-ptres')
        ->setCabecalho(array(
            'PTRES',
            'A��o',
            'Financeiro (R$)' => array(
                'Dota��o do PTRES',
                'Empenhado no PI',
                'Detalhado no PTRES')))
        ->esconderColunas('pliid', 'ptrid', 'acaid')
        ->addCallbackDeCampo(
            array('dotacaoinicial', 'dotacaosubacao', 'empenhado', 'detalhadoptres'),
            'mascaraMoeda')
        ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
