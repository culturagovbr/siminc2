<h2 style="color: #1AB394">3. Remanejamento Loa
    <a href="#" id="btnRemanejamentoLoa" tittle="Editar">
        <span class="btn btn-primary btn-sm glyphicon glyphicon-pencil"></span>
    </a>
</h2>
<div class="tabs-container tabs">
    <ul class="nav nav-tabs prodTabs">
        <li class="active " style="list-style-type: none;">
            <a class="tab-tipo" data-toggle="tab" id="tp_interno" href="#tab-tabela-loa">Tabela</a>
        </li>
        <li class=" " style="list-style-type: none;">
            <a class="tab-tipo" data-toggle="tab" id="tp_externo" href="#tab-grafico-loa">Gráfico</a>
        </li>
    </ul>
    <div class="tab-content">
        <div id="tab-tabela-loa" class="tab-pane active">
            <table class="table table-striped table-bordered table-hover dataTables">
                <thead>
                    <tr class="text-center">
                        <th>Funcional</th>
                        <th>Categoria</th>
                        <th>GND</th>
                        <th width="11%">Modalidade</th>
                        <th width="11%">Fonte</th>
                        <th>Idoc</th>
                        <th>Iduso</th>
                        <th>RP</th>
                        <th>Dotação Inicial</th>
                        <th>Suplementar</th>
                        <th>Cancelado</th>
                        <th>Suplementar por Excesso</th>
                        <th>Suplementar por Superávit</th>
                    </tr>
                </thead>
                <tbody>
                <?php
                    $loas = (new Alteracao_Model_RemanejamentoLoa())->recuperarTodosPorPedido($_REQUEST['pedid']);
                    foreach($loas as $loa): ?>
                        <tr>
                            <td align="center"><?= $loa['funcional']; ?></td>
                            <td align="center"><?= $loa['ctecod']; ?></td>
                            <td align="center"><?= $loa['gndcod']; ?></td>
                            <td align="center"><?= $loa['mapcod']; ?></td>
                            <td align="center"><?= $loa['fonid']; ?></td>
                            <td align="center"><?= $loa['idoid']; ?></td>
                            <td align="center"><?= $loa['iduid']; ?></td>
                            <td align="center"><?= $loa['rpcod']; ?></td>
                            <td align="center"><?= $loa['vldotacao'] ? number_format($loa['vldotacao'], 0, '', '.') : '-'; ?></td>
                            <td align="center"><?= $loa['vlsuplementar'] ? number_format($loa['vlsuplementar'], 0, '', '.') : '-'; ?></td>
                            <td align="center"><?= $loa['vlcancelado'] ? number_format($loa['vlcancelado'], 0, '', '.') : '-'; ?></td>
                            <td align="center"><?= $loa['vlsuplementarexcesso'] ? number_format($loa['vlsuplementarexcesso'], 0, '', '.') : '-'; ?></td>
                            <td align="center"><?= $loa['vlsuplementarsuperavit'] ? number_format($loa['vlsuplementarsuperavit'], 0, '', '.') : '-'; ?></td>
                        </tr>
                    <?php endforeach ?>
                </tbody>
            </table>
        </div>

        <div id="tab-grafico-loa" class="tab-pane">

        </div>
    </div>
</div>
<script>
$(document).ready(function(){
    $("#btnRemanejamentoLoa").click(function(){
        $("#wizard-t-2").click();
    });
});
</script>