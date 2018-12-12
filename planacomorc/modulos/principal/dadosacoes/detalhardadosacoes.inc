<?php
//Chamada de programa
include_once APPRAIZ . "includes/cabecalho.inc";
require_once APPRAIZ . "includes/library/simec/Helper/FlashMessage.php";
include_once APPRAIZ . "includes/library/simec/Grafico.php";
include_once '_funcoes.php';
/*
 * Carregando os dados de Identificação da Ação
 */
if (isset($_REQUEST['unicod']) && $_REQUEST['unicod'] != '' && isset($_REQUEST['acacod']) && $_REQUEST['acacod'] != '') {
    $sql = <<<DML
        SELECT
        uni.unicod || ' - ' || uni.unidsc   AS unidade                      ,
        aca.acacod|| ' - ' || aca.acatitulo AS acao                         ,
        aca.acatipodsc                                                      ,
        aca.acainiciativacod || ' - ' || aca.acainiciativadsc AS iniciativa ,
        aca.acaobjetivocod|| ' - ' || aca.acaobjetivodsc      AS objetivo   ,
        aca.prgtipo                                                         ,
        aca.prgcod|| ' - ' || aca.prgdsc AS programa                        ,
        aca.esfcod                                                          ,
        aca.funcod                                                          ,
        aca.sfundsc                                                         ,
        aca.acadescricao                                                    ,
        aca.acabaselegal                                                    ,
        aca.acatipoinclusao                                                 ,
        aca.prodsc                                                          ,
        aca.unmdsc
       FROM
        monitora.acao aca
       JOIN
        public.unidade uni
       USING
        ( unicod )
       WHERE
        aca.acastatus ='A'
        AND
        prgano = '{$_SESSION['exercicio']}'
       AND
        unicod = '{$_REQUEST['unicod']}'
       AND acacod = '{$_REQUEST['acacod']}'

DML;
    $sql_loccod = $sql . " AND loccod = '0001'";
    $dados['cadastro'] = $db->pegaLinha($sql_loccod);
    if (empty($dados['cadastro'])) {
        $dados['cadastro'] = $db->pegaLinha($sql);
    }

    $sql = <<<DML

    SELECT
        0                            AS metaacao        ,
        SUM( exe.vlrdotacaoinicial ) AS dotacaoinicial  ,
        SUM( exe.vlrdotacaoatual )   AS vlrdotacaoatual ,
        SUM( exe.vlrempenhado )      AS empenhado       ,
        SUM( exe.vlrliquidado )      AS liquidado       ,
        SUM( exe.vlrpago )           AS pago
       FROM
        spo.siopexecucao exe
       JOIN
        monitora.acao aca
       ON
        aca.unicod = exe.unicod
       AND
        aca.acacod = exe.acacod
       AND
        aca.prgano = exe.exercicio
       WHERE
        aca.acastatus ='A'
        AND
        aca.prgano = '{$_SESSION['exercicio']}'
       AND
        aca.unicod = '{$_REQUEST['unicod']}'
       AND aca.acacod = '{$_REQUEST['acacod']}'
DML;
    $dados['financeiro'] = $db->pegaLinha($sql);
}
//recuperando o retorno para submeter o formulário
if ($_REQUEST['requisicao'] == "EnviarExtrato") {
    if (enviarEmailExtratoAcoes($_REQUEST) == 1) {
        echo '<script>
                alert("E-mail foi enviado com sucesso. \nAguarde que iremos atendê-lo o mais breve possível!"); 
         </script>';
    } else {
        '<script>
                alert("E-mail não foi enviado com sucesso. \nTente novamente!");
                location.href=' . $_SESSION['sisdiretorio'] . '.php?modulo=inicio&acao=C' . $_SESSION['sisdsc'] . ';
         </script>';
    }
    // exit();
}

//####modificar para enviar para o WERTER
function enviarEmailExtratoAcoes($params) {
    switch ($params['origem']) {
        case "erro-identificacao":
            $origem = "Identificação da ação ";
            break;
        case "erro-localizadores":
            $origem = "Localizadores ";
            break;
        case "erro-planosorcamentarios":
            $origem = "Planos Orçamentários (POs)";
            break;
        case "erro-ptres":
            $origem = "Planos de Trabalho Resumidos (PTRES)";
            break;
        case "erro-emendas":
            $origem = "Emendas  ";
            break;
        case "erro-subacoes":
            $origem = "Subações";
            break;
        case "erro-pis":
            $origem = "Planos Internos (PIs)";
            break;
        case "erro-acompanhamento":
            $origem = "Acompanhamentos Orçamentários (SIOP)";
            break;
        case "erro-alteracoes":
            $origem = "Alterações Orçamentárias";
            break;
        case "erro-ploa":
            $origem = "PLOA";
            break;
        case "erro-progf":
            $origem = "Solicitações de Programação Financeira (PF)";
            break;
        case "erro-ted":
            $origem = "Termos de Execução Descentralizada (TED)";
            break;
    }
    $destinatarios[] = $_SESSION['email_sistema'];
    $paransEmail['remetente'] = array("nome" => SIGLA_SISTEMA, "email" => EMAIL_SISTEMA_NOREPLY);
    $paransEmail['assunto'] = "Reportar Divergência em um valor no quadro - '{$origem}'";
    $paransEmail['mensagem'] = "<pre>Prezado,
        Na Ação <span style='color: red;'>{$_REQUEST['acacod']}</span> e Unidade <span style='color: red;'>{$_REQUEST['unicod']} </span> existem alterações para serem feitas.

        No quadro '<span style='color: red;'>{$origem}</span>' ressalto as seguintes alterações:

        <div style='margin: 0px 100px;'>{$params['justificativa']}</div>

        Atenciosamente,
        {$_SESSION['usunome']}
        {$_SESSION['usuemail']}
        
        </pre>";
    enviar_email($paransEmail['remetente'], $destinatarios, $paransEmail['assunto'], $paransEmail['mensagem']);
    return true;
}
?>
<div id="modal-confirm" class="modal fade">
    <div class="modal-dialog-confirm" style="width:90%;margin:0 auto;"> 
        <div class="modal-content">
            <div class="modal-header">
                <button class="close" aria-hidden="true" data-dismiss="modal" type="button">×</button>
                <h4 class="modal-title">Aviso!</h4>
            </div>
            <div class="modal-body">
                <p></p>
            </div>
            <div class="modal-footer">
                <button class="btn btn-danger" data-dismiss="modal" type="button"><b>Não</b> Enviar E-mail</button>
                <button class="btn btn-primary" id="enviar-email" type="button">Enviar E-mail</button>
            </div>
        </div>
    </div>
</div>
<section class="col-md-12">
    <ol class="breadcrumb">
        <li><a href="/<?php echo $_SESSION['sisdiretorio']; ?>/<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C"><?= $_SESSION['sisdsc']; ?></a></li>
        <li><a href = "<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=principal/dadosacoes/listadadosacoes&acao=A">Extrato completo das Ações <?= $_SESSION['exercicio']; ?></a></li>
        <li class="active">Detalhamento da Ação</li>
    </ol>
</section>
<style>
    .col-md-7 {
        padding: 10px;
        text-align: justify;
    }
    p{
        padding: 5px;
    }
</style>
<script>

    $(document).ready(function () {
//        $( ".rodape" ).remove();

    });

    function abrir_modal(origem) {
//        origem = $(".botaoErro").attr("id");
//        $(".botaoErro").click(function () {
//            alert(this.id);
//        })
        url = "http://<?php echo $_SERVER['HTTP_HOST'] . $_SERVER['SCRIPT_NAME'] ?>?modulo=principal/dadosacoes/popupdadosacoes&acao=A&tipo=" + origem;
        $.post(url, function (response) {
            $('#modal-confirm .modal-body').html(response);
            $('#modal-confirm .modal-title').html('Aviso!');
            // $('#modal-confirm .btn-primary').remove();
            $('.modal-dialog-confirm').show();
            $('#modal-confirm').modal();

            $('#modal-confirm .modal-content').css('width', '60%');
            $("#modal-confirm #enviar-email").click(function () {
                $('#requisicao').html('<input type="hidden" name="requisicao" value="EnviarExtrato">');
                $("#formulario").submit();
                return true;
            })
        });
    }



    function alteraBotaoCollapse() {
        if ($('#botao').hasClass('btn-success')) {
            $('.panel-collapse').collapse('show');
            $('#botao').removeClass('btn-success');
            $('#botao').addClass('btn-warning');
            $('#botao').html('<span class="glyphicon glyphicon-resize-small"></span> Fechar Todos');

        } else {
            $('.panel-collapse').collapse('hide');
            $('#botao').removeClass('btn-warning');
            $('#botao').addClass('btn-success');
            $('#botao').html('<span class="glyphicon glyphicon-resize-full"></span> Expandir Todos');
        }
    }
    /* 
     * Abre o TED  em outra janela, no módulo de origem 
     */
    function abrirTED(ted) {
        window.open('http://simec.mec.gov.br/ted/ted.php?modulo=principal/termoexecucaodescentralizada/previsao&acao=A&ted=' + ted);
    }
    
    /* 
     * Abre a OBRA em outra janela, no módulo de origem 
     */
    function abrirObra(obrid) {
        window.open('http://simec.mec.gov.br/obras/obras.php?modulo=principal/cadastro_pi&acao=A&obrid=' + obrid);
    }

