<div class="ibox float-e-margins">
    <div class="ibox-title">
        <?php
        $janid = $janid ? $janid : $janelaAtiva['janid'];
        $aMetasPpa = $mAcompanhamento->getMetasPpaComPi($suoid, $janid);
        ?>
        <h5 style="margin-right: 30px;">Minhas Metas PPA <span style="color: red;">(<?php echo count($aMetasPpa); ?>)</span></h5>
        Janela:
        <select id="janid_ppa">
            <?php foreach($aJanelas as $janela){ ?>
                <option value="<?php echo $janela['janid']; ?>" <?php echo $janela['janid'] == $janid ? 'selected' : ''; ?>>
                    <?php echo "{$janela['jandsc']} - (" . Simec_Util::formatarData($janela['janinicio'], 'dd/MM/YYYYY') . " - " . Simec_Util::formatarData($janela['janfim'], 'dd/MM/YYYYY') . ")"; ?>
                </option>
            <?php } ?>
        </select>
    </div>
    <div class="ibox-content">
        <?php
        if(count($aMetasPpa)){ ?>
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
                    <?php foreach($aMetasPpa as $dados){
                        
                        if($dados['acoid']){
                            $url = '?modulo=principal/monitorar-ppa&acao=A&acoid='. $dados['acoid'];
                            $icone = '<span class="label label-primary">Preenchida</span>';
                        } else {
                            $url = '?modulo=principal/monitorar-ppa&acao=A&mppid='. $dados['mppid']. '&suoid='. $dados['suoid']. '&janid='. $janid;
                            $icone = '<span class="label label-danger">Pendente</span>';
                        }
                        $status = $dados['acoid'] ?
                            '<a href="?modulo=principal/monitorar-ppa&acao=A&acoid=' . $dados['acoid'] . '"><span class="label label-primary">Preenchida</span></a>' :
                            '<a href="?modulo=principal/monitorar-ppa&acao=A&mppid=' . $dados['mppid'] . '&suoid=' . $dados['suoid'] . '&janid=' . $janid . '"><span class="label label-danger">Pendente</span></a>';                        
                        # Caso seja perfil PNC não exibe o link de acesso a pagina de edição
                        if(in_array(PFL_PNC, $listaPerfis)){
                            $botao = $icone;
                        } else {
                            $botao = '<a href="'. $url. '">'. $icone. '</a>';
                        }
                    ?>
                        <tr>
                            <td class="text-center"><?php echo $botao; ?></td>
                            <td>
                                <?php
                                echo $dados['mppcod'] . " - " . $dados['mppnome'];
                                if($dados['compartilhada'] == 't'){
                                    echo ' <i style="font-size: 15px; color: #6892ff;" class="fa fa-handshake-o link" title="Meta Compartilhada com outras Subunidades."></i>';
                                }
                                ?>
                            </td>
                            <td class="text-right"><?php echo number_format($dados['mppquantidade'], 2, ',', '.'); ?></td>
                            <td class="text-right"><?php echo number_format($dados['acoquantidade'], 2, ',', '.'); ?></td>
                            <td class="text-center">
                                <a class="detalhe-pi" href="?modulo=principal/monitorar-ppa&acao=A&req=detalhe-pi&mppid=<?php echo $dados['mppid']; ?>&suoid=<?php echo $dados['suoid']; ?>"><?php echo $dados['pis']; ?></a>
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
        $('#janid_ppa').change(function(){
            $('#box-metas-ppa').load('?modulo=inicio&acao=C&req=listar-metas-ppa&janid=' + $(this).val() + '&suoid=' + $('#suoid').val());
        });
    })
</script>