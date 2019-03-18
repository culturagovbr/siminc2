<?php
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Cristiano Cabral (cristiano.cabral@gmail.com)
   Módulo:aba.inc
   Finalidade: Permitir o cadastro de abas que serão utilizadas pelos menus/módulos
   */
   //
$modulo=$_REQUEST['modulo'] ;
$act=$_REQUEST['act'];
$acao=$_REQUEST['acao'];

//inclui abas
if (($act == 'incluir' && $acao=='I') || ($act == 'alterar' && $acao=='A'))
{
    if ($acao == 'I') {
        $sql = "INSERT INTO aba (abadsc,sisid) VALUES ('".$_POST['abadsc']."'," . $_SESSION['sisid'] . ")";
        $db->executar($sql);
            $sql = "SELECT max(abacod) as abacodmax FROM aba";
            $saida = $db->recuperar($sql);
            if(is_array($saida)) foreach($saida as $k=>$v) ${$k}=$v;
    }else{
	$abacodmax = $_POST['abacod'];
	$sql = "update aba set abadsc='".$_POST['abadsc']."' WHERE abacod=".$abacodmax;
        $db->executar($sql);
    }
    
    //inclui aba_menu
    $sql = "SELECT * FROM menu WHERE sisid=".$_SESSION['sisid']." ORDER BY mnucod";
    $RS = $db->record_set($sql);
    $nlinhas = $db->conta_linhas($RS);
    if ($nlinhas >= 0)
    {
        for ($i=0; $i<=$nlinhas;$i++)
        {
            $res = $db->carrega_registro($RS,$i);
            // a linha abaixo transforma em variáveis todos os campos do array
            if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;

            if ($_POST[$mnuid]<>'')
            {
              // então marcou a opção
              // procurar se já foi marcada antes. se não foi, então inserir.
                $sql = 'SELECT * FROM aba_menu WHERE mnuid='.$mnuid.' AND abacod='.$abacodmax;
                $RS2 = $db->record_set($sql);
                $nlinhas2 = $db->conta_linhas($RS2);
                if ($nlinhas2 == -1) {
                   // incluir
                     $sql = 'INSERT INTO aba_menu (abacod, mnuid) VALUES ('.$abacodmax.','.$mnuid.')';
                      $db->executar($sql);
                }

            } else{	
                // caso não tenha marcado, procurar se existe no banco. Se existir, então apagar
                $sql = "SELECT * FROM aba_menu WHERE mnuid=$mnuid AND abacod=$abacodmax";
                $RS3 = $db->record_set($sql);
                $nlinhas3 = $db->conta_linhas($RS3);
                if ($nlinhas3 == 0)
                { // excluir
                    $sql = 'DELETE FROM aba_menu WHERE abacod='.$_POST['abacod'].'  AND mnuid='.$mnuid;
                    $saida = $db->executar($sql);
                }
            }
        }
    }

    $db ->commit();
    $db->sucesso('sistema/menu/aba');
}

if ($act == 'excluir' && $acao=='A')
{
	$sql = "UPDATE menu set abacod=null WHERE abacod=". $_POST['abacod'];
    $db->executar($sql);
	$sql = "DELETE FROM aba_menu WHERE abacod=". $_POST['abacod'];
    $db->executar($sql);
	$sql = "DELETE FROM aba WHERE abacod=". $_POST['abacod'];
    $db->executar($sql);
	$db ->commit();
    $db->sucesso('sistema/menu/aba');

}
/**
 * Funções de apoio ao gerenciamento do perfil.
 * @see _funcoesspo_componentes.php
 */
require_once APPRAIZ . "includes/funcoesspo_componentes.php";
/**
 * Classe de criação de listagens.
 * @see Simec_Listagem
 */