</script>
<!--<form class="form-detalhes" id="form-responsavel" method="POST" role="form">-->
<section class="col-md-12">
    <section class="panel panel-info">
        <section class="panel-heading">
            <p>
                <label class="control-label" >
                    Unidade Orçamentária (UO) :
                </label>
            <div id="unidade">
                <?php echo ($dados['cadastro']['unidade'] ? $dados['cadastro']['unidade'] : ' - '); ?>
            </div>
            </p>
            <p>
                <label class="control-label"  >
                    Ação:
                </label>
            <div id="acao">
                <?php echo $dados['cadastro']['acao']; ?>
            </div>
            </p>
        </section>
        <section class="panel-footer">
            <button type="button" id="botao" onclick="alteraBotaoCollapse();" class="btn btn-success"><span class="glyphicon glyphicon-resize-small"></span>
                Expandir Todos</button>
        </section>
    </section>


    <br/><div class="panel-group" id="accordion">
        <div class="panel panel-info">
            <div class="panel-heading" style="overflow: auto;">
                <h4 class="panel-title" >
                    <a data-toggle="collapse" style="" class="text-uppercase" data-parent="#accordion" href="#identificacao" >
                        Identificação da ação
                    </a>
                    <a style="float:right; cursor: pointer">
                        <span id="erro-identificacao" class="iconeBotao botaoErro glyphicon glyphicon-thumbs-down" style="color: #D79C2B; font-size: 16px; margin-right: 5px;" onclick="abrir_modal(this.id);" title="Reportar divergência em um valor neste quadro." ></span>
                    </a>
                </h4>
            </div>
            <div id="identificacao"class="collapse panel-collapse  " style="overflow-x: auto;">
                <br/><br/>
                <div style="width: 48%; padding-left: 2%; float:left">
                    <table class="table table-striped table-bordered table-hover">
                        <tbody>
                            <tr>
                                <td class="text-left text-info" style="width:33px">
                                    Unidade Orçamentária (UO) :
                                </td>
                                <td class="text-left" style="width:33px">
                                    <?php echo ($dados['cadastro']['unidade'] ? $dados['cadastro']['unidade'] : ' - '); ?>
                                </td>
                            </tr>

                            <tr>
                                <td class="text-left text-info" style="width:33px">
                                    Ação:
                                </td>
                                <td class="text-left" style="width:33px">
                                    <?php echo $dados['cadastro']['acao']; ?>
                                </td>
                            </tr>

                            <tr>
                                <td class="text-left text-info" style="width:33px">
                                    Tipo da Ação:
                                </td>
                                <td class="text-left" style="width:33px">
                                    <?php echo $dados['cadastro']['acatipodsc']; ?>
                                </td>
                            </tr>

                            <tr>
                                <td class="text-left text-info" style="width:33px">
                                    Iniciativa:
                                </td>
                                <td class="text-left" style="width:33px">
                                    <?php echo ($dados['cadastro']['iniciativa'] ? $dados['cadastro']['iniciativa'] : ' - '); ?>
                                </td>
                            </tr>

                            <tr>
                                <td class="text-left text-info" style="width:33px">
                                    Objetivo:
                                </td>
                                <td class="text-left" style="width:33px">
                                    <?php echo ($dados['cadastro']['objetivo'] ? $dados['cadastro']['objetivo'] : ' - '); ?>
                                </td>
                            </tr>

                            <tr>
                                <td class="text-left text-info" style="width:33px">
                                    Programa (Tipo):
                                </td>
                                <td class="text-left" style="width:33px">
                                    <?php echo ($dados['cadastro']['prgtipo'] ? $dados['cadastro']['prgtipo'] : ' - '); ?>
                                </td>
                            </tr>

                            <tr>
                                <td class="text-left text-info" style="width:33px">
                                    Programa:
                                </td>
                                <td class="text-left" style="width:33px">
                                    <?php echo $dados['cadastro']['programa']; ?>
                                </td>
                            </tr>

                            <tr>
                                <td class="text-left text-info" style="width:33px">
                                    Esfera:
                                </td>
                                <td class="text-left" style="width:33px">
                                    <?php echo $dados['cadastro']['esfcod']; ?>
                                </td>
                            </tr>

                            <tr>
                                <td class="text-left text-info" style="width:33px">
                                    Função:
                                </td>
                                <td class="text-left" style="width:33px">
                                    <?php echo $dados['cadastro']['funcod']; ?>
                                </td>
                            </tr>

                            <tr>
                                <td class="text-left text-info" style="width:33px">
                                    Subfunção:
                                </td>
                                <td class="text-left" style="width:33px">
                                    <?php echo $dados['cadastro']['sfundsc']; ?>
                                </td>
                            </tr>

                            <tr>
                                <td class="text-left text-info" style="width:33px">
                                    Descrição da Ação
                                </td>
                                <td class="text-left" style="width:33px">
                                    <?php echo ($dados['cadastro']['acadescricao'] ? $dados['cadastro']['acadescricao'] : ' - '); ?>
                                </td>
                            </tr>

                            <tr>
                                <td class="text-left text-info" style="width:33px">
                                    Base Legal da Ação:
                                </td>
                                <td class="text-left" style="width:33px">
                                    <?php echo ($dados['cadastro']['acabaselegal'] ? $dados['cadastro']['acabaselegal'] : ' - '); ?>
                                </td>
                            </tr>

                            <tr>
                                <td class="text-left text-info" style="width:33px">
                                    Tipo de Inclusão da Ação:
                                </td>
                                <td class="text-left" style="width:33px">
                                    <?php echo ($dados['cadastro']['acatipoinclusao'] ? $dados['cadastro']['acatipoinclusao'] : ' - '); ?>
                                </td>
                            </tr>

                            <tr>
                                <td class="text-left text-info" style="width:33px">
                                    Produto:
                                </td>
                                <td class="text-left" style="width:33px">
                                    <?php echo ($dados['cadastro']['prodsc'] ? $dados['cadastro']['prodsc'] : ' - '); ?>
                                </td>
                            </tr>

                            <tr>
                                <td class="text-left text-info" style="width:33px">
                                    Unidade de Medida:
                                </td>
                                <td class="text-left" style="width:33px">
                                    <?php echo ($dados['cadastro']['unmdsc'] ? $dados['cadastro']['unmdsc'] : ' - '); ?>
                                </td>
                            </tr>
                        </tbody>
                    </table>

                </div>
                <div style="width: 2%; float:left">
                    <p></p>
                </div>
                <div style="width: 48%; float:left">

                    <table class="table table-striped table-bordered table-hover">
                        <tbody>
                            <tr>
                                <td colspan="2" class="text-center text-info">
                                    <b>Dados do Físico / Financeiro</b>
                                </td>
                            </tr>

                            <tr>
                                <td class="text-left text-info" style="width:33px">
                                    Meta Física:
                                </td>
                                <td class="text-left" style="width:33px">
                                    <?php echo ($dados['financeiro']['metaacao'] ? $dados['financeiro']['metaacao'] : ' - '); ?>
                                </td>
                            </tr>

                            <tr>
                                <td class="text-left text-info" style="width:33px">
                                    Dotação Inicial (R$):
                                </td>
                                <td class="text-left" style="width:33px">
                                    <?php echo ($dados['financeiro']['dotacaoinicial'] ? mascaraMoeda($dados['financeiro']['dotacaoinicial']) : ' - '); ?>
                                </td>
                            </tr>

                            <tr>
                                <td class="text-left text-info" style="width:33px">
                                    Dotação Atual (R$):
                                </td>
                                <td class="text-left" style="width:33px">
                                    <?php echo ($dados['financeiro']['vlrdotacaoatual'] ? mascaraMoeda($dados['financeiro']['vlrdotacaoatual']) : ' - '); ?>
                                </td>
                            </tr>

                            <tr>
                                <td class="text-left text-info" style="width:33px">
                                    Empenhado (R$):
                                </td>
                                <td class="text-left" style="width:33px">
                                    <?php echo ($dados['financeiro']['empenhado'] ? mascaraMoeda($dados['financeiro']['empenhado']) : ' - '); ?>
                                </td>
                            </tr>

                            <tr>
                                <td class="text-left text-info" style="width:33px">
                                    Liquidado (R$):
                                </td>
                                <td class="text-left" style="width:33px">
                                    <?php echo ($dados['financeiro']['liquidado'] ? mascaraMoeda($dados['financeiro']['liquidado']) : ' - '); ?>
                                </td>
                            </tr>

                            <tr>
                                <td class="text-left text-info" style="width:33px">
                                    Pago (R$):
                                </td>
                                <td class="text-left" style="width:33px">
                                    <?php echo ($dados['financeiro']['pago'] ? mascaraMoeda($dados['financeiro']['pago']) : ' - '); ?>
                                </td>
                            </tr>

                            <tr>
                                <td colspan="2">
                        <center>
                            <div style="clear:both;">
                                <?php
                                /* Monta o HTML do grafico de Execução */
                                $sqlGrafico = " SELECT
                                        descricao,
                                        categoria,
                                        valor
                                       FROM
                                       (
                                    SELECT
                                        'Dotação Atual' AS descricao ,
                                        'Total'         AS categoria ,
                                        dotacaoatual    AS valor
                                       FROM
                                        (
                                       SELECT
                                          SUM( exe.vlrdotacaoatual ) AS dotacaoatual ,
                                          SUM( exe.vlrempenhado )    AS empenhado    ,
                                          SUM( exe.vlrliquidado )    AS liquidado    ,
                                          SUM( exe.vlrpago )         AS pago
                                       FROM
                                          monitora.acao aca
                                       LEFT JOIN
                                          spo.siopexecucao exe
                                       ON
                                          aca.unicod = exe.unicod
                                       AND
                                          aca.acacod = exe.acacod
                                       AND
                                          aca.prgano = exe.exercicio
                                          --
                                        WHERE
                                        aca.acastatus ='A'
                                       AND
                                        aca.prgano = '{$_SESSION['exercicio']}'
                                       AND
                                        aca.unicod = '{$_REQUEST['unicod']}'
                                       AND aca.acacod = '{$_REQUEST['acacod']}' ) foo
                                       UNION
                                       SELECT
                                        'Empenhado' AS descricao ,
                                        'Total'     AS categoria ,
                                        empenhado   AS valor
                                       FROM
                                        (
                                       SELECT
                                          SUM( exe.vlrdotacaoatual ) AS dotacaoatual ,
                                          SUM( exe.vlrempenhado )    AS empenhado    ,
                                          SUM( exe.vlrliquidado )    AS liquidado    ,
                                          SUM( exe.vlrpago )         AS pago
                                       FROM
                                          monitora.acao aca
                                       LEFT JOIN
                                          spo.siopexecucao exe
                                       ON
                                          aca.unicod = exe.unicod
                                       AND
                                          aca.acacod = exe.acacod
                                       AND
                                          aca.prgano = exe.exercicio
                                          --
                                     WHERE
                                        aca.acastatus ='A'
                                       AND
                                        aca.prgano = '{$_SESSION['exercicio']}'
                                       AND
                                        aca.unicod = '{$_REQUEST['unicod']}'
                                       AND aca.acacod = '{$_REQUEST['acacod']}' ) foo
                                       UNION
                                       SELECT
                                        'Liquidado' AS descricao ,
                                        'Total'     AS categoria ,
                                        liquidado   AS valor
                                       FROM
                                        (
                                       SELECT
                                          SUM( exe.vlrdotacaoatual ) AS dotacaoatual ,
                                          SUM( exe.vlrempenhado )    AS empenhado    ,
                                          SUM( exe.vlrliquidado )    AS liquidado    ,
                                          SUM( exe.vlrpago )         AS pago
                                       FROM
                                          monitora.acao aca
                                       LEFT JOIN
                                          spo.siopexecucao exe
                                       ON
                                          aca.unicod = exe.unicod
                                       AND
                                          aca.acacod = exe.acacod
                                       AND
                                          aca.prgano = exe.exercicio
                                          --
                                       WHERE
                                        aca.acastatus ='A'
                                       AND
                                        aca.prgano = '{$_SESSION['exercicio']}'
                                       AND
                                        aca.unicod = '{$_REQUEST['unicod']}'
                                       AND aca.acacod = '{$_REQUEST['acacod']}') foo
                                       UNION
                                       SELECT
                                        'Pago'  AS descricao ,
                                        'Total' AS categoria ,
                                        pago    AS valor
                                       FROM
                                        (
                                       SELECT
                                          SUM( exe.vlrdotacaoatual ) AS dotacaoatual ,
                                          SUM( exe.vlrempenhado )    AS empenhado    ,
                                          SUM( exe.vlrliquidado )    AS liquidado    ,
                                          SUM( exe.vlrpago )         AS pago
                                       FROM
                                          monitora.acao aca
                                       LEFT JOIN
                                          spo.siopexecucao exe
                                       ON
                                          aca.unicod = exe.unicod
                                       AND
                                          aca.acacod = exe.acacod
                                       AND
                                          aca.prgano = exe.exercicio
                                          WHERE
                                        aca.acastatus ='A'
                                       AND
                                        aca.prgano = '{$_SESSION['exercicio']}'
                                       AND
                                        aca.unicod = '{$_REQUEST['unicod']}'
                                       AND aca.acacod = '{$_REQUEST['acacod']}'
                                      ) foo ) AS foo2
                                        ORDER BY
                                         1";
