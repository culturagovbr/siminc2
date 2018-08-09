<?php
// Verifica se a sessao expirou.
if( ! isset($_SESSION['usucpf']) )
{
    session_unset();
    $_SESSION['MSG_AVISO'] = 'Sua sessão expirou. Efetue login novamente.';
    header('Location: login.php');
    exit();
}

include_once APPRAIZ. 'includes/library/simec/Crud/Listing.php';
include_once APPRAIZ. 'includes/funcoesspo_componentes.php';

if ($_POST['carregarSituacaoPainel']):

    if ($_POST['tpdid']):

        $sql = "select esdid as codigo, esddsc as descricao
		from workflow.estadodocumento
		where tpdid = {$_POST['tpdid']}
		order by descricao
		";
        $estadosDocumento = $db->carregar($sql);
        ?>
        <select id="estado-documento-situacao-workflow" name="esdid[]" class="form-control sel_chosen chosen" multiple="multiple" data-placeholder="Selecione">
            <option value=""></option>
            <?php if ($estadosDocumento): ?>
                <?php foreach ($estadosDocumento as $estadoDocumento): ?>
                    <option value="<?php echo $estadoDocumento['codigo'] ?>"><?php echo $estadoDocumento['descricao'] ?></option>
                <?php endforeach; ?>
            <?php endif ?>
        </select>
    <?php else : ?>
        <select id="" name="esdid[]" class="form-control sel_chosen chosen" multiple="multiple" data-placeholder="Selecione">
        </select>
    <?php  endif; exit; ?>
<?php elseif ($_POST['gerarGraficoPainel']): ?>
    <?php include_once APPRAIZ . "includes/library/simec/Grafico.php"; ?>
    <div  style="padding: 15px; ">
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
            <tr>
                <td>
                    <?php
                    $dt_inicio = $_REQUEST['dt_inicio'] ? formata_data_sql($_REQUEST['dt_inicio']) : date('Y') . '-01-01';
                    $dt_fim = $_REQUEST['dt_fim'] ? formata_data_sql($_REQUEST['dt_fim']) : date('Y-m-d');
                    $tpdid = $_POST['tpdid'];
                    $esdid = $_POST['esdid'];


                    switch ($_REQUEST['tipo_exibicao']) {
                        case('D'):
                            $formatoBusca = 'YYYYMMDD';
                            $formatoExibicao = 'DD/MM/YYYY';
                            break;
                        case('A'):
                            $formatoBusca = 'YYYY';
                            $formatoExibicao = 'YYYY';
                            break;
                        default:
                            $formatoBusca = 'YYYYMM';
                            $formatoExibicao = 'MM/YYYY';
                            break;
                    }

                    if ($esdid && is_array($esdid)) {
                        $esdid = 'and aed.esdiddestino in (' . implode(', ', array_filter($esdid)) . ')';
                    } else {
                        $esdid = '';
                    }

                    if ($tpdid == 44) {
                        // Se for fluxo PAR
                        $innerPar = 'inner join par.instrumentounidade iu on iu.docid = d.docid';
                    } else {
                        $innerPar = '';
                    }

                    $sql = "select count(*) as valor, to_char(h.htddata, '{$formatoBusca}') as data, to_char(h.htddata, '{$formatoExibicao}') as categoria, aed.esdiddestino, e.esddsc as descricao
							from
								workflow.historicodocumento h
								inner join  workflow.documento d  on h.docid = d.docid
								inner join workflow.acaoestadodoc aed on aed.aedid = h.aedid
								inner join workflow.estadodocumento e on aed.esdiddestino = e.esdid
								{$innerPar}
							where h.htddata BETWEEN '{$dt_inicio} 00:00:00' and '{$dt_fim} 23:59:59'
							and e.tpdid = {$tpdid}
							and e.esdstatus = 'A'
							and aed.aedstatus = 'A'
							$esdid
							$esdid
							group by data, categoria, aed.esdiddestino, descricao
							order by data, descricao";

                    $sql2 = "select count (d.docid) as valor, esd.esddsc  descricao
								FROM workflow.documento d
								inner join workflow.estadodocumento esd on esd.esdid = d.esdid
							  {$innerPar}
								where esd.tpdid = {$tpdid}
								group by esd.esddsc,esd.tpdid";

                    $grafico = new Grafico(NULL, false);
                    ?>
                    <table class="tabela">
                        <tr>
                            <td>
                                <?php $grafico->setTipo(Grafico::K_TIPO_LINHA)->setWidth('50%')->setHeight('300px')->setTitulo('Histórico')->gerarGrafico($sql); ?>
                            </td>
                            <td>
                                <?php $grafico->setTipo(Grafico::K_TIPO_PIZZA)->setWidth('40%')->setHeight('300px')->setTitulo('Situação atual')->gerarGrafico($sql2); ?>
                            </td>
                        </tr>
                    </table>
                </td>
            </tr>
        </table>
    </div>

    <?php
    exit;
