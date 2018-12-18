<?php

switch ($_REQUEST['req']){
    case 'salvar':
        $cMetaPnc = new Spo_Controller_SubUnidadeMetaPnc();
        $cMetaPnc->salvar($_REQUEST);
        die;
}

$aMetaPnc = (new Public_Model_MetaPnc())->recuperarTodos("*, lpad(mpncod, 2, '0') as codigo", ["prsano = '{$_SESSION['exercicio']}'", "mpnstatus = 'A'"], 'codigo');
$aSubUnidadeOrcamentaria = simec_preparar_array((new Public_Model_SubUnidadeOrcamentaria())->recuperarTodos("suoid as codigo, unosigla || ' - ' || suonome as descricao", ["prsano = '{$_SESSION['exercicio']}'", "suostatus = 'A'"], 'descricao'));
$aVinculos = (new Spo_Model_SubUnidadeMetaPnc())->recuperarPorExercicio($_SESSION['exercicio']);
/**
 * Cabeçalho padrão do sistema.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";

?>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-10">
        <h2>Víncular Metas PNC com Unidade</h2>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <div class="col-md-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Metas PNC</h5>
                </div>
                <div class="ibox-content">
                    <form id="formulario" name="formulario" method="post" class="form-horizontal">
                        <input type="hidden" name="req" id="req" value="salvar" />

                        <div class="table-responsive">
                            <table class="table table-striped table-bordered table-hover" >
                                <thead>
                                <tr>
                                    <th width="50%">Meta PNC</th>
                                    <th>Unidade</th>
                                </tr>
                                </thead>
                                <tbody>
                                <?php
                                    if($aMetaPnc):
                                        foreach($aMetaPnc as $dados):
                                ?>
                                            <tr>
                                                <td><?php echo $dados['mpncod'] . ' - ' . $dados['mpnnome']; ?></td>
                                                <td>
                                                    <?php
                                                    $vinculados = !empty($aVinculos[$dados['mpnid']]) ?  $aVinculos[$dados['mpnid']]: [];
                                                    echo $simec->select("vinculos[{$dados['mpnid']}][]", null, $vinculados, $aSubUnidadeOrcamentaria, null, ['input-size'=>12]);
                                                    ?>
                                                </td>
                                            </tr>
                                        <?php endforeach;
                                    endif; ?>
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