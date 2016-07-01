<?php
/*
session_start();
// controle o cache do navegador
header( "Cache-Control: no-store, no-cache, must-revalidate" );
header( "Cache-Control: post-check=0, pre-check=0", false );
header( "Cache-control: private, no-cache" );
header( "Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT" );
header( "Pragma: no-cache" );

// carrega as fun��es gerais
#include_once APPRAIZ . 'includes/workflow.php';

// abre conex�o com o servidor de banco de dados

*/
/*********** Configura��o Geral ***************/
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
$db = new cls_banco();

/*********** Recupera as soma dos quantitativos dos indicadores da �rea 3 por escolas ***************/
$sql = "SELECT 
			sum(qtd.qfaqtd) as quantidade
		FROM cte.dimensao d
			INNER JOIN cte.areadimensao ad ON ad.dimid = d.dimid
			INNER JOIN cte.indicador i ON i.ardid = ad.ardid
			INNER JOIN cte.criterio c ON c.indid = i.indid
			INNER JOIN cte.pontuacao p ON p.crtid = c.crtid AND p.indid = i.indid
			INNER JOIN cte.instrumentounidade iu ON iu.inuid = p.inuid
			INNER JOIN cte.acaoindicador a ON a.ptoid = p.ptoid
			INNER JOIN cte.subacaoindicador s ON s.aciid = a.aciid
			INNER JOIN  cte.qtdfisicoano qtd ON s.sbaid = qtd.sbaid
			LEFT JOIN  cte.subacaoparecertecnico spt ON spt.sbaid = s.sbaid and qtd.qfaano = spt.sptano
		WHERE 	iu.inuid = ".$_SESSION['inuid']." -- o estado desejado.
			AND 	spt.ssuid = 3 -- aprovado pela comiss�o.
			AND  	COALESCE(spt.sptano,0) <> 0 -- ano diferente de 0.
			AND 	s.sbaporescola = true -- por escola.
			AND 	ad.ardcod = 3 -- area 3.
		";
$somaIndArea3PorEscolas = (integer) $db->pegaUm($sql);
unset($sql);

/*********** Recupera as soma dos quantitativos de todos da area 1.3 ***************/

$sql="
	SELECT 
		sum(coalesce(quantidadePorEscola,0) + coalesce(quantidadeGlobal,0)) as valor
	FROM (
		-- Por Escola --
		SELECT 
			0 as quantidadeGlobal,
			sum(qtd.qfaqtd) as quantidadePorEscola
		FROM cte.dimensao d
			INNER JOIN cte.areadimensao ad ON ad.dimid = d.dimid
			INNER JOIN cte.indicador i ON i.ardid = ad.ardid
			INNER JOIN cte.criterio c ON c.indid = i.indid
			INNER JOIN cte.pontuacao p ON p.crtid = c.crtid AND p.indid = i.indid
			INNER JOIN cte.instrumentounidade iu ON iu.inuid = p.inuid
			INNER JOIN cte.acaoindicador a ON a.ptoid = p.ptoid
			INNER JOIN cte.subacaoindicador s ON s.aciid = a.aciid
			INNER JOIN  cte.qtdfisicoano qtd ON s.sbaid = qtd.sbaid
			LEFT JOIN  cte.subacaoparecertecnico spt ON spt.sbaid = s.sbaid and qtd.qfaano = spt.sptano
		WHERE 	iu.inuid = ".$_SESSION['inuid']." -- o estado desejado.
			AND 	spt.ssuid = 3 -- aprovado pela comiss�o.
			AND  	COALESCE(spt.sptano,0) <> 0 -- ano diferente de 0.
			AND 	s.sbaporescola = true -- por escola.
			AND 	d.dimcod = 1  -- indicador.
			AND 	ad.ardcod = 3 -- area 3.
	UNION ALL
		-- Global --
		SELECT 
				sum(spt.sptunt) as quantidadeGlobal,
				0 as quantidadePorEscola
		FROM cte.dimensao d
			INNER JOIN cte.areadimensao ad ON ad.dimid = d.dimid
			INNER JOIN cte.indicador i ON i.ardid = ad.ardid
			INNER JOIN cte.criterio c ON c.indid = i.indid
			INNER JOIN cte.pontuacao p ON p.crtid = c.crtid AND p.indid = i.indid
			INNER JOIN cte.instrumentounidade iu ON iu.inuid = p.inuid
			INNER JOIN cte.acaoindicador a ON a.ptoid = p.ptoid
			INNER JOIN cte.subacaoindicador s ON s.aciid = a.aciid
			INNER JOIN  cte.subacaoparecertecnico spt ON spt.sbaid = s.sbaid
		WHERE 	iu.inuid = ".$_SESSION['inuid']." -- o estado desejado.
			AND 	spt.ssuid = 3 -- aprovado pela comiss�o.
			AND  	COALESCE(spt.sptano,0) <> 0 -- ano diferente de 0.
			AND 	s.sbaporescola = false -- por escola.
			AND 	d.dimcod = 1  -- indicador.
			AND 	ad.ardcod = 3 -- area 3.
	) AS foo
	";
