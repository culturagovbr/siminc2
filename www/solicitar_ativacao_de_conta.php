<?php

    /**
     * Sistema Integrado de Monitoramento do Ministério da Educação
     * Setor responsvel: SPO/MEC
     * Desenvolvedor: Desenvolvedores Simec
     * Analistas: Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>, Alexandre Soares Diniz
     * Programadores: Renê de Lima Barbosa <renedelima@gmail.com>
     * Módulo: Autenticação
     * Finalidade: Permite que o usuário solicite justificadamente a ativação da sua conta.
     * Data de criação: 24/06/2005
     * Última modificação: 29/08/2006
     */

    // carrega as bibliotecas internas do sistema
    include "config.inc";
    require APPRAIZ . "includes/classes_simec.inc";
    include APPRAIZ . "includes/funcoes.inc";

    // abre conexão com o banco de dados
    $db = new cls_banco();

    $usucpf = $_REQUEST['usucpf'];
    $sisid = $_REQUEST['sisid'];

    if($_REQUEST['formulario']) {
        $cpf = corrige_cpf($_POST['usucpf']);
        $justificativa = trim($_POST['htudsc']);

        # carrega os dados do usuário
        $sql = sprintf(
            "SELECT u.usucpf, u.suscod FROM seguranca.usuario u WHERE u.usucpf = '%s'",
            $cpf
        );
        $usuario = (object) $db->recuperar( $sql );

        # atribuições requeridas para que a auditoria do sistema funcione
        $_SESSION['sisid'] = 4; # seleciona o sistema de segurança
        $_SESSION['usucpf'] = $usuario->usucpf;
        $_SESSION['usucpforigem'] = $usuario->usucpf;
        $_SESSION['superuser'] = $db->testa_superuser( $usuario->usucpf );

        $descricao = "Usuário solicitou a ativação da conta e apresentou a seguinte justificativa: ". $justificativa;
        if($usuario->usucpf){
            if($sisid){
                $sql = sprintf(
                    "SELECT us.* FROM seguranca.usuario_sistema us WHERE us.sisid = %d AND us.usucpf = '%s'",
                    $sisid,
                    $usuario->usucpf
                );
                $usuario_sistema = (object) $db->pegaLinha($sql);
                if ( $usuario_sistema->suscod == 'B' ) {
                    $db->alterar_status_usuario($cpf, 'P', $descricao, $usuario_sistema->sisid);
                }
            } else if($usuario->suscod == 'B'){
                $db->alterar_status_usuario($cpf, 'P', $descricao);
            }
        }
        $db->commit();
        $_SESSION['MSG_AVISO'] = array("Seu pedido foi submetido e será avaliado em breve.");

        if($sisid = 48){
            header("Location: sigest.php");
        } else {
            header("Location: login.php");
        }
        exit();
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
                         <span class="glyphicon glyphicon-user"></span> Solicitação de Ativação Conta <br>
                         <p>
                             <?php echo 'Preencha os Dados Abaixo e clique no botão: "Enviar Solicitação".<br/>' . obrigatorio() . ' Indica campo Obrigatório.'; ?>
                         </p>
                     </div>
                     <div class="panel-body">
                         <form method="POST" name="formulario">
                             <input type="hidden" name="formulario" value="1"/>
                             <div class="form-group">
                                 <div class="col-sm-12">
                                     <input type="text" maxlength="14" class="form-control" name="usucpf" id="usucpf" placeHolder="CPF" required="">
                                 </div>
                             </div>
                             <?php if($_REQUEST['sisid']): ?>
                             <div class="form-group">
                                 <div class="col-sm-12">
                                    <?php
                                        $sql = "
                                            SELECT
                                                s.sisid AS codigo,
                                                s.sisabrev AS descricao
                                            FROM seguranca.sistema s
                                            WHERE
                                                s.sisstatus = 'A'
                                                AND sismostra = 't'
                                            ORDER BY
                                                descricao";
                                        $sistemas = $db->carregar($sql);
                                        $select = '';

                                        if ($sistemas) {
                                            $select .= '<select name="sisid_modid" ' . $disabled . ' class="chosen-select" style="width: 100%" onchange="sel_modulo(this);">';
                                            $select .= '<option value="">Selecione...</option>';

                                            foreach ($sistemas as $sistema) {
                                                $select .= '<option value="' . $sistema['codigo'] . '"' . ($sisid == $sistema['codigo'] ? 'selected' : '') . '>' . $sistema['descricao'] . '</option>';
                                            }
                                            $select .= '</select>';
                                        }
                                        echo $select;
                                    ?>
                                 </div>
                             </div>
                             <?php endif; ?>
                             <div class="form-group">
                                 <div class="col-sm-12">
                                     <textarea name="htudsc" cols="45" rows="3" placeHolder="Justificativa"><?= $observacao ?></textarea>
                                 </div>
                             </div>
                             <div class="form-group" style="font-size: 14px;">
                                 <div class="col-sm-12 pull-right">
                                     <a class="btn btn-success" href="javascript:enviar_formulario();"><span class="glyphicon glyphicon glyphicon glyphicon-ok"></span> Enviar Solicita&ccedil;&atilde;o</a>
                                     <a class="btn btn-danger" href="javascript:history.back();"><span class="glyphicon glyphicon glyphicon glyphicon-remove"></span> Cancelar</a>
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
<script>

    $(function(){

        // Mascara de CPF
        $('#usucpf').keyup(function(){
            mascara(this, mcpf);
        });

    });

    function enviar_formulario() {
        if ( validar_formulario() ) {
            document.formulario.submit();
        }
    }

    function validar_formulario() {
        var validacao = true;
        var mensagem = '';
        if ( document.formulario.usucpf.value == "" ) {
            mensagem += '\nInforme o cpf.';
            validacao = false;
        }
        if ( document.formulario.htudsc.value == "" ) {
            mensagem += '\nVocê deve justificar o pedido.';
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