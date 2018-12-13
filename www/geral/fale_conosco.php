<?php

    include_once "config.inc";
    include_once APPRAIZ."includes/funcoes.inc";
    include_once APPRAIZ."includes/classes_simec.inc";
    
    # Verifica se a sessão não expirou, se tiver expirada envia pra tela de login.
//    controlarAcessoSemAutenticacao();
    
    include_once APPRAIZ . "includes/Email.php";

    $db = new cls_banco();

    function pegaDadosFaleConosco( $intSisId ){
        global $db;

        $strSql = "
            select *
            from public.fale_conosco
            where sisid = $intSisId
        ";
        $arrDadosFaleConosco = $db->pegaLinha( $strSql );

        if ( !$arrDadosFaleConosco ){
            $strSql ="
                select *
                from public.fale_conosco
                where sisid is null
            ";
            $arrDadosFaleConosco = $db->pegaLinha( $strSql );
        }

        return $arrDadosFaleConosco ? $arrDadosFaleConosco : array();
    }

    function enviaEmail($strAssunto, $strMensagem, $arrDadosFaleConosco) {
        global $db;

        if (trim($strMensagem) == "") {
            throw new Exception("O Texto do e-mail não pode estar vazio.");
        }

        $strEmailTo = $arrDadosFaleConosco['flcemail'];

        $objMensagem = new Email();
        /*
          $objMensagem->enviar(
          array(),
          $strAssunto,
          $strMensagem,
          array(),
          false,
          true,
          array( $strEmailTo ),
          false
          );
         */
        $db->commit();
    }

    function preparaTelaEnviaEmail( $arrDadosFaleConosco ){
        if ( count( $arrDadosFaleConosco ) == 0 ){
?>
            <html>
                <head>
                    <title>Fale conosco</title>
                    <link rel="stylesheet" type="text/css" href="/../includes/Estilo.css">
                    <link rel='stylesheet' type='text/css' href='/../includes/listagem.css'>
                    <script language="JavaScript" src="/../includes/funcoes.js"></script>
                </head>
                <body bgcolor="#ffffff">
                    <div style="margin: 100px 20px; text-align: center;">
                        Este módulo não disponibiliza esta funcionalidade.
                        <br/>
                        </br>
                        <a href="javascript:window.close();">fechar janela</a>
                    </div>
                </body>
            </html>
<?PHP
            return;
        }
        #FIM DO IF
?>
        <html>
            <head>
                <title>Fale conosco</title>
                <link rel="stylesheet" type="text/css" href="/../includes/Estilo.css">
                <link rel='stylesheet' type='text/css' href='/../includes/listagem.css'>
                <script language="JavaScript" src="/../includes/funcoes.js"></script>
                <script language="javascript" type="text/javascript" src="/../includes/tiny_mce.js"></script>
                <script language="JavaScript">
                    //Editor de textos
                    tinyMCE.init({
                        mode : "textareas",
                        theme : "advanced",
                        plugins : "table,save,advhr,advimage,advlink,emotions,iespell,insertdatetime,preview,zoom,flash,searchreplace,print,contextmenu,paste,directionality,fullscreen",
                        theme_advanced_buttons1 : "undo,redo,separator,bold,italic,underline,forecolor,backcolor,fontsizeselect,separator,justifyleft,justifycenter,justifyright, justifyfull, separator, outdent,indent, separator, bullist, code",
                        theme_advanced_buttons2 : "",
                        theme_advanced_buttons3 : "",
                        theme_advanced_toolbar_location : "top",
                        theme_advanced_toolbar_align : "left",
                        extended_valid_elements : "a[name|href|target|title|onclick],img[class|src|border=0|alt|title|hspace|vspace|width|height|align|onmouseover|onmouseout|name],hr[class|width|size|noshade],font[face|size|color|style],span[class|align|style]",
                        language : "pt_br",
                        entity_encoding : "raw"
                    });
                </script>
                <script>
                    function fechar_janela(){
                            window.close();
                    }

                    function envia_email(){
                        if ( !validaBranco( document.formulario.assunto, 'Assunto') ){
                            return;
                        }
                        // verificação do campo corpo email
                        document.formulario.email.value = tinyMCE.getContent( 'email' );
                        if ( !validaBranco( document.formulario.email, 'Texto da Mensagem' ) ){
                                return tinyMCE.execCommand( 'mceFocus', true, 'email' );
                        }
                        document.formulario.submit();
                    }

                </script>
            </head>

            <body bgcolor="#ffffff" leftmargin="0" rightmargin="0" topmargin="0" bottommargin="0" marginheight="0" marginwidth="0">
                <form method="POST"  name="formulario">
                    <input type=hidden name="modulo" value="<?= $modulo ?>">                        
                    <table align="center" border="0" class="listagem" cellpadding="3" cellspacing="1">
                        <tr>
                            <td>
                              <?= $arrDadosFaleConosco['flcconteudo'] ?>
                            </td>
                        </tr>
                    </table>
                    
                    <br>
                    
                    <table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
                        <tr>
                            <td colspan="2" align="Center" bgcolor="#dedede">
                                Enviar Email
                            </td>
                        </tr>
                        <tr>
                            <td width="12%" class="subtitulodireita">
                                Assunto:
                            </td>
                            <td>
                                <?= campo_texto('assunto', 'S', 'S', '', 106, 100, '', '') ?>
                            </td>
                        </tr>
                        <tr>
                            <td align="right" class="subtitulodireita">
                                &nbsp;
                            </td>
                            <td colspan="2">
                                <?= campo_textarea('email', 'N', 'S', '', '82%', 12, '') ?>
                            </td>
                        </tr>
                        <tr>
                            <td colspan="2" align="Center" bgcolor="#dedede">
                                <input type='button' class="botao" value='Enviar E-mail' onclick="envia_email()">
                                &nbsp;&nbsp;&nbsp;
                                <input type='button' class="botao" value='Fechar' onclick="fechar_janela()">
                            </td>
                        </tr>
                    </table>
                </form>
            </body>
        </html>
    <?PHP

    }
    #FIM DA FUNÇÃO.

    $intAno 	= (integer)$_SESSION['exercicio'];
    $intSisId	= (($_REQUEST['sisid']) ? (integer)$_REQUEST['sisid'] : (integer)$_SESSION['sisid']);

    //$intSisId	= (($_SESSION['sisid']) ? (integer)$_SESSION['sisid'] : (integer)$_REQUEST['sisid']);

    if( $intSisId == 0 ){
        exit();
    }

    $arrDadosFaleConosco    = pegaDadosFaleConosco( $intSisId );
    $strAssunto             = $_REQUEST[ 'assunto' ];
    $strMensagem            = $_REQUEST[ 'email' ];

    if( $strMensagem ){
        try {
            enviaEmail( $strAssunto , $strMensagem , $arrDadosFaleConosco );
?>
            <script>
                alert( 'Email enviado com sucesso. Esta janela será fechada.' );
                window.close();
            </script>
<?PHP
        }

        catch( Exception $objError ){
?>
            <script>
                alert( "<?= str_replace( "\"", "'", $objError->getMessage() ) ?>" );
                history.back();
            </script>
<?PHP
        }
    }else{
        preparaTelaEnviaEmail( $arrDadosFaleConosco );
    }
?>
