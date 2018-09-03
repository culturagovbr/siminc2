<?php
    $acpid = $_REQUEST['acpid'];
    $acompanhamentopnc = new Acompanhamento_Model_Acompanhamentopnc($acpid);
    $listaAtividades = $acompanhamentopnc->buscarAtividades();
    
    # Verifica se existe pelo menos uma atividade inserida no acompanhamento da Meta PNC
    $atividadePreenchida = $acompanhamentopnc->verificarPreenchimentoAtividade();

?>
<table class="atividades_table table table-bordered">
    <thead>
        <tr>
            <th class="tabela__header--acao">Ação</th>
            <th>Atividade realizadas em <?php echo (int)$_SESSION['exercicio']; ?></th>
            <th>Quantidade</th>
            <th>Produto ou serviço resultante</th>
            <th>Localização que a atividade foi realizada</th>
            <th>Fonte de verificação da informação</th>
        </tr>
    </thead>
    <?php if($listaAtividades): ?>
    <tbody>
        <?php foreach($listaAtividades as $atividade): ?>
            <tr>
                <td>
                    <span class=" editar_atividade btn btn-primary btn-sm glyphicon glyphicon-pencil" data-atvid="<?php echo $atividade['atvid']; ?>" data-acpid="<?php echo $acpid; ?>" title="Editar Atividade"></span>
                    <span style="color: red;">
                        <span class="excluir_atividade btn btn-danger btn-sm glyphicon glyphicon-trash" data-acpid="<?php echo $acpid; ?>" data-atvid="<?php echo $atividade['atvid']; ?>" data-atvtitulo="<?php echo $atividade['atvtitulo']; ?>" title="Excluir Atividade: <?php echo $atividade['atvtitulo']; ?>" /></span>
                    </span>
                </td>
                <td><?php echo $atividade['atvtitulo']; ?></td>
                <td><?= number_format($atividade['atvprodutoqtd'], 2, ',', '.'); ?></td>
                <td><?php echo $atividade['atvproduto']; ?></td>
                <td><?php echo $atividade['mundescricao']. '-'. $atividade['estuf']; ?></td>
                <td>
                    <?php if(Acompanhamento_Controller_Acompanhamentopnc::verificarPermissaoEditar($acpid)): ?>
                        <?= $atividade['atvfonteinformacao']; ?>
                        &nbsp;
                    <?php else: ?>
                        <?= $atividade['atvfonteinformacao']; ?>
                    <?php endif; ?>
                </td>
            </tr>
        <?php endforeach; ?>
    </tbody>
   
    <?php else: ?>
        <th>
            <tr>
                <td colspan="5">Não existem atividades cadastradas pra essa Meta PNC.</td>
            </tr>
        </th>
    <?php endif; ?>
</table>
 <div>
        <?php if(Acompanhamento_Controller_Acompanhamentopnc::verificarPermissaoEditar($acpid)): ?>
            <ul style="text-align: right;">
                <button class="btn btn-success editar_atividade" data-atvid="" data-acpid="<?php echo $acpid; ?>" type="button">
                    <i class="fa fa-plus-circle"></i>&nbsp;Inserir nova atividade
                </button>
            </ul>
        <?php endif; ?>
</div>

<?php echo '<input id="hid_atividade_preenchida" type="hidden" value="'.((int)$atividadePreenchida). '" />'; ?>
