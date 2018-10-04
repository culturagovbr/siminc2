<?php
if ($_SESSION['sislayoutbootstrap'] == 't') 
{
    include_once 'cabecalho_bootstrap.inc';
}
else if ($_SESSION['sislayoutbootstrap'] == 'zimec')
{
	include_once 'cabecalho_bootstrap_v3.inc';
}
else 
{
	if(isset($_SESSION["theme_simec"])) 
	{
		$theme = $_SESSION["theme_simec"];
	}
	else
	{
		$theme = recuperaThemaUsuario();
		
		if($theme) 
		{
			$_SESSION["theme_simec"] = $theme;
		}
		else 
		{
			$theme = $_SESSION['theme_temp'];
			
			if($theme)
			{
				$_SESSION["theme_simec"] = $theme;
			} 
			else 
			{
				$theme = "verde";
				$_SESSION["theme_simec"] = $theme;
			}
		}
	}

	if (isset($_POST["theme_simec"]) && $_SESSION["theme_simec"] != $_POST["theme_simec"]) {
		$theme = $_POST["theme_simec"] ? $_POST["theme_simec"] : 'areia';
		setcookie("theme_simec", $theme , time()+60*60*24*30, "/");
		$_SESSION["theme_simec"] = $theme;
		gravaThemaUsuario($theme);
	}
	
	if($_REQUEST['carregarcolaboradores'] and ( $_SESSION['superuser'] OR $db->testa_uma() OR $_SESSION['usuuma'] )) { 
		$sql = "select distinct
				u.usucpf as codigo,
				u.usunome as descricao
				from seguranca.usuario u
				inner join seguranca.perfilusuario pf on pf.usucpf = u.usucpf
				inner join perfil p on p.pflcod = pf.pflcod and	p.sisid = " . $_SESSION['sisid'] . "
				inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf and	us.sisid = p.sisid
				where us.suscod = 'A' and p.pflnivel >= (
				select min(pflnivel) from seguranca.perfil 
				inner join seguranca.perfilusuario on perfil.pflcod = perfilusuario.pflcod 
				where perfilusuario.usucpf = '" . $_SESSION['usucpforigem'] . "'
				and perfil.sisid = " . $_SESSION['sisid'] . " ) 
				order by 2 ";
		$opt = $db->carregar($sql);
		if($opt[0]) {
			foreach($opt as $op) {
				echo $op['codigo']."##".$op['descricao']."&&";
			}
		}
		exit;
	}
	/**
	 * Sistema Integrado de Monitoramento do Ministério da Educação
	 * Setor responsvel: DTI / SE / MEC
	 * Gerente: Cristiano Cabral <cristiano.cabral@gmail.com>
	 * Analistas: Cristiano Cabral <cristiano.cabral@gmail.com>, Adonias Malosso <malosso@gmail.com>
	 * Desenvolvedor: Equipe de Desenvolvedores Simec
	 * Módulo: Cabeçalho do Sistema
	 * Finalidade: Exibir Cabeçalho com funções gerais do sistema.
	 * Data de criação:
	 * Última modificação: 28/08/2006
	 **/

	// ativa o flag de inclusão do cabeçalho
	$cabecalho_sistema = true;

	if($_SESSION['sisdiretorio'] && $_SESSION["sisexercicio"] == 't'){
		$sql = "select prsano as codigo, prsano as descricao,prsexerccorrente,prsexercicioaberto from ". $_SESSION['sisdiretorio'] .".programacaoexercicio order by 1";
		$arrAnoExercicio = $db->carregar($sql);
	}

   if (! $_SESSION['exercicioaberto'] && $_SESSION['sisexercicio'] == 't')
   {
	   $sql = sprintf(
			"SELECT prsexercicioaberto FROM %s.programacaoexercicio WHERE prsano = '%s'",
			$_SESSION['sisdiretorio'],
			$_SESSION['exercicio']
		);
		$_SESSION['exercicioaberto'] = $db->pegaUm( $sql );
   }
	// altera o ano de exercício (caso o usuário solicite)
	if ( $_REQUEST['exercicio'] AND $_SESSION['exercicio'] != $_REQUEST['exercicio'] ) {
		if($arrAnoExercicio){
			foreach($arrAnoExercicio as $anoExercicio){
				$arrAno[] = $anoExercicio['codigo'];
				if($anoExercicio['prsexerccorrente'] == "t"){
					$ano_corrente = $anoExercicio['codigo'] ;
				}
			}
		}
		if(is_array($arrAno)){
			if(in_array($_REQUEST['exercicio'],$arrAno)){
				$_SESSION['exercicio'] = $_REQUEST['exercicio'];  
			}else{
				$_SESSION['exercicio'] = $ano_corrente;
			}
		}
		$exercicio = $_SESSION['exercicio'];
		
		$sql = sprintf(
			"SELECT prsexercicioaberto FROM %s.programacaoexercicio WHERE prsano = '%s'",
			$_SESSION['sisdiretorio'],
			$_REQUEST['exercicio']
		);
		$_SESSION['exercicioaberto'] = $db->pegaUm( $sql );
	}
	/*
	 * O contador de tempo online na tela deve ser atualizado toda vez que o
	 * usuário carregar uma tela do sistema. Ele é utilizado pelo "estou vivo"
	 * de acordo com a constante MAXONLINETIME, definido no config.inc.
	 */
	include APPRAIZ . "includes/registraracesso.php";
?>
<?php if ($cabecalho_painel==true) : ?>
<?php 	include APPRAIZ . 'www/painel/novo/cabecalho_painel.php'; ?>
<?php else: ?>
<?php 	if ($painelCabecalho) : ?>
<?php 		include APPRAIZ."includes/painel_slide_ajax.inc"; ?>
<?php 	else: ?>
    <link href="../library/sliding_panel/css/slide.css" rel="stylesheet" media="screen">
    <script src="../library/jquery/jquery-1.11.1.min.js" type="text/javascript" charset="ISO-8895-1"></script>
    <script src="../library/jquery/jquery-ui-1.10.3/jquery-ui.min.js"></script>
    <script src="../library/chosen-1.0.0/chosen.jquery.js"></script>
    
    <!--SWAL-->
    <script src="/zimec/public/temas/simec/js/plugins/sweetalert/sweetalert.min.js"></script>
    <link href="/zimec/public/temas/simec/css/plugins/sweetalert/sweetalert.css" rel="stylesheet">
    <script language="javascript">$1_11 = $.noConflict();</script>
<?php 	endif  ?>
<?php endif  ?>
<html>
	<head>
		<meta name="description" content="<?php echo NOME_SISTEMA; ?>, Permite o Monitoramento Físico e Financeiro e a Avaliação das Ações e Programas do Ministério dentre outras atividades estratégicas">
		<meta name="keywords" content="<?php echo SIGLA_SISTEMA; ?>, <?=NOME_ORGAO_SISTEMA?>">
		<META NAME="Author" CONTENT="Cristiano Cabral, cristiano.cabral@gmail.com">
		<meta name="audience" content="all">
		<meta http-equiv="Cache-Control" content="no-cache">
		<meta http-equiv="Pragma" content="no-cache">
		<meta http-equiv="Expires" content="-1">
        <meta content="IE=9" http-equiv="X-UA-Compatible" />

        <!-- Bootstrap CSS -->
        <link href="../library/bootstrap-3.0.0/css/bootstrap.min-simec.css" rel="stylesheet" media="screen">
        <link href="/zimec/public/temas/simec/fonts/awesome/font-awesome.css?v=1.0" rel="stylesheet">
        <script src="../includes/JQuery/jquery-1.5.1.min.js" type="text/javascript"></script>
        <script src="../library/chosen-1.0.0/chosen.jquery.js" type="text/javascript"></script>
        <script src="../library/chosen-1.0.0/docsupport/prism.js" type="text/javascript"></script>        
        <link href="../library/chosen-1.0.0/chosen.css" rel="stylesheet" media="screen" >
		<center>
			<div id="aguarde" style="background-color:#ffffff;position:absolute;color:#000033;top:50%;left:30%;border:2px solid #cccccc; width:300;font-size:12px;z-index:0;">
				<br><img src="../imagens/wait.gif" border="0" align="middle"> Aguarde! Carregando Dados...<br><br>
			</div>
		</center>
		<title><?php echo NOME_SISTEMA; ?></title>
		<script language="JavaScript" src="../includes/funcoes.js"></script>
        <link href="/library/simec/css/css_reset.css" rel="stylesheet">
        <link href="/library/simec/css/barra_brasil.css" rel="stylesheet">
		<link href="../includes/Estilo.css" rel="stylesheet" type="text/css"/>
		<link href='../includes/listagem.css' rel='stylesheet' type='text/css'/>

		<!-- INCLUSÃO DO CSS PARA NOVO LAYOUT DO SIMEC - 15/05/10 -->
		<?php if(is_file( APPRAIZ."www/includes/layout/".$theme."/include_simec.inc" )) include APPRAIZ."www/includes/layout/".$theme."/include_simec.inc"; ?>

		<script language="JavaScript">
			function setpfl() {
				document.getElementById('setperfil2').submit();
			}

			function setpfl1() {
				document.getElementById('setperfil').submit();
			}

			function abrirsistema( sisid ) {
				location.href = "../muda_sistema.php?sisid=" + sisid;
			}

			function abrir_popup_mensagem()
			{
				w = window.open( '../geral/popup_mensagem.php', 'mensagens', 'width=780,height=400,scrollbars=yes,menubar=no,toolbar=no,statusbar=no' );
				w.focus();
			}

            function changeSystem(system)
            {
                location.href = "../muda_sistema.php?sisid=" + system;
            }
            function exibeThemas()
            {
            	var div = document.getElementById('menu_theme');
				if (div.style.display == 'none')
            		div.style.display = '';
            	else
            		div.style.display = 'none';
            }

            function alteraTema(){
            	document.getElementById('formTheme').submit();
            }
		</script>

		<!-- AGRUPADOR -->
		<script type="text/javascript" language="javascript" src="../includes/agrupador.js"></script>
		<style type="text/css">
			.combo {
				width: 200px;
			}
		</style>
		<?php include APPRAIZ . "includes/gera_menu.inc"; ?>

	</head>
	<?php
		$sql = sprintf("SELECT TRIM( paginainicial ) AS paginic FROM seguranca.sistema WHERE sisid = %d", $_SESSION['sisid']);
		$paginic = $db->pegaUm( $sql );
	?>
	<body LEFTMARGIN="0" TOPMARGIN="0" MARGINWIDTH="0" MARGINHEIGHT="0" BGCOLOR="#ffffff">
	<?php if ($_SESSION['mostramsg']) : ?>
		<script>window.open("../geral/ctrlmensagens.php?tot=<?=$_SESSION['mostramsg']?>", "VtrlMensagem", "width=420,height=300,menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes");</script>
	<?php endif; unset($_SESSION['mostramsg']); ?>
    <style>
        a.chosen-single {
            -webkit-box-sizing:border-box;-moz-box-sizing:border-box;box-sizing:border-box;
            /*background-color: #FFFFFF !important;*/
            /*border: 1px solid #AAAAAA !important;*/
            border-radius: 5px !important;
            box-shadow: 0 0 3px #FFFFFF inset, 0 1px 1px rgba(0, 0, 0, 0.1) !important;
            /*color: #444444 !important;*/
            display: block !important;
            height: 25px !important;
            line-height: 25px !important;
            overflow: hidden !important;
            padding: 0 0 0 8px !important;
            position: relative !important;
            /*text-align: center !important;*/
            text-decoration: none !important;
            white-space: nowrap !important;
        }

        .chosen-container-multi .chosen-choices li.search-field input[type="text"] {
            margin: 1px 0;
            padding: 5px !important;
            height: 25px !important;
            outline: 0 !important;
            border: 0 !important;
            background: transparent !important;
            box-shadow: none !important;
            color: #666 !important;
            font-size: 100% !important;
            font-family: sans-serif !important;
            line-height: normal !important;
            border-radius: 0 !important;
        }

        .chosen-container-active.chosen-with-drop .chosen-single {
            border: 1px solid #aaa!important;
            -moz-border-radius-bottomright: 0 !important;
            border-bottom-right-radius: 0 !important;
            -moz-border-radius-bottomleft: 0 !important;
            border-bottom-left-radius: 0 !important;
            background-image: -webkit-gradient(linear, 50% 0%, 50% 100%, color-stop(20%, #eeeeee), color-stop(80%, #ffffff))!important;
            background-image: -webkit-linear-gradient(#eeeeee 20%, #ffffff 80%)!important;
            background-image: -moz-linear-gradient(#eeeeee 20%, #ffffff 80%)!important;
            background-image: -o-linear-gradient(#eeeeee 20%, #ffffff 80%)!important;
            background-image: linear-gradient(#eeeeee 20%, #ffffff 80%)!important;
            box-shadow: 0 1px 0 #fff inset!important;
        }

        .cabecalho a.chosen-single{
            -webkit-box-sizing:border-box;-moz-box-sizing:border-box;box-sizing:border-box;
            background-color: #FFFFFF !important;
            border: 1px solid #AAAAAA !important;
            border-radius: 5px !important;
            box-shadow: 0 0 3px #FFFFFF inset, 0 1px 1px rgba(0, 0, 0, 0.1) !important;
            color: #444444 !important;
            display: block !important;
            height: 30px !important;
            line-height: 29px !important;
            overflow: hidden !important;
            padding: 0 0 0 8px !important;
            position: relative !important;
            text-align: center !important;
            text-decoration: none !important;
            white-space: nowrap !important;
        }

        #div-dados-login *{
            color: #000;
        }
        #top_content{
            height: 117px;
            background-color:#E5EAED;
        }
        #top_content_logo_menu{
            float:left;
            width:392px;
        }
        #top_content_sistema{
            margin-left:392px;
        }
        #top_content_label_sistema{
            margin-right:300px;
            padding-top: 10px;
        }
        #div-dados-login{
            float:right;
            /*width:300px;*/
            width:320px;
        }
        #menubackmenu1{
            margim: 0px !important;
            width: 100% !important;
        }
        .barraAmbienteTexto {
            color: #FFFFFF;
            font-weight: bolder;
        }
        .barraAmbienteTabela {
            width: 100%;
            padding-right: 30px;
        }
        .barraAmbienteDivPrincipal {
            margin-top: 35px;
            width: 100%;
        }
        .barraAmbienteDivPrincipalCorFundoAzul {
            background-color: #00A5E0;
        }
        .barraAmbienteDivPrincipalCorFundoVerde {
            background-color: #269917;
        }
        .barraAmbienteLabelBanco {
            padding: 3px;
        }
        .barraAmbienteLabelBancoEspelho {
            background-color: #000587;
        }
        .barraAmbienteLabelBancoDesenv {
            background-color: #E92922;
        }
        .ajaxCarregando{
            z-index: 2100;
            position: fixed;
            right: 0;
            left: 0;
            border-width: 0 0 1px;
            bottom: 0;
            margin-bottom: 0;
            border-width: 1px 0 0;
            width: 100%;
            height: 40px;
            text-align: center;
            line-height: 40px;
            font-size: 27px;
            background: rgba(0, 143, 255, 0.62);
            color: #FFF;
            text-shadow: #555555 0px 0px 7px;
        }
        .ajaxCarregandoErro{
            z-index: 2100;
            position: fixed;
            right: 0;
            left: 0;
            border-width: 0 0 1px;
            bottom: 0;
            margin-bottom: 0;
            border-width: 1px 0 0;
            width: 100%;
            background-color: rgba(255, 0, 0, 0.55);
            height: 40px;
            text-align: center;
            line-height: 40px;
            font-size: 27px;
            color: #FFF;
            text-shadow: #313131 0px 0px 8px;
        }

    </style>
    <table border="0" id="main" cellpadding="0" cellspacing="0" style="width: 100%;">
        <tr>
            <td align="left" valign="top" colspan="2" >

            <div id="top_content" class="notprint">
				<br />
                <div id="top_content_logo_menu">
                    <div style="float: left;">
                        <img style="cursor:pointer" onclick="window.location.href='<?=$_SESSION['sisarquivo']?>.php?modulo=<?=$paginic?>'" src="../includes/layout/<? echo $theme ?>/img/logo.png" >
                    </div>
                    <?php $sistemas = carregarSistemas(); ?>
                    <div style="float: left;">
                        <div style="margin-top: 10px;" class="cabecalho">
                            <select data-placeholder="Escolha um módulo do sistema..." id="cabecalho" class="chosen-select" style="width:200px;" tabindex="2" onchange="javascipt:changeSystem(this.value);">
                                <?php if ($sistemas && is_array($sistemas)) : ?>
                                   <?php foreach ($sistemas as $sistema): ?>
                                        <option <?php if($_SESSION['sisid'] == $sistema['sisid']) echo 'selected="true"' ?> value="<?php echo $sistema['sisid'] ?>"><?php echo $sistema['sisabrev'] ?></option>
                                    <?php endforeach; ?>
                                <?php endif; ?>
                            </select>
                        </div>
                    </div>
                </div>
                <script type="text/javascript">
                    var jq = jQuery.noConflict();
                    jq('.chosen-select').chosen({allow_single_deselect:true});
                </script>

                <div id="top_content_sistema">

                <div id="div-dados-login" >
                    <?php if ($_SESSION["sisexercicio"] == 't'): ?>
                    <?php if ($_SESSION['exercicio']) $exercicio = $_SESSION['exercicio']; ?>
                    <?php $exerc = $_SESSION['sisdiretorio'] == 'monitora' ? 'Exercício' : 'Exercício'; ?>
                    <?php endif; ?>
                        <table id="content_top_simula">
                            <tr>
                                <td>
                                    <?= $exerc ? $exerc." :" : "" ?>
                                </td>
                                <td style="color:#FFFFFF" >
                                    <form id="setperfil" name="setperfil" action=<?= $_SESSION['sisarquivo'] ?>.php?modulo=<?= $paginic ?>&acao=C" method="post">
                                        <?php $exerc ? $db->monta_combo('exercicio',$arrAnoExercicio,'S','','setpfl1','') : ""; ?>
                                    </form>
                                </td>

                                <td align="right">
                                    <a href="javascript:exibeThemas()"><img src="../includes/layout/<? echo $theme ?>/img/bt_temas.png" style="vertical-align: middle" alt="Alterar Tema" title="Alterar Tema" border="0" /></a>
                                    <a href="javascript:self.print();"><img src="../includes/layout/<? echo $theme ?>/img/bt_print.png" style="vertical-align: middle" border="0" /></a>
                                    <a href="javascript:DoPopup('<?= $_SESSION['sisarquivo'] ?>.php?modulo=ajuda/ajuda&acao=C&mnuid=<?= $modulo_atual ?>','helpsimec',440,550);"><img style="vertical-align: middle" src="../includes/layout/<? echo $theme ?>/img/bt_help.png" border="0" id="bt_help_sistema"/></a>
                                    <a href="javascript:abrir_popup_mensagem();"><img src="../includes/layout/<? echo $theme ?>/img/bt_mail.png" style="vertical-align: middle" border="0" /></a>
                                    <a href="/logout.php"><img src="../includes/layout/<? echo $theme ?>/img/bt_logoff.png" border="0" style="vertical-align: middle" /></a>

                                    <div style="display:none;margin-left:-150px" id="menu_theme">
                                        <form id="formTheme" action="" method="post" >
                                            Tema:
                                            <select class="select_ylw" name="theme_simec" title="Tema do <?php echo SIGLA_SISTEMA; ?>" onchange="alteraTema(this.value)" >
                                                <?php include(APPRAIZ."www/listaTemas.php") ?>
                                            </select>
                                        </form>
                                    </div>

                                </td>
                            </tr>
                            <form id="setperfil2" name="setperfil2" action=<?= $_SESSION['sisarquivo'] ?>.php?modulo=<?= $paginic ?>&acao=C" method="post">
                                <tr>
                                    <td style="white-space: no-wrap;font-weight:bold" >
                                        Usuário:
                                    </td>
                                    <td colspan="2" style="white-space: no-wrap;font-weight:bold" >
                                        <table border="0" cellpadding="0" cellspacing="0">
                                            <tr>
                                                <td style="white-space: no-wrap;font-weight:bold" valing="top">
                                                    <?php $_SESSION['usucpf'] = $_REQUEST['usucpf_simu'] ? $_REQUEST['usucpf_simu'] : $_SESSION['usucpf']; ?>
                                                    <?php if ( $_SESSION['superuser'] OR $db->testa_uma() OR $_SESSION['usuuma'] ): ?>
                                                    <?php
                                                        $sql = "select distinct
                                                                       u.usucpf as codigo,
                                                                       u.usunome as descricao
                                                                  from seguranca.usuario u
                                                            inner join seguranca.perfilusuario pf on pf.usucpf = u.usucpf
                                                            inner join perfil p on p.pflcod = pf.pflcod and p.sisid = " . $_SESSION['sisid'] . "
                                                            inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf and us.sisid = p.sisid
                                                                 where us.suscod = 'A' 
    															   and u.usucpf='".$_SESSION['usucpf']."'
                                                              order by 2";
                                                        
                                                        $_SESSION['usuuma'] = 1;
                                                        
                                                        if (isset($_SESSION['dadosusuario'][$_SESSION['usucpf']]))
                                                        {
                                                        	$dadosusuario = $_SESSION['dadosusuario'][$_SESSION['usucpf']];
                                                        }
                                                        else
                                                        {
                                                        	$dadosusuario = $db->pegaLinha($sql);
                                                        	$_SESSION['dadosusuario'][$_SESSION['usucpf']] = $dadosusuario;
                                                        }	
                                                        
                                                        $usucpf_simu = $_SESSION['usucpf'];
                                                    ?>
                                                    	<span onclick="javascript:simularUsuario();" title="Simular Usuário" style="cursor: pointer;"><?php echo $dadosusuario['descricao']; ?></span>
	                                                    <input type="hidden" name="usucpf_simu" id="usrs" value="<?php echo $usucpf_simu;?>">
                                                    <?php else: ?>
                                                    <?php echo $_SESSION['usunome']; ?>
                                                    <?php endif; ?>
                                                </td>
                                                <td style="width: 30px; text-align: center;">
                                                    <?php if ($_SESSION['superuser'] OR $db->testa_uma() OR $_SESSION['usuuma']) : ?>
                                                        <img alt="" src="../imagens/simular_usuario.png" title="Simular Usuário" onclick="javascript:simularUsuario();" style="width: 18px; cursor: pointer;">
                                                    <?php endif; ?>
                                                </td>
                                            </tr>
                                        </table>
                                    </td>
                                </tr>
                                <tr>
                                    <td></td>
                                    <td colspan=2 style="white-space: no-wrap;">
                                        <div class="layBarraCronometroEstilo" id="cronometro_div">Sua sessão expira em:<span id="cronometro" style="font-weight:bold; color: #f00;"></span></div>
                                    </td>
                                </tr>
                        </table>
                    </form>
                </div>

                <div id="top_content_label_sistema">
                        <?php $arrSis = $sistemas[$_SESSION['sisid']]; ?>
                        <h2 style='text-align: center;'><?php echo $arrSis['sisabrev'] ?></h2>
                </div>
            </div>
            <div style="clear: both;"></div>
       </div>

        <!--Cabeçalho de impressão-->
        <table width="100%" border="0" cellpadding="0" cellspacing="0" class="notscreen"  style="border-bottom: 1px solid;" >
            <tr bgcolor="#ffffff">
                <td><img src="../imagens/brasao.gif" width="50" height="50" border="0"></td>
                <td height="20" nowrap>
                    <?php echo NOME_SISTEMA; ?><br>
                    <?= NOME_ORGAO_SISTEMA ?> / SE - Secretaria Executiva<br>
                </td>
                <td height="20" align="right">
                    Impresso por: <strong><?= $_SESSION['usunome']; ?></strong><br>
                    Hora da Impressão: <?= date("d/m/Y - H:i:s") ?>
                </td>
            </tr>
            <tr bgcolor="#ffffff">
                <td colspan="2">&nbsp;</td>
            </tr>
        </table>
        <!--Cabeçalho de impressão-->

			<table width="100%" border="0" cellpadding="0" cellspacing="0" class="notprint" style="display: none">
				<tr>
					<td valign="bottom">
						<?php $sistemas = carregarSistemas(); ?>
						<?php if ( $sistemas ) : ?>
							<script type="text/javascript">
							var goleftimage='../imagens/back.gif'
							var gorightimage='../imagens/foward.gif'
							//configure menu width (in px):
							var myWidth;
							if( typeof( window.innerWidth ) == 'number' ) {
							    //Non-IE
							    myWidth = window.innerWidth - 70 ;
							} else if( document.documentElement && ( document.documentElement.clientWidth || document.documentElement.clientHeight ) ) {
							    //IE 6+ in 'standards compliant mode'
							    myWidth = document.documentElement.clientWidth - 50;
							} else if( document.body && ( document.body.clientWidth || document.body.clientHeight ) ) {
							    //IE 4 compatible
							    myWidth = document.body.clientWidth - 50 ;
							}
							var menuwidth = myWidth;
							var menuheight=18
							var scrolldir="normal"
							var scrollspeed=10
							var menucontents='<table id="tb_menu" border="0" cellspacing="0" cellpadding="0"><tr>';
							<? foreach ( $sistemas as $key => $sistema ): ?>
								<? extract( $sistema ); ?>
								<? if ( $sisid == $_SESSION['sisid'] && $key == 0){ ?>
									menucontents = menucontents + '<td><img src="../includes/layout/<? echo $theme ?>/img/aba_left.png"></td>';
								<? }elseif ( $sisid != $_SESSION['sisid'] && $key == 0){ ?>
									menucontents = menucontents + '<td><img src="../includes/layout/<? echo $theme ?>/img/aba_left_off.png"></td>';
								<? }elseif ( $sisid == $_SESSION['sisid'] && $key != 0 ){ ?>
									menucontents = menucontents + '<td><img src="../includes/layout/<? echo $theme ?>/img/aba_mid_left_off.png"></td>';
								<? }elseif ( $sisid != $_SESSION['sisid'] && $key != 0 && $sistemas[$key - 1]['sisid'] == $_SESSION['sisid']){ ?>
									menucontents = menucontents + '<td><img src="../includes/layout/<? echo $theme ?>/img/aba_mid_right_off.png"></td>';
								<? }elseif ( $sisid != $_SESSION['sisid'] && $key != 0 ){ ?>
									menucontents = menucontents + '<td><img src="../includes/layout/<? echo $theme ?>/img/aba_mid_off.png"></td>';
								<? } ?>
							
								<? if ( $sisid == $_SESSION['sisid']){ ?>
									menucontents = menucontents + '<td nowrap class="td_on" onclick="abrirsistema(<?= $sisid ?>)" title="<?= $sisdsc ?>"><a href="javascript:abrirsistema(<?= $sisid ?>)" ><?= $sisabrev ?></a></td>';
								<? } else{ ?>
									menucontents = menucontents + '<td nowrap class="td_off" onclick="abrirsistema(<?= $sisid ?>)" title="<?= $sisdsc ?>"><a href="javascript:abrirsistema(<?= $sisid ?>)" ><?= $sisabrev ?></a></td>';
								<? } ?>
							
								<? if ( $sisid == $_SESSION['sisid'] && $key == (count($sistemas) - 1) ){ ?>
									menucontents = menucontents + '<td><img src="../includes/layout/<? echo $theme ?>/img/aba_right.png"></td>';
								<? } elseif ( $sisid != $_SESSION['sisid'] && $key == (count($sistemas) - 1) ){ ?>
									menucontents = menucontents + '<td><img src="../includes/layout/<? echo $theme ?>/img/aba_right_off.png"></td>';
								<? } ?>
							<? endforeach; ?>
							menucontents = menucontents + '</tr></table>';
							if( screen.width >= 800 && screen.width < 1024 ){
								var qtdAbas = 50;
							}else if( screen.width >= 1024 && screen.width < 1300 ) {
								var qtdAbas = 80;
							}else if( screen.width >= 1300 && screen.width < 1600 ) {
								var qtdAbas = 100;
							}else if( screen.width >= 1600 && screen.width < 1900 ) {
								var qtdAbas = 120;
							}else if( screen.width >= 1900 && screen.width < 3000 ) {
								var qtdAbas = 150;
							}else if( screen.width >= 3000 ) {
								var qtdAbas = 200;
							} else {
								var qtdAbas = 20;
							}
							var iedom=document.all||document.getElementById
							var leftdircode='onmousedown="moveleft('+qtdAbas+')"'
							var rightdircode='onmousedown="moveright('+qtdAbas+')"'
							if (scrolldir=="reverse"){
							var tempswap=leftdircode
							leftdircode=rightdircode
							rightdircode=tempswap
							}
							if (iedom)
							document.write('<span id="temp" style="visibility:hidden;position:absolute;top:-100;left:-5000">'+menucontents+'</span>')
							var actualwidth='';
							var cross_scroll, ns_scroll
							var loadedyes=0
							function fillup(){
							if (iedom){
							cross_scroll=document.getElementById? document.getElementById("test2") : document.all.test2
							cross_scroll.innerHTML=menucontents;
							actualwidth=document.all? parseInt(cross_scroll.offsetWidth) : parseInt(document.getElementById("temp").offsetWidth);
							}
							else if (document.layers){
							ns_scroll=document.ns_scrollmenu.document.ns_scrollmenu2
							ns_scroll.document.write(menucontents)
							ns_scroll.document.close()
							actualwidth=ns_scroll.document.width
							}
							loadedyes=1
							}
							
							function moveleft(i){
							if (loadedyes){
							if (iedom&&parseInt(cross_scroll.style.left)>(menuwidth-actualwidth)){
							cross_scroll.style.left=parseInt(cross_scroll.style.left)-scrollspeed+"px"
							}
							else if (document.layers&&ns_scroll.left>(menuwidth-actualwidth)) {
							ns_scroll.left-=scrollspeed
							}
							}
							if(i > 0) {
								lefttime=setTimeout("moveleft("+(i-1)+")",10)
							}
							
							}
							function moveright(i){
							if (loadedyes){
							if (iedom&&parseInt(cross_scroll.style.left)<0)
							cross_scroll.style.left=parseInt(cross_scroll.style.left)+scrollspeed+"px"
							else if (document.layers&&ns_scroll.left<0)
							ns_scroll.left+=scrollspeed
							}
							if(i > 0) {
								righttime=setTimeout("moveright("+(i-1)+")",10);
							}
							}
							if (iedom||document.layers){
							with (document){
							document.write('<table border="0" class="bg_linha_cinza" cellspacing="0 cellpadding="0" width="'+menuwidth+'" height="'+menuheight+'"><tr>')
							document.write('<td valign="top" id="dirleft" width="10"><table border="0" cellspacing="0" cellpadding="0"><tr><td style="cursor: pointer;background-color:#e6ebed;color:#285e78; border: 1px solid #555555;" '+rightdircode+'>&nbsp;«&nbsp;</td></tr></table></td>')
							document.write('<td width="'+menuwidth+'" height="'+menuheight+'" id="flutua1" class="bg_linha_cinza" valign="top">')
							if (iedom){
							document.write('<div id="flutua2" style="position:relative;width:'+menuwidth+';height:18;overflow:hidden;">')
							document.write('<div id="test2" style="position:absolute;left:0;top:0">')
							document.write('</div></div>')
							}
							else if (document.layers){
							document.write('<ilayer width='+menuwidth+' height='+menuheight+' name="ns_scrollmenu">')
							document.write('<layer name="ns_scrollmenu2" left=0 top=0></layer></ilayer>')
							}
							document.write('</td>')
							document.write('<td valign="top" class="bg_linha_cinza"  id="dirright"><table class="bg_linha_cinza"  border="0" cellspacing="0" cellpadding="0"><tr><td style="cursor: pointer;background-color:#e6ebed;color:#285e78; border: 1px solid #555555;" '+leftdircode+'>&nbsp;')
							document.write('»&nbsp;</td></tr></table>')
							document.write('</td></tr></table>')
							}
							}
							fillup();
							// Procedimento para ajustar a aba selecionada selecionado
							var tabela1 = document.getElementById("tb_menu");
							var countchar = 0;
							
							for(i = 1; i < tabela1.rows[0].cells.length; i=i+2) {
								var text = tabela1.rows[0].cells[i].innerHTML;
								countchar = countchar + text.length;
							
								if(tabela1.rows[0].cells[i].className == 'td_on') {
									var sel = countchar + (countchar/1.1); // padronização : 15 caracteres = 90px
								}
							}
							if(sel > menuwidth) {
								cross_scroll.style.left = (menuwidth + 10) - sel;
							}
							// Caso seja IE, utilizar o tamanho estimado
							if(actualwidth == 0) {
								actualwidth = (countchar*6 - 30);
							}
							if((countchar*6) < menuwidth) {
								document.getElementById("dirleft").style.display = 'none';
								document.getElementById("dirright").style.display = 'none';
							}
							
							
							window.onresize = redimensionarMenu;
							
							function redimensionarMenu() {
								var myWidth;
								if( typeof( window.innerWidth ) == 'number' ) {
							    	//Non-IE
								    myWidth = window.innerWidth;
								} else if( document.documentElement && ( document.documentElement.clientWidth || document.documentElement.clientHeight ) ) {
							    	//IE 6+ in 'standards compliant mode'
								    myWidth = document.documentElement.clientWidth;
								} else if( document.body && ( document.body.clientWidth || document.body.clientHeight ) ) {
								    //IE 4 compatible
							    	myWidth = document.body.clientWidth;
								}
								if(parseInt(cross_scroll.style.left) < 0) {
									cross_scroll.style.left = (myWidth - 80 - menuwidth) + parseInt(cross_scroll.style.left);
								}
								menuwidth = myWidth - 80;
							
								if((countchar*6) < menuwidth) {
									document.getElementById("dirleft").style.display = 'none';
									document.getElementById("dirright").style.display = 'none';
								} else {
									document.getElementById("dirleft").style.display = '';
									document.getElementById("dirright").style.display = '';
								}
							
								document.getElementById('flutua1').style.width = myWidth - 80;
								document.getElementById('flutua2').style.width = myWidth - 80;
							}
							var minutos=<? echo floor((MAXONLINETIME/60)); ?>;
							var seconds=<? echo floor(MAXONLINETIME%60); ?>;
							var campo = document.getElementById("cronometro");
							var campo_div = document.getElementById("cronometro_div");
							
									function startCountdown()
										{
										     	if (seconds<=0){
										            seconds=60;
												    minutos-=1;
												 }
												 if (minutos<=-1){
												    seconds=0;
												    seconds+=1;
												    campo.innerHTML="";
												    campo_div.innerHTML="Sessão expirada!";
													location.href = "../../logout.php";
												 } else{
													seconds-=1
													if(seconds < 10) {
														seconds = "0" + seconds;
													}
												    campo.innerHTML = " " + minutos+"min"+seconds+"s";
												    setTimeout("startCountdown()",1000);
												}
										}
										startCountdown();
										function simularUsuario(){
											var horizontal = 550;
											var vertical   = 320;
										
											var res_ver = screen.height;
											var res_hor = screen.width;
										
											var pos_ver_fin = (parseInt(res_ver) - parseInt(vertical) )/2;
											var pos_hor_fin = (parseInt(res_hor) - parseInt(horizontal) )/2;
										
											return window.open("../includes/simularUsuario.php","Simular Usuário","width="+horizontal+",height="+vertical+",top="+pos_ver_fin+",left="+pos_hor_fin);
										}
									</script>
								<?php endif; ?>
					</td>
				</tr>
				<tr>
					<td height="31px" class="submenu" ></td>
				</tr>
			</table>
			<table class="tbl_conteudo" width="100%" border="0" cellpadding="0" cellspacing="0">
				<tr>
					<td align="right" height="17" valign="middle" ></td>
				</tr>
				<tr>
					<td valign="top"  style="padding-bottom:15px;" >
                        <?php if ($modulo_atual != "") : ?>
						<?php 	
							$sql = sprintf("select distinct mnu1.mnudsc as menu1  , mnu2.mnudsc as menu2 , mnu3.mnudsc as menu3 , mnu1.mnulink as mnulink1  , mnu2.mnulink as mnulink2 , mnu3.mnulink as mnulink3    from seguranca.menu mnu1, seguranca.menu mnu2,  seguranca.menu mnu3 where mnu3.mnuidpai=mnu2.mnuid and mnu2.mnuidpai=mnu1.mnuid and mnu3.mnuid = %d", $modulo_atual);
							
							if (isset($_SESSION['rastro'][$modulo_atual]))
							{
								$rastro = $_SESSION['rastro'][$modulo_atual];
							}
							else 
							{
								$rastro = $db->pegaLinha($sql);
								$_SESSION['rastro'][$modulo_atual] = $rastro;
							}
								
							if ($rastro): 
						?>
		                    <div class="row" style="margin-top: 10px;">
		                        <div class="col-md-12">
		                            <div class="view">
		                                <ul class="breadcrumb">
		                                    <li>
		                                        <?php if($rastro['menulink1']): ?>
		                                            <a <?php if($url == $rastro['menulink1']) echo 'class="active"'; echo "href='{$rastro['menulink1']}'" ;?> contenteditable="true"><?php echo $rastro['menu1'] ?></a>
		                                        <?php else: ?>
		                                            <?php echo $rastro['menu1'] ?>
		                                        <?php endif ?>
		                                    </li>
		                                    <li>
		                                        <?php if($rastro['menulink2']): ?>
		                                            <a <?php if($url == $rastro['menulink2']) echo 'class="active"'; echo "href='{$rastro['menulink2']}'" ;?> contenteditable="true"><?php echo $rastro['menu2'] ?></a>
		                                        <?php else: ?>
		                                            <?php echo $rastro['menu2'] ?>
		                                        <?php endif ?>
		                                    </li>
		                                    <li>
		                                        <?php if($rastro['menulink3']): ?>
		                                            <a <?php if($url == $rastro['menulink3']) echo 'class="active"'; echo "href='{$rastro['menulink3']}'" ;?> contenteditable="true"><?php echo $rastro['menu3'] ?></a>
		                                        <?php else: ?>
		                                            <?php echo $rastro['menu3'] ?>
		                                        <?php endif ?>
		                                        <span class="divider"></span>
		                                    </li>
		                                </ul>
	                        	<?php endif; ?>
		                            </div>
		                        </div>
		                    </div>
