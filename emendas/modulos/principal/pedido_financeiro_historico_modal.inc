<?php
$historico = (new Emendas_Model_PedidoFinanceiro())->recuperaPedidoFinanceiroPorBenid($_REQUEST['benid']);

?>
<div class="ibox-content">
    <div class="table-responsive">
        <table class="table table-bordered table-hover" >
            <thead>
                <tr class="text-center">
                    <th width="5px" style="border-right: 1px black solid;">Data</th>
                    <th width="50px" style="border-right: 1px black solid;">Usuário</th>
                    <th width="50px" style="border-right: 1px black solid;">E-mail</th>
                    <th width="50px" style="border-right: 1px black solid;">Telefone</th>
                    <th width="1px" style="border-right: 1px black solid;">Vl. Custeio</th>
                    <th width="1px" style="border-right: 1px black solid;">Vl. Capital</th>
                    <th width="1px" style="border-right: 1px black solid;">Vinculação</th>
                </tr>
            </thead>
            <?php if($historico){ ?>
                <tbody>
                <?php foreach($historico as $lista): ?>
                    <tr class="lista_alteracoes">
                        <td style="border-right: 1px black solid; width: 20%; text-align: left;" class="text-right">
                            <?= $lista['dtcadastro']; ?>
                        </td>
                        <td style="border-right: 1px black solid; width: 30%; text-align: left;" class="text-right">
                            <?= $lista['usunome']; ?>
                        </td>
                        <td style="border-right: 1px black solid; width: 30%; text-align: left;" class="text-right">
                            <?= $lista['usuemail']; ?>
                        </td>
                        <td style="border-right: 1px black solid; width: 30%; text-align: left;" class="text-right">
                            <?= $lista['telefone']; ?>
                        </td>
                        <td style="border-right: 1px black solid; width: 30%; text-align: left;" class="text-right">
                            <?= number_format($lista['vlcusteiopedido'],0,',', '.' ); ?>
                        </td>
                        <td style="border-right: 1px black solid; width: 20%; text-align: left;" class="text-right">
                            <?= number_format($lista['vlcapitalpedido'],0,',', '.' ); ?>
                        </td>
                        <td style="border-right: 1px black solid; width: 20%; text-align: left;" class="text-right">
                            <?= $lista['vindsc']; ?>
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