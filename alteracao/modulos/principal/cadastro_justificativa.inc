<?php
    $justificativa = (new Alteracao_Model_Justificativa())->recuperaJustificativa($_REQUEST['pedid']);
?>

<style>
    .size{
        font-size: 20px;
        color: #1AB394;
    }
</style>
<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <div class="col-lg-12">
            <div style="text-align: left; color: #1AB394;">
                <h2>Essas Justificativas tem o objetivo de fornecer insumos para a rastreabilidade das Alterações Orçamentárias</h2>
                <hr style="width: 100%; margin: 0 auto 0 0; border-color: rgba(29, 29, 29, 0.95)">
            </div>
            <br>
            <h6><span style="color: #ff0000"> Os campos com * são de preenchimento obrigatório</span></h6>
        </div>
    </div>
    <div class="row">
        <form id="justificativa" name="justificativa" method="post" class="form-horizontal">
            <input type="hidden" name="pedid" id="pedid" value="<?= $_REQUEST['pedid']; ?>">
            <input type="hidden" name="jstid" id="jstid" value="<?= $justificativa->jstid; ?>">
            <div class="col-md-6">
                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <?php
                            echo $simec->textarea('jstnecessidade', 'Necessidade de Alteração <i class="fa fa-info-circle size" id="necessidade"></i>',
                                                $justificativa->jstnecessidade,
                                                ['cols' => 50, 'rows' => 6, 'required']
                            );
                            echo $simec->textarea('jstfinanciamento',
                                                'Formas de financiamento de Crédito e adequação da proposta à meta fiscal e ao limite de gastos <i class="fa fa-info-circle size" id="formas"></i>',
                                                $justificativa->jstfinanciamento,
                                                ['cols' => 50, 'rows' => 6]
                            );
                            echo $simec->textarea('jstmedida',
                                                'Medida Provisória <i class="fa fa-info-circle size" id="medida"></i>',
                                                $justificativa->jstmedida,
                                                ['cols' => 50, 'rows' => 6]
                            );
                        ?>
                    </div>
                </div>
            </div>
            <div class="col-md-6">
                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <?php
                            echo $simec->textarea('jstcausa',
                                'Causa da Demanda <i class="fa fa-info-circle size" id="causa"></i>',
                                $justificativa->jstcausa,
                                ['cols' => 50, 'rows' => 6, 'required']
                            );
                            echo $simec->textarea('jstfontes',
                                                'Verificação das fontes, dos Identificadores de uso - Iduso e dos identificadores de Resultado Primário - RP <i class="fa fa-info-circle size" id="verificacao"></i>',
                                                $justificativa->jstfontes,
                                                ['cols' => 50, 'rows' => 6]
                            );
                            echo $simec->textarea('jstlegislacao',
                                                'Legislação Específica <i class="fa fa-info-circle size" id="legislacao"></i>',
                                                $justificativa->jstlegislacao,
                                                ['cols' => 50, 'rows' => 6]
                            );
                        ?>
                    </div>
                </div>
            </div>

            <div class="col-md-12">
                <div class="ibox-title">
                    <?php
                        echo $simec->textarea('jstoutros',
                            'Outras Informações <i class="fa fa-info-circle size" id="info"></i>',
                            $justificativa->jstoutros,
                            ['cols' => 50, 'rows' => 6]
                        );
                    ?>
                </div>
            </div>
        </form>
        <div class="text-center">
            <button class="btn btn-primary btn-buscar" type="button" id="salvar-justificativa">
                <span class="glyphicon glyphicon-saved"></span> Salvar
            </button>
        </div>
    </div>
</div>
<script>
    $(document).ready(function (){
        $('#necessidade').prop('title', 'Indicar a necessidade da alteração orçamentária, informando a destinação da proposição.');
        $('#formas').prop('title', 'Indicar as compensações orçamentárias oferecidas e/ou a utilização de excesso de ' +
            'arrecadação ou superávit financeiro, além de verificar a adequação das alterações à meta fiscal do exercício e ao ' +
            'limite de gastos definido pela EC 95/2016, e que os cancelamentos oferecidos não trazem prejuízo à execução das programações canceladas.'
        );
        $('#medida').prop('title', 'Indicar a urgência, a relevância e a imprevisibilidade da proposição.');
        $('#causa').prop('title', 'Indicar a causa para a demanda, o que provocou a necessidade do ajuste e dos novos recursos e ' +
            'explicitar, caso haja, a alteração nas metas de atendimento da política pública envolvida.'
        );
        $('#verificacao').prop('title', 'Verificar o balanceamento dessas classificações orçamentárias, justificando, caso estejam desbalanceadas.');
        $('#legislacao').prop('title', 'Informar, quando for o caso, a legislação \'específica\' da qual decorre ou se baseia a alteração orçamentária solicitada.');
        $('#info').prop('title', 'Além de outras informações que forem julgadas como necessárias, registrar que não há óbices no prosseguimento da proposta/solicitação.');

        $('#salvar-justificativa').click(function () {

            $.post('?modulo=principal/cadastro_alteracoes&acao=C&req=salvar_justificativa',
                {
                    jstid: $('#jstid').val(),
                    pedid: $('#pedid').val(),
                    jstnecessidade: $('#jstnecessidade').val(),
                    jstcausa: $('#jstcausa').val(),
                    jstfinanciamento: $('#jstfinanciamento').val(),
                    jstfontes: $('#jstfontes').val(),
                    jstmedida: $('#jstmedida').val(),
                    jstlegislacao: $('#jstlegislacao').val(),
                    jstoutros: $('#jstoutros').val(),
                },
                function(retorno){
                    if (retorno){
                        swal({
                            title: "",
                            text: "Registro salvo com sucesso!",
                            type: "success",
                            confirmButtonText: 'Ok',
                            closeOnConfirm: true
                        })
                    }
                }
            );
        });
    });
</script>