endif;

if ($_POST['carregarUsuariosSistema'])
{
    $usuariosSistema = carregarUsuariosSistema(true);

    $listing = new Listing();
    $listing->setTypePage('M');
    $listing->setActions(array('eye-open' => 'simularUsuariosSistemas'));
    $listing->setHead(array('CPF', 'Nome', 'Email'));
    $listing->listing($usuariosSistema);
    exit;
}

if ($_POST['simularUsuariosSistema'])
{
    $cpf = $_POST['cpf'];

    global $db;

    $sql = "select distinct
				u.usucpf,
				u.usunome,
				u.usuemail
				--u.susdataultacesso,
				--u.usuacesso,
				from seguranca.usuario u
				inner join seguranca.perfilusuario pf on pf.usucpf = u.usucpf
				inner join seguranca.perfil p on p.pflcod = pf.pflcod and	p.sisid = " . $_SESSION['sisid'] . "
				inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf and	us.sisid = p.sisid
				where us.suscod = 'A' and p.pflnivel >= (
					select min(pflnivel) from seguranca.perfil
					inner join seguranca.perfilusuario on perfil.pflcod = perfilusuario.pflcod
					and perfil.sisid = " . $_SESSION['sisid'] . " )
					and u.usucpf = '$cpf';";

    $usuario = $db->pegaLinha($sql);

    if ($usuario) {
        $_SESSION['usucpf'] = $usuario['usucpf'];
        $_SESSION['usunome'] = $usuario['usunome'];
        $_SESSION['usuemail'] = $usuario['usuemail'];
        unset($_SESSION['testa_superuser']);
        unset($_SESSION['testa_superuser_cache']);
        unset($_SESSION['acl']);
        unset($_SESSION['sistemas']);
        echo 'ok';
    }

    exit;
}

// Definindo o tema do sistema.
if (isset($_POST['theme'])) {
    $theme = $_POST['theme'];
    setcookie("theme", $theme, time() + 60 * 60 * 24 * 30, "/");
} else if (isset($_COOKIE["theme"])) {
    $theme = $_COOKIE["theme"];
} else {
    $theme = '';
}

$arrTheme = array('default', 'default-inverse', 'ameliaa', 'cerulean', 'cosmo', 'cyborg', 'flatly', 'journal', 'readable', 'simplex', 'slate', 'spacelab', 'united', 'yeti');
$sistemas = carregarSistemas();
$menus = carregarMenus($_SESSION['sisid'], $_SESSION['usucpf']);
$menuHtml = montaMenuBootstrap($menus);

if ($_SESSION['sisdiretorio'] && $_SESSION["sisexercicio"] == 't')
{
    $sql = "select schemaname , relname from pg_stat_user_tables
				where schemaname = '{$_SESSION['sisdiretorio']}'
				and relname = 'programacaoexercicio'
				order by relname;";
    $tabelaExercicio = $db->pegaLinha($sql);

    if ($tabelaExercicio) {
        $sql = "select prsano as codigo, prsano as descricao,prsexerccorrente,prsexercicioaberto from " . $_SESSION['sisdiretorio'] . ".programacaoexercicio where prsstatus = 'A' order by 1";
        $arrAnoExercicio = $db->carregar($sql);
    }
}

