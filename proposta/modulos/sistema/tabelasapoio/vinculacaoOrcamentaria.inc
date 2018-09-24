<?php
if ($_REQUEST['popup']) {
    echo popup($_REQUEST['popup']);
    die();
}
set_time_limit(60 * 90);

include '_funcoesVinculacaoOrcamentaria.php';
include 'vinculacaoOrcamentariaAcao.inc';

function popup($vacid) {
    global $db;
    $html = '<style>.modal-dialog{width:80%;};</style><table class="table table-striped table-bordered table-hover" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"><tr><th width="150">&nbsp;</th>';
    $labels = array('Despesas Empenhadas:', 'Valores Pagos:', 'RAP não-Proc. Pagos:', 'RAP Processados Pagos:');
    $keys = array('empenho', 'pagamento', 'RapNPPago', 'rp_processado_pago');
    $years = array('2011', '2012', '2013', '2014');
    $acao = $subacao = $pis = $pos = $totalH = $totalL = $resultSet = array();
    if (!empty($vacid)) {
        foreach ($years as $year) {
            $strSQL = "SELECT v.vacid, vaeid, v.acaid, exercicio, v.vaetituloorcamentario, ve.vaedescricao
                FROM planacomorc.vinculacaoacaoestrategicaexercicio ve join planacomorc.vinculacaoacaoestrategica v using(vacid)
                WHERE exercicio = {$year} and vacid = {$vacid}";
            $resultSet[] = $db->pegaLinha($strSQL);
        }
    }

    foreach ($resultSet as $k => $result) {
        if (!empty($result['vaeid'])) {
            array_push($acao, resultado_soma_acoes($result['vaeid'], $years[$k]));
            array_push($subacao, carrega_soma_subacoes($result['vaeid'], $years[$k]));
            array_push($pis, carrega_soma_pi($result['vaeid'], $years[$k]));
            array_push($pos, carrega_soma_ptres($result['vaeid'], $years[$k]));
        }
    }

    foreach ($keys as $k) {
        foreach ($years as $i => $year) {
            $totalH[$k][$year] = $acao[$i]['total'][$k] + $subacao[$i]['total'][$k] + $pis[$i]['total'][$k] + $pos[$i]['total'][$k];
        }
    }

    $i = 0;

    foreach ($years as $k => $year) :
        $html .= '<th width="150" title="' . $resultSet[$k]['vaedescricao'] . '">' . $year . '</th>';
    endforeach;
    $html .= '</tr>';
    foreach ($totalH as $columnArray) :
        $html .= '<tr><td width = "150"><strong>' . $labels[$i] . '</strong></td>';
        $k = 0;
        foreach ($columnArray as $column) :
            $html .= '<td align = "center" title = "' . $resultSet[$k]['vaedescricao'] . '">' . numbFormat($column) . '</td>';
            $k++;
        endforeach;
        $html .= '</tr>';
        $i++;
    endforeach;
    $html .= '</table>';
    return $html;
}

$fm = new Simec_Helper_FlashMessage('planacomorc/tc/vinculacaoOrcamentaria');
$urlCall = 'planacomorc.php?modulo=sistema/tabelasapoio/vinculacaoOrcamentaria&acao=A';

function numbFormat($value) {
    return number_format($value, 2, ',', '.');
}

if ($_REQUEST['execucao'] == 'excluir') {
    $sql_del = "DELETE FROM planacomorc.vinculacaoacaoestrategica WHERE vacid = {$_REQUEST['registro']}";
    //ver($sql_del,d);
    if ($db->executar($sql_del)) {
        $db->commit();
    }
    $fm->addMensagem(
        'Registro excluido com sucesso.', Simec_Helper_FlashMessage::SUCESSO
    );
}

include APPRAIZ . 'includes/cabecalho.inc';
?>

<link rel="stylesheet" type="text/css" href="../planacomorc/css/vincOrcamentaria.css" />
<div class="row col-md-12">
    <ol class="breadcrumb">
        <li><a href="<?php echo MODULO; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li>Tabelas de Apoio</li>
        <li class="active">Vinculação Orçamentária da Ação Estratégica</li>
    </ol>
    <?php
    $strSqlGrid = "
        SELECT
            vacid,
            v.vaetituloorcamentario, a.acadsc
        FROM planacomorc.vinculacaoacaoestrategica v
        --JOIN planacomorc.vinculacaoacaoestrategicaexercicio ve on (ve.vacid=v.vacid)
        JOIN painel.acao a ON (a.acaid=v.acaid)
        ORDER BY v.vacid DESC";

    echo $fm->getMensagens();
    $listagem = new Simec_Listagem();
    $listagem->setCabecalho(array(
        //    "Alterar/Excluir",
        "Título",
        "Ação Estratégica"));
    $listagem->setQuery($strSqlGrid);
    $listagem->setAcoes(array(
        'edit' => 'editRegistro',
        'delete' => 'deleteRegistro',
        'view' => 'visualizarRegistro'
    ));
    $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
    $listagem->turnOnPesquisator();
    ?>
    <div class="form-group">
        <div class="well" style="text-align: left;">
            <button class="btn btn-warning" id="voltar" type="button" onclick="voltar();">Voltar</button>
            <button type="button" class="btn btn-success" onclick="cadastrar();">Novo</button>
        </div>
    </div>
    <?php $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM); ?>

    <script type="text/javascript">
        function editRegistro(registro) {
            window.location = "planacomorc.php?modulo=sistema/tabelasapoio/cadastrarVinculacaoOrcamentaria&acao=A&clicouexercicio=" +<?php echo $_SESSION['exercicio']; ?> + "&execucao=editar&vacid=" + registro;
        }

        function cadastrar() {
            window.location = "planacomorc.php?modulo=sistema/tabelasapoio/cadastrarVinculacaoOrcamentaria&acao=A";
        }

        function deleteRegistro(registro) {
            bootbox.confirm('Deseja realmente excluir este registro?',function(result){
                if (result) {
                    window.location = "planacomorc.php?modulo=sistema/tabelasapoio/vinculacaoOrcamentaria&acao=A&execucao=excluir&registro=" + registro;
                }
            });
        }

        function voltar() {
            window.location = "planacomorc.php?modulo=inicio&acao=C";
        }

        $('document').ready(function() {
            $('#modal-confirm .btn-primary').click(function() {
                $('#form-momentocredito').submit();
            });
        });

        function visualizarRegistro(id) {
            var url = 'planacomorc.php?modulo=sistema/tabelasapoio/popupImpressaoVinculacaoOrcamentaria&acao=A&vacid=' + id;
            $.get(url, function(html) {
                $('#modal-confirm .modal-body p').html(html);
                //$('#modal-confirm .highcharts-container').css('width','100%')$('#graficoExecucao')
                $('.modal-dialog').css('width', '90%');
                $('#modal-confirm #graficoExecucao').css('width', '100%');
                //$($('#modal-confirm svg rect')[2]).attr('width','100%');

                $('#modal-confirm .modal-title').html('Vinculação Orçamentária');
                $('#modal-confirm .btn-primary').remove();
                $('#modal-confirm .btn-default').html('Fechar');
                $('.modal-dialog').show();
                $('#modal-confirm').modal();

                setTimeout(function(){
                    $('[data-toggle="popover"]').popover({trigger:'hover'});
                },1000);
            });
        }
    </script>