$somaTodosIndArea3 = (integer) $db->pegaUm($sql);
unset($sql);


/***********  Quantidade de escolas Atendidas. ***********/ 
$sql = "SELECT				
				 COUNT( qf.* ) AS soma
				FROM cte.dimensao d
				 INNER JOIN cte.areadimensao ad ON ad.dimid = d.dimid
				 INNER JOIN cte.indicador i ON i.ardid = ad.ardid
				 INNER JOIN cte.criterio c ON c.indid = i.indid
				 INNER JOIN cte.pontuacao p ON p.crtid = c.crtid AND p.indid = i.indid
				 INNER JOIN cte.instrumentounidade iu ON iu.inuid = p.inuid
				 INNER JOIN cte.acaoindicador a ON a.ptoid = p.ptoid
				 INNER JOIN cte.subacaoindicador s ON s.aciid = a.aciid
				 INNER JOIN cte.qtdfisicoano qf ON qf.sbaid = s.sbaid
				WHERE
				 iu.inuid = ".$_SESSION['inuid']."
				 AND iu.itrid = 3";
//echo "<pre>$sql";
$escolaAtendidas = (integer) $db->pegaUm($sql);
unset($sql);

/***********  Seleciona quantitativos da dimens�o 1. ***********/ 
$sql = "SELECT 	dimcod,
			ardcod,
			indcod,
			sum(quantidadePorEscola + quantidadeGlobal) as valor
		FROM (
			SELECT	d.dimcod,
				ad.ardcod,
				i.indcod,
				0 as quantidadeGlobal,
				sum(coalesce(qf.qfaqtd,0)) as quantidadePorEscola
			FROM cte.dimensao d
				INNER JOIN cte.areadimensao ad ON ad.dimid = d.dimid
				INNER JOIN cte.indicador i ON i.ardid = ad.ardid
				INNER JOIN cte.criterio c ON c.indid = i.indid
				INNER JOIN cte.pontuacao p ON p.crtid = c.crtid AND p.indid = i.indid
				INNER JOIN cte.instrumentounidade iu ON iu.inuid = p.inuid
				INNER JOIN cte.acaoindicador a ON a.ptoid = p.ptoid
				INNER JOIN cte.subacaoindicador s ON s.aciid = a.aciid
				INNER JOIN cte.qtdfisicoano qf ON qf.sbaid = s.sbaid 
				LEFT JOIN cte.subacaoparecertecnico spt ON spt.sbaid = s.sbaid AND qf.qfaano = spt.sptano
			WHERE
				 iu.inuid = ".$_SESSION['inuid']."
				 AND iu.itrid = 3			
				 AND d.dimcod IN (1)	
				 AND ad.ardcod IN (3, 2)
				 AND indcod IN (1, 2, 3, 4, 5, 6, 7, 9, 10)
				 AND s.sbaporescola = true -- por escola.
			GROUP BY
				 d.dimcod,
				 ad.ardcod,
				 i.indcod
			UNION ALL
			-- GLOBAL --
			SELECT	d.dimcod,
				ad.ardcod,
				i.indcod,
				sum(coalesce(spt.sptunt,0)) as quantidadeGlobal,
				0 as quantidadePorEscola
			FROM cte.dimensao d
				INNER JOIN cte.areadimensao ad ON ad.dimid = d.dimid
				INNER JOIN cte.indicador i ON i.ardid = ad.ardid
				INNER JOIN cte.criterio c ON c.indid = i.indid
				INNER JOIN cte.pontuacao p ON p.crtid = c.crtid AND p.indid = i.indid
				INNER JOIN cte.instrumentounidade iu ON iu.inuid = p.inuid
				INNER JOIN cte.acaoindicador a ON a.ptoid = p.ptoid
				INNER JOIN cte.subacaoindicador s ON s.aciid = a.aciid
				LEFT JOIN cte.subacaoparecertecnico spt ON spt.sbaid = s.sbaid
			WHERE
				 iu.inuid = ".$_SESSION['inuid']."
				 AND iu.itrid = 3			
				 AND d.dimcod IN (1)	
				 AND ad.ardcod IN (3, 2)
				 AND indcod IN (1, 2, 3, 4, 5, 6, 7, 9, 10)
				 AND s.sbaporescola = false -- global.
			GROUP BY
				 d.dimcod,
				 ad.ardcod,
				 i.indcod
		) AS foo
		GROUP BY 	
			dimcod,
			ardcod,
			indcod
		ORDER BY 
			dimcod,
			ardcod,
			indcod";
