<?php

$fm = new Simec_Helper_FlashMessage('planacomorc/tc/cadexecorcfin');

    if($_REQUEST['execucao'] == 'excluir'){
        deleteRowExecutorOrcamentario($_REQUEST['registro']);
        $fm->addMensagem(
            'Registro excluido com sucesso.',
            Simec_Helper_FlashMessage::SUCESSO
        );
    }

include APPRAIZ . 'includes/cabecalho.inc';

?>
<div class="col-lg-12">
    <ol class="breadcrumb">
        <li><a href="<?php echo MODULO; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li class="active">Executor Orçamentário Financeiro</li>
    </ol>

<?php
$strSqlGrid = "
    SELECT
        pieid, piecod, piedsc, pieano
    FROM monitora.pi_executor
    WHERE
        piestatus='A' AND
        pieano = '{$_SESSION['exercicio']}'
    ORDER BY piecod
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
        window.location = "planacomorc.php?modulo=sistema/tabelasapoio/cadastrarExecutorOrcamentarioFinanceiro&acao=A&execucao=editar&registro="+registro;
    }
     function cadastrar() {
        window.location = "planacomorc.php?modulo=sistema/tabelasapoio/cadastrarExecutorOrcamentarioFinanceiro&acao=A";
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
     * abre um registro para edição
     */
    $(".edit-row").on("click", function(e){
        e.preventDefault();

        var $modal = $(".modal-dialog").css({width:"650px",height:"300px",display:"none"})
          , positionX = ($(window).width()/2) - ($modal.width()/2)
          , positionY = 120
          , rowid = $(this).attr("target-data-id")
          , urlAction = "planacomorc.php?modulo=sistema/tabelasapoio/cadExecutorOrcamentarioFinanceiro";

        urlAction+="&acao=A&editrow=true&rowid="+rowid;

        $modal.dialog({
            position: [positionX, positionY],
            height: $modal.height(),
            width: $modal.width(),
            title: "Editar registro",
            modal: true,
            open: function(event, ui) {
                callAjax(urlAction, function(xhr){
                    $("#pieid").val(xhr.pieid);
                    $("#piecod").val(xhr.piecod);
                    $("#piedsc").val(xhr.piedsc);
                    $("#pieano").val(xhr.pieano);
                });
            }
        });
    });

    /**
     * delete um registro do banco
     */
    $(".delete-row").on("click", function(e){
        e.preventDefault();

        if (!confirm("Deseja realmente apagar o registro?")) return false;

        if ($(this).attr("target-data-id")) {

            var urlAction = location.href+"&deleterow=true&rowid="+$(this).attr("target-data-id")
              , $row = $(this).parent().parent();

            callAjax(urlAction, function(xhr){

                if (xhr.result == 'success') {
                    notice("Registro apagado com sucesso!");
                    $row.remove();
                } else {
                    notice("Falha ao tentar apagar o registro.");
                }
            });
        }
    });

    /**
     * Abre o formulario para novas inserções
     */
    $("#rNovo").on("click", function(e){
        e.preventDefault();

        var $modal = $(".modal-dialog").css({width:"650px",height:"300px",display:"none"})
          , positionX = ($(window).width()/2) - ($modal.width()/2)
          , positionY = 120;

        $modal.dialog({
            position: [positionX, positionY],
            height: $modal.height(),
            width: $modal.width(),
            title: "Adicionar um novo",
            modal: true,
            open: function(event, ui) {
                $("#pieid").val("");
                $("#piecod").val("");
                $("#piedsc").val("");
                $("#pieano").val("<?= $_SESSION['exercicio_atual']; ?>");
            }
        });
    });

    /**
     * Submete um formulário
     */
    $("#fSubmit").on("click", function(e){
        e.preventDefault();

        var inputs = ['#piecod','#piedsc','#pieano']
          , frmData = $("#frmExecFinanceiro").serialize()
          , urlAction;

        for(var i in inputs) {

            if (!inputs.hasOwnProperty(i)) continue;

            if (!$(inputs[i]).val()) {
                notice("O campo "+$(inputs[i]).attr("title")+" deve ser preenchido!");
                $(inputs[i]).focus();
                return false;
            }
        }

        document.forms.frmExecFinanceiro.submit();
    });
});
</script>

