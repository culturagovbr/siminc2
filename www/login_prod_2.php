<?php
//        unset($_COOKIE['aviso_layout_novo']);

//        setcookie("aviso_layout_novo",false);

//    ver($_COOKIE['aviso_layout_novo'], d);

// VERIFICANDO SE JA VIU AVISO DO LAYOUT NOVO DO SIMEC
if(isset($_COOKIE['aviso_layout_novo']) && $_COOKIE['aviso_layout_novo']){
    $avisoLayoutNovo = false;
} else {
    setcookie("aviso_layout_novo", true, time() + 60 * 60 * 24 * 30, "/");
    $avisoLayoutNovo = true;
}

// Faz download de um dos arquivos solicitados
if( isset($_REQUEST["arquivo_login"]) && !empty( $_REQUEST["arquivo_login"]) ) {
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
include APPRAIZ . "includes/library/simec/funcoes.inc";

// abre conex�o com o servidor de banco de dados
$db = new cls_banco();


//faz download do arquivo informes
if($_REQUEST['download']){
	$arqid = $_REQUEST['download'];
    DownloadArquivoInfo($arqid);
}

// Valida o CPF, vindo do post
if($_POST['usucpf'] && !validaCPF($_POST['usucpf'])) {
    die('<script>
                                               alert(\'CPF inv�lido!\');
                                               history.go(-1);
                               </script>');
}


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

    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1" />
    <!--        <meta http-equiv="X-UA-Compatible" content="IE=EmulateIE9" /> -->
    <meta content="IE=9" http-equiv="X-UA-Compatible" />

    <title>Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle</title>

    <!-- Styles Boostrap -->
    <link href="library/bootstrap-3.0.0/css/bootstrap.min.css" rel="stylesheet">
    <!--        <link href="library/bootstrap-3.0.0/css/bootstrap-theme-default.css" rel="stylesheet">-->
    <link href="library/chosen-1.0.0/chosen.css" rel="stylesheet">
    <link href="library/bootstrap-switch/stylesheets/bootstrap-switch.css" rel="stylesheet">

    <!-- Custom Style -->
    <link href="estrutura/temas/default/css/css_reset.css" rel="stylesheet">
    <link href="estrutura/temas/default/css/estilo.css" rel="stylesheet">

    <!-- HTML5 shim, for IE6-8 support of HTML5 elements -->
    <!--[if lt IE 9]>
    <script src="estrutura/js/html5shiv.js"></script>
    <![endif]-->
    <!--[if IE]>
    <link href="estrutura/temas/default/css/styleie.css" rel="stylesheet">
    <![endif]-->

    <!-- Boostrap Scripts -->
    <script src="library/jquery/jquery-1.10.2.js"></script>
    <script src="library/jquery/jquery.maskedinput.js"></script>
    <script src="library/bootstrap-3.0.0/js/bootstrap.min.js"></script>
    <script src="library/chosen-1.0.0/chosen.jquery.min.js"></script>
    <script src="library/bootstrap-switch/js/bootstrap-switch.min.js"></script>

    <!-- Custom Scripts -->
    <script type="text/javascript" src="../includes/funcoes.js"></script>



    <!--        FancyBox -->
    <script type="text/javascript" src="library/fancybox-2.1.5/source/jquery.fancybox.js?v=2.1.5"></script>
    <link rel="stylesheet" type="text/css" href="library/fancybox-2.1.5/source/jquery.fancybox.css?v=2.1.5" media="screen" />
    <script type="text/javascript" src="library/fancybox-2.1.5/lib/jquery.mousewheel-3.0.6.pack.js"></script>

    <!-- Add Button helper (this is optional) -->
    <link rel="stylesheet" type="text/css" href="library/fancybox-2.1.5/source/helpers/jquery.fancybox-buttons.css?v=1.0.5" />
    <script type="text/javascript" src="library/fancybox-2.1.5/source/helpers/jquery.fancybox-buttons.js?v=1.0.5"></script>

    <!-- Add Thumbnail helper (this is optional) -->
    <link rel="stylesheet" type="text/css" href="library/fancybox-2.1.5/source/helpers/jquery.fancybox-thumbs.css?v=1.0.7" />
    <script type="text/javascript" src="library/fancybox-2.1.5/source/helpers/jquery.fancybox-thumbs.js?v=1.0.7"></script>

    <!-- Add Media helper (this is optional) -->
    <script type="text/javascript" src="library/fancybox-2.1.5/source/helpers/jquery.fancybox-media.js?v=1.0.6"></script>



    <script type="text/javascript">
        $(function(){

            $('.carousel').carousel();

            $('.chosen-select').chosen();

            $('span').tooltip({placement: 'bottom'})

            $('.cpf').mask('999.999.999-99');

            $(".fancybox").fancybox();

            $('#baselogincheck').change(function(){
                if($(this).is(':checked')){
                    $('#baselogin').val('simec_espelho_producao');
                } else {
                    $('#baselogin').val('simec_desenvolvimento');
                }
            });
        });
        
        function dinfo(id){
			var url = 'login_prod_2.php?download=' + id;
			var iframe;
	        iframe = document.getElementById("download-container");
	        if (iframe === null)
	        {
	            iframe = document.createElement('iframe');  
	            iframe.id = "download-container";
	            iframe.style.visibility = 'hidden';
	            document.body.appendChild(iframe);
	        }
	        iframe.src = url;
        }
    </script>

	<!-- style barra governo -->
	<style>
		#barra-brasil .brasil-flag {
			height: 100% !important;
		}
	</style>

</head>
<body>
<? // include "barragoverno_2014.php"; ?>

<div id="barra-brasil" style="background:#7F7F7F; height: 20px; padding:0 0 0 10px;display:block;"> 
	<ul id="menu-barra-temp" style="list-style:none;">
		<li style="display:inline; float:left;padding-right:10px; margin-right:10px; border-right:1px solid #EDEDED"><a href="http://brasil.gov.br" style="font-family:sans,sans-serif; text-decoration:none; color:white;">Portal do Governo Brasileiro</a></li> 
		<li><a style="font-family:sans,sans-serif; text-decoration:none; color:white;" href="http://epwg.governoeletronico.gov.br/barra/atualize.html">Atualize sua Barra de Governo</a></li>
	</ul>
</div>

<br>

<?php include "atualizar_browser.php"; ?>

<script type="text/javascript">
    $(function(){
        // Testando se � Internet Explorer
        if (/MSIE (\d+\.\d+);/.test(navigator.userAgent)){
            var ieversao = new Number(RegExp.$1)
            // Verificando vers�o antiga
            if(ieversao < 9){
//                alert(ieversao);
                document.getElementById('aviso_browser').style.display = 'block';
            }
        }
    });
</script>

<div class="container">
<? if ( $_SESSION['MSG_AVISO'] ): ?>
    <div class="alert alert-danger">
        <button type="button" class="close" data-dismiss="alert" aria-hidden="true">&times;</button>
        <?= implode( "<br />", (array) $_SESSION['MSG_AVISO'] ); ?>
    </div>
<? endif;
$_SESSION['MSG_AVISO'] = array(); ?>

<!-- LOGIN -->
<div class="row">
    <div class="col-md-7 col-sm-12">
        <img src="estrutura/temas/default/img/logo-simec.png">
    </div><!-- / .col-md-7 -->

    <div class="col-md-5 col-sm-12 login">

        <form class="form-horizontal" role="form" method="post" action="">
            <input type="hidden" name="versao" value="<?php echo $_POST['versao']; ?>"/>
            <input type="hidden" name="formulario" value="1"/>
            <input type="hidden" id="arquivo_login" name="arquivo_login" value="" />

            <div class="col-md-8 col-sm-9">
                <div class="form-group">
                    <label for="usucpf" class="hidden-xs col-md-2 col-sm-3 control-label">CPF: </label>
                    <div class="col-lg-10 input-group">
                        <span class="input-group-addon"><span class="glyphicon glyphicon-user"></span></span>
                        <input type="text" class="form-control login_input cpf" name="usucpf" id="usucpf" placeholder="Digite o CPF" required="required" >
                    </div>
                </div>
                <div class="form-group">
                    <label for="ususenha" class="hidden-xs col-md-2 col-sm-3 control-label">Senha: </label>
                    <div class="col-lg-10 input-group">
                        <span class="input-group-addon"><span class="glyphicon glyphicon-asterisk"></span></span>
                        <input type="password" class="form-control" name="ususenha" id="ususenha" placeholder="Digite a senha" required="required">
                    </div>
                </div>
            </div>
            <div class="col-md-4 col-sm-3">
                <button type="submit" class="btn btn-xs btn-block btn-success">Entrar</button>
                <a href="recupera_senha.php" class="btn btn-xs btn-block btn-danger">Esqueci a senha</a>
                <a href="cadastrar_usuario.php" class="btn btn-xs btn-block btn-warning">Solicitar acesso</a>
            </div>
        </form>
    </div><!-- / .col-md-5 -->
</div><!-- /LOGIN -->

<div class="hidden-xs">
    <!-- CAROUSEL -->
    <div class="row">
        <div class="col-md-9 col-sm-12" id="informes">
            <div class="box principal">
                <div class="titulo">
                                    <span class="text-center">
                                        <div class="texto">Informes</div>
                                    </span>
                    <div class="clearfix"></div>
                </div>
                <?php 
                /*
                	$aInformes = array(
                
                    'EJA' => '<h2>ATEN��O</h2><br>
                              <p>A SECRETARIA DE EDUCA��O CONTINUADA, ALFABETIZA��O, DIVERSIDADE E INCLUS�O � SECADI informa que a ades�o � Resolu��o 48/2013  (Transfer�ncia autom�tica de recursos financeiros aos estados, munic�pios e Distrito Federal para manuten��o de novas turmas de Educa��o de Jovens e Adultos, foi prorrogada, impreterivelmente, at� o dia 31 de janeiro de 2014.</p>',
                
                    'PROJETO ESPLANADA SUSTENT�VEL' => '<p>Informamos que o m�dulo Esplanada Sustent�vel, utilizado para cadastrar os contratos, acompanhamento das despesas pactuadas e plano de a��o , j� est� em funcionamento.</p>',

                    'PNLD' => '<h2>ATEN��O</h2>
                                    <p>Aos detentores de direito autoral das obras did�ticas inscritas no PNLD/2014.</p>
                                    <p>A Secretaria de Educa��o B�sica informa a divulga��o do resultado do processo de avalia��o realizado no �mbito do PNLD/2014. Os pareceres de todas as obras avaliadas est�o dispon�veis ao representante da editora cadastrado no SIMAD/FNDE.</p>
                                    <p>As informa��es relativas �s fases de interposi��o de recursos e de corre��o de falhas pontuais est�o dispon�veis na portaria do resultado publicado no D.O.U e no M�dulo PNLD.</p>',

                    'PRONACAMPO' => '<p>Informo que as a��es do Programa Nacional de Educa��o do Campo ? PRONACAMPO podem ser acessadas por meio do endere�o eletr�nico: <a href="http://pronacampo.mec.gov.br" target="_blank">http://pronacampo.mec.gov.br</a></p>',

                    'Programa��o Or�ament�ria' => '<h2>TERMOS DE COOPERA��O</h2>
                                    <p>Informamos que o m�dulo de descentraliza��o de cr�ditos, utilizado para se cadastrar os TERMOS DE COOPERA��O firmados com as secretarias do Minist�rio da Educa��o, j� est� em funcionamento no m�dulo de programa��o or�ament�ria.</p>
                                    <p>O manual de utiliza��o do sistema pode ser visualizado clicando-se no seguinte link: <a href="http://simec.mec.gov.br/Manual_do_Modulo_de_Descentralizacao.pdf">Manual</a>.</p>',

                    'PACTO NACIONAL PELO FORTALECIMENTO DO ENSINO M�DIO' => '<h2>ATEN��O</h2>
                                    <p>Para saber mais sobre o Pacto Nacional pelo Fortalecimento do Ensino M�dio, leia a Resolu��o n� 51, de 11/12/13, dispon�vel no site do FNDE (www.fnde.gov.br>> FNDE >> Legisla��o). Para acessar o Manual do SisM�dio � M�dulo Diretor, <a href="http://portal.mec.gov.br/index.php?option=com_content&view=article&id=20189&Itemid=811" target="_blank">clique aqui</a>.</p>',

                    'Mais Educa��o' => '<h2>Cadastro de Novas Escolas - Diretores</h2>
                                    <p>As escolas interessadas em aderir ao Programa Mais Educa��o para o ano de 2013 dever�o, por meio de seu Diretor (a), solicitar o cadastro no http://simec.mec.gov.br/ no campo ACESSO O SISTEMA - Solicitar Cadastro. </p>
                                    <p>O diretor (a) deve selecionar o M�dulo ESCOLA, inserir o CPF e continuar. Em seguida, o sistema solicitar� os dados pessoais e um perfil, selecionar CADASTRADOR MAIS EDUCA��O. > Enviar solicita��o.</p>
                                    <p>Ap�s solicitado o cadastro do(a) Diretor(a) um t�cnico da Secretaria de Educa��o dever� acessar a p�gina principal do Simec, pois ser� ele (a) respons�vel por liberar a senha dos diretores.</p>',

                    'PAR' => '<h2>ATEN��O</h2>
                                    <p>Sr(a) Usu�rio do M�dulo PAR, </p>
                                    <p>Lembramos que o acesso ao PAR municipal pode ser liberado para o(a) prefeito municipal, para o(a) dirigente municipal de educa��o (DME) e para apenas um(a) t�cnico(a) indicado(a) pelo(a) DME. No caso dos estados, para o(a) secret�rio(a) estadual de educa��o e para os t�cnicos indicados por ele(a).</p>
                                    <p>Os t�cnicos da secretaria de educa��o, engenheiros, diretores de escola ou outros usu�rios de estados e munic�pios que n�o foram devidamente autorizados pelo gestor permanecer�o bloqueados. </p>
                                    <p>No caso de escolas benefici�rias de a��es como a constru��o de quadras escolares, cobertura de quadras existentes ou pelo programa �gua na Escola, a apresenta��o do pleito ser� feita pelo secret�rio de educa��o e sua equipe, n�o pela escola. </p>',


                    'PDE INTERATIVO' => '<h2>ATEN��O</h2>
                                    <p>Para acessar o PDE Interativo, fa�a o seu login no novo endere�o: <a target="_blank" href="http://pdeinterativo.mec.gov.br">http://pdeinterativo.mec.gov.br</a></p>',
                	); 
				*/
                
                // buscando informes
			    $sql = "SELECT ifmtitulo as titulo, ifmtexto as texto, arqid
                        FROM seguranca.informes
                        WHERE ifmstatus='A' AND ifmmodal=false
                        and (
                            (CURRENT_TIMESTAMP >=ifmdatainicio and  ifmdatafim is null) or
                            (CURRENT_TIMESTAMP between ifmdatainicio and ifmdatafim)
                        )
                        ORDER BY ifmid";
			
			    $aInformes = $db->carregar($sql);
				?>

                <div id="carousel-informes" class="carousel slide">
                    <!-- Indicators -->
                    <ol class="carousel-indicators" style="color: black !important;">
                        <?php
                        if($aInformes){
	                        $count = 0;
	                        foreach($aInformes as $v) { ?>
	                            <li data-target="#carousel-informes" data-slide-to="<?php echo $count; ?>" class="<?php echo $count == 0 ? 'active' : ''; ?>"></li>
	                            <?php $count++;
	                        }
						} ?>
                    </ol>

                    <!-- Wrapper for slides -->
                    <div class="carousel-inner">
                        <?php
                        $count = 0;
                        if($aInformes){
	                        foreach($aInformes as $v) { ?>
	                            <div class="item <?php echo $count == 0 ? 'active' : ''; ?>">
	                                <div style="height: 200px; !important; background: #fff" class="conteudo">
	                                    <h1><strong><?php echo $v['titulo']; ?></strong></h1>
	                                    <?php 
	                                    	if($v['arqid']){
	                                    		$link = '<a href="javascript:dinfo('.$v['arqid'].')">Clique Aqui</a>';
	                                    		echo str_replace("[LINK]", $link, $v['texto']); 
	                                    	}else{
	                                    		echo $v['texto']; 
	                                    	}
	                                    ?>
	                                </div>
	                            </div>
	                            <?php $count++;
	                        }
						}else{
							?>
							<div class="item <?php echo $count == 0 ? 'active' : ''; ?>">
	                                <div style="height: 200px; !important; background: #fff" class="conteudo">
	                                </div>
	                        </div>
							<?
						}	
						?>
                    </div>

                    <!-- Controls -->
                    <a class="left carousel-control" href="#carousel-informes" data-slide="prev"><span class="icon-prev"></span></a>
                    <a class="right carousel-control" href="#carousel-informes" data-slide="next"><span class="icon-next"></span></a>
                </div><!-- /#carousel-informes -->
            </div>
        </div><!-- /#informes -->


        <div class="col-md-3 hidden-xs hidden-sm" id="premios">
            <div class="box principal black">
                <div class="titulo">
                                    <span class="text-center">
                                        <div class="texto">Pr�mios</div>
                                    </span>
                    <div class="clearfix"></div>
                </div>

                <?php $aPremios = array(
                    'E-Gov 2013' => 'estrutura/temas/default/img/premios/egov2013.jpg',
                    'E-Gov 2012' => 'estrutura/temas/default/img/premios/premioe-gov2012.png',
                    'E-Gov 2011' => 'estrutura/temas/default/img/premios/premiogovernoti2011.png',
                    'Excel�ncia em Inova��o na Gest�o P�blica - 2010' => 'estrutura/temas/default/img/premios/conip.gif',
                    'E-Gov 2009' => 'estrutura/temas/default/img/premios/premioe-gov.png',
                    'Selo Inova��o' => 'estrutura/temas/default/img/premios/selo-inovacao.gif',
                ); ?>

                <div id="carousel-premios" class="carousel slide">
                    <!-- Wrapper for slides -->
                    <div class="carousel-inner" style="height: 200px;">
                        <?php
                        $count = 0;
                        foreach($aPremios as $descricaoPremio => $imgPremio) { ?>
                            <div class="item <?php echo $count == 0 ? 'active' : ''; ?>">
                                <div>
                                    <img class=".img-responsive" src="<?php echo $imgPremio; ?>" alt="<?php echo $descricaoPremio; ?>" title="<?php echo $descricaoPremio; ?>">
                                </div>
                            </div>
                            <?php $count++;
                        } ?>
                    </div>

                    <!-- Controls -->
                    <a class="left carousel-control" href="#carousel-premios" data-slide="prev"><span class="icon-prev"></span></a>
                    <a class="right carousel-control" href="#carousel-premios" data-slide="next"><span class="icon-next"></span></a>
                </div><!-- /#carousel-premios -->
            </div>
        </div><!-- /#premios -->
    </div><!-- /CAROUSEL -->
    <div class="spacer"></div>
</div>

<div class="visible-md visible-lg">

    <?
    // buscando a lista de sistemas
    $sql = "SELECT sisid, sisabrev, sisdsc, sisfinalidade, sispublico, sisrelacionado
                            FROM seguranca.sistema
                            WHERE sisstatus='A' AND sismostra=true
                            ORDER BY sisid";

    $sistemas = $db->carregar($sql);
    ?>

    <div class="row" id="div-sistemas">
        <?php
        $aClasses = array('', 'panel-success', 'panel-warning', 'panel-danger', 'panel-info');
        foreach ($sistemas as $count => $sistema) {

            $class = next($aClasses);
            if('panel-info' == $class){
                reset($aClasses);
            }

            if ($count && !($count%4)) {
                echo '<div class="clearfix"></div>';
            }
            ?>
            <div class="col-md-3 box-sistemas box-login-<?php echo $class; ?>">
                <div class="panel <?php echo $class; ?> bg-<?php echo $class; ?>">
                    <div class="panel-heading">
                        <h3 class="panel-title"><?php echo $sistema['sisabrev']; ?></h3>
                    </div>
                    <div class="panel-body">
                        <img src="estrutura/temas/default/img/modulos/pin-<?php echo $class; ?>.png" style="float: left; margin-right: 10px;" />
                        <span class="span-descricao"><?php echo $sistema['sisdsc']; ?></span>
                        <div class="clearfix"></div>
                        <div style="margin-top: 10px; text-align: center;">
                            <a href="javascript:janela('/geral/fale_conosco.php?sisid=<?php echo $sistema['sisid']; ?>',850,550)"><span data-toggle="tooltip" title="D�vidas" class="glyphicon glyphicon-info-sign" style="margin-right: 20px;"></span></a>
                            <a href="cadastrar_usuario.php?sisid=<?php echo $sistema['sisid']; ?>"><span data-toggle="tooltip" title="Solicitar Acesso"  class="glyphicon glyphicon-bell" style="margin-right: 20px;"></span></a>
                        </div>
                    </div>
                </div>
            </div>
        <?php } ?>
    </div>
</div>

<div class="hidden-xs hidden-sm">
    <footer>
        <div class="row footer">
            <div class="col-md-12">
                <p>Data do Sistema <?php echo date('d/m/Y')?></p>
            </div>
        </div>
    </footer>
</div>


<div class="modal fade" id="myModal" >
    <div class="modal-dialog">
        <div class="modal-content text-center">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
                <h4 class="modal-title">Gestores do Sistema</h4>
            </div>
            <div class="modal-body">
                <span class="glyphicon glyphicon-user"></span><span></span><br />
                <span class="glyphicon glyphicon-earphone"></span><span></span>
            </div>
        </div><!-- /.modal-content -->
    </div><!-- /.modal-dialog -->
</div><!-- /.modal -->


<?php if($avisoLayoutNovo): ?>
    <div id="modal-aviso-layout" class="modal fade" >
        <div class="modal-dialog">
            <div class="modal-content text-center">
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
                    <h4 class="modal-title" style="font-weight: bold;">Mudan�as no SIMEC</h4>
                </div>
                <div class="modal-body text-left">
                    <p>Em 2014 o SIMEC est� de cara nova, mais din�mica e atualizada.</p>
                    <br />
                    <p>Com o novo menu ficou mais f�cil navegar entre os m�dulos, dando agilidade e tornando o servi�o mais pr�tico.</p>
                    <br />
                    <p>Verifique agora as novas telas na vers�o de computador, tablet, celular e o novo menu:</p>
                    <br />

                    <a class="fancybox" data-fancybox-group="gallery" rel="fancybox" href="/estrutura/temas/default/img/versoes/computador.jpg" title="Computador"><img height="80px" src="/estrutura/temas/default/img/versoes/computador.jpg" class="versoes" alt="Computador"></a>
                    <a class="fancybox" data-fancybox-group="gallery" rel="fancybox" href="/estrutura/temas/default/img/versoes/tablet.jpg" title="Tablet"><img height="80px" src="/estrutura/temas/default/img/versoes/tablet.jpg" class="versoes" alt="Tablet"></a>
                    <a class="fancybox" data-fancybox-group="gallery" rel="fancybox" href="/estrutura/temas/default/img/versoes/celular.jpg" title="Celular"><img height="80px" src="/estrutura/temas/default/img/versoes/celular.jpg" class="versoes" alt="Celular"></a>
                    <a class="fancybox" data-fancybox-group="gallery" rel="fancybox" href="/estrutura/temas/default/img/versoes/menu.jpg" title="Menu"><img width="80px" src="/estrutura/temas/default/img/versoes/menu.jpg" class="versoes" alt="Menu"></a>

                </div>
                <div class="modal-footer">
                    <button type="button" class="btn btn-primary" data-dismiss="modal">Ok</button>
                </div>
            </div><!-- /.modal-content -->
        </div><!-- /.modal-dialog -->
    </div><!-- /.modal -->
<?php endif ?>



<?//mostra show modal informes
// buscando informes
$sql = "SELECT ifmtitulo as titulo, ifmtexto as texto, arqid
		FROM seguranca.informes
        WHERE ifmstatus='A' AND ifmmodal=true
        and (
            (CURRENT_TIMESTAMP >=ifmdatainicio and  ifmdatafim is null) or
            (CURRENT_TIMESTAMP between ifmdatainicio and ifmdatafim)
        )
        ORDER BY ifmid desc";
$aInformesModal = $db->carregar($sql);
if($aInformesModal){
	$i=0;
	foreach($aInformesModal as $v) {
	?>
	<div id="modal-aviso-manutencao-<?=$i?>" class="modal fade modal-informes" >
	    <div class="modal-dialog">
	        <div class="modal-content text-center">
	            <div class="modal-header">
	                <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
	                <h4 class="modal-title" style="font-weight: bold;"><?php echo $v['titulo']; ?></h4>
	            </div>
	            <div class="modal-body text-left">
	                <div class="alert alert-danger" style="font-weight: bold">
						<?php 
                             if($v['arqid']){
                             	$link = '<a href="javascript:dinfo('.$v['arqid'].')">Clique Aqui</a>';
                             	echo str_replace("[LINK]", $link, $v['texto']); 
                             }else{
                             	echo $v['texto']; 
                             }
                        ?>
	                </div>
	            </div>
	            <div class="modal-footer">
	                <button type="button" class="btn btn-primary" data-dismiss="modal">Ok</button>
	            </div>
	        </div><!-- /.modal-content -->
	    </div><!-- /.modal-dialog -->
	</div><!-- /.modal -->
	<?}
}?>


</div> <!-- /container -->


<!-- Fim barra governo -->
<script src="//static00.mec.gov.br/barragoverno/barra.js" type="text/javascript"></script>


</body>
</html>

<script language="javascript">
   <?php if($aInformesModal) { ?>
        $('.modal-informes').modal('show');
   <?php } ?>
</script>

<?
function DownloadArquivoInfo($arqid){
		global $db;
		
		$sql ="SELECT * FROM public.arquivo WHERE arqid = ".$arqid;
		$arquivo = $db->carregar($sql);
        $caminho = APPRAIZ . 'arquivos/informes/'. floor($arquivo[0]['arqid']/1000) .'/'.$arquivo[0]['arqid'];
		if ( !is_file( $caminho ) ) {
            die('<script>alert("Arquivo n�o encontrado.");</script>');
        }
        $filename = str_replace(" ", "_", $arquivo[0]['arqnome'].'.'.$arquivo[0]['arqextensao']);
        header( 'Content-type: '. $arquivo[0]['arqtipo'] );
        header( 'Content-Disposition: attachment; filename='.$filename);
        readfile( $caminho );
        exit();
}
?>