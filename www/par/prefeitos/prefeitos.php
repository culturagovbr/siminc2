<?php
 
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

session_start();
 
// CPF do administrador de sistemas
if( !$_SESSION['usucpf'] ){
	$_SESSION['usucpforigem'] = '';
	$_SESSION['usucpf'] = '';
}

if( !$db ){
	$db = new cls_banco();
}

$muncod = $_REQUEST['muncod'] ? $_REQUEST['muncod'] : '3106200';

$inuid = $db->pegaUm("SELECT inuid FROM par.instrumentounidade WHERE muncod = '{$muncod}'");
// Recupera dados do Munic�pio
$sqlDadosMunicipio = "SELECT 
			mun.mundescricao as nome,
			mun.estuf as uf,
			est.estdescricao,
			esd.esddsc as situacao
		FROM
			par.instrumentounidade iu
		INNER JOIN territorios.municipio mun ON mun.muncod = iu.muncod
		INNER JOIN territorios.estado est ON est.estuf = mun.estuf
		INNER JOIN workflow.documento doc ON doc.docid = iu.docid
		INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
		WHERE
			iu.muncod = '".$muncod."'";

$arrDadosMunicipio = $db->pegaLinha($sqlDadosMunicipio,0,86400);

$nomeMun = strtoupper ($arrDadosMunicipio['nome']); 
$nomeMun = str_replace ("�", "�", $nomeMun); 
$nomeMun = str_replace ("�", "�", $nomeMun); 
$nomeMun = str_replace ("�", "�", $nomeMun); 
$nomeMun = str_replace ("�", "A", $nomeMun); 
$nomeMun = str_replace ("�", "�", $nomeMun); 
$nomeMun = str_replace ("�", "�", $nomeMun); 
$nomeMun = str_replace ("�", "I", $nomeMun); 
$nomeMun = str_replace ("�", "�", $nomeMun); 
$nomeMun = str_replace ("�", "�", $nomeMun); 
$nomeMun = str_replace ("�", "�", $nomeMun); 
$nomeMun = str_replace ("�", "�", $nomeMun); 
$nomeMun = str_replace ("�", "�", $nomeMun); 
$nomeMun = str_replace ("�", "U", $nomeMun); 
$nomeMun = str_replace ("�", "�", $nomeMun); 

$nomeEst = strtoupper ($arrDadosMunicipio['estdescricao']); 
$nomeEst = str_replace ("�", "�", $nomeEst); 
$nomeEst = str_replace ("�", "�", $nomeEst); 
$nomeEst = str_replace ("�", "�", $nomeEst); 
$nomeEst = str_replace ("�", "A", $nomeEst); 
$nomeEst = str_replace ("�", "�", $nomeEst); 
$nomeEst = str_replace ("�", "�", $nomeEst); 
$nomeEst = str_replace ("�", "I", $nomeEst); 
$nomeEst = str_replace ("�", "�", $nomeEst); 
$nomeEst = str_replace ("�", "�", $nomeEst); 
$nomeEst = str_replace ("�", "�", $nomeEst); 
$nomeEst = str_replace ("�", "�", $nomeEst); 
$nomeEst = str_replace ("�", "�", $nomeEst); 
$nomeEst = str_replace ("�", "U", $nomeEst); 
$nomeEst = str_replace ("�", "�", $nomeEst); 


//SIGARP
$sqlSigarp = "select 
				tdi1.tiddsc as tipo, sum(dsh.dshqtde) as quantidade, dsh.dshcodmunicipio as ibge
			from painel.indicador i
			inner join painel.seriehistorica sh on sh.indid = i.indid
			inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
			inner join painel.detalhetipodadosindicador tdi1 on tdi1.tidid = dsh.tidid1
			where i.indid = 1580 AND dsh.dshcodmunicipio = '".$muncod."'
			and sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid)
				group by tdi1.tiddsc, dsh.dshcodmunicipio
				order by tdi1.tiddsc, dsh.dshcodmunicipio";

$dadoSigarp = $db->pegaLinha($sqlSigarp,0,86400);

//HABILITA / SAPE
$sqlHabilita = "select 
					tdi1.tiddsc as situacao, dsh.dshcodmunicipio as ibge
				from painel.indicador i
				inner join painel.seriehistorica sh on sh.indid = i.indid
				inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
				inner join painel.detalhetipodadosindicador tdi1 on tdi1.tidid = dsh.tidid1
				where i.indid = 1583 AND dsh.dshcodmunicipio = '".$muncod."'
				and sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid)
				group by tdi1.tiddsc, dsh.dshcodmunicipio
				order by tdi1.tiddsc, dsh.dshcodmunicipio";

$dadoHabilita = $db->pegaLinha($sqlHabilita,0,86400);

// Obras do PAC2
$sqlObrasPAC2 = "SELECT 
					tdi1.tidid as tidid1, tdi1.tiddsc as situacao, tdi2.tidid as tidid2, tdi2.tiddsc as tipologia, sum(dsh.dshqtde) as quantidade, dsh.dshcodmunicipio as ibge
				FROM 
					painel.indicador i
				inner join painel.seriehistorica sh on sh.indid = i.indid
				inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
				inner join painel.detalhetipodadosindicador tdi1 on tdi1.tidid = dsh.tidid1
				inner join painel.detalhetipodadosindicador tdi2 on tdi2.tidid = dsh.tidid2
				WHERE 
					i.indid = 1565 AND
					dsh.dshcodmunicipio = '".$muncod."' AND
					sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid)
				GROUP BY 
					tdi1.tiddsc, tdi2.tiddsc, dsh.dshcodmunicipio, tdi1.tidid, tdi2.tidid
				ORDER BY 
					tdi1.tiddsc, tdi2.tiddsc, dsh.dshcodmunicipio";
$dadosPAC2 = $db->carregar($sqlObrasPAC2,0,86400);

//Cotas para sele��o 2013
$sqlCotas2013 = "(SELECT 
						tdi1.tidid as tidid1, tdi1.tiddsc as situacao, tdi2.tidid as tidid2, tdi2.tiddsc as tipologia, sum(dsh.dshqtde) as quantidade, dsh.dshcodmunicipio as ibge
					FROM 
						painel.indicador i
					inner join painel.seriehistorica sh on sh.indid = i.indid
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalhetipodadosindicador tdi1 on tdi1.tidid = dsh.tidid1
					inner join painel.detalhetipodadosindicador tdi2 on tdi2.tidid = dsh.tidid2
					WHERE 
						i.indid = 1861 AND
						dsh.dshcodmunicipio = '".$muncod."' AND
						sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid)
					GROUP BY 
						tdi1.tiddsc, tdi2.tiddsc, dsh.dshcodmunicipio, tdi1.tidid, tdi2.tidid
					ORDER BY 
						tdi1.tiddsc, tdi2.tiddsc, dsh.dshcodmunicipio)
					
					UNION ALL
					
					(SELECT 
						tdi1.tidid as tidid1, tdi1.tiddsc as situacao, tdi2.tidid as tidid2, tdi2.tiddsc as tipologia, sum(dsh.dshqtde) as quantidade, dsh.dshcodmunicipio as ibge
					FROM 
						painel.indicador i
					inner join painel.seriehistorica sh on sh.indid = i.indid
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalhetipodadosindicador tdi1 on tdi1.tidid = dsh.tidid1
					inner join painel.detalhetipodadosindicador tdi2 on tdi2.tidid = dsh.tidid2
					WHERE 
						i.indid = 1863 AND
						dsh.dshcodmunicipio = '".$muncod."' AND
						sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid)
					GROUP BY 
						tdi1.tiddsc, tdi2.tiddsc, dsh.dshcodmunicipio, tdi1.tidid, tdi2.tidid
					ORDER BY 
						tdi1.tiddsc, tdi2.tiddsc, dsh.dshcodmunicipio)";
$dadosCotas2013 = $db->carregar($sqlCotas2013,0,86400);

//SIGPC - Contas Online
$sqlSIGPC = "select 
				tdi1.tiddsc as tipo, sum(dsh.dshqtde) as quantidade, dsh.dshcodmunicipio as ibge, i.indobjetivo
			from painel.indicador i
			inner join painel.seriehistorica sh on sh.indid = i.indid
			inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
			inner join painel.detalhetipodadosindicador tdi1 on tdi1.tidid = dsh.tidid1
			where i.indid = 1859 AND dsh.dshcodmunicipio = '".$muncod."'
			and sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid)
				group by tdi1.tiddsc, dsh.dshcodmunicipio, i.indobjetivo
				order by tdi1.tiddsc, dsh.dshcodmunicipio";

$dadoSIGPC = $db->pegaLinha($sqlSIGPC,0,86400);

$soma1 = 0;
$soma2 = 0;
$soma3 = 0;

if( is_array($dadosPAC2) ){
	foreach( $dadosPAC2 as $pac ){
		// Creche / Pr�-escola
		if( $pac['tidid2'] == 3850 ){ // Em planejamento
			$local11 = $pac['quantidade'] ? $pac['quantidade'] : 0;
			$soma1 = $soma1 + $pac['quantidade'];
		}
		if( !in_array($pac['tidid1'], array(3840, 3845, 3844, 3839)) && $pac['tidid2'] == 3850 ){ // Em licita��o
			$local12 = $pac['quantidade'] ? $pac['quantidade'] : 0;
			$soma1 = $soma1 + $pac['quantidade'];
		}
		if( $pac['tidid1'] == 3840 && $pac['tidid2'] == 3850 ){ // Em execu��o
			$local13 = $pac['quantidade'] ? $pac['quantidade'] : 0;
			$soma1 = $soma1 + $pac['quantidade'];
		}
		if( $pac['tidid1'] == 3845 && $pac['tidid2'] == 3850 ){ // Paralisada
			$local14 = $pac['quantidade'] ? $pac['quantidade'] : 0;
			$soma1 = $soma1 + $pac['quantidade'];
		}
		if( $pac['tidid1'] == 3844 && $pac['tidid2'] == 3850 ){ // Cancelada
			$local15 = $pac['quantidade'] ? $pac['quantidade'] : 0;
			$soma1 = $soma1 + $pac['quantidade'];
		}
		if( $pac['tidid1'] == 3839 && $pac['tidid2'] == 3850 ){ // Concluida
			$local16 = $pac['quantidade'] ? $pac['quantidade'] : 0;
			$soma1 = $soma1 + $pac['quantidade'];
		}
	
		// Constru��o de Quadras
		if( $pac['tidid2'] == 3851 ){ // Em planejamento
			$local21 = $pac['quantidade'] ? $pac['quantidade'] : 0;
			$soma2 = $soma2 + $pac['quantidade'];
		}
		if( !in_array($pac['tidid1'], array(3840, 3845, 3844, 3839)) && $pac['tidid2'] == 3851 ){ // Em licita��o
			$local22 = $pac['quantidade'] ? $pac['quantidade'] : 0;
			$soma2 = $soma2 + $pac['quantidade'];
		}
		if( $pac['tidid1'] == 3840 && $pac['tidid2'] == 3851 ){ // Em execu��o
			$local23 = $pac['quantidade'] ? $pac['quantidade'] : 0;
			$soma2 = $soma2 + $pac['quantidade'];
		}
		if( $pac['tidid1'] == 3845 && $pac['tidid2'] == 3851 ){ // Paralisada
			$local24 = $pac['quantidade'] ? $pac['quantidade'] : 0;
			$soma2 = $soma2 + $pac['quantidade'];
		}
		if( $pac['tidid1'] == 3844 && $pac['tidid2'] == 3851 ){ // Cancelada
			$local25 = $pac['quantidade'] ? $pac['quantidade'] : 0;
			$soma2 = $soma2 + $pac['quantidade'];
		}
		if( $pac['tidid1'] == 3839 && $pac['tidid2'] == 3851 ){ // Concluida
			$local26 = $pac['quantidade'] ? $pac['quantidade'] : 0;
			$soma2 = $soma2 + $pac['quantidade'];
		}
	
		// Cobertura de Quadras
		if( $pac['tidid2'] == 3852 ){ // Em planejamento
			$local31 = $pac['quantidade'] ? $pac['quantidade'] : 0;
			$soma3 = $soma3 + $pac['quantidade'];
		}
		if( !in_array($pac['tidid1'], array(3840, 3845, 3844, 3839)) && $pac['tidid2'] == 3852 ){ // N�o iniciadas
			$local32 = $pac['quantidade'] ? $pac['quantidade'] : 0;
			$soma3 = $soma3 + $pac['quantidade'];
		}
		if( $pac['tidid1'] == 3840 && $pac['tidid2'] == 3852 ){ // Em execu��o
			$local33 = $pac['quantidade'] ? $pac['quantidade'] : 0;
			$soma3 = $soma3 + $pac['quantidade'];
		}
		if( $pac['tidid1'] == 3845 && $pac['tidid2'] == 3852 ){ // Paralisada
			$local34 = $pac['quantidade'] ? $pac['quantidade'] : 0;
			$soma3 = $soma3 + $pac['quantidade'];
		}
		if( $pac['tidid1'] == 3844 && $pac['tidid2'] == 3852 ){ // Cancelada
			$local35 = $pac['quantidade'] ? $pac['quantidade'] : 0;
			$soma3 = $soma3 + $pac['quantidade'];
		}
		if( $pac['tidid1'] == 3839 && $pac['tidid2'] == 3852 ){ // Concluida
			$local36 = $pac['quantidade'] ? $pac['quantidade'] : 0;
			$soma3 = $soma3 + $pac['quantidade'];
		}
	}
	$pacteste2 = true;
}
$somaC1 = 0;
$somaC2 = 0;
$somaC3 = 0;

if( is_array($dadosCotas2013) ){
	foreach( $dadosCotas2013 as $cota2013 ){
		// Creche / Pr�-escola
		if( $cota2013['tidid2'] == 4549 ){ // Obra deferida
			$localP11 = $cota2013['quantidade'] ? $cota2013['quantidade'] : 0;
			$somaC1 = $somaC1 + $cota2013['quantidade'];
		}
		if( $cota2013['tidid2'] == 4548 ){ // Em revis�o de an�lise
			$localP12 = $cota2013['quantidade'] ? $cota2013['quantidade'] : 0;
			$somaC1 = $somaC1 + $cota2013['quantidade'];
		}
		if( $cota2013['tidid2'] == 4547 ){ // Aguardando An�lise - FNDE
			$localP13 = $cota2013['quantidade'] ? $cota2013['quantidade'] : 0;
			$somaC1 = $somaC1 + $cota2013['quantidade'];
		}
		if( $cota2013['tidid2'] == 4251 ){ // Valida��o de deferimento
			$localP14 = $cota2013['quantidade'] ? $cota2013['quantidade'] : 0;
			$somaC1 = $somaC1 + $cota2013['quantidade'];
		}
		if( $cota2013['tidid2'] == 4546 ){ // Em dilig�ncia
			$localP15 = $cota2013['quantidade'] ? $cota2013['quantidade'] : 0;
			$somaC1 = $somaC1 + $cota2013['quantidade'];
		}
		if( $cota2013['tidid2'] == 4253 ){ // Valida��o de dilig�ncia
			$localP16 = $cota2013['quantidade'] ? $cota2013['quantidade'] : 0;
			$somaC1 = $somaC1 + $cota2013['quantidade'];
		}
		if( $cota2013['tidid2'] == 4545 ){ // Em an�lise/ retorno de dilig�ncia
			$localP16 = $cota2013['quantidade'] ? $cota2013['quantidade'] : 0;
			$somaC1 = $somaC1 + $cota2013['quantidade'];
		}
		if( $cota2013['tidid2'] == 4252 ){ // Em an�lise
			$localP16 = $cota2013['quantidade'] ? $cota2013['quantidade'] : 0;
			$somaC1 = $somaC1 + $cota2013['quantidade'];
		}
	
		// Constru��o de Quadras
		if( $cota2013['tidid1'] == 4260 && $cota2013['tidid2'] == 4264 ){ // Obra deferida
			$localP21 = $cota2013['quantidade'] ? $cota2013['quantidade'] : 0;
			$somaC2 = $somaC2 + $cota2013['quantidade'];
		}
		if( $cota2013['tidid1'] == 4260 && $cota2013['tidid2'] == 4265 ){ //Em revis�o de an�lise
			$localP22 = $cota2013['quantidade'] ? $cota2013['quantidade'] : 0;
			$somaC2 = $somaC2 + $cota2013['quantidade'];
		}
		if( $cota2013['tidid1'] == 4260 && $cota2013['tidid2'] == 4266 ){ // Aguardando An�lise - FNDE
			$localP23 = $cota2013['quantidade'] ? $cota2013['quantidade'] : 0;
			$somaC2 = $somaC2 + $cota2013['quantidade'];
		}
		if( $cota2013['tidid1'] == 4260 && $cota2013['tidid2'] == 4552 ){ // Valida��o de deferimento
			$localP24 = $cota2013['quantidade'] ? $cota2013['quantidade'] : 0;
			$somaC2 = $somaC2 + $cota2013['quantidade'];
		}
		if( $cota2013['tidid1'] == 4260 && $cota2013['tidid2'] == 4550 ){ // Em dilig�ncia
			$localP25 = $cota2013['quantidade'] ? $cota2013['quantidade'] : 0;
			$somaC2 = $somaC2 + $cota2013['quantidade'];
		}
		if( $cota2013['tidid1'] == 4260 && $cota2013['tidid2'] == 4553 ){ // Valida��o de dilig�ncia
			$localP26 = $cota2013['quantidade'] ? $cota2013['quantidade'] : 0;
			$somaC2 = $somaC2 + $cota2013['quantidade'];
		}
		if( $cota2013['tidid1'] == 4260 && $cota2013['tidid2'] == 4551 ){ // Em an�lise/ retorno de dilig�ncia
			$localP27 = $cota2013['quantidade'] ? $cota2013['quantidade'] : 0;
			$somaC2 = $somaC2 + $cota2013['quantidade'];
		}
		if( $cota2013['tidid1'] == 4260 && $cota2013['tidid2'] == 4554 ){ // Em an�lise
			$localP28 = $cota2013['quantidade'] ? $cota2013['quantidade'] : 0;
			$somaC2 = $somaC2 + $cota2013['quantidade'];
		}
	
		// Cobertura de Quadras
		if( $cota2013['tidid1'] == 4261 && $cota2013['tidid2'] == 4264 ){ // Obra deferida
			$localP31 = $cota2013['quantidade'] ? $cota2013['quantidade'] : 0;
			$somaC3 = $somaC3 + $cota2013['quantidade'];
		}
		if( $cota2013['tidid1'] == 4261 && $cota2013['tidid2'] == 4265 ){ // Em revis�o de an�lise
			$localP32 = $cota2013['quantidade'] ? $cota2013['quantidade'] : 0;
			$somaC3 = $somaC3 + $cota2013['quantidade'];
		}
		if( $cota2013['tidid1'] == 4261 && $cota2013['tidid2'] == 4266 ){ // Aguardando An�lise - FNDE
			$localP33 = $cota2013['quantidade'] ? $cota2013['quantidade'] : 0;
			$somaC3 = $somaC3 + $cota2013['quantidade'];
		}
		if( $cota2013['tidid1'] == 4261 && $cota2013['tidid2'] == 4552 ){ // Valida��o de deferimento
			$localP34 = $cota2013['quantidade'] ? $cota2013['quantidade'] : 0;
			$somaC3 = $somaC3 + $cota2013['quantidade'];
		}
		if( $cota2013['tidid1'] == 4261 && $cota2013['tidid2'] == 4550 ){ // Em dilig�ncia
			$localP35 = $cota2013['quantidade'] ? $cota2013['quantidade'] : 0;
			$somaC3 = $somaC3 + $cota2013['quantidade'];
		}
		if( $cota2013['tidid1'] == 4261 && $cota2013['tidid2'] == 4553 ){ // Valida��o de dilig�ncia
			$localP36 = $cota2013['quantidade'] ? $cota2013['quantidade'] : 0;
			$somaC3 = $somaC3 + $cota2013['quantidade'];
		}
		if( $cota2013['tidid1'] == 4261 && $cota2013['tidid2'] == 4551 ){ // Em an�lise/ retorno de dilig�ncia
			$localP37 = $cota2013['quantidade'] ? $cota2013['quantidade'] : 0;
			$somaC3 = $somaC3 + $cota2013['quantidade'];
		}
		if( $cota2013['tidid1'] == 4261 && $cota2013['tidid2'] == 4554 ){ // Em an�lise
			$localP38 = $cota2013['quantidade'] ? $cota2013['quantidade'] : 0;
			$somaC3 = $somaC3 + $cota2013['quantidade'];
		}
	}
	$pacteste3 = true;
}

// Obras do PAR
$sqlObrasPAR = "SELECT 
					tdi1.tidid as tidid1, tdi1.tiddsc as situacao, tdi2.tidid as tidid2, tdi2.tiddsc as tipologia, sum(dsh.dshqtde) as quantidade, dsh.dshcodmunicipio as ibge
				FROM 
					painel.indicador i
				inner join painel.seriehistorica sh on sh.indid = i.indid
				inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
				inner join painel.detalhetipodadosindicador tdi1 on tdi1.tidid = dsh.tidid1
				inner join painel.detalhetipodadosindicador tdi2 on tdi2.tidid = dsh.tidid2
				WHERE 
					i.indid = 1569 AND
					sehstatus='A' AND 
					dsh.dshcodmunicipio = '".$muncod."'
				GROUP BY 
					tdi1.tiddsc, tdi2.tiddsc, dsh.dshcodmunicipio, tdi1.tidid, tdi2.tidid
				ORDER BY 
					tdi1.tiddsc, tdi2.tiddsc, dsh.dshcodmunicipio";
$dadosPAR = $db->carregar($sqlObrasPAR, null, 86400);

$somapar1 = 0;
$somapar2 = 0;
$somapar3 = 0;

$localpar11 = 0;
$localpar12 = 0;
$localpar13 = 0;
$localpar14 = 0;
$localpar15 = 0;
$localpar16 = 0;
$localpar17 = 0;
$localpar21 = 0;
$localpar22 = 0;
$localpar23 = 0;
$localpar24 = 0;
$localpar25 = 0;
$localpar26 = 0;
$localpar27 = 0;
$localpar31 = 0;
$localpar32 = 0;
$localpar33 = 0;
$localpar34 = 0;
$localpar35 = 0;
$localpar36 = 0;
$localpar37 = 0;

if(is_array($dadosPAR)){
	foreach( $dadosPAR as $par ){
		// Urbana (nao inclui pro infancia)
		if( $par['tidid1'] == 3856 && $par['tidid2'] == 3860 ){ // Em planejamento
			$localpar11 = $par['quantidade'] ? $par['quantidade'] : 0;
			$somapar1 = $somapar1 + $par['quantidade'];
		}
		if( $par['tidid1'] == 3855 && $par['tidid2'] == 3860 ){ // Em licita��o
			$localpar12 = $par['quantidade'] ? $par['quantidade'] : 0;
			$somapar1 = $somapar1 + $par['quantidade'];
		}
		if( $par['tidid1'] == 3854 && $par['tidid2'] == 3860 ){ // Em execu��o
			$localpar13 = $par['quantidade'] ? $par['quantidade'] : 0;
			$somapar1 = $somapar1 + $par['quantidade'];
		}
		if( $par['tidid1'] == 3859 && $par['tidid2'] == 3860 ){ // Paralisada
			$localpar14 = $par['quantidade'] ? $par['quantidade'] : 0;
			$somapar1 = $somapar1 + $par['quantidade'];
		}
		if( $par['tidid1'] == 3858 && $par['tidid2'] == 3860 ){ // Cancelada
			$localpar15 = $par['quantidade'] ? $par['quantidade'] : 0;
			$somapar1 = $somapar1 + $par['quantidade'];
		}
		if( $par['tidid1'] == 3857 && $par['tidid2'] == 3860 ){ // Em Reformula��o
			$localpar17 = $par['quantidade'] ? $par['quantidade'] : 0;
			$somapar1 = $somapar1 + $par['quantidade'];
		}
		if( $par['tidid1'] == 3853 && $par['tidid2'] == 3860 ){ // Concluida
			$localpar16 = $par['quantidade'] ? $par['quantidade'] : 0;
			$somapar1 = $somapar1 + $par['quantidade'];
		}
	
		// rural/ campo
		if( $par['tidid1'] == 3856 && $par['tidid2'] == 3861 ){ // Em planejamento
			$localpar21 = $par['quantidade'] ? $par['quantidade'] : 0;
			$somapar2 = $somapar2 + $par['quantidade'];
		}
		if( $par['tidid1'] == 3855 && $par['tidid2'] == 3861 ){ // Em licita��o
			$localpar22 = $par['quantidade'] ? $par['quantidade'] : 0;
			$somapar2 = $somapar2 + $par['quantidade'];
		}
		if( $par['tidid1'] == 3854 && $par['tidid2'] == 3861 ){ // Em execu��o
			$localpar23 = $par['quantidade'] ? $par['quantidade'] : 0;
			$somapar2 = $somapar2 + $par['quantidade'];
		}
		if( $par['tidid1'] == 3859 && $par['tidid2'] == 3861 ){ // Paralisada
			$localpar24 = $par['quantidade'] ? $par['quantidade'] : 0;
			$somapar2 = $somapar2 + $par['quantidade'];
		}
		if( $par['tidid1'] == 3858 && $par['tidid2'] == 3861 ){ // Cancelada
			$localpar25 = $par['quantidade'] ? $par['quantidade'] : 0;
			$somapar2 = $somapar2 + $par['quantidade'];
		}
		if( $par['tidid1'] == 3857 && $par['tidid2'] == 3861 ){ // Em Reformula��o
			$localpar27 = $par['quantidade'] ? $par['quantidade'] : 0;
			$somapar2 = $somapar2 + $par['quantidade'];
		}
		if( $par['tidid1'] == 3853 && $par['tidid2'] == 3861 ){ // Concluida
			$localpar26 = $par['quantidade'] ? $par['quantidade'] : 0;
			$somapar2 = $somapar2 + $par['quantidade'];
		}
	
		// Proinf�ncia pre pac
		if( $par['tidid1'] == 3856 && $par['tidid2'] == 3862 ){ // Em planejamento
			$localpar31 = $par['quantidade'] ? $par['quantidade'] : 0;
			$somapar3 = $somapar3 + $par['quantidade'];
		}
		if( $par['tidid1'] == 3855 && $par['tidid2'] == 3862 ){ // Em licita��o
			$localpar32 = $par['quantidade'] ? $par['quantidade'] : 0;
			$somapar3 = $somapar3 + $par['quantidade'];
		}
		if( $par['tidid1'] == 3854 && $par['tidid2'] == 3862 ){ // Em execu��o
			$localpar33 = $par['quantidade'] ? $par['quantidade'] : 0;
			$somapar3 = $somapar3 + $par['quantidade'];
		}
		if( $par['tidid1'] == 3859 && $par['tidid2'] == 3862 ){ // Paralisada
			$localpar34 = $par['quantidade'] ? $par['quantidade'] : 0;
			$somapar3 = $somapar3 + $par['quantidade'];
		}
		if( $par['tidid1'] == 3858 && $par['tidid2'] == 3862 ){ // Cancelada
			$localpar35 = $par['quantidade'] ? $par['quantidade'] : 0;
			$somapar3 = $somapar3 + $par['quantidade'];
		}
		if( $par['tidid1'] == 3857 && $par['tidid2'] == 3862 ){ // Em Reformula��o
			$localpar37 = $par['quantidade'] ? $par['quantidade'] : 0;
			$somapar3 = $somapar3 + $par['quantidade'];
		}
		if( $par['tidid1'] == 3853 && $par['tidid2'] == 3862 ){ // Concluida
			$localpar36 = $par['quantidade'] ? $par['quantidade'] : 0;
			$somapar3 = $somapar3 + $par['quantidade'];
		}
	}
	$parteste = true;
}


//Termos de compromisso
$sqlTermos = "select DISTINCT
			COALESCE(count(dop.dopid),0) as conta
		from 
			par.vm_documentopar_ativos dop
		INNER JOIN par.processopar prp ON prp.prpid = dop.prpid and prp.prpstatus = 'A'
		INNER JOIN par.instrumentounidade iu ON iu.inuid = prp.inuid
		INNER JOIN territorios.municipio mun ON mun.muncod = iu.muncod
		where 
			dop.mdoid = 20 AND 
			mun.muncod = '".$muncod."' AND
			dop.dopid NOT IN ( SELECT dopid FROM par.documentoparvalidacao dpv )";

$documentosnaovalidados = $db->pegaUm($sqlTermos,0,86400);

$documentosnaovalidados = $documentosnaovalidados ? $documentosnaovalidados : 0;


