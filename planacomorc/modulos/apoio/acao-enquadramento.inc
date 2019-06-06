<?php

switch ($_REQUEST['req']){
    case 'salvar':
        (new Planacomorc_Controller_EnquadramentoRp())->salvar($_REQUEST);
        die;
}

$aEnquadramento = (new Planacomorc_Model_EnquadramentoDespesa())->recuperarTodos('*', ["eqdano = '{$_SESSION['exercicio']}'", "eqdstatus = 'A'"], 'eqddsc');
$aRp = simec_preparar_array((new Public_Model_IdentResultPrimario())->recuperarTodos("irpcod as codigo, irpcod || ' - ' || irpdsc as descricao", ["irpstatus = 'A'"], 'descricao'));
$aVinculos = (new Planacomorc_Model_EnquadramentoRp())->recuperarPorExercicio($_SESSION['exercicio']);


/**
 * Cabeçalho padrão do sistema.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";

?>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-10">
        <h2>Vincular Enquadramento de Despesas com Resultado Primário (RP)</h2>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <div class="col-md-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Mapeamento</h5>
                </div>
                <div class="ibox-content">
                    <form id="formulario" name="formulario" method="post" class="form-horizontal">
                        <input type="hidden" name="req" id="req" value="salvar" />

                        <div class="table-responsive">
                            <table class="table table-striped table-bordered table-hover" >
                                <thead>
                                <tr>
                                    <th width="50%">Enquadramento de Despesas</th>
                                    <th>Resultado Primário (RP)</th>
                                </tr>
                                </thead>
                                <tbody>
                                <?php
                                    if($aEnquadramento):
                                        foreach($aEnquadramento as $dados):
                                    ?>
                                            <tr>
                                                <td><?php echo $dados['eqddsc']; ?></td>
                                                <td>
                                                    <?php
                                                    $vinculados = !empty($aVinculos[$dados['eqdid']]) ?  $aVinculos[$dados['eqdid']]: [];
                                                    echo $simec->select("vinculos[{$dados['eqdid']}][]", null, $vinculados, $aRp, null, ['input-size'=>12]);
                                                    ?>
                                                </td>
                                            </tr>
                                        <?php endforeach; ?>
                                    <?php endif; ?>
                                </tbody>
                            </table>
                        </div>
                        <div class="form-group">
                            <div class="text-center">
                                <button type="submit" class="btn btn-primary fa fa-check" name="btg" value="Salvar"> Salvar</button>
                                <a href="planacomorc.php?modulo=inicio&acao=C" class="btn btn-warning fa fa-arrow-left" id="btnVoltar" type="button"> Voltar</a>
                            </div>
                        </div>
                    </form>
                </div>
            </div>
        </div>
    </div>
</div>