<?php

$fm = new Simec_Helper_FlashMessage('planacomorc/tc/caddespesas');


    if($_REQUEST['execucao'] == 'excluir'){
        deleteRowEnquadramentoDespesas($_REQUEST['registro']);
        $fm->addMensagem(
            'Registro excluido com sucesso.',
            Simec_Helper_FlashMessage::SUCESSO
        );
    }


include APPRAIZ . 'includes/cabecalho.inc';

?>
<div class="row col-md-12">
        <ol class="breadcrumb">
            <li><a href="<?php echo MODULO; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
            <li class="active">Enquadramentos de Despesa</li>
        </ol>

    <?php
 $strSqlGrid = "SELECT
    eqdid, eqdcod, eqddsc, eqdano
FROM monitora.pi_enquadramentodespesa
    WHERE
        eqdstatus='A' AND
        eqdano = '{$_SESSION['exercicio']}'
    ORDER BY eqdcod
    ";


    echo $fm->getMensagens();
    $listagem = new Simec_Listagem();
    $listagem->setCabecalho(array (
    //    "Alterar/Excluir",
        "C&oacute;digo",
        "Descrição",
        "Ano"));
    $listagem->setQuery($strSqlGrid);
   $listagem->setAcoes(array(
        'edit' => 'editRegistro',
        'delete' => 'deleteRegistro'
    ));
    $listagem->addCallbackDeCampo(array('eqddsc' ), 'alinhaParaEsquerda');
    $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
    ?>
    <?php if (false === $listagem->render()): ?>
        <div class="alert alert-info col-md-4 col-md-offset-4 text-center">
            Nenhum registro encontrado
        </div>
    <?php endif; ?>

    <div class="form-group">
        <div class="col-lg-12" style="text-align: center;">
            <button class="btn btn-primary" id="voltar" type="button" onclick="voltar();">Voltar</button>
            <button type="button" class="btn btn-warning" onclick="cadastrar();">Novo</button>
        </div>
    </div>
</div>

<script type="text/javascript">
    function editRegistro(registro) {
        window.location = "planacomorc.php?modulo=sistema/tabelasapoio/cadastrarEnquadramentoDespesas&acao=A&execucao=editar&registro="+registro;
    }
     function cadastrar() {
        window.location = "planacomorc.php?modulo=sistema/tabelasapoio/cadastrarEnquadramentoDespesas&acao=A";
    }
    function voltar() {
        window.location = "planacomorc.php?modulo=inicio&acao=C";
    }

    function deleteRegistro(registro){
        if(confirm("Deseja realmente excluir este registro?")){
            window.location += '&execucao=excluir&registro='+registro;
        }
    }
$(document).ready(function(){


    /**
     * Submete um formulário
     */
    $("#fSubmit").on("click", function(e){
        e.preventDefault();

        var inputs = ['#eqdcod','#eqddsc','#eqdano']
          , frmData = $("#frmEnquadramento").serialize()
          , urlAction;

        for(var i in inputs) {

            if (!inputs.hasOwnProperty(i)) continue;

            if (!$(inputs[i]).val()) {
                notice("O campo "+$(inputs[i]).attr("title")+" deve ser preenchido!");
                $(inputs[i]).focus();
                return false;
            }
        }

        document.forms.frmEnquadramento.submit();
    });
});
</script>
</div>