// Tecnologias educacionais. 
$sqlTecEduc = "SELECT 
                    atendimentos, quantidade, ibge
                FROM (
                    SELECT CASE dsh.tidid2
                        WHEN 27 then 'Laborat�rio de Inform�tica Proinfo � urbano' --Urbano
                        WHEN 28 then 'Laborat�rio de Inform�tica Proinfo � rural' --Rural
                        END AS atendimentos, sum(dsh.dshqtde) as quantidade, dsh.dshcodmunicipio as ibge
                    FROM painel.indicador i
                    INNER JOIN painel.seriehistorica sh ON sh.indid = i.indid
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    WHERE i.indid = 224 AND dsh.dshcodmunicipio = '".$muncod."'
                    AND dsh.tidid1 = 122 -- Municipal
                    GROUP BY atendimentos, dsh.dshcodmunicipio
                UNION ALL
                    SELECT 
                        'Projetor Proinfo (computador interativo)' as atendimentos, COUNT(DISTINCT dsh.dshcod) as quantidade, dsh.dshcodmunicipio as ibge
                    FROM painel.indicador i
                    INNER JOIN painel.seriehistorica sh ON sh.indid = i.indid
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    WHERE i.indid = 750 AND dsh.dshcodmunicipio = '".$muncod."'
                    AND dsh.tidid1 = 2798 -- Municipal
                    GROUP BY atendimentos, dsh.dshcodmunicipio
                UNION ALL
                    SELECT 
                        'Escolas com Internet (Programa Banda Larga na Escola)' as atendimentos, sum(dsh.dshqtde) as quantidade, dsh.dshcodmunicipio as ibge
                    FROM painel.indicador i
                    INNER JOIN painel.seriehistorica sh ON sh.indid = i.indid
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    WHERE i.indid = 235 AND dsh.dshcodmunicipio = '".$muncod."'
                    AND dsh.tidid1 = 105 -- Municipal
                    GROUP BY atendimentos, dsh.dshcodmunicipio
                ) AS foo
                ORDER BY atendimentos";

$dadosTecEduc = $db->carregar($sqlTecEduc,null,86400);

// Mais Educa��o
$sqlMaisEducacao = "select 
                                2012 as ano, tipo, quantidade, ibge, tidid1
                        from	(
                                select 'Escolas que Aderiram' as tipo, sum(dsh.dshqtde) as quantidade, dsh.dshcodmunicipio as ibge, dsh.tidid1
                                from painel.indicador i
                                inner join painel.seriehistorica sh on sh.indid = i.indid
                                inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                                inner join painel.detalheperiodicidade dp on dp.dpeid = sh.dpeid
                                where i.indid = 690
                                and dp.dpeanoref = '2012' AND dsh.dshcodmunicipio = '".$muncod."'
                                and dsh.tidid1 = 2569 --Municipal
                                group by ibge, dsh.tidid1
                        union all
                                select 'Alunado das escolas que aderiram' as tipo, sum(dsh.dshqtde) as quantidade, dsh.dshcodmunicipio as ibge, dsh.tidid1
                                from painel.indicador i
                                inner join painel.seriehistorica sh on sh.indid = i.indid
                                inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                                inner join painel.detalheperiodicidade dp on dp.dpeid = sh.dpeid
                                where i.indid = 102
                                and dp.dpeanoref = '2012' AND dsh.dshcodmunicipio = '".$muncod."'
                                and dsh.tidid1 = 639 --Municipal
                                group by ibge, dsh.tidid1
                        union all
                                select 'Valor Repassado PDDE' as tipo, sum(dsh.dshqtde) as quantidade, dsh.dshcodmunicipio as ibge, dsh.tidid1
                                from painel.indicador i
                                inner join painel.seriehistorica sh on sh.indid = i.indid
                                inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                                inner join painel.detalheperiodicidade dp on dp.dpeid = sh.dpeid
                                inner join entidade.entidade e on e.entcodent = dsh.dshcod and e.entstatus = 'A'
                                where i.indid = 1420
                                and dp.dpeanoref = '2012' AND dsh.dshcodmunicipio = '".$muncod."'
                                and e.tpcid = 3 --Municipal
                                group by ibge, dsh.tidid1
                                ) as foo
                        order by ano";

$dadosMaisEducacao = $db->carregar($sqlMaisEducacao,null,86400);

// Mais Educa��o - 2013
$sqlMaisEducacao2013 = "select 
						2013 as ano, tipo, quantidade, ibge, tidid1
					from	(
						select 'Escolas que Aderiram' as tipo, sum(dsh.dshqtde) as quantidade, dsh.dshcodmunicipio as ibge, dsh.tidid1
						from painel.indicador i
						inner join painel.seriehistorica sh on sh.indid = i.indid
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						inner join painel.detalheperiodicidade dp on dp.dpeid = sh.dpeid
						where i.indid = 690
						and dp.dpeanoref = '2013' AND dsh.dshcodmunicipio = '".$muncod."'
						and dsh.tidid1 = 2569 --Municipal
						group by ibge, dsh.tidid1
					union all
						select 'Alunado das escolas que aderiram' as tipo, sum(dsh.dshqtde) as quantidade, dsh.dshcodmunicipio as ibge, dsh.tidid1
						from painel.indicador i
						inner join painel.seriehistorica sh on sh.indid = i.indid
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						inner join painel.detalheperiodicidade dp on dp.dpeid = sh.dpeid
						where i.indid = 102
						and dp.dpeanoref = '2013' AND dsh.dshcodmunicipio = '".$muncod."'
						and dsh.tidid1 = 639 --Municipal
						group by ibge, dsh.tidid1
					union all
						select 'Valor Repassado PDDE' as tipo, sum(dsh.dshqtde) as quantidade, dsh.dshcodmunicipio as ibge, dsh.tidid1
						from painel.indicador i
						inner join painel.seriehistorica sh on sh.indid = i.indid
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						inner join painel.detalheperiodicidade dp on dp.dpeid = sh.dpeid
						inner join entidade.entidade e on e.entcodent = dsh.dshcod and e.entstatus = 'A'
						where i.indid = 1420
						and dp.dpeanoref = '2013' AND dsh.dshcodmunicipio = '".$muncod."'
						and e.tpcid = 3 --Municipal
						group by ibge, dsh.tidid1
						) as foo
					order by ano";

$dadosMaisEducacao2013 = $db->carregar($sqlMaisEducacao2013,null,86400);

// Mais Educa��o - 2014
$sqlMaisEducacao2014 = "select 
                            2014 as ano, tipo, quantidade, ibge, tidid1
                        from	(
                            select 'Escolas que Aderiram' as tipo, sum(dsh.dshqtde) as quantidade, dsh.dshcodmunicipio as ibge, dsh.tidid1
                            from painel.indicador i
                            inner join painel.seriehistorica sh on sh.indid = i.indid
                            inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                            inner join painel.detalheperiodicidade dp on dp.dpeid = sh.dpeid
                            where i.indid = 690
                            and dp.dpeanoref = '2014' AND dsh.dshcodmunicipio = '".$muncod."'
                            and dsh.tidid1 = 2569 --Municipal
                            group by ibge, dsh.tidid1
                        union all
                            select 'Alunado das escolas que aderiram' as tipo, sum(dsh.dshqtde) as quantidade, dsh.dshcodmunicipio as ibge, dsh.tidid1
                            from painel.indicador i
                            inner join painel.seriehistorica sh on sh.indid = i.indid
                            inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                            inner join painel.detalheperiodicidade dp on dp.dpeid = sh.dpeid
                            where i.indid = 102
                            and dp.dpeanoref = '2014' AND dsh.dshcodmunicipio = '".$muncod."'
                            and dsh.tidid1 = 639 --Municipal
                            group by ibge, dsh.tidid1
                        union all
                            select 'Valor Repassado PDDE' as tipo, sum(dsh.dshqtde) as quantidade, dsh.dshcodmunicipio as ibge, dsh.tidid1
                            from painel.indicador i
                            inner join painel.seriehistorica sh on sh.indid = i.indid
                            inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                            inner join painel.detalheperiodicidade dp on dp.dpeid = sh.dpeid
                            inner join entidade.entidade e on e.entcodent = dsh.dshcod and e.entstatus = 'A'
                            where i.indid = 1420
                            and dp.dpeanoref = '2014' AND dsh.dshcodmunicipio = '".$muncod."'
                            and e.tpcid = 3 --Municipal
                            group by ibge, dsh.tidid1
                            ) as foo
                        order by ano";

$dadosMaisEducacao2014 = $db->carregar($sqlMaisEducacao2014,null,86400);
    
//Escolas pass�veis de Ades�o em 2014 
//Alterado pela busca $dadosMaisEducacao2014
//$sqlEscolasPass = "SELECT 
//				count(me.entid)
//			FROM 
//				pdeescola.memaiseducacao me 
//			INNER JOIN entidade.endereco ende ON ende.entid = me.entid AND ende.endstatus = 'A'
//			INNER JOIN entidade.entidade ent ON ent.entid = me.entid AND ent.entstatus = 'A' and ent.tpcid = 3
//			where 
//				me.memanoreferencia in (2012,2013,2014) AND 
//				me.memstatus = 'A' AND
//				ende.muncod = '".$muncod."'";
//
//$escolasPass2014 = $db->pegaUm($sqlEscolasPass,0,86400);
//
//$escolasPass2014 = $escolasPass2014 ? $escolasPass2014 : 0;

if( is_array($dadosMaisEducacao) ){
	foreach( $dadosMaisEducacao as $maisEduc ){
		// "Escolas que Aderiram"
		if( $maisEduc['tidid1'] == 2569 ){
			$quantidadeME1 = $maisEduc['quantidade'] ? $maisEduc['quantidade'] : 0;
		}
		// "Alunado das escolas que aderiram"
		if( $maisEduc['tidid1'] == 639 ){
			$quantidadeME2 = $maisEduc['quantidade'] ? $maisEduc['quantidade'] : 0;
		}
		// "Valor Repassado PDDE"
		if( $maisEduc['tidid1'] == 3355 ){
			$quantidadeME3 = $maisEduc['quantidade'] ? $maisEduc['quantidade'] : 0;
		}
	}
}

if( is_array($dadosMaisEducacao2013) ){
	foreach( $dadosMaisEducacao2013 as $maisEduc2013 ){
		// "Escolas que Aderiram"
		if( $maisEduc2013['tidid1'] == 2569 ){
			$quantidadeME11 = $maisEduc2013['quantidade'] ? $maisEduc2013['quantidade'] : 0;
		}
		// "Alunado das escolas que aderiram"
		if( $maisEduc2013['tidid1'] == 639 ){
			$quantidadeME12 = $maisEduc2013['quantidade'] ? $maisEduc2013['quantidade'] : 0;
		}
		// "Valor Repassado PDDE"
		if( $maisEduc2013['tidid1'] == 3355 ){
			$quantidadeME13 = $maisEduc2013['quantidade'] ? $maisEduc2013['quantidade'] : 0;
		}
	}
}

//var_dump($dadosMaisEducacao2014, d);
if( is_array($dadosMaisEducacao2014) ){
	foreach( $dadosMaisEducacao2014 as $maisEduc2014 ){
		// "Escolas que Aderiram"
		if( $maisEduc2014['tidid1'] == 2569 ){
			$quantidadeME21 = $maisEduc2014['quantidade'] ? $maisEduc2014['quantidade'] : 0;
		}
		// "Alunado das escolas que aderiram"
		if( $maisEduc2014['tidid1'] == 639 ){
			$quantidadeME22 = $maisEduc2014['quantidade'] ? $maisEduc2014['quantidade'] : 0;
		}
		// "Valor Repassado PDDE"
		if( $maisEduc2014['tidid1'] == 3355 ){
			$quantidadeME23 = $maisEduc2014['quantidade'] ? $maisEduc2014['quantidade'] : 0;
		}
	}
}

// PACTO
$sqlPacto = "SELECT 
				ap.situacao as situacao1, 
				op.situacao as situacao2, 
				'( Dispon�veis: ' || op.\"Total de vagas dispon�veis\" || '/ preenchidas: ' || op.\"Total de vagas preenchidas\" || ')' as vagas2,
				fp.\"Fez a forma��o inicial?\" as formacaoinicial,	
				up.\"UNIVERSIDADE\" as universidade
			FROM 
				par.prefeitospactoadesoes ap
			LEFT JOIN par.prefeitospactoorientadores op ON op.\"COD IBGE\" = ap.\"COD IBGE\"
			LEFT JOIN par.prefeitospactoformacaoinicial fp ON fp.\"COD IBGE\" = ap.\"COD IBGE\"
			LEFT JOIN par.prefeitospactouniversidade up ON up.\"COD IBGE\" = ap.\"COD IBGE\"
			WHERE
				ap.\"COD IBGE\" = '".$muncod."'";
//ver($sqlPacto, d);
$dadosPacto = $db->pegaLinha($sqlPacto,0,86400);

// Ades�o
if( $dadosPacto['situacao1'] == 1 ){
	$situacaoAdesao = "Concluiu a ades�o";
	$oquefazerAdesao = "Cadastrar Orientadores de Estudo e assegurar a participa��o destes na forma��o inicial.";
} elseif($dadosPacto['situacao1'] == 2){
	$situacaoAdesao = "N�o concluiu a ades�o";
	$oquefazerAdesao = "Acessar o PAR e finalizar a ades�o. Em seguida, o coordenador local deve cadastrar os Orientadores de Estudo e assegurar a participa��o destes na forma��o inicial.";	
} elseif($dadosPacto['situacao1'] == 3){
	$situacaoAdesao = "N�o se manifestou sobre o Pacto";
	$oquefazerAdesao = "Se deseja aderir, deve enviar e-mail para pactonacional@mec.gov.br, com o t�tulo �Ades�o ao Pacto�. Em seguida, receber� as orienta��es do MEC.";	
} elseif($dadosPacto['situacao1'] == 4){
	$situacaoAdesao = "N�o aderiu ao Pacto";
	$oquefazerAdesao = "Se deseja aderir, deve enviar e-mail para pactonacional@mec.gov.br, com o t�tulo �Ades�o ao Pacto�. Em seguida, receber� as orienta��es do MEC.";	
}

// Cadastro dos orientadores de estudo
if( $dadosPacto['situacao2'] == 1 ){
	$situacaoOrientadores = "Concluiu o cadastro dos Orientadores de Estudo<br/>".$dadosPacto['vagas2'];
	$oquefazerOrientadores = "Assegurar a participa��o dos orientadores cadastrados na forma��o inicial e, a partir de Janeiro/ 2013, o Coordenador local deve iniciar o cadastro dos Professores Alfabetizadores, no SisPacto.";
} elseif($dadosPacto['situacao2'] == 2){
	$situacaoOrientadores = "N�o concluiu o cadastro dos Orientadores de Estudo<br/>".$dadosPacto['vagas2'];
	$oquefazerOrientadores = "O Coordenador local deve acessar o SisPacto e concluir o cadastro dos Orientadores de Estudo do munic�pio antes do in�cio da forma��o inicial com a Institui��o de Ensino Superior (IES) respons�vel.";	
} elseif($dadosPacto['situacao2'] == 3){
	$situacaoOrientadores = "N�o iniciou o cadastro dos Orientadores de Estudo<br/>".$dadosPacto['vagas2'];
	$oquefazerOrientadores = "O Coordenador local deve acessar o SisPacto e cadastrar os Orientadores de Estudo do munic�pio antes do in�cio da forma��o inicial com a Institui��o de Ensino Superior (IES) respons�vel.";	
} elseif($dadosPacto['situacao2'] == 4){
	$situacaoOrientadores = "Deseja substituir os Orientadores de Estudo j� cadastrados<br/>".$dadosPacto['vagas2'];
	$oquefazerOrientadores = "O Coordenador local deve acessar o SisPacto e seguir os procedimentos de substitui��o informados pelo MEC e dispon�veis no site do Pacto (http://pacto.mec.gov.br).";	
}

// Forma��o inicial dos orientadores de estudo
if( $dadosPacto['formacaoinicial'] == 'SIM' ){
	$situacaoFormacao = "Participou da forma��o inicial dos seus Orientadores de Estudo";
	$oquefazerFormacao = "O Coordenador local deve iniciar a organiza��o das turmas de professores alfabetizadores com os Orientadores de Estudo que participaram da forma��o inicial. No final de Janeiro/ 2013 deve iniciar o cadastramento dessas turmas no SisPacto.";
} elseif($dadosPacto['formacaoinicial'] == 'N�O' ){
	$situacaoFormacao = "N�o participou da forma��o inicial dos seus Orientadores de Estudo";
	$oquefazerFormacao = "A Secretaria de Educa��o deve ficar atenta ao calend�rio da forma��o inicial, identificando a institui��o respons�vel pelo seu munic�pio e a data dos encontros. Em caso de d�vida sobre o local da forma��o, deve entrar em contato com a institui��o respons�vel nos endere�os disponibilizados no site do Pacto (http://pacto.mec.gov.br).";	
}

// IDEB

$sqlIDEB = "SELECT 
	            idbensino as intensino,
	            idbano as ano,
	            idbvlrobservado as valor,
	            idbvlrmeta as meta
			FROM public.ideb
			WHERE idbrede = 'Municipal'
			AND muncod = '".$muncod."'
			AND idbano IN ('2005', '2007', '2009', '2011', '2013')
			ORDER BY idbensino DESC, idbano";

$dadosIDEB = $db->carregar($sqlIDEB,null,86400);

if( is_array($dadosIDEB) ){
	foreach( $dadosIDEB as $ideb ){
		//inicial
		if( $ideb['intensino'] == 'I' && $ideb['ano'] == 2005 ){
			$metaI2005 = $ideb['meta'];
			$valorI2005 = $ideb['valor'];
		}
		if( $ideb['intensino'] == 'I' && $ideb['ano'] == 2007 ){
			$metaI2007 = $ideb['meta'];
			$valorI2007 = $ideb['valor'];
		}
		if( $ideb['intensino'] == 'I' && $ideb['ano'] == 2009 ){
			$metaI2009 = $ideb['meta'];
			$valorI2009 = $ideb['valor'];
		}
		if( $ideb['intensino'] == 'I' && $ideb['ano'] == 2011 ){
			$metaI2011 = $ideb['meta'];
			$valorI2011 = $ideb['valor'];
		}
		if( $ideb['intensino'] == 'I' && $ideb['ano'] == 2013 ){
			$metaI2013 = $ideb['meta'];
			$valorI2013 = $ideb['valor'];
		}
		
		//Final
		if( $ideb['intensino'] == 'F' && $ideb['ano'] == 2005 ){
			$metaF2005 = $ideb['meta'];
			$valorF2005 = $ideb['valor'];
		}
		if( $ideb['intensino'] == 'F' && $ideb['ano'] == 2007 ){
			$metaF2007 = $ideb['meta'];
			$valorF2007 = $ideb['valor'];
		}
		if( $ideb['intensino'] == 'F' && $ideb['ano'] == 2009 ){
			$metaF2009 = $ideb['meta'];
			$valorF2009 = $ideb['valor'];
		}
		if( $ideb['intensino'] == 'F' && $ideb['ano'] == 2011 ){
			$metaF2011 = $ideb['meta'];
			$valorF2011 = $ideb['valor'];
		}
		if( $ideb['intensino'] == 'F' && $ideb['ano'] == 2013 ){
			$metaF2013 = $ideb['meta'];
			$valorF2013 = $ideb['valor'];
		}
	}
}

# Brasil Alfabetizado
$arrIndBrasilAlfabetizado = array(2581=> 'Alfabetizandos', 2582 => 'Valor de apoio repassado');//, 2582
$arrBrasilAlfabetizado = array();
foreach ($arrIndBrasilAlfabetizado as $indicador => $indnome) {
    $sql = "SELECT 
                dpeid,
                dpedsc,
                sum(qtde)::integer  as dshqtde,
                sum(valor) as dshvalor 
            FROM (
                SELECT 
                    dp.dpeid,
                    d.indid,
                    dp.dpedsc,
                    dp.dpedatainicio,
                    dp.dpedatafim,
                    CASE WHEN d.indcumulativo = 'N' 
                        THEN
                            CASE WHEN (
                                SELECT 
                                    d1.dpeid
                                FROM 
                                    painel.detalheperiodicidade d1
                                INNER JOIN painel.seriehistorica sh ON sh.dpeid=d1.dpeid
                                WHERE d1.dpedatainicio>=dp.dpedatainicio 
                                    AND d1.dpedatafim<=dp.dpedatafim 
                                    AND sh.indid=d.indid
                                    AND sehstatus <> 'I'
                                    AND d1.dpedatainicio >= ( SELECT dpedatainicio FROM painel.detalheperiodicidade WHERE dpeid = 9)
                                    AND d1.dpedatainicio <= ( SELECT dpedatafim FROM painel.detalheperiodicidade WHERE dpeid = 1255)
                                ORDER BY d1.dpedatainicio DESC 
                                limit 1
                            ) = d.dpeid 
                                THEN sum(d.qtde)  
                                ELSE 0 
                            END
                        ELSE sum(d.qtde)
                    END as qtde,
                    CASE WHEN d.indcumulativovalor = 'N' 
                        THEN
                            CASE WHEN (
                                SELECT 
                                    d1.dpeid
                                FROM painel.detalheperiodicidade d1
                                INNER JOIN painel.seriehistorica sh ON sh.dpeid=d1.dpeid
                                WHERE d1.dpedatainicio>=dp.dpedatainicio 
                                    AND d1.dpedatafim<=dp.dpedatafim 
                                    AND sh.indid=d.indid
                                    AND sehstatus <> 'I'
                                    AND d1.dpedatainicio >= ( SELECT dpedatainicio FROM painel.detalheperiodicidade WHERE dpeid = 9)
                                    AND d1.dpedatainicio <= ( SELECT dpedatafim FROM painel.detalheperiodicidade WHERE dpeid = 1255)
                                ORDER BY d1.dpedatainicio desc 
                                LIMIT 1
                            ) = d.dpeid 
                                THEN sum(d.valor)
                                ELSE 0 
                            END
                        ELSE sum(d.valor)
                    END AS valor
                FROM painel.v_detalheindicadorsh d
                INNER JOIN painel.detalheperiodicidade dp ON d.dpedatainicio>=dp.dpedatainicio AND d.dpedatafim<=dp.dpedatafim
                WHERE dp.perid = 3
                    AND d.indid = {$indicador}
                    AND sehstatus <> 'I'
                    AND d.dpedatainicio >= ( SELECT dpedatainicio FROM painel.detalheperiodicidade WHERE dpeid = 9)
                    AND d.dpedatainicio <= ( SELECT dpedatafim FROM painel.detalheperiodicidade WHERE dpeid = 1255)
                    AND d.dshcodmunicipio = '{$muncod}'
                GROUP BY 
                    d.indid,
                    d.dpeid,
                    dp.dpedsc,
                    dp.dpeid,
                    dp.dpedatainicio,
                    dp.dpedatafim,
                    d.indcumulativo,
                    d.indcumulativovalor				
            ) foo
            GROUP BY 
                dpedatainicio,
                dpedatafim,
                dpeid,
                dpedsc,
                indid
            ORDER BY 
                dpedatainicio";
    $arrBrasilAlfabetizado[$indicador.' - '.$indnome] = $db->carregar($sql,null,86400);
}

//$sqlBA = "select 
//				linha, tidid, ciclo, tipo, quantidade, ibge
//			from	(
//				select 1 as linha, 'Alfabetizandos' as tipo, tdi2.tidid, tdi2.tiddsc as ciclo, sum(dsh.dshqtde) as quantidade, dsh.dshcodmunicipio as ibge
//				from painel.indicador i
//				inner join painel.seriehistorica sh on sh.indid = i.indid
//				inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
//				inner join painel.detalhetipodadosindicador tdi2 on tdi2.tidid = dsh.tidid2
//				where i.indid = 2581 AND dsh.dshcodmunicipio = '".$muncod."'
//				and dsh.tidid1 = 806 --Municipal
//				group by ciclo, ibge, tdi2.tidid
//			union all
//				select 2 as linha, 'Valor de apoio repassado' as tipo, tdi1.tidid, tdi1.tiddsc as ciclo, sum(dsh.dshqtde) as quantidade, dsh.dshcodmunicipio as ibge
//				from painel.indicador i
//				inner join painel.seriehistorica sh on sh.indid = i.indid
//				inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
//				inner join painel.detalhetipodadosindicador tdi1 on tdi1.tidid = dsh.tidid1
//				where i.indid = 2582 AND dsh.dshcodmunicipio = '".$muncod."'
//				group by ciclo, ibge, tdi1.tidid
//				) as foo
//			order by ciclo";
//
//$dadosBA = $db->carregar($sqlBA,null,86400);
//
//if( is_array($dadosBA) ){
//	foreach( $dadosBA as $bA ){
//		// Alfabetizandos
//		if( $bA['tidid'] == 804 ){ // Ciclo 2009
//			$quantidade11 = $bA['quantidade'] ? $bA['quantidade'] : 0;
//		}
//		if( $bA['tidid'] == 807 ){ // Ciclo 2010
//			$quantidade12 = $bA['quantidade'] ? $bA['quantidade'] : 0;
//		}
//		if( $bA['tidid'] == 2564 ){ // Ciclo 2011
//			$quantidade13 = $bA['quantidade'] ? $bA['quantidade'] : 0;
//		}
//		if( $bA['tidid'] == 3504){ // Ciclo 2012
//			$quantidade14 = $bA['quantidade'] ? $bA['quantidade'] : 0;
//		}
//		
//		// Valor de apoio repassado
//		if( $bA['tidid'] == 997 ){ // Ciclo 2009
//			$quantidade21 = $bA['quantidade'] ? $bA['quantidade'] : 0;
//		}
//		if( $bA['tidid'] == 998 ){ // Ciclo 2010
//			$quantidade22 = $bA['quantidade'] ? $bA['quantidade'] : 0;
//		}
//		if( $bA['tidid'] == 1004 ){ // Ciclo 2011
//			$quantidade23 = $bA['quantidade'] ? $bA['quantidade'] : 0;
//		}
//		if( $bA['tidid'] == 4134 ){ // Ciclo 2012
//			$quantidade24 = $bA['quantidade'] ? $bA['quantidade'] : 0;
//		}
//	}
//}

// Sala de Recurso Multifuncionais
$sqlRecMult = "select 
					dp.dpeanoref as ano, count(distinct dsh.dshcod) as totalescolas, sum(dsh.dshqtde) as quantidade, dsh.dshcodmunicipio as ibge
				from painel.indicador i
				inner join painel.seriehistorica sh on sh.indid = i.indid
				inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
				inner join painel.detalheperiodicidade dp on dp.dpeid = sh.dpeid
				inner join painel.detalhetipodadosindicador tdi1 on tdi1.tidid = dsh.tidid1
				where i.indid = 268 AND dsh.dshcodmunicipio = '".$muncod."'
				and dsh.tidid2 = 3105 --Municipal
				group by ano, dsh.dshcodmunicipio
				order by ano";

$dadosRecMult = $db->carregar($sqlRecMult,null,86400);

$dadosRecMult = $dadosRecMult ? $dadosRecMult : array();

// necessita depara stoid = esdid
// Lista de Obras no Munic�pio
//$sqlListaObras = "select 
//						o.obrdesc as nome, t.tpodsc as tipologia, stodesc as situacao, o.obrpercexec, 
//						CASE WHEN stodesc <> 'Conclu�da' THEN
//							CASE WHEN o.obrdtvistoria IS NOT NULL THEN
//								    (DATE_PART('days', NOW() - o.obrdtvistoria))
//							ELSE
//								    (DATE_PART('days', NOW() - o.obsdtinclusao))
//							END 
//						END AS diassematualizacao, 
//						e.muncod, 
//						CASE WHEN o.obrvlrrealobra IS NOT NULL THEN CASE WHEN o.obrvlrrealobra > 0 THEN o.obrvlrrealobra ELSE o.obrvalorprevisto END ELSE o.obrvalorprevisto END as valor_aprovado,
//						(SELECT
//								CASE WHEN eo.preid IS NOT NULL THEN sum( eo.eobvalorempenho ) ELSE sum( eop.eobvalorempenho ) END as valor_empenhado
//							FROM
//								obras.preobra po
//							left join par.empenhoobra eo 
//								inner join par.empenho emp ON emp.empid = eo.empid  and eo.eobstatus = 'A'
//							ON eo.preid = po.preid
//							left join par.empenhoobrapar eop 
//								inner join par.empenho emp2 ON emp2.empid = eop.empid AND emp2.empsituacao <> 'CANCELADO' and eop.eobstatus = 'A'
//							ON eop.preid = po.preid
//							WHERE
//								po.preid = o.preid
//							GROUP BY
//								eo.preid) as valor_empenhado,
//						(SELECT
//								CASE WHEN po.preid IS NOT NULL THEN sum( po.pobvalorpagamento ) ELSE sum( pop.popvalorpagamento ) END as valor_pago
//							FROM
//								obras.preobra p
//							left join par.pagamentoobra po 
//								inner join par.pagamento pag ON pag.pagid = po.pagid AND pag.pagsituacaopagamento not ilike '%CANCELADO%'
//							ON po.preid = p.preid
//							left join par.pagamentoobrapar pop 
//								inner join par.pagamento pag2 ON pag2.pagid = pop.pagid AND pag2.pagsituacaopagamento not ilike '%CANCELADO%'
//							ON pop.preid = p.preid
//							WHERE
//								p.preid = o.preid
//							GROUP BY
//								po.preid) as valor_pago	--CASE WHEN po.preid IS NOT NULL THEN sum( po.pobvalorpagamento ) ELSE sum( pop.popvalorpagamento ) END as valor_pago
//					from 
//						obr as.ob rainfraestrutura o
//					inner join obras.situacaoobra s on s.stoid = o.stoid
//					inner join entidade.endereco e on e.endid = o.endid
//					left join obras.tipologiaobra t on t.tpoid = o.tpoid
//					where 
//						o.orgid = 3
//						and o.obsstatus = 'A'
//						and o.obrtipoesfera = 'M'
//						and o.cloid is not null
//						and o.stoid in (2,10,9,99,5,1,3)
//						AND e.muncod = '".$muncod."'";
						
