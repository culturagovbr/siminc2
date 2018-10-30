<?php

switch ($_REQUEST['req']){
    case 'salvar':
        $cSubUnidadeIniciaticaPpa = new Spo_Controller_SubUnidadeIniciativaPpa();
        $cSubUnidadeIniciaticaPpa->salvar($_REQUEST);
        die;
}

$aIniciativaPpa = (new Public_Model_IniciativaPpa())->recuperarTodos('*', ["prsano = '{$_SESSION['exercicio']}'", "ippstatus = 'A'"], 'ippcod');
$aSubUnidadeOrcamentaria = simec_preparar_array((new Public_Model_SubUnidadeOrcamentaria())->recuperarTodos("suoid as codigo, unosigla || ' - ' || suonome as descricao", ["prsano = '{$_SESSION['exercicio']}'", "suostatus = 'A'"], 'descricao'));
$aVinculos = (new Spo_Model_SubUnidadeIniciativaPpa())->recuperarPorExercicio($_SESSION['exercicio']);

/**
 * Cabeçalho padrão do sistema.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";

?>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-10">
        <h2>Víncular Iniciativas PPA com Unidade</h2>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <div class="col-md-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Iniciativas PPA</h5>
                </div>
                <div class="ibox-content">
                    <form id="formulario" name="formulario" method="post" class="form-horizontal">
                        <input type="hidden" name="req" id="req" value="salvar" />

                        <div class="table-responsive">
                            <table class="table table-striped table-bordered table-hover" >
                                <thead>
                                <tr>
                                    <th width="50%">Iniciativa PPA</th>
                                    <th>Unidade</th>
                                </tr>
                                </thead>
                                <tbody>
                                <?php foreach($aIniciativaPpa as $dados){ ?>
                                    <tr>
                                        <td><?php echo $dados['ippcod'] . ' - ' . $dados['ippnome']; ?></td>
                                        <td>
                                            <?php
                                            $vinculados = !empty($aVinculos[$dados['ippid']]) ?  $aVinculos[$dados['ippid']]: [];
                                            echo $simec->select("vinculos[{$dados['ippid']}][]", null, $vinculados, $aSubUnidadeOrcamentaria, null, ['input-size'=>12]);
                                            ?>
                                        </td>
                                    </tr>
                                <?php } ?>
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
                        <?php }?>
                    </form>
                </div>
            </div>
        </div>
    </div>
</div>