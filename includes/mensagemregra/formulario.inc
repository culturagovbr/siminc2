<?php
global $db;

include_once( APPRAIZ . "seguranca/classes/MensagemRegra.class.inc" );
include_once( APPRAIZ . "seguranca/classes/MensagemCampo.class.inc" );

$modelMensagemRegra = new MensagemRegra($_REQUEST['mreid']);

if($_REQUEST['requisicao'] == 'atualizarEmail') {
    $listaPerfil = $_REQUEST['listaPerfil'];
    $sisid = $_SESSION['sisid'];
    $sql = "
        SELECT
            DISTINCT
            u.usunome AS nome,
            u.usucpf AS cpf,
            u.usuemail AS email,
            u.usufoneddd AS ddd,
            u.usufonenum AS telefone
        FROM
            seguranca.usuario u
            JOIN seguranca.perfilusuario pu ON(u.usucpf = pu.usucpf)
            JOIN seguranca.perfil p ON(pu.pflcod = p.pflcod)
            JOIN seguranca.sistema s ON(p.sisid = s.sisid)
        WHERE
            p.sisid = {$sisid}
            AND p.pflcod IN({$listaPerfil})
        order by nome
    ";
    $listaEmail = $db->carregar($sql);
    $listaEmailSelecionado = explode(',', $_REQUEST['listaEmailSelecionado']);

    foreach($listaEmail as $email){
        $atributoChecked = in_array($email['cpf'], $listaEmailSelecionado)? 'checked=checked': '';
        echo "<input class='chk_email_avisados' type=checkbox value=".$email['cpf']." name='email_avisados[]' $atributoChecked />". $email['nome']. ' - ' .$email['email']." - (".$email['ddd'].") ".$email['telefone']."<br />";
    }
    die;
}

