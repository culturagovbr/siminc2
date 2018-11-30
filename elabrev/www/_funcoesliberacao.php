<?php

function cabecalhoLiberacao($lbrid){
	global $db;
	
	if($lbrid){
		
		$sql = "SELECT 
				  	li.libnumeroliberacao||'/'||to_char(li.libdataliberacao, 'YYYY') as liberacao,
				  	li.libnumprocesso,
				  	li.libdescricao,
				  	li.libvalortotalprevisto,
					to_char(li.libdataprevinicio, 'DD/MM/YYYY') as datainicio,
					CASE WHEN ti.tcddsc is not null THEN
						ti.tcddsc
					ELSE
						'-'
					END as tipocdo,
					CASE WHEN tc.tpcdsc is not null THEN
						tc.tpcdsc
					ELSE
						'-'
					END as tipocontrato
					
				FROM 
				  	elabrev.liberacao li
				LEFT JOIN elabrev.tipocdo ti on ti.tcdid = li.tcdid
				LEFT JOIN elabrev.tipocontratacao tc on tc.tpcid = li.tpcid
				WHERE
					li.lbrstatus = 'A'
					and li.lbrid = ".$lbrid;
		
		$dados = $db->pegaLinha( $sql );
		
		?>
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloDireita" style="width: 30.4%">Nº da Solicitação:</td>
				<td><? echo $dados['liberacao']; ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Nº do Processo:</td>
				<td><? echo $dados['libnumprocesso']; ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Tipo de CDO:</td>
				<td><? echo $dados['tipocdo']; ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Modalidade de Contratação:</td>
				<td><? echo $dados['tipocontrato']; ?></td>
			</tr>
			
			<tr>
				<td class="SubTituloDireita">Data Início:</td>
				<td><? echo $dados['datainicio']; ?></td>
			</tr>			
			<tr>
				<td class="SubTituloDireita">Unidades detalhadas:</td>
				<td>
				<? 
				$sql = "SELECT ent.entid, ent.entnome FROM elabrev.liberacaodetalhe lid 
						LEFT JOIN entidade.entidade ent ON ent.entid=lid.entid 
						WHERE lbrid=".$lbrid." AND lidstatus='A'";
				$entidadesdet = $db->carregar($sql);
				if($entidadesdet[0]) {
					foreach($entidadesdet as $en) {
						$entd[] = $en['entnome'];
						$ents[] = $en['entid'];
					}
					echo implode(", ",$entd);
				} else {
					echo "Nenhuma unidades detalhada";
				}
				?>
				</td>
			</tr>
			</table>		
		<? 
	}
	
}

/**
 * Caso o documento não estaja criado cria um novo
 *
 * @param string $lbrid
 * @return integer
 */
function criarDocumento( $lbrid ) {
	global $db;

	$docid = pegarDocid($lbrid);

	if( ! $docid ) {
		// recupera o tipo do documento
		$tpdid = 56;

		// descrição do documento
		$docdsc = "Liberação Orçamentária - n°" . $lbrid;

		// cria documento do WORKFLOW
		$docid = wf_cadastrarDocumento( $tpdid, $docdsc );

		// atualiza o plano de trabalho
		$sql = "UPDATE
					elabrev.liberacao
				SET 
					docid = ".$docid." 
				WHERE
					lbrid = ".$lbrid;

		$db->executar( $sql );
		$db->commit();
	}

	return $docid;
}

/**
 * Pega o id do documento do plano de trabalho
 *
 * @param integer $lbrid
 * @return integer
 */
function pegarDocid($tcpid) {
	global $db;

	if($tcpid){
		$sql = "Select	docid
				From monitora.termocooperacao
				Where tcpid = $tcpid
		";
		return $db->pegaUm($sql);
	}
	return false;
}

/**
 * Pega o estado atual do workflow
 *
 * @param integer $lbrid
 * @return integer
 */
function pegarEstadoAtual($tcpid, $retornarDescricao = false) {
	global $db;

	$docid = pegarDocid($tcpid);

	if ($docid) {
        $sql = <<<DML
SELECT ed.esdid,
       ed.esddsc
  FROM workflow.documento d
    INNER JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
  WHERE d.docid = {$docid}
DML;
        $esddoc = $db->carregar($sql);
        if ($esddoc) {
            if (!$retornarDescricao) {
                return (integer)$esddoc[0]['esdid'];
            }
            return array((int)$esddoc[0]['esdid'], $esddoc[0]['esddsc']);
        }
	}
	return false;
}

function disabled($categoria = 'geral') {
	global $db;
	$retorno = '';
	
	if( empty($categoria) ) $categoria = 'geral';
	if(!possuiPermissao($categoria)) {
		$retorno = 'disabled="disabled"';
	}
	return $retorno;
}

function possuiPermissao($categoria = 'geral') {

	include_once APPRAIZ . "elabrev/www/permissoes_perfil.php";

	global $db;
	if( empty($categoria) ) $categoria = 'geral';
	// deve-se depois modificar o método 'pegaPerfil'
	$sql = "SELECT
				pu.pflcod
			FROM 
				seguranca.perfil AS p 
			LEFT JOIN 
				seguranca.perfilusuario AS pu ON pu.pflcod = p.pflcod
			WHERE 
				p.sisid = '".$_SESSION['sisid']."'
			  	AND pu.usucpf = '".$_SESSION['usucpf']."'";
	$pflcod = $db->carregarColuna($sql);
	$retorno = false;

	if($pflcod) {
		foreach($pflcod as $perfil){
			if($perfil == SUPER_USUARIO) {
				$retorno = true;
				break;
			} else {
				if(!$retorno){
					$retorno = permissoesPerfil($perfil, $_REQUEST['modulo'], $categoria);
				}
			}
		}
	}
	
	$retorno = ($retorno == NULL) ? false : $retorno;
	return $retorno;
}


function removerLiberacao($dados) {
	global $db;
	$sql = "UPDATE elabrev.liberacao SET lbrstatus='I' WHERE lbrid='".$dados['lbrid']."'";
	
	$db->executar($sql);
	
	$db->commit();
	
	echo "<script>
			window.location='?modulo=principal/liberacaoorcamentaria&acao=A';
		  </script>";
	
}

function espacamento($nivel) {
	for($i=0;$i<$nivel;$i++) {
		$espacamento .= "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
	}
	return $espacamento;
}

