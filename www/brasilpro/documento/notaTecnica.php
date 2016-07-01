<?php


if( !isset( $boGerarNotaTecnica ) ){
	include_once "config.inc";
	include_once APPRAIZ . "includes/funcoes.inc";
	include_once APPRAIZ . "includes/classes_simec.inc";
	$db = new cls_banco();
}		

$inuid = $_SESSION["inuid"];

$sql = sprintf("SELECT estdescricao 
				FROM cte.instrumentounidade ins inner join territorios.estado est on ins.estuf = est.estuf 
				WHERE inuid = %d", $inuid);

$estDsc = $db->pegaUm( $sql );

$sql = "select parid from cte.parecer where inuid = ".$inuid;
$numeroParecer = $db->pegaUm($sql);
unset($sql);

$sql = "SELECT 	
			dimcod,
			ardcod,
			indcod,
			valorGlobal + valorPorEscola as valor,
			descricaoSubacao
			--sum(valorGlobal + valorPorEscola) as soma
			-- Global
		FROM (
				SELECT	
					d.dimcod,
					ad.ardcod,
					i.indcod,
					s.sbaid AS idSubacao,
					s.sbadsc AS descricaoSubacao,
					sum(cosvlruni * cosqtd) AS valorGlobal,
					0 as valorPorEscola
					
				FROM cte.dimensao d
					INNER JOIN cte.areadimensao ad ON ad.dimid = d.dimid
					INNER JOIN cte.indicador i ON i.ardid = ad.ardid
					INNER JOIN cte.criterio c ON c.indid = i.indid
					INNER JOIN cte.pontuacao p ON p.crtid = c.crtid AND p.indid = i.indid
					INNER JOIN cte.instrumentounidade iu ON iu.inuid = p.inuid
					INNER JOIN cte.acaoindicador a ON a.ptoid = p.ptoid
					INNER JOIN cte.subacaoindicador s ON s.aciid = a.aciid	
					INNER JOIN cte.composicaosubacao csa ON csa.sbaid = s.sbaid AND cosano = 2008 -- AND cosano = date_part('year', current_date)
					LEFT JOIN cte.subacaoparecertecnico spt ON spt.sbaid = s.sbaid  AND sptano = 2008 -- AND sptano = date_part('year', current_date)
					
				WHERE
					 iu.inuid = $inuid	
					 AND s.sbaporescola = false -- global.
					 AND spt.ssuid in ( 3, 7 )
					 AND s.frmid in(16,17)
				GROUP BY
					 d.dimcod,
					 ad.ardcod,
					 i.indcod,
					 s.sbaid,
					 s.sbadsc
			UNION ALL
				-- PorEscola
				SELECT	
					d.dimcod,
					ad.ardcod,
					i.indcod,
					s.sbaid AS idSubacao, 
					s.sbadsc AS descricaoSubacao,
					0 AS valorGlobal,
					sum(cosvlruni * cosqtd) AS valorPorEscola	
				FROM cte.dimensao d
					INNER JOIN cte.areadimensao ad ON ad.dimid = d.dimid
					INNER JOIN cte.indicador i ON i.ardid = ad.ardid
					INNER JOIN cte.criterio c ON c.indid = i.indid
					INNER JOIN cte.pontuacao p ON p.crtid = c.crtid AND p.indid = i.indid
					INNER JOIN cte.instrumentounidade iu ON iu.inuid = p.inuid
					INNER JOIN cte.acaoindicador a ON a.ptoid = p.ptoid
					INNER JOIN cte.subacaoindicador s ON s.aciid = a.aciid	
					INNER JOIN cte.composicaosubacao csa ON csa.sbaid = s.sbaid  AND cosano = 2008 -- AND cosano = date_part('year', current_date)
					LEFT JOIN (SELECT cosid, SUM(ecsqtd) as total FROM cte.escolacomposicaosubacao GROUP BY cosid) ecs ON csa.cosid = ecs.cosid
					LEFT JOIN cte.subacaoparecertecnico spt ON spt.sbaid = s.sbaid  AND sptano = 2008 -- AND sptano = date_part('year', current_date)
					
				WHERE
					 iu.inuid = $inuid	
					 AND s.sbaporescola = true -- por escola.
					 AND spt.ssuid in ( 3, 7 )
					 AND s.frmid in(16,17)
				GROUP BY
					 d.dimcod,
					 ad.ardcod,
					 i.indcod,
					 s.sbaid,
					 s.sbadsc
		) AS resultado
		ORDER BY 
				dimcod, 
				ardcod, 
				indcod";
				
$res = $db->carregar( $sql ) ? $db->carregar( $sql ) : array();

// n�o h� controle do locale do servidor...
$mes = array(
	"Janeiro",
	"Fevereiro",
	"Mar�o",
	"Abril",
	"Maio",
	"Junho",
	"Julho",
	"Agosto",
	"Setembro",
	"Outubro",
	"Novembro",
	"Dezembro"
);



$parecer_data      = date( "d" ) . " de " . $mes[date( "m" )-1] . " de " . date( "Y" );

$arConcordancia["da"] = array( "Bahia", "Para�ba" );
$arConcordancia["de"] = array( "Alagoas", "Minas Gerais", "Pernambuco", "Rond�nia", "Roraima", "Santa Catarina", "S�o Paulo", "Sergipe", "Tocantins" );

if( in_array( $estDsc, $arConcordancia["da"] ) ){
	$estCompleto = "<span class='destaque'>Estado da $estDsc</span>";
}
elseif( in_array( $estDsc, $arConcordancia["de"] ) ){
	$estCompleto = "<span class='destaque'>Estado de $estDsc</span>";
}
else{
	$estCompleto = "<span class='destaque'>Estado do $estDsc</span>";
}

$interessadoAtual = "Secret�ria de Educa��o do $estCompleto";

?>
<html xmlns="http://www.w3.org/1999/xhtml">
	<head>
		<meta http-equiv="content-type" content="text/html; charset=utf-8" />
		<title></title>
		<style type="text/css">

			* { margin: 0; font-family: Arial; font-size: 11pt; }
			
			@page { size: 8.5inch 11inch; margin-top: 0.5inch; margin-bottom: 0.7874inch; margin-left: 1.1811inch; margin-right: 0.3291inch }
			
			table { border-collapse: collapse; border-spacing: 0; empty-cells: show; border-color: #fff; }
			
			.cabecalho{ text-align: center; margin-bottom: 30px;  }
			
			.cabecalho .orgao{ font-weight: bold; }
			
			.cabecalho .setor{ font-weight: bold; font-style: italic; }
			
			.paragrafoNormal{ text-indent: 40px; margin-top:0.494cm; margin-bottom:0.494cm; text-align:justify ! important; }
			
			.divAssinatura{ text-align: center; }
			
			.divTabelaResultado{ margin: 30px 3%; }
			
			.divTabelaResultado td{ padding: 3px; }
			
			.tabelaCabecalho{ text-align: center; background: #ccc; font-weight: bold; }
			
			.divData{ text-align: right; margin: 20px 0 30 0; }
			
			.linhaAssinatura{ margin-bottom: 10px; }
			
			.divAssunto{ margin-bottom: 30px; }
			
			.destaque{ font-weight: bold; }
			
		</style>
	</head>
	
	<body dir="ltr">
		<div style="margin: 10px;">
			
			<div class="cabecalho">
				<img width="80" height="80" src="/imagens/brasao.gif"/>
				<p class="orgao">Minist�rio da Educa��o�</p>
				<p class="setor">Secretaria de Educa��o Profissional e Tecnol�gica</p>
				<p>Diretoria de Articula��o e Projetos Especiais</p>
			</div>
			
			<div class="divAssunto">
				<p>NOTA T�CNICA N.� #NOTATECNICA#</p>
				<p>
					INTERESSADO: <span class="destaque"><?php echo $interessadoAtual; ?></span>
				</p>
				<p>ASSUNTO: <span class="destaque">Plano de A��es Articuladas/Brasil Profissionalizado</span></p>
			<div>
						
			<p class="paragrafoNormal">
				O Governo Federal, por interm�dio do Decreto 6.302, de dezembro de 2007, publicado no D.O.U no dia 12 de dezembro de 2007, disp�e sobre a implementa��o do "PAR/Brasil Profissionalizado", assim, definido em seu Art. 1. "Fica institu�do, no �mbito do Minist�rio da Educa��o, o Programa Brasil Profissionalizado, com vistas a estimular o Ensino M�dio Integrado � educa��o profissional, enfatizando a educa��o cient�fica e human�stica, por meio da articula��o entre forma��o geral e educa��o profissional no contexto dos arranjos produtivos e das voca��es locais e regionais". 
			</p>
			<p class="paragrafoNormal">
				Considerando os princ�pios b�sicos do Plano de Desenvolvimento da Educa��o - PDE - educa��o sist�mica, ordena��o territorial e desenvolvimento, com foco nos prop�sitos de melhoria da qualidade da educa��o no Ensino M�dio e na redu��o de desigualdades relativas �s oportunidades educacionais, o Minist�rio da Educa��o construiu 64 indicadores, distribu�dos em 04 (quatro) Dimens�es (Gest�o Educacional; Forma��o de Professores e Profissionais de Servi�o e Apoio Escolar; Pr�ticas Pedag�gicas e Avalia��o; e, Infra-estrutura F�sica e Recursos Pedag�gicos) que nortearam o <?= $estCompleto; ?> na realiza��o do diagn�stico da Educa��o B�sica no Ensino M�dio e na Educa��o Profissional e Tecnol�gica do sistema local. 
			</p>
			<p class="paragrafoNormal">
				A partir desse diagn�stico, o <?= $estCompleto; ?> elaborou o PAR/Brasil Profissionalizado que visa � expans�o do Ensino M�dio Integrado a Educa��o Profissional e Tecnol�gica no �mbito estadual.
			</p>
			<p class="paragrafoNormal">
				Algumas solicita��es do PAR / Brasil Profissionalizado, em quest�o, s�o bastante significativas e merecem destaque, a saber:
			</p>
			
			<div class="divTabelaResultado">
				
				<table border="1" width="95%">
					<?php if( count( $res ) ){ ?>
						<tr class="tabelaCabecalho">
							<td width="5%">Indicadores</td>
							<td width="75%">Descri��o</td>
							<td width="20%">Valor</td>
						</tr>
						 
						<?php
						$cor = "#eee"; 
						foreach( $res as $arSubacao ){ 
							$cor = $cor == "#fff" ? "#eee" : "#fff";?>
							
							<tr style="background: <?php echo $cor ?>">
								<td align="center"><?php echo $arSubacao["dimcod"].".".$arSubacao["ardcod"].".".$arSubacao["indcod"] ?></td>
								<td><?php echo $arSubacao["descricaosubacao"] ?></td>
								<td align="right">R$ <?php  echo number_format( $arSubacao["valor"], 2, ',', '.' ) ?></td>
							</tr>
						<?php } ?>
					<?php }
					else{ ?>
						<tr class="tabelaCabecalho">
							<td>N�o foram encontrados dados.</td>
						<tr>
					<?php } ?>
				</table>
			</div>
			
			<p class="paragrafoNormal">
				Diante do exposto, o Plano de A��es Articuladas / Brasil Profissionalizado do <?= $estCompleto; ?> se enquadra no compromisso assumido pelo MEC e atende �s expectativas de cumprimento ao que prev� a legisla��o da Educa��o Profissional. Sendo assim, sou de parecer  favor�vel ao apoio financeiro e de forma  integral.
			</p>
			
			<br />
			
			<div class="divData">
				<p>Bras�lia, <?= $parecer_data ?></p>
			</div>
			
			<div class="divAssinatura">
				<p class="linhaAssinatura">_______________________________</p>
				<p>Marcelo Camilo Pedra</p>
				<p>Coordenador Geral</p>
				<p>Coordena��o Geral de Projetos Especiais</p>
			</div>
			
			<div class="divAssinatura">
				<p class="linhaAssinatura">_______________________________</p>
				<p>Gleisson Cardoso Rubin</p>
				<p>Diretor</p>
				<p>Diretoria de Articula��o e Projetos Especiais</p>
			</div>
			
			<p class="paragrafoNormal" style="margin: 40px 0;">
				De acordo com o despacho supra, encaminhe-se o presente processo � DIPRO- FNDE para os tr�mites legais. 
			</p>
			
		</div>	
	</body>
</html>