<?php
    $acpid = $opcoes['acpid']? $opcoes['acpid']: $_REQUEST['acpid'];
    $acompanhamentoPnc = new Acompanhamento_Model_Acompanhamentopnc($acpid);
?>

<form class="dropzone" id="formularioAcompanhamentoAnexo" name="formularioAcompanhamentoAnexo" method="POST" enctype="multipart/form-data" action="acompanhamento.php?modulo=principal/monitorar-pnc&acao=A&req=anexar-arquivos-monitoramento-Pnc&acpid=<?php echo $acompanhamentoPnc->acpid; ?>">
    <div class="form-group">
        <label for="arqdescricao" class="col-lg-2 control-label">Descrição:</label>
        <div class="col-lg-10">
            <input type="text" value="" class="CampoEstilo normal form-control" placeholder="Insira a descrição do arquivo." title="Descrição" id="arqdescricao" name="arqdescricao" maxlength="255" size="2">
        </div>
    </div>
    <div class="fallback">
        <input name="file" type="file" multiple />
    </div>
</form>
