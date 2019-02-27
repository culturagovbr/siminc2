<div class="col-md-6">
    <div class="ibox float-e-margins">
        <div class="ibox-title">
            <h5>Informações Complementares:</h5>
        </div>
        <div class="ibox-content">
            <div class="form-group" id="div_area_cultural">
                <div class="col-md-3 text-right">
                    <label for="mdeid" class="control-label">Área Cultural: <span title="Campo obrigatório" class="link campo-obrigatorio">*</span></label>
                </div>
                <div class="col-lg-9">
                    <?php
                        $sql = "
                            SELECT
                                MAX(mdeid) AS codigo,
                                upper(public.removeacento(mdedsc)) AS descricao
                            FROM monitora.pi_modalidadeensino
                            WHERE
                                mdestatus = 'A'
                                AND mdeano = '{$_SESSION['exercicio']}' -- Resolver 2014
                            GROUP BY
                                descricao
                            ORDER BY
                                descricao ASC
                        ";

                        $mdeid = $dadosPI['mdeid'];
                        $db->monta_combo('mdeid', $sql, 'S', 'Selecione', '', null, null, null, 'N', 'mdeid', null, '', null, 'class="form-control chosen-select" style="width=100%;"', null, (isset($mdeid) ? $mdeid : null));
                    ?>
                </div>
            </div>
            <div class="form-group" id="div_segmento_cultural">
                <div class="col-md-3 text-right">
                    <label class="control-label" for="neeid">Segmento Cultural: <span title="Campo obrigatório" class="link campo-obrigatorio">*</span></label>
                </div>
                <div class="col-lg-9 div_neeid">
                    <?php carregarSegmentoCultural($dadosPI['mdeid'], $dadosPI['neeid']); ?>
                </div>
            </div>
            <div class="form-group" id="div_pactuacao">
                <div class="col-md-3 text-right">
                    <label class="control-label" for="capid">Modalidade de Pactuação: <span title="Campo obrigatório" class="link campo-obrigatorio">*</span></label>
                </div>
                <div class="col-lg-9">
                    <?php
                        $sql = "
                            SELECT
                                capid AS codigo,
                                capdsc AS descricao
                            FROM monitora.pi_categoriaapropriacao
                            WHERE
                                capano = '{$_SESSION['exercicio']}'
                                AND capstatus = 'A'
                            ORDER BY
                                descricao
                        ";
                        $capid = $dadosPI['capid'];
                        $db->monta_combo('capid', $sql, 'S', 'Selecione', '', null, null, null, 'N', 'capid', null, '', null, 'class="form-control chosen-select" style="width=100%;"', null, (isset($capid) ? $capid : null));
                    ?>
                </div>
            </div>
            <div class="panel panel-default" id="div_siconv">
                <div class="panel-body">
                    Números de Proposta SICONV
                    <div class="form-group">
                        <div class="col-lg-9">
                            <input type="text" placeholder="Digite um número e clique em adicionar." value="" class="CampoEstilo normal inteiro form-control" title="Número de Convênio" id="input_convenio"  maxlength="10" name="input_convenio" />
                        </div>
                        <div class="col-lg-3">
                            <input type="button"  id="btn_selecionar_convenio" value="Adicionar" class="btn btn-info" style="width: 100%;"/>
                        </div>
                    </div>
                    <div class="form-group">
                        <div class="col-lg-9">
                            <table id="table_convenio" cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                                <?php foreach($aConvenio as $convenio){ ?>
                                    <tr style="height: 30px;" id="tr_convenio_<?php echo $convenio['pcoid']; ?>" >
                                        <td style="text-align: left;"><?php echo $convenio['pcoconvenio']; ?></td>
                                        <td style="text-align: center;">
                                            <input type="hidden" name="lista_convenio[]" value="<?php echo $convenio['pcoconvenio']; ?>" />
                                            <span class="glyphicon glyphicon-trash link btnRemoveConvenio" data-convenio="<?php echo $convenio['pcoid']; ?>" ></span>
                                        </td>
                                    </tr>
                                <?php } ?>
                            </table>
                        </div>
                    </div>
                </div>
            </div>

            <div class="form-group" id="div_ted">
                <div class="col-md-3 text-right">
                    <label class="control-label" for="picted">TED:</label>
                </div>
                <div class="col-lg-9">
                    <input type="checkbox" name="picted" id="picted" value="t" class="js-switch" <?php echo !empty($dadosPI['picted']) && 't' == $dadosPI['picted'] ? 'checked="checked"' : ''; ?> />
                </div>
            </div>

            <div class="form-group" id="div_botao_edital">
                <div class="col-md-3 text-right">
                    <label class="control-label" for="picedital">Edital:</label>
                </div>
                <div class="col-lg-9">
                    <input type="checkbox" name="picedital" id="picedital" value="t" class="js-switch element_edital" <?php echo !empty($dadosPI['picedital']) && 't' == $dadosPI['picedital'] ? 'checked="checked"' : ''; ?> />
                </div>
            </div>

            <div class="form-group"  id="div_edital">
                <div class="col-md-3 text-right">
                    <label class="control-label" for="mes">Prev. Lançamento Edital:</label>
                </div>
                <div class="col-lg-9">
                    <?php
                    $sql = "
                        SELECT
                            mescod as codigo,
                            mesdsc as descricao
                        FROM public.meses 
                        ORDER BY
                            codigo
                    ";
                    $mescod = $dadosPI['mescod'];
                    $db->monta_combo('mescod', $sql, 'S', 'Selecione', null, null, null, null, 'N', 'mes', null, '', null, 'class="form-control chosen-select" style="width=100%;"', null, (isset($mescod) ? $mescod: null));
                    ?>
                </div>
            </div>

        </div>
    </div>
</div>
<div class="col-md-6">                                    
    <div class="ibox float-e-margins div_etapas_pi">
        <div class="ibox-title">
            <h5>Etapas:</h5>
        </div>
        <div class="ibox-content">
            <input type="hidden" id="etaid" name="etaid" value="<?=$etaid?>">
            <?php
            echo $simec->input('etadsci', 'Descrição', $etadsc, ['required', 'maxlength' => 250]);
            echo $simec->data('etadatai', 'Data', $etadata, ['required']);
            ?>
            <div class="form-group">
                <div class="text-center">
                    <button class="btn btn-primary" type="button" id="btn-salvar-etapa"><i class="fa fa-check"></i>&nbsp;Adicionar
                    </button>
                </div>
            </div>
            <div id="div_listagem_etapas">
                <?php include_once APPRAIZ.'planacomorc/modulos/principal/unidade/lista_etapas.inc';?>
            </div>
        </div>
    </div>
</div>