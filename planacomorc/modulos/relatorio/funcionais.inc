<?php

    include_once '_funcoespi.php';
    include_once APPRAIZ. "monitora/classes/Pi_PlanoInterno.class.inc";

    $filtros = array(
        "suo.prsano = '". $_SESSION['exercicio']. "'",
        'suo.unofundo = FALSE',
    );
    # Filtro por Subunidades do perfil do usuário logado.
    if($listaSubUnidadeUsuario){
        $filtros[] = "suo.suocod IN('". join("','", $listaSubUnidadeUsuario). "')";
    }

    $modelPlanoInterno = new Pi_PlanoInterno();
    $sql = $modelPlanoInterno->montarSqlRelFuncionais($filtros);
//ver($sql, d);
    $listaFuncionais = $modelPlanoInterno->carregar($sql);

    if($_REQUEST['req'] == 'exportar-xls'){
        include_once APPRAIZ. "planacomorc/modulos/relatorio/funcionais-xls.inc";
    }

    include APPRAIZ . "includes/cabecalho.inc";

?>

<style>
    
    .title-disponivel{
        cursor: help;
    }

</style>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-4">
        <h2><?php echo $titulo_modulo; ?></h2>
    </div>
    <div class="col-lg-8">
        <div style="padding-top: 10px;" class="text-right">
            <a class="btn btn-primary" href="planacomorc.php?modulo=relatorio/funcionais&acao=A&req=exportar-xls">
                <i class="fa fa-file-excel-o"></i> Exportar XLS
            </a>
        </div>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <div class="col-md-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Acompanhamento de Plano Interno</h5>
                </div>
                <div class="ibox-content">

                    <div class="table-responsive">
                        <table class="table table-bordered table-hover dataTablesFuncionais">
                            <thead>
                                <tr class="text-center">
                                    <th rowspan="2" style="border-left: 1px black solid;">
                                        Unidade
                                    </th>
                                    <th rowspan="2" style="border-right: 1px black solid;">
                                        Subunidade
                                    </th>
                                    <th rowspan="2">
                                        PTRES
                                    </th>
                                    <th rowspan="2" style="border-right: 1px black solid;">
                                        Funcional
                                    </th>
                                    <th colspan="2" style="border-right: 1px black solid;">Custeio</th>
                                    <th colspan="2" style="border-right: 1px black solid;">Capital</th>
                                    <th colspan="2" style="border-right: 1px black solid;">Total</th>
                                    <th colspan="4" style="border-right: 1px black solid;">&nbsp;</th>
                                </tr>
                                <tr class="text-center">
                                    <th>Dotação</th>
                                    <th style="border-right: 1px black solid;">Planejado</th>
                                    <th>Dotação</th>
                                    <th style="border-right: 1px black solid;">Planejado</th>
                                    <th>Dotação</th>
                                    <th style="border-right: 1px black solid;">Planejado</th>
                                    <th>Provisionado</th>
                                    <th>Empenhado</th>
                                    <th>Liquidado</th>
                                    <th style="border-right: 1px black solid;">Pago</th>
                                </tr>
                            </thead>
                            <?php if($listaFuncionais): ?>
                                <tbody>
                                <?php foreach($listaFuncionais as $dado): ?>
                                    <tr class="tr_<?php echo $dado['suoid']; ?> tr_detalhe">
                                        <td style="border-left: 1px black solid;">
                                            <span class="detalhe-subunidade link" data-suoid="<?php echo $dado['suoid']; ?>" title="Visualizar PIs dessa Unidade">
                                                <?php echo $dado['unosigla']. ' (' . $dado['unocod'] . ')'; ?>
                                            </span>
                                        </td>
                                        <td style="border-right: 1px black solid;">
                                            <?php echo $dado['suonome']. ' ('. $dado['suocod']. ')'; ?>
                                        </td>
                                        <td>
                                            <?php echo $dado['ptres']; ?>
                                        </td>
                                        <td style="padding-left: 30px; border-right: 1px black solid;" >
                                            <span class="detalhe-funcional link" data-ptrid="<?php echo $dado['ptrid']; ?>" data-suoid="<?php echo $dado['suoid']; ?>" title="Visualizar lista de PIs com essa funcional">
                                                <?php echo $dado['funcional'] . ' - ' . $dado['acatitulo'] . ': ' . $dado['plodsc'] . ' (RP ' . $dado['irpcod'] . ')'; ?>
                                            </span>
                                            <?php if($dado['compartilhada'] > 1): ?>
                                                &nbsp;<i class="fa fa-handshake-o link" title="Funcional Compartilhada com outras Subunidades."></i>
                                            <?php endif; ?>
                                        </td>
                                        <td class="text-right"><?php echo number_format($dado['ptrdotacao_custeio'], 0, ',', '.'); ?></td>
                                        <?php $disponivel = $dado['ptrdotacao_custeio'] - $dado['picvalorcusteio']; ?>
                                        <td style="color: <?php echo $modelPlanoInterno->getCorPainelPorValorDisponivel($disponivel); ?>; border-right: 1px black solid;" class="text-right title-disponivel" title="Disponível: <?php echo number_format($disponivel, 0, ',', '.'); ?>">
                                            <?php echo number_format($dado['picvalorcusteio'], 0, ',', '.'); ?>
                                        </td>
                                        <td class="text-right"><?php echo number_format($dado['ptrdotacao_capital'], 0, ',', '.'); ?></td>
                                        <?php $disponivel = $dado['ptrdotacao_capital'] - $dado['picvalorcapital']; ?>
                                        <td style="color: <?php echo $modelPlanoInterno->getCorPainelPorValorDisponivel($disponivel); ?>; border-right: 1px black solid;" class="text-right title-disponivel" title="Disponível: <?php echo number_format($disponivel, 0, ',', '.'); ?>">
                                            <?php echo number_format($dado['picvalorcapital'], 0, ',', '.'); ?>
                                        </td>
                                        <td class="text-right">
                                            <?php echo number_format(($dado['ptrdotacao_custeio'] + $dado['ptrdotacao_capital']), 0, ',', '.'); ?>
                                        </td>
                                        <?php $disponivel = ($dado['ptrdotacao_custeio'] + $dado['ptrdotacao_capital']) - ($dado['picvalorcusteio'] + $dado['picvalorcapital']); ?>
                                        <td style="color: <?php echo $modelPlanoInterno->getCorPainelPorValorDisponivel($disponivel); ?>; border-right: 1px black solid;" class="text-right title-disponivel" title="Disponível: <?php echo number_format($disponivel, 0, ',', '.'); ?>">
                                            <?php echo number_format(($dado['picvalorcusteio'] + $dado['picvalorcapital']), 0, ',', '.'); ?>
                                        </td>
                                        <td class="text-right">	
                                            <?php echo number_format(($dado['provisionado']), 0, ',', '.'); ?>	
                                        </td>	
                                        <td class="text-right">	
                                            <?php echo number_format(($dado['empenhado']), 0, ',', '.'); ?>	
                                        </td>	
                                        <td class="text-right">	
                                            <?php echo number_format(($dado['liquidado']), 0, ',', '.'); ?>	
                                        </td>	
                                        <td style="border-right: 1px black solid;" class="text-right">	
                                            <?php echo number_format(($dado['pago']), 0, ',', '.'); ?>	
                                        </td>
                                    </tr>
                                <?php endforeach; ?>
                                </tbody>
                            <?php endif; ?>
                        </table>
                    </div>

                </div>
            </div>
        </div>
    </div>
