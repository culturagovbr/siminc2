<?php

include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include APPRAIZ . "includes/cabecalho.inc";
include_once APPRAIZ . "includes/library/simec/Helper/FlashMessage.php";

$fm = new Simec_Helper_FlashMessage('planacomorc/tc/cadexecorcfin');

if($_POST['execucao'] == 'cadastrar' && !$_POST['id']){

    $codigo = $_POST['codigo'];
    $descricao = $_POST['descricao'];
    $status = $_POST['status'];
    $ano = $_POST['ano'];
    
    $sql = "
        INSERT INTO monitora.pi_executor (piecod,piedsc,pieano,piestatus) VALUES
            ('$codigo', '$descricao', '$ano', '$status') ";
      if ($db->executar( $sql )) {
        $db->commit ();
      }
   $fm->addMensagem(
            'Registro cadastrado com sucesso.',
            Simec_Helper_FlashMessage::SUCESSO
        );
   echo "<script>window.location.assign(\"http:planacomorc.php?modulo=sistema/tabelasapoio/cadExecutorOrcamentarioFinanceiro&acao=A\")</script>";

}
if($_REQUEST['execucao'] == 'editar'){
    $strSql = 'SELECT * FROM monitora.pi_executor WHERE pieid=' . $_REQUEST['registro'];
    $rs = $db->pegaLinha($strSql);
    $id = $rs['pieid'];
    $codigo = $rs['piecod'];
    $descricao = $rs['piedsc'];
    $ano = $rs['pieano'];
    $status = $rs['piestatus'];
    //ver($rs,$codigo,d);
}

if($_POST['id']){
    $id = $_POST['id'];
    $codigo = $_POST['codigo'];
    $descricao = $_POST['descricao'];
    $status = $_POST['status'];
    $ano = $_POST['ano'];
    
    $sql = "
        UPDATE monitora.pi_executor SET
            piecod='$codigo', piedsc='$descricao', pieano='$ano', piestatus='$status'
        WHERE pieid='$id'
    ";
      if ($db->executar( $sql )) {
        $db->commit ();
      }
    
      $fm->addMensagem(
            'Registro alterado com sucesso.',
            Simec_Helper_FlashMessage::SUCESSO
        );
    echo "<script>window.location.assign(\"http:planacomorc.php?modulo=sistema/tabelasapoio/cadExecutorOrcamentarioFinanceiro&acao=A\")</script>";

}
?>
<div class="col-lg-12">
    <ol class="breadcrumb">
        <li><a href="<?php echo MODULO; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li class="active">Cadastro de Executor Or�ament�rio Financeiro</li>
    </ol>
    <div class="well">
        <fieldset>
            <form id="formPrincipal" name="formPrincipal" method="POST" class="form-horizontal" enctype="multipart/form-data" onsubmit="return validaCampo();">
                <div class="form-group">
                        <div class="col-lg-10">
                            <input type="hidden" name="execucao" id="execucao" value="cadastrar">
                            <input type="hidden" name="id" id="id" value="<?php echo $id; ?>">
                            <input type="hidden" class="normal form-control" name="ano" id="ano" value="<?php echo $ano ? $ano : $_SESSION['exercicio']; ?>" />
                            <input type="hidden" name="status" id="status" value="A">
                        </div>
                </div>
                <div class="form-group">
                    <label for="nomeArquivo" class="col-lg-2 control-label">
                        Codigo:
                    </label>
                    <div class="col-lg-10">
                        <input type="text" maxlength="1" class="normal form-control" name="codigo" id="codigo" value="<?php echo $codigo; ?>" />
                    </div>
                </div>
                 <div class="form-group">
                    <label for="nomeArquivo" class="col-lg-2 control-label">
                        Descri��o:
                    </label>
                    <div class="col-lg-10">
                        <?inputTextArea('descricao', $descricao, 'descricao', '250')?>
                    </div>
                </div>                 
                <div class="form-group">
                    <div class="col-lg-10 col-lg-offset-2">
                        <button class="btn btn-warning" type="reset" onclick="window.location = window.location;">Limpar</button>
                        <button class="btn btn-primary" id="btnVoltar" type="button" onclick="voltar();">Voltar</button>
                        <button class="btn btn-success" id="inserir" type="submit">
                            <i class="glyphicon glyphicon-upload"></i>
                            Salvar
                        </button>
                    </div>
                </div>
            </form>
        </fieldset>
    </div>
</div>
<script type="text/javascript">
   
    function voltar(){
        window.location = 'planacomorc.php?modulo=sistema/tabelasapoio/cadExecutorOrcamentarioFinanceiro&acao=A';
    }
    
    function validaCampo() {
        if (document.formPrincipal.file.value == "")
        {
            alert("Favor selecionar um arquivo!");
            return false;
        }
        if (document.formPrincipal.nomeArquivo.value == "")
        {
            alert("O Campo nome � obrigat�rio!");
            return false;
        }
    }
</script>