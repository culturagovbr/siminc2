<div class="col-sm-12 div_ptres">
    <div class="ibox float-e-margins">
        <div class="ibox-title">
            <h5>Detalhamento Orçamentário <span title="Campo obrigatório" class="link campo-obrigatorio">*</span></h5>
        </div>
        <div class="ibox-content">

            <div class="form-group">
                <div class="col-lg-12">
                    <table cellpadding="5" border="0" width="98%" align="center" id="orcamento" style="border:#c9c9c9 1px solid" onmouseover="tabindexcampo();"  class="table table-hover table-bordered table-hover" >
                        <tr>
                            <td rowspan="2" nowrap>
                                <b>PTRES</b>
                            </td>
                            <td rowspan="2" style="width:45%" nowrap>
                                <b>Funcional</b>
                            </td>
                            <td rowspan="2" nowrap>
                                <b>Dotação na Unidade(R$)</b>
                            </td>
                            <td colspan="2" nowrap>
                                <b><span id="detPi">Detalhado em PI Pela Unidade(R$)</span></b>
                            </td>
                            <td colspan="2">
                                <b>Empenho na Funcional(R$)</b>
                            </td>
                            <td rowspan="2" align="center">
                                <b>Valor do Projeto</b>
                            </td>
                        </tr>
                        <tr>
                            <td nowrap><b>Detalhado</b></td>
                            <td nowrap><b>Não&nbsp;Detalhado</b></td>
                            <td nowrap><b>Empenhado</b></td>
                            <td nowrap><b>Não&nbsp;Empenhado</b></td>
                        </tr>
                        <?php
                            if($dadosPI['pliid'] || $dadosPI['ptrid']){
                                if($_REQUEST['replicarProposta']){
                                    $sql_lista = montarSqlBuscarFuncionalImportacao((object)array(
                                        'exercicio' => $_SESSION['exercicio'],
                                        'ptrid' => $dadosPI['ptrid'],
                                    ));
//ver($sql_lista);
                                    $dadosPTRES = $db->carregar($sql_lista);
                                    if(is_array($dadosPTRES)) {
                                        $dadosPTRES = formatarValoresFuncional($dadosPTRES);
                                    }
                                    if($dadosPTRES) {
//ver($dadosPTRES);
                                        foreach ($dadosPTRES as $ptres):
                                            $vlrPiDetalhado = strip_tags($ptres['det_pi']);
                                            $vlrPiDetalhadoCusteio = '';
                                            $vlrPiDetalhadoCapital = '';
                                            $vlrPtresCusteio = strip_tags($ptres['ptrdotacaocusteio']);
                                            $vlrPtresCapital = strip_tags($ptres['ptrdotacaocapital']);
                                            $vlrPiNaoDetalhado = strip_tags($ptres['nao_det_pi']);
                                            $vlrPiNaoDetalhadoCusteio = strip_tags($ptres['nao_det_pi_custeio']);
                                            $vlrPiNaoDetalhadoCapital = strip_tags($ptres['nao_det_pi_capital']);
                                            ?>
                                            <tr style="height:30px;" id="ptres_<?php echo $ptres['ptrid']; ?>">
                                                <td align="center">
                                                    <span class="link btnVisualizarDetalhes" ptrid="<?php echo $ptres['ptrid']; ?>" ><?php echo $ptres['ptres']; ?></span>
                                                </td>
                                                <td align="left">
                                                    <span class="link btnVisualizarDetalhes" ptrid="<?php echo $ptres['ptrid']; ?>" ><?php echo $ptres['descricao']; ?></span>
                                                </td>
                                                <td align="right"><?php echo $ptres['dotacaoatual']; ?></td>
                                                <td align="right" id="td_pi_detalhado">
                                                    <?php echo $ptres['det_pi']; ?>
                                                </td>
                                                <td align="right" id="td_pi_nao_detalhado">
                                                    <?php echo $ptres['nao_det_pi']; ?>
                                                </td>
                                                <td align="right"><?php echo $ptres['empenhado']; ?></td>
                                                <td align="right">
                                                    <?php echo $ptres['nao_empenhado']; ?>
                                                    <input type="hidden" name="ptrid" value="<?php echo $ptres['ptrid']; ?>" />
                                                </td>
                                                <td align="center" id="td_valor_projeto">
                                                    <?php echo 'R$ '. $ptres['pipvalor']; ?>
                                                </td>
                                            </tr>
                                            <?php
                                        endforeach;
                                    }
                                } else {
                                    $dadosPTRES = buscarPTRES((object) array(
                                        'exercicio' => $_SESSION['exercicio'],
                                        'pliid' => $dadosPI['pliid'],
                                        'ptrid' => $dadosPI['ptrid'],
                                        'importar' => ($_REQUEST['replicarProposta']? TRUE: FALSE)
                                    ));
                                    if($dadosPTRES) {
//ver($dadosPTRES);
                                        foreach ($dadosPTRES as $ptres):
                                            $vlrPiDetalhado = strip_tags($ptres['det_pi']);
                                            $vlrPiDetalhadoCusteio = strip_tags($ptres['det_pi_custeio']);
                                            $vlrPiDetalhadoCapital = strip_tags($ptres['det_pi_capital']);
                                            $vlrPtresCusteio = strip_tags($ptres['ptrdotacaocusteio']);
                                            $vlrPtresCapital = strip_tags($ptres['ptrdotacaocapital']);
                                            $vlrPiNaoDetalhado = strip_tags($ptres['nao_det_pi']);
                                            $vlrPiNaoDetalhadoCusteio = strip_tags($ptres['nao_det_pi_custeio']);
                                            $vlrPiNaoDetalhadoCapital = strip_tags($ptres['nao_det_pi_capital']);
                                            ?>
                                            <tr style="height:30px;" id="ptres_<?php echo $ptres['ptrid']; ?>">
                                                <td align="center">
                                                    <span class="link btnVisualizarDetalhes" ptrid="<?php echo $ptres['ptrid']; ?>" ><?php echo $ptres['ptres']; ?></span>
                                                </td>
                                                <td align="left">
                                                    <span class="link btnVisualizarDetalhes" ptrid="<?php echo $ptres['ptrid']; ?>" ><?php echo $ptres['descricao']; ?></span>
                                                </td>
                                                <td align="right"><?php echo $ptres['dotacaoatual']; ?></td>
                                                <td align="right" id="td_pi_detalhado">
                                                    <?php echo $ptres['det_pi']; ?>
                                                </td>
                                                <td align="right" id="td_pi_nao_detalhado">
                                                    <?php echo $ptres['nao_det_pi']; ?>
                                                </td>
                                                <td align="right"><?php echo $ptres['empenhado']; ?></td>
                                                <td align="right">
                                                    <?php echo $ptres['nao_empenhado']; ?>
                                                    <input type="hidden" name="ptrid" value="<?php echo $ptres['ptrid']; ?>" />
                                                </td>
                                                <td align="center" id="td_valor_projeto">
                                                    <?php echo 'R$ '. $ptres['pipvalor']; ?>
                                                </td>
                                            </tr>
                                            <?php
                                        endforeach;
                                    }
                                }
                            }
                        ?>
                    </table>

                    <input type="hidden" id="ptresCusteio" value="<?php echo $vlrPtresCusteio; ?>" />
                    <input type="hidden" id="ptresCapital" value="<?php echo $vlrPtresCapital; ?>" />
                    <input type="hidden" id="piDetalhado" value="<?php echo $vlrPiDetalhado; ?>" />
                    <input type="hidden" id="piDetalhadoCusteio" value="<?php echo $vlrPiDetalhadoCusteio; ?>" />
                    <input type="hidden" id="piDetalhadoCapital" value="<?php echo $vlrPiDetalhadoCapital; ?>" />
                    <input type="hidden" id="piNaoDetalhadoCusteio" value="<?php echo $vlrPiNaoDetalhadoCusteio; ?>" />
                    <input type="hidden" id="piNaoDetalhadoCapital" value="<?php echo $vlrPiNaoDetalhadoCapital; ?>" />
                    <input type="hidden" id="piNaoDetalhado" value="<?php echo $vlrPiNaoDetalhado; ?>" />

                </div>
            </div>
            <div class="form-group text-right">
                <div class="col-lg-12">
                    <input type="button" id="btn_selecionar_functional" value="Selecionar Funcional/PTRES" class="btn btn-info" />
                </div>
            </div>

        </div>
    </div>
</div>