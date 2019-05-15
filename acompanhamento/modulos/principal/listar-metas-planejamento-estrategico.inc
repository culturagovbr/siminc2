<div class="ibox float-e-margins">
    <div class="ibox-title">
        <?php
//        $aMetasPpa = $mAcompanhamento->getMetasPpaComPi($suoid, $janid);
        ?>
        <h5 style="margin-right: 30px;">Minhas Metas do Planejamento Estratégico
            <!-- <span style="color: red;">(<?= count($aMetasEstrategica); ?>)</span>  -->
        </h5>
    </div>
    <div class="ibox-content">
        <?php
        if(count($aMetasEstrategica)){ ?>
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
                    <?php foreach($aMetasEstrategica as $metaEstrategica){
                        
                        if($metaEstrategica['acoid']){
                            $url = '?modulo=principal/monitorar-ppa&acao=A&acoid='. $metaEstrategica['acoid'];
                            $icone = '<span class="label label-primary">Preenchida</span>';
                        } else {
                            $url = '?modulo=principal/monitorar-ppa&acao=A&mppid='. $metaEstrategica['mppid']. '&suoid='. $metaEstrategica['suoid'];
                            $icone = '<span class="label label-danger">Pendente</span>';
                        }
                        $status = $metaEstrategica['acoid'] ?
                            '<a href="?modulo=principal/monitorar-ppa&acao=A&acoid=' . $metaEstrategica['acoid'] . '"><span class="label label-primary">Preenchida</span></a>' :
                            '<a href="?modulo=principal/monitorar-ppa&acao=A&mppid=' . $metaEstrategica['mppid'] . '&suoid=' . $metaEstrategica['suoid'].'"><span class="label label-danger">Pendente</span></a>';
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
                                echo $metaEstrategica['mppcod'] . " - " . $metaEstrategica['mppnome'];
                                if($metaEstrategica['compartilhada'] == 't'){
                                    echo ' <i style="font-size: 15px; color: #6892ff;" class="fa fa-handshake-o link" title="Meta Compartilhada com outras Subunidades."></i>';
                                }
                                ?>
                            </td>
                            <td class="text-right"><?php echo number_format($metaEstrategica['mppquantidade'], 2, ',', '.'); ?></td>
                            <td class="text-right"><?php echo number_format($metaEstrategica['acoquantidade'], 2, ',', '.'); ?></td>
                            <td class="text-center">
                                <a class="detalhe-pi" href="?modulo=principal/monitorar-ppa&acao=A&req=detalhe-pi&mppid=<?php echo $metaEstrategica['mppid']; ?>&suoid=<?php echo $metaEstrategica['suoid']; ?>"><?php echo $metaEstrategica['pis']; ?></a>
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
