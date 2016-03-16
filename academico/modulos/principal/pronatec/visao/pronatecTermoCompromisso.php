<?php 

include_once APPRAIZ.'includes/classes/RequestHttp.class.inc';

function pegaMes2()
{
	$mes = date('m');
	
	switch ($mes)
	{
		case 1: $mes = "JANEIRO"; break;
		case 2: $mes = "FEVEREIRO"; break;
		case 3: $mes = "MAR�O"; break;
		case 4: $mes = "ABRIL"; break;
		case 5: $mes = "MAIO"; break;
		case 6: $mes = "JUNHO"; break;
		case 7: $mes = "JULHO"; break;
		case 8: $mes = "AGOSTO"; break;
		case 9: $mes = "SETEMBRO"; break;
		case 10: $mes = "OUTUBRO"; break;
		case 11: $mes = "NOVEMBRO"; break;
		case 12: $mes = "DEZEMBRO"; break;
	}
	
	return $mes;
}

if( $_REQUEST['geraPDF'] )
{
	$obMunicipio	= new Municipio();
	$municipio		= $obMunicipio->descricaoMunicipio($_SESSION['par']['muncod'], false);
	
	$sql = "SELECT
				count(tprcampus)
			FROM
				par.termopronatec
			WHERE
				muncod = '".trim($_SESSION['par']['muncod'])."'";
	
	$qtd = $db->pegaUm($sql);
	
	$sql = "SELECT 
				entprefeito.entnome
            FROM 
            	entidade.entidade entprefeito
           	INNER JOIN 
           		entidade.funcaoentidade funprefeito ON entprefeito.entid = funprefeito.entid
            INNER JOIN 
            	entidade.funentassoc feaprefeito ON feaprefeito.fueid = funprefeito.fueid
            INNER JOIN 
            	entidade.entidade entprefeitura ON entprefeitura.entid = feaprefeito.entid
            INNER JOIN 
            	entidade.funcaoentidade funprefeitura ON funprefeitura.entid = entprefeitura.entid
            INNER JOIN 
            	entidade.endereco entd ON entd.entid = entprefeitura.entid
            INNER JOIN 
            	territorios.municipio mun ON entd.muncod = mun.muncod
            WHERE 
            	funprefeito.funid = 2 AND 
            	--funprefeitura.funid = 1 AND 
            	mun.muncod ='".$_SESSION['par']['muncod']."'";
	$prefeito = $db->pegaUm($sql);
	
	$sql = "SELECT
				tprcampus
			FROM
				par.termopronatec
			WHERE
				muncod = '".trim($_SESSION['par']['muncod'])."'";
	$instituto = $db->pegaUm($sql);
	
	if( $_SESSION['par']['muncod'] != '5300108' )
	{
		$pref =	'<br />
			 	'.$prefeito.'
			 	<br /><br />
			 	Prefeito de '.$municipio.'';
	}
	
	$html = '<center><font size="14px"><b>TERMO DE COMPROMISSO</b></font></center>
			 <br /><br />
			 <p>
			 Com a finalidade de ser credenciado como munic�pio sede para implanta��o de '.$qtd.' campus '.$municipio.' do Instituto Federal de Educa��o Profissional, Ci�ncia e Tecnologia '.ucwords(strtolower($instituto)).', na qualidade de representante legal, devidamente autorizado, firmo em nome  do munic�pio de '.$municipio.' o compromisso  de promover a transfer�ncia dominial, no prazo m�ximo de 150 dias, a partir da assinatura deste termo,  devidamente legalizado, de �rea de terra ou infraestrutura  f�sica edificada  em conson�ncia com os requisitos listados  neste documento e aprovado pelo Instituto Federal  com vistas � instala��o  do campus.
			 <br /><br />
			 Ciente de que o n�o cumprimento da transfer�ncia do im�vel para propriedade do ente federado at� a data prevista, autoriza a institui��o respons�vel pela implanta��o da unidade a buscar munic�pio alternativo  para execu��o do pleito.
			 <br /><br />
			 <center>
			 '.$municipio.', '.date('d').' de '.ucwords(strtolower(pegaMes2())).' de 2011
			 <br /><br /><br /><br />
			 __________________________________________
			 '.$pref.'
			 </center>
			 </p>';
	
	$trans = get_html_translation_table(HTML_ENTITIES);
	$html = strtr($html, $trans);
	$html = str_replace("&lt;", "<", $html);
	$html = str_replace("&gt;", ">", $html);
	
	$http = new RequestHttp();
	$http->toPdfDownload($html, 'termo_compromisso');
	die;
}

$preidTx = $_SESSION['par']['preid'] ? '&preid='.$_SESSION['par']['preid'] : '';
$lnkabas = "par.php?modulo=principal/programas/pronatec/popupPronatec&acao=A&tipoAba=TermoCompromisso".$preidTx;

echo carregaAbasPronatec($lnkabas);
monta_titulo( 'Termo de Compromisso', ''  );

?>

<script type="text/javascript">

$(document).ready(function()
{
	$('#btTermo').click(function()
	{
		$("#formTermo").submit();
	});
	
	jQuery('.navegar').click(function(){
	
		var preid = '<?php echo ($_REQUEST['preid']) ?  $_REQUEST['preid'] : 'nulo'?>';

		if(this.value == 'Pr�ximo'){
			aba = 'Dados';
		}
	
		if(preid != 'nulo'){
			preid = '&preid='+preid;
		}else{
			preid = '';
		}
	
		document.location.href = 'par.php?modulo=principal/programas/pronatec/popupPronatec&acao=A&tipoAba='+aba+preid;
	});
});



</script>
<form id="formTermo" method="post">
<input name="geraPDF" type="hidden" value="1" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td>
			<center>
				<br />
				Clique no bot�o abaixo para visualizar ou fazer download do Termo:
				<br /><br />
				<input type="button" value="Gerar Termo de Compromisso" id="btTermo" />
				<br /><br />
			</center>
			</td>
		</tr>
	</table>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr bgcolor="#dcdcdc">
			<td style="text-align: center">
				<table width="100%">
					<tr>
						<td align="left"></td>
						<td align="center">
						<?php if( $boAtivo == 'S' ){ ?>
							<input class="enviar" type="submit" value="Salvar" /> 
						<?php } ?>
						</td>
						<td align="right">
							<input class="navegar" type="button" value="Pr�ximo" />
						</td>
					</tr>
				</table>
			</td>
		</tr>
	</table>
</form>