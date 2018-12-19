<h2 style="color: #1AB394">2. Plano Interno
    <?php if ($podeEditarAlteracao === TRUE): ?>
        <a href="#" id="btnPi" title="Editar">
            <span class="btn btn-primary btn-sm glyphicon glyphicon-pencil"></span>
        </a>
    <?php endif; ?>
</h2>
<div class="tabs-container tabs">
    <ul class="nav nav-tabs prodTabs">
        <li class="active " style="list-style-type: none;">
            <a class="tab-tipo" data-toggle="tab" id="tp_interno" href="#tab-tabela-pi">Tabela</a>
        </li>
        <li class=" " style="list-style-type: none;">
            <a class="tab-tipo" data-toggle="tab" id="tp_externo" href="#tab-grafico-pi">Gráfico</a>
        </li>
    </ul>
    <div class="tab-content">
        <div id="tab-tabela-pi" class="tab-pane active">
            <table class="table table-striped table-bordered table-hover dataTables">
                <thead>
                    <tr class="text-center">
                        <th>Funcional</th>
                        <th>ID Planejamento</th>
                        <th>Título</th>
                        <th width="11%">Tipo</th>
                        <th width="11%">Saldo a Empenhar</th>
                        <th>Suplementar</th>
                        <th>Cancelar</th>
                        <th>Suplementar por Excesso</th>
                        <th>Suplementar por Superávit</th>
                        <th>Resultado Provável</th>
                    </tr>
                </thead>
                <tbody>
                <?php
                    $aDadosPiSelecionados = $mAlteracaoPedido->listaPisSelecionados($_REQUEST['pedid']);
                    foreach($aDadosPiSelecionados as $piSelecionado): ?>
                        <tr>
                            <td><?= $piSelecionado['funcional']; ?></td>
                            <td><?= $piSelecionado['pliid']; ?></td>
                            <td>
                                <?= $piSelecionado['plititulo']; ?><br>
                                <b>Unidade:</b> <?= $piSelecionado['unonome']; ?><br>
                                <b>Produto:</b> <?= $piSelecionado['pprnome']; ?>
                            </td>
                            <td align="left" style="padding: 0;font-size: 0;margin: 0;vertical-align: middle;">
                                <table class="table table-striped table-bordered table-hover" border="0" style="border: none;font-size: 10px; height: 100%; margin: 0">
                                    <tr>
                                        <td style="border: none;">
                                            <input type="hidden" id="picusteio<?= $piSelecionado['pliselid']; ?>" name="vlsaldoempenhocusteio" value="<?= number_format($piSelecionado['custeio'],0,',','.'); ?>">
                                            Custeio: R$ <?= number_format($piSelecionado['custeio'],0,',','.'); ?>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td style="border: none; border-top: 1px solid #e7eaec;">
                                            <input type="hidden" id="picapital<?= $piSelecionado['pliselid']; ?>" name="vlsaldoempenhocapital" value="<?= number_format($piSelecionado['capital'],0,',','.'); ?>">
                                            Capital: R$ <?= number_format($piSelecionado['capital'],0,',','.'); ?>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td style="border: none; border-top: 1px solid #e7eaec;">
                                            <input type="hidden" id="pifisico<?= $piSelecionado['pliselid']; ?>" name="vlsaldoempenhofisico" value="<?= number_format($piSelecionado['picquantidade'],0,',','.'); ?>">
                                            Físico: <?= number_format($piSelecionado['picquantidade'],0,',','.'); ?>
                                        </td>
                                    </tr>
                                </table>
                            </td>
                            <td style="vertical-align: middle;">
                                <?php
                                    $empenhado = $piSelecionado['custeio']+$piSelecionado['capital']-$piSelecionado['empenhado'];
                                    $empenhado = number_format($empenhado,0,',','.');
                                    echo "R$ ".$empenhado;
                                ?>
                                <input type="hidden" id="empenhado<?= $piSelecionado['pliselid']; ?>" value="<?= $empenhado; ?>">
                            </td>

                            <td align="left" style="padding: 0;font-size: 0;margin: 0;vertical-align: middle;">
                                <table class="table table-striped table-bordered table-hover" border="0" style="border: none;font-size: 10px; height: 100%; margin: 0">
                                    <tr>
                                        <td style="border: none;">
                                            <p><?= $piSelecionado['vlsuplementarcusteio'] ? number_format($piSelecionado['vlsuplementarcusteio'], 0, '', '.') : '-'; ?></p>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td style="border: none; border-top: 1px solid #e7eaec;">
                                            <p><?= $piSelecionado['vlsuplementarcapital'] ? number_format($piSelecionado['vlsuplementarcapital'], 0, '', '.') : '-'; ?></p>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td style="border: none; border-top: 1px solid #e7eaec;">
                                            <p><?= $piSelecionado['vlsuplementarfisico'] ? number_format($piSelecionado['vlsuplementarfisico'], 0, '', '.') : '-'; ?></p>
                                        </td>
                                    </tr>
                                </table>
                            </td>
                            <td align="left" style="padding: 0;font-size: 0;margin: 0;vertical-align: middle;">
                                <table class="table table-striped table-bordered table-hover" border="0" style="border: none;font-size: 10px; height: 100%; margin: 0">
                                    <tr>
                                        <td style="border: none;">
                                            <p><?= $piSelecionado['vlcancelarcusteio'] ? number_format($piSelecionado['vlcancelarcusteio'], 0, '', '.') : '-'; ?></p>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td style="border: none; border-top: 1px solid #e7eaec;">
                                            <p><?= $piSelecionado['vlcancelarcapital'] ? number_format($piSelecionado['vlcancelarcapital'], 0, '', '.') : '-'; ?></p>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td style="border: none; border-top: 1px solid #e7eaec;">
                                            <p><?= $piSelecionado['vlcancelarfisico'] ? number_format($piSelecionado['vlcancelarfisico'], 0, '', '.') : '-'; ?></p>
                                        </td>
                                    </tr>
                                </table>
                            </td>
                            <td align="left" style="padding: 0;font-size: 0;margin: 0;vertical-align: middle;">
                                <table class="table table-striped table-bordered table-hover" border="0" style="border: none;font-size: 10px; height: 100%; margin: 0">
                                    <tr>
                                        <td style="border: none;">
                                            <p><?= $piSelecionado['vlsuplementarexcessocusteio'] ? number_format($piSelecionado['vlsuplementarexcessocusteio'], 0 , '', '.') : '-'; ?></p>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td style="border: none; border-top: 1px solid #e7eaec;">
                                            <p><?= $piSelecionado['vlsuplementarexcessocapital'] ? number_format($piSelecionado['vlsuplementarexcessocapital'], 0, '', '.') : '-'; ?></p>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td style="border: none; border-top: 1px solid #e7eaec;">
                                            <p><?= $piSelecionado['vlsuplementarexcessofisico'] ? number_format($piSelecionado['vlsuplementarexcessofisico'], 0,'', '.') : '-'; ?></p>
                                        </td>
                                    </tr>
                                </table>
                            </td>
                            <td align="left" style="padding: 0;font-size: 0;margin: 0;vertical-align: middle;">
                                <table class="table table-striped table-bordered table-hover" border="0" style="border: none;font-size: 10px; height: 100%; margin: 0">
                                    <tr>
                                        <td style="border: none;">
                                            <p><?= $piSelecionado['vlsuplementarsuperavitcusteio'] ? number_format($piSelecionado['vlsuplementarsuperavitcusteio'], 0, '', '.') : '-'; ?></p>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td style="border: none; border-top: 1px solid #e7eaec;">
                                            <p><?= $piSelecionado['vlsuplementarsuperavitcapital'] ? number_format($piSelecionado['vlsuplementarsuperavitcapital'], 0, '', '.') : '-'; ?></p>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td style="border: none; border-top: 1px solid #e7eaec;">
                                            <p><?= $piSelecionado['vlsuplementarsuperavitfisico'] ? number_format($piSelecionado['vlsuplementarsuperavitfisico'], 0, '','.') : '-'; ?></p>
                                        </td>
                                    </tr>
                                </table>
                            </td>
                            <td align="left" style="padding: 0;font-size: 0;margin: 0;vertical-align: middle;">
                                <table class="table table-striped table-bordered table-hover" border="0" style="border: none;font-size: 10px; height: 100%; margin: 0">
                                    <tr>
                                        <td style="border: none;">
                                            <p><?= $piSelecionado['vldotacaocusteio'] ? number_format($piSelecionado['vldotacaocusteio'], 0, '', '.') : '-'; ?></p>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td style="border: none; border-top: 1px solid #e7eaec;">
                                            <p><?= $piSelecionado['vldotacaocapital'] ? number_format($piSelecionado['vldotacaocapital'], 0, '', '.') : '-'; ?></p>
                                        </td>
                                    </tr>
                                    <tr>
                                        <?php
                                            if ($piSelecionado['vlrprovavel']<0){
                                                $color='red';
                                            }else{
                                                $color='';
                                            }
                                        ?>
                                        <td style="color: <?=$color;?>; border: none; border-top: 1px solid #e7eaec;" id="fisico<?=$piSelecionado['pliselid'];?>">
                                            <?= $piSelecionado['vlrprovavel'] ? $piSelecionado['vlrprovavel'] : '0'; ?>
                                        </td>
                                    </tr>
                                </table>
                            </td>
                        </tr>
                    <?php endforeach ?>
                </tbody>
            </table>
        </div>

        <div id="tab-grafico-pi" class="tab-pane">

        </div>
    </div>
</div>
<script>
$(document).ready(function(){
    $("#btnPi").click(function(){
        $("#wizard-t-1").click();
    });
});
</script>