#ver($sqlGrafico);
                                $grafico = new Grafico(null, false);
                                $grafico->setTipo(Grafico::K_TIPO_COLUNA)->setId('graficoAcao')->setTitulo('Execução Orçamentária')->gerarGrafico($sqlGrafico);
                                ?>
                            </div>
                        </center>
                        </td>
                        </tr>

                        </tbody>
                    </table>
                </div>
            </div>
        </div>
    </div>
    <br/><div class="panel-group" id="accordion">
        <div class="panel panel-info">
            <div class="panel-heading" style="overflow: auto;">
                <h4 class="panel-title" >
                    <a data-toggle="collapse" style="" class="text-uppercase" data-parent="#accordion" href="#localizadores" >
                        Localizadores
                    </a>
                    <a style="float:right; cursor: pointer">
                        <span id="erro-localizadores" class="iconeBotao botaoErro glyphicon glyphicon-thumbs-down" style="color: #D79C2B; font-size: 16px; margin-right: 5px;" title="Reportar divergência em um valor neste quadro." onclick="abrir_modal(this.id);" ></span>
                    </a>
                </h4>
            </div>
            <div id="localizadores"class="collapse panel-collapse  " style="overflow-x: auto;">
                <ul class="nav nav-tabs" role="tablist">
                    <!--                    <li role="presentation" class="active"><a href="#tabela-localizadores" aria-controls="tabela-localizadores" role="tab" data-toggle="tab">Dados</a></li>-->
                    <!--                    <li role="presentation"><a href="#grafico-localizadores" aria-controls="grafico-localizadores" role="tab" data-toggle="tab">Gráfico da Execução Orçamentária</a></li>-->
                </ul>
                <div class="tab-content">
                    <div role="tabpanel" class="tab-pane active" id="tabela-localizadores">
                        <?php
                        $sql = <<<DML
                       SELECT
                        aca.loccod || ' - '|| aca.sacdsc AS localizador     ,
                        aca.metalocalizador              AS metalocalizador ,
                        aca.prodsc                                          ,
                        aca.unmdsc                                          ,
                        SUM( exe.vlrdotacaoatual ) AS dotacaoatual          ,
                        SUM( exe.vlrempenhado )    AS empenhado             ,
                        SUM( exe.vlrliquidado )    AS liquidado             ,
                        SUM( exe.vlrpago )         AS pago
                       FROM
                        monitora.acao aca
                       LEFT JOIN
                        spo.siopexecucao exe
                       ON
                        aca.unicod = exe.unicod
                       AND
                        aca.acacod = exe.acacod
                       AND
                        aca.prgano = exe.exercicio
                       AND aca.loccod = exe.loccod
                        WHERE
                            aca.acastatus ='A'
                           AND
                            aca.prgano = '{$_SESSION['exercicio']}'
                           AND
                            aca.unicod = '{$_REQUEST['unicod']}'
                           AND aca.acacod = '{$_REQUEST['acacod']}'
                        GROUP BY
                         1 ,
                         2 ,
                         3 ,
                         4
                        ORDER BY 1

