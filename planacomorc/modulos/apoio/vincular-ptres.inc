<?php

$mPtresSubunidade = new Spo_Model_PtresSubunidade();
$cPtresSubunidade = new Spo_Controller_PtresSubunidade();

switch ($_REQUEST['req']){
    case 'vincular':
        $cPtresSubunidade->vincularPtres($_REQUEST);
        die;
}

$aVinculos = $mPtresSubunidade->recuperarPorExercicio($_SESSION['exercicio']);

/**
 * Cabeçalho padrão do sistema.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";

?>

<style>
    .table *{
        font-size: 12px;
    }

    .table > thead > tr > th, .table > tbody > tr > th, .table > tfoot > tr > th, .table > thead > tr > td, .table > tbody > tr > td, .table > tfoot > tr > td{
        padding: 5px;
        padding-top: 8px;
    }
</style>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-10">
        <h2>Vincular PTRES com Unidade</h2>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row" id="listagem">

        <style>
            .marcado{background-color:#C1FFC1!important}
            .remover{display:none}
        </style>

        <script type="text/javascript" lang="JavaScript">
            jQuery(document).ready(function(){
                jQuery.expr[':'].contains = function(a, i, m) {
                    return jQuery(a).text().toUpperCase().indexOf(m[3].toUpperCase()) >= 0;
                };

                jQuery("#textFind").keyup(function()
                {
                    jQuery('#listagem table tbody tr td').removeClass('marcado');
                    jQuery('#listagem table tbody tr').removeClass('remover');
                    stringPesquisa = jQuery("#textFind").val();

                    console.log(stringPesquisa)

                    if (stringPesquisa) {
                        jQuery('#listagem table tbody tr td:contains(' + stringPesquisa + ')').addClass('marcado');
                        jQuery('#listagem table tbody tr:not(:contains(' + stringPesquisa + '))').addClass('remover');
                    }
                });
            });
        </script>

        <div class="col-md-12">
            <input class="normal form-control" type="text" id="textFind" placeholder="Digite aqui o que procura" style="width: 100%; margin-bottom: 10px;" />
        </div>

        <div class="col-md-12">
            <div class="ibox float-e-margins">
                <div class="ibox-content">
                    <form id="formulario" name="formulario" method="post" class="form-horizontal">
                        <input type="hidden" name="req" id="req" value="salvar" />

                        <div class="table-responsive">
                            <?php
                            $aPtresSubunidade = (new Spo_Model_Ptres())->recuperarPtresSubunidade($_SESSION['exercicio'], $tipo);
                            $aSubunidadePtrid = (new Spo_Model_Ptres())->recuperarPtridSubUnidade($_SESSION['exercicio'], $tipo);
                            $arrSubunidades = (new Public_Model_SubUnidadeOrcamentaria())->recuperarSqlCombo(array(), null, 'suonome');
                            ?>
                            <table class="table table-striped table-bordered table-hover" >
                                <thead>
                                <tr class="text-center">
                                    <th>--</th>
                                    <th>PTRES</th>
                                    <th>UO</th>
                                    <th>Prog</th>
                                    <th>Ação</th>
                                    <th>Local.</th>
                                    <th>P.O.</th>
                                    <th>Unidade Orçamentária</th>
                                    <th>Sub unidade</th>
                                </tr>
                                </thead>
                                <tbody>
                                <?php
                                $i=1;
                                    if($aPtresSubunidade):
                                        foreach($aPtresSubunidade as $dados):
                                ?>
                                            <tr>
                                                <td><?php echo $i++ ?></td>
                                                <td><?php echo $dados['ptres']; ?></td>
                                                <td><?php echo $dados['unicod']; ?></td>
                                                <td><?php echo $dados['prgcod']; ?></td>
                                                <td><?php echo $dados['acacod']; ?></td>
                                                <td><?php echo $dados['loccod']; ?></td>
                                                <td><?php echo $dados['plocod']; ?></td>
                                                <td style="width:400px;"><?php echo $dados['unidade_orcamentaria']; ?></td>
                                                <td><?php echo $simec->select('suoid'.$dados['ptrid'].'[]', null, $aSubunidadePtrid[$dados['ptrid']], $arrSubunidades, ['class' => ' vinculo', 'data-ptrid' => $dados['ptrid'], 'data-unofundo' => $dados['unofundo']]);?></td>
                                            </tr>
                                    <?php endforeach; ?>
                                <?php endif; ?>
                                </tbody>
                            </table>
                        </div>
                    </form>
                </div>
            </div>
        </div>
    </div>
</div>

<script>
    $(function(){
        $(".vinculo").change(function(){
            var acao = 'vincular';
            var ptrid = $(this).data('ptrid');
            var suoid = $(this).val();
            var unofundo = $(this).data('unofundo');
            $.ajax({
                url: 'planacomorc.php?modulo=apoio/vincular-ptres&acao=A&req=' + acao + '&ptrid=' + ptrid + '&suoid=' + suoid + '&unofundo=' + unofundo
            });
        });
    });
</script>