if (!$_SESSION['exercicioaberto'] && $_SESSION['sisexercicio'] == 't' && $tabelaExercicio) {
    $sql = sprintf("SELECT prsexercicioaberto FROM %s.programacaoexercicio WHERE prsano = '%s'", $_SESSION['sisdiretorio'], $_SESSION['exercicio']);
    $_SESSION['exercicioaberto'] = $db->pegaUm($sql);
}

// altera o ano de exercício (caso o usuário solicite)
if ($_REQUEST['exercicio'] AND $_SESSION['exercicio'] != $_REQUEST['exercicio']) {


    if ($arrAnoExercicio) {
        foreach ($arrAnoExercicio as $anoExercicio) {
            $arrAno[] = $anoExercicio['codigo'];
            if ($anoExercicio['prsexerccorrente'] == "t") {
                $ano_corrente = $anoExercicio['codigo'];
            }
        }
    }
    if (is_array($arrAno)) {
        if (in_array($_REQUEST['exercicio'], $arrAno)) {
            $_SESSION['exercicio'] = $_REQUEST['exercicio'];
        } else {
            $_SESSION['exercicio'] = $ano_corrente;
        }
    }
    $exercicio = $_SESSION['exercicio'];

    $sql = sprintf("SELECT prsexercicioaberto FROM %s.programacaoexercicio WHERE prsano = '%s'", $_SESSION['sisdiretorio'], $_REQUEST['exercicio']);
    $_SESSION['exercicioaberto'] = $db->pegaUm($sql);
}

if (!isset($_COOKIE['navbar'])) {
    $_COOKIE['navbar'] = 'false';
}
$navBar = $_COOKIE['navbar']=='false' ? 'mini-navbar' : null;