require_once APPRAIZ . 'includes/library/simec/Listagem.php';
include APPRAIZ."includes/cabecalho.inc";
?>
<div class="row col-md-12">
    <ol class="breadcrumb">
        <li><a href="<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li class="active">Administrar Abas</li>
    </ol>
    <?php if ( $acao=='I' ){ ?>
        <script>
            jQuery('ol').remove();
        </script>
        <ol class="breadcrumb">
            <li><a href="<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
            <li class="active">Incluir Aba </li>
        </ol>
    <?php } elseif($acao=='I' || ( $acao=='A' && !empty($_REQUEST['abacod_int']) ) ){?>
        <script>
            jQuery('ol').remove();
        </script>
        <ol class="breadcrumb">
            <li><a href="<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
            <li class="active">Alterar Aba </li>
        </ol>
    <?php }
    
    $db->cria_aba($abacod_tela,$url,'');
    
    if ($acao=='I' || ($acao=='A' && $_REQUEST['abacod_int']<>'')) 
    {
        if($_REQUEST['abacod_int']) 
        { 
            $sql= "SELECT * FROM aba WHERE abacod=".$_REQUEST['abacod_int'];
            $saida = $db->recuperar($sql,$res);
            if(is_array($saida)) {foreach($saida as $k=>$v) ${$k}=$v;}
        }?>
    <div class="well col-md-12">
        <form method="POST"  name="formulario" class="form-horizontal">
            <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
            <input type='hidden' name="act">
            <input type='hidden' name="abacod" value="<?=$abacod?>">
                <tr>
                  <div class="form-group control-group">
                      <label for="sisid" class="col-lg-2 control-label pad-12">Descrição:</label>
                      <div class="col-lg-6">
                          <?php//=campo_texto('abadsc','S','S','',50,50,'','');?>  
                          <?=inputTexto('abadsc', $valor, 'abadsc', 50, false, array('habil' => '$habil')) ?>
                      </div>
                  </div>
                </tr>
                <tr>
                    <div class="form-group control-group">
                        <label for="sisid" class="col-lg-2 control-label pad-12">Módulos/Menus:</label>
                        <div class="col-lg-6" style="border: 1px solid #e3e3e3;">
                            <div class="form-group-lg control-group">
                              <?php
                                //$cabecalho = array('Ações','Código','Aba','Qtde. de Módulos');
                                //$sql = "SELECT case when aba_menu.mnucod isnull then '<input type=\"checkbox\" name=\"'||menu.mnucod||'\" value=\"'||menu.mnucod||'\">' else '<input type=\"checkbox\" name=\"'||menu.mnucod||'\" value=\"'||menu.mnucod||'\" checked>' end as check, menu.mnucod||' - '||menu.mnudsc FROM menu LEFT JOIN aba_menu on menu.mnucod=aba_menu.mnucod and aba_menu.abacod =".$abacod;
                                if ($acao=='I') $sql = "SELECT menu.mnuid,menu.mnucod, menu.mnudsc, menu.mnulink, menu.mnutransacao, menu.mnutipo, '' as check FROM menu WHERE sisid=".$_SESSION['sisid']." ORDER BY menu.mnucod";
                                else $sql = "SELECT menu.mnuid, menu.mnucod, menu.mnudsc, menu.mnulink, menu.mnutransacao, menu.mnutipo, case when aba_menu.mnuid isnull then '' else 'checked' end as check FROM menu LEFT JOIN aba_menu on menu.mnuid=aba_menu.mnuid AND aba_menu.abacod =$abacod WHERE menu.sisid=".$_SESSION['sisid']." ORDER BY menu.mnucod";
                                $RS = $db->carregar($sql);
                                $nlinhas = count($RS)-1;
                                if ($nlinhas >= 0)
                                {
                                    for ($i=0; $i<=$nlinhas;$i++)
                                    {	
                                              foreach($RS[$i] as $k=>$v) ${$k}=$v;
                                              if ($mnutipo==2) $space = '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'; elseif ($mnutipo==3) $space = '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'; else $space='';
                                              if ($mnulink<>'') $block = ''; else $block = 'disabled';
                                              print $space.'<input type="checkbox" name="'.$mnuid.'" value="'.$mnuid.'"'.$check.' '.$block.'>'.$mnucod.' - '.$mnutransacao.'<br>';
                                    }
                                }
                              ?>
                            </div>
                        </div>
                    </div>
                </tr>
                <div class="form-group">
                    <div class="col-lg-10 col-lg-offset-2">
                       <?if ($acao == 'A'){?>
                       <!--<input type="button" name="btalterar" value="Alterar" onclick="return altera()" class="botao">-->
                       <!--&nbsp;&nbsp;&nbsp;&nbsp;<input type="button" name="btexcluir" value="Excluir" onclick="return exclui()" class="botao">-->
                       <button type="button" name="btalterar" id="btalterar" onclick="return altera();" class="btn btn-info">Atualizar</button>
                       <button type="button" name="btexcluir" id="btexcluir" onclick="return exclui();" class="btn btn-danger">Excluir</button>

                       <?}else{?>
                       <button type="button" name="btinserir" id="btinserir" onclick="return inclui()" class="btn btn-success">Incluir</button>
                       <!--<input type="button" name="btinserir" value="Incluir" onclick="return inclui()" class="botao">-->
                       <?}?>
                       <button type="button" name="btcancelar" id="btcancelar" onclick="history.back();" class="btn btn-warning" >Voltar</button>
                       <!--&nbsp;&nbsp;&nbsp;&nbsp;<input type="button" name="btcancelar" value="Voltar" onclick="history.back();" class="botao"></td>-->
                    </div>
                </div>
            </table>
        </form>
    </div>
</div>
    <script language="JavaScript" type="text/javascript">

      function inclui() {
            if (!validaBranco(document.formulario.abadsc, 'Descrição')) return;
        document.formulario.act.value = 'incluir';
            document.formulario.submit();
      }

      function altera() {
            if (!validaBranco(document.formulario.abadsc, 'Descrição')) return;
        document.formulario.act.value = 'alterar';
            document.formulario.submit();
      }


      function exclui() { 
        if( window.confirm( "Confirma a exclusão deste Ítem?") )
        {
            document.formulario.act.value = 'excluir';
            document.formulario.submit();
        } else {};
      }
    </script>

<?php } elseif ($acao=='A') {?>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <form method="POST"  name="formulario">
        <input type='hidden' name="act">
        <input type='hidden' name="abacod">
        <?php
            $cabecalho = array('Código','Aba','Qtde. de Módulos');
            $sql= "SELECT 
                     abacod, 
                     abacod as codigo,
                     abadsc, 
                    (SELECT count(*) 
                     FROM aba_menu 
                     WHERE aba_menu.abacod=aba.abacod 
                     AND aba.sisid=".$_SESSION['sisid']." ) as abaqtd
                  FROM aba 
                  WHERE aba.sisid=".$_SESSION['sisid']."
                  ORDER BY abadsc";
            
            $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
            $listagem->setAcoes(array('edit' => 'altera_aba', 'delete' => 'excluir_aba'))
                     ->setCabecalho ($cabecalho)							
                     ->setQuery ( $sql )
                     ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
            //$_SESSION['sisdiretorio'] = "monitora/monitora";
        ?>
    </form>
</table>
<script language="JavaScript" type="text/javascript">
  function altera_aba(cod) {
     location.href = '?modulo=sistema/menu/aba&acao=A&abacod_int='+cod;
  }
  

  function excluir_aba(cod) { 
    if( window.confirm( "Confirma a exclusão do ítem "+ cod) )
    {
	document.formulario.abacod.value = cod;
	document.formulario.act.value = 'excluir';
	document.formulario.submit();
    } else {};
  }
</script>
<?php
}
?>

