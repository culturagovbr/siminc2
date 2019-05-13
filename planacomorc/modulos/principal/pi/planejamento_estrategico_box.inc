<div class="div-grupo-estrategico">
    <div class="ibox-title">
        <h5>Planejamento Estratégico </h5>
    </div>
    <div class="ibox-content">
        <div class="div_meeid">
            <?= $simec->select('meuid', 'Meta Unidade', $dadosPI['meuid'], (new Planacomorc_Model_MetaUnidade())->recuperarSqlCombo('meunome', null, 'meunome')); ?>
        </div>
            <?= $simec->select('obeid', 'Objetivo', $dadosPI['obeid'], (new Planacomorc_Model_ObjetivoEstrategico())->recuperarSqlCombo('obenome')); ?>
        <div class="div_dieid">
            <?= $simec->select('dieid', 'Dimensão Estratégica', $dadosPI['dieid'], (new Planacomorc_Model_DimensaoEstrategica())->recuperarSqlCombo('dimenome', null, 'dimenome')); ?>
        </div>
            <?= $simec->select('meeid', 'Meta Estratégica', $dadosPI['meeid'], (new Planacomorc_Model_MetaEstrategica())->recuperarSqlCombo('meenome', null, 'meenome')); ?>
    </div>
</div>
