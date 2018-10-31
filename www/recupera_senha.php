<?php
	if(isset($_POST["theme_simec"])){
		$theme = $_POST["theme_simec"];
		setcookie("theme_simec", $_POST["theme_simec"] , time()+60*60*24*30, "/");
	} else {
		if(isset($_COOKIE["theme_simec"])){
			$theme = $_COOKIE["theme_simec"];
		}
	}

	/**
	 * Sistema Integrado de Monitoramento do Ministério da Educação
	 * Setor responsvel: SPO/MEC
	 * Desenvolvedor: Desenvolvedores Simec
	 * Analistas: Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>
	 * Programadores: Renê de Lima Barbosa <renedelima@gmail.com>
	 * Módulo: Segurança
	 * Finalidade: Permite que o usuário solicite uma nova senha.
	 * Última modificação: 26/08/2006
	 */

	function erro(){
		global $db;
		$db->commit();
		$_SESSION = array();
		$_SESSION['MSG_AVISO'] = func_get_args();
		header( "Location: ". $_SERVER['PHP_SELF'] );
		exit();
	}

	// carrega as bibliotecas internas do sistema
	require_once "config.inc";
	require_once APPRAIZ . "includes/classes_simec.inc";
	require_once APPRAIZ . "includes/funcoes.inc";

	// abre conexão com o servidor de banco de dados
	$db = new cls_banco();

	if(!$theme) {
		$theme = $_SESSION['theme_temp'];
	}

	// executa a rotina de recuperação de senha quando o formulário for submetido
	if ( $_POST['usucpf'] ) {
		// verifica se a conta está ativa
		$sql = sprintf("SELECT u.* FROM seguranca.usuario u WHERE u.usucpf = '%s'", corrige_cpf( $_REQUEST['usucpf'] ));
		$usuario = (object) $db->pegaLinha( $sql );
		
		if ( $usuario->suscod != 'A' ) {
			erro( "A conta não está ativa." );
		}

		$_SESSION['mnuid'] = 10;
		$_SESSION['sisid'] = 4;
		$_SESSION['exercicio_atual'] = $db->pega_ano_atual();
		$_SESSION['usucpf'] = $usuario->usucpf;
		$_SESSION['usucpforigem'] = $usuario->usucpf;

		// cria uma nova senha
	    //$senha = $db->gerar_senha();
	    $senha = strtoupper(senha());
		$sql = sprintf(
			"UPDATE seguranca.usuario SET ususenha = '%s', usuchaveativacao = 'f' WHERE usucpf = '%s'",
			md5_encrypt_senha( $senha, '' ),
			$usuario->usucpf
		);
		$db->executar( $sql );

		// envia email de confirmação
		$sql = "select ittemail from public.instituicao where ittstatus = 'A'";
		$remetente = $db->pegaUm( $sql );
		$destinatario = $usuario->usuemail;
		$assunto = SIGLA_SISTEMA. " - Recuperação de Senha";
	    $conteudo = sprintf(
	    	"%s %s<br/>Sua senha foi alterada para %s<br>Ao se conectar, altere esta senha para a sua senha preferida.",
	    	$usuario->ususexo == 'F' ?  'Prezada Sra.': 'Prezado Sr.',
	    	$usuario->usunome,
	    	$senha
	    );
		enviar_email( $remetente, $destinatario, $assunto, $conteudo );

		$db->commit();
		$_SESSION = array();
		$_SESSION['MSG_AVISO'][] = "Recuperação de senha concluída. Em breve você receberá uma nova senha por email.";
		header( "Location: /" );
		exit();
	}

	if ( $_REQUEST['expirou'] ) {
		$_SESSION['MSG_AVISO'][] = "Sua conexão expirou por tempo de inatividade. Para entrar no sistema efetue login novamente.";
	}
