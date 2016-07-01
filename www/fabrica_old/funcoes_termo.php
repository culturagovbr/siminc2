<?php
include APPRAIZ . 'includes/workflow.php';
function TermoCabecalho($titulo = '', $ss = '', $os = '', $sidid = ''){
	global $db;
	
	//pega sistema
	if($sidid){
		$sql = "select sidabrev, siddescricao, sidfinalidade 
				from demandas.sistemadetalhe 
				where sidid = $sidid";
		$sistema = $db->pegaLinha($sql);
	}	
  
	if($ss) $trss = '<tr><td style="border-bottom: 1px solid;"><b>N� S.S.: '.$ss.'</b></td></tr>';
	
	if($os) $tros = '<tr><td style="border-bottom: 1px solid;"><b>N� O.S.: '.$os.'</b></td></tr>';
	
	return '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
			<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
    		<head>
      			<meta http-equiv="content-type" content="text/html; charset=iso-8859-1" />
      			<title></title>
				<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
				<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
				<style>
					 @media print {
					 	.notprint { display: none }
					 }
					 .conteudo{
	                	margin: 0 auto;
	                	width: 95%;
	                 }

	                 thead { display: table-header-group; } 
 				     tfoot { display: table-footer-group; }
				</style>
			</head>
			<body>
			<table width="100%">
				<thead>
					<tr>
						<td align="center" width="100%">
							<table cellspacing="0" cellpadding="2" border="0" width="95%"  class="notscreen1 debug">
									<tr bgcolor="#ffffff" >
										<td width="20%" align="center" style="border-top: 1px solid; border-left: 1px solid; " >
											<br>
											<img  border="0"  src="../imagens/logo/logo_mec.jpg">
											<br>
											<br>
										</td>
										<td style="border-top: 1px solid; border-left: 1px solid; " nowrap align="center" >	
											<b><font style="font-size: 14px;">'.$titulo.'</font></b>
										</td>
										<td width="20%" style="border-top: 1px solid; border-left: 1px solid; border-right: 1px solid;" >	
											<table border=0 width="100%" >
												'.$trss.'
												'.$tros.'
												<tr>
													<td ><b>Data Emiss�o:</b> '.date('d/m/Y H:i').'</td>
												</tr>
											</table>
										</td>
									</tr>
									<tr bgcolor="#ffffff">
										<td valign="top" align="left" style="border-top: 1px solid; border-left: 1px solid; border-bottom: 1px solid; "  >
											<b>Sistema:</b><br>'.$sistema['siddescricao'].'
										</td>
										<td valign="top" align="left" style="border-top: 1px solid; border-left: 1px solid; border-bottom: 1px solid; ">	
											<b>Descri��o:</b><br>'.$sistema['sidfinalidade'].'
										</td>
										<td valign="top" align="left" style="border-top: 1px solid; border-bottom: 1px solid; border-left: 1px solid; border-right: 1px solid;">	
											<b>M�dulo:</b><br>'.$sistema['sidabrev'].'
										</td>
									</tr>									
							</table>
							<br>
							<br>
						</td>
					</tr>
				</thead>
			';
	
}

function ComunicacaoOcorrencias($os){
	
	$html = TermoCabecalho('Comunica��o de Ocorr�ncias');

	$html .= '<div class="conteudo">
				Senhor Gestor do Contrato,
				<br>
				<br>
				Assunto: Contrato n�
				<br>
				<br>
				Informo a Vossa Senhoria, paras as provid�ncias cab�veis, que na execu��o do contrato acima 
				referenciado observaram-se as seguintes ocorr�ncias:
				
				<br>
				<br>
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type="checkbox">&nbsp;Atraso injustificado no fornecimento ou presta��o do servi�o.
				
				<br>
				<br>
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type="checkbox">&nbsp;Data Solicita��o:    /     /   			Data Entrega: 	/     /  
				
				<br>
				<br>
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type="checkbox">&nbsp;N�o fornecimento do material ou inexecu��o do servi�o solicitado.
				
				<br>
				<br>
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type="checkbox">&nbsp;Prorrogar contrato.
				
				<br>
				<br>
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type="checkbox">&nbsp;Proceder novo certame licitat�rio.
				
				<br>
				<br>
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type="checkbox">&nbsp;Aditar o contrato: acrescer at� 25%&nbsp;<input type="checkbox">&nbsp;reduzir valor&nbsp;<input type="checkbox">
				
				<br>
				<br>
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type="checkbox">&nbsp;Outras (especificar):
				
				<br>
				<br>
				_____________________________________________________________________
				<br>
				_____________________________________________________________________
				<br>
				_____________________________________________________________________
				
				<br>
				<br>
				<br>
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Atenciosamente,
				<br>
				<br>
				
				<center>_________________________________________________</center>
				<center>FISCAL DO CONTRATO</center>
				<center>(assinatura e carimbo)</center>
				
			</div><h2>&nbsp;</h2>
			'.TermoCabecalho().'
			<div class="conteudo">
				Senhor Coordenador-Geral de Recursos Log�sticos,
				<br>
				<br>
				Com base nas ocorr�ncias informadas, sugiro a aplica��o dos seguintes procedimentos administrativos:
				
				<br>
				<br>
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type="checkbox">&nbsp;Dar ci�ncia � SPOA.
				
				<br>
				<br>
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type="checkbox">&nbsp;Multa de mora no percentual estabelecido no contrato.
				
				<br>
				<br>
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type="checkbox">&nbsp;Multa compensat�ria no percentual estabelecido no contrato.
				
				<br>
				<br>
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type="checkbox">&nbsp;Advert�ncia.
				
				<br>
				<br>
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type="checkbox">&nbsp;Suspens�o tempor�ria de participa��o em licita��o e impedimento de contratar com a Administra��o, pelo prazo de. < Limitado a 2 anos>
				
				<br>
				<br>
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type="checkbox">&nbsp;Declara��o de inidoneidade para licitar ou contratar com a Administra��o P�blica, na forma do inciso IV do art .87 da Lei n� 8666/1993.
				
				<br>
				<br>
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type="checkbox">&nbsp;Rescis�o contratual fundamentada nos motivos elencados nos incisos I a XVII do art. 78 da Lei n� 8666/1993.
				
				<br>
				<br>
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type="checkbox">&nbsp;Prorrogar contrato.
				
				<br>
				<br>
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type="checkbox">&nbsp;0  Proceder novo certame licitat�rio.
				
				<br>
				<br>
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type="checkbox">&nbsp;Aditar o contrato:  acrescer at� 25%&nbsp;<input type="checkbox">&nbsp;reduzir valor&nbsp;<input type="checkbox">
				
				<br>
				<br>
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type="checkbox">Outras (especificar).
				
				<br>
				<br>
				_____________________________________________________________________
				<br>
				_____________________________________________________________________
				<br>
				_____________________________________________________________________
				
				<br>
				<br>
				<br>
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Atenciosamente,
				<br>
				<br>
				
				<center>_________________________________________________</center>
				<center>FISCAL DO CONTRATO</center>
				<center>(assinatura e carimbo)</center>
			  </div>';
	
	return $html.'</body></html>';
	
}

