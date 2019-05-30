<?php

$requisicao = $_REQUEST['req'];
$filtroEmenda = $_SESSION['filtroEmenda'] = ($_POST? $_POST: $_SESSION['filtroEmenda']);

$perfis = pegaPerfilGeral();
$mEmenda = new Emendas_Model_Emenda($_REQUEST['emeid']);
$podeEditar = !in_array(PFL_SUBUNIDADE, $perfis)? TRUE: FALSE;

# Verifica se o usuário tem vinculo de Sub-Unidades no seu Perfil
$listaSubUnidadeUsuario = buscarSubUnidadeUsuario((object) array('usucpf' => $_SESSION['usucpf']));

# Filtros padrões(default)
$filtroEmenda['exercicio'] = $_SESSION['exercicio'];
$filtroEmenda['listaSubUnidadeUsuario'] = $listaSubUnidadeUsuario;

switch ($requisicao) {
    case 'carregarAcaoPorUo':
        $sqlPtres = (new Monitora_Model_Ptres())->recuperarSqlCombo([
            "'(Ptres: ' || ptres.ptres || ')'",
            "coalesce(ptres.prgcod, '')",
            "coalesce(ptres.acacod, '')",
            "coalesce(ptres.loccod, '')",
            "coalesce(acaobjetivocod, '')",
            "coalesce(ptres.plocod, '')",
            "acatitulo"
        ],
            [
                "plocod = 'EIND'",
                "acao.unicod = '". (int)$_REQUEST['unicod']. "'"
            ]);
        echo $simec->select('ptrid', 'Funcional <span class="campo-obrigatorio" title="Campo obrigatório">*</span>', null, $sqlPtres);
        die;
    case 'proposta_modal':
        include 'proposta_modal.inc';
        die;
    case 'salvar-proposta':
        $cProposta = new Emendas_Controller_SiconvBeneficiario();
        $cProposta->salvar($_REQUEST);
        die;
    case 'limpar':
        $filtroEmenda = $_SESSION['filtroEmenda'] = array();
        $filtroEmenda['exercicio'] = $_SESSION['exercicio'];
        $filtroEmenda['listaSubUnidadeUsuario'] = $listaSubUnidadeUsuario;
        break;
    case 'listaremendas-xls':
        $aEmenda = (new Emendas_Model_Emenda)->recuperarListagem((object)$filtroEmenda);
        include_once APPRAIZ. "emendas/modulos/principal/emenda-xls.inc";
        die;
        break;
    case 'carregarComboUG':
        echo $simec->select(
            'suoid',
            'SubUnidade Beneficiário',
            $filtroEmenda['suoid'],
            Public_Model_SubUnidadeOrcamentaria::queryComboChaveSuoid((object) array(
                'exercicio' => $_SESSION['exercicio'],
                'unicod' => $filtroEmenda['unicod']
            )));

        die();
        break;
}

include APPRAIZ . "includes/cabecalho.inc";
$filtroEmenda['exercicio'] = $_SESSION['exercicio'];
$aEmenda = (new Emendas_Model_Emenda)->recuperarListagem((object)$filtroEmenda);
?>

<script src="js/principal/emenda.js?v=5"></script>
<style>
    .text-red {
        color: red !important;
    }