</div>

<?php
// -- popup de detalhamento do PI
bootstrapPopup("PI's da Unidade", 'detalhe-subunidade', '', array('fechar'), array('tamanho' => 'lg'));
bootstrapPopup("PI's da Funcional na Unidade", 'detalhe-funcional', '', array('fechar'), array('tamanho' => 'lg'));
bootstrapPopup("Detalhe da Unidade", 'detalhe-grafico', '', array('fechar'), array('tamanho' => 'lg'));
?>

<script>
    $(document).ready(function(){

        $('.dataTablesFuncionais').DataTable({
            bPaginate: false,
            responsive: true,
            dom: '<"html5buttons"B>lTfgitp',
            "language": {
                "url": "/zimec/public/temas/simec/js/plugins/dataTables/Portuguese-Brasil.json"
            }
        });

        $('.tabs').on('click', '.tablink, .prodTabs a',function (e) {
            e.preventDefault();
            var url = $(this).attr("data-url");

            if (typeof url !== "undefined") {
                var pane = $(this), href = this.hash;

                // ajax load from data-url
                $(href).empty().load(url, function(result){      
                    pane.tab('show');
                });
            } else {
                $(this).tab('show');
            }
        });

        $('.detalhe-subunidade').click(function(){
            $('#detalhe-subunidade .modal-body').load('planacomorc.php?modulo=principal/unidade/painel&acao=A&req=detalhe-subunidade&suoid='+ $(this).data('suoid'));
            $('#detalhe-subunidade').modal();
        });

        $('.detalhe-funcional').click(function(){
            $('#detalhe-funcional .modal-body').load('planacomorc.php?modulo=principal/unidade/painel&acao=A&req=detalhe-funcional&ptrid='+ $(this).data('ptrid')+ '&suoid='+ $(this).data('suoid'));
            $('#detalhe-funcional').modal();
        });

        $('.detalhar-linha').click(function(){

            var suoid = $(this).data('suoid');

            if($(this).hasClass('fa-minus')){
                $(this).removeClass('fa-minus').addClass('fa-plus');
                $('.tr_' + suoid).hide();
            } else {
                $(this).removeClass('fa-plus').addClass('fa-minus');
                $('.tr_' + suoid).show('slow');
            }
        });

        $('.detalhar-todos').click(function(){

            if($(this).hasClass('fa-minus')){
                $(this).removeClass('fa-minus').addClass('fa-plus');
                $('.detalhar-linha').removeClass('fa-minus').addClass('fa-plus');
                $('.tr_detalhe').hide();
            } else {
                $(this).removeClass('fa-plus').addClass('fa-minus');
                $('.detalhar-linha').removeClass('fa-plus').addClass('fa-minus');
                $('.tr_detalhe').show('slow');
            }
        });
    });

    function exibirModalDetalheGrafico(tipoGraficoSubunidade, tipoValor, sigla){
        var urlBase = 'planacomorc.php?modulo=principal/unidade/painel&acao=A'
        if(tipoGraficoSubunidade === 1){
            var parametroTipoGrafico = '&req=detalhe-grafico-subunidade';
        } else {
            var parametroTipoGrafico = '&req=detalhe-grafico-uo';
        }
        // Dotação ou Planejado
        if(tipoValor.substr(0, 3) === "Dot" || tipoValor.substr(0, 3) === "Pla"){
            $('#detalhe-grafico .modal-body').load(urlBase+ '&tipo_valor='+ tipoValor.substr(0, 3)+ '&sigla='+ sigla+ parametroTipoGrafico);
            $('#detalhe-grafico').modal();
        }
    }


</script>