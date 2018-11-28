<?php
$mIndicador = new Painel_Model_Indicador();
$listaPerfis = $mIndicador->RetornaPerfil();
if ($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']();
}

/**
 * Lista de perfis por usuário
 */
$mIndicador = new Painel_Model_Indicador();
$listaPerfis = $mIndicador->RetornaPerfil();

//Ajax
if ($_REQUEST['filtraAcaoAjax'] && $_REQUEST['secid']) {
	header('content-type: text/html; charset=ISO-8859-1');
	filtraAcao($_REQUEST['secid']);
	exit;
}

// verificando a validade do indicador
if(is_numeric($_REQUEST['indid'])) {
	$indid = $db->pegaUm("SELECT indid FROM painel.indicador WHERE indid='".$_REQUEST['indid']."'");
	// testando se existe
	if($indid) {
		$_SESSION['indid'] = $indid;
	} else {
		echo "<script>alert('Indicador informado não existe');window.location='?modulo=inicio&acao=C';</script>";
		exit;
	}
}
    
// monta cabeçalho 
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

// verificando se é um indicador novo
if($_REQUEST['indid'] == 'novoIndicador') {
	// carregando o menu
	$menu[0] = array("descricao" => "Lista de Indicadores", "link"=> "/painel/painel.php?modulo=principal/lista&acao=A&1=1");
	$menu[1] = array("descricao" => "Cadastro de Indicadores", "link"=> "/painel/painel.php?modulo=principal/cadastro&acao=A&indid=".$_REQUEST['indid']."&evento=".$_REQUEST['evento']."");
	// se for super usuário não precisa mostrar quais os indicadores, pois tem acesso a todos
	if(!$db->testa_superuser()) {
		$menu[2] = array("descricao" => "Meus indicadores", "link"=> "/painel/painel.php?modulo=principal/lista&acao=A&evento=M");
	}
	echo montarAbasArray($menu, "/painel/painel.php?modulo=principal/cadastro&acao=A&indid=".$_REQUEST['indid']."&evento=".$_REQUEST['evento']."");
	$acesso = true;
	unset($_SESSION['indid']);
	$indhomologado = 't';
}

if($_SESSION['indid']):

	$db->cria_aba($abacod_tela,$url,'');
	!$_SESSION['indid']? $_SESSION['indid'] = $_REQUEST['indid']: $_SESSION['indid'] = $_SESSION['indid'];

        $permissoes = verificaPerfilPainel();
        if($permissoes['verindicadores'] == 'vertodos') {
                $acesso = true;
        } else {
                $acesso = validaAcessoIndicadores($permissoes['verindicadores'], $_SESSION['indid']);
        }
	
endif;

$arrPerfil = pegaPerfil();

$titulo = "Cadastro de Indicador";
monta_titulo( $titulo, '<img border="0" src="../imagens/obrig.gif"/>Indica Campo Obrigatório.' );

if( $_SESSION['indid'] || ( $_REQUEST['indid'] && $_REQUEST['indid'] != "novoIndicador") )
	cabecalhoIndicador($_SESSION['indid']);
 
if($_SESSION['indid'] && !$_REQUEST['evento']){
	$_REQUEST['evento'] = 'A';
}

