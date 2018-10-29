<?php

/**
 * Sistema Integrado de Planejamento, Orçamento e Finanças do Ministério da Educação
 * Setor responsvel: DTI/SE/MEC
 * Autor: Cristiano Cabral <cristiano.cabral@gmail.com>
 * Módulo: Segurança
 * Finalidade: Tela de apresentação. Permite que o usuário entre no sistema.
 * Data de criação: 24/06/2005
 * Última modificação: 02/09/2013 por Orion Teles <orionteles@gmail.com>
 */

// carrega as bibliotecas internas do sistema
require_once 'config.inc';
require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
require_once APPRAIZ . "includes/library/simec/funcoes.inc";

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

//faz download do arquivo informes
if($_REQUEST['download']){
	$arqid = $_REQUEST['download'];
    DownloadArquivoInfo($arqid);
}

// Valida o CPF, vindo do post
if($_POST['usucpf'] && !validaCPF($_POST['usucpf'])) {
    die('<script>alert(\'CPF inválido!\');history.go(-1);</script>');
}

// executa a rotina de autenticação quando o formulário for submetido
if ( $_POST['usucpf'] ) {
    if(AUTHSSD) {
        include_once APPRAIZ . "includes/autenticarssd.inc";
    } else {
        include_once APPRAIZ . "includes/autenticar.inc";
    }
}

if ( $_REQUEST['expirou'] ) {
    $_SESSION['MSG_AVISO'][] = "Sua conexão expirou por tempo de inatividade. Para entrar no sistema efetue login novamente.";
}

function DownloadArquivoInfo($arqid){
    global $db;

    $sql ="SELECT * FROM public.arquivo WHERE arqid = ".$arqid;
    $arquivo = $db->carregar($sql);
    $caminho = APPRAIZ . 'arquivos/informes/'. floor($arquivo[0]['arqid']/1000) .'/'.$arquivo[0]['arqid'];

    if ( !is_file( $caminho ) ) {
            die('<script>alert("Arquivo não encontrado.");</script>');
    }

    $filename = str_replace(" ", "_", $arquivo[0]['arqnome'].'.'.$arquivo[0]['arqextensao']);
    header( 'Content-type: '. $arquivo[0]['arqtipo'] );
    header( 'Content-Disposition: attachment; filename='.$filename);
    readfile( $caminho );
    exit();
}
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="description" content="">
    <meta name="author" content="">

    <title><?php echo NOME_SISTEMA; ?></title>

	<!-- Styles Boostrap -->
    <link href="library/bootstrap-3.0.0/css/bootstrap.min.css" rel="stylesheet">
    <link href="library/bootstrap-3.0.0/css/portfolio.css" rel="stylesheet">
    <link href="library/chosen-1.0.0/chosen.css" rel="stylesheet">
    <link href="library/bootstrap-switch/stylesheets/bootstrap-switch.css" rel="stylesheet">
	
    <!-- Custom CSS -->
    <link href="estrutura/temas/default/css/css_reset.css" rel="stylesheet">
    <link href="estrutura/temas/default/css/estilo.css" rel="stylesheet">
	<link href="library/simec/css/custom_login.css" rel="stylesheet">

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
    <script type="text/javascript" src="../includes/funcoes.js?v=1"></script>
    
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

    <style>
        body{
            height: auto;
        }

        .panel-login{
            margin-top: 50px;
        }

        .panel-login hr{
            margin-bottom: 0;
        }

        .panel-body{
            padding-bottom: 10px;
        }

        .panel-login input.form-control{
            height: 50px;
        }

        .panel-login input.form-control{
            height: 50px;
        }
    </style>

</head>