?>
<!DOCTYPE html>
<html>
<head>
	<meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="description" content="">
    <meta name="author" content="">

    <title>Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle</title>

	<!-- Styles Boostrap -->
    <link href="library/bootstrap-3.0.0/css/bootstrap.min.css" rel="stylesheet">
    <link href="library/bootstrap-3.0.0/css/portfolio.css" rel="stylesheet">
    <link href="library/chosen-1.0.0/chosen.css" rel="stylesheet">
    <link href="library/bootstrap-switch/stylesheets/bootstrap-switch.css" rel="stylesheet">
	
    <!-- Custom CSS -->
    <link href="estrutura/temas/default/css/css_reset.css" rel="stylesheet">
    <link href="estrutura/temas/default/css/estilo.css" rel="stylesheet">
	<link href="library/simec/css/custom.css" rel="stylesheet">

    <!-- Custom Fonts -->
    <link href="library/font-awesome/css/font-awesome.min.css" rel="stylesheet" type="text/css">
    <link href="http://fonts.googleapis.com/css?family=Source+Sans+Pro:300,400,700,300italic,400italic,700italic" rel="stylesheet" type="text/css">

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
    
    <!-- FancyBox -->
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
</head>
<body class="page-index">

	<?php if ( $_SESSION['MSG_AVISO'] ): ?>
		<div class="col-md-4 col-md-offset-4">
			<div class="alert alert-danger" style="font-size: 14px; line-height: 20px;">
				<button type="button" class="close" data-dismiss="alert" aria-hidden="true">&times;</button>
				<i class="fa fa-bell"></i> <?php echo implode( "<br />", (array) $_SESSION['MSG_AVISO'] ); ?>
			</div>
		</div>
	<?php endif; ?>
	<?php $_SESSION['MSG_AVISO'] = array(); ?>
    
     <!-- Register -->
    <section class="login">
		<div class="content">
			<div class="col-md-4 col-md-offset-4">
				<div class="panel panel-default">
					<div class="panel-heading">
						<span class="glyphicon glyphicon-user"></span> Recuperação de senha de usuários <br>
					</div>
					<div class="panel-body">
						<form name="formulario" id="formulario" class="form-horizontal" role="form" method="post" action="" onsubmit="return false;">
							<div class="form-group">
								<div class="col-sm-12">
									<input type="text" class="form-control cpf" name="usucpf" id="usucpf" placeHolder="CPF" required="" value=<?php echo $usucpf; ?>/>
								</div>
							</div>
							<div class="form-group" style="font-size: 14px;">
								<div class="col-sm-12 pull-right">
									<a class="btn btn-success" href="javascript:enviar_formulario()"><span class="glyphicon glyphicon glyphicon glyphicon-ok"></span> Lembrar senha</a>
									<a class="btn btn-danger" href="javascript:history.back();" ><span class="glyphicon glyphicon glyphicon glyphicon-remove"></span> Cancelar</a>
								</div>
							</div>
                		</form>
					</div>
					<div class="panel-footer text-center" style="font-size: 14px;">
					   Data do Sistema: <? echo date("d/m/Y - H:i:s") ?>
					</div>
				</div>
			</div>
		</div>
	</section>
</body>
<script language="javascript">
	$(function(){
	    $('.modal-informes').modal('show');
		$('span').tooltip({placement: 'bottom'})
		$('.carousel').carousel();
		$('.chosen-select').chosen();
		$('.cpf').mask('999.999.999-99');
		$(".menu-close").click(function(e) {
			e.preventDefault();
			$("." + $(this).data('toggle')).toggleClass("active");
		});
		$(".menu-toggle").click(function(e) {
			e.preventDefault();
			$("." + $(this).data('toggle')).toggleClass("active");
		});
	});
	        
	document.formulario.usucpf.focus();

	function enviar_formulario() {
		if ( validar_formulario() ) {
			document.formulario.submit();
		}
	}

	function validar_formulario() {
		var validacao = true;
		var mensagem = '';
		if ( !validar_cpf( document.formulario.usucpf.value ) ) {
			mensagem += '\nO cpf informado não é válido.';
			validacao = false;
		}
		if ( !validacao ) {
			alert( mensagem );
		}
		return validacao;
	}
</script>
</html>
<?php $db->close(); ?>