// pega a página inicial do sistema
$sql = sprintf("SELECT TRIM( paginainicial ) AS paginic FROM seguranca.sistema WHERE sisid = %d", $_SESSION['sisid']);
$paginic = $db->pegaUm($sql);
?>
<!DOCTYPE html>
<html>
<head>
    <title><?php echo NOME_SISTEMA; ?></title>
    <meta http-equiv='Content-Type' content='text/html; charset=ISO-8859-1'>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">

    <!-- Fonts  -->
    <link href="/zimec/public/temas/simec/fonts/awesome/font-awesome.css?t=1.0" rel="stylesheet">

    <link href="/zimec/public/temas/simec/fonts/glyphicons/glyphicons.css?t=1.0" rel="stylesheet">
    <link href="/zimec/public/temas/simec/fonts/material/material.css?t=1.0" rel="stylesheet">
    <link href="/zimec/public/temas/simec/fonts/inconsolata/inconsolata.css?t=1.0" rel="stylesheet">
    <link href="/zimec/public/temas/simec/fonts/opensans/opensans.css?t=1.0" rel="stylesheet">

    <!-- Mainly scripts -->
    <script src="/zimec/public/temas/simec/js/jquery-1.11.3.js"></script>
    <script src="/zimec/public/temas/simec/js/bootstrap.min.js"></script>
    <script src="/zimec/public/temas/simec/js/plugins/jquery-ui/jquery-ui.min.js"></script>
    <script src="/estrutura/js/funcoes.js"></script>
    <script src="/includes/funcoes.js?v=2"></script>

    <!-- materialize menu lateral -->
    <script src="/zimec/public/temas/simec/js/plugins/materialize/materialize.js"></script>
    <script src="/zimec/public/temas/simec/js/plugins/materialize/materialize.min.js"></script>

    <!-- Highcharts -->
    <script language="javascript" src="../includes/Highcharts-4.0.3/js/highcharts.js"></script>
    <script language="javascript" src="../includes/Highcharts-4.0.3/js/highcharts-more.js"></script>
    <script language="javascript" src="../includes/Highcharts-4.0.3/js/modules/solid-gauge.src.js"></script>

    <!-- Data picker -->
    <script src="/zimec/public/temas/simec/js/plugins/datapicker/bootstrap-datepicker.js"></script>
    <script src="/zimec/public/temas/simec/js/plugins/datapicker/locales/bootstrap-datepicker.pt-BR.js"></script>

    <!-- jQuery UI custom -->
    <script src="/zimec/public/temas/simec/js/jquery-ui.custom.min.js"></script>

    <!-- Flot -->
    <script src="/zimec/public/temas/simec/js/plugins/flot/jquery.flot.js"></script>
    <script src="/zimec/public/temas/simec/js/plugins/flot/jquery.flot.pie.js"></script>

    <!-- Peity -->
    <script src="/zimec/public/temas/simec/js/plugins/peity/jquery.peity.min.js"></script>

    <!-- Sweet Aler -->
    <script src="/zimec/public/temas/simec/js/plugins/sweetalert/sweetalert.min.js"></script>

    <!-- Ion Range Slider -->
    <script src="/zimec/public/temas/simec/js/plugins/ionRangeSlider/ion.rangeSlider.min.js"></script>

    <!-- Textext JS -->
    <script src="/zimec/public/temas/simec/js/plugins/textext/js/textext.core.js"></script>
    <script src="/zimec/public/temas/simec/js/plugins/textext/js/textext.plugin.tags.js"></script>
    <script src="/zimec/public/temas/simec/js/plugins/textext/js/textext.plugin.autocomplete.js"></script>
    <script src="/zimec/public/temas/simec/js/plugins/textext/js/textext.plugin.suggestions.js"></script>
    <script src="/zimec/public/temas/simec/js/plugins/textext/js/textext.plugin.filter.js"></script>
    <script src="/zimec/public/temas/simec/js/plugins/textext/js/textext.plugin.focus.js"></script>
    <script src="/zimec/public/temas/simec/js/plugins/textext/js/textext.plugin.prompt.js"></script>
    <script src="/zimec/public/temas/simec/js/plugins/textext/js/textext.plugin.ajax.js"></script>
    <script src="/zimec/public/temas/simec/js/plugins/textext/js/textext.plugin.arrow.js"></script>

    <link href="/zimec/public/temas/simec/css/plugins/summernote/summernote-bs3.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/bootstrap.min.css" rel="stylesheet" media="all">
    <link href="/zimec/public/temas/simec/js/plugins/jquery-ui/jquery-ui.theme.min.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/js/plugins/jquery-ui/jquery-ui.structure.min.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/js/plugins/jquery-ui/jquery-ui.min.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/plugins/iCheck/custom.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/plugins/chosen/chosen.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/plugins/colorpicker/bootstrap-colorpicker.min.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/plugins/gritter/jquery.gritter.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/plugins/nouslider/jquery.nouislider.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/plugins/datapicker/datepicker3.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/plugins/ionRangeSlider/ion.rangeSlider.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/plugins/ionRangeSlider/ion.rangeSlider.skinHTML5.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/plugins/ionRangeSlider/normalize.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/plugins/switchery/switchery.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/plugins/bootstrap-tour/bootstrap-tour.min.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/plugins/summernote/summernote.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/js/plugins/textext/css/textext.core.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/js/plugins/textext/css/textext.plugin.tags.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/js/plugins/textext/css/textext.plugin.autocomplete.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/js/plugins/textext/css/textext.plugin.focus.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/js/plugins/textext/css/textext.plugin.prompt.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/js/plugins/textext/css/textext.plugin.arrow.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/plugins/sweetalert/sweetalert.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/plugins/dropzone/dropzone.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/plugins/dropzone/basic.css" rel="stylesheet">

    <link href="/zimec/public/temas/simec/css/plugins/bootstrap-switch/bootstrap-switch.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/plugins/awesome-bootstrap-checkbox/awesome-bootstrap-checkbox.css" rel="stylesheet">

    <link href="/zimec/public/temas/simec/css/plugins/awesome-bootstrap-checkbox/awesome-bootstrap-checkbox.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/plugins/nanoscroll/nanoscroller.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/plugins/jstree/style.min.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/plugins/fullcalendar/fullcalendar.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/animate.css" rel="stylesheet">

    <!-- Toastr style -->
    <link href="/zimec/public/temas/simec/css/plugins/toastr/toastr.min.css" rel="stylesheet">

    <link href="/zimec/public/temas/simec/css/style.min.css?t=1.0" rel="stylesheet" media="all">
    <link href="/zimec/public/temas/simec/css/simec.css?t=2.1" rel="stylesheet" media="all">

    <!-- HTML5 shim and Respond.js IE8 support of HTML5 elements and media queries -->
    <!--[if lt IE 9]>
    <!--<script src="js/html5shiv.js"></script>-->
    <!--<script src="js/respond.min.js"></script>-->
    <!--[endif]-->

    <link href="/zimec/public/temas/simec/css/print.css?t=1.0" rel="stylesheet" media="print" />

    <script data-pace-options='{ "ajax": false }' src="/zimec/public/temas/simec/js/plugins/pace/pace.min.js"></script>