DML;
                        $cabecalho = array(
                            'Localizador',
                            'Meta Física',
                            'Produto',
                            'Unidade de Medida',
                            'Dotação Atual (R$)',
                            'Empenhado (R$)',
                            'Liquidado (R$)',
                            'Pago (R$)'
                        );
                        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
                        $listagem->setCabecalho($cabecalho);
                        $listagem->addCallbackDeCampo(array('dotacaoatual', 'empenhado', 'liquidado', 'pago'), 'mascaraMoeda');
                        $listagem->addCallbackDeCampo(array('localizador'), 'alinhaParaEsquerda');
                        $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, array('dotacaoatual', 'empenhado', 'liquidado', 'pago'));
                        $listagem->setFormOff();
                        $listagem->setQuery($sql);
                        $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
                        ?>
                    </div>
                    <div role="tabpanel" class="tab-pane" id="grafico-localizadores">
                        <?php
                        /* Monta o HTML do grafico de Execução */
                        $sqlGrafico = "
                                    SELECT
                                        'Dotação Atual' AS descricao ,
                                        'Total'         AS categoria ,
                                        dotacaoatual    AS valor
                                       FROM
                                        (
                                       SELECT
                                          SUM( exe.vlrdotacaoatual ) AS dotacaoatual ,
                                          SUM( exe.vlrempenhado )    AS empenhado    ,
                                          SUM( exe.vlrliquidado )    AS liquidado    ,
                                          SUM( exe.vlrpago )         AS pago
                                       FROM
                                          monitora.acao aca
                                       LEFT JOIN
                                          spo.siopexecucao exe
                                       ON
                                          aca.unicod = exe.unicod
                                       AND
                                          aca.acacod = exe.acacod
                                       AND
                                          aca.prgano = exe.exercicio
                                       AND
                                          aca.loccod = exe.loccod
                                          --
                                        WHERE
                                        aca.acastatus ='A'
                                       AND
                                        aca.prgano = '{$_SESSION['exercicio']}'
                                       AND
                                        aca.unicod = '{$_REQUEST['unicod']}'
                                       AND aca.acacod = '{$_REQUEST['acacod']}' ) foo
                                       UNION
                                       SELECT
                                        'Empenhado' AS descricao ,
                                        'Total'     AS categoria ,
                                        empenhado   AS valor
                                       FROM
                                        (
                                       SELECT
                                          SUM( exe.vlrdotacaoatual ) AS dotacaoatual ,
                                          SUM( exe.vlrempenhado )    AS empenhado    ,
                                          SUM( exe.vlrliquidado )    AS liquidado    ,
                                          SUM( exe.vlrpago )         AS pago
                                       FROM
                                          monitora.acao aca
                                       LEFT JOIN
                                          spo.siopexecucao exe
                                       ON
                                          aca.unicod = exe.unicod
                                       AND
                                          aca.acacod = exe.acacod
                                       AND
                                          aca.prgano = exe.exercicio
                                       AND
                                          aca.loccod = exe.loccod
                                          --
                                     WHERE
                                        aca.acastatus ='A'
                                       AND
                                        aca.prgano = '{$_SESSION['exercicio']}'
                                       AND
                                        aca.unicod = '{$_REQUEST['unicod']}'
                                       AND aca.acacod = '{$_REQUEST['acacod']}' ) foo
                                       UNION
                                       SELECT
                                        'Liquidado' AS descricao ,
                                        'Total'     AS categoria ,
                                        liquidado   AS valor
                                       FROM
                                        (
                                       SELECT
                                          SUM( exe.vlrdotacaoatual ) AS dotacaoatual ,
                                          SUM( exe.vlrempenhado )    AS empenhado    ,
                                          SUM( exe.vlrliquidado )    AS liquidado    ,
                                          SUM( exe.vlrpago )         AS pago
                                       FROM
                                          monitora.acao aca
                                       LEFT JOIN
                                          spo.siopexecucao exe
                                       ON
                                          aca.unicod = exe.unicod
                                       AND
                                          aca.acacod = exe.acacod
                                       AND
                                          aca.prgano = exe.exercicio
                                       AND
                                          aca.loccod = exe.loccod
                                          --
                                       WHERE
                                        aca.acastatus ='A'
                                       AND
                                        aca.prgano = '{$_SESSION['exercicio']}'
                                       AND
                                        aca.unicod = '{$_REQUEST['unicod']}'
                                       AND aca.acacod = '{$_REQUEST['acacod']}') foo
                                       UNION
                                       SELECT
                                        'Pago'  AS descricao ,
                                        'Total' AS categoria ,
                                        pago    AS valor
                                       FROM
                                        (
                                       SELECT
                                          SUM( exe.vlrdotacaoatual ) AS dotacaoatual ,
                                          SUM( exe.vlrempenhado )    AS empenhado    ,
                                          SUM( exe.vlrliquidado )    AS liquidado    ,
                                          SUM( exe.vlrpago )         AS pago
                                       FROM
                                          monitora.acao aca
                                       LEFT JOIN
                                          spo.siopexecucao exe
                                       ON
                                          aca.unicod = exe.unicod
                                       AND
                                          aca.acacod = exe.acacod
                                       AND
                                          aca.prgano = exe.exercicio
                                       AND
                                          aca.loccod = exe.loccod
                                          --
                                          WHERE
                                        aca.acastatus ='A'
                                       AND
                                        aca.prgano = '{$_SESSION['exercicio']}'
                                       AND
                                        aca.unicod = '{$_REQUEST['unicod']}'
                                       AND aca.acacod = '{$_REQUEST['acacod']}'
                                      ) foo";

                        $grafico = new Grafico(null, false);
                        $grafico->setTipo(Grafico::K_TIPO_COLUNA)->setId('graficoLocalizador')->setTitulo('Execução Orçamentária')->gerarGrafico($sqlGrafico);
                        ?>
                    </div>
                </div>
            </div>
        </div>
    </div>

    <br/><div class="panel-group" id="accordion">
        <div class="panel panel-info">
            <div class="panel-heading" style="overflow: auto;">
                <h4 class="panel-title" >
                    <a data-toggle="collapse" style="" class="text-uppercase" data-parent="#accordion" href="#planosorcamentarios" >
                        Planos Orçamentários (POs)
                    </a>
                    <a style="float:right; cursor: pointer">
                        <span id="erro-planosorcamentarios" class="iconeBotao botaoErro glyphicon glyphicon-thumbs-down" style="color: #D79C2B; font-size: 16px; margin-right: 5px;" title="Reportar divergência em um valor neste quadro." onclick="abrir_modal(this.id);" ></span>
                    </a>
                </h4>
            </div>
            <div id="planosorcamentarios"class="collapse panel-collapse  " style="overflow-x: auto;">

                <?php
                $sql = <<<DML
                            SELECT DISTINCT
                             aca.loccod                                 ,
                             plo.plocodigo || ' - ' ||plo.plotitulo as po          ,
                             ptr.ptres                                  ,
                             plo.metafisica                             ,
                             plo.prddsc                                 ,
                             plo.unmdsc                                 ,
                             SUM( exe.vlrdotacaoatual ) AS dotacaoatual ,
                             SUM( exe.vlrempenhado )    AS empenhado    ,
                             SUM( exe.vlrliquidado )    AS liquidado    ,
                             SUM( exe.vlrpago )         AS pago
                            FROM
                             monitora.acao aca
                            JOIN
                             monitora.planoorcamentario plo
                            USING
                             ( acaid )
                            JOIN
                             monitora.ptres ptr
                            ON
                            ptr.acaid = plo.acaid
                           AND
                            ptr.plocod = plo.plocodigo
                           LEFT JOIN
                            spo.siopexecucao exe
                           ON
                            aca.unicod = exe.unicod
                           AND
                            aca.acacod = exe.acacod
                           AND
                            aca.prgano = exe.exercicio
                           AND
                            aca.loccod = exe.loccod
                           AND
                            plo.plocodigo = exe.plocod
                           WHERE
                            aca.acastatus ='A'
                           AND aca.prgano = '{$_SESSION['exercicio']}'
                           AND plo.exercicio  =  '{$_SESSION['exercicio']}'
                           AND ptr.ptrano  = '{$_SESSION['exercicio']}'
                           AND aca.unicod = '{$_REQUEST['unicod']}'
                           AND aca.acacod = '{$_REQUEST['acacod']}'
                        GROUP BY
                         1 ,
                         2 ,
                         3 ,
                         4 ,
                         5,
                           6
                        ORDER BY 1 , 2

DML;
                # ver($sql);
                $cabecalho = array(
                    'Localizador',
                    'Plano Orçamentário (PO)',
                    'PTRES',
                    'Meta Física',
                    'Produto',
                    'Unidade de Medida',
                    'Dotação Atual (R$)',
                    'Empenhado (R$)',
                    'Liquidado (R$)',
                    'Pago (R$)'
                );
#ver($sql);
                $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
                $listagem->setCabecalho($cabecalho);
                $listagem->addCallbackDeCampo(array('dotacaoatual', 'empenhado', 'liquidado', 'pago'), 'mascaraMoeda');
                $listagem->addCallbackDeCampo(array('po'), 'alinhaParaEsquerda');
                $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, array('dotacaoatual', 'empenhado', 'liquidado', 'pago'));
                $listagem->setFormOff();
                $listagem->setQuery($sql);
                $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
                ?>
            </div>
        </div>
    </div>


    <br/><div class="panel-group" id="accordion">
        <div class="panel panel-info">
            <div class="panel-heading" style="overflow: auto;">
                <h4 class="panel-title" >
                    <a data-toggle="collapse" style="" class="text-uppercase" data-parent="#accordion" href="#ptres" >
                        Planos de Trabalho Resumidos (PTRES)
                    </a>
                    <a style="float:right; cursor: pointer">
                        <span id="erro-ptres" class="iconeBotao botaoErro glyphicon glyphicon-thumbs-down" style="color: #D79C2B; font-size: 16px; margin-right: 5px;" title="Reportar divergência em um valor neste quadro." onclick="abrir_modal(this.id);" ></span>
                    </a>
                </h4>
            </div>
            <div id="ptres"class="collapse panel-collapse  " style="overflow-x: auto;">
                <?php
                $sql = <<<DML
                       SELECT DISTINCT
                        ptr.ptres               AS ptres            ,
                        ptr.ptrdotacao          AS dotacaoatual     ,
                        SUM( DISTINCT sbd.sadvalor )     AS detalhadosubacao ,
                        SUM( DISTINCT exe.vlrempenhado ) AS empenhado        ,
                        SUM( DISTINCT exe.vlrliquidado ) AS liquidado        ,
                        SUM( DISTINCT exe.vlrpago )      AS pago
                       FROM
                        monitora.acao aca
                       JOIN
                        monitora.ptres ptr
                       ON
                        ptr.acaid = aca.acaid
                       LEFT JOIN
                        monitora.pi_subacaodotacao sbd
                       USING
                        ( ptrid )
                       LEFT JOIN
                        spo.siopexecucao exe
                       ON
                        aca.unicod = exe.unicod
                       AND
                        aca.acacod = exe.acacod
                       AND
                        aca.prgano = exe.exercicio
                       AND
                        aca.loccod = exe.loccod
                       AND
                        ptr.ptres = exe.ptres
                       WHERE
                            aca.acastatus ='A'
                           AND
                            aca.prgano = '{$_SESSION['exercicio']}'
                           AND
                            aca.unicod = '{$_REQUEST['unicod']}'
                           AND aca.acacod = '{$_REQUEST['acacod']}'
                        GROUP BY
                         1 ,
                         2
                        ORDER BY 1