//echo "<pre>$sql";
$dados = (array) $db->carregar($sql);

foreach ($dados as $dados):
	$dados['dimcod'] = $dados['dimcod'] ? $dados['dimcod'] : '';
	$dados['ardcod'] = $dados['ardcod'] ? $dados['ardcod'] : '';
	$dados['indcod'] = $dados['indcod'] ? $dados['indcod'] : ''; 
	${"soma_".$dados['dimcod'].$dados['ardcod'].$dados['indcod']} = $dados['valor'];
endforeach;

?>
<html xmlns:o="urn:schemas-microsoft-com:office:office"
xmlns:w="urn:schemas-microsoft-com:office:word"
xmlns="http://www.w3.org/TR/REC-html40">

<head>
<meta http-equiv=Content-Type content="text/html; charset=windows-1252">
<meta name=ProgId content=Word.Document>
<meta name=Generator content="Microsoft Word 9">
<meta name=Originator content="Microsoft Word 9">
<link rel=File-List href="./termocompromisso_arquivos/filelist.xml">
<title>Sobre o Termo de Coopera��o T�cnica</title>
<!--[if gte mso 9]><xml>
 <o:DocumentProperties>
  <o:Author>irineucolombo</o:Author>
  <o:LastAuthor>alexandredourado</o:LastAuthor>
  <o:Revision>2</o:Revision>
  <o:TotalTime>3</o:TotalTime>
  <o:Created>2008-08-18T17:43:00Z</o:Created>
  <o:LastSaved>2008-08-18T17:43:00Z</o:LastSaved>
  <o:Pages>1</o:Pages>
  <o:Words>323</o:Words>
  <o:Characters>1846</o:Characters>
  <o:Lines>15</o:Lines>
  <o:Paragraphs>3</o:Paragraphs>
  <o:CharactersWithSpaces>2267</o:CharactersWithSpaces>
  <o:Version>9.3821</o:Version>
 </o:DocumentProperties>
</xml><![endif]--><!--[if gte mso 9]><xml>
 <w:WordDocument>
  <w:View>Print</w:View>
  <w:HyphenationZone>21</w:HyphenationZone>
 </w:WordDocument>
</xml><![endif]-->
<style>
<!--
 /* Font Definitions */
@font-face
	{font-family:Wingdings;
	panose-1:5 0 0 0 0 0 0 0 0 0;
	mso-font-charset:2;
	mso-generic-font-family:auto;
	mso-font-pitch:variable;
	mso-font-signature:0 268435456 0 0 -2147483648 0;}
 /* Style Definitions */
p.MsoNormal, li.MsoNormal, div.MsoNormal
	{mso-style-parent:"";
	margin:0cm;
	margin-bottom:.0001pt;
	mso-pagination:widow-orphan;
	font-size:12.0pt;
	font-family:"Times New Roman";
	mso-fareast-font-family:"Times New Roman";}
p.MsoBodyText, li.MsoBodyText, div.MsoBodyText
	{margin:0cm;
	margin-bottom:.0001pt;
	mso-pagination:widow-orphan;
	mso-layout-grid-align:none;
	text-autospace:none;
	font-size:10.0pt;
	font-family:"Courier New";
	mso-fareast-font-family:"Times New Roman";}