function detalharliberacaoorcamentaria($dados) {
	global $db;
	
	$sql = "SELECT lbs.lbsid as id, sba.sbacod as codigo, sba.sbadsc as descricao 
			FROM elabrev.liberacaosubacao lbs
			INNER JOIN elabrev.liberacaodetalhe lbd ON lbd.lbdid=lbs.lbdid 
			INNER JOIN elabrev.liberacao lib ON lib.lbrid=lbd.lbrid 
			INNER JOIN monitora.subacao sba ON sba.sbaid=lbs.sbaid
			WHERE lib.lbrid='".$_SESSION['elabrev_var']['lbrid']."' AND 
				  lbd.entid='".$dados['entid']."' AND 
				  lbs.lbsstatus='A'";
	
	$subacoes = $db->carregar($sql);
	
	$somenteLeitura = false;
	
	if($subacoes[0]) { // if subacoes
		$_HTML .= espacamento(1).(($somenteLeitura)?"<img src='../imagens/seta_baixo.gif' align='absmiddle'> <font color='#0066CC'>SubAção</font> <br/>":"<a style='cursor:pointer;' onclick=\"liberacaoOrcamentariaPopup('&tipo=subacao&entid=".$dados['entid']."');\"><img src='../imagens/seta_baixo.gif' align='absmiddle'> <font color='#0066CC'>SubAção</font></a> <br/>");
		foreach($subacoes as $subacao) { // foreach subacoes
			$_HTML .= espacamento(1)."<img src='../imagens/seta_filho.gif' align='absmiddle'> ".$subacao['codigo']." - ".$subacao['descricao']."<br/>";
			$sql = "SELECT lbp.lbpid as id, pli.plicod as codigo, pli.plidsc as descricao 
					FROM elabrev.liberacaopi lbp 
					INNER JOIN elabrev.liberacaosubacao lbs ON lbs.lbsid=lbp.lbsid
					INNER JOIN monitora.planointerno pli ON lbp.plicod=pli.plicod AND lbs.sbaid=pli.sbaid 
					WHERE lbs.lbsid='".$subacao['id']."' AND plistatus='A' AND lbp.lbpstatus='A'
					GROUP BY lbp.lbpid, pli.plicod, pli.plidsc";
			
			$pis = $db->carregar($sql);
			if($pis[0]) { // if pi
				$_HTML .= espacamento(2).(($somenteLeitura)?"<img src='../imagens/seta_baixo.gif' align='absmiddle'> <font color='#0066CC'>PI</font></a> <br/>":"<a style='cursor:pointer;' onclick=\"liberacaoOrcamentariaPopup('&tipo=pi&lbsid=".$subacao['id']."');\"><img src='../imagens/seta_baixo.gif' align='absmiddle'> <font color='#0066CC'>PI</font></a> <br/>");
				foreach($pis as $pi) { // foreach pi
					$_HTML .= espacamento(2)."<img src='../imagens/seta_filho.gif' align='absmiddle'> ".$pi['codigo']." - ".$pi['descricao']."<br/>";
					$sql = "SELECT lptid as id,
								   pli.pliptres as codigo
						    FROM elabrev.liberacaoptres pt
						    INNER JOIN monitora.ptres res ON res.ptrid=pt.ptrid
							INNER JOIN monitora.planointerno pli ON pli.pliptres=res.ptres 
							WHERE pli.plicod='".$pi['codigo']."' AND pt.lptstatus='A' AND pt.lbpid='".$pi['id']."'";
					
					$ptres = $db->carregar($sql);
					if($ptres[0]) { // if ptres
						$_HTML .= espacamento(3).(($somenteLeitura)?"<img src='../imagens/seta_baixo.gif' align='absmiddle'> <font color='#0066CC'>PTRES</font> <br/>":"<a style='cursor:pointer;' onclick=\"liberacaoOrcamentariaPopup('&tipo=ptres&lbpid=".$pi['id']."');\"><img src='../imagens/seta_baixo.gif' align='absmiddle'> <font color='#0066CC'>PTRES</font></a> <br/>");
						foreach($ptres as $res) { // foreach ptres
							$_HTML .= espacamento(3)."<img src='../imagens/seta_filho.gif' align='absmiddle'> ".$res['codigo']."<br/>";
							$sql = "SELECT fnt.lbfid as id,
										   fsf.foscod as codigo,
										   fsf.fosdsc as descricao 
									FROM elabrev.liberacaofonte fnt 
									INNER JOIN financeiro.fontesiafi fsf ON fsf.foscod=fnt.foscod 
									WHERE fnt.lptid='".$res['id']."' AND fnt.lbfstatus='A'";
							$fontedetalhada = $db->carregar($sql);
							
							if($fontedetalhada[0]) {
								//$_HTML .= espacamento(4).(($somenteLeitura)?"<img src='../imagens/seta_baixo.gif' align='absmiddle'> <font color='#0066CC'>Fonte Detalhada</font> <br/>":"<a style='cursor:pointer;' onclick=\"liberacaoOrcamentariaPopup('&tipo=fontedetalhada&lptid=".$res['id']."');\"><img src='../imagens/seta_baixo.gif' align='absmiddle'> <font color='#0066CC'>Fonte Detalhada</font></a> <br/>");
								foreach($fontedetalhada as $fonte) {
									//$_HTML .= espacamento(4)."<img src='../imagens/seta_filho.gif' align='absmiddle'> ".$fonte['codigo']." - ".$fonte['descricao']."<br/>";
									$sql = "SELECT lbn.lbnid as id,
												   nat.ndpcod as codigo,
												   nat.ndpdsc as descricao 
										    FROM elabrev.liberacaonatureza lbn 
											INNER JOIN public.naturezadespesa nat ON nat.ndpid=lbn.ndpid 
											WHERE lbn.lbfid='".$fonte['id']."' AND lbn.lbnstatus='A'";
									$naturezadespesa = $db->carregar($sql);
									
									if($naturezadespesa[0]) {
										//$_HTML .= espacamento(5).(($somenteLeitura)?"<img src='../imagens/seta_baixo.gif' align='absmiddle'> <font color='#0066CC'>Natureza da despesa</font> <br/>":"<a style='cursor:pointer;' onclick=\"liberacaoOrcamentariaPopup('&tipo=naturezadespesa&lbfid=".$fonte['id']."');\"><img src='../imagens/seta_baixo.gif' align='absmiddle'> <font color='#0066CC'>Natureza da despesa</font></a> <br/>");
										$_HTML .= espacamento(4).(($somenteLeitura)?"<img src='../imagens/seta_baixo.gif' align='absmiddle'> <font color='#0066CC'>Natureza da despesa</font> <br/>":"<a style='cursor:pointer;' onclick=\"liberacaoOrcamentariaPopup('&tipo=naturezadespesa&lbfid=".$fonte['id']."');\"><img src='../imagens/seta_baixo.gif' align='absmiddle'> <font color='#0066CC'>Natureza da despesa</font></a> <br/>");
										foreach($naturezadespesa as $nat) {
											//$_HTML .= espacamento(5)."<img src='../imagens/seta_filho.gif' align='absmiddle'> ".$nat['codigo']." - ".$nat['descricao']."<br/>";
											$_HTML .= espacamento(4)."<img src='../imagens/seta_filho.gif' align='absmiddle'> ".$nat['codigo']." - ".$nat['descricao']."<br/>";
										}
									} else { // else naturezadespesa
										//$_HTML .= espacamento(5).(($somenteLeitura)?"<img src='../imagens/seta_baixo.gif' align='absmiddle'> <font color='#0066CC'>Natureza da despesa</font> <br/>":"<a style='cursor:pointer;' onclick=\"liberacaoOrcamentariaPopup('&tipo=naturezadespesa&lbfid=".$fonte['id']."');\"><img src='../imagens/seta_baixo.gif' align='absmiddle'> <font color='#0066CC'>Natureza da despesa</font></a> <br/>");
										$_HTML .= espacamento(4).(($somenteLeitura)?"<img src='../imagens/seta_baixo.gif' align='absmiddle'> <font color='#0066CC'>Natureza da despesa</font> <br/>":"<a style='cursor:pointer;' onclick=\"liberacaoOrcamentariaPopup('&tipo=naturezadespesa&lbfid=".$fonte['id']."');\"><img src='../imagens/seta_baixo.gif' align='absmiddle'> <font color='#0066CC'>Natureza da despesa</font></a> <br/>");
									}
								} // foreach fontedetalhada
							} else { // else fontedetalhada
								//$_HTML .= espacamento(4).(($somenteLeitura)?"<img src='../imagens/seta_baixo.gif' align='absmiddle'> <font color='#0066CC'>Fonte Detalhada</font> <br/>":"<a style='cursor:pointer;' onclick=\"liberacaoOrcamentariaPopup('&tipo=fontedetalhada&lptid=".$res['id']."');\"><img src='../imagens/seta_baixo.gif' align='absmiddle'> <font color='#0066CC'>Fonte Detalhada</font></a> <br/>");
							}
						} // foreach ptres
					} else { // else ptres
						$_HTML .= espacamento(3).(($somenteLeitura)?"<img src='../imagens/seta_baixo.gif' align='absmiddle'> <font color='#0066CC'>PTRES</font> <br/>":"<a style='cursor:pointer;' onclick=\"liberacaoOrcamentariaPopup('&tipo=ptres&lbpid=".$pi['id']."');\"><img src='../imagens/seta_baixo.gif' align='absmiddle'> <font color='#0066CC'>PTRES</font></a> <br/>");
					}
				} // foreach pi
			} else { // else pi
				$_HTML .= espacamento(2).(($somenteLeitura)?"<img src='../imagens/seta_baixo.gif' align='absmiddle'> <font color='#0066CC'>PI</font> <br/>":"<a style='cursor:pointer;' onclick=\"liberacaoOrcamentariaPopup('&tipo=pi&lbsid=".$subacao['id']."');\"><img src='../imagens/seta_baixo.gif' align='absmiddle'> <font color='#0066CC'>PI</font></a> <br/>");
			}
		} // foreach subacoes
	} else { // else subacoes
		$_HTML .= espacamento(1).(($somenteLeitura)?"<img src='../imagens/seta_baixo.gif' align='absmiddle'> <font color='#0066CC'>SubAção</font> <br/>":"<a style='cursor:pointer;' onclick=\"liberacaoOrcamentariaPopup('&tipo=subacao&entid=".$dados['entid']."');\"><img src='../imagens/seta_baixo.gif' align='absmiddle'> <font color='#0066CC'>SubAção</font></a> <br/>");
	}
	
	echo $_HTML;
	exit;
}

function criarNovaLiberacao($dados = null) {
	global $db;
	
	$libnumeroliberacao = $db->pegaUm("SELECT MAX(libnumeroliberacao) FROM elabrev.liberacao WHERE lbrstatus='A'");
	
	$sql = "INSERT INTO elabrev.liberacao(usucpf, libnumeroliberacao, libdataliberacao, lbrstatus)
		    VALUES ('".$_SESSION['usucpf']."', '".(($libnumeroliberacao)?($libnumeroliberacao+1):"1")."', NOW(), 'A') RETURNING lbrid;";
	
	$_SESSION['elabrev_var']['lbrid'] = $db->pegaUm($sql);
	
	$db->commit();
	
	echo "<script>
			window.location='?modulo=principal/dadosGeraisCDO&acao=A';
		  </script>";
	
}

function inserirPILiberacao($dados) {
	global $db;
	
	$sql = "INSERT INTO elabrev.liberacaopi(
            lbsid, plicod, lbpstatus, lbpdata)
    		VALUES ('".$dados['lbsid']."', '".$dados['plicod']."', 'A', NOW());";
	
	$db->executar($sql);
	
	$db->commit();
}

function inserirPtresLiberacao($dados) {
	global $db;
	$sql = "INSERT INTO elabrev.liberacaoptres(
            lbpid, ptrid, lptstatus, lptdata)
    		VALUES ('".$dados['lbpid']."', '".$dados['ptrid']."', 'A', NOW());";
	
	$db->executar($sql);
	
	$db->commit();
	
}

function alterarLiberacao($dados) {
	if($dados['lbrid']) {
		$_SESSION['elabrev_var']['lbrid'] = $dados['lbrid'];

		echo "<script>
				window.location='?modulo=principal/liberacaoorcamentariadetalhes&acao=A';
			  </script>";
	} else {
		
		echo "<script>
				alert('Liberção não encontrada');
				window.location='?modulo=principal/liberacaoorcamentaria&acao=A';
			  </script>";
		
	}
}

function inserirFontedetalhadaLiberacao($dados) {
	global $db;
	$sql = "INSERT INTO elabrev.liberacaofonte(
            lptid, foscod, lbfstatus, lbfdata)
    		VALUES ('".$dados['lptid']."', '".$dados['foscod']."', 'A', NOW());";
	$db->executar($sql);
	$db->commit();
}

function inserirNaturezadespesaLiberacao($dados) {
	global $db;
	$sql = "INSERT INTO elabrev.liberacaonatureza(
            lbfid, ndpid, lbnstatus, lbndata)
    		VALUES ('".$dados['lbfid']."', '".$dados['ndpid']."', 'A', NOW());";
	$db->executar($sql);
	$db->commit();
}

