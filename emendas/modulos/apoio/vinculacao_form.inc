<?php

$cVinculacao = new Emendas_Controller_Vinculacao();
switch ($_REQUEST['req']) {
    case 'salvar':
        $cVinculacao->salvar($_REQUEST);
        die;
    case 'excluir':
        $cVinculacao->salvar(array('vinid' => $_REQUEST['vinid'], 'vinstatus' => 'I'));
        die;
}

$mVinculacao = new Emendas_Model_Vinculacao($_REQUEST['vinid']);

include APPRAIZ . "includes/cabecalho.inc";
?>

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
                    <form id="formulario" name="formulario" method="post" class="form-horizontal">
                        <input type="hidden" name="req" id="req" value="salvar" />
                        <input name="vinid" id="vinid" type="hidden" value="<?php echo $mVinculacao->vinid; ?>">
                        
                        <?php                         
                            echo $simec->input('vindsc', 'Descrição', $mVinculacao->vindsc, ['maxlength' => 200]);
                        ?>
                        
                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-primary" type="submit" id="btn-salvar"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                                <a href="?modulo=apoio/vinculacao&acao=A" class="btn btn-warning" id="btn-voltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
                                <?php if($mVinculacao->vinid){ ?>
                                    <a href="?modulo=apoio/vinculacao_form&acao=A&req=excluir&vinid=<?php echo $mVinculacao->vinid; ?>" class="btn btn-danger link-excluir" id="btn-excluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
                                <?php } ?>                                
                            </div>
                        </div>                        
                    </form>
                </div>
            </div>
        </div>
    </div>
</div>