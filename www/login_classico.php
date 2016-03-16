<?php


// Faz download de um dos arquivos solicitados
if( $_REQUEST["arquivo_login"] )
{
	// caminho do arquivo
	$path = "./";
	// recupera o nome e o tipo do arquivo
	switch($_REQUEST["arquivo_login"])
	{
		case 'comunicado':
			$file = "comunicado_pdde.pdf";
			$type = "application/pdf";
			break;
		case 'manual':
			$file = "manual_de_orientacao_pdde.pdf";
			$type = "application/pdf";
			break;
		case 'lista':
			$file = "lista_de_escolas_agua_pdde.pdf";
			$type = "application/pdf";
			break;
		case 'lista2':
			$file = "lista_de_escolas_campo_pdde.pdf";
			$type = "application/pdf";
			break;
		/*
		case 'pesquisa':
			$file = "pesquisa_educacao_campo.pdf";
			$type = "application/pdf";
			break;
		*/
	}

	// caminho completo
	$file = $path . $file;
	// cabe�alho
	header("Content-type: $type");
	header("Content-Disposition: attachment;filename=$file");
	// mostra o download
	readfile($file);
	// destr�i a vari�vel do formul�rio
	unset($_REQUEST["formulario"]);
	exit;
}

/**
 * Sistema Integrado de Planejamento, Or�amento e Finan�as do Minist�rio da Educa��o
 * Setor responsvel: DTI/SE/MEC
 * Autor: Cristiano Cabral <cristiano.cabral@gmail.com>
 * M�dulo: Seguran�a
 * Finalidade: Tela de apresenta��o. Permite que o usu�rio entre no sistema.
 * Data de cria��o: 24/06/2005
 * �ltima modifica��o: 24/08/2008
 */

//Verifica Temas

if(isset($_COOKIE["theme_simec"])){
	$theme = $_COOKIE["theme_simec"];


}

$_POST["theme_simec"] = 'natal';
if(isset($_POST["theme_simec"])){
	$theme = $_POST["theme_simec"];
	setcookie("theme_simec", $_POST["theme_simec"] , time()+60*60*24*30, "/");
}

// carrega as bibliotecas internas do sistema
include "config.inc";
require APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

