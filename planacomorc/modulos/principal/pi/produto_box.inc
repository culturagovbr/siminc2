<div class="div_produto_pi">
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
