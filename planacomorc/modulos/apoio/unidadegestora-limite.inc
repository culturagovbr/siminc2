<?php
include_once APPRAIZ . "includes/library/simec/Grafico.php";

include_once '_funcoespi.php';

require_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . "monitora/classes/Pi_PlanoInterno.class.inc";

# Verifica se o usuário tem vinculo de Sub-Unidades no seu Perfil
$listaSubUnidadeUsuario = buscarSubUnidadeUsuario((object) array('usucpf' => $_SESSION['usucpf']));

$oPlanoInterno = new Pi_PlanoInterno();
$cUnidadegestoraLimite = new Planacomorc_Controller_Unidadegestoralimite();

switch ($_REQUEST['req']) {
    case 'detalhe-subunidade':
        $oPlanoInterno->recuperarPiPorSubunidade($_REQUEST['suoid']);
        die;
    case 'detalhe-funcional':
        $oPlanoInterno->recuperarPiPorFuncional($_REQUEST['ptrid']);
        die;
    case 'detalhe-grafico':
        $oPlanoInterno->recuperarDetalhesValoresGrafico($_REQUEST['unosigla']);
        die;
    case 'salvar':
        $cUnidadegestoraLimite->salvar($_REQUEST);
        die;
    case 'salvar-dotacao':
        $mPtresSubunidade = new Spo_Model_PtresSubunidade($_GET['psuid']);
        $mPtresSubunidade->popularDadosObjeto($_GET);
        $mPtresSubunidade->salvar(null, null, ['ptrdotacaocusteio', 'ptrdotacaocapital']);
        $mPtresSubunidade->commit();
        die;
    case 'salvar-dotacao-fnc':
        $mPtres = new Planacomorc_Model_Ptres($_REQUEST['ptrid']);
        $mPtres->popularDadosObjeto($_REQUEST);
        $mPtres->salvar(null, null, ['ptrdotacaocusteio', 'ptrdotacaocapital']);
        $mPtres->commit();
        die;
}

$filtros = array(
    "suo.prsano = '". $_SESSION['exercicio']. "'",
    'suo.unofundo = FALSE',
);
# Filtro por Subunidades do perfil do usuário logado.
if($listaSubUnidadeUsuario){
    $filtros[] = "suo.suocod IN('". join("','", $listaSubUnidadeUsuario). "')";
}
$aPropostas = $oPlanoInterno->recuperarExecucaoOrcamentaria($filtros);
$aFnc = $oPlanoInterno->recuperarExecucaoOrcamentariaFnc();

