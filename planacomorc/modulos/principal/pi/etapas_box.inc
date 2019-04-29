<div class="div_etapas_pi">
    <div class="ibox-title">
        <h5>Etapas:</h5>
    </div>
    <div class="ibox-content">
        <input type="hidden" id="etaid" name="etaid" value="<?=$etaid?>">
        <?php
        echo $simec->input('etadsci', 'Descrição', $etadsc, ['maxlength' => 250]);
        echo $simec->data('etadatai', 'Data', $etadata);
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