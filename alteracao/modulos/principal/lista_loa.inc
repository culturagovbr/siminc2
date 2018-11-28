<?php
$linha = '<table id="tbListaLoa" class="table table-striped table-bordered table-hover" >
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
        </tr>';

            $loas = (new Alteracao_Model_RemanejamentoLoa())->recuperarTodosPorPedido($_REQUEST['pedid']);
            foreach($loas as $loa){
                $linha .= '<tr class="text-center">
                              <td width="5% "><span class="glyphicon glyphicon-trash btn btn-danger btnRemoverLoa" id="btnRemoverLoa" rlid="'.$loa['rlid'].'"></span></td>
                              <td>';
                        $options = (new Alteracao_Model_RemanejamentoLoa())->recuperaFuncionalSelecionada($_REQUEST['pedid']);
                $linha .= montaListaLoa('pliselid',$options, $loa['pliselid'], $loa['rlid']);
                $linha .= '</td>';
                $linha .= '<td>';
                        $options = (new Alteracao_Model_RemanejamentoLoa())->montaComboCategoria();
                $linha .= montaListaLoa('ctecod', $options, $loa['ctecod'], $loa['rlid']);
                $linha .= '</td>';
                $linha .= '<td>';
                        $options = (new Alteracao_Model_RemanejamentoLoa())->montaComboGND();
                $linha .= montaListaLoa('gndcod', $options, $loa['gndcod'], $loa['rlid']);
                $linha .= '</td>';
                $linha .= '<td>';
                        $options = (new Alteracao_Model_RemanejamentoLoa())->montaComboModalidade();
                $linha .= montaListaLoa('mapcod', $options, $loa['mapcod'], $loa['rlid']);
                $linha .= '</td>';
                $linha .= '<td>';
                        $options = (new Alteracao_Model_RemanejamentoLoa())->montaComboFonte();
                $linha .= montaListaLoa('fonid', $options, $loa['fonid'], $loa['rlid']);
                $linha .= '</td>';
                $linha .= '<td>';
                        $options = (new Alteracao_Model_RemanejamentoLoa())->montaComboIdoc();
                $linha .= montaListaLoa('idoid', $options, $loa['idoid'], $loa['rlid']);
                $linha .= '</td>';
                $linha .= '<td>';
                        $options = (new Alteracao_Model_RemanejamentoLoa())->montaComboIduso();
                $linha .= montaListaLoa('iduid', $options, $loa['iduid'], $loa['rlid']);
                $linha .= '</td>';
                $linha .= '<td>';
                        $options = (new Alteracao_Model_RemanejamentoLoa())->montaComboRP();
                $linha .= montaListaLoa('rpcod', $options, $loa['rpcod'], $loa['rlid']);
                $linha .= '</td>';
                $linha .= '        <td><input type="text" id="vlDotacao'.$loa['rlid'].'" name="vlDotacao" rlid="'.$loa['rlid'].'" value="'.$loa['vldotacao'].'" size="15" maxlength="15" class="input_valor_remanejamento"></td>';
                $linha .= '        <td><input type="text" id="vlSuplementar'.$loa['rlid'].'" name="vlSuplementar" rlid="'.$loa['rlid'].'" value="'.$loa['vlsuplementar'].'" size="15" maxlength="15" class="input_valor_remanejamento"></td>';
                $linha .= '        <td><input type="text" id="vlCancelado'.$loa['rlid'].'" name="vlCancelado" rlid="'.$loa['rlid'].'" value="'.$loa['vlcancelado'].'" size="15" maxlength="15" class="input_valor_remanejamento"></td>';
                $linha .= '        <td><input type="text" id="vlSuplementarExcesso'.$loa['rlid'].'" name="vlSuplementarExcesso" rlid="'.$loa['rlid'].'" value="'.$loa['vlsuplementarexcesso'].'" size="15" maxlength="15" class="input_valor_remanejamento"></td>';
                $linha .= '        <td><input type="text" id="vlSuplementarSuperavit'.$loa['rlid'].'" name="vlSuplementarSuperavit" rlid="'.$loa['rlid'].'" value="'.$loa['vlsuplementarsuperavit'].'" size="15" maxlength="15" class="input_valor_remanejamento"></td>';
                $linha .= '        <td><span class="glyphicon glyphicon-check btn btn-primary btnSalvarLoa" id="btnSalvarLoa" rlid="'.$loa['rlid'].'" ></span></td>';
                $linha .= '     </tr>';
            }
$linha .= '</table>';
echo $linha;
        ?>