$sqlListaObras = "SELECT 
						o.obrnome AS nome, t.tpodsc AS tipologia, 
						esddsc AS situacao, 
						o.obrpercentultvistoria, 
						CASE WHEN esd.esdid <> 693 THEN
							CASE WHEN o.obrdtvistoria IS NOT NULL THEN
								    (DATE_PART('days', NOW() - o.obrdtvistoria))
							ELSE
								    (DATE_PART('days', NOW() - o.obrdtinclusao))
							END 
						END AS diassematualizacao, 
						e.muncod, 
						--CASE WHEN o.obrvlrrealobra IS NOT NULL THEN CASE WHEN o.obrvlrrealobra > 0 THEN o.obrvlrrealobra ELSE o.obrvalorprevisto END ELSE o.obrvalorprevisto END as valor_aprovado,
						(SELECT
								CASE WHEN eo.preid IS NOT NULL THEN sum( eo.eobvalorempenho ) ELSE sum( eop.eobvalorempenho ) END as valor_empenhado
							FROM
								obras.preobra po
							LEFT JOIN par.empenhoobra eo 
								INNER JOIN par.empenho emp ON emp.empid = eo.empid  and eo.eobstatus = 'A' and empstatus = 'A'
							ON eo.preid = po.preid
							LEFT JOIN par.empenhoobrapar eop 
								INNER JOIN par.empenho emp2 ON emp2.empid = eop.empid AND emp2.empsituacao <> 'CANCELADO' and eop.eobstatus = 'A' and empstatus = 'A'
							ON eop.preid = po.preid
							WHERE
								po.preid = o.preid
							GROUP BY
								eo.preid) as valor_empenhado,
						(SELECT
								CASE WHEN po.preid IS NOT NULL THEN sum( po.pobvalorpagamento ) ELSE sum( pop.popvalorpagamento ) END as valor_pago
							FROM
								obras.preobra p
							LEFT JOIN par.pagamentoobra po 
								INNER JOIN par.pagamento pag ON pag.pagid = po.pagid AND pag.pagsituacaopagamento not ilike '%CANCELADO%' AND pag.pagstatus = 'A'
							ON po.preid = p.preid
							LEFT JOIN par.pagamentoobrapar pop 
								INNER JOIN par.pagamento pag2 ON pag2.pagid = pop.pagid AND pag2.pagsituacaopagamento not ilike '%CANCELADO%' AND pag2.pagstatus = 'A'
							ON pop.preid = p.preid
							WHERE
								p.preid = o.preid
							GROUP BY
								po.preid) as valor_pago	--CASE WHEN po.preid IS NOT NULL THEN sum( po.pobvalorpagamento ) ELSE sum( pop.popvalorpagamento ) END as valor_pago
					FROM 
						obras2.obras o
					INNER JOIN workflow.documento doc ON doc.docid = o.docid
					INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
					INNER JOIN obras2.empreendimento emp ON emp.empid = o.empid
					INNER JOIN entidade.endereco e ON e.endid = o.endid
					LEFT  JOIN obras2.tipologiaobra t ON t.tpoid = o.tpoid
					WHERE 
						emp.orgid = 3
						AND o.obrstatus = 'A'
						AND emp.empesfera = 'M'
						AND o.cloid is not null
						AND doc.esdid in (690, 691, 693, 763, 768, 769, 689)
						AND e.muncod = '$muncod'";

$dadosListaObras = $db->carregar($sqlListaObras,null,86400);

// Presta��o de Contas
$sqlPC = "select distinct
			tdi1.tiddsc as programa, 
			CASE WHEN dpeanoref = '2011' THEN tdi2.tiddsc END as situacao2011, 
			CASE WHEN dpeanoref = '2012' THEN tdi2.tiddsc END as situacao2012, 
			CASE WHEN dpeanoref = '2013' THEN tdi2.tiddsc END as situacao2013, 
			sum(dsh.dshqtde) as quantidade, 
			dsh.dshcodmunicipio as ibge
		from painel.indicador i
		inner join painel.seriehistorica sh on sh.indid = i.indid
		inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
		inner join painel.detalhetipodadosindicador tdi1 on tdi1.tidid = dsh.tidid1
		inner join painel.detalhetipodadosindicador tdi2 on tdi2.tidid = dsh.tidid2
		inner join painel.detalheperiodicidade dep on dep.dpeid = sh.dpeid
		where i.indid = 1857 AND dsh.dshcodmunicipio = '".$muncod."'
		--and sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid)
		and dpeanoref IN ( '2011', '2012', '2013' )
		and sh.sehstatus <> 'I'
		group by tdi1.tiddsc, tdi2.tiddsc, dsh.dshcodmunicipio, dpeanoref
		order by tdi1.tiddsc, situacao2011, situacao2012, dsh.dshcodmunicipio  
			";

$prestContas = $db->carregar($sqlPC);

//Obras do PAC 2 � Cotas para sele��o 2013.

$sqlCotas = "select tipo, quantidade, ibge, tidid
			from      (
			            select 
			            	1 as tidid, 
			            	'Creches e Pr�-escolas' as tipo, 
			            	sum(dsh.dshqtde) as quantidade, 
			            	dsh.dshcodmunicipio as ibge
			            from painel.indicador i
			            inner join painel.seriehistorica sh on sh.indid = i.indid
			            inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
			            where i.indid = 1567 AND dsh.dshcodmunicipio = '".$muncod."'
			            and sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid)
			            group by tipo, dsh.dshcodmunicipio
			union all
			            select
			            	tdi1.tidid,
							tdi1.tiddsc as tipo, 
							sum(dsh.dshqtde) as quantidade, 
							dsh.dshcodmunicipio as ibge
			            from painel.indicador i
			            inner join painel.seriehistorica sh on sh.indid = i.indid
			            inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
			            inner join painel.detalhetipodadosindicador tdi1 on tdi1.tidid = dsh.tidid1
			            where i.indid = 1568 AND dsh.dshcodmunicipio = '".$muncod."'
			            and sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid)
			            group by tipo, dsh.dshcodmunicipio, tdi1.tidid
			) as foo
			order by ibge";

$dadosCotas = $db->carregar($sqlCotas);

$dadosCotas = $dadosCotas ? $dadosCotas : array();

foreach( $dadosCotas as $cota ){
	if( $cota['tidid'] == 3886 ){ //"Cobertura de Quadra"
		$cotaCobertura = $cota['quantidade'];
	}
	if( $cota['tidid'] == 3885 ){ //"Constru��o de Quadra Coberta"
		$cotaConstrucao = $cota['quantidade'];
	}
	if( $cota['tidid'] == 1 ){ //"Creches e Pr�-escolas"
		$cotaCreche = $cota['quantidade'];
	}
}

$sqlRedeMunicipal = "select 
						count(*) 
					from 
						sispacto.identificacaousuario i 
					inner join sispacto.tipoperfil t on t.iusd = i.iusd and t.pflcod=849 
					inner join sispacto.pactoidadecerta p on p.picid = i.picid 
					where 
						p.muncod='".$muncod."'";

$redeMunicipal = $db->pegaUm($sqlRedeMunicipal);
			
$sqlRedeEstadual = "select 
						count(*) 
					from 
						sispacto.identificacaousuario i 
					inner join sispacto.tipoperfil t on t.iusd = i.iusd and t.pflcod=849 
					inner join sispacto.pactoidadecerta p on p.picid = i.picid 
					where 
						i.muncodatuacao='".$muncod."' and 
						p.estuf is not null";

$redeEstadual = $db->pegaUm($sqlRedeEstadual);

// PDE-Interativo
$sqlPDEint = "select tipo, quantidade, ibge
			from      (
			            select 1 as tipo, sum(dsh.dshqtde) as quantidade, dsh.dshcodmunicipio as ibge
			            from painel.indicador i
			            inner join painel.seriehistorica sh on sh.indid = i.indid
			            inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
			            inner join painel.detalheperiodicidade dp on dp.dpeid = sh.dpeid
			            where i.indid = 1576 AND dsh.dshcodmunicipio = '".$muncod."'
			            and dp.dpeanoref = '2012'
			            group by tipo, ibge
			union all
			            select 2 as tipo, sum(dsh.dshqtde) as quantidade, dsh.dshcodmunicipio as ibge
			            from painel.indicador i
			            inner join painel.seriehistorica sh on sh.indid = i.indid
			            inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
			            inner join painel.detalheperiodicidade dp on dp.dpeid = sh.dpeid
			            where i.indid = 1577 AND dsh.dshcodmunicipio = '".$muncod."'
			            and dp.dpeanoref = '2012'
			            group by tipo, ibge
			union all
			            select 3 as tipo, sum(dsh.dshqtde) as quantidade, dsh.dshcodmunicipio as ibge
			            from painel.indicador i
			            inner join painel.seriehistorica sh on sh.indid = i.indid
			            inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
			            inner join painel.detalheperiodicidade dp on dp.dpeid = sh.dpeid
			            where i.indid = 1578 AND dsh.dshcodmunicipio = '".$muncod."'
			            and dp.dpeanoref = '2012'
			            group by tipo, ibge
			union all
			            select 4 as tipo, sum(dsh.dshqtde) as quantidade, dsh.dshcodmunicipio as ibge
			            from painel.indicador i
			            inner join painel.seriehistorica sh on sh.indid = i.indid
			            inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
			            inner join painel.detalheperiodicidade dp on dp.dpeid = sh.dpeid
			            where i.indid = 1579 AND dsh.dshcodmunicipio = '".$muncod."'
			            and dp.dpeanoref = '2012'
			            group by tipo, ibge
			union all
			            select 5 as tipo, sum(dsh.dshvalor) as quantidade, dsh.dshcodmunicipio as ibge
			            from painel.indicador i
			            inner join painel.seriehistorica sh on sh.indid = i.indid
			            inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
			            inner join painel.detalheperiodicidade dp on dp.dpeid = sh.dpeid
			            where i.indid = 1579 AND dsh.dshcodmunicipio = '".$muncod."'
			            and dp.dpeanoref = '2012'
			            group by tipo, ibge
			            ) as foo
			order by ibge";

$dadosPDEInterativo = $db->carregar($sqlPDEint,null,86400);

$dadosPDEInterativo = $dadosPDEInterativo ? $dadosPDEInterativo : array();

foreach( $dadosPDEInterativo as $dadoPDE ){
	if( $dadoPDE['tipo'] == 1 ){
		$PDE1 = $dadoPDE['quantidade'];
	}
	if( $dadoPDE['tipo'] == 2 ){
		$PDE2 = $dadoPDE['quantidade'];
	}
	if( $dadoPDE['tipo'] == 3 ){
		$PDE3 = $dadoPDE['quantidade'];
	}
	if( $dadoPDE['tipo'] == 4 ){
		$PDE4 = $dadoPDE['quantidade'];
	}
	if( $dadoPDE['tipo'] == 5 ){
		$PDE5 = $dadoPDE['quantidade'];
	}
}

// Mobiliario 2010 - 2011
$sqlMobiliario20102011 = "select dp.dpeanoref as ano, sum(dsh.dshqtde) as quantidade, sum(dsh.dshvalor) as valor, dsh.dshcodmunicipio as ibge
							from painel.indicador i
							inner join painel.seriehistorica sh on sh.indid = i.indid
							inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
							inner join painel.detalheperiodicidade dp on dp.dpeid = sh.dpeid
							where i.indid = 648 AND dsh.dshcodmunicipio = '".$muncod."'
							and dsh.tidid1 = 2206 --Recurso FNDE
							and dsh.tidid2 = 2209 --Municipal
							and dp.dpeanoref between '2010' and '2011'
							group by ano, dsh.dshcodmunicipio
							order by ano";

$dadosMobiliario20102011 = $db->carregar($sqlMobiliario20102011,null,86400);

$dadosMobiliario20102011 = $dadosMobiliario20102011 ? $dadosMobiliario20102011 : array();

foreach($dadosMobiliario20102011 as $dadoMob20102011){
	if( $dadoMob20102011['ano'] == 2010 ){
		$mob2010qtd = $dadoMob20102011['quantidade'];
		$mob2010vlr = $dadoMob20102011['valor'];
	}
	if( $dadoMob20102011['ano'] == 2011 ){
		$mob2011qtd = $dadoMob20102011['quantidade'];
		$mob2011vlr = $dadoMob20102011['valor'];
	}
}

// Mobiliario 2013
$sqlMobiliario2012 = "select tdi2.tidid, tdi2.tiddsc as tipo, sum(dsh.dshqtde) as quantidade, 
					            (select 
					            	sum(dsh1.dshqtde)
					            from painel.indicador i1
					            inner join painel.seriehistorica sh1 on sh1.indid = i1.indid
					            inner join painel.detalheseriehistorica dsh1 on dsh1.sehid = sh1.sehid
					            inner join painel.detalheperiodicidade dp1 on dp1.dpeid = sh1.dpeid
					            where i1.indid = 1574
					            and dsh1.tidid1 = 3881 --Municipal
					            and dp1.dpeanoref = '2013'
					            and dsh1.dshcodmunicipio = dsh.dshcodmunicipio
					            group by dsh1.dshcodmunicipio) as valor,
								dsh.dshcodmunicipio as ibge
					from painel.indicador i
					inner join painel.seriehistorica sh on sh.indid = i.indid
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalhetipodadosindicador tdi2 on tdi2.tidid = dsh.tidid2
					inner join painel.detalheperiodicidade dp on dp.dpeid = sh.dpeid
					where i.indid = 1570 AND dsh.dshcodmunicipio = '".$muncod."'
					and dsh.tidid1 = 3863 --Municipal
					and dp.dpeanoref = '2013'
					group by tdi2.tiddsc, dsh.dshcodmunicipio, tdi2.tidid
					order by dsh.dshcodmunicipio";

$dadosMobiliario2012 = $db->carregar($sqlMobiliario2012,null,86400);

$dadosMobiliario2012 = $dadosMobiliario2012 ? $dadosMobiliario2012 : array();

foreach( $dadosMobiliario2012 as $dadoMob2012 ){
	if( $dadoMob2012['valor'] ){
		$valorMobiliario2012 = $dadoMob2012['valor'];
	}
	if( $dadoMob2012['tidid'] == 3865 ){ //Conjunto Aluno - tamanho 3
		$CJA03qtd = $dadoMob2012['quantidade'];
	}
	if( $dadoMob2012['tidid'] == 3871 ){ //Conjunto Aluno - tamanho 4
		$CJA04qtd = $dadoMob2012['quantidade'];
	}
	if( $dadoMob2012['tidid'] == 3872 ){ //Conjunto Aluno - tamanho 6
		$CJA06qtd = $dadoMob2012['quantidade'];
	}
	if( $dadoMob2012['tidid'] == 3867 ){ //Conjunto professor
		$CJPqtd = $dadoMob2012['quantidade'];
	}
	if( $dadoMob2012['tidid'] == 3866 ){ //Conjunto inform�tica M2C-04
		$M2C4qtd = $dadoMob2012['quantidade'];
	}
	if( $dadoMob2012['tidid'] == 3873 ){ //Conjunto inform�tica M2C-06
		$M2C6qtd = $dadoMob2012['quantidade'];
	}
	if( $dadoMob2012['tidid'] == 3868 ){ //Mesa acess�vel
		$MAqtd = $dadoMob2012['quantidade'];
	}
}


//PDDE
$sqlPDDE = "select 
				dp.dpeanoref as ano, COUNT(dsh.dshcod) as escolasbeneficiadas, sum(dsh.dshqtde) as recursosrepassados, dsh.dshcodmunicipio as ibge
			from painel.indicador i
			inner join painel.seriehistorica sh on sh.indid = i.indid
			inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
			inner join painel.detalheperiodicidade dp on dp.dpeid = sh.dpeid
			inner join entidade.entidade e on e.entcodent = dsh.dshcod and e.entstatus = 'A'
			where i.indid = 410 AND dsh.dshcodmunicipio = '".$muncod."'
			and e.tpcid = 3 --Municipal
			and dp.dpeanoref between '2011' and '2014'
			group by ano, ibge
			order by ano";
$dadosPDDE = $db->carregar($sqlPDDE,null,86400);

$dadosPDDE = $dadosPDDE ? $dadosPDDE : array();

foreach($dadosPDDE as $dadoPDDE){
	if( $dadoPDDE['ano'] == 2011 ){
		$PDDEescolasben2011 = $dadoPDDE['escolasbeneficiadas'];
		$PDDErecursosrep2011 = $dadoPDDE['recursosrepassados'];
	}
	if( $dadoPDDE['ano'] == 2012 ){
		$PDDEescolasben2012 = $dadoPDDE['escolasbeneficiadas'];
		$PDDErecursosrep2012 = $dadoPDDE['recursosrepassados'];
	}
	if( $dadoPDDE['ano'] == 2013 ){
		$PDDEescolasben2013 = $dadoPDDE['escolasbeneficiadas'];
		$PDDErecursosrep2013 = $dadoPDDE['recursosrepassados'];
	}
	if( $dadoPDDE['ano'] == 2014 ){
		$PDDEescolasben2014 = $dadoPDDE['escolasbeneficiadas'];
		$PDDErecursosrep2014 = $dadoPDDE['recursosrepassados'];
	}
}

$sqlSituacaoBA = "select tdi1.tiddsc as situacao, sum(dsh.dshqtde) as quantidade, dsh.dshcodmunicipio as ibge
					from painel.indicador i
					inner join painel.seriehistorica sh on sh.indid = i.indid
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalhetipodadosindicador tdi1 on tdi1.tidid = dsh.tidid1
					where i.indid = 1573 AND dsh.dshcodmunicipio = '".$muncod."'
					and sh.dpeid = 1148
					group by tdi1.tiddsc, dsh.dshcodmunicipio
					order by tdi1.tiddsc, dsh.dshcodmunicipio";

$dadosSituacaoBA = $db->pegaLinha($sqlSituacaoBA,0,86400);


//Educa��o Jovens e Adultos
$sqlJovensAdultos = "select 
						'Aderiu' as situacao, 
						sum(dsh.dshqtde) as quantidade, 
						sum(dsh.dshvalor) as valor, 
						dsh.dshcodmunicipio as ibge
					from painel.indicador i
					inner join painel.seriehistorica sh on sh.indid = i.indid
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalhetipodadosindicador tdi1 on tdi1.tidid = dsh.tidid1
					inner join painel.detalheperiodicidade dp on dp.dpeid = sh.dpeid
					where i.indid = 1572 AND dsh.dshcodmunicipio = '".$muncod."'
					and dsh.tidid1 = 3875 --Municipal
					and dp.dpeanoref = '2012'
					group by dsh.dshcodmunicipio
					order by dsh.dshcodmunicipio";
					
$dadosJovensAdultos = $db->pegaLinha($sqlJovensAdultos,0,86400);

#Comentado por Jair FOro: Altera��o no formato de exibicao utilizando o ano e indicado
// PACTO NACIONAL
//$sqlPactoNacional = "select 
//						tdi1.tiddsc as tipo, 
//						sum(dsh.dshqtde) as quantidade, 
//						dsh.dshcodmunicipio as ibge
//					from painel.indicador i
//					inner join painel.seriehistorica sh on sh.indid = i.indid
//					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
//					inner join painel.detalhetipodadosindicador tdi1 on tdi1.tidid = dsh.tidid1
//					where i.indid = 2840 AND dsh.dshcodmunicipio = '".$muncod."'
//					and sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid)
//						group by tdi1.tiddsc, dsh.dshcodmunicipio
//						order by tdi1.tiddsc, dsh.dshcodmunicipio";
//
//$pactoNacional = $db->carregar($sqlPactoNacional,0,86400);
//
//if(is_array($pactoNacional)){
//	$pactonacionaltx = "";
//	foreach( $pactoNacional as $pacto ){
//		$pactonacionaltx .= $pacto['tipo'].": ". ( $pacto['quantidade'] ? simec_number_format($pacto['quantidade'], 0) : '-' )." <br> ";		
//	}
//}

//Brasil Carinhoso
/*$sqlBrasilCarinhoso = "(SELECT 
						                i.indid, tdi1.tidid as tidid1, tdi1.tiddsc as situacao, sum(dsh.dshqtde) as quantidade, dsh.dshcodmunicipio as ibge
						FROM 
						                painel.indicador i
						inner join painel.seriehistorica sh on sh.indid = i.indid
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						inner join painel.detalhetipodadosindicador tdi1 on tdi1.tidid = dsh.tidid1
						WHERE 
						                i.indid = 1883 AND
						                dsh.dshcodmunicipio = '".$muncod."' --AND
						                --sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid)
						GROUP BY 
						                tdi1.tiddsc, dsh.dshcodmunicipio, tdi1.tidid, i.indid
						ORDER BY 
						                tdi1.tiddsc,  dsh.dshcodmunicipio)
						                
						UNION ALL
						
						(SELECT 
						                i.indid, tdi1.tidid as tidid1, tdi1.tiddsc as situacao, sum(dsh.dshqtde) as quantidade, dsh.dshcodmunicipio as ibge
						FROM 
						                painel.indicador i
						inner join painel.seriehistorica sh on sh.indid = i.indid
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						inner join painel.detalhetipodadosindicador tdi1 on tdi1.tidid = dsh.tidid1
						WHERE 
						                i.indid = 1884 AND
						                dsh.dshcodmunicipio = '".$muncod."' --AND
						                --sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid)
						GROUP BY 
						                tdi1.tiddsc, dsh.dshcodmunicipio, tdi1.tidid, i.indid
						ORDER BY 
						                tdi1.tiddsc,  dsh.dshcodmunicipio)
						
						UNION ALL
						
						(SELECT 
						                i.indid, tdi1.tidid as tidid1, tdi1.tiddsc as situacao, sum(dsh.dshqtde) as quantidade, dsh.dshcodmunicipio as ibge
						FROM 
						                painel.indicador i
						inner join painel.seriehistorica sh on sh.indid = i.indid
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						inner join painel.detalhetipodadosindicador tdi1 on tdi1.tidid = dsh.tidid1
						WHERE 
						                i.indid = 2014 AND
						                dsh.dshcodmunicipio = '".$muncod."' --AND
						                --sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid)
						GROUP BY 
						                tdi1.tiddsc, dsh.dshcodmunicipio, tdi1.tidid, i.indid
						ORDER BY 
						                tdi1.tiddsc,  dsh.dshcodmunicipio)
";

$dadosBC = $db->carregar($sqlBrasilCarinhoso,null,86400);

$dadosBC = $dadosBC ? $dadosBC : array();
*/
/*foreach( $dadosBC as $bcdado ){
	if( $bcdado['indid'] == 1883 ){
		if( $bcdado['tidid1'] == 8090 ){ // Analisado
			$bc1Analisado = $bcdado['quantidade'];
		}
		if( $bcdado['tidid1'] == 8089 ){ // Aguardando pagamento
			$bc1AguarPag = $bcdado['quantidade'];
		}
		if( $bcdado['tidid1'] == 4365 ){ // Pagamento efetuado
			$bc1PagEfet = $bcdado['quantidade'];
		}
	} 
	if( $bcdado['indid'] == 1884 ){
		if( $bcdado['tidid1'] == 8091 ){ // Cadastramento
			$bc2Cadastramento = $bcdado['quantidade'];
		}
		if( $bcdado['tidid1'] == 4369 ){ // Dilig�ncia
			$bc2Diligencia = $bcdado['quantidade'];
		}
		if( $bcdado['tidid1'] == 4370 ){ // Pagamento efetuado
			$bc2PagEfet = $bcdado['quantidade'];
		}
		if( $bcdado['tidid1'] == 4371 ){ // Analisado
			$bc2Analisado = $bcdado['quantidade'];
		}
		if( $bcdado['tidid1'] == 4372 ){ // Aguardando pagamento
			$bc2AguarPag = $bcdado['quantidade'];
		}
	}
	if( $bcdado['indid'] == 2014 ){
		if( $bcdado['tidid1'] == 8094 ){ // Cadastramento
			$bc3Cadastramento = $bcdado['quantidade'];
		}
		if( $bcdado['tidid1'] == 8097 ){ // Dilig�ncia
			$bc3Diligencia = $bcdado['quantidade'];
		}
		if( $bcdado['tidid1'] == 8096 ){ // Pagamento efetuado
			$bc3PagEfet = $bcdado['quantidade'];
		}
		if( $bcdado['tidid1'] == 4570 ){ // Analisado
			$bc3Analisado = $bcdado['quantidade'];
		}
		if( $bcdado['tidid1'] == 4569 ){ // Aguardando pagamento
			$bc3AguarPag = $bcdado['quantidade'];
		}
	}
}*/

$sqlPrefeito = "SELECT DISTINCT
					u.usunome
				FROM
					par.usuarioresponsabilidade ur
				    inner join seguranca.usuario u on u.usucpf = ur.usucpf
				    inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf
				WHERE
					ur.pflcod = 556
					and us.sisid = 23
				    and ur.muncod = '".$muncod."'
				    and ur.rpustatus = 'A'
				    and us.suscod = 'A'";

$prefeito = $db->pegaUm($sqlPrefeito,null,86400);

$sql = "SELECT 
			dpe.dpeanoref AS ano, 
		    dsh.dshcodmunicipio AS ibge, 
		    SUM(cast(dsh.dshqtde as integer)) AS quantidade, 
		    SUM(dsh.dshvalor) AS valor
		FROM painel.seriehistorica sh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
			INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
		WHERE 
			sh.indid IN (1883)
			AND sh.sehstatus <> 'I'
			and dsh.dshcodmunicipio = '$muncod'
		    and dpe.dpeanoref  = '2013'
		GROUP BY ano, ibge
		ORDER BY ano";
$bcIndicador2013 = $db->pegaLinha($sql);

$sql = "SELECT 
			dpe.dpeanoref AS ano, 
		    dsh.dshcodmunicipio AS ibge, 
		    SUM(cast(dsh.dshqtde as integer)) AS quantidade, 
		    SUM(dsh.dshvalor) AS valor
		FROM painel.seriehistorica sh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
			INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
		WHERE 
			sh.indid IN (1883)
			AND sh.sehstatus <> 'I'
			and dsh.dshcodmunicipio = '$muncod'
		    and dpe.dpeanoref  = '2014'
		GROUP BY ano, ibge
		ORDER BY ano";
$bcIndicador2014 = $db->pegaLinha($sql);

$sql = "SELECT 
			tab.turano,
		    SUM(tab.emcadastramento) as emcadastramento,
		   	SUM(tab.emanalise) as emanalise,
		   	SUM(tab.emdiligencia) as emdiligencia,
		   	SUM(tab.aguardandopagamento) as aguardandopagamento,
		   	SUM(tab.pagamentoefetuado) as pagamentoefetuado
		FROM
			territorios.municipio mun
		    LEFT JOIN 	proinfantil.novasturmasmunicipios ntm on ntm.muncod = mun.muncod
		    LEFT JOIN	(SELECT ntw.muncod, doc.esdid, t.turano,
		                        CASE WHEN doc.esdid = 535 THEN COUNT(ntw.turid) ELSE '0' END as emcadastramento, 
		                        CASE WHEN doc.esdid = 536 THEN COUNT(ntw.turid) ELSE '0' END as emanalise, 
		                        CASE WHEN doc.esdid = 587 THEN COUNT(ntw.turid) ELSE '0' END as emdiligencia, 
		                        CASE WHEN doc.esdid = 586 THEN COUNT(ntw.turid) ELSE '0' END as aguardandopagamento, 
		                        CASE WHEN doc.esdid = 599 THEN COUNT(ntw.turid) ELSE '0' END as pagamentoefetuado
		                   FROM	proinfantil.novasturmasworkflowturma ntw
		                   		inner join workflow.documento doc ON doc.docid = ntw.docid
		                   		inner join proinfantil.turma t on t.turid = ntw.turid and t.turstatus = 'A' and t.turano in ('2014', '2015')
		                   GROUP BY	doc.esdid, ntw.muncod, t.turano) as tab ON tab.muncod = ntm.muncod
		WHERE
			mun.muncod = '$muncod'
		GROUP BY tab.turano";
