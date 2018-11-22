<?php
unset($_SESSION['painel']['metid']);
include (APPRAIZ.'www/painel/_funcoes_metas.php');

if($_POST['requisicaoAjax']){
	header('content-type: text/html; charset=ISO-8859-1');
	$_POST['requisicaoAjax']();
	exit();
}

if($_POST['requisicao']){
	$_POST['requisicao']();
}
$mIndicador = new Painel_Model_Indicador();
$listaPerfis = $mIndicador->RetornaPerfil();
// monta cabeçalho 
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$abacod_tela = 57279;
$db->cria_aba($abacod_tela,$url,'');
$titulo = "Metas do Indicador";
monta_titulo( $titulo, '&nbsp' );

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../painel/js/metas_indicador.js"></script>
<style>
	.SubtituloTabela{background-color:#cccccc}
	.negrito{font-weight:bold}
	.bold{font-weight:bold}
	.normal{font-weight:normal}
	.center{text-align: center;}
	.direita{text-align: right;}
	.esquerda{text-align: left;}
	.link{cursor: pointer;}
</style>

<form name="formulario_metas" id="formulario_metas"  method="post" action="" >
    <input type="hidden" name="requisicao" value="pesquisarMetas" />
    <input type="hidden" name="metid" value="" />
</form>
<?php
$arrWhere[] = "metstatus = 'A'";
$arrWhere[] = "indstatus = 'A'";

if($_POST['secid']){
	$arrWhere[] = "ind.secid = {$_POST['secid']}";
}
if($_POST['acaid']){
	$arrWhere[] = "ind.acaid = {$_POST['acaid']}";
}
if($_SESSION['indid']){
	$arrWhere[] = "ind.indid = {$_SESSION['indid']}";
}
if($_POST['metdesc']){
	$arrWhere[] = "(removeacento(upper(met.metdesc)) like removeacento(('%".strtoupper($_POST['metdesc'])."%')) or removeacento(upper(ind.indnome)) like removeacento(('%".strtoupper($_POST['metdesc'])."%')) or ind.indid::text like removeacento(('%".strtoupper($_POST['metdesc'])."%')))";
}
$sql = "select ";
                if (in_array(PAINEL_PERFIL_GESTOR_INDICADOR, $listaPerfis)){
                    $sql .= "'<img src=\"../imagens/alterar.gif\" class=\"link\" onclick=\"editarMeta(\'' || met.metid || '\')\" /> <img src=\"../imagens/excluir_01.gif\" class=\"link\" onclick=\"excluirMeta(\'' || met.metid || '\')\" />' as acao,    ";
                }else{
                    $sql .= "'<img src=\"../imagens/alterar.gif\" class=\"link\" onclick=\"editarMeta(\'' || met.metid || '\')\" /> <img src=\"../imagens/excluir.gif\" class=\"link\" onclick=\"excluirMeta(\'' || met.metid || '\')\" />' as acao,    ";
                }
		$sql .= "met.metdesc,
			ind.indid || ' - ' || ind.indnome as indnome,
			(select secdsc from painel.secretaria sec where sec.secid = ind.secid) as secdsc,
			(select acadsc from painel.acao aca where aca.acaid = ind.acaid) as acadsc,
			per.perdsc,
			met.metid,
			ind.indid
		from
			painel.metaindicador met
		inner join
			painel.indicador ind ON ind.indid = met.indid
		inner join
			painel.periodicidade per ON per.perid = met.perid
";
        if (in_array(PAINEL_PERFIL_GESTOR_INDICADOR, $listaPerfis)){
            $sql .= " inner join painel.usuarioresponsabilidade ur on ind.secid = ur.secid and ur.usucpf = '".$_SESSION['usucpf']."'";
        }
	$sql .=	($arrWhere ? " where ".implode(" and ",$arrWhere) : "")."
		order by (select s1.secordem from painel.secretaria s1 where s1.secid = ind.secid and s1.secstatus = 'A')";

$arrDados = $db->carregar($sql);
if($arrDados){
	foreach($arrDados as $dado){
		
		unset($arrEstid);
		unset($arrEstdesc);
		unset($arrAcadsc);
		
		$sqlEstrategia = "	select 
								estid,
								estdesc
							from
								painel.estrategiameta
							where
								metid = {$dado['metid']}
							and
								eststatus = 'A'";
		$arrEstrategias = $db->carregar($sqlEstrategia);
		if($arrEstrategias){
			foreach($arrEstrategias as $estrategia){
				$arrEstid[]   = $estrategia['estid'];
				$arrEstdesc[] = $estrategia['estdesc'];
			}
		}
		if($arrEstid){
			$sqlAcao = "	select 
								acadsc
							from
								painel.acao aca
							inner join
								painel.acaoestrategia ace ON ace.acaid = aca.acaid
							where
								estid in (".implode(",",$arrEstid).")
							and
								acastatus = 'A'";
			$arrAcao = $db->carregar($sqlAcao);
			if($arrAcao){
				foreach($arrAcao as $acao){
					$arrAcadsc[] = $acao['acadsc'];
				}
			}
		}
		
		$dados_array[] = array(
            "acao" 		=> 	$dado['acao'],
            "metdesc" 	=> 	$dado['metdesc'],
            "indnome" 	=> 	$dado['indnome'],
            "secdsc" 	=> 	$dado['secdsc'],
            "acadsc" 	=> 	$dado['acadsc'],
            "perdsc" 	=> 	$dado['perdsc']
       );
	}
}
$cabecalho = array("Ações", "Meta", "Indicador", "Secretaria","Ação","Periodicidade da Meta");
$db->monta_lista_array($dados_array,$cabecalho,100,5,'N','center',$par2);
?>