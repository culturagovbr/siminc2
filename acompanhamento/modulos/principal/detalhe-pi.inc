<?php if(count($listaPi)): ?>
    <div class="table-responsive">
        <table class="table table-striped table-bordered table-hover bedidTables" >
            <thead>
                <tr class="text-center">
                    <th>Ações</th>
                    <th>Código</th>
                    <th>Título</th>
                    <th>Vl. Previsto (R$)</th>
                    <th>Vl. Provisionado (R$)</th>
                    <th>Vl. Empenhado (R$)</th>
                    <th>Vl. Liquidado (R$)</th>
                    <th>Vl. Pago (R$)</th>
                </tr>
            </thead>
            <tbody>
            <?php
                $totais = [];
                foreach($listaPi as $pi){
                    $totais['previsto'] += $pi['previsto'];
                    $totais['autorizado'] += $pi['autorizado'];
                    $totais['empenhado'] += $pi['empenhado'];
                    $totais['liquidado'] += $pi['liquidado'];
                    $totais['pago'] += $pi['pago'];
                ?>
                <tr>
                    <td>
                        <a href="#" class="a_espelho btn btn-primary btn-sm glyphicon glyphicon-eye-open" data-pi="<?php echo $pi['pliid']; ?>"></a>
                    </td>
                    <td><?php echo $pi['plicod']; ?></td>
                    <td><?php echo $pi['plititulo']; ?></td>
                    <td class="text-right"><?php echo number_format($pi['previsto'], 2, ',', '.'); ?></td>
                    <td class="text-right"><?php echo number_format($pi['autorizado'], 2, ',', '.'); ?></td>
                    <td class="text-right"><?php echo number_format($pi['empenhado'], 2, ',', '.'); ?></td>
                    <td class="text-right"><?php echo number_format($pi['liquidado'], 2, ',', '.'); ?></td>
                    <td class="text-right"><?php echo number_format($pi['pago'], 2, ',', '.'); ?></td>
                </tr>
            <?php } ?>
            </tbody>
            <tfoot>
                <tr>
                    <th colspan="3">TOTAL</th>
                    <th class="text-right"><?php echo number_format($totais['previsto'], 2, ',', '.'); ?></th>
                    <th class="text-right"><?php echo number_format($totais['autorizado'], 2, ',', '.'); ?></th>
                    <th class="text-right"><?php echo number_format($totais['empenhado'], 2, ',', '.'); ?></th>
                    <th class="text-right"><?php echo number_format($totais['liquidado'], 2, ',', '.'); ?></th>
                    <th class="text-right"><?php echo number_format($totais['pago'], 2, ',', '.'); ?></th>
                </tr>
            </tfoot>
        </table>
    </div>
<?php else: ?>
    <div class="alert alert-danger">Nenhum PI vinculado</div>
<?php endif; ?>

<script>
    $(function(){
        initLinkEspelho();
    });
</script>