if ($_POST['mretipo']) {
    $modelMensagemRegra->popularDadosObjeto();
    $modelMensagemRegra->mreenviaemail = $_POST['mreenviaemail'] ? 't' : 'f';
    $modelMensagemRegra->mreenviasms = $_POST['mreenviasms'] ? 't' : 'f';
//    ver($modelMensagemRegra, d);

    if(!$modelMensagemRegra->mreid){
        $modelMensagemRegra->mredatacriacao = date('Y-m-d H:i:s');
    }

    $modelMensagemRegra->salvar();
    $modelMensagemRegra->Commit();
//ver($_POST, $modelMensagemRegra, d);

    // SOLICITAÇÃO DE ACESSO
    if($_POST['mretipo'] == 'A'){

//        $sql = "delete from pde.enemservidor where ediid = '{$modelMensagemRegra->ediid}'";
//        $mctid = $modelMensagemRegra->pegaUm($sql);
        $modelMensagemCampo = new MensagemCampo();
        $modelMensagemCampo->excluirVarios(" mreid = '{$modelMensagemRegra->mreid}' ");
        unset($modelMensagemCampo);
        if(is_array($_POST['perfis_solicitados'])){
            foreach($_POST['perfis_solicitados'] as $valor){
                $modelMensagemCampo = new MensagemCampo();
                $modelMensagemCampo->mreid    = $modelMensagemRegra->mreid;
                $modelMensagemCampo->mctid    = MensagemCampo::ID_PFL_PERFIL_SOLICITADO;
                $modelMensagemCampo->mcavalor = $valor;

                $modelMensagemCampo->salvar();
                $modelMensagemCampo->Commit();
                unset($modelMensagemCampo);
            }
        }
        
        // Deleta os emails registrados anteriormente
        $sql = "
            DELETE FROM 
              seguranca.mensagemcampo 
            WHERE 
             mreid = {$modelMensagemRegra->mreid}
             AND mctid = ".MensagemCampo::ID_WKF_ESTADO_ORIGEM;
        $db->executar($sql);
        // Insere os emails selecionados pelo usuario
        if(is_array($_POST['email_avisados'])){
            foreach($_POST['email_avisados'] as $valor){
                $modelMensagemCampo = new MensagemCampo();
                $modelMensagemCampo->mreid    = $modelMensagemRegra->mreid;
                $modelMensagemCampo->mctid    = MensagemCampo::ID_WKF_ESTADO_ORIGEM;
                $modelMensagemCampo->mcavalor = $valor;

                $modelMensagemCampo->salvar();
                $modelMensagemCampo->Commit();
                unset($modelMensagemCampo);
            }
        }
    }

    if(is_array($_POST['perfis_avisados'])){
        foreach($_POST['perfis_avisados'] as $valor){
            $modelMensagemCampo = new MensagemCampo();
            $modelMensagemCampo->mreid    = $modelMensagemRegra->mreid;
            $modelMensagemCampo->mctid    = MensagemCampo::ID_PFL_PERFIL_AVISADO;
            $modelMensagemCampo->mcavalor = $valor;

            $modelMensagemCampo->salvar();
            $modelMensagemCampo->Commit();
            unset($modelMensagemCampo);
        }
    }

    die("<script>
            alert('Operação realizada com sucesso!');
            window.location = '?modulo=sistema/mensagemregra/formulario&acao=A&mreid=" . $modelMensagemRegra->mreid . "';
        </script>");
}
?>

<link href="css/jquery-ui/custom-theme/jquery-ui-1.10.3.custom.css" rel="stylesheet">
<script src="js/jquery-1.9.1.js"></script>
<script src="js/jquery-ui-1.10.3.custom.js"></script>
<link href="css/estilo.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/funcoes.js"></script>

<style>
    .label_campo{
        margin-left: 2px;
        margin-right: 10px;
    }
</style>

<?php
extract($_REQUEST);

include APPRAIZ . 'includes/cabecalho.inc';

$db->cria_aba($abacod_tela, $url, '');
monta_titulo($titulo_modulo, '&nbsp;');
?>
<?php
/*

  CREATE TABLE seguranca.mensagemregra
(
  mreid serial NOT NULL, -- Chave Primária
  sisid integer NOT NULL, -- Identificador do sistema
  mretextoemail text, -- Texto da mensagem por email
  mretextocelular character varying(150), -- Texto da mensagem por celular
  mreenviaemail boolean, -- Identificador de envio de email
  mreenviasms boolean, -- Identificador de envio de sms
  mretipo character(1) NOT NULL, -- Tipo da Regra:...
  mretituloemail character varying(200), -- Título do email
  CONSTRAINT mreid_pk PRIMARY KEY (mreid)
)

  * */

$modelMensagemRegra->sisid = $_SESSION['sisid'];
?>

<form action="" method="post" name="formulario" id="formulario" class="form-control">
    <input type="hidden" name="mreid" id="mreid" value="<?php echo $modelMensagemRegra->mreid; ?>"/>
    <table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr>
            <td align='right' class="SubTituloDireita" style="width:25%;">Módulo:</td>
            <td>
                <?php
                $sql = "select sisid as codigo, sisdsc || ' (' || sisabrev || ')' as descricao
                        from seguranca.sistema
                        where sisid = '{$_SESSION['sisid']}'
                        order by descricao";

                $sisid = $modelMensagemRegra->sisid;
                echo $db->monta_combo("sisid",$sql,'N',"Selecione...","","","","400","S","sisid","",$modelMensagemRegra->sisid);
                ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="width:25%;">Tipo:</td>
            <td>
                <?php
                $sql = "
                    select 'A' as codigo, 'Solicitação de Acesso' as descricao
                    union
                    select 'W' as codigo, 'Workflow' as descricao
                ";

                echo $db->monta_combo("mretipo",$sql,'S',"Selecione...","","","","400","S","mretipo","",$modelMensagemRegra->mretipo);
                ?>
            </td>
        </tr>
        <tr class="camposemail">
            <td align='right' class="SubTituloDireita" style="width:25%;">Descrição:</td>
            <td><?= campo_texto('mredescricao', 'S', 'S', '', 40, 200, '', '', '', '', '', 'id="mredescricao"', '', $modelMensagemRegra->mredescricao); ?></td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="width:25%;">Enviar via:</td>
            <td>
                <input type="checkbox" class="enviaemail" name="mreenviaemail" id="mreenviaemail" value="t" <?php echo $modelMensagemRegra->mreenviaemail == 't' || !$modelMensagemRegra->mreid ? 'checked="checked"' : ''  ?> /><label for="mreenviaemail" class="label_campo">E-mail</label>
                <input type="checkbox" class="enviasms" name="mreenviasms" id="mreenviasms" value="t" <?php echo $modelMensagemRegra->mreenviasms == 't' ? 'checked="checked"' : ''  ?> /><label for="mreenviasms" class="label_campo">SMS</label>
            </td>
        </tr>
        <tr class="camposemail">
            <td align='right' class="SubTituloDireita" style="width:25%;">Assunto:</td>
            <td><?= campo_texto('mretituloemail', 'N', 'S', '', 40, 200, '', '', '', '', '', 'id="mretituloemail"', '', $modelMensagemRegra->mretituloemail); ?></td>
        </tr>
        <tr class="camposemail">
            <td align='right' class="SubTituloDireita" style="width:25%;">Texto Email:</td>
            <td><?= campo_textarea('mretextoemail', 'N', 'S', '', 50, 5, 5000, '', null, null, null, null, $modelMensagemRegra->mretextoemail, '400px'); ?></td>
        </tr>
        <tr class="campossms">
            <td align='right' class="SubTituloDireita" style="width:25%;">Texto SMS:</td>
            <td><?= campo_textarea('mretextocelular', 'N', 'S', '', 20, 2, 150, '', null, null, null, null, $modelMensagemRegra->mretextocelular, '400px'); ?></td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="width:25%;">Perfis solicitados:</td>
            <td>
                <?php
                
                $sql = "
                    SELECT
                        p.pflcod AS codigo,
                        p.pfldsc || ' (' || s.sisabrev || ')' AS descricao
                    FROM
                        seguranca.perfil p
                        JOIN seguranca.sistema s ON s.sisid = p.sisid
                    WHERE
                        p.sisid = '{$_SESSION['sisid']}'
                        AND p.pflstatus = 'A'
                        AND s.sisstatus = 'A'
                    ORDER BY
                        descricao
                ";
                
                if($modelMensagemRegra->mreid){
                    $sqlCarregar = "select mcavalor as codigo, p.pfldsc || ' (' || s.sisabrev || ')' as descricao
                                    from seguranca.mensagemcampo mc
                                        inner join seguranca.perfil p on p.pflcod::text = mc.mcavalor
                                        inner join seguranca.sistema s on s.sisid = p.sisid
                                    where mreid = '{$modelMensagemRegra->mreid}'
                                    and mctid = " . MensagemCampo::ID_PFL_PERFIL_SOLICITADO . " ";

                    $perfis_solicitados = $modelMensagemRegra->carregar($sqlCarregar);
                }
                combo_popup( "perfis_solicitados", $sql, "Perfis Solicitados", "400x400", 50, array(), "", "S", false, false, 5, 400 );
                ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="width:25%;">
                Perfis a serem avisados:
                <br />
                <span style="color: #ff0000">Utilize esse campo caso queira enviar e-mail para todos os usuários que sejam do perfil selecionado.</span>
            </td>
            <td>
                <?php

                if($modelMensagemRegra->mreid){
                    $sqlCarregar = "select mcavalor as codigo, p.pfldsc || ' (' || s.sisabrev || ')' as descricao
                                    from seguranca.mensagemcampo mc
                                        inner join seguranca.perfil p on p.pflcod::text = mc.mcavalor
                                        inner join seguranca.sistema s on s.sisid = p.sisid
                                    where mreid = '{$modelMensagemRegra->mreid}'
                                    and mctid = " . MensagemCampo::ID_PFL_PERFIL_AVISADO . " ";

                    $perfis_avisados = $modelMensagemRegra->carregar($sqlCarregar);
                }
                combo_popup( "perfis_avisados", $sql, "Perfis a serem avisados", "400x400", 50, array(), "", "S", false, false, 5, 400 );
                ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="width:25%;">
                Usuários de perfis a serem avisados:
                <br />
                <span style="color: #ff0000">Utilize esse campo caso queira enviar e-mail para usuários específicos selecionados no campo abaixo.</span>
            </td>
            <td>
                <?php
                    $perfils_avisados = $db->carregar($sql);
                ?>
                <select style="width:400px;" class="CampoEstilo" id="perfis_email_avisados" name="perfis_email_avisados[]" size="5" multiple="multiple" tipo="combo_popup" maximo="50">
                    <?php foreach($perfils_avisados as $perfil): ?>
                        <option value="<?php echo $perfil['codigo']; ?>"><?php echo $perfil['descricao']; ?>
                    <?php endforeach; ?>
                </select>
            </td>
        </tr>
        <?php
            $listaEmail = FALSE;
            if($modelMensagemRegra->mreid){

                $sql = "
                    SELECT DISTINCT
                            u.usucpf AS cpf,
                        u.usuemail AS email,
                        u.usufoneddd AS ddd,
                        u.usufonenum AS telefone
                    FROM
                      seguranca.mensagemcampo mc
                      JOIN seguranca.usuario u ON(mc.mcavalor = u.usucpf)
                    WHERE
                        mc.mreid = {$modelMensagemRegra->mreid}
                        AND mc.mctid = ".MensagemCampo::ID_WKF_ESTADO_ORIGEM;
                $listaEmail = $db->carregar($sql);
            }
            $atributoDisplay = $listaEmail==false? 'style="display: none;"': '';
        ?>
        <tr id="tr_check_email" <?php echo $atributoDisplay; ?> >
            <td align='right' class="SubTituloDireita" style="width:25%;">
                Emails a serem avisados:
            </td>
            <td id="td_check_email">
                <?php
                    if($listaEmail!=false){
                        foreach($listaEmail as $email){
                            echo "<input class='chk_email_avisados' type=checkbox value=".$email['cpf']." name='email_avisados[]' checked=checked />".$email['email']." (".$email['ddd'].") ".$email['telefone']."<br />";
                        }
                    }
                ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="width:25%;">Status:</td>
            <td>
                <?php
                $sql = "
                    select 'A' as codigo, 'Ativo' as descricao
                    union
                    select 'I' as codigo, 'Inativo' as descricao
                ";

                echo $db->monta_combo("mrestatus",$sql,'S',"Selecione...","","","","150","S","mrestatus","",$modelMensagemRegra->mrestatus);
                ?>
            </td>
        </tr>
        <tr id="tr_botoes_acao" style="background-color: #cccccc">
            <td align='right' style="vertical-align:top; width:25%;">&nbsp;</td>
            <td>
                <input type="button" name="botao_enviar" id="botao_enviar" value="Salvar" />
                <input type="button" name="botao_voltar" id="botao_voltar" value="Voltar" />
            </td>
        </tr>
    </table>
</form>

<script type="text/javascript">
    jQuery(function() {

        verificarCamposEmail();
        verificarCamposSms();

        jQuery('#botao_voltar').click(function() {
            window.location = '?modulo=sistema/mensagemregra/listar&acao=A';
        });

        jQuery('#mreenviaemail').click(function(){
            verificarCamposEmail();
        });

        jQuery('#mreenviasms').click(function(){
            verificarCamposSms();
        });

        jQuery('#botao_enviar').click(function() {

            selectAllOptions(document.getElementById('perfis_avisados'));
            selectAllOptions(document.getElementById('perfis_solicitados'));

            if (!jQuery('#sisid').val()) {
                alert('O campo [Módulo] é obrigatório.');
                return false;
            }

            if (!jQuery('#mretipo').val()) {
                alert('O campo [Tipo] é obrigatório.');
                return false;
            }

            if (!jQuery('#mredescricao').val()) {
                alert('O campo [Descrição] é obrigatório.');
                return false;
            }

            if (!jQuery('#mrestatus').val()) {
                alert('O campo [Status] é obrigatório.');
                return false;
            }

            if (jQuery('#mreenviaemail').is(':checked') && (!jQuery('#mretituloemail').val() || !jQuery('#mretextoemail').val())) {
                alert('Favor preencher o assunto e o texto do email');
                return false;
            }

            if (jQuery('#mreenviasms').is(':checked') && !jQuery('#mretextocelular').val()) {
                alert('Favor preencher o texto do SMS');
                return false;
            }

            jQuery('#formulario').submit();
        });

        jQuery('#perfis_email_avisados').change(function(){
            jQuery('#tr_check_email').show();
            var listaSelecionado = '';
            jQuery('.chk_email_avisados:checked').each(function(key, data){
                listaSelecionado += jQuery(data).val()+',';
            });
            var listaPerfil = jQuery(this).val();
            jQuery('#td_check_email').empty().load(
                window.location+"&requisicao=atualizarEmail&listaPerfil="+listaPerfil+"&listaEmailSelecionado="+listaSelecionado);
        });
    });

    function verificarCamposEmail()
    {
        if (jQuery('#mreenviaemail').is(':checked')) {
            jQuery('.camposemail').show();
        } else {
            jQuery('.camposemail').hide();
        }
    }

    function verificarCamposSms()
    {
        if (jQuery('#mreenviasms').is(':checked')) {
            jQuery('.campossms').show();
        } else {
            jQuery('.campossms').hide();
        }
    }

</script>