</head>
<body class="fixed-nav <?php echo $navBar; ?>">
<script>
    Pace.on("done", function(){
        $("#loading").fadeOut(1500);
    });
    Pace.start;
</script>
<div class="loading-dialog" id="loading"></div>
<div id="wrapper">
    <nav class="navbar-default navbar-static-side" role="navigation">
        <div class="sidebar-collapse">
            <?php echo $menuHtml ?>
        </div>
    </nav>
    <div id="page-wrapper" class="gray-bg">
        <div class="row border-bottom">
            <nav class="navbar navbar-fixed-top" role="navigation" style="margin: 0px; height: 61px; border-bottom: 1px solid #2f4050 !important; background: #2f4050") no-repeat">
            <div class="navbar-left">
                <ul class="header-nav header-nav-options">
                    <li class="header-nav-brand" style="cursor:pointer;" onclick="window.location='<?= $_SESSION['sisarquivo'] ?>.php?modulo=inicio&acao=C';">
                        <img src="/zimec/public/img/logo-simec.png" class="img-responsive" style="width: 139px; margin: 10px 0px;">
                    </li>
                    <li id="liSistemaXs" class="header-nav-brand hidden-sm hidden-md hidden-lg" style="padding-top: 8px;">
                        <select class="form-control chosen-select .chosen-sistema" data-placeholder="Selecione" onchange="javascript:changeSystem(this.value)">
                            <?php foreach ($sistemas as $sistema): ?>
                                <?php $selected = ($sistema['sisid'] == $_SESSION['sisid']) ? 'selected="selected"' : null; ?>
                                <option <?php echo $selected; ?> value="<?php echo $sistema['sisid'] ?>"><?php echo $sistema['sisabrev'] ?></option>
                            <?php endforeach ?>
                        </select>
                    </li>
                </ul>
            </div>
            <div class="navbar-center hidden-xs">
                <form id="setperfil" name="setperfil" action="<?= $_SESSION['sisarquivo'] ?>.php?modulo=<?= $paginic ?>" method="post">
                    <input type="hidden" id="exercicio" name="exercicio" value=""  class="form-control" />
                </form>
                <script>
                    $(function(){
                        $('#chosen-exercicio').change(function(){
                            $('#exercicio').val($(this).val());
                            document.getElementById('setperfil').submit();
                        });
                    })
                </script>
                <ul class="header-nav header-nav-options">
                    <li>
                        <a class="navbar-minimalize btn btn-primary" href="#"><i class="fa fa-bars"></i></a>
                    </li>
                    <li>
                        <input type="text" class="form-control typeahead" placeholder="Digite aqui o que procura">
                    </li>
                    <li>
                        <select class="form-control chosen-select .chosen-sistema" data-placeholder="Selecione" onchange="javascript:changeSystem(this.value)">
                            <?php foreach ($sistemas as $sistema): ?>
                                <?php $selected = ($sistema['sisid'] == $_SESSION['sisid']) ? 'selected="selected"' : null; ?>
                                <option <?php echo $selected; ?> value="<?php echo $sistema['sisid'] ?>"><?php echo $sistema['sisabrev'] ?></option>
                            <?php endforeach ?>
                        </select>
                    </li>
                    <?php if ($arrAnoExercicio && is_array($arrAnoExercicio)): ?>
                        <li>
                            <select id="chosen-exercicio" class="form-control chosen-select" data-placeholder="Exercicio">
                                <?php foreach ($arrAnoExercicio as $anoExercicio): ?>
                                    <option value="<?php echo $anoExercicio['codigo'] ?>" <?php echo $_SESSION['exercicio'] == $anoExercicio['codigo'] ? 'selected="selected"' : ''; ?>><?php echo $anoExercicio['descricao'] ?></option>
                                <?php endforeach ?>
                            </select>
                        </li>
                    <?php endif ?>
                </ul>
            </div>
            <div class="navbar-right hidden-xs">
                <ul class="header-nav header-nav-profile">
                    <li class="dropdown ms-hover">
                        <a href="javascript:void(0);" class="dropdown-toggle ink-reaction" data-toggle="dropdown">
                            <img alt="image" class="profile-pic" src="/seguranca/imagemperfil.php">
                            <span class="profile-info">
                                        <?php echo $_SESSION['usunome']; ?>
                                <small><?php echo $_SESSION['usufuncao']; ?></small>
                                    </span>
                        </a>
                        <ul class="dropdown-menu animation-dock">
                            <?php if ($_SESSION['superuser'] == 1): ?>
                                <li class="dropdown-header">Super Usuário</li>
                            <?php else: ?>
                                <li class="dropdown-header">Opções Usuário</li>
                            <?php endif; ?>
                            <?php if ($_SESSION['superuser'] == 1 || $_SESSION['usucpf'] != $_SESSION['usucpforigem'] || $exibirSimular == true || $db->testa_uma() || $_SESSION['usuuma']): ?>
                                <li>
                                    <?php if ($_SESSION['usucpf'] == $_SESSION['usucpforigem']): ?>
                                        <a data-toggle="modal" id="btSimularUsuario">
                                            <i class="glyphicon glyphicon-eye-open"></i> Simular Usuário
                                        </a>
                                    <?php else: ?>
                                        <a id="voltar_usuario_origem">
                                            <i  class="glyphicon glyphicon-eye-close"></i> Voltar Usuário
                                        </a>
                                    <?php endif; ?>
                                </li>
                            <?php endif; ?>
                            <li>
                                <a id="btUser" data-toggle="modal" data-target="#modal-user">
                                    <i class="glyphicon glyphicon-wrench"></i> Gerenciar seus dados
                                </a>
                            </li>
                            <?php if ($_SESSION['superuser'] == 1 || $exibirGraficoWorflow == true): ?>
                                <li>
                                    <?php $sql = "select tpdid from workflow.tipodocumento where sisid = '{$_SESSION['sisid']}'"; ?>
                                    <?php $tipoDocumento = $db->pegaUm($sql); ?>
                                    <?php if ($tipoDocumento): ?>
                                        <a data-toggle="modal" id="btPainelWorkflow">
                                            <i class="glyphicon glyphicon-stats"></i> Painel do workflow
                                        </a>
                                    <?php endif ?>
                                </li>
                            <?php endif; ?>
                            <li class="divider"></li>
                            <li>
                                <a onclick="javascript:window.location.href = '/logout.php'">
                                    <i class="fa fa-sign-out"></i> Sair
                                </a>
                            </li>
                        </ul><!--end .dropdown-menu -->
                    </li><!--end .dropdown -->
                </ul>
            </div>
            </nav>
        </div>
