<?php
    $suoid = $suoid? $suoid: $_REQUEST['suoid'];
    $janid = $janid? $janid: $_REQUEST['janid'];

    $mJanela = new Acompanhamento_Model_Janela();
    $janelaAtiva = $mJanela->recuperarJanelaAtiva(Acompanhamento_Model_Tipo::K_TIPO_INDICADOR_PNC);
    $aJanelas = $mJanela->recuperarTodos('*', [
        "prsano = '{$_SESSION['exercicio']}'",
        "janstatus = 'A'",
        "tipid = " . Acompanhamento_Model_Tipo::K_TIPO_INDICADOR_PNC], 'janinicio DESC');

?>
<div class="ibox float-e-margins">
    <div class="ibox-title">
        <?php
            $janid = $janid ? $janid : $janelaAtiva['janid'];
            $aMetasPnc = Acompanhamento_Model_Acompanhamentopnc::getMetasPnc((object)array(
                'exercicio' => $_SESSION['exercicio'],
                'suoid' => $suoid,
                'janid' => $janid));
        ?>
        <h5 style="margin-right: 30px;">Minhas Metas PNC <span style="color: red;">(<?php echo count($aMetasPnc); ?>)</span></h5>
        Janela:
        <select id="janid_pnc">
            <?php foreach($aJanelas as $janela){ ?>
                <option value="<?php echo $janela['janid']; ?>" <?php echo $janela['janid'] == $janid ? 'selected' : ''; ?>>
                    <?php echo "{$janela['jandsc']} - (" . Simec_Util::formatarData($janela['janinicio'], 'dd/MM/YYYYY') . " - " . Simec_Util::formatarData($janela['janfim'], 'dd/MM/YYYYY') . ")"; ?>
                </option>
            <?php } ?>
        </select>
    </div>

    <div class="ibox-content">
        <?php if(count($aMetasPnc)){ ?>
            <div class="table-responsive">
                <table class="table table-striped table-bordered table-hover table-condensed" >
                    <thead>
                    <tr class="text-center">
                        <th width="5%">Status</th>
                        <th>Meta</th>
                        <th width="5%">PIs</th>
                    </tr>
                    </thead>
                    <tbody>
                    <?php foreach($aMetasPnc as $dados){
                        $status = $dados['acompanhamento']?
                            '<a href="?modulo=principal/monitorar-pnc&acao=A&mpnid=' . $dados['mpnid'] . '&suoid=' . $suoid . '&janid=' . $janid . '"><span class="label label-primary">Preenchida</span></a>':
                            '<a href="?modulo=principal/monitorar-pnc&acao=A&mpnid=' . $dados['mpnid'] . '&suoid=' . $suoid . '&janid=' . $janid . '"><span class="label label-danger">Pendente</span></a>';
                        ?>

                        <tr>
                            <td class="text-center"><?php echo $status; ?></td>
                            <td><?php echo $dados['mpncod']. " - ". $dados['mpndsc']; ?></td>
                            <td class="text-center">
                                <a class="detalhe-pi" href="?modulo=principal/monitorar-pnc&acao=A&req=detalhe-pi&mpnid=<?php echo $dados['mpnid']; ?>&suoid=<?php echo $dados['suoid']; ?>"><?php echo $dados['pis']; ?></a>
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
        $('#janid_pnc').change(function(){
            $('#box-metas-pnc').load('?modulo=inicio&acao=C&req=listar-metas-pnc&janid=' + $(this).val() + '&suoid=' + $('#suoid').val());
        });
    })
</script>
