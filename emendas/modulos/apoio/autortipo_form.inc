<?php

$cAutorTipo = new Emendas_Controller_AutorTipo();
switch ($_REQUEST['req']) {
	case 'salvar':
        $cAutorTipo->salvar($_REQUEST);
		die;
	case 'excluir':
        $cAutorTipo->excluir($_REQUEST['atpid']);
		die;
}

$mAutorTipo = new Emendas_Model_AutorTipo($_REQUEST['atpid']);

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
                        <input name="atpid" id="atpid" type="hidden" value="<?php echo $mAutorTipo->atpid; ?>">
                        
                        <?php                         
                        echo $simec->input('atpdsc', 'atpdsc', $mAutorTipo->atpdsc, ['maxlength' => 200]);                    
                        echo $simec->input('agrid', 'agrid', $mAutorTipo->agrid);                        
                        ?>
                        
                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-primary" type="submit" id="btn-salvar"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                                <a href="?modulo=apoio/autortipo&acao=A" class="btn btn-warning" id="btn-voltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
                                <?php if($mAutorTipo->atpid){ ?>
                                    <a href="?modulo=apoio/autortipo_form&acao=A&req=excluir&atpid=<?php echo $mAutorTipo->atpid; ?>" class="btn btn-danger link-excluir" id="btn-excluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
                                <?php } ?>                                
                            </div>
                        </div>                        
                    </form>
                </div>
            </div>
        </div>
    </div>
</div>