p.MsoBodyText2, li.MsoBodyText2, div.MsoBodyText2
	{margin:0cm;
	margin-bottom:.0001pt;
	text-align:center;
	mso-pagination:widow-orphan;
	mso-layout-grid-align:none;
	text-autospace:none;
	font-size:12.0pt;
	mso-bidi-font-size:10.0pt;
	font-family:Arial;
	mso-fareast-font-family:"Times New Roman";
	font-style:italic;}
@page Section1
	{size:612.0pt 792.0pt;
	margin:70.85pt 3.0cm 70.85pt 3.0cm;
	mso-header-margin:35.4pt;
	mso-footer-margin:35.4pt;
	mso-paper-source:0;}
div.Section1
	{page:Section1;}
 /* List Definitions */
@list l0
	{mso-list-id:194277592;
	mso-list-type:hybrid;
	mso-list-template-ids:-162921286 68550671 68550681 68550683 68550671 68550681 68550683 68550671 68550681 68550683;}
@list l0:level1
	{mso-level-tab-stop:36.0pt;
	mso-level-number-position:left;
	text-indent:-18.0pt;}
@list l0:level2
	{mso-level-number-format:alpha-lower;
	mso-level-tab-stop:72.0pt;
	mso-level-number-position:left;
	text-indent:-18.0pt;}
@list l1
	{mso-list-id:444497217;
	mso-list-type:hybrid;
	mso-list-template-ids:-889168370 277762984 68550681 68550683 68550671 68550681 68550683 68550671 68550681 68550683;}
@list l1:level1
	{mso-level-text:%1-;
	mso-level-tab-stop:36.0pt;
	mso-level-number-position:left;
	text-indent:-18.0pt;}
@list l2
	{mso-list-id:570121866;
	mso-list-type:hybrid;
	mso-list-template-ids:477369902 480429446 68550681 68550683 68550671 68550681 68550683 68550671 68550681 68550683;}
@list l2:level1
	{mso-level-number-format:alpha-lower;
	mso-level-text:"%1\)";
	mso-level-tab-stop:56.25pt;
	mso-level-number-position:left;
	margin-left:56.25pt;
	text-indent:-21.0pt;}
@list l3
	{mso-list-id:1535457375;
	mso-list-type:hybrid;
	mso-list-template-ids:1323618542 1926628922 68550659 68550661 68550657 68550659 68550661 68550657 68550659 68550661;}
@list l3:level1
	{mso-level-number-format:bullet;
	mso-level-text:-;
	mso-level-tab-stop:72.0pt;
	mso-level-number-position:left;
	margin-left:72.0pt;
	text-indent:-18.0pt;
	font-family:"Times New Roman";
	mso-fareast-font-family:"Times New Roman";}
@list l4
	{mso-list-id:1827815285;
	mso-list-type:hybrid;
	mso-list-template-ids:174096718 68550671 68550681 68550683 68550671 68550681 68550683 68550671 68550681 68550683;}
@list l4:level1
	{mso-level-tab-stop:36.0pt;
	mso-level-number-position:left;
	text-indent:-18.0pt;}
@list l4:level2
	{mso-level-number-format:alpha-lower;
	mso-level-tab-stop:72.0pt;
	mso-level-number-position:left;
	text-indent:-18.0pt;}
ol
	{margin-bottom:0cm;}
ul
	{margin-bottom:0cm;}
-->
</style>
</head>

<body lang=PT-BR style='tab-interval:35.4pt'>

<div class=Section1>

<p class=MsoNormal align=center style='text-align:center;mso-layout-grid-align:
none;text-autospace:none'><b style='mso-bidi-font-weight:normal'><span
style='font-size:11.0pt;font-family:Arial'>Termo de compromisso � Brasil
Profissionalizado<o:p></o:p></span></b></p>

<p class=MsoNormal align=center style='text-align:center;mso-layout-grid-align:
none;text-autospace:none'><span style='font-size:11.0pt;font-family:Arial'><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></span></p>

<p class=MsoBodyText2 style='text-align:justify;text-indent:35.4pt'><span
style='font-size:11.0pt'>Itens que fazem refer�ncia ao PAR-Brasil
Profissionalizado, gerados automaticamente. S�o relevantes com rela��o a
sustentabilidade e adequa��o ao programa, para al�m das cl�usulas regulares dos
conv�nios do FNDE.<o:p></o:p></span></p>

<p class=MsoBodyText2 style='text-align:justify;text-indent:35.4pt'>Estes itens
s�o contrapartidas aos compromissos do MEC estabelecido no <u>Termo de
Coopera��o</u>.<span style='font-size:11.0pt'><o:p></o:p></span></p>

