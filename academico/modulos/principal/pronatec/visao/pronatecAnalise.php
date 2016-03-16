<?php

$preid = ($_SESSION['par']['preid']) ? $_SESSION['par']['preid'] : $_REQUEST['preid'];

$preidTx = $_SESSION['par']['preid'] ? '&preid='.$_SESSION['par']['preid'] : '';
$lnkabas = "academico.php?modulo=principal/pronatec/popupPronatec&acao=A&tipoAba=Analise".$preidTx;

echo carregaAbasPronatec($lnkabas);
monta_titulo( 'Analise', ''  );

$oPreObra = new PreObra();
$oSubacaoControle = new SubacaoControle();
$pacFNDE  = $oSubacaoControle->verificaObraFNDE($preid, 23);
$arDados  = $oSubacaoControle->recuperarPreObra($preid);

if($preid){	
	
	$qrpid = pegaQrpidPAC( $preid, 43 );	
	
	$pacDados = $oSubacaoControle->verificaTipoObra($preid, 23);	
	$pacFotos = $oSubacaoControle->verificaFotosObra($preid, 23);
	$pacDocumentos = $oSubacaoControle->verificaDocumentosObra($preid, 23, $pacDados);
	$pacQuestionario = $oPreObra->verificaQuestionario($qrpid);	
}

$docid = prePegarDocid($preid);
$esdid = prePegarEstadoAtual($docid);	

$arPendencias = array('Dados do im�vel' => 'Falta o preenchimento dos dados.',
					  'Caracteristicas do im�vel' => 'Falta o preenchimento dos dados.',
					  'Cadastro de fotos do im�vel' => 'Deve conter no m�nimo 3 fotos do terreno.',
					  'Documentos anexos' => 'Falta anexar os arquivos.');

?>
<?php echo cabecalho();?>
<table class="tabela" align="center">
<?php 
	$x=0; 
	foreach($arPendencias as $k => $v){ 
		$cor = ($x % 2) ? 'white' : '#d9d9d9;'; 
		if(  ( !$pacDados && $k == 'Dados do im�vel' ) || 
			 ( $k == 'Caracteristicas do im�vel' && $pacQuestionario != 22 ) || 
			 ( $pacFotos < 3 && $k == 'Cadastro de fotos do im�vel' ) ||
			 ( ($pacDocumentos['arqid'] != $pacDocumentos['podid'] || !$pacDocumentos) && $k == 'Documentos anexos' ) 
			 ){
			if(!$boMsg){
?>
	<tr>
		<td colspan="3" style="text-align:center;font-size:14px;font-weight:bold;color:#900;height:50px;">
			O sistema verificou que alguns dados n�o foram preenchidos:
		</td>
	</tr>
<?php 
				$boMsg = true;
			} 
?>
	<tr style="background-color: <?php echo $cor ?>;">
		<td>
<?php 
			switch($k){
				case 'Dados do im�vel':
					$aba = 'dados';
					break;
				case 'Caracter�sticas do im�vel':
					$aba = 'questionario';
					break;
				case 'Cadastro de fotos do im�vel':
					$aba = 'foto';
					break;
				case 'Documentos anexos':
					$aba = 'documento';
					break;
				default:
					$aba = "dados";
					break;
			}
?>
			<a href="academico.php?modulo=principal/pronatec/popupPronatec&acao=A&tipoAba=<?php echo $aba ?>&preid=<?php echo $preid ?>">
			<img border="0" src='/imagens/consultar.gif' onclick='javascript:void(0)'>
			</a>
		</td>
		<td>
			<b><?php echo $k ?></b>
			<br />
			&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - 
			<?php echo $v ?>
			<br />
		</td>
		<td style="background:white;width:100px;"></td>
	</tr>
<?php 
			$x++;
		}
	}
	if(!$boMsg){ 
?>
	<tr>
		<td colspan="3" style="text-align:center;font-size:14px;font-weight:bold;color:#900;height:50px;">
			O sistema n�o encontrou pend�ncias. 
		</td>
	</tr>
<?php 
	} 
?>
	<tr>
		<td colspan="3" height="170px;"></td>
	</tr>		
</table>