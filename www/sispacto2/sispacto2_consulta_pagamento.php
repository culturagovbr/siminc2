<?
$_REQUEST['baselogin'] = "simec_espelho_producao";

/* configura��es */
ini_set("memory_limit", "2048M");
set_time_limit(0);
/* FIM configura��es */


// carrega as fun��es gerais
include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/workflow.php";

error_reporting(1);

include "_constantes.php";
include "_funcoes.php";

// CPF do administrador de sistemas
if(!$_SESSION['usucpf']) {
	$_SESSION['usucpforigem'] = '';
	$_SESSION['usucpf'] = '';
}

// abre conex�o com o servidor de banco de dados
$db = new cls_banco();

?>
<html>
<head>
	<title>SIMEC- Sistema Integrado de Monitoramento do Minist�rio da Educa��o</title>
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	<script>
	function consultarPagamento() {
		if(document.getElementById('cpf').value=='') {
			alert('CPF em branco');
			return false;
		}
		
		if(!validar_cpf(document.getElementById('cpf').value)) {
			alert('CPF inv�lido');
			return false;
		}
		
		if(document.getElementById('datanascimento').value=='') {
			alert('Data de Nascimento em branco');
			return false;
		}
		
		if(!validaData(document.getElementById('datanascimento'))) {
			alert('Data de Nascimento inv�lida');
			return false;
		}
		
		document.getElementById('formulario').submit();
	
	}
	
	function abrirDetalhes(id) {
		if(document.getElementById('img_'+id).title=='mais') {
			document.getElementById('tr_'+id).style.display='';
			document.getElementById('img_'+id).title='menos';
			document.getElementById('img_'+id).src='../imagens/menos.gif'
		} else {
			document.getElementById('tr_'+id).style.display='none';
			document.getElementById('img_'+id).title='mais';
			document.getElementById('img_'+id).src='../imagens/mais.gif'

		}
	
	}
	</script>
</head>
<body topmargin="0" leftmargin="0">

<?

$menu = array( 0 => array("id" => 1, "descricao" => "SISPACTO 2013", "link" => "/sispacto/sispacto_consulta_pagamento.php"),
			   1 => array("id" => 2, "descricao" => "SISPACTO 2014", "link" => "/sispacto2/sispacto2_consulta_pagamento.php"));

echo "<br/>";
echo montarAbasArray($menu, $abaAtiva);


?>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
<tr>
	<td width="25%"><img src="/includes/layout/azul/img/logo.png" border="0" /></td>
	<td valign="middle" style="font-size:15px;"><b>Consulta Pagamento/Avalia��o no SISPACTO 2014</b></td>
</tr>
</table>
<?php