<p class=MsoNormal style='text-align:justify;mso-layout-grid-align:none;
text-autospace:none'><span style='font-size:11.0pt;font-family:Arial'><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></span></p>

<p class=MsoNormal style='text-align:justify;mso-layout-grid-align:none;
text-autospace:none'><b style='mso-bidi-font-weight:normal'><span
style='font-size:11.0pt;font-family:Arial'>COMPROMISSO DO ESTADO<o:p></o:p></span></b></p>

<p class=MsoNormal style='text-align:justify;mso-layout-grid-align:none;
text-autospace:none'><span style='font-size:11.0pt;font-family:Arial'><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></span></p>

<ol style='margin-top:0cm' start=1 type=1>
 <li class=MsoNormal style='text-align:justify;mso-list:l0 level1 lfo3;
     tab-stops:list 36.0pt;mso-layout-grid-align:none;text-autospace:none'><b><span
     style='font-size:11.0pt;font-family:Arial'>GEST�O EDUCACIONAL<o:p></o:p></span></b></li>
 <ol style='margin-top:0cm' start=1 type=a>
  <li class=MsoNormal style='text-align:justify;mso-list:l0 level2 lfo3;
      tab-stops:list 72.0pt;mso-layout-grid-align:none;text-autospace:none'><span
      style='font-size:11.0pt;font-family:Arial'>Ofertar <b><?= $somaIndArea3PorEscolas ? $somaIndArea3PorEscolas : 0 ?></b> vagas de
      Educa��o Profissional e Tecnol�gica nos pr�ximos 04 anos. Com <b><?=  $somaTodosIndArea3 ? $somaTodosIndArea3 : 0 ?> </b>vagas
      de Ensino M�dio Integrado nas diferentes formas, sendo <b><?= ( ($soma_131 ? $soma_131 : 0) + ($soma_132 ? $soma_132 : 0) ) ?></b> de EMI regular, <b><?= ($soma_133 ? $soma_133 : 0) ?></b> EMI ind�gena, <b><?= ($soma_134 ? $soma_134 : 0) ?></b><span style="mso-spacerun: yes">� </span>para Quilombolas,
      Ribeirinhos e comunidades tradicionais, <b><?= ($soma_135 ? $soma_135 : 0) ?></b> para escolas do campo, <b><?= ($soma_136 ? $soma_136 : 0) ?></b> para jovens e adolescentes em conflito com a lei, <b><?= ($soma_137 ? $soma_137 : 0) ?></b> de PROEJA; com <b>(quantitativo da
      unidade de medida �alunos de EaD dos cursos do cat�logo�)</b>de EaD e-Tec
      Brasil; com <b><?= ($soma_139 ? $soma_139 : 0) ?></b> de
      concomitante e com <b><?= ($soma_1310 ? $soma_1310 : 0) ?></b>
      de subseq�ente.<o:p></o:p></span></li>
  <li class=MsoNormal style='text-align:justify;mso-list:l0 level2 lfo3;
      tab-stops:list 72.0pt;mso-layout-grid-align:none;text-autospace:none'><span
      style='font-size:11.0pt;font-family:Arial'>Contratar <b><?= ($soma_124 ? $soma_124 : 0) ?></b> professores para atender a expans�o da EPT nos
      pr�ximos 04 anos<b><o:p></o:p></b></span></li>
  <li class=MsoNormal style='text-align:justify;mso-list:l0 level2 lfo3;
      tab-stops:list 72.0pt;mso-layout-grid-align:none;text-autospace:none'><span
      style='font-size:11.0pt;font-family:Arial'>Construir o Projeto Pol�tico
      Pedag�gico de �X� escolas <b><?= $escolaAtendidas; ?><o:p></o:p></b></span></li>
 </ol>
</ol>

<p class=MsoNormal style='margin-left:54.0pt;text-align:justify;mso-layout-grid-align:
none;text-autospace:none'><span style='font-size:11.0pt;font-family:Arial'><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></span></p>

<p class=MsoNormal style='margin-left:54.0pt;text-align:justify;mso-layout-grid-align:
none;text-autospace:none'><span style='font-size:11.0pt;font-family:Arial'><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></span></p>

