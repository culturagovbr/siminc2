<?php

$mPtresSubunidade = new Spo_Model_PtresSubunidade();

switch ($_REQUEST['req']){
    case 'vincular':
        $mPtresSubunidade->vincularPtres($_REQUEST);
        die;
    case 'desvincular':
        $mPtresSubunidade->desvincularPtres($_REQUEST);
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

        <?php

        $aBlocos = [
            'D' => 'Administração Direta',
            'V' => 'Vinculadas',
            'F' => 'Fundo Nacional da Cultura',
        ];
        foreach ($aBlocos as $tipo => $descricao) { ?>
            <div class="col-md-6">
                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <h5><?php echo $descricao; ?></h5>
                    </div>
                    <div class="ibox-content">
                        <form id="formulario" name="formulario" method="post" class="form-horizontal">
                            <input type="hidden" name="req" id="req" value="salvar" />

                            <div class="table-responsive">
                                <?php
                                $aPtresSubunidade = (new Spo_Model_Ptres())->recuperarPtresSubunidade($_SESSION['exercicio'], $tipo);
                                ?>
                                <table class="table table-striped table-bordered table-hover" >
                                    <thead>
                                    <tr class="text-center">
                                        <th><input type="checkbox" class="marcar-todos-ptres" marcar="check_<?php echo $tipo; ?>" checked></th>
                                        <th>Prog</th>
                                        <th>Ação</th>
                                        <th>Local.</th>
                                        <th>P.O.</th>
                                        <th>Unidade</th>
                                    </tr>
                                    </thead>
                                    <tbody>
                                    <?php
                                        if($aPtresSubunidade):
                                            foreach($aPtresSubunidade as $dados):
                                    ?>
                                                <tr>
                                                    <td>
                                                        <?php $checked = (!empty($aVinculos[$dados['ptrid']]) && (in_array($dados['suoid'], $aVinculos[$dados['ptrid']])) || isset($aVinculos[$dados['ptrid']]['t'])) ? 'checked' : '' ; ?>
                                                        <input type="checkbox" class="vincular check_<?php echo $tipo; ?>" data-ptrid="<?php echo $dados['ptrid']; ?>" data-unofundo="<?php echo $dados['unofundo']; ?>" data-suoid="<?php echo $dados['suoid']; ?>" <?php echo $checked; ?> />
                                                    </td>
                                                    <td><?php echo $dados['prgcod']; ?></td>
                                                    <td><?php echo $dados['acacod']; ?></td>
                                                    <td><?php echo $dados['loccod']; ?></td>
                                                    <td><?php echo $dados['plocod']; ?></td>
                                                    <td><?php echo $dados['unosigla'] . ' - ' . $dados['suonome']; ?></td>
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
        <?php } ?>
    </div>
</div>

<script>
    $(function(){
        $('.vincular').click(function(){
            var acao = $(this).is(':checked') ? 'vincular' : 'desvincular';
            var ptrid = $(this).data('ptrid');
            var suoid = $(this).data('suoid');
            var unofundo = $(this).data('unofundo');
            $.ajax({
                url: 'planacomorc.php?modulo=apoio/vincular-ptres&acao=A&req=' + acao + '&ptrid=' + ptrid + '&suoid=' + suoid + '&unofundo=' + unofundo
            });
        });

        jQuery('.marcar-todos-ptres').click(function () {
            var marcado = jQuery(this).prop('checked');
            jQuery('tr:visible .' + jQuery(this).attr('marcar')).each(function(index, obj){
                if($(obj).prop('checked') != marcado){
                    $(obj).click();
                }
            });
        });
    })
</script>