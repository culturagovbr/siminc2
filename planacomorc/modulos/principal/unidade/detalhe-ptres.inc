<?php
    $dado = count($dados) ? current($dados) : [];
    $urlBaseCadastro = $tipoPi == 'fnc'? 'planacomorc.php?modulo=principal/unidade/cadastro_pi_fnc&acao=A': 'planacomorc.php?modulo=principal/unidade/cadastro_pi&acao=A';
?>

<div class="row">
    <div class="col-md-12">
        <div class="ibox float-e-margins">
            <div class="ibox-title">
                <h5><?php echo count($dado) ? $dado['funcional'] . ' - ' . $dado['acatitulo'] : "PI's da Funcional"; ?></h5>
            </div>
            <div class="ibox-content">
                <?php if(count($dados)){ ?>
                    <div class="table-responsive">
                        <table class="table table-bordered table-hover " >
                            <thead>
                            <tr class="text-center">
                                <th>Ações</th>
                                <th>Id</th>
                                <th>Código</th>
                                <th>Título</th>
                                <th>RP</th>
                                <th>Custeio</th>
                                <th>Capital</th>
                                <th>Provisionado</th>
                                <th>Empenhado</th>
                                <th>Liquidado</th>
                                <th>Pago</th>
                            </tr>
                            </thead>
                            <tbody>
                            <?php
                            $total['custeio'] = $total['capital'] = $total['empenhado'] = $total['pago'] = $total['liquidado'] = 0;
                            foreach($dados as $dado){
                                $total['custeio'] += $dado['picvalorcusteio'];
                                $total['capital'] += $dado['picvalorcapital'];
                                $total['autorizado'] += $dado['autorizado'];
                                $total['empenhado'] += $dado['empenhado'];
                                $total['pago'] += $dado['pago'];
                                $total['liquidado'] += $dado['liquidado'];
                                
                                ?>
                                <tr>
                                    <td class="text-center">
                                        <a href="<?php echo $urlBaseCadastro; ?>&pliid=<?php echo $dado['pliid']; ?>" class="btn btn-primary btn-sm glyphicon glyphicon-pencil"></a>
                                    </td>
                                    <td>
                                        <a href="#" title="Exibir detalhes do Plano Interno(Espelho)" class="a_espelho" data-pi="<?php echo $dado['pliid']; ?>" >
                                        <?php echo $dado['pliid']; ?>
                                        </a>
                                    </td>
                                    <td>
                                        <a href="#" title="Exibir detalhes do Plano Interno(Espelho)" class="a_espelho" data-pi="<?php echo $dado['pliid']; ?>" >
                                        <?php echo $dado['plicod']; ?>
                                        </a>
                                    </td>
                                    <td>
                                        <a href="#" title="Exibir detalhes do Plano Interno(Espelho)" class="a_espelho" data-pi="<?php echo $dado['pliid']; ?>" >
                                        <?php echo $dado['plititulo']; ?>
                                        </a>
                                    </td>
                                    <td class="text-center"><?php echo $dado['irpcod']; ?></td>
                                    <td class="text-right"><?php echo number_format($dado['picvalorcusteio'], 0, ',', '.'); ?></td>
                                    <td class="text-right"><?php echo number_format($dado['picvalorcapital'], 0, ',', '.'); ?></td>
                                    <td class="text-right"><?php echo marcarVermelhoValorSuperior($dado['autorizado'], $dado['picvalorcusteio']+ $dado['picvalorcapital']); ?></td>
                                    <td class="text-right"><?php echo marcarVermelhoValorSuperior($dado['empenhado'], $dado['picvalorcusteio']+ $dado['picvalorcapital']); ?></td>
                                    <td class="text-right"><?php echo marcarVermelhoValorSuperior($dado['liquidado'], $dado['picvalorcusteio']+ $dado['picvalorcapital']); ?></td>
                                    <td class="text-right"><?php echo marcarVermelhoValorSuperior($dado['pago'], $dado['picvalorcusteio']+ $dado['picvalorcapital']); ?></td>
                                </tr>
                            <?php } ?>
                            </tbody>
                            <tfoot>
                                <tr>
                                    <th colspan="5">TOTAL PLANOS INTERNOS</th>
                                    <th class="text-right"><?php echo number_format($total['custeio'], 0, ',', '.'); ?></th>
                                    <th class="text-right"><?php echo number_format($total['capital'], 0, ',', '.'); ?></th>
                                    <th class="text-right"><?php echo marcarVermelhoValorSuperior($total['autorizado'], $total['custeio']+ $total['capital']); ?></th>
                                    <th class="text-right"><?php echo marcarVermelhoValorSuperior($total['empenhado'], $total['custeio']+ $total['capital']); ?></th>
                                    <th class="text-right"><?php echo marcarVermelhoValorSuperior($total['liquidado'], $total['custeio']+ $total['capital']); ?></th>
                                    <th class="text-right"><?php echo marcarVermelhoValorSuperior($total['pago'], $total['custeio']+ $total['capital']); ?></th>
                                </tr>
                                <tr>
                                    <th colspan="5">TOTAL NA FUNCIONAL</th>
                                    <th class="text-right"><?php echo number_format($totaisFuncional['custeio'], 0, ',', '.'); ?></th>
                                    <th class="text-right"><?php echo number_format($totaisFuncional['capital'], 0, ',', '.'); ?></th>
                                    <th class="text-right"><?php // echo marcarVermelhoValorSuperior($totaisFuncional['provisionado'], $total['custeio']+ $total['capital']); ?></th>
                                    <th class="text-right"><?php echo marcarVermelhoValorSuperior($totaisFuncional['empenhado'], $total['custeio']+ $total['capital']); ?></th>
                                    <th class="text-right"><?php echo marcarVermelhoValorSuperior($totaisFuncional['liquidado'], $total['custeio']+ $total['capital']); ?></th>
                                    <th class="text-right"><?php echo marcarVermelhoValorSuperior($totaisFuncional['pago'], $total['custeio']+ $total['capital']); ?></th>
                                </tr>
                            </tfoot>
                        </table>
                    </div>
                <?php } else {
                    echo '<div class="alert alert-danger">Não há PI vinculado a esta Funcional</div>';
                } ?>
            </div>
        </div>
    </div>
</div>
