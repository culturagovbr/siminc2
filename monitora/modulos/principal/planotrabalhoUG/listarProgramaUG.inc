<?php
require_once APPRAIZ . "www/planacomorc/_funcoes.php";
require_once APPRAIZ . "www/planacomorc/_constantes.php";

if (isAjax() && isset($_POST['prgcod']) && isset($_POST['comboacao'])) {
    
    $sql = "SELECT ac.acacod as codigo, (ac.acacod || ' - ' || ac.acadsc) as descricao
            FROM planejamento.acao ac 
            WHERE 
                ac.unicod IS NOT NULL AND 
                ac.prgano = '".$_SESSION['exercicio']."' AND 
                ac.acastatus = 'A' AND 
                ac.acasnrap = false AND
                ac.prgcod = '".(int)$_POST['prgcod']."'
            GROUP BY ac.acacod, ac.acadsc 
            ORDER BY ac.acacod";
    $db->monta_combo('acacod', $sql, 'S', 'Selecione','','','',400); 
    exit;
}

// verificando se é undidade ou unidade gestora
$where .= $_REQUEST['sbaid'] ? "AND dt.ptrid IN (SELECT ptrid FROM monitora.pi_subacaodotacao WHERE sbaid = ".$_REQUEST['sbaid'].")" : '';
$where .= $_POST['prgcod'] ? "AND UPPER(dtl.prgcod) LIKE('%".strtoupper($_POST['prgcod'])."%')" : '';
$where .= $_POST['acacod'] ? "AND UPPER(dtl.acacod) LIKE('%".strtoupper($_POST['acacod'])."%')" : '';
$where .= $_POST['buscalivre'] ? "AND (trim(aca.prgcod||'.'||aca.acacod||'.'||aca.loccod||' - '||aca.acadsc) ilike('%".$_POST['buscalivre']."%') OR dtl.ptres ilike '%".$_POST['buscalivre']."%')" : '';

// -- Query utilizada também em: simec/www/planacomorc/_funcoespi.php

/* Parametros para montar a consulta */
$params['where'] = $where;

$params['SELECT'] = <<<SQL
SELECT '<input type=\"checkbox\" id=\"chk_'||dtl.ptres||'\" onclick=\"resultado(this,\''||dtl.ptrid||'\',\''||dtl.ptres||'\');\">' as checkbox,
                         '<div class=\"linkSubacao\" onclick=\"detalhePtres(\''|| dtl.ptrid ||'\');\">'|| dtl.ptres ||'</div>'as sbacod,
                         trim(aca.prgcod || '.' || aca.acacod || '.' || aca.unicod || '.' || aca.loccod || ' - ' || aca.acatitulo) AS descricao,
                         uni.unidsc,
                         COALESCE(SUM(dtl.ptrdotacao)+0.00, 0.00) AS dotacaoinicial,
                         COALESCE(SUM(dt.valor), 0.00) AS det_subacao,
                         COALESCE(SUM(dt2.valorpi), 0.00) AS det_pi,
                         COALESCE((pemp.total), 0.00) AS empenhado,
                         COALESCE(SUM(dtl.ptrdotacao),0.00) - COALESCE(pemp.total, 0.00) AS saldo
SQL;
$sql_lista = retornaConsultaPTRES($params);
#ver($sql_lista);
?>
<html>
<head>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.9.1/jquery-1.9.1.js"></script>
    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
    <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
</head>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<?php monta_titulo($entnome, '&nbsp'); ?>
<form action="" method="post" name="formulario">

<table width="100%" class="tabela" bgcolor="#f5f5f5" border="0" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloDireita" align="right">Programa:</td>
	<td>
	<?
	$sql = "SELECT p.prgcod as codigo, (p.prgcod || ' - ' || p.prgdsc) as descricao
			FROM planejamento.programa p
			WHERE p.prgano = '".$_SESSION['exercicio']."' ORDER BY prgcod";
	$db->monta_combo('prgcod', $sql, 'S','Selecione','carregaAcao','','',400); 
	?>
	</td>
</tr>	
<tr>
	<td class="SubTituloDireita" align="right">Ação:</td>
	<td class="comboAcao">
	<?
	$sql = "SELECT ac.acacod as codigo, (ac.acacod || ' - ' || ac.acadsc) as descricao
 			FROM planejamento.acao ac 
 			WHERE ac.unicod IS NOT NULL AND ac.prgano = '".$_SESSION['exercicio']."' AND ac.acastatus = 'A' AND ac.acasnrap = false  
 			GROUP BY ac.acacod, ac.acadsc 
 			ORDER BY ac.acacod";
	$db->monta_combo('acacod', $sql, 'S', 'Selecione','','','',400); 
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" align="right">Buscar:</td>
	<td>
	<? echo campo_texto('buscalivre', "N", "S", "", 67, 150, "", "", '', '', 0, '' ); ?>
	</td>
