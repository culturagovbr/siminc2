<?php

    /**
     * Tela de detalhes do PI.
     *
     * @filesource
     */
    $pi = new Spo_Model_Planointerno();
    $select = 't1.*, t2.eqdcod, t2.eqddsc, t3.neecod, t3.needsc, t4.capcod, t4.capdsc,';
    $select .= 't5.arccod, t5.arcdsc, t6.unicod, t6.unidsc, t7.ungcod, t7.ungdsc,';
    $select .= 't8.sbacod, t8.sbadsc';

    $options = array(
        'join' => array('eqdid', 'secid', 'capid'),
        'leftjoin' => array('arceid', 'unicod', 'ungcod', 'sbaid')
    );
    $dadosPi = $pi->recuperarTodos($select, array("t1.pliid = {$_REQUEST['pliid']}"), '', $options);

    if (!empty($dadosPi)) {
        $dadosPi = current($dadosPi);
    }
    // -- Tratamento da subação
    if (empty($dadosPi['sbaid'])) {
        $sbacod = "Código livre - {$dadosPi['plicodsubacao']}";
    } else {
        $sbacod = "{$dadosPi['sbacod']} - {$dadosPi['sbatitulo']}";
    }

    // -- Tratamento da unidade orçamentária/unidade gestora
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
        <div style="width:100%;float:left">
            <table class="tabela table table-striped table-bordered table-hover text-center horizontal">
                <tr>
                    <th>Código do PI:</th>
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
                    <th>Modalidade de Pactuação:</th>
                    <td>{$dadosPi['capcod']} - {$dadosPi['capdsc']}</td>
                </tr>
                <tr>
                    <th>Área Cultural:</th>
                    <td>{$dadosPi['arccod']} - {$dadosPi['arcdsc']}</td>
                </tr>
                <tr>
                    <th>Título:</th>
                    <td>{$dadosPi['plititulo']}</td>
                </tr>
                <tr>
                    <th>Descrição do PI:</th>
                    <td>{$dadosPi['plidsc']}</td>
                </tr>
                <tr>
                    <th>Unidade Orçamentária:</th>
                    <td>{$unicod}</td>
                </tr>
                <tr>
                    <th>Unidade Gestora:</th>
                    <td>{$dadosPi['ungcod']} - {$dadosPi['ungdsc']}</td>
                </tr>
            </table>
        </div>
HTML;
    
$list = new Simec_Listagem();
$list->setQuery(Spo_Model_Planointerno::buscarSubUnidades((object) array(
    'exercicio' => $_SESSION['exercicio'],
    'pliid' => $dadosPi['pliid'] )))
    ->setTitulo('Lista de Sub-Unidades Delegadas')
//    ->setId('list-ptres')
    ->setCabecalho(array(
        'Unidade Orçamentária',
        'Unidade'))
//    ->esconderColunas('pliid', 'ptrid', 'acaid')
//    ->addCallbackDeCampo(
//        array('dotacaoinicial', 'dotacaosubacao', 'empenhado', 'detalhadoptres'),
//        'mascaraMoeda')
    ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
