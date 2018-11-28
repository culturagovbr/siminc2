<table id="tbListaLoa" class="table table-striped table-bordered table-hover" >
    <thead>
        <tr class="text-center">
            <th width="5% ">Ação</th>
            <th>Funcional</th>
            <th>Categoria</th>
            <th>GND</th>
            <th>Modalidade</th>
            <th>Fonte</th>
            <th>IDOC</th>
            <th>IDUSO</th>
            <th>RP</th>
            <th>Dotação Inicial</th>
            <th>Suplementar</th>
            <th>Cancelado</th>
            <th>Suplementar por excesso</th>
            <th>Suplementar por Superávit</th>
            <th></th>
        </tr>
    </thead>
    <tbody>

    </tbody>
</table>
<script>
    $(function () {

        $('#btnloa').click(function () {
            // recuperaFuncionalSelecionada
            var tr = '<tr class="text-center">' +
                '        <th width="5% ">Ação</th>\n' +
                '        <th><?php $options = (new Alteracao_Model_RemanejamentoLoa())->recuperaFuncionalSelecionada($_REQUEST['pedid']);
                                montaListaLoa('ptrid',$options);
                             ?>'+
                         '</th>\n' +
                '        <th><?php $options = (new Alteracao_Model_RemanejamentoLoa())->montaComboCategoria();
                               montaListaLoa('ctecod', $options);
                           ?>'+
                        '</th>\n' +
                '        <th><?php $options = (new Alteracao_Model_RemanejamentoLoa())->montaComboGND();
                               montaListaLoa('gndcod', $options);
                           ?>'+
                        '</th>\n' +
                '        <th><?php $options = (new Alteracao_Model_RemanejamentoLoa())->montaComboModalidade();
                               montaListaLoa('mapcod', $options);
                           ?>'+
                        '</th>\n' +
                '        <th><?php $options = (new Alteracao_Model_RemanejamentoLoa())->montaComboFonte();
                               montaListaLoa('foncod', $options);
                           ?>'+
                        '</th>\n' +
                '        <th><?php $options = (new Alteracao_Model_RemanejamentoLoa())->montaComboIdoc();
                               montaListaLoa('idoid', $options);
                           ?>'+
                        '</th>\n' +
                '        <th><?php $options = (new Alteracao_Model_RemanejamentoLoa())->montaComboIduso();
                               montaListaLoa('iducod', $options);
                           ?>'+
                        '</th>\n' +
                '        <th><?php $options = (new Alteracao_Model_RemanejamentoLoa())->montaComboRP();
                               montaListaLoa('rpcod', $options);
                           ?>'+
                        '</th>\n' +
                '        <th>Dotação Inicial</th>\n' +
                '        <th>Suplementar</th>\n' +
                '        <th>Cancelado</th>\n' +
                '        <th>Suplementar por excesso</th>\n' +
                '        <th>Suplementar por Superávit</th>\n' +
                '        <th></th>' +
                '     </tr>';
            var tbody = $("#tbListaLoa > tbody");
            tbody.append(tr);
        })


    })
</script>