function removerSubacaoLiberacao($dados) {
	global $db;
	
	$sql = "SELECT lbsid FROM elabrev.liberacaosubacao l
			INNER JOIN elabrev.liberacaodetalhe ld ON ld.lbdid=l.lbdid 
			WHERE ld.entid='".$dados['entid']."' AND ld.lbrid='".$_SESSION['elabrev_var']['lbrid']."' AND l.sbaid='".$dados['sbaid']."'";
	$lbsid = $db->pegaUm($sql);
	
	$sql = "UPDATE elabrev.liberacaosubacao SET lbsstatus='I' WHERE lbsid='".$lbsid."'";
	$db->executar($sql);
	$db->commit();
}
function inserirLiberacaoDetalhe($entid) {
	global $db;
	$sql = "INSERT INTO elabrev.liberacaodetalhe(lbrid, entid, lidvalor, lidobservacao, lidnotacredito, 
            lidstatus, liddata)
    		VALUES ('".$_SESSION['elabrev_var']['lbrid']."', '".$entid."', NULL, NULL, NULL, 'A', NOW()) RETURNING lbdid;";
	$lbdid = $db->pegaUm($sql);
	
	$db->commit();
	
	return $lbdid; 
	
}

function inserirSubacaoLiberacao($dados) {
	global $db;
	
	$sql = "SELECT lbdid FROM elabrev.liberacaodetalhe WHERE entid='".$dados['entid']."' AND lbrid='".$_SESSION['elabrev_var']['lbrid']."'";
	$lbdid = $db->pegaUm($sql);
	
	if(!$lbdid) {
		$lbdid = inserirLiberacaoDetalhe($dados['entid']);
	}
	
	$sql = "INSERT INTO elabrev.liberacaosubacao(
            lbdid, sbaid, lbsstatus, lbsdata)
    		VALUES ('".$lbdid."', '".$dados['sbaid']."', 'A', NOW());";
	$db->executar($sql);
	
	$db->commit();
}


function removerNaturezadespesaLiberacao($dados) {
	global $db;
	$sql = "UPDATE elabrev.liberacaonatureza SET lbnstatus='I' WHERE ndpid='".$dados['ndpid']."' AND lbfid='".$dados['lbfid']."'";
	$db->executar($sql);
	$db->commit();
}

function removerFontedetalhadaLiberacao($dados) {
	global $db;
	$sql = "UPDATE elabrev.liberacaofonte SET lbfstatus='I' WHERE foscod='".$dados['foscod']."' AND lptid='".$dados['lptid']."'";
	$db->executar($sql);
	$db->commit();
}
function removerPtresLiberacao($dados) {
	global $db;
	$sql = "UPDATE elabrev.liberacaoptres SET lptstatus='I' WHERE ptrid='".$dados['ptrid']."' AND lbpid='".$dados['lbpid']."'";
	$db->executar($sql);
	$db->commit();
}
function removerPILiberacao($dados) {
	global $db;
	$sql = "UPDATE elabrev.liberacaopi SET lbpstatus='I' WHERE plicod='".$dados['plicod']."' AND lbsid='".$dados['lbsid']."'";
	$db->executar($sql);
	$db->commit();
	
}
function gravarDadosLiberacao($dados) {
	global $db;
	
	$sql = "DELETE FROM elabrev.liberacaodados WHERE lbrid='".$_SESSION['elabrev_var']['lbrid']."'";
	$db->executar($sql);
	
	if($dados['valor']) {
		foreach($dados['valor'] as $lbachave => $valor) {
			if($valor || $dados['obs'][$lbachave] || $dados['nc'][$lbachave]) {
				$sql = "INSERT INTO elabrev.liberacaodados(
			            lbachave, lbavalor, lbaobservacao, lbanc, lbrid, lbapreempenho)
			    		VALUES ('".$lbachave."', ".(($valor)?"'".str_replace(array(".",","),array("","."),$valor)."'":"NULL").", ".(($dados['obs'][$lbachave])?"'".$dados['obs'][$lbachave]."'":"NULL").", ".(($dados['nc'][$lbachave])?"'".$dados['nc'][$lbachave]."'":"NULL").", '".$_SESSION['elabrev_var']['lbrid']."',".(($dados['empenho'][$lbachave])?"'".$dados['empenho'][$lbachave]."'":"NULL").");";
				
				$db->executar($sql);
			}
		}
	}
	
	$sql = "DELETE FROM elabrev.responsavel WHERE lbrid='".$_SESSION['elabrev_var']['lbrid']."'";
	$db->executar($sql);
	
	if($dados['responsavel']) {
		foreach($dados['responsavel'] as $fnlid => $valor) {
			if($valor) {
				$sql = "INSERT INTO elabrev.responsavel(
	            		lbrid, lrpid, rpsstatus, rpsdatainc)
				    	VALUES ('".$_SESSION['elabrev_var']['lbrid']."', '".$valor."', 'A', NOW());";
				$db->executar($sql);
			}
		}
	}
	
	$sql = "UPDATE elabrev.liberacao SET libdataliberacao=".(($dados['libdataliberacao'])?"'".formata_data_sql($dados['libdataliberacao'])."'":"NULL")." WHERE lbrid='".$_SESSION['elabrev_var']['lbrid']."'";
	$db->executar($sql);
	
	$db->commit();
	
	echo "<script>
			alert('Dados da liberação gravados com sucesso');
			window.location='?modulo=principal/liberacaoorcamentariaresumo&acao=A';
		  </script>";
}

function menuAbasLiberacao() {
	global $db;
	
	$estadoAtual = pegarEstadoAtual( $_SESSION['elabrev_var']['lbrid'] );
	/*
	$abas = array(0 => array("id" => 1, "descricao" => "Liberações",   				"link" => "/elabrev/elabrev.php?modulo=principal/liberacaoorcamentaria&acao=A"),
				  1 => array("id" => 2, "descricao" => "Dados Gerais do CDO",  		"link" => "/elabrev/elabrev.php?modulo=principal/dadosGeraisCDO&acao=A"),
				  2 => array("id" => 3, "descricao" => "Unidade Gestora",   		"link" => "/elabrev/elabrev.php?modulo=principal/liberacaoorcamentariadetalhes&acao=A"),
				  3 => array("id" => 4, "descricao" => "Certificação de Disponibilidade Orçamentária", 	"link" => "/elabrev/elabrev.php?modulo=principal/liberacaoorcamentariaresumo&acao=A"),
				  4 => array("id" => 5, "descricao" => "Solicitação de Crédito", 	"link" => "/elabrev/elabrev.php?modulo=principal/solicitacaoCredito&acao=A"),
				  5 => array("id" => 6, "descricao" => "Anexos",				 	"link" => "/elabrev/elabrev.php?modulo=principal/anexos&acao=A"),
				  6 => array("id" => 7, "descricao" => "Licitação/Resultado",		"link" => "/elabrev/elabrev.php?modulo=principal/licitacaoResultado&acao=A"));
				  
	$tcdid = $db->pegaUm("SELECT tcdid FROM elabrev.liberacao WHERE lbrid = {$_SESSION['elabrev_var']['lbrid']}");
	
	if($liberacao['lbrsituacao']=="E") {
		$abas = array(0 => array("id" => 1, "descricao" => "Liberações",   				"link" => "/elabrev/elabrev.php?modulo=principal/liberacaoorcamentaria&acao=A"),
					  1 => array("id" => 2, "descricao" => "Dados Gerais do CDO",  		"link" => "/elabrev/elabrev.php?modulo=principal/dadosGeraisCDO&acao=A"),
					  2 => array("id" => 3, "descricao" => "Unidade Gestora",   		"link" => "/elabrev/elabrev.php?modulo=principal/liberacaoorcamentariadetalhes&acao=A"),
					  3 => array("id" => 4, "descricao" => "Solicitação de Crédito", 	"link" => "/elabrev/elabrev.php?modulo=principal/solicitacaoCredito&acao=A"),
					  4 => array("id" => 5, "descricao" => "Anexos",				 	"link" => "/elabrev/elabrev.php?modulo=principal/anexos&acao=A"),
					  5 => array("id" => 6, "descricao" => "Licitação/Resultado",		"link" => "/elabrev/elabrev.php?modulo=principal/licitacaoResultado&acao=A"));
	}
	
	if( $estadoAtual != APROVADO || $tcdid != 5){
		$abas = array(0 => array("id" => 1, "descricao" => "Liberações",   				"link" => "/elabrev/elabrev.php?modulo=principal/liberacaoorcamentaria&acao=A"),
					  1 => array("id" => 2, "descricao" => "Dados Gerais do CDO",  		"link" => "/elabrev/elabrev.php?modulo=principal/dadosGeraisCDO&acao=A"),
					  2 => array("id" => 3, "descricao" => "Unidade Gestora",   		"link" => "/elabrev/elabrev.php?modulo=principal/liberacaoorcamentariadetalhes&acao=A"),
					  3 => array("id" => 4, "descricao" => "Certificação de Disponibilidade Orçamentária", 	"link" => "/elabrev/elabrev.php?modulo=principal/liberacaoorcamentariaresumo&acao=A"),
					  4 => array("id" => 5, "descricao" => "Anexos",				 	"link" => "/elabrev/elabrev.php?modulo=principal/anexos&acao=A"),
					  5 => array("id" => 6, "descricao" => "Licitação/Resultado",		"link" => "/elabrev/elabrev.php?modulo=principal/licitacaoResultado&acao=A"));
	} 
	*/

	$abas = array(0 => array("id" => 1, "descricao" => "Liberações",   				"link" => "/elabrev/elabrev.php?modulo=principal/liberacaoorcamentaria&acao=A"),
				  1 => array("id" => 2, "descricao" => "Dados Gerais do CDO",  		"link" => "/elabrev/elabrev.php?modulo=principal/dadosGeraisCDO&acao=A"),
				  2 => array("id" => 3, "descricao" => "Solicitação de Crédito", 	"link" => "/elabrev/elabrev.php?modulo=principal/solicitacaoCredito&acao=A"),
				  3 => array("id" => 4, "descricao" => "Anexos",				 	"link" => "/elabrev/elabrev.php?modulo=principal/anexos&acao=A"),
				  4 => array("id" => 5, "descricao" => "Licitação/Resultado",		"link" => "/elabrev/elabrev.php?modulo=principal/licitacaoResultado&acao=A"));
				  
	$tcdid = $db->pegaUm("SELECT tcdid FROM elabrev.liberacao WHERE lbrid = {$_SESSION['elabrev_var']['lbrid']}");
	
	if($liberacao['lbrsituacao']=="E") {
		$abas = array(0 => array("id" => 1, "descricao" => "Liberações",   				"link" => "/elabrev/elabrev.php?modulo=principal/liberacaoorcamentaria&acao=A"),
					  1 => array("id" => 2, "descricao" => "Dados Gerais do CDO",  		"link" => "/elabrev/elabrev.php?modulo=principal/dadosGeraisCDO&acao=A"),
					  2 => array("id" => 3, "descricao" => "Solicitação de Crédito", 	"link" => "/elabrev/elabrev.php?modulo=principal/solicitacaoCredito&acao=A"),
					  3 => array("id" => 4, "descricao" => "Anexos",				 	"link" => "/elabrev/elabrev.php?modulo=principal/anexos&acao=A"),
					  4 => array("id" => 5, "descricao" => "Licitação/Resultado",		"link" => "/elabrev/elabrev.php?modulo=principal/licitacaoResultado&acao=A"));
	}
	
	if( $estadoAtual != APROVADO || $tcdid != 5){
		$abas = array(0 => array("id" => 1, "descricao" => "Liberações",   				"link" => "/elabrev/elabrev.php?modulo=principal/liberacaoorcamentaria&acao=A"),
					  1 => array("id" => 2, "descricao" => "Dados Gerais do CDO",  		"link" => "/elabrev/elabrev.php?modulo=principal/dadosGeraisCDO&acao=A"),
					  2 => array("id" => 3, "descricao" => "Anexos",				 	"link" => "/elabrev/elabrev.php?modulo=principal/anexos&acao=A"),
					  3 => array("id" => 4, "descricao" => "Licitação/Resultado",		"link" => "/elabrev/elabrev.php?modulo=principal/licitacaoResultado&acao=A"));
	} 
	
	return $abas;
}