function TermoAberturaOrdemServico($os){
	global $db;
	
	$data = new Data();
	
	$sql = "SELECT
				ansgarantia as garantia,
				fas.ansid,
				su.usunome,
				su.usuemail,
				su.usufoneddd,
				su.usufonenum,
				ss.scsid,
				fas.tpsid,
				to_char(ss.dataabertura, 'DD/MM/YYYY') as dataabertura,
				to_char(odsdtprevinicio, 'DD/MM/YYYY') as datainicio,
				to_char(odsdtprevinicio, 'YYYY') as ano,
				to_char(odsdtprevtermino, 'DD/MM/YYYY') as datatermino,
				os.odsqtdpfestimada,
				ss.odsidorigem,
				os.odsdetalhamento,
				dsd.sidid,
				dsd.sidabrev, 
				dsd.siddescricao,
				un.unidsc,
				os.tosid,
				os.odsidpai,
				CASE WHEN os.tosid = 1 THEN
						(select ctrnumero from fabrica.contrato fc
						INNER JOIN entidade.entidade ee ON ee.entid = fc.entidcontratado 
						inner join fabrica.analisesolicitacao a on a.ctrid = fc.ctrid
						where ctrstatus='A' and ctrcontagem = false and a.scsid = fas.scsid limit 1)
				     ELSE
						(select ctrnumero from fabrica.contrato fc
						INNER JOIN entidade.entidade ee ON ee.entid = fc.entidcontratado 
						inner join fabrica.analisesolicitacao a on a.ctrid = fc.ctrid
						where ctrstatus='A' and ctrcontagem = true and a.scsid = fas.scsid limit 1)
				END as contrato,
				CASE WHEN os.tosid = 1 THEN
						(select entnome from fabrica.contrato fc
						INNER JOIN entidade.entidade ee ON ee.entid = fc.entidcontratado 
						inner join fabrica.analisesolicitacao a on a.ctrid = fc.ctrid
						where ctrstatus='A' and ctrcontagem = false and a.scsid = fas.scsid limit 1)
				     ELSE
						(select entnome from fabrica.contrato fc
						INNER JOIN entidade.entidade ee ON ee.entid = fc.entidcontratado 
						inner join fabrica.analisesolicitacao a on a.ctrid = fc.ctrid
						where ctrstatus='A' and ctrcontagem = true and a.scsid = fas.scsid limit 1)
				END as entnome
			FROM 
				fabrica.ordemservico  os
			LEFT JOIN
				fabrica.profissionalos po ON po.odsid = os.odsid
			INNER JOIN
				fabrica.solicitacaoservico ss ON ss.scsid = os.scsid
			LEFT JOIN
				seguranca.usuario su ON su.usucpf = ss.usucpfrequisitante
			INNER JOIN
				fabrica.analisesolicitacao fas ON fas.scsid = os.scsid
			LEFT JOIN 
				public.unidade un ON un.unicod = su.unicod
			LEFT JOIN
				demandas.sistemadetalhe dsd ON dsd.sidid = ss.sidid AND dsd.sidstatus = 'A'
			WHERE 
				os.odsid = {$os};";
                
	$solicitacao = $db->pegaLinha($sql);
	

	$html = TermoCabecalho('Termo de Abertura de Ordem de Servi�o', $solicitacao['scsid'], $os.' / '.$solicitacao['ano'], $solicitacao['sidid']);
	
	$html .= '<tbody><tr><td width=100% valign="top">
				<table cellSpacing="1" cellPadding=3 align="center" width="100%" border="1">
					<tr>
						<td>Data de abertura: '.$solicitacao['dataabertura'].'<td>
						<td>N� '.$os.' / '.date('Y').'<td>
					</tr>
				</table>
				
				<br>
				
				<table class="tabela" cellSpacing="1" cellPadding=3 align="center" width="100%" border="1">
					<tr>
						<td colspan="6">Dado(s) do Requisitante</td>
					</tr>
					<tr>
						<td colspan="2">Nome</td>
						<td colspan="2">'.$solicitacao['usunome'].'</td>
						<td colspan="1">Setor</td>
						<td colspan="1">'.$solicitacao['unidsc'].'</td>
					</tr>
					<tr>
						<td colspan="2">Telefone(s)</td>
						<td colspan="4">('.$solicitacao['usufoneddd'].') '.$solicitacao['usufonenum'].'</td>
					</tr>
					<tr>
						<td colspan="2">E-mail</td>
						<td colspan="4">'.$solicitacao['usuemail'].'</td>
					</tr>
					<tr>
						<td colspan="2">Contrato</td>
						<td colspan="2">'.$solicitacao['contrato'].'</td>
						<td colspan="1">Empresa Contratada</td>
						<td colspan="1">'.$solicitacao['entnome'].'</td>
					</tr>
					<tr>
						<td colspan="2">Sigla do Sistema</td>
						<td colspan="2">'.(($solicitacao['sidabrev'])?$solicitacao['sidabrev']:'&nbsp;').'</td>
						<td colspan="1">Nome do Sistema</td>
						<td colspan="1">'.(($solicitacao['siddescricao'])?$solicitacao['siddescricao']:'&nbsp;').'</td>
					</tr>';
	
	
	
	//verifica se � contrada ou contratante
	if($solicitacao['tosid'] == '2' || $solicitacao['tosid'] == '3'){
			
			//tiposervico
			$estimado = '&nbsp;&nbsp;';
			$detalhado = '&nbsp;&nbsp;';
			if($solicitacao['tosid'] == '2') $estimado = 'X';
			if($solicitacao['tosid'] == '3') $detalhado = 'X'; 
			
			$htmlTipoServico = '<tr>
									<td colspan="2">Tipo de Servi�o</td>
									<td colspan="4">( '.$estimado.' ) Contagem APF Estimada <br>
	 												( '.$detalhado.' ) Contagem APF Detalhada
	 								</td>
								</tr>';
			
			// disciplinas
			$htmlDisciplinas = '';
	}
	else{
			//tiposervico
			$sql = "SELECT 
						tpsid, 
						tpsdsc 
					FROM 
						fabrica.tiposervico 
					WHERE 
						tpsstatus='A'
						OR tpsid={$solicitacao['tpsid']}";
			$tiposervico = $db->carregar($sql);
			
			if($tiposervico[0]) {
				$htmlTipoServico = '<tr>
								<td colspan="2">Tipo de Servi�o</td>
								<td colspan="4">';
				foreach($tiposervico as $tps) {
					$htmlTipoServico .= '( '.(($tps['tpsid']==$solicitacao['tpsid'])?'X':'&nbsp;&nbsp;').' ) '.$tps['tpsdsc'].'<br/>';
				}
				
				$htmlTipoServico .= '</td>
							</tr>';
			}
			
			
			

			
			// disciplinas
			$sql = "SELECT
						dspid,
						dspdsc
					FROM
						fabrica.disciplina
					WHERE
						dspstatus = 'A'";
			
			$disciplinas = $db->carregar($sql);
			
			// $fases = carregar fases
			$sql = "SELECT
						fasid,
						fasdsc
					FROM 
						fabrica.fase 
					WHERE 
						fasstatus = 'A'";
			
			$fases = $db->carregar($sql);						
			
			$qtd_colunas = (int)count($fases) + 1;
			
			$htmlDisciplinas = '<tr>
								<td colspan="2">Disciplinas contratadas</td>
								<td colspan="5">';
			
			$htmlDisciplinas .= '<table border="1" width="100%">
							<tr>
								<td colspan="'.$qtd_colunas.'" align="center"><b>Fase</b></td>
							</tr>
							<tr>
								<td align="center"><b>Disciplina</b></td>';
			
			// cabe�alho
			foreach ($fases as $fase) {
				$htmlDisciplinas .= '	<td align="center"><b>'.$fase['fasdsc'].'</b></td>';
			}
			
			$htmlDisciplinas .= '	</tr>';
			
			// conte�do
			foreach ($disciplinas as $disciplina) {
				$htmlDisciplinas .= '<tr>
								<td>'.$disciplina['dspdsc'].'</td>';
				
				foreach ($fases as $fase) {
					if($solicitacao['ansid']){
						// query que verifica quais fases tem produtos prontos
						$sql = "SELECT count(sp.fdpid) as total 
								FROM fabrica.servicofaseproduto sp 
								INNER JOIN fabrica.fasedisciplinaproduto fdp ON fdp.fdpid = sp.fdpid 
								INNER JOIN fabrica.fasedisciplina fd ON fd.fsdid = fdp.fsdid 
								INNER JOIN fabrica.produto p ON p.prdid = fdp.prdid 
								WHERE sp.ansid = {$solicitacao['ansid']} 
								and fd.fasid = {$fase['fasid']} 
								and fd.dspid = {$disciplina['dspid']}
								and sp.tpeid = 1";
						
						$valor = $db->pegaLinha($sql);
					}// if tempor�rio. A pedidos do Henrique, foi colocado esse if aqui para que n�o ocorram erros caso os campos do banco estejam em branco
					
					if( $valor['total'] ){
						$htmlDisciplinas .= '<td align="center">( X )</td>';
					}else{
						$htmlDisciplinas .= '<td align="center">(&nbsp;&nbsp;&nbsp;&nbsp;)</td>';
					}// fim do if
					
				}// fim do foreach das fases
				
				$htmlDisciplinas .= '</tr>';
				
			}// fim do foreach das disciplinas
			
			$htmlDisciplinas .= '		</table>
						 	</td>
						</tr>'; 
	
	
	}

	$html .=	    $htmlTipoServico.
					$htmlDisciplinas.
					'<tr>
						<td>In�cio</td>
						<td>'.$solicitacao['datainicio'].'</td>
						<td>T�rmino</td>
						<td colspan="3">'.$solicitacao['datatermino'].'</td>
					</tr>
					<tr>
						<td colspan="2">Volume de Pontos de Fun��o</td>
						<td colspan="4">'.$solicitacao['odsqtdpfestimada'].'</td>
					</tr>
					<tr>
						<td colspan="2">Servi�o em garantia?</td>
						<td>
							'.( $solicitacao['garantia'] == 'f' ? 'Sim (&nbsp;&nbsp;) N�o ( X )' : 'Sim ( X ) N�o (&nbsp;&nbsp;)' ).' 
						</td>
						<td colspan="2">Ordem de Servi�o Associada</td>
						<td>'.$solicitacao['odsidorigem'].'</td>
					</tr>
				</table>
				<br>
				<br>
				1.	Descri��o do Servi�o
				<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;';
					
					$html .= $solicitacao['odsdetalhamento'];

					
	//artefatos entregues
	if($solicitacao['odsidpai']){
		$ansOSPai = $db->pegaUm("select ansid 
								 from fabrica.analisesolicitacao a
								 LEFT JOIN fabrica.ordemservico o ON o.scsid = a.scsid
								 where odsid = ".$solicitacao['odsidpai']);
	}	
	if($ansOSPai){
		$htmlArtefatosEntregres = '<br>
					<br>
					<br>
					2.	Artefatos Entregres <br>';
		
		
		//pega tipo
		if($solicitacao['tosid'] == '1') $tpeid = 1;
		else $tpeid = 2;
		
		if($tpeid) $where = "WHERE tpeid = $tpeid";
		$sql = "SELECT tpeid, tpedsc FROM fabrica.tipoexecucao $where ORDER BY 1";
		$tipo = $db->carregar($sql);
		
		if($tipo){
			
			$idtable = true;
			
			if($idtable) $htmlArtefatosEntregres .= '<table class=tabela bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 width="100%" >';
			
			for($t=0;$t<=count($tipo)-1;$t++){
				
				$tpeid = $tipo[$t]['tpeid'];
				$tpedsc = $tipo[$t]['tpedsc'];
		
				//pega disciplinas
				$sql = "SELECT distinct d.dspid, d.dspdsc
						FROM fabrica.servicofaseproduto sp 
						INNER JOIN fabrica.fasedisciplinaproduto fdp ON fdp.fdpid = sp.fdpid
						INNER JOIN fabrica.fasedisciplina fd ON fd.fsdid = fdp.fsdid
						INNER JOIN fabrica.disciplina d ON d.dspid = fd.dspid
						WHERE sp.ansid = {$ansOSPai} 
						AND sp.tpeid = {$tpeid}
						order by 1";
				$disciplina = $db->carregar($sql);
				
				$txtTd = '';
				
				if($disciplina){
					
					for($j=0;$j<=count($disciplina)-1;$j++){
						
						$dspid = $disciplina[$j]['dspid'];
						
						$txtTd .= '<span style="padding-left:40px"><b>'.trim($disciplina[$j]['dspdsc']).'</b></span><br>';
					
						//pega fases
						$sql = "SELECT distinct f.fasid, f.fasdsc 
								FROM fabrica.servicofaseproduto sp 
								INNER JOIN fabrica.fasedisciplinaproduto fdp ON fdp.fdpid = sp.fdpid
								INNER JOIN fabrica.fasedisciplina fd ON fd.fsdid = fdp.fsdid
								INNER JOIN fabrica.fase f ON f.fasid = fd.fasid
								WHERE sp.ansid = {$ansOSPai}
								AND sp.tpeid = {$tpeid}
								AND fd.dspid = {$dspid}
								ORDER BY 1";
						$fase = $db->carregar($sql);
					
						if($fase) {
							
							for($i=0;$i<=count($fase)-1;$i++){
								
								$fasid = $fase[$i]['fasid'];
								
								$sql = "SELECT p.prddsc 
										FROM fabrica.servicofaseproduto sp 
										INNER JOIN fabrica.fasedisciplinaproduto fdp ON fdp.fdpid = sp.fdpid
										INNER JOIN fabrica.fasedisciplina fd ON fd.fsdid = fdp.fsdid
										INNER JOIN fabrica.produto p ON p.prdid = fdp.prdid
										WHERE sp.ansid = {$ansOSPai}
										and sp.tpeid = {$tpeid} 
										and fd.dspid = {$dspid}
										and fd.fasid = {$fasid}
										ORDER BY 1";
								$produto = $db->carregarColuna($sql);
						
								if($produto){
									$txtTd .= '<span style="padding-left:60px"><b> - '.$fase[$i]['fasdsc'].'</b></span><br> <div style="padding-left:80px"> - ' . implode(";<br> - ", $produto) . ';</div>';
								}
								
							}
							
						}
								
					}//fecha for disciplina		
					
				}
				else{
						$txtTd = "N/A";
				}
				
				
				$htmlArtefatosEntregres .= '
						<tr>
							<td width="100%">
							    <br>'.$txtTd.'<br>
							</td>
						</tr>
				';
			
			}
			
			if($idtable) $htmlArtefatosEntregres .= '</table>';
					
		}
		
	}// fim artefatos entregues
					
					
	//artefatos gerados
	$htmlArtefatosGerados = '<br>
				<br>
				<br>
				3.	Artefatos a serem gerados <br>';
	
	
	//pega tipo
	if($solicitacao['tosid'] == '1') $tpeid = 1;
	else $tpeid = 2;
	
	if($tpeid) $where = "WHERE tpeid = $tpeid";
	$sql = "SELECT tpeid, tpedsc FROM fabrica.tipoexecucao $where ORDER BY 1";
	$tipo = $db->carregar($sql);
	
	if($tipo){
		
		$idtable = true;
		
		if($idtable) $htmlArtefatosGerados .= '<table class=tabela bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 width="100%" >';
		
		for($t=0;$t<=count($tipo)-1;$t++){
			
			$tpeid = $tipo[$t]['tpeid'];
			$tpedsc = $tipo[$t]['tpedsc'];
	
			//pega disciplinas
			$sql = "SELECT distinct d.dspid, d.dspdsc
					FROM fabrica.servicofaseproduto sp 
					INNER JOIN fabrica.fasedisciplinaproduto fdp ON fdp.fdpid = sp.fdpid
					INNER JOIN fabrica.fasedisciplina fd ON fd.fsdid = fdp.fsdid
					INNER JOIN fabrica.disciplina d ON d.dspid = fd.dspid
					WHERE sp.ansid = {$solicitacao['ansid']} 
					AND sp.tpeid = {$tpeid}
					order by 1";
			$disciplina = $db->carregar($sql);
			
			$txtTd = '';
			
			if($disciplina){
				
				for($j=0;$j<=count($disciplina)-1;$j++){
					
					$dspid = $disciplina[$j]['dspid'];
					
					$txtTd .= '<span style="padding-left:40px"><b>'.trim($disciplina[$j]['dspdsc']).'</b></span><br>';
				
					//pega fases
					$sql = "SELECT distinct f.fasid, f.fasdsc 
							FROM fabrica.servicofaseproduto sp 
							INNER JOIN fabrica.fasedisciplinaproduto fdp ON fdp.fdpid = sp.fdpid
							INNER JOIN fabrica.fasedisciplina fd ON fd.fsdid = fdp.fsdid
							INNER JOIN fabrica.fase f ON f.fasid = fd.fasid
							WHERE sp.ansid = {$solicitacao['ansid']}
							AND sp.tpeid = {$tpeid}
							AND fd.dspid = {$dspid}
							ORDER BY 1";
					$fase = $db->carregar($sql);
				
					if($fase) {
						
						for($i=0;$i<=count($fase)-1;$i++){
							
							$fasid = $fase[$i]['fasid'];
							
							$sql = "SELECT p.prddsc 
									FROM fabrica.servicofaseproduto sp 
									INNER JOIN fabrica.fasedisciplinaproduto fdp ON fdp.fdpid = sp.fdpid
									INNER JOIN fabrica.fasedisciplina fd ON fd.fsdid = fdp.fsdid
									INNER JOIN fabrica.produto p ON p.prdid = fdp.prdid
									WHERE sp.ansid = {$solicitacao['ansid']}
									and sp.tpeid = {$tpeid} 
									and fd.dspid = {$dspid}
									and fd.fasid = {$fasid}
									ORDER BY 1";
							$produto = $db->carregarColuna($sql);
					
							if($produto){
								$txtTd .= '<span style="padding-left:60px"><b> - '.$fase[$i]['fasdsc'].'</b></span><br> <div style="padding-left:80px"> - ' . implode(";<br> - ", $produto) . ';</div>';
							}
							
						}
						
					}
							
				}//fecha for disciplina		
				
			}
			else{
					$txtTd = "N/A";
			}
			
			
			$htmlArtefatosGerados .= '
					<tr>
						<td width="100%">
						    <br>'.$txtTd.'<br>
						</td>
					</tr>
			';
		
		}
		
		if($idtable) $htmlArtefatosGerados .= '</table>';
				
	}// fim artefatos gerados
	
					
	$html .=  $htmlArtefatosEntregres.
			  $htmlArtefatosGerados.
			  ' <br>
				<br>
				<br>
				4.	Profissionais Respons�veis';
	
				$sql = "SELECT distinct 
							u.usunome || ' - ' || p.pfldsc as descricao 
						FROM 
							seguranca.usuario u
						INNER JOIN 
							seguranca.perfilusuario o ON u.usucpf=o.usucpf 
						INNER JOIN 
							seguranca.perfil p ON p.pflcod=o.pflcod 
						INNER JOIN 
							demandas.usuarioresponsabilidade ur ON p.pflcod=ur.pflcod AND u.usucpf=ur.usucpf
						INNER JOIN 
							fabrica.profissionalos pr ON u.usucpf=pr.usucpf
						WHERE 
							pr.odsid={$os}";
				
				$profissionais = $db->carregarColuna($sql);
				
				foreach ($profissionais as $profissional) {
					$html .= '	<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$profissional;
				}
				
				if($solicitacao['tosid'] == 1){ //requisitante
					$sql = "select 
								u.usunome,
								n.nu_matricula_siape
							from 
								fabrica.solicitacaoservico f 
							inner join 
								seguranca.usuario u ON u.usucpf = f.usucpfrequisitante
							inner join 
								siape.tb_siape_cadastro_servidor_ativos n ON n.nu_cpf = u.usucpf
							where  
								scsid = {$solicitacao['scsid']}";				
				}
				else{ //fiscal
					$sql = "select
							      u.usunome,
							      n.nu_matricula_siape
							from
							      fabrica.fiscalsolicitacao fs
							inner join
							      seguranca.usuario u ON u.usucpf = fs.usucpf
							inner join
							      seguranca.perfilusuario p ON p.usucpf = fs.usucpf and p.pflcod = ".PERFIL_FISCAL_CONTRATO."
							inner join
							      siape.tb_siape_cadastro_servidor_ativos n ON n.nu_cpf = fs.usucpf
							where  
							      fs.scsid = {$solicitacao['scsid']}";
					
				}				
				$requisitante = $db->pegaLinha($sql); 

				$sql = "select 
							u.usunome,
							n.nu_cpf,
							n.nu_matricula_siape
						from 
							seguranca.perfilusuario s
						inner join 
							seguranca.usuario u ON u.usucpf = s.usucpf
						inner join 
							siape.tb_siape_cadastro_servidor_ativos n ON n.nu_cpf = u.usucpf
						where  
							s.pflcod = ".PERFIL_GESTOR_CONTRATO."";
				
				$gestor = $db->pegaLinha($sql);
				
				$sql = "select 
							u.usunome,
							s.usucpf
						from 
							seguranca.perfilusuario s
						left join 
							seguranca.usuario u ON u.usucpf = s.usucpf
						where  
							s.pflcod = ".($solicitacao['tosid'] == 1 ? PERFIL_PREPOSTO : PERFIL_CONTAGEM_PF)."";
				
				$preposto = $db->pegaLinha($sql);
				
				
				
	$html .= '	<br>
				<br>
				<center>

					<table border="0">
						<tr>
							<td colspan="3">
								<center>De Acordo</center>
								<br><br>
							</td>
						</tr>
						<tr>
							<td>
								<center>Contratante Requisitante</center>
							</td>
							<td width="80">&nbsp;</td>
							<td>
								<center>Contratante Gestor do Contrato</center>
							</td>
						</tr>
						<tr>
							<td><br><center>___________________________________<br></center>
								Nome: '.$requisitante['usunome'].'
								<br>Mat: '.$requisitante['nu_matricula_siape'].'
							</td>
							<td width="80">&nbsp;</td>
							<td><br><center>___________________________________<br></center>
								Nome: '.$gestor['usunome'].'
								<br>Mat: '.$gestor['nu_matricula_siape'].'
							</td>
						</tr>
						<tr><td colpan="3"><br>&nbsp;<br></td></tr>
						<tr>
							<td>&nbsp;</td>
							<td width="80"><br>
								<center>Contratada Preposto</center></td>
							<td>&nbsp;</td>
						</tr>
						<tr>
							<td>&nbsp;
							</td>
							<td width="100%"><br>
								<center>___________________________________<br></center>
								Nome: '.$preposto['usunome'].'
								<br>CPF: '.$preposto['usucpf'].'</td> 
							<td>&nbsp;</td>
						</tr>
					</table>
					
					<br>
					
					Bras�lia, '.date('d').' de '.$data->mesTextual( (int)date('m') ).' de '.date('Y').'.';
					
					
				$html .= '</center>
					
				
			  ';
	
	$html .= '</td></tr></tbody></table>';
	
	$html .= '</body></html>';
	
	//insere historico
	if($os){
		//recupera ss
		$sql = "select scsid from fabrica.ordemservico where odsid = $os";
		$scsid = $db->pegaUm($sql);
		
		//inserir historico
		$tptid = 1;
		$sql = "INSERT INTO fabrica.termo(tptid, scsid, odsid, usucpf, data, corpo)
	    		VALUES ($tptid, 
	    				$scsid, 
	    				$os, 
	    				'".$_SESSION['usucpf']."', 
	    				'".date('Y-m-d H:i:s')."', 
	    				'".$html."')";
		$db->executar($sql);
		$db->commit();
	}			
	
	return $html;
}

function TermoAberturaOrdemServicoWeb($os){
	global $db;
	
	$data = new Data();
	
	$sql = "SELECT
				ansgarantia as garantia,
				fas.ansid,
				su.usunome,
				su.usuemail,
				su.usufoneddd,
				su.usufonenum,
				ss.scsid,
				fas.tpsid,
				to_char(ss.dataabertura, 'DD/MM/YYYY') as dataabertura,
				to_char(fas.ansprevinicio, 'DD/MM/YYYY') as previnicio,
				to_char(fas.ansprevinicio, 'YYYY') as ano,
				to_char(fas.ansprevtermino, 'DD/MM/YYYY') as prevtermino,
				os.odsqtdpfestimada,
				ss.odsidorigem,
				os.odsdetalhamento,
				os.odsqtdpfestimada,
				dsd.sidid,
				dsd.sidabrev, 
				dsd.siddescricao,
				un.unidsc,
				os.tosid,
				CASE WHEN os.tosid = 1 THEN
						(select ctrnumero from fabrica.contrato fc
						INNER JOIN entidade.entidade ee ON ee.entid = fc.entidcontratado
						inner join fabrica.analisesolicitacao a on a.ctrid = fc.ctrid 
						where ctrstatus='A' and ctrcontagem = false and a.scsid = fas.scsid limit 1)
				     ELSE
						(select ctrnumero from fabrica.contrato fc
						INNER JOIN entidade.entidade ee ON ee.entid = fc.entidcontratado 
						inner join fabrica.analisesolicitacao a on a.ctrid = fc.ctrid
						where ctrstatus='A' and ctrcontagem = true and a.scsid = fas.scsid limit 1)
				END as contrato,
				CASE WHEN os.tosid = 1 THEN
						(select entnome from fabrica.contrato fc
						INNER JOIN entidade.entidade ee ON ee.entid = fc.entidcontratado
						inner join fabrica.analisesolicitacao a on a.ctrid = fc.ctrid 
						where ctrstatus='A' and ctrcontagem = false and a.scsid = fas.scsid limit 1)
				     ELSE
						(select entnome from fabrica.contrato fc
						INNER JOIN entidade.entidade ee ON ee.entid = fc.entidcontratado 
						inner join fabrica.analisesolicitacao a on a.ctrid = fc.ctrid
						where ctrstatus='A' and ctrcontagem = true and a.scsid = fas.scsid limit 1)
				END as entnome
			FROM 
				fabrica.ordemservico  os
			LEFT JOIN
				fabrica.profissionalos po ON po.odsid = os.odsid
			INNER JOIN
				fabrica.solicitacaoservico ss ON ss.scsid = os.scsid
			LEFT JOIN
				seguranca.usuario su ON su.usucpf = ss.usucpfrequisitante
			INNER JOIN
				fabrica.analisesolicitacao fas ON fas.scsid = os.scsid
			LEFT JOIN 
				public.unidade un ON un.unicod = su.unicod
			LEFT JOIN
				demandas.sistemadetalhe dsd ON dsd.sidid = ss.sidid AND dsd.sidstatus = 'A'
			WHERE 
				os.odsid = {$os};";
	
	$solicitacao = $db->pegaLinha($sql);

	
	$html = TermoCabecalho('Termo de Abertura de Ordem de Servi�o Web', $solicitacao['scsid'], $os.' / '.$solicitacao['ano'], $solicitacao['sidid']);
	
	$html .= '<tbody><tr><td width=100% valign="top">
	
				<table cellSpacing="1" cellPadding=3 align="center" width="100%" border="1">
					<tr>
						<td>Data de abertura: '.$solicitacao['dataabertura'].'<td>
						<td>N� '.$os.' / '.date('Y').'<td>
					</tr>
				</table>
				
				<br>
				
				<table class="tabela" cellSpacing="1" cellPadding=3 align="center" width="100%" border="1">
					<tr>
						<td colspan="6">Dado(s) do Requisitante</td>
					</tr>
					<tr>
						<td colspan="2">Nome</td>
						<td colspan="4">'.$solicitacao['usunome'].'</td>
					</tr>
					<tr>
						<td colspan="2">Telefone(s)</td>
						<td colspan="4">('.$solicitacao['usufoneddd'].') '.$solicitacao['usufonenum'].'</td>
					</tr>
					<tr>
						<td colspan="2">E-mail</td>
						<td colspan="4">'.$solicitacao['usuemail'].'</td>
					</tr>
					<tr>
						<td colspan="2">Setor</td>
						<td colspan="4">'.$solicitacao['unidsc'].'</td>
					</tr>
					<tr>
						<td colspan="2">Empresa Contratada</td>
						<td colspan="4">'.$solicitacao['entnome'].'</td>
					</tr>
					<tr>
						<td colspan="2">Sigla do Sistema</td>
						<td colspan="2">'.$solicitacao['sidabrev'].'</td>
						<td colspan="1">Nome do Sistema</td>
						<td colspan="1">'.(($solicitacao['siddescricao'])?$solicitacao['siddescricao']:'&nbsp;').'</td>
					</tr>';
	
	//verifica se � contrada ou contratante
	if($solicitacao['tosid'] == '2' || $solicitacao['tosid'] == '3'){
			
			//tiposervico
			$estimado = '&nbsp;&nbsp;';
			$detalhado = '&nbsp;&nbsp;';
			if($solicitacao['tosid'] == '2') $estimado = 'X';
			if($solicitacao['tosid'] == '3') $detalhado = 'X'; 
			
			$htmlTipoServico = '<tr>
									<td colspan="2">Tipo de Servi�o</td>
									<td colspan="4">( '.$estimado.' ) Contagem APF Estimada <br>
	 												( '.$detalhado.' ) Contagem APF Detalhada
	 								</td>
								</tr>';
			
			// disciplinas
			$htmlDisciplinas = '';
	}
	else{
			//tiposervico
			$sql = "SELECT 
						tpsid, 
						tpsdsc 
					FROM 
						fabrica.tiposervico 
					WHERE 
						tpsstatus='A'
						OR tpsid={$solicitacao['tpsid']}";
			$tiposervico = $db->carregar($sql);
			
			if($tiposervico[0]) {
				$htmlTipoServico = '<tr>
								<td colspan="2">Tipo de Servi�o</td>
								<td colspan="4">';
				foreach($tiposervico as $tps) {
					$htmlTipoServico .= '( '.(($tps['tpsid']==$solicitacao['tpsid'])?'X':'&nbsp;&nbsp;').' ) '.$tps['tpsdsc'].'<br/>';
				}
				
				$htmlTipoServico .= '</td>
							</tr>';
			}
	
	}

		
	$html .=		$htmlTipoServico.
					'<tr>
						<td colspan="2">Servi�o contratado</td>
						<td colspan="5">
						
							<table border="1" width="100%">
								<tr>
									<td>EST - A ( )</td>
									<td>EST - C1 ( )</td>
									<td>EST - C2 ( )</td>
									<td>LAY - A ( )</td>
								</tr>
								<tr>
									<td>Quantidade ( )</td>
									<td>Quantidade ( )</td>
									<td>Quantidade ( )</td>
									<td>Quantidade ( )</td>
								</tr>
								<tr>
									<td>&nbsp;</td>
									<td>Complexidade ( )</td>
									<td>Complexidade ( )</td>
									<td>&nbsp;</td>
								</tr>
								<tr>
									<td>CBD ( )</td>
									<td>MSG ( )</td>
									<td>MNU ( )</td>
									<td>CDT ( )</td>
								</tr>
								<tr>
									<td>&nbsp;</td>
									<td>Quantidade ( )</td>
									<td>Quantidade ( )</td>
									<td>&nbsp;</td>
								<tr>
									<td>FOR ( )</td>
									<td>NMU ( )</td>
									<td>AUX ( )</td>
									<td>PGE ( )</td>
								<tr>
									<td>ENG ( )</td>
									<td>SAT ( )</td>
									<td>GCM - A ( )</td>
									<td>POR ( )</td>
								</tr>
								<tr>
									<td>&nbsp;</td>
									<td>&nbsp;</td>
									<td>�tem ( )</td>
									<td>�tens ( )</td>
								</tr>
								<tr>
									<td>&nbsp;</td>
									<td>&nbsp;</td>
									<td>Quantidade ( )</td>
									<td>&nbsp;</td>
								</tr>
							</table> 
							
						</td>
					</tr>
					<tr>
						<td>In�cio Previsto</td>
						<td>'.$solicitacao['previnicio'].'</td>
						<td>T�rmino Previsto</td>
						<td>'.$solicitacao['prevtermino'].'</td>
						<td>Data para entrega do Plano do Projeto</td>
						<td>n�o sei onde fica</td>
					</tr>
					<tr>
						<td colspan="2">Quantidade prevista de Pontos de Fun��o</td>
						<td colspan="4">'.$solicitacao['odsqtdpfestimada'].'</td>
					</tr>
					<tr>
						<td colspan="2">Servi�o em garantia?</td>
						<td>
							'.( $solicitacao['garantia'] == 'f' ? 'Sim (&nbsp;&nbsp;) N�o ( X )' : 'Sim ( X ) N�o (&nbsp;&nbsp;)' ).' 
						</td>
						<td colspan="2">Ordem de Servi�o Associada</td>
						<td>'.$solicitacao['odsidorigem'].'</td>
					</tr>
				</table>
				<br>
				<br>
				1.	Descri��o Detalhada do Servi�o Solicitado
					<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;';
				
	
	$html .= $solicitacao['odsdetalhamento'];
	
	$html .= '<br>
				<br>
				<br>
				2.	Anexos (impress�o opcional)';
	
	$sql = "SELECT 
				arqdescricao||' - '||arqnome||'.'||arqextensao as anexo
			FROM 
				fabrica.anexosolicitacao an 
			LEFT JOIN 
				fabrica.tipoanexosolicitacao tp ON an.tasid=tp.tasid 
			LEFT JOIN 
				public.arquivo ar ON ar.arqid=an.arqid 
			LEFT JOIN 
				seguranca.usuario us ON us.usucpf=ar.usucpf 
			LEFT JOIN
				fabrica.ordemservico fos ON fos.scsid = an.scsid
			WHERE 
				fos.odsid = {$os}
				AND ansstatus='A'";
	
	$anexos = $db->carregar($sql);
	
	if($anexos[0]){
		foreach($anexos as $anexo){
			$html .= '<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$anexo['anexo'];
		}
	}else{
		$html .= '<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;N�o h� anexos.<br/>';
	}
	
	
	
	//artefatos gerados
	$htmlArtefatosGerados = '<br>
				<br>
				<br>
				3.	Artefatos a serem gerados <br>';
	
	
	//pega tipo
	if($solicitacao['tosid'] == '1') $tpeid = 1;
	else $tpeid = 2;
	
	if($tpeid) $where = "WHERE tpeid = $tpeid";
	$sql = "SELECT tpeid, tpedsc FROM fabrica.tipoexecucao $where ORDER BY 1";
	$tipo = $db->carregar($sql);
	
	if($tipo){
		
		$idtable = true;
		
		if($idtable) $htmlArtefatosGerados .= '<table class=tabela bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 width="100%" >';
		
		for($t=0;$t<=count($tipo)-1;$t++){
			
			$tpeid = $tipo[$t]['tpeid'];
			$tpedsc = $tipo[$t]['tpedsc'];
	
			//pega disciplinas
			$sql = "SELECT distinct d.dspid, d.dspdsc
					FROM fabrica.servicofaseproduto sp 
					INNER JOIN fabrica.fasedisciplinaproduto fdp ON fdp.fdpid = sp.fdpid
					INNER JOIN fabrica.fasedisciplina fd ON fd.fsdid = fdp.fsdid
					INNER JOIN fabrica.disciplina d ON d.dspid = fd.dspid
					WHERE sp.ansid = {$solicitacao['ansid']} 
					AND sp.tpeid = {$tpeid}
					order by 1";
			$disciplina = $db->carregar($sql);
			
			$txtTd = '';
			
			if($disciplina){
				
				for($j=0;$j<=count($disciplina)-1;$j++){
					
					$dspid = $disciplina[$j]['dspid'];
					
					$txtTd .= '<span style="padding-left:40px"><b>'.trim($disciplina[$j]['dspdsc']).'</b></span><br>';
				
					//pega fases
					$sql = "SELECT distinct f.fasid, f.fasdsc 
							FROM fabrica.servicofaseproduto sp 
							INNER JOIN fabrica.fasedisciplinaproduto fdp ON fdp.fdpid = sp.fdpid
							INNER JOIN fabrica.fasedisciplina fd ON fd.fsdid = fdp.fsdid
							INNER JOIN fabrica.fase f ON f.fasid = fd.fasid
							WHERE sp.ansid = {$solicitacao['ansid']}
							AND sp.tpeid = {$tpeid}
							AND fd.dspid = {$dspid}
							ORDER BY 1";
					$fase = $db->carregar($sql);
				
					if($fase) {
						
						for($i=0;$i<=count($fase)-1;$i++){
							
							$fasid = $fase[$i]['fasid'];
							
							$sql = "SELECT p.prddsc 
									FROM fabrica.servicofaseproduto sp 
									INNER JOIN fabrica.fasedisciplinaproduto fdp ON fdp.fdpid = sp.fdpid
									INNER JOIN fabrica.fasedisciplina fd ON fd.fsdid = fdp.fsdid
									INNER JOIN fabrica.produto p ON p.prdid = fdp.prdid
									WHERE sp.ansid = {$solicitacao['ansid']}
									and sp.tpeid = {$tpeid} 
									and fd.dspid = {$dspid}
									and fd.fasid = {$fasid}
									ORDER BY 1";
							$produto = $db->carregarColuna($sql);
					
							if($produto){
								$txtTd .= '<span style="padding-left:60px"><b> - '.$fase[$i]['fasdsc'].'</b></span><br> <div style="padding-left:80px"> - ' . implode(";<br> - ", $produto) . ';</div>';
							}
							
						}
						
					}
							
				}//fecha for disciplina		
				
			}
			else{
					$txtTd = "N/A";
			}
			
			
			$htmlArtefatosGerados .= '
					<tr>
						<td width="100%">
						    <br>'.$txtTd.'<br>
						</td>
					</tr>
			';
		
		}
		
		if($idtable) $htmlArtefatosGerados .= '</table>';
				
	}// fim artefatos gerados
	
	
	$html .= 	$htmlArtefatosGerados.
				'<br>
				<br>
				<br>
				4.	Cronograma de Execu��o
					<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$solicitacao['previnicio'].' at� '.$solicitacao['prevtermino'].'
				<br>
				<br>
				<br>
								
				<center>
					<table border="0" width="80%">
						<tr>
							<td><center>___________________________________<br>
								Preposto Contratada</center>
							</td>
							<td>
								<center>___________________________________<br>
								Gestor do Contrato</center>
							</td>
						</tr>
						<tr>
							<td><br><br><center>___________________________________<br>
								Requisitante</center>
							</td>
							<td>&nbsp;</td>
						</tr>
					</table>

					<br>
					<br>
					<br>
					
					Bras�lia, '.date('d').' de '.$data->mesTextual( (int)date('m') ).' de '.date('Y').'.
					
					<div style="page-break-before: always;"></div>

				</center>
				
			  ';
	
	$html .= '</td></tr></tbody></table>';
	
	$html .= '</body></html>';
	
	//insere historico
	if($os){
		//recupera ss
		$sql = "select scsid from fabrica.ordemservico where odsid = $os";
		$scsid = $db->pegaUm($sql);
		
		//inserir historico
		$tptid = 2;
		$sql = "INSERT INTO fabrica.termo(tptid, scsid, odsid, usucpf, data, corpo)
	    		VALUES ($tptid, 
	    				$scsid, 
	    				$os, 
	    				'".$_SESSION['usucpf']."', 
	    				'".date('Y-m-d H:i:s')."',
	    				'".$html."')";
		$db->executar($sql);
		$db->commit();
	}			
	
	return $html;
}

function TermoAceitedoServico($os){
	
	global $db;
	
	$data = new Data();
	
	$sql = "SELECT
				ansgarantia as garantia,
				fas.ansid,
				su.usunome,
				su.usuemail,
				su.usufoneddd,
				su.usufonenum,
				ss.scsid,
				fas.tpsid,
				to_char(ss.dataabertura, 'DD/MM/YYYY') as dataabertura,
				to_char(odsdtprevinicio, 'DD/MM/YYYY') as datainicio,
				to_char(odsdtprevinicio, 'YYYY') as ano,
				to_char(odsdtprevtermino, 'DD/MM/YYYY') as datatermino,
				to_char(fas.ansprevinicio, 'DD/MM/YYYY') as previnicio,
				to_char(fas.ansprevtermino, 'DD/MM/YYYY') as prevtermino,
				os.odsqtdpfestimada,
				ss.odsidorigem,
				os.odsqtdpfestimada,
				os.odsdetalhamento,
				to_char(ss.scsprevatendimento, 'DD/MM/YYYY') as expectativaatendimento,
				os.odsqtdpfdetalhada,
				dst.sitdsc as tecnologia,
				dsd.sidid,
				dsd.sidabrev, 
				dsd.siddescricao,
				un.unidsc,
				os.tosid,
				CASE WHEN os.tosid = 1 THEN
						(select ctrnumero from fabrica.contrato where ctrstatus='A' and ctrcontagem = false limit 1)
				     ELSE
						(select ctrnumero from fabrica.contrato where ctrstatus='A' and ctrcontagem = true limit 1)
				END as contrato,
				CASE WHEN os.tosid = 1 THEN
						(select entnome from fabrica.contrato fc
						INNER JOIN entidade.entidade ee ON ee.entid = fc.entidcontratado 
						where ctrstatus='A' and ctrcontagem = false limit 1)
				     ELSE
						(select entnome from fabrica.contrato fc
						INNER JOIN entidade.entidade ee ON ee.entid = fc.entidcontratado 
						where ctrstatus='A' and ctrcontagem = true limit 1)
				END as entnome				
			FROM 
				fabrica.ordemservico  os
			LEFT JOIN
				fabrica.profissionalos po ON po.odsid = os.odsid
			INNER JOIN
				fabrica.solicitacaoservico ss ON ss.scsid = os.scsid
			LEFT JOIN
				seguranca.usuario su ON su.usucpf = ss.usucpfrequisitante
			INNER JOIN
				fabrica.analisesolicitacao fas ON fas.scsid = os.scsid
			LEFT JOIN 
				public.unidade un ON un.unicod = su.unicod
			LEFT JOIN
				demandas.sistemadetalhe dsd ON dsd.sidid = ss.sidid AND dsd.sidstatus = 'A'
			LEFT JOIN
				demandas.sistematecnologia dst ON dst.sitid = dsd.sitid AND dst.sitstatus = 'A'
			WHERE 
				os.odsid = {$os};";
	
	$solicitacao = $db->pegaLinha($sql);
	

	$html = TermoCabecalho('Termo de Aceite do Servi�o',  $solicitacao['scsid'], $os.' / '.$solicitacao['ano'], $solicitacao['sidid']);
	
	$html .= '<tbody><tr><td width=100% valign="top">
	
				<table cellSpacing="1" cellPadding=3 align="center" width="100%" border="1">
					<tr>
						<td>Data do Aceite T�cnico: '.$solicitacao['dataabertura'].'<td>
						<td>N� '.$os.' / '.date('Y').'<td>
					</tr>
				</table>
				
				<br>
				
				<table class="tabela" cellSpacing="1" cellPadding=3 align="center" width="100%" border="1">
					<tr>
						<td colspan="6">Dado(s) do Usu�rio(s) Gestor(es) Solicitante(s)</td>
					</tr>
					<tr>
						<td colspan="2">Nome</td>
						<td colspan="4">'.$solicitacao['usunome'].'</td>
					</tr>
					<tr>
						<td colspan="2">Telefone(s)</td>
						<td colspan="4">('.$solicitacao['usufoneddd'].') '.$solicitacao['usufonenum'].'</td>
					</tr>
					<tr>
						<td colspan="2">E-mail</td>
						<td colspan="4">'.$solicitacao['usuemail'].'</td>
					</tr>
					<tr>
						<td colspan="2">Setor</td>
						<td colspan="4">'.$solicitacao['unidsc'].'</td>
					</tr>
					<tr>
						<td colspan="2">Nome do Sistema</td>
						<td colspan="4">'.(($solicitacao['siddescricao'])?$solicitacao['siddescricao']:'&nbsp;').'</td>
					</tr>';
	
	
	//verifica se � contrada ou contratante
	if($solicitacao['tosid'] == '2' || $solicitacao['tosid'] == '3'){
			
			//tiposervico
			$estimado = '&nbsp;&nbsp;';
			$detalhado = '&nbsp;&nbsp;';
			if($solicitacao['tosid'] == '2') $estimado = 'X';
			if($solicitacao['tosid'] == '3') $detalhado = 'X'; 
			
			$htmlTipoServico = '<tr>
									<td colspan="2">Tipo de Servi�o</td>
									<td colspan="4">( '.$estimado.' ) Contagem APF Estimada <br>
	 												( '.$detalhado.' ) Contagem APF Detalhada
	 								</td>
								</tr>';
			
			// disciplinas
			$htmlDisciplinas = '';
	}
	else{
			//tiposervico
			$sql = "SELECT 
						tpsid, 
						tpsdsc 
					FROM 
						fabrica.tiposervico 
					WHERE 
						tpsstatus='A'
						OR tpsid={$solicitacao['tpsid']}";
			$tiposervico = $db->carregar($sql);
			
			if($tiposervico[0]) {
				$htmlTipoServico = '<tr>
								<td colspan="2">Tipo de Servi�o</td>
								<td colspan="4">';
				foreach($tiposervico as $tps) {
					$htmlTipoServico .= '( '.(($tps['tpsid']==$solicitacao['tpsid'])?'X':'&nbsp;&nbsp;').' ) '.$tps['tpsdsc'].'<br/>';
				}
				
				$htmlTipoServico .= '</td>
							</tr>';
			}
	
	}
	
	
	$html .=        $htmlTipoServico.
					'<tr>
						<td colspan="2">Documentos anexados</td>
						<td colspan="4">';
							
	$sql = "SELECT 
				arqdescricao||' - '||arqnome||'.'||arqextensao as anexo
			FROM 
				fabrica.anexosolicitacao an 
			LEFT JOIN 
				fabrica.tipoanexosolicitacao tp ON an.tasid=tp.tasid 
			LEFT JOIN 
				public.arquivo ar ON ar.arqid=an.arqid 
			LEFT JOIN 
				seguranca.usuario us ON us.usucpf=ar.usucpf 
			LEFT JOIN
				fabrica.ordemservico fos ON fos.scsid = an.scsid
			WHERE 
				fos.odsid = {$os}
				AND ansstatus='A'";
	
	$anexos = $db->carregar($sql);
	
	if($anexos[0]){
		foreach($anexos as $anexo){
			$html .= $anexo['anexo'].'<br/>';
		}
	}else{
		$html .= 'N�o h� anexos.<br/>';
	}
	
	$html .=           '</td>
					</tr>
					<tr>
						<td>In�cio Previsto</td>
						<td>'.$solicitacao['previnicio'].'</td>
						<td>T�rmino Previsto</td>
						<td>'.$solicitacao['prevtermino'].'</td>
						<td>Previs�o Plano do Projeto</td>
						<td>N�o sei onde fica</td>
					</tr>
					<tr>
						<td>In�cio Formal</td>
						<td>'.$solicitacao['previnicio'].'</td>
						<td>T�rmino Formal</td>
						<td>'.$solicitacao['datatermino'].'</td>
						<td>Plano do Projeto Entregue</td>
						<td>N�o sei onde fica</td>
					</tr>
					<tr>
						<td colspan="2">Tecnologia adotada</td>
						<td colspan="4">'.$solicitacao['tecnologia'].'</td>
					</tr>
					<tr>
						<td colspan="3">Quantidade estimada de Pontos de Fun��o</td>
						<td colspan="3">'.$solicitacao['odsqtdpfestimada'].'</td>
					</tr>
					<tr>
						<td colspan="3">Quantidade detalhada de Pontos de Fun��o</td>
						<td colspan="3">'.$solicitacao['odsqtdpfdetalhada'].'</td>
					</tr>
					<tr>
						<td colspan="2">Servi�o em garantia?</td>
						<td>'.( $solicitacao['garantia'] == 'f' ? 'Sim (&nbsp;&nbsp;) N�o ( X )' : 'Sim ( X ) N�o (&nbsp;&nbsp;)' ).'</td>
						<td colspan="2">Solicita��o de Servi�o original </td>
						<td>'.$solicitacao['odsidorigem'].'</td>
					</tr>
				</table>
				<br>
				<br>
				1.	Descri��o das Necessidades
					<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;';
	
			    $html .= $solicitacao['odsdetalhamento'];
			    		
				$html .= '<br>
				<br>
				<br>
				2.	Documentos e Legisla��es relacionadas
					<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
				<br>
				<br>
				<br>
				3.	Expectativa do Usu�rio para Atendimento
					<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$solicitacao['expectativaatendimento'].'
				';
	
	
	//artefatos gerados
	$htmlArtefatosGerados = '<br>
				<br>
				<br>
				4.	Artefatos / Produtos <br>';
	
	
	//pega tipo
	if($solicitacao['tosid'] == '1') $tpeid = 1;
	else $tpeid = 2;
	
	if($tpeid) $where = "WHERE tpeid = $tpeid";
	$sql = "SELECT tpeid, tpedsc FROM fabrica.tipoexecucao $where ORDER BY 1";
	$tipo = $db->carregar($sql);
	
	if($tipo){
		
		$idtable = true;
		
		if($idtable) $htmlArtefatosGerados .= '<table class=tabela bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 width="100%" >';
		
		for($t=0;$t<=count($tipo)-1;$t++){
			
			$tpeid = $tipo[$t]['tpeid'];
			$tpedsc = $tipo[$t]['tpedsc'];
	
			//pega disciplinas
			$sql = "SELECT distinct d.dspid, d.dspdsc
					FROM fabrica.servicofaseproduto sp 
					INNER JOIN fabrica.fasedisciplinaproduto fdp ON fdp.fdpid = sp.fdpid
					INNER JOIN fabrica.fasedisciplina fd ON fd.fsdid = fdp.fsdid
					INNER JOIN fabrica.disciplina d ON d.dspid = fd.dspid
					WHERE sp.ansid = {$solicitacao['ansid']} 
					AND sp.tpeid = {$tpeid}
					order by 1";
			$disciplina = $db->carregar($sql);
			
			$txtTd = '';
			
			if($disciplina){
				
				for($j=0;$j<=count($disciplina)-1;$j++){
					
					$dspid = $disciplina[$j]['dspid'];
					
					$txtTd .= '<span style="padding-left:40px"><b>'.trim($disciplina[$j]['dspdsc']).'</b></span><br>';
				
					//pega fases
					$sql = "SELECT distinct f.fasid, f.fasdsc 
							FROM fabrica.servicofaseproduto sp 
							INNER JOIN fabrica.fasedisciplinaproduto fdp ON fdp.fdpid = sp.fdpid
							INNER JOIN fabrica.fasedisciplina fd ON fd.fsdid = fdp.fsdid
							INNER JOIN fabrica.fase f ON f.fasid = fd.fasid
							WHERE sp.ansid = {$solicitacao['ansid']}
							AND sp.tpeid = {$tpeid}
							AND fd.dspid = {$dspid}
							ORDER BY 1";
					$fase = $db->carregar($sql);
				
					if($fase) {
						
						for($i=0;$i<=count($fase)-1;$i++){
							
							$fasid = $fase[$i]['fasid'];
							
							$sql = "SELECT p.prddsc 
									FROM fabrica.servicofaseproduto sp 
									INNER JOIN fabrica.fasedisciplinaproduto fdp ON fdp.fdpid = sp.fdpid
									INNER JOIN fabrica.fasedisciplina fd ON fd.fsdid = fdp.fsdid
									INNER JOIN fabrica.produto p ON p.prdid = fdp.prdid
									WHERE sp.ansid = {$solicitacao['ansid']}
									and sp.tpeid = {$tpeid} 
									and fd.dspid = {$dspid}
									and fd.fasid = {$fasid}
									ORDER BY 1";
							$produto = $db->carregarColuna($sql);
					
							if($produto){
								$txtTd .= '<span style="padding-left:60px"><b> - '.$fase[$i]['fasdsc'].'</b></span><br> <div style="padding-left:80px"> - ' . implode(";<br> - ", $produto) . ';</div>';
							}
							
						}
						
					}
							
				}//fecha for disciplina		
				
			}
			else{
					$txtTd = "N/A";
			}
			
			
			$htmlArtefatosGerados .= '
					<tr>
						<td width="100%">
						    <br>'.$txtTd.'<br>
						</td>
					</tr>
			';
		
		}
		
		if($idtable) $htmlArtefatosGerados .= '</table>';
				
	}// fim artefatos gerados
	
	
	$html .=	$htmlArtefatosGerados.
				'<br>
				<br>
				<br>
				4.1.	Artefatos Fornecidos
					<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
				<br>
				<br>
				<br>
				4.2.	Artefatos a serem gerados
					<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
				<br>
				<br>
				<br>
				5.	Cronograma de execu��o da OS
					<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$solicitacao['previnicio'].' at� '.$solicitacao['prevtermino'].'
				<br>
				<br>
				<br>
				5.1.	N�vel de Satisfa��o<br>
				&nbsp;&nbsp;&nbsp;(  ) Declaro que os servi�os foram executados totalmente<br>
				&nbsp;&nbsp;&nbsp;(  ) Declaro que os servi�os foram executados com ressalva
				<br>
				<br>
				<br>
				5.1.1.	Ressalvas quanto ao servi�o executado:
				<br>
				<br>
				<br> 
				<center>
				
					<table border="0" width="80%">
						<tr>
							<td><center>___________________________________<br>
								Preposto T�cnico</center>
							</td>
							<td>
								<center>___________________________________<br>
								Respons�vel CGD/DTI</center>
							
							</td>
						</tr>
						<tr>
							<td><br><br><center>___________________________________<br>
								Gerente Respons�vel</center>
							</td>
							<td>&nbsp;</td>
						</tr>
					</table>
					
					<br>
					<br>
					<br>
					
					Bras�lia, '.date('d').' de '.$data->mesTextual( (int)date('m') ).' de '.date('Y').'.
					
					<div style="page-break-before: always;"></div>

				</center>
				
			  ';
	
	$html .= '</td></tr></tbody></table>';
	
	$html .= '</body></html>';
	
	//insere historico
	if($os){
		//recupera ss
		$sql = "select scsid from fabrica.ordemservico where odsid = $os";
		$scsid = $db->pegaUm($sql);
		
		//inserir historico
		$tptid = 3;
		
		$sql = "INSERT INTO fabrica.termo(tptid, scsid, odsid, usucpf, data, corpo)
	    		VALUES ($tptid, 
	    				$scsid, 
	    				$os, 
	    				'".$_SESSION['usucpf']."', 
	    				'".date('Y-m-d H:i:s')."', 
	    				'".$html."')";
		$db->executar($sql);
		$db->commit();
	}		
	
	return $html;
}

function TermoEntregaOrdemServicoHomologacao($os){
	
	global $db;
	
	$data = new Data();
	
	$sql = "SELECT
				ansgarantia as garantia,
				os.docid,
				os.scsid,
				to_char(odsdtprevinicio, 'YYYY') as ano,
				un.unidsc,
				dsd.sidid,
				dsd.sidabrev, 
				dsd.siddescricao,
				os.tosid,
				CASE WHEN os.tosid = 1 THEN
						(select ctrnumero from fabrica.contrato fc
						INNER JOIN entidade.entidade ee ON ee.entid = fc.entidcontratado
						inner join fabrica.analisesolicitacao a on a.ctrid = fc.ctrid 
						where ctrstatus='A' and ctrcontagem = false and a.scsid = fas.scsid limit 1)
				     ELSE
						(select ctrnumero from fabrica.contrato fc
						INNER JOIN entidade.entidade ee ON ee.entid = fc.entidcontratado
						inner join fabrica.analisesolicitacao a on a.ctrid = fc.ctrid 
						where ctrstatus='A' and ctrcontagem = true and a.scsid = fas.scsid limit 1)
				END as contrato,
				CASE WHEN os.tosid = 1 THEN
						(select entnome from fabrica.contrato fc
						INNER JOIN entidade.entidade ee ON ee.entid = fc.entidcontratado
						inner join fabrica.analisesolicitacao a on a.ctrid = fc.ctrid 
						where ctrstatus='A' and ctrcontagem = false and a.scsid = fas.scsid limit 1)
				     ELSE
						(select entnome from fabrica.contrato fc
						INNER JOIN entidade.entidade ee ON ee.entid = fc.entidcontratado
						inner join fabrica.analisesolicitacao a on a.ctrid = fc.ctrid 
						where ctrstatus='A' and ctrcontagem = true and a.scsid = fas.scsid limit 1)
				END as entnome
			FROM 
				fabrica.ordemservico  os
			LEFT JOIN
				fabrica.profissionalos po ON po.odsid = os.odsid
			INNER JOIN
				fabrica.solicitacaoservico ss ON ss.scsid = os.scsid
			LEFT JOIN
				seguranca.usuario su ON su.usucpf = ss.usucpfrequisitante
			INNER JOIN
				fabrica.analisesolicitacao fas ON fas.scsid = os.scsid
			LEFT JOIN 
				public.unidade un ON un.unicod = su.unicod
			LEFT JOIN
				demandas.sistemadetalhe dsd ON dsd.sidid = ss.sidid AND dsd.sidstatus = 'A'
			WHERE 
				os.odsid = {$os}
				AND dsd.sidstatus = 'A';";
		
	$solicitacao = $db->pegaLinha($sql);
	
	$historico = wf_pegarHistorico( $solicitacao['docid'] );
	
	foreach ($historico as $valores) {
		if($valores['aeddscrealizada'] == 'Enviado para Avalia��o'){
			$data_homologacao = $valores['htddata'];
			break;
		}else{
			$data_homologacao = 'Ainda n�o foi homologado.';
		}// fim do if
	}// fim do foreach

	
	$html = TermoCabecalho('Termo de Entrega de Ordem de Servi�o Homologa��o',  $solicitacao['scsid'], $os.' / '.$solicitacao['ano'], $solicitacao['sidid']);
	
	$html .= '<tbody><tr><td width=100% valign="top">
			  
			  <table border="1" align="right">
			  	<tr>
			  		<td>N� OS(s)</td>
			  		<td>'.$os.'</td>
			  		<td>/</td>
			  		<td>2011</td>
			  	</tr>
			  </table>
			  
			  <br>
			  <br>
			  
				<table class="tabela" cellSpacing="1" cellPadding=3 align="center" width="100%" border="1">
					<tr>
						<td colspan="2">Dado(s) do(s) Contrato</td>
					</tr>
					<tr>
						<td>Contrato</td>
						<td>'.$solicitacao['contrato'].'</td>
					</tr>
					<tr>
						<td>Contratante</td>
						<td>Minist�rio da Educa��o</td>
					</tr>
					<tr>
						<td>Empresa Contratada</td>
						<td>'.$solicitacao['entnome'].'</td>
					</tr>
					<tr>
						<td>�rea Requisitante</td>
						<td>'.$solicitacao['unidsc'].'</td>
					</tr>
					<tr>
						<td>Sistema</td>
						<td>'.$solicitacao['siddescricao'].'</td>
					</tr>
				</table>
				<br>
				<br>
				<br>
				Declaramos que os produtos da(s) ordem(ns) de servi�o supracitada(s), foram entregues para homologa��o no dia:

				<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<i>'.$data_homologacao.'</i>
				
				<br>
				<br>
				<br>
				<br>
				<br>
				<br>
				<center>

					<table border="0" width="80%">
						<tr>
							<td><center>___________________________________<br>
								Contratante Fiscal T�cnico</center>
							</td>
							<td>
								<center>___________________________________<br>
								Contratada Preposto</center>
							
							</td>
						</tr>
						<tr>
							<td><br><br><center>___________________________________<br>
								Contratante Gerente de Projeto</center>
							</td>
							<td>&nbsp;</td>
						</tr>
					</table>
					
					<br>
					<br>
					<br>
					
					Bras�lia, '.date('d').' de '.$data->mesTextual( (int)date('m') ).' de '.date('Y').'.
					
					<div style="page-break-before: always;"></div>

				</center>
				
			  ';
	
	$html .= '</td></tr></tbody></table>';
	
	$html .= '</body></html>';
	
	//insere historico
	if($os){
		//recupera ss
		$sql = "select scsid from fabrica.ordemservico where odsid = $os";
		$scsid = $db->pegaUm($sql);
		
		//inserir historico
		$tptid = 4;
		
		$sql = "INSERT INTO fabrica.termo(tptid, scsid, odsid, usucpf, data, corpo)
	    		VALUES ($tptid, 
	    				$scsid, 
	    				$os, 
	    				'".$_SESSION['usucpf']."', 
	    				'".date('Y-m-d H:i:s')."',
	    				'".$html."')";
		$db->executar($sql);
		$db->commit();
	}		
	
	return $html;
	
}

function TermoHomologacao($os){
	
	global $db;
	
	$data = new Data();
	
	$sql = "SELECT
				ansgarantia as garantia,
				os.docid,
				os.scsid,
				to_char(odsdtprevinicio, 'YYYY') as ano,
				un.unidsc,
				dsd.sidid,
				dsd.sidabrev, 
				dsd.siddescricao,
				os.tosid,
				CASE WHEN os.tosid = 1 THEN
						(select ctrnumero from fabrica.contrato fc
						INNER JOIN entidade.entidade ee ON ee.entid = fc.entidcontratado 
						inner join fabrica.analisesolicitacao a on a.ctrid = fc.ctrid
						where ctrstatus='A' and ctrcontagem = false and a.scsid = fas.scsid limit 1)
				     ELSE
						(select ctrnumero from fabrica.contrato fc
						INNER JOIN entidade.entidade ee ON ee.entid = fc.entidcontratado 
						inner join fabrica.analisesolicitacao a on a.ctrid = fc.ctrid
						where ctrstatus='A' and ctrcontagem = true and a.scsid = fas.scsid limit 1)
				END as contrato,
				CASE WHEN os.tosid = 1 THEN
						(select entnome from fabrica.contrato fc
						INNER JOIN entidade.entidade ee ON ee.entid = fc.entidcontratado 
						inner join fabrica.analisesolicitacao a on a.ctrid = fc.ctrid
						where ctrstatus='A' and ctrcontagem = false and a.scsid = fas.scsid limit 1)
				     ELSE
						(select entnome from fabrica.contrato fc
						INNER JOIN entidade.entidade ee ON ee.entid = fc.entidcontratado 
						inner join fabrica.analisesolicitacao a on a.ctrid = fc.ctrid
						where ctrstatus='A' and ctrcontagem = true and a.scsid = fas.scsid limit 1)
				END as entnome
			FROM 
				fabrica.ordemservico  os
			LEFT JOIN
				fabrica.profissionalos po ON po.odsid = os.odsid
			INNER JOIN
				fabrica.solicitacaoservico ss ON ss.scsid = os.scsid
			LEFT JOIN
				seguranca.usuario su ON su.usucpf = ss.usucpfrequisitante
			INNER JOIN
				fabrica.analisesolicitacao fas ON fas.scsid = os.scsid
			LEFT JOIN 
				public.unidade un ON un.unicod = su.unicod
			LEFT JOIN
				demandas.sistemadetalhe dsd ON dsd.sidid = ss.sidid AND dsd.sidstatus = 'A'
			WHERE 
				os.odsid = {$os}
				AND dsd.sidstatus = 'A';";
		
	$solicitacao = $db->pegaLinha($sql);
	
	
	$html = TermoCabecalho('Termo de Entrega de Ordem de Servi�o Homologa��o', $solicitacao['scsid'], $os.' / '.$solicitacao['ano'], $solicitacao['sidid']);
	
	$html .= '<tbody><tr><td width=100% valign="top">
			  
			  <table border="1" align="right">
			  	<tr>
			  		<td>N� OS(s)</td>
			  		<td>'.$os.'</td>
			  		<td>/</td>
			  		<td>2011</td>
			  	</tr>
			  </table>
			  
			  <br>
			  <br>
			  
				<table class="tabela" cellSpacing="1" cellPadding=3 align="center" width="100%" border="1">
					<tr>
						<td colspan="2"><b>Dado(s) do(s) Contrato</b></td>
					</tr>
					<tr>
						<td><b>Contrato</b></td>
						<td>'.$solicitacao['contrato'].'</td>
					</tr>
					<tr>
						<td><b>Contratante</b></td>
						<td>Minist�rio da Educa��o</td>
					</tr>
					<tr>
						<td><b>Empresa Contratada</b></td>
						<td>'.$solicitacao['entnome'].'</td>
					</tr>
					<tr>
						<td><b>�rea Requisitante</b></td>
						<td>'.$solicitacao['unidsc'].'</td>
					</tr>
					<tr>
						<td><b>Sistema</b></td>
						<td>'.$solicitacao['siddescricao'].'</td>
					</tr>
				</table>
				<br>
				<br>
				<br>
				Declaramos que os produtos da(s) ordem(ns) de servi�o supracitada(s), foram homologadas de acordo com o descrito abaixo

				<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<i>(repetir quadro abaixo para cada produto que contenha justificativas diferentes. Caso os produtos tenha resultados de homologa��o iguais, pode-se relacionar mais de um produto por quadro):</i>
				
				<br>
				<br>
				
				<table class="tabela" cellSpacing="1" cellPadding=3 align="center" width="100%" border="1">
					<tr>
						<td><b>Produto1...n</b></td>
						<td>Nome do Produto</td>
					</tr>
					<tr>
						<td><b>Situa��o</b></td>
						<td>
							Homologado (   )<br>
							Homologado com ajustes (   )<br>
							Rejeitado (    )
						</td>
					</tr>
					<tr>
						<td><b>Justificativa</b></td>
						<td>Incluir a justificativa da homologa��o</td>
					</tr>
				</table>
				
				
				<br>
				<br>
				<br>
				<br>
				<br>
				<br>
				<center>

					<table border="0" >
						<tr>
							<td>
								<center>Contratante Fiscal T�cnico</center>
							</td>
							<td width="80">&nbsp;</td>
							<td>
								<center>Contratada Preposto</center>
							</td>
						</tr>
						<tr>
							<td><br><center>___________________________________<br></center>
								Nome:
								<br>Mat:
							</td>
							<td width="80">&nbsp;</td>
							<td><br>
								<center>___________________________________<br></center>
								Nome
								<br>CPF:
							
							</td>
						</tr>
					</table>
					
					<br>
					<br>
					<br>
					
					Bras�lia, '.date('d').' de '.$data->mesTextual( (int)date('m') ).' de '.date('Y').'.
					
					<div style="page-break-before: always;"></div>

				</center>
				
			  ';
	
	$html .= '</td></tr></tbody></table>';
	
	$html .= '</body></html>';
	
	//insere historico
	if($os){
		//recupera ss
		$sql = "select scsid from fabrica.ordemservico where odsid = $os";
		$scsid = $db->pegaUm($sql);
		
		//inserir historico
		$tptid = 5;
		
		$sql = "INSERT INTO fabrica.termo(tptid, scsid, odsid, usucpf, data, corpo)
	    		VALUES ($tptid, 
	    				$scsid, 
	    				$os, 
	    				'".$_SESSION['usucpf']."', 
	    				'".date('Y-m-d H:i:s')."',
	    				'".$html."')";
		$db->executar($sql);
		$db->commit();
	}		
	
	return $html;
	
}

function TermoRecebimentoDefinitivo($os){
	
	global $db;
	
	$data = new Data();
	
	$sql = "SELECT
				ansgarantia as garantia,
				os.docid,
				os.scsid,
				to_char(odsdtprevinicio, 'YYYY') as ano,
				un.unidsc,
				dsd.sidid,
				dsd.sidabrev, 
				dsd.siddescricao,
				os.tosid,
				CASE WHEN os.tosid = 1 THEN
						(select ctrnumero from fabrica.contrato fc
						INNER JOIN entidade.entidade ee ON ee.entid = fc.entidcontratado 
						inner join fabrica.analisesolicitacao a on a.ctrid = fc.ctrid
						where ctrstatus='A' and ctrcontagem = false and a.scsid = fas.scsid limit 1)
				     ELSE
						(select ctrnumero from fabrica.contrato fc
						INNER JOIN entidade.entidade ee ON ee.entid = fc.entidcontratado
						inner join fabrica.analisesolicitacao a on a.ctrid = fc.ctrid 
						where ctrstatus='A' and ctrcontagem = true and a.scsid = fas.scsid limit 1)
				END as contrato,
				CASE WHEN os.tosid = 1 THEN
						(select ctrobjeto from fabrica.contrato fc
						INNER JOIN entidade.entidade ee ON ee.entid = fc.entidcontratado 
						inner join fabrica.analisesolicitacao a on a.ctrid = fc.ctrid
						where ctrstatus='A' and ctrcontagem = false and a.scsid = fas.scsid limit 1)
				     ELSE
						(select ctrobjeto from fabrica.contrato fc
						INNER JOIN entidade.entidade ee ON ee.entid = fc.entidcontratado
						inner join fabrica.analisesolicitacao a on a.ctrid = fc.ctrid 
						where ctrstatus='A' and ctrcontagem = true and a.scsid = fas.scsid limit 1)
				END as ctrobjeto,
				CASE WHEN os.tosid = 1 THEN
						(select entnome from fabrica.contrato fc
						INNER JOIN entidade.entidade ee ON ee.entid = fc.entidcontratado 
						inner join fabrica.analisesolicitacao a on a.ctrid = fc.ctrid
						where ctrstatus='A' and ctrcontagem = false and a.scsid = fas.scsid limit 1)
				     ELSE
						(select entnome from fabrica.contrato fc
						INNER JOIN entidade.entidade ee ON ee.entid = fc.entidcontratado
						inner join fabrica.analisesolicitacao a on a.ctrid = fc.ctrid 
						where ctrstatus='A' and ctrcontagem = true and a.scsid = fas.scsid limit 1)
				END as entnome
			FROM 
				fabrica.ordemservico  os
			LEFT JOIN
				fabrica.profissionalos po ON po.odsid = os.odsid
			INNER JOIN
				fabrica.solicitacaoservico ss ON ss.scsid = os.scsid
			LEFT JOIN
				seguranca.usuario su ON su.usucpf = ss.usucpfrequisitante
			INNER JOIN
				fabrica.analisesolicitacao fas ON fas.scsid = os.scsid
			LEFT JOIN 
				public.unidade un ON un.unicod = su.unicod
			LEFT JOIN
				demandas.sistemadetalhe dsd ON dsd.sidid = ss.sidid AND dsd.sidstatus = 'A'
			WHERE 
				os.odsid = {$os}
				AND dsd.sidstatus = 'A';";
		
	$solicitacao = $db->pegaLinha($sql);
	

	$html = TermoCabecalho('Termo de Recebimento Definitivo', $solicitacao['scsid'], $os.' / '.$solicitacao['ano'], $solicitacao['sidid']);
	
	$sql = "select 
							u.usunome,
							n.nu_cpf,
							n.nu_matricula_siape
						from 
							seguranca.perfilusuario s
						inner join 
							seguranca.usuario u ON u.usucpf = s.usucpf
						inner join 
							siape.tb_siape_cadastro_servidor_ativos n ON n.nu_cpf = u.usucpf
						where  
							s.pflcod = ".PERFIL_GESTOR_CONTRATO."";
				
				$gestor = $db->pegaLinha($sql);
	
	$html .= '<tbody><tr><td width=100% valign="top">
			  
			  <table border="1" align="right">
			  	<tr>
			  		<td>N� OS(s)</td>
			  		<td>'.$os.'</td>
			  		<td>/</td>
			  		<td>2011</td>
			  	</tr>
			  </table>
			  
			  <br>
			  <br>
			  
				<table class="tabela" cellSpacing="1" cellPadding=3 align="center" width="100%" border="1">
					<tr>
						<td colspan="2"><b>Dado(s) do(s) Contrato</b></td>
					</tr>
					<tr>
						<td><b>Contrato</b></td>
						<td>'.$solicitacao['contrato'].'</td>
					</tr>
					<tr>
						<td><b>Objeto</b></td>
						<td>'.$solicitacao['ctrobjeto'].'</td>
					</tr>
					<tr>
						<td><b>Contratante</b></td>
						<td>Minist�rio da Educa��o</td>
					</tr>
					<tr>
						<td><b>Empresa Contratada</b></td>
						<td>'.$solicitacao['entnome'].'</td>
					</tr>
					<tr>
						<td><b>�rea Requisitante</b></td>
						<td>'.$solicitacao['unidsc'].'</td>
					</tr>
				</table>
				<br>
				<br>
				<br>
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Informamos que os servi�os constantes na Ordem de Servi�o descrita acima, foram executados de acordo com as especifica��es e prazo contratual estabelecido. Caso existam ocorr�ncias contratuais, verificar em anexo para c�lculo das OS.
				<br>
				<br>
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Solicitamos que esse Termo seja enviado para Faturamento.
				
				<br>
				<br>
				<br>
				<br>
				<br>
				<br>
				<center>

					<table border="0">
						<tr>
							<td colspan="3">
								<center>De Acordo</center>
								<br><br>
							</td>
						</tr>
						<tr>
							<td>
								<center>Contratante Gestor do Contrato</center>
							</td>
							<td width="80">&nbsp;</td>
							<td>
								<center>Contratada Preposto</center>
							</td>
						</tr>
						<tr>
							<td><br><center>___________________________________<br></center>
								Nome: '.$gestor['usunome'].'
								<br>Mat: '.$gestor['nu_matricula_siape'].'
							</td>
							<td width="80">&nbsp;</td>
							<td><br>
								<center>___________________________________<br></center>
								Nome
								<br>CPF:
							
							</td>
						</tr>
						<tr>
							<td><br><br>
								<center>Contratante Fiscal Administrativo</center>
							</td>
							<td width="80">&nbsp;</td>
							<td>&nbsp;</td>
						</tr>
						<tr>
							<td><br><center>___________________________________<br></center>
								Nome: Olavo Irineu de Araujo Neto
								<br>Mat: 1771763
							</td>
							<td width="80">&nbsp;</td>
							<td>&nbsp;</td>
						</tr>
					</table>
					
					<br>
					<br>
					<br>
					
					Bras�lia, '.date('d').' de '.$data->mesTextual( (int)date('m') ).' de '.date('Y').'.
					
					

				</center>
				
			  ';
	
	$html .= '</td></tr></tbody></table>';
	
	$html .= '</body></html>';
	
	//insere historico
	if($os){
		//recupera ss
		$sql = "select scsid from fabrica.ordemservico where odsid = $os";
		$scsid = $db->pegaUm($sql);
		
		//inserir historico
		$tptid = 6;
		
		$sql = "INSERT INTO fabrica.termo(tptid, scsid, odsid, usucpf, data, corpo)
	    		VALUES ($tptid, 
	    				$scsid, 
	    				$os, 
	    				'".$_SESSION['usucpf']."', 
	    				'".date('Y-m-d H:i:s')."',
	    				'".$html."')";
		$db->executar($sql);
		$db->commit();
	}	
	
	return $html;
		
}

function TermoRecebimentoProvisorio($os){
	global $db;
	
	$data = new Data();
	
	$sql = "SELECT
				ansgarantia as garantia,
				os.docid,
				os.scsid,
				to_char(odsdtprevinicio, 'YYYY') as ano,
				un.unidsc,
				dsd.sidid,
				dsd.sidabrev, 
				dsd.siddescricao,
				os.tosid,
				CASE WHEN os.tosid = 1 THEN
					(select ctrnumero from fabrica.contrato fc
					INNER JOIN entidade.entidade ee ON ee.entid = fc.entidcontratado
					inner join fabrica.analisesolicitacao a on a.ctrid = fc.ctrid 
					where ctrstatus='A' and ctrcontagem = false and a.scsid = fas.scsid limit 1)
				     ELSE
					(select ctrnumero from fabrica.contrato fc
					INNER JOIN entidade.entidade ee ON ee.entid = fc.entidcontratado 
					inner join fabrica.analisesolicitacao a on a.ctrid = fc.ctrid
					where ctrstatus='A' and ctrcontagem = true and a.scsid = fas.scsid limit 1)
				END as contrato,
				CASE WHEN os.tosid = 1 THEN
					(select ctrobjeto from fabrica.contrato fc
					INNER JOIN entidade.entidade ee ON ee.entid = fc.entidcontratado
					inner join fabrica.analisesolicitacao a on a.ctrid = fc.ctrid 
					where ctrstatus='A' and ctrcontagem = false and a.scsid = fas.scsid limit 1)
				     ELSE
					(select ctrobjeto from fabrica.contrato fc
					INNER JOIN entidade.entidade ee ON ee.entid = fc.entidcontratado 
					inner join fabrica.analisesolicitacao a on a.ctrid = fc.ctrid
					where ctrstatus='A' and ctrcontagem = true and a.scsid = fas.scsid limit 1)
				END as ctrobjeto,
				CASE WHEN os.tosid = 1 THEN
					(select entnome from fabrica.contrato fc
					INNER JOIN entidade.entidade ee ON ee.entid = fc.entidcontratado
					inner join fabrica.analisesolicitacao a on a.ctrid = fc.ctrid 
					where ctrstatus='A' and ctrcontagem = false and a.scsid = fas.scsid limit 1)
				     ELSE
					(select entnome from fabrica.contrato fc
					INNER JOIN entidade.entidade ee ON ee.entid = fc.entidcontratado 
					inner join fabrica.analisesolicitacao a on a.ctrid = fc.ctrid
					where ctrstatus='A' and ctrcontagem = true and a.scsid = fas.scsid limit 1)
				END as entnome
				
			FROM 
				fabrica.ordemservico  os
			LEFT JOIN
				fabrica.profissionalos po ON po.odsid = os.odsid
			INNER JOIN
				fabrica.solicitacaoservico ss ON ss.scsid = os.scsid
			LEFT JOIN
				seguranca.usuario su ON su.usucpf = ss.usucpfrequisitante
			INNER JOIN
				fabrica.analisesolicitacao fas ON fas.scsid = os.scsid
			LEFT JOIN 
				public.unidade un ON un.unicod = su.unicod
			LEFT JOIN
				demandas.sistemadetalhe dsd ON dsd.sidid = ss.sidid AND dsd.sidstatus = 'A'
			WHERE 
				os.odsid = {$os}
				AND dsd.sidstatus = 'A';";
		
	$solicitacao = $db->pegaLinha($sql);
	

	$html = TermoCabecalho('Termo de Recebimento Provis�rio', $solicitacao['scsid'], $os.' / '.$solicitacao['ano'], $solicitacao['sidid']);
	
	//verifica se � contratante ou contratada
	if($solicitacao['tosid'] == '1'){
		$trArea = '<tr>
						<td><b>�rea Requisitante</b></td>
						<td>'.$solicitacao['unidsc'].'</td>
					</tr>';
		/*
		$CpfNomePreposto = $db->pegaLinha("select 
								u.usucpf,
								u.usunome
							from 
								seguranca.perfilusuario pu
							inner join
								seguranca.usuario u ON u.usucpf = pu.usucpf
							where 
								pu.pflcod = ".PERFIL_PREPOSTO);
		*/
		 
	}
	else{
		/*
		$CpfNomePreposto = $db->pegaLinha("select 
								u.usucpf,
								u.usunome
							from 
								seguranca.perfilusuario pu
							inner join
								seguranca.usuario u ON u.usucpf = pu.usucpf
							where 
								pu.pflcod = ".PERFIL_CONTAGEM_PF);
		*/
		 
	}
	
				$sql = "select
						      u.usunome,
						      n.nu_matricula_siape
						from
						      fabrica.fiscalsolicitacao fs
						inner join
						      seguranca.usuario u ON u.usucpf = fs.usucpf
						inner join
						      seguranca.perfilusuario p ON p.usucpf = fs.usucpf and p.pflcod = ".PERFIL_FISCAL_CONTRATO."
						inner join
						      siape.tb_siape_cadastro_servidor_ativos n ON n.nu_cpf = fs.usucpf
						where  
						      fs.scsid = {$solicitacao['scsid']}";
				
				$fiscal = $db->pegaLinha($sql);
				
				$sql = "select 
							u.usunome,
							n.nu_matricula_siape
						from 
							fabrica.solicitacaoservico f 
						inner join 
							seguranca.usuario u ON u.usucpf = f.usucpfrequisitante
						inner join 
							siape.tb_siape_cadastro_servidor_ativos n ON n.nu_cpf = u.usucpf
						where  
							scsid = {$solicitacao['scsid']}";				
				
				if($solicitacao['tosid'] == 1)
				   $requisitante = $db->pegaLinha($sql);
				else  $requisitante = $fiscal;
				
				$sql = "select 
							u.usunome,
							s.usucpf
						from 
							seguranca.perfilusuario s
						left join 
							seguranca.usuario u ON u.usucpf = s.usucpf
						where  
							s.pflcod = ".($solicitacao['tosid'] == 1 ? PERFIL_PREPOSTO : PERFIL_CONTAGEM_PF)."";
				
				$preposto = $db->pegaLinha($sql);
	
					
	$html .= '<tbody><tr><td width=100% valign="top">
			  
			  <table border="1" align="right">
			  	<tr>
			  		<td>N� OS(s)</td>
			  		<td>'.$os.'</td>
			  		<td>/</td>
			  		<td>'.$solicitacao['ano'].'</td>
			  	</tr>
			  </table>
			  
			  <br>
			  <br>
			  
				<table class="tabela" cellSpacing="1" cellPadding=3 align="center" width="100%" border="1">
					<tr>
						<td colspan="2"><b>Dado(s) do(s) Contrato</b></td>
					</tr>
					<tr>
						<td width="20%"><b>Contrato</b></td>
						<td>'.$solicitacao['contrato'].'</td>
					</tr>
					<tr>
						<td><b>Objeto</b></td>
						<td>'.$solicitacao['ctrobjeto'].'</td>
					</tr>
					<tr>
						<td><b>Contratante</b></td>
						<td>Minist�rio da Educa��o</td>
					</tr>
					<tr>
						<td><b>Empresa Contratada</b></td>
						<td>'.$solicitacao['entnome'].'</td>
					</tr>
					'.$trArea.'
				</table>
				<br>
				<br>
				
				<table class="tabela" cellSpacing="1" cellPadding=3 align="center" width="100%" border="1">
					<tr>
						<td width="20%"><b>Situa��o</b></td>
						<td>Homologado (&nbsp;&nbsp;&nbsp;&nbsp;) 
						 
							&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
							&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
							
						    Homologado com ajustes (&nbsp;&nbsp;&nbsp;&nbsp;)
						    
						    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
						    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
						    
							Rejeitado  (&nbsp;&nbsp;&nbsp;&nbsp;)
						</td>
					</tr>
				</table>
								
				<br>
				<br>
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Por este instrumento, atestamos que os servi�os (ou bens), relacionados na O.S. acima identificada, foram recebidos nesta data.<br>
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Ressaltamos que o recebimento definitivo destes servi�os (ou bens) ocorrer� em at� 05 (cinco) dias, desde que n�o ocorram problemas t�cnicos ou diverg�ncias quanto �s especifica��es constantes do Termo de Refer�ncia correspondente ao Contrato supracitado.<br>
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Informamos que os servi�os constantes na Ordem de Servi�o descrita acima foram executados e avaliados quanto aos aspectos de qualidade, de acordo com os Crit�rios de Aceita��o previamente definidos pela Contratante.<br>
				<br>
				<br>
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Solicitamos que esse Termo seja enviado para elabora��o do Termo de Recebimento Definitivo.
				
				<br>
				<br>
				<br>
				<br>
				<br>
				<br>
				<center>

					<table border="0">
						<tr>
							<td colspan="3">
								<center>De Acordo</center>
								<br><br>
							</td>
						</tr>
						<tr>
							<td>
								<center>Contratante Requisitante</center>
							</td>
							<td width="80">&nbsp;</td>
							<td>
								<center>Contratante Fiscal T�cnico</center>
							</td>
						</tr>
						<tr>
							<td align="left"><br><center>___________________________________<br></center>
								Nome: '.$requisitante['usunome'].'
								<br>Mat: '.$requisitante['nu_matricula_siape'].'
							</td>
							<td width="80">&nbsp;</td>
							<td align="left"><br><center>___________________________________<br></center>
								Nome: '.$fiscal['usunome'].'
								<br>Mat: '.$fiscal['nu_matricula_siape'].'
							</td>
						</tr>
						<tr><td colpan="3"><br>&nbsp;<br></td></tr>
						<tr>
							<td>&nbsp;</td>
							<td align="center" ><br>
								<center>Contratada Preposto</center></td>
							<td>&nbsp;</td>
						</tr>
						<tr>
							<td>&nbsp;
							</td>
							<td align="left" >
								
								<br>
								<center>___________________________________<br></center>
								Nome: '.$preposto['usunome'].'
								<br>CPF: '.$preposto['usucpf'].'</td> 
								
							</td>
							<td>&nbsp;</td>
						</tr>
					</table>
				<br>
					
					Bras�lia, '.date('d').' de '.$data->mesTextual( (int)date('m') ).' de '.date('Y').'.';
					
					

				$html .= '</center>
				
			  ';
	
	$html .= '</td></tr></tbody></table>';
	
	$html .= '</body></html>';
	
	//insere historico
	if($os){
		//recupera ss
		$sql = "select scsid from fabrica.ordemservico where odsid = $os";
		$scsid = $db->pegaUm($sql);
		
		//inserir historico
		$tptid = 7;
		
		$sql = "INSERT INTO fabrica.termo(tptid, scsid, odsid, usucpf, data, corpo)
	    		VALUES ($tptid, 
	    				$scsid, 
	    				$os, 
	    				'".$_SESSION['usucpf']."', 
	    				'".date('Y-m-d H:i:s')."',
	    				'".$html."')";
		$db->executar($sql);
		$db->commit();
	}	
	
	return $html;
		
}

function SugestaoAplicacaoSancoes($os) {
	global $db;
	
	$data = new Data();
	
	$html = TermoCabecalho('Sugest�o para Aplica��o de San��es');
	
	$html .= '<tbody><tr><td width=100% valign="top">';
	
	$html .= '<center><b>Sugest�o para Aplica��o de San��es</b></center><br>';
	
	$html .= '<center>
				<label><input type="checkbox">&nbsp;Advert�ncia&nbsp;&nbsp;</label>
				<label><input type="checkbox">&nbsp;Suspens�o</label>
				<label><input type="checkbox">&nbsp;Declara��o de inidoneidade</label>
				<label><input type="checkbox">&nbsp;Multa</label>
			 </center>';
	
	$html .= '<br><br>&nbsp;&nbsp;(Justificativa para a aplica��o da penalidade proposta)
			  <br>
			  <br>
			  <br>
			  <br>
			  <br>
			  <br>
			  <br>
			  <br>
			  <br>
			  <br>
			  <br>
			  <br>
			  <br>
			  <br>
			  <br>
			  <br>
			  <br>
			  <br>
			  <br>
			  <br>';
	
	$html .= '<center>

					<table border="0" width="80%">
						<tr>
							<td align="left"><center>___________________________________<br>
								Contratante Fiscal T�cnico</center>
							</td>
							<td align="right">
								<center>___________________________________<br>
								Contratante Gestor do Contrato</center>
							</td>
						</tr>
						<tr>
							<td align="left"><br><br><center>___________________________________<br>
								Contratante Fiscal Administrativo</center>
							</td>
							<td>&nbsp;</td>
						</tr>
					</table>

					<br>
					<br>
					<br>
					
					Bras�lia, '.date('d').' de '.$data->mesTextual( (int)date('m') ).' de '.date('Y').'.
					
					<div style="page-break-before: always;"></div>

				</center>
				
			  ';
	
	$html .= '</td></tr></tbody></table>';
	
	$html .= '</body></html>';
	
	
	if($os){
		//recupera ss
		$sql = "select scsid from fabrica.ordemservico where odsid = $os";
		$scsid = $db->pegaUm($sql);
		
		//inserir historico
		$tptid = 9;
		
		$sql = "INSERT INTO fabrica.termo(tptid, scsid, odsid, usucpf, data, corpo)
	    		VALUES ($tptid, 
	    				$scsid, 
	    				$os, 
	    				'".$_SESSION['usucpf']."', 
	    				'".date('Y-m-d H:i:s')."',
	    				'".$html."')";
		$db->executar($sql);
		$db->commit();
	}
	
	
	return $html;
	
}

function TermoSolicitacaoServico($ss){
	
	global $db;
	
	$data = new Data();
	
	//$html = TermoCabecalho('Termo de Solicita��o de Servi�o', 'S.S: '.$ss.'/'.date('Y'));
	
	$sql = "SELECT
				ansgarantia as garantia,
				fas.ansid,
				su.usunome,
				su.usuemail,
				su.usufoneddd,
				su.usufonenum,
				fc.ctrnumero as contrato,
				ee.entnome,
				ss.scsid,
				fas.tpsid,
				to_char(ss.dataabertura, 'DD/MM/YYYY') as dataabertura,
				to_char(odsdtprevinicio, 'DD/MM/YYYY') as datainicio,
				to_char(odsdtprevtermino, 'DD/MM/YYYY') as datatermino,
				to_char(fas.ansprevinicio, 'DD/MM/YYYY') as previnicio,
				to_char(fas.ansprevtermino, 'DD/MM/YYYY') as prevtermino,
				os.odsqtdpfestimada,
				ss.odsidorigem,
				os.odsdetalhamento,
				dst.sitdsc as tecnologia,
				dsd.sidid,
				dsd.sidabrev, 
				dsd.siddescricao,
				un.unidsc,
				to_char(ss.scsprevatendimento, 'DD/MM/YYYY') as scsprevatendimento,
				ss.scsnecessidade,
				os.tosid
			FROM 
				fabrica.solicitacaoservico ss
			LEFT JOIN
				fabrica.analisesolicitacao fas ON fas.scsid = ss.scsid
			LEFT JOIN
				fabrica.ordemservico os ON os.scsid = ss.scsid
			LEFT JOIN
				fabrica.profissionalos po ON po.odsid = os.odsid
			LEFT JOIN
				seguranca.usuario su ON su.usucpf = ss.usucpfrequisitante
			LEFT JOIN
				fabrica.contrato fc ON fc.ctrid = fas.ctrid
			LEFT JOIN
				entidade.entidade ee ON ee.entid = fc.entidcontratado
			LEFT JOIN 
				public.unidade un ON un.unicod = su.unicod
			LEFT JOIN
				demandas.sistemadetalhe dsd ON dsd.sidid = ss.sidid AND dsd.sidstatus = 'A'
			LEFT JOIN
				demandas.sistematecnologia dst ON dst.sitid = dsd.sitid AND dst.sitstatus = 'A'
			WHERE 
				ss.scsid = {$ss}
            AND (os.tosid = ".TIPO_OS_GERAL." or os.tosid is null)";
	
	$solicitacao = $db->pegaLinha($sql);

	$sql2 = "select distinct ctrnumero, entnome from fabrica.contrato c
			inner join entidade.entidade e ON e.entid = c.entidcontratado
			inner join fabrica.analisesolicitacao a on a.ctrid = c.ctrid
			where ctrstatus = 'A' and ctrcontagem = false and scsid=".$solicitacao['scsid'];	
	$contrato = $db->pegaLinha($sql2);

	$html = TermoCabecalho('Termo de Solicita��o de Servi�o', $solicitacao['scsid'], '', $solicitacao['sidid']);
	
				$sql = "select 
							u.usunome,
							n.nu_matricula_siape
						from 
							fabrica.solicitacaoservico f 
						inner join 
							seguranca.usuario u ON u.usucpf = f.usucpfrequisitante
						inner join 
							siape.tb_siape_cadastro_servidor_ativos n ON n.nu_cpf = u.usucpf
						where  
							scsid = {$solicitacao['scsid']}";				
				
				$requisitante = $db->pegaLinha($sql); 

				$sql = "select 
							u.usunome,
							n.nu_cpf,
							n.nu_matricula_siape
						from 
							seguranca.perfilusuario s
						inner join 
							seguranca.usuario u ON u.usucpf = s.usucpf
						inner join 
							siape.tb_siape_cadastro_servidor_ativos n ON n.nu_cpf = u.usucpf
						where  
							s.pflcod = ".PERFIL_GESTOR_CONTRATO."";
				
				$gestor = $db->pegaLinha($sql);
				

				$sql = "select 
							u.usunome,
							s.usucpf
						from 
							seguranca.perfilusuario s
						left join 
							seguranca.usuario u ON u.usucpf = s.usucpf
						where  
							s.pflcod = ".PERFIL_PREPOSTO."";
				
				$preposto = $db->pegaLinha($sql);
	
	$html .= '<tbody><tr><td width=100% valign="top">
	
				
				<table cellSpacing="1" cellPadding=3 width="100%" border="1">
					<tr>
						<td>Data de abertura: '.$solicitacao['dataabertura'].'<td>
						<td>N� '.$ss.' / '.date('Y').'<td>
					</tr>
				</table>
				
				<br>
				
				<table class="tabela" cellSpacing="1" cellPadding=3 align="center" width="100%" border="1">
					<tr>
						<td colspan="6">Dado(s) do Requisitante</td>
					</tr>
					<tr>
						<td colspan="2">Nome</td>
						<td colspan="2">'.$solicitacao['usunome'].'</td>
						<td colspan="1">Setor</td>
						<td colspan="1">'.$solicitacao['unidsc'].'</td>
					</tr>
					<tr>
						<td colspan="2">Telefone(s)</td>
						<td colspan="4">('.$solicitacao['usufoneddd'].') '.$solicitacao['usufonenum'].'</td>
					</tr>
					<tr>
						<td colspan="2">E-mail</td>
						<td colspan="4">'.$solicitacao['usuemail'].'</td>
					</tr>
					<tr>
						<td colspan="2">Contrato</td>
						<td colspan="2">'.$contrato['ctrnumero'].'</td>
						<td colspan="1">Empresa Contratada</td>
						<td colspan="1">'.$contrato['entnome'].'</td>
					</tr>
					<tr>
						<td colspan="2">Sigla do Sistema</td>
						<td colspan="2">'.$solicitacao['sidabrev'].'</td>
						<td colspan="1">Nome do Sistema</td>
						<td colspan="1">'.$solicitacao['siddescricao'].'</td>
					</tr>
					<tr>
						<td colspan="2">In�cio Previsto</td>
						<td colspan="2">'.$solicitacao['previnicio'].'</td>
						<td colspan="1">T�rmino Previsto</td>
						<td colspan="1">'.$solicitacao['prevtermino'].'</td>
					</tr>
				</table>
				<br>
				<br>
				1.	Descri��o do Servi�o Solicitado
					<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$solicitacao['scsnecessidade'].'
				<br>
				<br>
				<br>
				<br>
				
				<center>

					<table border="0">
						<tr>
							<td>&nbsp;</td>
							<td width="100%"><br>
								<center>Contratante Requisitante</center></td>
							<td>&nbsp;</td>
						</tr>
						<tr>
							<td>&nbsp;
							</td>
							<td width="100%"><br><center>___________________________________<br></center>
								<center>Nome: '.$requisitante['usunome'].'</center>
								<center>Mat: '.$requisitante['nu_matricula_siape'].'</center>
							<td>&nbsp;</td>
						</tr>
					</table>
					
					<br>
					<br>
					<br>
					
					Bras�lia, '.date('d').' de '.$data->mesTextual( (int)date('m') ).' de '.date('Y').'.';
					
				$html .= '<div style="page-break-before:always;font-size:1;margin:0;border:0;"><span style="visibility: hidden;">-</span></div>';


				$html .= '</center>
				
				<br>
				<br>
				2.	Detalhe do Servi�o
					<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;';
	
				$html .= $solicitacao['odsdetalhamento'];
					
			    $html .= '
				<br>
				<br>
				
				<table class="tabela" cellSpacing="1" cellPadding=3 align="center" width="100%" border="1">
					<tr>
						<td colspan="2">Tecnologia adotada</td>
						<td colspan="4">'.$solicitacao['tecnologia'].'</td>
					</tr>
					<tr>
						<td colspan="2">Volume previsto em Pontos de Fun��o</td>
						<td colspan="4">'.$solicitacao['odsqtdpfestimada'].'</td>
					</tr>
					<tr>
						<td colspan="2">Servi�o em garantia?</td>
						<td>
							'.( $solicitacao['garantia'] == 'f' ? 'Sim (&nbsp;&nbsp;) N�o ( X )' : 'Sim ( X ) N�o (&nbsp;&nbsp;)' ).' 
						</td>
					</tr>
				</table>
				<br>
				<br>
				<table class="tabela" cellSpacing="1" cellPadding=3 align="center" width="100%" border="1">
					<tr>
						<td colspan="2">Tipo de Servi�o</td>
						<td colspan="4">';
	
	$sql = "SELECT 
				tpsid, 
				tpsdsc 
			FROM 
				fabrica.tiposervico 
			WHERE 
				tpsstatus='A'";
	$tiposervico = $db->carregar($sql);
	
	if($tiposervico[0]) {
		foreach($tiposervico as $tps) {
			$html .= '( '.(($tps['tpsid']==$solicitacao['tpsid'])?'X':'&nbsp;&nbsp;').' ) '.$tps['tpsdsc'].'<br/>';
		}
	}
	
	$html .=			'</td>
					</tr>
					<!-- <tr>
						<td colspan="2">Disciplinas contratadas</td>
						<td colspan="5"> --> ';
	/*					
	// $disciplina = carregar disciplinas
	$sql = "SELECT
				dspid,
				dspdsc
			FROM
				fabrica.disciplina
			WHERE
				dspstatus = 'A'";
	
	$disciplinas = $db->carregar($sql);
	
	// $fases = carregar fases
	$sql = "SELECT
				fasid,
				fasdsc
			FROM 
				fabrica.fase 
			WHERE 
				fasstatus = 'A'";
	
	$fases = $db->carregar($sql);						
	
	$qtd_colunas = (int)count($fases) + 1;
	
	$html_aux = '<table border="1" width="100%">
					<tr>
						<td colspan="'.$qtd_colunas.'" align="center"><b>Fase</b></td>
					</tr>
					<tr>
						<td align="center"><b>Disciplina</b></td>';

	// cabe�alho
	foreach ($fases as $fase) {
		$html_aux .= '	<td align="center"><b>'.$fase['fasdsc'].'</b></td>';
	}
	
	
	$html_aux .= '	</tr>';
	
	// conte�do
	foreach ($disciplinas as $disciplina) {
		$html_aux .= '<tr>
						<td>'.$disciplina['dspdsc'].'</td>';
		
		foreach ($fases as $fase) {
			if($solicitacao['ansid']){
				// query que verifica quais fases tem produtos prontos
				$sql = "SELECT count(sp.fdpid) as total 
						FROM fabrica.servicofaseproduto sp 
						INNER JOIN fabrica.fasedisciplinaproduto fdp ON fdp.fdpid = sp.fdpid 
						INNER JOIN fabrica.fasedisciplina fd ON fd.fsdid = fdp.fsdid 
						INNER JOIN fabrica.produto p ON p.prdid = fdp.prdid 
						WHERE sp.ansid = {$solicitacao['ansid']} 
						and fd.fasid = {$fase['fasid']} 
						and fd.dspid = {$disciplina['dspid']}
						and sp.tpeid = 1";
				
				$valor = $db->pegaLinha($sql);
			}// if tempor�rio. A pedidos do Henrique, foi colocado esse if aqui para que n�o ocorram erros caso os campos do banco estejam em branco
			
			if( $valor['total'] ){
				$html_aux .= '<td align="center">( X )</td>';
			}else{
				$html_aux .= '<td align="center">(&nbsp;&nbsp;&nbsp;&nbsp;)</td>';
			}// fim do if
			
		}// fim do foreach das fases
		
		$html_aux .= '</tr>';
		
	}// fim do foreach das disciplinas
	
	$html_aux .= '</table>'; 
	*/
	$html .=	   $html_aux.
					'	</td>
					</tr>
				</table>';
	/*			
	$html .= '	<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
				<br>
				3.	Artefatos / Produtos<br>
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;3.1	Artefatos fornecidos';

	$html .= '	<br>
				<br>
				<br>
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;3.2	Artefatos a serem gerados';
	
//pega tipo
	$tpeid = 1;
	if($tpeid) $where = "WHERE tpeid = $tpeid";
	$sql = "SELECT tpeid, tpedsc FROM fabrica.tipoexecucao $where ORDER BY 1";
	$tipo = $db->carregar($sql);
	
	if($tipo){
		
		$idtable = true;;
		
		if($idtable) $html .= '<table class=tabela bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 width="100%" >';
		
		for($t=0;$t<=count($tipo)-1;$t++){
			
			$tpeid = $tipo[$t]['tpeid'];
			$tpedsc = $tipo[$t]['tpedsc'];
	
			//pega disciplinas
			$sql = "SELECT distinct d.dspid, d.dspdsc
					FROM fabrica.servicofaseproduto sp 
					INNER JOIN fabrica.fasedisciplinaproduto fdp ON fdp.fdpid = sp.fdpid
					INNER JOIN fabrica.fasedisciplina fd ON fd.fsdid = fdp.fsdid
					INNER JOIN fabrica.disciplina d ON d.dspid = fd.dspid
					WHERE sp.ansid = {$solicitacao['ansid']} 
					AND sp.tpeid = {$tpeid}
					order by 1";
			$disciplina = $db->carregar($sql);
			
			$txtTd = '';
			
			if($disciplina){
				
				for($j=0;$j<=count($disciplina)-1;$j++){
					
					$dspid = $disciplina[$j]['dspid'];
					
					$txtTd .= '<span style="padding-left:40px"><b>'.trim($disciplina[$j]['dspdsc']).'</b></span><br>';
				
					//pega fases
					$sql = "SELECT distinct f.fasid, f.fasdsc 
							FROM fabrica.servicofaseproduto sp 
							INNER JOIN fabrica.fasedisciplinaproduto fdp ON fdp.fdpid = sp.fdpid
							INNER JOIN fabrica.fasedisciplina fd ON fd.fsdid = fdp.fsdid
							INNER JOIN fabrica.fase f ON f.fasid = fd.fasid
							WHERE sp.ansid = {$solicitacao['ansid']}
							AND sp.tpeid = {$tpeid}
							AND fd.dspid = {$dspid}
							ORDER BY 1";
					$fase = $db->carregar($sql);
				
					if($fase) {
						
						for($i=0;$i<=count($fase)-1;$i++){
							
							$fasid = $fase[$i]['fasid'];
							
							$sql = "SELECT p.prddsc 
									FROM fabrica.servicofaseproduto sp 
									INNER JOIN fabrica.fasedisciplinaproduto fdp ON fdp.fdpid = sp.fdpid
									INNER JOIN fabrica.fasedisciplina fd ON fd.fsdid = fdp.fsdid
									INNER JOIN fabrica.produto p ON p.prdid = fdp.prdid
									WHERE sp.ansid = {$solicitacao['ansid']}
									and sp.tpeid = {$tpeid} 
									and fd.dspid = {$dspid}
									and fd.fasid = {$fasid}
									ORDER BY 1";
							$produto = $db->carregarColuna($sql);
					
							if($produto){
								$txtTd .= '<span style="padding-left:60px"><b> - '.$fase[$i]['fasdsc'].'</b></span><br> <div style="padding-left:80px"> - ' . implode(";<br> - ", $produto) . ';</div>';
							}
							
						}
						
					}
							
				}//fecha for disciplina		
				
			}
			else{
					$txtTd = "N/A";
			}
			
			
			$html .= '
					<tr>
						<td width="100%">
						    <br>'.$txtTd.'<br>
						</td>
					</tr>
			';
		
		}
		
		if($idtable) $html .= '</table>';
				
	}//fim

	$html .= '  <br>
				<br>
				4.	Anexos';
	
	$sql = "SELECT 
				arqdescricao||' - '||arqnome||'.'||arqextensao as anexo
			FROM 
				fabrica.anexosolicitacao an 
			LEFT JOIN 
				fabrica.tipoanexosolicitacao tp ON an.tasid=tp.tasid 
			LEFT JOIN 
				public.arquivo ar ON ar.arqid=an.arqid 
			LEFT JOIN 
				seguranca.usuario us ON us.usucpf=ar.usucpf 
			LEFT JOIN
				fabrica.ordemservico fos ON fos.scsid = an.scsid
			WHERE 
				an.scsid = {$ss}
				AND ansstatus='A'";
	
	$anexos = $db->carregar($sql);
	
	if($anexos[0]){
		foreach($anexos as $anexo){
			$html .= '<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$anexo['anexo'];
		}
	}else{
		$html .= '<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;N�o h� anexos.';
	}
	*/			
	$html .= '<!--4.	Cronograma de Execu��o<br>
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$solicitacao['previnicio'].' at� '.$solicitacao['prevtermino'].'
				<br>
				<br>
				<br>-->
				
				<br>
				<br>
				<br>

				<center>

					<table border="0">
						<tr>
							<td colspan="3">
								<center>De Acordo</center>
								<br><br>
							</td>
						</tr>
						<tr>
							<td>
								<center>Contratante Gestor do Contrato</center>
							</td>
							<td width="80">&nbsp;</td>
							<td>
								<center>Contratada Preposto</center>
							</td>
						</tr>
						<tr>
							<td><br><center>___________________________________<br></center>
								Nome: '.$gestor['usunome'].'
								<br>Mat: '.$gestor['nu_matricula_siape'].'
							</td>
							<td width="80">&nbsp;</td>
							<td><br>
								<center>___________________________________<br></center>
								Nome: '.$preposto['usunome'].'
								<br>CPF: '.$preposto['usucpf'].'</td> 
							</td>
						</tr>
					</table>
					
					<br>
					<br>
					
					Bras�lia, '.date('d').' de '.$data->mesTextual( (int)date('m') ).' de '.date('Y').'.
					
					

				</center>
				
				
			  ';
	
	
	$html .= '</td></tr></tbody></table>';
	
	$html .= '</body></html>';
	
	
	if($ss){
		//inserir historico
		$tptid = 8;
		
		$sql = "INSERT INTO fabrica.termo(tptid, scsid, odsid, usucpf, data, corpo)
	    		VALUES ($tptid, 
	    				$ss, 
	    				null, 
	    				'".$_SESSION['usucpf']."', 
	    				'".date('Y-m-d H:i:s')."',
	    				'".addslashes($html)."')";
		$db->executar($sql);
		$db->commit();
	}
	
	return $html;
	
}

/**
 * Fun��o que retorna todas as Ordens de Servido relacionadas a Solicita��o de Servi�o
 * 
 * @param integer
 * @return Array
 * @author Rodrigo Pereira de Souza Silva
 */
function buscaOSSolicitacaoServico($scsid = 0) {
	global $db;
	
	$scsid = (int)$scsid;
	
	$sql = "SELECT 
				odsid 
			FROM 
				fabrica.ordemservico 
			WHERE 
				scsid = {$scsid}
			order by odsid";
	$os = $db->carregarColuna($sql);
	
	return $os;
	
}

?>