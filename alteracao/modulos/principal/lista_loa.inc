<?php
$req = '<span style="color: #ff0000">*</span>';
$linha = '<table id="tbListaLoa" class="table table-striped table-bordered table-hover" >
        <tr class="text-center">
            <th width="5%">Ação</th>
            <th>Funcional'.$req.'</th>
            <th>Categoria'.$req.'</th>
            <th>GND'.$req.'</th>
            <th>Modalidade'.$req.'</th>
            <th>Fonte'.$req.'</th>
            <th>IDOC'.$req.'</th>
            <th>IDUSO'.$req.'</th>
            <th>RP'.$req.'</th>
            <th>Dotação Inicial</th>
            <th>Suplementar</th>
            <th>Cancelado</th>
            <th>Suplementar por excesso</th>
            <th>Suplementar por Superávit</th>
        </tr>';

            $loas = (new Alteracao_Model_RemanejamentoLoa())->recuperarTodosPorPedido($_REQUEST['pedid']);
            foreach($loas as $loa){
                $linha .= '<tr class="text-center">
                              <td width="5% ">
                                  <a href="#" class="btnRemoverLoa" id="btnRemoverLoa" rlid="'.$loa['rlid'].'"><i style="font-weight: 20px" class="fa fa-close"></i></a>&nbsp;&nbsp;&nbsp;
                                  <a href="#" class="btnSalvarLoa" id="btnSalvarLoa" rlid="'.$loa['rlid'].'"><i class="fa fa-check"></i></a>
                              </td>';
                $linha .= '<td class="combo">';
                        $options = (new Alteracao_Model_RemanejamentoLoa())->recuperaFuncionalSelecionada($_REQUEST['pedid']);
                $linha .= montaListaLoa('pliselid',$options, $loa['pliselid'], $loa['rlid']);
                $linha .= '</td>';
                $linha .= '<td class="combo">';
                        $options = (new Alteracao_Model_RemanejamentoLoa())->montaComboCategoria();
                $linha .= montaListaLoa('ctecod', $options, $loa['ctecod'], $loa['rlid']);
                $linha .= '</td>';
                $linha .= '<td class="combo">';
                        $options = (new Alteracao_Model_RemanejamentoLoa())->montaComboGND();
                $linha .= montaListaLoa('gndcod', $options, $loa['gndcod'], $loa['rlid']);
                $linha .= '</td>';
                $linha .= '<td class="combo">';
                        $options = (new Alteracao_Model_RemanejamentoLoa())->montaComboModalidade();
                $linha .= montaListaLoa('mapcod', $options, $loa['mapcod'], $loa['rlid']);
                $linha .= '</td>';
                $linha .= '<td class="combo">';
                        $options = (new Alteracao_Model_RemanejamentoLoa())->montaComboFonte();
                $linha .= montaListaLoa('fonid', $options, $loa['fonid'], $loa['rlid']);
                $linha .= '</td>';
                $linha .= '<td class="combo">';
                        $options = (new Alteracao_Model_RemanejamentoLoa())->montaComboIdoc();
                $linha .= montaListaLoa('idoid', $options, $loa['idoid'], $loa['rlid']);
                $linha .= '</td>';
                $linha .= '<td class="combo">';
                        $options = (new Alteracao_Model_RemanejamentoLoa())->montaComboIduso();
                $linha .= montaListaLoa('iduid', $options, $loa['iduid'], $loa['rlid']);
                $linha .= '</td>';
                $linha .= '<td class="combo">';
                        $options = (new Alteracao_Model_RemanejamentoLoa())->montaComboRP();
                $linha .= montaListaLoa('rpcod', $options, $loa['rpcod'], $loa['rlid']);
                $linha .= '</td>';
                $linha .= '        <td class="combo"><input type="text" id="vlDotacao'.$loa['rlid'].'" name="vlDotacao" rlid="'.$loa['rlid'].'" value="'.$loa['vldotacao'].'" size="15" maxlength="15" class="form-control input_valor_remanejamento"></td>';
                $linha .= '        <td class="combo"><input type="text" id="vlSuplementar'.$loa['rlid'].'" name="vlSuplementar" rlid="'.$loa['rlid'].'" value="'.$loa['vlsuplementar'].'" size="15" maxlength="15" class="form-control input_valor_remanejamento"></td>';
                $linha .= '        <td class="combo"><input type="text" id="vlCancelado'.$loa['rlid'].'" name="vlCancelado" rlid="'.$loa['rlid'].'" value="'.$loa['vlcancelado'].'" size="15" maxlength="15" class="form-control input_valor_remanejamento"></td>';
                $linha .= '        <td class="combo"><input type="text" id="vlSuplementarExcesso'.$loa['rlid'].'" name="vlSuplementarExcesso" rlid="'.$loa['rlid'].'" value="'.$loa['vlsuplementarexcesso'].'" size="15" maxlength="15" class="form-control input_valor_remanejamento"></td>';
                $linha .= '        <td class="combo"><input type="text" id="vlSuplementarSuperavit'.$loa['rlid'].'" name="vlSuplementarSuperavit" rlid="'.$loa['rlid'].'" value="'.$loa['vlsuplementarsuperavit'].'" size="15" maxlength="15" class="form-control input_valor_remanejamento"></td>';
                $linha .= '     </tr>';
            }
