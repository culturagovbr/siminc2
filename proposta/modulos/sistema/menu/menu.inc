<?php
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   Módulo:menu.inc
   Finalidade: permitir o cadastro de itens de menu
   */
   	
$modulo=$_REQUEST['modulo'] ;//
if ($_REQUEST['act'] == 'inserir')
{
   //verifica se existe módulo igual cadastrado
   $sql = "select count(*) as total from menu where mnulink<>'' and sisid = ". $_SESSION['sisid'] . " and trim(mnulink)='".trim($_REQUEST['mnulink'])."'";
   $RS = $db->recuperar($sql,$res);
   if ($RS['total']>0){ ?>
        <script language="JavaScript">alert('Este Menu/Módulo já se encontra cadastrado!');history.back();</script>
   <?php
   $db -> close();
   exit();
   }
   
   // fazer inserção de menu na base de dados.
   if (! $_REQUEST['abacod']){ 
       $_REQUEST['abacod']='null';
   }
   $sql = "INSERT INTO
                seguranca.menu 
                    (mnucod,mnuidpai,mnudsc,mnutransacao,mnulink,mnutipo,mnustile,mnuhtml, mnusnsubmenu,mnushow,mnustatus,abacod,sisid) 
           VALUES ("
                      .$_REQUEST['mnucod'].",";
   if ($_REQUEST['mnuidpai']){
       $sql = $sql.$_REQUEST['mnuidpai'].","; 
   }else{
       $sql = $sql.' null,';
   }
        $sql = $sql .
        "'".$_REQUEST['mnudsc']."',".
        "'".$_REQUEST['mnutransacao']."',".
        "'".$_REQUEST['mnulink']."',".
        "'".$_REQUEST['mnutipo']."',".
        "'".$_REQUEST['mnustile']."',".
        "'".$_REQUEST['mnuhtml']."',".
        "'".$_REQUEST['mnusnsubmenu']."',".
        "'".$_REQUEST['mnushow']."',".
        "'A',".$_REQUEST['abacod'].",".$_SESSION['sisid'].")";
        
        $saida = $db->executar($sql);
        $db->commit();
        $db->sucesso($modulo);
}
if ($_REQUEST['act']=='alterar')
{
	//verifica se existe outro módulo igual alterado
   $sql = "SELECT count(*) as total FROM seguranca.menu WHERE mnulink<>'' AND sisid = " . $_SESSION['sisid'] . " AND mnulink='".$_REQUEST['mnulink']."' AND mnuid<>".$_REQUEST['mnuid_int'];
   $RS = $db->recuperar($sql,$res);
   if ($RS['total']>0)
   {
   ?>
   <script language="JavaScript">alert('Este Menu/Módulo já se encontra cadastrado!');history.back();</script>
   <?php
   $db -> close();
   exit();
   }
   // fazer alteração do menu na base de dados.
   if ($_REQUEST['mnusnsubmenu'] == 't'){
       $_REQUEST['mnusnsubmenu'] = 'true' ; 
   }else{
       $_REQUEST['mnusnsubmenu'] = 'false';
   }
   if ($_REQUEST['mnushow'] == 't'){ 
       $_REQUEST['mnushow'] = 'true' ;
   }else{
       $_REQUEST['mnushow'] = 'false';
   }
   
   $sql = "UPDATE seguranca.menu SET mnucod=".$_REQUEST['mnucod'].", mnudsc='".
                                                $_REQUEST['mnudsc'].
                                                "', mnutransacao='".$_REQUEST['mnutransacao'].
                                                "',  mnulink='".$_REQUEST['mnulink'].
                                                "',mnutipo='".
                                                $_REQUEST['mnutipo'].
                                                "',   mnustile='".
                                                $_REQUEST['mnustile'].
                                                "',   mnuhtml='".$_REQUEST['mnuhtml']."',mnuidpai=";
   if ($_REQUEST['mnuidpai']){ 
       $sql = $sql.$_REQUEST['mnuidpai'].","; 
   }else{ 
       $sql = $sql.' null,';
   }
   
   $sql = $sql." abacod = ";
   
   if ($_REQUEST['abacod']){ 
       $sql = $sql.$_REQUEST['abacod'].",";
   } else{ 
       $sql = $sql.' null,';
   }
   
   $sql = $sql.
            "  mnusnsubmenu=".$_REQUEST['mnusnsubmenu'].
            ",  mnushow=".$_REQUEST['mnushow'].
            "  where mnuid=".$_REQUEST['mnuid_int'];
   
    $saida = $db->executar($sql);
    $db->commit();?>
        <script type="text/javascript" language="javascript">
              alert('Operação realizada com sucesso');
              location.href="<?=$_SESSION['sisdiretorio']?>.php?modulo=<?=$modulo.'&acao=A&mnuid_int='.$_REQUEST['mnuid']?>";
        </script><?php
    $db -> close();
    exit();
}

if ($_POST['exclui'] > 0) 
{
    
    $perfilmenu = $db->pegaUm("SELECT * FROM perfilmenu WHERE mnuid = ".$_POST['exclui']);
    if($perfilmenu > 0){
        echo  "<script> alert('Menu vinculado a uma perfil. Primeiro desvincule perfil do menu que deseja excluir na  aba Associar Menu. Verifique suas permissões.');</script>";
    }else{
        $sql = "DELETE FROM seguranca.estatistica WHERE mnuid=".$_POST['exclui'];
        $saida = $db->executar($sql);
        //$sql = "DELETE FROM auditoria WHERE mnuid=".$_POST['exclui'];
        $saida = $db->executar($sql);	  
        $sql = "DELETE FROM seguranca.menu WHERE mnuid=".$_POST['exclui'];
        // não pode deixar escluir se já estiver associado à algum perfil
        //$sql = "delete from seguranca.perfilmenu where mnuid=".$_POST['exclui'];
        //$saida = $db->executar($sql);
        $saida = $db->executar($sql);
        unset($_POST['exclui']);
        $db->commit();
        $db->sucesso($modulo);    
    }
}
/**
 * Classe de criação de listagens.
 * @see Simec_Listagem
 */
require_once APPRAIZ . 'includes/library/simec/Listagem.php';
/**
 * Funções de apoio ao gerenciamento do perfil.
 * @see _funcoesspo_componentes.php
 */
require_once APPRAIZ . "includes/funcoesspo_componentes.php";
include_once APPRAIZ."includes/cabecalho.inc";
?>
<!--removendo '<div id="print_' . $var . '" class="notscreen" style="text-align: left;">'  adicionado pelo componente campo_textearea, pois repete string salva no campo HTML-->  
<style>
    .notscreen {display: none}
</style>

<div class="row col-md-12">
    <ol class="breadcrumb">
        <li><a href="<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li class="active">Incluir Menu</li>
    </ol>
    <?php if ($_REQUEST['acao']=='A' && !$_REQUEST['mnuid_int']){?>
        <script>
             $('ol').remove();
        </script>
        <ol class="breadcrumb">
            <li><a href="<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
            <li class="active">Administrar Menus</li>
        </ol>
    <?php }?>
    <?php $parametros = array('','','');
          $db->cria_aba($abacod_tela,$url,$parametros);
    
          if ($_REQUEST['acao']=='I' ||  ($_REQUEST['acao']=='A' && $_REQUEST['mnuid_int']<>'')){
          //título da página
          if ($_REQUEST['acao']=='I') $titulo_modulo = 'Incluir Menu'; else $titulo_modulo = 'Alterar Menu'; 
     
          if($_REQUEST['mnuid_int'] && $_REQUEST["act"]=='') { 
            $sql= "select * from seguranca.menu where mnuid=".$_REQUEST['mnuid_int'];    
            $saida = $db->recuperar($sql,$res);
            if(is_array($saida)) {foreach($saida as $k=>$v) ${$k}=$v;}
          } else { 
            //recupera todas as variaveis que veio pelo post
            foreach($_REQUEST as $k=>$v) ${$k}=$v;
          } 
    ?>
    
    <div class="well col-md-12">
        <form method="POST"  name="formulario" class="form-horizontal" novalidate="novalidate" role="form">
            <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                <input type='hidden' name="modulo" value="<?=$modulo?>">
                <input type='hidden' name="mnuid_int" value=<?=$_REQUEST['mnuid_int']?>>
                <input type='hidden' name='exclui' value=0>
                <input type='hidden' name='act' value=0>
                <input type='hidden' name='acao' value=<?=$_REQUEST['acao']?>>
                <tr>
                    <div class="form-group control-group">
                        <label for="mnucod" class="col-lg-2 control-label pad-12">Código:</label>
                        <div class="col-lg-6">
                              <?php if (! $_REQUEST['mnuid']) $habil='S' ;else $habil= 'N';?>
                              <?php //=campo_texto('mnucod','S',$habil,'',6,5,'#####','');?>
                              <?=inputTexto('mnucod', $valor, 'mnucod', 6, false, array('habil' => $habil,'masc'=>'#####')) ?>
                        </div>
                    </div>
                </tr>
                <tr>
                    <div class="form-group control-group">
                        <label for="mnudsc" class="col-lg-2 control-label pad-12">Descrição:</label>
                        <div class="col-lg-6">
                            <?php //=campo_texto('mnudsc','S','S','',50,50,'','');?>
                            <?=inputTexto('mnudsc', $valor, 'mnudsc', 50, false, array( 'habil' => 'S')) ?>
                        </div>
                    </div>
                </tr>
                <tr>
                    <div class="form-group control-group">
                        <label for="mnutransacao" class="col-lg-2 control-label pad-12">Transação:</label>
                        <div class="col-lg-6">
                            <?php //=campo_texto('mnutransacao','S','S','',50,50,'','');?>
                            <?=inputTexto('mnutransacao', $valor, 'mnutransacao', 50, false, array( 'habil' => 'S')) ?>
                        </div>
                    </div>
                </tr>
                <tr>
                    <div class="form-group control-group">
                        <label for="mnutipo" class="col-lg-2 control-label pad-12">Tipo:</label>
                        <div class="col-lg-6">
                            <?php $arrMnutipo = array(array( 'codigo' => 1, 'descricao' => 1 ),
                                                      array( 'codigo' => 2, 'descricao' => 2 ),
                                                      array( 'codigo' => 3, 'descricao' => 3 ),
                                                      array( 'codigo' => 4, 'descricao' => 4 ));
                            
                                  inputCombo('mnutipo', $arrMnutipo, $valor, 'mnutipo',array(  'acao'=>"onchange = document.formulario.submit()"));?>
                            <?php//=obrigatorio();?>
                        </div>
                  </div>
                </tr>
                <?php if ($mnutipo <> "" && $mnutipo <> "1") {?>
                        <tr>
                            <div class="form-group control-group">
                                <label for="mnuidpai" class="col-lg-2 control-label pad-12">Menu Pai:</label>
                                <div class="col-lg-6">
                                    <?php
                                         $sql="SELECT
                                                 mnuid as codigo, 
                                                 mnudsc as descricao 
                                               FROM 
                                                 seguranca.menu 
                                               WHERE mnutipo = " . ($mnutipo-1) . " 
                                               AND mnusnsubmenu='t' 
                                               AND sisid=".$_SESSION['sisid']."
                                               ORDER BY mnudsc";
                                         //$db->monta_combo("mnuidpai",$sql,'S',"Selecione o Menu",'','');
                                         inputCombo('mnuidpai', $sql, $valor, 'mnuidpai');
                                         
                                    ?>
                                    <?php//=obrigatorio();?>
                                </div>
                            </div>
                        </tr>
                        <?php }?>
                        <tr>
                            <div class="form-group control-group">
                                <label for="mnusnsubmenu" class="col-lg-2 control-label pad-12">Possui Sub-menu?</label>
                                <div class="col-lg-6">
                                    <div class="btn-group" data-toggle="buttons">
                                        <label class="btn btn-default <?php if ($mnusnsubmenu == 't') { echo 'active';} ?>" > 
                                            <input type="radio" name="mnusnsubmenu" id="mnusnsubmenu" value="t" onchange="submenu('S');" <?=($mnusnsubmenu=='t'?"CHECKED":"")?> />Sim
                                        </label> 
                                        <label class="btn btn-default <?php if ($mnusnsubmenu == 'f') { echo 'active';} ?>" > 
                                            <input type="radio" name="mnusnsubmenu" value="f" onchange="submenu('N');" <?=($mnusnsubmenu=='f'?"CHECKED":"")?>/> Não
                                        </label> 
                                    </div>
                                </div>
                            </div>
                        </tr> 
                        <tr>
                            <div class="form-group control-group">
                                <label for="mnulink" class="col-lg-2 control-label pad-12">Link:</label>
                                <div class="col-lg-6">
                                    <?php //=campo_texto('mnulink','N','S','',50,100,'','');?>
                                    <?=inputTexto('mnulink', $valor, 'mnulink', 100, false, array( 'habil' => 'S')) ?>
                                </div>
                            </div>
                        </tr>
                        <tr>
                            <div class="form-group control-group">
                                <label for="abacod" class="col-lg-2 control-label pad-12">Aba:</label>
                                <div class="col-lg-6">
                                    <?php $sql="SELECT abacod as codigo, abadsc as descricao FROM seguranca.aba where sisid=".$_SESSION['sisid'];
                                          inputCombo('abacod', $sql, $valor, 'abacod',array('titulo' => 'Selecione uma aba'));
                                    ?>
                                </div>
                            </div>
                        </tr>
                        <tr>
                            <div class="form-group control-group">
                                <label for="mnushow" class="col-lg-2 control-label pad-12">Faz parte da árvore?</label>
                                <div class="col-lg-6">
                                    <div class="btn-group" data-toggle="buttons">
                                        <label class="btn btn-default <?php if ($mnushow == 't') { echo 'active';} ?>" > 
                                            <input type="radio" name="mnushow" value="t" <?=($mnushow=='t'?"CHECKED":"")?> />Sim
                                            <!--$('[name=mnusnsubmenu]:checked').parent().attr('class', 'btn btn-default active' );-->
                                        </label> 
                                        <label class="btn btn-default <?php if ($mnushow == 'f') { echo 'active';} ?>" > 
                                            <input type="radio" name="mnushow" value="f" <?=($mnushow=='f'?"CHECKED":"")?>/> Não
                                        </label> 
                                    </div>
                                </div>
                            </div>
                        </tr>
                        <tr>
                            <div class="form-group control-group">
                                <label for="mnustile" class="col-lg-2 control-label pad-12">Estilo:</label>
                                <div class="col-lg-6">
                                    <?php //=campo_texto('mnustile','N','S','',50,150,'','');?>
                                    <?=inputTexto('mnustile', $valor, 'mnustile', 50, false, array( 'habil' => 'S')) ?>
                                </div>
                            </div>
                        </tr>
                        <tr>
                            <div class="form-group control-group">
                                <label for="mnuhtml" class="col-lg-2 control-label pad-12">Html:</label>
                                <div class="col-lg-6">
                                    <?php //=campo_textarea('mnuhtml','N','S','',60,5,500);?>
                                    <?=inputTextArea('mnuhtml', $valor, 'mnuhtml', 500, array( 'cols'=>102,'rols'=>20))?>
                                    
                                </div>
                            </div>
                        </tr>

                        <?php if   ($_REQUEST["mnuid_int"]) { ?>
                               <div class="form-group">
                                    <div class="col-lg-10 col-lg-offset-2">
                                        <button type="button" name="btalterar" id="btalterar" onclick="validar_cadastro('A');" class="btn btn-info">Atualizar</button>
                                        <button type="button" name="btcancelar" id="btcancelar" onclick="history.back();" class="btn btn-warning">Voltar</button>
                                    </div>
                               </div>
                        <?php } else { ?>
                                <div class="form-group">
                                    <div class="col-lg-10 col-lg-offset-2">
                                        <button type="button" name="btinserir" id="btinserir" onclick="validar_cadastro('I');" class="btn btn-success">Incluir</button>
                                        <button type="button" name="btcancelar" id="btcancelar" onclick="history.back();" class="btn btn-warning" >Voltar</button>
                                    </div>
                                </div>
                        <?php  } ?>
            </table>
        </form>
    </div>
</div>
<!-- else da condição ($mnutipo <> "" && $mnutipo <> "1")-->
<?php } elseif ($_REQUEST['acao']=='A' && !$_REQUEST['mnuid_int']){?> 
    <form method="POST"  name="formulario">
        <input type='hidden' name='mnuidpai'>
        <input type='hidden' name='mnutipo'>
        <input type='hidden' name='exclui'>
    </form>

<div class="row col-md-12">
<?php
//teste utilizando a função Monta Lista
$cabecalho = array('Código','Menu / Módulo','Visível','Transação');
//$sql = "select acacod, acadsc from acao";
$sql= "SELECT
            mnuid,
            mnusnsubmenu,
            mnutipo,
            /*CASE WHEN mnusnsubmenu=true THEN '<td style=\"text-align:left\"><img border=\"0\" src=\"../imagens/alterar.gif\" title=\"Alterar Menu\" onclick=\"altera_menu('||mnuid||')\"></td>&nbsp;&nbsp;&nbsp;
                                              <img border=\"0\" src=\"../imagens/excluir.gif\" title=\"Excluir Menu\" onclick=\"excluir_menu('||mnuid||')\">&nbsp;&nbsp;&nbsp;
                                              <img border=\"0\" src=\"../imagens/gif_inclui.gif\" title=\"Incluir Menu em » '||mnudsc||'\" onclick=\"incluir_menu('||mnuid||','||mnutipo||')\">' 
                ELSE '<img border=\"0\" src=\"../imagens/alterar.gif\" title=\"Alterar Menu\" onclick=\"altera_menu('||mnuid||')\">&nbsp;&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\"Excluir Menu\" onclick=\"excluir_menu('||mnuid||')\">' 
            END as acao, */
            mnucod, 
            CASE 
                WHEN mnutipo=2 THEN '&nbsp;&nbsp;<img src=\"../imagens/seta_filho.gif\" align=\"absmiddle\">'||mnudsc 
                WHEN mnutipo=3 THEN '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src=\"../imagens/seta_filho.gif\" align=\"absmiddle\">'||mnudsc 
                WHEN mnutipo=4 THEN '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src=\"../imagens/seta_filho.gif\" align=\"absmiddle\">'||mnudsc 
                ELSE mnudsc 
            END as mnudsc, 
            CASE 
                WHEN mnushow=false THEN '<font color=#808080>Não</font>' 
                ELSE '<font color=#008000>Sim</font>' 
            END as mnushow, 
            mnutransacao 
       FROM 
            seguranca.menu 
       WHERE 
            mnustatus = 'A' 
       AND 
            sisid=".$_SESSION['sisid']."
       ORDER BY 
            mnucod";

//$db->monta_lista_simples($sql,$cabecalho,200,20,'','','');
    $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
    $listagem->setAcoes(array('edit' => 'altera_menu', 'delete' => 'excluir_menu', 'file' => array('func' => 'incluir_menu','extra-params' => array('mnutipo'))))
             ->setAcaoComoCondicional( 'file',array(array('campo' => 'mnusnsubmenu', 'operacao' => 'igual', 'valor' => 't') ) )
             ->esconderColuna( array('mnusnsubmenu','mnutipo'))
             ->setCabecalho ($cabecalho)							
             ->setQuery ( $sql );

    $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
}?>
</div>
<script type="text/javascript" language="javascript">
$( document ).ready(function() {
    $('#tb_render > tbody > tr ').each(function() {
       var linha = $(this);
      linha.find('td:eq(4)').removeClass('text-center').addClass('text-left');
    });
});    
<?php if ($_REQUEST["mnuid_int"]){?>
    
            if ($('input[name="mnusnsubmenu"]:checked').val() === 't'){
                $('#mnulink').attr('disabled', 'disabled');
                $('#abacod').attr('disabled', 'disabled');

                for (i=0;i<6;i++)
                {
                        $('#sub'+i).css("visibility", "hidden")
                        $('#sub'+i).hide();
                }
            }
    
<?php  }?>


    function submenu(op){ 
        if (op == 'S')
        {
            $('#mnulink').attr('disabled', 'disabled');
            $('#abacod').attr('disabled', 'disabled');
            $('#mnulink').hide();
            $("label[for='mnulink']").hide();
            $('#abacod').hide();
            $('#abacod_chosen').hide();
            $("label[for='abacod']").hide();
            $('label input[name=mnushow]').parent().hide();
            $("label[for='mnushow']").hide();
            
             document.formulario.mnushow[0].checked = true;
	     document.formulario.mnushow[1].checked = false;
            
            if($('input[name="mnushow"]:checked').val() === 't'){ $('input[name="mnushow"]:checked').attr('checked','checked'); }
            if($('input[name="mnushow"]:checked').val() === 'f'){ $('input[name="mnushow"]:checked').attr('checked', false); }
            
            for (i=0;i<6;i++)
            {
                $('#sub'+i).css("visibility", "hidden")
                $('#sub'+i).hide();
            }
        }
        else
        {
            $('#mnulink').removeAttr("disabled");
            $('#abacod').removeAttr("disabled");
            $('#mnulink').show();
            $("label[for='mnulink']").show();
            $('#abacod').show();
            $("label[for='abacod']").show();
            $('label input[name=mnushow]').parent().show();
            $("label[for='mnushow']").show();
            
            for (i=0;i<6;i++)
            {
                $('#sub'+i).css("visibility", "hidden")
                $('#sub'+i).hide();
            }
        }
    }
    function altera_menu(cod) {
        location.href = '<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/menu/menu&acao=A&mnuid_int='+cod;
    }
  
    function incluir_menu(codpai, tipo) {
        var tipoSon = Number(tipo)+1;
        location.href = '<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/menu/menu&acao=I&mnuidpai='+codpai+'&mnutipo='+tipoSon;
    }
  
    function excluir_menu(cod) { 
        if( window.confirm( "Confirma a exclusão do ítem "+ cod + " no Menu?") )
        {
          document.formulario.exclui.value = cod;
          document.formulario.submit();
      } else document.formulario.exclui.value = 0;
    }
  
    function validar_cadastro(cod) {    
        if (!validaBranco(document.formulario.mnucod, 'Código')) return;	
        if (!validaBranco(document.formulario.mnudsc, 'Descrição')) return;	
        if (!validaBranco(document.formulario.mnutransacao, 'Transação')) return;
        if (!validaBranco(document.formulario.mnutipo, 'Tipo')) return;			
        if (document.formulario.mnutipo.value != "1" )
        {		
        if (!validaBranco(document.formulario.mnuidpai, 'Menu Pai')) return;

        }		
        if (!validaRadio(document.formulario.mnusnsubmenu,'Possui sub-menu')) return;
        if (document.formulario.mnusnsubmenu[1].checked) {
                if (!validaBranco(document.formulario.mnulink, 'Link')) return;
                if (!validaRadio(document.formulario.mnushow,'Faz Parte da Árvore')) return;
        }else	{
                document.formulario.mnulink.value = '';
                document.formulario.abacod.value = '';
                }

        if (cod == 'I') document.formulario.act.value = 'inserir'; else document.formulario.act.value = 'alterar';
        document.formulario.submit();

     }   		    
	      

</script>
