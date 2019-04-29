<div class="div_metas_ppa_pnc">
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