<script>
    $(function () {

        $('#btnloa').click(function () {
            // recuperaFuncionalSelecionada
            var tr = '<tr class="text-center">' +
                '        <th width="5% "><span class="glyphicon glyphicon-trash btn btn-danger btnRemoverLoa" id="btnRemoverLoa" rlid=""></span></th>' +
                '        <th><?php $options = (new Alteracao_Model_RemanejamentoLoa())->recuperaFuncionalSelecionada($_REQUEST['pedid']);
                                echo montaListaLoa('pliselid',$options);
                             ?>'+
                         '</th>' +
                '        <th><?php $options = (new Alteracao_Model_RemanejamentoLoa())->montaComboCategoria();
                               echo montaListaLoa('ctecod', $options);
                           ?>'+
                        '</th>' +
                '        <th><?php $options = (new Alteracao_Model_RemanejamentoLoa())->montaComboGND();
                               echo montaListaLoa('gndcod', $options);
                           ?>'+
                        '</th>' +
                '        <th><?php $options = (new Alteracao_Model_RemanejamentoLoa())->montaComboModalidade();
                               echo montaListaLoa('mapcod', $options);
                           ?>'+
                        '</th>' +
                '        <th><?php $options = (new Alteracao_Model_RemanejamentoLoa())->montaComboFonte();
                               echo montaListaLoa('fonid', $options);
                           ?>'+
                        '</th>' +
                '        <th><?php $options = (new Alteracao_Model_RemanejamentoLoa())->montaComboIdoc();
                               echo montaListaLoa('idoid', $options);
                           ?>'+
                        '</th>' +
                '        <th><?php $options = (new Alteracao_Model_RemanejamentoLoa())->montaComboIduso();
                               echo montaListaLoa('iduid', $options);
                           ?>'+
                        '</th>' +
                '        <th><?php $options = (new Alteracao_Model_RemanejamentoLoa())->montaComboRP();
                               echo montaListaLoa('rpcod', $options);
                           ?>'+
                        '</th>' +
                '        <th><input type="text" id="vlDotacao" name="vlDotacao" rlid="" size="15" maxlength="15" class="input_valor_remanejamento"></th>' +
                '        <th><input type="text" id="vlSuplementar" name="vlSuplementar" rlid="" size="15" maxlength="15" class="input_valor_remanejamento"></th>' +
                '        <th><input type="text" id="vlCancelado" name="vlCancelado" rlid="" size="15" maxlength="15" class="input_valor_remanejamento"></th>' +
                '        <th><input type="text" id="vlSuplementarExcesso" name="vlSuplementarExcesso" rlid="" size="15" maxlength="15" class="input_valor_remanejamento"></th>' +
                '        <th><input type="text" id="vlSuplementarSuperavit" name="vlSuplementarSuperavit" rlid="" size="15" maxlength="15" class="input_valor_remanejamento"></th>' +
                '        <th><span class="glyphicon glyphicon-check btn btn-primary btnSalvarLoa" id="btnSalvarLoa" rlid=""></span></th>' +
                '     </tr>';
            var tbody = $("#tbListaLoa > tbody");
            tbody.append(tr);
            $('.input_valor_remanejamento').keyup(function(){
                $(this).val(mascaraglobal('###.###.###.###,##', $(this).val()));
            }).keyup();   
            $(".btnSalvarLoa").click(function(){
                var rlid = $(this).attr('rlid');
                SalvarLoa(rlid);
            });
        });
        $('.input_valor_remanejamento').keyup(function(){
            $(this).val(mascaraglobal('###.###.###.###,##', $(this).val()));
        }).keyup();
        
        $(".btnSalvarLoa").click(function(){
            var rlid = $(this).attr('rlid');
            SalvarLoa(rlid);
        });
        $(".btnRemoverLoa").click(function(){
            var rlid = $(this).attr('rlid');
            RemoverLoa(rlid);
        });
    });
    
    function SalvarLoa(rlid){
        var ptrid = $("#pliselid"+rlid).val();
        var vlDotacao = acertaNumero($("#vlDotacao"+rlid).val());
        var vlSuplementar = acertaNumero($("#vlSuplementar"+rlid).val());
        var vlCancelado = acertaNumero($("#vlCancelado"+rlid).val());
        var vlSuplementarExcesso = acertaNumero($("#vlSuplementarExcesso"+rlid).val());
        var vlSuplementarSuperavit = acertaNumero($("#vlSuplementarSuperavit"+rlid).val());
        $.post('?modulo=principal/cadastro_alteracoes&acao=C&req=salvar_loa',
        {
            rlid: rlid,
            pliselid: $("#pliselid"+rlid).val(),
            ctecod: $("#ctecod"+rlid).val(),
            gndcod: $("#gndcod"+rlid).val(),
            mapcod: $("#mapcod"+rlid).val(),
            fonid: $("#fonid"+rlid).val(),
            idoid: $("#idoid"+rlid).val(),
            iduid: $("#iduid"+rlid).val(),
            rpcod: $("#rpcod"+rlid).val(),
            vlDotacao: vlDotacao,
            vlSuplementar: vlSuplementar,
            vlCancelado: vlCancelado,
            vlSuplementarExcesso: vlSuplementarExcesso,
            vlSuplementarSuperavit: vlSuplementarSuperavit
        },
            function(retorno){
                if (retorno){
                    $(".div_remanejamento_loa").load('?modulo=principal/cadastro_alteracoes&acao=C&req=carrega_remanejamento_loa&pedid='+$("#pedid").val()+'&tpdid='+$("#tpdid").val());
                }
            }
        );        
    }
    
    function RemoverLoa(rlid){
        $.post('?modulo=principal/cadastro_alteracoes&acao=C&req=remover_loa',
        {
            rlid: rlid
        },
            function(retorno){
                if (retorno){
                    $(".div_remanejamento_loa").load('?modulo=principal/cadastro_alteracoes&acao=C&req=carrega_remanejamento_loa&pedid='+$("#pedid").val()+'&tpdid='+$("#tpdid").val());
                }
            }
        );        
    }
    
    function acertaNumero(num){
        num = str_replace(['.', ','], ['', '.'],num);
        return num;
    }
</script>