</style>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-12">
        <h2><?php echo $titulo_modulo; ?></h2>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <div class="col-md-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Pesquisa avançada por Emenda</h5>
                </div>
                <div class="ibox-content">
                    <form id="formFiltroEmenda" name="formFiltroEmenda" method="POST" role="form" class="form-horizontal">
                        <input name="req" id="req" value="" type="hidden">

                        <?php
                       /* if(!$listaSubUnidadeUsuario){
                            echo $simec->select('unicod', 'Unidade Emenda', $filtroEmenda['unicod'], Spo_Model_Unidade::queryCombo((object)array(
                                'prsano::INTEGER = '. (int)$_SESSION['exercicio']) ));
                        }*/
                        echo '<div id=div_ungcod>';
                        echo $simec->select('suoid', 'SubUnidade Beneficiário', $filtroEmenda['suoid'], Public_Model_SubUnidadeOrcamentaria::queryComboChaveSuoid((object)array(
                            'exercicio' => $_SESSION['exercicio'],
                            'unicod' => $filtroEmenda['unicod'],
                            'listaSubUnidadeUsuario' => $listaSubUnidadeUsuario
                        )));
                        echo '</div>';
                        echo '<div id=div_ungcod_delegada>';
                        echo $simec->select('suoid_delegada', 'SubUnidade Delegada', $filtroEmenda['suoid_delegada'], Public_Model_SubUnidadeOrcamentaria::queryComboChaveSuoid((object)array(
                            'exercicio' => $_SESSION['exercicio'],
                            'unicod' => $filtroEmenda['unicod_delegada'],
                            'listaSubUnidadeUsuario' => $listaSubUnidadeUsuario
                        )));
                        echo '</div>';
                        $sqlPtres = (new Monitora_Model_Ptres())->recuperarSqlCombo([
                            "'(Ptres: ' || ptres.ptres || ')'",
                            "coalesce(ptres.prgcod, '')",
                            "coalesce(ptres.acacod, '')",
                            "coalesce(ptres.loccod, '')",
                            "coalesce(acaobjetivocod, '')",
                            "coalesce(ptres.plocod, '')",
                            "acatitulo"
                        ], [
                            "plocod = 'EIND'",
                            "acao.unicod = '". (int)$filtroEmenda['unicod']. "'"
                        ]);
                        echo '<span id="span-funcional">'.
                            $simec->select(
                                'ptrid',
                                'Funcional',
                                $filtroEmenda['ptrid'],
                                $sqlPtres
                            ).
                            '</span>';
                        echo $simec->select('lista_esdid', 'Situação', $filtroEmenda['lista_esdid'], Spo_Model_Unidade::queryComboEstados((object) array('tpdid' => WF_TPDID_BENEFICIARIO)), array('multiple' => 'true'));
                        $modelSiconvSituacao = new Emendas_Model_SiconvSituacao();
                        $sqlSituacaoSiconv = $modelSiconvSituacao->recuperarSqlCombo(['sitid', 'sitdsc'], NULL, 'sitid');
                        echo $simec->select('lista_sitid', 'Situação SICONV', $filtroEmenda['lista_sitid'], $sqlSituacaoSiconv, array('multiple' => 'true'));
                        echo $simec->radio('benimpedimento', 'Impedimento', $filtroEmenda['benimpedimento'], ['t' => 'Sim', 'f' => 'Não']);
                        ?>

                        <div class="form-group">
                            <div class="text-center">
                                <button id="btn-limpar" class="btn btn-warning" type="button">
                                    <span class="glyphicon glyphicon-remove-circle"></span> Limpar
                                </button>
                                <button class="btn btn-primary btn-buscar" type="submit">
                                    <span class="glyphicon glyphicon-search"></span> Buscar
                                </button>
                                <button id="btn-exportar-xls" class="btn btn-primary btn-buscar" type="button">
                                    <i class="fa fa-file-excel-o"></i> Exportar XLS
                                </button>
                                <?php if($podeEditar): ?>
                                    <button id="btn_novo" class="btn btn-success" type="button">
                                        <span class="glyphicon glyphicon-plus"></span>
                                        Novo
                                    </button>
                                <?php endif; ?>
                            </div>
                        </div>

                    </form>
                </div>
            </div>
        </div>
    </div>

    <div class="row">
        <div class="col-md-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Dados Gerais</h5>
                </div>
                <div class="ibox-content">

                    <div class="table-responsive">
                        <table class="table table-bordered table-hover dataTablesSP" >
                            <thead>
                            <tr class="text-center">
                                <th>Emenda</th>
                                <th width="10%">Unidade</th>
                                <th width="10%">Autor</th>
                                <th>UF</th>
                                <th>Id Beneficiário</th>
                                <th>Beneficiário</th>
                                <th width="10%">Subunidade</th>
                                <th>Situação</th>
                                <th>Nº Proposta</th>
                                <th>Situação SICONV</th>
                                <th>PI</th>
                                <th>Valor</th>
                                <th>Empenhado</th>
                                <th>Pago</th>
                            </tr>
                            </thead>
                            <tbody>
                            <?php foreach($aEmenda as $dados){?>
                                <tr <?php echo $dados['impid']? 'class="text-red"': NULL; ?> >
                                    <td>
                                        <a title="Emenda" href="?modulo=principal/emenda_form&acao=A&emeid=<?php echo $dados['emeid']; ?>">
                                            <?php echo $dados['emenumero']; ?>
                                        </a>
                                    </td>
                                    <td><?php echo $dados['unidade']; ?></td>
                                    <td><?php echo $dados['autnome']; ?></td>
                                    <td><?php echo $dados['estuf']; ?></td>
                                    <td>
                                        <?php if($dados['benid']): ?>
                                            <a title="Beneficiário" href="?modulo=principal/beneficiario_form&acao=A&benid=<?php echo $dados['benid']; ?>" style="margin-left: 5px;"><?php echo $dados['benid']; ?></a>
                                        <?php else: ?>
                                            <a title="Beneficiário" href="?modulo=principal/beneficiario_form&acao=A&emeid=<?php echo $dados['emeid']; ?>" style="margin-left: 5px;"><?php echo $dados['benid']; ?></a>
                                        <?php endif; ?>
                                    </td>
                                    <td><?php echo $dados['pronome']; ?></td>
                                    <td class="link" title="<?php echo "Subunidade(s) Delegada(s): \n ". $dados['subunidade_delegada']; ?>" >
                                        <?php echo $dados['subunidade']; ?>
                                    </td>
                                    <td><?php echo $dados['esddsc']; ?></td>
                                    <td><?php echo $dados['numero_proposta']; ?></td>
                                    <td>
                                        <?php if($podeEditar): ?>
                                        <!-- Se Situação SICONV(Parecer/Proposta) tiver mais de uma proposta no mesmo CNPJ e na mesma Emenda(Número de emenda), Exibe o ícone da tela modal de Selecionar Proposta -->
                                        <?php if($dados['qtdrepetida']): ?>
                                        <span class="editar_situacao fa fa-pencil detalhar-linha link" data-benid="<?php echo $dados['benid']; ?>" title="Selecionar Proposta">
                                            <?php endif; ?>
                                            <?php endif; ?>
                                            <?php echo $dados['sitdsc']; ?>
                                      </span>
                                    </td>
                                    <td>
                                        <?php echo $dados['pliid']? "<span class='link a_espelho' title='Exibir dados do Plano Interno' data-pi='{$dados['pliid']}'>{$dados['pliid']}</span>": NULL; ?>
                                    </td>
                                    <td class="link" title="<?php echo ' Custeio: '. number_format($dados['custeio'], 2, ',', '.'). "\n Capital: ". number_format($dados['capital'], 2, ',', '.'); ?>"><?php echo number_format(($dados['custeio'] + $dados['capital']), 2, ',', '.'); ?></td>
                                    <td><?php echo number_format($dados['empenhado'], 2, ',', '.'); ?></td>
                                    <td><?php echo number_format($dados['pago'], 2, ',', '.'); ?></td>
                                </tr>
                            <?php } ?>
                            </tbody>
                        </table>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>

<?php bootstrapPopup("Selecionar Proposta", 'proposta_modal', '', array('salvar'), array('tamanho' => 'lg')); ?>

<script>

    $(document).ready(function(){

        initListaEmendas();

    });

</script>