DML;

                $cabecalho = array(
                    'PTRES',
                    'Dotação Atual (R$)',
                    'Detalhado em Subações (R$)',
                    'Empenhado (R$)',
                    'Liquidado (R$)',
                    'Pago (R$)'
                );
                $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
                $listagem->setCabecalho($cabecalho);
                $listagem->addCallbackDeCampo(array('dotacaoatual', 'empenhado', 'liquidado', 'pago', 'detalhadosubacao'), 'mascaraMoeda');
                $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, array('dotacaoatual', 'empenhado', 'liquidado', 'pago', 'detalhadosubacao'));
                $listagem->setFormOff();
                $listagem->setQuery($sql);
                $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
                #var_dump($sql); die;
                ?>
            </div>
        </div>
    </div>


    <br/><div class="panel-group" id="accordion">
        <div class="panel panel-info">
            <div class="panel-heading" style="overflow: auto;">
                <h4 class="panel-title" >
                    <a data-toggle="collapse" style="" class="text-uppercase" data-parent="#accordion" href="#emendas" >
                        Emendas
                    </a>
                    <a style="float:right; cursor: pointer">
                        <span id="erro-emendas" class="iconeBotao botaoErro glyphicon glyphicon-thumbs-down" style="color: #D79C2B; font-size: 16px; margin-right: 5px;" title="Reportar divergência em um valor neste quadro." onclick="abrir_modal(this.id);"></span>
                    </a>
                </h4>
            </div>
            <div id="emendas" class="collapse panel-collapse  " style="overflow-x: auto;">
                <?php
                $sql = <<<DML
                            SELECT
                             vede.emecod                                                                                ,
                             ( CASE WHEN vede.emetipo = 'E' THEN 'Emenda' ELSE 'Complemento' END ) AS tipoemenda        ,
                             aut.autnome                                                                                ,
                             vede.foncod||' - '||fon.fondsc AS foncod                                                    ,
                             ( SUM( per.pervalor ) - COALESCE( ei.edivalor , 0 ) ) AS vrlliberado                        ,
                             SUM( exf.exfvalor )                                   AS vrlempenhado                       ,
                             SUM( ord.orbvalorparcela )                            AS vrlpagamento
                            FROM
                             emenda.ptiniciativa pti
                            INNER JOIN
                             emenda.ptiniciativaespecificacao pte
                            ON
                             pte.ptiid = pti.ptiid
                            AND
                             pte.ptestatus = 'A'
                            INNER JOIN
                             emenda.ptemendadetalheentidade ped
                            ON
                             ped.ptrid = pti.ptrid
                            INNER JOIN
                             emenda.tipoensino tpe
                            ON
                             pti.tpeid = tpe.tpeid
                            INNER JOIN
                             emenda.v_emendadetalheentidade vede
                            ON
                             vede.edeid = ped.edeid
                            INNER JOIN
                             emenda.autor aut
                            ON
                             aut.autid = vede.autid
                            INNER JOIN
                             emenda.ptiniesprecurso per
                            ON
                             per.pteid = pte.pteid
                            AND
                             per.pedid = ped.pedid
                            LEFT JOIN
                             emenda.v_funcionalprogramatica fup
                            ON
                             fup.acaid = vede.acaid
                            LEFT JOIN
                             public.gnd gn
                            ON
                             gn.gndcod = vede.gndcod
                            AND
                             gn.gndstatus = 'A'
                            LEFT JOIN
                             public.modalidadeaplicacao map
                            ON
                             map.mapcod = vede.mapcod
                            LEFT JOIN
                             public.fonterecurso fon
                            ON
                             fon.foncod = vede.foncod
                            AND
                             fon.fonstatus = 'A'
                            LEFT JOIN
                             emenda.emendadetalheimpositivo ei
                            ON
                             ei.emdid = vede.emdid
                            AND
                             ei.edistatus = 'A'
                            LEFT JOIN
                             emenda.execucaofinanceira exf
                            ON
                             exf.ptrid = pti.ptrid
                            AND
                             exf.pedid = ped.pedid
                            AND
                             exf.exfstatus = 'A'
                            LEFT JOIN
                             emenda.ordembancaria ord
                            ON
                             ord.exfid = exf.exfid

                            WHERE
                            fup.prgano ='{$_SESSION['exercicio']}'
                             AND fup.unicod = '{$_REQUEST['unicod']}'
                             AND fup.acacod = '{$_REQUEST['acacod']}'
                            GROUP BY
                             tpe.tpedsc                   ,
                             tpe.tpeid                    ,
                             vede.emecod                  ,
                             aut.autnome                  ,
                             vede.gndcod                  ,
                             vede.mapcod                  ,
                             vede.foncod                  ,
                             fup.fupfuncionalprogramatica ,
                             vede.emetipo                 ,
                             gn.gnddsc                    ,
                             map.mapdsc                   ,
                             fon.fondsc                   ,
                             vede.emdliberado             ,
                             vede.acaid                   ,
                             ei.edivalor
                            ORDER BY
                             vede.gndcod                  ,
                             fup.fupfuncionalprogramatica ,
                             vede.emecod                  ,
                             aut.autnome                  ,
                             vede.acaid
DML;

                $cabecalho = array(
                    'Emenda',
                    'Tipo',
                    'Autor(a)',
                    'Fonte',
                    'Valor Liberado (R$)',
                    'Empenhado (R$)',
                    'Pago (R$)'
                );
                $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
                $listagem->setCabecalho($cabecalho);
                $listagem->addCallbackDeCampo(array('vrlliberado', 'vrlempenhado', 'vrlpagamento'), 'mascaraMoeda');
                $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, array('vrlliberado', 'vrlempenhado', 'vrlpagamento'));
                $listagem->setFormOff();
                $listagem->setQuery($sql);
                $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
                #var_dump($sql); die;
                ?>
            </div>
        </div>
    </div>

    <br/><div class="panel-group" id="accordion">
        <div class="panel panel-info">
            <div class="panel-heading" style="overflow: auto;">
                <h4 class="panel-title" >
                    <a data-toggle="collapse" style="" class="text-uppercase" data-parent="#accordion" href="#subacoes" >
                        Subações
                    </a>
                    <a style="float:right; cursor: pointer">
                        <span id="erro-subacoes" class="iconeBotao botaoErro glyphicon glyphicon-thumbs-down" style="color: #D79C2B; font-size: 16px; margin-right: 5px;" title="Reportar divergência em um valor neste quadro." onclick="abrir_modal(this.id);"></span>
                    </a>
                </h4>
            </div>
            <div id="subacoes"class="collapse panel-collapse  " style="overflow-x: auto;">
                <?php
                $sql = <<<DML
                      SELECT DISTINCT
                        sba.sbacod              AS sbacod           ,
                        (SELECT SUM(sbd.sadvalor) FROM monitora.pi_subacaodotacao WHERE sbaid = sba.sbaid LIMIT 1) AS detalhadosubacaotoal,
                        SUM( sbd.sadvalor )     AS detalhadosubacao ,
                        SUM( exe.vlrempenhado ) AS empenhado        ,
                        SUM( exe.vlrliquidado ) AS liquidado        ,
                        SUM( exe.vlrpago )      AS pago
                       FROM
                        monitora.acao aca
                       JOIN
                        monitora.ptres ptr
                       ON
                        ptr.acaid = aca.acaid
                       LEFT JOIN
                        monitora.pi_subacaodotacao sbd
                       USING
                        ( ptrid )
                       LEFT JOIN
                        monitora.pi_subacao sba
                       USING
                        ( sbaid )
                       LEFT JOIN
                        spo.siopexecucao exe
                       ON
                        aca.unicod = exe.unicod
                       AND
                        aca.acacod = exe.acacod
                       AND
                        aca.prgano = exe.exercicio
                       AND
                        aca.loccod = exe.loccod
                       AND
                        sba.sbacod = SUBSTR( exe.plicod , 2 , 4 )
                       WHERE
                            aca.acastatus ='A'
                           AND aca.prgano = '{$_SESSION['exercicio']}'
                           AND sba.sbaano = '{$_SESSION['exercicio']}'
                           AND aca.unicod = '{$_REQUEST['unicod']}'
                           AND aca.acacod = '{$_REQUEST['acacod']}'
                        GROUP BY
                         sba.sbacod , sba.sbaid
                        ORDER BY 1

