<?php
    $mpnid = $_REQUEST['mpnid'];
    $suoid = $_REQUEST['suoid'];
    $mPi = new Pi_PlanoInterno();
    $aPis = $mPi->recuperarPiPorMetaPncESubunidade($mpnid, $suoid);
    if(count($aPis)):
?>
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
                $aTotal = [];
                foreach($aPis as $dados){
                    $aTotal['previsto'] += $dados['previsto'];
                    $aTotal['autorizado'] += $dados['autorizado'];
                    $aTotal['empenhado'] += $dados['empenhado'];
                    $aTotal['liquidado'] += $dados['liquidado'];
                    $aTotal['pago'] += $dados['pago'];
                ?>
                <tr>
                    <td>
                        <a href="#" class="a_espelho btn btn-primary btn-sm glyphicon glyphicon-eye-open" data-pi="<?php echo $dados['pliid']; ?>"></a>
                    </td>
                    <td><?php echo $dados['plicod']; ?></td>
                    <td><?php echo $dados['plititulo']; ?></td>
                    <td class="text-right"><?php echo number_format($dados['previsto'], 2, ',', '.'); ?></td>
                    <td class="text-right"><?php echo number_format($dados['autorizado'], 2, ',', '.'); ?></td>
                    <td class="text-right"><?php echo number_format($dados['empenhado'], 2, ',', '.'); ?></td>
                    <td class="text-right"><?php echo number_format($dados['liquidado'], 2, ',', '.'); ?></td>
                    <td class="text-right"><?php echo number_format($dados['pago'], 2, ',', '.'); ?></td>
                </tr>
            <?php } ?>
            </tbody>
            <tfoot>
                <tr>
                    <th colspan="3">TOTAL</th>
                    <th class="text-right"><?php echo number_format($aTotal['previsto'], 2, ',', '.'); ?></th>
                    <th class="text-right"><?php echo number_format($aTotal['autorizado'], 2, ',', '.'); ?></th>
                    <th class="text-right"><?php echo number_format($aTotal['empenhado'], 2, ',', '.'); ?></th>
                    <th class="text-right"><?php echo number_format($aTotal['liquidado'], 2, ',', '.'); ?></th>
                    <th class="text-right"><?php echo number_format($aTotal['pago'], 2, ',', '.'); ?></th>
                </tr>
            </tfoot>
        </table>
    </div>
<?php
    else:
        echo '<div class="alert alert-danger">Nenhum PI vinculado</div>';
    endif;
?>

<script>
    $(function(){
        initLinkEspelho();
    });
</script>
