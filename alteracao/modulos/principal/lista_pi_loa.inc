<table id="tbListaPisSelecionados" class="table table-striped table-bordered table-hover dataTables" >
    <thead>
    <tr class="text-center">
        <th>Funcional</th>
        <th>Id Planejamento</th>
        <th>Número do PI</th>
        <th>Título</th>
        <th width="11%">Tipo</th>
        <th>Suplementar</th>
        <th>Cancelado</th>
        <th>Suplementar por Excesso</th>
        <th>Suplementar por Superávit</th>
    </tr>
    </thead>
    <tbody>
    <?php
    foreach($aDadosPiSelecionados as $dados): ?>
        <tr>
            <td ><?= $dados['funcional']; ?></td>
            <td ><?= $dados['pliid']; ?></td>
            <td ><?= $dados['plicod']; ?></td>
            <td ><?= $dados['plititulo']; ?></td>
            <td align="left" style="padding: 0;font-size: 0;margin: 0;vertical-align: middle;">
                <table class="table table-striped table-bordered table-hover" border="0" style="border: none;font-size: 10px; height: 100%; margin: 0">
                    <tr>
                        <td style="border: none;">
                            <input type="hidden" id="picusteio<?=$dados['pliselid'];?>" value="<?= number_format($dados['custeio'],2,',','.'); ?>">
                            Custeio: R$ <?= number_format($dados['custeio'],2,',','.'); ?>
                        </td>
                    </tr>
                    <tr>
                        <td style="border: none; border-top: 1px solid #e7eaec;">
                            <input type="hidden" id="picapital<?=$dados['pliselid'];?>" value="<?= number_format($dados['capital'],2,',','.'); ?>">
                            Capital: R$ <?= number_format($dados['capital'],2,',','.'); ?>
                        </td>
                    </tr>
                    <tr>
                        <td style="border: none; border-top: 1px solid #e7eaec;">
                            <input type="hidden" id="picquantidade<?=$dados['pliselid'];?>" value="<?= number_format($dados['picquantidade'],2,',','.'); ?>">
                            Físico: <?= number_format($dados['picquantidade'],0,',','.'); ?>
                        </td>
                    </tr>
                </table>
            </td>
            <td align="left" style="padding: 0;font-size: 0;margin: 0;vertical-align: middle;">
                <table class="table table-striped table-bordered table-hover" border="0" style="border: none;font-size: 10px; height: 100%; margin: 0">
                    <tr>
                        <td style="border: none;">
                            <?=number_format($dados['vlsuplementarcusteio'],2,',','.');?>
                        </td>
                    </tr>
                    <tr>
                        <td style="border: none; border-top: 1px solid #e7eaec;">
                            <?=number_format($dados['vlsuplementarcapital'],2,',','.');?>
                        </td>
                    </tr>
                    <tr>
                        <td style="border: none; border-top: 1px solid #e7eaec;">
                            <?=number_format($dados['plisuplementarquantidade'],2,',','.');?>
                        </td>
                    </tr>
                </table>
            </td>
            <td align="left" style="padding: 0;font-size: 0;margin: 0;vertical-align: middle;">
                <table class="table table-striped table-bordered table-hover" border="0" style="border: none;font-size: 10px; height: 100%; margin: 0">
                    <tr>
                        <td style="border: none;">
                            <?=number_format($dados['vlcancelarcusteio'],2,',','.');?>
                        </td>
                    </tr>
                    <tr>
                        <td style="border: none; border-top: 1px solid #e7eaec;">
                            <?=number_format($dados['vlcancelarcapital'],2,',','.');?>
                        </td>
                    </tr>
                    <tr>
                        <td style="border: none; border-top: 1px solid #e7eaec;">
                            <?=number_format($dados['plicancelarquantidade'],2,',','.');?>
                        </td>
                    </tr>
                </table>
            </td>
            <td align="left" style="padding: 0;font-size: 0;margin: 0;vertical-align: middle;">
                <table class="table table-striped table-bordered table-hover" border="0" style="border: none;font-size: 10px; height: 100%; margin: 0">
                    <tr>
                        <td style="border: none;">
                            <?=number_format($dados['vlsuplementarexcessocusteio'],2,',','.');?>
                        </td>
                    </tr>
                    <tr>
                        <td style="border: none; border-top: 1px solid #e7eaec;">
                            <?=number_format($dados['vlsuplementarexcessocapital'],2,',','.');?>
                        </td>
                    </tr>
                    <tr>
                        <td style="border: none; border-top: 1px solid #e7eaec;">
                            <?=number_format($dados['plisuplementarexcessoquantidade'],2,',','.');?>
                        </td>
                    </tr>
                </table>
            </td>
            <td align="left" style="padding: 0;font-size: 0;margin: 0;vertical-align: middle;">
                <table class="table table-striped table-bordered table-hover" border="0" style="border: none;font-size: 10px; height: 100%; margin: 0">
                    <tr>
                        <td style="border: none;">
                            <?=number_format($dados['vlsuplementarsuperavitcusteio'],2,',','.');?>
                        </td>
                    </tr>
                    <tr>
                        <td style="border: none; border-top: 1px solid #e7eaec;">
                            <?=number_format($dados['vlsuplementarsuperavitcapital'],2,',','.');?>
                        </td>
                    </tr>
                    <tr>
                        <td style="border: none; border-top: 1px solid #e7eaec;">
                            <?=number_format($dados['plisuplementarsuperavitquantidade'],2,',','.');?>
                        </td>
                    </tr>
                </table>
            </td>
        </tr>
    <?php endforeach ?>
    </tbody>
</table>