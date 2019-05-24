<div class="ibox float-e-margins">
    <div class="ibox-title">
        <?php
        $janid = $janid ? $janid : $janelaAtiva['janid'];
        $dto = ['suoid' => $suoid, 'janid' => $janid ];
        $aMetasPlanejamentoEstrategico = $mAcompanhamento->recuperarMetasPlanejamentoComPI((object)$dto);
        ?>
        <h5 style="margin-right: 30px;">Minhas Metas do Planejamento Estratégico <span style="color: red;">(<?= count($aMetasPlanejamentoEstrategico); ?>)</span></h5>
        Janela:
        <select id="janid_planejamento_estrategico">
            <?php foreach($aJanelas as $janela){ ?>
                <option value="<?= $janela['janid']; ?>" <?= $janela['janid'] == $janid ? 'selected' : ''; ?>>
                    <?= "{$janela['jandsc']} - (" . Simec_Util::formatarData($janela['janinicio'], 'dd/MM/YYYYY') . " - " . Simec_Util::formatarData($janela['janfim'], 'dd/MM/YYYYY') . ")"; ?>
                </option>
            <?php } ?>
        </select>
    </div>
    <div class="ibox-content">
        <?php
        if(count($aMetasPlanejamentoEstrategico)){ ?>
            <div class="table-responsive">
                <table class="table table-striped table-bordered table-hover table-condensed" >
                    <thead>
                        <tr class="text-center">
                            <th>Status</th>
                            <th>Meta</th>
                            <th>Qtd. Prevista</th>
                            <th>Qtd. Alcançada</th>
                            <th>PAs</th>
                        </tr>
                    </thead>
                    <tbody>
                    <?php foreach ($aMetasPlanejamentoEstrategico as $metaEstrategica) {

                        if ($metaEstrategica['acoid']) {
                            $url = '?modulo=principal/monitorar-planejamento-estrategico&acao=A&acoid='. $metaEstrategica['acoid'];
                            $icone = '<span class="label label-primary">Preenchida</span>';
                        } else {
                            $url = '?modulo=principal/monitorar-planejamento-estrategico&acao=A&meuid='. $metaEstrategica['meuid']. '&suoid='. $metaEstrategica['suoid'];
                            $icone = '<span class="label label-danger">Pendente</span>';
                        }
                        $status = $metaEstrategica['acoid'] ?
                            '<a href="?modulo=principal/monitorar-planejamento-estrategico&acao=A&acoid=' . $metaEstrategica['acoid'] . '"><span class="label label-primary">Preenchida</span></a>' :
                            '<a href="?modulo=principal/monitorar-planejamento-estrategico&acao=A&meuid=' . $metaEstrategica['meuid'] . '&suoid=' . $metaEstrategica['suoid'].'"><span class="label label-danger">Pendente</span></a>';
                        # Caso seja perfil PNC não exibe o link de acesso a pagina de edição
                        if (in_array(PFL_PNC, $listaPerfis)) {
                            $botao = $icone;
                        } else {
                            $botao = '<a href="'. $url. '">'. $icone. '</a>';
                        }
                    ?>
                        <tr>
                            <td class="text-center"><?= $botao; ?></td>
                            <td>
                                <?= $metaEstrategica['meunome'];
                                if ($metaEstrategica['compartilhada'] == 't') {
                                    echo ' <i style="font-size: 15px; color: #6892ff;" class="fa fa-handshake-o link" title="Meta Compartilhada com outras Subunidades."></i>';
                                }
                                ?>
                            </td>
                            <td class="text-right"><?= number_format($metaEstrategica['meuquantidade'], 2, ',', '.'); ?></td>
                            <td class="text-right"><?= number_format($metaEstrategica['acoquantidade'], 2, ',', '.'); ?></td>
                            <td class="text-center">
                                <a class="detalhe-pi" href="?modulo=principal/monitorar-planejamento-estrategico&acao=A&req=detalhe-pi&meuid=<?= $metaEstrategica['meuid']; ?>&suoid=<?= $metaEstrategica['suoid']; ?>"><?= $metaEstrategica['pis']; ?></a>
                            </td>
                        </tr>
                    <?php } ?>
                    </tbody>
                </table>
            </div>
        <?php } else {
            echo '<div class="alert alert-danger">Nenhum registro cadastrado</div>';
        } ?>
    </div>
</div>

<script>
    $(function(){
        $('#janid_planejamento_estrategico').change(function(){
            $('#box_planejamento_estrategico').load('?modulo=inicio&acao=C&req=listar-metas-estrategicas&janid=' + $(this).val() + '&suoid=' + $('#suoid').val());
        });
    })
</script>
