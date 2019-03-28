<div class="col-md-6 div-grupo-estrategico">
    <div class="ibox float-e-margins">
        <div class="ibox-title">
            <h5>Planejamento Estratégico </h5>
        </div>
        <div class="ibox-content">
            <?php echo $simec->select('obeid', 'Objetivo Estratégico', $dadosPI['obeid'], (new Planacomorc_Model_ObjetivoEstrategico())->recuperarSqlCombo(['obecod', 'obenome'], null, 'obecod')); ?>
            <div class="div_meeid">
                <?php echo $simec->select('meeid', 'Meta Estratégica', $dadosPI['meeid'], (new Planacomorc_Model_MetaEstrategica())->recuperarSqlCombo(['meecod', 'meenome'], null, 'meecod')); ?>
            </div>
            <div class="div_dieid">
                <?php echo $simec->select('dieid', 'Diretriz Estratégica', $dadosPI['dieid'], (new Planacomorc_Model_DiretrizEstrategica())->recuperarSqlCombo(['diecod', 'dienome'], null, 'diecod')); ?>
            </div>
        </div>
    </div>
</div>
<div class="col-md-6">
    <div class="ibox float-e-margins div_metas_ppa_pnc">
        <div class="ibox-title">
            <h5>Metas </h5>
        </div>
        <div class="ibox-content">
            <div class="form-group">
                <div class="col-md-3 text-right div-objetivo">
                    <label for="oppid" class=" control-label">Objetivo PPA: </label>
                </div>
                <div class="col-lg-9">
                    <?php (new Public_Model_ObjetivoPpa())->monta_combo($dadosPI['oppid']); ?>
                </div>
            </div>
            <div class="form-group">
                <div class="col-md-3 text-right div-metas">
                    <label for="mppid" class=" control-label">Metas PPA: </label>
                </div>
                <div class="col-lg-9 div_mppid">
                    <?php carregarMetasPPA($dadosPI['oppid'], $dadosPI['mppid']); ?>
                </div>
            </div>
            <div class="form-group">
                <div class="col-md-3 text-right div-iniciativa">
                    <label for="ippid" class="control-label">Iniciativa PPA: </label>
                </div>
                <div class="col-lg-9 div_ippid">
                    <?php carregarIniciativaPPA($dadosPI['oppid'], $dadosPI['ippid']); ?>
                </div>
            </div>
            <div class="form-group grupo_pnc">
                <div class="col-md-3 text-right">
                    <label for="mpnid" class="control-label">Meta PNC: <span title="Campo obrigatório" class="link campo-obrigatorio">*</span></label>
                </div>
                <div class="col-lg-9 div_mpnid">
                    <?php carregarMetaPNC($dadosPI['ungcod'], $dadosPI['mpnid']); ?>
                </div>
            </div>
            <div class="form-group grupo_pnc">
                <div class="col-md-3 text-right">
                    <label for="ipnid" class="control-label">Indicador PNC: <span title="Campo obrigatório" class="link campo-obrigatorio">*</span></label>
                </div>
                <div class="col-lg-9 div_ipnid">
                    <?php carregarIndicadorPNC($dadosPI['mpnid'], $dadosPI['ipnid']); ?>
                </div>
            </div>
        </div>
    </div>
</div>
<div class="col-md-6">
    <div class="ibox float-e-margins div_produto_pi">
        <div class="ibox-title">
            <h5>Produto PA:</h5>
        </div>
        <div class="ibox-content">
            <div class="form-group">
                <div class="col-md-3 text-right">
                    <label for="pprid" class="control-label">Produto: <span title="Campo obrigatório" class="link campo-obrigatorio">*</span></label>
                </div>
                <div class="col-lg-9">
                    <?php
                        $sql = "
                            SELECT
                                pprid AS codigo,
                                pprnome AS descricao
                            FROM monitora.pi_produto
                            WHERE
                                prsano = '{$_SESSION['exercicio']}'
                                AND pprstatus = 'A'
                            ORDER BY
                                descricao
                        ";

                        $pprid = $dadosPI['pprid'];
                        $db->monta_combo('pprid', $sql, 'S', 'Selecione', '', null, null, null, 'N', 'pprid', null, '', null, 'class="form-control chosen-select" style="width=100%;"', null, (isset($pprid)? $pprid: null));
                    ?>
                </div>
            </div>
            <div class="form-group div_unidade_medida">
                <div class="col-md-3 text-right">
                    <label for="pumid" class="control-label">Unidade de Medida: <span title="Campo obrigatório" class="link campo-obrigatorio">*</span></label>
                </div>
                <div class="col-lg-9">
                    <?php
                        $sql = "
                            SELECT
                                pumid AS codigo,
                                pumdescricao AS descricao
                            FROM monitora.pi_unidade_medida
                            WHERE
                                prsano = '{$_SESSION['exercicio']}'
                                AND pumstatus = 'A'
                            ORDER BY
                                descricao
                        ";

                        $pumid = !empty($dadosPI['pumid'])? $dadosPI['pumid']: '9';
                        $db->monta_combo('pumid', $sql, 'S', 'Selecione', '', null, null, null, 'N', 'pumid', null, '', null, 'class="form-control chosen-select" style="width=100%;"', null, (isset($pumid) ? $pumid: null));
                    ?>
                </div>
            </div>
            <div class="form-group div_quantidade_produto">
                <div class="col-md-3 text-right">
                    <label for="picquantidade" class="control-label">Quantidade: </label>
                </div>
                <div class="col-lg-9">
                    <input type="text" value="<?php echo !empty($dadosPI['picquantidade']) ? $dadosPI['picquantidade'] : ''; ?>" class="CampoEstilo normal inteiro form-control" title="Quantidade" id="picquantidade" name="picquantidade" maxlength="50" />
                </div>
            </div>
        </div>
    </div>
</div>