function alterarSituacaoLiberacao($dados) {
	global $db;
	if($dados) {
		foreach($dados as $key => $value) {
			if(substr($key,0,12) == "lbrsituacao_") {
				$dados['lbrid'][] = $value;
			}
		}
	}
	
	$sitat = array("E" => array("E" => true, "G" => true, "I" => true, "A" => true),
				   "G" => array("E" => true, "G" => true, "I" => true, "A" => true),
				   "I" => array("E" => false, "G" => false, "I" => true, "A" => true),
				   "A" => array("E" => true, "G" => true, "I" => true, "A" => true));
	
	
	
	if(is_array($dados['lbrid'])) {
		
		foreach($dados['lbrid'] as $lbrid) {
			$situacaoatual = $db->pegaUm("SELECT lbrsituacao FROM elabrev.liberacao WHERE lbrid='".$lbrid."'");
			if($sitat[$situacaoatual][$dados['lbrsituacao']]) {
				$sql = "UPDATE elabrev.liberacao SET lbrsituacao='".$dados['lbrsituacao']."' WHERE lbrid='".$lbrid."'";
				$db->executar($sql);
				$db->commit();
			}
		}
		
	} else {
		$situacaoatual = $db->pegaUm("SELECT lbrsituacao FROM elabrev.liberacao WHERE lbrid='".$lbrid."'");
		if($sitat[$situacaoatual][$dados['lbrsituacao']]) {
			$sql = "UPDATE elabrev.liberacao SET lbrsituacao='".$dados['lbrsituacao']."' WHERE lbrid='".$_SESSION['elabrev_var']['lbrid']."'";
			$db->executar($sql);
			$db->commit();
		}
	}
	
	
}

function possuiPerfil( $pflcods )
{
	global $db;
		
	if ( $db->testa_superuser() ) {
		
		return true;
		
	}else{
		
		if ( is_array( $pflcods ) )
		{
			$pflcods = array_map( "intval", $pflcods );
			$pflcods = array_unique( $pflcods );
		}
		else
		{
			$pflcods = array( (integer) $pflcods );
		}
		if ( count( $pflcods ) == 0 )
		{
			return false;
		}
		$sql = "
			select
				count(*)
			from seguranca.perfilusuario
			where
				usucpf = '" . $_SESSION['usucpf'] . "' and
				pflcod in ( " . implode( ",", $pflcods ) . " ) ";
		return $db->pegaUm( $sql ) > 0;
			
	}
}

function gravarDadosGeraisCDO( $dados ){
	global $db;
	extract($dados);
	
	$libvalortotalprevisto  	= $libvalortotalprevisto 	? 	retiraPontos($libvalortotalprevisto) 	: 'null';
	$libvalorexercicioatual 	= $libvalorexercicioatual 	? 	retiraPontos($libvalorexercicioatual) 	: 'null';
	$libvalorexercicioproximo 	= $libvalorexercicioproximo ?	retiraPontos($libvalorexercicioproximo) : 'null';
	$libdataprevinicio 			= $libdataprevinicio 		?	"'".formata_data_sql($libdataprevinicio)."'" 	: 'null';
	
	$sql = "UPDATE elabrev.liberacao SET
				  libnumprocesso = '$libnumprocesso',
				  libdescricao = '$libdescricao',
				  libvalortotalprevisto = $libvalortotalprevisto,
				  libdataprevinicio = $libdataprevinicio,
				  tcdid = '$tcdid',
				  tpcid = '$tpcid',
				  libdscoutras = ".($libdscoutras ? "'".$libdscoutras."'" : 'null').",
				  lbrnulibano = ".($lbrnulibano ? "'".$lbrnulibano."'" : 'null')."			 
			WHERE 
			  lbrid = ".$_SESSION['elabrev_var']['lbrid'];
	$db->executar( $sql );
	
	if($tcdid == "1"){
		$db->executar("DELETE FROM elabrev.liberacaocdo WHERE lbrid = ".$_SESSION['elabrev_var']['lbrid']);
	}
	
	$db->executar("DELETE FROM elabrev.liberacaoexercicio WHERE lbrid = ".$_SESSION['elabrev_var']['lbrid']);
	
	$sql = "INSERT INTO elabrev.liberacaoexercicio(lieexercicio, lievalorexercicio, lbrid) 
			VALUES ($anoatual, $libvalorexercicioatual, {$_SESSION['elabrev_var']['lbrid']});
			
			INSERT INTO elabrev.liberacaoexercicio(lieexercicio, lievalorexercicio, lbrid) 
			VALUES ($anoproximo, $libvalorexercicioproximo, {$_SESSION['elabrev_var']['lbrid']});";
	$db->executar( $sql );
	if( $db->commit() ){
		echo "<script>
					alert('Operação realizada com sucesso!');
					window.location='?modulo=principal/dadosGeraisCDO&acao=A';					
		  	  </script>";
		exit();
	} else {
		echo "<script>
					alert('Falha na Operação!');					
		  	  </script>";
	}
}

function retiraPontos($valor){
	$valor = str_replace(".","", $valor);
	$valor = str_replace(",",".", $valor);

	return $valor;
}

function enviarEmailSPO($lbrid){
	global $db;
	
	if ( $_SESSION['baselogin'] != "simec_desenvolvimento" && $_SESSION['baselogin'] != "simec_espelho_producao" ){
		
		$sql = "SELECT ent.entid, ent.entnome FROM elabrev.liberacaodetalhe lid 
				LEFT JOIN entidade.entidade ent ON ent.entid=lid.entid 
				WHERE lbrid='".$_SESSION['elabrev_var']['lbrid']."' AND lidstatus='A'";
		
		$entidadesdet = $db->carregar($sql);
		if($entidadesdet[0]) {
			foreach($entidadesdet as $en) {
				$entd[] = $en['entnome'];
				$ents[] = $en['entid'];
			}
			$unidade = implode(", ",$entd);
		} else {
			echo "Nenhuma unidades detalhada";
		}
		
		$libnumprocesso = $db->pegaUm( "SELECT libnumprocesso FROM elabrev.liberacao WHERE lbrid = ".$_SESSION['elabrev_var']['lbrid']." and lbrstatus = 'A'" );
		
		$remetente = array("nome"=>SIGLA_SISTEMA, "email"=>EMAIL_SISTEMA_NOREPLY);
		$strEmailTo = array($_SESSION['email_sistema']);
		$strAssunto = 'Solicitação de CDO encaminhada via Simec pela UG '. $unidade .' - Processo nº '.$libnumprocesso;
		$strMensagem = 'Solicitação de CDO encaminhada via Simec pela UG '.$unidade;
		
		return enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
	}
	return true;
}

function enviarEmailSPOAprovada(){
	global $db;
	
	if ( $_SESSION['baselogin'] != "simec_desenvolvimento" && $_SESSION['baselogin'] != "simec_espelho_producao" ){
		
		$sql = "SELECT ent.entid, ent.entnome FROM elabrev.liberacaodetalhe lid 
				LEFT JOIN entidade.entidade ent ON ent.entid=lid.entid 
				WHERE lbrid='".$_SESSION['elabrev_var']['lbrid']."' AND lidstatus='A'";
		
		$entidadesdet = $db->carregar($sql);
		if($entidadesdet[0]) {
			foreach($entidadesdet as $en) {
				$entd[] = $en['entnome'];
				$ents[] = $en['entid'];
			}
			$unidade = implode(", ",$entd);
		} else {
			echo "Nenhuma unidades detalhada";
		}
		
		$libnumprocesso = $db->pegaUm( "SELECT libnumprocesso FROM elabrev.liberacao WHERE lbrid = ".$_SESSION['elabrev_var']['lbrid']." and lbrstatus = 'A'" );
		
		$remetente = array("nome"=>SIGLA_SISTEMA, "email"=>EMAIL_SISTEMA_NOREPLY);
		$strEmailTo = array($_SESSION['email_sistema']);
		$strAssunto = 'Solicitação de CDO encaminhada via Simec pela UG '. $unidade .' - Processo nº '.$libnumprocesso;
		$strMensagem = 'CDO aprovado no Simec pela SPO';
		
		return enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
	}
	return true;
}

