<?php

switch ($_REQUEST['req']){
    case 'salvar':
        $cMetaPpa = new Planacomorc_Controller_SubunidadeMetaEstrategica();
        $cMetaPpa->salvar($_REQUEST);
        die;
}

$aMetasEstrategicas = (new Planacomorc_Model_MetaEstrategica())->recuperarTodos('*', ["prsano = '{$_SESSION['exercicio']}'", "meestatus = 'A'"], 'meecod');

$aSubUnidadeOrcamentaria = simec_preparar_array((new Public_Model_SubUnidadeOrcamentaria())->recuperarTodos("suoid as codigo, unosigla || ' - ' || suonome as descricao", ["prsano = '{$_SESSION['exercicio']}'", "suostatus = 'A'"], 'descricao'));
$aVinculos = (new Planacomorc_Model_SubunidadeMetaEstrategica())->recuperarPorExercicio($_SESSION['exercicio']);

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
                    <h5>Metas Estratégicas</h5>
                </div>
                    <div class="ibox-content">
                    <?php if($aMetasEstrategicas): ?>
                        <form id="formulario" name="formulario" method="post" class="form-horizontal">
                            <input type="hidden" name="req" id="req" value="salvar" />
    
                            <div class="table-responsive">
                                <table class="table table-striped table-bordered table-hover" >
                                    <thead>
                                    <tr>
                                        <th width="50%">Meta Estratégica</th>
                                        <th>Unidade</th>
                                    </tr>
                                    </thead>
                                    <tbody>
                                        <?php foreach ($aMetasEstrategicas AS $meta): ?>
                                            <tr>
                                                <td><?php echo $meta['meecod'] . ' - ' . $meta['meenome']; ?></td>
                                                <td>
                                                    <?php
                                                        $vinculados = !empty($aVinculos[$meta['meeid']]) ?  $aVinculos[$meta['meeid']]: [];
                                                        echo $simec->select("vinculos[{$meta['meeid']}][]", null, $vinculados, $aSubUnidadeOrcamentaria, null, ['input-size'=>12]);
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
                    <?php else: ?>
                        <div class="alert alert-danger">Nenhum registro cadastrado</div>
                    <?php endif; ?>
                </div>
            </div>
        </div>
    </div>
</div>