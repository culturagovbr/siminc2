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
            <a class="tab-tipo" data-toggle="tab" id="tp_externo" href="#tab-grafico-pi">Gr�fico</a>
        </li>
    </ul>
    <div class="tab-content">
        <div id="tab-tabela-pi" class="tab-pane active">
            <table class="table table-striped table-bordered table-hover dataTables">
                <thead>
                    <tr class="text-center">
                        <th>Funcional</th>
                        <th>ID Planejamento</th>
                        <th>T�tulo</th>
                        <th width="11%">Valor do Projeto</th>
                        <th width="11%">Dispon�vel</th>
                        <th>Cancelar</th>
                        <th>Suplementar</th>
                        <th>Suplementar por Excesso</th>
                        <th>Suplementar por Super�vit</th>
                        <th>Resultado Prov�vel</th>
                    </tr>
                </thead>
                <tbody>
                <?php
                    $aDadosPiSelecionados = $mAlteracaoPedido->listaPisSelecionados( (object)[
                        'pedid' => $_REQUEST['pedid'],
                        'exercicio' => $_SESSION['exercicio']
                    ]);
                    foreach($aDadosPiSelecionados as $piSelecionado): 
                        $vlrAEmpenharCusteio = (int)$piSelecionado['custeio'] - (int)$piSelecionado['vlrempenhadocusteio'];
                        $vlrAEmpenharCapital = (int)$piSelecionado['capital'] - (int)$piSelecionado['vlrempenhadocapital'];
                        ?>
                        <tr>
                            <td>
                                <a href="#" title="Exibir detalhes do Plano Interno(Espelho)" class="a_espelho" data-pi="<?=$piSelecionado['pliid'];?>">
                                    <?= $piSelecionado['funcional']; ?>
                                </a>
                            </td>
                            <td>
                                <a href="#" title="Exibir detalhes do Plano Interno(Espelho)" class="a_espelho" data-pi="<?=$piSelecionado['pliid'];?>">
                                    <?= $piSelecionado['pliid']; ?>
                                </a>
                            </td>
                            <td>
                                <a href="#" title="Exibir detalhes do Plano Interno(Espelho)" class="a_espelho" data-pi="<?=$piSelecionado['pliid'];?>">
                                    <?= $piSelecionado['plititulo']; ?><br>
                                    <b>Unidade:</b> <?= $piSelecionado['unonome']; ?><br>
                                    <b>Produto:</b> <?= $piSelecionado['pprnome']; ?>
                                </a>
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
                                            F�sico: <?= number_format($piSelecionado['picquantidade'],0,',','.'); ?>
                                        </td>
                                    </tr>
                                </table>
                            </td>
                            <td align="left" style="padding: 0;font-size: 0;margin: 0;vertical-align: middle;">
                                <table class="table table-striped table-bordered table-hover" border="0" style="border: none;font-size: 10px; height: 100%; margin: 0">
                                    <tr>
                                        <td style="border: none;">
                                            <input type="hidden" id="vlempenharcusteio<?=$dados['pliselid'];?>" name="vlempenharcusteio" value="<?= number_format($vlrAEmpenharCusteio-$dados['vltotalcancelarcusteio'],0,',','.'); ?>">
                                            Custeio: R$ <?= number_format($vlrAEmpenharCusteio-$dados['vltotalcancelarcusteio']-$dados['vlcancelarcusteio'],0,',','.'); ?>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td style="border: none; border-top: 1px solid #e7eaec;">
                                            <input type="hidden" id="vlempenharcapital<?=$dados['pliselid'];?>" name="vlempenharcapital" value="<?= number_format($vlrAEmpenharCapital-$dados['vltotalcancelarcapital'],0,',','.'); ?>">
                                            Capital: R$ <?= number_format($vlrAEmpenharCapital-$dados['vltotalcancelarcapital']-$dados['vlcancelarcapital'],0,',','.'); ?>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td style="border: none; border-top: 1px solid #e7eaec;">
                                            <input type="hidden" id="vlempenharfisico<?=$dados['pliselid'];?>" name="vlempenharfisico" value="<?= number_format($dados['fisico']-$dados['vltotalcancelarfisico']-$dados['vlcancelarfisico'],0,',','.'); ?>">
                                            F�sico: <?= number_format($dados['fisico']-$dados['vltotalcancelarfisico']-$dados['vlcancelarfisico'],0,',','.'); ?>
                                        </td>
                                    </tr>
                                </table>                
                            </td>
                            <td align="left" style="padding: 0;font-size: 0;margin: 0;vertical-align: middle;">
                                <table class="table table-striped table-bordered table-hover" border="0" style="border: none;font-size: 10px; height: 100%; margin: 0">
                                    <tr>
                                        <td style="border: none;">
                                            <?= $piSelecionado['vlcancelarcusteio'] ? number_format($piSelecionado['vlcancelarcusteio'], 0, '', '.') : '-'; ?>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td style="border: none; border-top: 1px solid #e7eaec;">
                                            <?= $piSelecionado['vlcancelarcapital'] ? number_format($piSelecionado['vlcancelarcapital'], 0, '', '.') : '-'; ?>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td style="border: none; border-top: 1px solid #e7eaec;">
                                            <?= $piSelecionado['vlcancelarfisico'] ? number_format($piSelecionado['vlcancelarfisico'], 0, '', '.') : '-'; ?>
                                        </td>
                                    </tr>
                                </table>
                            </td>
                            <td align="left" style="padding: 0;font-size: 0;margin: 0;vertical-align: middle;">
                                <table class="table table-striped table-bordered table-hover" border="0" style="border: none;font-size: 10px; height: 100%; margin: 0">
                                    <tr>
                                        <td style="border: none;">
                                            <?= $piSelecionado['vlsuplementarcusteio'] ? number_format($piSelecionado['vlsuplementarcusteio'], 0, '', '.') : '-'; ?>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td style="border: none; border-top: 1px solid #e7eaec;">
                                            <?= $piSelecionado['vlsuplementarcapital'] ? number_format($piSelecionado['vlsuplementarcapital'], 0, '', '.') : '-'; ?>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td style="border: none; border-top: 1px solid #e7eaec;">
                                            <?= $piSelecionado['vlsuplementarfisico'] ? number_format($piSelecionado['vlsuplementarfisico'], 0, '', '.') : '-'; ?>
                                        </td>
                                    </tr>
                                </table>
                            </td>
                            <td align="left" style="padding: 0;font-size: 0;margin: 0;vertical-align: middle;">
                                <table class="table table-striped table-bordered table-hover" border="0" style="border: none;font-size: 10px; height: 100%; margin: 0">
                                    <tr>
                                        <td style="border: none;">
                                            <?= $piSelecionado['vlsuplementarexcessocusteio'] ? number_format($piSelecionado['vlsuplementarexcessocusteio'], 0 , '', '.') : '-'; ?>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td style="border: none; border-top: 1px solid #e7eaec;">
                                            <?= $piSelecionado['vlsuplementarexcessocapital'] ? number_format($piSelecionado['vlsuplementarexcessocapital'], 0, '', '.') : '-'; ?>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td style="border: none; border-top: 1px solid #e7eaec;">
                                            <?= $piSelecionado['vlsuplementarexcessofisico'] ? number_format($piSelecionado['vlsuplementarexcessofisico'], 0,'', '.') : '-'; ?>
                                        </td>
                                    </tr>
                                </table>
                            </td>
                            <td align="left" style="padding: 0;font-size: 0;margin: 0;vertical-align: middle;">
                                <table class="table table-striped table-bordered table-hover" border="0" style="border: none;font-size: 10px; height: 100%; margin: 0">
                                    <tr>
                                        <td style="border: none;">
                                            <?= $piSelecionado['vlsuplementarsuperavitcusteio'] ? number_format($piSelecionado['vlsuplementarsuperavitcusteio'], 0, '', '.') : '-'; ?>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td style="border: none; border-top: 1px solid #e7eaec;">
                                            <?= $piSelecionado['vlsuplementarsuperavitcapital'] ? number_format($piSelecionado['vlsuplementarsuperavitcapital'], 0, '', '.') : '-'; ?>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td style="border: none; border-top: 1px solid #e7eaec;">
                                            <?= $piSelecionado['vlsuplementarsuperavitfisico'] ? number_format($piSelecionado['vlsuplementarsuperavitfisico'], 0, '','.') : '-'; ?>
                                        </td>
                                    </tr>
                                </table>
                            </td>
                            <td align="left" style="padding: 0;font-size: 0;margin: 0;vertical-align: middle;">
                                <table class="table table-striped table-bordered table-hover" border="0" style="border: none;font-size: 10px; height: 100%; margin: 0">
                                    <tr>
                                        <td style="border: none;">
                                            <?= $piSelecionado['vldotacaocusteio'] ? number_format($piSelecionado['vldotacaocusteio'], 0, '', '.') : '-'; ?>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td style="border: none; border-top: 1px solid #e7eaec;">
                                            <?= $piSelecionado['vldotacaocapital'] ? number_format($piSelecionado['vldotacaocapital'], 0, '', '.') : '-'; ?>
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