function gravarSolicitacaoCredito( $dados ){
	global $db;
	
	extract( $dados );
	
	$socvalorcontratacao	= $socvalorcontratacao ?	retiraPontos($socvalorcontratacao) 				: 'null';
	$socdatinilicitacao		= $socdatinilicitacao  ?	"'".formata_data_sql($socdatinilicitacao)."'" 	: 'null';
	$socdatfimlicitacao		= $socdatfimlicitacao  ?	"'".formata_data_sql($socdatfimlicitacao)."'" 	: 'null';
	
	if( empty($socid) ){
		$sql = "INSERT INTO elabrev.solicitacaocredito(lbrid, socvalorcontratacao, socdatinilicitacao, socdatfimlicitacao) 
				VALUES ({$_SESSION['elabrev_var']['lbrid']}, $socvalorcontratacao, $socdatinilicitacao, $socdatfimlicitacao)";
	} else {
		$sql = "UPDATE elabrev.solicitacaocredito SET 
				  	lbrid = {$_SESSION['elabrev_var']['lbrid']},
				  	socvalorcontratacao = $socvalorcontratacao,
				  	socdatinilicitacao = $socdatinilicitacao,
				  	socdatfimlicitacao = $socdatfimlicitacao 
				WHERE 
  					socid = $socid";
	}
	$db->executar( $sql );	
	if( $db->commit() ){
		$db->sucesso( 'principal/solicitacaoCredito' );
	} else {
		echo "<script>
					alert('Falha na Operação!');					
		  	  </script>";
	}
}

function carregarSubAcoes_Ajax( $acacod ){
	global $db;
	
	//prgcod || '.' || acacod || '.' || unicod || '.' || loccod
	
	$sql = "SELECT 
				sbacod||' - '||sbadsc as subacao, funcod, sfucod, prgcod, p.unicod, loccod, sa.sbaid, lc.lbcid, lc.foscod, lc.gndcod, lc.lbcvalor
			FROM monitora.pi_subacao sa
				inner join monitora.pi_subacaodotacao sad ON sad.sbaid = sa.sbaid
				inner join monitora.ptres p ON p.ptrid = sad.ptrid
				inner join monitora.pi_subacaounidade sau ON sau.sbaid = sa.sbaid
				left join elabrev.liberacaocredito lc on lc.acacod = p.acacod
			WHERE 
				ungcod is not null
				and p.acacod = '$acacod'
			GROUP BY funcod, sfucod, prgcod, p.unicod, loccod, sa.sbaid, sbacod, sbadsc, p.acacod, lc.lbcid, lc.foscod, lc.gndcod, lc.lbcvalor
			ORDER BY sbacod ";
	
	$arrSubacao = $db->carregar( $sql );
	$arrSubacao = $arrSubacao ? $arrSubacao : array();
	
	if($arrSubacao){
		$html = '
			<table id="tblform" class="listagem" width="80%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<thead>
				<tr>
					<td align="Center" class="title" width="10%"
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
						onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>SubAção</strong></td>
					<td align="Center" class="title" width="10%"
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
						onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Fonte</strong></td>
					<td align="Center" class="title" width="25%"
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
						onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>GND</strong></td>
					<td align="Center" class="title" width="10%"
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
						onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Valor Disponível</strong></td>
					<td align="Center" class="title" width="10%"
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
						onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Valor CDO</strong></td>
					<td align="Center" class="title" width="10%"
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
						onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Valor Contingenciado</strong></td>
				</tr>
			</thead>';
		$sqlGND = "SELECT 
						gndcod as codigo, 
					    gndcod||' - '||gnddsc as descricao 
					FROM public.gnd
					WHERE gndstatus = 'A'";
		
		$sqlFonte = "SELECT 
						foscod as codigo, 
						foscod||' - '||fosdsc as descricao
					FROM financeiro.fontesiafi
					 order by foscod";
		
		$arrGND = $db->carregar( $sqlGND );
		$arrGND = $arrGND ? $arrGND : array();
		$html .= '<input type="hidden" name="acacod" id="acacod" value="'.$acacod.'">';
		
		foreach ($arrSubacao as $key => $value) {
			$key % 2 ? $cor = "#dedfde" : $cor = "";
			
			if( $value['gndcod'] ){
				$lbcvalorcdo = carregaValorCDO( $value['gndcod'], $acacod, true );
				$valorcontigente = $value['lbcvalor'] ? (float) $value['lbcvalor'] - (float) $lbcvalorcdo : $value['lbcvalor'];
				$valorcontigente = number_format( $valorcontigente, 2, ',', '.' );
				$lbcvalorcdo = number_format( $lbcvalorcdo, 2, ',', '.' );
			}
			
			$value['lbcvalor'] = $value['lbcvalor'] ? number_format( $value['lbcvalor'], 2, ',', '.' ) : $value['lbcvalor'];
			
			$html .= '<input type="hidden" name="lbcid['.$value['sbaid'].']" id="lbcid" value="'.$value['lbcid'].'">';
				
			$html .= '<input type="hidden" name="subacao['.$value['sbaid'].']" id="subacao['.$value['sbaid'].']" value="'.$value['sbaid'].'">';
			$html.= '<tr bgcolor="'.$cor.'" onmouseout="this.bgColor=\''.$cor.'\';" onmouseover="this.bgColor=\'#ffffcc\';">
						<td>'.campo_texto('sbaid['.$value['sbaid'].']', 'N', 'N', $value['subacao'], 50, 250, '', '','','','','id="sbaid"', '', $value['subacao']).'</td>
						<td>'.$db->monta_combo('foscod['.$value['sbaid'].']', $sqlFonte, 'S','-- Selecione --','', '', '',250,'N','foscod', true, $value['foscod'], 'Fonte').'</td>';
				$html .= "<td><select  name='gndcod[".$value['sbaid']."]'  class='CampoEstilo' title='GND' style='width:200px;' onchange=\"carregaValorCDO(this.value, '".$value['sbaid']."')\" id=\"gndcod\">
					<option value=''>-- Selecione --</option>";
						foreach ($arrGND as $v) {
							$select = '';
							if( $v['codigo'] == $value['gndcod'] ) $select = 'selected="selected"';
							
							$html .= "<option value='".$v['codigo']."' $select>".$v['descricao']."</option";
						}
					$html .="</select></td>";
			$html .= '<td>'.campo_texto('lbcvalor['.$value['sbaid'].']', 'N', 'S', 'Valor Disponível', 15, 13, '[###.]###,##', '','','','','id="lbcvalor_'.$value['sbaid'].'"', '', $value['lbcvalor'], "this.value=mascaraglobal('[###.]###,##',this.value); calculaContigente(".$value['sbaid'].");").'</td>
						<td>'.campo_texto('lbcvalorcdo['.$value['sbaid'].']', 'N', 'N', 'Valor CDO', 15, 13, '', '','','','','id="lbcvalorcdo_'.$value['sbaid'].'"', '', $lbcvalorcdo).'</td>
						<td>'.campo_texto('valorcontigente['.$value['sbaid'].']', 'N', 'N', 'Valor Contingenciado', 15, 13, '', '','','','','id="valorcontigente_'.$value['sbaid'].'"', '', $valorcontigente).'</td>
					</tr>';
			
			//echo '<script>calculaContigente('.$value['sbaid'].')</script>';
			
		}
		$html.= '</table>';
	} else{
		$html = '<table width="100%" align="left" border="0" cellspacing="0" cellpadding="2" class="listagem">';
		$html .= '<tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>';
		$html .= '</table>';
	}
	//ver( simec_htmlentities( $html ),d );
	echo $html;	
}

function carregaValorCDO( $gnd, $acaid, $boAjax = false ){
	global $db;
	
	$sql = "SELECT
				aca.acaid,
				nat.gndcod as natureza,
			    sum(ld.lbavalor) as valorcdo
			FROM elabrev.liberacao   	lbr 
				INNER JOIN elabrev.liberacaodetalhe  lbd ON lbd.lbrid=lbr.lbrid 
			    INNER JOIN elabrev.liberacaodados    ld  ON ld.lbrid=lbr.lbrid 
			    INNER JOIN entidade.entidade         ent ON ent.entid=lbd.entid 
			    INNER JOIN public.unidade            uni ON uni.unicod=ent.entunicod
			    INNER JOIN elabrev.liberacaosubacao  lbs ON lbs.lbdid=lbd.lbdid 
			    INNER JOIN elabrev.liberacaopi       lbp ON lbp.lbsid=lbs.lbsid 
			    INNER JOIN monitora.planointerno     pli ON pli.plicod=lbp.plicod 
			    INNER JOIN elabrev.liberacaoptres    lpt ON lpt.lbpid=lbp.lbpid 
			    INNER JOIN monitora.ptres            res ON res.ptrid=lpt.ptrid
			    INNER JOIN monitora.acao             aca ON aca.acaid=res.acaid
			    INNER JOIN elabrev.liberacaofonte    lbf ON lbf.lptid=lpt.lptid 
			    INNER JOIN elabrev.liberacaonatureza lbn ON lbn.lbfid=lbf.lbfid 
			    INNER JOIN public.naturezadespesa    nat ON nat.ndpid=lbn.ndpid
			WHERE  
				lbr.lbrstatus='A' AND 
			    lbd.lidstatus='A' AND
			    lbs.lbsstatus='A' AND 
			    lbp.lbpstatus='A' AND 
			    lpt.lptstatus='A' AND 
			    lbf.lbfstatus='A' AND 
			    lbn.lbnstatus='A' AND
			    aca.acaid = '$acaid' AND
			    substring(nat.ndpcod, 1,1) = '$gnd'
			GROUP BY 
				aca.acaid,
			    aca.acacod,
			    nat.gndcod";
	$valorcdo = $db->pegaUm( $sql, 'valorcdo');
	
	if( $boAjax ) return $valorcdo;
	else echo number_format( $valorcdo, '2', ',', '.' );;
}

