<div class="div-grupo-estrategico">
    <div class="ibox-title">
        <h5>Planejamento Estratégico </h5>
    </div>
    <div class="ibox-content">
        <div class="div_meeid">
            <?= $simec->select('meeid', 'Meta Unidade', $dadosPI['meeid'], (new Planacomorc_Model_MetaEstrategica())->recuperarSqlCombo('meenome', null, 'meenome')); ?>
        </div>
        <?= $simec->select('obeid', 'Objetivo Estratégico', $dadosPI['obeid'], (new Planacomorc_Model_ObjetivoEstrategico())->recuperarSqlCombo('obenome')); ?>
        <div class="div_dieid">
            <?= $simec->select('dieid', 'Dimensão Estratégica', $dadosPI['dieid'], (new Planacomorc_Model_DimensaoEstrategica())->recuperarSqlCombo('dimenome', null, 'dimenome')); ?>
        </div>
    </div>
</div>
