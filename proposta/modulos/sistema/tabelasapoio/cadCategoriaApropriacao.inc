<?php

$fm = new Simec_Helper_FlashMessage('planacomorc/tc/cadcategoriaapropriacao');


    if($_REQUEST['execucao'] == 'excluir'){
        deleteRowCategoriaApropriacao($_REQUEST['registro']);
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
            <li class="active">Modalidade de Pactuação</li>
        </ol>

    <?php
 $strSqlGrid = "
    SELECT capid,capcod,capdsc,capano
    FROM monitora.pi_categoriaapropriacao
    WHERE capano = '{$_SESSION['exercicio']}'
        AND capstatus = 'A'
    ORDER BY capcod
    ";


    echo $fm->getMensagens();
    $listagem = new Simec_Listagem();
    $listagem->setCabecalho(array (
        "Código",
        "Descrição",
        "Ano"));
    $listagem->setQuery($strSqlGrid);
    $listagem->setAcoes(array(
        'edit' => 'editRegistro',
        'delete' => 'deleteRegistro'
    ));
    $listagem->addCallbackDeCampo(array('capdsc' ), 'alinhaParaEsquerda');
    $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
    $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    ?>

    <div class="form-group">
        <div class="col-lg-12" style="text-align: center;">
            <button class="btn btn-primary" id="voltar" type="button" onclick="voltar();">Voltar</button>
            <button type="button" class="btn btn-warning" onclick="cadastrar();">Novo</button>
        </div>
    </div>
</div>

<script type="text/javascript">
    function editRegistro(registro) {
        window.location = "planacomorc.php?modulo=sistema/tabelasapoio/cadastrarCategoriaApropriacao&acao=A&execucao=editar&registro="+registro;
    }
     function cadastrar() {
        window.location = "planacomorc.php?modulo=sistema/tabelasapoio/cadastrarCategoriaApropriacao&acao=A";
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