/**
 * Cabeçalho padrão do sistema.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";


?>
<script>
    function exibirModalDetalheGrafico(unosigla){
        $('#detalhe-grafico .modal-body').load('planacomorc.php?modulo=principal/unidade/painel&acao=A&req=detalhe-grafico&unosigla=' + unosigla);
        $('#detalhe-grafico').modal();
    }
</script>

<style>
    .title-disponivel{
        cursor: help;
    }

    .linha_subunidade {
        background-color: rgba(241, 246, 215, 0.3) !important;
        font-weight: bold;
    }
</style>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-10">
        <h2><?php echo $titulo_modulo; ?></h2>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">

        <div class="col-md-12">

            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Dados Gerais</h5>
                </div>
                <div class="ibox-content">
                    <div class="table-responsive">
                        <table class="table table-bordered table-hover" >
                            <thead>
                            <tr class="text-center">
                                <th style="border-right: 1px black solid;">
                                    <i class="fa fa-plus detalhar-todos link"></i>&nbsp;&nbsp;Unidade
                                </th>
                                <th width="10%" style="border-right: 1px black solid;">Custeio</th>
                                <th width="10%" style="border-right: 1px black solid;">Capital</th>
                                <th width="10%" style="border-right: 1px black solid;">Total</th>
                                <th width="10%" style="border-right: 1px black solid;">Limite</th>
                            </tr>
                            </thead>
                            <tbody>

                            <?php foreach($aFnc as $dados){
                                $dado = $dados['detalhe'][0];
                                ?>
                                <tr class="linha_subunidade" style="background-color: #ffe6e1 !important;">
                                    <td style="border-right: 1px black solid;">
                                        <i class="fa fa-plus detalhar-linha link" data-suoid="<?php echo $dado['unoid']; ?>"></i>&nbsp;&nbsp;
                                        <span class="detalhe-subunidade" data-suoid="<?php echo $dado['unoid']; ?>">
                                            <?php echo $dado['unosigla'] . ' - ' . $dado['unonome'] . ' (' . $dado['unocod'] . ')'; ?>
                                        </span>
                                    </td>

                                    <td class="text-right" style="border-right: 1px black solid;" id="custeio<?php echo $dado['unoid']; ?>"><?php echo number_format($dados['dotacaoCusteio'], 0, ',', '.'); ?></td>

                                    <td class="text-right" style="border-right: 1px black solid;" id="capital<?php echo $dado['unoid']; ?>"><?php echo number_format($dados['dotacaoCapital'], 0, ',', '.'); ?></td>

                                    <td class="text-right" style="border-right: 1px black solid;" id="total<?php echo $dado['unoid']; ?>"><?php echo number_format(($dados['dotacaoCusteio'] + $dados['dotacaoCapital']), 0, ',', '.'); ?></td>

                                    <td style="color: <?php echo $oPlanoInterno->getCorPainelPorValorDisponivel($disponivel); ?>; border-right: 1px black solid;" class="text-right">
                                        <input name="limites" type="text" class="input_valor salvar-limite-fnc" value="<?php echo $dado['lmuvlr']; ?>" data-unoid="<?php echo $dado['unoid']; ?>" data-lmuid="<?php echo $dado['lmuid']; ?>" >
                                    </td>
                                </tr>
                                <?php foreach($dados['detalhe'] as $dado){ ?>
                                    <tr class="tr_<?php echo $dado['unoid']; ?> tr_detalhe" style="display: none; background-color: #fff0e6 !important;">
                                        <td style="padding-left: 30px; border-right: 1px black solid;" >
                                            <span class="detalhe-funcional" data-ptrid="<?php echo $dado['ptrid']; ?>">
                                                <?php echo $dado['funcional'] . ' - ' . $dado['acatitulo'] . ' (RP ' . $dado['irpcod'] . ') (PTRES:'. $dado['ptres']. ')'; ?>
                                            </span>
                                            <?php if($dado['compartilhada'] > 1): ?>
                                                &nbsp;<i class="fa fa-handshake-o link" title="Funcional Compartilhada com outras Subunidades."></i>
                                            <?php endif; ?>
                                        </td>

                                        <td class="text-right" style="border-right: 1px black solid;">
                                            <input name="limites[]" type="text" class="input_valor salvar-dotacao-fnc" id="custeio<?php echo $dado['ptrid']; ?>" value="<?php echo number_format($dado['ptrdotacao_custeio'], 2, '.', ''); ?>" data-unoid="<?php echo $dado['unoid']; ?>" data-ptrid="<?php echo $dado['ptrid']; ?>" data-campo="ptrdotacaocusteio" >
                                        </td>

                                        <td class="text-right" style="border-right: 1px black solid;">
                                            <input name="limites[]" type="text" class="input_valor salvar-dotacao-fnc" id="capital<?php echo $dado['ptrid']; ?>" value="<?php echo number_format($dado['ptrdotacao_capital'], 2, '.', ''); ?>" data-unoid="<?php echo $dado['unoid']; ?>" data-ptrid="<?php echo $dado['ptrid']; ?>" data-campo="ptrdotacaocapital" >
                                        </td>

                                        <td class="text-right" style="border-right: 1px black solid;" id="totalDetalhe<?php echo $dado['ptrid']; ?>"><?php echo number_format(($dado['ptrdotacao_custeio'] + $dado['ptrdotacao_capital']), 2, ',', '.'); ?></td>
                                        <td class="text-center" style="border-right: 1px black solid;"> - </td>
                                    </tr>
                                <?php } ?>
                            <?php } ?>
                            <?php foreach($aPropostas as $dados){
                                $dado = $dados['detalhe'][0];
                                ?>
                                <tr class="linha_subunidade">
                                    <td style="border-right: 1px black solid;">
                                        <i class="fa fa-plus detalhar-linha link" data-suoid="<?php echo $dado['suoid']; ?>"></i>&nbsp;&nbsp;
                                        <span class="detalhe-subunidade" data-suoid="<?php echo $dado['suoid']; ?>">
                                            <?php echo $dado['unosigla'] . ' - ' . $dado['suonome'] . ' (' . $dado['suocod'] . ')'; ?>
                                        </span>
                                    </td>

                                    <td class="text-right" style="border-right: 1px black solid;" id="custeio<?php echo $dado['suoid']; ?>"><?php echo number_format($dados['dotacaoCusteio'], 0, ',', '.'); ?></td>

                                    <td class="text-right" style="border-right: 1px black solid;" id="capital<?php echo $dado['suoid']; ?>"><?php echo number_format($dados['dotacaoCapital'], 0, ',', '.'); ?></td>

                                    <td class="text-right" style="border-right: 1px black solid;" id="total<?php echo $dado['suoid']; ?>"><?php echo number_format(($dados['dotacaoCusteio'] + $dados['dotacaoCapital']), 0, ',', '.'); ?></td>

                                    <?php $disponivel = $dado['lmuvlr'] - $dados['totalPlanejado']; ?>
                                    <td style="color: <?php echo $oPlanoInterno->getCorPainelPorValorDisponivel($disponivel); ?>; border-right: 1px black solid;" class="text-right" title="Disponível: <?php echo number_format($disponivel, 0, ',', '.'); ?>">
                                        <input name="limites[]" type="text" class="input_valor salvar-limite" value="<?php echo $dado['lmuvlr']; ?>" data-ungcod="<?php echo $dado['suocod']; ?>" data-lmuid="<?php echo $dado['lmuid']; ?>" >
                                    </td>
                                </tr>
                                <?php foreach($dados['detalhe'] as $dado){ ?>
                                    <tr class="tr_<?php echo $dado['suoid']; ?> tr_detalhe" style="display: none;">
                                        <td style="padding-left: 30px; border-right: 1px black solid;" >
                                            <span class="detalhe-funcional" data-ptrid="<?php echo $dado['ptrid']; ?>">
                                                <?php echo $dado['funcional'] . ' - ' . $dado['acatitulo'] . ' (RP ' . $dado['irpcod'] . ') (PTRES:'. $dado['ptres']. ')'; ?>
                                            </span>
                                            <?php if($dado['compartilhada'] > 1): ?>
                                                &nbsp;<i class="fa fa-handshake-o link" title="Funcional Compartilhada com outras Subunidades."></i>
                                            <?php endif; ?>
                                        </td>

                                        <td class="text-right" style="border-right: 1px black solid;">
                                            <input name="limites[]" type="text" class="input_valor salvar-dotacao" id="custeio<?php echo $dado['ptrid']; ?>" value="<?php echo number_format($dado['ptrdotacao_custeio'], 2, '.', ''); ?>" data-suoid="<?php echo $dado['suoid']; ?>" data-psuid="<?php echo $dado['psuid']; ?>" data-ptrid="<?php echo $dado['ptrid']; ?>" data-campo="ptrdotacaocusteio" >
                                        </td>

                                        <td class="text-right" style="border-right: 1px black solid;">
                                            <input name="limites[]" type="text" class="input_valor salvar-dotacao" id="capital<?php echo $dado['ptrid']; ?>" value="<?php echo number_format($dado['ptrdotacao_capital'], 2, '.', ''); ?>" data-suoid="<?php echo $dado['suoid']; ?>" data-psuid="<?php echo $dado['psuid']; ?>" data-ptrid="<?php echo $dado['ptrid']; ?>" data-campo="ptrdotacaocapital" >
                                        </td>

                                        <td class="text-right" style="border-right: 1px black solid;" id="totalDetalhe<?php echo $dado['ptrid']; ?>"><?php echo number_format(($dado['ptrdotacao_custeio'] + $dado['ptrdotacao_capital']), 2, ',', '.'); ?></td>
                                        <td class="text-center" style="border-right: 1px black solid;"> - </td>
                                    </tr>
                                <?php } ?>
                            <?php } ?>
                            </tbody>
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
bootstrapPopup("PI's da Funcional", 'detalhe-funcional', '', array('fechar'), array('tamanho' => 'lg'));
bootstrapPopup("Detalhe da Unidade", 'detalhe-grafico', '', array('fechar'), array('tamanho' => 'lg'));
?>

<script>
    $(document).ready(function(){

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

        $('.input_valor').keyup(function(){
            $(this).val(mascaraglobal('###.###.###.###,##', $(this).val()));
        }).keyup();

        $('.salvar-limite').change(function(){
            var lmuvlr = $(this).val() ? str_replace(['.', ','], ['', '.'], $(this).val()) : 0;
            var ungcod = $(this).data('ungcod');
            var lmuid = $(this).data('lmuid');
            $.ajax({
                url: 'planacomorc.php?modulo=apoio/unidadegestora-limite&acao=A&req=salvar&lmuvlr=' + lmuvlr + '&ungcod=' + ungcod + '&lmuid=' + lmuid
            });
        });

        $('.salvar-limite-fnc').change(function(){
            var lmuvlr = $(this).val() ? str_replace(['.', ','], ['', '.'], $(this).val()) : 0;
            var unoid = $(this).data('unoid');
            var lmuid = $(this).data('lmuid');
            $.ajax({
                url: 'planacomorc.php?modulo=apoio/unidadegestora-limite&acao=A&req=salvar&lmuvlr=' + lmuvlr + '&unoid=' + unoid + '&lmuid=' + lmuid
            });
        });

        $('.salvar-dotacao').change(function(){
            var valor = $(this).val() ? str_replace(['.', ','], ['', '.'], $(this).val()) : 0;
            var campo = $(this).data('campo');
            var psuid = $(this).data('psuid');
            $.ajax({
                url: 'planacomorc.php?modulo=apoio/unidadegestora-limite&acao=A&req=salvar-dotacao&psuid=' + psuid + '&' + campo + '=' + valor
            });
        });

        $('.salvar-dotacao-fnc').change(function(){
            var valor = $(this).val() ? str_replace(['.', ','], ['', '.'], $(this).val()) : 0;
            var campo = $(this).data('campo');
            var ptrid = $(this).data('ptrid');
            var unoid = $(this).data('unoid');
            var tipo = 'capital';
            if (campo==='ptrdotacaocusteio'){
                tipo = 'custeio';
            }
            var vlrSoma=0;
            var vlrSomaTotal=0;
            $(".salvar-dotacao-fnc").each(function(){
                if ($(this).data("unoid")===unoid){
                    valorDotacao = $(this).val() ? str_replace('.','',$(this).val()) : 0;
                    vlrSomaTotal = parseFloat(vlrSomaTotal) + parseFloat(valorDotacao);                
                    if ($(this).data("campo")===campo){
                        vlrSoma = parseFloat(vlrSoma) + parseFloat(valorDotacao);
                    }
                } 
                
            });
            var valorCusteio = $("#custeio"+ptrid).val() ? str_replace('.','',$("#custeio"+ptrid).val()) : 0;
            var valorCapital = $("#capital"+ptrid).val() ? str_replace('.','',$("#capital"+ptrid).val()) : 0;
            var totalGeral = textToFloat(valorCusteio) + textToFloat(valorCapital);
            $("#"+tipo+unoid).html(number_format(vlrSoma.toFixed(0), 0, ',', '.'));
            $("#totalDetalhe"+ptrid).html(number_format(totalGeral.toFixed(2), 2, ',', '.'));
            $("#total"+unoid).html(number_format(vlrSomaTotal.toFixed(0), 0, ',', '.'));
            $.ajax({
                url: 'planacomorc.php?modulo=apoio/unidadegestora-limite&acao=A&req=salvar-dotacao-fnc&ptrid=' + ptrid + '&' + campo + '=' + valor
            });
        });

        $('.salvar-dotacao').change(function(){
            var valor = $(this).val() ? str_replace(['.', ','], ['', '.'], $(this).val()) : 0;
            var campo = $(this).data('campo');
            var ptrid = $(this).data('ptrid');
            var suoid = $(this).data('suoid');
            var tipo = 'capital';
            if (campo==='ptrdotacaocusteio'){
                tipo = 'custeio';
            }
            var vlrSoma=0;
            var vlrSomaTotal=0;
            $(".salvar-dotacao").each(function(){
                if ($(this).data("suoid")===suoid){
                console.log($(this).data("suoid"));
                    valorDotacao = $(this).val() ? str_replace('.','',$(this).val()) : 0;
                    vlrSomaTotal = parseFloat(vlrSomaTotal) + parseFloat(valorDotacao);                
                    if ($(this).data("campo")===campo){
                        vlrSoma = parseFloat(vlrSoma) + parseFloat(valorDotacao);
                    }
                } 
                
            });
            var valorCusteio = $("#custeio"+ptrid).val() ? str_replace('.','',$("#custeio"+ptrid).val()) : 0;
            var valorCapital = $("#capital"+ptrid).val() ? str_replace('.','',$("#capital"+ptrid).val()) : 0;
            var totalGeral = textToFloat(valorCusteio) + textToFloat(valorCapital);
            console.log(valorCusteio);
            console.log(valorCapital);
            console.log(totalGeral);
            $("#"+tipo+suoid).html(number_format(vlrSoma.toFixed(0), 0, ',', '.'));
            $("#totalDetalhe"+ptrid).html(number_format(totalGeral.toFixed(2), 2, ',', '.'));
            $("#total"+suoid).html(number_format(vlrSomaTotal.toFixed(0), 0, ',', '.'));
//            $.ajax({
//                url: 'planacomorc.php?modulo=apoio/unidadegestora-limite&acao=A&req=salvar-dotacao&ptrid=' + ptrid + '&' + campo + '=' + valor
//            });
        });

    });

</script>

