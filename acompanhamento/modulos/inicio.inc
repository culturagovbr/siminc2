<?php

    include_once APPRAIZ. "includes/library/simec/Grafico.php";
    include_once APPRAIZ. "monitora/classes/Pi_PlanoInterno.class.inc";

    $cAcompanhamento = new Acompanhamento_Controller_Acompanhamento();
    $cAcompanhamentopnc = new Acompanhamento_Controller_Acompanhamentopnc();
    switch ($_REQUEST['req']) {
        case 'espelho-pi':
            include APPRAIZ. 'planacomorc/modulos/principal/unidade/espelho-pi.inc';
            die;
        case 'listar-metas-ppa':
            $cAcompanhamento->listarMetasPpa($_REQUEST['suoid'], $_REQUEST['janid']);
            die;
        case 'listar-metas-pnc':
            include_once APPRAIZ. 'acompanhamento/modulos/principal/listar-metas-pnc.inc';
            die;
        case 'listar-metas-estrategicas':
            include_once APPRAIZ. 'acompanhamento/modulos/principal/listar-metas-planejamento-estrategico.inc';
            die;
    }

    include APPRAIZ. "includes/cabecalho.inc";
    
    $mEmendas = new Emendas_Model_Emenda();
    $mAcompanhamento = new Acompanhamento_Model_Acompanhamento($_REQUEST['acoid']);

    $listaPerfis = pegaPerfilGeral();

    $listaSubunidadeUsuario = (new Public_Model_SubUnidadeOrcamentaria())->recuperarPorUsuario();
    $suoid = $_REQUEST['suoid'] ? $_REQUEST['suoid'] : key($listaSubunidadeUsuario);
    
    $listaPas = (new Pi_PlanoInterno())->recuperarPorUnidade((object)[
        'exercicio' => $_SESSION['exercicio'],
        'suoid' => $suoid,
        'naoOrcamentaria' => FALSE
    ]);
    
    $listaPasNaoOrcamentaria = (new Pi_PlanoInterno())->recuperarPorUnidade((object)[
        'exercicio' => $_SESSION['exercicio'],
        'suoid' => $suoid,
        'naoOrcamentaria' => TRUE
    ]);
?>

