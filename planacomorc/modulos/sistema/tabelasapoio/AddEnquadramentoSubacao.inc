<?php

$fm = new Simec_Helper_FlashMessage('planacomorc/tc/enquadramentosubacao');

/**
 * Get Param INPUT_GET
 * @param string $param
 * @param string $default
 * @return String
 */
function getFilterParam($param, $default = null) {
    return (isset($_GET[$param])) ? $_GET[$param] : $default;
}

if (count($_POST) > 0) {

    $strSQLDelete = "DELETE FROM pi_subacaoenquadramento WHERE sbaid=%s";
    foreach ($_POST['eqdid'] as $_eqdid) {
        $stmt = sprintf($strSQLDelete, $_POST['sbaid_disable']);
        $db->executar($stmt);
        $db->commit();
    }

    $strBase = "INSERT INTO pi_subacaoenquadramento(sbaid, eqdid) VALUES('%s', '%s')";
    foreach ($_POST['eqdid'] as $_eqdid) {
        $strInsert = sprintf($strBase, $_POST['sbaid_disable'], $_eqdid);
        $db->executar($strInsert);
        $db->commit();
    }
    $fm->addMensagem(
            'Enquadramento(s) vinculado(s) a subação.', Simec_Helper_FlashMessage::SUCESSO
    );
}

include APPRAIZ . 'includes/cabecalho.inc';

?>
<div class="row col-md-12" style="min-height: 550px;">
    <ol class="breadcrumb">
        <li><a href="<?php echo MODULO; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li class="">Tabelas de Apoio</li>
        <li class="active">Adicionar Enquadramento a subação</li>
    </ol>

    <?php echo $fm->getMensagens(); ?>
    <div class="well">
        <form method="POST" action="" class="form-horizontal" name="frmEnqdrSubacao" id="frmEnqdrSubacao">

            <div class="form-group">
                <label class="control-label col-md-2" for="mcrid">Selecione a Subação:</label>
                <div class="col-md-10">
                    <?php
                    $strSqlSubacao = "SELECT sbaid AS codigo, sbacod ||' - '|| sbatitulo AS descricao FROM monitora.pi_subacao WHERE sbaano = '{$_SESSION['exercicio']}' ORDER BY sbacod";
                    $db->monta_combo('sbaid', $strSqlSubacao, '', 'Selecione', null, null, null, null, '', 'changeSubacao', null, getFilterParam('sbaid', ''), null, 'class="form-control chosen-select" style="width=100%"');
                    ?>
                </div>
            </div>
            <?php if (isset($_GET['sbaid'])) : ?>
                <div class="form-group">
                    <label class="control-label col-md-2" for="mcrid">Selecione o Enquadramento:</label>
                    <div class="col-md-10">
                        <? $sqlEqdid = "
                                SELECT ed.eqdid AS codigo, ed.eqdcod ||' - '||ed.eqddsc AS descricao
                                FROM planejamento.enquadramento_despesa ed
                                WHERE ed.eqdano = '{$_SESSION['exercicio']}'";
                            $strItens = "SELECT se.eqdid AS codigo
                                FROM monitora.pi_subacaoenquadramento se
                                INNER JOIN planejamento.enquadramento_despesa ed ON (ed.eqdid = se.eqdid)
                                WHERE se.sbaid = {$_GET['sbaid']}
                                    AND ed.eqdano = '{$_SESSION['exercicio']}'";
                            $rs = $db->carregar($strItens);
                            $new = array();
                            if($rs){
                                foreach($rs as $key){
                                    $new[] = $key['codigo'];
                                }
                            }
                        ?>
                        <? inputCombo('eqdid[]', $sqlEqdid, $new, 'eqdid', array('multiple' => 'multiple', 'titulo' => 'Selecione')); ?>
                    </div>
                </div>


                <div class="form-group row">
                    <div class="col-md-10 col-md-offset-2">
                        <button type="button" class="btn btn-primary save" name="enviar">Salvar</button>
                    </div>
                </div>
            <?php endif; ?>
            </table>
        </form>
    </div>
</div>
    <script type="text/javascript">
        $(function() {
            $("#changeSubacao").on("change", function(e) {
                e.preventDefault();
                var urlAction = location.href.replace(/&sbaid=\d+/, "");
                location.href = urlAction + "&sbaid=" + $(this).val();
            });

            $(".save").on("click", function(e) {
                e.preventDefault();
                //selectAllOptions( document.getElementById( 'eqdid' ) );
                if($('#eqdid').val() === null){
                    bootbox.alert('Selecione ao menos um enquadramento para poder salvar.');
                    $('#eqdid').parent().parent().addClass('has-error');
                    return false;
                }
                $('#eqdid').parent().parent().removeClass('has-error');
                document.getElementById('frmEnqdrSubacao').submit();
            });
        });
    </script>