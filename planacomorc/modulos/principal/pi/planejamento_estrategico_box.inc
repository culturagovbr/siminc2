<div class="div-grupo-estrategico">
    <div class="ibox-title">
        <h5>Planejamento Estratégico </h5>
    </div>
    <div class="ibox-content">
        <div class="div_meeid">
            <?= $simec->select('meuid', 'Meta Unidade', $dadosPI['meuid'], (new Planacomorc_Model_MetaUnidade())->recuperarSqlMeta($dadosPI['ungcod'])); ?>
        </div>
        <div id="div_objetivo_dimensao_meta">
            <?php carregarComboEstrategico($dadosPI['meuid']); ?>
        </div>
    </div>
</div>