<script src="js/principal/acompanhamento.js?v=1"></script>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-4">
        <h2>Painel de Acompanhamento</h2>
    </div>
    <div class="col-lg-4">
        <div class="form-horizontal" style="padding-top: 10px;">
            <?php echo $simec->select('suoid', 'Unidade', $suoid, $listaSubunidadeUsuario, null, ['label-size'=>2]); ?>
        </div>
    </div>
    <div class="col-lg-4">
        <div style="padding-top: 10px;" class="text-right">
            <a class="btn btn-danger" href="?modulo=principal/monitorar-ppa&acao=A&suoid=<?php echo $suoid; ?>">
                <i class="fa fa-list"></i> Monitorar PPA
            </a>
            <a class="btn btn-success" href="?modulo=principal/monitorar-pnc&acao=A&suoid=<?php echo $suoid; ?>">
                <i class="fa fa-list"></i> Monitorar PNC
            </a>
        </div>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <div class="col-md-12" id="box-metas-ppa">
            <?php $cAcompanhamento->listarMetasPpa($suoid); ?>
        </div>
        <div class="col-md-12" id="box_planejamento_estrategico">
            <?php $cAcompanhamento->listarMetasPlanejamentoEstrategico($suoid); ?>
        </div>
        <div class="col-md-6">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Meus Planos de Ação <span style="color: red;">(<?php echo count($listaPas); ?>)</span></h5>
                </div>
                <div class="ibox-content">
                    <?php
                    $aObjetivos = [];
                    if(count($listaPas)){ ?>
                        <div class="table-responsive">
                            <table class="table table-striped table-bordered table-hover table-condensed" >
                                <thead>
                                <tr class="text-center">
                                    <th>Status</th>
                                    <th>PI</th>
                                    <th>Título</th>
                                    <th>Vl. Total (R$)</th>
                                    <th>Físico</th>
                                    <th>Unidade</th>
                                    <th>Produto</th>
                                </tr>
                                </thead>
                                <tbody>
                                <?php
                                $total = 0;

                                foreach($listaPas as $dados){
                                    # Cria icones e url de acesso conforme o preenchimento.
                                    if($dados['acoid']){
                                        $url = '?modulo=principal/monitorar-pi&acao=A&pliid='. $dados['pliid'];
                                        $icone = '<span class="label label-primary">Preenchida</span>';
                                    } else {
                                        $url = '?modulo=principal/monitorar-pi&acao=A&pliid='. $dados['pliid']. '&suoid='. $suoid;
                                        $icone = '<span class="label label-danger">Pendente</span>';
                                    }

                                    # Caso seja perfil PNC não exibe o link de acesso a pagina de edição
                                    if(in_array(PFL_PNC, $listaPerfis)){
                                        $botao = $icone;
                                    } else {
                                        $botao = '<a href="'. $url. '">'. $icone. '</a>';
                                    }

                                    if($dados['oppcod']){
                                        $aObjetivos[$dados['oppcod']] = $dados;
                                    }
                                    $total += $dados['valor'];
                                ?>
                                    <tr>
                                        <td><?php echo $botao; ?></td>
                                        <td>
                                            <a href="javascript: exibirEspelhoPi('<?php echo $dados['pliid']; ?>')">
                                                <?php echo $dados['plicod'] ?>
                                            </a>
                                        </td>
                                        <td>
                                            <a href="javascript: exibirEspelhoPi('<?php echo $dados['pliid']; ?>')">
                                                <?php echo ($dados['unofundo'] == 't' ? "<span class='label label-dark'>FNC</span>  " : ''). $dados['plititulo']; ?>
                                            </a>
                                        </td>
                                        <td class="text-right"><?php echo number_format($dados['valor'], 0, ',', '.'); ?></td>
                                        <td class="text-right"><?php echo number_format($dados['picquantidade'], 0, ',', '.'); ?></td>
                                        <td><?php echo $dados['pumnome'] ?></td>
                                        <td><?php echo $dados['pprnome'] ?></td>
                                    </tr>
                                <?php } ?>
                                </tbody>
                                <tfoot>
                                <tr>
                                    <th colspan="3">TOTAL</th>
                                    <th class="text-right"><?php echo number_format($total, 2, ',', '.'); ?></th>
                                    <th colspan="3"></th>
                                </tr>
                                </tfoot>
                            </table>
                        </div>
                    <?php } else {
                        echo '<div class="alert alert-danger">Nenhum registro cadastrado</div>';
                    } ?>
                </div>
            </div>
        </div>
        <div class="col-md-6">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Meus Planos de Ação(Não Orçamentários) <span style="color: red;">(<?php echo count($listaPasNaoOrcamentaria); ?>)</span></h5>
                </div>
                <div class="ibox-content">
                    <?php
                    $aObjetivos = [];
                    if(count($listaPasNaoOrcamentaria)){ ?>
                        <div class="table-responsive">
                            <table class="table table-striped table-bordered table-hover table-condensed" >
                                <thead>
                                    <tr class="text-center">
                                        <th>Status</th>
                                        <th>ID</th>
                                        <th>Título</th>
                                        <th>Quantidade Prevista</th>
                                        <th>Executado</th>
                                        <th>Produto</th>
                                        <th>Classificação</th>
                                    </tr>
                                </thead>
                                <tbody>
                                <?php
                                $total = 0;

                                foreach($listaPasNaoOrcamentaria as $dados){
                                    # Cria icones e url de acesso conforme o preenchimento.
                                    if($dados['acoid']){
                                        $url = '?modulo=principal/monitorar-pi&acao=A&pliid='. $dados['pliid'];
                                        $icone = '<span class="label label-primary">Preenchida</span>';
                                    } else {
                                        $url = '?modulo=principal/monitorar-pi&acao=A&pliid='. $dados['pliid']. '&suoid='. $suoid;
                                        $icone = '<span class="label label-danger">Pendente</span>';
                                    }

                                    # Caso seja perfil PNC não exibe o link de acesso a pagina de edição
                                    if(in_array(PFL_PNC, $listaPerfis)){
                                        $botao = $icone;
                                    } else {
                                        $botao = '<a href="'. $url. '">'. $icone. '</a>';
                                    }

                                    if($dados['oppcod']){
                                        $aObjetivos[$dados['oppcod']] = $dados;
                                    }
                                    $total += $dados['valor'];
                                ?>
                                    <tr>
                                        <td><?php echo $botao; ?></td>
                                        <td>
                                            <a href="javascript: exibirEspelhoPi('<?php echo $dados['pliid']; ?>')"><?php echo $dados['pliid'] ?></a>
                                        </td>
                                        <td>
                                            <a href="javascript: exibirEspelhoPi('<?php echo $dados['pliid']; ?>')">
                                            <?php echo ($dados['unofundo'] == 't' ? "<span class='label label-dark'>FNC</span>  " : ''). $dados['plititulo']; ?>
                                            </a>
                                        </td>
                                        <td class="text-right"><?php echo number_format($dados['picquantidade'], 0, ',', '.'); ?></td>
                                        <td class="text-right"><?php echo number_format($dados['acoquantidade'], 0, ',', '.'); ?></td>
                                        <td><?php echo $dados['pprnome'] ?></td>
                                        <td><?php echo $dados['cladsc'] ?></td>
                                    </tr>
                                <?php } ?>
                                </tbody>
                                <tfoot>
                                <tr>
                                    <th colspan="3">TOTAL</th>
                                    <th class="text-right"><?php echo number_format($total, 2, ',', '.'); ?></th>
                                    <th colspan="3"></th>
                                </tr>
                                </tfoot>
                            </table>
                        </div>
                    <?php } else {
                        echo '<div class="alert alert-danger">Nenhum registro cadastrado</div>';
                    } ?>
                </div>
            </div>
        </div>
        <div class="col-md-12" id="box-metas-pnc">
            <?php include_once APPRAIZ. 'acompanhamento/modulos/principal/listar-metas-pnc.inc'; ?>
        </div>
        <div class="col-md-6">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <?php $aPos = (new Spo_Model_PtresSubunidade())->recuperarPorUnidade($suoid); ?>
                    <h5>Meus Planos Orçamentários <span style="color: red;">(<?php echo count($aPos); ?>)</span></h5>
                </div>
                <div class="ibox-content">
                    <?php
                    if(count($listaPas)){ ?>
                        <div class="table-responsive">
                            <table class="table table-striped table-bordered table-hover table-condensed" >
                                <thead>
                                <tr class="text-center">
                                    <th>Funcional</th>
                                </tr>
                                </thead>
                                <tbody>
                                <?php foreach($aPos as $dados){ ?>
                                    <tr>
                                        <td><?php echo $dados['funcional'] . ' - ' . $dados['plodsc'] ?></td>
                                    </tr>
                                <?php } ?>
                                </tbody>
                            </table>
                        </div>
                    <?php } else {
                        echo '<div class="alert alert-danger">Nenhum registro cadastrado</div>';
                    } ?>
                </div>
            </div>
        </div>
        <div class="col-md-6">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Objetivos PPA que Contribuo <span style="color: red;">(<?php echo count($aObjetivos); ?>)</span></h5>
                </div>
                <div class="ibox-content">
                    <?php
                    if(count($aObjetivos)){ ?>
                        <div class="table-responsive">
                            <table class="table table-striped table-bordered table-hover table-condensed" >
                                <thead>
                                <tr class="text-center">
                                    <th>Objetivo</th>
                                </tr>
                                </thead>
                                <tbody>
                                <?php
                                $total = 0;
                                foreach($aObjetivos as $dados){ ?>
                                    <tr>
                                        <td><?php echo $dados['oppcod'] . ' - ' . $dados['oppnome'] ?></td>
                                    </tr>
                                <?php } ?>
                                </tbody>
                            </table>
                        </div>
                    <?php } else {
                        echo '<div class="alert alert-danger">Nenhum registro cadastrado</div>';
                    } ?>
                </div>
            </div>
        </div>
    </div>
</div>

<!-- Modal -->
<div class="modal fade" id="detalhe-pi" tabindex="-1" role="dialog" aria-labelledby="myModalLabel">
    <div class="modal-dialog modal-lg" role="document">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
                <h4 class="modal-title" id="myModalLabel">Pi's Vinculados</h4>
            </div>
            <div class="modal-body">

            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
            </div>
        </div>
    </div>
</div>

<script>
    $(function(){
        $('#suoid').change(function(){
            window.location.href = '?modulo=inicio&acao=C&suoid=' + $(this).val();
        });

        $('body').on('click', '.detalhe-pi', function(){
            var url = $(this).attr('href');
            $('#detalhe-pi .modal-body').load(url);
            $('#detalhe-pi').modal();
            return false;
        });
    });

    /**
     * Exibe popup com Detalhes do pi. Tela de Espelho de PI.
     *
     * @param pliid int
     * @returns VOID
     */
    function exibirEspelhoPi(pliid){
        window.open(
            '?modulo=inicio&acao=C&req=espelho-pi&pliid='+ pliid,
            'popup_espelho_pi',
            'width=780,height=1000,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
    }
</script>
