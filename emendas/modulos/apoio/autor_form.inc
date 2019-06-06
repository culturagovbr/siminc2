<?php

$cAutor = new Emendas_Controller_Autor();
switch ($_REQUEST['req']) {
    case 'salvar':
        $cAutor->salvar($_REQUEST);
    die;
    case 'excluir':
        $cAutor->excluir($_REQUEST['autid']);
    die;
}

$mAutor = new Emendas_Model_Autor($_REQUEST['autid']);

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
                        <input name="autid" id="autid" type="hidden" value="<?php echo $mAutor->autid; ?>">

                        <?php
                            echo $simec->select('atpid', 'Tipo de Autor', $mAutor->atpid, Emendas_Model_AutorTipo::buscarTodosCombo((object) array()));
                            echo $simec->input('autnome', 'Nome', $mAutor->autnome, ['maxlength' => 200]);                    
                            echo $simec->email('autemail', 'Email', $mAutor->autemail, ['required', 'maxlength' => 300]);                    
                            echo $simec->select('estuf', 'Estado', $mAutor->estuf, Territorios_Model_Estado::pegarSQLSelect(array()));
                            echo $simec->input('autcod', 'Código', $mAutor->autcod, ['class' => 'inteiro', 'required', 'maxlength' => 10]);                    
                            echo $simec->select('parid', 'Partido', $mAutor->parid, Emendas_Model_Partido::buscarTodosCombo((object) array()));
                            echo $simec->radio('autsexo', 'Sexo', $mAutor->autsexo, array(
                                'M' => 'Masculino',
                                'F' => 'Feminino'
                            ));
                        ?>

                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-primary" type="submit" id="btn-salvar"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                                <a href="?modulo=apoio/autor&acao=A" class="btn btn-warning" id="btn-voltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
                                <?php if($mAutor->autid){ ?>
                                    <a href="?modulo=apoio/autor_form&acao=A&req=excluir&autid=<?php echo $mAutor->autid; ?>" class="btn btn-danger link-excluir" id="btn-excluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
                                <?php } ?>                                
                            </div>
                        </div>                        
                    </form>
                </div>
            </div>
        </div>
    </div>
</div>