// Valida o CPF, vindo do post
if($_POST['usucpf'] && !validaCPF($_POST['usucpf'])) {
	die('<script>
			alert(\'CPF inv�lido!\');
			history.go(-1);
		 </script>');
}

// abre conex�o com o servidor de banco de dados
$db = new cls_banco();

// executa a rotina de autentica��o quando o formul�rio for submetido
if ( $_POST['formulario'] ) {
	if(AUTHSSD) {
		include APPRAIZ . "includes/autenticarssd.inc";
	} else {
		include APPRAIZ . "includes/autenticar.inc";
	}
}

if ( $_REQUEST['expirou'] ) {
	$_SESSION['MSG_AVISO'][] = "Sua conex�o expirou por tempo de inatividade. Para entrar no sistema efetue login novamente.";
}


//Define um tema existente (padr�o), caso nenhum tenha sido escolhido

if(!$theme) {

	$diretorio = APPRAIZ."www/includes/layout";
	if(is_dir($diretorio)){
		if ($handle = opendir($diretorio)) {
		   while (false !== ($file = readdir($handle))) {
			  if ($file != "." && $file != ".." && $file != ".svn" && is_dir($diretorio."/".$file)) {
				  $dirs[] = $file;
			  }
		   }
		   closedir($handle);
		}
	}

	if($dirs) {
		// sorteia um tema para exibi��o
		$theme = $dirs[rand(0, (count($dirs)-1))];
		$_SESSION['theme_temp'] = $theme;
	}

}
?>
<!--
	Sistema Integrado de Monitoramento, Execu��o e Controle
	Setor responsvel: DTI/SE/MEC
	Finalidade: Tela de apresenta��o do sistema. Permite abrir uma sess�o no sistema.
	Autor: Alexandre Dourado
-->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="X-UA-Compatible" content="IE=7" />
<meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1">


<title>Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle</title>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<?php if(is_file( "includes/layout/".$theme."/include_login.inc" )) include "includes/layout/".$theme."/include_login.inc"; ?>
<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery.accordion.source.js"></script>
<script src="../includes/BeautyTips/excanvas.js" type="text/javascript"></script>
<script type="text/javascript" src="../includes/BeautyTips/jquery.bt.min.js"></script>


</head>

<body>
	<div id="tutorial_theme" style="display:none"><span style="color:red;font-weight:bold;">Novidade!</span><br>Agora voc� pode escolher o VISUAL do seu SIMEC, clique no �cone ao lado e experimente!</div>
	<? include "barragoverno_old.php"; ?>

<table width="100%" cellpadding="0" cellspacing="0" id="main">
<tr>
	<td width="50%" ><img src="/includes/layout/<? echo $theme ?>/img/logo.png" border="0" /></td>
	<td align="right" style="padding-right: 30px;padding-left:250px;" >

<!--		<img src="/includes/layout/--><?// echo $theme ?><!--/img/bt_temas.png" style="cursor:pointer" id="img_change_theme" alt="Alterar Tema" title="Alterar Tema" border="0" />-->
		<div style="display:none" id="menu_theme">
		<script>
/*
			$(document).ready(function() {
			        $().click(function () {
			        	$('#menu_theme').hide();
			        });
			        $("#img_change_theme").click(function () {
						$('#img_change_theme').btOff()
			        	$('#menu_theme').show();
			        	return false;
			        });
			        $("#menu_theme").click(function () {
			        	$('#menu_theme').show();
			        	return false;
			        });
			});

			function alteraTema(){
				document.getElementById('formTheme').submit();
			}
			*/
		</script>

		<form id="formTheme" action="" method="post" >

		Tema:
			<select class="select_ylw" name="theme_simec" title="Tema do SIMEC" onchange="alteraTema(this.value)" >
		            <?php include(APPRAIZ."www/listaTemas.php") ?>
	        </select>
		</form>
		</div>
	</td>
</tr>
<form id="formulario" name="formulario" method="post">

<input type="hidden" name="formulario" value="1"/>

<input type="hidden" id="arquivo_login" name="arquivo_login" value="" />

<tr>
  <td width="55%" valign="top">

  <!-- Lista de M�dulos-->
  <table width="98%" border="0" cellpadding="0" cellspacing="0" class="tabela_modulos">
  <tr>
  	<td class="td_bg">&nbsp;M�dulos - <small> lista de m�dulos</small></td>
  </tr>
  <tr>
	<td valign="middle" class="td_table_inicio">
    <div id="pageWrap" class="pageWrap">
	    <ul class="accordion">
		<?
		// buscando a lista de sistemas
		$sql = "SELECT sisid, sisabrev, sisdsc, sisfinalidade, sispublico, sisrelacionado
				FROM seguranca.sistema
				WHERE sisstatus='A' AND sismostra=true
				ORDER BY sisid";
		?>
		<? foreach ( $db->carregar( $sql ) as $sistema ) : ?>
		<? extract( $sistema ); ?>
		<li>
			<a href="javascript:void(0)" class="link"><span class="txt_azul_bold"><?= $sisabrev ?></span> - <?= $sisdsc ?></a>
			<div style="width:95%">
            <table width="100%" border="0" style="cursor: default" align="center" cellpadding="2" cellspacing="0">
				<tr>
					<td valign="top" width="24%" align="right" class="txt_laranja txt_padrao">Finalidade:</td>
					<td style="text-align: justify;" class="txt_padrao" valign="top" width="76%"><?= $sisfinalidade ?></td>
					<td rowspan="3" valign="top" align="right">
		               	<?if (montaLinkManual2($sisid)){?>
						<div class="botao1"><?= montaLinkManual2($sisid) ?></div>
						<?}?>
		               	<a href="javascript:janela('/geral/fale_conosco.php?sisid=<?= $sisid; ?>',550,600)" class="botao1">D�vidas</a>
		               	<a href="cadastrar_usuario.php?sisid=<? echo $sisid; ?>" class="botao2">Solicitar Cadastro</a>
	               </td>
				</tr>
				<tr>
					<td valign="top" align="right" class="txt_laranja txt_padrao">P&uacute;blico-Alvo:</td>
					<td valign="top" class="txt_padrao" ><?= $sispublico ?></td>
				</tr>
				<tr>
					<td valign="top" align="right" class="txt_laranja txt_padrao">Sistemas Relacionados:</td>
					<td valign="top" class="txt_padrao" ><?= $sisrelacionado ?></td>
				</tr>
            </table>
		</li>
		<?php endforeach; ?>
		</ul>
	</div>
    </td>

  </tr>
  </table>
  </td>

      <td width="30%" align="center" valign="top">
      <table width="92%" border="0" align="center" cellpadding="0" cellspacing="0" class="tabela_modulos">
        <tr>
          <td class="td_bg">&nbsp;Acesse o Sistema</td>
        </tr>
        <tr>
          <td height="106" align="center">
		  <? if ( $_SESSION['MSG_AVISO'] ): ?>
		  <div class="error_msg">
		  <ul><li><?= implode( '</li><li>', (array) $_SESSION['MSG_AVISO'] ); ?></li></ul>
		  </div>
		  <? endif; ?>
		  <? $_SESSION['MSG_AVISO'] = array(); ?>
          <!--Caixa de Login-->
          <table class="tbl_login" width="95%" border="0" cellspacing="0" cellpadding="3">
            <tr>
              <td style="font-weight: bold;"  width="13%" align="right">
              	CPF:
              </td>
              <td width="51%">
              	<input tabindex="1"  type="text" name="usucpf" value="" size="20" class="login_input" onkeypress="return controlar_foco_cpf( event );" onkeyup="this.value=mascaraglobal('###.###.###-##',this.value);" />
              </td>
              <td width="36%">
              	<a tabindex="3" class="botao2" href="javascript:enviar_formulario()" >Entrar</a>
              </td>
            </tr>
            <tr>
              <td style="font-weight: bold;" valign="middle"  width="13%" align="right">
              	SENHA:
              </td>
              <td valign="middle" width="51%">
              	<input tabindex="2" type="password" name="ususenha" class="login_input" autocomplete="off" size="20" onkeypress="return controlar_foco_senha( event );" />
              </td>
              <td valign="middle" width="36%">
              	<a tabindex="4" href="./cadastrar_usuario.php" class="botao1">Solicitar Cadastro</a>
              </td>
            </tr>
            <tr>
              <td colspan="3" align="left" class="txt_laranja" ><a class="link_laranja" href="recupera_senha.php" >Esqueceu a senha?</a></td>
            </tr>
          </table>
          <!--fim Caixa de Login -->

          </td>

        </tr>
        <tr>
          <td class="td_bg">&nbsp;Pr�mios</td>
        </tr>
        <tr>
          <td height="115" align="center">
          	<div id="premios">&nbsp;
				<a target="_blank" href="http://www.premio-e.gov.br/conteudo/580/?S%C3%A3o_divulgados_os_vencedores_do_Pr%C3%AAmio_e-GOV_2012" ><img style="cursor:pointer" src="/imagens/logo/premioe-gov2012.png" border="0" width="200px" height="60px" /></a>
				<a target="_blank" href="http://www.planoeditorial.com.br/anuariotigoverno/2011/index.html" ><img style="cursor:pointer" src="/imagens/logo/premiogovernoti2011.png" border="0" width="200px" height="60px" /></a>
	          	<a target="_blank" href="http://inovacao.enap.gov.br/index.php?option=com_content&task=blogcategory&id=51&Itemid=57" ><img style="cursor:pointer" src="/imagens/logo/selo-inovacao.gif" border="0" /></a>
				<a target="_blank" href="javascript:void(0);" ><img style="cursor:pointer" src="/imagens/logo/conip.gif" border="0" /></a>
	          	<a target="_blank" href="http://www.premio-e.gov.br/conteudo/432/?Ano_2009" ><img style="cursor:pointer" src="/imagens/logo/premioe-gov.png" border="0" /></a>
			</div>
          </td>
        </tr>
        <tr>
          <td class="td_bg">&nbsp;Informes</td>
        </tr>
        <!--<tr>
          <td height="115">
			<div id="informes"><strong><p align="left"><font color="red" size="4">AVISO!</font></p>
			          <font color="red" size="3">  <p align="left"><strong>Prezados usu�rios, <br>
			  O SIMEC estar� em manuten��o de 12 horas e 30 minutos at� �s 13 horas do dia 8 de novembro.<br>
			</strong></p></font></strong></div>
          </td>
        </tr>
		-->
		<tr>
	<td height="115">
	  <div id="informes">
	  					<p align="left">
							<table border=0 cellspacing=0 cellpadding=0 width='100%' >
							 <tr >
							  <td width=480 style='width:360.0pt;border:solid windowtext 1.0pt; padding:0cm 5.4pt 0cm 5.4pt;height:71.85pt;border-color:-moz-use-text-color'>
								    <br>
								    <p align="left"><div style="float:left"><img src="/imagens/seta_galeria1.gif" align="bottom">&nbsp;</div> <div style="float:left"> <b>PRONACAMPO</b></div></p>
						           	<p align="center"><font color=red size=3><strong><br><b>ATEN��O</b></strong></font></p>
									<p align="justify">
										Informo que as a��es do Programa Nacional de Educa��o do Campo ? PRONACAMPO podem ser acessadas por meio do endere�o eletr�nico: <a href="http://pronacampo.mec.gov.br" target="_blank">http://pronacampo.mec.gov.br</a>
									</p>
									<br>
							  </td>
							 </tr>
							</table>
					  	</p>

			      		<p align="left">
		                    <table border=0 cellspacing=0 cellpadding=0 width='100%' >
		                        <tr>
		                            <td width=480 style='width:360.0pt;border:solid windowtext 1.0pt; padding:0cm 5.4pt 0cm 5.4pt;height:71.85pt;border-color:-moz-use-text-color'>
		                                <p align="center"><font color=red size=3><strong><br><b>ATEN��O</b></strong></font></p>
		                                <p align="left">
		                                    <div style="float:left">
		                                        <img src="/imagens/seta_galeria1.gif" align="bottom">
		                                        &nbsp;<b>Edital n� 03/2013 - Pr�-sele��o de munic�pios para implanta��o de curso de medicina</b>
		                                    </div>
		                                </p>
		                                <br>
		                                <p align="justify">Est� aberta a etapa de recurso para os munic�pios que n�o foram pr�-selecionados para implanta��o do curso de medicina, conforme Edital n� 3/2013 e Portaria n� 646/2013.</p>
		                                <p align="justify">De acordo com a Portaria n� 646/2013, n�o ser�o considerados como fundamento de recurso os casos previstos abaixo:</p>
		                                <p align="justify">I.munic�pios que n�o finalizaram a inscri��o no SIMEC, permanecendo com o status em "em preenchimento pelo munic�pio";</p>
		                                <p align="justify">II.munic�pios que n�o realizaram inscri��o no SIMEC e enviaram documentos apenas por via postal;</p>
		                                <p align="justify">III.munic�pios que postaram documentos fora do prazo estabelecido no Edital MEC n� 03, de 2013, conforme comprova��o dos correios;</p>
		                                <p align="justify">IV.munic�pios que n�o atendem ao crit�rio populacional de 70 (setenta) mil ou mais habitantes;</p>
		                                <p align="justify">V.munic�pios que possuem curso de medicina em seu territ�rio.</p>
		                                <p align="justify">O recurso dever� ser dirigido ao Secret�rio de Regula��o e Supervis�o da Educa��o Superior do Minist�rio da Educa��o e ser apresentado em formato PDF a ser anexado em campo pr�prio no endere�o simec.mec.gov.br, m�dulo PAR MAIS M�DICOS, at� o dia 9 de dezembro de 2013.</p>
		                                <p align="justify">A SERES n�o analisar� recurso impresso ou encaminhado em formato incompat�vel com o disposto na Portaria.</p>
		                            </td>
		                        </tr>
		                    </table>
		                </p>

						<p align="left">
							<table border=0 cellspacing=0 cellpadding=0 width='100%' >
							 <tr >
							  <td width=480 style='width:360.0pt;border:solid windowtext 1.0pt; padding:0cm 5.4pt 0cm 5.4pt;height:71.85pt;border-color:-moz-use-text-color'>
								    <br>
								    <p align="left"><div style="float:left"><img src="/imagens/seta_galeria1.gif" align="bottom">&nbsp;</div> <div style="float:left"> <b>PRONACAMPO</b></div></p>
						           	<p align="center"><font color=red size=3><strong><br><b>ATEN��O</b></strong></font></p>
									<br>Comunicamos que o Minist�rio da Educa��o por meio da Secretaria de Educa��o Continuada, Alfabetiza��o, Diversidade e Inclus�o ? SECADI/ME promover� II Reuni�o T�cnica com os Gestores Municipais para apresenta��o das a��es do Programa Nacional de Educa��o do Campo ? PRONACAMPO, a realizar-se no per�odo de 25 a partir das 19 horas , 26 e 27 a partir das 09horas �s 18horas de setembro de 2013 em Bras�lia/DF.
									<br><br>Outrossim, informamos que os munic�pios que n�o participar�o da referida reuni�o, poder�o assistir a mesma em tempo real atrav�s do link <a href="http://portal.mec.gov.br/secadi/transmissao" target="_blank">http://portal.mec.gov.br/secadi/transmissao</a>.
									</b>
									<br><br>
							  </td>
							 </tr>
							</table>
					  	</p>


						<p align="left">
							<table border=0 cellspacing=0 cellpadding=0 width='100%' >
							 <tr >
							  <td width=480 style='width:360.0pt;border:solid windowtext 1.0pt; padding:0cm 5.4pt 0cm 5.4pt;height:71.85pt;border-color:-moz-use-text-color'>
								    <br>
								    <p align="left"><div style="float:left"><img src="/imagens/seta_galeria1.gif" align="bottom">&nbsp;</div> <div style="float:left"><b>Esplanada Sustent�vel</b></div></p>
						           	<p align="center"><font color=red size=3><strong><br><br><b>PROJETO ESPLANADA SUSTENT�VEL</b><br><br></strong></font></p>
									<b>
									Informamos que o m�dulo Esplanada Sustent�vel, utilizado para cadastrar os contratos, acompanhamento das despesas pactuadas e plano de a��o , j� est� em funcionamento.
									<!--
									<br>
									O manual de utiliza��o do sistema pode ser visualizado clicando-se no seguinte link: <a href="/Manual_PES.pdf" target="_blank">Manual</a>.</font>
									-->
									<br><br>

							  </td>
							 </tr>
							</table>
					  	</p>

						<p align="left">
							<table border=0 cellspacing=0 cellpadding=0 width='100%' >
							 <tr >
							  <td width=480 style='width:360.0pt;border:solid windowtext 1.0pt; padding:0cm 5.4pt 0cm 5.4pt;height:71.85pt;border-color:-moz-use-text-color'>
								    <br>
								    <p align="left"><div style="float:left"><img src="/imagens/seta_galeria1.gif" align="bottom">&nbsp;</div> <div style="float:left"> <b>PNLD</b></div></p>
						           	<p align="center"><font color=red size=3><strong><br><b>ATEN��O</b><br><br></strong></font></p>

									<b>
									Aos detentores de direito autoral das obras did�ticas inscritas no PNLD/2014,
									<br><br>
									A Secretaria de Educa��o B�sica informa a divulga��o do resultado do processo de avalia��o realizado no �mbito do PNLD/2014. Os pareceres de todas as obras avaliadas est�o dispon�veis ao representante da editora cadastrado no SIMAD/FNDE.
									<br><br>
									As informa��es relativas �s fases de interposi��o de recursos e de corre��o de falhas pontuais est�o dispon�veis na portaria do resultado publicado no D.O.U e no M�dulo PNLD.
									</b>
									<br><br>

							  </td>
							 </tr>
							</table>
					  	</p>

						<p align="left">
							<table border=0 cellspacing=0 cellpadding=0 width='100%' >
							 <tr >
							  <td width=480 style='width:360.0pt;border:solid windowtext 1.0pt; padding:0cm 5.4pt 0cm 5.4pt;height:71.85pt;border-color:-moz-use-text-color'>
								    <br>
								    <p align="left"><div style="float:left"><img src="/imagens/seta_galeria1.gif" align="bottom">&nbsp;</div> <div style="float:left"><b>Programa��o Or�ament�ria</b></div></p>
						           	<p align="center"><font color=red size=3><strong><br><br><b>TERMOS DE COOPERA��O</b><br><br></strong></font></p>
									<b>
									Informamos que o m�dulo de descentraliza��o de cr�ditos, utilizado para se cadastrar os TERMOS DE COOPERA��O firmados com as secretarias do Minist�rio da Educa��o, j� est� em funcionamento no m�dulo de programa��o or�ament�ria.<br>
									O manual de utiliza��o do sistema pode ser visualizado clicando-se no seguinte link: <a href="/Manual_do_Modulo_de_Descentralizacao.pdf" target="_blank">Manual</a>.</font>
									<br><br>

							  </td>
							 </tr>
							</table>
					  	</p>

						<p align="left">
							<table border=0 cellspacing=0 cellpadding=0 width='100%' >
							 <tr >
							  <td width=480 style='width:360.0pt;border:solid windowtext 1.0pt; padding:0cm 5.4pt 0cm 5.4pt;height:71.85pt;border-color:-moz-use-text-color'>
								    <br>
								    <p align="left"><div style="float:left"><img src="/imagens/seta_galeria1.gif" align="bottom">&nbsp;</div> <div style="float:left"><b>SISPACTO</b></div></p>
						           	<p align="center"><font color=red size=3><strong><br><br><b>PACTO NACIONAL DE ALFABETIZA��O NA IDADE CERTA<br><br>ATEN��O</b><br><br></strong></font></p>
									<b>
									Avisamos aos Estados e Munic�pios do PACTO que dia <font color=#156C94>15 de Fevereiro</font> encerra-se o prazo para troca de Orientadores de Estudos do PACTO.<br>
									Assim, a partir dessa data, o Sispacto estar� fechado para a execu��o da a��o: <font color=#156C94>Efetuar troca de Orientadores de Estudo.</font>
									<br><br>

							  </td>
							 </tr>
							</table>
					  	</p>

						<p align="left">
							<table border=0 cellspacing=0 cellpadding=0 width='100%' >
							 <tr >
							  <td width=480 style='width:360.0pt;border:solid windowtext 1.0pt; padding:0cm 5.4pt 0cm 5.4pt;height:71.85pt;border-color:-moz-use-text-color'>
								    <br>
								    <p align="left"><div style="float:left"><img src="/imagens/seta_galeria1.gif" align="bottom">&nbsp;</div> <div style="float:left"><b>Mais Educa��o</b></div></p>
						           	<p align="center"><font color=red size=3><strong><br><br><b>Cadastro de Novas Escolas ? Diretores<br><br>ATEN��O</b><br><br></strong></font></p>

									<b>
									As escolas interessadas em aderir ao Programa Mais Educa��o para o ano de 2013 dever�o, por meio de seu Diretor (a), solicitar o cadastro no <a href="http://simec.mec.gov.br" target="_blank">http://simec.mec.gov.br/</a> no campo ACESSO O SISTEMA ? Solicitar Cadastro. <br>O diretor (a) deve selecionar o M�dulo ESCOLA, inserir o CPF e continuar. Em seguida, o sistema solicitar� os dados pessoais e um perfil, selecionar CADASTRADOR MAIS EDUCA��O. > Enviar solicita��o.
									<br><br>Ap�s solicitado o cadastro do(a) Diretor(a) um t�cnico da Secretaria de Educa��o dever� acessar a p�gina principal do Simec, pois ser� ele (a) respons�vel por liberar a senha dos diretores.
									</b>
									<br><br>

							  </td>
							 </tr>
							</table>
					  	</p>

						<p align="left">
							<table border=0 cellspacing=0 cellpadding=0 width='100%' >
							 <tr >
							  <td width=480 style='width:360.0pt;border:solid windowtext 1.0pt; padding:0cm 5.4pt 0cm 5.4pt;height:71.85pt;border-color:-moz-use-text-color'>
								    <br>
								    <p align="left"><div style="float:left"><img src="/imagens/seta_galeria1.gif" align="bottom">&nbsp;</div> <div style="float:left"> <b>PAR</b></div></p>
						           	<p align="center"><font color=red size=3><strong><br><b>ATEN��O</b><br><br></strong></font></p>

									<b>
									Sr(a) Usu�rio do M�dulo PAR,
									<br><br>Lembramos que o acesso ao PAR municipal pode ser liberado para o(a) prefeito municipal, para o(a) dirigente municipal de educa��o (DME) e para apenas um(a) t�cnico(a) indicado(a) pelo(a) DME. No caso dos estados, para o(a) secret�rio(a) estadual de educa��o e para os t�cnicos indicados por ele(a).
									<br><br>Os t�cnicos da secretaria de educa��o, engenheiros, diretores de escola ou outros usu�rios de estados e munic�pios que n�o foram devidamente autorizados pelo gestor permanecer�o bloqueados.
									<br><br>No caso de escolas benefici�rias de a��es como a constru��o de quadras escolares, cobertura de quadras existentes ou pelo programa �gua na Escola, a apresenta��o do pleito ser� feita pelo secret�rio de educa��o e sua equipe, n�o pela escola.
									</b>
									<br><br>

							  </td>
							 </tr>
							</table>
					  	</p>

						<p align="left">
							<table border=0 cellspacing=0 cellpadding=0 width='100%' >
							 <tr >
							  <td align="center" width=480 style='width:360.0pt;border:solid windowtext 1.0pt; padding:0cm 5.4pt 0cm 5.4pt;height:71.85pt;border-color:-moz-use-text-color'>
								    <br>
								    <p align="left"><div style="float:left"><img src="/imagens/seta_galeria1.gif" align="bottom">&nbsp;</div> <div style="float:left"> <b>Ensino M�dio Inovador</b></div></p>
						           	<p align="center"><font color=red size=3><strong>
						           					<br>
						           					<br>
														<b>A V I S O</b><BR><BR>
														<!--O prazo para envio do Plano de Atendimento Global - PAG foi prorrogado para <b>31 de agosto de 2012</b>.-->
														<!--O m�dulo do Programa Ensino M�dio Inovador estar� dispon�vel para ajustes, mediante solicita��o do Comit� Gestor de cada Secretaria de Educa��o junto a COEM/SEB/MEC.-->
														O m�dulo do Programa Ensino M�dio Inovador est� dispon�vel para ajustes no PRC.<br>Informa��es sobre recursos financeiros transferidos FNDE, recursos financeiros utilizados no PRC e saldo dispon�vel, acesse a Matriz Or�ament�ria do PRC de sua escola.
						           	</strong></font></p>
							  </td>
							 </tr>
							</table>
					   </p>

					  <p align="left">
							<table border=0 cellspacing=0 cellpadding=0 width='100%' >
							 <tr >
							  <td align="center" width=480 style='width:360.0pt;border:solid windowtext 1.0pt; padding:0cm 5.4pt 0cm 5.4pt;height:71.85pt;border-color:-moz-use-text-color'>
								    <br>
								    <p align="left"><div style="float:left"><img src="/imagens/seta_galeria1.gif" align="bottom">&nbsp;</div> <div style="float:left"> <b>PDE INTERATIVO</b></div></p>
						           	<p align="center"><font color=red size=3><strong>
						           					<br>
														<b>ATEN��O</b><br><br>
														Para acessar o PDE Interativo, fa�a o seu login no novo endere�o: http://pdeinterativo.mec.gov.br
						           	</strong></font></p>
							  </td>
							 </tr>
							</table>
					  </p>


						<!--
					   <p align="left">
							<table border=0 cellspacing=0 cellpadding=0 width='100%' >
							 <tr >
							  <td align="center" width="90"  style='border:solid windowtext 1.0pt;padding:0cm 5.4pt 0cm 5.4pt;height:61.85pt'>
							  	<img border=0 width="96" height="79" src="/imagens/st.gif">
							  </td>
							  <td align="center" width=480 style='width:360.0pt;border:solid windowtext 1.0pt;border-left:none;padding:0cm 5.4pt 0cm 5.4pt;height:101.85pt;border-color:-moz-use-text-color'>
								  <b><span style='font-size:11.0pt;line-height:200%;font-family:"Arial","sans-serif";color:navy'>
								  	3�&nbsp;&nbsp;VIDEOCONFER�NCIA - 2011
								  </span></b>
								  <br>
								  <b><span style='font-size:11.0pt;font-family:"Arial","sans-serif";color:black'>
								  	PST / Mais Educa��o
									<br><br>
									<font color="navy">28/06/2011</font>
									<br>
									<font color="black">14h30 �s 17h30</font>
								  </span></b>
								  <br>
								  <br>
								  <b><u><span style='font-family:"Arial","sans-serif";color:red'><a href="javascript:void(0);" onclick="montaSegTempo();">Clique aqui</a>
								  	<a href="http://www.esporte.gov.br/snee/segundotempo/maiseducacao/videoconferencia32011.jsp" target="_blank"><font color='red'>Clique aqui para mais informa��es</font></a></span></u></b>
							  </td>
							 </tr>
							</table>
					   </p>
					    -->

						<!--
						<p align="left">
							<table border=0 cellspacing=0 cellpadding=0 width='100%' >
							 <tr >
							  <td align="center" width=480 style='width:360.0pt;border:solid windowtext 1.0pt; padding:0cm 5.4pt 0cm 5.4pt;height:71.85pt;border-color:-moz-use-text-color'>
								    <br>
								    <p align="left"><div style="float:left"><img src="/imagens/seta_galeria1.gif" align="bottom">&nbsp;</div> <div style="float:left; "> <font color="red"><b>LEMBRETE - CRONOGRAMA DO PAC 2 (M�DULO PAR 2010)</b></font></div></p>
						           	 <p align="left">
						           	 	<b>
						           	 	<br><br>
						           	 	<div style="text-align: left">
						           	 	<li><font color="red">Preenchimento do PAC 2 - Proinf�ncia, Constru��o de Quadras Escolares Cobertas e Cobertura de Quadras Escolares - 15 de setembro a 30 de novembro de 2011</font></li>
										</div>
										</b>
			           				 </p>
						           	<br>
							  </td>
							 </tr>
							</table>
					   </p>
					    -->

						<!--
					    <p align="left">
							<table border=0 cellspacing=0 cellpadding=0 width='100%' >
							 <tr >
							  <td align="center" width=480 style='width:360.0pt;border:solid windowtext 1.0pt; padding:0cm 5.4pt 0cm 5.4pt;height:71.85pt;border-color:-moz-use-text-color'>
								    <br>
								    <p align="left"><div style="float:left"><img src="/imagens/seta_galeria1.gif" align="bottom">&nbsp;</div> <div style="float:left; "> <b>LEMBRETE - CRONOGRAMA DE ADES�ES A PROGRAMAS NO PAR 2010</b></div></p>
						           	 <p align="left">
						           	 	<b>
						           	 	<br><br>
						           	 	<div style="text-align: left">
						           	 	<li>Ades�o ao Programa Proinfantil (forma��o) - 13 a 30 de setembro de 2011</li>
										<br><li>Ades�o ao Programa Nacional Escola de Gestores da Educa��o B�sica P�blica (forma��o) - 19 de setembro a 14 de outubro de 2011</li>
										<br><li>Preenchimento do PAC 2 ? Proinf�ncia, Constru��o de Quadras Escolares Cobertas e Cobertura de Quadras Escolares - 15 de setembro a 30 de novembro de 2011</li>
										</div>
										</b>
			           				 </p>
						           	<br>
							  </td>
							 </tr>
							</table>
					   </p>
					    -->

					   <!--
					    <p align="left">
							<table border=0 cellspacing=0 cellpadding=0 width='100%' >
							 <tr >
							  <td align="center" width=480 style='width:360.0pt;border:solid windowtext 1.0pt; padding:0cm 5.4pt 0cm 5.4pt;height:71.85pt;border-color:-moz-use-text-color'>
								    <br>
								    <p align="left"><div style="float:left"><img src="/imagens/seta_galeria1.gif" align="bottom">&nbsp;</div> <div style="float:left; "> <b>SECRETARIA DE EDUCA��O CONTINUADA, ALFABETIZA��O, DIVERSIDADE e</b></div></p>
						           	 <p align="left">
						           	  <b>INCLUS�O</b>
						           	 	<br><br>
						           	 	<strong><font color=black>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
			           					Senhor (a) Prefeito (a), est&aacute; dispon&iacute;vel na p&aacute;gina do FNDE a rela&ccedil;&atilde;o das escolas pass&iacute;veis de atendimento em 2011 com os recursos do Programa Dinheiro Direto na Escola (PDDE �gua na Escola e PDDE - Campo). Para tanto, acessar <a href="http://www.fnde.gov.br" target="_blank">www.fnde.gov.br</a> no menu ?Programas?, clicar em, Dinheiro Direto na Escola, em seguida, em Legisla��o, e, Resolu��o n�. 26/2011 e Resolu��o n�. 28/2011, respectivamente.
			           					<br><br>
			           					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
			           					Encontra-se tamb�m dispon�vel o ?Manual de Orienta��o? para auxiliar as Unidades Executoras (UEx) das escolas benefici�rias sobre os procedimentos de planejamento, execu��o e presta��o de contas dos recursos transferidos. Cabe ressaltar que o prazo para atualiza��o dos dados cadastrais da UEx, no Sistema PDDEweb encerra em 31/10/2011.
			           					<br><br>
			           					 Clique nos arquivos abaixo para fazer os downloads:
			           					 <br>
			           					 <br>Arquivo 1: <a href="javascript:void(0);" onclick="abreArquivo('comunicado');">Comunicado</a>
			           					 <br>Arquivo 2: <a href="javascript:void(0);" onclick="abreArquivo('lista');">Lista de Escolas PDDE �gua na Escola</a>
			           					 <br>Arquivo 3: <a href="javascript:void(0);" onclick="abreArquivo('lista2');">Lista de Escolas PDDE Campo</a>
			           					 </font>
			           					 </strong>
			           				 </p>
						           	<br>
							  </td>
							 </tr>
							</table>
					   </p>
						 -->

						<!--
					    <p align="left">
							<table border=0 cellspacing=0 cellpadding=0 width='100%' >
							 <tr >
							  <td align="center" width=480 style='width:360.0pt;border:solid windowtext 1.0pt; padding:0cm 5.4pt 0cm 5.4pt;height:71.85pt;border-color:-moz-use-text-color'>
								    <br>
								    <p align="left"><div style="float:left"><img src="/imagens/seta_galeria1.gif" align="bottom">&nbsp;</div> <div style="float:left"> <b>ESCOLA - MAIS EDUCA��O</b></div></p>
						           	<p align="left"><strong>
						           					<br>
						           					<br>
						           					<font color=red>
							           					Aten��o: Informamos que o Relat�rio Geral Consolidado de cada Munic�pio/ Estado dever� ser impresso e encaminhado via sedex para o seguinte endere�o:

														<br><br>
														Esplanada dos Minist�rios Bloco L<br>
														Anexo II 3� andar, sala 300<br>
														Diretoria de Concep��es e Orienta��es Curriculares para Educa��o B�sica - DCOCEB   Bras�lia - DF<br>
														Cep: 70047-900<br>
														Programa Mais Educa��o<br>
														<br><br>

														OBS: Ressaltamos a import�ncia da assinatura e carimbo nos Relat�rios Consolidados.
														<br>
														Para Munic�pios assinatura e carimbo do Prefeito e para os Estados do Secret�rio Estadual de Educa��o.
						           					</font>
						           	</strong></p>
						           	<br>
							  </td>
							 </tr>
							</table>
					   </p>
					    -->

						<!--
					    <p align="left">
							<table border=0 cellspacing=0 cellpadding=0 width='100%' >
							 <tr >
							  <td align="center" width=480 style='width:360.0pt;border:solid windowtext 1.0pt; padding:0cm 5.4pt 0cm 5.4pt;height:71.85pt;border-color:-moz-use-text-color'>
								   	<br>
								    <p align="left"><div style="float:left"><img src="/imagens/seta_galeria1.gif" align="bottom">&nbsp;</div> <div style="float:left"> <b>Senhor (a) Secret�rio (a) Municipal,</b></div></p>

						           	<p align="left"><strong>
						           					<br><br>
													Informamos a prorroga��o do prazo para envio das respostas relativa � ?PESQUISA DE DADOS SOBRE A EDUCA��O INFANTIL DO CAMPO? para 31/03/2011 (instrumento anexo), dispon�vel tamb�m na p�gina do MEC/SECAD/Programas e A��es/Educa��o Infantil do Campo e  solicitamos a devolu��o para o seguinte endere�o:
													<br><br>
													Prof� Gilmara da Silva<br>
													MEC/SECAD/DEDI-CGEC<br>
													gilmara.dasilva@mec.gov.br / profgilmaradasilva@hotmail.com<br>
													(47) 9142-1102 - (47) 3348-4496
													<br><br>
													Maiores informa��es:<br>
													Coordena��o Geral de Educa��o do Campo<br>
													Esplanada dos Minist�rios - Bloco L, Anexo I, 4� andar, Sala 402, 70.047-900    Bras�lia-DF 		(61) 2022 9302/9011.<br>
													E-mail: coordenacaoeducampo@mec.gov.br ou mariajoselma@mec.gov.br

													<br><br>
						           					 Clique no arquivo abaixo para fazer o download:
						           					 <br>
						           					 <br>Arquivo: <a href="javascript:void(0);" onclick="abreArquivo('pesquisa');">Pesquisa</a>
					           		</strong></p>
					           		<br>
							  </td>
							 </tr>
							</table>
					   </p>
						 -->

			           <!--
					<br>

					   <p align="left"><b>SECRETARIA DE EDUCA��O CONTINUADA, ALFABETIZA��O E DIVERSIDADE</b></p>
			           <p align="left"><strong><font color=red>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
			           					Senhor (a) Prefeito (a), est&aacute; dispon&iacute;vel na p&aacute;gina do FNDE e Anexo a rela&ccedil;&atilde;o das escolas pass&iacute;veis de atendimento em 2010 com os recursos do Programa Dinheiro Direto na Escola (PDDE - Campo), bem como o Manual de Orienta&ccedil;&atilde;o para auxiliar as Unidades Executoras (UEx) das escolas benefici&aacute;rias sobre os procedimentos de planejamento, execu&ccedil;&atilde;o e presta&ccedil;&atilde;o de contas dos recursos transferidos.
			           					<br><br>
			           					 Clique nos arquivos abaixo para fazer os downloads:
			           					 <br>
			           					 <br>Arquivo 1: <a href="javascript:void(0);" onclick="abreArquivo('comunicado');">Comunicado</a>
			           					 <br>Arquivo 2: <a href="javascript:void(0);" onclick="abreArquivo('lista');">Lista de Escolas</a>
			           					 <br>Arquivo 3: <a href="javascript:void(0);" onclick="abreArquivo('manual');">Manual de Orienta&ccedil;&atilde;o</a>
			           	</font></strong></p>
					 -->

			</div>
          </td>
        </tr>


      </table>
      </td>
  </tr>

	<tr>
	  <td colspan="2" class="rodape"> Data do Sistema: <? echo date("d/m/Y - H:i:s") ?></td>
  </tr>
</table>

</form>

</body>
</html>


<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>

<script language="javascript">

	$('#img_change_theme').bt({
  		trigger: 'none',
  		contentSelector: "$('#tutorial_theme')",
  		width: 200,
  		shadow: true,
	    shadowColor: 'rgba(0,0,0,.5)',
	    shadowBlur: 8,
	    shadowOffsetX: 4,
	    shadowOffsetY: 4
	});

$(document).ready(function () {
	$('#img_change_theme').btOn();
	window.setTimeout("$('#img_change_theme').btOff()", 10000);
});

	if ( document.formulario.usucpf.value == '' ) {
		document.formulario.usucpf.focus();
	} else {
		document.formulario.ususenha.focus();
	}

	function enviar_formulario() {
		if ( validar_formulario() ) {
			document.formulario.submit();
		}
	}

	function validar_formulario() {
		var validacao = true;
		var mensagem = '';
		if ( !validar_cpf( document.formulario.usucpf.value ) ) {
			mensagem += '\nO cpf informado n�o � v�lido.';
			validacao = false;
		}
		if ( document.formulario.ususenha.value == "" ) {
			mensagem += '\n� necess�rio preencher a senha.';
			validacao = false;
		}
		if ( !validacao ) {
			alert( mensagem );
		}

		//limpa variavel de download
		var arquivo = document.getElementById("arquivo_login");
		arquivo.value = "";

		return validacao;
	}

	function controlar_foco_cpf( evento ) {
		if ( window.event || evento.which ) {
			if ( evento.keyCode == 13) {
				return document.formulario.ususenha.focus();
			};
		} else {
			return true;
		}
	}

	function controlar_foco_senha( evento ) {
		if ( window.event || evento.which ) {
			if ( evento.keyCode == 13) {
				return enviar_formulario();
			};
		} else {
			return true;
		}
	}

	function abreArquivo(arq)
	{
		var form	= document.getElementById("formulario");
		var arquivo = document.getElementById("arquivo_login");

		arquivo.value = arq;
		form.submit();
	}

	/*** INICIO SHOW MODAL ***/
	function montaShowModal() {
		var alert='';
		alert += '<p align=center style=font-size:15;><font size=4 color=red><b>Aten��o!</b></font><br>Seu navegador de internet est� ultrapassado.<br/><br/>Em breve vamos descontinuar o suporte para Internet Explorer 6 e vers�es anteriores.<strong><br/><br/> Atualize seu navegador para obter uma experi�ncia on-line mais rica, sugerimos algumas op��es para download nos links abaixo:</strong></p>';
		alert += '<p><a target=_blank href=http://www.google.com/chrome/index.html?brand=CHNY&amp;utm_campaign=en&amp;utm_source=en-et-youtube&amp;utm_medium=et><img src=../imagens/browsers_chrome.png border=0></a> <a target=_blank href=http://www.microsoft.com/windows/internet-explorer/default.aspx><img src=../imagens/browsers_ie.png border=0></a> <a target=_blank href=http://www.mozilla.com/?from=sfx&amp;uid=267821&amp;t=449><img src=../imagens/browsers_firefox.png border=0></a></p>';
		alert += '<p align=center><input type=button value=Fechar onclick=closeMessage();></p>';
		displayStaticMessage(alert,false,'280');
		return false;
	}

	function displayStaticMessage(messageContent,cssClass,height) {
		messageObj = new DHTML_modalMessage();	// We only create one object of this class
		messageObj.setShadowOffset(5);	// Large shadow
		messageObj.setHtmlContent(messageContent);
		messageObj.setSize(570,height);
		messageObj.setCssClassMessageBox(cssClass);
		messageObj.setSource(false);	// no html source since we want to use a static message here.
		messageObj.setShadowDivVisible(false);	// Disable shadow for these boxes
		messageObj.display();
	}

	function closeMessage() {
		messageObj.close();
	}
	/*** FIM SHOW MODAL ***/

	/*
	function montaSegTempo() {
		var alert='';
		alert += '<center><font color=blue><b>2� Videoconfer�ncia Nacional</b></font></center>';
		alert += '<br><br>Informamos que no dia <b>16 de Novembro</b>, �s <b>15h</b> (hor�rio de Bras�lia), teremos a <u><b>2� videoconfer�ncia do PST no Mais Educa��o</b></u> com o intuito de darmos continuidade �s orienta��es sobre o desenvolvimento pedag�gico das atividades em sua escola.';
		alert += '<br><br>Esta 2� videoconfer�ncia ter� como principal objetivo orientar aos <font color=red><b>monitores</b></font> sobre o desenvolvimento das atividades a serem desenvolvidas pelo PST/Mais educa��o.';
		alert += '<br><br>Mobilize as Secretarias de Educa��o (Estadual e Municipal), gestores das escolas, professores e monitores. <font color=red><b>Todos est�o convocados!</b></font>';
		alert += '<br><br>Link de transmiss�o: <a href="http://portal.mec.gov.br/secad/transmissao" target="_blank"><b>http://portal.mec.gov.br/secad/transmissao</b></a>';
		alert += '<br><br>Aproveitamos para agradecer a todos que participaram da 1� Videoconfer�ncia realizada no dia 07 de Outubro de 2010. Todos puderam colaborar com perguntas e sugest�es.';
		alert += '<br><br>A participa��o efetiva expressa o quanto cada professor e cidad�o envolvido no PST/Mais Educa��o acredita na import�ncia da coletividade, da coes�o e da democracia para efic�cia deste trabalho junto � sociedade.';
		alert += '<br><br>Em caso de d�vida entre em contato pelo telefone (61) 3217-9490, ou pelo <br> email: <a href="mailto:segundotempo_maisedu@esporte.gov.br">segundotempo_maisedu@esporte.gov.br</a>';
		alert += '<br><br>Contamos com a participa��o de todos!';
		alert += '<br><br>Atenciosamente,';
		alert += '<br><br>Equipe Gestora do Programa Segundo Tempo.';
		alert += '<p align=center><input type=button value=Fechar onclick=closeMessage();></p>';
		displayStaticMessage(alert,false,'490');
		return false;
	}
	*/
</script>

<?php
// verificando se o browser � IE6 ou inferior
require APPRAIZ . "includes/classes/browser.class.inc";
$browser = new Browser();
if( $browser->getBrowser() == Browser::BROWSER_IE && $browser->getVersion() <= 6 ) {
	?>
		<script>montaShowModal();</script>
	<?
}
?>