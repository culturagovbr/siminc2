<h2 style="color: #1AB394">4. Natureza da Receita
    <a href="javascript:window.location.href='?modulo=principal/cadastro_alteracoes&acao=C&pedid=<?=$_REQUEST['pedid']?>&tpdid=<?=$_REQUEST['tpdid']?>'" title="Editar">
        <span class="btn btn-primary btn-sm glyphicon glyphicon-pencil"></span>
    </a>
</h2>
<div class="tabs-container tabs">
    <ul class="nav nav-tabs prodTabs">
        <li class="active " style="list-style-type: none;">
            <a class="tab-tipo" data-toggle="tab" id="tp_interno" href="#tab-tabela-natureza">Tabela</a>
        </li>
        <li class=" " style="list-style-type: none;">
            <a class="tab-tipo" data-toggle="tab" id="tp_externo" href="#tab-grafico-natureza">Gráfico</a>
        </li>
    </ul>
    <div class="tab-content">
        <div id="tab-tabela-natureza" class="tab-pane active">
            <table class="table table-striped table-bordered table-hover dataTables">
                <thead>
                    <tr class="text-center">
                        <th>Natureza da Receita</th>
                        <th>Fonte</th>
                        <th>Dotação Inicial</th>
                        <th>Outros Acréscimos</th>
                        <th>Outras Reduções</th>
                        <th>Acréscimo Redução</th>
                        <th>Saldo</th>
                    </tr>
                </thead>
                <tbody>
                        <tr>
                            <td>''</td>
                            <td>''</td>
                            <td>''</td>
                            <td>''</td>
                            <td>''</td>
                            <td>''</td>
                            <td>''</td>
                        </tr>
                </tbody>
            </table>
        </div>

        <div id="tab-grafico-natureza" class="tab-pane">

        </div>
    </div>
</div>