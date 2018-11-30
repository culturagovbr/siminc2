<?php

    /**
     * Sistema Integrado de Monitoramento do Ministério da Educação
     * Setor responsvel: SPO/MEC
     * Desenvolvedor: Desenvolvedores Simec
     * Analistas: Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>, Alexandre Soares Diniz
     * Programadores: Renê de Lima Barbosa <renedelima@gmail.com>, Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>
     * Módulo: Segurança
     * Finalidade: Solicitação de cadastro de contas de usuário.
     * Data de criação:
     * Última modificação: 30/08/2006
     */
    define("SIS_PDEESCOLA", 34);
    define("SIS_PSEESCOLA", 65);

    // carrega as bibliotecas internas do sistema
    require_once "config.inc";
    require_once APPRAIZ . "includes/classes_simec.inc";
    require_once APPRAIZ . "includes/funcoes.inc";

    // abre conexão com o servidor de banco de dados
    $db = new cls_banco();

    if (!$theme) {
        $theme = $_SESSION['theme_temp'];
    }

    // Particularidade feita para o PDE Escola
    $selecionar_modulo_habilitado = 'S';
    if ($_REQUEST['banner_pdeescola'] == 'acessodireto') {
        $selecionar_modulo_habilitado = 'N';
        $_REQUEST['sisid'] = SIS_PDEESCOLA;
    }
    if ($_REQUEST['banner_pseescola'] == 'acessodireto') {
        $selecionar_modulo_habilitado = 'N';
        $_REQUEST['sisid'] = SIS_PSEESCOLA;
    }

    $sisid = $_REQUEST['sisid'];
    $modid = $_REQUEST['modid'];
    $usucpf = $_REQUEST['usucpf'];

    // leva o usuário para o passo seguinte do cadastro
    if ($_REQUEST['usucpf'] && $_REQUEST['modulo'] && $_REQUEST['varaux'] == '1') {
        $_SESSION = array();
        if($theme){
            $_SESSION['theme_temp'] = $theme;
        }
        header("Location: cadastrar_usuario_2.php?sisid=$sisid&modid=$modid&usucpf=$usucpf");
        exit();
    }

?>

<!DOCTYPE html>
<html lang="en">
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

        <?php if (strlen($mensagens) > 5) : ?>
            <div class="error_msg"><? echo (($mensagens)?$mensagens:""); ?></div>
        <?php endif; ?>

        <!-- Register -->
        <section id="register" class="register">
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
                    <div class="panel panel-default">
                        <div class="panel-heading">
                            <span class="glyphicon glyphicon-user"></span> Solicitação de cadastro de usuários <br>
                        </div>
                        <div class="panel-body">
                            <form name="formulario" id="formulario" class="form-horizontal" role="form" method="post" action="" onsubmit="return false;">
                                <input type=hidden name="modulo" value="./inclusao_usuario.php"/>
                                <input type=hidden name="varaux" value="">
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
                                                if ($selecionar_modulo_habilitado == 'S') {
                                                    $disabled = '';
                                                } else {
                                                    $disabled = 'disabled="disabled"';
                                                }

                                                /*                                             * * Inicia a montagem do combo ** */
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

                                <input type="hidden" name="sisid" id="sisid" value="<?php echo $sisid; ?>" />
                                <input type="hidden" name="modid" id="modid" value="<?php echo $modid; ?>" />

                                <div class="form-group">
                                    <div class="col-sm-12">
                                        <?php if ($sisid): ?>
                                            <?php $sql = sprintf("select sisid, sisdsc, sisfinalidade, sispublico, sisrelacionado from sistema where sisid = %d", $sisid); ?>
                                            <?php $sistema = (object) $db->pegaLinha($sql); ?>
                                            <?php if ($sistema->sisid) : ?>
                                                <div class="sistema-texto">
                                                    <h2><?php echo $sistema->sisdsc ?></h2><br/>
                                                    <p><?php echo $sistema->sisfinalidade ?></p>
                                                    <ul>
                                                        <li><i class="fa fa-bullseye"></i>&nbsp;&nbsp;&nbsp;Público-Alvo: <?php echo $sistema->sispublico ?><br></li>
                                                        <li><i class="fa fa-cubes"></i> Sistemas Relacionados: <?php echo $sistema->sisrelacionado ?></li>
                                                    </ul>
                                                </div>
                                            <?php endif; ?>
                                        <?php endif; ?>
                                        <input type="hidden" name="sisfinalidade_selc" value="<?php echo $sisfinalidade_selc; ?>"/>
                                    </div>
                                </div>

                                <div class="form-group">
                                    <div class="col-sm-12">
                                        <input type="text" class="form-control cpf" name="usucpf" id="usucpf" placeHolder="CPF" required="" value=<?php echo $usucpf; ?>/>
                                    </div>
                                </div>
                                <div class="form-group" style="font-size: 14px;">
                                    <div class="col-sm-12 text-right">
                                        <a class="btn btn-success" href="javascript:validar_formulario()"><span class="glyphicon glyphicon glyphicon glyphicon-ok"></span> Continuar</a>
                                        <a class="btn btn-danger" href="<?php echo URL_SISTEMA; ?>" ><span class="glyphicon glyphicon glyphicon glyphicon-remove"></span> Cancelar</a>
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
        <!--/Register -->

    </body>

<!-- Custom Theme JavaScript -->
<script>
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

    function ImprimeStatus(texto){
        document.formul.numCaracteres.value = texto
    }

    function sel_modulo(obj){
    	if( obj.value == "" ){
            document.getElementById('sisid').value = "";
            document.getElementById('modid').value = "";
        } else {
            var option = obj.options[obj.selectedIndex];
            var sisid = option.parentNode.id;

            if( sisid == "" ){
            	document.getElementById('sisid').value = obj.value;
                document.getElementById('modid').value = "";
            } else {
            	document.getElementById('sisid').value = sisid;
                document.getElementById('modid').value = obj.value;
            }
            document.getElementById('formulario').submit();
        }
    }

    function selecionar_modulo(){
    	document.formulario.submit();
    }

    function validar_formulario(){
    	var validacao = true;
    	var mensagem  = '';

        if (!validar_cpf(document.formulario.usucpf.value)) {
            mensagem += '\nO cpf informado não é válido.';
            validacao = false;
        }

        document.formulario.varaux.value = '1';

        if ( !validacao ) {
            alert( mensagem );
        } else {
            document.formulario.submit();
        }
    }
</script>
</html>
<?php $db->close(); ?>