if($_POST['requisicao']=='consultarPagamento') :
	
	if(strlen(trim($_POST['datanascimento']))==10 && strlen(trim($_POST['cpf']))==14) {
		$sql = "SELECT uncid, iusd FROM sispacto2.identificacaousuario WHERE iuscpf='".addslashes(str_replace(array(".","-"),array("",""),$_POST['cpf']))."' AND iusdatanascimento='".formata_data_sql($_POST['datanascimento'])."'";
		$identificacaousuario = $db->pegaLinha($sql);
	}
	
	if(!$identificacaousuario['iusd']) {
		$al = array("alert"=>"Usu�rio n�o encontrado no SISPACTO","location"=>"sispacto2_consulta_pagamento.php");
		alertlocation($al);
	}
	
	if($identificacaousuario['uncid']) {
		
		echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="5" cellPadding="8" align="center">';
		echo '<tr>';
		echo '<td class="SubTituloDireita">Recomenda��es:</td>';
		echo '<td>';
		
		$sql = "SELECT i2.iusnome||' ( '||p.pfldsc||' )' as iusnome,
			       CASE WHEN ma.mavrecomendadocertificacao='1' THEN '<font style=color:blue;><b>Recomendado '|| CASE WHEN (t2.pflcod='".PFL_FORMADORIES."' OR t2.pflcod='".PFL_ORIENTADORESTUDO."') THEN 'para certifica��o IES' WHEN t2.pflcod='".PFL_COORDENADORLOCAL."' THEN 'para SISPACTO 2015' END ||'</b></font>'
				    	WHEN ma.mavrecomendadocertificacao='2' THEN '<font style=color:red;><b>N�o recomendado '|| CASE WHEN (t2.pflcod='".PFL_FORMADORIES."' OR t2.pflcod='".PFL_ORIENTADORESTUDO."') THEN 'para certifica��o IES' WHEN t2.pflcod='".PFL_COORDENADORLOCAL."' THEN 'para SISPACTO 2015' END ||'</b></font>' END as certificacao,
				   '<textarea cols=\"45\" rows=\"3\" style=\"width:98%;\" class=\"txareanormal\">'||ma.mavrecomendadocertificacaojustificativa||'</textarea>'as justificativa
			FROM sispacto2.identificacaousuario i
			INNER JOIN sispacto2.mensario m ON m.iusd = i.iusd
			INNER JOIN sispacto2.mensarioavaliacoes ma ON ma.menid = m.menid
			INNER JOIN sispacto2.identificacaousuario i2 ON i2.iusd = ma.iusdavaliador
			INNER JOIN sispacto2.tipoperfil t2 ON t2.iusd = i2.iusd
			INNER JOIN seguranca.perfil p ON p.pflcod = t2.pflcod
			WHERE i.iusd='".$identificacaousuario['iusd']."' AND ma.mavrecomendadocertificacao IS NOT NULL";
		
		$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','',false, false, false, false);
			
		echo '</td>';
		echo '</tr>';
		echo '</table>';
		
		$sql = "SELECT p.plpmaximobolsas
			FROM sispacto2.identificacaousuario i
			INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd
			INNER JOIN sispacto2.pagamentoperfil p ON p.pflcod = t.pflcod
			WHERE i.iusd='".$identificacaousuario['iusd']."'";
		
		$nmaximobolsas = $db->pegaUm($sql);
	
		
		$sql = "SELECT f.fpbid as codigo, 
					   rf.rfuparcela ||'� Parcela ( Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia ||' )' as descricao,
					   COALESCE((
					   	SELECT e.esddsc || ' ( ' || to_char(h.htddata,'dd/mm/YYYY HH24:MI') || ' )' as s FROM sispacto2.pagamentobolsista p 
						INNER JOIN workflow.documento d ON d.docid = p.docid 
						INNER JOIN workflow.estadodocumento e ON e.esdid = d.esdid 
						LEFT JOIN workflow.historicodocumento h ON h.hstid = d.hstid 
						WHERE p.iusd='".$identificacaousuario['iusd']."' AND p.fpbid=f.fpbid
						),'') as statuspagamento  
				FROM sispacto2.folhapagamento f 
				INNER JOIN sispacto2.folhapagamentouniversidade rf ON rf.fpbid = f.fpbid AND rf.pflcod=(SELECT pflcod FROM sispacto2.tipoperfil WHERE iusd=".$identificacaousuario['iusd'].")
				INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia
				WHERE f.fpbstatus='A' AND rf.uncid='".$identificacaousuario['uncid']."' AND to_char(NOW(),'YYYYmmdd')>=to_char((fpbanoreferencia::text||lpad(fpbmesreferencia::text, 2, '0')||'01')::date,'YYYYmm') ".(($nmaximobolsas)?"LIMIT ".$nmaximobolsas:"");
		
		$folhapagamento = $db->carregar($sql);
		
		if($folhapagamento[0]) {
			echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="5" cellPadding="8" align="center">';
			echo '<tr><td class="SubTituloEsquerda" colspan="2">Extrato de pagamento/avalia��es</td></tr>';
			echo '<tr><td class="SubTituloCentro" width="50%">Parcela</td><td class="SubTituloCentro" width="50%">Situa��o pagamento(Data de atualiza��o)</td></tr>';
			foreach($folhapagamento as $fl) {
				
				$periodoslistados[] = $fl['codigo'];
				
				echo '<tr><td class="SubTituloEsquerda">'.$fl['descricao'].'</td>
						  <td><font size=3><b>'.$fl['statuspagamento'].'</b></font></td></tr>';
				echo '<tr><td colspan="2"><img src="../imagens/mais.gif" style="cursor:pointer;" title="mais" id="img_ava_'.$fl['codigo'].'" onclick="abrirDetalhes(\'ava_'.$fl['codigo'].'\');"> Detalhes da avalia��o</td></td></tr>';
				echo '<tr style="display:none;" id="tr_ava_'.$fl['codigo'].'"><td colspan="2">';
				echo '<p align="center"><b>INFORMA��ES SOBRE AVALIA��ES</b></p>';
				$sql = "SELECT * FROM sispacto2.mensario WHERE fpbid='".$fl['codigo']."' AND iusd='".$identificacaousuario['iusd']."'";
				$mensario = $db->pegaLinha($sql);
				
				if($mensario['menid']) consultarDetalhesAvaliacoes(array('menid'=>$mensario['menid']));
				else echo '<p align=center style=color:red;>N�o existem avalia��es nesse per�odo de refer�ncia</p>';
				
				echo '</td></tr>';
				echo '<tr><td colspan="2"><img src="../imagens/mais.gif" style="cursor:pointer;" title="mais" id="img_pag_'.$fl['codigo'].'" onclick="abrirDetalhes(\'pag_'.$fl['codigo'].'\');"> Detalhes do pagamento</td></td></tr>';
				echo '<tr style="display:none;" id="tr_pag_'.$fl['codigo'].'"><td colspan="2">';
				echo '<p align="center"><b>INFORMA��ES SOBRE PAGAMENTO</b></p>';
				$sql = "SELECT pboid FROM sispacto2.pagamentobolsista WHERE fpbid='".$fl['codigo']."' AND iusd='".$identificacaousuario['iusd']."'";
				$pboid = $db->pegaUm($sql);
				
				if($pboid) {
					consultarDetalhesPagamento(array('pboid'=>$pboid));
				} else {
					echo "<p align=center style=color:red;>N�o existem pagamentos nesse per�odo de refer�ncia</p>";	
					
					$restricao = pegarRestricaoPagamento(array('iusd' => $identificacaousuario['iusd'], 'fpbid' => $fl['codigo']));
					echo "<table class=\"listagem\" bgcolor=\"#f5f5f5\" cellSpacing=\"5\" cellPadding=\"10\" align=\"center\">";
					echo "<tr>";
					echo "<td class=\"SubTituloDireita\"><b>Poss�vel restri��o:</b></td>";
					echo "<td><b>".$restricao."</b></td>";
					echo "</tr>";
					echo "</table>";
				} 
				
				
				echo '</td></tr>';
				
			}
			
			
			if($periodoslistados) {
					
				$sql = "SELECT f.fpbid as codigo,
					   	   'Parcela ( Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia ||' )' as descricao,
						   COALESCE((
											   	SELECT e.esddsc || ' ( ' || to_char(h.htddata,'dd/mm/YYYY HH24:MI') || ' )' as s FROM sispacto2.pagamentobolsista p
												INNER JOIN workflow.documento d ON d.docid = p.docid
												INNER JOIN workflow.estadodocumento e ON e.esdid = d.esdid
												LEFT JOIN workflow.historicodocumento h ON h.hstid = d.hstid
												WHERE p.iusd='".$identificacaousuario['iusd']."' AND p.fpbid=f.fpbid
												),'') as statuspagamento
					FROM sispacto2.pagamentobolsista p
					INNER JOIN sispacto2.folhapagamento f ON f.fpbid = p.fpbid
					INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia
					WHERE iusd='".$identificacaousuario['iusd']."' AND f.fpbid NOT IN('".implode("','",$periodoslistados)."')";
					
				$pagamentosrestantes = $db->carregar($sql);
					
			}
			
			if($pagamentosrestantes[0]) {
				echo '<tr><td class="SubTituloEsquerda" style="color:red;" colspan="2">Extrato de pagamento/avalia��es - OUTRAS UNIVERSIDADES</td></tr>';
				echo '<tr><td class="SubTituloCentro" width="50%">Parcela</td><td class="SubTituloCentro" width="50%">Situa��o pagamento (Data de atualiza��o)</td></tr>';
					
				foreach($pagamentosrestantes as $pr) {
			
					echo '<tr><td class="SubTituloEsquerda">'.$pr['descricao'].'</td>
									  <td><font size=3><b>'.$pr['statuspagamento'].'</b></font></td></tr>';
					echo '<tr><td colspan="2"><img src="../imagens/mais.gif" style="cursor:pointer;" title="mais" id="img_ava_'.$pr['codigo'].'" onclick="abrirDetalhes(\'ava_'.$pr['codigo'].'\');"> Detalhes da avalia��o</td></td></tr>';
					echo '<tr style="display:none;" id="tr_ava_'.$pr['codigo'].'"><td colspan="2">';
					echo '<p align="center"><b>INFORMA��ES SOBRE AVALIA��ES</b></p>';
						
					$sql = "SELECT * FROM sispacto2.mensario WHERE fpbid='".$pr['codigo']."' AND iusd='".$identificacaousuario['iusd']."'";
					$mensario = $db->pegaLinha($sql);
						
					if($mensario['menid']) consultarDetalhesAvaliacoes(array('menid'=>$mensario['menid']));
					else echo '<p align=center style=color:red;>N�o existem avalia��es nesse per�odo de refer�ncia</p>';
						
					echo '</td></tr>';
					echo '<tr><td colspan="2"><img src="../imagens/mais.gif" style="cursor:pointer;" title="mais" id="img_pag_'.$pr['codigo'].'" onclick="abrirDetalhes(\'pag_'.$pr['codigo'].'\');"> Detalhes do pagamento</td></td></tr>';
					echo '<tr style="display:none;" id="tr_pag_'.$pr['codigo'].'"><td colspan="2">';
					echo '<p align="center"><b>INFORMA��ES SOBRE PAGAMENTO</b></p>';
						
					$sql = "SELECT pboid FROM sispacto2.pagamentobolsista WHERE fpbid='".$pr['codigo']."' AND iusd='".$identificacaousuario['iusd']."'";
					$pboid = $db->pegaUm($sql);
						
					if($pboid) {
						consultarDetalhesPagamento(array('pboid'=>$pboid));
					} else {
						echo "<p align=center style=color:red;>N�o existem pagamentos nesse per�odo de refer�ncia</p>";
			
						$restricao = pegarRestricaoPagamento(array('iusd' => $identificacaousuario['iusd'], 'fpbid' => $pr['codigo']));
			
						echo "<table class=\"listagem\" bgcolor=\"#f5f5f5\" cellSpacing=\"5\" cellPadding=\"10\" align=\"center\">";
						echo "<tr>";
						echo "<td class=\"SubTituloDireita\"><b>Poss�vel restri��o:</b></td>";
						echo "<td><b>".$restricao."</b></td>";
						echo "</tr>";
						echo "</table>";
					}
						
						
					echo '</td></tr>';
			
				}
			
			}
			
			echo '<tr><td class="SubTituloCentro" colspan="2"><input type=button value=Voltar onclick="window.location=\'sispacto2_consulta_pagamento.php\';"></td></tr>';
			echo '</table>';
			echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="5" cellPadding="1" align="center">';
			echo '<tr><td colspan="2" style="font-size:xx-small;"><p>Prezado bolsista, ap�s ser APROVADO no fluxo de avalia��o, o pagamento das bolsas no �mbito do Pacto Nacional pela Alfabetiza��o na Idade Certa obedece ao seguinte fluxo:</p></td></tr>';
			echo '<tr><td class="SubTituloCentro" style="font-size:xx-small;">Status de pagamento</td><td class="SubTituloCentro" style="font-size:xx-small;">Descri��o</td></tr>';
			echo '<tr><td style="font-size:xx-small;">Aguardando autoriza��o IES</td><td style="font-size:xx-small;">O bolsista foi avaliado e considerado apto a receber a bolsa. A libera��o do pagamento est� aguardando autoriza��o final pela Universidade respons�vel pela forma��o.</td></tr>';
			echo '<tr><td style="font-size:xx-small;">Autorizado IES</td><td style="font-size:xx-small;">O pagamento da bolsa foi autorizado pela Universidade e est� sendo processado pelos sistemas do MEC.</td></tr>';
			echo '<tr><td style="font-size:xx-small;">Aguardando autoriza��o SGB</td><td style="font-size:xx-small;">O pagamento da bolsa est� no Sistema de Gest�o de Bolsas, aguardando autoriza��o do MEC.</td></tr>';
			echo '<tr><td style="font-size:xx-small;">Aguardando pagamento</td><td style="font-size:xx-small;">O pagamento da bolsa foi autorizado pelo SGB e est� em processamento.</td></tr>';
			echo '<tr><td style="font-size:xx-small;">Enviado ao Banco</td><td style="font-size:xx-small;">A ordem banc�ria referente ao pagamento da bolsa foi emitida. O pagamento estar� dispon�vel para saque em at� 02 dias �teis, em fun��o do processamento da O.B. pelo banco</td></tr>';
			echo '<tr><td style="font-size:xx-small;">Pagamento efetivado</td><td style="font-size:xx-small;">O pagamento foi creditado em conta e confirmado pelo banco.</td></tr>';
			echo '<tr><td style="font-size:xx-small;">Pagamento n�o autorizado FNDE</td><td style="font-size:xx-small;">O pagamento da bolsa n�o foi autorizado pelo FNDE, pois o bolsista recebe bolsa de outro programa do MEC.</td></tr>';
			echo '<tr><td style="font-size:xx-small;">Pagamento recusado</td><td style="font-size:xx-small;">Pagamento recusado em fun��o de algum erro de registro. Ser� reencaminhado a IES respons�vel pela forma��o.</td></tr>';
			echo '<tr><td colspan="2" style="font-size:xx-small;"><p><b>Observa��o: Caso o seu status no fluxo de pagamento esteja em BRANCO, significa que o m�s de referencia ainda n�o teve o seu fluxo de avalia��o conclu�do. Voc� deve procurar a coordena��o local do PACTO ou a IES respons�vel pela forma��o do seu munic�pio.</b></p></td></tr>';
			echo '</table>';
			
			
		} else {
			$al = array("alert"=>"A universidade do Usu�rio n�o possui per�odo de refer�ncia atribu�do","location"=>"sispacto2_consulta_pagamento.php");
			alertlocation($al);
		}
		
		
	} else {
		$al = array("alert"=>"Usu�rio n�o esta vinculado a nenhuma Universidade","location"=>"sispacto2_consulta_pagamento.php");
		alertlocation($al);
	}
	
else :

?>
<form method="post" id="formulario" name="formulario">
<input type="hidden" name="requisicao" value="consultarPagamento">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
<tr>
	<td class="SubTituloDireita">CPF:</td>
	<td><input type="text" id="cpf" name="cpf" value="" size="20" onkeypress="return controlar_foco_cpf( event );" onkeyup="this.value=mascaraglobal('###.###.###-##',this.value);" /></td>
</tr>
<tr>
	<td class="SubTituloDireita">Data de Nascimento:</td>
	<td><input type="text" id="datanascimento" name="datanascimento" value="" size="12" onkeyup="this.value=mascaraglobal('##/##/####',this.value);" /></td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="button" name="consultar" value="Consultar" onclick="consultarPagamento();"></td>
</tr>
</table>
</form>
<?
endif;
?>
</body>
</html>