</tr>
<tr style="background-color: #cccccc">
	<td align='right' style="vertical-align:top; width:25%;">&nbsp;</td>
	<td>
	<input type="submit" name="botao" value="Pesquisar"/>
	<!--<input type="button" name="botao" value="Todos" onclick="window.location='?modulo=principal/planotrabalhoUG/listarProgramaUG&acao=A';"/>-->
	<input type="button" name="close" value="Fechar" onclick="window.close();">	
	</td>
</tr>		
</table>

</form>
<br>
<?php
$cabecalho = array( 
    " ",
    "PTRES",
    "Ação",
    "Unidade Orçamentária",
    "Dotação Atual(R$)",
    "Detalhado em<br />Subação (R$)",
    "Detalhado em<br />PI (R$)",
    "Empenhado&nbsp;(R$)",
    "Não Empenhado (R$)"
);

$db->monta_lista($sql_lista, $cabecalho ,60,20,'','','');

?>
<script type="text/javascript">

function carregaAcao(prgcod) {
    jQuery.ajax({
        url: 'monitora.php?modulo=principal/planotrabalhoUG/listarProgramaUG&acao=A'
      , type: 'POST'
      , data: {prgcod:prgcod,comboacao:true}
      , success:function(data){
          $(".comboAcao").html(data);
      }
    });
}

/* CARREGANDO OS DADOS DE PTRES */
var tabelaorigem = window.opener.document.getElementById('orcamento');
for(i=2;i<tabelaorigem.rows.length-2;i++) {
	if(document.getElementById("chk_"+tabelaorigem.rows[i].cells[0].innerHTML)) {
		document.getElementById("chk_"+tabelaorigem.rows[i].cells[0].innerHTML).checked=true;
	}
}
/* FIM CARREGANDO OS DADOS DE PTRES */

function resultado(dados, acaid, ptres){

	if(!ptres) {
		alert('Não existe PTRES. Entre em contato com o administrador do sistema.');
		return false;
	}


	if(dados.checked) {

	var linhaTbl = dados.parentNode.parentNode;
	var tabelaorigem = window.opener.document.getElementById('orcamento');
	if(eval(tabelaorigem.rows.length%2)) {
		var cor = "";
	} else {
		var cor = "#DCDCDC";
	}
	var linha = tabelaorigem.insertRow(2);
	linha.id = "ptres_"+ptres;
	linha.style.backgroundColor = cor;
	linha.style.height = '30px';
	
	// setando o ptres
	var celula1 = tabelaorigem.rows[2].insertCell(0);
	celula1.style.textAlign = "center";
	celula1.innerHTML = ptres;
	
	var celula2 = tabelaorigem.rows[2].insertCell(1);
	celula2.style.textAlign = "left";
	celula2.innerHTML = linhaTbl.cells[2].innerHTML+"<input type='hidden' name='acaid["+ptres+"]' value='"+acaid+"'>";

	var celula3 = tabelaorigem.rows[2].insertCell(2);
	celula3.style.textAlign = "right";
	celula3.innerHTML = linhaTbl.cells[4].innerHTML;
	
	var celula4 = tabelaorigem.rows[2].insertCell(3);
	celula4.style.textAlign = "right";
	celula4.innerHTML = linhaTbl.cells[5].innerHTML;
	
	var celula5 = tabelaorigem.rows[2].insertCell(4);
	celula5.style.textAlign = "right";
	celula5.innerHTML = "<a href=javascript:detfin('"+ptres+"')>"+linhaTbl.cells[6].innerHTML+"</a>";
	
	var celula6 = tabelaorigem.rows[2].insertCell(5);
	celula6.style.textAlign = "right";
	celula6.innerHTML = linhaTbl.cells[8].innerHTML;

	var celula7 = tabelaorigem.rows[2].insertCell(6);
	celula7.style.textAlign = "center";
	celula7.innerHTML = "<input type=\"text\" name=\"plivalor["+ptres+"]["+acaid+"]\" size=\"28\" maxlength=\"\" value=\"\" onKeyUp=\"this.value=mascaraglobal('###.###.###.###,##',this.value);calculovalorPI();\" onBlur=\"return verificaDisponivel(this,'"+ptres+"','0');\"  class=\"normal\"  onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" style=\"text-align : right; width:25ex;\" title='' />";



	} else {

	var tabelaorigem = window.opener.document.getElementById('orcamento');
	tabelaorigem.deleteRow(window.opener.document.getElementById('ptres_'+ptres).rowIndex);
	window.opener.calculovalorPI();

	}

}
</script>
</body>
</html>