function gravarCreditoDisponivel( $dados ){
	global $db;
	
	if($dados['acaid']){
		extract($dados);
		
		for($i=0; $i<count($ungcod); $i++) {
			
			if($ungcod[$i] && $foncod[$i] && $gndcod[$i] && $ldivalorug[$i] && $ldivalorcontigenciado[$i]){
				$sql = "INSERT INTO 
							elabrev.liberacaocreditodisp(acaid, ungcod, foncod, gndcod, ldivalorug, ldivalorcontigenciado) 
						VALUES ($acaid, 
								'$ungcod[$i]', 
								'$foncod[$i]',
								'$gndcod[$i]',  
								".retiraPontos($ldivalorug[$i]).", 
								".retiraPontos($ldivalorcontigenciado[$i]).")";
				$db->executar( $sql );
			}
		}
		$db->commit();
		
		echo "<script>alert('Operação efetuada com sucesso!');</script>";
		echo "<script>location.href='elabrev.php?modulo=principal/gerenciarCredito&acao=A&acaid=".$acaid."';</script>";
		exit;
	}
	
	/*
	foreach ($dados['subacao'] as $sbaid => $v) {
		$lbcid 	  = $dados['lbcid'][$sbaid];
		$foscod   = $dados['foscod'][$sbaid];
		$gndcod   = $dados['gndcod'][$sbaid];
		$lbcvalor = retiraPontos($dados['lbcvalor'][$sbaid]);
		$acacod   = $dados['acacod'];
		
		if( empty($lbcid) ){
			$sql = "INSERT INTO elabrev.liberacaocredito(sbaid, foscod, gndcod, lbcvalor, acacod) 
					VALUES ($sbaid, '$foscod', '$gndcod', $lbcvalor, '$acacod')";
		} else {
			$sql = "UPDATE elabrev.liberacaocredito SET 
					  sbaid = $sbaid,
					  foscod = '$foscod',
					  gndcod = '$gndcod',
					  lbcvalor = $lbcvalor,
					  acacod = '$acacod' 
					WHERE 
					  lbcid = $lbcid";
		}
		
		$db->executar( $sql );
	}
	echo $db->commit();	
	*/
}


function cadDadosCdo($dados){
	global $db;
	
	if($dados){
		extract($dados);
		
		$sql = "select distinct ungcod from public.unidadegestora where ungcod='".$ug."'";
		$ungcod = $db->pegaUm($sql);
		if(!$ungcod){
			echo "ERRO: código da UG inexistente!";
			exit;
		}
		
		$sql = "select distinct acaid from monitora.acao where acacod='".$acao."'";
		$acaid = $db->pegaUm($sql);
		if(!$acaid){
			echo "ERRO: código da Ação inexistente!";
			exit;
		}

		$sql = "select distinct ptrid from monitora.ptres where ptres='".$ptres."'";
		$ptrid = $db->pegaUm($sql);
		if(!$ptrid){
			echo "ERRO: código do PTRES inexistente!";
			exit;
		}
		
		$sql = "select distinct sbaid from monitora.subacao where sbacod='".$subacao."'";
		$sbaid = $db->pegaUm($sql);
		if(!$sbaid){
			echo "ERRO: código da SubAção inexistente!";
			exit;
		}
		
		$sql = "select distinct pliid from monitora.planointerno where plicod='".$pi."'";
		$pliid = $db->pegaUm($sql);
		if(!$pliid){
			echo "ERRO: código do PI inexistente!";
			exit;
		}

		$sql = "select distinct ndpid from naturezadespesa where ndpcod='".$natureza."'";
		$ndpid = $db->pegaUm($sql);
		if(!$ndpid){
			echo "ERRO: código da Natureza inexistente!";
			exit;
		}
		
		
		
		$sql = "INSERT INTO elabrev.liberacaocdo(
            				lbrid, ungcod, acaid, ptrid, sbaid, 
            				pliid, ndpid, lcdvalor, lcddata, lcdstatus)
    				   VALUES  (".$_SESSION['elabrev_var']['lbrid'].", 
    				   			'".$ungcod."', 
    				   			".$acaid.", 
    				   			".$ptrid.", 
    				   			".$sbaid.", 
    				   			".$pliid.", 
    				   			".$ndpid.", 
    				   			".retiraPontos($valor).", 
    				   			now(),
    				   			'A')";
		
		$db->executar( $sql );
		$db->commit();
		
		echo "OK";
		exit;
		//$lista = listaDadosCdo();
		//echo $lista;
		//exit; 
	}
}

function listaDadosCdo(){
	global $db;
	
	$sql = "select  lcd.ungcod,
					aca.acacod,
					ptr.ptres,
					sba.sbacod,
					pli.plicod,		
					nde.ndpcod,			
					lcd.lcdvalor as valor,
					'<center><a href=\'javascript:excluirDadosCdo('||lcd.lcdid||');\'><img border=0 src=\'../imagens/excluir.gif\'></center>' as botao
					--'<center>'||to_char(lcddata::date, 'DD/MM/YYYY')||'</center>' as lcddata
			from elabrev.liberacaocdo lcd
			inner join public.unidadegestora ung on ung.ungcod = lcd.ungcod
			inner join monitora.acao aca on aca.acaid = lcd.acaid
			inner join monitora.ptres ptr on ptr.ptrid = lcd.ptrid
			inner join monitora.subacao sba on sba.sbaid = lcd.sbaid
			inner join monitora.planointerno pli on pli.pliid = lcd.pliid
			inner join public.naturezadespesa nde on nde.ndpid = lcd.ndpid
			where lcd.lbrid = ".$_SESSION['elabrev_var']['lbrid']."
			and lcd.lcdstatus = 'A'
			order by lcd.lcddata
			";
	$listacdo = $db->carregar($sql);
	
	//dbg($sql,1);
	//$cabecalho = array("UG","Ação","PTRES","SubAção","PI","Natureza","Valor","Ação");
	//$db->monta_lista_simples( $sql, $cabecalho, 100, 1, 'S', '', '');
	
	if($listacdo){
		$html  = '<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2">';
		$corcdo = '#FFFFFF';
		foreach($listacdo as $v){
			
			if($corcdo == "#F7F7F7") $corcdo = '#FFFFFF';
			else $corcdo = "#F7F7F7";
			$valorTotal = $valorTotal + $v['valor'];
			$html .= '<tr valign="bottom" bgcolor="'.$corcdo.'" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\''.$corcdo.'\';">
								<td width="90" style="height: 0; margin: 0; padding: 0;">'.$v['ungcod'].'</td>
								<td width="90" style="height: 0; margin: 0; padding: 0;">'.$v['acacod'].'</td>
								<td width="90" style="height: 0; margin: 0; padding: 0;">'.$v['ptres'].'</td>
								<td width="90" style="height: 0; margin: 0; padding: 0;">'.$v['sbacod'].'</td>
								<td width="90" style="height: 0; margin: 0; padding: 0;">'.$v['plicod'].'</td>
								<td width="90" style="height: 0; margin: 0; padding: 0;">'.$v['ndpcod'].'</td>
								<td width="374" align="right" style="height: 0; margin: 0; padding: 0;">'.number_format($v['valor'],2,",",".").'</td>
								<td width="67" align="center" style="height: 0; margin: 0; padding: 0;">'.$v['botao'].'</td>
						</tr>';
		}
		
		$html .= '<tr valign="bottom" bgcolor="#ffffcc" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\'#ffffcc\';">
								<td width="90" style="height: 0; margin: 0; padding: 0;">&nbsp;</td>
								<td width="90" style="height: 0; margin: 0; padding: 0;">&nbsp;</td>
								<td width="90" style="height: 0; margin: 0; padding: 0;">&nbsp;</td>
								<td width="90" style="height: 0; margin: 0; padding: 0;">&nbsp;</td>
								<td width="90" style="height: 0; margin: 0; padding: 0;">&nbsp;</td>
								<td width="90" align="right" style="height: 0; margin: 0; padding: 0;"><b>Total:</b></td>
								<td width="374" align="right" style="height: 0; margin: 0; padding: 0;"><b>'.number_format($valorTotal,2,",",".").'</b></td>
								<td width="67" align="center" style="height: 0; margin: 0; padding: 0;">&nbsp;</td>
						</tr>';
		
		$html .= '</table>';
		
		return $html;
		
	} 
}


function excluirDadosCdo($dados){
	global $db;
	
	if($dados['lcdid']){
		$sql = "UPDATE elabrev.liberacaocdo SET lcdstatus = 'I'	WHERE lcdid = ".$dados['lcdid'];
		$db->executar( $sql );
		$db->commit();
	}
	
	//$lista = listaDadosCdo();
	//echo $lista;
	//exit;
	
	echo "OK";
	exit;
		
}