<body class="page-index">

    <!-- Login -->
    <section id="login" class="login">
        <div class="content">
            <?php if ($_SESSION['MSG_AVISO']): ?>
                <div class="col-md-4 col-md-offset-4">
                    <div class="alert alert-danger" style="font-size: 14px; line-height: 20px;">
                        <button type="button" class="close" data-dismiss="alert" aria-hidden="true">&times;</button>
                        <i class="fa fa-bell"></i> <?php echo implode("<br />", (array) $_SESSION['MSG_AVISO']); ?>
                    </div>
                </div>
            <?php endif; ?>
            <?php $_SESSION['MSG_AVISO'] = array(); ?>

            <div class="col-md-4 col-md-offset-4">
                <div class="panel-login">
                    <div class="panel-heading">
                        Nova Sigla
                    </div>
                    <div class="panel-body">
                        <form class="form-horizontal" role="form" method="post" action="">
                            <input type="hidden" name="versao" value="<?php echo $_POST['versao']; ?>"/>
                            <input type="hidden" name="formulario" value="1"/>

                            <?php if (!IS_PRODUCAO) : ?>
    <!--				            <input type="hidden" name="baselogin" id="baselogin" value="simec_espelho_producao"/>
     <div class="form-group text-right">
     <div class="col-lg-12">
         <div class="make-switch" data-on-label="Espelho" data-off-label="Desenv. " data-on="primary" data-off="danger">
             <input type="checkbox" name="baselogincheck" id="baselogincheck" value="simec_espelho_producao" checked="checked" />
         </div>
     </div>
    </div>-->
                            <?php endif; ?>
                            <div class="form-group">
                                <div class="col-sm-12">
                                    <input type="text" maxlength="14" class="form-control" name="usucpf" id="usucpf" placeHolder="CPF" required="">
                                </div>
                            </div>
                            <div class="form-group">
                                <div class="col-sm-12">
                                    <input type="password" class="form-control" name="ususenha" id="ususenha" placeHolder="Senha" required="">
                                </div>
                            </div>
                            <div class="form-group" style="font-size: 14px;">
                                <div class="col-sm-7" style="margin-top: 3px">
                                    <i class="fa fa-key"></i> <a href="recupera_senha.php" style="color: #fff">Esqueci minha senha?</a>
                                </div>
                                <div class="col-sm-5 text-right">
                                    <button style="background-color: #1da589; border-color: #1da589;" type="submit" class="btn btn-success"><span class="glyphicon glyphicon-ok"></span> Acessar</button>
                                </div>
                            </div>
                        </form>
                        <hr>
                    </div>
                    <div class=" text-center" style="font-size: 14px;">
                        <div class="btn-group">
                            Não tem acesso ainda?&nbsp;
                            <i class="fa fa-user"></i> 
                            <a href="cadastrar_usuario.php" id="btn-cadastro" style="color: #fff">Solicitar acesso</a>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </section>
    <!--/LOGIN -->

    <!-- Custom Theme JavaScript -->
    <script>
        $(function(){
            $('[data-tooltip="left"]').tooltip({placement : 'left'});
            $('.modal-informes').modal('show');
            $('span').tooltip({placement: 'bottom'})
            $('.carousel').carousel();
            $('.chosen-select').chosen();
            
            // Mascara de CPF
            $('#usucpf').keyup(function(){
                mascara(this, mcpf);
            });
            
            $(".menu-close").click(function(e) {
                e.preventDefault();
                $("." + $(this).data('toggle')).toggleClass("active");
            });
            $(".menu-toggle").click(function(e) {
                e.preventDefault();
                $("." + $(this).data('toggle')).toggleClass("active");
            });
            $('#baselogincheck').change(function(){
                if($(this).is(':checked')){
                    $('#baselogin').val('simec_espelho_producao');
                } else {
                    $('#baselogin').val('simec_desenvolvimento');
                }
            });
        });
        
        function dinfo(id){
            var url = 'login.php?download=' + id;
            var iframe;
            iframe = document.getElementById("download-container");
            if (iframe === null){
                iframe = document.createElement('iframe');  
                iframe.id = "download-container";
                iframe.style.visibility = 'hidden';
                document.body.appendChild(iframe);
            }
            iframe.src = url;
        }

    </script>

</body>

</html>
<?php $db->close(); ?>