if( (isset($_REQUEST[evento]) && ($_REQUEST[evento] != '')) || $_SESSION['indid']){
	
	if($_SESSION['indid']){
		$_REQUEST['indid'] = $_SESSION['indid'];
	}
	switch($_REQUEST[evento]) {
		
		case 'I':	
						
				$indid = $_REQUEST['indid'];
				$exoid 			= $_REQUEST['exoid'];
				$unmid 			= $_REQUEST['unmid']; 
				$cliid 			= $_REQUEST['cliid'];
				$tpiid 			= $_REQUEST['tpiid'];  
				$secid  		= $_REQUEST['secid'];  
				$secidgestora		= $_REQUEST['secidgestora'];
				$acaid   		= $_REQUEST['acaid'];
                $produto		= $_REQUEST['indprod'];
				$perid  		= $_REQUEST['perid'];  
				$peridatual		= $_REQUEST['peridatual'];
				$estid  		= $_REQUEST['estid'];
				$regid			= $_REQUEST['regid'];
				$umeid			= $_REQUEST['umeid'];
				$colid  		= $_REQUEST['colid'];  
				$usucpf  		= $_REQUEST['usucpf'];  
				$indnome  		= $_REQUEST['indnome'];  
				$indobjetivo  		= $_REQUEST['indobjetivo']; 
				$indformula  		= $_REQUEST['indformula'];  
				$indtermos  		= $_REQUEST['indtermos'];  
				$indfontetermo  	= $_REQUEST['indfontetermo'];  
				$indnormalinicio  	= str_replace(array(".",","), array("","."), $_REQUEST['indnormalinicio']);  
				$indnormalfinal  	= str_replace(array(".",","), array("","."), $_REQUEST['indnormalfinal']);  
				$indatencaoinicio  	= str_replace(array(".",","), array("","."), $_REQUEST['indatencaoinicio']); 
				$indatencaofinal  	= str_replace(array(".",","), array("","."), $_REQUEST['indatencaofinal']);  
				$indcriticoinicio  	= str_replace(array(".",","), array("","."), $_REQUEST['indcriticoinicio']);  
				$indcriticofinal  	= str_replace(array(".",","), array("","."), $_REQUEST['indcriticofinal']);  
				$indmetavalor 		= str_replace(array(".",","), array("","."), $_REQUEST['indmetavalor']);
				$indmetadatalimite	= $_REQUEST['indmetadatalimite'];
				if($unmid == UNIDADEMEDICAO_MOEDA) {
					$indqtdevalor	= "false";
				} else {
					$indqtdevalor	= $_REQUEST['indqtdevalor'];
				}
				$indcumulativo		= (($_REQUEST['indcumulativo'])?$_REQUEST['indcumulativo']:"N");
				$indcumulativovalor = $_REQUEST['indcumulativovalor'];
				$indpublicado		= (($_REQUEST['indpublicado'])?$_REQUEST['indpublicado']:"false");
				$indpublico			= (($_REQUEST['indpublico'])?$_REQUEST['indpublico']:"false");
				$indencerrado		= (($_REQUEST['indencerrado'])?$_REQUEST['indencerrado']:"false");
				$indhomologado		= (($_REQUEST['indhomologado'])?$_REQUEST['indhomologado']:"false");
				$indobservacao		= $_REQUEST['indobservacao'];
				$indescala			= $_REQUEST['indescala'];
				$mtiid 				= $_REQUEST['mtiid'];
				$indshformula		= $_REQUEST['indshformula'];
				$formulash			= $_REQUEST['formulash'];
				if($indpublicado=='true'){
					$indhomologado = true;
				}
				
			if($indid){						
				
				$sql = "
				SELECT 
					indid				
				FROM  painel.indicador i
				WHERE 
				i.indid = ".$indid."
				";					
				$dados = $db->pegaUm($sql);
			}
			
			//Campos Não obrigatorios
				(!$cliid)? $cliid = 'null' : $cliid = $cliid;
				($umeid == "")? $umeid = 'null' : $umeid = $umeid;
				($regid == "")? $regid = 'null' : $regid = $regid;
				($tpiid == "")? $tpiid = '1' : $tpiid = $tpiid;
				($indnormalinicio == "")?	$indnormalinicio 	= "null" : $indnormalinicio 	= $indnormalinicio;
				($indnormalfinal == "")? 	$indnormalfinal 	= "null" : $indnormalfinal 		= $indnormalfinal;
				($indatencaoinicio == "")? 	$indatencaoinicio 	= "null" : $indatencaoinicio 	= $indatencaoinicio;
				($indatencaofinal == "")? 	$indatencaofinal 	= "null" : $indatencaofinal 	= $indatencaofinal;
				($indcriticoinicio == "")? 	$indcriticoinicio 	= "null" : $indcriticoinicio 	= $indcriticoinicio;
				($indcriticofinal == "")? 	$indcriticofinal 	= "null" : $indcriticofinal 	= $indcriticofinal;
				($indmetavalor == "")? 		$indmetavalor 		= "null" : $indmetavalor	 	= $indmetavalor;
				($secidgestora == "")? 		$secidgestora 		= "null" : $secidgestora	 	= $secidgestora;
				$mtiid = $mtiid ? $mtiid : "NULL";
				
				$usucpf = $_SESSION['usucpf'];
			
			// caso não exista o indicador insere um novo
			if(!$dados){
				if($indhomologado=='f'){
					$indhomologado='false';
				}
				$sql_I = "INSERT 
						  INTO painel.indicador
							  (unmid, cliid, tpiid, secid, secidgestora,acaid, perid, estid, colid, usucpf, indnome, indobjetivo,
							  indformula, indtermos, indfontetermo, indnormalinicio, indnormalfinal, indatencaoinicio,
							  indatencaofinal, indcriticoinicio, indcriticofinal, indmetavalor, indmetadatalimite,exoid,regid,umeid,
                                                          indqtdevalor,indcumulativo,indpublicado,indescala,indobservacao,peridatual,indcumulativovalor,indpublico,
                                                          indhomologado,mtiid,indshformula,formulash,indencerrado, indprod)
						  VALUES 
						  	   (".$unmid.", ".$cliid.", ".$tpiid.", ".$secid.", ".$secidgestora.", ".$acaid.", ".$perid.", ".$estid.", ".$colid.", '".$usucpf."', '".$indnome."', '".$indobjetivo."',
							  '".$indformula."', '".$indtermos."', '".$indfontetermo."', ".$indnormalinicio.", ".$indnormalfinal.", ".$indatencaoinicio.",
							  ".$indatencaofinal.", ".$indcriticoinicio.", ".$indcriticofinal.", ".$indmetavalor.", '".$indmetadatalimite."','".$exoid."',$regid,$umeid,$indqtdevalor,'$indcumulativo',"
                                        . "               $indpublicado,$indescala,'$indobservacao','$peridatual','$indcumulativovalor',$indpublico,$indhomologado,$mtiid,$indshformula,'$formulash',$indencerrado, '".$produto."')
						  RETURNING	indid
						  ";

				$indid = $db->pegaUm($sql_I);
				
				//OBJETIVO
				$_REQUEST['obeid'] = array();
				foreach($_REQUEST['obeid'] as $obeid) {
					$sql = "INSERT INTO painel.indicadorobjetivo(obeid, indid) VALUES (". $obeid .", ". $indid .")";			
					$db->executar($sql);
				}
				
				//AGENDA DE GOVERNO
				foreach($_REQUEST['aggid'] as $aggid) {
					if($aggid){
						$sql = "INSERT INTO painel.agendaindicador(aggid, indid) VALUES (". $aggid .", ". $indid .")";			
						$db->executar($sql);
					}
				}
                            $db->commit();
                            echo "<script>
                                            alert('Dados salvos com sucesso.');
                                            window.location = '?modulo=principal/cadastro&acao=A&indid=$indid&evento=A';
                                      </script>";				
			//caso exista o indicador altera os dados do mesmo	
			} else {

                            if (!in_array(PAINEL_PERFIL_GESTOR_INDICADOR,$listaPerfis)){
                                $sql = "select perid, regid from painel.indicador where indid = ".$indid;
                                $registro = $db->pegalinha($sql);
                                $peridAtual = $registro['perid'];
                                $regidAtual = $registro['regid'];
                                $sql_U = "
                                        UPDATE painel.indicador
                                        SET
                                         unmid = 				".$unmid.",
                                         exoid = 				".$exoid.",
                                         cliid = 				".$cliid.",
                                         tpiid = 				".$tpiid.",
                                         estid = 				".$estid.",
                                         secid = 				".$secid.",
                                         secidgestora =			".$secidgestora.",
                                         acaid = 				".$acaid.",
                                         indprod =              '".$produto."',
                                         perid = 				".$perid.",
                                         peridatual =			'".$peridatual."',
                                         colid = 				".$colid.",
                                         usucpf = 				'".$usucpf."',
                                         indnome = 				'".$indnome."',
                                         indobjetivo = 			'".$indobjetivo."',
                                         indformula = 			'".$indformula."',
                                         indtermos = 			'".$indtermos."',
                                         indfontetermo = 		'".$indfontetermo."',
                                         indnormalinicio = 		".$indnormalinicio.",
                                         indnormalfinal = 		".$indnormalfinal.",
                                         indatencaoinicio = 	".$indatencaoinicio.",
                                         indatencaofinal = 		 ".$indatencaofinal.",	
                                         indcriticoinicio = 	".$indcriticoinicio.",
                                         indcriticofinal = 		".$indcriticofinal.",
                                         indmetavalor = 		".$indmetavalor.",
                                         regid	= 				".$regid.",
                                         umeid	= 				".$umeid.",
                                         indmetadatalimite = 	'".$indmetadatalimite."',
                                         indqtdevalor = 		'".$indqtdevalor."',
                                         indcumulativo = 		'".($indcumulativo==''?'N' : $indcumulativo)."',	
                                         indpublicado = 		".($indpublicado=='' ? 'indpublicado' : "'".$indpublicado."'").",
                                         indescala = 			'".$indescala."',
                                         indobservacao = 		'".$indobservacao."',
                                         indcumulativovalor	=	'$indcumulativovalor',
                                         indpublico = 			".($indpublico=='' ? 'indpublico' : "'".$indpublico."'").",
                                         indhomologado = 		".($indhomologado=='' ? 'indhomologado' : "'".$indhomologado."'").",
                                         indencerrado = 		".($indencerrado=='' ? 'indencerrado' : "'".$indencerrado."'").",
                                         mtiid = $mtiid,
                                         indshformula = $indshformula,
                                         formulash = '$formulash'
                                        WHERE indid = 			".$indid."	
                                ";

                                $db->executar($sql_U);

                                //OBJETIVO
                                $sql = "DELETE FROM painel.indicadorobjetivo WHERE indid =  ". $indid ."";			
                                $db->executar($sql);		
                                $_POST['obeid'] = array();
                                foreach($_POST['obeid'] as $obeid) {
                                        $sql = "INSERT INTO painel.indicadorobjetivo(obeid, indid) VALUES (". $obeid .", ". $indid .")";			
                                        $db->executar($sql);
                                }

                                //Remove Série Histórica se a periodicidade ou a Regionalização for diferente.
                                if (($peridAtual!=$perid) || ($regidAtual!=$regid)){
                                    $sql = "update painel.seriehistorica set sehstatus = 'I' where indid = ".$indid;
                                    $db->executar($sql);
                                }         
                                $db->commit();
                                echo "<script>
                                                alert('Dados salvos com sucesso.');
                                                window.location = '?modulo=principal/cadastro&acao=A&indid=$indid&evento=A';
                                          </script>";                                
                            }else{
                                echo "<script>
                                                alert('Você não tem permissão para alterar os dados do indicador');
                                                window.location = '?modulo=principal/cadastro&acao=A&indid=$indid&evento=A';
                                          </script>";                                 
                            }
			}
		exit;
		break;
		
		case 'A': 
			$indid = $_REQUEST['indid']; 			
			$sql = "
				SELECT 
					unmid, indcumulativo, indcumulativovalor, indpublicado, indobservacao, indescala, indqtdevalor, exoid, regid, umeid, cliid, tpiid, secid, secidgestora, acaid, indprod, perid, estid, colid, usucpf, indnome, indobjetivo,
					indformula, indtermos, indfontetermo, peridatual,indshformula,formulash,
					TRIM (TO_CHAR(indnormalinicio , '999G999G999D99')) as indnormalinicio, 
					TRIM (TO_CHAR(indnormalfinal , '999G999G999D99')) as indnormalfinal, 
					TRIM (TO_CHAR(indatencaoinicio , '999G999G999D99')) as indatencaoinicio,
					TRIM (TO_CHAR(indatencaofinal , '999G999G999D99')) as indatencaofinal, 
					TRIM (TO_CHAR(indcriticoinicio , '999G999G999D99')) as indcriticoinicio, 
					TRIM (TO_CHAR(indcriticofinal , '999G999G999D99')) as indcriticofinal, 
					TRIM (TO_CHAR(indmetavalor , '999G999G999D99')) as indmetavalor, 
					indmetadatalimite,
					indpublico,
					indhomologado,
					indencerrado,
					mtiid,
                                        indavalgestor,
                                        indobsgestor
				FROM  painel.indicador i
				WHERE 
				i.indid = ".$indid."
				";
			$dados = $db->pegaLinha($sql);
			
			//OBJETIVO
			$sql_obj = "SELECT 
							o.obeid AS codigo,
							o.obenome AS descricao				
						FROM 
							painel.objetivoestrategico o
						INNER JOIN  
							painel.indicadorobjetivo io On io.obeid = o.obeid  AND io.indid = ".$indid." 
						WHERE
							obestatus = 'A' ";
			
			$obeid = $db->carregar($sql_obj);
			
			//AGENDA DE GOVERNO
			$sql_age = "SELECT 
							agg.aggid AS codigo,
							agg.aggdsc AS descricao				
						FROM 
							painel.agendagoverno agg
						INNER JOIN  
							painel.agendaindicador agi ON agi.aggid = agg.aggid AND agi.indid = ".$indid." 
						WHERE
							aggstatus = 'A' ";
			
			$aggid = $db->carregar($sql_age);
			
			if($dados){
				$exoid 				= $dados['exoid'];
				$unmid 				= $dados['unmid'];				 
				$cliid 				= $dados['cliid'];
				$tpiid 				= $dados['tpiid'];  
				$secid  			= $dados['secid'];
				$secidgestora		= $dados['secidgestora']; 
				$acaid   			= $dados['acaid']; 
				$indprod   			= $dados['indprod'];
				$perid  			= $dados['perid'];
				$peridatual			= $dados['peridatual'];
				$regid				= $dados['regid'];
				$umeid				= $dados['umeid'];  
				$estid  			= $dados['estid'];  
				$colid  			= $dados['colid'];  
				$usucpf  			= $dados['usucpf'];  
				$indnome  			= $dados['indnome'];  
				$indobjetivo  		= $dados['indobjetivo']; 
				$indformula  		= $dados['indformula'];  
				$indtermos  		= $dados['indtermos'];  
				$indfontetermo  	= $dados['indfontetermo'];  
				$indnormalinicio  	= $dados['indnormalinicio'];  
				$indnormalfinal  	= $dados['indnormalfinal'];  
				$indatencaoinicio  	= $dados['indatencaoinicio']; 
				$indatencaofinal  	= $dados['indatencaofinal'];  
				$indcriticoinicio  	= $dados['indcriticoinicio'];  
				$indcriticofinal  	= $dados['indcriticofinal'];  
				$indmetavalor  		= $dados['indmetavalor']; 
				$indmetadatalimite 	= $dados['indmetadatalimite'];
				$indqtdevalor		= $dados['indqtdevalor'];
				$indcumulativo 		= $dados['indcumulativo'];
				$indcumulativovalor	= $dados['indcumulativovalor'];
				$indpublicado 		= $dados['indpublicado'];
				$indescala 			= $dados['indescala'];
				$indobservacao		= $dados['indobservacao'];
				$indpublico			= $dados['indpublico'];
				$indencerrado		= $dados['indencerrado'];
				$indhomologado		= $dados['indhomologado'];
				$mtiid				= $dados['mtiid'];
				$indshformula		= $dados['indshformula'];
				$formulash			= $dados['formulash'];
				$indavalgestor			= $dados['indavalgestor'];
				$indobsgestor			= $dados['indobsgestor'];
			}		
		break;	
	}
}
if ($_SESSION['indid']){
    $sql = "select secid from painel.usuarioresponsabilidade where usucpf = '".$_SESSION['usucpf']."' and pflcod = ".PAINEL_PERFIL_GESTOR_INDICADOR." and secid = ".$secid;
    $secid_confirma = $db->pegaUm($sql);
    if (!in_array(PAINEL_PERFIL_GESTOR_ACOMPANHAMENTO,$listaPerfis) && !in_array(PAINEL_PERFIL_SUPER_USUARIO,$listaPerfis)){
        if (!(in_array(PAINEL_PERFIL_GESTOR_INDICADOR,$listaPerfis) && $secid_confirma)){
            $acesso=false;
        }
    }
}
?>
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>

function escondeSHFormula()
{
	jQuery("#tr_formula_sh").hide();
	jQuery("#tr_detalhe_formula_sh").hide();
}

function exibeSHFormula()
{
	jQuery("#tr_formula_sh").show();
	jQuery("#tr_detalhe_formula_sh").show();
}

function openIndPopUp()
{
	var perid = jQuery("[name='perid']").val();
	var regid = jQuery("[name='regid']").val();
	var erro = 0;
	
	if(!perid){
		alert('Favor selecionar a Periodicidade do Indicador');
		erro = 1;
		return false;
	}
	if(!regid){
		alert('Favor selecionar a Regionalização do Indicador');
		erro = 1;
		return false;
	}
	
	if(erro == 0){
		janela("painel.php?modulo=principal/cadastro&acao=A&requisicao=listaIndicadorFormulaSHPopup&perid="+perid+"&regid="+regid,800,600,"Indicadores");
	}
}

function testarFormulaSHIndicador()
{
	var perid = jQuery("[name='perid']").val();
	var regid = jQuery("[name='regid']").val();
	var erro = 0;
	
	if(!perid){
		alert('Favor selecionar a Periodicidade do Indicador');
		erro = 1;
		return false;
	}
	if(!regid){
		alert('Favor selecionar a Regionalização do Indicador');
		erro = 1;
		return false;
	}
	
	if(erro == 0){
		var parametros = jQuery("#formulario").serialize();
		jQuery("#btn_formula_testar").val("Carregando...");
		jQuery("#btn_formula_testar").attr("disabled",true);
		jQuery.ajax({
		   type: "POST",
		   url: window.location,
		   data: "requisicao=testarFormulaSHIndicador&" + parametros,
		   success: function(msg){
		   		jQuery('#div_testa_formula').html( msg );
		   		jQuery("#btn_formula_testar").val("Testar Fórmula");
		   		jQuery("#btn_formula_testar").attr("disabled",false);
		   }
		});
	}
}

function exibeValorAcumulado(){
	
	if(document.getElementById('indqtdevalor_t').checked == true){
		document.getElementById('cumulatividadevalormonetario').style.display = "";
	}if(document.getElementById('indqtdevalor_f').checked == true){
		document.getElementById('cumulatividadevalormonetario').style.display = "none";
		document.getElementById('indcumulativovalor_s').checked = true;
	}
}

function validaUnidadeMedicao(valor) {
	switch(valor) {
		case '3':
			document.getElementById('coletarvalormonetario').style.display = '';
			document.getElementById('indicarEscala').style.display = '';
			document.formulario.indcumulativo[0].disabled=false;
			document.formulario.indcumulativo[1].disabled=false;
			document.formulario.indcumulativo[2].disabled=false;

			break;
		case '5':
			document.getElementById('coletarvalormonetario').style.display = 'none';
			document.formulario.indqtdevalor[0].checked=true;
			document.getElementById('indicarEscala').style.display = 'none';
			document.formulario.indescala[1].checked=true;
			document.formulario.indcumulativo[0].disabled=false;
			document.formulario.indcumulativo[1].disabled=false;
			document.formulario.indcumulativo[2].disabled=false;

			break;
		case '<?=UNIDADEMEDICAO_PERCENTUAL?>':
			document.formulario.indcumulativo[0].disabled=true;
			document.formulario.indcumulativo[1].disabled=true;
			document.formulario.indcumulativo[2].disabled=true;
			document.formulario.indcumulativo[0].checked=true;
			break;
		case '<?=UNIDADEMEDICAO_NUM_INDICE?>':
			document.formulario.indcumulativo[0].disabled=true;
			document.formulario.indcumulativo[1].disabled=true;
			document.formulario.indcumulativo[2].disabled=true;
			document.formulario.indcumulativo[0].checked=true;
			break;
		default:
			document.formulario.indqtdevalor[0].checked=true;
			document.getElementById('coletarvalormonetario').style.display = 'none';
			document.formulario.indescala[0].checked=true;
			
			document.getElementById('indicarEscala').style.display = 'none';
			document.formulario.indcumulativo[0].disabled=false;
			document.formulario.indcumulativo[1].disabled=false;
			document.formulario.indcumulativo[2].disabled=false;

	}
	
	exibeValorAcumulado();
}
</script>
<form method="POST" id="formulario" name="formulario">
<input type="hidden" name="evento" id="evento" value="I">
<input type="hidden" name="indid" id="indid" value="<?=$indid?>">
<table style="border-bottom:0px"  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloEsquerda" colspan="2">Dados gerais do indicador</td>
</tr>
<tr>
	<?php
	//Verifica se o usuário é do tipo Super Usuário, se não for, só exibe o Eixo que possui permissão.
	$arrPflcod = pegaPerfil();

	if(!in_array(PAINEL_PERFIL_ADM_EIXO,$arrPflcod) || $db->testa_superuser() || in_array(PAINEL_PERFIL_ATUALIZA_SH,$arrPflcod) || in_array(PAINEL_PERFIL_CONSULTA,$arrPflcod)){

		$sql= "SELECT 
			exodsc AS descricao, 
			exoid AS codigo
		FROM 
			painel.eixo
		order by
			 descricao";
	}else{
		$sql= "SELECT 
			exodsc AS descricao, 
			exoid AS codigo
		FROM 
			painel.eixo
		WHERE
			exoid in (
						select 
							distinct exoid
						from
							painel.usuarioresponsabilidade
						where
							usucpf = '{$_SESSION['usucpf']}'
						and
							rpustatus = 'A'
						)
		order by
			 descricao";
	}

	$arrEixos = $db->carregar($sql);

	!$arrEixos? $arrEixos = array() : $arrEixos = $arrEixos;

	foreach($arrEixos as $eixos){
		$arrExoid[] = $eixos['codigo'];
	}

	if($arrEixos && $exoid && in_array(PAINEL_PERFIL_ADM_EIXO,$arrPflcod) && !in_array($exoid,$arrExoid)){
		$sql= "SELECT 
			exodsc AS descricao, 
			exoid AS codigo
		FROM 
			painel.eixo
		order by
			 descricao
		";
		
		$display_botoes = "none";
	}

	?>
    <td align='right' class="SubTituloDireita">Eixo:</td>
    <td><?=$db->monta_combo('exoid',$sql,(($acesso)?'S':'N'),'Selecione...',"",'','','','S','','',$exoid);?></td>
</tr>
<tr>
	<?php 
		$sql= "SELECT 
			secdsc AS descricao, 
			secid AS codigo
		FROM 		
			painel.secretaria
		WHERE
			secstatus = 'A'
		order by
			 descricao	 
		";
	?>
    <td align='right' class="SubTituloDireita">Secretaria/Autarquia Executora:</td>
    <td><?=$db->monta_combo('secid',$sql,(($acesso)?'S':'N'),'Selecione...',"filtraAcao",'','','','S','','',$secid);?></td>
</tr> 

<tr style="display: none" >
	<?php 
		$sql= "SELECT 
			u.usunome AS descricao, 
			u.usucpf AS codigo
			FROM 		
			seguranca.usuario u	
			INNER JOIN seguranca.usuario_sistema us ON us.usucpf = u.usucpf  
			WHERE us.sisid = ".$_SESSION['sisid']."		
		";
	?>
    <td align='right' class="SubTituloDireita">Responsável - Usuário:</td>
    <td><?=$db->monta_combo('usucpf',$sql,(($acesso)?'S':'N'),'Selecione...','','','','','S');?></td>
</tr>
<tr>
	<td align='right' class="SubTituloDireita">Agenda(s) de Governo(s):</td>
	<td>
	<?php 
	$Sql = "SELECT 
				aggdsc AS descricao, 
				aggid AS codigo
			FROM 
				painel.agendagoverno
			WHERE aggstatus = 'A'
			order by
				 descricao";
	combo_popup( 'aggid', $Sql, 'Selecione a(s) Agenda(s) de Governo(s)', '360x460', 0, array(), "", (($acesso)?'S':'N'), false, false, 5, 400);
	?>
	</td>
</tr>
	<?php
		//Verifica se o usuário é do tipo Super Usuário, se não for, só exibe as Ações permitidas.
		if(!in_array(PAINEL_PERFIL_ADM_ACAO,$arrPflcod) || $db->testa_superuser()  || in_array(PAINEL_PERFIL_ATUALIZA_SH,$arrPflcod) || in_array(PAINEL_PERFIL_CONSULTA,$arrPflcod)  ){
		
			$sql= "SELECT 
						acadsc AS descricao, 
						acaid AS codigo
					FROM 		
						painel.acao
					WHERE
						acastatus = 'A'
					order by
						 descricao";
		}else{
			$sql= "SELECT 
				acadsc AS descricao, 
				acaid AS codigo
			FROM 
				painel.acao
			WHERE
				acaid in (
							select 
								distinct acaid
							from
								painel.usuarioresponsabilidade
							where
								usucpf = '{$_SESSION['usucpf']}'
							and
								rpustatus = 'A'
							)
				and acastatus = 'A'	
			order by
				 descricao";
		}

		$arrAcoes = $db->carregar($sql);
		
		!$arrAcoes? $arrAcoes = array() : $arrAcoes = $arrAcoes;
	
		foreach($arrAcoes as $acoes){
			$arrAcaid[] = $acoes['codigo'];
		}
		
		if($arrAcoes && $acaid && in_array(PAINEL_PERFIL_ADM_ACAO,$arrPflcod) && !in_array($acaid,$arrAcaid)){
			$sql= "SELECT 
						acadsc AS descricao, 
						acaid AS codigo
					FROM 		
						painel.acao
					WHERE
						acastatus = 'A'
					order by
						 descricao";
			
			$display_botoes = "none";
			
		}

	?>
    <td align='right' class="SubTituloDireita">Ação:</td>
    <td id="td_acao" ><?=$db->monta_combo('acaid',$sql,(($acesso)?'S':'N'),'Selecione...','','','','','S','','',$acaid);?></td>
</tr>
<tr>
	<?php 
		$sql= "SELECT 
			secdsc AS descricao, 
			secid AS codigo
		FROM 		
			painel.secretaria
		WHERE
			secstatus = 'A'
		order by
			 descricao	 
		";
	?>
    <td align='right' class="SubTituloDireita">Secretaria/Autarquia Gestora:</td>
    <td><?=$db->monta_combo('secidgestora',$sql,(($acesso)?'S':'N'),'Selecione...',"",'','','','S','','',$secidgestora);?></td>
</tr> 
<tr style='display:none'>
	<?php 
		$sql= "SELECT 
			tpidsc AS descricao, 
			tpiid AS codigo
		FROM 		
			painel.tipoindicador
		order by
			 descricao	 
		";
	?>
    <td align='right' class="SubTituloDireita">Tipo Indicador:</td>
    <td><?=$db->monta_combo('tpiid',$sql,(($acesso)?'S':'N'),'Selecione...','','','','','S');?></td>
</tr>
<tr>
	<td width="25%" align='right' class="SubTituloDireita">Nome Indicador:</td>
    <td><?=campo_texto('indnome','S',(($acesso)?'S':'N'),'',100,200,'','');?></td>
</tr>
<tr>
    <td align='right' class="SubTituloDireita">Objetivo Indicador:</td>
    <td><?=campo_textarea( 'indobjetivo', 'S', (($acesso)?'S':'N'), '', 60, 2, 200 );?></td>
</tr>
<tr>
    <td align='right' class="SubTituloDireita">Observação:</td>
    <td><?=campo_textarea( 'indobservacao', 'N', (($acesso)?'S':'N'), '', 98, 5, 2000 );?></td>
</tr> 
<tr>
	<?php 
		$sql= "SELECT 
			mtidsc AS descricao, 
			mtiid AS codigo
		FROM 
			pde.monitoratipoindicador 
		order by
			 descricao";
	?>
    <td align='right' class="SubTituloDireita">Tipo de Indicador:</td>
    <td><?=$db->monta_combo('mtiid',$sql,(($acesso)?'S':'N'),'Selecione...','','','','N','','',$mtiid);?></td>
</tr> 
<tr>
	<td class="SubTituloEsquerda" colspan="2">Dados de exibição</td>
</tr>
<tr>
	<?php 
		$sql= "SELECT 
			unmdesc AS descricao, 
			unmid AS codigo
		FROM 
			painel.unidademedicao
		order by
			 descricao
		";
	?>
    <td align='right' class="SubTituloDireita">Unidade de Medição:</td>
    <td><?=$db->monta_combo('unmid',$sql,(($acesso)?'S':'N'),'Selecione...','validaUnidadeMedicao','','','','S');?></td>
</tr>  
<tr>
	<?php 
		$sql= "SELECT 
			umedesc AS descricao, 
			umeid AS codigo
		FROM 		
			painel.unidademeta	
		where 
			umestatus = 'A'
		order by
			 descricao	 
		";
	?>
    <td style="display: none;" align='right' class="SubTituloDireita">Exibir produto da unidade como:</td>
    <td style="display: none;"><?=$db->monta_combo('umeid',$sql,'N','Selecione...','','','','','S', null, null, 76);?></td>

    <td align='right' class="SubTituloDireita">Produto:</td>
    <td><?= campo_textarea( 'indprod', 'S', (($acesso)?'S':'N'), '', 98, 1, 70 );?></td>
</tr>

<tr style="display: none" >
	<?php 
		$sql= "SELECT 
			clidsc AS descricao, 
			cliid AS codigo
		FROM 		
			painel.classeindicador
		order by
			 descricao		 
		";
	?>
    <td align='right' class="SubTituloDireita">Classe do Indicador:</td>
    <td><?=$db->monta_combo('cliid',$sql,(($acesso)?'S':'N'),'Selecione...','','','','','N');?></td>
</tr>
<tr style="display: none" id="coletarvalormonetario">
	<?php
		$checkNao = "";
		$checkSim = "";
		$indqtdevalor != "t" ? $checkNao = "checked=checked" : $checkSim = "checked=checked";
	?>
    <td align='right' class="SubTituloDireita">Coletar valor monetário?</td>
    <td><input type="radio" onclick="exibeValorAcumulado()" id="indqtdevalor_f" value="false" <?=$checkNao?> name="indqtdevalor" />Não <input onclick="exibeValorAcumulado()" type="radio" <?=$checkSim?> value="true" id="indqtdevalor_t" name="indqtdevalor" />Sim</td>
</tr>
<tr style="display: none" id="indicarEscala">
<?php
//Pergunta para a escala
$checkNaoEsc = "";
$checkSimEsc = "";
$indescala != "t" ? $checkNaoEsc = "checked=checked" : $checkSimEsc = "checked=checked";

?>
    <td align='right' class="SubTituloDireita">Utilizar escala de exibição?</td>
    <td><input type="radio" value="false" <?=$checkNaoEsc?> name="indescala" />Não <input type="radio" <?=$checkSimEsc?> value="true" name="indescala" />Sim</td>
</tr>

<tr>
	<?php
		$checkNaoAc = "";
		$checkSimAc = "";
		$indcumulativo == "S" ? $checkSimAc = "checked=checked" : (($indcumulativo == "N")? $checkNaoAc = "checked=checked": $checkAnoAc = "checked=checked");
		!$indcumulativo ? $checkNaoAc = "checked=checked" : "";
		!$indcumulativo ? $checkAnoAc = "" : "";
		
		if($unmid == UNIDADEMEDICAO_PERCENTUAL || $unmid == UNIDADEMEDICAO_NUM_INDICE){
			$checkNaoAc = "checked=checked";
			$checkSimAc = "";
			$checkAnoAc = "";
			$disabledCheck = "disabled=disabled";
		}
		
	?>
    <td align='right' class="SubTituloDireita">Exibir dados acumulados entre os períodos?</td>
    <td><input type="radio" <?=$disabledCheck?> value="N" <?=$checkNaoAc?> name="indcumulativo" />Não <input type="radio" <?=$checkSimAc?> value="S" <?=$disabledCheck?> name="indcumulativo" />Sim <input type="radio" <?=$checkAnoAc?> value="A" <?=$disabledCheck?> name="indcumulativo" />Por ano</td>
</tr>

<tr id="cumulatividadevalormonetario" >
	<?php
		$checkValorNaoAc = "";
		$checkValorSimAc = "";
		$indcumulativovalor == "S" ? $checkValorSimAc = "checked=checked" : (($indcumulativovalor == "N")? $checkValorNaoAc = "checked=checked" : $checkValorAnoAc = "checked=checked");
		!$indcumulativovalor ? $checkValorSimAc = "checked=checked" : "";
		!$indcumulativovalor ? $checkValorNaoAc = "" : "";
		!$indcumulativovalor ? $checkValorAnoAc = "" : "";
	?>
    <td align='right' class="SubTituloDireita">Exibir valor monetário acumulado entre os períodos?</td>
    <td><input type="radio" value="N" <?=$checkValorNaoAc?> name="indcumulativovalor" id="indcumulativovalor_n" />Não <input id="indcumulativovalor_s" type="radio" <?=$checkValorSimAc?> value="S" name="indcumulativovalor" />Sim <input type="radio" <?=$checkValorAnoAc?> value="A" name="indcumulativovalor" />Por ano</td>
</tr>

<script>
validaUnidadeMedicao('<?php echo $unmid; ?>');
</script>
	<?php
//Testa se é Super Usuário ou Gestor PDE ou Equipe de apoio ao Gestor PDE
$sql = "select count(*)
		from seguranca.perfilusuario pu, seguranca.perfil p 
		where p.pflcod = pu.pflcod and 
			  pu.usucpf = '{$_SESSION['usucpf']}' and 
			  p.sisid = {$_SESSION['sisid']} and
			  pflstatus = 'A' and
			  pu.pflcod in (349,272,271)";	  
$permite = $db->pegaUm($sql);

$checkNaoPub = "";
$checkSimPub = "";
$indpublicado != "t" || !$indpublicado ? $checkNaoPub = "checked=checked" : $checkSimPub = "checked=checked";

/*
 * 17/03/2010 - Solicitado por Cristiano Cabral - Se for novo indicador, a publicação sempre devera ser FALSE.
 * 17/03/2010 - Solicitado por Vitor Sad -  Se for novo indicador, a opção publicado deve vir marcado com FALSE
 */
		
if($permite>0 && (in_array(PAINEL_PERFIL_SUPER_USUARIO,$arrPflcod) || in_array(GESTOR_PDE,$arrPflcod) || in_array(EQUIPE_APOIO_GESTOR_PDE,$arrPflcod))){
?>
	<tr>	
		<td align='right' class="SubTituloDireita">Publicado?</td>
		<td>	
			<input type="radio" value="false" <?=$checkNaoPub?> name="indpublicado" onclick="mostraindpublico(0)"/>Não 
			<input type="radio" value="true"  <?=$checkSimPub?> name="indpublicado" onclick="mostraindpublico(1)"/>Sim
		</td>
	</tr>
    <tr style="display: none;">
		<td align='right' class="SubTituloDireita">Homologado?</td>
		<td>
			<input type="radio" value="false" name="indhomologado" id="indhomologado0" <?= ($indhomologado == 'f') ? 'checked="checked"' : '' ?>/>Não
            <input type="radio" value="true"  name="indhomologado" id="indhomologado1" <?= ($indhomologado == 't') ? 'checked="checked"' : '' ?>/>Sim
		</td>
	</tr>
	<tr style="display: none;" id="mod-publico">
		<td align='right' class="SubTituloDireita">Módulo Público?</td>
		<td>
			<input type="radio" value="false" name="indpublico" id="indpublico0" <?= ($indpublico == 'f') ? 'checked="checked"' : '' ?>/>Não 
			<input type="radio" value="true"  name="indpublico" id="indpublico1" <?= ($indpublico == 't') ? 'checked="checked"' : '' ?>/>Sim
		</td>
	</tr>

<tr style="display: none;">
<?php
}else{
	if($indpublicado=='')
		$indpublicado = 'false';
	echo "<input type='hidden' name='indpublicado' value='$indpublicado'>";

	if($indpublico=='')
		$indpublico = 'false';
	echo "<input type='hidden' name='indpublico' value='$indpublico'>";

}
?>
	<td align='right' class="SubTituloDireita">Série Histórica com Fórmula?</td>
	<td>
		<input type="radio" value="false" onclick="escondeSHFormula()" name="indshformula" id="indshformula0" <?= (!$indshformula || $indshformula == 'f') ? 'checked="checked"' : '' ?>/>Não 
		<input type="radio" value="true"  onclick="exibeSHFormula()"   name="indshformula" id="indshformula1" <?= ($indshformula == 't') ? 'checked="checked"' : '' ?>/>Sim
	</td>
</tr>
    <input type="hidden" name="indshformula" value="false">
<?php 
if (in_array(PAINEL_PERFIL_SUPER_USUARIO,$arrPflcod))
{
?>
<tr >	
	<td align='right' class="SubTituloDireita">Indicador Encerrado?</td>
	<td>
		<input type="radio" value="false" name="indencerrado" id="indencerrado0" <?= (!$indencerrado ||  $indencerrado == 'f') ? 'checked="checked"' : '' ?>/>Não 
		<input type="radio" value="true"  name="indencerrado" id="indencerrado1" <?= ($indencerrado == 't') ? 'checked="checked"' : '' ?>/>Sim
	</td>
</tr>
<?php 
}
else 
{
	echo "<input type='hidden' name='indencerrado' value='$indencerrado'>";
}
?>
	<tr id="tr_formula_sh" style="display:<?php echo $indshformula == 't' ? '' : 'none'; ?>" >
		<td class="SubtituloDireita" >Fórmula:</td>
		<td>
			<?php echo campo_textarea("formulash","S","S","",81,5,5000) ?>
			<?php 
				if($formulash){
					$hdn_indid = explode("{Indicador:",str_replace(array(" ","-","+","*"),array("","","",""),$formulash));
					if($hdn_indid){
							foreach($hdn_indid as $i){
								if($i){
									$fim = strpos($i,"}");
									$valor = substr($i,0,$fim);
									$dado = explode("_",$valor);
									if($dado[1]){
										$sql = "select dpeid from painel.detalheperiodicidade where dpedsc = '".$dado[1]."'";
										$dpeid__ = $db->pegaUm($sql);
										if($dpeid__){
											$arrIndFormula[] = $dado[0]."_".$dpeid__;
										}
									}else{
										$arrIndFormula[] = $dado[0];
									}
								}
							}
						}
				}
				?>
			<input type="button" value="Inserir Indicador" name="tbn_formula_add_indicador" onclick="openIndPopUp()" />
			<input type="button" id="btn_formula_testar" onclick="testarFormulaSHIndicador()" value="Testar Fórmula" />
			<input type="hidden" id="hdn_indid" name="hdn_indid" value="<?php echo $arrIndFormula ? implode(",",$arrIndFormula) : "" ?>" />
			<div id="div_testa_formula" ></div>
		</td>
	</tr>
	<tr id="tr_detalhe_formula_sh" style="display:<?php echo $indshformula == 't' ? '' : 'none'; ?>" >
		<td class="SubtituloDireita" >Detalhes da Fórmula:</td>
		<td>
		<div id="div_detalhe_formula" >
			<?php
				if($arrIndFormula){
					$arrIndUsados = array();
					foreach($arrIndFormula as $indF){
						if(strstr($indF,"_") && !in_array($indF,$arrIndUsados)){
							$data = explode("_",$indF);
							$ind_ = $data[0];
							$dpeid_ = $data[1];
							$sql = "select 
										indnome
									from 
										painel.indicador ind
									where 
										indid = '$ind_'";
							$indnomeF = $db->pegaUm($sql);
							$sql = "select 
										dpedsc
									from 
										painel.detalheperiodicidade
									where 
										dpeid = '$dpeid_'";
							$dpedsc = $db->pegaUm($sql);
							echo "<p><b>Indicador: $ind_</b> - $indnomeF de $dpedsc</p>";
							$arrIndUsados[] = $ind;
						}else{
							if($indF && !in_array($indF,$arrIndUsados)){
								$sql = "select 
										indnome
									from 
										painel.indicador ind
									where 
										indid = '$indF'";
								$indnomeF = $db->pegaUm($sql);
								echo "<p><b>Indicador: $indF</b> - $indnomeF - Padrão</p>";
								$arrIndUsados[] = $indF;
							}
						}
					}
				}
				?>
		</div>
		</td>
	</tr>
</table>

<table style="border-top:0px;border-top:0px;border-bottom:0px"  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloEsquerda" colspan="2">Dados sobre a coleta do indicador</td>
</tr>
<tr style="display:none" >
	<?php 
		$sql= "SELECT 
			pesdsc AS descricao, 
			pesid AS codigo
		FROM 		
			painel.perspectiva
		order by
			 descricao
		";
	?>
    <td align='right' class="SubTituloDireita">Perspectiva:</td>
    <td><?=$db->monta_combo('pesid',$sql,(($acesso)?'S':'N'),'Selecione...','','','','','S');?></td>
</tr>
<tr style="display:none" >
	<?php 
		$sql= "SELECT 
			temdsc AS descricao, 
			temid AS codigo
		FROM 		
			painel.tema
		order by
			 descricao 
		";
	?>
    <td width="25%" align='right' class="SubTituloDireita">Elemento Estratégico:</td>
    <td><?=$db->monta_combo('temid',$sql,(($acesso)?'S':'N'),'Selecione...','','','','','S');?></td>
</tr>  
<tr>
    <td align='right' class="SubTituloDireita">Fórmula:</td>
    <td><?=campo_textarea( 'indformula', 'N', (($acesso)?'S':'N'), '', 98, 5, 1000 );?></td>
</tr> 
<tr style="display: none;">
    <td align='right' class="SubTituloDireita">Termos:</td>
    <td><?= campo_textarea( 'indtermos', 'N', (($acesso)?'S':'N'), '', 98, 5, 1000 );?></td>
</tr>
<tr style="display: none;">
    <td align='right' class="SubTituloDireita">Fontes por Termo:</td>
    <td><?= campo_textarea( 'indfontetermo', 'N', (($acesso)?'S':'N'), '', 98, 5, 1000 );?></td>
</tr>
<tr>
	<?php 
		$sql= "SELECT 
			perdsc AS descricao, 
			perid AS codigo
		FROM 
			painel.periodicidade
		WHERE
			perstatus = 'A'	
		and perid in (1,2,3)
		order by
			 descricao		 
		";
	?>
    <td width="25%" align='right' class="SubTituloDireita">Periodicidade do Indicador:</td>
    <td><?=$db->monta_combo('perid',$sql,(($acesso)?'S':'N'),'Selecione...','verificaSH','','','','S');?> <input type="hidden" name="perid_conf" id="perid_conf" value="<?php echo $perid; ?>"></td>
</tr>
<tr>
	<?php 
		$sql= "SELECT 
			estdsc AS descricao, 
			estid AS codigo
		FROM 		
			painel.estilo
		order by
			 descricao	 
		";
	?>
    <td align='right' class="SubTituloDireita">Estilo:</td>
    <td><?=$db->monta_combo('estid',$sql,(($acesso)?'S':'N'),'Selecione...','','','','','S','esdid','',$estid);?></td>
</tr> 
<tr>
	<?php 
		$sql= "SELECT 
			regdescricao AS descricao, 
			regid AS codigo
		FROM 		
			painel.regionalizacao
                where regid in (1,6,4,16)
		order by
			 descricao		 
		";
	?>
    <td align='right' class="SubTituloDireita">Regionalização:</td>
    <td><?=$db->monta_combo('regid',$sql,(($acesso)?'S':'N'),'Selecione...','verificaSH','','','','S');?> <input type="hidden" name="regid_conf" id="regid_conf" value="<?php echo $regid; ?>"></td>
</tr>
<tr style="display: none" >
	<td align='right' class="SubTituloDireita">Objetivo Estratégico:</td>
    <td>
    	<?php
    	
    	$sqlobj= " SELECT
						obeid AS codigo,
						obenome AS descricao
					FROM 
						painel.objetivoestrategico
					WHERE
						obestatus = 'A'
					order by
			 			descricao
					";
		combo_popup( "obeid", $sqlobj, "Objetivos Estratégicos", "192x400", 0, array(), "", "S", false, false, 5, 400 );
		?>
	</td>
</tr>
<tr>
	<td class="SubTituloEsquerda" colspan="2">Cronograma de atualização</td>
</tr>
<tr>
	<?php 
		$sql= "SELECT 
			perdsc AS descricao, 
			perid AS codigo
		FROM 
			painel.periodicidade
		WHERE
			perstatus = 'A'	
                    and perid in (1,2,3)
		order by
			 descricao		 
		";
	?>
    <td width="25%" align='right' class="SubTituloDireita">Periodicidade de atualização:</td>
    <td><?=$db->monta_combo('peridatual',$sql,(($acesso)?'S':'N'),'Selecione...','','','','','S');?></td>
</tr> 
<tr>
	<?php 
		$sql= "SELECT 
			coldsc AS descricao, 
			colid AS codigo
		FROM 		
			painel.coleta
		order by
			 descricao		 
		";
	?>
    <td align='right' class="SubTituloDireita">Coleta:</td>
    <td><?=$db->monta_combo('colid',$sql,(($acesso)?'S':'N'),'Selecione...','','','','','S');?></td>
</tr>
</table>


<table style="display:none" style="border-top:0px;border-bottom:0px;"  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td colspan="3" style="text-align: left;font-weight: bold;" bgcolor="#DCDCDC" >Informe as faixas para os níveis de medição abaixo:</td>
</tr>
<tr>
	<td width="10%" rowspan="2" align="center"  bgcolor="#DCDCDC"><b>Normal</b></td>
	<td width="15%" align='right' class="SubTituloDireita"> Início:</td>
    <td><?=campo_texto('indnormalinicio','S','S','',20,'','###.###.###,##','');?></td>
</tr>
<tr>
	<td align='right' class="SubTituloDireita"> Final:</td>
    <td><?=campo_texto('indnormalfinal','S','S','',20,'','###.###.###,##','');?></td>
</tr>
<tr>
	<td rowspan="2" align="center"  bgcolor="#DCDCDC"><b>Atenção</b></td>
	<td align='right' class="SubTituloDireita">Início:</td>
    <td><?=campo_texto('indatencaoinicio','S','S','',20,'','###.###.###,##','');?></td>
</tr>
<tr>
	<td align='right' class="SubTituloDireita">Final:</td>
    <td><?=campo_texto('indatencaofinal','S','S','',20,'','###.###.###,##','');?></td>
</tr>
<tr>
	<td rowspan="2" align="center"  bgcolor="#DCDCDC"><b>Crítico</b></td>
	<td align='right' class="SubTituloDireita">Início:</td>
    <td><?=campo_texto('indcriticoinicio','S','S','',20,'','###.###.###,##','');?></td>
</tr>
<tr>
	<td align='right' class="SubTituloDireita">Final:</td>
    <td><?=campo_texto('indcriticofinal','S','S','',20,'','###.###.###,##','');?></td>
</tr>
</table>
	<table style="border-top:0px;" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
            <tr bgcolor="#cccccc">
                <td style="text-align: center" > 
                    <?php if(!$acesso){ ?>
                        <input type="button" disabled="disabled" class="botao" name="btassociar" value="Gravar">
                    <?php }else{ ?>
                        <input type="button" class="botao" name="btassociar" value="Gravar" onclick="submeter('I', '<?=$indid?>');">
                        <input type="button" class="botao" name="btassociar" value="Novo Indicador" onclick="novo();">	  	  
                        <input type="button" class="botao" name="btassociar" value="Limpar" onclick="limpar();">	  	  	

                    <?php } ?>		      
                </td>	  	  
            </tr>
	</table>
</form>
<script type="text/javascript" src="/includes/prototype.js"></script>	
<script type="text/javascript">
function verificaSH(){
    if (document.formulario.indid.value!=''){
        if (document.formulario.perid.value!==document.formulario.perid_conf.value){
            alert('Se alterar a periodicidade a série histórica será apagada!'); 
            return;
        }
        if (document.formulario.regid.value!==document.formulario.regid_conf.value){
            alert('Se alterar a regionalização a série histórica será apagada!'); 
            return;
        }    
    }
}

function submeter(evento, indid){	
	if(validar()){
		selectAllOptions( document.getElementById( 'obeid' ) );
		selectAllOptions( document.getElementById( 'aggid' ) );
		document.getElementById('evento').value = evento;
		document.getElementById('indid').value = indid;	
		document.formulario.submit();	
	}
}

function filtraAcao(secid) {
	if(!secid){
		return false;
	}
	var destino = document.getElementById("td_acao");
	var myAjax = new Ajax.Request(
		window.location.href,
		{
			method: 'post',
			parameters: "filtraAcaoAjax=true&" + "secid=" + secid,
			asynchronous: false,
			onComplete: function(resp) {
				if(destino) {
					destino.innerHTML = resp.responseText;
				} 
			},
			onLoading: function(){
				destino.innerHTML = 'Carregando...';
			}
		});
}

function novo(){
	window.location.href = "?modulo=principal/cadastro&acao=A&indid=novoIndicador";		
}

function limpar(){	
	document.getElementById('evento').value = '';	
	document.formulario.unmid.value = '';		
	document.formulario.cliid.value = '';
	document.formulario.tpiid.value = '';
	document.formulario.acaid.value = '';
	document.formulario.perid.value = '';
	document.formulario.estid.value = '';
	document.formulario.colid.value = '';
	document.formulario.usucpf.value = '';
	document.formulario.secid.value = '';
	document.formulario.secidgestora.value = '';
	document.formulario.indnome.value = '';
	document.formulario.indobjetivo.value = '';
	document.formulario.indformula.value = '';
	document.formulario.indtermos.value = ''; 
	document.formulario.indfontetermo.value = '';
	document.formulario.indnormalinicio.value = '';
	document.formulario.indobservacao.value = '';
	document.formulario.indnormalfinal.value = '';
	document.formulario.indatencaoinicio.value = '';
	document.formulario.indatencaofinal.value = '';
	document.formulario.indcriticoinicio.value = '';
	document.formulario.indcriticofinal.value = '';
	document.formulario.secidgestora.value = '';
}

function validar(){	
	var msg = "";	
	if(document.formulario.unmid.value == ''){
		msg+="O preenchimento do campo \"Unidade de Medição\" é obrigatório.\n";
	}
	if(document.formulario.exoid.value == ''){
		msg+="O preenchimento do campo \"Eixo\" é obrigatório.\n";
	}
	if(document.formulario.acaid.value == ''){
		msg+="O preenchimento do campo \"Ação\" é obrigatório.\n";
	}
	if(document.formulario.perid.value == ''){
		msg+="O preenchimento do campo \"Periodicidade\" é obrigatório.\n";
	}
	if(document.formulario.peridatual.value == ''){
		msg+="O preenchimento do campo \"Periodicidade de atualização\" é obrigatório.\n";
	}
	if(document.formulario.estid.value == ''){
		msg+="O preenchimento do campo \"Estilo\" é obrigatório.\n";
	}
	if(document.formulario.colid.value == ''){
		msg+="O preenchimento do campo \"Coleta\" é obrigatório.\n";
	}
	if(document.formulario.secid.value == ''){
		msg+="O preenchimento do campo \"Secretaria/Autarquia Executora\" é obrigatório.\n";
	}
	if(document.formulario.secidgestora.value == ''){
		msg+="O preenchimento do campo \"Secretaria/Autarquia Gestora\" é obrigatório.\n";
	}
	if(document.formulario.indnome.value == ''){
		msg+="O preenchimento do campo \"Nome Indicador\" é obrigatório.\n";
	}
	if(document.formulario.indobjetivo.value == ''){
		msg+="O preenchimento do campo \"Objetivo Indicador\" é obrigatório.\n";
	}
	if(document.formulario.umeid.value == ''){
		msg+="O preenchimento do campo \"Unidade Meta\" é obrigatório.\n";
	}
	if(document.formulario.regid.value == ''){
		msg+="O preenchimento do campo \"Regionalização\" é obrigatório.\n";
	}
	if(document.formulario.indprod.value == ''){
		msg+="O preenchimento do campo \"Produto\" é obrigatório.\n";
	}

	if(msg != "") {
		alert(msg);
		return false;
	} else {
		return true;			
	}	
}

function mostraindpublico(tr){
	var trPublico   = document.getElementById('mod-publico');
	var indpublico0 = document.getElementById('indpublico0');
	var indpublico1 = document.getElementById('indpublico1');
	if( tr == 1 ){
		trPublico.style.display = 'table-row';
		document.formulario.indhomologado[1].checked=true;
	}else{
		trPublico.style.display = 'none';
		indpublico0.checked = true; 
		indpublico1.checked = false; 
	}
}

</script>