function usuarioPossuiPermissaoUnidades( $unicod, $esquema = 'elabrev' ){
	global $db;
	$unicod = str_replace( "'", "\\'", $unicod );
	$cpf = $_SESSION['usucpf'];
	
	if( $db->testa_superuser() ){
		return true;
	}
	
	$sql = "select count(*) from " . $esquema . ".usuarioresponsabilidade where usucpf = '" . $cpf . "' and rpustatus = 'A' and unicod in ('" . implode("','", $unicod) . "')";
	$boUsu = $db->pegaUm( $sql );
	if( $boUsu > 0 ) return true;
	else return false;
}
/*
function enviar_email_a($docid){
 	
	global $db;
	
 	$sql = "SELECT
				usuemail
			FROM
				seguranca.usuario usu
			INNER JOIN elabrev.usuarioresponsabilidade rpu ON rpu.usucpf = usu.usucpf
			INNER JOIN monitora.termocooperacao tcp ON tcp.ungcodproponente = rpu.ungcod
			WHERE
				rpu.pflcod = ".PERFIL_REITOR." 
				AND docid = ".$docid."
				AND usustatus = 'A'
				AND rpu.rpustatus = 'A'"; 
 	
	//$email = $db->carregarColuna($sql);
	$email = "bruno.figueira@mev.gov.br";	
 	ver($email);
 		ver("A");
	$sql = "SELECT
				tcpid
			FROM
				monitora.termocooperacao 
			WHERE
				docid = ".$docid;
 	
 	$tcpid = $db->pegaUm($sql);
 	
	$remetente = array('nome'=>'Programação Orçamentária - Descetralização de Crédito', 'email'=>$_SESSION['email_sistema']);
	
	$assunto  = "O termo de Execução Descentralizado $tcpid foi cadastrado. Aguardando aprovação da reitoria.";
	
	$conteudo = "<p>O termo de Execução Descentralizado $tcpid foi cadastrado. Aguardando aprovação da reitoria.</p>";
	
	
	enviar_email($remetente, $email, $assunto, $conteudo, $cc, $cco );
}
function enviar_email_tec($docid){//retornar para correção
 	
	global $db;
	
 	$sql ="SELECT
					usuemail
		   FROM
					seguranca.usuario usu
    		INNER JOIN elabrev.usuarioresponsabilidade rpu ON rpu.usucpf = usu.usucpf
			INNER JOIN monitora.termocooperacao tcp ON tcp.ungcodproponente = rpu.ungcod
			WHERE
				rpu.pflcod = ".UO_EQUIPE_TECNICA."
				AND docid =".$docid."
				AND usustatus = 'A'
				AND rpu.rpustatus = 'A'"; 
 	
	//$email = $db->carregarColuna($sql);
	$email = "bruno.figueira@mev.gov.br";	
	ver($email);
		ver("tecnica");
 	$sql = "SELECT
				tcpid
			FROM
				monitora.termocooperacao 
			WHERE
				docid = ".$docid;
 	
 	$tcpid = $db->pegaUm($sql);
 	
	$remetente = array('nome'=>'Programação Orçamentária - Descetralização de Crédito', 'email'=>$_SESSION['email_sistema']);
	
	$assunto  = "O termo de execução descentralizado $tcpid necessita de ajustes.";
	
	$conteudo = "<p>O termo de execução descentralizado $tcpid necessita de ajustes.</p>";
	
	
	enviar_email($remetente, $email, $assunto, $conteudo, $cc, $cco );
}
function enviar_email_rei($docid){//retornar para aprovação reitor
 	
	global $db;
	
 	$sql ="SELECT
					usuemail
		   FROM
					seguranca.usuario usu
    		INNER JOIN elabrev.usuarioresponsabilidade rpu ON rpu.usucpf = usu.usucpf
			INNER JOIN monitora.termocooperacao tcp ON tcp.ungcodproponente = rpu.ungcod
			WHERE
				rpu.pflcod = ".PERFIL_REITOR." 
				AND docid =".$docid."
				AND usustatus = 'A'
				AND rpu.rpustatus = 'A'"; 
 	
	//$email = $db->carregarColuna($sql);
	$email = "bruno.figueira@mev.gov.br";	
	ver($email);
		ver("Reitor");
 	$sql = "SELECT
				tcpid
			FROM
				monitora.termocooperacao 
			WHERE
				docid = ".$docid;
 	
 	$tcpid = $db->pegaUm($sql);
 	
	$remetente = array('nome'=>'Programação Orçamentária - Descetralização de Crédito', 'email'=>$_SESSION['email_sistema']);
	
	$assunto  = "O termo de execução descentralizado $tcpid necessita de ajustes.";
	
	$conteudo = "<p>O termo de execução descentralizado $tcpid necessita de ajustes.</p>";
	
	
	enviar_email($remetente, $email, $assunto, $conteudo, $cc, $cco );
}
function enviar_email_sec($docid){
 	
	global $db;
	
 	$sql ="SELECT
					usuemail
		   FROM
					seguranca.usuario usu
    		INNER JOIN elabrev.usuarioresponsabilidade rpu ON rpu.usucpf = usu.usucpf
			INNER JOIN monitora.termocooperacao tcp ON tcp.ungcodproponente = rpu.ungcod
			WHERE
				rpu.pflcod = ".PERFIL_SECRETARIA." 	
				AND docid =".$docid."
				AND usustatus = 'A'
				AND rpu.rpustatus = 'A'"; 
 	$teste = $db->carregarColuna($sql);
 	ver($teste);
	//$email = $db->carregarColuna($sql);
	$email = "bruno.figueira@mev.gov.br";
	ver($email);
		ver("Secretaria");	
 	$sql = "SELECT
				tcpid
			FROM
				monitora.termocooperacao 
			WHERE
				docid = ".$docid;
 	
 	$tcpid = $db->pegaUm($sql);
 	
	$remetente = array('nome'=>'Programação Orçamentária - Descetralização de Crédito', 'email'=>$_SESSION['email_sistema']);
	
	$assunto  = "O termo de execução descentralizado $tcpid necessita de ajustes.";
	
	$conteudo = "<p>O termo de execução descentralizado $tcpid necessita de ajustes.</p>";
	
	
	enviar_email($remetente, $email, $assunto, $conteudo, $cc, $cco );
}
function enviar_email_dir($docid){
 	
	global $db;
	
 	$sql ="SELECT
					usuemail
		   FROM
					seguranca.usuario usu
    		INNER JOIN elabrev.usuarioresponsabilidade rpu ON rpu.usucpf = usu.usucpf
			INNER JOIN monitora.termocooperacao tcp ON tcp.ungcodproponente = rpu.ungcod
			WHERE
				rpu.pflcod = ".PERFIL_DIRETORIA." 	
				AND docid =".$docid."
				AND usustatus = 'A'
				AND rpu.rpustatus = 'A'"; 
 	
	//$email = $db->carregarColuna($sql);
	$email = "bruno.figueira@mev.gov.br";	
	ver($email);
		ver("Diretoria");
 	$sql = "SELECT
				tcpid
			FROM
				monitora.termocooperacao 
			WHERE
				docid = ".$docid;
 	
 	$tcpid = $db->pegaUm($sql);
 	
	$remetente = array('nome'=>'Programação Orçamentária - Descetralização de Crédito', 'email'=>$_SESSION['email_sistema']);
	
	$assunto  = "O termo de execução descentralizado $tcpid necessita de ajustes.";
	
	$conteudo = "<p>O termo de execução descentralizado $tcpid necessita de ajustes.</p>";
	
	
	enviar_email($remetente, $email, $assunto, $conteudo, $cc, $cco );
}
function enviar_email_cgso($docid){
 	
	global $db;
	
 	$sql ="SELECT
					usuemail
		   FROM
					seguranca.usuario usu
    		INNER JOIN elabrev.usuarioresponsabilidade rpu ON rpu.usucpf = usu.usucpf
			INNER JOIN monitora.termocooperacao tcp ON tcp.ungcodproponente = rpu.ungcod
			WHERE
				rpu.pflcod = ".PERFIL_CGSO." 
				AND docid =".$docid."
				AND usustatus = 'A'
				AND rpu.rpustatus = 'A'"; 
 	
	//$email = $db->carregarColuna($sql);
	$email = "bruno.figueira@mev.gov.br";
	ver($email);
		ver("CGSO");	
 	$sql = "SELECT
				tcpid
			FROM
				monitora.termocooperacao 
			WHERE
				docid = ".$docid;
 	
 	$tcpid = $db->pegaUm($sql);
 	
	$remetente = array('nome'=>'Programação Orçamentária - Descetralização de Crédito', 'email'=>$_SESSION['email_sistema']);
	
	$assunto  = "O termo de execução descentralizado $tcpid necessita de ajustes.";
	
	$conteudo = "<p>O termo de execução descentralizado $tcpid necessita de ajustes.</p>";
	
	
	enviar_email($remetente, $email, $assunto, $conteudo, $cc, $cco );
}
function enviar_email_c($docid){
 	
	global $db;
	
 	$sql = "SELECT
				usuemail
			FROM
				seguranca.usuario usu
			INNER JOIN elabrev.usuarioresponsabilidade rpu ON rpu.usucpf = usu.usucpf
			INNER JOIN monitora.termocooperacao tcp ON tcp.ungcodproponente = rpu.ungcod
			WHERE
				rpu.pflcod = ".PERFIL_SECRETARIA." 	
				AND docid = ".$docid."
 				AND usustatus = 'A'
				AND rpu.rpustatus = 'A'"; 
 	
	//$email = $db->carregarColuna($sql);
	$email = "bruno.figueira@mev.gov.br";	
		ver($email);	
		ver("C");
 	$sql = "SELECT
				tcpid
			FROM
				monitora.termocooperacao 
			WHERE
				docid = ".$docid;
 	
 	$tcpid = $db->pegaUm($sql);
 	
	$remetente = array('nome'=>'Programação Orçamentária - Descetralização de Crédito', 'email'=>$_SESSION['email_sistema']);
	
	$assunto  = "O termo de Execução Descentralizado $tcpid foi aprovado pela reitoria. Aguarda posicionamento da secretaria.";
	
	$conteudo = "<p>O termo de Execução Descentralizado $tcpid foi aprovado pela reitoria. Aguarda posicionamento da secretaria.</p>";
	
	
	enviar_email($remetente, $email, $assunto, $conteudo, $cc, $cco );
}
function enviar_email_d($docid){
 	
	global $db;
	
 	$sql = "SELECT
				usuemail
			FROM
				seguranca.usuario usu
			INNER JOIN elabrev.usuarioresponsabilidade rpu ON rpu.usucpf = usu.usucpf
			INNER JOIN monitora.termocooperacao tcp ON tcp.ungcodproponente = rpu.ungcod
			WHERE
				rpu.pflcod = ".PERFIL_COORDENADOR_SEC." 
				AND docid = ".$docid."
 				AND usustatus = 'A'
				AND rpu.rpustatus = 'A'";
 	
	//$email = $db->carregarColuna($sql);
	$email = "bruno.figueira@mev.gov.br";
	ver($email);
	ver("D");
 	$sql = "SELECT
				tcpid
			FROM
				monitora.termocooperacao 
			WHERE
				docid = ".$docid;
 	
 	$tcpid = $db->pegaUm($sql);
 	
	$remetente = array('nome'=>'Programação Orçamentária - Descetralização de Crédito', 'email'=>$_SESSION['email_sistema']);
	
	$assunto  = "O termo de execução descentralizado $tcpid aguarda análise e parecer da coordenação.";
	
	$conteudo = "<p>O termo de execução descentralizado $tcpid aguarda análise e parecer da coordenação.</p>";
	
	
	enviar_email($remetente, $email, $assunto, $conteudo, $cc, $cco );
}
function enviar_email_e($docid){
 	
	global $db;
	
 	$sql = "SELECT
				usuemail
			FROM
				seguranca.usuario usu
			INNER JOIN elabrev.usuarioresponsabilidade rpu ON rpu.usucpf = usu.usucpf
			INNER JOIN monitora.termocooperacao tcp ON tcp.ungcodproponente = rpu.ungcod
			WHERE
				rpu.pflcod = ".PERFIL_DIRETORIA." 
				AND docid = ".$docid."
 				AND usustatus = 'A'
				AND rpu.rpustatus = 'A'";
 	
	//$email = $db->carregarColuna($sql);
	$email = "bruno.figueira@mev.gov.br";
	ver($email);
	ver("E");	
 	$sql = "SELECT
				tcpid
			FROM
				monitora.termocooperacao 
			WHERE
				docid = ".$docid;
 	
 	$tcpid = $db->pegaUm($sql);
 	
	$remetente = array('nome'=>'Programação Orçamentária - Descetralização de Crédito', 'email'=>$_SESSION['email_sistema']);
	
	$assunto  = "O termo de Execução Descentralizado $tcpid foi aprovado. Aguardando aprovação pela diretoria.";
	
	$conteudo = "<p>O termo de Execução Descentralizado $tcpid foi aprovado. Aguardando aprovação pela diretoria.</p>";
	
	
	enviar_email($remetente, $email, $assunto, $conteudo, $cc, $cco );
}
function enviar_email_f($docid){
 	
	global $db;
	
 	$sql = "SELECT
				usuemail
			FROM
				seguranca.usuario usu
			INNER JOIN elabrev.usuarioresponsabilidade rpu ON rpu.usucpf = usu.usucpf
			INNER JOIN monitora.termocooperacao tcp ON tcp.ungcodproponente = rpu.ungcod
			WHERE
				rpu.pflcod = ".UO_EQUIPE_TECNICA."
				AND docid = ".$docid."
 				AND usustatus = 'A'
				AND rpu.rpustatus = 'A'";
 	
	//$email = $db->carregarColuna($sql);
	$email = "bruno.figueira@mev.gov.br";
	ver($email);
	ver("F");
 	$sql = "SELECT
				tcpid
			FROM
				monitora.termocooperacao 
			WHERE
				docid = ".$docid;
 	
 	$tcpid = $db->pegaUm($sql);
 	
	$remetente = array('nome'=>'Programação Orçamentária - Descetralização de Crédito', 'email'=>$_SESSION['email_sistema']);
	
	$assunto  = "O termo de execução descentralizado $tcpid foi devolvido para ajustes (em diligência). Aguardando posicionamento da unidade técnica.";
	
	$conteudo = "<p>O termo de execução descentralizado $tcpid foi devolvido para ajustes (em diligência). Aguardando posicionamento da unidade técnica.</p>";
	
	
	enviar_email($remetente, $email, $assunto, $conteudo, $cc, $cco );
}
function enviar_email_g($docid){
 	
	global $db;
	
 	$sql = "SELECT
				usuemail
			FROM
				seguranca.usuario usu
			INNER JOIN elabrev.usuarioresponsabilidade rpu ON rpu.usucpf = usu.usucpf
			INNER JOIN monitora.termocooperacao tcp ON tcp.ungcodproponente = rpu.ungcod
			WHERE
				rpu.pflcod = ".PERFIL_CGSO." 
				AND docid =".$docid."
				AND usustatus = 'A'
				AND rpu.rpustatus = 'A'"; 
 	
	//$email = $db->carregarColuna($sql);
	$email = "bruno.figueira@mev.gov.br";
	ver($email);
	ver("G");	
 	$sql = "SELECT
				tcpid
			FROM
				monitora.termocooperacao 
			WHERE
				docid = ".$docid;
 	
 	$tcpid = $db->pegaUm($sql);
 	
	$remetente = array('nome'=>'Programação Orçamentária - Descetralização de Crédito', 'email'=>$_SESSION['email_sistema']);
	
	$assunto  = "O termo de execução descentralizado $tcpid foi aprovado pela secretaria. Aguardando posicionamento da SPO para descentralização.";
	
	$conteudo = "<p>O termo de execução descentralizado $tcpid foi aprovado pela secretaria. Aguardando posicionamento da SPO para descentralização.</p>";
	
	
	enviar_email($remetente, $email, $assunto, $conteudo, $cc, $cco );
}
function enviar_email_h($docid){
 	
	global $db;
	
 	$sql = "SELECT
				usuemail
			FROM
				seguranca.usuario usu
			INNER JOIN elabrev.usuarioresponsabilidade rpu ON rpu.usucpf = usu.usucpf
			INNER JOIN monitora.termocooperacao tcp ON tcp.ungcodproponente = rpu.ungcod
			WHERE
				rpu.pflcod = ".PERFIL_SUBSECRETARIO." 
				AND docid = ".$docid."
				AND usustatus = 'A'
				AND rpu.rpustatus = 'A'";
 	
	//$email = $db->carregarColuna($sql);
	$email = "bruno.figueira@mev.gov.br";
	ver($email);
	ver("H");	
 	$sql = "SELECT
				tcpid
			FROM
				monitora.termocooperacao 
			WHERE
				docid = ".$docid;
 	
 	$tcpid = $db->pegaUm($sql);
 	
	$remetente = array('nome'=>'Programação Orçamentária - Descetralização de Crédito', 'email'=>$_SESSION['email_sistema']);
	
	$assunto  = "O termo de execução descentralizado $tcpid aguarda autorização para descentralização.";
	
	$conteudo = "<p>O termo de execução descentralizado $tcpid aguarda autorização para descentralização.</p>";
	
	
	enviar_email($remetente, $email, $assunto, $conteudo, $cc, $cco );
}
function enviar_email_i($docid){
 	
	global $db;
	
 	$sql = "SELECT
				usuemail
			FROM
				seguranca.usuario usu
			INNER JOIN elabrev.usuarioresponsabilidade rpu ON rpu.usucpf = usu.usucpf
			INNER JOIN monitora.termocooperacao tcp ON tcp.ungcodproponente = rpu.ungcod
			WHERE
				rpu.pflcod = ".PERFIL_CGSO." 
				AND docid =".$docid."
				AND usustatus = 'A'
				AND rpu.rpustatus = 'A'"; 
 	
	//$email = $db->carregarColuna($sql);
	$email = "bruno.figueira@mev.gov.br";	
	ver($email);
	ver("I");
 	$sql = "SELECT
				tcpid
			FROM
				monitora.termocooperacao 
			WHERE
				docid = ".$docid;
 	
 	$tcpid = $db->pegaUm($sql);
 	
	$remetente = array('nome'=>'Programação Orçamentária - Descetralização de Crédito', 'email'=>$_SESSION['email_sistema']);
	
	$assunto  = "O termo de execução descentralizado $tcpid pode ser enviado para execução. Descentralizar os recursos.";
	
	$conteudo = "<p>O termo de execução descentralizado $tcpid pode ser enviado para execução. Descentralizar os recursos.</p>";
	
	
	enviar_email($remetente, $email, $assunto, $conteudo, $cc, $cco );
}
function enviar_email_j($docid){
 	
	global $db;
	
 	$sql = "SELECT
				usuemail
			FROM
				seguranca.usuario usu
			INNER JOIN elabrev.usuarioresponsabilidade rpu ON rpu.usucpf = usu.usucpf
			INNER JOIN monitora.termocooperacao tcp ON tcp.ungcodproponente = rpu.ungcod
			WHERE
				rpu.pflcod = ".PERFIL_REITOR." 
				AND docid = ".$docid."
				AND usustatus = 'A'
				AND rpu.rpustatus = 'A'";
 	
	//$email = $db->carregarColuna($sql);
	$email = "bruno.figueira@mev.gov.br";	
	ver($email);
	ver("j");
 	$sql = "SELECT
				tcpid
			FROM
				monitora.termocooperacao 
			WHERE
				docid = ".$docid;
 	
 	$tcpid = $db->pegaUm($sql);
 	
	$remetente = array('nome'=>'Programação Orçamentária - Descetralização de Crédito', 'email'=>$_SESSION['email_sistema']);
	
	$assunto  = "O termo de execução descentralizado $tcpid foi alterado. Aguardando aprovação da reitoria.";
	
	$conteudo = "<p>O termo de execução descentralizado $tcpid foi alterado. Aguardando aprovação da reitoria.</p>";
	
	
	enviar_email($remetente, $email, $assunto, $conteudo, $cc, $cco );
}
function enviar_email_k($docid){
 	
	global $db;
	
 	$sql = "SELECT
				usuemail
			FROM
				seguranca.usuario usu
			INNER JOIN elabrev.usuarioresponsabilidade rpu ON rpu.usucpf = usu.usucpf
			INNER JOIN monitora.termocooperacao tcp ON tcp.ungcodproponente = rpu.ungcod
			WHERE
				rpu.pflcod = ".PERFIL_SUBSECRETARIO." 
				AND docid = ".$docid."
				AND usustatus = 'A'
				AND rpu.rpustatus = 'A'";
 	
	//$email = $db->carregarColuna($sql);
	$email = "bruno.figueira@mev.gov.br";
	ver($email);
	ver("k");	
 	$sql = "SELECT
				tcpid
			FROM
				monitora.termocooperacao 
			WHERE
				docid = ".$docid;
 	
 	$tcpid = $db->pegaUm($sql);
 	
	$remetente = array('nome'=>'Programação Orçamentária - Descetralização de Crédito', 'email'=>$_SESSION['email_sistema']);
	
	$assunto  = "O termo de execução descentralizado $tcpid foi aprovado. Aguardando aprovação do secretário.";
	
	$conteudo = "<p>O termo de execução descentralizado $tcpid foi aprovado. Aguardando aprovação do secretário.</p>";
	
	
	enviar_email($remetente, $email, $assunto, $conteudo, $cc, $cco );
}
*/
?>