DML;
                $cabecalho = array(
                    'Subação',
                    'Detalhado total da Subação (R$)',
                    'Detalhado da Ação na Subação (R$)',
                    'Empenhado (R$)',
                    'Liquidado (R$)',
                    'Pago (R$)'
                );
                $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
                $listagem->setCabecalho($cabecalho);
                $listagem->addCallbackDeCampo(array('dotacaoatual', 'empenhado', 'liquidado', 'pago', 'detalhadosubacao', 'detalhadosubacaotoal'), 'mascaraMoeda');
                $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, array('dotacaoatual', 'empenhado', 'liquidado', 'pago', 'detalhadosubacao', 'detalhadosubacaotoal'));
                $listagem->setFormOff();
                $listagem->setQuery($sql);
                $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
                ?>
            </div>
        </div>
    </div>


    <br/><div class="panel-group" id="accordion">
        <div class="panel panel-info">
            <div class="panel-heading" style="overflow: auto;">
                <h4 class="panel-title" >
                    <a data-toggle="collapse" style="" class="text-uppercase" data-parent="#accordion" href="#pis" >
                        Planos Internos (PIs)
                    </a>
                    <a style="float:right; cursor: pointer">
                        <span id="erro-pis" class="iconeBotao botaoErro glyphicon glyphicon-thumbs-down" style="color: #D79C2B; font-size: 16px; margin-right: 5px;" title="Reportar divergência em um valor neste quadro." onclick="abrir_modal(this.id);" ></span>
                    </a>
                </h4>
            </div>
            <div id="pis"class="collapse panel-collapse  " style="overflow-x: auto;">
                <?php
                $sql = <<<DML
               SELECT DISTINCT
                    pli.plicod                        AS plicod    ,
                    ptr.unicod || ' - ' || uni.unidsc AS unidade   ,
                    SUM( exe.vlrempenhado )           AS empenhado ,
                    SUM( exe.vlrliquidado )           AS liquidado ,
                    SUM( exe.vlrpago )                AS pago
                   FROM
                    monitora.acao aca
                   JOIN
                    monitora.ptres ptr
                   ON
                    ptr.acaid = aca.acaid
                   JOIN
                    monitora.pi_planointernoptres pip
                   ON
                    pip.ptrid = ptr.ptrid
                   JOIN
                    monitora.pi_planointerno pli
                   ON
                    pip.pliid = pli.pliid
                   JOIN
                    public.unidade uni
                   ON
                    pli.unicod = uni.unicod
                   LEFT JOIN
                    spo.siopexecucao exe
                   ON
                    aca.unicod = exe.unicod
                   AND
                    aca.acacod = exe.acacod
                   AND
                    aca.prgano = exe.exercicio
                   AND
                    pli.plicod = exe.plicod
                   WHERE
                           aca.acastatus ='A'
                           AND aca.prgano = '{$_SESSION['exercicio']}'
                           AND aca.unicod = '{$_REQUEST['unicod']}'
                           AND aca.acacod = '{$_REQUEST['acacod']}'
                        GROUP BY
                         1,2
                        ORDER BY 1

DML;
                $cabecalho = array(
                    'Plano Interno (PI)',
                    'Unidade Orçamentária (UO)',
                    'Empenhado (R$)',
                    'Liquidado (R$)',
                    'Pago (R$)'
                );
                $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
                $listagem->setCabecalho($cabecalho);
                $listagem->addCallbackDeCampo(array('empenhado', 'liquidado', 'pago'), 'mascaraMoeda');
                $listagem->addCallbackDeCampo(array('unidade'), 'alinhaParaEsquerda');
                $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, array('empenhado', 'liquidado', 'pago'));
                $listagem->setFormOff();
                $listagem->setQuery($sql);
                $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
                ?>
            </div>
        </div>
    </div>
    <br/><div class="panel-group" id="accordion">
        <div class="panel panel-info">
            <div class="panel-heading" style="overflow: auto;">
                <h4 class="panel-title" >
                    <a data-toggle="collapse" style="" class="text-uppercase" data-parent="#accordion" href="#acompanhamento" >
                        Acompanhamentos Orçamentários (SIOP  <?php echo $_SESSION['exercicio']; ?>)
                    </a>
                    <a style="float:right; cursor: pointer">
                        <span id="erro-acompanhamento" class="iconeBotao botaoErro glyphicon glyphicon-thumbs-down" style="color: #D79C2B; font-size: 16px; margin-right: 5px;" title="Reportar divergência em um valor neste quadro." onclick="abrir_modal(this.id);"></span>
                    </a>
                </h4>
            </div>
            <div id="acompanhamento"class="collapse panel-collapse  " style="overflow-x: auto;">
                <?php
                $sql = <<<DML
                  SELECT
                    prf.prftitulo ,
                    snl.loccod    ,
                    0, /*( ssp.dotacaoatual / snl.metafisica ) /( snl.liquidado / acl.fisicoexecutado ) AS efloa    , */
                    esd.esddsc                                                                     AS situacao ,
                    COALESCE(COUNT( ssp.sspid ),0)                                                 AS pos
                   FROM
                    acomporc.snapshotlocalizador snl
                   JOIN
                    acomporc.periodoreferencia prf
                   ON
                    snl.prfid = prf.prfid
                   LEFT JOIN
                    acomporc.acompanhamentolocalizador acl
                   ON
                    snl.acaid = acl.acaid
                   LEFT JOIN
                    acomporc.snapshotplanoorcamentario ssp
                   ON
                    ssp.acaid = snl.acaid
                   LEFT JOIN
                    workflow.documento doc
                   ON
                    acl.docid = doc.docid
                   LEFT JOIN
                    workflow.estadodocumento esd
                   ON
                    esd.esdid = doc.esdid
                   WHERE
                    snl.unicod = '{$_REQUEST['unicod']}'
                   AND
                    snl.acacod = '{$_REQUEST['acacod']}'
                   AND
                    prf.prsano = '{$_SESSION['exercicio']}'
                   GROUP BY
                    1 ,
                    2 ,
                    3 ,
                    4
                   ORDER BY
                    1 ,
                    2
DML;
                $cabecalho = array(
                    'Período',
                    'Localizador',
                    'EFLOA do Localizador (%)',
                    'Situação',
                    'PO(s)'
                );
                $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
                $listagem->setCabecalho($cabecalho);
                $listagem->addCallbackDeCampo(array('efloa'), 'mascaraMoeda');
                $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, array('efloa', 'pos'));
                $listagem->setFormOff();
                $listagem->setQuery($sql);
                $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
                ?>
            </div>
        </div>
    </div>
    <br/><div class="panel-group" id="accordion">
        <div class="panel panel-info">
            <div class="panel-heading" style="overflow: auto;">
                <h4 class="panel-title" >
                    <a data-toggle="collapse" style="" class="text-uppercase" data-parent="#accordion" href="#alteracoes" >
                        Alterações Orçamentárias (<?php echo $_SESSION['exercicio']; ?>)
                    </a>
                    <a style="float:right; cursor: pointer">
                        <span id="erro-alteracoes" class="iconeBotao botaoErro glyphicon glyphicon-thumbs-down" style="color: #D79C2B; font-size: 16px; margin-right: 5px;" title="Reportar divergência em um valor neste quadro." onclick="abrir_modal(this.id);" ></span>
                    </a>
                </h4>
            </div>
            <div id="alteracoes"class="collapse panel-collapse  " style="overflow-x: auto;">
                <?php
                $sql = <<<DML
                       SELECT
                        mcr.mcrdsc                                               AS momento       ,
                        aca.loccod || ' - '|| aca.sacdsc                         AS localizador   ,
                        SUM( pas.valsuplementacao )                              AS suplementacao ,
                        SUM( pas.valcancelamento )                               AS cancelamento  ,
                        SUM( pas.valsuplementacao ) - SUM( pas.valcancelamento ) AS diferenca
                       FROM
                        altorc.snapshotacao saca
                       JOIN
                        altorc.snapshotplanoorcamentario spo
                       ON
                        spo.snaid = saca.snaid
                       JOIN
                        altorc.momentocredito mcr
                       ON
                        mcr.mcrid = saca.mcrid
                       JOIN
                        altorc.pedidoalteracaofinanceiro pas
                       ON
                        pas.spoid = spo.spoid
                       JOIN
                        monitora.acao aca
                       ON
                        aca.unicod = saca.unicod
                       AND
                        aca.acacod = saca.acacod
                       AND
                        aca.loccod = saca.loccod
                       WHERE
                        aca.prgano = '{$_SESSION['exercicio']}'
                       AND
                        aca.unicod = '{$_REQUEST['unicod']}'
                       AND
                        aca.acacod = '{$_REQUEST['acacod']}'
                        AND saca.snaexercicio = '{$_SESSION['exercicio']}'
                       GROUP BY
                        1 ,
                        2
                       ORDER BY
                        1 ,
                        2

