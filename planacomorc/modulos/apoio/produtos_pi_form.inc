<?php

$cProdutoPI = new Public_Controller_ProdutoPI();
switch ($_REQUEST['req']) {
	case 'salvar':
        $cProdutoPI->salvar($_REQUEST);
		die;
	case 'excluir':
        $cProdutoPI->excluir($_REQUEST['pprid']);
		die;
}

$mProdutoPI = new Monitora_Model_PiProduto($_REQUEST['pprid']);

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
                        <input name="pprid" id="pprid" type="hidden" value="<?php echo $mProdutoPI->pprid; ?>">

                        <?php
                            echo $simec->input('pprnome', 'Nome', $mProdutoPI->pprnome, ['required','maxlength' => 200]);
                            echo $simec->input('pprdescricao', 'Descrição', $mProdutoPI->pprdescricao, ['maxlength' => 500]);
                        ?>

                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-primary" type="submit" id="btn-salvar"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                                <a href="?modulo=apoio/produtos_pi&acao=A" class="btn btn-warning" id="btn-voltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
                                <?php if($mProdutoPI->pprid){ ?>
                                    <a href="?modulo=apoio/produtos_pi_form&acao=A&req=excluir&pprid=<?php echo $mProdutoPI->pprid; ?>" class="btn btn-danger link-excluir" id="btn-excluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
                                <?php } ?>                                
                            </div>
                        </div>                        
                    </form>
                </div>
            </div>
        </div>
    </div>
</div>