$linha .= '</table>';
echo $linha;
        ?>
<script>
    $(function () {

        $('#btnloa').click(function () {
            $('#btnloa').prop('disabled', 'true');
            // recuperaFuncionalSelecionada
            var tr = '<tr class="text-center">' +
                '        <th width="5% ">' +
                                 '<a href="#" class="btnRemoverLoa" id="btnRemoverLoa" rlid=""><i style="font-weight: 20px" class="fa fa-close"></i></a>&nbsp;&nbsp;&nbsp;\n' +
                                 '<a href="#" class="btnSalvarLoa" id="btnSalvarLoa" rlid=""><i class="fa fa-check"></i></a>' +
                         '</th>' +
                '        <th class="combo"><?php $options = (new Alteracao_Model_RemanejamentoLoa())->recuperaFuncionalSelecionada($_REQUEST['pedid']);
                                echo montaListaLoa('pliselid',$options);
                             ?>'+
                         '</th>' +
                '        <th class="combo"><?php $options = (new Alteracao_Model_RemanejamentoLoa())->montaComboCategoria();
                               echo montaListaLoa('ctecod', $options);
                           ?>'+
                        '</th>' +
                '        <th class="combo"><?php $options = (new Alteracao_Model_RemanejamentoLoa())->montaComboGND();
                               echo montaListaLoa('gndcod', $options);
                           ?>'+
                        '</th>' +
                '        <th class="combo"><?php $options = (new Alteracao_Model_RemanejamentoLoa())->montaComboModalidade();
                               echo montaListaLoa('mapcod', $options);
                           ?>'+
                        '</th>' +
                '        <th class="combo"><?php $options = (new Alteracao_Model_RemanejamentoLoa())->montaComboFonte();
                               echo montaListaLoa('fonid', $options);
                           ?>'+
                        '</th>' +
                '        <th class="combo"><?php $options = (new Alteracao_Model_RemanejamentoLoa())->montaComboIdoc();
                               echo montaListaLoa('idoid', $options, '1');
                           ?>'+
                        '</th>' +
                '        <th class="combo"><?php $options = (new Alteracao_Model_RemanejamentoLoa())->montaComboIduso();
                               echo montaListaLoa('iduid', $options, '6');
                           ?>'+
                        '</th>' +
                '        <th class="combo"><?php $options = (new Alteracao_Model_RemanejamentoLoa())->montaComboRP();
                               echo montaListaLoa('rpcod', $options);
                           ?>'+
                        '</th>' +
                '        <th><input type="text" id="vlDotacao" name="vlDotacao" rlid="" size="15" maxlength="15" class="form-control input_valor_remanejamento"></th>' +
                '        <th><input type="text" id="vlSuplementar" name="vlSuplementar" rlid="" size="15" maxlength="15" class="form-control input_valor_remanejamento"></th>' +
                '        <th><input type="text" id="vlCancelado" name="vlCancelado" rlid="" size="15" maxlength="15" class="form-control input_valor_remanejamento"></th>' +
                '        <th><input type="text" id="vlSuplementarExcesso" name="vlSuplementarExcesso" rlid="" size="15" maxlength="15" class="form-control input_valor_remanejamento"></th>' +
                '        <th><input type="text" id="vlSuplementarSuperavit" name="vlSuplementarSuperavit" rlid="" size="15" maxlength="15" class="form-control input_valor_remanejamento"></th>' +
                '     </tr>';
            var tbody = $("#tbListaLoa > tbody");
            tbody.append(tr);
            $('.input_valor_remanejamento').keyup(function(){
                $(this).val(mascaraglobal('###.###.###.###,##', $(this).val()));
            }).keyup();   
            $(".btnSalvarLoa").click(function(){
                var rlid = $(this).attr('rlid');
                if(validaObrigatorio(rlid)) {
                    SalvarLoa(rlid);
                }
            });

            $(".chosen").chosen({
                placeholder_text_single: " "
            });

            $(".btnRemoverLoa").click(function(){
                var rlid = $(this).attr('rlid');
                RemoverLoa(rlid);
            });

        });

        $(".chosen").chosen({
            placeholder_text_single: " "
        });

        $('.input_valor_remanejamento').keyup(function(){
            $(this).val(mascaraglobal('###.###.###.###', $(this).val()));
        }).keyup();

        $(".btnSalvarLoa").click(function(){
            var rlid = $(this).attr('rlid');

            if(validaObrigatorio(rlid)) {
                SalvarLoa(rlid);
            }
        });
        $(".btnRemoverLoa").click(function(){
            var rlid = $(this).attr('rlid');

            swal({
                    title: "Cuidado!",
                    text: "Tem certeza que deseja excluir o registro ?",
                    type: "warning",
                    confirmButtonText: 'Sim',
                    cancelButtonText: 'Não',
                    confirmButtonColor: "#DD6B55",
                    cancelButtonColor: "#DD6B55",
                    showCancelButton: true,
                    closeOnConfirm: true
                },
                function(isConfirm){
                    if(isConfirm){
                        RemoverLoa(rlid);
                    }
                });

        });
    });

    function SalvarLoa(rlid){

        // validaObrigatorio(rlid);

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

                    swal({
                            title: "",
                            text: "Registro salvo com sucesso!",
                            type: "success",
                            confirmButtonText: 'Ok',
                            closeOnConfirm: true
                        },
                        function(){
                            $(".div_remanejamento_loa").load('?modulo=principal/cadastro_alteracoes&acao=C&req=carrega_remanejamento_loa&pedid='+$("#pedid").val()+'&tpdid='+$("#tpdid").val());
                        })
                }else{
                    swal({
                        title: "",
                        text: "Não foi possível salvar o registro!",
                        type: "error"
                    })
                }
            }
        )
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

    function validaObrigatorio(rlid) {

        var msg = '';

        if($('#pliselid'+rlid).val() == '' || $('#pliselid'+rlid).val() == null){
            msg += "O preenchimento do campo \"Funcional\" é obrigatório.";
        }
        if($('#ctecod'+rlid).val() == '' || $('#ctecod'+rlid).val() == null){
            msg+="<br>O preenchimento do campo \"Categoria\" é obrigatório.";
        }
        if($('#gndcod'+rlid).val() == '' || $('#gndcod'+rlid).val() == null){
            msg+="<br>O preenchimento do campo \"GND\" é obrigatório.";
        }
        if($('#mapcod'+rlid).val() == '' || $('#mapcod'+rlid).val() == null){
            msg+="<br>O preenchimento do campo \"Modalidade\" é obrigatório.";
        }
        if($('#fonid'+rlid).val() == '' || $('#fonid'+rlid).val() == null){
            msg+="<br>O preenchimento do campo \"Fonte\" é obrigatório.";
        }
        if($('#idoid'+rlid).val() == '' || $('#idoid'+rlid).val() == null){
            msg+="<br>O preenchimento do campo \"IDOC\" é obrigatório.";
        }
        if($('#iduid'+rlid).val() == '' || $('#iduid'+rlid).val() == null){
            msg+="<br>O preenchimento do campo \"IDUSO\" é obrigatório.";
        }
        if($('#rpcod'+rlid).val() == '' || $('#rpcod'+rlid).val() == null){
            msg+="<br>O preenchimento do campo \"RP\" é obrigatório.";
        }

        if(msg !== '') {
            alert(msg);
            return false;
        } else {
            return true;
        }

    }
</script>
<style>
    .combo > .chosen-container {
        font-size: 9px;
    }
    .input_valor_remanejamento {
        font-size: 9px;
        width: 75px;
        padding: 0px;
    }
</style>