<ol style='margin-top:0cm' start=2 type=1>
 <li class=MsoNormal style='text-align:justify;mso-list:l0 level1 lfo3;
     tab-stops:list 36.0pt;mso-layout-grid-align:none;text-autospace:none'><b><span
     style='font-size:11.0pt;font-family:Arial'>FORMA��O DE PROFESSORES E DE
     PROFISSIONAIS DE SERVI�O E APOIO ESCOLAR<o:p></o:p></span></b></li>
 <ol style='margin-top:0cm' start=1 type=a>
  <li class=MsoNormal style='text-align:justify;mso-list:l0 level2 lfo3;
      tab-stops:list 72.0pt;mso-layout-grid-align:none;text-autospace:none'><span
      style='font-size:11.0pt;font-family:Arial'>Proporcionar as condi��es para
      formar professores e profissionais das escolas para atender as demandas
      das Escolas T�cnicas<o:p></o:p></span></li>
 </ol>
</ol>

<p class=MsoNormal style='margin-left:54.0pt;text-align:justify;mso-layout-grid-align:
none;text-autospace:none'><span style='font-size:11.0pt;font-family:Arial'><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></span></p>

<ol style='margin-top:0cm' start=3 type=1>
 <li class=MsoNormal style='text-align:justify;mso-list:l0 level1 lfo3;
     tab-stops:list 36.0pt;mso-layout-grid-align:none;text-autospace:none'><b><span
     style='font-size:11.0pt;font-family:Arial'>PR�TICAS PEDAG�GICAS E
     AVALIA��O<o:p></o:p></span></b></li>
 <ol style='margin-top:0cm' start=1 type=a>
  <li class=MsoNormal style='text-align:justify;mso-list:l0 level2 lfo3;
      tab-stops:list 72.0pt;mso-layout-grid-align:none;text-autospace:none'><span
      style='font-size:11.0pt;font-family:Arial'>Adequar os cursos t�cnicos ao
      Cat�logo Nacional de Cursos T�cnicos.<o:p></o:p></span></li>
  <li class=MsoNormal style='text-align:justify;mso-list:l0 level2 lfo3;
      tab-stops:list 72.0pt;mso-layout-grid-align:none;text-autospace:none'><span
      style='font-size:11.0pt;font-family:Arial'>Proporcionar condi��es e
      estimular para que todas as escolas de Ensino M�dio tenham Est�gio
      Curricular supervisionado.<o:p></o:p></span></li>
  <li class=MsoNormal style='text-align:justify;mso-list:l0 level2 lfo3;
      tab-stops:list 72.0pt;mso-layout-grid-align:none;text-autospace:none'><span
      style='font-size:11.0pt;font-family:Arial'>Fomentar a cria��o de
      programas de inicia��o cient�fica <b>(se houver suba��o)</b><o:p></o:p></span></li>
 </ol>
</ol>

<p class=MsoNormal style='text-align:justify;mso-layout-grid-align:none;
text-autospace:none'><span style='font-size:11.0pt;font-family:Arial'><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></span></p>

<ol style='margin-top:0cm' start=4 type=1>
 <li class=MsoNormal style='text-align:justify;mso-list:l0 level1 lfo3;
     tab-stops:list 36.0pt;mso-layout-grid-align:none;text-autospace:none'><b><span
     style='font-size:11.0pt;font-family:Arial'>INFRA-ESTRUTURA F�SICA E RECURSOS<span
     style="mso-spacerun: yes">� </span>PEDAG�GICOS<o:p></o:p></span></b></li>
 <ol style='margin-top:0cm' start=1 type=a>
  <li class=MsoNormal style='text-align:justify;mso-list:l0 level2 lfo3;
      tab-stops:list 72.0pt;mso-layout-grid-align:none;text-autospace:none'><span
      style='font-size:11.0pt;font-family:Arial'>Equipar, manter e dar
      funcionalidade aos laborat�rios cient�ficos em todas as escolas<o:p></o:p></span></li>
  <li class=MsoNormal style='text-align:justify;mso-list:l0 level2 lfo3;
      tab-stops:list 72.0pt;mso-layout-grid-align:none;text-autospace:none'><span
      style='font-size:11.0pt;font-family:Arial'>Equipar, manter e dar
      funcionalidade aos laborat�rios tecnol�gicos nas Escolas T�cnicas</span><span
      style='font-size:11.0pt'><o:p></o:p></span></li>
 </ol>
</ol>

</div>

</body>

</html>