$dadosNovasTurmas = $db->carregar($sql);
$dadosNovasTurmas = $dadosNovasTurmas ? $dadosNovasTurmas : array();
$arrNovasTurmas = array();
foreach ($dadosNovasTurmas as $v) {
	$arrNovasTurmas[$v['turano']] = $v;
}

$sql = "SELECT 
		    CASE WHEN esdid = 369 THEN count(obrid) ELSE '0' END as emcadastramento, 
		    CASE WHEN esdid = 370 THEN count(obrid) ELSE '0' END as emanalise, 
		    CASE WHEN esdid = 518 THEN count(obrid) ELSE '0' END as emdiligencia, 
		    CASE WHEN esdid = 372 THEN count(obrid) ELSE '0' END as aguardandopagamento, 
		    CASE WHEN esdid = 373 THEN count(obrid) ELSE '0' END as pagamentoefetuado
		from(
			SELECT
		        tm.estuf as UF,
		        tm.mundescricao as municipio,
		        oi.obrid,
		        edoc.esddsc as descricaowork,
		        edoc.esdid
		    FROM
		        obras2.obras AS oi
		        INNER JOIN obras2.empreendimento e ON e.empid =  oi.empid
		        INNER JOIN entidade.entidade AS ee ON oi.entid= ee.entid
		        INNER JOIN entidade.endereco AS ed ON oi.endid = ed.endid
		        INNER JOIN territorios.municipio AS tm ON ed.muncod = tm.muncod                                         
		        INNER JOIN workflow.documento d1 ON d1.docid = oi.docid
		        INNER JOIN workflow.estadodocumento esd1 on esd1.esdid = d1.esdid
		        INNER JOIN obras2.programafonte AS pf ON e.prfid = pf.prfid
		        LEFT JOIN obras2.tipologiaobra AS tpl ON oi.tpoid = tpl.tpoid AND tpl.tpostatus = 'A'
		        LEFT JOIN proinfantil.proinfantil pi ON pi.obrid = oi.obrid AND pi.obrid IS NOT NULL 			
                        INNER JOIN workflow.documento d ON d.docid = pi.docid
                        INNER JOIN workflow.estadodocumento edoc on edoc.esdid = d.esdid
		    WHERE
		        oi.obrstatus = 'A' AND
		        ee.entstatus = 'A' AND
		        pf.prfid = 41 AND
		        oi.obrpercentultvistoria >= 90 AND
		        esd1.esdid IN (690, 693) and
		        oi.obridpai IS NULL
		        and tpl.tpoid in (16,9,10,104,105)
		        and tm.muncod = '$muncod'
		    ORDER BY
		        ee.entnome
		) as foo
		group by esdid";
//ver($sql, d);
$arrProinfantil = $db->pegaLinha($sql);
?>
<html>
<head>
	<!--<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
	--><meta http-equiv="Content-Type" content="text/html;">
	<title>SIMEC - Munic�pios Fortes, Brasil Sustent�vel</title>
	<style type="text/css">

		@media print {.notprint { display: none } .div_rolagem{display: none !important;} }	
		
		.div_rolagem{ overflow-x: auto; overflow-y: auto; width: 30px;}
		
		.quebra    { 
			page-break-after: always !important;
			height: 20px; 
			clear: both;
		}
	</style>
