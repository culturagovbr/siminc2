<?php

$listaHistorico = (new Emendas_Model_BeneficiarioHistorico())->recuperarHistorico($_REQUEST['benid']);
$listaAlteracoes = (new Emendas_Model_BeneficiarioHistoricoAlteracao())->recuperarAlteracoes($_REQUEST['benid']);

?>
<div class="ibox-content">
    <div class="table-responsive">
        <table class="table table-bordered table-hover" >
            <thead>
                <tr class="text-center">
                    <th width="5px" style="border-right: 1px black solid;">Data</th>
                    <th width="50px" style="border-right: 1px black solid;">Usuário</th>
                    <th width="1px" style="border-right: 1px black solid;">E-mail</th>
                    <th width="1px" style="border-right: 1px black solid;">Telefone</th>
                </tr>
            </thead>
            <?php if($listaHistorico){ ?>
                <tbody>
                <?php foreach($listaHistorico as $lista): ?>
                    <tr class="lista_alteracoes">
                        <td id="alteracao_<?= $lista['benid']; ?>" style="border-right: 1px black solid; width: 20%; text-align: left;" class="text-right">
                            <i class="fa fa-plus detalhar-linha link" data-behid="<?= $lista['behid']; ?>"></i>&nbsp;&nbsp;
                            <?= $lista['behdata']; ?>
                        </td>
                        <td id="alteracao_<?= $lista['benid']; ?>" style="border-right: 1px black solid; width: 30%; text-align: left;" class="text-right">
                            <?= $lista['usunome']; ?>
                        </td>
                        <td id="alteracao_<?= $lista['benid']; ?>" style="border-right: 1px black solid; width: 30%; text-align: left;" class="text-right">
                            <?= $lista['usuemail']; ?>
                        </td>
                        <td id="alteracao_<?= $lista['benid']; ?>" style="border-right: 1px black solid; width: 20%; text-align: left;" class="text-right">
                            <?= $lista['telefone']; ?>
                        </td>
                    </tr>
                    <tr class="tr_<?= $lista['behid']; ?> tr_detalhe" style="display: none;">
                        <td colspan="4">
                            <table class="table table-bordered table-hover" >
                                <thead>
                                    <tr class="text-center">
                                        <th style="border-right: 1px black solid;">Informação alterada</th>
                                        <th style="border-right: 1px black solid;">de</th>
                                        <th style="border-right: 1px black solid;">para</th>
                                    </tr>
                                </thead>
                                <tbody>
                            <?php foreach($listaAlteracoes as $alteracoes) :
                                if( $alteracoes['behid'] == $lista['behid']) : ?>
                                <tr>
                                    <td style="border-right: 1px black solid; text-align: center;" >
                                        <?= $alteracoes['bahcampo']; ?>
                                    </td>
                                    <td style="border-right: 1px black solid; text-align: left;" class="text-right">
                                        <?= $alteracoes['bahanterior']; ?>
                                    </td>
                                    <td class="text-right" style="border-right: 1px black solid; text-align: left;">
                                        <?= $alteracoes['bahnovo']; ?>
                                    </td>
                                </tr>
                            <?php endif; ?>
                            <?php endforeach; ?>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                <?php endforeach; ?>
                </tbody>
            <?php } else { ?>
                <div class="alert alert-danger">Não há Histórico de alterações para este Beneficiário</div>
            <?php } ?>
        </table>
    </div>
</div>

<script>
    $(function(){

        $('.detalhar-linha').click(function(){

            var behid = $(this).data('behid');

            if($(this).hasClass('fa-minus')){
                $(this).removeClass('fa-minus').addClass('fa-plus');
                $('.tr_' + behid).hide();
            } else {
                $(this).removeClass('fa-plus').addClass('fa-minus');
                $('.tr_' + behid).show('slow');
            }
        });

    });
</script>