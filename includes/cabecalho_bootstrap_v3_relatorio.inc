<?php
// Verifica se a sessao expirou.
if( ! isset($_SESSION['usucpf']) )
{
	session_unset();
	$_SESSION['MSG_AVISO'] = 'Sua sessão expirou. Efetue login novamente.';
	header('Location: login.php');
	exit();
}

include_once APPRAIZ . 'includes/library/simec/Crud/Listing.php';

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

if (!isset($_COOKIE['navbar'])) {
    $_COOKIE['navbar'] = 'false';
}
$navBar = $_COOKIE['navbar']=='false' ? 'mini-navbar' : null;

?>
<!DOCTYPE html>
<html>
    <head>
        <title><?php echo NOME_SISTEMA; ?></title>
        <meta http-equiv='Content-Type' content='text/html; charset=ISO-8859-1'>
        <meta name="viewport" content="width=device-width, initial-scale=1.0">

        <!-- Fonts  -->
        <link href="/zimec/public/temas/simec/fonts/awesome/font-awesome.css?v=1.0" rel="stylesheet">

        <link href="/zimec/public/temas/simec/fonts/glyphicons/glyphicons.css?v=1.0" rel="stylesheet">
        <link href="/zimec/public/temas/simec/fonts/material/material.css?v=1.0" rel="stylesheet">
        <link href="/zimec/public/temas/simec/fonts/inconsolata/inconsolata.css?v=1.0" rel="stylesheet">
        <link href="/zimec/public/temas/simec/fonts/opensans/opensans.css?v=1.0" rel="stylesheet">

        <!-- Mainly scripts -->
        <script src="/zimec/public/temas/simec/js/jquery-1.11.3.js"></script>
        <script src="/zimec/public/temas/simec/js/bootstrap.min.js"></script>
        <script src="/zimec/public/temas/simec/js/plugins/jquery-ui/jquery-ui.min.js"></script>
        <script src="/estrutura/js/funcoes.js"></script>
        <script src="/includes/funcoes.js"></script>

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

        <link href="/zimec/public/temas/simec/css/style.min.css?v=1.1.1" rel="stylesheet" media="all">
        <link href="/zimec/public/temas/simec/css/simec.css?v=2.3.0" rel="stylesheet" media="all">

        <!-- HTML5 shim and Respond.js IE8 support of HTML5 elements and media queries -->
        <!--[if lt IE 9]>
        <!--<script src="js/html5shiv.js"></script>-->
        <!--<script src="js/respond.min.js"></script>-->
        <!--[endif]-->

        <link href="/zimec/public/temas/simec/css/print.css?v=1.0" rel="stylesheet" media="print" />

        <script data-pace-options='{ "ajax": false }' src="/zimec/public/temas/simec/js/plugins/pace/pace.min.js"></script>

    </head>
    <body class="pace-done mini-navbar fixed-sidebar">
        <script>
            Pace.on("done", function(){
                $("#loading").fadeOut(1500);
            });
            Pace.start;
        </script>
        
        <div class="loading-dialog" id="loading"></div>
        
        <div id="wrapper">
            <div id="page-wrapper" class="gray-bg" style="margin-left: 0;">
                <div class="row border-bottom">
                    <nav class="navbar navbar-fixed-top" role="navigation" style="margin: 0px; height: 61px; border-bottom: 1px solid #2f4050 !important; background: #2f4050") no-repeat">
                        <div class="navbar-left">
                            <ul class="header-nav header-nav-options">
                                <li class="header-nav-brand" style="cursor:pointer;" onclick="window.location='<?= $_SESSION['sisarquivo'] ?>.php?modulo=inicio&acao=C';"><img src="/zimec/public/img/logo-simec.png" class="img-responsive" style="width: 139px; margin: 10px 0px;"></li>
                            </ul>
                        </div>
                    </nav>
                </div>
            
                <div class="wrapper wrapper-content animated fadeInRight">
                    <div class="clearfix"></div>
                </div>

                    <div class="ibox float-e-margins" style="margin-bottom: 0px !important;">
                        <div class="ibox-title">
                            <p style="font-weight: bold">Minist&eacute;rio da Cultura</p>
                            <p style="margin-top: -1em;font-weight: bold">Coordena&ccedil;&atilde;o de Planejamento, Or&ccedil;amento, Finan&ccedil;as e Contabilidade</p>
                            <p style="margin-top: -1em;font-weight: bold">Relat&oacute;rio de Capta&ccedil;&atilde;o Executiva emitido em <?= date('d/m/Y')?></p>
                        </div>
                    </div>