</head>
<body>
	<?php if( 1 == 2 ){ // ESCONDENDO A CAPA (A PEDIDO DO DANIEL NO DIA 08/04/2013) ?>
	<table border="0" align="left" width="100%" cellspacing="0" cellpadding="0" style="font-family: Calibri">
		<thead>
		<tr>
			<td>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
					<tr>
						<td style="text-align: right;"><div style="position:absolute; width:300px; height:115px; z-index:1; left: 380px; top: 80px;"><b><? echo $nomeEst.'<br>'.$nomeMun; ?></b></div></td>
					</tr>
				</table>
			</td>
		</tr>
		</thead>
	</table>
	
	<!-- Quebra Pagina -->	
	<div class="quebra"></div>
	
	<table border="0" align="left" width="100%" cellspacing="0" cellpadding="0" style="font-family: Calibri">
		<thead>
		<tr>
			<td>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
					<tr>
						<td style="text-align: right;"><div style="position:absolute; width:300px; height:115px; z-index:1; left: 380px; top: 80px;"><font color="white"><b><? echo $nomeEst.'<br>'.$nomeMun; ?></b></font></div><img src="imagem/capa.jpg" width="710px" height="930" alt="" ></img></td>
					</tr>
				</table>
			</td>
		</tr>
		</thead>
	</table>
	
	<!-- Quebra Pagina -->	
	<div class="quebra"></div>
<?php } ?>
<?php if(!$geraPdf){ ?>
<img class="div_rolagem" src="imagem/print2.gif" onclick="window.print()" title="Imprimir" style="cursor: pointer"></img>
<?php } ?>
	<table border="0" align="left" width="100%" cellspacing="0" cellpadding="0" style="font-family: Calibri">
		<thead>
		<tr>
			<td>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
					<tr>
						<td><img src="imagem/cabecalho-7.jpg" width="710px" alt="" ></img></td>
					</tr>
					<tr style="color: white; background-color: #00008B; text-align: center; font-size: 17px">
						<td><?php echo 'Munic�pio: <b>'.$arrDadosMunicipio['nome'].' - '.$arrDadosMunicipio['uf'].'</b>'; ?></td>
					</tr>
				</table>
			</td>
		</tr>
		</thead>
		<tbody>
		<tr>
			<td>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
					<tr>
						<td align="justify" style="font-family: Calibri"><br><b>1) Sistemas e habilita��o � MEC / FNDE</b><br/><br/>
							Para que os munic�pios possam acessar os recursos dos diferentes programas do Minist�rio da Educa��o, � imprescind�vel que o munic�pio esteja habilitado 
							no Fundo Nacional de Desenvolvimento da Educa��o � FNDE e cadastrado nos sistemas corporativos do �rg�o. A habilita��o almeja consolidar os documentos legais para efetiva��o das transfer�ncias de 
							recursos pelo FNDE. J� os sistemas informatizados s�o a porta de entrada para cadastramento de projetos, planejamento das a��es educacionais, consultas, 
							monitoramento de informa��es  entre outros. No site www.fnde.gov.br � poss�vel acessar os sistemas, a legisla��o e o contato com os respons�veis por cada programa executado pelo FNDE.
						</td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
				<tr><td>
					<table border="1" style="border-bottom-style: solid; border-color: black;" align="center" width="100%" cellspacing="0" cellpadding="5">
						<tr style="font-family: Calibri">
							<th width="15%">Institui��o</th>
							<th width="15%">Sistema</th>
							<th width="15%">Situa��o</th>
							<th width="55%">O que fazer</th>
						</tr>
						<tr style="font-family: Calibri; font-size: 12px">
							<td><b>MEC</b></td>
							<td><b>SIMEC</b> - Sistema Integrado do MEC</td>
							<td><?=($prefeito ? 'Senha Ativa' : 'Senha Inativa') ?></td>
							<td>Para obter ou atualizar sua senha, siga os passos previstos no site: <a href="http://simec.mec.gov.br">http://simec.mec.gov.br</a></td>
						</tr>
						<tr style="font-family: Calibri; font-size: 12px">
							<td><b>FNDE</b></td>
							<td><b>Habilita</b> - Sistema de Habilita��o de Entidades</td>
							<td><?=$dadoHabilita['situacao'] ?></td>
							<td>Acessar o site do FNDE e consultar a Resolu��o FNDE n� 10 de 31 de maio de 2012 que prev� os documentos necess�rios para cadastro.
								<a href="http://www.fnde.gov.br/fnde/legislacao/resolucoes/item/3520-resolu%C3%A7%C3%A3o-cd-fnde-n%C2%BA-10-de-31-de-maio-de-2012">http://www.fnde.gov.br/fnde/legislacao/resolucoes/item/3520-resolu%C3%A7%C3%A3o-cd-fnde-n%C2%BA-10-de-31-de-maio-de-2012</a></td>
						</tr>
						<tr style="font-family: Calibri; font-size: 12px">
							<td><b>FNDE</b></td>
							<td><b>SIGARPWEB</b> - Sistema de Gerenciamento de Ades�o a Registro de Pre�os</td>
							<td><?=$dadoSigarp['tipo'] ?></td>
							<td>O SIGARP permite ao munic�pio o acesso a produtos escolares padronizados e de qualidade, por meio da ades�o aos registros de pre�os nacionais, 
								com contrata��o  de  empresas licitadas pelo FNDE. Para acessar entre  pelos m�dulos �Produtos -  Ades�o on line� ou  �Sistemas� em 
								<a href="http://www.fnde.gov.br/portaldecompras">http://www.fnde.gov.br/portaldecompras/</a></td>
						</tr>
						<tr style="font-family: Calibri; font-size: 12px">
							<td><b>FNDE</b></td>
							<td><b>SIGPC</b> - Sistema de Gest�o de Presta��o de Contas</td>
							<td><?=$dadoSIGPC['tipo'] ?></td>
							<td>Para obter ou atualizar sua senha, siga os passos previstos no site: <a href="http://www.fnde.gov.br/sigpc/">www.fnde.gov.br/sigpc/</a></td>
						</tr>
					</table>
				</td></tr>
				</table>
			</td>
		</tr>
		<?php if(1==2){ ?>
		<tr>
			<td>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
				<tr><td>
					<table border="1" style="border-bottom-style: solid; border-color: black;" align="center" width="100%" cellspacing="0" cellpadding="5">
						<tr style="font-family: Calibri">
							<th width="20%">Sistema</th>
							<th width="30%">O que �:</th>
							<th width="50%">Como acessar:</th>
						</tr>
						<tr style="font-family: Calibri; font-size: 12px">
							<td>Simec - PAR</td>
							<td>No M�dulo PAR  do Simec o munic�pio elabora o seu Plano de A��es Articuladas. Na edi��o atual, o PAR apresenta as a��es e suba��es  
								para o per�odo de 2011 a 2014.</td>
							<td>Acesse <a href="http://simec.mec.gov.br">http://simec.mec.gov.br</a><br/>Na tela inicial do Simec, solicitar cadastro, preencher os dados cadastrais e enviar a solicita��o. 
								A senha de acesso � enviada para o e-mail informado no cadastro, desde que o e-mail esteja correto e perten�a � pessoa 
								cadastrada - prefeito(a) ou dirigente municipal de educa��o.</td>
						</tr>
						<tr style="font-family: Calibri; font-size: 12px">
							<td>SIOPE</td>
							<td>Sistema de Informa��es sobre Or�amentos P�blicos em Educa��o.</td>
							<td>Acesse http://www.fnde.gov.br/fnde-sistemas/sistema-siope-apresentacao, onde est�o dispon�veis todas as informa��es que o SIOPE disponibiliza 
								para o p�blico e �rg�os de controle.</td>
						</tr>
						<tr style="font-family: Calibri; font-size: 12px">
							<td>Portal FNDE</td>
							<td>S�tio de Internet com informa��es atualizadas sobre a��es e programas executados pelo FNDE. Disponibiliza acesso a sistemas, legisla��o e 
								listagem dos respons�veis na autarquia.</td>
							<td>Acesse <a href="http://fnde.gov.br">http://fnde.gov.br</a></td>
						</tr>
					</table>
				</td></tr>
				</table>
			</td>
		</tr>
		<? } ?>
		</tbody>
		<tfoot>
		<tr>
			<td>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5" style="border-bottom-style: solid; font-size: 12px; border-color: black;">
					<tr>
						<td align="right"><?=date("j/n/Y H:i:s"); ?></td>
					</tr>
				</table>
			</td>
		</tr>
		</tfoot>
	</table>
	
	<!-- Quebra Pagina -->	
	<div class="quebra"></div>
	
	<table border="0" align="left" width="100%" cellspacing="0" cellpadding="0" style="font-family: Calibri">
		<thead>
		<tr>
			<td>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
					<tr>
						<td><img src="imagem/cabecalho-7.jpg" width="710px" alt="" ></img></td>
					</tr>
					<tr style="color: white; background-color: #00008B; text-align: center; font-size: 17px">
						<td><?php echo 'Munic�pio: <b>'.$arrDadosMunicipio['nome'].' - '.$arrDadosMunicipio['uf'].'</b>'; ?></td>
					</tr>
				</table>
			</td>
		</tr>
		</thead>
		<tbody>
		<tr>
                    <td>
                        <table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
                            <tr style="font-family: Calibri; font-weight: bold; font-size: 18px" align="justify">
                                <td>2) Presta��o de Contas</td>
                            </tr>
                            <tr>
                                <td align="justify">
                                    A presta��o de contas tem a finalidade de comprovar a boa e regular aplica��o dos recursos repassados, bem como o cumprimento do objeto e do objetivo do programa ou do projeto educacional. A partir do exerc�cio de 2012, a presta��o de contas � realizada por meio do SiGPC - Contas Online (Sistema de Gest�o de Presta��o de Contas).<br/><br/>
                                    O FNDE disponibilizou o canal de comunica��o senha.sigpc@fnde.gov.br para tratar especificamente de assuntos relativos � senha de acesso ao Sistema de Gest�o de Presta��o de Contas - SiGPC - Contas Online. Visando � melhoria da informa��o sobre senha de acesso ao SiGPC, seguem instru��es para as d�vidas mais frequentes.<br/><br/>

                                    <strong>Quem n�o tem acesso ao SiGPC ou  possui acesso limitado?</strong>
                                    <ul>
                                        <li>Os Conselhos do Controle Social n�o possuem senha de acesso ao SiGPC; estes ter�o acesso �s informa��es do SiGPC por  meio do Sistema  SiGECON. Para tratar sobre o acesso a esse sistema, os Conselhos dever�o entrar em contato com o FNDE por meio dos endere�os: sigecon.cae@fnde.gov.br ou sigecon.cacs@fnde.gov.br. </li>
                                        <li>O FNDE n�o disponibilizou o acesso ao SIGPC para as Unidades Executoras, o acesso somente foi dado aos Gestores das Entidades Executoras que s�o os respons�veis por dar acesso secund�rio aos demais usu�rios da entidade, bem como por analisar as respectivas presta��es de contas e inform�-las no SiGPC.</li>
                                        <li>Os ex-gestores possuem acesso somente � visualiza��o das notifica��es. Para efetuar o envio das presta��es de contas ser� necess�rio entrar em contato com a atual gest�o. </li>
                                    </ul>

                                    <strong>Como realizar o primeiro acesso ao SiGPC?</strong>
                                    <ul style="list-style:none">
                                        <li><strong>1</strong> - A Entidade dever� atualizar os dados cadastrais mediante o Anexo I da Resolu��o n� 10/2012, que ser� encaminhado ao FNDE via postal.</li>
                                        <li><strong>2</strong> - O atual gestor acessar� o endere�o <a href="http://www.fnde.gov.br/sigpc" target="_blank">www.fnde.gov.br/sigpc</a> e informar� seu CPF no campo usu�rio e, deixando em branco o campo senha, clicar� em "Entrar", pois esse procedimento automaticamente far� o envio da mensagem com as orienta��es de acesso ao e-mail da entidade registrado no FNDE.  Sempre que esse procedimento � realizado, uma nova senha � enviada, por isso, � importante que apenas o gestor execute o procedimento, e n�o diversos usu�rios fa�am a tentativa ao mesmo tempo. Al�m disso, confira se o e-mail informado no Anexo I est� ativo e se h� espa�o dispon�vel na caixa de entrada. Caso o e-mail da entidade esteja incorreto ou com erro e seja preciso alter�-lo, ser� necess�rio que se encaminhe a solicita��o de altera��o do e-mail para o endere�o: cohap@fnde.gov.br</li>
                                    </ul>

                                    � importante ressaltar que o cadastro inicial e a valida��o de dados dever�o ser realizados pelo gestor, que, ap�s se registrar, dever� cadastrar a equipe t�cnica respons�vel pela inclus�o das informa��es relativas �s presta��es de contas no SiGPC - Contas Online, bem como excluir ou desativar usu�rios j� cadastrados e que n�o fa�am mais parte da equipe incumbida desta atividade. A defini��o dos usu�rios deve ser feita na op��o "Cadastrar Usu�rio de Entidade". <br/><br/>
                                   <img src="/imagens/contas_online.jpg"><br/><br/>

                                    <strong>Como recuperar a senha do SiGPC?</strong>
                                    <ul style="list-style:none">
                                        <li><strong>1</strong> - Caso o gestor n�o possua mais a senha de acesso ao sistema, bastar� que utilize a funcionalidade "esqueci minha senha" (utilizando o CPF da pessoa cadastrada), e, ent�o, a senha ser� encaminhada para o e-mail cadastrado pelo gestor no SiGPC. � importante verificar se o e-mail cadastrado est� correto.</li>
                                        <li><strong>2</strong> - Caso seja necess�rio alterar o e-mail cadastrado pelo gestor no SiGPC, basta encaminhar um of�cio assinado pelo gestor e em papel com o timbre da entidade (o of�cio pode ser encaminhado por e-mail).</li>
                                    </ul>
                                </td>
                            </tr>
                        </table>
                    </td>
		</tr>
		</tbody>
		<tfoot>
		<tr>
			<td>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5" style="border-bottom-style: solid; font-size: 12px; border-color: black;">
					<tr>
						<td align="right"><?=date("j/n/Y H:i:s"); ?></td>
					</tr>
				</table>
			</td>
		</tr>
		</tfoot>
	</table>
	
	<!-- Quebra Pagina -->	
	<div class="quebra"></div>
	
	<table border="0" align="left" width="100%" cellspacing="0" cellpadding="0" style="font-family: Calibri">
            <thead>
                <tr>
                    <td>
                        <table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
                            <tr>
                                <td><img src="imagem/cabecalho-7.jpg" width="710px" alt="" ></img></td>
                            </tr>
                            <tr style="color: white; background-color: #00008B; text-align: center; font-size: 17px">
                                <td><?php echo 'Munic�pio: <b>'.$arrDadosMunicipio['nome'].' - '.$arrDadosMunicipio['uf'].'</b>'; ?></td>
                            </tr>
                        </table>
                    </td>
                </tr>
            </thead>
            <tbody>
                <tr>
                    <td>
                        <table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
                            <tr>
                                <td align="justify" style="font-family: Calibri"><b>3) Obras do PAC 2</b><br/><br/>
                                    O Minist�rio da Educa��o apoia, desde 2011, a constru��o de creches/pr�-escolas e quadras esportivas
                                    Escolares Por meio do Programa de Acelera��o do Crescimento (PAC). Projetos arquitet�nicos padronizados est�o dispon�veis em
                                    <a href="http://www.fnde.gov.br/programas/par/par-projetos-arquitetonicos-para-construcao">http://www.fnde.gov.br/programas/par/par-projetos-arquitetonicos-para-construcao</a>.<br> 
                                    A situa��o das Obras do PAC 2 no seu munic�pio �:
                                </td>
                            </tr>
                        </table>
                    </td>
                </tr>
                <tr>
                    <td>
                        <table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
                            <tr>
                                <td style="text-align: center;">
                                    <table border="1" style="border-bottom-style: solid; font-size: 12px; border-color: black;" align="center" width="100%" cellspacing="0" cellpadding="5">
                                        <tr>
                                            <th>Situa��o das Obras</th>
                                            <th>Aprovadas</th>
                                            <th>Nao iniciadas</th>
                                            <th>Em execu��o</th>
                                            <th>Paralisada</th>
                                            <th>Canceladas</th>
                                            <th>Conclu�das</th>
                                            <th>TOTAL</th>
                                        </tr>
                                        <tr>
                                            <td style="text-align: left;">Creches e Pr�-escolas</td>
                                            <td style="text-align: center;"><?=$local11 ? simec_number_format($local11, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$local12 ? simec_number_format($local12, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$local13 ? simec_number_format($local13, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$local14 ? simec_number_format($local14, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$local15 ? simec_number_format($local15, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$local16 ? simec_number_format($local16, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$soma1 ? $soma1 : '-' ?></td>
                                        </tr>
                                        <tr>
                                            <td style="text-align: left;">Constru��o de Quadras</td>
                                            <td style="text-align: center;"><?=$local21 ? simec_number_format($local21, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$local22 ? simec_number_format($local22, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$local23 ? simec_number_format($local23, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$local24 ? simec_number_format($local24, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$local25 ? simec_number_format($local25, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$local26 ? simec_number_format($local26, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$soma2 ? $soma2 : '-' ?></td>
                                        </tr>
                                        <tr>
                                            <td style="text-align: left;">Cobertura de Quadras</td>
                                            <td style="text-align: center;"><?=$local31 ? simec_number_format($local31, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$local32 ? simec_number_format($local32, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$local33 ? simec_number_format($local33, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$local34 ? simec_number_format($local34, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$local35 ? simec_number_format($local35, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$local36 ? simec_number_format($local36, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$soma3 ? $soma3 : '-' ?></td>
                                        </tr>
                                        <tr>
                                            <td style="text-align: left;">TOTAL</td>
                                            <td style="text-align: center;">
                                                <?php 
                                                $total1 = ($local11 + $local21 + $local31);
                                                echo ($total1) ? simec_number_format($total1, 0, ',', '.') : '-' ?>
                                            </td>
                                            <td style="text-align: center;">
                                                <?php 
                                                $total2 = ($local12 + $local22 + $local32);
                                                echo ($total2) ? simec_number_format($total2, 0, ',', '.') : '-' ?>
                                            </td>
                                            <td style="text-align: center;">
                                                <?php 
                                                $total3 = ($local13 + $local23 + $local33);
                                                echo ($total3) ? simec_number_format($total3, 0, ',', '.') : '-' ?>
                                            </td>
                                            <td style="text-align: center;">
                                                <?php 
                                                $total4 = ($local14 + $local24 + $local34);
                                                echo ($total4) ? simec_number_format($total4, 0, ',', '.') : '-' ?>
                                            </td>
                                            <td style="text-align: center;">
                                                <?php 
                                                $total5 = ($local15 + $local25 + $local35);
                                                echo ($total5) ? simec_number_format($total5, 0, ',', '.') : '-' ?>
                                            </td>
                                            <td style="text-align: center;">
                                                <?php 
                                                $total6 = ($local16 + $local26 + $local36);
                                                echo ($total6) ? simec_number_format($total6, 0, ',', '.') : '-' ?>
                                            </td>
                                            <td style="text-align: center;">
                                                <?php 
                                                $totalFull = ($soma1 + $soma2 + $soma3);
                                                echo ($totalFull) ? simec_number_format($totalFull, 0, ',', '.') : '-' ?>
                                            </td>
                                        </tr>
                                    </table>
<!--                                    <table border="1" style="border-bottom-style: solid; font-size: 12px; border-color: black;" align="center" width="100%" cellspacing="0" cellpadding="5">
                                        <tr>
                                            <th>Situa��o das Obras Aprovadas</th>
                                            <th>Em planejamento</th>
                                            <th>Em licita��o</th>
                                            <th>Em execu��o</th>
                                            <th>Paralisada</th>
                                            <th>Cancelada</th>
                                            <th>Conclu�da</th>
                                            <th>TOTAL</th>
                                        </tr>
                                        <tr>
                                            <td style="text-align: left;">Creches e Pr�-escolas</td>
                                            <td style="text-align: center;"><?=$local11 ? simec_number_format($local11, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$local12 ? simec_number_format($local12, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$local13 ? simec_number_format($local13, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$local14 ? simec_number_format($local14, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$local15 ? simec_number_format($local15, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$local16 ? simec_number_format($local16, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$soma1 ? $soma1 : '-' ?></td>
                                        </tr>
                                        <tr>
                                            <td style="text-align: left;">Constru��o de Quadras</td>
                                            <td style="text-align: center;"><?=$local21 ? simec_number_format($local21, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$local22 ? simec_number_format($local22, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$local23 ? simec_number_format($local23, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$local24 ? simec_number_format($local24, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$local25 ? simec_number_format($local25, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$local26 ? simec_number_format($local26, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$soma2 ? $soma2 : '-' ?></td>
                                        </tr>
                                        <tr>
                                            <td style="text-align: left;">Cobertura de Quadras</td>
                                            <td style="text-align: center;"><?=$local31 ? simec_number_format($local31, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$local32 ? simec_number_format($local32, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$local33 ? simec_number_format($local33, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$local34 ? simec_number_format($local34, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$local35 ? simec_number_format($local35, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$local36 ? simec_number_format($local36, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$soma3 ? $soma3 : '-' ?></td>
                                        </tr>
                                    </table>-->
                                </td>					
                            </tr>
                        </table>
                    </td>
                </tr>
            </tbody>
            <tfoot>
                <tr>
                    <td>
                        <table border="0" align="center" width="100%" cellspacing="4" cellpadding="5" style="border-bottom-style: solid; font-size: 12px; border-color: black;">
                            <tr>
                                <td align="right"><?php echo date("j/n/Y H:i:s"); ?></td>
                            </tr>
                        </table>
                    </td>
                </tr>
            </tfoot>
	</table>
	
	<!-- Quebra Pagina -->	
	<div class="quebra"></div>
	
	<table border="0" align="left" width="100%" cellspacing="0" cellpadding="0" style="font-family: Calibri">
            <thead>
		<tr>
                    <td>
                        <table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
                            <tr>
                                <td><img src="imagem/cabecalho-7.jpg" width="710px" alt="" ></img></td>
                            </tr>
                            <tr style="color: white; background-color: #00008B; text-align: center; font-size: 17px">
                                <td><?php echo 'Munic�pio: <b>'.$arrDadosMunicipio['nome'].' - '.$arrDadosMunicipio['uf'].'</b>'; ?></td>
                            </tr>
                        </table>
                    </td>
		</tr>
            </thead>
            <tbody>		
		<tr>
                    <td>
                        <table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
                            <tr>
                                <td align="justify" style="font-family: Calibri"><b>4) Plano de A��es Articuladas - PAR</b><br/><br/>
                                    O PAR � o planejamento estrat�gico educacional elaborado pelo munic�pio e coordenado pela secretaria municipal de educa��o. 
                                    Trata-se de um plano plurianual que permite ao munic�pio conhecer sua realidade educacional em quatro dimens�es: Gest�o Educacional, Forma��o de 
                                    Professores e de Profissionais de Servi�o e Apoio Escolar, Pr�ticas pedag�gicas e avalia��o e Infraestrutura F�sica e Recursos Pedag�gicos.<br /><br />
                                    O ciclo do PAR 2011-2014 encontra-se em fase de finaliza��o das a��es ainda em execu��o. Ao ser aberto o ciclo 2015-2018, ser� poss�vel ao munic�pio o levantamento e o cadastramento de novas demandas pass�veis de atendimento por transfer�ncia volunt�ria de recursos. 
                                </td>
                            </tr>
                        </table>
                    </td>
		</tr>
		<tr>
                    <td>
                        <table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
                            <tr>
                                <td align="justify" style="font-family: Calibri"><b>4.1) Obras PAR</b><br/>
                                    A��es de infraestrutura escolar, como amplia��es e constru��es de novas escolas (urbanas ou rurais), foram apoiadas pelo PAR mediante o cadastramento de projetos t�cnicos no Simec. Projetos arquitet�nicos padronizados poder�o ser consultados no link indicado no item 3 (Obras do PAC 2).<br/><br/>
                                    A situa��o das Obras do PAR no seu munic�pio � a seguinte:
                                </td>
                            </tr>
                        </table>
                    </td>
		</tr>
		<tr>
                    <td>
                        <table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
                            <tr>
                                <td style="text-align: center;">
                                    <table border="1" style="border-bottom-style: solid; font-size: 12px; border-color: black;" align="center" width="100%" cellspacing="0" cellpadding="5">
                                        <tr>
                                            <th>Situa��o das Obras</th>
                                            <th>Aprovadas</th>
                                            <th>Nao iniciadas</th>
                                            <th>Em execu��o</th>
                                            <th>Paralisadas</th>
                                            <th>Canceladas</th>
                                            <th>Conclu�da</th>
                                            <th>TOTAL</th>
                                        </tr>
                                        <tr>
                                            <td style="text-align: left;">Escolas Urbanas</td>
                                            <td style="text-align: center;"><?=$localpar11 ? simec_number_format($localpar11, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$localpar12 ? simec_number_format($localpar12, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$localpar13 ? simec_number_format($localpar13, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$localpar14 ? simec_number_format($localpar14, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$localpar15 ? simec_number_format($localpar15, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$localpar16 ? simec_number_format($localpar16, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$somapar1 ? $somapar1 : '-' ?></td>
                                        </tr>
                                        <tr>
                                            <td style="text-align: left;">Escolas do Campo</td>
                                            <td style="text-align: center;"><?=$localpar21 ? simec_number_format($localpar21, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$localpar22 ? simec_number_format($localpar22, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$localpar23 ? simec_number_format($localpar23, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$localpar24 ? simec_number_format($localpar24, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$localpar25 ? simec_number_format($localpar25, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$localpar26 ? simec_number_format($localpar26, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$somapar2 ? $somapar2 : '-' ?></td>
                                        </tr>
                                        <tr>
                                            <td style="text-align: left;">Creches e Pr�-escolas (Pr�-PAC)</td>
                                            <td style="text-align: center;"><?=$localpar31 ? simec_number_format($localpar31, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$localpar32 ? simec_number_format($localpar32, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$localpar33 ? simec_number_format($localpar33, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$localpar34 ? simec_number_format($localpar34, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$localpar35 ? simec_number_format($localpar35, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$localpar36 ? simec_number_format($localpar36, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$somapar3 ? $somapar3 : '-' ?></td>
                                        </tr>
                                    </table>
<!--                                    <table border="1" style="border-bottom-style: solid; font-size: 12px; border-color: black;" align="center" width="100%" cellspacing="0" cellpadding="5">
                                        <tr>
                                            <th>Situa��o das Obras Aprovadas</th>
                                            <th>Em planejamento</th>
                                            <th>Em licita��o</th>
                                            <th>Em execu��o</th>
                                            <th>Paralisada</th>
                                            <th>Cancelada</th>
                                            <th>Em Reformula��o</th>
                                            <th>Conclu�da</th>
                                            <th>TOTAL</th>
                                        </tr>
                                        <tr>
                                            <td style="text-align: left;">Escolas Urbanas</td>
                                            <td style="text-align: center;"><?=$localpar11 ? simec_number_format($localpar11, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$localpar12 ? simec_number_format($localpar12, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$localpar13 ? simec_number_format($localpar13, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$localpar14 ? simec_number_format($localpar14, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$localpar15 ? simec_number_format($localpar15, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$localpar17 ? simec_number_format($localpar17, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$localpar16 ? simec_number_format($localpar16, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$somapar1 ? $somapar1 : '-' ?></td>
                                        </tr>
                                        <tr>
                                            <td style="text-align: left;">Escolas do Campo</td>
                                            <td style="text-align: center;"><?=$localpar21 ? simec_number_format($localpar21, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$localpar22 ? simec_number_format($localpar22, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$localpar23 ? simec_number_format($localpar23, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$localpar24 ? simec_number_format($localpar24, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$localpar25 ? simec_number_format($localpar25, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$localpar27 ? simec_number_format($localpar27, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$localpar26 ? simec_number_format($localpar26, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$somapar2 ? $somapar2 : '-' ?></td>
                                        </tr>
                                        <tr>
                                            <td style="text-align: left;">Creches e Pr�-escolas (Pr�-PAC)</td>
                                            <td style="text-align: center;"><?=$localpar31 ? simec_number_format($localpar31, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$localpar32 ? simec_number_format($localpar32, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$localpar33 ? simec_number_format($localpar33, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$localpar34 ? simec_number_format($localpar34, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$localpar35 ? simec_number_format($localpar35, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$localpar37 ? simec_number_format($localpar37, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$localpar36 ? simec_number_format($localpar36, 0, ',', '.') : '-' ?></td>
                                            <td style="text-align: center;"><?=$somapar3 ? $somapar3 : '-' ?></td>
                                        </tr>
                                    </table>-->
                                </td>					
                            </tr>
                        </table>
                    </td>
		</tr>
                <tr>
                    <td>
                        <table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
                            <tr>
                                <td align="justify" style="font-family: Calibri">
                                    � importante o acompanhamento da execu��o das obras do PAR e do PAC para que sejam evitadas as seguintes situa��es que podem atrapalhar a solicita��o de novos pleitos no novo ciclo do PAR 2015/2018:
                                    <ul style="list-style:none">
                                        <li>a)	Obra n�o iniciada h� mais de 360 dias ap�s o primeiro repasse;</li>
                                        <li>b)	Obras sem vistoria h� mais de 60 dias, ap�s ter iniciado sua execu��o;</li>
                                        <li>c)	Obras que evolu�ram fisicamente menos de 10% no per�odo de 90 dias;</li>
                                        <li>d)	Obras paralisadas;</li>
                                        <li>e)	Obras em reformula��o, com dilig�ncia sem resposta h� mais de 60 dias.</li>
                                    </ul>
                                </td>
                            </tr>
                        </table>
                    </td>
		</tr>
		<tr>
                    <td>
                        <table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
                            <tr>
                                <td align="justify" style="font-family: Calibri"><b>4.2) Termos de Compromisso</b><br/>
                                    Ap�s a aprova��o t�cnica de um projeto no PAR, � gerado um termo de compromisso no SIMEC, que deve ser validado eletronicamente pelo(a) prefeito(a). Para realizar esse procedimento, � necess�rio que o(a) prefeito(a) tenha perfil de "Equipe Municipal - prefeito" no SIMEC e acesse o documento na aba "Plano de trabalho" na visualiza��o "�rvore" e link "Documentos".<br/><br/> 
                                    No momento, o PAR encontra-se em processo de finaliza��o de seu ciclo 2011-2014. Nesta etapa, � necess�rio que o munic�pio atualize uma importante funcionalidade do sistema, denominada "Execu��o e Acompanhamento". Essa funcionalidade, dispon�vel na aba "Execu��o e Acompanhamento" na visualiza��o �rvore, tem como objetivo possibilitar o acompanhamento da execu��o f�sico-financeira dos recursos pactuados para os diversos itens aprovados no PAR e que foram objetos de termos de compromisso.<br/><br/>
                                </td>
                            </tr>
                            <tr>
                                <td>
                                    <!-- DOCUMENTOS PAR - SUBACAO -->
                                    <?php 
                                    $sql = "SELECT
                                                prpnumeroprocesso AS processo,
                                                doc, 
                                                tipodocumento, 
                                                (
                                                        SELECT 
                                                                TO_CHAR(vigencia, 'MM/YYYY') -- seleciona maior vig�ncia entre documento validado e ex-of�cio
                                                        FROM (
                                                           SELECT TO_DATE(dopdatafimvigencia, 'MM/YYYY') AS vigencia --Seleciona maior vig�ncia entre termos validados
                                                                  FROM par.documentopar  d
                                                                  INNER JOIN par.documentoparvalidacao v ON d.dopid = v.dopid AND v.dpvstatus = 'A'
                                                                  WHERE d.prpid = foo.prpid --8146
                                                                  AND dopstatus <> 'E'
                                                                  AND mdoid NOT IN (69,82,81,41,80,68,42,67,65,76,79,74,44,78,56,62,52,71,66,73,75,77)
                                                           UNION ALL
                                                           SELECT TO_DATE(dopdatafimvigencia, 'MM/YYYY') AS vigencia -- Seleciona maior vig�ncia de Ex-Of�cio
                                                                  FROM par.documentopar  d
                                                                  WHERE d.prpid = foo.prpid
                                                                  AND dopstatus <> 'E'
                                                                  AND mdoid IN (69,82,81,41,80,68,42,67,65,76,79,74,44,78,56,62,52,71,66,73,75,77)
                                                                ) AS foo
                                                        GROUP BY vigencia
                                                        ORDER BY vigencia DESC LIMIT 1
                                                ) AS data_vigencia,
                                                dopvalortermo AS vt, 
                                                valorempenho AS ve, 
                                                valorpagamentosolicitado AS ps,
                                                valorpagamento AS vp,  
                                                dados_bancarios,
                                                (
                                                        SELECT 
                                                                COALESCE(SUM(saldo)::text, 'N�o Informado') AS saldo 
                                                        FROM
                                                        (
                                                                SELECT (dfi.dfisaldoconta + dfi.dfisaldofundo + dfi.dfisaldopoupanca + dfi.dfisaldordbcdb) AS saldo
                                                                FROM painel.dadosfinanceirosconvenios dfi
                                                                WHERE dfi.dfiprocesso = prpnumeroprocesso
                                                                AND TO_CHAR(dfi.dfidatasaldo, 'YYYYMM') = TO_CHAR((now() - INTERVAL '1 MONTH'), 'YYYYMM')
                                                                UNION
                                                                SELECT NULL
                                                        ) AS saldomes
                                                )  AS sb
                                         FROM (
                                                SELECT
                                                        dp.dopid AS id,
                                                        dp.prpid,
                                                        dp.dopacompanhamento AS acompanhamento,
                                                        dp.dopidpai AS dopidpai,
                                                        d.mdoid AS mdoid,
                                                        d.tpdcod AS tipo_doc,
                                                        (
                                                                SELECT 
                                                                        dopdatafimvigencia 
                                                                FROM par.documentopar  d
                                                                INNER JOIN par.documentoparvalidacao v ON d.dopid = v.dopid
                                                                WHERE prpid = pp.prpid
                                                                        AND dopstatus <> 'E'
                                                                        AND dpvstatus = 'A'
                                                                        AND mdoid NOT IN (79,65,66,68,76,80,67,73,82)
                                                                ORDER BY d.dopid DESC
                                                                LIMIT 1
                                                        ) AS data_vigencia,
                                                        d.mdonome AS tipodocumento,
                                                        pp.prpfinalizado,
                                                        iu.inuid,
                                                        iu.estuf,
                                                        iu.muncod,
                                                        dp.dopusucpfvalidacaogestor,
                                                        d.mdoqtdvalidacao,
                                                        dp.dopnumerodocumento AS doc,
                                                        pp.prpnumeroprocesso,
                                                        (
                                                                SELECT 
                                                                        count(dopid) 
                                                                FROM par.documentoparvalidacao 
                                                                WHERE dopid = dp.dopid 
                                                                        AND dpvstatus = 'A'
                                                        ) AS contagem,
                                                        dp.dopvalortermo::numeric(20,2),
                                                        COALESCE((SELECT SUM(vrlempenhocancelado) FROM par.v_vrlempenhocancelado WHERE processo = pp.prpnumeroprocesso ), 0.00) AS valorempenho,
                                                        pgs.valor_pagamento AS valorpagamentosolicitado,
                                                        pm.valor_pagamento AS valorpagamento,
                                                        dp.arqid,
                                                        dp.dopdatafimvigencia,
                                                        'Banco: '||COALESCE(prpbanco, 'n/a')||' Conta: '||COALESCE(prpagencia, 'n/a')||'<br> Conta Corrente: '||COALESCE(nu_conta_corrente, 'n/a') AS dados_bancarios
                                                FROM
                                                        par.documentopar  dp
                                                INNER JOIN par.modelosdocumentos   d ON d.mdoid = dp.mdoid
                                                INNER JOIN par.processopar pp ON pp.prpid = dp.prpid
                                                INNER JOIN par.instrumentounidade iu ON iu.inuid = pp.inuid
                                                INNER JOIN ( 
                                                        SELECT
                                                                d.dopid,
                                                                SUM(vve.vrlempenhocancelado) + SUM(COALESCE(emr.vrlreforco,0)) AS valor
                                                        FROM
                                                                par.documentopar d
                                                        INNER JOIN par.processopar prp ON prp.prpid = d.prpid
                                                        INNER JOIN par.empenho emp ON emp.empnumeroprocesso = prp.prpnumeroprocesso AND empcodigoespecie NOT IN ('03', '13', '02', '04') AND empstatus = 'A'
                                                        INNER JOIN par.v_vrlempenhocancelado vve ON vve.empid = emp.empid
                                                        LEFT JOIN (
                                                                SELECT 
                                                                        empnumeroprocesso, empidpai, SUM(empvalorempenho) AS vrlreforco, empcodigoespecie
                                                                FROM par.empenho
                                                                WHERE empcodigoespecie IN ('02') AND empstatus = 'A'
                                                                GROUP BY
                                                                        empnumeroprocesso,
                                                                        empcodigoespecie,
                                                                        empidpai
                                                        ) AS emr ON emr.empidpai = emp.empid
                                                        INNER JOIN par.empenhosubacao ems ON ems.empid = emp.empid AND eobstatus = 'A' 
                                                        GROUP BY d.dopid
                                                ) em ON em.dopid = dp.dopid
                                                LEFT JOIN (
                                                        SELECT
                                                            d.dopid, SUM( pobvalorpagamento ) AS valor_pagamento
                                                        FROM
                                                            par.vm_documentopar_ativos d
                                                        INNER JOIN par.processopar prp ON prp.prpid = d.prpid
                                                        INNER JOIN par.empenho emp ON emp.empnumeroprocesso = prp.prpnumeroprocesso AND empcodigoespecie NOT IN ('03', '13', '02', '04') AND empstatus = 'A'
                                                        INNER JOIN par.pagamento pag ON pag.empid = emp.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento = '2 - EFETIVADO'
                                                        INNER JOIN par.pagamentosubacao ps ON ps.pagid = pag.pagid AND pobstatus = 'A' 
                                                        GROUP BY d.dopid, pagsituacaopagamento
                                                ) pm ON pm.dopid = dp.dopid
                                            LEFT JOIN (
                                                        SELECT
                                                            d.dopid, SUM( pobvalorpagamento ) AS valor_pagamento
                                                        FROM
                                                            par.vm_documentopar_ativos d
                                                        INNER JOIN par.processopar prp ON prp.prpid = d.prpid
                                                        INNER JOIN par.empenho emp ON emp.empnumeroprocesso = prp.prpnumeroprocesso AND empcodigoespecie NOT IN ('03', '13', '02', '04') AND empstatus = 'A'
                                                        INNER JOIN par.pagamento pag ON pag.empid = emp.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento IN ('8 - SOLICITA��O APROVADA', 'ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', 'Enviado ao SIGEF')
                                                        INNER JOIN par.pagamentosubacao ps ON ps.pagid = pag.pagid AND pobstatus = 'A' 
                                                        GROUP BY d.dopid, pagsituacaopagamento
                                                ) pgs ON pgs.dopid = dp.dopid
                                        ) AS foo
                                        WHERE inuid = {$inuid}  AND id IS NOT NULL
                                        AND tipo_doc IN (102, 21, 16 )";
//                                     ver($sql, d);
                                    $arrDocSubacao = $db->carregar($sql); 
                                    ?>
                                    <table border="1" style="border-bottom-style: solid; border-color: black;" align="center" width="100%" cellspacing="0" cellpadding="5">
                                        <caption><strong>Documentos do PAR</strong></caption>
                                        <tr>
                                            <th>N� do Processo</th>
                                            <th>N� do Documento</th>
                                            <th>Tipo de documento</th>
                                            <th>Data de Vig�ncia</th>
                                            <th>Valor do Termo</th>
                                            <th>Valor Empenhado</th>
                                            <th>Pagamento Solicitado</th>
                                            <th>Pagamento Efetivado</th>
                                            <th>Dados Banc�rios</th>
                                            <th>Saldo Banc�rio <br/>(CC+CP+Fundo)</th>
                                        </tr>
                                        <?php
                                        if (is_array($arrDocSubacao)):
                                            foreach($arrDocSubacao as $docSubacao):
                                        ?>
                                        <tr>
                                            <td><?php echo $docSubacao['processo']; ?></td>
                                            <td><?php echo $docSubacao['doc']; ?></td>
                                            <td><?php echo $docSubacao['tipodocumento']; ?></td>
                                            <td><?php echo $docSubacao['data_vigencia']; ?></td>
                                            <td>R$ <?=( is_numeric($docSubacao['vt']) ? simec_number_format($docSubacao['vt'], 2, ',', '.') : $docSubacao['vt']); ?></td>
                                            <td>R$ <?=( is_numeric($docSubacao['ve']) ? simec_number_format($docSubacao['ve'], 2, ',', '.') : $docSubacao['ve']); ?></td>
                                            <td>R$ <?=( is_numeric($docSubacao['ps']) ? simec_number_format($docSubacao['ps'], 2, ',', '.') : $docSubacao['ps']); ?></td>
                                            <td>R$ <?=( is_numeric($docSubacao['vp']) ? simec_number_format($docSubacao['vp'], 2, ',', '.') : $docSubacao['vp']); ?></td>
                                            <td><?php echo $docSubacao['dados_bancarios']; ?></td>
                                            <td>R$ <?=( is_numeric($docSubacao['sb']) ? simec_number_format($docSubacao['sb'], 2, ',', '.') : $docSubacao['sb']); ?></td>
                                        </tr>
                                        <?php
                                            endforeach;
                                        endif;
                                        ?>
                                    </table>
                                    <!-- FIM DOCUMENTOS PAR - SUBACAO -->
                                    <br /><br />
                                    <!-- DOCUMENTOS DE OBRAS DO PAR -->
                                    <?php 
                                    $sql = "SELECT
                                            pronumeroprocesso as processo, 
                                            doc, 
                                            tipodocumento,
                                            data_vigencia, 
                                            qtdObra, 
                                            dopvalortermo AS vt, 
                                            valorempenho AS ve, 
                                            valorpagamentosolicitado AS ps,
                                            valorpagamento AS vp, 
                                            dados_bancarios,
                                            (
                                                    SELECT COALESCE(sum(saldo)::text, 'N�o Informado') AS saldo FROM
                                                    (
                                                            SELECT 
                                                                    (dfi.dfisaldoconta + dfi.dfisaldofundo + dfi.dfisaldopoupanca + dfi.dfisaldordbcdb) AS saldo
                                                            FROM painel.dadosfinanceirosconvenios dfi
                                                            WHERE dfi.dfiprocesso = pronumeroprocesso
                                                            AND TO_CHAR(dfi.dfidatasaldo, 'YYYYMM') = TO_CHAR((now() - INTERVAL '1 MONTH'), 'YYYYMM')
                                                            UNION
                                                            SELECT NULL
                                                    ) AS saldomes
                                            )  AS sb
                                    FROM (
                                            SELECT 
                                                    dp.dopid AS id, 
                                                    dp.dopidpai,
                                                    (
                                                            SELECT 
                                                                            TO_CHAR(vigencia, 'MM/YYYY') -- seleciona maior vig�ncia entre documento validado e ex-of�cio
                                                                    FROM (
                                                                            SELECT 
                                                                                    TO_DATE(dopdatafimvigencia, 'MM/YYYY') AS vigencia --Seleciona maior vig�ncia entre termos validados
                                                                            FROM par.documentopar  d
                                                                            INNER JOIN par.documentoparvalidacao v ON d.dopid = v.dopid AND v.dpvstatus = 'A'
                                                                            WHERE d.proid = dp.proid
                                                                                    AND dopstatus <> 'E'
                                                                                    AND mdoid NOT IN (69,82,81,41,80,68,42,67,65,76,79,74,44,78,56,62,52,71,66,73,75,77)
                                                                            UNION ALL
                                                                            SELECT 
                                                                                    TO_DATE(dopdatafimvigencia, 'MM/YYYY') AS vigencia -- Seleciona maior vig�ncia de Ex-Of�cio
                                                                             FROM par.documentopar  d
                                                                             WHERE d.proid = dp.proid
                                                                             AND dopstatus <> 'E'
                                                                             AND mdoid IN (69,82,81,41,80,68,42,67,65,76,79,74,44,78,56,62,52,71,66,73,75,77)
                                                                    ) AS foo
                                                                    GROUP BY vigencia
                                                                    ORDER BY vigencia DESC LIMIT 1
                                                    ) AS data_vigencia,
                                                    d.mdonome AS tipodocumento, 
                                                    dopstatus, iu.inuid, iu.estuf, iu.muncod, dp.dopusucpfvalidacaogestor, d.mdoqtdvalidacao, (SELECT dopnumerodocumento FROM par.documentopar WHERE proid = dp.proid and dopstatus <> 'E' order by dopid asc LIMIT 1) AS doc,
                                                    'Banco: '||COALESCE(probanco, 'n/a')||'
                                     Conta: '||COALESCE(proagencia, 'n/a')||'
                                     Conta Corrente: '||COALESCE(nu_conta_corrente, 'n/a') AS dados_bancarios,
                                                                            (SELECT count(dopid) FROM par.documentoparvalidacao WHERE dopid = dp.dopid AND dpvstatus = 'A' ) AS contagem,
                                                                            dp.dopvalortermo::numeric(20,2), 
                                                                            em.valor AS valorempenho,
                                                                            pgs.valor_pagamento AS valorpagamentosolicitado,
                                                                            pm.valor_pagamento AS valorpagamento,
                                                                            --CASE WHEN pm.pagsituacaopagamento = '2 - EFETIVADO' THEN pm.valor_pagamento END AS valorpagamento,
                                                                            pp.pronumeroprocesso,
                                                                            (SELECT count(pc.preid) FROM par.processoobraspar po INNER JOIN par.processoobrasparcomposicao pc on pc.proid = po.proid WHERE pc.pocstatus = 'A' and po.proid = pp.proid) AS qtdObra
                                                                    FROM par.documentopar  dp
                                                                    INNER JOIN par.modelosdocumentos   d ON d.mdoid = dp.mdoid
                                                                    INNER JOIN par.processoobraspar pp ON pp.proid = dp.proid and pp.prostatus = 'A'
                                                                    INNER JOIN par.instrumentounidade iu ON iu.inuid = pp.inuid
                                                                    LEFT JOIN ( 
                                                                            SELECT
                                                                                    dopid,
                                                                                    sum(valor) AS valor
                                                                            FROM
                                                                                    (
                                                                                    SELECT DISTINCT
                                                                                            d.dopid, 
                                                                                            vve.empid,
                                                                                            vve.vrlempenhocancelado AS valor
                                                                                    FROM 
                                                                                            par.documentopar d
                                                                                    INNER JOIN par.processoobraspar prp on prp.proid = d.proid and prp.prostatus = 'A'
                                                                                    INNER JOIN par.empenho emp on emp.empnumeroprocesso = prp.pronumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
                                                                                    INNER JOIN par.v_vrlempenhocancelado vve on vve.empid = emp.empid
                                                                                    LEFT  JOIN (
                                                                                            SELECT empnumeroprocesso, empidpai, sum(empvalorempenho) AS vrlreforco, empcodigoespecie 
                                                                                            FROM par.empenho
                                                                                            WHERE empcodigoespecie IN ('02') AND empstatus = 'A'
                                                                                            GROUP BY empnumeroprocesso, empcodigoespecie, empidpai
                                                                                            ) AS emr ON emr.empidpai = emp.empid 
                                                                                    INNER JOIN par.empenhoobrapar ems on ems.empid = emp.empid and eobstatus = 'A'  
                                                                                    ) AS foo
                                                                            GROUP BY dopid
                                                                            ) em ON em.dopid = dp.dopid
                                                                            LEFT JOIN (SELECT 
                                                                                        d.dopid, sum( pobvalorpagamento ) AS valor_pagamento
                                                                                    FROM 
                                                                                        par.vm_documentopar_ativos d
                                                                                    INNER JOIN par.processopar prp on prp.prpid = d.prpid
                                                                                    INNER JOIN par.empenho emp on emp.empnumeroprocesso = prp.prpnumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
                                                                                    INNER JOIN par.pagamento pag on pag.empid = emp.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento = '2 - EFETIVADO'
                                                                                    INNER JOIN par.pagamentosubacao ps on ps.pagid = pag.pagid and pobstatus = 'A' GROUP BY d.dopid, pagsituacaopagamento) pm ON pm.dopid = dp.dopid
                                                                        LEFT JOIN (SELECT 
                                                                                        d.dopid, sum( pobvalorpagamento ) AS valor_pagamento
                                                                                    FROM 
                                                                                        par.vm_documentopar_ativos d
                                                                                    INNER JOIN par.processopar prp on prp.prpid = d.prpid
                                                                                    INNER JOIN par.empenho emp on emp.empnumeroprocesso = prp.prpnumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
                                                                                    INNER JOIN par.pagamento pag on pag.empid = emp.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento in ('8 - SOLICITA��O APROVADA', 'ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', 'Enviado ao SIGEF')
                                                                                    INNER JOIN par.pagamentosubacao ps on ps.pagid = pag.pagid and pobstatus = 'A' GROUP BY d.dopid, pagsituacaopagamento) pgs ON pgs.dopid = dp.dopid
                                    ) AS foo
                                    WHERE inuid = {$inuid}  
                                    ORDER BY doc";
                                    $arrDocObrasPar = $db->carregar($sql); 
                                    ?>
                                    <table border="1" style="border-bottom-style: solid; border-color: black;" align="center" width="100%" cellspacing="0" cellpadding="5">
                                        <caption><strong>Documentos de Obras do PAR</strong></caption>
                                        <tr>
                                            <th>N� do Processo</th>
                                            <th>N� do Documento</th>
                                            <th>Tipo de documento</th>
                                            <th>Data de Vig�ncia</th>
                                            <th>Qnt de Obras</th>
                                            <th>Valor do Termo</th>
                                            <th>Valor Empenhado</th>
                                            <th>Pagamento Solicitado</th>
                                            <th>Pagamento Efetivado</th>
                                            <th>Dados Banc�rios</th>
                                            <th>Saldo Banc�rio <br/>(CC+CP+Fundo)</th>
                                        </tr>
                                        <?php
                                        if (is_array($arrDocObrasPar)):
                                            foreach($arrDocObrasPar as $docObrasPar):
                                        ?>
                                        <tr>
                                            <td><?php echo $docObrasPar['processo']; ?></td>
                                            <td><?php echo $docObrasPar['doc']; ?></td>
                                            <td><?php echo $docObrasPar['tipodocumento']; ?></td>
                                            <td><?php echo $docObrasPar['data_vigencia']; ?></td>
                                            <td><?=( is_numeric($docObrasPar['qtdobra']) ? simec_number_format($docObrasPar['qtdobra'], 0, ',', '.') : $docObrasPar['qtdobra']); ?></td>
                                            <td>R$ <?=( is_numeric($docObrasPar['vt']) ? simec_number_format($docObrasPar['vt'], 2, ',', '.') : $docObrasPar['vt']); ?></td>
                                            <td>R$ <?=( is_numeric($docObrasPar['ve']) ? simec_number_format($docObrasPar['ve'], 2, ',', '.') : $docObrasPar['ve']); ?></td>
                                            <td>R$ <?=( is_numeric($docObrasPar['ps']) ? simec_number_format($docObrasPar['ps'], 2, ',', '.') : $docObrasPar['ps']); ?></td>
                                            <td>R$ <?=( is_numeric($docObrasPar['vp']) ? simec_number_format($docObrasPar['vp'], 2, ',', '.') : $docObrasPar['vp']); ?></td>
                                            <td><?php echo $docObrasPar['dados_bancarios']; ?></td>
                                            <td>R$ <?=( is_numeric($docObrasPar['sb']) ? simec_number_format($docObrasPar['sb'], 2, ',', '.') : $docObrasPar['sb']); ?></td>
                                        </tr>
                                        <?php
                                            endforeach;
                                        endif;
                                        ?>
                                    </table>
                                    <!-- FIM DOCUMENTOS DE OBRAS DO PAR -->
                                    <br /><br />
                                    <!-- DOCUMENTOS DO PAC -->
                                    <?php 
                                    $sql = "SELECT
				pro.pronumeroprocesso as processo,
				'PAC2'||to_char(tc.terid,'00000')||'/'||to_char(tc.terdatainclusao,'YYYY') as ternum,
				CASE WHEN terassinado = 't' AND tc.terdataassinatura IS NULL THEN 'Validado Manualmente' ELSE to_char(tc.terdataassinatura, 'DD/MM/YYYY') END as data,
				to_char(vig.data, 'DD/MM/YYYY') as datafimvigencia,
				CASE WHEN terassinado = 't' AND tc.terdataassinatura IS NULL THEN 'Validado Manualmente' ELSE u.usunome END as usu,
                                (select count(pc.preid) from par.processoobra po inner join par.processoobraspaccomposicao pc on pc.proid = po.proid where
                                        pc.pocstatus = 'A' and po.proid = pro.proid) as qtdObra,
				( select sum( prevalorobra ) from par.termoobra ter inner join obras.preobra po on po.preid = ter.preid AND po.prestatus = 'A' WHERE ter.terid = tc.terid ) as valor_termo,
				em.valor as valorempenho,
				sum(pm.valor_pagamento) as valorpagamentosolicitado,
				sum(CASE WHEN pm.pagsituacaopagamento = '2 - EFETIVADO' THEN pm.valor_pagamento ELSE 0 END) as valorpagamento,
				'Banco: '||coalesce(probanco, 'n/a')||'
 Conta: '||coalesce(proagencia, 'n/a')||'
 Conta Corrente: '||coalesce(nu_conta_corrente, 'n/a') as dados_bancarios,
				 (
                    select coalesce(sum(saldo)::text, 'N�o Informado') as saldo from
                    (
                            select (dfi.dfisaldoconta + dfi.dfisaldofundo + dfi.dfisaldopoupanca + dfi.dfisaldordbcdb) AS saldo
                            from painel.dadosfinanceirosconvenios dfi
                            where dfi.dfiprocesso = pronumeroprocesso
                            and to_char(dfi.dfidatasaldo, 'YYYYMM') = to_char((now() - INTERVAL '1 MONTH'), 'YYYYMM')
                            union
                            select null
                    ) as saldomes
                )  as sb
			FROM 
				par.termocompromissopac  tc
			INNER JOIN par.processoobra 	pro ON pro.proid = tc.proid and pro.prostatus = 'A'
			LEFT  JOIN seguranca.usuario 	u 	ON u.usucpf = tc.usucpfassinatura 
			LEFT JOIN ( 
				SELECT
					terid,
					sum(valor) as valor
				FROM
					( 
					SELECT DISTINCT
						tc.terid, 
						vve.empid,
						vve.vrlempenhocancelado + coalesce(emr.vrlreforco,0) as valor
					FROM 
						par.termocompromissopac tc
					INNER JOIN par.processoobra 			prp ON prp.proid = tc.proid and prp.prostatus = 'A'
					INNER JOIN par.empenho 					emp ON emp.empnumeroprocesso = prp.pronumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
					INNER JOIN par.v_vrlempenhocancelado 	vve ON vve.empid = emp.empid
					LEFT  JOIN (
						SELECT empnumeroprocesso, empidpai, sum(empvalorempenho) as vrlreforco, empcodigoespecie 
						FROM par.empenho
						WHERE empcodigoespecie in ('02') AND empstatus = 'A'
						GROUP BY empnumeroprocesso, empcodigoespecie, empidpai) as emr on emr.empidpai = emp.empid 
					INNER JOIN par.empenhoobra ems on ems.empid = emp.empid and eobstatus = 'A' 
					) as foo
				GROUP BY terid
				) as em ON em.terid = tc.terid
			LEFT JOIN (
				SELECT 
					tc.terid, sum( pobvalorpagamento ) as valor_pagamento, pag.pagsituacaopagamento
				FROM 
					par.termocompromissopac tc
				INNER JOIN par.processoobra 	prp ON prp.proid = tc.proid and prp.prostatus = 'A'
				INNER JOIN par.empenho 			emp ON emp.empnumeroprocesso = prp.pronumeroprocesso AND empcodigoespecie NOT IN ('03', '13', '02', '04') AND empstatus = 'A'
				INNER JOIN par.pagamento 		pag ON pag.empid = emp.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento not ilike '%CANCELADO%'
				INNER JOIN par.pagamentoobra 	ps  ON ps.pagid = pag.pagid  
				GROUP BY tc.terid, pagsituacaopagamento ) pm ON pm.terid = tc.terid
				left join (
						select distinct
						    terid,
						    max(prazo) as data
						from(
						    SELECT
						        po.preid,
						        case when vig.data is not null then vig.data else (MIN(pag.pagdatapagamentosiafi) + 720) end  as prazo,
						        tc.terid, popvalidacao
						    FROM
						        par.pagamentoobra po
						        inner join par.pagamento pag ON pag.pagid = po.pagid AND pag.pagstatus = 'A'
						        inner join par.empenho emp on emp.empid = pag.empid and emp.empstatus = 'A'
						        inner join par.termoobraspaccomposicao tc on tc.preid = po.preid
						                                    
						        left join obras.preobraprorrogacao pp on pp.preid = po.preid and pp.popdataprazoaprovado is not null and pp.popvalidacao = 't'
						        left join(
						            SELECT popdataprazoaprovado as data, preid FROM obras.preobraprorrogacao WHERE popstatus = 'A'
						        ) vig on vig.preid = po.preid
						group by po.preid, vig.data, tc.terid, popvalidacao
						) as foo 
						group by terid) vig on vig.terid = tc.terid
			WHERE 
				tc.terstatus = 'A'	
				AND tc.muncod = '{$muncod}'
			GROUP BY
				tc.usucpfassinatura, tc.terassinado, tc.proid, tc.terid, tc.terdatainclusao, tc.terdataassinatura,
				u.usunome, vig.data,
				em.valor, pro.proid,
				pro.probanco, pro.proagencia, pro.nu_conta_corrente, pro.pronumeroprocesso
			ORDER BY
				tc.terid";
                                    $arrDocObrasPAC = $db->carregar($sql); 
                                    ?>
                                    <table border="1" style="border-bottom-style: solid; border-color: black;" align="center" width="100%" cellspacing="0" cellpadding="5">
                                        <caption><strong>Documentos do PAC</strong></caption>
                                        <tr>
                                            <th>N� do Processo</th>
                                            <th>N� do Documento</th>
                                            <th>Data da Valida��o</th>
                                            <th>Vig�ncia do Termo</th>
                                            <th>Usu�rio da Valida��o</th>
                                            <th>Qnt de Obras</th>
                                            <th>Valor do Termo</th>
                                            <th>Valor Empenhado</th>
                                            <th>Pagamento Solicitado</th>
                                            <th>Pagamento Efetivado</th>
                                            <th>Dados Banc�rios</th>
                                            <th>Saldo Banc�rio <br/>(CC+CP+Fundo)</th>
                                        </tr>
                                        <?php
                                        if (is_array($arrDocObrasPAC)):
                                            foreach($arrDocObrasPAC as $docObrasPAC):
                                        ?>
                                        <tr>
                                            <td><?php echo $docObrasPAC['processo']; ?></td>
                                            <td><?php echo $docObrasPAC['ternum']; ?></td>
                                            <td><?php echo $docObrasPAC['data']; ?></td>
                                            <td><?php echo $docObrasPAC['datafimvigencia']; ?></td>
                                            <td><?php echo $docObrasPAC['usu'] ?></td>
                                            <td><?=( is_numeric($docObrasPAC['vp']) ? simec_number_format($docObrasPAC['qtdobra'], 0, ',', '.') : $docObrasPAC['qtdobra']); ?></td>
                                            <td>R$ <?=( is_numeric($docObrasPAC['vp']) ? simec_number_format($docObrasPAC['valor_termo'], 2, ',', '.') : $docObrasPAC['qtdobra']); ?></td>
                                            <td>R$ <?=( is_numeric($docObrasPAC['vp']) ? simec_number_format($docObrasPAC['valorempenho'], 2, ',', '.') : $docObrasPAC['qtdobra']); ?></td>
                                            <td>R$ <?=( is_numeric($docObrasPAC['vp']) ? simec_number_format($docObrasPAC['valorpagamentosolicitado'], 2, ',', '.') : $docObrasPAC['qtdobra']); ?></td>
                                            <td>R$ <?=( is_numeric($docObrasPAC['sb']) ? simec_number_format($docObrasPAC['valorpagamento'], 2, ',', '.') : $docObrasPAC['qtdobra']); ?></td>
                                            <td><?php echo $docObrasPAC['dados_bancarios']; ?></td>
                                            <td>R$ <?=( is_numeric($docObrasPAC['sb']) ? simec_number_format($docObrasPAC['sb'], 2, ',', '.') : $docObrasPAC['qtdobra']); ?></td>
                                        </tr>
                                        <?php
                                            endforeach;
                                        endif;
                                        ?>
                                    </table>
                                    <!-- FIM DOCUMENTOS DO PAC -->
                                </td>
                            </tr>
                            <tr>
                                <td align="justify" style="font-family: Calibri">
                                    Para o(s) Termo(s) de Compromisso que j� foi (foram) validado(s), mas os recursos correspondentes ainda n�o foram disponibilizados, o munic�pio estar� apto a receber esses recursos somente depois que anexar o(s) contrato(s) no SIMEC - M�DULO PAR.<br/><br/> 
                                    Para outras informa��es, consulte a Diretoria de Gest�o, Articula��o e Projetos Educacionais do FNDE pelos fones (61) 2022-4903/5909/4359/5282 ou pelo e-mail digap@fnde.gov.br.
                                </td>
                            </tr>
                            <tr>
                                <td style="text-align: center;">
                                    <table border="1" style="border-bottom-style: solid; border-color: black;" align="center" width="100%" cellspacing="0" cellpadding="5">
                                        <tr>
                                            <td>Sr. Prefeito, o Plano de A��es Articuladas do seu munic�pio encontra-se em: <b><?=$arrDadosMunicipio['situacao'] ?></b></td>
                                        </tr>
                                        <?php
                                                if( $arrDadosMunicipio['situacao'] == "Diagn�stico" ){
                                                        $strSit = "<b>Diagn�stico</b> significa que o munic�pio deve realizar o diagn�stico da realidade educacional local, elaborar o seu Plano de A��es Articuladas e enviar 
                                                                para an�lise do MEC/FNDE.";
                                                } elseif( $arrDadosMunicipio['situacao'] == "Elabora��o" ){
                                                        $strSit = "<b>Elabora��o</b> significa que o munic�pio deve concluir a elabora��o e enviar para an�lise do MEC/FNDE.";
                                                } elseif( $arrDadosMunicipio['situacao'] == "An�lise" ){
                                                        $strSit = "<b>An�lise</b> significa que o munic�pio deve acompanhar e verificar se h� suba��es analisadas e aprovadas que geraram Termo de 
                                                                Compromisso. Nesse caso, o(s) Termo(s) precisa(m) ser validado(s) eletronicamente pelo(a) prefeito(a). Suba��es em dilig�ncia devem 
                                                                ser corrigidas e enviadas novamente para an�lise do  MEC/FNDE.";
                                                }
                                        ?>
                                        <tr>
                                            <td style="text-align: left; font-size: 12px"><?=$strSit ?></td>
                                        </tr>
                                    </table>
                                </td>					
                            </tr>
                        </table>
                    </td>
		</tr>
            </tbody>
            <tfoot>
		<tr>
                    <td>
                        <table border="0" align="center" width="100%" cellspacing="4" cellpadding="5" style="border-bottom-style: solid; font-size: 12px; border-color: black;">
                            <tr>
                                <td align="right"><?=date("j/n/Y H:i:s"); ?></td>
                            </tr>
                        </table>
                    </td>
		</tr>
            </tfoot>
	</table>
	
	
	<!-- Quebra Pagina -->	
	<div class="quebra"></div>
	
	<table border="0" align="left" width="100%" cellspacing="0" cellpadding="0" style="font-family: Calibri">
            <thead>
		<tr>
                    <td>
                        <table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
                            <tr>
                                <td><img src="imagem/cabecalho-7.jpg" width="710px" alt="" ></img></td>
                            </tr>
                            <tr style="color: white; background-color: #00008B; text-align: center; font-size: 17px">
                                <td><?php echo 'Munic�pio: <b>'.$arrDadosMunicipio['nome'].' - '.$arrDadosMunicipio['uf'].'</b>'; ?></td>
                            </tr>
                        </table>
                    </td>
		</tr>
            </thead>
            <tbody>
		<tr>
                    <td>
                        <table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
                            <tr>
                                <td align="justify" style="font-family: Calibri"><b>5) Registro de pre�os</b><br/><br/>
                                    O Registro de Pre�os Nacional � RPN � um modelo gerencial pelo qual o Governo Federal realiza apenas um �nico processo de compra para todo o pa�s, 
                                    disponibilizando o registro de pre�os de produtos de empresas licitadas, obtido por meio de preg�o eletr�nico. Com base na demanda proveniente dos 
                                    alunos da educa��o b�sica, s�o confeccionadas especifica��es de produtos e materiais para atendimento ao sistema educacional brasileiro 
                                    (estados e munic�pios), proporcionando adequa��o �s reais necessidades de seus usu�rios e maior qualidade. Dentre os produtos destacam-se o 
                                    �nibus escolar, e os mobili�rios escolares, os uniformes escolares. Todos os produtos registrados podem ser conhecidos pelo acesso no m�dulo "Produtos" em 
                                    <a href="http://www.fnde.gov.br/portaldecompras">http://www.fnde.gov.br/portaldecompras/</a>.
                                </td>
                            </tr>
                        </table>
                    </td>
		</tr>
		<tr>
                    <td>
                        <table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
                            <tr>
                                <td align="justify" style="font-family: Calibri"><b>6) Caminho da Escola</b><br/><br/>
                                    O Caminho da Escola, criado em 2007, tem o objetivo de renovar a frota de ve�culos escolares, ampliando, por meio do transporte di�rio, o acesso e a 
                                    perman�ncia dos estudantes na escola. Por meio do Registro de Pre�os Nacional, se encontra dispon�vel para a Prefeitura o �nibus Rural Escolar para uso 
                                    na �rea rural. Em 2012, o Projeto Viver Sem Limites se integrou ao Programa, disponibilizando o �nibus Urbano Escolar  Acess�vel. Al�m de �nibus, faz 
                                    parte do projeto a compra de lanchas escolares e bicicletas. As informa��es t�cnicas para ades�o e contrata��o encontram-se dispon�veis em: <br/> 
                                    <a href="http://www.fnde.gov.br/portaldecompras/index.php/produtos/onibus-escolar-rural">http://www.fnde.gov.br/portaldecompras/index.php/produtos/onibus-escolar-rural</a>.<br/><br/>
                                    
                                    O programa consiste na aquisi��o, por meio de preg�o eletr�nico para registro de pre�os realizado pelo FNDE, de ve�culos padronizados para o transporte escolar. Existem tr�s formas para estados e munic�pios participarem do Caminho da Escola: com recursos pr�prios, bastando aderir ao preg�o; via conv�nio firmado com o FNDE; ou por meio de financiamento do Banco Nacional de Desenvolvimento Econ�mico e Social (BNDES), que disponibiliza linha de cr�dito especial para a aquisi��o de �nibus zero quil�metro e de embarca��es novas.
                                </td>
                            </tr>
                        </table>
                    </td>
		</tr>
		<?php
		//Caminho da Escola
		$sqlCaminhoEscola = "select 
                                                    tidid, ano, quantidade, valor, ibge
                                        from  (
                                                    select 0 as tidid, '�nibus (2008-2011)' as ano, sum(dsh.dshqtde) as quantidade, sum(dsh.dshvalor) as valor, dsh.dshcodmunicipio as ibge
                                                    from painel.indicador i
                                                    inner join painel.seriehistorica sh on sh.indid = i.indid
                                                    inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                                                    inner join painel.detalheperiodicidade dp on dp.dpeid = sh.dpeid
                                                    where i.indid = 1865 AND dsh.dshcodmunicipio = '".$muncod."'
                                                    and dp.dpeanoref between '2008' and '2011'
                                                    and dsh.tidid1 = 20
                                                    group by ano, dsh.dshcodmunicipio
                                        union all
                                                    select tdi1.tidid as tidid, '�nibus ' || tdi1.tiddsc || ' - ' || dp.dpeanoref as ano, sum(dsh.dshqtde) as quantidade, sum(dsh.dshvalor) as valor, dsh.dshcodmunicipio as ibge
                                                    from painel.indicador i
                                                    inner join painel.seriehistorica sh on sh.indid = i.indid
                                                    inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                                                    inner join painel.detalheperiodicidade dp on dp.dpeid = sh.dpeid
                                                    inner join painel.detalhetipodadosindicador tdi1 on tdi1.tidid = dsh.tidid1
                                                    where i.indid = 1865 AND dsh.dshcodmunicipio = '".$muncod."'
                                                    and dp.dpeanoref >= '2012'
                                                    and dsh.tidid2 = 3890 --Municipal
                                                    group by ano, dsh.dshcodmunicipio, tdi1.tidid
                                        union all
                                                    --Bicicletas
                                                    select 1 as tidid, 'Bicicletas e capacetes - ' || dp.dpeanoref as ano, sum(dsh.dshqtde) as quantidade, sum(dsh.dshvalor) as valor, dsh.dshcodmunicipio as ibge
                                                    from painel.indicador i
                                                    inner join painel.seriehistorica sh on sh.indid = i.indid
                                                    inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                                                    inner join painel.detalheperiodicidade dp on dp.dpeid = sh.dpeid
                                                    where i.indid = 2888 AND dsh.dshcodmunicipio = '".$muncod."'
                                                    and dp.dpeanoref <= '2014'
                                                    and dsh.tidid1 = 2007
                                                    group by ano, dsh.dshcodmunicipio
                                        union all
                                                    --Lanchas
                                                    select 2 as tidid, 'Lanchas Escolares - ' || dp.dpeanoref as ano, sum(dsh.dshqtde) as quantidade, sum(dsh.dshvalor) as valor, dsh.dshcodmunicipio as ibge
                                                    from painel.indicador i
                                                    inner join painel.seriehistorica sh on sh.indid = i.indid
                                                    inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                                                    inner join painel.detalheperiodicidade dp on dp.dpeid = sh.dpeid
                                                    where i.indid = 2826 AND dsh.dshcodmunicipio = '".$muncod."'
                                                    and dp.dpeanoref <= '2014'
                                                    and dsh.tidid2 = 3360
                                                    group by ano, dsh.dshcodmunicipio
                                        ) as foo
                                        order by ano";
		
		$dadosCaminhoEscola = $db->carregar($sqlCaminhoEscola,null,86400);

		if( is_array($dadosCaminhoEscola) ){
			foreach( $dadosCaminhoEscola as $caminhoE ){
				// �nibus (2008-2011)
				if( $caminhoE['tidid'] == 0 ){
					$quantidade1 = $caminhoE['quantidade'] ? $caminhoE['quantidade'] : 0;
					$valor1 = $caminhoE['valor'] ? $caminhoE['valor'] : 0;
				}
				// Acess�vel (urbano) - 2012
				if( $caminhoE['tidid'] == 3704 ){
					$quantidade2 = $caminhoE['quantidade'] ? $caminhoE['quantidade'] : 0;
					$valor2 = $caminhoE['valor'] ? $caminhoE['valor'] : 0;
				}
				// N�o Acess�vel (rural) - 2012
				if( $caminhoE['tidid'] == 3705 ){
					$quantidade3 = $caminhoE['quantidade'] ? $caminhoE['quantidade'] : 0;
					$valor3 = $caminhoE['valor'] ? $caminhoE['valor'] : 0;
				}
				// Lanchas - 2012
				if( $caminhoE['tidid'] == 2 ){
					$quantidade4 = $caminhoE['quantidade'] ? $caminhoE['quantidade'] : 0;
					$valor4 = $caminhoE['valor'] ? $caminhoE['valor'] : 0;
				}
				// Bicicletas - 2012
				if( $caminhoE['tidid'] == 1 ){
					$quantidade5 = $caminhoE['quantidade'] ? $caminhoE['quantidade'] : 0;
					$valor5 = $caminhoE['valor'] ? $caminhoE['valor'] : 0;
				}
			}
			$caminhoEscolateste = true;
		}
		?>
		<tr>
			<td>
			<br/>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
				<tr>
					<td style="text-align: center;">
						<table border="1" style="border-bottom-style: solid; border-color: black;" align="center" width="100%" cellspacing="0" cellpadding="5">
							<tr>
								<th>Caminho da Escola</th>
								<th>Quantidade</th>
								<th>Valor</th>
							</tr>
							<tr>
								<td style="text-align: left">�nibus (2008-2011)</td>
								<td style="text-align: center;"><?=$quantidade1 ? simec_number_format($quantidade1, 0, ',', '.') : '-' ?></td>
								<td style="text-align: center;">R$ <?=( is_numeric($valor1) ? simec_number_format($valor1, 2, ',', '.') : $valor1 ); ?></td>
							</tr>
							<tr>
								<td style="text-align: left">�nibus Urbano Escolar Acess�vel (2012-2014)</td>
								<td style="text-align: center;"><?=$quantidade2 ? simec_number_format($quantidade2, 0, ',', '.') : '-' ?></td>
								<td style="text-align: center;">R$ <?=( is_numeric($valor2) ? simec_number_format($valor2, 2, ',', '.') : $valor2 ); ?></td>
							</tr>
							<tr>
								<td style="text-align: left">�nibus Rural Escolar (2012-2014)</td>
								<td style="text-align: center;"><?=$quantidade3 ? simec_number_format($quantidade3, 0, ',', '.') : '-' ?></td>
								<td style="text-align: center;">R$ <?=( is_numeric($valor3) ? simec_number_format($valor3, 2, ',', '.') : $valor3 ); ?></td>
							</tr>
							<tr>
								<td style="text-align: left">Lanchas Escolares (2012-2014)</td>
								<td style="text-align: center;"><?=$quantidade4 ? simec_number_format($quantidade4, 0, ',', '.') : '-' ?></td>
								<td style="text-align: center;">R$ <?=( is_numeric($valor4) ? simec_number_format($valor4, 2, ',', '.') : $valor4 ); ?></td>
							</tr>
							<tr>
								<td style="text-align: left">Bicicletas e capacetes (2012-2014)</td>
								<td style="text-align: center;"><?=$quantidade5 ? simec_number_format($quantidade5, 0, ',', '.') : '-' ?></td>
								<td style="text-align: center;">R$ <?=( is_numeric($valor5) ? simec_number_format($valor5, 2, ',', '.') : $valor5 ); ?></td>
							</tr>
						</table>
					</td>					
				</tr>
				</table>
			</td>
		</tr>
		</tbody>
		<tfoot>
		<tr>
			<td>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5" style="border-bottom-style: solid; font-size: 12px; border-color: black;">
					<tr>
						<td align="right"><?php echo date("j/n/Y H:i:s"); ?></td>
					</tr>
				</table>
			</td>
		</tr>
		</tfoot>
	</table>	
	
	<!-- Quebra Pagina -->	
	<div class="quebra"></div>
	<table border="0" align="left" width="100%" cellspacing="0" cellpadding="0" style="font-family: Calibri">
            <thead>
		<tr>
                    <td>
                        <table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
                            <tr>
                                <td><img src="imagem/cabecalho-7.jpg" width="710px" alt="" ></img></td>
                            </tr>
                            <tr style="color: white; background-color: #00008B; text-align: center; font-size: 17px">
                                <td><?php echo 'Munic�pio: <b>'.$arrDadosMunicipio['nome'].' - '.$arrDadosMunicipio['uf'].'</b>'; ?></td>
                            </tr>
                        </table>
                    </td>
		</tr>
            </thead>
            <tbody>
		<tr>
                    <td>
                        <table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
                            <tr>
                                <td align="justify" style="font-family: Calibri"><b>7) Mobili�rio Escolar</b><br/><br/>
                                    � uma a��o que almeja renovar o mobili�rio escolar nas escolas p�blicas. O munic�pio pode aderir �s Atas de Registro de pre�o do FNDE para a aquisi��o de mesas e cadeiras para alunos (3 tamanhos dispon�veis), cadeiras para professor e mesa acess�vel para pessoa com cadeira de rodas. As informa��es t�cnicas para ades�o e contrata��o se encontram dispon�veis em:<br/>
                                    <a href="http://www.fnde.gov.br/portaldecompras/index.php/produtos/mobiliario-escolar" target="_blank">http://www.fnde.gov.br/portaldecompras/index.php/produtos/mobiliario-escolar</a>.
                                </td>
                            </tr>
                            <tr>
                                <td>
                                    <table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
                                        <tr>
                                            <td style="text-align: center;">
                                                <table border="1" style="border-bottom-style: solid; border-color: black;" align="center" width="100%" cellspacing="0" cellpadding="5">
                                                    <tr>
                                                        <th colspan="3">2013</th>
                                                    </tr>
                                                    <tr>
                                                        <th>Categoria</th>
                                                        <th>Quantidade</th>
                                                        <th>Valor</th>
                                                    </tr>
                                                    <tr>
                                                        <td>Conjunto Aluno - tamanho 3</td>
                                                        <td style="text-align: center;"><?=$CJA03qtd ? simec_number_format($CJA03qtd, 0, "", ".") : '-' ?></td>
                                                        <td rowspan="7" style="text-align: center;">R$ <?=simec_number_format($valorMobiliario2012, 2, ",", ".") ?></td>
                                                    </tr>
                                                    <tr>
                                                        <td>Conjunto Aluno - tamanho 4</td>
                                                        <td style="text-align: center;"><?=$CJA04qtd ? simec_number_format($CJA04qtd, 0, "", ".") : '-' ?></td>
                                                    </tr>
                                                    <tr>
                                                        <td>Conjunto Aluno - tamanho 6</td>
                                                        <td style="text-align: center;"><?=$CJA06qtd ? simec_number_format($CJA06qtd, 0, "", ".") : '-' ?></td>
                                                    </tr>
                                                    <tr>
                                                        <td>Conjunto Professor</td>
                                                        <td style="text-align: center;"><?=$CJPqtd ? simec_number_format($CJPqtd, 0, "", ".") : '-' ?></td>
                                                    </tr>
                                                    <tr>
                                                        <td>Conjunto para inform�tica M2C-04</td>
                                                        <td style="text-align: center;"><?=$M2C4qtd ? simec_number_format($M2C4qtd, 0, "", ".") : '-' ?></td>
                                                    </tr>
                                                    <tr>
                                                        <td>Conjunto para inform�tica M2C-06</td>
                                                        <td style="text-align: center;"><?=$M2C6qtd ? simec_number_format($M2C6qtd, 0, "", ".") : '-' ?></td>
                                                    </tr>
                                                    <tr>
                                                        <td>Mesa Acess�vel</td>
                                                        <td style="text-align: center;"><?=$MAqtd ? simec_number_format($MAqtd, 0, "", ".") : '-' ?></td>
                                                    </tr>
                                                </table>
                                            </td>					
                                        </tr>
                                    </table>
                                </td>
                            </tr>
                        </table>
                    </td>
		</tr>
            </tbody>
            <tfoot>
		<tr>
                    <td>
                        <table border="0" align="center" width="100%" cellspacing="4" cellpadding="5" style="border-bottom-style: solid; font-size: 12px; border-color: black;">
                            <tr>
                                <td align="right"><?=date("j/n/Y H:i:s"); ?></td>
                            </tr>
                        </table>
                    </td>
		</tr>
            </tfoot>
	</table>
	
	<!-- Quebra Pagina -->	
	<div class="quebra"></div>
	<table border="0" align="left" width="100%" cellspacing="0" cellpadding="0" style="font-family: Calibri">
            <thead>
		<tr>
                    <td>
                        <table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
                            <tr>
                                <td><img src="imagem/cabecalho-7.jpg" width="710px" alt="" ></img></td>
                            </tr>
                            <tr style="color: white; background-color: #00008B; text-align: center; font-size: 17px">
                                <td><?php echo 'Munic�pio: <b>'.$arrDadosMunicipio['nome'].' - '.$arrDadosMunicipio['uf'].'</b>'; ?></td>
                            </tr>
                        </table>
                    </td>
		</tr>
            </thead>
            <tbody>
		<tr>
                    <td>
                        <table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
                            <tr>
                                <td align="justify" style="font-family: Calibri"><b>8) Tecnologias educacionais</b><br/><br/>
                                    O Programa Nacional de Tecnologias Educacionais � ProInfo promove o uso pedag�gico das tecnologias educacionais (computadores, projetores, tablets, 
                                    notebooks, dentre outras) na rede p�blica de educa��o b�sica, visando melhorar a qualidade e a equidade do sistema de ensino do pa�s, 
                                    com o uso das tecnologias educacionais no apoio aos processos de ensino-aprendizagem das escolas p�blicas.
                                </td>
                            </tr>
                        </table>
                    </td>
		</tr>
		<tr>
                    <td>
                        <table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
                            <tr>
                                <td style="text-align: center;">
                                    <table border="1" style="border-bottom-style: solid; border-color: black;" align="center" width="100%" cellspacing="0" cellpadding="5">
                                        <tr>
                                            <th>Produto</th>
                                            <th>Quantidade</th>
                                        </tr>
                                        <?php if( is_array($dadosTecEduc) ){
                                            foreach( $dadosTecEduc as $tecEduc ){ ?>
                                            <tr>
                                                <td style="text-align: left;"><?=$tecEduc['atendimentos'] ?></td>
                                                <td style="text-align: center;"><?=$tecEduc['quantidade'] ? simec_number_format($tecEduc['quantidade'],0, ',', '.') : '-' ?></td>
                                            </tr>
                                        <?php } } else { ?>
                                            <tr>
                                                <td style="text-align: center;">-</td>
                                                <td style="text-align: center;">-</td>
                                            </tr>
                                        <?php } ?>
                                    </table>
                                </td>					
                            </tr>
                        </table>
                    </td>
		</tr>
		<tr>
                    <td>
                        <table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
                            <tr>
                                <td style="text-align: center;">
                                    <table border="1" style="border-bottom-style: solid; border-color: black;" align="center" width="100%" cellspacing="0" cellpadding="5">
                                        <tr>
                                            <td align="justify">Tecnologias podem ser apoiadas pelo PAR mediante o cadastramento da demanda no Simec para avalia��o. O FNDE mant�m em seu Portal 
                                                de Compras, Atas de Registro de pre�os para que sejam aproveitadas pelos Munic�pios, em uma compra garantida pela qualidade e pela economicidade 
                                                j� avaliadas pelo FNDE. <a href="http://www.fnde.gov.br/portaldecompras/index.php/produtos/laboratorio-de-informatica-proinfo">http://www.fnde.gov.br/portaldecompras/index.php/produtos/laboratorio-de-informatica-proinfo</a>.
                                            </td>
                                        </tr>
                                    </table>
                                </td>					
                            </tr>
                        </table>
                    </td>
		</tr>
            </tbody>
            <tfoot>
		<tr>
                    <td>
                        <table border="0" align="center" width="100%" cellspacing="4" cellpadding="5" style="border-bottom-style: solid; font-size: 12px; border-color: black;">
                            <tr>
                                <td align="right"><?php echo date("j/n/Y H:i:s"); ?></td>
                            </tr>
                        </table>
                    </td>
		</tr>
            </tfoot>
	</table>	
	
	<!-- Quebra Pagina -->	
	<div class="quebra"></div>
	
	<table border="0" align="left" width="100%" cellspacing="0" cellpadding="0" style="font-family: Calibri">
		<thead>
		<tr>
			<td>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
					<tr>
						<td><img src="imagem/cabecalho-7.jpg" width="710px" alt="" ></img></td>
					</tr>
					<tr style="color: white; background-color: #00008B; text-align: center; font-size: 17px">
						<td><?php echo 'Munic�pio: <b>'.$arrDadosMunicipio['nome'].' - '.$arrDadosMunicipio['uf'].'</b>'; ?></td>
					</tr>
				</table>
			</td>
		</tr>
		</thead>
		<tbody>
		<tr>
			<td align="justify" style="font-family: Calibri"><br/><b>9) Educa��o Integral</b><br/><br/>
				Institu�do pela Portaria Interministerial n� 17/2007, o programa Mais Educa��o tem como objetivo promover a amplia��o da jornada e espa�os escolares para no m�nimo sete horas di�rias, visando � implementa��o da Educa��o Integral na rede p�blica de ensino com atividades nas �reas de acompanhamento pedag�gico, cultura, artes e educa��o patrimonial, esporte e lazer, educa��o em direitos humanos, ci�ncias da natureza, educa��o ambiental e desenvolvimento sustent�vel, cultura digital, preven��o de doen�as e promo��o da sa�de, investiga��o no campo das ci�ncias da natureza, entre outros. A ades�o ao Programa Mais Educa��o � realizada pelo site do PDDE Interativo, ferramenta de apoio � gest�o escolar do Minist�rio da Educa��o. <br/><br/>
                                Ap�s o recebimento do of�cio enviado pela Secretaria de Educa��o B�sica (SEB) do MEC, a Prefeitura ou Secretaria Estadual de Educa��o dever�o confirmar a ades�o ao Programa Mais Educa��o e nomear dois t�cnicos da Secretaria Municipal de Educa��o, sendo um coordenador respons�vel pelas atividades realizadas nas escolas e o outro respons�vel pelo funcionamento administrativo do Programa.
			</td>
		</tr>
		<tr>
			<td style="text-align: center;">
			<br/>
				<table border="1" style="border-bottom-style: solid; border-color: black;" align="center" width="100%" cellspacing="0" cellpadding="5">
					<tr>
						<th>Ano</th>
						<th>Escolas que aderiram</th>
						<th>Alunado das escolas que aderiram</th>
						<th>Valor Repassado PDDE</th>
					</tr>
					<tr>
						<td>2012</td>
						<td style="text-align: center;"><?=simec_number_format($quantidadeME1, 0, ',', '.') ?></td>
						<td style="text-align: center;"><?=simec_number_format($quantidadeME2, 0, ',', '.') ?></td>
						<td style="text-align: center;">R$ <?=simec_number_format($quantidadeME3, 2, ',', '.') ?></td>
					</tr>
					<tr>
						<td>2013</td>
						<td style="text-align: center;"><?=simec_number_format($quantidadeME11, 0, ',', '.') ?></td>
						<td style="text-align: center;"><?=simec_number_format($quantidadeME12, 0, ',', '.') ?></td>
						<td style="text-align: center;">R$ <?=simec_number_format($quantidadeME13, 2, ',', '.') ?></td>
					</tr>
					<tr>
						<td>2014</td>
						<td style="text-align: center;"><?=simec_number_format($quantidadeME21, 0, ',', '.') ?></td>
						<td style="text-align: center;"><?=simec_number_format($quantidadeME22, 0, ',', '.') ?></td>
						<td style="text-align: center;">R$ <?=simec_number_format($quantidadeME23, 2, ',', '.') ?></td>
					</tr>
				</table>
			</td>					
		</tr>
		<tr>
			<td>
			<br/>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
					
					<tr>
						<td>
							<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
							<tr>
								<td style="text-align: center;">
									<table border="1" style="border-bottom-style: solid; border-color: black;" align="center" width="100%" cellspacing="0" cellpadding="5">
										<tr>
											<td align="justify">Ap�s o recebimento do of�cio enviado pela Secretaria de Educa��o B�sica (SEB) do MEC, a Prefeitura 
											dever� confirmar a ades�o ao Programa Mais Educa��o e nomear dois t�cnicos da Secretaria Municipal de Educa��o, sendo um 
											coordenador respons�vel pelas atividades realizadas nas escolas participantes do Programa e o outro respons�vel pelo funcionamento 
											administrativo do Programa.
											</td>
										</tr>
									</table>
								</td>					
							</tr>
							</table>
						</td>
					</tr>
				</table>
			</td>
		</tr>
		</tbody>
		<tfoot>
		<tr>
			<td>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5" style="border-bottom-style: solid; font-size: 12px; border-color: black;">
					<tr>
						<td align="right"><?=date("j/n/Y H:i:s"); ?></td>
					</tr>
				</table>
			</td>
		</tr>
		</tfoot>
	</table>
		
		
		
	<!-- Quebra Pagina -->	
	<div class="quebra"></div>
	
	<table border="0" align="left" width="100%" cellspacing="0" cellpadding="0" style="font-family: Calibri">
		<thead>
		<tr>
			<td>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
					<tr>
						<td><img src="imagem/cabecalho-7.jpg" width="710px" alt="" ></img></td>
					</tr>
					<tr style="color: white; background-color: #00008B; text-align: center; font-size: 17px">
						<td><?php echo 'Munic�pio: <b>'.$arrDadosMunicipio['nome'].' - '.$arrDadosMunicipio['uf'].'</b>'; ?></td>
					</tr>
				</table>
			</td>
		</tr>
		</thead>
		<tbody>
		<tr>
			<td>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
					<tr>
						<td align="justify" style="font-family: Calibri"><b>10) PDDE-Interativo</b><br/><br/>
							O PDDE Interativo � uma ferramenta de apoio � gest�o escolar desenvolvida pelo Minist�rio da Educa��o, em parceria com as Secretarias de Educa��o, 
							e est� dispon�vel para todas as escolas p�blicas cadastradas no Censo Escolar de 2013. V�rios programas do MEC, que tem intera��o com a escola, devem utilizar o 
							PDDE-Interativo como ferramenta de apoia � gest�o escolar. S�o eles os programas: a) Mais Educa��o; b) PDE-Escola; c) Ensino M�dio Inovador; 
							d) Atleta na Escola; e) Escolas Sustent�veis; f) Escolas do Campo; g) �gua na Escola e Esgotamento Sanit�rio; e h) Forma��o Continuada. 
							� por meio deste sistema que os recursos de v�rios programas s�o viabilizados para as escolas. <br><br>

							Os munic�pios que ainda n�o cadastraram o comit� municipal do PDDE-Interativo � que � respons�vel por prover acesso das escolas ao sistema � 
							devem entrar em contato com os t�cnicos do Minist�rio da Educa��o, por meio do e-mail <?php echo $_SESSION['email_sistema']; ?>, 
							dispon�vel no s�tio <a href="http://pddeinterativo.mec.gov.br/">http://pddeinterativo.mec.gov.br/</a>, pedindo acesso para o Secret�rio de Educa��o ao sistema. 
							O Secret�rio de Educa��o, por sua vez, ao acessar o sistema, deve definir os integrantes do Comit� Municipal do PDDE-Interativo que ser�o respons�veis por 
							identificar os diretores de cada escola de sua rede de ensino. Este comit� tamb�m � respons�vel por atribuir login e senha para os diretores de 
							escola no sistema e apoi�-los sempre que for demandado. Sem o Comit� Municipal do PDDE-Interativo, a rede de ensino n�o tem acesso aos programas identificados 
							acima, deixando de receber recursos para a implementa��o de importantes pol�ticas educacionais. 
						</td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
					<tr>
						<td align="justify" style="font-family: Calibri"><b>11) Programa Dinheiro Direto na Escola - PDDE</b><br/><br/>
							O MEC, por meio do Programa Dinheiro Direto na Escola, transfere recursos financeiros diretamente �s escolas p�blicas de educa��o b�sica e escolas privadas de educa��o especial, de forma suplementar, visando � melhoria da infraestrutura f�sica e pedag�gica, � autonomia gerencial dos recursos e � participa��o coletiva na gest�o e no controle social.<br/><br/>
							Pelo PDDE s�o transferidos tamb�m recursos para as a��es de Educa��o Integral, Mais Cultura, Atleta na Escola, PDE Escola, Escola do Campo, �gua e Esgotamento Sanit�rio, Ensino M�dio Inovador, Escola Sustent�vel e Escola Acess�vel. Para que o FNDE repasse os recursos, os governos municipais, estaduais e distrital, as Unidades Executoras Pr�prias (representativas de escolas p�blicas), e Entidades Mantenedoras (representativas de escolas privadas de educa��o especial) devem manter sempre seus cadastros atualizados no PDDEWEB, sistema informatizado, dispon�vel no s�tio: www.fnde.gov.br, por meio do qual s�o formalizados os procedimentos de ades�o, cadastro e atualiza��o cadastral. Importante: sempre que houver mudan�as nos dados da entidade ou de seus dirigentes, as informa��es devem ser imediatamente informadas no sistema mencionado.
						</td>
					</tr>
					<tr>
						<td>
						<br/>
							<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
							<tr>
								<td style="text-align: center;">
									<table border="1" style="border-bottom-style: solid; border-color: black;" align="center" width="100%" cellspacing="0" cellpadding="5">
										<tr>
											<th>PDDE</th>
											<th>2011</th>
											<th>2012</th>
											<th>2013</th>
											<th>2014</th>
										</tr>
										<tr>
											<td style="text-align: left;">Escolas Beneficiadas</td>
											<td style="text-align: center;"><?=$PDDEescolasben2011 ? simec_number_format($PDDEescolasben2011, 0, ',', '.') : '-' ?></td>
											<td style="text-align: center;"><?=$PDDEescolasben2012 ? simec_number_format($PDDEescolasben2012, 0, ',', '.') : '-' ?></td>
											<td style="text-align: center;"><?=$PDDEescolasben2013 ? simec_number_format($PDDEescolasben2013, 0, ',', '.') : '-' ?></td>
											<td style="text-align: center;"><?=$PDDEescolasben2014 ? simec_number_format($PDDEescolasben2014, 0, ',', '.') : '-' ?></td>
										</tr>
										<tr>
											<td style="text-align: left;">Recursos repassados</td>
											<td style="text-align: center;">R$ <?=simec_number_format($PDDErecursosrep2011, 2, ",",".") ?></td>
											<td style="text-align: center;">R$ <?=simec_number_format($PDDErecursosrep2012, 2, ",",".") ?></td>
											<td style="text-align: center;">R$ <?=simec_number_format($PDDErecursosrep2013, 2, ",",".") ?></td>
											<td style="text-align: center;">R$ <?=simec_number_format($PDDErecursosrep2014, 2, ",",".") ?></td>
										</tr>
									</table>
								</td>					
							</tr>
							</table>
						</td>
					</tr>
				</table>
			</td>
		</tr>
		</tbody>
		<tfoot>
		<tr>
			<td>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5" style="border-bottom-style: solid; font-size: 12px; border-color: black;">
					<tr>
						<td align="right"><?=date("j/n/Y H:i:s"); ?></td>
					</tr>
				</table>
			</td>
		</tr>
		</tfoot>
	</table>
		
		
		
	<!-- Quebra Pagina -->	
	<div class="quebra"></div>
	
	<table border="0" align="left" width="100%" cellspacing="0" cellpadding="0" style="font-family: Calibri">
		<thead>
		<tr>
			<td>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
					<tr>
						<td><img src="imagem/cabecalho-7.jpg" width="710px" alt="" ></img></td>
					</tr>
					<tr style="color: white; background-color: #00008B; text-align: center; font-size: 17px">
						<td><?php echo 'Munic�pio: <b>'.$arrDadosMunicipio['nome'].' - '.$arrDadosMunicipio['uf'].'</b>'; ?></td>
					</tr>
				</table>
			</td>
		</tr>
		</thead>
		<tbody>
		<tr>
			<td>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
					<tr>
						<td align="justify" style="font-family: Calibri"><b>12) PACTO NACIONAL PELA ALFABETIZA��O NA IDADE CERTA</b><br/><br/>
							O Pacto Nacional pela Alfabetiza��o na Idade Certa � um compromisso formal assumido pelos governos federal, do Distrito Federal, dos estados e dos 
							munic�pios de assegurar que todas as crian�as estejam alfabetizadas at� os oito anos de idade, ao final do 3� ano do ensino fundamental.<br/><br/>As a��es
							do Pacto s�o um conjunto integrado de programas, materiais e refer�ncias curriculares e pedag�gicas que ser�o  disponibilizados pelo MEC e que contribuem 
							para a alfabetiza��o e o letramento. Estas a��es apoiam-se em quatro eixos de atua��o: (I) Forma��o Continuada de Professores Alfabetizadores; (II) 
							Materiais Did�ticos e Pedag�gicos; (III) Avalia��es; e (IV) Gest�o, Controle Social e Mobiliza��o.
							A Forma��o ser� realizada entre pares: orientadores de estudo, escolhidos entre os pr�prios professores pertencentes ao quadro das redes de ensino e 
							com experi�ncia como tutores do Pr�-Letramento, formar�o os professores alfabetizadores.
						</td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td>
                            <table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
				<tr>
					<td style="text-align: center;">
						<table border="1" style="border-bottom-style: solid; border-color: black;" align="center" width="100%" cellspacing="0" cellpadding="5">
							<tr>
								<th>Ano</th>
								<th>Etapa</th>
								<th>Situa��o</th>
								<th>Professores inscritos</th>
								<th>Universidade respons�vel pela forma��o</th>
							</tr>
							<tr>
                                                            <?php
                                                            #Informacoes 2013 Indicador: 2840
                                                            $sql = "SELECT 
                                                                        tdi1.tiddsc AS tipo, 
                                                                        SUM(dsh.dshqtde) AS quantidade, 
                                                                        dsh.dshcodmunicipio AS ibge
                                                                    FROM painel.indicador i
                                                                    INNER JOIN painel.seriehistorica sh ON sh.indid = i.indid
                                                                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                                                                    INNER JOIN painel.detalhetipodadosindicador tdi1 ON tdi1.tidid = dsh.tidid1
                                                                    INNER JOIN painel.detalheperiodicidade dp ON dp.dpeid = sh.dpeid
                                                                    WHERE i.indid = 2840 AND dp.dpeanoref = '2013' AND dsh.dshcodmunicipio = '".$muncod."'
                                                                   -- AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid)
                                                                        GROUP BY tdi1.tiddsc, dsh.dshcodmunicipio
                                                                        ORDER BY tdi1.tiddsc";
                                                            $arrIndicador2840 = $db->carregar($sql);
                                                            if(is_array($arrIndicador2840)){
                                                                $pactonacionaltx2840 = "";
                                                                foreach( $arrIndicador2840 as $indicador2840 ){
                                                                    $pactonacionaltx2840 .= $indicador2840['tipo'].": ". ( $indicador2840['quantidade'] ? simec_number_format($indicador2840['quantidade'], 0, ',', '.') : '-' )." <br> ";		
                                                                }
                                                            }
                                                            ?>
                                                            <td style="text-align: center; font-size: 12px;">2013</td>
                                                            <td style="text-align: center; font-size: 12px;">Ades�o</td>
                                                            <td style="text-align: center; font-size: 12px;"><?php echo $situacaoAdesao ? $situacaoAdesao : '-' ?></td>
                                                            <td style="text-align: center; font-size: 12px;"><?php echo $pactonacionaltx2840 ? $pactonacionaltx2840 : '-'?></td>
                                                            <td style="text-align: center; font-size: 12px;"><?php echo $dadosPacto['universidade'] ? $dadosPacto['universidade'] : '-' ?></td>
							</tr>
							<tr>
                                                            <?php
                                                            #Informacoes 2014 Indicador: 2841
                                                            $sql = "SELECT 
                                                                        tdi1.tiddsc AS tipo, 
                                                                        SUM(dsh.dshqtde) AS quantidade, 
                                                                        dsh.dshcodmunicipio AS ibge
                                                                    FROM painel.indicador i
                                                                    INNER JOIN painel.seriehistorica sh ON sh.indid = i.indid
                                                                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                                                                    INNER JOIN painel.detalhetipodadosindicador tdi1 ON tdi1.tidid = dsh.tidid1
                                                                    INNER JOIN painel.detalheperiodicidade dp ON dp.dpeid = sh.dpeid
                                                                    WHERE i.indid = 2841 AND dp.dpeanoref = '2015' AND dsh.dshcodmunicipio = '".$muncod."'
                                                                   -- AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid)
                                                                        GROUP BY tdi1.tiddsc, dsh.dshcodmunicipio
                                                                        ORDER BY tdi1.tiddsc";
                                                            $arrIndicador2841 = $db->carregar($sql);
                                                            if(is_array($arrIndicador2841)){
                                                                $pactonacionaltx2841 = "";
                                                                foreach( $arrIndicador2841 as $indicador2841 ){
                                                                    $pactonacionaltx2841 .= $indicador2841['tipo'].": ". ( $indicador2841['quantidade'] ? simec_number_format($indicador2841['quantidade'], 0, ',', '.') : '-' )." <br> ";		
                                                                }
                                                            }
                                                            ?>
                                                            <td style="text-align: center; font-size: 12px;">2014</td>
                                                            <td style="text-align: center; font-size: 12px;">Ades�o</td>
                                                            <td style="text-align: center; font-size: 12px;"><?php echo $situacaoAdesao ? $situacaoAdesao : '-' ?></td>
                                                            <td style="text-align: center; font-size: 12px;"><?php echo $pactonacionaltx2841 ? $pactonacionaltx2841 : '-' ?></td>
                                                            <td style="text-align: center; font-size: 12px;"><?php echo $dadosPacto['universidade'] ? $dadosPacto['universidade'] : '-' ?></td>
							</tr>
						</table>
					</td>					
				</tr>
                            </table>
<!--                            <table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
				<tr>
					<td style="text-align: center;">
						<table border="1" style="border-bottom-style: solid; border-color: black;" align="center" width="100%" cellspacing="0" cellpadding="5">
							<tr>
								<th width="30%">Etapa</th>
								<th width="30%">Situa��o</th>
								<th>O que fazer</th>
							</tr>
							<tr>
								<td style="text-align: center; font-size: 12px;">Ades�o</td>
								<td style="text-align: center; font-size: 12px;"><?=$situacaoAdesao ? $situacaoAdesao : '-' ?></td>
								<td style="text-align: center; font-size: 12px;"><?=$oquefazerAdesao ? $oquefazerAdesao : '-' ?></td>
							</tr>
							<tr>
								<td style="text-align: center; font-size: 12px;" colspan="2">Universidade respons�vel pela forma��o dos orientadores de estudo:</td>
								<td style="text-align: center; font-size: 12px;"><?=$dadosPacto['universidade'] ? $dadosPacto['universidade'] : '-' ?></td>
							</tr>
							<tr>
								<td td style="text-align: center; font-size: 12px;" colspan="2">Quantidade de professores cadastrados para o programa de forma��o:</td>
								<td style="text-align: center; font-size: 12px;"><?php echo $pactonacionaltx; ?></td>
							</tr>
						</table>
					</td>					
				</tr>
                            </table>-->
			</td>
		</tr>
		</tbody>
		<tfoot>
		<tr>
			<td>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5" style="border-bottom-style: solid; font-size: 12px; border-color: black;">
					<tr>
						<td align="right"><?=date("j/n/Y H:i:s"); ?></td>
					</tr>
				</table>
			</td>
		</tr>
		</tfoot>
	</table>
        
        <!-- Quebra Pagina -->	
	<div class="quebra"></div>
	
	<table border="0" align="left" width="100%" cellspacing="0" cellpadding="0" style="font-family: Calibri">
		<thead>
		<tr>
			<td>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
					<tr>
						<td><img src="imagem/cabecalho-7.jpg" width="710px" alt="" ></img></td>
					</tr>
					<tr style="color: white; background-color: #00008B; text-align: center; font-size: 17px">
						<td><?php echo 'Munic�pio: <b>'.$arrDadosMunicipio['nome'].' - '.$arrDadosMunicipio['uf'].'</b>'; ?></td>
					</tr>
				</table>
			</td>
		</tr>
		</thead>
		<tbody>
		<tr>
			<td>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
					<tr>
						<td align="justify" style="font-family: Calibri"><b>13) PACTO NACIONAL PELO FORTALECIMENTO DO ENSINO M�DIO</b><br/><br/>
							O Pacto Nacional pelo Fortalecimento do Ensino M�dio, representa a articula��o e a coordena��o de a��es e estrat�gias entre a Uni�o e os governos estaduais e distrital na formula��o e implanta��o de pol�ticas para elevar o padr�o de qualidade do Ensino M�dio brasileiro, em suas diferentes modalidades, orientado pela perspectiva de inclus�o de todos que a ele tem direito. Neste primeiro momento duas a��es estrat�gicas est�o articuladas, o redesenho curricular, em desenvolvimento nas escolas por meio do Programa Ensino M�dio Inovador - ProEMI.<br/><br/>
                                                        Neste primeiro momento duas a��es estrat�gicas est�o articuladas, o redesenho curricular, em desenvolvimento nas escolas por meio do Programa Ensino M�dio Inovador - ProEMI e a Forma��o Continuada de professores do Ensino M�dio.
						</td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td>
                            <table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
				<tr>
					<td style="text-align: center;">
						<table border="1" style="border-bottom-style: solid; border-color: black;" align="center" width="100%" cellspacing="0" cellpadding="5">
							<tr>
								<th>Ano</th>
								<th>Etapa</th>
								<th>Situa��o</th>
								<th>Professores inscritos</th>
								<th>Universidade respons�vel pela forma��o</th>
							</tr>
							<tr>
                                                            <?php
                                                            #Informacoes 2014 Indicador: 3106
                                                            $sql = "SELECT 
                                                                        tdi1.tiddsc AS tipo, 
                                                                        SUM(dsh.dshqtde) AS quantidade, 
                                                                        dsh.dshcodmunicipio AS ibge
                                                                    FROM painel.indicador i
                                                                    INNER JOIN painel.seriehistorica sh ON sh.indid = i.indid
                                                                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                                                                    INNER JOIN painel.detalhetipodadosindicador tdi1 ON tdi1.tidid = dsh.tidid1
                                                                    INNER JOIN painel.detalheperiodicidade dp ON dp.dpeid = sh.dpeid
                                                                    WHERE i.indid = 3106 AND dp.dpeanoref = '2014' 
                                                                    AND dsh.dshcodmunicipio = '".$muncod."'
                                                                    --AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid)
                                                                        GROUP BY tdi1.tiddsc, dsh.dshcodmunicipio
                                                                        ORDER BY tdi1.tiddsc";
                                                            $arrIndicador3106 = $db->carregar($sql);
                                                            if(is_array($arrIndicador3106)){
                                                                $pactonacionaltx3106 = "";
                                                                foreach( $arrIndicador3106 as $indicador3106 ){
                                                                    $pactonacionaltx3106 .= $indicador3106['tipo'].": ". ( $indicador3106['quantidade'] ? simec_number_format($indicador3106['quantidade'], 0, ',', '.') : '-' )." <br> ";		
                                                                }
                                                            }
                                                            ?>
                                                            <td style="text-align: center; font-size: 12px;">2014</td>
                                                            <td style="text-align: center; font-size: 12px;">Ades�o</td>
                                                            <td style="text-align: center; font-size: 12px;"><?php echo $situacaoAdesao ? $situacaoAdesao : '-' ?></td>
                                                            <td style="text-align: center; font-size: 12px;"><?php echo ($pactonacionaltx3106)? $pactonacionaltx3106 : '-'?></td>
                                                            <td style="text-align: center; font-size: 12px;"><?php echo $dadosPacto['universidade'] ? $dadosPacto['universidade'] : '-' ?></td>
							</tr>
						</table>
					</td>					
				</tr>
                            </table>
			</td>
		</tr>
		</tbody>
		<tfoot>
		<tr>
			<td>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5" style="border-bottom-style: solid; font-size: 12px; border-color: black;">
					<tr>
						<td align="right"><?=date("j/n/Y H:i:s"); ?></td>
					</tr>
				</table>
			</td>
		</tr>
		</tfoot>
	</table>
		
		
	<!-- Quebra Pagina -->	
	<div class="quebra"></div>
	
	<table border="0" align="left" width="100%" cellspacing="0" cellpadding="0" style="font-family: Calibri">
		<thead>
		<tr>
			<td>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
					<tr>
						<td><img src="imagem/cabecalho-7.jpg" width="710px" alt="" ></img></td>
					</tr>
					<tr style="color: white; background-color: #00008B; text-align: center; font-size: 17px">
						<td><?php echo 'Munic�pio: <b>'.$arrDadosMunicipio['nome'].' - '.$arrDadosMunicipio['uf'].'</b>'; ?></td>
					</tr>
				</table>
			</td>
		</tr>
		</thead>
		<tbody>
			<tr>
				<td>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
					<tr>
						<td align="justify" style="font-family: Calibri"><b>14) Brasil Carinhoso</b><br/>
							O Brasil Carinhoso faz parte do Plano Brasil sem Mis�ria e contempla tr�s a��es: 
						</td>
					</tr>
					<tr>
						<td align="justify" style="font-family: Calibri"><b>14.1) Creches com crian�as do bolsa fam�lia</b><br/>
							Aux�lio � educa��o infantil para o atendimento de crian�as de zero a 48 meses, informadas no Censo Escolar da Educa��o B�sica, cujas fam�lias sejam benefici�rias do Programa Bolsa Fam�lia, em creches p�blicas ou conveniadas com o poder p�blico. <br/><br/>
							Desde 2014, as transfer�ncias de recursos da Uni�o aos Munic�pios e ao Distrito Federal s�o realizadas automaticamente pelo FNDE, com base na quantidade de matr�culas de crian�as de 0 (zero) a 48 (quarenta e oito) meses cadastradas pelos Munic�pios e pelo Distrito Federal no Censo Escolar da Educa��o B�sica
						</td>
					</tr>
					<tr>
						<td>
							<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
							<tr>
								<td style="text-align: center;">
									<table border="1" style="border-bottom-style: solid; border-color: black;" align="center" width="100%" cellspacing="0" cellpadding="5">
										<tr>
											<th colspan="5">Situa��o das Solicita��es</th>
										</tr>
										<tr>
											<th>Ano</th>
											<th>Quantidade de Estudantes</th>
											<th>Pagamento efetuado</th>
										</tr>
										<tr>
											<td style="text-align: center;">2013</td>
											<td style="text-align: center;"><?=$bcIndicador2013['quantidade'] ? simec_number_format($bcIndicador2013['quantidade'], 0, ',', '.') : '-' ?></td>
											<td style="text-align: center;"><?=$bcIndicador2013['valor'] ? simec_number_format($bcIndicador2013['valor'], 2, ',', '.') : '-' ?></td>
										</tr>
										<tr>
											<td style="text-align: center;">2014</td>
											<td style="text-align: center;"><?=$bcIndicador2014['quantidade'] ? simec_number_format($bcIndicador2014['quantidade'], 0, ',', '.') : '-' ?></td>
											<td style="text-align: center;"><?=$bcIndicador2014['valor'] ? simec_number_format($bcIndicador2014['valor'], 2, ',', '.') : '-' ?></td>
										</tr>
										<tr>
											<td style="text-align: center;"><b>Totalizador</b></td>
											<td style="text-align: center;"><b><?=simec_number_format(((int)$bcIndicador2013['quantidade'] + (int)$bcIndicador2014['quantidade']), 0, ',', '.') ?></b></td>
											<td style="text-align: center;"><b><?=(simec_number_format((float)$bcIndicador2013['valor'] + (float)$bcIndicador2014['valor'], 2, ',', '.')) ?></b></td>
										</tr>
									</table>
								</td>					
							</tr>
							</table>
						</td>
					</tr>
					<tr>
						<td align="justify" style="font-family: Calibri"><b>14.2) Novas turmas de educa��o infantil</b><br/>
							Apoio financeiro � manuten��o de novas matr�culas em novas turmas de educa��o infantil oferecidas em estabelecimentos educacionais p�blicos ou em 
							institui��es comunit�rias, confessionais ou filantr�picas sem fins lucrativos conveniadas com o poder p�blico. <br/><br/>
							Os munic�pios dever�o cadastrar no Simec, M�dulo E. I. Manuten��o, s�tio eletr�nico <a href="http://simec.mec.gov.br">http://simec.mec.gov.br</a>, 
							cada nova turma.
						</td>
					</tr>
					<tr>
						<td>
							<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
							<tr>
								<td style="text-align: center;">
									<table border="1" style="border-bottom-style: solid; border-color: black;" align="center" width="100%" cellspacing="0" cellpadding="5">
										<tr>
											<th colspan="6">Situa��o das Solicita��es</th>
										</tr>
										<tr>
											<th>Ano</th>
											<th>Cadastramento</th>
											<th>Dilig�ncia</th>
											<th>Pagamento efetuado</th>
											<th>Analisado</th>
											<th>Aguardando pagamento</th>
										</tr>
										<tr>
											<td style="text-align: center;">2014</td>
											<td style="text-align: center;"><?=($arrNovasTurmas['2014']['emcadastramento'] ? simec_number_format($arrNovasTurmas['2014']['emcadastramento'], 0, ',', '.') : '-' )?></td>
											<td style="text-align: center;"><?=($arrNovasTurmas['2014']['emdiligencia'] ? simec_number_format($arrNovasTurmas['2014']['emdiligencia'], 0, ',', '.') : '-' )?></td>
											<td style="text-align: center;"><?=($arrNovasTurmas['2014']['pagamentoefetuado'] ? simec_number_format($arrNovasTurmas['2014']['pagamentoefetuado'], 0, ',', '.') : '-' )?></td>
											<td style="text-align: center;"><?=($arrNovasTurmas['2014']['emanalise'] ? simec_number_format($arrNovasTurmas['2014']['emanalise'], 0, ',', '.') : '-' )?></td>
											<td style="text-align: center;"><?=($arrNovasTurmas['2014']['aguardandopagamento'] ? simec_number_format($arrNovasTurmas['2014']['aguardandopagamento'], 0, ',', '.') : '-' )?></td>
										</tr>
										<tr>
											<td style="text-align: center;">2015</td>
											<td style="text-align: center;"><?=($arrNovasTurmas['2015']['emcadastramento'] ? simec_number_format($arrNovasTurmas['2015']['emcadastramento'], 0, ',', '.') : '-' )?></td>
											<td style="text-align: center;"><?=($arrNovasTurmas['2015']['emdiligencia'] ? simec_number_format($arrNovasTurmas['2015']['emdiligencia'], 0, ',', '.') : '-' )?></td>
											<td style="text-align: center;"><?=($arrNovasTurmas['2015']['pagamentoefetuado'] ? simec_number_format($arrNovasTurmas['2015']['pagamentoefetuado'], 0, ',', '.') : '-' )?></td>
											<td style="text-align: center;"><?=($arrNovasTurmas['2015']['emanalise'] ? simec_number_format($arrNovasTurmas['2015']['emanalise'], 0, ',', '.') : '-' )?></td>
											<td style="text-align: center;"><?=($arrNovasTurmas['2015']['aguardandopagamento'] ? simec_number_format($arrNovasTurmas['2015']['aguardandopagamento'], 0, ',', '.') : '-' )?></td>
										</tr>
									</table>
								</td>					
							</tr>
							</table>
						</td>
					</tr>
					</table>
				</td>
			</tr>
		</tbody>
		<tfoot>
			<tr>
				<td>
					<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5" style="border-bottom-style: solid; font-size: 12px; border-color: black;">
						<tr>
							<td align="right"><?=date("j/n/Y H:i:s"); ?></td>
						</tr>
					</table>
				</td>
			</tr>
		</tfoot>
	</table>
		
	
	<!-- Quebra Pagina -->	
	<div class="quebra"></div>
	
	<table border="0" align="left" width="100%" cellspacing="0" cellpadding="0" style="font-family: Calibri">
		<thead>
		<tr>
			<td>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
					<tr>
						<td><img src="imagem/cabecalho-7.jpg" width="710px" alt="" ></img></td>
					</tr>
					<tr style="color: white; background-color: #00008B; text-align: center; font-size: 17px">
						<td><?php echo 'Munic�pio: <b>'.$arrDadosMunicipio['nome'].' - '.$arrDadosMunicipio['uf'].'</b>'; ?></td>
					</tr>
				</table>
			</td>
		</tr>
		</thead>
		<tbody>
			<tr>
				<td>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
					<tr>
						<td align="justify" style="font-family: Calibri"><br/><b>14.3) Novas escolas de educa��o infantil ainda sem FUNDEB</b><br/>
							Apoio � manuten��o de novas escolas p�blicas de educa��o infantil que ainda n�o tenham sido contempladas com os recursos do FUNDEB. <br/><br/>
							Os munic�pios dever�o cadastrar no SIMEC, M�dulo E. I. Manuten��o, s�tio eletr�nico <a href="http://simec.mec.gov.br">http://simec.mec.gov.br</a>, 
							as novas escolas infantis.
						</td>
					</tr>
					<tr>
						<td>
							<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
							<tr>
								<td style="text-align: center;">
									<table border="1" style="border-bottom-style: solid; border-color: black;" align="center" width="100%" cellspacing="0" cellpadding="5">
										<tr>
											<th colspan="5">Situa��o das Solicita��es</th>
										</tr>
										<tr>
											<th>Cadastramento</th>
											<th>Dilig�ncia</th>
											<th>Pagamento efetuado</th>
											<th>Analisado</th>
											<th>Aguardando pagamento</th>
										</tr>
										<tr>
											<td style="text-align: center;"><?=($arrProinfantil['emcadastramento'] ? simec_number_format($arrProinfantil['emcadastramento'], 0, ',', '.') : '-')?></td>
											<td style="text-align: center;"><?=($arrProinfantil['emdiligencia'] ? simec_number_format($arrProinfantil['emdiligencia'], 0, ',', '.') : '-')?></td>
											<td style="text-align: center;"><?=($arrProinfantil['pagamentoefetuado'] ? simec_number_format($arrProinfantil['pagamentoefetuado'], 0, ',', '.') : '-')?></td>
											<td style="text-align: center;"><?=($arrProinfantil['emanalise'] ? simec_number_format($arrProinfantil['emanalise'], 0, ',', '.') : '-')?></td>
											<td style="text-align: center;"><?=($arrProinfantil['aguardandopagamento'] ? simec_number_format($arrProinfantil['aguardandopagamento'], 0, ',', '.') : '-')?></td>
										</tr>
									</table>
								</td>					
							</tr>
							</table>
						</td>
					</tr>
					<tr>
						<td align="justify" style="font-family: Calibri"><b>Observa��o</b></td>
					</tr>
					<tr>
						<td>
							<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
							<tr>
								<td style="text-align: center;">
									<table border="1" style="border-bottom-style: solid; border-color: black;" align="center" width="100%" cellspacing="0" cellpadding="5">
										<tr>
											<td align="justify">Sr./Sra. Prefeito(a): para as solicita��es que estejam "<b>Em cadastramento</b>" ou "<b>Em dilig�ncia</b>", favor acessar o m�dulo "E. I. Manuten��o" 
												do Simec e inserir as informa��es solicitadas. Para situa��es que envolvem <b>an�lise</b> � necess�rio aguardar an�lise t�cnica. 
												Caso ainda n�o possua cadastro no sistema, solicitar acesso pelo e-mail planodemetas@mec.gov.br ou pelos telefones (61) 2022-8335/8336/8337/8338
											</td>
											<?php $v = "Pelo m�dulo E.I. Manuten��o tamb�m � poss�vel acessar recursos espec�ficos a t�tulo de apoio � manuten��o dos novos estabelecimentos 
												p�blicos de educa��o infantil, que tenham sido constru�dos com recursos do Governo Federal (Proinf�ncia), estejam em plena atividade 
												e que possuam matr�culas ainda n�o contempladas com recursos do Fundo de Manuten��o e Desenvolvimento da Educa��o B�sica e de 
												Valoriza��o dos Profissionais da Educa��o � Fundeb."; ?>
										</tr>
									</table>
								</td>					
							</tr>
							</table>
						</td>
					</tr>
				</table>
			</td>
		</tr>
		</tbody>
		<tfoot>
			<tr>
				<td>
					<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5" style="border-bottom-style: solid; font-size: 12px; border-color: black;">
						<tr>
							<td align="right"><?=date("j/n/Y H:i:s"); ?></td>
						</tr>
					</table>
				</td>
			</tr>
		</tfoot>
	</table>
		
		
	<!-- Quebra Pagina -->	
	<div class="quebra"></div>
	
	<table border="0" align="left" width="100%" cellspacing="0" cellpadding="0" style="font-family: Calibri">
		<thead>
		<tr>
			<td>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
					<tr>
						<td><img src="imagem/cabecalho-7.jpg" width="710px" alt="" ></img></td>
					</tr>
					<tr style="color: white; background-color: #00008B; text-align: center; font-size: 17px">
						<td><?php echo 'Munic�pio: <b>'.$arrDadosMunicipio['nome'].' - '.$arrDadosMunicipio['uf'].'</b>'; ?></td>
					</tr>
				</table>
			</td>
		</tr>
		</thead>
		<tbody>
		<tr>
			<td>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
					<tr>
						<td colspan="2" align="justify" style="font-family: Calibri"><b>15) �ndice de Desenvolvimento da Educa��o B�sica - Ideb</b><br/><br/>
							O Ideb � um indicador que sintetiza dois conceitos igualmente importantes para a qualidade da educa��o: aprova��o e m�dia de desempenho dos 
							estudantes em l�ngua portuguesa e matem�tica. A s�rie hist�rica do Ideb se inicia em 2005, a partir de quando foram estabelecidas metas bienais de 
							qualidade a serem atingidas n�o apenas pelo pa�s, mas tamb�m por cada escola e por cada munic�pio. A l�gica � a de que cada munic�pio evolua de forma 
							a contribuir, em conjunto, para que o Brasil atinja o patamar educacional da m�dia dos pa�ses da OCDE. 
							Veja como est� a situa��o do Ideb em seu munic�pio: 
						</td>
					</tr>
					<tr>
						<td>
							<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
							<tr>
								<td style="text-align: center;">
									<table border="1" style="border-bottom-style: solid; border-color: black;" align="center" width="100%" cellspacing="0" cellpadding="5">
										<tr>
											<th>Ensino Fundamental<br/>(Anos iniciais)</th>
											<th>2005</th>
											<th>2007</th>
											<th>2009</th>
											<th>2011</th>
											<th>2013</th>
										</tr>
										<tr>
											<td style="text-align: left;">Meta projetada</td>
											<td style="text-align: center;"><?=$metaI2005 > 0 ? simec_number_format($metaI2005, 1, ",", "") : '-' ?></td>
											<td style="text-align: center;"><?=$metaI2007 > 0 ? simec_number_format($metaI2007, 1, ",", "") : '-' ?></td>
											<td style="text-align: center;"><?=$metaI2009 > 0 ? simec_number_format($metaI2009, 1, ",", "") : '-' ?></td>
											<td style="text-align: center;"><?=$metaI2011 > 0 ? simec_number_format($metaI2011, 1, ",", "") : '-' ?></td>
											<td style="text-align: center;"><?=$metaI2013 > 0 ? simec_number_format($metaI2013, 1, ",", "") : '-' ?></td>
										</tr>
										<tr>
											<td style="text-align: left;">Ideb verificado</td>
											<td style="text-align: center;"><?=$valorI2005 > 0 ? simec_number_format($valorI2005, 1, ",", "") : '-' ?></td>
											<td style="text-align: center;"><?=$valorI2007 > 0 ? simec_number_format($valorI2007, 1, ",", "") : '-' ?></td>
											<td style="text-align: center;"><?=$valorI2009 > 0 ? simec_number_format($valorI2009, 1, ",", "") : '-' ?></td>
											<td style="text-align: center;"><?=$valorI2011 > 0 ? simec_number_format($valorI2011, 1, ",", "") : '-' ?></td>
											<td style="text-align: center;"><?=$valorI2013 > 0 ? simec_number_format($valorI2013, 1, ",", "") : '-' ?></td>
										</tr>
									</table>
								</td>					
							</tr>
							</table>
						</td>
						<td>
							<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
							<tr>
								<td style="text-align: center;">
									<table border="1" style="border-bottom-style: solid; border-color: black;" align="center" width="100%" cellspacing="0" cellpadding="5">
										<tr>
											<th>Ensino Fundamental<br/>(Anos Finais)</th>
											<th>2005</th>
											<th>2007</th>
											<th>2009</th>
											<th>2011</th>
											<th>2013</th>
										</tr>
										<tr>
											<td style="text-align: left;">Meta projetada</td>
											<td style="text-align: center;"><?=$metaF2005 > 0 ? simec_number_format($metaF2005, 1, ",", "") : '-' ?></td>
											<td style="text-align: center;"><?=$metaF2007 > 0 ? simec_number_format($metaF2007, 1, ",", "") : '-' ?></td>
											<td style="text-align: center;"><?=$metaF2009 > 0 ? simec_number_format($metaF2009, 1, ",", "") : '-' ?></td>
											<td style="text-align: center;"><?=$metaF2011 > 0 ? simec_number_format($metaF2011, 1, ",", "") : '-' ?></td>
											<td style="text-align: center;"><?=$metaF2013 > 0 ? simec_number_format($metaF2013, 1, ",", "") : '-' ?></td>
										</tr>
										<tr>
											<td style="text-align: left;">Ideb verificado</td>
											<td style="text-align: center;"><?=$valorF2005 > 0 ? simec_number_format($valorF2005, 1, ",", "") : '-' ?></td>
											<td style="text-align: center;"><?=$valorF2007 > 0 ? simec_number_format($valorF2007, 1, ",", "") : '-' ?></td>
											<td style="text-align: center;"><?=$valorF2009 > 0 ? simec_number_format($valorF2009, 1, ",", "") : '-' ?></td>
											<td style="text-align: center;"><?=$valorF2011 > 0 ? simec_number_format($valorF2011, 1, ",", "") : '-' ?></td>
											<td style="text-align: center;"><?=$valorF2013 > 0 ? simec_number_format($valorF2013, 1, ",", "") : '-' ?></td>
										</tr>
									</table>
								</td>					
							</tr>
							</table>
						</td>
					</tr>
				</table>
			</td>
		</tr>
		</tbody>
		<tfoot>
		<tr>
			<td>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5" style="border-bottom-style: solid; font-size: 12px; border-color: black;">
					<tr>
						<td align="right"><?=date("j/n/Y H:i:s"); ?></td>
					</tr>
				</table>
			</td>
		</tr>
		</tfoot>
	</table>
		
	<!-- Quebra Pagina -->	
	<div class="quebra"></div>
	
	<table border="0" align="left" width="100%" cellspacing="0" cellpadding="0" style="font-family: Calibri">
		<thead>
		<tr>
			<td>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
					<tr>
						<td><img src="imagem/cabecalho-7.jpg" width="710px" alt="" ></img></td>
					</tr>
					<tr style="color: white; background-color: #00008B; text-align: center; font-size: 17px">
						<td><?php echo 'Munic�pio: <b>'.$arrDadosMunicipio['nome'].' - '.$arrDadosMunicipio['uf'].'</b>'; ?></td>
					</tr>
				</table>
			</td>
		</tr>
		</thead>
		<tbody>
		<tr>
			<td>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
					<tr>
						<td align="justify" style="font-family: Calibri"><b>16) Brasil Alfabetizado</b><br/><br/>
							O programa Brasil Alfabetizado repassa recursos de apoio aos estados, ao Distrito Federal e aos munic�pios parceiros para a alfabetiza��o de pessoas com 15 anos ou mais. S�o pagas bolsas aos alfabetizadores, coordenadores de turma e tradutores int�rpretes de Libras. A cada ciclo, os entes devem aderir ao programa e enviar o Plano Plurianual de Alfabetiza��o (PPAlfa), para an�lise e aprova��o do MEC. O valor de apoio � repassado em duas parcelas, sendo 60% ap�s aprova��o do PPAlfa e 40% ap�s o cadastramento das turmas. 
						</td>
					</tr>
					<tr>
						<td>
							<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
							<tr>
								<td style="text-align: center;">
									<table border="1" style="border-bottom-style: solid; border-color: black;" align="center" width="100%" cellspacing="0" cellpadding="5">
										<tr>
											<th>Situa��o do Munic�pio em rela��o ao Ciclo 2012</th>
											<th>Meta de atendimento (alfabetizandos)</th>
										</tr>
										<tr>
											<td style="text-align: center;"><?=$dadosSituacaoBA['situacao'] ? $dadosSituacaoBA['situacao'] : 'N�o aderiu' ?></td>
											<td style="text-align: center;"><?=$dadosSituacaoBA['quantidade'] ? simec_number_format($dadosSituacaoBA['quantidade'], 0, ',', '.') : '-' ?></td>
										</tr>
									</table>
								</td>					
							</tr>
							</table>
						</td>
					</tr>
					<tr>
                                            <td>
                                                <table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
                                                    <tr>
                                                        <td style="text-align: center;">
                                                            <table border="1" style="border-bottom-style: solid; border-color: black;" align="center" width="100%" cellspacing="0" cellpadding="5">
                                                                <tr>
                                                                    <th>Indicador</th>
                                                                    <?php 
                                                                        #Cria intervalo de exibicao de 8 anos
                                                                        $arrCicloBrasilAlfabetizado = range(date('Y')-7, date('Y'));
                                                                        foreach ($arrCicloBrasilAlfabetizado as $ciclo): ?>
                                                                            <th>Ciclo <?php echo $ciclo?></th>
                                                                    <?php endforeach;?>
                                                                </tr>
                                                                <?php 
                                                                foreach ($arrBrasilAlfabetizado as $chave => $arrValor): ?>
                                                                    <tr>
                                                                        <td style="text-align: left;"><?php echo $chave ?></td>
                                                                        <?php 
                                                                        foreach ($arrCicloBrasilAlfabetizado as $ciclo): 
                                                                            echo "<td style='text-align:center'>";
                                                                            if (is_array($arrValor)) {
                                                                                foreach ($arrValor as $valor) {
                                                                                    if($valor['dpedsc'] == $ciclo){
                                                                                        echo simec_number_format($valor['dshqtde'], 0, ',', '.');
                                                                                    }
                                                                                }
                                                                            } else {
                                                                                echo '-';
                                                                            }
                                                                            echo "</td>";
                                                                        endforeach;
                                                                        ?>
                                                                    </tr>
                                                                <?php 
                                                                endforeach;
                                                                ?>
                                                            </table>
                                                        </td>					
                                                    </tr>
                                                </table>
                                            </td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
					<tr>
						<td align="justify" style="font-family: Calibri"><b>17) Educa��o de Jovens e Adultos � Resolu��o FNDE N� 48/2012</b><br/><br/>
							Com o objetivo de ampliar a oferta de Educa��o de Jovens e Adultos (EJA), a Resolu��o n� 48/2012 regulamentou a transfer�ncia autom�tica de recursos aos Estados, ao Distrito Federal e aos munic�pios para a manuten��o de novas turmas de EJA, que ainda n�o tenham sido contempladas pelo Fundeb. O apoio financeiro � repassado em at� duas parcelas e tem como base de c�lculo o valor anual m�nimo por aluno definido nacionalmente para a EJA no exerc�cio, nos termos da Lei n� 11.494/2007, e � calculado a partir do m�s de in�cio do funcionamento da nova turma, independentemente do n�mero de dias de aulas nesse m�s de refer�ncia. 
						</td>
					</tr>
					<tr>
						<td>
							<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
							<tr>
								<td style="text-align: center;">
									<table border="1" style="border-bottom-style: solid; border-color: black;" align="center" width="100%" cellspacing="0" cellpadding="5">
										<tr>
											<th>Situa��o do Munic�pio</th>
											<th>N� de estudantes matriculados</th>
											<th>Se aderiu, valor da parcela j� repassada (50% do total)</th>
										</tr>
										<tr>
											<td style="text-align: center;"><b><?=$dadosJovensAdultos['situacao'] ? $dadosJovensAdultos['situacao'] : 'N�o aderiu' ?></b></td>
											<td style="text-align: center;"><?=$dadosJovensAdultos['quantidade'] ? simec_number_format($dadosJovensAdultos['quantidade'], 0, ',', '.') : '-' ?></td>
											<td style="text-align: center;">R$ <?=simec_number_format($dadosJovensAdultos['valor'], 2, ",", ".") ?></td>
										</tr>
									</table>
								</td>					
							</tr>
							</table>
						</td>
					</tr>
				</table>
			</td>
		</tr>
		</tbody>
		<tfoot>
		<tr>
			<td>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5" style="border-bottom-style: solid; font-size: 12px; border-color: black;">
					<tr>
						<td align="right"><?=date("j/n/Y H:i:s"); ?></td>
					</tr>
				</table>
			</td>
		</tr>
		</tfoot>
	</table>
		
		
		
	<!-- Quebra Pagina -->	
	<div class="quebra"></div>
	
	<table border="0" align="left" width="100%" cellspacing="0" cellpadding="0" style="font-family: Calibri">
		<thead>
		<tr>
			<td>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
					<tr>
						<td><img src="imagem/cabecalho-7.jpg" width="710px" alt="" ></img></td>
					</tr>
					<tr style="color: white; background-color: #00008B; text-align: center; font-size: 17px">
						<td><? echo 'Munic�pio: <b>'.$arrDadosMunicipio['nome'].' - '.$arrDadosMunicipio['uf'].'</b>'; ?></td>
					</tr>
				</table>
			</td>
		</tr>
		</thead>
		<tbody>
		<tr>
			<td>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
					<tr>
						<td align="justify" style="font-family: Calibri"><b>18) Sala de Recursos Multifuncionais</b><br/>
							O programa apoia os sistemas de ensino na implanta��o e atualiza��o de salas de recursos multifuncionais �s escolas de ensino regular, compostas por equipamentos, 
							mobili�rios, materiais pedag�gicos e de acessibilidade, para a realiza��o do atendimento educacional especializado, complementar ou suplementar � escolariza��o. 
							As escolas a serem contempladas s�o disponibilizadas pelo MEC com base no Educacenso. As secretarias de educa��o selecionam as escolas que receber�o as Salas de 
							Recursos Multifuncionais por meio do Sistema de Gest�o Tecnol�gica do Minist�rio da Educa��o - SIGETEC.  
Conforme a Portaria n�25/2012, do Minist�rio da Educa��o, �s secretarias de educa��o cabem as seguintes contrapartidas:<br><br>
I- subordinar-se �s diretrizes do Programa;<br>
II- responsabilizar-se pela preserva��o do espa�o f�sico para a instala��o dos bens doados;<br>
III- disponibilizar professor para atuar na organiza��o e oferta do atendimento educacional especializado - AEE;<br>
IV- responsabilizar-se pela manuten��o dos equipamentos doados;<br>
V- orientar a escola destinat�ria para instituir no seu Projeto Pol�tico Pedag�gico, a organiza��o e oferta do Atendimento Educacional Especializado complementar ou suplementar � escolariza��o de estudantes p�blico alvo da educa��o especial, matriculados nas classes comuns do ensino regular, na Educa��o de Jovens e Adultos e na Educa��o Profissional;<br>
VI- promover a forma��o continuada aos professores que atuam no AEE;<br>
VII- zelar pela seguran�a e integridade dos equipamentos, inclusive acionar as respectivas "garantias de funcionamento" oferecido pelo fornecedor; e<br>
VIII- restituir os bens doados em perfeitas condi��es de conserva��o e funcionamento em caso de revers�o da doa��o.
							<br>
													 
						</td>
					</tr>
					<tr>
						<td>
							<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
							<tr>
								<td style="text-align: center;">
									<table border="1" style="border-bottom-style: solid; border-color: black;" align="center" width="100%" cellspacing="0" cellpadding="5">
										<tr>
											<th>Ano</th>
											<th>N� Escolas</th>
											<th>N� de salas/kits</th>
										</tr><?php
										if( $dadosRecMult[0] ){
											foreach( $dadosRecMult as $recMult ){
												?>
													<tr>
														<td style="text-align: center;"><?=$recMult['ano'] ?></td>
														<td style="text-align: center;"><?=$recMult['totalescolas'] ? simec_number_format($recMult['totalescolas'],0, ',', '.') : '-' ?></td>
														<td style="text-align: center;"><?=$recMult['quantidade'] ? simec_number_format($recMult['quantidade'], 0, ',', '.') : '-' ?></td>
													</tr>
												<?php
											} 
										} else {
												?>											
												<tr>
													<td style="text-align: center;">-</td>
													<td style="text-align: center;">-</td>
													<td style="text-align: center;">-</td>
												</tr>
										<?php
										}
										?>

									</table>
								</td>					
							</tr>
							</table>
						</td>
					</tr>
				</table>
			</td>
		</tr>
		</tbody>
		<tfoot>
		<tr>
			<td>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5" style="border-bottom-style: solid; font-size: 12px; border-color: black;">
					<tr>
						<td align="right"><?=date("j/n/Y H:i:s"); ?></td>
					</tr>
				</table>
			</td>
		</tr>
		</tfoot>
	</table>
	
	<?php 
	if(is_array($dadosListaObras)) {
		$m=0;
		$y=0;
		foreach($dadosListaObras as $o) {
			$llista[$m][] = $o;
			
			if($y<5) {
				$y++;	
			} else {
				$y=0;
				$m++;	
			}
		}
	} 
	
	if($llista) {
		foreach($llista as $dadosListaObras) {
	?>
	<div class="quebra"></div>
	<table border="0" align="left" width="100%" cellspacing="0" cellpadding="0" style="font-family: Calibri">
		<tbody>
			<tr>
				<td>
					<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
						<tr>
							<td align="justify" style="font-family: Calibri"><b>ANEXO: Lista de Obras</b></td>
						</tr>
						<tr>
							<td>
								<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
								<tr>
									<td style="text-align: center;">
										<table border="1" style="border-bottom-style: solid; border-color: black; font-size: 10px" align="center" width="100%" cellspacing="0" cellpadding="5">
											<tr>
												<th>Nome</th>
												<th>Tipologia</th>
												<th>Situa��o</th>
												<th>Valor Aprovado</th>
												<th>Valor Empenhado</th>
												<th>Valor Pago</th>
												<th>Percentual de Execu��o</th>
												<th>Dias sem atualiza��o</th>
											</tr>
											<?php
												foreach($dadosListaObras as $listaObra){
													?>
														<tr>
															<td style="text-align: left;"><?=$listaObra['nome'] ?></td>
															<td style="text-align: center;"><?=$listaObra['tipologia'] ? $listaObra['tipologia'] : '-' ?></td>
															<td style="text-align: center;"><?=$listaObra['situacao'] ? $listaObra['situacao'] : '-' ?></td>
															<td style="text-align: center;">R$ <?=simec_number_format($listaObra['valor_aprovado'], 2, ",", ".") ?></td>
															<td style="text-align: center;">R$ <?=simec_number_format($listaObra['valor_empenhado'], 2, ",", ".") ?></td>
															<td style="text-align: center;">R$ <?=simec_number_format($listaObra['valor_pago'], 2, ",", ".") ?></td>
															<?php if( $listaObra['situacao'] <> 'Conclu�da' && $listaObra['situacao'] <> 'Obra Cancelada' ){ ?>
																<td style="text-align: center;"><?=$listaObra['obrpercexec'] > 0 ? simec_number_format($listaObra['obrpercexec'], 2, ",", "") : '-' ?></td>
															<?php } else { ?>
																<td style="text-align: center;">-</td>
															<?php } ?>
															<?php if( $listaObra['situacao'] <> 'Conclu�da' && $listaObra['situacao'] <> 'Obra Cancelada' ){ ?>
																<td style="text-align: center;"><?=$listaObra['diassematualizacao'] > 60 ? '<font color="red">'.$listaObra['diassematualizacao'].' dia(s)</font>' : $listaObra['diassematualizacao'].' dias(s)' ?></td>
															<?php } else { ?>
																<td style="text-align: center;">-</td>
															<?php } ?>
														</tr>
													<?php
												}
												?>
										</table>
									</td>
								</tr>
								</table>
							</td>
						</tr>
					</table>
				</td>
			</tr>
		</tbody>
	</table>
		<?php } ?>
	<?php } ?>
</body>