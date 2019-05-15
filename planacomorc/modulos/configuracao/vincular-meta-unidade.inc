<?php

switch ($_REQUEST['req']){
    case 'salvar':
        $cMetaPpa = new Planacomorc_Controller_SubunidadeMetaUnidade();
        $cMetaPpa->salvar($_REQUEST);
        die;
}

$aMetaUnidade = (new Planacomorc_Model_MetaUnidade())->recuperarTodos('*', ["meustatus = 'A'"], 'meunome');
$aSubUnidadeOrcamentaria = simec_preparar_array((new Public_Model_SubUnidadeOrcamentaria())->recuperarTodos("suoid AS codigo, unosigla || ' - ' || suonome || '( ' || suocod || ' )' AS descricao", ["prsano = '{$_SESSION['exercicio']}'", "suostatus = 'A'"], 'descricao'));
$aVinculos = (new Planacomorc_Model_SubunidadeMetaUnidade())->recuperarPorExercicio($_SESSION['exercicio']);

include APPRAIZ . "includes/cabecalho.inc";
?>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-10">
        <h2><?= $titulo_modulo; ?></h2>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <div class="col-md-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Metas Unidade</h5>
                </div>
                <div class="ibox-content">
                    <form id="formulario" name="formulario" method="post" class="form-horizontal">
                        <input type="hidden" name="req" id="req" value="salvar" />

                        <div class="table-responsive">
                            <table class="table table-striped table-bordered table-hover" >
                                <thead>
                                    <tr>
                                        <th width="50%">Meta Unidade</th>
                                        <th>Unidade</th>
                                    </tr>
                                </thead>
                                <tbody>
                                    <?php foreach ($aMetaUnidade AS $meta): ?>
                                        <tr>
                                            <td><?= $meta['meunome']; ?></td>
                                            <td>
                                                <?php
                                                    $vinculados = !empty($aVinculos[$meta['meuid']]) ?  $aVinculos[$meta['meuid']] : [];
                                                    echo $simec->select("vinculos[{$meta['meuid']}][]", null, $vinculados, $aSubUnidadeOrcamentaria, null, ['input-size' => 12]);
                                                ?>
                                            </td>
                                        </tr>
                                    <?php endforeach; ?>
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