DML;
                $cabecalho = array(
                    'Momento de Créditos',
                    'Localizador',
                    'Suplementação (R$)',
                    'Cancelamento (R$)',
                    'Diferença (R$)'
                );
#ver($sql);
                $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
                $listagem->setCabecalho($cabecalho);
                $listagem->addCallbackDeCampo(array('suplementacao', 'cancelamento', 'diferenca'), 'mascaraMoeda');
                $listagem->addCallbackDeCampo(array('localizador', 'momento'), 'alinhaParaEsquerda');
                $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, array('suplementacao', 'cancelamento', 'diferenca'));
                $listagem->setFormOff();
                $listagem->setQuery($sql);
                $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
                ?>
            </div>
        </div>
    </div>

    <br/><div class="panel-group" id="accordion">
        <div class="panel panel-info">
            <div class="panel-heading" style="overflow: auto;">
                <h4 class="panel-title" >
                    <a data-toggle="collapse" style="" class="text-uppercase" data-parent="#accordion" href="#ploa" >
                        PLOA
                    </a>
                    <a style="float:right; cursor: pointer">
                        <span id="erro-ploa" class="iconeBotao botaoErro glyphicon glyphicon-thumbs-down" style="color: #D79C2B; font-size: 16px; margin-right: 5px;" title="Reportar divergência em um valor neste quadro." onclick="abrir_modal(this.id);" ></span>
                    </a>
                </h4>
            </div>
            <div id="ploa"class="collapse panel-collapse  " style="overflow-x: auto;">
                <div style="width: 48%; float:left">
                    <table class="table table-striped table-bordered table-hover">
                        <tbody>
                            <tr>
                                <td class="text-center text-info">
                                    <b>PLOA <?php echo $_SESSION['exercicio'] ?></b>
                                </td>
                            </tr>

                            <tr>
                                <td>
                                    <?php
                                    /*
                                     * PLOA ANO ATUAL
                                     */
                                    $anoPloaAtual = $_SESSION['exercicio'] - 1;
                                    $sql = <<<DML
                                    SELECT
                                         aca.loccod || ' - '|| aca.sacdsc        AS localizador,
                                         plo.plocodigo || ' - ' || plo.plotitulo AS po ,
                                         dpa.foncod                                    ,
                                         SUM(COALESCE( plf.plfvalor , dpa.dpavalor )) AS valor
                                        FROM
                                         elabrev.despesaacao dpa
                                        JOIN
                                         elabrev.ppaacao_orcamento pao
                                        USING
                                         ( acaid )
                                        JOIN
                                         monitora.acao aca
                                        ON
                                         pao.unicod = aca.unicod
                                        AND
                                         pao.acacod = aca.acacod
                                        AND
                                         pao.loccod = aca.loccod
                                        AND
                                         pao.prgano = aca.prgano
                                        INNER JOIN
                                         elabrev.planoorcamentario plo
                                        ON
                                         dpa.ploid = plo.ploid
                                        AND
                                         plo.plostatus = 'A'
                                        AND
                                         dpa.acaid = plo.acaid
                                        LEFT JOIN
                                         proporc.ploafinanceiro plf
                                        ON
                                         plf.dpaid = dpa.dpaid
                                        WHERE
                                         aca.acastatus ='A'
                                        AND
                                         aca.prgano = '{$anoPloaAtual}'
                                        AND
                                         aca.unicod = '{$_REQUEST['unicod']}'
                                        AND aca.acacod = '{$_REQUEST['acacod']}'
                                     GROUP BY
                                      1,2,3
                                     ORDER BY 1

DML;
                                    $cabecalho = array(
                                        'Localizador ',
                                        'Plano Orçamentário (PO)',
                                        'Fonte',
                                        'Valor (R$)'
                                    );
                                    $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
                                    $listagem->setCabecalho($cabecalho);
                                    $listagem->addCallbackDeCampo(array('valor'), 'mascaraMoeda');
                                    $listagem->addCallbackDeCampo(array('localizador', 'po'), 'alinhaParaEsquerda');
                                    $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, array('valor'));
                                    $listagem->setFormOff();
                                    $listagem->setQuery($sql);
                                    $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
                                    ?>
                                </td>
                            </tr>
                        </tbody>
                    </table>
                </div>
                <div style="width: 2%; float:left">
                    <p></p>
                </div>
                <div style="width: 48%; float:left">
                    <table class="table table-striped table-bordered table-hover">
                        <tbody>
                            <tr>
                                <td class="text-center text-info">
                                    <b>PLOA <?php echo $_SESSION['exercicio'] + 1 ?></b>
                                </td>
                            </tr>

                            <tr>
                                <td>
                                    <?php
                                    /*
                                     * PLOA PRÓXIMO ANO
                                     */
                                    $sql = <<<DML
                                    SELECT
                                         aca.loccod || ' - '|| aca.sacdsc        AS localizador,
                                         plo.plocodigo || ' - ' || plo.plotitulo AS po ,
                                         dpa.foncod                                    ,
                                         SUM(COALESCE( plf.plfvalor , dpa.dpavalor )) AS valor
                                        FROM
                                         elabrev.despesaacao dpa
                                        JOIN
                                         elabrev.ppaacao_orcamento pao
                                        USING
                                         ( acaid )
                                        JOIN
                                         monitora.acao aca
                                        ON
                                         pao.unicod = aca.unicod
                                        AND
                                         pao.acacod = aca.acacod
                                        AND
                                         pao.loccod = aca.loccod
                                        AND
                                         pao.prgano = aca.prgano
                                        INNER JOIN
                                         elabrev.planoorcamentario plo
                                        ON
                                         dpa.ploid = plo.ploid
                                        AND
                                         plo.plostatus = 'A'
                                        AND
                                         dpa.acaid = plo.acaid
                                        LEFT JOIN
                                         proporc.ploafinanceiro plf
                                        ON
                                         plf.dpaid = dpa.dpaid
                                        WHERE
                                         aca.acastatus ='A'
                                        AND
                                         aca.prgano = '{$_SESSION['exercicio']}'
                                        AND
                                         aca.unicod = '{$_REQUEST['unicod']}'
                                        AND aca.acacod = '{$_REQUEST['acacod']}'
                                     GROUP BY
                                      1,2,3
                                     ORDER BY 1

DML;
                                    $cabecalho = array(
                                        'Localizador ',
                                        'Plano Orçamentário (PO)',
                                        'Fonte',
                                        'Valor (R$)'
                                    );
                                    $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
                                    $listagem->setCabecalho($cabecalho);
                                    $listagem->addCallbackDeCampo(array('valor'), 'mascaraMoeda');
                                    $listagem->addCallbackDeCampo(array('localizador', 'po'), 'alinhaParaEsquerda');
                                    $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, array('valor'));
                                    $listagem->setFormOff();
                                    $listagem->setQuery($sql);
                                    $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
                                    ?>
                                </td>
                            </tr>
                        </tbody>
                    </table>
                </div>
            </div>
        </div>
    </div>
    <br/>
    <div class="panel-group" id="accordion">
        <div class="panel panel-info">
            <div class="panel-heading" style="overflow: auto;">
                <h4 class="panel-title" >
                    <a data-toggle="collapse" style="" class="text-uppercase" data-parent="#accordion" href="#progf" >
                        Solicitações de Programação Financeira (PF)
                    </a>
                    <a style="float:right; cursor: pointer">
                        <span id="erro-progf" class="iconeBotao botaoErro glyphicon glyphicon-thumbs-down" style="color: #D79C2B; font-size: 16px; margin-right: 5px;" title="Reportar divergência em um valor neste quadro." onclick="abrir_modal(this.id);" ></span>
                    </a>
                </h4>
            </div>
            <div id="progf"class="collapse panel-collapse  " style="overflow-x: auto;">
                <?php
                $sql = <<<DML
                     SELECT
                        lpad( plf.plfid||'' , 6 , '0' ) AS pedido        ,
                        clp.clpdsc                      AS classificacao ,
                        esd.esddsc                      AS estado        ,
                        SUM( lfn.lfnvalorsolicitado )   AS solicitado    ,
                        SUM( lfn.lfnvalorautorizado )   AS autorizado    ,
                        SUM( lfn.lfnvaloratendido )     AS atendido
                        FROM
                         progfin.pedidoliberacaofinanceira plf
                        JOIN
                         progfin.liberacoesfinanceiras lfn
                        USING
                         ( plfid )
                        JOIN
                         progfin.classificacaopedido clp
                        USING
                         ( clpid )
                        JOIN
                         workflow.documento doc
                        ON
                         lfn.docid = doc.docid
                        JOIN
                         workflow.estadodocumento esd
                        ON
                         doc.esdid = esd.esdid
                        WHERE
                        COALESCE(lfn.acacod, lfn.acacod_2) = '{$_REQUEST['acacod']}'
                       AND
                        plf.unicod = '{$_REQUEST['unicod']}'
                       AND
                        TO_CHAR( plf.dataultimaatualizacao , 'YYYY' ) = '{$_SESSION['exercicio']}'
                       GROUP BY
                        1 ,
                        2 ,
                        3
                       ORDER BY
                        1 DESC