<?php endif; ?>
<?php } ?>
<?php
    if (!IS_PRODUCAO):
        $host = $_SERVER['HTTP_HOST'];

        if(false !== strpos($host, 'simec-d')){
            $labelAmbiente = 'Ambiente de Desenvolvimento';
            $classCorAmbiente = 'barraAmbienteDivPrincipalCorFundoVerde';
        } else {
            $labelAmbiente = 'Ambiente Local';
            $classCorAmbiente = 'barraAmbienteDivPrincipalCorFundoAzul';
        }

        if($_SESSION['baselogin'] == 'simec_espelho_producao'){
            $labelBanco = 'Espelho';
            $classCorLabelBanco = 'barraAmbienteLabelBancoEspelho';
        } else {
            $labelBanco = 'Desenvolvimento';
            $classCorLabelBanco = 'barraAmbienteLabelBancoDesenv';
        }
        $labelBanco = $_SESSION['baselogin'] == 'simec_espelho_producao'? 'Espelho': 'Desenvolvimento';
?>
    <div id="divAmbiente" style="display: none;">
        <div class="barraAmbienteDivPrincipal <?php echo $classCorAmbiente; ?>">
            <table class="barraAmbienteTabela">
                <tr>
                    <td style="text-align: left;">
                        <span class="barraAmbienteTexto">
                            <?php echo $labelAmbiente ?>
                        </span>
                    </td>
                    <td style="text-align: right;">
                        <span class="barraAmbienteTexto">
                            Banco de dados:
                                <span class="barraAmbienteLabelBanco <?php echo $classCorLabelBanco; ?>">
                                    <?php echo $labelBanco; ?>(<?php echo $nome_bd; ?>)
                                </span>
                        </span>
                    </td>
                </tr>
            </table>
        </div>
    </div>
    <script>
        // Pega o conteudo da divAmbiente com os dados que serão exibidos na tela
        divAmbiente = document.getElementById('divAmbiente').innerHTML;
        
        // Verifica se a div do menu fixo existe
        if( document.getElementById('menubackmenu1') ){
            // Insere os dados de ambiente com formatação na barra de menu para ficar sempre visivel
            document.getElementById('menubackmenu1').innerHTML = divAmbiente;
        }
    </script>
<?php endif; ?>