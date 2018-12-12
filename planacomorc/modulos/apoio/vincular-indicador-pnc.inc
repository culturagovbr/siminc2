<?php

switch ($_REQUEST['req']){
    case 'salvar':
        $cIndicadorPnc = new Spo_Controller_SubUnidadeIndicadorPnc();
        $cIndicadorPnc->salvar($_REQUEST);
        die;
}

$aIndicadorPnc = (new Public_Model_IndicadorPnc())->recuperarIndicadores();
$aSubUnidadeOrcamentaria = simec_preparar_array((new Public_Model_SubUnidadeOrcamentaria())->recuperarTodos("suoid as codigo, unosigla || ' - ' || suonome as descricao", ["prsano = '{$_SESSION['exercicio']}'", "suostatus = 'A'"], 'descricao'));
$aVinculos = (new Spo_Model_SubUnidadeIndicadorPnc())->recuperarPorExercicio($_SESSION['exercicio']);
/**
 * Cabeçalho padrão do sistema.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";

?>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-10">
        <h2>Víncular Indicadores PNC com Unidade</h2>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <div class="col-md-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Indicadores PNC</h5>
                </div>
                <div class="ibox-content">
                    <form id="formulario" name="formulario" method="post" class="form-horizontal">
                        <input type="hidden" name="req" id="req" value="salvar" />

                        <div class="table-responsive">
                            <table class="table table-striped table-bordered table-hover" >
                                <thead>
                                <tr>
                                    <th width="50%">Indicador PNC</th>
                                    <th>Unidade</th>
                                </tr>
                                </thead>
                                <tbody>
                                <?php
                                    if($aIndicadorPnc):
                                        foreach($aIndicadorPnc as $dados):
                                ?>
                                            <tr>
                                                <td>
                                                    <span title="Meta <?php echo $dados['mpncod'] . ' - ' . $dados['mpndsc']; ?>">Meta <?php echo $dados['mpncod']; ?></span><br>
                                                    <?php echo $dados['ipncod'] . ' - ' . $dados['ipndsc']; ?>
                                                </td>
                                                <td>
                                                    <?php
                                                    $vinculados = !empty($aVinculos[$dados['ipnid']]) ?  $aVinculos[$dados['ipnid']]: [];
                                                    echo $simec->select("vinculos[{$dados['ipnid']}][]", null, $vinculados, $aSubUnidadeOrcamentaria, null, ['input-size'=>12]);
                                                    ?>
                                                </td>
                                            </tr>
                                        <?php endforeach; ?>
                                    <?php endif; ?>
                                </tbody>
                            </table>
                        </div>
                        <?php if (!(in_array(PFL_CONSULTA, $perfis))) {?>
                            <div class="form-group">
                                <div class="text-center">
                                    <button type="submit" class="btn btn-primary fa fa-check" name="btg" value="Salvar"> Salvar</button>
                                    <a href="planacomorc.php?modulo=inicio&acao=C" class="btn btn-warning fa fa-arrow-left" id="btnVoltar" type="button"> Voltar</a>
                                </div>
                            </div>
                        <?php } ?>
                    </form>
                </div>
            </div>
        </div>
    </div>
</div>