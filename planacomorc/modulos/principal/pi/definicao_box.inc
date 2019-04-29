<div class="ibox-title">
    <h5>Descrição</h5>
</div>
<div class="ibox-content">
    <?php
        echo $simec->input('plititulo', 'Título', $dadosPI['plititulo'], ['maxlength' => 250, 'required']);
        echo $simec->textarea('plidsc', 'Descrição / Finalidade', $dadosPI['plidsc'], ['rows' => 6, 'cols' => 60, 'required', 'maxlength' => 1000]);
        echo $simec->textarea('plirecursosnecessarios', 'Recursos Necessários', $dadosPI['plirecursosnecessarios'], ['rows' => 6, 'cols' => 60, 'maxlength' => 1000]);
    ?>
</div>