DML;
                $cabecalho = array(
                    'Pedido',
                    'Classificação',
                    'Situação',
                    'Solicitado (R$)',
                    'Aprovado (R$)',
                    'Atendido (R$)'
                );
#ver($sql);
                $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
                $listagem->setCabecalho($cabecalho);
                $listagem->addCallbackDeCampo(array('solicitado', 'autorizado', 'atendido'), 'mascaraMoeda');
                $listagem->addCallbackDeCampo(array('classificacao'), 'alinhaParaEsquerda');
                $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, array('solicitado', 'autorizado', 'atendido'));
                $listagem->setFormOff();
                $listagem->setQuery($sql);
                $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
                ?>
            </div>
        </div>
    </div>
    <br/>
    <div class="panel-group" id="accordion">
        <div class="panel panel-warning ">
            <div class="panel-heading" style="overflow: auto;">
                <h4 class="panel-title" >
                    <a data-toggle="collapse" style="" class="text-uppercase" data-parent="#accordion" href="#ted" >
                        Termos de Execução Descentralizada (TED)
                    </a>
                    <a style="float:right; cursor: pointer">
                        <span id="erro-ted" class="iconeBotao botaoErro glyphicon glyphicon-thumbs-down" style="color: #D79C2B; font-size: 16px; margin-right: 5px;" title="Reportar divergência em um valor neste quadro." onclick="abrir_modal(this.id);" ></span>
                    </a>
                </h4>
            </div>
            <div id="ted"class="collapse panel-collapse  " style="overflow-x: auto;">
                <?php
                $sql = <<<DML
                   SELECT
                    ppr.tcpid ,
                    ppr.tcpid                                                                                    AS termo ,
                    unipro.unicod || ' - ' ||unipro.unidsc ||' / '||ted.ungcodproponente || ' - ' ||ugpro.ungdsc AS proponente ,
                    unicon.unicod || ' - ' ||unicon.unidsc ||' / '||ted.ungcodconcedente || ' - ' ||ugcon.ungdsc AS concedente ,
                    SUM( provalor )                                                                              AS total
                   FROM
                    ted.previsaoorcamentaria ppr
                   JOIN
                    ted.termocompromisso ted
                   ON
                    ted.tcpid = ppr.tcpid
                   JOIN
                    public.unidadegestora ugpro
                   ON
                    ted.ungcodproponente = ugpro.ungcod
                   JOIN
                    public.unidade unipro
                   ON
                    ugpro.unicod = unipro.unicod
                   JOIN
                    public.unidadegestora ugcon
                   ON
                    ted.ungcodconcedente = ugcon.ungcod
                   JOIN
                    public.unidade unicon
                   ON
                    ugcon.unicod = unicon.unicod
                   JOIN
                    monitora.ptres ptr
                   ON
                    ptr.ptrid = ppr.ptrid
                   JOIN
                    monitora.acao aca
                   ON
                    ptr.acaid = aca.acaid
                    WHERE
                    ppr.proanoreferencia = '{$_SESSION['exercicio']}'
                   AND
                    aca.acastatus = 'A'
                   AND ted.tcpstatus = 'A'
                   AND
                     aca.prgano = '{$_SESSION['exercicio']}'
                    AND
                    aca.unicod = '{$_REQUEST['unicod']}'
                    AND aca.acacod = '{$_REQUEST['acacod']}'
                   GROUP BY
                    1 ,
                    2 ,
                    3 ,
                    4
                   ORDER BY
                    1

DML;
                $cabecalho = array(
                    'TED',
                    'Proponente',
                    'Concedente',
                    "Total para {$_SESSION['exercicio']} (R$)",
                );
                $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
                $listagem->setCabecalho($cabecalho);
                $listagem->addAcao('view', 'abrirTED');
                $listagem->addCallbackDeCampo(array('total'), 'mascaraMoeda');
                $listagem->addCallbackDeCampo(array('proponente', 'concedente'), 'alinhaParaEsquerda');
                $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, array('total'));
                $listagem->setFormOff();
                $listagem->setQuery($sql);
                $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
                ?>
            </div>
        </div>
    </div>

    <br/>
    <div class="panel-group" id="accordion">
        <div class="panel panel-warning" >
            <div class="panel-heading" style="overflow: auto;">
                <h4 class="panel-title" >
                    <a data-toggle="collapse" style="" class="text-uppercase" data-parent="#accordion" href="#obras" >
                        Obras
                    </a>
                    <a style="float:right; cursor: pointer">
                        <span id="erro-obras" class="iconeBotao botaoErro glyphicon glyphicon-thumbs-down" style="color: #D79C2B; font-size: 16px; margin-right: 5px;" title="Reportar divergência em um valor neste quadro." onclick="abrir_modal(this.id);" ></span>
                    </a>
                </h4>
            </div>
            <div id="obras"class="collapse panel-collapse  " style="overflow-x: auto;">
                <?php
                $sql = <<<DML
                        SELECT
                        obr.obrid,
                        obr.obrid as obra,
                        obr.obrdesc,
                        COALESCE(TO_CHAR(obr.obrdtinicio,'dd/mm/yyyy'), '-') as inicio,
                        pli.plicod,
                        pli.plidsc,
                        SUM(exe.vlrempenhado) AS empenhado,
                        SUM(exe.vlrliquidado) AS liquidado,
                        SUM(exe.vlrpago)      AS pago
                    FROM
                        obras.obrainfraestrutura obr
                    JOIN
                        monitora.pi_obra pio
                    USING
                        (obrid)
                    JOIN
                        monitora.pi_planointerno pli
                    USING
                        (pliid)
                    JOIN
                        monitora.pi_planointernoptres pip
                    USING
                        (pliid)
                    JOIN
                        monitora.ptres ptr
                    USING
                        (ptrid)
                    JOIN
                        monitora.acao aca
                    USING
                        (acaid)
                    LEFT JOIN
                        spo.siopexecucao exe
                    ON
                        pli.unicod = exe.unicod
                    AND pli.plicod = exe.plicod
                    AND pli.pliano = exe.exercicio
                    WHERE
                        pli.plistatus = 'A'
                    AND (TO_CHAR(obr.obrdtinicio,'yyyy') =  '{$_SESSION['exercicio']}' OR obr.obrdtinicio IS NULL)
                    AND pli.pliano = '{$_SESSION['exercicio']}'
                    AND aca.acacod = '{$_REQUEST['acacod']}'
                    AND aca.unicod = '{$_REQUEST['unicod']}'
                    GROUP BY
                        1, 2, 3, 4, 5, 6
                    ORDER BY
                        1 DESC

DML;
                $cabecalho = array(
                    'Obra',
                    'Descrição',
                    'Início',
                    'PI Vinculado',
                    'Descrição do PI',
                    'Empenhado (R$)',
                    'Liquidado (R$)',
                    'Pago (R$)'
                );
                $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
                $listagem->setCabecalho($cabecalho);
                $listagem->addAcao('view', 'abrirObra');
                $listagem->addCallbackDeCampo(array('empenhado', 'liquidado', 'pago'), 'mascaraMoeda');
                $listagem->addCallbackDeCampo(array('obrdesc', 'plidsc'), 'alinhaParaEsquerda');
                $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, array('empenhado', 'liquidado', 'pago'));
                $listagem->setFormOff();
                $listagem->setQuery($sql);
                $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
                ?>
            </div>
        </div>
    </div>
<br/>
    <div class="panel-group" id="accordion">
        <div class="panel panel-warning" >
            <div class="panel-heading" style="overflow: auto;">
                <h4 class="panel-title" >
                    <a data-toggle="collapse" style="" class="text-uppercase" data-parent="#accordion" href="#contratos" >
                        Contratos
                    </a>
                    <a style="float:right; cursor: pointer">
                        <span id="erro-contratos" class="iconeBotao botaoErro glyphicon glyphicon-thumbs-down" style="color: #D79C2B; font-size: 16px; margin-right: 5px;" title="Reportar divergência em um valor neste quadro." onclick="abrir_modal(this.id);" ></span>
                    </a>
                </h4>
            </div>
            <div id="contratos"class="collapse panel-collapse  " style="overflow-x: auto;">
                <?php
                $sql = <<<DML
                        SELECT NULL where FALSE

DML;
                $cabecalho = array(
                    'Contrato',
                    'Descrição',
                    'Início / Fim',
                    'PI Vinculado',
                    'Descrição do PI',
                    'Empenhado (R$)',
                    'Liquidado (R$)',
                    'Pago (R$)'
                );
                $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
                $listagem->setCabecalho($cabecalho);
                $listagem->addAcao('view', 'abrirObra');
                $listagem->addCallbackDeCampo(array('empenhado', 'liquidado', 'pago'), 'mascaraMoeda');
                $listagem->addCallbackDeCampo(array('obrdesc', 'plidsc'), 'alinhaParaEsquerda');
                $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, array('empenhado', 'liquidado', 'pago'));
                $listagem->setFormOff();
                $listagem->setQuery($sql);
                $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
                ?>
            </div>
        </div>
    </div>

</section>
<!--</form>-->