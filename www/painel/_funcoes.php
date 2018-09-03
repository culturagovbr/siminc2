<?php

include_once("_funcoes_indicador_usuario.php");

/**
 * Função que controi o cabeçalho das páginas
 *
 * @author Juliano Meinen de Souza
 * @param inteiro indid (ID do Indicador)
 * @return null
 */
function cabecalhoIndicador($indid){
	global $db;
	
	if(!$indid):
		echo "<script>alert('Indicador inexistente.');window.location.href='painel.php?modulo=inicio&acao=A';</script>";	
	endif;
	if($indid):
		$sql = "SELECT 
						i.indnome,
						i.indobjetivo,
						s.sehid,
						um.umedesc,
						sec.secdsc,
						e.exodsc,
						per.perdsc,
						reg.regdescricao,
						peratual.perdsc as atualizacao
					FROM  
						painel.indicador	i
					LEFT JOIN
						painel.eixo e ON e.exoid = i.exoid
					LEFT JOIN
						painel.secretaria sec ON sec.secid = i.secid
					LEFT JOIN
						painel.unidademeta um ON um.umeid = i.umeid
					LEFT JOIN
						painel.periodicidade per ON per.perid = i.perid 
					LEFT JOIN 
						painel.seriehistorica s ON s.indid = i.indid
					LEFT JOIN 
						painel.regionalizacao reg ON reg.regid = i.regid
					LEFT JOIN
						painel.periodicidade peratual ON peratual.perid = i.peridatual 
					WHERE 
						i.indid = $indid 
					LIMIT 1";
			
			$dados = $db->pegaLinha($sql);
	?>
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td width="30%" valign="top" class="SubTituloDireita">
			Identificador do Indicador:
		</td>
		<td width="70%">
			<?=$indid;?>
		</td>
	</tr>
	<tr>
		<td width="30%" valign="top" class="SubTituloDireita">
			Eixo:
		</td>
		<td width="70%">
			<?=$dados['exodsc'];?>
		</td>
	</tr>
	<tr>
		<td width="30%" valign="top" class="SubTituloDireita">
			Secretaria/Autarquia Executora:
		</td>
		<td width="70%">
			<?=$dados['secdsc'];?>
		</td>
	</tr>
	<tr>
		<td width="30%" valign="top" class="SubTituloDireita">
			Nome Indicador:
		</td>
		<td width="70%">
			<?=$dados['indnome'];?>
		</td>
	</tr>	
	
	<tr >
		<td valign="top" class="SubTituloDireita">
			Objetivo:
		</td>
		<td>
			<?=$dados['indobjetivo'];?>
		</td>
	</tr>	
	<tr >
		<td valign="top" class="SubTituloDireita">
			Unidade Meta:
		</td>
		<td>
			<?=$dados['umedesc'];?>
		</td>
	</tr>
	<tr >
		<td valign="top" class="SubTituloDireita">
			Regionalização:
		</td>
		<td>
			<?=$dados['regdescricao'];?>
		</td>
	</tr>
	<tr >
		<td valign="top" class="SubTituloDireita">
			Periodicidade do Indicador:
		</td>
		<td id="td_cab_periodo" >
			<?=$dados['perdsc'];?>
		</td>
	</tr>
	<tr >
		<td valign="top" class="SubTituloDireita">
			Periodicidade de atualização:
		</td>
		<td id="td_cab_periodo" >
			<?=$dados['atualizacao'];?>
		</td>
	</tr>
        <tr>
            <td valign="middle" class="SubTituloDireita">
                Legenda:
            </td>
            <td>
        <?php
            $sql = 'select emidsc from painel.estrategia_meta_indicador where indid = '.$indid;
            $dados = $db->carregar($sql);
            echo '<table>';
            foreach($dados as $value){
                echo '<tr><td>'.$value['emidsc'].'</td></tr>';
            }
            echo '</table>';
        ?>
            </td>
        </tr>
	</table>
	<?php
	endif;
}


function montaCampoFormulario($campo,$valor = null,$indid = null){
	global $db;
	
	//Estado,Município,Quantidade,Valor,Ações
	
	switch($campo){
		
		case is_array($campo):
			
			$sql = "select
						tiddsc AS descricao, 
						tidid AS codigo
					from
						painel.detalhetipodadosindicador
					where
						tdiid = {$campo['tdiid']}
					and
						tidstatus = 'A'
					order by
						descricao";
			$db->monta_combo('detalhe_'.$campo['tdiid'],$sql,'S','Selecione...','','','','','S','detalhe_'.$campo['tdiid'],'',$valor);
		break;

		case 'Estado':
			$sql= "SELECT 
						estdescricao AS descricao, 
						estuf AS codigo
					FROM 		
						territorios.estado
					order by
			 			descricao";
			$db->monta_combo('Estado',$sql,'S','Selecione...','mudaMunicipios','','','','S','estado','',$valor);
			break;
		
		case 'Município':
			
			!$valor? $valor = "false" : $valor = $valor;		
					
			
			$sql = array(array("codigo" => "","descricao" => ""));
						
			echo "<div id=\"exibe_municipios\">";
			$db->monta_combo('municipio',$sql,'N','Selecione...','mudaIES(this.value);mudaEscola','','','','S','municipio','',$valor);
			echo "</div>";
			echo "<script>mudaMunicipios(document.getElementById('estado').value,$valor);</script>";
			break;
		
		case 'Escola':
			
			!$valor? $valor = "false" : $valor = $valor;
			
			if($valor != "false"){
				$permissao = "S";
				$sql = "SELECT 
							esc.escdsc AS descricao, 
							esc.esccodinep AS codigo 
						FROM 
							painel.escola esc 
						INNER JOIN 
							territorios.municipio ter ON ter.muncod = esc.escmuncod 
						WHERE 
							ter.estuf = ( 
									select 
										estuf 
									from 
										territorios.municipio
									where 
										muncod = (
						
											select 
												escmuncod 
											from 
												painel.escola
											where 
												esccodinep = '$valor' 
											)
								) 
						order by 
							descricao";
			}else{
			$permissao = "N";
			$sql= "SELECT 
						escdsc AS descricao, 
						esccodinep AS codigo
					FROM 		
						painel.escola
					order by
			 			descricao
			 		limit
			 			5";
			}
			
			echo "<div id=\"exibe_escolas\">";
			$db->monta_combo('Escola',$sql,$permissao,'Selecione...','','','','','S','escola','',$valor);
			echo "</div>";
			echo "<script>mudaEscola(document.getElementById('municipio').value,'$valor');</script>";
			break;
		
		case 'IES':
			
			!$valor? $valor = "false" : $valor = $valor;
			
			if($valor != "false"){
				$permissao = "S";
				$sql = "SELECT 
							ies.iesdsc AS descricao, 
							ies.iesid AS codigo
						FROM 
							painel.ies ies
						INNER JOIN 
							territorios.municipio ter ON ter.muncod = ies.iesmuncod 
						WHERE 
							ter.muncod =  (
						
											select distinct
												iesmuncod 
											from 
												painel.ies
											where 
												iesid = '$valor' 
										)
						AND
							ies.iesano = (
									select
										max(iesano) 
									from 
										painel.ies 
									) 
						order by 
							descricao";
			}else{
				$permissao = "N";
				$sql= "SELECT 
							iesdsc AS descricao, 
							iesid AS codigo
						FROM 		
							painel.ies
						order by
				 			descricao
				 		limit
				 			5";
			}
			echo "<div id=\"exibe_ies\">";
			$db->monta_combo('IES',$sql,$permissao,'Selecione...','','','','','S','ies','',$valor);
			echo "</div>";
			echo "<script>mudaIES(document.getElementById('municipio').value,'$valor');</script>";
			break;
		
		case 'Quantidade':
			if($indid){
			//Tipo de Quantitativo do Indicador
				$sql = "select
							unmid
						from
							painel.indicador
						where
							indid = $indid";
				$unmid = $db->pegaUm($sql);
				
				switch($unmid){
					case 1:
						$mask = "###.###.###.###,##";
						$valor = number_format(str_replace(',','',$valor),2,',','.');
						break;
					case 2:
						$mask = "###.###.###.###,##";
						$valor = number_format(str_replace(',','',$valor),2,',','.');
						break;
					case 3:
						$mask = "###.###.###.###.###";
						$valor = str_replace(',00','',$valor);
						break;
					case 4:
						$mask = "###.###.###.###.###";
						$valor = str_replace(',00','',$valor);
						break;
					case 5:
						$mask = "###.###.###.###,##";
						$valor = number_format(str_replace(',','',$valor),2,',','.');
						break;
				}
				
			}
			
			echo campo_texto('quantidade','S','S','',20,20,$mask,'quantidade','','','','id="quantidade"','',$valor);
			break;
			
		case 'Valor':
			echo campo_texto('valor','S','S','',20,20,'###.###.###.###,##','valor','','','','id="valor"','',$valor,'');
			break;
		
		case 'Ações':
					
			echo "<input type=\"hidden\" name=\"dshid\" value=\"$valor\" id=\"dshid\" />";
			echo '<img src="/imagens/gif_inclui.gif" style="cursor:pointer" align="absmiddle"  onclick="addDetalheSH(document.getElementById(\'dshid\').value);" title="Adicionar Série Histórica" />';
			break;
	}
	
}

function carregaMunicipios($regcod,$muncod){
	global $db;
	$sql= "SELECT 
					mundescricao AS descricao, 
					muncod AS codigo
				FROM 		
					territorios.municipio
				WHERE
					estuf = '$regcod'
				order by
		 			mundescricao ";
		
		echo "<div id=\"exibe_municipios\">";
		$db->monta_combo('municipio',$sql,'S','Selecione...','mudaIES(this.value);mudaEscola','','','','S','municipio','',$muncod);
		echo "</div>";
}

function carregaEscolas($muncod,$esccodinep){
	global $db;
	$sql= "SELECT 
					escdsc AS descricao, 
					esccodinep AS codigo
				FROM 		
					painel.escola
				WHERE
					escmuncod = '$muncod'
				order by
		 			descricao ";
		
		echo "<div id=\"exibe_escolas\">";		
		$db->monta_combo('escola',$sql,'S','Selecione...','','','','','S','escola','',$esccodinep);
		echo "</div>";
}

function carregaIES($muncod,$iesid){
	global $db;
	$sql= "SELECT 
					iesdsc AS descricao, 
					iesid AS codigo
				FROM 		
					painel.ies
				WHERE
					iesmuncod = '$muncod'
				AND
					iesano = (
								select 
									max(iesano) as ano 
								from 
									painel.ies
							)
				order by
		 			descricao ";
		
		echo "<div id=\"exibe_ies\">";
		$db->monta_combo('ies',$sql,'S','Selecione...','','','','','S','ies','',$ies);
		echo "</div>";
}


function carregaSerieHistorica($indid,$sehid = null){
	global $db;
	
	$sql = "select
				indqtdevalor,
				regid
			from
				painel.indicador
			where 
				indid = $indid";
	
	$ind = $db->pegaLinha($sql);

	
	//painel.detalhetipoindicador
//	$sql = "select
//				dtdi.tiddsc
//			from
//				painel.detalhetiposeriehistorica as dts
//			inner join
//				painel.detalhetipodadosindicador as dtdi ON dtdi.tidid = dts.tidid
//				dshid = $sehid
//			and
//				tdistatus = 'A'";
//	$detalhe = $db->carregar($sql);	
	
	$sql= "SELECT 
						count(ddiid)
					FROM 		
						painel.detalhedadoindicador
					WHERE
						ddistatus = 'A'
						and indid = $indid";
						
	$detalhe = $db->pegaUm($sql);

	if(count($detalhe) != 0){
//		$n = 1;
//		foreach($detalhe as $det){
//			$sqlDetalhe .= " {$det['tdiid']} as detalhe_$n,";
//			$n++;
//		}
		$imgEditar = "<center><img src=\"../imagens/alterar_01.gif\" title=\"Editar Detalhe da Série Histórica\" style=\"cursor:pointer\" onclick=\"alert(\'Operação não Permitida.\')\" /> <img src=\"../imagens/excluir.gif\" title=\"Excluir Detalhe da Série Histórica\" style=\"cursor:pointer\" onclick=\"excluirDetalheSH(' || dsh.dshid || ')\" /> <img title=\"Detalhe não Atribuído\" src=\"../imagens/atencaoVermelho.png\" /></center>";
	}else{
		$SQLdetalhe = "";
		$imgEditar = "<center><img src=\"../imagens/alterar.gif\" title=\"Editar Detalhe da Série Histórica\" style=\"cursor:pointer\" onclick=\"editarDetalheSH(' || dsh.dshid || ');\" /> <img src=\"../imagens/excluir.gif\" title=\"Excluir Detalhe da Série Histórica\" style=\"cursor:pointer\" onclick=\"excluirDetalheSH(' || dsh.dshid || ')\" /></center>";
	}
	
	
	$sql= "SELECT 
						unmid
					FROM 		
						painel.indicador
					WHERE
						indid = $indid";
						
	$unmid = $db->pegaUm($sql);
	
	$unmid == 1 ? $qtd = "Porcentagem" : $qtd = "Quantidade";
	
	echo "<input type=\"hidden\" value=\"$sehid\" id=\"SHsehid\" />";
	
	if($ind['indqtdevalor'] == 't')
			$valor = "coalesce(dsh.dshvalor,0) as dshvalor,";
	
$sql = "select
				unmdesc
			from
				painel.indicador i
			left join
				painel.unidademedicao u on i.unmid = u.unmid
			where
				indid = $indid";
	$unmdesc = $db->pegaUm($sql);
	($unmdesc == "")? $unmdesc = "Razão" : $unmdesc = $unmdesc;

//Verifica o tipo de medição e aplica as regras
	switch($unmdesc){
		case "Número inteiro":
			//$SQLsehqtde = "coalesce(sehqtde,0)::integer as sehqtde ";
			$SQLsehqtde = "replace(
				       substring(to_char(coalesce(dsh.dshqtde,0), '999,999,999'), 1, position('.' in to_char(coalesce(dsh.dshqtde,0), '999,999,999.99'))-1)
				       , ',', '.')
				       || ' ' ||
				       substring(to_char(coalesce(dsh.dshqtde,0), '999,999,999'), position('.' in to_char(coalesce(dsh.dshqtde,0), '999,999,999.99'))+1) as dshqtde";
			
			break;
		case "Percentual":
			//$SQLsehqtde = "coalesce(sehqtde,0) || '%' as sehqtde ";
			$SQLsehqtde = "replace(
				       substring(to_char(coalesce(dsh.dshqtde,0), '999,999,999.99'), 1, position('.' in to_char(coalesce(dsh.dshqtde,0), '999,999,999.99'))-1)
				       , ',', '.')
				       || ',' ||
				       substring(to_char(coalesce(dsh.dshqtde,0), '999,999,999.99'), position('.' in to_char(coalesce(dsh.dshqtde,0), '999,999,999.99'))+1) || '%'as dshqtde";
			break;
		case "Razão":
			//$SQLsehqtde = "coalesce(sehqtde,0) as sehqtde ";
			$SQLsehqtde = "replace(
				       substring(to_char(coalesce(dsh.dshqtde,0), '999,999,999.99'), 1, position('.' in to_char(coalesce(dsh.dshqtde,0), '999,999,999.99'))-1)
				       , ',', '.')
				       || ',' ||
				       substring(to_char(coalesce(dsh.dshqtde,0), '999,999,999.99'), position('.' in to_char(coalesce(dsh.dshqtde,0), '999,999,999.99'))+1) as dshqtde";
			break;
		case "Número índice":
			$SQLsehqtde = "coalesce(dsh.dshqtde,0) as sehqtde ";
			break;
		default:
			//$SQLsehqtde = "coalesce(sehqtde,0) as sehqtde ";
			$SQLsehqtde = "replace(
				       substring(to_char(coalesce(dsh.dshqtde,0), '999,999,999.99'), 1, position('.' in to_char(coalesce(dsh.dshqtde,0), '999,999,999.99'))-1)
				       , ',', '.')
				       || ',' ||
				       substring(to_char(coalesce(dsh.dshqtde,0), '999,999,999.99'), position('.' in to_char(coalesce(dsh.dshqtde,0), '999,999,999.99'))+1) as dshqtde";
			break;
	}
	
			
	
	!$sehid? $sehid = 0 : $sehid = $sehid;
	
	switch($ind['regid']){
			case 1: //Brasil
				$cabecalho = array($qtd);
				$sql = "(select
							$SQLsehqtde,
							$SQLdetalhe
							$valor
							'$imgEditar'
						from
							painel.detalheseriehistorica dsh
						where
							dsh.sehid = $sehid
						and
							dsh.ddiid is null
						order by
							dsh.dshid desc)";
				if($detalhe != 0){
						$sql .="UNION
						(select
							$SQLsehqtde,
							ddi.ddidsc as detalhe,
							$valor
							'<center><img src=\"../imagens/alterar.gif\" title=\"Editar Detalhe da Série Histórica\" style=\"cursor:pointer\" onclick=\"editarDetalheSH(' || dsh.dshid || ');\" /> <img src=\"../imagens/excluir.gif\" title=\"Excluir Detalhe da Série Histórica\" style=\"cursor:pointer\" onclick=\"excluirDetalheSH(' || dsh.dshid || ')\" /></center>'
						from
							painel.detalheseriehistorica dsh
						inner join
							painel.detalhedadoindicador ddi ON ddi.ddiid = dsh.ddiid
						where
							dsh.sehid = $sehid
						and
							dsh.ddiid is not null
						order by
							dsh.dshid desc)";
				}
				break;
			case 2: //Escola
				$cabecalho = array('Estado','Município','Escola',$qtd);
				$sql = "(select
							est.estdescricao,
							mun.mundescricao,
							esc.escdsc,
							$SQLsehqtde,
							$SQLdetalhe
							$valor
							'$imgEditar'
						from
							painel.detalheseriehistorica dsh
						inner join
							painel.escola esc ON esc.esccodinep = dsh.dshcod
						inner join
							territorios.municipio mun ON mun.muncod = esc.escmuncod
						inner join
							territorios.estado est ON mun.estuf = est.estuf
						where
							dsh.sehid = $sehid
						and
							dsh.ddiid is null
						order by
							dsh.dshid desc)";
				if($detalhe != 0){
						$sql .="UNION
						(select
							est.estdescricao,
							mun.mundescricao,
							esc.escdsc,
							$SQLsehqtde,
							ddi.ddidsc as detalhe,
							$valor
							'<center><img src=\"../imagens/alterar.gif\" title=\"Editar Detalhe da Série Histórica\" style=\"cursor:pointer\" onclick=\"editarDetalheSH(' || dsh.dshid || ');\" /> <img src=\"../imagens/excluir.gif\" title=\"Excluir Detalhe da Série Histórica\" style=\"cursor:pointer\" onclick=\"excluirDetalheSH(' || dsh.dshid || ')\" /></center>'
						from
							painel.detalheseriehistorica dsh
						inner join
							painel.escola esc ON esc.esccodinep = dsh.dshcod
						nner join
							territorios.municipio mun ON mun.muncod = esc.escmuncod
						inner join
							territorios.estado est ON mun.estuf = est.estuf
						where
							dsh.sehid = $sehid
						and
							dsh.ddiid is not null
						order by
							dsh.dshid desc)";
				}
				break;
			case 3: //Global
			$cabecalho = array($qtd);
				$sql = "(select
							$SQLsehqtde,
							$SQLdetalhe
							$valor
							'$imgEditar'
						from
							painel.detalheseriehistorica dsh
						where
							dsh.sehid = $sehid
						and
							dsh.ddiid is null
						order by
							dsh.dshid desc)";
				if($detalhe != 0){
						$sql .="UNION
						(select
							$SQLsehqtde,
							ddi.ddidsc as detalhe,
							$valor
							'<center><img src=\"../imagens/alterar.gif\" title=\"Editar Detalhe da Série Histórica\" style=\"cursor:pointer\" onclick=\"editarDetalheSH(' || dsh.dshid || ');\" /> <img src=\"../imagens/excluir.gif\" title=\"Excluir Detalhe da Série Histórica\" style=\"cursor:pointer\" onclick=\"excluirDetalheSH(' || dsh.dshid || ')\" /></center>'
						from
							painel.detalheseriehistorica dsh
						inner join
							painel.detalhedadoindicador ddi ON ddi.ddiid = dsh.ddiid
						where
							dsh.sehid = $sehid
						and
							dsh.ddiid is not null
						order by
							dsh.dshid desc)";
				}
				break;
			case 4: //Municipal
				$cabecalho = array('Estado','Município',$qtd);
				$sql = "(select
							uf.estdescricao,
							mun.mundescricao,
							$SQLsehqtde,
							$SQLdetalhe
							$valor
							'$imgEditar'
						from
							painel.detalheseriehistorica dsh
						inner join
							territorios.municipio mun ON dsh.dshcodmunicipio = mun.muncod
						inner join
							territorios.estado uf ON uf.estuf = mun.estuf
						where
							dsh.sehid = $sehid
						and
							dsh.ddiid is null
						order by
							dsh.dshid desc)";
				if($detalhe != 0){
						$sql .="UNION
						(select
							uf.estdescricao,
							mun.mundescricao,
							$SQLsehqtde,
							ddi.ddidsc as detalhe,
							$valor
							'<center><img src=\"../imagens/alterar.gif\" title=\"Editar Detalhe da Série Histórica\" style=\"cursor:pointer\" onclick=\"editarDetalheSH(' || dsh.dshid || ');\" /> <img src=\"../imagens/excluir.gif\" title=\"Excluir Detalhe da Série Histórica\" style=\"cursor:pointer\" onclick=\"excluirDetalheSH(' || dsh.dshid || ')\" /></center>'
						from
							painel.detalheseriehistorica dsh
						inner join
							painel.detalhedadoindicador ddi ON ddi.ddiid = dsh.ddiid
						inner join
							territorios.municipio mun ON dsh.dshcodmunicipio = mun.muncod
						inner join
							territorios.estado uf ON uf.estuf = mun.estuf
						where
							dsh.sehid = $sehid
						and
							dsh.ddiid is not null
						order by
							dsh.dshid desc)";
				}
				break;
			case 5: //IES
				$cabecalho = array('Estado','Município','IES',$qtd);
				$sql = "(select distinct
							est.estdescricao,
							mun.mundescricao,
							ies.iesdsc,
							$SQLsehqtde,
							$SQLdetalhe
							$valor
							'$imgEditar'
						from
							painel.detalheseriehistorica dsh
						inner join
							painel.ies ies ON ies.iesid = dsh.dshcod::integer
						inner join
							territorios.municipio mun ON mun.muncod = ies.iesmuncod
						inner join
							territorios.estado est ON mun.estuf = est.estuf
						where
							dsh.sehid = $sehid
						and
							dsh.ddiid is null
						and
							ies.iesano = (	select 
												max(iesano) as ano 
											from 
												painel.ies )
						)";
				if($detalhe != 0){
						$sql .="UNION
						(select distinct
							est.estdescricao,
							mun.mundescricao,
							ies.iesdsc,
							$SQLsehqtde,
							ddi.ddidsc as detalhe,
							$valor
							'<center><img src=\"../imagens/alterar.gif\" title=\"Editar Detalhe da Série Histórica\" style=\"cursor:pointer\" onclick=\"editarDetalheSH(' || dsh.dshid || ');\" /> <img src=\"../imagens/excluir.gif\" title=\"Excluir Detalhe da Série Histórica\" style=\"cursor:pointer\" onclick=\"excluirDetalheSH(' || dsh.dshid || ')\" /></center>'
						from
							painel.detalheseriehistorica dsh
						inner join
							painel.ies ies ON ies.iesid = dsh.dshcod::integer
						inner join
							territorios.municipio mun ON mun.muncod = ies.iesmuncod
						inner join
							territorios.estado est ON mun.estuf = est.estuf
						where
							dsh.sehid = $sehid
						and
							dsh.ddiid is not null
						and
							ies.iesano = (	select 
												max(iesano) as ano 
											from 
												painel.ies )";
				}
				break;
			case 6: //Estado
				$cabecalho = array('Estado',$qtd);
				$sql = "(select distinct
							est.estdescricao,
							$SQLsehqtde,
							$SQLdetalhe
							$valor
							'$imgEditar'
						from
							painel.detalheseriehistorica dsh
						inner join
							territorios.estado est ON dsh.dshuf = est.estuf
						where
							dsh.sehid = $sehid
						and
							dsh.ddiid is null
						and
							dsh.dshcodmunicipio is null)
							";
				if($detalhe != 0){
						$sql .="UNION
						(select distinct
							est.estdescricao,
							$SQLsehqtde,
							ddi.ddidsc as detalhe,
							$valor
							'<center><img src=\"../imagens/alterar.gif\" title=\"Editar Detalhe da Série Histórica\" style=\"cursor:pointer\" onclick=\"editarDetalheSH(' || dsh.dshid || ');\" /> <img src=\"../imagens/excluir.gif\" title=\"Excluir Detalhe da Série Histórica\" style=\"cursor:pointer\" onclick=\"excluirDetalheSH(' || dsh.dshid || ')\" /></center>'
						from
							painel.detalheseriehistorica dsh
						inner join
							territorios.estado est ON dsh.dshuf = est.estuf
						where
							dsh.sehid = $sehid
						and
							dsh.ddiid is not null
						and
							dsh.dshcodmunicipio is null)";
				}
				break;
			default:
				
			$cabecalho = array($qtd);
				$sql = "(select
							$SQLsehqtde,
							$SQLdetalhe
							$valor
							'$imgEditar'
						from
							painel.detalheseriehistorica dsh
						where
							dsh.sehid = $sehid
						and
							dsh.ddiid is null
						order by
							dsh.dshid desc)";
				if($detalhe != 0){
						$sql .="UNION
						(select
							$SQLsehqtde,
							ddi.ddidsc as detalhe,
							$valor
							'<center><img src=\"../imagens/alterar.gif\" title=\"Editar Detalhe da Série Histórica\" style=\"cursor:pointer\" onclick=\"editarDetalheSH(' || dsh.dshid || ');\" /> <img src=\"../imagens/excluir.gif\" title=\"Excluir Detalhe da Série Histórica\" style=\"cursor:pointer\" onclick=\"excluirDetalheSH(' || dsh.dshid || ')\" /></center>'
						from
							painel.detalheseriehistorica dsh
						inner join
							painel.detalhedadoindicador ddi ON ddi.ddiid = dsh.ddiid
						where
							dsh.sehid = $sehid
						and
							dsh.ddiid is not null
						order by
							dsh.dshid desc)";
				}
				break;
		}
		
		if($detalhe != 0)
			array_push($cabecalho, "Detalhe");
		
		if($ind['indqtdevalor'] == 't')
			array_push($cabecalho, "Valor");
			array_push($cabecalho, "Ações");

	$serieHistoria = $db->carregar($sql);
	$db->monta_lista_array($serieHistoria, $cabecalho, 50, 20, '', 'center', '');
}


function verificaRegistro($sehid,$dshcod,$dshcodmunicipio,$dshuf){
	global $db;
	if($dshcod != "null"){
		$sql = "select 
					count(dshid)
				from
					painel.detalheseriehistorica
				where
					sehid = $sehid
				and
					dshcod = '$dshcod'";
	}
	elseif($dshcodmunicipio != "null" && $dshuf != "null"){
		$sql = "select 
					count(dshid)
				from
					painel.detalheseriehistorica
				where
					sehid = $sehid
				and
					dshcodmunicipio = '$dshcodmunicipio'
				and
					dshuf = $dshuf";
	}
	elseif($dshcodmunicipio == "null" && $dshuf != "null"){
		$sql = "select 
					count(dshid)
				from
					painel.detalheseriehistorica
				where
					sehid = $sehid
				and
					dshuf = $dshuf";
	}
	

	$num = $db->pegaUm($sql);
	
	if($num != 0){
		return true;
	}
	else{
		return false;
	}
		
}


function excluirDetalheSH($indid,$sehid,$dshid){
	global $db;
		
	$sql = "delete 
			from
				painel.detalheseriehistorica
			where
				dshid = $dshid";
	$db->executar($sql);
	
	$sql = "select 
					coalesce(sum(dshqtde),0) as dshqtde,
					coalesce(sum(dshvalor),0) as dshvalor
				from
					painel.detalheseriehistorica
				where
					sehid = $sehid";
	$seh = $db->pegaLinha($sql);
		
	$sql = "update
					painel.seriehistorica
				set
					sehvalor = {$seh['dshvalor']},
					sehqtde = {$seh['dshqtde']}
				where
					sehid = $sehid";
	
	$db->executar($sql);	
	$db->commit();
	
	carregaSerieHistorica($indid,$sehid);
}

function editarDetalheSH($indid,$sehid,$dshid){
	global $db;
	$sql = "select
				indqtdevalor,
				regid
			from
				painel.indicador
			where 
				indid = $indid";
	
	$ind = $db->pegaLinha($sql);
	
	
	//painel.detalhetipoindicador
	$sql = "select
				tdiid,
				tdidsc
			from
				painel.detalhetipoindicador
			where
				indid = $indid
			and
				tdistatus = 'A'";
	$detalhe = $db->carregar($sql);
	
//	$sql= "SELECT 
//						count(ddiid)
//					FROM 		
//						painel.detalhedadoindicador
//					WHERE
//						ddistatus = 'A'
//						and indid = $indid";
//						
//	$detalhe = $db->pegaUm($sql);
	
//	if(count($detalhe) != 0){
//		$sqlDetalhe = "ddi.ddiid as detalhe,";
//		$innerDetalhe = "inner join
//							painel.detalhedadoindicador ddi ON dsh.ddiid = ddi.ddiid";
//	}

	if(count($detalhe) != 0){
		$n = 1;
		foreach($detalhe as $det){
			$sqlDetalhe .= " {$det['tdiid']} as detalhe_$n,";
			$n++;
		}
//		$innerDetalhe = "inner join
//							painel.detalhedadoindicador ddi ON dsh.ddiid = ddi.ddiid";
	}
	
		
	if($ind['regid'] == 4){
		$sql = "select
					dsh.dshid,
					uf.estuf,
					dsh.dshcodmunicipio,
					$sqlDetalhe
					coalesce(dsh.dshvalor,0) as dshvalor,
					coalesce(dsh.dshqtde,0) as dshqtde
				from
					painel.detalheseriehistorica dsh
				$innerDetalhe
				inner join
					territorios.municipio mun ON dsh.dshcodmunicipio = mun.muncod
				inner join
					territorios.estado uf ON uf.estuf = mun.estuf
				where
					dshid = $dshid";
		}
	elseif($ind['regid'] == 2){//Escola
			$sql = "select
					dsh.dshid,
					es.estuf,
					esc.escmuncod,
					esc.esccodinep,
					$sqlDetalhe
					coalesce(dsh.dshvalor,0) as dshvalor,
					coalesce(dsh.dshqtde,0) as dshqtde
				from
					painel.detalheseriehistorica dsh
				$innerDetalhe
				inner join
					painel.escola esc ON dsh.dshcod = esc.esccodinep
				inner join
					territorios.municipio mun ON esc.escmuncod = mun.muncod
				inner join
					territorios.estado es ON es.estuf = mun.estuf
				where
					dshid = $dshid";
		
	}
	elseif($ind['regid'] == 5){//IES
			$sql = "select
					dsh.dshid,
					es.estuf,
					ies.iesmuncod,
					ies.iesid,
					$sqlDetalhe
					coalesce(dsh.dshvalor,0) as dshvalor,
					coalesce(dsh.dshqtde,0) as dshqtde
				from
					painel.detalheseriehistorica dsh
				$innerDetalhe
				inner join
					painel.ies ies ON dsh.dshcod = ies.iesid::varchar(20)
				inner join
					territorios.municipio mun ON ies.iesmuncod = mun.muncod
				inner join
					territorios.estado es ON es.estuf = mun.estuf
				where
					dshid = $dshid";
	}
	elseif($ind['regid'] == 6){//Estado
			$sql = "select
					dsh.dshid,
					dsh.dshuf,
					$sqlDetalhe
					coalesce(dsh.dshvalor,0) as dshvalor,
					coalesce(dsh.dshqtde,0) as dshqtde
				from
					painel.detalheseriehistorica dsh
				$innerDetalhe
				where
					dshid = $dshid";
	}
	else{
		$sql = "select
					dsh.dshid,
					dsh.dshcodmunicipio,
					$sqlDetalhe
					coalesce(dsh.dshvalor,0) as dshvalor,
					coalesce(dsh.dshqtde,0) as dshqtde
				from
					painel.detalheseriehistorica dsh
				$innerDetalhe
				where
					dshid = $dshid";
	}
	
	$dsh = $db->pegaLinha($sql);
			
	switch($ind['regid']){
			case 1: //Brasil
				$arrTitulo = array('Quantidade');
				$arrValor = array($dsh['dshqtde']);
				if(count($detalhe) != 0){
					foreach($detalhe as $det){
							array_push($arrTitulo, array('titulo' => $det['tdidsc'] ,'tdiid' => $det['tdiid']));
							array_push($arrValor, $det['tdiid']);
					}
				}
				
				break;
			case 2: //Escola
				$arrTitulo = array('Estado','Município','Escola','Quantidade');
				$arrValor = array($dsh['estuf'],$dsh['escmuncod'],$dsh['esccodinep'],number_format(str_replace(',','',$dsh['dshqtde']),2,',','.'));
				if(count($detalhe) != 0){
					foreach($detalhe as $det){
							array_push($arrTitulo, array('titulo' => $det['tdidsc'] ,'tdiid' => $det['tdiid']));
							array_push($arrValor, $det['tdiid']);
					}
				}
				break;
			case 3: //Global
				$arrTitulo = array('Quantidade');
				$arrValor = array($dsh['dshqtde']);
				if(count($detalhe) != 0){
					foreach($detalhe as $det){
							array_push($arrTitulo, array('titulo' => $det['tdidsc'] ,'tdiid' => $det['tdiid']));
							array_push($arrValor, $det['tdiid']);
					}
				}
				break;
			case 4: //Municipal
				$arrTitulo = array('Estado','Município','Quantidade');
				$arrValor = array($dsh['estuf'],$dsh['dshcodmunicipio'],number_format(str_replace(',','',$dsh['dshqtde']),2,',','.'));
				if(count($detalhe) != 0){
					foreach($detalhe as $det){
							array_push($arrTitulo, array('titulo' => $det['tdidsc'] ,'tdiid' => $det['tdiid']));
							array_push($arrValor, $det['tdiid']);
					}
				}
				break;
			case 5: //IES
				$arrTitulo = array('Estado','Município','IES','Quantidade');
				$arrValor = array($dsh['estuf'],$dsh['iesmuncod'],$dsh['iesid'],number_format(str_replace(',','',$dsh['dshqtde']),2,',','.'));
				if(count($detalhe) != 0){
					foreach($detalhe as $det){
							array_push($arrTitulo, array('titulo' => $det['tdidsc'] ,'tdiid' => $det['tdiid']));
							array_push($arrValor, $det['tdiid']);
					}
				}
				break;
			case 6: //Estado
				$arrTitulo = array('Estado','Quantidade');
				$arrValor = array($dsh['dshuf'],number_format(str_replace(',','',$dsh['dshqtde']),2,',','.'));
				if(count($detalhe) != 0){
					foreach($detalhe as $det){
							array_push($arrTitulo, array('titulo' => $det['tdidsc'] ,'tdiid' => $det['tdiid']));
							array_push($arrValor, $det['tdiid']);
					}
				}
				break;
			default:
				$arrTitulo = array('Quantidade');
				$arrValor = array($dsh['dshqtde']);
				if(count($detalhe) != 0){
					foreach($detalhe as $det){
							array_push($arrTitulo, array('titulo' => $det['tdidsc'] ,'tdiid' => $det['tdiid']));
							array_push($arrValor, $det['tdiid']);
					}
				}
				break;
		}
		
		if($ind['indqtdevalor'] == 't'){
			array_push($arrTitulo, "Valor");
			array_push($arrValor, number_format(str_replace(',','',$dsh['dshvalor']),2,',','.'));
			
		}
			array_push($arrTitulo, "Ações");
			array_push($arrValor, $dsh['dshid']);

	
		$k = 0;
		foreach($arrTitulo as $titulo):?>
				<td style="font-weight: bold;text-align:center" bgcolor="#fcfcfc" onmouseout="this.bgColor='#fcfcfc';" onmouseover="this.bgColor='#c0c0c0';" valign="top" class="title">
					<? montaCampoFormulario($titulo,$arrValor[$k],$indid) ?>
				</td>
				
			<?php 
			$k++;
		endforeach;
	
}

function excluirSerieHistorica($indid,$sehid){
	global $db;
	
	$sql = "update 
				painel.seriehistorica
			set 
				sehstatus = 'I'
			where
				sehid = $sehid";
				
	$db->executar($sql);
	$db->commit(); 
	
	carregaSerieHistoria($indid);
}

/**
 * Função que cria todas as regras de negocio envolvendo perfis
 * 
 * @author Alexandre Dourado
 * @return array $permissoes
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 12/06/2009
 */
function verificaPerfilPainel() {
	global $db;
	/*
	 * Permissão padrão é sem acesso para todos os perfis
	 */
	// construindo o menu
	$enderecosweb = array("/painel/painel.php?modulo=principal/lista&acao=A"     => true,
						  "/painel/painel.php?modulo=principal/lista&acao=A&1=1" => true);
	
	
	if($db->testa_superuser()) {
		$permissoes['verindicadores']         = 'vertodos';
		$permissoes['condicaolista']          = "'<img style=\"cursor: pointer;\" src=\"/imagens/excluir.gif \" border=0 onclick=\"alterar(\'E\','||i.indid||');\" title=\"Excluir\">'";
		$permissoes['bloquearseriehistorica'] = true;
		$permissoes['sou_solicitante']        = true;
		$permissoes['removerseriehistorica'] = true;
		$permissoes['sou_atendente']          = (($db->pegaUm("SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf='".$_SESSION['usucpf']."' AND pflcod='".PAINEL_PERFIL_ATENDENTE."'"))?true:false);
		
		$permissoes['menu'][0] = array("descricao" => "Lista de Indicadores", "link"=> ($enderecosweb[$_SERVER['REQUEST_URI']])?$_SERVER['REQUEST_URI']:key($enderecosweb));
		$permissoes['menu'][1] = array("descricao" => "Meus indicadores", "link"=> "/painel/painel.php?modulo=principal/lista&acao=A&evento=M");
		$permissoes['menu'][2] = array("descricao" => "Cadastro de Indicadores", "link"=> "/painel/painel.php?modulo=principal/cadastro&acao=A&indid=novoIndicador");
		$permissoes['menu'][3] = array("descricao" => "Relatório de Indicadores", "link"=> "/painel/painel.php?modulo=principal/tabela&acao=A");
		$permissoes['menu'][4] = array("descricao" => "Relatório de Viagem PR", "link"=> "/painel/painel.php?modulo=principal/relatorioViagemPR&acao=A");
		$permissoes['menu'][5] = array("descricao" => "Relatório Pronatec", "link"=> "/painel/painel.php?modulo=principal/relatorioPronatec&acao=A");
		$permissoes['menu'][6] = array("descricao" => "Vincular Temas e Etapas", "link"=> "/painel/painel.php?modulo=principal/vincularTemas&acao=A");

	} else {
		// selecionando o perfil de maior nivel
		$sql = "SELECT p.pflcod FROM seguranca.perfil p 
				LEFT JOIN seguranca.perfilusuario pu ON pu.pflcod = p.pflcod 
				WHERE pu.usucpf = '". $_SESSION['usucpf'] ."' and p.pflstatus = 'A' and p.sisid =  '". $_SESSION['sisid'] ."' 
				ORDER BY pflnivel ASC LIMIT 1";
		
		$perfilcod = $db->pegaUm($sql);
		
		switch($perfilcod) {
			//equipe de apoio do responsável pela ação
			case PAINEL_PERFIL_ATUALIZA_SH:
				$permissoes['condicaolista']		 = "'<img style=\"cursor: pointer;\" src=\"/imagens/excluir_01.gif \" border=\"0\" title=\"Excluir\">'";
				$permissoes['verindicadores'] 		 = array();
				$sql = "SELECT ind.indid FROM painel.indicador ind
						INNER JOIN painel.usuarioresponsabilidade ur ON ur.acaid = ind.acaid
						WHERE ur.usucpf='".$_SESSION['usucpf']."' AND ur.rpustatus='A' GROUP BY ind.indid";
				
				$dadosacesso = $db->carregar($sql);
				if($dadosacesso[0]) {
					unset($permissoes['verindicadores']);
					foreach($dadosacesso as $ac) {
						$permissoes['verindicadores'][] = $ac['indid'];
					}
				}
				
				$permissoes['sou_solicitante']        = (($db->pegaUm("SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf='".$_SESSION['usucpf']."' AND pflcod='".PAINEL_PERFIL_SOLICITANTE."'"))?true:false);

				if (validaAcessoIndicadores($permissoes['verindicadores'], $_SESSION['indid']))
					$permissoes['removerseriehistorica'] = true;

			case PAINEL_PERFIL_ADM_ACAO:
				$permissoes['sou_solicitante']        = (($db->pegaUm("SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf='".$_SESSION['usucpf']."' AND pflcod='".PAINEL_PERFIL_SOLICITANTE."'"))?true:false);
				$permissoes['condicaolista']           = "CASE WHEN (i.acaid in (SELECT ur.acaid FROM painel.usuarioresponsabilidade ur WHERE ur.usucpf='".$_SESSION['usucpf']."' AND ur.acaid = i.acaid AND ur.rpustatus='A')) THEN '<img style=\"cursor: pointer;\" src=\"/imagens/excluir.gif \" border=0 onclick=\"alterar(\'E\','||i.indid||');\" title=\"Excluir\">' ELSE '<img style=\"cursor: pointer;\" src=\"/imagens/excluir_01.gif \" border=\"0\" title=\"Excluir\">' END";
				$permissoes['condicaomeusindicadores'] = "i.acaid in (SELECT ur.acaid FROM painel.usuarioresponsabilidade ur WHERE ur.usucpf='".$_SESSION['usucpf']."' AND ur.acaid = i.acaid AND ur.rpustatus='A')";
				
				$permissoes['menu'][0] = array("descricao" => "Lista de Indicadores", "link"=> ($enderecosweb[$_SERVER['REQUEST_URI']])?$_SERVER['REQUEST_URI']:key($enderecosweb));
				$permissoes['menu'][1] = array("descricao" => "Meus indicadores", "link"=> "/painel/painel.php?modulo=principal/lista&acao=A&evento=M");
				$permissoes['menu'][2] = array("descricao" => "Cadastro de Indicadores", "link"=> "/painel/painel.php?modulo=principal/cadastro&acao=A&indid=novoIndicador");
				//$permissoes['menu'][3] = array("descricao" => "Tabela de Indicadores", "link"=> "/painel/painel.php?modulo=principal/tabela&acao=A");
				
				$sql = "SELECT ind.indid FROM painel.indicador ind
						INNER JOIN painel.usuarioresponsabilidade ur ON ur.acaid = ind.acaid 
						WHERE ur.usucpf='".$_SESSION['usucpf']."' AND ur.rpustatus='A' GROUP BY ind.indid";
				
				$dadosacesso = $db->carregar($sql);
				if($dadosacesso[0]) {
					unset($permissoes['verindicadores']);
					foreach($dadosacesso as $ac) {
						$permissoes['verindicadores'][] = $ac['indid'];
					}
				}
				break;
			case PAINEL_PERFIL_ADM_EIXO:
				$permissoes['condicaolista']           = "CASE WHEN (i.exoid in (SELECT ur.exoid FROM painel.usuarioresponsabilidade ur WHERE ur.usucpf='".$_SESSION['usucpf']."' AND ur.exoid = i.exoid AND ur.rpustatus='A')) THEN '<img style=\"cursor: pointer;\" src=\"/imagens/excluir.gif \" border=0 onclick=\"alterar(\'E\','||i.indid||');\" title=\"Excluir\">' ELSE '<img style=\"cursor: pointer;\" src=\"/imagens/excluir_01.gif \" border=\"0\" title=\"Excluir\">' END";
				$permissoes['condicaomeusindicadores'] = "i.exoid in (SELECT ur.acaid FROM painel.usuarioresponsabilidade ur WHERE ur.usucpf='".$_SESSION['usucpf']."' AND ur.exoid = i.exoid AND ur.rpustatus='A')";
				$permissoes['removerseriehistorica'] = true;
				$permissoes['sou_solicitante']        = (($db->pegaUm("SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf='".$_SESSION['usucpf']."' AND pflcod='".PAINEL_PERFIL_SOLICITANTE."'"))?true:false);
				
				$permissoes['menu'][0] = array("descricao" => "Lista de Indicadores", "link"=> ($enderecosweb[$_SERVER['REQUEST_URI']])?$_SERVER['REQUEST_URI']:key($enderecosweb));
				$permissoes['menu'][1] = array("descricao" => "Meus indicadores", "link"=> "/painel/painel.php?modulo=principal/lista&acao=A&evento=M");
				$permissoes['menu'][2] = array("descricao" => "Cadastro de Indicadores", "link"=> "/painel/painel.php?modulo=principal/cadastro&acao=A&indid=novoIndicador");
				//$permissoes['menu'][3] = array("descricao" => "Tabela de Indicadores", "link"=> "/painel/painel.php?modulo=principal/tabela&acao=A");
				
				
				$sql = "SELECT ind.indid FROM painel.indicador ind
						INNER JOIN painel.usuarioresponsabilidade ur ON ur.exoid = ind.exoid 
						WHERE ur.usucpf='".$_SESSION['usucpf']."' GROUP BY ind.indid";
				
				$dadosacesso = $db->carregar($sql);
				if($dadosacesso[0]) {
					unset($permissoes['verindicadores']);
					foreach($dadosacesso as $ac) {
						$permissoes['verindicadores'][] = $ac['indid'];
					}
				}
				break;
			case EQUIPE_APOIO_GESTOR_PDE:
				$permissoes['condicaolista']		  = "'<img style=\"cursor: pointer;\" src=\"/imagens/excluir_01.gif \" border=\"0\" title=\"Excluir\">'";
				$permissoes['verindicadores'] 		  = array();
				$permissoes['bloquearseriehistorica'] = true;
				$permissoes['removerseriehistorica'] = true;
				$permissoes['sou_solicitante']        = (($db->pegaUm("SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf='".$_SESSION['usucpf']."' AND pflcod='".PAINEL_PERFIL_SOLICITANTE."'"))?true:false);
				
				$permissoes['menu'][0] = array("descricao" => "Lista de Indicadores", "link"=> ($enderecosweb[$_SERVER['REQUEST_URI']])?$_SERVER['REQUEST_URI']:key($enderecosweb));
				$permissoes['menu'][1] = array("descricao" => "Meus indicadores", "link"=> "/painel/painel.php?modulo=principal/lista&acao=A&evento=M");
				$permissoes['menu'][2] = array("descricao" => "Cadastro de Indicadores", "link"=> "/painel/painel.php?modulo=principal/cadastro&acao=A&indid=novoIndicador");
				//$permissoes['menu'][3] = array("descricao" => "Tabela de Indicadores", "link"=> "/painel/painel.php?modulo=principal/tabela&acao=A");
				break;
			case GESTOR_PDE:
				$permissoes['condicaolista']		  = "'<img style=\"cursor: pointer;\" src=\"/imagens/excluir_01.gif \" border=\"0\" title=\"Excluir\">'";
				$permissoes['verindicadores'] 		  = "vertodos";
				$permissoes['bloquearseriehistorica'] = true;
				$permissoes['removerseriehistorica'] = true;
				$permissoes['sou_solicitante']        = (($db->pegaUm("SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf='".$_SESSION['usucpf']."' AND pflcod='".PAINEL_PERFIL_SOLICITANTE."'"))?true:false);
				
				$permissoes['menu'][0] = array("descricao" => "Lista de Indicadores", "link"=> ($enderecosweb[$_SERVER['REQUEST_URI']])?$_SERVER['REQUEST_URI']:key($enderecosweb));
				$permissoes['menu'][1] = array("descricao" => "Meus indicadores", "link"=> "/painel/painel.php?modulo=principal/lista&acao=A&evento=M");
				$permissoes['menu'][2] = array("descricao" => "Cadastro de Indicadores", "link"=> "/painel/painel.php?modulo=principal/cadastro&acao=A&indid=novoIndicador");
				//$permissoes['menu'][3] = array("descricao" => "Tabela de Indicadores", "link"=> "/painel/painel.php?modulo=principal/tabela&acao=A");
				break;                            
			default:
				$permissoes['condicaolista']		 = "'<img style=\"cursor: pointer;\" src=\"/imagens/excluir_01.gif \" border=\"0\" title=\"Excluir\">'";
				$permissoes['verindicadores'] 		 = array();
				$permissoes['sou_solicitante']        = (($db->pegaUm("SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf='".$_SESSION['usucpf']."' AND pflcod='".PAINEL_PERFIL_SOLICITANTE."'"))?true:false);
				$permissoes['menu'][0] = array("descricao" => "Lista de Indicadores", "link"=> ($enderecosweb[$_SERVER['REQUEST_URI']])?$_SERVER['REQUEST_URI']:key($enderecosweb));				
		}
        //VERIFICA SE O PERFIL ADMINISTRADOR DO EIXO (Educação Profissional e Tecnológica) TEM PERMISSÃO DE ACESSO A ABA
        $sql = "select pflcod from painel.usuarioresponsabilidade WHERE rpustatus = 'A' AND pflcod = ".PAINEL_PERFIL_ADM_EIXO." AND usucpf = '". $_SESSION['usucpf'] ."' AND exoid = 3";
        $acessoPronatec = $db->pegaUm($sql);
        if($acessoPronatec){
            $permissoes['menu'][5] = array("descricao" => "Relatório Pronatec", "link"=> "/painel/painel.php?modulo=principal/relatorioPronatec&acao=A");
        }
	}
	return $permissoes;
}

function validaAcessoIndicadores($permissoes, $indid) {
	if($permissoes == 'semacesso')$permissoes = array();
	if(is_array($permissoes)) $permissoes = array_flip($permissoes);
	if(!isset($permissoes[$indid])) {
		return false;
	} else {
		return true;
	}
}

function pegaPerfil(){
	global $db;
	
	$sql = "select 
				pu.pflcod
			from 
				seguranca.perfilusuario pu 
			inner join 
				seguranca.perfil p on p.pflcod = pu.pflcod
			and 
				pu.usucpf = '{$_SESSION['usucpf']}' 
			and 
				p.sisid = {$_SESSION['sisid']}
			and
				pflstatus = 'A'";
				
	$arrPflcod = $db->carregar($sql);
	
	!$arrPflcod? $arrPflcod = array() : $arrPflcod = $arrPflcod;
	
	foreach($arrPflcod as $pflcod){
		$arrPerfil[] = $pflcod['pflcod'];
	}
	
	return $arrPerfil;
}

function filtraAcao($secid){
	global $db;
	
/*	$sql= "	select 
				acadsc AS descricao, 
				acaid AS codigo
			FROM 		
				painel.acao
			where
				acaid in(
						select 
							distinct acaid
						from
							painel.indicador
						where
							secid = $secid
						)
			order by
				descricao";
*/
	$sql= "	select 
				acadsc AS descricao, 
				acaid AS codigo
			FROM 		
				painel.acao
			where
				acaid in(
						select 
							distinct acaid
						from
							painel.acaosecretaria
						where
							secid = $secid
						)
				and acastatus='A'		
			order by
				descricao";
				
				
	echo $db->monta_combo('acaid',$sql,'S','Selecione...','','','','','N',"acaid","");

}


function exibeTabelaIndicador ( $indid, $arrTipoDetalhe = array("naoexibir"), $periodo = null, $arrQtdVlr = array("quantidade","valor"), $arrFiltros = array() , $arrTotalizadores = array(1,1) , $exibirRegionalizador = false, $semDetalhe = false, $exibeNome = true, $exibeObservacao = false, $exibeResponsavel = false, $exibeFonte = false){
	global $db;
	/*
	 $indid -> inteiro que recebe o identificador do indicador (campo obrigatório)
	 
	 $arrTipoDetalhe -> é composto de um array que passa os identificadores (tdiid) dos detalhamentos do Indicador ou apenas informa que não é necessária a exibição dos detalhamentos (campo não obrigatório)
		Exempo: array(100) ou array(200) ou array(100,200) ou array(200,100) ou array("naoexibir")
		
	 $periodo -> inteiro indentificador (perid) da periodicidade (painel.periodicidade) (campo não obrigatório)
	 
	 $arrQtdVlr -> array que informa quais as informações exibidas na tabela, podendo ser composta por 'quantidade' e 'valor'
	 	Exemplo: array("quantidade") ou array("valor") ou array("quantidade","valor")
	 
	 $arrFiltros -> array que recebe todos os filtros que serão aplicados na query que retorna os dados exibidos na tabela
	 
	 $arrTotalizadores -> array que informa quais totalizadores serão exibidos na tabela, sendo array(1,0) p/ apenas Total (direita), array(0,1) p/ apenas Total Geral (abaixo), array(1,1) p/ ambos (direita e abaixo) e array(0,0) p/ não exibir totalizadores
	 
	 $exibirRegionalizador -> boleana sendo true p/ exibir o total de regionalizadores (escolas,municípios,estados,etc) e false p/ ñ exibir
	 
	 $exibeNome -> boleana que indica se vai ou não ser exibido o nome do indicador (schema painel, tabela indicador, campo indnome)

	$exibeObservacao -> boleana que informa a Observação do Indicador;

	$exibeResponsavel -> boleana que informa os responsáveis 

	$exibeFonte -> boleana que informa a fonte
	 
	*/
			
	/* ********* *  INICIO QUERY PARA PEGAR OS DADOS DO INDICADOR * ********* */
	$sql = "select 
				ind.indid,
				ind.indnome,
				ind.perid,
				ind.indcumulativo,
				ind.indcumulativovalor,
				per.perdsc,
				per.perunidade,
				ind.unmid,
				reg.regunidade,
				ind.regid,
				ume.umedesc,
				ind.indqtdevalor,
				ind.indobjetivo,
				ind.indobservacao,
				ind.secid,
				ind.indfontetermo,
				ind.indshformula,
				ind.formulash
			from
				painel.indicador ind
			inner join
				painel.periodicidade per ON per.perid = ind.perid
			inner join
				painel.unidademeta ume ON ind.umeid = ume.umeid
			inner join
				painel.regionalizacao reg ON reg.regid = ind.regid
			where
				ind.indid = $indid
			and
				ind.indstatus = 'A'";
	$arrDadosIndicador = $db->pegaLinha($sql);
	
	if(!$arrDadosIndicador)
		return false;
	
	extract($arrDadosIndicador);
	$regid = (int)$regid;
	
	/* ********* *  FIM QUERY PARA PEGAR OS DADOS DO INDICADOR * ********* */
	
	
	/* ********* *  INICÍO REGRAS OBRIGATÓRIAS * ********* */
	
	//Se o indicador for monetário, a quantidade é a representação financeira
	if($unmid == UNIDADEMEDICAO_MOEDA)
		$arrQtdVlr = array("quantidade");
	//Se o indicador for inteiro, deve-se pegar apenas valores antes da ','
	if($unmid == UNIDADEMEDICAO_NUM_INTEIRO)
		$tipoUnmid = "::bigint ";
		
	//Mensagem de informação de registros não encontrados
	$msgErro = "Não Atendido.";
	
	//Se o indicador for razão é necessário informar o regionalizador
	$sql = "select 
				regdescricao, 
				rgaidentificador,
				rgafiltro 
			from 
				painel.regagreg reg1
			inner join
				painel.regionalizacao reg2 ON reg1.regid = reg2.regid 
			where 
				reg1.regid = $regid 
			and 
				regsqlcombo is not null";
	$campoRegiona = $db->pegaLinha($sql);
	/* Fim - Filtro por regvalue*/
	
	$retorno = true;
	
	if($campoRegiona && $unmid == UNIDADEMEDICAO_RAZAO && !$arrFiltros[$campoRegiona['rgaidentificador']] ){
		$msgErro = "Indicador de razão! Por favor, selecione o(a) {$campoRegiona['regdescricao']} e tente novamente!";
		$retorno = false;
	}
	
	if($unmid == UNIDADEMEDICAO_PERCENTUAL){
		$sql = "select tdiid from painel.detalhetipoindicador where indid = $indid";
		$arrTds = $db->carregarColuna($sql);
		if($arrTds && !$arrFiltros['tidid_1'] && !$arrFiltros['tidid_2']){
			$arrTotalizadores = array(0,0);
			$arrTipoDetalhe = $arrTds;
		}
	}
	
	/* ********* *  FIM REGRAS OBRIGATÓRIAS * ********* */
	
	
	/* ********* *  INICIO RETORNO FALSO * ********* */
	if($retorno == false){
		if($msgErro){
			echo "<center>$msgErro</center>";
		}
		return false;
	}
	/* ********* *  FIM RETORNO FALSO * ********* */
	
	
	/* ********* *  INICÍO DA APLICAÇÃO DOS FILTROS * ********* */
	
	//Filtro por tidid
	if($arrFiltros['tidid_1']){
		foreach($arrFiltros['tidid_1'] as $key => $tdi1){		
			if(strstr($tdi1[0],",")){
				$arrTd = explode(",",$tdi1[0]);
				foreach($arrTd as $td){
					if($td){
						$arrTid1[] = $td;
					}
				}
			}else{
				if(is_array($tdi1)){
					$arrTid1 = $tdi1;
				}else{
					$arrTid1[] = $tdi1;
				}
			}
			if($arrTid1[0]){
				$sql = "select 
								tdinumero 
							from 
								painel.detalhetipoindicador 
							where 
								tdiid in ($key)";
				$tdinumero = $db->pegaUm($sql);
				$tdinumero = !$tdinumero ? "1" : $tdinumero;
				if(!empty($tdi1[0])){
					$and[] = "tidid$tdinumero in ( ".implode(",",$arrTid1)." ) ";
					$AndTiid[$key] = " and tidid in ( ".implode(",",$arrTid1)." )";
				}
			}else{
				$arrTid1 = false;
			}
		}
	}

	if($arrFiltros['tidid_2']){
		foreach($arrFiltros['tidid_2'] as $key => $tdi2){
			if(strstr($tdi2[0],",")){
				$arrTd = explode(",",$tdi2[0]);
				foreach($arrTd as $td){
					if($td){
						$arrTid2[] = $td;
					}
				}
			}else{
				if(is_array($tdi2)){
					$arrTid2 = $tdi2;
				}else{
					$arrTid2[] = $tdi2;
				}
			}
			if($arrTid2[0]){
				$sql = "select 
							tdinumero 
						from 
							painel.detalhetipoindicador 
						where 
							tdiid in ($key)";
				$tdinumero = $db->pegaUm($sql);
				$tdinumero = !$tdinumero ? "2" : $tdinumero;
				if(!empty($tdi2[0])){
					$and[] = "tidid$tdinumero in ( ".implode(",",$arrTid2)." ) ";
					$AndTiid[$key] = " and tidid in ( ".implode(",",$arrTid2)." )";
				}
			}else{
				$arrTid2 = false;
			}
		}
		
	}
	//Filtro por microregiao
	if($arrFiltros['miccod'] && $arrFiltros['miccod'] != "" && $arrFiltros['miccod'] != "todos"){
		$and[] = "d.dshcodmunicipio in (	select muncod from territorios.municipio where miccod = '{$arrFiltros['miccod']}' )";
		$andReg[] = "d.dshcodmunicipio in (	select muncod from territorios.municipio where miccod = '{$arrFiltros['miccod']}' )";
		$groupBy[] = "d.dshcodmunicipio";
	}
	
	//Filtro por regcod
	if($indshformula == "t"){
		if($arrFiltros['regcod'] && $arrFiltros['regcod'] != "" && $arrFiltros['regcod'] != "todos"){
			$and[] = "d.estuf in ( select estuf from territorios.estado where regcod = '{$arrFiltros['regcod']}' ) ";
			$andReg[] = "d.estuf in ( select estuf from territorios.estado where regcod = '{$arrFiltros['regcod']}' ) ";
			$msgErro = "A região não solicitou recursos deste programa, não preencheu os critérios de atendimento do MEC, ou está com processo em andamento.";
			$groupBy[] = "d.estuf";
		}		
	}else{
		if($arrFiltros['regcod'] && $arrFiltros['regcod'] != "" && $arrFiltros['regcod'] != "todos"){
			$and[] = "d.dshuf in ( select estuf from territorios.estado where regcod = '{$arrFiltros['regcod']}' ) ";
			$andReg[] = "d.dshuf in ( select estuf from territorios.estado where regcod = '{$arrFiltros['regcod']}' ) ";
			$msgErro = "A região não solicitou recursos deste programa, não preencheu os critérios de atendimento do MEC, ou está com processo em andamento.";
			$groupBy[] = "d.dshuf";
		}
	}
	
	//Filtro por estuf
	if($indshformula == "t"){
		if($arrFiltros['estuf'] && $arrFiltros['estuf'] != "" && $arrFiltros['estuf'] != "todos"){
			$and[] = "d.estuf = '{$arrFiltros['estuf']}'";
			$andReg[] = "d.estuf = '{$arrFiltros['estuf']}'";
			$msgErro = "O estado não solicitou recursos deste programa, não preencheu os critérios de atendimento do MEC, ou está com processo em andamento.";
			$groupBy[] = "d.estuf";
		}	
	}else{
		if($arrFiltros['estuf'] && $arrFiltros['estuf'] != "" && $arrFiltros['estuf'] != "todos"){
			$and[] = "d.dshuf = '{$arrFiltros['estuf']}'";
			$andReg[] = "d.dshuf = '{$arrFiltros['estuf']}'";
			$msgErro = "O estado não solicitou recursos deste programa, não preencheu os critérios de atendimento do MEC, ou está com processo em andamento.";
			$groupBy[] = "d.dshuf";
		}
	}
	
	
	//Início - Filtro por grupo de municípios
	if($arrFiltros['gtmid'] && $arrFiltros['gtmid'] != "" && $arrFiltros['gtmid'] != "todos"){
		$and[] = "d.dshcodmunicipio in (select muncod from territorios.muntipomunicipio where tpmid in (select tpmid from territorios.tipomunicipio where gtmid = {$arrFiltros['gtmid']}) )";
		$andReg[] = "d.dshcodmunicipio in (select muncod from territorios.muntipomunicipio where tpmid in (select tpmid from territorios.tipomunicipio where gtmid = {$arrFiltros['gtmid']}) )";
		$groupBy[] = "d.dshcodmunicipio";
	}
	//Filtro por tpmid
	if($arrFiltros['tpmid'] && $arrFiltros['tpmid'] != "" && $arrFiltros['tpmid'] != "todos"){
		$and[] = "d.dshcodmunicipio in (	select muncod from territorios.muntipomunicipio where tpmid = '{$arrFiltros['tpmid']}' )";
		$andReg[] = "d.dshcodmunicipio in (	select muncod from territorios.muntipomunicipio where tpmid = '{$arrFiltros['tpmid']}' )";
		$groupBy[] = "d.dshcodmunicipio";
	}
	//Filtro por muncod
	if($arrFiltros['muncod'] && $arrFiltros['muncod'] != "" && $arrFiltros['muncod'] != "todos"){
		$and[] = "d.dshcodmunicipio = '{$arrFiltros['muncod']}' ";
		$andReg[] = "d.dshcodmunicipio = '{$arrFiltros['muncod']}' ";
		$msgErro = "O município não solicitou recursos deste programa, não preencheu os critérios de atendimento do MEC, ou está com processo em andamento.";
		$groupBy[] = "d.dshcodmunicipio";
	}
	//Filtro por mescod
	if($arrFiltros['mescod'] && $arrFiltros['mescod'] != ""){
		$and[] = "d.dshcodmunicipio in (select distinct muncod from territorios.municipio where mescod  = '{$arrFiltros['mescod']}') ";
		$andReg[] = "d.dshcodmunicipio in (select distinct muncod from territorios.municipio where mescod  = '{$arrFiltros['mescod']}') ";
		$msgErro = "A mesorregião não solicitou recursos deste programa, não preencheu os critérios de atendimento do MEC, ou está com processo em andamento.";
		$groupBy[] = "d.dshcodmunicipio";
	}
	//Filtro por polid
	if($arrFiltros['polid'] && $arrFiltros['polid'] != ""){
		$and[] = "d.polid = '{$arrFiltros['polid']}' ";
		$andReg[] = "d.polid = '{$arrFiltros['polid']}' ";
	}
	//Filtro por unicod
	if($arrFiltros['unicod'] && $arrFiltros['unicod'] != ""){
		$and[] = "d.unicod = '{$arrFiltros['unicod']}' ";
		$andReg[] = "d.unicod = '{$arrFiltros['unicod']}' ";
	}
	//Filtro por entid
	if($arrFiltros['entid'] && $arrFiltros['entid'] != ""){
		$and[] = "d.entid = '{$arrFiltros['entid']}' ";
		$andReg[] = "d.entid = '{$arrFiltros['entid']}' ";
	}
	//Filtro por iepid
	if($arrFiltros['iepid'] && $arrFiltros['iepid'] != ""){
		$and[] = "d.iepid = '{$arrFiltros['iepid']}' ";
		$andReg[] = "d.iepid = '{$arrFiltros['iepid']}' ";
	}
	//Filtro por dshcod
	if($arrFiltros['dshcod'] && $arrFiltros['dshcod'] != ""){
		$and[] = "d.dshcod = '{$arrFiltros['dshcod']}' ";
		$andReg[] = "d.dshcod = '{$arrFiltros['dshcod']}' ";
		$groupBy[] = "d.dshcod";
	}
	//Filtro por entid
	if($arrFiltros['entid'] && $arrFiltros['entid'] != ""){
		$and[] = "d.entid = '{$arrFiltros['entid']}' ";
		$andReg[] = "d.entid = '{$arrFiltros['entid']}' ";
		$groupBy[] = "d.entid";
	}
	
	//Filtro por Zona
	if($arrFiltros['zonid'] && $arrFiltros['zonid'] != ""){
		$and[] = "zonid = '{$arrFiltros['zonid']}' ";
		$andReg[] = "zonid = '{$arrFiltros['zonid']}' ";
		$groupBy[] = "zonid";
	}
	
	//Filtro por Subprefeitura
	if($arrFiltros['subid'] && $arrFiltros['subid'] != ""){
		$and[] = "subid= '{$arrFiltros['subid']}' ";
		$andReg[] = "subid= '{$arrFiltros['subid']}' ";
		$groupBy[] = "subid";
	}
	
	//Filtro por Distrito
	if($arrFiltros['disid'] && $arrFiltros['disid'] != ""){
		$and[] = "disid= '{$arrFiltros['disid']}' ";
		$andReg[] = "disid= '{$arrFiltros['disid']}' ";
		$groupBy[] = "disid";
	}
	
	//Filtro por Setor
	if($arrFiltros['setid'] && $arrFiltros['setid'] != ""){
		$and[] = "setid= '{$arrFiltros['setid']}' ";
		$andReg[] = "setid= '{$arrFiltros['setid']}' ";
		$groupBy[] = "setid";
	}
	
	//Filtro Por dpeid
	if(count($arrFiltros['dpeid'])){
		if($arrFiltros['dpeid'][0] && $arrFiltros['dpeid'][1]){
			$andDpe = " and 
						d.dpedatainicio >= ( select dpedatainicio from painel.detalheperiodicidade where dpeid = {$arrFiltros['dpeid'][0]})
					and
						d.dpedatainicio <= ( select dpedatafim from painel.detalheperiodicidade where dpeid = {$arrFiltros['dpeid'][1]})";
		}
	}
	
	/* Início - Filtro por regvalue*/
	if($arrFiltros[$campoRegiona['rgaidentificador']] && $arrFiltros[$campoRegiona['rgaidentificador']] != "" && $arrFiltros[$campoRegiona['rgaidentificador']] != "todos"){
		$and[] = "d.{$campoRegiona['rgaidentificador']} = '{$arrFiltros[$campoRegiona['rgaidentificador']]}' ";
		$andReg[] = "d.{$campoRegiona['rgaidentificador']} = '{$arrFiltros[$campoRegiona['rgaidentificador']]}' ";
		$groupBy[] = "d.{$campoRegiona['rgaidentificador']}";
	}
	/* Fim - Filtro por regvalue*/
	
		
	if($exibirRegionalizador){
		switch($regid){
			case REGIONALIZACAO_ESCOLA:
				$campoReg = "d.dshcod";
				$campoFiltro = "dshcod";
				$campoSelecionar = "a Escola";
				//$groupBy[] = "d.dshcod";
			break;
			case REGIONALIZACAO_IES:
				$campoReg = "d.dshcod";
				$campoFiltro = "dshcod";
				$campoSelecionar = "o Instituto de Ensino Superiro";
				//$groupBy[] = "d.dshcod";
			break;
			case REGIONALIZACAO_MUN:
				$campoFiltro = "muncod";
				$campoSelecionar = "o Município";
				if($arrFiltros['muncod'] && $arrFiltros['muncod'] != "" && $arrFiltros['muncod'] != "todos"){
					$campoReg = "";
					$exibirRegionalizador = false;
				}else
					$campoReg = "d.dshcodmunicipio";
					//$groupBy[] = "d.dshcodmunicipio";
			break;
			case REGIONALIZACAO_UF:
				$campoFiltro = "estuf";
				$campoSelecionar = "o Estado";
				if($arrFiltros['estuf'] && $arrFiltros['estuf'] != "" && $arrFiltros['estuf'] != "todos"){
					$campoReg = "";
					$exibirRegionalizador = false;
				}else
					$campoReg = "d.dshuf";
					//$groupBy[] = "d.dshuf";
			break;
			case REGIONALIZACAO_POSGRADUACAO:
				$campoFiltro = "dshcod";
				$campoReg = "d.dshcod";
				$campoSelecionar = "Instituto de Pós-Graduação";
				//$groupBy[] = "d.dshcod";
			break;
			case REGIONALIZACAO_CAMPUS_SUPERIOR:
				$campoFiltro = "entid";
				$campoReg = "d.entid";
				$campoSelecionar = "o Campus";
				//$groupBy[] = "d.entid";
			break;
			case REGIONALIZACAO_CAMPUS_PROFISSIONAL:
				$campoFiltro = "entid";
				$campoReg = "d.entid";
				$campoSelecionar = "o Campus";
				//$groupBy[] = "d.entid";
			break;
			case REGIONALIZACAO_UNIVERSIDADE:
				$campoFiltro = "unicod";
				$campoReg = "d.unicod";
				$campoSelecionar = "a Universidade";
				//$groupBy[] = "d.unicod";
			break;
			case REGIONALIZACAO_INSTITUTO:
				$campoFiltro = "dshcod";
				$campoReg = "d.dshcod";
				$campoSelecionar = "o Instituto";
				//$groupBy[] = "d.dshcod";
			break;
			case REGIONALIZACAO_HOSPITAL:
				$campoFiltro = "entid";
				$campoReg = "d.entid";
				$campoSelecionar = "o Hospital";
				//$groupBy = "d.entid";
			break;
			case REGIONALIZACAO_POLO:
				$campoFiltro = "polid";
				$campoReg = "d.polid";
				$campoSelecionar = "o Pólo";
				//$groupBy[] = "d.polid";
			break;
			case REGIONALIZACAO_ZONA:
				$campoFiltro = "zonid";
				$campoReg = "d.dshcod";
				$campoSelecionar = "a Zona";
				//$groupBy[] = "d.polid";
			break;
			case REGIONALIZACAO_SUBPREFEITURA:
				$campoFiltro = "subid";
				$campoReg = "d.dshcod";
				$campoSelecionar = "a Subprefeitura";
				//$groupBy[] = "d.polid";
			break;
			case REGIONALIZACAO_DISTRITO:
				$campoFiltro = "disid";
				$campoReg = "d.dshcod";
				$campoSelecionar = "o Distrito";
				//$groupBy[] = "d.polid";
			break;
			case REGIONALIZACAO_SETOR:
				$campoFiltro = "setid";
				$campoReg = "d.dshcod";
				$campoSelecionar = "o Setor";
				//$groupBy[] = "d.polid";
			break;
			default:
				$campoFiltro = "";
				$campoReg = "";
		}
	}
	
	if(is_array($and)){
		foreach($and as $andInt){
			$andInterno[] = str_replace("dsh.","dsh2.",$andInt);
		}
	}
	if(is_array($groupBy)){
		foreach($groupBy as $group){
			$groupByInterno[] = str_replace("dsh.","dsh2.",$group);
		}
	}
	
	/* ********* *  FIM DA APLICAÇÃO DOS FILTROS * ********* */
	
	/* ********* *  INÍCIO - REGRA PARA PROCENTAGEM E ÍNDICE * ********* */
	if($indshformula != "t" && ($unmid == UNIDADEMEDICAO_PERCENTUAL || $unmid == UNIDADEMEDICAO_NUM_INDICE) && $regid != REGIONALIZACAO_BRASIL){
		
		if($regid == REGIONALIZACAO_UF && $arrFiltros["estuf"] != "todos" && $arrFiltros["estuf"] != "" && $arrFiltros["estuf"] != null){
			
		}elseif($regid == REGIONALIZACAO_MUN && $arrFiltros["muncod"] != "todos" && $arrFiltros["muncod"] != "" && $arrFiltros["muncod"] != null){
			
		}else{?>
			<table class="tabela" width="100%" cellSpacing="1" border=0 cellPadding="3" align="center">
				<tr><td style="text-align: center;color:#990000" >Favor selecionar <?=$regid == REGIONALIZACAO_UF ? "o Estado" : "o Município"?>.</td></tr>
			</table>
		<?php
			return false;
		}
	}
	/* ********* *  FIM - REGRA PARA PROCENTAGEM E ÍNDICE * ********* */
	
	
	
	/* ********* *  INÍCIO SQL COMPLETO - 19/07/10 * ********* */
	$arrTipoDetalhe = !$arrTipoDetalhe ? array("naoexibir") : $arrTipoDetalhe;
	
	$periodo = !$periodo ? $perid : $periodo;
	
	$periodo =  strtoupper($periodo) == "ANUAL" || strtoupper($periodo) == "ANO" ? PERIODO_ANUAL : $periodo;
	$periodo = !$periodo ? PERIODO_ANUAL : $periodo;

	/* Inicio - Se não é necessária a exibição dos detalhamentos */
	if( in_array("naoexibir",$arrTipoDetalhe)){
		
		if($formulash && $indshformula == "t" && $unmid == UNIDADEMEDICAO_PERCENTUAL){
			if($arrFiltros['regcod']){
				$where_reg = "foo.regcod";
				$campo = "regcod,";
				$groupBy[] = "d.regcod";
			}
			if($arrFiltros['estuf']){
				$where_reg = "foo.estuf";
				$campo = "estuf,";
				$groupBy[] = "d.estuf";
			}
			if($arrFiltros['muncod']){
				$where_reg = "foo.muncod";
				$campo = "muncod,";
				$groupBy[] = "d.muncod";
			}
			if($arrFiltros['zonid']){
				$where_reg = "foo.zonid";
				$campo = "zonid,";
			}
			if($arrFiltros['subid']){
				$where_reg = "foo.subid";
				$campo = "subid,";
			}
			if($arrFiltros['disid']){
				$where_reg = "foo.disid";
				$campo = "disid,";
			}
			if($arrFiltros['setid']){
				$where_reg = "foo.setid";
				$campo = "setid,";
			}
			$dshqtde = "(".trataFormulaPorcentagem($formulash,"foo",$where_reg).")::numeric(12,2) as dshqtde";
		}else{
			$dshqtde = "sum(qtde)$tipoUnmid as dshqtde";
		}
	
		$sqlNovo = "select 
					dpeid,
					dpedsc,
					".($campoReg ? "(	select 
											count( distinct $campoReg) as reg 
										from 
											painel.v_detalheindicadorsh d
										where
											d.indid = foo.indid
										and
											d.dpedatainicio >= foo.dpedatainicio
										and
											d.dpedatainicio <= foo.dpedatafim
										and
											sehstatus <> 'I'
										".str_replace(array("dsh.","d1."),"d.",$andDpe)."
										".(count($andReg) ? " and ".implode(" and ",$andReg) : "" )." 
										group by
											d.indid
									) as reg," : "" )."
					$dshqtde,
					sum(valor) as dshvalor 
				from (
						select $campo
							dp.dpeid,
							d.indid,
							dp.dpedsc,
							dp.dpedatainicio,
							dp.dpedatafim,
							case when d.indcumulativo = 'N' then
				        			case when (
						                        select 
						                        	d1.dpeid
						                        from 
						                        	painel.detalheperiodicidade d1
												inner join 
													painel.seriehistorica sh on sh.dpeid=d1.dpeid
												where 
													d1.dpedatainicio>=dp.dpedatainicio 
												and 
													d1.dpedatafim<=dp.dpedatafim 
												and 
													sh.indid=d.indid
												and
													sehstatus <> 'I'
												".str_replace(array("dsh.","d."),"d1.",$andDpe)."
												order by 
													d1.dpedatainicio desc 
												limit 
													1
				                				) = d.dpeid then sum(d.qtde)  
				                	else 0 end
								else sum(d.qtde)
							end as qtde,
							case when d.indcumulativovalor = 'N' then
				        			case when (
				                        		select 
				                        			d1.dpeid
				                        		from 
				                        			painel.detalheperiodicidade d1
				                                inner join 
				                                	painel.seriehistorica sh on sh.dpeid=d1.dpeid
				                                where 
				                                	d1.dpedatainicio>=dp.dpedatainicio 
				                                and 
				                                	d1.dpedatafim<=dp.dpedatafim 
				                                and 
				                                	sh.indid=d.indid
												and
													sehstatus <> 'I'
				                                ".str_replace(array("dsh.","d."),"d1.",$andDpe)."
				                                order by 
													d1.dpedatainicio desc 
												limit 
													1
				                				) = d.dpeid then sum(d.valor)
				                			else 0 end
									else sum(d.valor)
							end as valor
						from 
							painel.v_detalheindicadorsh d
						inner join 
							painel.detalheperiodicidade dp on d.dpedatainicio>=dp.dpedatainicio and d.dpedatafim<=dp.dpedatafim
						-- periodo que vc quer exibir
						where 
							dp.perid = $periodo
						-- indicador que vc quer exibir
						and 
							d.indid = $indid
						and
							sehstatus <> 'I'
						".(count($and) ? " and ".implode(" and ",$and) : "" )."
						--range de data compreendida no periodo
						".str_replace("dsh.","d.",$andDpe)."
						group by 
							d.indid,
							d.dpeid,
							dp.dpedsc,
							dp.dpeid,
							dp.dpedatainicio,
							dp.dpedatafim,
							d.indcumulativo,
							d.indcumulativovalor
							".(count($groupBy) ? ",".implode(",",$groupBy) : "" )."
					) foo
				group by 
					$campo
					dpedatainicio,
					dpedatafim,
					dpeid,
					dpedsc,
					indid
				order by 
					dpedatainicio";

		//dbg($sqlNovo);
		$arrDados = $db->carregar($sqlNovo);
		if(is_array($arrDados)){
			
			foreach($arrDados as $arrDado){
				
				$arrValor[ $arrDado['dpeid'] ] = array( "periodo" 		 => $arrDado["dpedsc"],
														"qtde"	  		 => $arrDado["dshqtde"] ,
														"valor"	  		 => $arrDado["dshvalor"] ,
														"regionalizador" => $arrDado["reg"] );
				
				$arrPeriodos[ $arrDado['dpeid'] ] = $arrDado["dpedsc"];
				
			}
			
		}
	/* Fim - Se não é necessária a exibição dos detalhamentos */
	
	/* Inicio - Se é necessária a exibição de apenas 1 detalhamento */
	}elseif( !in_array("naoexibir",$arrTipoDetalhe) && count($arrTipoDetalhe) == 1 ){
	
		//Pega o numero do detalhe
		$sqlTdiNumero = "select tdinumero from painel.detalhetipoindicador where tdiid = {$arrTipoDetalhe[0]}";
		$tdinumero = $db->pegaUm($sqlTdiNumero);
					
		$sqlDetalhe = "select 
							tidid,
							tiddsc
						from 
							painel.detalhetipodadosindicador 
						where 
							tdiid = {$arrTipoDetalhe[0]}
						and
							tidstatus = 'A'
						".$AndTiid[$arrTipoDetalhe[0]]."
						order by tiddsc";
		$arrDetalhes1 = $db->carregar($sqlDetalhe);
	
		if($formulash && $indshformula == "t" && $unmid == UNIDADEMEDICAO_PERCENTUAL){
			if($arrFiltros['regcod']){
				$where_reg = "foo.regcod";
				$campo = "regcod,";
				$groupBy[] = "d.regcod";
			}
			if($arrFiltros['estuf']){
				$where_reg = "foo.estuf";
				$campo = "estuf,";
				$groupBy[] = "d.estuf";
			}
			if($arrFiltros['muncod']){
				$where_reg = "foo.muncod";
				$campo = "muncod,";
				$groupBy[] = "d.muncod";
			}
			if($arrFiltros['zonid']){
				$where_reg = "foo.zonid";
				$campo = "zonid,";
			}
			if($arrFiltros['zonid']){
				$where_reg = "foo.zonid";
				$campo = "zonid,";
			}
			if($arrFiltros['subid']){
				$where_reg = "foo.subid";
				$campo = "subid,";
			}
			if($arrFiltros['disid']){
				$where_reg = "foo.disid";
				$campo = "disid,";
			}
			if($arrFiltros['setid']){
				$where_reg = "foo.setid";
				$campo = "setid,";
			}
			$dshqtde = "(".trataFormulaPorcentagem($formulash,"foo",$where_reg).")::numeric(12,2) as dshqtde";
		}else{
			$dshqtde = "sum(qtde)$tipoUnmid as dshqtde";
		}
		
		$sqlNovo = "select 
					dpeid,
					tidid$tdinumero,
					dpedsc,
					".($campoReg ? "(	select 
											count( distinct $campoReg) as reg 
										from 
											painel.v_detalheindicadorsh d
										where
											d.indid = foo.indid
										and
											d.dpedatainicio >= foo.dpedatainicio
										and
											d.dpedatainicio <= foo.dpedatafim
										and
											sehstatus <> 'I'
										and
											d.tidid$tdinumero = foo.tidid$tdinumero
										".str_replace(array("dsh.","d1."),"d.",$andDpe)."
										".(count($andReg) ? " and ".implode(" and ",$andReg) : "" )." 
										group by
											d.indid
									) as reg," : "" )."
					$dshqtde,
					sum(valor) as dshvalor 
				from (
						select $campo
							d.indid,
							dp.dpeid,
							dp.dpedsc,
							tidid$tdinumero,
							dp.dpedatainicio,
							dp.dpedatafim,
							case when d.indcumulativo = 'N' then
				        			case when (
						                        select 
						                        	d1.dpeid 
						                        from 
						                        	painel.detalheperiodicidade d1
												inner join 
													painel.seriehistorica sh on sh.dpeid=d1.dpeid
												where 
													d1.dpedatainicio>=dp.dpedatainicio 
												and 
													d1.dpedatafim<=dp.dpedatafim
												and 
													sh.indid=d.indid
												and
													sehstatus <> 'I'
												".str_replace(array("dsh.","d."),"d1.",$andDpe)."
												order by 
													d1.dpedatainicio desc 
												limit 
													1
				                				)=d.dpeid then sum(d.qtde)
				                	else 0 end
								else sum(d.qtde)
							end as qtde,
							case when d.indcumulativovalor = 'N' then
				        			case when (
				                        		select 
				                        			d1.dpeid 
				                        		from 
				                        			painel.detalheperiodicidade d1
				                                inner join 
				                                	painel.seriehistorica sh on sh.dpeid=d1.dpeid
				                                where 
				                                	d1.dpedatainicio>=dp.dpedatainicio 
				                                and 
				                                	d1.dpedatafim<=dp.dpedatafim 
				                                and 
				                                	sh.indid=d.indid
												and
													sehstatus <> 'I'
				                                ".str_replace(array("dsh.","d."),"d1.",$andDpe)."
				                                order by 
				                                	d1.dpedatainicio desc 
				                                limit 
				                                	1
				                				)=d.dpeid then sum(d.valor)
				                			else 0 end
									else sum(d.valor)
							end as valor
						from 
							painel.v_detalheindicadorsh d
						inner join 
							painel.detalheperiodicidade dp on d.dpedatainicio>=dp.dpedatainicio and d.dpedatafim<=dp.dpedatafim
						-- periodo que vc quer exibir
						where 
							dp.perid = $periodo
						-- indicador que vc quer exibir
						and 
							d.indid = $indid
						and
							sehstatus <> 'I'
						".(count($and) ? " and ".implode(" and ",$and) : "" )."
						--range de data compreendida no periodo
						".str_replace("dsh.","d.",$andDpe)."
						group by 
							d.indid,
							d.dpeid,
							dp.dpedsc,
							dp.dpeid,
							dp.dpedatainicio,
							dp.dpedatafim,
							d.indcumulativo,
							d.indcumulativovalor,
							tidid$tdinumero
							".(count($groupBy) ? ",".implode(",",$groupBy) : "" )."
					) foo
				group by 
					$campo
					indid,
					dpedatainicio,
					dpedatafim,
					dpeid,
					dpedsc,
					tidid$tdinumero
				order by 
					dpedatainicio";

		//dbg($sqlNovo,1);
		$arrDados = $db->carregar($sqlNovo);
		if(is_array($arrDados)){
			
			foreach($arrDados as $arrDado){
				
				$arrValor[ $arrDado['dpeid'] ] [ $arrDado["tidid$tdinumero"] ] = array( 	"periodo" 		 => $arrDado["dpedsc"],
																						"qtde"	  		 => $arrDado["dshqtde"] ,
																						"valor"	  		 => $arrDado["dshvalor"] ,
																						"regionalizador" => $arrDado["reg"] );
				
				$arrPeriodos[ $arrDado['dpeid'] ] = $arrDado["dpedsc"];
				
			}
			
		}
		
	/* Fim - Se é necessária a exibição de apenas 1 detalhamento */
	
	/* Inicio - Se é necessária a exibição de 2 detalhamentos */
	}elseif( !in_array("naoexibir",$arrTipoDetalhe) && count($arrTipoDetalhe) == 2 ){
	
		//Pega o numero do detalhe 1
		$sqlTdiNumero1 = "select tdinumero from painel.detalhetipoindicador where tdiid = {$arrTipoDetalhe[0]}";
		$tdinumero1 = $db->pegaUm($sqlTdiNumero1);
		
		//Pega o numero do detalhe 2
		$sqlTdiNumero2 = "select tdinumero from painel.detalhetipoindicador where tdiid = {$arrTipoDetalhe[1]}";
		$tdinumero2 = $db->pegaUm($sqlTdiNumero2);
					
		$sqlDetalhe1 = "select 
							tidid,
							tiddsc 
						from 
							painel.detalhetipodadosindicador 
						where 
							tdiid = {$arrTipoDetalhe[0]}
						and
							tidstatus = 'A'
						".$AndTiid[$arrTipoDetalhe[0]]."
						order by tiddsc";
		$arrDetalhes1 = $db->carregar($sqlDetalhe1);
		
		$sqlDetalhe2 = "select 
							tidid,
							tiddsc 
						from 
							painel.detalhetipodadosindicador 
						where 
							tdiid = {$arrTipoDetalhe[1]}
						and
							tidstatus = 'A'
						".$AndTiid[$arrTipoDetalhe[1]]."
						order by tiddsc";
		$arrDetalhes2 = $db->carregar($sqlDetalhe2);
		
		if($formulash && $indshformula == "t" && $unmid == UNIDADEMEDICAO_PERCENTUAL){
			$dshqtde = "(".trataFormulaPorcentagem($formulash,"foo").")::numeric(12,2) as dshqtde";
		}else{
			$dshqtde = "sum(qtde)$tipoUnmid as dshqtde";
		}
	
		$sqlNovo = "select 
					dpeid,
					tidid$tdinumero1,
					tidid$tdinumero2,
					dpedsc,
					".($campoReg ? "(	select 
											count( distinct $campoReg) as reg 
										from 
											painel.v_detalheindicadorsh d
										where
											d.indid = foo.indid
										and
											d.dpedatainicio >= foo.dpedatainicio
										and
											d.dpedatainicio <= foo.dpedatafim
										and
											sehstatus <> 'I'
										and
											d.tidid$tdinumero1 = foo.tidid$tdinumero1
										and
											d.tidid$tdinumero2 = foo.tidid$tdinumero2
										".str_replace(array("dsh.","d1."),"d.",$andDpe)."
										".(count($andReg) ? " and ".implode(" and ",$andReg) : "" )." 
										group by
											d.indid
									) as reg," : "" )."
					$dshqtde,
					sum(valor) as dshvalor 
				from (
						select 
							dp.dpeid,
							d.indid,
							dp.dpedsc,
							tidid$tdinumero1,
							tidid$tdinumero2,
							dp.dpedatainicio,
							dp.dpedatafim,
							case when d.indcumulativo = 'N' then
				        			case when (
						                        select 
						                        	d1.dpeid 
						                        from 
						                        	painel.detalheperiodicidade d1
												inner join 
													painel.seriehistorica sh on sh.dpeid=d1.dpeid
												where 
													d1.dpedatainicio>=dp.dpedatainicio 
												and 
													d1.dpedatafim<=dp.dpedatafim 
												and 
													sh.indid=d.indid
												and
													sehstatus <> 'I'
												".str_replace(array("dsh.","d."),"d1.",$andDpe)."
												order by 
													d1.dpedatainicio desc 
												limit 
													1
				                				)=d.dpeid then sum(d.qtde)
				                	else 0 end
								else sum(d.qtde)
							end as qtde,
							case when d.indcumulativovalor = 'N' then
				        			case when (
				                        		select 
				                        			d1.dpeid
				                        		from 
				                        			painel.detalheperiodicidade d1
				                                inner join 
				                                	painel.seriehistorica sh on sh.dpeid=d1.dpeid
				                                where 
				                                	d1.dpedatainicio>=dp.dpedatainicio 
				                                and 
				                                	d1.dpedatafim<=dp.dpedatafim 
				                                and 
				                                	sh.indid=d.indid
												and
													sehstatus <> 'I'
				                                ".str_replace(array("dsh.","d."),"d1.",$andDpe)."
				                                order by 
				                                	d1.dpedatainicio desc 
				                                limit 
				                                	1
				                				) = d.dpeid then sum(d.valor) 
				                			else 0 end
									else sum(d.valor)
							end as valor
						from 
							painel.v_detalheindicadorsh d
						inner join 
							painel.detalheperiodicidade dp on d.dpedatainicio>=dp.dpedatainicio and d.dpedatafim<=dp.dpedatafim
						-- periodo que vc quer exibir
						where 
							dp.perid = $periodo
						-- indicador que vc quer exibir
						and 
							d.indid = $indid
						and
							sehstatus <> 'I'
						".(count($and) ? " and ".implode(" and ",$and) : "" )."
						--range de data compreendida no periodo
						".str_replace("dsh.","d.",$andDpe)."
						group by 
							d.indid,
							d.dpeid,
							dp.dpedsc,
							dp.dpeid,
							dp.dpedatainicio,
							dp.dpedatafim,
							d.indcumulativo,
							d.indcumulativovalor,
							tidid$tdinumero1,
							tidid$tdinumero2
							".(count($groupBy) ? ",".implode(",",$groupBy) : "" )."
					) foo
				group by 
					dpedatainicio,
					dpedatafim,
					indid,
					dpeid,
					dpedsc,
					tidid$tdinumero1,
					tidid$tdinumero2
				order by 
					dpedatainicio";
		//dbg($sqlNovo,1);
		$arrDados = $db->carregar($sqlNovo);
		
		if(is_array($arrDados)){
			
			foreach($arrDados as $arrDado){
				
				$arrValor[ $arrDado['dpeid'] ] [ $arrDado["tidid$tdinumero1"] ] [ $arrDado["tidid$tdinumero2"] ] = array( 	"periodo" 		 => $arrDado["dpedsc"],
																															"qtde"	  		 => $arrDado["dshqtde"] ,
																															"valor"	  		 => $arrDado["dshvalor"] ,
																															"regionalizador" => $arrDado["reg"] );
				
				$arrPeriodos[ $arrDado['dpeid'] ] = $arrDado["dpedsc"];
				
			}
			
		}
		
	};
	/* Fim - Se é necessária a exibição de 2 detalhamentos*/
	
	/* ********* *  FIM SQL COMPLETO - 19/07/10 * ********* */	
	//Verifica possibilidade de exibição de Qtd. / Valor
	if(count($arrQtdVlr) == 2 && $indqtdevalor == "t" ){
		$QtdValor = 2;
		$ExibeQtd = true;
		$ExibeValor = true;
	}elseif(in_array("quantidade", $arrQtdVlr)){
		$QtdValor = 1;
		$ExibeQtd = true;
		$ExibeValor = false;
	}elseif(in_array("valor", $arrQtdVlr) && $indqtdevalor == "t"){
		$QtdValor = 1;
		$ExibeQtd = false;
		$ExibeValor = true;
	}

	if(trim($regunidade) == trim($umedesc)){
		$exibirRegionalizador = false;
		$campoReg = false;
	}

	if($exibirRegionalizador && $campoReg != ""){
		$QtdValor += 1;	
	}

	$count1 = !$arrDetalhes1 ? 1 : count($arrDetalhes1);
	$count2 = !$arrDetalhes2 ? 1 : count($arrDetalhes2); 
	$arrDetalhes1 = !$arrDetalhes1 ? array() : $arrDetalhes1;
	$arrDetalhes2 = !$arrDetalhes2 ? array() : $arrDetalhes2;
	
	/* INÍCIO - Retira exibição de detalhes sem valores */
	unset($arrTidid1);
	unset($arrTidid2);	
	foreach($arrDetalhes1 as $arrD1){
		$arrTidid1[] = $arrD1['tidid'];
	}
	
	if($arrTidid1){
		$sql = "select 
								distinct tdinumero 
							from 
								painel.detalhetipoindicador dti 
							inner join
								painel.detalhetipodadosindicador dtdi ON dtdi.tdiid = dti.tdiid 
							where 
								dtdi.tidid in (".implode(",",$arrTidid1).")";
					$tdinumero = $db->pegaUm($sql);
		
		$sql = "select 
				tidid, 
				tiddsc 
			from 
				painel.detalhetipodadosindicador 
			where 
				tidid in (".implode(",",$arrTidid1).") 
				order by tiddsc";
		$arrDetalhes1 = $db->carregar($sql);
	}
	
	foreach($arrDetalhes2 as $arrD2){
		$arrTidid2[] = $arrD2['tidid'];
	}
	if($arrTidid2){
		$sql = "select 
								distinct tdinumero 
							from 
								painel.detalhetipoindicador dti 
							inner join
								painel.detalhetipodadosindicador dtdi ON dtdi.tdiid = dti.tdiid 
							where 
								dtdi.tidid in (".implode(",",$arrTidid2).")";
					$tdinumero2 = $db->pegaUm($sql);
		
		$sql = "select 
				tidid, 
				tiddsc 
			from 
				painel.detalhetipodadosindicador 
			where 
				tidid in (".implode(",",$arrTidid2).")
				order by tiddsc";
		$arrDetalhes2 = $db->carregar($sql);
	}
	
	/* Início - Aplicação de filtro para regionalizadores */
	if($arrFiltros['dpeid'][0])
		$arrDpeReg[] = "d.dpedatainicio >= (select dpedatainicio from painel.detalheperiodicidade where dpeid = {$arrFiltros['dpeid'][0]} )"; 
	if($arrFiltros['dpeid'][1])
		$arrDpeReg[] = "d.dpedatafim <= (select dpedatafim from painel.detalheperiodicidade where dpeid = {$arrFiltros['dpeid'][1]} )";
	/* Fim - Aplicação de filtro para regionalizadores */
	
	$count1 = !$arrDetalhes1 ? 1 : count($arrDetalhes1);
	$count2 = !$arrDetalhes2 ? 1 : count($arrDetalhes2); 
	$arrDetalhes1 = !$arrDetalhes1 ? array() : $arrDetalhes1;
	$arrDetalhes2 = !$arrDetalhes2 ? array() : $arrDetalhes2;
	/* FIM - Retira exibição de detalhes sem valores */
	
	//$coslpanGeral = (Periodo + (detalhe1 * detalhe2 * $QtdValor) + regionalizador + total)
	$coslpanGeral = (1 + ( $count1 * $count2 * $QtdValor) + $QtdValor);
				
	if($exibeResponsavel){

		$sqlResp = "select 
					respnome,
					respemail,
					(case when respdddcelular is not null
						then '(' || respdddcelular || ') ' || respcelular
						else 'N/A'
					end) as  celular,
					(case when respdddtelefone is not null
						then '(' || respdddtelefone || ') ' || resptelefone
						else 'N/A'
					end) as  telefone
				from
					painel.responsavelsecretaria
				where
					secid = $secid
				and
					respstatus = 'A'";
				$resp = $db->carregar($sqlResp);
				if($resp){
						$responsaveis = "<table cellspacing=\"0\" border=\"0\" cellpadding=\"0\" >";
					foreach($resp as $rs){
						$responsaveis .= "<tr><td style=\"color:#888888;border:0px\"><b>Responsável:</b> <span style=\"padding-right:30px\" >{$rs['respnome']}</span> </td><td style=\"color:#888888;border:0px\"><b>E-mail:</b></span> {$rs['respemail']}</td></tr>";
						$responsaveis .= "<tr><td style=\"color:#888888;border:0px;\" ><b>Telefone:</b> {$rs['telefone']} </td><td style=\"color:#888888;border:0px\" ><b>Celular:</b></span> {$rs['celular']}</td></tr>";
					}
					$responsaveis .= "</table>";
				}
	}
	
	$exibeFonte;
	//dbg($sqlNovo,1);
	?>

<?php /* INÍCIO - EXIBIÇÃO DO INICIO DA TABELA COM O NOME DO INDICADOR */?>
	<table class="tabela" width="100%" bgcolor="FFFFFF" cellSpacing="1" border=0 cellPadding="3" align="center">
<thead>
<?php if($exibeNome){?>
		<tr bgcolor="#e9e9e9" >
			<th style="text-align:center;font-weight:bold" colspan="<?php echo $coslpanGeral ?>"><?=$indnome?></th>
		</tr>
<?php } 
/* FIM - EXIBIÇÃO DO INICIO DA TABELA COM O NOME DO INDICADOR */

/* INÍCIO - EXIBIÇÃO DA OBSERVAÇÃO DO INDICADOR */?>
<?php if($exibeObservacao && ($indobjetivo || $indobservacao) ){?>
		<tr bgcolor="#e9e9e9" >
			<th style="color:#888888;text-align:justify;font-size:11px;font-weight: normal" colspan="<?php echo $coslpanGeral ?>"><?=$indobjetivo?> <? echo $indobservacao ? "<br/><b>Obs:</b> $indobservacao" : ""?></th>
		</tr>
<?php } ?>
<?php /* FIM - EXIBIÇÃO DA OBSERVAÇÃO DO INDICADOR */ 

/* INÍCIO - EXIBIÇÃO DA OBSERVAÇÃO DO INDICADOR */?>
<?php if($exibeResponsavel && $responsaveis){?>
		<tr bgcolor="#e9e9e9" >
			<th style="color:#888888;text-align:justify;font-size:11px;font-weight: normal" colspan="<?php echo $coslpanGeral ?>"><?=$responsaveis?></th>
		</tr>
<?php } ?>
<?php /* FIM - EXIBIÇÃO DA OBSERVAÇÃO DO INDICADOR */ ?>

<?php	
$arrPeriodos = !$arrPeriodos ? array() : $arrPeriodos;

if($arrPeriodos == null){?>
	<tr bgcolor="#e9e9e9" >
		<td style="color:#880000;text-align:center;font-size:11px;font-weight: bold" colspan="<?php echo $coslpanGeral ?>">Não Atendido.</td>
	</tr>
<?php return false;	
}?>

<?php /* INICIO - EXIBIÇÃO DO DETALHES 1 */ ?>
<tr bgcolor="#e9e9e9" >
			<?php $perunidade = $db->pegaUm( "select perunidade from painel.periodicidade where perid = ".($periodo ? $periodo : $perid)."" ) ?>
			<th rowspan="3" style="font-weight:bold;text-align:center;color:#0000AA;"><?=$perunidade?></th>
			
			<?php foreach ( $arrDetalhes1 as $detalhe1){/* Inicio foreach detalhe1 */ ?>
				<th  colspan=<?=($count2 * $QtdValor)?> style="font-weight:bold;text-align:center"><?=$detalhe1['tiddsc']?></th>
			<?php }/* Fim foreach detalhe1 */ ?>
			
			<?php /* INICIO - EXIBIÇÃO DE TOTALIZADORES */ ?>
			<?php if($arrTotalizadores == array(1,1) || $arrTotalizadores == array(1,0)){ ?>
				<th rowspan="2" <?php echo ( $QtdValor != 1 ? "colspan='$QtdValor'" : "") ?> style="font-weight:bold;text-align:center">Total</th>
			<?php } ?>
			<?php /* FIM - EXIBIÇÃO DE TOTALIZADORES */ ?>
			
</tr>
<?php /* FIM - EXIBIÇÃO DO DETALHES 1 */ ?>

<?php /* INICIO - EXIBIÇÃO DO DETALHES 2 */ ?>
<tr bgcolor="#e9e9e9" >
	<?php foreach ( $arrDetalhes1 as $detalhe1){/* Inicio foreach detalhe1 */ ?>
			<?php foreach ( $arrDetalhes2 as $detalhe2){/* Inicio foreach detalhe2 */ ?>
				<th  colspan=<?=$QtdValor?> style="font-weight:bold;text-align:center"><?=$detalhe2['tiddsc']?></th>
			<?php }/* Fim foreach detalhe2 */ ?>
	<?php }/* Fim foreach detalhe1 */ ?>
</tr>
<?php /* FIM - EXIBIÇÃO DO DETALHES 2 */ ?>

<?php /* INCIO - EXIBIÇÃO DOS CAMPOS */?>
<tr bgcolor="#e9e9e9" >
<?php for($i=0;$i < ($count1 * $count2); $i++){?>
	<?php if($exibirRegionalizador && $campoReg!= ""){?>
		<th style="text-align:center" ><?=$regunidade?>*</th>
	<?php }?>
	<?php if($ExibeQtd){?>
		<th style="text-align:center" ><?=$umedesc?></th>
	<?php }?>
	<?php if($ExibeValor){?>
		<th style="text-align:center" >Valor (R$)</th>
	<?php }?>
<?php } ?>

	<?php /* INICIO - EXIBIÇÃO DE TOTALIZADORES */ ?>
	<?php if($arrTotalizadores == array(1,1) || $arrTotalizadores == array(1,0)){ ?>
		<?php if($arrTipoDetalhe != array("naoexibir")){?>
			<?php if($exibirRegionalizador && $campoReg!= ""){?>
				<th style="text-align:center" ><?=$regunidade?>*</th>
			<?php }?>
			<?php if($ExibeQtd){?>
				<th style="text-align:center" ><?=$umedesc?></th>
			<?php }?>
			<?php if($ExibeValor){?>
				<th style="text-align:center" >Valor (R$)</th>
			<?php }?>
		<?php }?>
	<?php }?>
	<?php /* FIM - EXIBIÇÃO DE TOTALIZADORES */ ?>

</tr>
</thead>
<?php /* FIM - EXIBIÇÃO DOS CAMPOS */ ?>

<?php /* INCIO - EXIBIÇÃO DOS PERIODOS */?>
<?php $mascara = pegarMascaraIndicador($indid); $num = 0;
	$arrExcel = array(); ?>
	
<?php foreach($arrPeriodos as $dpedid => $dpedsc){
	$arrRegistroExcel = array();
	$arrRegistroExcel [$perunidade.'/'.$dpedsc] = $dpedsc;
	
	$cor = ($num%2) ? "#f7f7f7" : "";
				$num++;
				?>
	<tr bgcolor="<?=$cor?>" onmouseout="this.bgColor='<?=$cor?>';" onmouseover="this.bgColor='#ffffcc';">
		<td style="font-weight:bold;text-align:center;color:#0000AA" ><?=$dpedsc?></td>
		
		<?php if(count($arrTipoDetalhe) == 1 && $arrTipoDetalhe != array("naoexibir")){ /* Inicio 1 IF */ ?>
				
				<?php /* INICIO - EXIBIÇÃO DO DETALHES 1 */ ?>
						
						<?php $totalQtde = 0; $totalValor = 0;?>
										
					<?php foreach ( $arrDetalhes1 as $detalhe1){/* Inicio foreach detalhe1 */ ?>
						
								<?php if($exibirRegionalizador && $campoReg!= ""){?>
									<td style="text-align:right" >
										<?php echo ((!$arrValor[$dpedid][$detalhe1['tidid']]['regionalizador']) ? "-" : number_format($arrValor[$dpedid][$detalhe1['tidid']]['regionalizador'],0,3,"."));
										$arrRegistroExcel [$detalhe1['tiddsc'].'/'.$regunidade]= ((!$arrValor[$dpedid][$detalhe1['tidid']]['regionalizador']) ? "-" : number_format($arrValor[$dpedid][$detalhe1['tidid']]['regionalizador'],0,3,"."));
										?>
									</td>
								<?php }?>
								<?php if($ExibeQtd){?>
									<td style="text-align:right" ><?php echo !$arrValor[$dpedid][$detalhe1['tidid']]['qtde'] ? "-" :  mascaraglobal( str_replace(".00", "", $arrValor[$dpedid][$detalhe1['tidid']]['qtde']) , $mascara['mascara'] )  ?></td>
									<? $arrRegistroExcel [$detalhe1['tiddsc'].'/'.$umedesc]= !$arrValor[$dpedid][$detalhe1['tidid']]['qtde'] ? "-" :  mascaraglobal( str_replace(".00", "", $arrValor[$dpedid][$detalhe1['tidid']]['qtde']) , $mascara['mascara'] ); ?>
									<?php $totalQtde += $arrValor[$dpedid][$detalhe1['tidid']]['qtde']?>
									<?php $totalGeral[$detalhe1['tidid']]['qtde'] += $arrValor[$dpedid][$detalhe1['tidid']]['qtde']?>
								<?php }?>
								<?php if($ExibeValor){?>
									<td style="text-align:right" ><?php echo !$arrValor[$dpedid][$detalhe1['tidid']]['valor'] ? "-" : mascaraglobal( str_replace(".00", "", $arrValor[$dpedid][$detalhe1['tidid']]['valor']) , $mascara['campovalor']['mascara'] ) ?></td>
									<? $arrRegistroExcel [$detalhe1['tiddsc'].'/'.'Valor (R$)']= !$arrValor[$dpedid][$detalhe1['tidid']]['valor'] ? "-" : mascaraglobal( str_replace(".00", "", $arrValor[$dpedid][$detalhe1['tidid']]['valor']) , $mascara['campovalor']['mascara'] );?>
									<?php $totalValor += $arrValor[$dpedid][$detalhe1['tidid']]['valor']?>
									<?php $totalGeral[$detalhe1['tidid']]['valor'] += $arrValor[$dpedid][$detalhe1['tidid']]['valor']?>
								<?php }?>
								
					<?php }/* Fim foreach detalhe1 */ ?>
					
				<?php /* FIM - EXIBIÇÃO DO DETALHES 1 */ ?>
				
				<?php /* INICIO - EXIBIÇÃO DE TOTALIZADORES */ ?>
				<?php if($arrTotalizadores == array(1,1) || $arrTotalizadores == array(1,0)){ ?>
					<?php if($exibirRegionalizador && $campoReg!= ""){?>
					<?php					
						$sql = "select 
									count( distinct $campoReg) as reg
								from 
									painel.v_detalheindicadorsh d
								where
									d.indid = $indid
								and
									d.dpedatainicio >= ( select dpedatainicio from painel.detalheperiodicidade where dpeid = $dpedid )
								and
									d.dpedatafim <= ( select dpedatafim from painel.detalheperiodicidade where dpeid = $dpedid )
								$andDpe
								".(count($and) ? " and ".implode(" and ",$and) : "" )."
								group by
									d.indid";
						//dbg($sql);
						$totalReg = $db->pegaUm($sql);
						$arrRegistroExcel ['Total/'.$regunidade] = number_format($totalReg,0,3,"."); ?>
						<td style="text-align:right;font-weight:bold" ><?=number_format($totalReg,0,3,".")?></td>
					<?php }?>
					<?php if($ExibeQtd){?>
						<td style="text-align:right;font-weight:bold" ><?=mascaraglobal( str_replace(".00", "", $totalQtde) , $mascara['mascara'] )?></td>
						<?php $totalGeralQtde += $totalQtde;
							$arrRegistroExcel ['Total/'.$umedesc] = mascaraglobal( str_replace(".00", "", $totalQtde) , $mascara['mascara'] );
						?>
					<?php }?>
					<?php if($ExibeValor){?>
						<td style="text-align:right;font-weight:bold" ><?=mascaraglobal( str_replace(".00", "", $totalValor) , $mascara['campovalor']['mascara'] )?></td>
						<?php $totalGeralValor += $totalValor;
						$arrRegistroExcel ['Valor (R$)/'.$umedesc] = mascaraglobal( str_replace(".00", "", $totalValor) , $mascara['campovalor']['mascara'] ); ?>
					<?php }?>
				<?php } ?>
				<?php /* FIM - EXIBIÇÃO DE TOTALIZADORES */ ?>
				
		<?php } /* Fim 1 IF */?>
		
		<?php if(count($arrTipoDetalhe) == 2){ /* Inicio 2 IF */ ?>
				
				<?php /* INICIO - EXIBIÇÃO DO DETALHES 2 */ ?>
					
					<?php $totalQtde = 0; $totalValor = 0;?>
				
					<?php foreach ( $arrDetalhes1 as $detalhe1){/* Inicio foreach detalhe1 */ ?>
							<?php foreach ( $arrDetalhes2 as $detalhe2){/* Inicio foreach detalhe2 */ ?>
								<?php if($exibirRegionalizador && $campoReg!= ""){?>
									<td style="text-align:right" ><?php echo !$arrValor[$dpedid][$detalhe1['tidid']][$detalhe2['tidid']]['regionalizador'] ? "-" : $arrValor[$dpedid][$detalhe1['tidid']][$detalhe2['tidid']]['regionalizador'] ?></td>
									<? $arrRegistroExcel [$detalhe1['tiddsc'].'/'.$detalhe2['tiddsc'].'/'.$regunidade] = !$arrValor[$dpedid][$detalhe1['tidid']][$detalhe2['tidid']]['regionalizador'] ? "-" : $arrValor[$dpedid][$detalhe1['tidid']][$detalhe2['tidid']]['regionalizador']; ?>
								<?php }?>
								<?php if($ExibeQtd){?>
									<td style="text-align:right" ><?php echo !$arrValor[$dpedid][$detalhe1['tidid']][$detalhe2['tidid']]['qtde'] ? "-" :  mascaraglobal( str_replace(".00", "", $arrValor[$dpedid][$detalhe1['tidid']][$detalhe2['tidid']]['qtde']) , $mascara['mascara'] )  ?></td>
									<?php $totalQtde += $arrValor[$dpedid][$detalhe1['tidid']][$detalhe2['tidid']]['qtde']?>
									<?php $totalGeral[$detalhe1['tidid']][$detalhe2['tidid']]['qtde'] += $arrValor[$dpedid][$detalhe1['tidid']][$detalhe2['tidid']]['qtde']?>
									<? $arrRegistroExcel [$detalhe1['tiddsc'].'/'.$detalhe2['tiddsc'].'/'.$umedesc] = !$arrValor[$dpedid][$detalhe1['tidid']][$detalhe2['tidid']]['qtde'] ? "-" :  mascaraglobal( str_replace(".00", "", $arrValor[$dpedid][$detalhe1['tidid']][$detalhe2['tidid']]['qtde']) , $mascara['mascara'] ); ?>
								<?php }?>
								<?php if($ExibeValor){?>
									<td style="text-align:right" ><?php echo !$arrValor[$dpedid][$detalhe1['tidid']][$detalhe2['tidid']]['valor'] ? "-" : mascaraglobal( str_replace(".00", "", $arrValor[$dpedid][$detalhe1['tidid']][$detalhe2['tidid']]['valor']) , $mascara['campovalor']['mascara'] ) ?></td>
									<?php $totalValor += $arrValor[$dpedid][$detalhe1['tidid']][$detalhe2['tidid']]['valor']?>
									<?php $totalGeral[$detalhe1['tidid']][$detalhe2['tidid']]['valor'] += $arrValor[$dpedid][$detalhe1['tidid']][$detalhe2['tidid']]['valor']?>
									<? $arrRegistroExcel [$detalhe1['tiddsc'].'/'.$detalhe2['tiddsc'].'/Valor (R$)'] = !$arrValor[$dpedid][$detalhe1['tidid']][$detalhe2['tidid']]['valor'] ? "-" : mascaraglobal( str_replace(".00", "", $arrValor[$dpedid][$detalhe1['tidid']][$detalhe2['tidid']]['valor']) , $mascara['campovalor']['mascara'] ); ?>
								<?php }?>
							<?php }/* Fim foreach detalhe2 */ ?>
					<?php } /* Fim foreach detalhe1 */ ?>
				<?php /* FIM - EXIBIÇÃO DO DETALHES 2 */ ?>
				
				<?php /* INICIO - EXIBIÇÃO DE TOTALIZADORES */ ?>
				<?php if($arrTotalizadores == array(1,1) || $arrTotalizadores == array(1,0)){ ?>
					<?php if($exibirRegionalizador && $campoReg!= ""){?>
					<?php					
						$sql = "select 
									count( distinct $campoReg) as reg
								from 
									painel.v_detalheindicadorsh d
								where
									d.indid = $indid
								and
									d.dpedatainicio >= ( select dpedatainicio from painel.detalheperiodicidade where dpeid = $dpedid )
								and
									d.dpedatafim <= ( select dpedatafim from painel.detalheperiodicidade where dpeid = $dpedid )
								$andDpe
								".(count($and) ? " and ".implode(" and ",$and) : "" )."
								group by
									d.indid";
						//dbg($sql);
						$totalReg = $db->pegaUm($sql); 
						$arrRegistroExcel ['Total/'.$regunidade] = number_format($totalReg,0,3,".");?>
						<td style="text-align:right;font-weight:bold" ><?=number_format($totalReg,0,3,".")?></td>
					<?php }?>
					<?php if($ExibeQtd){?>
						<td style="text-align:right;font-weight:bold" ><?=mascaraglobal( str_replace(".00", "", $totalQtde) , $mascara['mascara'] )?></td>
						<?php $totalGeralQtde += $totalQtde?>
						<? $arrRegistroExcel ['Total/'.$umedesc] = mascaraglobal( str_replace(".00", "", $totalQtde) , $mascara['mascara'] ); ?>
					<?php }?>
					<?php if($ExibeValor){?>
						<td style="text-align:right;font-weight:bold" ><?=mascaraglobal( str_replace(".00", "", $totalValor) , $mascara['campovalor']['mascara'] )?></td>
						<?php $totalGeralValor += $totalValor?>
						<? $arrRegistroExcel ['Valor (R$)/'.$regunidade] = mascaraglobal( str_replace(".00", "", $totalValor) , $mascara['campovalor']['mascara'] );?>
					<?php }?>
				<?php } ?>
				<?php /* FIM - EXIBIÇÃO DE TOTALIZADORES */ ?>
				
		<?php } /* Fim 2 IF */?>
		
		<?php if($arrTipoDetalhe == array("naoexibir")){ /* Inicio 3 IF */ ?>
				
				<?php /* INICIO - EXIBIÇÃO DE TOTALIZADORES */ ?>
				<?php if($arrTotalizadores == array(1,1) || $arrTotalizadores == array(1,0)){ ?>
					<?php if($exibirRegionalizador && $campoReg!= ""){?>
						<?php					
							$sql = "select 
										count( distinct $campoReg) as reg
									from 
										painel.v_detalheindicadorsh d
									where
										d.indid = $indid
									and
										d.dpedatainicio >= ( select dpedatainicio from painel.detalheperiodicidade where dpeid = $dpedid )
									and
										d.dpedatafim <= ( select dpedatafim from painel.detalheperiodicidade where dpeid = $dpedid )
									$andDpe
									".(count($and) ? " and ".implode(" and ",$and) : "" )."
									group by
										d.indid";
							$totalReg = $db->pegaUm($sql);
							//dbg($sql);
							$arrRegistroExcel ['Total/'.$regunidade] = number_format($totalReg,0,3,".");?>
						<td style="text-align:right;font-weight:bold" ><?=number_format($totalReg,0,3,".")?></td>
					<?php }?>
					<?php if($ExibeQtd){?>
						<td style="text-align:right;font-weight:bold" ><?=mascaraglobal( str_replace(".00", "", $arrValor[$dpedid]['qtde']) , $mascara['mascara'] )?></td>
						<?php $totalGeralQtde += $arrValor[$dpedid]['qtde']?>
						<? $arrRegistroExcel ['Total/'.$umedesc] = mascaraglobal( str_replace(".00", "", $arrValor[$dpedid]['qtde']) , $mascara['mascara'] );?>
					<?php }?>
					<?php if($ExibeValor){?>
						<td style="text-align:right;font-weight:bold" ><?=mascaraglobal( str_replace(".00", "", $arrValor[$dpedid]['valor']) , $mascara['campovalor']['mascara'] )?></td>
						<?php $totalGeralValor += $arrValor[$dpedid]['valor']?>
						<? $arrRegistroExcel ['Valor (R$)/'] = mascaraglobal( str_replace(".00", "", $arrValor[$dpedid]['valor']) , $mascara['campovalor']['mascara'] );?>
					<?php }?>
				<?php } ?>
				<?php /* FIM - EXIBIÇÃO DE TOTALIZADORES */ ?>
				
		<?php } /* Fim 3 IF */?>
		<?php array_push($arrExcel, $arrRegistroExcel); ?>
</tr>
<?php } ?>

<?php /* FIM - EXIBIÇÃO DOS PERIODOS */?>

<?php $arrTotalizadores = count($arrPeriodos) == 1 ? array(0,0) : $arrTotalizadores; ?>

<?php /* INICIO - EXIBIÇÃO DOS TOTALIZADORES GERAIS */ 
$arrRegistroExcel = array();?>
<?php if(($arrTotalizadores == array(1,1) || $arrTotalizadores == array(1,0) || $arrTotalizadores == array(0,1)) &&  ( ($indcumulativo == "S" || $indcumulativo == "A") || ($indcumulativovalor == "S" || $indcumulativovalor == "A") ) ){ ?>
	<tr bgcolor="#e9e9e9" >
	<td style="text-align:right;font-weight:bold" >Total Geral</td>
	<?php if($arrTipoDetalhe == array("naoexibir")){ /* Inicio 3 IF */ ?>
	<?$arrRegistroExcel['Total Geral/'] = 'Total Geral'; ?>
	<?php if($exibirRegionalizador && $campoReg!= ""){ ?>
					
					<?php $sql = "select 
										count( distinct $campoReg) as reg
									from 
										painel.v_detalheindicadorsh d
									where
										d.indid = $indid
									$andDpe
									".(count($and) ? " and ".implode(" and ",$and) : "" )."
									".(count($arrDpeReg) ? " and ".implode(" and ",$arrDpeReg) : "" )."
									group by
										d.indid";
					//dbg($sql);
					$totalGeralReg = $db->pegaUm($sql);
					$arrRegistroExcel ['Total Geral/'.$regunidade] = number_format($totalGeralReg,0,3,".");
					?>
						<td style="text-align:right;font-weight:bold" ><?=number_format($totalGeralReg,0,3,".")?></td>
					<?php }?>
		<?php if($ExibeQtd){?>
			<td style="text-align:right;font-weight:bold" >
				<?php if($indcumulativo == "S"){ ?>
					<?=mascaraglobal( str_replace(".00", "", $totalGeralQtde ), $mascara['mascara'] )?>
					<? $arrRegistroExcel ['Total Geral/'.$umedesc] = mascaraglobal( str_replace(".00", "", $totalGeralQtde ), $mascara['mascara'] ); ?>
				<?php }elseif($indcumulativo == "A" && ($periodo ? $periodo : $perid) == PERIODO_ANUAL){ ?>
					<?=mascaraglobal( str_replace(".00", "", $arrValor[$dpedid]['qtde'] ), $mascara['mascara'] )?>
					<? $arrRegistroExcel ['Total Geral/'.$umedesc] = mascaraglobal( str_replace(".00", "", $arrValor[$dpedid]['qtde'] ), $mascara['mascara'] ); ?>
				<?php }else{ ?>
					-
					<? $arrRegistroExcel ['Total Geral/'.$umedesc] = '-'; ?>
				<?php }?>
			</td>
		<?php }?>
		<?php if($ExibeValor){?>
			<td style="text-align:right;font-weight:bold" >
				<?php if($indcumulativovalor == "S"){ ?>
					<?=mascaraglobal( str_replace(".00", "", $totalGeralValor) , $mascara['campovalor']['mascara'] )?>
					<? $arrRegistroExcel ['Total Geral/Valor (R$)'] = mascaraglobal( str_replace(".00", "", $totalGeralValor) , $mascara['campovalor']['mascara'] ); ?>
				<?php }elseif($indcumulativovalor == "A" && ($periodo ? $periodo : $perid) == PERIODO_ANUAL){ ?>
					<?=mascaraglobal( str_replace(".00", "", $arrValor[$dpedid]['valor']) , $mascara['campovalor']['mascara'])?>
					<? $arrRegistroExcel ['Total Geral/Valor (R$)'] = mascaraglobal( str_replace(".00", "", $arrValor[$dpedid]['valor']) , $mascara['campovalor']['mascara']); ?>
				<?php }else{ ?>
					-
					<? $arrRegistroExcel ['Total Geral/Valor (R$)'] = '-'; ?>
				<?php }?>
			</td>
		<?php }?>
		<? //array_push($arrExcel, $arrRegistroExcel); ?>
	<?php }else{ ?>
		<? $arrRegistroExcel = array(); ?>
		<?$arrRegistroExcel ['Total Geral/'] = 'Total Geral'; ?>
		<?php if(count($arrTipoDetalhe) == 1 && $arrTipoDetalhe != array("naoexibir")){ /* Inicio 1 IF */ ?>
					
					<?php /* INICIO - EXIBIÇÃO DO DETALHES 1 */ ?>
							
						<?php foreach ( $arrDetalhes1 as $detalhe1){/* Inicio foreach detalhe1 */ ?>
							
									<?php if($exibirRegionalizador && $campoReg!= ""){
										
											$sql = "select 
														tdinumero 
													from 
														painel.detalhetipoindicador d1
													inner join
														painel.detalhetipodadosindicador d2 ON d1.tdiid = d2.tdiid 
													where 
														tidid = {$detalhe1['tidid']}";
											$tdinumero = $db->pegaUm($sql);
										
											$sql = "select 
														count( distinct $campoReg) as reg
													from 
														painel.v_detalheindicadorsh d
													where
														 tidid$tdinumero = {$detalhe1['tidid']}
													and
														indid = $indid
													$andDpe
													".(count($andReg) ? " and ".implode(" and ",$andReg) : "" )."
													group by
														d.indid";
											//dbg($sql);
											$regionalizador = $db->pegaUm($sql);
											$arrRegistroExcel ['Total Geral/'.$detalhe1['tiddsc'].'/'.$regunidade] = number_format($regionalizador,0,3,".");
											?>
											<td style="text-align:right;font-weight:bold" ><?=number_format($regionalizador,0,3,".")?> </td>
										
									<?php }?>
									<?php if($ExibeQtd){?>
										<td style="text-align:right;font-weight:bold" >
										<?php if($indcumulativo == "S"){ ?>
											<?php echo !$totalGeral[$detalhe1['tidid']]['qtde'] ? "-" :  mascaraglobal( str_replace(".00", "", $totalGeral[$detalhe1['tidid']]['qtde']) , $mascara['mascara'] )  ?>
											<?php $TotalGeralQtde+=  !$totalGeral[$detalhe1['tidid']]['qtde'] ? 0 : $totalGeral[$detalhe1['tidid']]['qtde']?>
											<? $arrRegistroExcel ['Total Geral/'.$detalhe1['tiddsc'].'/'.$umedesc] = !$totalGeral[$detalhe1['tidid']]['qtde'] ? "-" :  mascaraglobal( str_replace(".00", "", $totalGeral[$detalhe1['tidid']]['qtde']) , $mascara['mascara']); ?>
										<?php }elseif($indcumulativo == "A" && ($periodo ? $periodo : $perid) == PERIODO_ANUAL){ ?>
											<?=!$arrValor[$dpedid][$detalhe1['tidid']]['qtde'] ? "-" : mascaraglobal( str_replace(".00", "", $arrValor[$dpedid][$detalhe1['tidid']]['qtde']) , $mascara['mascara'] )?>
											<?php $TotalGeralQtde+=  !$arrValor[$dpedid][$detalhe1['tidid']]['qtde'] ? 0 : $arrValor[$dpedid][$detalhe1['tidid']]['qtde']?>
											<? $arrRegistroExcel ['Total Geral/'.$detalhe1['tiddsc'].'/'.$umedesc] = !$arrValor[$dpedid][$detalhe1['tidid']]['qtde'] ? "-" : mascaraglobal( str_replace(".00", "", $arrValor[$dpedid][$detalhe1['tidid']]['qtde']) , $mascara['mascara'] ); ?>
										<?php }else{ ?>
											-
											<? $arrRegistroExcel ['Total Geral/'.$detalhe1['tiddsc'].'/'.$umedesc] = '-'; ?>
											<?php $TotalGeralQtde+= 0?>
										<?php }?>
										</td>
									<?php }?>
									<?php if($ExibeValor){?>
										<td style="text-align:right;font-weight:bold" >
										<?php if($indcumulativovalor == "S"){ ?>
											<?php echo !$totalGeral[$detalhe1['tidid']]['valor'] ? "-" : mascaraglobal( str_replace(".00", "", $totalGeral[$detalhe1['tidid']]['valor']) , $mascara['campovalor']['mascara'] ) ?>
											<?php $TotalGeralValor+= !$totalGeral[$detalhe1['tidid']]['valor'] ? 0 : $totalGeral[$detalhe1['tidid']]['valor']?>
											<? $arrRegistroExcel ['Total Geral/'.$detalhe1['tiddsc'].'/'.'Valor (R$)'] = !$totalGeral[$detalhe1['tidid']]['valor'] ? "-" : mascaraglobal( str_replace(".00", "", $totalGeral[$detalhe1['tidid']]['valor']) , $mascara['campovalor']['mascara'] ); ?>
										<?php }elseif($indcumulativovalor == "A" && ($periodo ? $periodo : $perid) == PERIODO_ANUAL){ ?>
											<?=!$arrValor[$dpedid][$detalhe1['tidid']]['valor'] ? "-" : mascaraglobal( str_replace(".00", "", $arrValor[$dpedid][$detalhe1['tidid']]['valor']) , $mascara['campovalor']['mascara'] )?>
											<?php $TotalGeralValor+= !$arrValor[$dpedid][$detalhe1['tidid']]['valor'] ? 0 : $arrValor[$dpedid][$detalhe1['tidid']]['valor']?>
											<? $arrRegistroExcel ['Total Geral/'.$detalhe1['tiddsc'].'/Valor (R$)'] = !$arrValor[$dpedid][$detalhe1['tidid']]['valor'] ? "-" : mascaraglobal( str_replace(".00", "", $arrValor[$dpedid][$detalhe1['tidid']]['valor']) , $mascara['campovalor']['mascara'] ); ?>
										<?php }else{ ?>
											-
											<? $arrRegistroExcel ['Total Geral/'.$detalhe1['tiddsc'].'/Valor (R$)'] = '-'; ?>
											<?php $TotalGeralValor+= 0?>
										<?php } ?>
										</td>
										<?php $totalValor += $arrValor[$dpedid][$detalhe1['tidid']]['valor']?>
									<?php } ?>
									
						<?php }/* Fim foreach detalhe1 */ ?>
						<? //array_push($arrExcel, $arrRegistroExcel); ?>
					<?php /* FIM - EXIBIÇÃO DO DETALHES 1 */ ?>
					
									
			<?php } /* Fim 1 IF */?>
			
			
			<?php if(count($arrTipoDetalhe) == 2){ /* Inicio 2 IF */ ?>
					
					<?php /* INICIO - EXIBIÇÃO DO DETALHES 2 */ ?>
						
						<?php $totalQtde = 0; $totalValor = 0;?>
					
						<?php foreach ( $arrDetalhes1 as $detalhe1){/* Inicio foreach detalhe1 */ ?>
								<?php foreach ( $arrDetalhes2 as $detalhe2){/* Inicio foreach detalhe2 */ ?>
									<?php if($exibirRegionalizador && $campoReg!= ""){
										
											$sql = "select 
														tdinumero 
													from 
														painel.detalhetipoindicador d1
													inner join
														painel.detalhetipodadosindicador d2 ON d1.tdiid = d2.tdiid 
													where 
														tidid = {$detalhe1['tidid']}";
											$tdinumero1 = $db->pegaUm($sql);
											
											$sql = "select 
														tdinumero 
													from 
														painel.detalhetipoindicador d1
													inner join
														painel.detalhetipodadosindicador d2 ON d1.tdiid = d2.tdiid 
													where 
														tidid = {$detalhe2['tidid']}";
											$tdinumero2 = $db->pegaUm($sql);
										
											$sql = "select 
														count( distinct $campoReg) as reg
													from 
														painel.v_detalheindicadorsh d
													where
														 tidid$tdinumero1 = {$detalhe1['tidid']}
													and
														 tidid$tdinumero2 = {$detalhe2['tidid']}
													and
														d.indid = $indid
													$andDpe
													".(count($andReg) ? " and ".implode(" and ",$andReg) : "" )."
													group by
														d.indid";
											//dbg($sql);
											$regionalizador = $db->pegaUm($sql);
											$arrRegistroExcel ['Total Geral/'.$detalhe1['tiddsc'].'/'.$detalhe2['tiddsc'].'/'.$regunidade] = number_format($regionalizador,0,3,"."); 
											?>
											<td style="text-align:right;font-weight:bold" ><?=number_format($regionalizador,0,3,".")?></td>
									<?php }?>
									<?php if($ExibeQtd){?>
										<td style="text-align:right;font-weight:bold" >
										<?php if($indcumulativo == "S"){ ?>
											<?php echo !$totalGeral[$detalhe1['tidid']][$detalhe2['tidid']]['qtde'] ? "-" :  mascaraglobal( str_replace(".00", "", $totalGeral[$detalhe1['tidid']][$detalhe2['tidid']]['qtde']) , $mascara['mascara'] )  ?>
											<?php $TotalGeralQtde+= !$totalGeral[$detalhe1['tidid']][$detalhe2['tidid']]['qtde'] ? 0 : $totalGeral[$detalhe1['tidid']][$detalhe2['tidid']]['qtde']?>
											<?$arrRegistroExcel ['Total Geral/'.$detalhe1['tiddsc'].'/'.$detalhe2['tiddsc'].'/'.$umedesc] = !$totalGeral[$detalhe1['tidid']][$detalhe2['tidid']]['qtde'] ? "-" :  mascaraglobal( str_replace(".00", "", $totalGeral[$detalhe1['tidid']][$detalhe2['tidid']]['qtde']) , $mascara['mascara'] ); ?>
										<?php }elseif($indcumulativo == "A" && ($periodo ? $periodo : $perid) == PERIODO_ANUAL){ ?>
											<?=!$arrValor[$dpedid][$detalhe1['tidid']][$detalhe2['tidid']]['qtde'] ? "-" : mascaraglobal( str_replace(".00", "", $arrValor[$dpedid][$detalhe1['tidid']][$detalhe2['tidid']]['qtde']) , $mascara['mascara'] )?>
											<?php $TotalGeralQtde+= !$arrValor[$dpedid][$detalhe1['tidid']][$detalhe2['tidid']]['qtde'] ? 0 : $arrValor[$dpedid][$detalhe1['tidid']][$detalhe2['tidid']]['qtde']?>
											<? $arrRegistroExcel ['Total Geral/'.$detalhe1['tiddsc'].'/'.$detalhe2['tiddsc'].'/'.$umedesc] = !$arrValor[$dpedid][$detalhe1['tidid']][$detalhe2['tidid']]['qtde'] ? "-" : mascaraglobal( str_replace(".00", "", $arrValor[$dpedid][$detalhe1['tidid']][$detalhe2['tidid']]['qtde']) , $mascara['mascara'] ); ?>
										<?php }else{ ?>
											-
											<?php $TotalGeralQtde+= 0?>
											<? $arrRegistroExcel ['Total Geral/'.$detalhe1['tiddsc'].'/'.$detalhe2['tiddsc'].'/'.$umedesc] = '-'; ?>
										<?php }?>
										</td>
										<?php $totalQtde += $arrValor[$dpedid][$detalhe1['tidid']][$detalhe2['tidid']]['qtde']?>
									<?php }?>
									<?php if($ExibeValor){?>
										<td style="text-align:right;font-weight:bold" >
										<?php if($indcumulativovalor == "S"){ ?>
											<?php echo !$totalGeral[$detalhe1['tidid']][$detalhe2['tidid']]['valor'] ? "-" : mascaraglobal( str_replace(".00", "", $totalGeral[$detalhe1['tidid']][$detalhe2['tidid']]['valor']) , $mascara['campovalor']['mascara'] ) ?>
											<?php $TotalGeralValor+= !$totalGeral[$detalhe1['tidid']][$detalhe2['tidid']]['valor'] ? 0 : $totalGeral[$detalhe1['tidid']][$detalhe2['tidid']]['valor']?>
											<? $arrRegistroExcel ['Total Geral/'.$detalhe1['tiddsc'].'/'.$detalhe2['tiddsc'].'/Valor (R$)'] = !$totalGeral[$detalhe1['tidid']][$detalhe2['tidid']]['valor'] ? "-" : mascaraglobal( str_replace(".00", "", $totalGeral[$detalhe1['tidid']][$detalhe2['tidid']]['valor']) , $mascara['campovalor']['mascara'] ); ?>
										<?php }elseif($indcumulativovalor == "A" && ($periodo ? $periodo : $perid) == PERIODO_ANUAL){ ?>
											<?=!$arrValor[$dpedid][$detalhe1['tidid']][$detalhe2['tidid']]['valor'] ? "-" : mascaraglobal( str_replace(".00", "", $arrValor[$dpedid][$detalhe1['tidid']][$detalhe2['tidid']]['valor']) , $mascara['campovalor']['mascara'] )?>
											<?php $TotalGeralValor+= !$arrValor[$dpedid][$detalhe1['tidid']][$detalhe2['tidid']]['valor'] ? 0 : $arrValor[$dpedid][$detalhe1['tidid']][$detalhe2['tidid']]['valor']?>
											<? $arrRegistroExcel ['Total Geral/'.$detalhe1['tiddsc'].'/'.$detalhe2['tiddsc'].'/Valor (R$)'] = !$arrValor[$dpedid][$detalhe1['tidid']][$detalhe2['tidid']]['valor'] ? "-" : mascaraglobal( str_replace(".00", "", $arrValor[$dpedid][$detalhe1['tidid']][$detalhe2['tidid']]['valor']) , $mascara['campovalor']['mascara'] ); ?>
										<?php }else{ ?>
											-
											<?php $TotalGeralValor+= 0?>
											<? $arrRegistroExcel ['Total Geral/'.$detalhe1['tiddsc'].'/'.$detalhe2['tiddsc'].'/Valor (R$)'] = '-'; ?>
										<?php }?>
										</td>
									<?php }?>
								<?php }/* Fim foreach detalhe2 */ ?>
						<?php }/* Fim foreach detalhe1 */ ?>
					<?php /* FIM - EXIBIÇÃO DO DETALHES 2 */ ?>
					
			<?php } /* Fim 2 IF */?>
		<?php if($arrTotalizadores == array(1,1) || $arrTotalizadores == array(1,0)){?>
			<?php if($exibirRegionalizador && $campoReg!= ""){?>
						
						<?php $sql = "select 
											count( distinct $campoReg) as reg
										from 
											painel.v_detalheindicadorsh d
										where
											indid = $indid
										$andDpe
										".(count($and) ? " and ".implode(" and ",$and) : "" )."
										".(count($arrDpeReg) ? " and ".implode(" and ",$arrDpeReg) : "" )."
										group by
											d.indid";
						//dbg($sql);
						$totalGeralReg = $db->pegaUm($sql);
						$arrRegistroExcel ['Total Geral/'.$regunidade] = number_format($totalGeralReg,0,3,".");
						?>
							<td style="text-align:right;font-weight:bold" ><?=number_format($totalGeralReg,0,3,".")?></td>
						<?php }?>
			<?php if($ExibeQtd){?>
				<td style="text-align:right;font-weight:bold" ><?=!$TotalGeralQtde || $TotalGeralQtde == 0 ? "-" : mascaraglobal( str_replace(".00", "", $TotalGeralQtde ), $mascara['mascara'] )?></td>
				<? $arrRegistroExcel ['Total Geral/'.$umedesc] = !$TotalGeralQtde || $TotalGeralQtde == 0 ? "-" : mascaraglobal( str_replace(".00", "", $TotalGeralQtde ), $mascara['mascara'] ); ?>
			<?php }?>
			<?php if($ExibeValor){?>
				<td style="text-align:right;font-weight:bold" ><?=!$TotalGeralValor || $TotalGeralValor == 0 ? "-" : mascaraglobal( str_replace(".00", "", $TotalGeralValor) , $mascara['campovalor']['mascara'] )?></td>
				<? $arrRegistroExcel ['Total Geral/Valor (R$)'] = !$TotalGeralValor || $TotalGeralValor == 0 ? "-" : mascaraglobal( str_replace(".00", "", $TotalGeralValor) , $mascara['campovalor']['mascara'] ); ?>
			<?php }?>
			
		<?php } ?>
		<? //array_push($arrExcel, $arrRegistroExcel); ?>
	<?php } ?>
	</tr>
	<? array_push($arrExcel, $arrRegistroExcel); ?>
<?php } ?>
<?php /* FIM - EXIBIÇÃO DOS TOTALIZADORES GERAIS */ ?>

<?php /* INICIO - EXIBIÇÃO DA EXPLICAÇÃO DOS TOTALIZADORES REGIONAIS */ ?>
<?php if($exibirRegionalizador && $campoReg!= "" && ( $arrTotalizadores == array(0,1) || $arrTotalizadores == array(1,1) || $arrTotalizadores == array(1,0) ) ){?>
<tr bgcolor="#e9e9e9" >
	<th style="text-align:left;font-weight:normal" colspan="<?php echo $coslpanGeral ?>">* No cálculo dos totais foram considerada(o)s apenas <?=$regunidade?> distinta(o)s.</th>
</tr>
<?php } ?>
<?php /* FIM - EXIBIÇÃO DA EXPLICAÇÃO DOS TOTALIZADORES REGIONAIS */ ?>

<?php /* INÍCIO - EXIBIÇÃO DA OBSERVAÇÃO DO INDICADOR */?>
<?php if($indfontetermo && $exibeFonte){?>
		<tr bgcolor="#e9e9e9" >
			<td style="text-align:right;font-size:10px;color:#888888" colspan="<?php echo $coslpanGeral ?>">FONTE: <?=$indfontetermo?></td>
		</tr>
<?php } ?>
<?php /* FIM - EXIBIÇÃO DA OBSERVAÇÃO DO INDICADOR */ ?>

</table>
	<?php
	if( $_POST['tipoEvento'] == 'EX' ){
		$count = 0;
		$cabecalhoEx = array();
		foreach ($arrExcel[0] as $key => $valorC) {
			$arrKey = explode('/', $key);
			if( $count == 0 ) $cabecalhoEx[]= $arrKey[0];
			else $cabecalhoEx[]= $key;
			$count++;
		}
		ob_clean();
		//ver($arrExcel);
		$arrExcel = $arrExcel ? $arrExcel : array();
		$indnome = str_ireplace(' ', '_', $indnome);
		$db->sql_to_excel($arrExcel, $indnome, $cabecalhoEx);
	}
}

function pegarMascaraIndicador($indid = null) {
	global $db;
	/*
	 * Verificando o tipo de unidade de medição do indicador
	 * regra 1: se for moeda (unmid=5), o formato dos campos devem ser ###.###.###,##
	 * regra 2: se for Inteiro (unmid=3), verificar se indqtdevalor == true, caso sim, mostrar os dois campos
	 */
	$indid = !$indid ? $_SESSION['indid'] : $indid;
	if($indid) {
		$ind = $db->pegaLinha("SELECT unmid, indqtdevalor FROM painel.indicador WHERE indid='".$indid."'");
		switch($ind['unmid']) {
			case '5':
				$formatoinput = array('mascara'             => '###.###.###.###,##',
									  'size'                => '18',
									  'maxlength'           => '17',
									  'label'               => 'Valor',
									  'unmid'				=> $ind['unmid']);
				break;
			case '1':
				$formatoinput = array('mascara'             => '###.###.###.###,##',
									  'size'                => '18',
									  'maxlength'           => '17',
									  'label'               => 'Valor',
									  'unmid'				=> $ind['unmid']);
				break;
			case '2':
				$formatoinput = array('mascara'             => '###.###.###.###,##',
									  'size'                => '18',
									  'maxlength'           => '17',
									  'label'               => 'Valor',
									  'unmid'				=> $ind['unmid']);
				break;
			case '4':
				$formatoinput = array('mascara'             => '###,##',
									  'size'                => '7',
									  'maxlength'           => '6',
									  'label'               => 'Indíce',
									  'unmid'				=> $ind['unmid']);
				break;
			case '3':
				$formatoinput = array('mascara'             => '###.###.###.###',
									  'size'                => '15',
									  'maxlength'           => '14',
									  'label'               => 'Quantidade',
									  'unmid'				=> $ind['unmid']);
				
				if($ind['indqtdevalor'] == "t") {
					// mostar os dois campos (quantidade e valor)
					$formatoinput['campovalor'] = array('mascara'             => '###.###.###.###,##',
									  					'size'                => '18',
									  					'maxlength'           => '17',
									  					'label'               => 'Valor',
									  					'unmid'				  => $ind['unmid']);
				}
				break;
			default:
				$formatoinput = array('mascara'             => '###.###.###.###',
									  'size'                => '15',
									  'maxlength'           => '14',
									  'label'               => 'Quantidade',
									  'unmid'				=> $ind['unmid']);
		}
		return $formatoinput;
	} else {
		echo "<p align='center'>Problemas na identificação do indicador. <b><a href=\"?modulo=inicio&acao=C\">Clique aqui</a></b> e refaça os procedimentos.</p>";
		//exit;
	}

}

function exibeRelatorioRedeFederalEducacaoSuperior($arrFiltros = array()){
	global $db;
	
	$msgErro = "Não atendido.";
	
	$filtroSQL[] = "tuo.orgid IN('1')";
	
	$filtroSQL[] = "fen.funid IN (18)";
	
	//Filtro por regcod
	if($arrFiltros['regcod'] && $arrFiltros['regcod'] != "" && $arrFiltros['regcod'] != "todos"){
		$filtroSQL[] = "edc.estuf IN ( select estuf from territorios.estado where regcod = '{$arrFiltros['regcod']}' ) ";
		$msgErro = "A região não solicitou recursos deste programa, não preencheu os critérios de atendimento do MEC, ou está com processo em andamento.";
	}
	//Filtro por estuf
	if($arrFiltros['estuf'] && $arrFiltros['estuf'] != "" && $arrFiltros['estuf'] != "todos"){
		$filtroSQL[] = "edc.estuf = '{$arrFiltros['estuf']}'";
		$msgErro = "O estado não solicitou recursos deste programa, não preencheu os critérios de atendimento do MEC, ou está com processo em andamento.";
	}
	//Filtro por tpmid
	if($arrFiltros['tpmid'] && $arrFiltros['tpmid'] != "" && $arrFiltros['tpmid'] != "todos"){
		$filtroSQL[] = "edc.muncod in (	select muncod from territorios.muntipomunicipio where tpmid = '{$arrFiltros['tpmid']}' )";
	}
	//Filtro por muncod
	if($arrFiltros['muncod'] && $arrFiltros['muncod'] != "" && $arrFiltros['muncod'] != "todos"){
		$filtroSQL[] = "edc.muncod = '{$arrFiltros['muncod']}' ";
		$municipio = true;
		$msgErro = "O município não solicitou recursos deste programa, não preencheu os critérios de atendimento do MEC, ou está com processo em andamento.";
	}
	//Filtro por mescod
	if($arrFiltros['mescod'] && $arrFiltros['mescod'] != ""){
		$filtroSQL[] = "mun.mescod  = '{$arrFiltros['mescod']}' ";
		$msgErro = "A mesorregião não solicitou recursos deste programa, não preencheu os critérios de atendimento do MEC, ou está com processo em andamento.";
	}
	
	//Filtro solicitado pelo Vitor a pedido da Manoela (388754)
		$filtroSQL[] = "uor.entid NOT IN (388754) ";
	
	//Filtro solicitado pelo Vitor
		$filtroSQL[] = "uor.entstatus = 'A' AND ent.entstatus = 'A' ";
		
	$sql =  "SELECT  
					mun.mundescricao,
					ent.entnome, 
					uor.entsig,
					uor.entid as ent_pai, 
					uor.entnome as nomeinst,
				 	exi.exidsc, 
				 	cam.cmpsituacao, 
				 	cam.cmpinstalacao, 
				 	cam.cmpsituacaoobra, 
				(SELECT coalesce(cast(SUM(cpivalor) as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid IN (45) AND cmi.cpiano = '2010' AND cpitabnum=1) AS ano2010,
				 (SELECT coalesce(cast(SUM(cpivalor) as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid IN (45) AND cmi.cpiano = '2011' AND cpitabnum=1) AS ano2011,
				 (SELECT coalesce(cast(SUM(cpivalor) as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid IN (45) AND cmi.cpiano = '2012' AND cpitabnum=1) AS ano2012,
				 to_char(cam.cmpdatainauguracao::date,'DD/MM/YYYY') as cmpdatainauguracao,
				 cam.cmpdataimplantacao,
				 (SELECT SUM(cpivalor) FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_INVESTIMENTO_SUP."' AND cpitabnum=1) AS insvtot,
				 (SELECT CASE WHEN count(*) >= 1 THEN 'Sim' ELSE 'Não' END FROM obras.obrainfraestrutura oi WHERE stoid = 1 AND obsstatus = 'A' AND entidcampus = cam.entid) AS obrascampus,
				 cam.datacriacao,
				 ct.cptdsc,
				 uor.entobs,
				 uor.entescolanova,
				 cam.exiid
		  FROM academico.campus cam 
		  LEFT JOIN academico.existencia exi ON exi.exiid = cam.exiid  
		  LEFT JOIN entidade.entidade ent ON ent.entid = cam.entid 		  
		  LEFT JOIN entidade.endereco edc ON edc.entid = ent.entid 
		  LEFT JOIN territorios.municipio mun ON mun.muncod = edc.muncod 
		  LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
		  LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid    
		  LEFT JOIN entidade.entidade uor ON uor.entid = fea.entid 
		  LEFT JOIN entidade.funcaoentidade fen2 ON fen2.entid = uor.entid 
		  LEFT JOIN academico.orgaouo tuo ON tuo.funid = fen2.funid
		  LEFT JOIN academico.campustipo ct ON ct.cptid = cam.cptid
		  ". (($filtroSQL)?"WHERE ".implode(" AND ",$filtroSQL):"") ." 
		  ORDER BY uor.entsig, edc.estuf, ent.entnome";
	
	$dadoscsv = $db->carregar($sql);
	
	$htmlHEADER = "<table cellspacing=1 cellpadding=3 class=\"tabela\" align=\"center\" bgcolor=\"#ffffff\" >
				<tr style=\"font-weight: bold; text-align: center; color: rgb(0, 0, 170);\" bgcolor=\"#e9e9e9\">
					<td colspan='14' align='center'>
					MINISTÉRIO DA EDUCAÇÃO<br>
					Responsável pela informação: Secretaria de Educação Superior (SESU)<br>
					<br>
					Expansão da Educação Superior
					</td>
				</tr>";
		$htmlCampos =  "
				<tr>
					".($municipio ? "" : "<td rowspan='2' class='SubTituloCentro'>Município</td>")."
					<td rowspan='2' class='SubTituloCentro'>Campus</td>
					<td rowspan='2' class='SubTituloCentro'>Status</td>
					<td rowspan='2' class='SubTituloCentro'>Situação</td>
					<td rowspan='2' class='SubTituloCentro'>Instalações</td>
					<!--<td rowspan='2' class='SubTituloCentro'>Situação sobre obras</td>-->
					<td rowspan='2' class='SubTituloCentro'>Obras no campus</td>
					<td colspan='3' class='SubTituloCentro'>Matrículas</td>
					<!--<td rowspan='2' class='SubTituloCentro'>Data de criação</td>-->
					<td rowspan='2' class='SubTituloCentro'>Tipo</td>
					<!--<td rowspan='2' class='SubTituloCentro'>Data do início das atividades</td>-->
					<!--<td rowspan='2' class='SubTituloCentro'>Implantação</td>-->
					<!--<td rowspan='2' class='SubTituloCentro'>Investimento total</td>-->
				</tr>
				<tr>
					<td class='SubTituloCentro'>2010</td>
					<td class='SubTituloCentro'>2011</td>
					<td class='SubTituloCentro'>2012</td>
				</tr>";
	
	if($dadoscsv[0]){
		
		//$_cmpexistencia = array("N"=>"Novo","P"=>"Preexistente", "R"=>"Previsto");
		$_cmpinstalacao = array("P"=>"Instalações Provisórias","D"=>"Instalações Definitivas" ,"S"=>"Sem Instalações");
		$_cmpsituacao = array("F"=>"Funcionando","N"=>"Não Funcionando");
		$_cmpsituacaoobra = array("L"=>"Licitação de Obras","A"=>"Obras em Andamento","C"=>"Obras Concluídas");
		$_cmp1etapa = array("N"=>"Não se aplica","A"=>"Em andamento","C"=>"Concluída");
				
		$i=1;
		$html = array();
		
		foreach($dadoscsv as $registro) {
			
			$cor = ($i%2) ? "#f7f7f7" : "";
			$i++;
			
			$html [$registro['ent_pai']]['header'] = "
				<tr style=\"font-weight: bold; text-align: center;\" bgcolor=\"#cccccc\">
					<td colspan='14' style=\"color:#005500;font-size:14px;font-weight:bold;text-align:center\" align='center'>
					{$registro['nomeinst']} - {$registro['entsig']}
					".(($registro['entescolanova'] == "t" && $registro['entobs'] != "")?"<br>({$registro['entobs']})":"")."
					</td>
				</tr>
				<tr>
					".($municipio ? "" : "<td rowspan='2' class='SubTituloCentro'>Município</td>")."
					<td rowspan='2' class='SubTituloCentro'>Campus</td>
					<td rowspan='2' class='SubTituloCentro'>Status</td>
					<td rowspan='2' class='SubTituloCentro'>Situação</td>
					<td rowspan='2' class='SubTituloCentro'>Instalações</td>
					<!--<td rowspan='2' class='SubTituloCentro'>Situação sobre obras</td>-->
					<td rowspan='2' class='SubTituloCentro'>Obras no campus</td>
					<td colspan='3' class='SubTituloCentro'>Matrículas</td>
					<!--<td rowspan='2' class='SubTituloCentro'>Data de criação</td>-->
					<td rowspan='2' class='SubTituloCentro'>Tipo</td>
					<!--<td rowspan='2' class='SubTituloCentro'>Data do início das atividades</td>-->
					<!--<td rowspan='2' class='SubTituloCentro'>Implantação</td>-->
					<!--<td rowspan='2' class='SubTituloCentro'>Investimento total</td>-->
				</tr>
				<tr>
					<td class='SubTituloCentro'>2010</td>
					<td class='SubTituloCentro'>2011</td>
					<td class='SubTituloCentro'>2012</td>
				</tr>";
			
			/* Gerar linha HTML */
			//if (fmod($i,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
			$html [$registro['ent_pai']]['tr'][$i] = "<tr onmouseout=\"this.bgColor='".$cor."';\" onmouseover=\"this.bgColor='#ffffcc';\" bgcolor='".$cor."'>".($municipio ? "" : "<td>".$registro['mundescricao']."</td>")."
			<td>".$registro['entnome']."</td><td>".$registro['exidsc']."</td>
					<td>".$_cmpsituacao[$registro['cmpsituacao']]."</td>
				       		<td>".$_cmpinstalacao[$registro['cmpinstalacao']]."</td>
				       		<!--<td>".$_cmpsituacaoobra[$registro['cmpsituacaoobra']]."</td>--><td align='center'>" . $registro['obrascampus'] . "</td>
				       		<td align='right'>".number_format($registro['ano2010'],0,',','.')."</td><td align='right'>".number_format($registro['ano2011'],0,',','.')."</td><td align='right'>".number_format($registro['ano2012'],0,',','.')."</td><!--<td>".((trim($registro['datacriacao']))?substr($registro['datacriacao'],4,2)."/".substr($registro['datacriacao'],0,4):"")."</td>--><td>".$registro['cptdsc']."</td><!--<td>".((trim($registro['cmpdataimplantacao']))?substr($registro['cmpdataimplantacao'],4,2)."/".substr($registro['cmpdataimplantacao'],0,4):"")."</td>--><!--<td>".$registro['cmpdatainauguracao']."</td>--><!--<td align='right'>".number_format($registro['insvtot'], 2, ',', '.')."</td> --></tr>";
			
			$total_insvtot [$registro['ent_pai']][$i] = $registro['insvtot'];
			$total_ano2012 [$registro['ent_pai']][$i] = $registro['ano2012'];
			$total_ano2011 [$registro['ent_pai']][$i] = $registro['ano2011'];
			$total_ano2010 [$registro['ent_pai']][$i] = $registro['ano2010'];
			
			$arrRegistro[$registro['ent_pai']][] = $registro['exiid'];
			$arrSituacao[$registro['ent_pai']][] = $registro['cmpsituacao'];
			$arrInstalacao[$registro['ent_pai']][] = $registro['cmpinstalacao'];
			
			$totalGeral_insvtot += $registro['insvtot'];
			$totalGeral_ano2012 += $registro['ano2012'];
			$totalGeral_ano2011 += $registro['ano2011'];
			$totalGeral_ano2010 += $registro['ano2010'];
			
		}
		
		$arrRegistro = !$arrRegistro ? array() : $arrRegistro;
		
		foreach($arrRegistro as $k => $reg){
			$i = 0;
			foreach($reg as $re){
				switch($re){
					case 2:
						$arrP[$k][] = $re;
						$arrTotalP[] = $re;
					break; 
					case 3:
						$arrN1[$k][] = $re;
						$arrTotalN1[] = $re;
					break;
					case 4:
						$arrC[$k][] = $re;
						$arrTotalC[] = $re;
					break;
					case 5:
						$arrN2[$k][] = $re;
						$arrTotalN2[] = $re;
					break;
					case 6:
						$arrI[$k][] = $re;
						$arrTotalI[] = $re;
					break;
					case 7:
						$arrNE[$k][] = $re;
						$arrTotalNE[] = $re;
					break;
				}
				
				switch($arrSituacao[$k][$i]){
					case "F":
						$arrSF[$k][] = $arrSituacao[$k][$i];
						$arrTotalSF[] = $arrSituacao[$k][$i];
					break; 
					case "N":
						$arrSN[$k][] = $arrSituacao[$k][$i];
						$arrTotalSN[] = $arrSituacao[$k][$i];
					break;
				}
				
				switch($arrInstalacao[$k][$i]){
					case "P":
						$arrIP[$k][] = $arrInstalacao[$k][$i];
						$arrTotalIP[] = $arrInstalacao[$k][$i];
					break; 
					case "D":
						$arrID[$k][] = $arrInstalacao[$k][$i];
						$arrTotalID[] = $arrInstalacao[$k][$i];
					break;
					case "S":
						$arrIS[$k][] = $arrInstalacao[$k][$i];
						$arrTotalIS[] = $arrInstalacao[$k][$i];
					break;
				}
				
				if($arrRegistro[$k][$i] == "N" && $arrSituacao[$k][$i] == "F"){
					$arrSSF[$k][] = 1;
					$arrTotalSSF[] = 1;
				}
				if($arrRegistro[$k][$i] == "N" && $arrSituacao[$k][$i] == "N"){
					$arrSSN[$k][] = 1;
					$arrTotalSSN[] = 1;
				}
				
				$i++;
				
			}
			
			$arrSSF[$k] = !$arrSSF[$k] ? array(0) : $arrSSF[$k];
			$arrSSN[$k] = !$arrSSN[$k] ? array(0) : $arrSSN[$k];
			$arrTotalSSF = !$arrTotalSSF ? array(0) : $arrTotalSSF;
			$arrTotalSSN = !$arrTotalSSN ? array(0) : $arrTotalSSN;
			
			$html [$k]['css'] = "<style>tr.bold td{font-weight:bold} </style> <tr class=\"bold\" bgcolor='#e9e9e9'>
									<td align=\"right\" colspan=".($municipio ? "5" : "6")." >Total</td>
								<td align='right'>".number_format(array_sum($total_ano2010[$k]),0,',','.')."</td>
									<td align='right'>".number_format(array_sum($total_ano2011[$k]),0,',','.')."</td>
									<td align='right'>".number_format(array_sum($total_ano2012[$k]),0,',','.')."</td>
									<td colspan=1></td>
									
									<!--<td>-</td>--><!--<td align='right'>".number_format( array_sum($total_insvtot[$k]), 2, ',', '.')."</td>-->
									
			</tr>";
			
			$html [$k]['tr2'] = "<tr class=\"bold\" bgcolor='#e9e9e9'>
									<td valign=\"top\" colspan=".($municipio ? "1" : "2")." ><span style=\"color:#0000AA\">Campus:</span> ".number_format(count($html[$k]['tr']),0,',','.')."</td>
									<td valign=\"top\" colspan=2 ><span style=\"color:#0000AA\">Status:</span><br/> - Preexistentes: ".number_format(count($arrP[$k]),0,',','.')."<br/>- Novos 2011/2012: ".number_format(count($arrN1[$k]),0,',','.')."<br/>- Criados em 2003/2010: ".number_format(count($arrC[$k]),0,',','.')."<br/>- Novos 2013/2014: ".number_format(count($arrN2[$k]),0,',','.')."<br/>- Incorporados: ".number_format(count($arrI[$k]),0,',','.')."<br/>- Novas expansões: ".number_format(count($arrNE[$k]),0,',','.')."</td>
								<td valign=\"top\" colspan=2 ><span style=\"color:#0000AA\">Situação:</span><br/> - Funcionando: ".number_format(count($arrSF[$k]),0,',','.')."<br/> - Não Funcionando: ".number_format(count($arrSN[$k]),0,',','.')."</td><td valign=\"top\" colspan=5 ><span style=\"color:#0000AA\">Instalações:</span><br/> - Provisória: ".number_format(count($arrIP[$k]),0,',','.')."<br/> - Definitiva: ".number_format(count($arrID[$k]),0,',','.')."<br/> - Sem Instalação: ".number_format(count($arrIS[$k]),0,',','.')."</td></tr>";
			
			$htmlCABECALHOTOTALGERAL = "<style>tr.bold td{font-weight:bold} </style> <tr class=\"bold\" bgcolor='#cccccc'>
									<td rowspan=2 colspan=".($municipio ? "5" : "6")." >
									<td colspan=3 align=\"center\" >VAGAS PACTUADAS</td>
									<td rowspan=3></td>
									<!--<td rowspan=2 align='center'>Investimento total</td>--></tr>";
			
			$htmlCABECALHOTOTALGERAL .= "<style>tr.bold td{font-weight:bold} </style> <tr class=\"bold\" bgcolor='#cccccc'><td>2010</td><td>2011</td><td>2012</td></tr>";
			
			$htmlTOTALGERAL = "<style>tr.bold td{font-weight:bold} </style> <tr class=\"bold\" bgcolor='#cccccc'><td align=\"right\"colspan=".($municipio ? "5" : "6")." >Total Geral</td><td align='right'>".number_format($totalGeral_ano2010,0,',','.')."</td><td align='right'>".number_format($totalGeral_ano2011,0,',','.')."</td><td align='right'>".number_format($totalGeral_ano2012,0,',','.')."</td><!--<td colspan=1></td><!--<td>-</td>--><!--<td align='right'>".number_format($totalGeral_insvtot, 2, ',', '.')."</td>--></tr>";
			$htmlTOTALGERAL .= "<tr class=\"bold\" bgcolor='#cccccc'><td valign=\"top\" colspan=".($municipio ? "1" : "2")." ><span style=\"color:#0000AA\">Campus:</span> ".number_format(count($dadoscsv),0,',','.')."</td><td valign=\"top\" colspan=2 ><span style=\"color:#0000AA\">Status:</span><br/> - Preexistentes: ".number_format(count($arrTotalP),0,',','.')."<br/>- Novos 2011/2012: ".number_format(count($arrTotalN1),0,',','.')."<br/>- Criados em 2003/2010: ".number_format(count($arrTotalC),0,',','.')."<br/>- Novos 2013/2014: ".number_format(count($arrTotalN2),0,',','.')."<br/>- Incorporados: ".number_format(count($arrTotalI),0,',','.')."<br/>- Novas expansões: ".number_format(count($arrTotalNE),0,',','.')."</td><td valign=\"top\" colspan=2 ><span style=\"color:#0000AA\">Situação:</span><br/> - Funcionando: ".number_format(count($arrTotalSF),0,',','.')."<br/> - Não Funcionando: ".number_format(count($arrTotalSN),0,',','.')."</td><td valign=\"top\" colspan=5 ><span style=\"color:#0000AA\">Instalações:</span><br/> - Provisória: ".number_format(count($arrTotalIP),0,',','.')."<br/> - Definitiva: ".number_format(count($arrTotalID),0,',','.')."<br/> - Sem Instalação: ".number_format(count($arrTotalIS),0,',','.')."</td></tr>";
			$htmlTOTALGERAL .= "<tr class=\"bold\" style=\"color:#cccccc\" bgcolor='#cccccc'><td colspan=\"14\" valign=\"top\" >.</td></tr>";
		}
		
	}else{
		$htmlNA = "<tr><td style=\"color:#990000;text-align:center;font-weight:bold\" colspan=\"10\" >$msgErro</td></tr>";
	}
	$htmlFIM = "</table>";
		
		echo $htmlHEADER;
		if(count($html) != 1 ){
			echo $htmlCABECALHOTOTALGERAL;
			echo $htmlTOTALGERAL;	
		}
	if(count($html) != 0 ){
		foreach($html as $k => $h){
			echo $html[$k]['header'];
			if(count($h["tr"]) == 0){
				echo $htmlNA;
			}else{
				foreach($h["tr"] as $i){
					echo $i;
				}
				if(count($h["tr"]) > 1){
					echo $html[$k]['css'];
					echo $html[$k]['tr2'];
				}
			}
		}
	}else{
		echo $htmlCampos;
		echo $htmlNA;
	}
	echo $htmlFIM;
}


function exibeRelatorioRedeFederalEducacaoProfissional($arrFiltros = array()){
	global $db;
	
	$msgErro = "Não atendido.";
	
	$filtroSQL[] = "tuo.orgid IN('2')";
	
	$filtroSQL[] = "fen.funid IN (17)";
	
	//Filtro por regcod
	if($arrFiltros['regcod'] && $arrFiltros['regcod'] != "" && $arrFiltros['regcod'] != "todos"){
		$filtroSQL[] = "edc.estuf IN ( select estuf from territorios.estado where regcod = '{$arrFiltros['regcod']}' ) ";
		$msgErro = "A região não solicitou recursos deste programa, não preencheu os critérios de atendimento do MEC, ou está com processo em andamento.";
	}
	//Filtro por estuf
	if($arrFiltros['estuf'] && $arrFiltros['estuf'] != "" && $arrFiltros['estuf'] != "todos"){
		$filtroSQL[] = "edc.estuf = '{$arrFiltros['estuf']}'";
		$msgErro = "O estado não solicitou recursos deste programa, não preencheu os critérios de atendimento do MEC, ou está com processo em andamento.";
	}
	//Filtro por tpmid
	if($arrFiltros['tpmid'] && $arrFiltros['tpmid'] != "" && $arrFiltros['tpmid'] != "todos"){
		$filtroSQL[] = "edc.muncod in (	select muncod from territorios.muntipomunicipio where tpmid = '{$arrFiltros['tpmid']}' )";
		$msgErro = "Não atendido.";
	}
	//Filtro por muncod
	if($arrFiltros['muncod'] && $arrFiltros['muncod'] != "" && $arrFiltros['muncod'] != "todos"){
		$filtroSQL[] = "edc.muncod = '{$arrFiltros['muncod']}' ";
		$municipio = true;
		$msgErro = "O município não solicitou recursos deste programa, não preencheu os critérios de atendimento do MEC, ou está com processo em andamento.";
	}
	//Filtro por mescod
	if($arrFiltros['mescod']){
		$filtroSQL[] = "edc.estuf IN ( select estuf from territorios.mesoregiao where mescod = '{$arrFiltros['mescod']}' ) ";
		$msgErro = "A mesorregião não solicitou recursos deste programa, não preencheu os critérios de atendimento do MEC, ou está com processo em andamento.";
	}
	
	//Filtros solicitados pela Elisangela (Regra retirada dia 26/01/2012 às 16:10 a pedido da Elisângela) 
		//Não exibir as instiuições "Colégio Pedro II", "Instituto Nacional de Educação de Surdos" e "Instituto Benjamin Constant"
		//$filtroSQL[] = "uor.entid NOT IN (411791, 411790, 388730) ";
		//Não exibir os campus previstos (Regra retirada dia 13/10/2010 às 10:40 a pedido da Manoela)
		//$filtroSQL[] = "cam.cmpexistencia <> 'R' ";
	//Manoela solicitou para retornar os campos avançados 14-04-2010 às 11:40
		//$filtroSQL[] = "ent.entid NOT IN (391951,391827,391949,392760,391948,392763,392761,392762,411770,411747,389177,614647,614658,614648,614699,614655,614688,614700,614682,614659,614687,614685,614697,614683,614649,614695,614656,614698,614653,614694,614660,614650,614651,614654,614681,614696) ";
		//$filtroSQL[] = "ent.entid NOT IN (391827,391949,392760,391948,392763,392761,392762,411770,411747,389177) ";
		
	//Filtro solicitado pelo Vitor
		$filtroSQL[] = "uor.entstatus = 'A' AND ent.entstatus = 'A' ";
	
	$sql =  "SELECT  
					".($municipio ? "" : "mun.mundescricao,")." 
					ent.entnome,
					uor.entid as ent_pai, 
					uor.entsig,
					uor.entnome as nomeinst,
				 	exi.exidsc, 
				 	cam.cmpsituacao, 
				 	cam.cmpinstalacao, 
				 	cam.cmpsituacaoobra, 
				 (SELECT coalesce(cast(SUM(cpivalor) as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid IN (47, 48) AND cmi.cpiano = '2010' AND cpitabnum=1) AS ano2010,
				 (SELECT coalesce(cast(SUM(cpivalor) as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid IN (47, 48) AND cmi.cpiano = '2011' AND cpitabnum=1) AS ano2011,
				 (SELECT coalesce(cast(SUM(cpivalor) as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid IN (47, 48) AND cmi.cpiano = '2012' AND cpitabnum=1) AS ano2012,
				 to_char(cam.cmpdatainauguracao::date,'DD/MM/YYYY') as cmpdatainauguracao,
				 (SELECT CASE WHEN count(*) >= 1 THEN 'Sim' ELSE 'Não' END FROM obras.obrainfraestrutura oi WHERE stoid = 1 AND obsstatus = 'A' AND entidcampus = cam.entid) AS obrascampus,
				 ct.cptdsc,
				 cam.exiid
		  FROM academico.campus cam 
		  LEFT JOIN academico.existencia exi ON exi.exiid = cam.exiid
		  LEFT JOIN entidade.entidade ent ON ent.entid = cam.entid  
		  LEFT JOIN entidade.endereco edc ON edc.entid = ent.entid 
		  LEFT JOIN territorios.municipio mun ON mun.muncod = edc.muncod 
		  LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
		  LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid    
		  LEFT JOIN entidade.entidade uor ON uor.entid = fea.entid 
		  LEFT JOIN entidade.funcaoentidade fen2 ON fen2.entid = uor.entid 
		  LEFT JOIN academico.orgaouo tuo ON tuo.funid = fen2.funid
		  LEFT JOIN academico.campustipo ct ON ct.cptid = cam.cptid
		  ". (($filtroSQL)?"WHERE ".implode(" AND ",$filtroSQL):"") ." 
		  ORDER BY edc.estuf, uor.entsig, ent.entnome";
	$dadoscsv = $db->carregar($sql);
	
	$htmlHEADER = "<table cellspacing=1 border=0 cellpadding=3 class=\"tabela\" align=\"center\" bgcolor=\"#ffffff\">
					<tr style=\"font-weight: bold; text-align: center; color: rgb(0, 0, 170);\" bgcolor=\"#e9e9e9\">
						<td colspan='10' align='center'>
						MINISTÉRIO DA EDUCAÇÃO<br>
						Responsável pela informação: Secretaria de Educação Profissional e Tecnológica (SETEC)<br>
						<br>
						Expansão da Educação Profissional e Tecnológica
						</td>
					</tr>";
		$htmlCampos =  "<tr>
						".($municipio ? "" : "<td rowspan='2' class='SubTituloCentro'>Município</td>")."
						<td rowspan='2' class='SubTituloCentro'>Campus</td>
						<td rowspan='2' class='SubTituloCentro'>Status</td>
						<td rowspan='2' class='SubTituloCentro'>Situação</td>
						<td rowspan='2' class='SubTituloCentro'>Instalações</td>
						<td rowspan='2' class='SubTituloCentro'>Obras no campus</td>
						<td colspan='3' class='SubTituloCentro'>MATRÍCULAS</td>
						<td rowspan='2' class='SubTituloCentro'>Tipo</td>
					</tr>
					<tr>
						<td class='SubTituloCentro'>2010</td>
						<td class='SubTituloCentro'>2011</td>
						<td class='SubTituloCentro'>2012</td>
					</tr>";
	
	if($dadoscsv[0]){
		$_cmpinstalacao = array("P"=>"Instalações Provisórias","D"=>"Instalações Definitivas" ,"S"=>"Sem Instalações");
		$_cmpsituacao = array("F"=>"Funcionando","N"=>"Não Funcionando");
		$_cmpsituacaoobra = array("L"=>"Licitação de Obras","A"=>"Obras em Andamento","C"=>"Obras Concluídas");
		$_cmp1etapa = array("N"=>"Não se aplica","A"=>"Em andamento","C"=>"Concluída");
		
		$i=1;
		foreach($dadoscsv as $registro) {
			
			$cor = ($i%2) ? "#f7f7f7" : "";
			$i++;
			
			$html [$registro['ent_pai']]['header'] = "
				<tr style=\"font-weight: bold; text-align: center;\" bgcolor=\"#cccccc\">
					<td colspan='10' style=\"color:#005500;font-size:14px;font-weight:bold;text-align:center\" align='center'>
			{$registro['nomeinst']} - {$registro['entsig']}
					</td>
				</tr>
				<tr>
						".($municipio ? "" : "<td rowspan='2' class='SubTituloCentro'>Município</td>")."
						<td rowspan='2' class='SubTituloCentro'>Campus</td>
						<td rowspan='2' class='SubTituloCentro'>Status</td>
						<td rowspan='2' class='SubTituloCentro'>Situação</td>
						<td rowspan='2' class='SubTituloCentro'>Instalações</td>
						<td rowspan='2' class='SubTituloCentro'>Obras no campus</td>
						<td colspan='3' class='SubTituloCentro'>MATRÍCULAS</td>
						<td rowspan='2' class='SubTituloCentro'>Tipo</td>
					</tr>
					<tr>
						<td class='SubTituloCentro'>2010</td>
						<td class='SubTituloCentro'>2011</td>
						<td class='SubTituloCentro'>2012</td>
					</tr>";
			
			
			/* Gerar linha HTML */
			$html [$registro['ent_pai']]['tr'][$i] = "
			<tr onmouseout=\"this.bgColor='".$cor."';\" onmouseover=\"this.bgColor='#ffffcc';\" bgcolor='".$cor."'>".($municipio ? "" : "<td>".$registro['mundescricao']."</td>")."<td>".$registro['entnome']."</td>
				<td>".$registro['exidsc']."</td>
				<td>".$_cmpsituacao[$registro['cmpsituacao']]."</td>
				<td>".$_cmpinstalacao[$registro['cmpinstalacao']]."</td>
				<td align='center'>" . $registro['obrascampus'] . "</td>
				<td align='right'>".number_format($registro['ano2010'],0,',','.')."</td>
				<td align='right'>".number_format($registro['ano2011'],0,',','.')."</td>
				<td align='right'>".number_format($registro['ano2012'],0,',','.')."</td>
				<td>".$registro['cptdsc']."</td>
			</tr>";
			
			$total_ano2010 [$registro['ent_pai']][$i] = $registro['ano2010'];
			$total_ano2011 [$registro['ent_pai']][$i] = $registro['ano2011'];
			$total_ano2012 [$registro['ent_pai']][$i] = $registro['ano2012'];
			
			$arrRegistro[$registro['ent_pai']][] = $registro['exiid'];
			$arrSituacao[$registro['ent_pai']][] = $registro['cmpsituacao'];
			$arrInstalacao[$registro['ent_pai']][] = $registro['cmpinstalacao'];
			
			$totalGeral_ano2010 += $registro['ano2010'];
			$totalGeral_ano2011 += $registro['ano2011'];
			$totalGeral_ano2012 += $registro['ano2012'];
		}
			$arrRegistro = !$arrRegistro ? array() : $arrRegistro;
			foreach($arrRegistro as $k => $reg){
				$i = 0;
				foreach($reg as $re){
					switch($re){
					case 2:
						$arrP[$k][] = $re;
						$arrTotalP[] = $re;
					break; 
					case 3:
						$arrN1[$k][] = $re;
						$arrTotalN1[] = $re;
					break;
					case 4:
						$arrC[$k][] = $re;
						$arrTotalC[] = $re;
					break;
					case 5:
						$arrN2[$k][] = $re;
						$arrTotalN2[] = $re;
					break;
					case 6:
						$arrI[$k][] = $re;
						$arrTotalI[] = $re;
					break;
				}
					
					switch($arrSituacao[$k][$i]){
						case "F":
							$arrSF[$k][] = $arrSituacao[$k][$i];
							$arrTotalSF[] = $arrSituacao[$k][$i];
						break; 
						case "N":
							$arrSN[$k][] = $arrSituacao[$k][$i];
							$arrTotalSN[] = $arrSituacao[$k][$i];
						break;
					}
					
					switch($arrInstalacao[$k][$i]){
						case "P":
							$arrIP[$k][] = $arrInstalacao[$k][$i];
							$arrTotalIP[] = $arrInstalacao[$k][$i];
						break; 
						case "D":
							$arrID[$k][] = $arrInstalacao[$k][$i];
							$arrTotalID[] = $arrInstalacao[$k][$i];
						break;
						case "S":
							$arrIS[$k][] = $arrInstalacao[$k][$i];
							$arrTotalIS[] = $arrInstalacao[$k][$i];
						break;
					}
					
					if($arrRegistro[$k][$i] == "N" && $arrSituacao[$k][$i] == "F"){
						$arrSSF[$k][] = 1;
						$arrTotalSSF[] = 1;
					}
					if($arrRegistro[$k][$i] == "N" && $arrSituacao[$k][$i] == "N"){
						$arrSSN[$k][] = 1;
						$arrTotalSSN[] = 1;
					}
					
					$i++;
					
				}
				
				$arrSSF[$k] = !$arrSSF[$k] ? array(0) : $arrSSF[$k];
				$arrSSN[$k] = !$arrSSN[$k] ? array(0) : $arrSSN[$k];
				$arrTotalSSF = !$arrTotalSSF ? array(0) : $arrTotalSSF;
				$arrTotalSSN = !$arrTotalSSN ? array(0) : $arrTotalSSN;

				$html [$k]['css'] = "<style>tr.bold td{font-weight:bold} </style> 
				<tr class=\"bold\" bgcolor='#e9e9e9'>
					<td align=\"right\" colspan=".($municipio ? "5" : "6")." >Total</td>
					<td align='right'>".number_format(array_sum($total_ano2010[$k]),0,',','.')."</td>
					<td align='right'>".number_format(array_sum($total_ano2011[$k]),0,',','.')."</td>
					<td align='right'>".number_format(array_sum($total_ano2012[$k]),0,',','.')."</td>
					<td>&nbsp;</td>
				</tr>";
				$html [$k]['tr2'] = "
				<tr class=\"bold\" bgcolor='#e9e9e9'>
					<td valign=\"top\" colspan=".($municipio ? "1" : "2")." ><span style=\"color:#0000AA\">Campus:</span> ".number_format(count($html[$k]['tr']),0,',','.')."</td>
					<td valign=\"top\" colspan=2 ><span style=\"color:#0000AA\">Status:</span><br/> - Preexistentes: ".number_format(count($arrP[$k]),0,',','.')."<br/>- Novos 2011/2012: ".number_format(count($arrN1[$k]),0,',','.')."<br/>- Criados em 2003/2010: ".number_format(count($arrC[$k]),0,',','.')."<br/>- Novos 2013/2014: ".number_format(count($arrN2[$k]),0,',','.')."<br/>- Incorporados: ".number_format(count($arrI[$k]),0,',','.')."</td>
					<td valign=\"top\" colspan=2 ><span style=\"color:#0000AA\">Situação:</span><br/> - Funcionando: ".count($arrSF[$k])."<br/> - Não Funcionando: ".count($arrSN[$k])."</td>
					<td valign=\"top\" colspan=5 ><span style=\"color:#0000AA\">Instalações:</span><br/> - Provisória: ".count($arrIP[$k])."<br/> - Definitiva: ".count($arrID[$k])."<br/> - Sem Instalação: ".count($arrIS[$k])."</td>
				</tr>";
				
				$htmlCABECALHOTOTALGERAL = "<style>tr.bold td{font-weight:bold} </style> 
				<tr class=\"bold\" bgcolor='#cccccc'>
					<td rowspan=2 colspan=".($municipio ? "5" : "6")." >
					<td colspan=3 align=\"center\" >MATRÍCULAS</td>
					<td rowspan=2>&nbsp;</td>
				</tr>";
				$htmlCABECALHOTOTALGERAL .= "<style>tr.bold td{font-weight:bold} </style> 
				<tr class=\"bold\" bgcolor='#cccccc'>
					<td>2010</td>
					<td>2011</td>
					<td>2012</td>
				</tr>";
				
				$htmlTOTALGERAL = "<style>tr.bold td{font-weight:bold} </style> 
				<tr class=\"bold\" bgcolor='#cccccc'>
					<td align=\"right\" colspan=".($municipio ? "5" : "6")." >Total Geral</td>
					<td align='right'>".number_format($totalGeral_ano2010,0,',','.')."</td>
					<td align='right'>".number_format($totalGeral_ano2011,0,',','.')."</td>
					<td align='right'>".number_format($totalGeral_ano2012,0,',','.')."</td>
					<td>&nbsp;</td>
				</tr>";
				$htmlTOTALGERAL .= "
				<tr class=\"bold\" bgcolor='#cccccc'>
					<td valign=\"top\" colspan=".($municipio ? "1" : "2")." ><span style=\"color:#0000AA\">Campus:</span> ".number_format(count($dadoscsv),0,',','.')."</td>
					<td valign=\"top\" colspan=2 ><span style=\"color:#0000AA\">Status:</span><br/> - Preexistentes: ".number_format(count($arrTotalP),0,',','.')."<br/>- Novos 2011/2012: ".number_format(count($arrTotalN1),0,',','.')."<br/>- Criados em 2003/2010: ".number_format(count($arrTotalC),0,',','.')."<br/>- Novos 2013/2014: ".number_format(count($arrTotalN2),0,',','.')."<br/>- Incorporados: ".number_format(count($arrTotalI),0,',','.')."</td>
					<td valign=\"top\" colspan=2 ><span style=\"color:#0000AA\">Situação:</span><br/> - Funcionando: ".count($arrTotalSF)."<br/> - Não Funcionando: ".count($arrTotalSN)."</td>
					<td valign=\"top\" colspan=5 ><span style=\"color:#0000AA\">Instalações:</span><br/> - Provisória: ".count($arrTotalIP)."<br/> - Definitiva: ".count($arrTotalID)."<br/> - Sem Instalação: ".count($arrTotalIS)."</td>
				</tr>";
				$htmlTOTALGERAL .= "<tr class=\"bold\" style=\"color:#cccccc\" bgcolor='#cccccc' ><td colspan=10>.</td></tr>";
		}
		
	}else{
		$htmlNA = "<tr><td style=\"color:#990000;text-align:center;font-weight:bold\" colspan=\"10\" >$msgErro</td></tr>";
	}
	$htmlFIM = "</table>";
		
		echo $htmlHEADER;
		if(count($html) != 1 ){
			echo $htmlCABECALHOTOTALGERAL;
			echo $htmlTOTALGERAL;	
		}
	if(count($html) != 0 ){
		foreach($html as $k => $h){
			echo $html[$k]['header'];
			if(count($h["tr"]) == 0){
				echo $htmlNA;
			}else{
				foreach($h["tr"] as $i){
					echo $i;
				}
				if(count($h["tr"]) > 1){
					echo $html[$k]['css'];
					echo $html[$k]['tr2'];
				}
			}
			
		}
	}else{
		echo $htmlCampos;
		echo $htmlNA;
	}
	echo $htmlFIM;
}

function testaPermissaoTela($usucpf = null, $mnuid = null, $sisid = null){
	global $db;
	
	$usucpf = !$usucpf ? $_SESSION['usucpf'] : $usucpf;
	$mnuid = !$mnuid ? $_SESSION['mnuid'] : $mnuid; //6190
	$sisid = !$sisid ? $_SESSION['sisid'] : $sisid;
	
	$sql = "SELECT 
				distinct per.pflcod
			FROM 
				seguranca.perfilmenu men
			INNER JOIN
				seguranca.perfil per ON per.pflcod = men.pflcod
			where
				per.sisid =  $sisid
			and
				mnuid = $mnuid";
	$arrPerfilAcesso = $db->carregar($sql);
	$arrPerfilAcesso = !$arrPerfilAcesso ? array() : $arrPerfilAcesso;
	foreach($arrPerfilAcesso as $pflcod){
		$arrPerfil[] = $pflcod['pflcod'];
	}
	$arrPerfil = !$arrPerfil ? array() : $arrPerfil;
	
	$sql = "SELECT p.pflcod FROM seguranca.perfil p 
			LEFT JOIN seguranca.perfilusuario pu ON pu.pflcod = p.pflcod 
			WHERE pu.usucpf = '". $usucpf ."' and p.pflstatus = 'A' and p.sisid =  '". $sisid ."' 
			ORDER BY pflnivel ASC LIMIT 1";
	$PerfilUsuario = $db->pegaUm($sql);
	$PerfilUsuario = !$PerfilUsuario ? "Sem Acesso" : $PerfilUsuario;
	
	if(in_array($PerfilUsuario,$arrPerfil)){
		return true;
	}else{
		return false;
	}	
	
}

function monta_cabecalho_relatorio_painel( $largura  = 100){
	
	global $db;
	
	$cabecalho = '<table width="'.$largura.'%" border="0" cellpadding="0" cellspacing="0" style="border-bottom: 1px solid;">'
				.'	<tr bgcolor="#ffffff">' 	
				.'		<td valign="top" width="50" rowspan="2"><img src="../imagens/brasao.gif" width="45" height="45" border="0"></td>'			
				.'		<td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;">'				
				.'			'. NOME_SISTEMA. '<br/>'				
//				.'			Acompanhamento da Execução Orçamentária<br/>'					
				.'			MEC / SE - Secretaria Executiva <br />'
				.'		</td>'
				.'		<td align="right" valign="middle" height="1" style="padding:5px 0 0 0;">'										
				.'			Data do Relatório:' . date( 'd/m/Y - H:i:s' ) . '<br />'					
				.'		</td>'					
				.'	</tr><tr bgcolor="#ffffff">'
				.'		<td colspan="2" align="center" valign="top" style="padding:0 0 5px 0;">'
				.'			<b><font style="font-size:14px;">' . $_REQUEST["titulo"] . '</font></b>'
				.'		</td>'
				.'	</tr>'					
				.'</table>';					
								
		return $cabecalho;						
						
}

function getPeriodicidadeIndicador($indid = null){
	global $db;

	if(!$indid){
		echo "Favor informar um indicador para retornar as periodicidades!";
		dbg($indid);
		return array();
	}
	
	$sql = "select
				per.perid,
				per.perdsc
			from
				painel.periodicidade per
			where
				per.pernivel >= ( 
								select 
									min(per.pernivel)
								from
									painel.periodicidade per
								inner join
									painel.detalheperiodicidade dpe ON per.perid = dpe.perid
								inner join
									painel.seriehistorica seh ON seh.dpeid = dpe.dpeid
								where
									seh.indid = $indid
								and
									per.perstatus = 'A'
								and
									seh.sehstatus <> 'I'
							  )
			and
				per.perstatus = 'A'";
	
	return $db->carregar($sql);
}


function getPeridIndicador($indid = null,$integer = false){
	global $db;

	if(!$indid){
		echo "Favor informar um indicador para retornar as periodicidades!";
		return array();
	}
	
	$sql = "select
				per.perid,
			from
				painel.periodicidade per
			where
				per.pernivel >= ( 
								select 
									min(per.pernivel)
								from
									painel.periodicidade per
								inner join
									painel.detalheperiodicidade dpe ON per.perid = dpe.perid
								inner join
									painel.seriehistorica seh ON seh.dpeid = dpe.dpeid
								where
									seh.indid = $indid
								and
									per.perstatus = 'A'
								and
									seh.sehstatus <> 'I'
							  )
			and
				per.perstatus = 'A'";
	
	$dados = $db->carregar($sql);
	
	if($dados[0]){
		foreach($dados as $d){
			if($integer)
				$arrPerid[] = (int)$d['perid'];
			else
				$arrPerid[] = $d['perid'];
		}
		return $arrPerid;
	}else{
		return array();
	}
	
}


function getAgrupadoresPorDetalhe($indid = null){
	global $db;
	
	if(!$indid)
		return array();
	
	$sql = "select
				tdiid as codigo,
				tdidsc as descricao
			from
				painel.detalhetipoindicador
			where
				indid = $indid
			and
				tdistatus = 'A'";
	
	$dados = $db->carregar($sql);
	
	//Período - Agrupador Padrão
	$arr[] = array( "codigo" => "dp.dpedsc" , "descricao" => "Período" );
	
	if(is_array($dados)){
		foreach($dados as $d){
			$arr[] = array( "codigo" => $d['codigo'] , "descricao" => $d['descricao'] );
		}
	}
		return $arr;	
}

function getAgrupadorPorRegionalizador($indid = null){
	global $db;
	
	if(!$indid)
		return false;
		
	$sql = "select
				regid
			from
				painel.indicador
			where
				indid = $indid
			and
				indstatus = 'A'";
	
	$regid = $db->pegaUm($sql);
		
	switch($regid){
		
		case REGIONALIZACAO_ESCOLA:
			$arr[] = array( "codigo" => "escdsc" , "descricao" => "Escola" );
			$arr[] = array( "codigo" => "mundescricao" , "descricao" => "Município" );
			$arr[] = array( "codigo" => "dshuf" , "descricao" => "Estado" );
		break;
		
		case REGIONALIZACAO_IES:
			$arr[] = array( "codigo" => "iesdsc" , "descricao" => "IES" );
			$arr[] = array( "codigo" => "mundescricao" , "descricao" => "Município" );
			$arr[] = array( "codigo" => "dshuf" , "descricao" => "Estado" );
		break;
		
		case REGIONALIZACAO_MUN:
			$arr[] = array( "codigo" => "mundescricao" , "descricao" => "Município" );
			$arr[] = array( "codigo" => "dshuf" , "descricao" => "Estado" );
		break;
		
		case REGIONALIZACAO_UF:
			$arr[] = array( "codigo" => "dshuf" , "descricao" => "Estado" );
		break;
		
		case REGIONALIZACAO_BRASIL:
			return false;
		break;
		
		case REGIONALIZACAO_POSGRADUACAO:
			$arr[] = array( "codigo" => "iepdsc" , "descricao" => "Pós-Graduação" );
			$arr[] = array( "codigo" => "mundescricao" , "descricao" => "Município" );
			$arr[] = array( "codigo" => "dshuf" , "descricao" => "Estado" );
		break;
		
		case REGIONALIZACAO_CAMPUS_SUPERIOR:
			$arr[] = array( "codigo" => "instituicaosuperior" , "descricao" => "Instituição" );
			$arr[] = array( "codigo" => "entnome" , "descricao" => "Campus (Superior)" );
			$arr[] = array( "codigo" => "mundescricao" , "descricao" => "Município" );
			$arr[] = array( "codigo" => "dshuf" , "descricao" => "Estado" );
		break;
		
		case REGIONALIZACAO_CAMPUS_PROFISSIONAL:
			$arr[] = array( "codigo" => "instituicaoprofissional" , "descricao" => "Instituição" );
			$arr[] = array( "codigo" => "entnome" , "descricao" => "Campus (Profissional)" );
			$arr[] = array( "codigo" => "mundescricao" , "descricao" => "Município" );
			$arr[] = array( "codigo" => "dshuf" , "descricao" => "Estado" );
		break;
		
		case REGIONALIZACAO_UNIVERSIDADE:
			$arr[] = array( "codigo" => "unidsc" , "descricao" => "Universidade" );
			$arr[] = array( "codigo" => "mundescricao" , "descricao" => "Município" );
			$arr[] = array( "codigo" => "dshuf" , "descricao" => "Estado" );
		break;
		
		case REGIONALIZACAO_INSTITUTO:
			$arr[] = array( "codigo" => "unidsc" , "descricao" => "Instituto" );
			$arr[] = array( "codigo" => "mundescricao" , "descricao" => "Município" );
			$arr[] = array( "codigo" => "dshuf" , "descricao" => "Estado" );
		break;
		
		case REGIONALIZACAO_HOSPITAL:
			$arr[] = array( "codigo" => "entnome" , "descricao" => "Hospital" );
			$arr[] = array( "codigo" => "mundescricao" , "descricao" => "Município" );
			$arr[] = array( "codigo" => "dshuf" , "descricao" => "Estado" );
		break;
		
		case REGIONALIZACAO_POLO:
			$arr[] = array( "codigo" => "poldsc" , "descricao" => "Pólo" );
			$arr[] = array( "codigo" => "mundescricao" , "descricao" => "Município" );
			$arr[] = array( "codigo" => "dshuf" , "descricao" => "Estado" );
		break;
		
		case REGIONALIZACAO_IESCPC:
			$arr[] = array( "codigo" => "iecdsc" , "descricao" => "IES/Município" );
			$arr[] = array( "codigo" => "mundescricao" , "descricao" => "Município" );
			$arr[] = array( "codigo" => "dshuf" , "descricao" => "Estado" );
		break;
		
		default:
			return false;
		
	}
	return $arr;
	
}
















function getValorIndicador( $indid = null , $arrPost = null){
	global $db;
	
	if(!$indid || !$arrPost)
		return false;
		
	$sql = "select
				regid,
				perid,
				indqtdevalor,
				indcumulativo,
				indcumulativovalor,
				unmid
			from
				painel.indicador
			where
				indid = $indid";
	extract($db->pegaLinha($sql));
	
	//Filtro por regvalue
	$sql = "select 
				regdescricao, 
				rgaidentificador,
				rgafiltro,
				regsqlcombo
			from 
				painel.regagreg reg1
			inner join
				painel.regionalizacao reg2 ON reg1.regid = reg2.regid 
			where 
				reg1.regid = $regid 
			and 
				regsqlcombo is not null";
	$campoRegiona = $db->pegaLinha($sql);
	/* Fim - Filtro por regvalue*/
	
	$agroup = false;
	
	$arrPost['agrupador'] = (array) $arrPost['agrupador'];
	if(is_array($arrPost['agrupador'])){
		foreach($arrPost['agrupador'] as $agr){
			if(strstr(strtoupper($campoRegiona['regsqlcombo']), strtoupper("$agr as descricao") )){
				$agroup = true;
			}
		}
	}
	
	if(!$agroup && $campoRegiona && $unmid == UNIDADEMEDICAO_RAZAO && !$arrPost['regvalue']){
		die("<center><br />Indicador de razão! Por favor, selecione o(a) {$campoRegiona['regdescricao']} e tente novamente!</center>");
	}

	
	if( is_array($arrPost['tidid_1']) && $arrPost['tidid_1'][0] != "" && $arrPost['tidid_1'][0] != "todos"){
		$arrFiltros[] = "d.tidid1 IN (" . implode(",", $arrPost['tidid_1']) . ")";
	}
	
	if( is_array($arrPost['tidid_2']) && $arrPost['tidid_2'][0] != "" && $arrPost['tidid_2'][0] != "todos"){
		$arrFiltros[] = "d.tidid2 IN (" . implode(",", $arrPost['tidid_2']) . ")";
	}
	
	if($arrPost['periodo_inicio'])
		$arrFiltros[] = "d.dpedatainicio >= ( select dpedatainicio from painel.detalheperiodicidade where dpeid = {$arrPost['periodo_inicio']})";
	if($arrPost['periodo_fim'])
		$arrFiltros[] = "d.dpedatainicio <= ( select dpedatafim from painel.detalheperiodicidade where dpeid = {$arrPost['periodo_fim']})";
	if($arrPost['regcod'] && $arrPost['regcod'] != "todos" && $arrPost['regcod'] != "")
		$arrFiltros[] = "d.dshuf in ( select estuf from territorios.estado where regcod = '{$arrPost['regcod']}' )";
	if($arrPost['estuf'] && $arrPost['estuf'] != "todos" && $arrPost['estuf'] != "")
		$arrFiltros[] = "d.dshuf = '{$arrPost['estuf']}'";
	if($arrPost['tpmid'] && $arrPost['tpmid'] != "todos" && $arrPost['tpmid'] != "")
		$arrFiltros[] = "d.dshcodmunicipio in (select muncod from territorios.muntipomunicipio where tpmid = {$arrPost['tpmid']} )";
	if($arrPost['gtmid'] && $arrPost['gtmid'] != "todos" && $arrPost['gtmid'] != "")
		$arrFiltros[] = "d.dshcodmunicipio in (select muncod from territorios.muntipomunicipio where tpmid in (select tpmid from territorios.tipomunicipio where gtmid = {$arrPost['gtmid']}) )";
	if($arrPost['muncod'] && $arrPost['muncod'] != "todos" && $arrPost['muncod'] != "")
		$arrFiltros[] = "d.dshcodmunicipio = '{$arrPost['muncod']}'";
	if($arrPost['regvalue'] && $arrPost['regvalue'] != "todos" && $arrPost['regvalue'] != ""){
		$sql = "select rgaidentificador,rgafiltro from painel.regagreg where regid = $regid";
		$campoReg = $db->pegaLinha($sql);
		$arrFiltros[] = str_replace(array("AND","and","{".$campoReg['rgaidentificador']."}"),array("","",$arrPost['regvalue']),$campoReg['rgafiltro']);
	}
	if($arrPost['tidid1'] && $arrPost['tidid1'] != "todos" && $arrPost['tidid1'] != ""){
		$sql = "select
					tdinumero 
				from 
					painel.detalhetipoindicador d1
				inner join
					painel.detalhetipodadosindicador d2 ON d1.tdiid = d2.tdiid
				where 
					d2.tidid = {$arrPost['tidid1']}";
		$tdinumero1 = $db->pegaUm($sql);
		$arrFiltros[] = "tidid$tdinumero1 = '{$arrPost['tidid1']}'";
	}if($arrPost['tidid2'] && $arrPost['tidid2'] != "todos" && $arrPost['tidid2'] != ""){
		$sql = "select 
					tdinumero 
				from 
					painel.detalhetipoindicador d1
				inner join
					painel.detalhetipodadosindicador d2 ON d1.tdiid = d2.tdiid
				where 
					d2.tidid = {$arrPost['tidid2']}";
		$tdinumero2 = $db->pegaUm($sql);
		$arrFiltros[] = "tidid$tdinumero2 = '{$arrPost['tidid2']}'";	
	}
	
	// Início - Monta os Agrupadores
	if(is_array($arrPost['agrupador'])){
		foreach($arrPost['agrupador'] as $agrupador){
			if(is_numeric($agrupador)){
				$sql = "select tdinumero, tdidsc from painel.detalhetipoindicador where tdiid = $agrupador";
				$dado = $db->pegaLinha($sql);
//				$arrAgrup[] = array("campo" => "tiddsc".$dado['tdinumero'],
//									"label" => $dado['tdidsc']
//								    );
				//Campos para o SQL
				$arrCampos[] = "tiddsc".$dado['tdinumero'];
				//Group by
				$arrGroupBy[] = "tiddsc".$dado['tdinumero'];
				//Coluna p/ Excel
//				$arrParametros['colunasExcel'][] = $dado['tdidsc'];
			}else{
				if($agrupador == "instituicaoprofissional" || $agrupador == "instituicaosuperior"){
//					$arrAgrup[] =  array("campo" => $agrupador,
//										 "label" => "Instituição"
//										 );
					
					//Campos para o SQL
					$arrCampos[] = $agrupador;
					//Group by
					$arrGroupBy[] = $agrupador;
					//Coluna p/ Excel
//					$arrParametros['colunasExcel'][] = "Instituição";
					
				}else{
//					$arrAgrup[] =  array("campo" => $agrupador,
//										 "label" => getAgrupadorPorCampo($agrupador,$regid)
//										 );
					
					//Campos para o SQL
					$arrCampos[] = $agrupador;
					//Group by
					$arrGroupBy[] = $agrupador;
					//Coluna p/ Excel
//					$arrParametros['colunasExcel'][] = getAgrupadorPorCampo($agrupador,$regid);
				}
			}
		}
	}
	// Fim - Monta os Agrupadores

	// Início - Monta as Colunas
	$arrCamposInterno = $arrCampos;
	$arrColunas[] = "qtde";

	if($unmid == UNIDADEMEDICAO_MOEDA) {
//		$arrLabelColunas[] = array("label" => "R$" , "campo" => "qtde");
//		$arrParametros['colunasExcel'][] = "R$";
		$arrCampos[] = "sum(qtde) as qtde";		
	}elseif($unmid == UNIDADEMEDICAO_PERCENTUAL) {
//		$arrLabelColunas[] = array("label" => "Porcentagem" , "campo" => "qtde");
//		$arrParametros['colunasExcel'][] = "Porcentagem";
		$arrCampos[] = "sum(qtde) || '%' as qtde";		
	} else {
//		$arrLabelColunas[] = array("label" => "Quantidade" , "campo" => "qtde", "type" => "numeric");
//		$arrParametros['colunasExcel'][] = "Quantidade";
		$arrCampos[] = "sum(qtde) as qtde";
	}

	if($indqtdevalor == "t"){
		$arrColunas[] = "valor";
		$arrCampos[] = "sum(valor) as valor";
//		$arrLabelColunas[] = array("label" => "Valor" , "campo" => "valor");
//		$arrParametros['colunasExcel'][] = "Valor";
	}
	// Fim - Monta as Colunas
	
	$arrGroupByInterno = $arrGroupBy;
	
	if(in_array("dp.dpedsc",$arrCampos)){
		foreach($arrCampos as $key => $campo){
			if($campo == "dp.dpedsc"){
//				$arrAgrup[$key] = array("campo" => "dpedsc" , "label" => "Período");
//				$arrParametros['colunasExcel'][$key] = "Período";
				$arrGroupBy[] = "dpedatainicio";
			} 
		}
	}
	
//	$arrParametros['agrupadores'] = array(	
//											"agrupador" 		=> $arrAgrup,
//								 			"agrupadoColuna" 	=> $arrColunas
//										  );
	
//	$arrParametros['colunas'] = $arrLabelColunas;
										  
	//Início - Inner
	if(in_array("entnome",$arrCampos)){
//		if($excel){
//			foreach($arrCamposInterno as $key => $campoInterno){
//				if($campoInterno == "entnome"){
//					unset($arrColunasExcel);
//					foreach($arrParametros['colunasExcel'] as $chave => $arrExcel):
//						if($arrExcel == "Campus (Profissional)" || $arrExcel == "Campus (Superior)" || $arrExcel == "Hospital"){
//							$arrColunasExcel[] = $arrExcel;
//							$arrColunasExcel[] = "Código $arrExcel";
//						}else{
//							$arrColunasExcel[] = $arrExcel;
//						}
//					endforeach;
//					$arrParametros['colunasExcel'] = $arrColunasExcel;
//					$arrCampos[$key] = "entnome , dp.entid ";
//					$arrCamposInterno[$key] = "entnome , d.entid ";
//					$arrGroupByInterno[] = "d.entid";
//					$arrGroupBy[] = "dp.entid";
//					break;
//				}
//			}
//		}else{
			foreach($arrCamposInterno as $key => $campoInterno){
				if($campoInterno == "entnome"){
					$arrCamposInterno[$key] = "(entnome || '<span style=\"display:none\" >' || ent.entid || '</span>') as entnome";
					$arrGroupByInterno[] = "ent.entid";
					break;
				}
			}
//		}
		$arrInner[] = "entidade.entidade ent ON ent.entid = d.entid";
	}if(in_array("iepdsc",$arrCampos)){
//		if($excel){
//			foreach($arrCamposInterno as $key => $campoInterno){
//				if($campoInterno == "iepdsc"){
//					unset($arrColunasExcel);
//					foreach($arrParametros['colunasExcel'] as $chave => $arrExcel):
//						if($arrExcel == "Pós-Graduação"){
//							$arrColunasExcel[] = $arrExcel;
//							$arrColunasExcel[] = "Código $arrExcel";
//						}else{
//							$arrColunasExcel[] = $arrExcel;
//						}
//					endforeach;
//					$arrParametros['colunasExcel'] = $arrColunasExcel;
//					$arrCampos[$key] = "iepdsc , dp.iepid ";
//					$arrCamposInterno[$key] = "iepdsc , d.iepid ";
//					$arrGroupByInterno[] = "d.iepid";
//					$arrGroupBy[] = "dp.iepid";
//					break;
//				}
//			}
//		}else{
			foreach($arrCamposInterno as $key => $campoInterno){
				if($campoInterno == "iepdsc"){
					$arrCamposInterno[$key] = "(iepdsc || '<span style=\"display:none\" >' || iep.iepid || '</span>') as iepdsc";
					$arrGroupByInterno[] = "iep.iepid";
					break;
				}
			}
//		}
		$arrInner[] = "painel.iepg iep ON iep.iepid = d.iepid";
	}if(in_array("unidsc",$arrCampos)){
//	if($excel){
//			foreach($arrCamposInterno as $key => $campoInterno){
//				if($campoInterno == "unidsc"){
//					unset($arrColunasExcel);
//					foreach($arrParametros['colunasExcel'] as $chave => $arrExcel):
//						if($arrExcel == "Instituto" || $arrExcel == "Universidade"){
//							$arrColunasExcel[] = $arrExcel;
//							$arrColunasExcel[] = "Código $arrExcel";
//						}else{
//							$arrColunasExcel[] = $arrExcel;
//						}
//					endforeach;
//					$arrParametros['colunasExcel'] = $arrColunasExcel;
//					$arrCampos[$key] = "unidsc , dp.unicod ";
//					$arrCamposInterno[$key] = "unidsc , d.unicod ";
//					$arrGroupByInterno[] = "d.unicod";
//					$arrGroupBy[] = "dp.unicod";
//					break;
//				}
//			}
//		}else{
			foreach($arrCamposInterno as $key => $campoInterno){
				if($campoInterno == "unidsc"){
					$arrCamposInterno[$key] = "(unidsc || '<span style=\"display:none\" >' || uni.unicod || '</span>') as unidsc";
					$arrGroupByInterno[] = "uni.unicod";
					break;
				}
			}
//		}
		$arrInner[] = "public.unidade uni ON uni.unicod = d.unicod";
	}if(in_array("mundescricao",$arrCampos)){
//		if($excel){
//			foreach($arrCamposInterno as $key => $campoInterno){
//				if($campoInterno == "mundescricao"){
//					unset($arrColunasExcel);
//					foreach($arrParametros['colunasExcel'] as $chave => $arrExcel):
//						if($arrExcel == "Município"){
//							$arrColunasExcel[] = $arrExcel;
//							$arrColunasExcel[] = "Código IBGE";
//						}else{
//							$arrColunasExcel[] = $arrExcel;
//						}
//					endforeach;
//					$arrParametros['colunasExcel'] = $arrColunasExcel;
//					$arrCampos[$key] = "mundescricao , dp.dshcodmunicipio ";
//					$arrCamposInterno[$key] = "mundescricao , d.dshcodmunicipio ";
//					$arrGroupByInterno[] = "d.dshcodmunicipio";
//					$arrGroupBy[] = "dp.dshcodmunicipio";
//					break;
//				}
//			}
//		}else{
			foreach($arrCamposInterno as $key => $campoInterno){
				if($campoInterno == "mundescricao"){
					$arrCamposInterno[$key] = "(mundescricao || '<span style=\"display:none\" >' || mun.muncod || '</span>') as mundescricao";
					$arrGroupByInterno[] = "mun.muncod";
					break;
				}
			}
//		}
		$arrInner[] = "territorios.municipio mun ON mun.muncod = d.dshcodmunicipio";
	}
	if(in_array("poldsc",$arrCampos)){
		$arrInner[] = "painel.polo pol ON pol.polid = d.polid";
//		if($excel){
//			foreach($arrCamposInterno as $key => $campoInterno){
//				if($campoInterno == "poldsc"){
//					unset($arrColunasExcel);
//					foreach($arrParametros['colunasExcel'] as $chave => $arrExcel):
//						if($arrExcel == "Pólo"){
//							$arrColunasExcel[] = $arrExcel;
//							$arrColunasExcel[] = "Código do Polo";
//						}else{
//							$arrColunasExcel[] = $arrExcel;
//						}
//					endforeach;
//					$arrParametros['colunasExcel'] = $arrColunasExcel;
//					$arrCampos[$key] = "poldsc , dp.polid ";
//					$arrCamposInterno[$key] = "poldsc , d.polid ";
//					$arrGroupByInterno[] = "d.polid";
//					$arrGroupBy[] = "dp.polid";
//					break;
//				}
//			}
//		}else{
			foreach($arrCamposInterno as $key => $campoInterno){
				if($campoInterno == "poldsc"){
					$arrCamposInterno[$key] = "(poldsc || '<span style=\"display:none\" >' || pol.polid || '</span>') as poldsc";
					$arrGroupByInterno[] = "pol.polid";
					break;
				}
			}
//		}
	}
	if(in_array("instituicaosuperior",$arrCampos)){
		foreach($arrCamposInterno as $key => $campoInterno){
			if($campoInterno == "instituicaosuperior"){
				$arrCamposInterno[$key] = "( 
												select 
													ent2.entnome
												from
													entidade.entidade ent
												inner join
													entidade.funcaoentidade fun ON fun.entid = ent.entid and fun.funid = 18
												inner join
													entidade.funentassoc fea ON fea.fueid = fun.fueid
												inner join
													entidade.entidade ent2 ON ent2.entid = fea.entid
												inner join
													entidade.funcaoentidade fun2 ON fun2.entid = ent2.entid and fun2.funid = 12
												where
													ent.entid = d.entid
											) as instituicaosuperior";
				$arrGroupByInterno[] = "instituicaosuperior";
				break;
			}
		}
	}
	if(in_array("instituicaoprofissional",$arrCampos)){
		foreach($arrCamposInterno as $key => $campoInterno){
			if($campoInterno == "instituicaoprofissional"){
				$arrCamposInterno[$key] = "( 
												select 
													ent2.entnome
												from
													entidade.entidade ent
												inner join
													entidade.funcaoentidade fun ON fun.entid = ent.entid and fun.funid = 17
												inner join
													entidade.funentassoc fea ON fea.fueid = fun.fueid
												inner join
													entidade.entidade ent2 ON ent2.entid = fea.entid
												inner join
													entidade.funcaoentidade fun2 ON fun2.entid = ent2.entid and fun2.funid = 11
												where
													ent.entid = d.entid
											) as instituicaoprofissional";
				$arrGroupByInterno[] = "instituicaoprofissional";
				break;
			}
		}
	}
	if(in_array("iesdsc",$arrCampos)){
		$arrInner[] = "painel.ies ies ON ies.iesid::integer = d.dshcod::integer";
//		if($excel){
//			foreach($arrCamposInterno as $key => $campoInterno){
//				if($campoInterno == "iesdsc"){
//					unset($arrColunasExcel);
//					foreach($arrParametros['colunasExcel'] as $chave => $arrExcel):
//						if($arrExcel == "IES"){
//							$arrColunasExcel[] = $arrExcel;
//							$arrColunasExcel[] = "Código $arrExcel";
//						}else{
//							$arrColunasExcel[] = $arrExcel;
//						}
//					endforeach;
//					$arrParametros['colunasExcel'] = $arrColunasExcel;
//					$arrCampos[$key] = "iesdsc , dp.dshcod ";
//					$arrCamposInterno[$key] = "ies.iesdsc , d.dshcod ";
//					$arrGroupByInterno[] = "d.dshcod";
//					$arrGroupBy[] = "dp.dshcod";
//					break;
//				}
//			}
//		}else{
			foreach($arrCamposInterno as $key => $campoInterno){
				if($campoInterno == "iesdsc"){
					$arrCamposInterno[$key] = "'<span style=\"display:none\" >' || ies.iesid || '</span>' || ies.iesdsc as iesdsc";
					$arrGroupByInterno[] = "ies.iesid";
					break;
				}
			}
//		}
	}if(in_array("escdsc",$arrCampos)){
//		if($excel){
//			foreach($arrCamposInterno as $key => $campoInterno){
//				if($campoInterno == "escdsc"){
//					unset($arrColunasExcel);
//					foreach($arrParametros['colunasExcel'] as $chave => $arrExcel):
//						if($arrExcel == "Escola"){
//							$arrColunasExcel[] = $arrExcel;
//							$arrColunasExcel[] = "Código INEP";
//						}else{
//							$arrColunasExcel[] = $arrExcel;
//						}
//					endforeach;
//					$arrParametros['colunasExcel'] = $arrColunasExcel;
//					$arrCampos[$key] = "escdsc , esccodinep ";
//					$arrCamposInterno[$key] = "esc.escdsc , esc.esccodinep ";
//					$arrGroupByInterno[] = "esccodinep";
//					$arrGroupBy[] = "esccodinep";
//					break;
//				}
//			}
//		}else{
			foreach($arrCamposInterno as $key => $campoInterno){
				if($campoInterno == "escdsc"){
					$arrCamposInterno[$key] = "'<span style=\"display:none\" >' || esc.esccodinep || '</span>' || esc.escdsc as escdsc";
					$arrGroupByInterno[] = "esc.esccodinep";
					break;
				}
			}
//		}
		$arrInner[] = "painel.escola esc ON esc.esccodinep::text = d.dshcod::text";
	}if(in_array("iecdsc",$arrCampos)){
//		if($excel){
//			foreach($arrCamposInterno as $key => $campoInterno){
//				if($campoInterno == "iecdsc"){
//					unset($arrColunasExcel);
//					foreach($arrParametros['colunasExcel'] as $chave => $arrExcel):
//						if($arrExcel == "IES/Município"){
//							$arrColunasExcel[] = $arrExcel;
//							$arrColunasExcel[] = "Código $arrExcel";
//						}else{
//							$arrColunasExcel[] = $arrExcel;
//						}
//					endforeach;
//					$arrParametros['colunasExcel'] = $arrColunasExcel;
//					$arrCampos[$key] = "iecdsc , dp.iecid ";
//					$arrCamposInterno[$key] = "ies.iecdsc , d.iecid ";
//					$arrGroupByInterno[] = "d.iecid";
//					$arrGroupBy[] = "dp.iecid";
//					break;
//				}
//			}
//		}else{
			foreach($arrCamposInterno as $key => $campoInterno){
				if($campoInterno == "iecdsc"){
					$arrCamposInterno[$key] = "'<span style=\"display:none\" >' || ies.iecid || '</span>' || ies.iecdsc as iecdsc";
					$arrGroupByInterno[] = "ies.iecid";
					break;
				}
			}
//		}
		$arrInner[] = "painel.iescpc ies ON ies.iecid = d.iecid";
	//Fim - Inner
	}
	//Se o indicador for cumulativo e a coluna de Período não estiver presente, exibi-se apenas o último período de cada ítem
	if($indcumulativo == "N" && !in_array("dp.dpedsc",$arrCampos)){
		$whereDataInterna = "";
		if($arrPost['periodo_inicio']){
			$whereDataInterna .= " d1.dpedatainicio >= ( select dpedatainicio from painel.detalheperiodicidade where dpeid = {$arrPost['periodo_inicio']}) and ";
		}
		if($arrPost['periodo_fim']){
			$whereDataInterna .= " d1.dpedatainicio <= ( select dpedatafim from painel.detalheperiodicidade where dpeid = {$arrPost['periodo_fim']}) and ";
		}
	}else{
		$whereDataInterna = " d1.dpedatainicio>=dp.dpedatainicio 
							and 
								d1.dpedatafim<=dp.dpedatafim 
							and ";
	}
										  
	/* Início SQL */
	$arrParametros['sql'] = "	select 
									".(count($arrCampos) ? implode(" , ",$arrCampos) : "" )."
								from (
										select 
											dp.dpedatainicio,
											".(count($arrCamposInterno) ? implode(" , ",$arrCamposInterno)."," : "" )."
											case when d.indcumulativo = 'N' then
								        			case when (select 
						                        	d1.dpeid
						                        from 
						                        	painel.detalheperiodicidade d1
												inner join 
													painel.seriehistorica sh on sh.dpeid=d1.dpeid
												where
													$whereDataInterna 
													d1.dpedatainicio>=dp.dpedatainicio 
												and 
													d1.dpedatafim<=dp.dpedatafim 
												and 
													sh.indid=d.indid
												order by 
													d1.dpedatainicio desc 
												limit 
													1
				                				) = d.dpeid then sum(d.qtde)  
				                	else 0 end
								else sum(d.qtde)
							end as qtde,
											case when d.indcumulativovalor = 'N' then
				        			case when (
				                        		select 
				                        			d1.dpeid
				                        		from 
				                        			painel.detalheperiodicidade d1
				                                inner join 
				                                	painel.seriehistorica sh on sh.dpeid=d1.dpeid
				                                where 
				                                	$whereDataInterna
				                                	d1.dpedatainicio>=dp.dpedatainicio 
				                                and 
				                                	d1.dpedatafim<=dp.dpedatafim 
				                                and 
				                                	sh.indid=d.indid
				                                order by 
													d1.dpedatainicio desc 
												limit 
													1
				                				) = d.dpeid then sum(d.valor)
				                			else 0 end
									else sum(d.valor)
							end as valor
										from 
											painel.v_detalheindicadorsh d
										left join 
											painel.detalheperiodicidade dp on d.dpedatainicio>=dp.dpedatainicio and d.dpedatafim<=dp.dpedatafim
										".(count($arrInner) ? " left join ".implode(" left join ",$arrInner) : "" )." 
										-- periodo que vc quer exibir
										where 
											dp.perid = ".(!$arrPost['perid'] ? $perid : $arrPost['perid'])."
										-- indicador que vc quer exibir
										and 
											d.indid = $indid
										".(count($arrFiltros) ? " and ".implode(" and ",$arrFiltros) : "" )."
										--range de data compreendida no periodo
										".str_replace("dsh.","d.",$andDpe)."
										group by 
											d.indcumulativo,
											dp.dpedatainicio,
											dp.dpedatafim,
											d.indid,
											d.dpeid,
											d.indcumulativovalor 
											".(count($arrGroupByInterno) ? ",".implode(",",$arrGroupByInterno) : "")."
									) dp
									".(count($arrGroupBy) ? " group by ".implode(",",$arrGroupBy) : "")."
									".(count($arrGroupBy) ? " order by ".implode(",",$arrGroupBy) : "")."";	
	
	/* Fim SQL */
//	dbg($arrParametros['sql']);

	$arDados = $db->carregar( $arrParametros['sql'] ); 				                                			

	switch ( $arrPost['extracao'] ){
		case 'produto':
			$indSoma = 'qtde';
			break;
		case 'valor':
			$indSoma = 'valor';
			break;
		case 'agrupador':
			$indSoma = '';
			break;
	}
	
	if ( is_array($arDados) ){
		$resultValor = 0;	
		foreach ($arDados as $dados){
			$resultValor +=	(empty($indSoma) ? 1 : $dados[$indSoma]); 
		}
	}
	
	return $resultValor;
}
















function getParametrosRelatorioIndicador( $indid = null , $arrPost = null , $excel = false){
	global $db;
	
	if(!$indid || !$arrPost)
		return false;
		
	$sql = "select
				regid,
				perid,
				indqtdevalor,
				indcumulativo,
				indcumulativovalor,
				unmid
			from
				painel.indicador
			where
				indid = $indid";
	extract($db->pegaLinha($sql));
	
	//Filtro por regvalue
	$sql = "select 
				regdescricao, 
				rgaidentificador,
				rgafiltro,
				regsqlcombo
			from 
				painel.regagreg reg1
			inner join
				painel.regionalizacao reg2 ON reg1.regid = reg2.regid 
			where 
				reg1.regid = $regid 
			and 
				regsqlcombo is not null";
	$campoRegiona = $db->pegaLinha($sql);
	/* Fim - Filtro por regvalue*/
	
	$agroup = false;
	
	$arrPost['agrupador'] = (array) $arrPost['agrupador'];
	if(is_array($arrPost['agrupador'])){
		foreach($arrPost['agrupador'] as $agr){
			if(strstr(strtoupper($campoRegiona['regsqlcombo']), strtoupper("$agr as descricao") )){
				$agroup = true;
			}
		}
	}
	
	if(!$agroup && $campoRegiona && $unmid == UNIDADEMEDICAO_RAZAO && !$arrPost['regvalue']){
		die("<center><br />Indicador de razão! Por favor, selecione o(a) {$campoRegiona['regdescricao']} e tente novamente!</center>");
	}

	
	if( is_array($arrPost['tidid_1']) && $arrPost['tidid_1'][0] != ""){
		$arrFiltros[] = "d.tidid1 IN (" . implode(",", $arrPost['tidid_1']) . ")";
	}
	
	if( is_array($arrPost['tidid_2']) && $arrPost['tidid_2'][0] != ""){
		$arrFiltros[] = "d.tidid2 IN (" . implode(",", $arrPost['tidid_2']) . ")";
	}
	
	if($arrPost['periodo_inicio'])
		$arrFiltros[] = "d.dpedatainicio >= ( select dpedatainicio from painel.detalheperiodicidade where dpeid = {$arrPost['periodo_inicio']})";
	if($arrPost['periodo_fim'])
		$arrFiltros[] = "d.dpedatainicio <= ( select dpedatafim from painel.detalheperiodicidade where dpeid = {$arrPost['periodo_fim']})";
	if($arrPost['regcod'] && $arrPost['regcod'] != "todos" && $arrPost['regcod'] != "")
		$arrFiltros[] = "d.dshuf in ( select estuf from territorios.estado where regcod = '{$arrPost['regcod']}' )";
	if($arrPost['estuf'] && $arrPost['estuf'] != "todos" && $arrPost['estuf'] != "")
		$arrFiltros[] = "d.dshuf = '{$arrPost['estuf']}'";
	if($arrPost['tpmid'] && $arrPost['tpmid'] != "todos" && $arrPost['tpmid'] != "")
		$arrFiltros[] = "d.dshcodmunicipio in (select muncod from territorios.muntipomunicipio where tpmid = {$arrPost['tpmid']} )";
	if($arrPost['gtmid'] && $arrPost['gtmid'] != "todos" && $arrPost['gtmid'] != "")
		$arrFiltros[] = "d.dshcodmunicipio in (select muncod from territorios.muntipomunicipio where tpmid in (select tpmid from territorios.tipomunicipio where gtmid = {$arrPost['gtmid']}) )";
	if($arrPost['muncod'] && $arrPost['muncod'] != "todos" && $arrPost['muncod'] != "")
		$arrFiltros[] = "d.dshcodmunicipio = '{$arrPost['muncod']}'";
	if($arrPost['regvalue'] && $arrPost['regvalue'] != "todos" && $arrPost['regvalue'] != ""){
		$sql = "select rgaidentificador,rgafiltro from painel.regagreg where regid = $regid";
		$campoReg = $db->pegaLinha($sql);
		$arrFiltros[] = str_replace(array("AND","and","{".$campoReg['rgaidentificador']."}"),array("","",$arrPost['regvalue']),$campoReg['rgafiltro']);
	}
	if($arrPost['tidid1'] && $arrPost['tidid1'] != "todos" && $arrPost['tidid1'] != ""){
		$sql = "select
					tdinumero 
				from 
					painel.detalhetipoindicador d1
				inner join
					painel.detalhetipodadosindicador d2 ON d1.tdiid = d2.tdiid
				where 
					d2.tidid = {$arrPost['tidid1']}";
		$tdinumero1 = $db->pegaUm($sql);
		$arrFiltros[] = "tidid$tdinumero1 = '{$arrPost['tidid1']}'";
	}if($arrPost['tidid2'] && $arrPost['tidid2'] != "todos" && $arrPost['tidid2'] != ""){
		$sql = "select 
					tdinumero 
				from 
					painel.detalhetipoindicador d1
				inner join
					painel.detalhetipodadosindicador d2 ON d1.tdiid = d2.tdiid
				where 
					d2.tidid = {$arrPost['tidid2']}";
		$tdinumero2 = $db->pegaUm($sql);
		$arrFiltros[] = "tidid$tdinumero2 = '{$arrPost['tidid2']}'";	
	}
	
	// Início - Monta os Agrupadores
	if(is_array($arrPost['agrupador'])){
		foreach($arrPost['agrupador'] as $agrupador){
			if(is_numeric($agrupador)){
				$sql = "select tdinumero, tdidsc from painel.detalhetipoindicador where tdiid = $agrupador";
				$dado = $db->pegaLinha($sql);
				$arrAgrup[] = array("campo" => "tiddsc".$dado['tdinumero'],
									"label" => $dado['tdidsc']
								    );
				//Campos para o SQL
				$arrCampos[] = "tiddsc".$dado['tdinumero'];
				//Group by
				$arrGroupBy[] = "tiddsc".$dado['tdinumero'];
				//Coluna p/ Excel
				$arrParametros['colunasExcel'][] = $dado['tdidsc'];
			}else{
				if($agrupador == "instituicaoprofissional" || $agrupador == "instituicaosuperior"){
					$arrAgrup[] =  array("campo" => $agrupador,
										 "label" => "Instituição"
										 );
					
					//Campos para o SQL
					$arrCampos[] = $agrupador;
					//Group by
					$arrGroupBy[] = $agrupador;
					//Coluna p/ Excel
					$arrParametros['colunasExcel'][] = "Instituição";
					
				}else{
					$arrAgrup[] =  array("campo" => $agrupador,
										 "label" => getAgrupadorPorCampo($agrupador,$regid)
										 );
					
					//Campos para o SQL
					$arrCampos[] = $agrupador;
					//Group by
					$arrGroupBy[] = $agrupador;
					//Coluna p/ Excel
					$arrParametros['colunasExcel'][] = getAgrupadorPorCampo($agrupador,$regid);
				}
			}
		}
	}
	// Fim - Monta os Agrupadores

	// Início - Monta as Colunas
	$arrCamposInterno = $arrCampos;
	$arrColunas[] = "qtde";

	if($unmid == UNIDADEMEDICAO_MOEDA) {
		$arrLabelColunas[] = array("label" => "R$" , "campo" => "qtde");
		$arrParametros['colunasExcel'][] = "R$";
		$arrCampos[] = "sum(qtde) as qtde";		
	}elseif($unmid == UNIDADEMEDICAO_PERCENTUAL) {
		$arrLabelColunas[] = array("label" => "Porcentagem" , "campo" => "qtde");
		$arrParametros['colunasExcel'][] = "Porcentagem";
		$arrCampos[] = "sum(qtde) || '%' as qtde";		
	} else {
		$arrLabelColunas[] = array("label" => "Quantidade" , "campo" => "qtde", "type" => "numeric");
		$arrParametros['colunasExcel'][] = "Quantidade";
		$arrCampos[] = "sum(qtde) as qtde";
	}

	if($indqtdevalor == "t"){
		$arrColunas[] = "valor";
		$arrCampos[] = "sum(valor) as valor";
		$arrLabelColunas[] = array("label" => "Valor" , "campo" => "valor");
		$arrParametros['colunasExcel'][] = "Valor";
	}
	// Fim - Monta as Colunas
	
	$arrGroupByInterno = $arrGroupBy;
	
	if(in_array("dp.dpedsc",$arrCampos)){
		foreach($arrCampos as $key => $campo){
			if($campo == "dp.dpedsc"){
				$arrAgrup[$key] = array("campo" => "dpedsc" , "label" => "Período");
				$arrParametros['colunasExcel'][$key] = "Período";
				$arrGroupBy[] = "dpedatainicio";
			} 
		}
	}
	
	$arrParametros['agrupadores'] = array(	
											"agrupador" 		=> $arrAgrup,
								 			"agrupadoColuna" 	=> $arrColunas
										  );
	
	$arrParametros['colunas'] = $arrLabelColunas;
										  
	//Início - Inner
	if(in_array("entnome",$arrCampos)){
		if($excel){
			foreach($arrCamposInterno as $key => $campoInterno){
				if($campoInterno == "entnome"){
					unset($arrColunasExcel);
					foreach($arrParametros['colunasExcel'] as $chave => $arrExcel):
						if($arrExcel == "Campus (Profissional)" || $arrExcel == "Campus (Superior)" || $arrExcel == "Hospital"){
							$arrColunasExcel[] = $arrExcel;
							$arrColunasExcel[] = "Código $arrExcel";
						}else{
							$arrColunasExcel[] = $arrExcel;
						}
					endforeach;
					$arrParametros['colunasExcel'] = $arrColunasExcel;
					$arrCampos[$key] = "entnome , dp.entid ";
					$arrCamposInterno[$key] = "entnome , d.entid ";
					$arrGroupByInterno[] = "d.entid";
					$arrGroupBy[] = "dp.entid";
					break;
				}
			}
		}else{
			foreach($arrCamposInterno as $key => $campoInterno){
				if($campoInterno == "entnome"){
					$arrCamposInterno[$key] = "(entnome || '<span style=\"display:none\" >' || ent.entid || '</span>') as entnome";
					$arrGroupByInterno[] = "ent.entid";
					break;
				}
			}
		}
		$arrInner[] = "entidade.entidade ent ON ent.entid = d.entid";
	}if(in_array("iepdsc",$arrCampos)){
		if($excel){
			foreach($arrCamposInterno as $key => $campoInterno){
				if($campoInterno == "iepdsc"){
					unset($arrColunasExcel);
					foreach($arrParametros['colunasExcel'] as $chave => $arrExcel):
						if($arrExcel == "Pós-Graduação"){
							$arrColunasExcel[] = $arrExcel;
							$arrColunasExcel[] = "Código $arrExcel";
						}else{
							$arrColunasExcel[] = $arrExcel;
						}
					endforeach;
					$arrParametros['colunasExcel'] = $arrColunasExcel;
					$arrCampos[$key] = "iepdsc , dp.iepid ";
					$arrCamposInterno[$key] = "iepdsc , d.iepid ";
					$arrGroupByInterno[] = "d.iepid";
					$arrGroupBy[] = "dp.iepid";
					break;
				}
			}
		}else{
			foreach($arrCamposInterno as $key => $campoInterno){
				if($campoInterno == "iepdsc"){
					$arrCamposInterno[$key] = "(iepdsc || '<span style=\"display:none\" >' || iep.iepid || '</span>') as iepdsc";
					$arrGroupByInterno[] = "iep.iepid";
					break;
				}
			}
		}
		$arrInner[] = "painel.iepg iep ON iep.iepid = d.iepid";
	}if(in_array("unidsc",$arrCampos)){
	if($excel){
			foreach($arrCamposInterno as $key => $campoInterno){
				if($campoInterno == "unidsc"){
					unset($arrColunasExcel);
					foreach($arrParametros['colunasExcel'] as $chave => $arrExcel):
						if($arrExcel == "Instituto" || $arrExcel == "Universidade"){
							$arrColunasExcel[] = $arrExcel;
							$arrColunasExcel[] = "Código $arrExcel";
						}else{
							$arrColunasExcel[] = $arrExcel;
						}
					endforeach;
					$arrParametros['colunasExcel'] = $arrColunasExcel;
					$arrCampos[$key] = "unidsc , dp.unicod ";
					$arrCamposInterno[$key] = "unidsc , d.unicod ";
					$arrGroupByInterno[] = "d.unicod";
					$arrGroupBy[] = "dp.unicod";
					break;
				}
			}
		}else{
			foreach($arrCamposInterno as $key => $campoInterno){
				if($campoInterno == "unidsc"){
					$arrCamposInterno[$key] = "(unidsc || '<span style=\"display:none\" >' || uni.unicod || '</span>') as unidsc";
					$arrGroupByInterno[] = "uni.unicod";
					break;
				}
			}
		}
		$arrInner[] = "public.unidade uni ON uni.unicod = d.unicod";
	}if(in_array("mundescricao",$arrCampos)){
		if($excel){
			foreach($arrCamposInterno as $key => $campoInterno){
				if($campoInterno == "mundescricao"){
					unset($arrColunasExcel);
					foreach($arrParametros['colunasExcel'] as $chave => $arrExcel):
						if($arrExcel == "Município"){
							$arrColunasExcel[] = $arrExcel;
							$arrColunasExcel[] = "Código IBGE";
						}else{
							$arrColunasExcel[] = $arrExcel;
						}
					endforeach;
					$arrParametros['colunasExcel'] = $arrColunasExcel;
					$arrCampos[$key] = "mundescricao , dp.dshcodmunicipio ";
					$arrCamposInterno[$key] = "mundescricao , d.dshcodmunicipio ";
					$arrGroupByInterno[] = "d.dshcodmunicipio";
					$arrGroupBy[] = "dp.dshcodmunicipio";
					break;
				}
			}
		}else{
			foreach($arrCamposInterno as $key => $campoInterno){
				if($campoInterno == "mundescricao"){
					$arrCamposInterno[$key] = "(mundescricao || '<span style=\"display:none\" >' || mun.muncod || '</span>') as mundescricao";
					$arrGroupByInterno[] = "mun.muncod";
					break;
				}
			}
		}
		$arrInner[] = "territorios.municipio mun ON mun.muncod = d.dshcodmunicipio";
	}
	if(in_array("poldsc",$arrCampos)){
		$arrInner[] = "painel.polo pol ON pol.polid = d.polid";
		if($excel){
			foreach($arrCamposInterno as $key => $campoInterno){
				if($campoInterno == "poldsc"){
					unset($arrColunasExcel);
					foreach($arrParametros['colunasExcel'] as $chave => $arrExcel):
						if($arrExcel == "Pólo"){
							$arrColunasExcel[] = $arrExcel;
							$arrColunasExcel[] = "Código do Polo";
						}else{
							$arrColunasExcel[] = $arrExcel;
						}
					endforeach;
					$arrParametros['colunasExcel'] = $arrColunasExcel;
					$arrCampos[$key] = "poldsc , dp.polid ";
					$arrCamposInterno[$key] = "poldsc , d.polid ";
					$arrGroupByInterno[] = "d.polid";
					$arrGroupBy[] = "dp.polid";
					break;
				}
			}
		}else{
			foreach($arrCamposInterno as $key => $campoInterno){
				if($campoInterno == "poldsc"){
					$arrCamposInterno[$key] = "(poldsc || '<span style=\"display:none\" >' || pol.polid || '</span>') as poldsc";
					$arrGroupByInterno[] = "pol.polid";
					break;
				}
			}
		}
	}
	if(in_array("instituicaosuperior",$arrCampos)){
		foreach($arrCamposInterno as $key => $campoInterno){
			if($campoInterno == "instituicaosuperior"){
				$arrCamposInterno[$key] = "( 
												select 
													ent2.entnome
												from
													entidade.entidade ent
												inner join
													entidade.funcaoentidade fun ON fun.entid = ent.entid and fun.funid = 18
												inner join
													entidade.funentassoc fea ON fea.fueid = fun.fueid
												inner join
													entidade.entidade ent2 ON ent2.entid = fea.entid
												inner join
													entidade.funcaoentidade fun2 ON fun2.entid = ent2.entid and fun2.funid = 12
												where
													ent.entid = d.entid
											) as instituicaosuperior";
				$arrGroupByInterno[] = "instituicaosuperior";
				break;
			}
		}
	}
	if(in_array("instituicaoprofissional",$arrCampos)){
		foreach($arrCamposInterno as $key => $campoInterno){
			if($campoInterno == "instituicaoprofissional"){
				$arrCamposInterno[$key] = "( 
												select 
													ent2.entnome
												from
													entidade.entidade ent
												inner join
													entidade.funcaoentidade fun ON fun.entid = ent.entid and fun.funid = 17
												inner join
													entidade.funentassoc fea ON fea.fueid = fun.fueid
												inner join
													entidade.entidade ent2 ON ent2.entid = fea.entid
												inner join
													entidade.funcaoentidade fun2 ON fun2.entid = ent2.entid and fun2.funid = 11
												where
													ent.entid = d.entid
											) as instituicaoprofissional";
				$arrGroupByInterno[] = "instituicaoprofissional";
				break;
			}
		}
	}
	if(in_array("iesdsc",$arrCampos)){
		$arrInner[] = "painel.ies ies ON ies.iesid::integer = d.dshcod::integer";
		if($excel){
			foreach($arrCamposInterno as $key => $campoInterno){
				if($campoInterno == "iesdsc"){
					unset($arrColunasExcel);
					foreach($arrParametros['colunasExcel'] as $chave => $arrExcel):
						if($arrExcel == "IES"){
							$arrColunasExcel[] = $arrExcel;
							$arrColunasExcel[] = "Código $arrExcel";
						}else{
							$arrColunasExcel[] = $arrExcel;
						}
					endforeach;
					$arrParametros['colunasExcel'] = $arrColunasExcel;
					$arrCampos[$key] = "iesdsc , dp.dshcod ";
					$arrCamposInterno[$key] = "ies.iesdsc , d.dshcod ";
					$arrGroupByInterno[] = "d.dshcod";
					$arrGroupBy[] = "dp.dshcod";
					break;
				}
			}
		}else{
			foreach($arrCamposInterno as $key => $campoInterno){
				if($campoInterno == "iesdsc"){
					$arrCamposInterno[$key] = "'<span style=\"display:none\" >' || ies.iesid || '</span>' || ies.iesdsc as iesdsc";
					$arrGroupByInterno[] = "ies.iesid";
					break;
				}
			}
		}
	}if(in_array("escdsc",$arrCampos)){
		if($excel){
			foreach($arrCamposInterno as $key => $campoInterno){
				if($campoInterno == "escdsc"){
					unset($arrColunasExcel);
					foreach($arrParametros['colunasExcel'] as $chave => $arrExcel):
						if($arrExcel == "Escola"){
							$arrColunasExcel[] = $arrExcel;
							$arrColunasExcel[] = "Código INEP";
						}else{
							$arrColunasExcel[] = $arrExcel;
						}
					endforeach;
					$arrParametros['colunasExcel'] = $arrColunasExcel;
					$arrCampos[$key] = "escdsc , esccodinep ";
					$arrCamposInterno[$key] = "esc.escdsc , esc.esccodinep ";
					$arrGroupByInterno[] = "esc.esccodinep";
					$arrGroupBy[] = "esccodinep";
					break;
				}
			}
		}else{
			foreach($arrCamposInterno as $key => $campoInterno){
				if($campoInterno == "escdsc"){
					$arrCamposInterno[$key] = "'<span style=\"display:none\" >' || esc.esccodinep || '</span>' || esc.escdsc as escdsc";
					$arrGroupByInterno[] = "esc.esccodinep";
					break;
				}
			}
		}
		$arrInner[] = "painel.escola esc ON esc.esccodinep::text = d.dshcod::text";
	}if(in_array("iecdsc",$arrCampos)){
		if($excel){
			foreach($arrCamposInterno as $key => $campoInterno){
				if($campoInterno == "iecdsc"){
					unset($arrColunasExcel);
					foreach($arrParametros['colunasExcel'] as $chave => $arrExcel):
						if($arrExcel == "IES/Município"){
							$arrColunasExcel[] = $arrExcel;
							$arrColunasExcel[] = "Código $arrExcel";
						}else{
							$arrColunasExcel[] = $arrExcel;
						}
					endforeach;
					$arrParametros['colunasExcel'] = $arrColunasExcel;
					$arrCampos[$key] = "iecdsc , dp.iecid ";
					$arrCamposInterno[$key] = "ies.iecdsc , d.iecid ";
					$arrGroupByInterno[] = "d.iecid";
					$arrGroupBy[] = "dp.iecid";
					break;
				}
			}
		}else{
			foreach($arrCamposInterno as $key => $campoInterno){
				if($campoInterno == "iecdsc"){
					$arrCamposInterno[$key] = "'<span style=\"display:none\" >' || ies.iecid || '</span>' || ies.iecdsc as iecdsc";
					$arrGroupByInterno[] = "ies.iecid";
					break;
				}
			}
		}
		$arrInner[] = "painel.iescpc ies ON ies.iecid = d.iecid";
	//Fim - Inner
	}
	//Se o indicador for cumulativo e a coluna de Período não estiver presente, exibi-se apenas o último período de cada ítem
	if($indcumulativo == "N" && !in_array("dp.dpedsc",$arrCampos)){
		$whereDataInterna = "";
		if($arrPost['periodo_inicio']){
			$whereDataInterna .= " d1.dpedatainicio >= ( select dpedatainicio from painel.detalheperiodicidade where dpeid = {$arrPost['periodo_inicio']}) and ";
		}
		if($arrPost['periodo_fim']){
			$whereDataInterna .= " d1.dpedatainicio <= ( select dpedatafim from painel.detalheperiodicidade where dpeid = {$arrPost['periodo_fim']}) and ";
		}
	}else{
		$whereDataInterna = " d1.dpedatainicio>=dp.dpedatainicio 
							and 
								d1.dpedatafim<=dp.dpedatafim 
							and ";
	}
										  
	/* Início SQL */
	$arrParametros['sql'] = "	select 
									".(count($arrCampos) ? implode(" , ",$arrCampos) : "" )."
								from (
										select 
											dp.dpedatainicio,
											".(count($arrCamposInterno) ? implode(" , ",$arrCamposInterno)."," : "" )."
											case when d.indcumulativo = 'N' then
								        			case when (select 
						                        	d1.dpeid
						                        from 
						                        	painel.detalheperiodicidade d1
												inner join 
													painel.seriehistorica sh on sh.dpeid=d1.dpeid
												where
													$whereDataInterna 
													d1.dpedatainicio>=dp.dpedatainicio 
												and 
													d1.dpedatafim<=dp.dpedatafim 
												and 
													sh.indid=d.indid
												order by 
													d1.dpedatainicio desc 
												limit 
													1
				                				) = d.dpeid then sum(d.qtde)  
				                	else 0 end
								else sum(d.qtde)
							end as qtde,
											case when d.indcumulativovalor = 'N' then
				        			case when (
				                        		select 
				                        			d1.dpeid
				                        		from 
				                        			painel.detalheperiodicidade d1
				                                inner join 
				                                	painel.seriehistorica sh on sh.dpeid=d1.dpeid
				                                where 
				                                	$whereDataInterna
				                                	d1.dpedatainicio>=dp.dpedatainicio 
				                                and 
				                                	d1.dpedatafim<=dp.dpedatafim 
				                                and 
				                                	sh.indid=d.indid
				                                order by 
													d1.dpedatainicio desc 
												limit 
													1
				                				) = d.dpeid then sum(d.valor)
				                			else 0 end
									else sum(d.valor)
							end as valor
										from 
											painel.v_detalheindicadorsh d
										left join 
											painel.detalheperiodicidade dp on d.dpedatainicio>=dp.dpedatainicio and d.dpedatafim<=dp.dpedatafim
										".(count($arrInner) ? " left join ".implode(" left join ",$arrInner) : "" )." 
										-- periodo que vc quer exibir
										where 
											dp.perid = ".(!$arrPost['perid'] ? $perid : $arrPost['perid'])."
										-- indicador que vc quer exibir
										and 
											d.indid = $indid
										".(count($arrFiltros) ? " and ".implode(" and ",$arrFiltros) : "" )."
										--range de data compreendida no periodo
										".str_replace("dsh.","d.",$andDpe)."
										group by 
											d.indcumulativo,
											dp.dpedatainicio,
											dp.dpedatafim,
											d.indid,
											d.dpeid,
											d.indcumulativovalor 
											".(count($arrGroupByInterno) ? ",".implode(",",$arrGroupByInterno) : "")."
									) dp
									".(count($arrGroupBy) ? " group by ".implode(",",$arrGroupBy) : "")."
									".(count($arrGroupBy) ? " order by ".implode(",",$arrGroupBy) : "")."";	
	
	/* Fim SQL */
//	dbg($arrParametros['sql']);
	return $arrParametros;
	
}

function getAgrupadorPorCampo($campo,$regid){
	switch($campo){	
		case "dp.dpedsc":
			return "Período";
		break;
		case "escdsc":
			return "Escola";
		break;
		case "iesdsc":
			return "IES";
		break;
		case "mundescricao":
			return "Município";
		break;
		case "dshuf":
			return "Estado";
		break;
		case "iepdsc":
			return "Pós-Graduação";
		break;
		case "entnome":
			switch($regid){
				case REGIONALIZACAO_CAMPUS_SUPERIOR:
					return "Campus (Superior)";
				break;
				case REGIONALIZACAO_CAMPUS_PROFISSIONAL:
					return "Campus (Profissional)";
				break;
				case REGIONALIZACAO_HOSPITAL:
					return "Hospital";
				break;
			}
		break;
		case "unidsc":
			switch($regid){
				case REGIONALIZACAO_UNIVERSIDADE:
					return "Universidade";
				break;
				case REGIONALIZACAO_INSTITUTO:
					return "Instituto";
				break;
			}
		break;
		case "poldsc":
			return "Pólo";
		break;
		case "iecdsc":
			return "IES/Município";
		break;
		default:
			return false;
	}
}

function getResponsavelSecretariaIndicador($indid = null)
{
	global $db;
	
	if(!indid)
		return false;

	$sql = "select
					respnome,
					respemail,
					'(' || respdddtelefone || ')' || resptelefone as telefone,
					'(' || respdddcelular || ')' || respcelular as celular
				from 
					painel.responsavelsecretaria 
				where 
					secid = ( select secid from painel.indicador where indid = $indid ) 
				and 
					respstatus = 'A'
				order by
					respnome";
	return $db->carregar($sql);
}


function listaIndicadorFormulaSHPopup()
{
	global $db;
	$perid = $_GET['perid'];
	$regid = $_GET['regid'];
	?>
	<html>
		<head>
			<script language="JavaScript" src="../includes/funcoes.js"></script>
			<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
			<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
			<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
		</head>
	<body>
		<script>
			function addTema(indid,indnome)
			{
				var hdn_indid = $("#hdn_indid",window.opener.document).val();
				var dpe = $("#indid_dpe_" + indid).val();

				if(!hdn_indid){
					if(!dpe){
						$("#hdn_indid",window.opener.document).val(indid);
					}else{
						$("#hdn_indid",window.opener.document).val(indid + "_" + dpe);
					}
				}else{
					if(!dpe){
						$("#hdn_indid",window.opener.document).val(hdn_indid + "," + indid);
					}else{
						$("#hdn_indid",window.opener.document).val(hdn_indid + "," + indid + "_" + dpe);
					}
				}
				
				var formula = $("#formulash",window.opener.document).val();
				if(!dpe){
					$("#formulash",window.opener.document).val(formula + "{Indicador: " + indid + "}");
				}else{
					$("#formulash",window.opener.document).val(formula + "{Indicador: " + indid + "_" + $("#indid_dpe_" + indid + " option:selected").text() + "}");
				}
				var detalhe = $("#div_detalhe_formula",window.opener.document).html();
				if(!dpe){
					if(detalhe.search( "Indicador: " + indid + " - " + indnome + " - Padrão" ) < 0){
						$("#div_detalhe_formula",window.opener.document).html(detalhe + "<p><b>Indicador: " + indid + "</b> - "+ indnome + " - Padrão</p>");
					}
				}else{
					if(detalhe.search( "Indicador: " + indid + " - "+ indnome + " em " + $("#indid_dpe_" + indid + " option:selected").text() ) < 0){
						$("#div_detalhe_formula",window.opener.document).html(detalhe + "<p><b>Indicador: " + indid + "</b> - "+ indnome + " em " + $("#indid_dpe_" + indid + " option:selected").text() + "</p>");
					}
				}
				
			}
		</script>
		<?php monta_titulo("Indicadores","Selecione os indicadores para compor a fórmula"); ?>
		<form name="formulario" id="formulario"  method="post" action="" >
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td width="25%" class="SubtituloDireita" >Indicador:</td>
				<td><?php $indnome = $_POST['indnome']; echo campo_texto("indnome","S","S","",40,250,"","") ?></td>
			</tr>
			<tr>
				<td class="SubtituloDireita" ></td>
				<td class="SubtituloEsquerda" ><input type="submit" name="btn_buscar" value="Buscar" /><input type="button" name="btn_todos" onclick="window.location.href=window.location" value="ver Todos" /></td>
			</tr>
		</table>
	</form>
	<?php
	if($_POST['indnome']){
		$arrWhere[] = "removeacento(indnome) ilike(removeacento('%{$_POST['indnome']}%'))";
		$arrWhere[] = "ind.indid::text ilike('%{$_POST['indnome']}%')";
	}
	$sql = "select distinct
			'<img src=\"../imagens/gif_inclui.gif\" style=\"cursor:pointer\" onclick=\"addTema(' || ind.indid || ',\'' || indnome || '\')\" />' as acao,
			ind.indid || ' - ' || indnome as indnome,
			ind.indid
		from
			painel.indicador ind
		inner join
			painel.periodicidade per ON per.perid = ind.perid
		inner join
			painel.seriehistorica seh ON seh.indid = ind.indid
		where
			indstatus = 'A'
		and
			per.pernivel <= (select per2.pernivel from painel.periodicidade per2 where per2.perid = $perid)
		and
			ind.regid = $regid
		".($arrWhere ? " and (".implode(" or ",$arrWhere).")" : "")."
		order by
			indnome";
	$arrCab = array("Ação","Indicador","Referência");

	$arrDados = $db->carregar($sql);
	if($arrDados){
		foreach($arrDados as $n => $dado){
			$sqlI = "select 
						dpe.dpeid as codigo,
						dpedsc as descricao
					from 
						painel.detalheperiodicidade dpe
					inner join
						painel.seriehistorica seh ON seh.dpeid = dpe.dpeid
					where
						sehstatus != 'I'
					and
						dpe.dpestatus = 'A'
					and
						seh.indid = {$dado['indid']}
					and
						perid = $perid
					order by
						dpe.dpedatainicio";
			$arrOpcoes = $db->carregar($sqlI);
			if($arrOpcoes){
				array_unshift($arrOpcoes,array("codigo" => "","descricao" => "N/A"));
				unset($arrDados[$n]['indid']);
				$arrDados[$n]['referencia'] = $db->monta_combo("indid_dpe_{$dado['indid']}",$arrOpcoes,"S","","","","","150","N","indid_dpe_{$dado['indid']}",true);
			}else{
				$sqlI = "	select 
								dpeid as codigo,
								dpedsc as descricao
							from
								painel.detalheperiodicidade
							where 
								(
								select 
									dpe.dpedatainicio
								from 
									painel.detalheperiodicidade dpe
								inner join
									painel.seriehistorica seh ON seh.dpeid = dpe.dpeid
								where
									sehstatus != 'I'
								and
									dpe.dpestatus = 'A'
								and
									seh.indid = {$dado['indid']} limit 1) between dpedatainicio and dpedatafim
							and
								perid = $perid
							and
								dpestatus = 'A'
							order by
								dpedatainicio";
				if($sqlI){
					$arrOpcoes = $db->carregar($sqlI);
				}
				array_unshift($arrOpcoes,array("codigo" => "","descricao" => "N/A"));
				unset($arrDados[$n]['indid']);
				if($arrOpcoes){
					$arrDados[$n]['referencia'] = $db->monta_combo("indid_dpe_{$dado['indid']}",$arrOpcoes,"S","","","","","150","N","indid_dpe_{$dado['indid']}",true);
				}
			}
		}
	}
	$db->monta_lista_array($arrDados,$arrCab,50,10,"N","center");
	?>
		</body>
	</html>
	<?php
	exit;
}

function testarFormulaSHIndicador()
{
	global $db;
	ini_set("memory_limit", "3000M");
	set_time_limit(0);
	$formulash = $_POST['formulash'];
	$perid = $_POST['perid'];
	$regid = $_POST['regid'];
	
	if(!$formulash){
		echo "<p style=\"color:#990000\" ><b>Favor escrever uma fórmula!</b></p>";
		exit;
	}
	if(!$perid){
		echo "<p style=\"color:#990000\" ><b>Favor selecionar o Período.</b></p>";
		exit;
	}
	if(!$regid){
		echo "<p style=\"color:#990000\" ><b>Favor selecionar a Regionalização.</b></p>";
		exit;
	}
	$arrResultadoFinal = trataFormulaSHIndicador($formulash,$perid,$regid);
	
	if($arrResultadoFinal){
		$n=0;
		
		$sql = "select 
					rgaidentificador,
					regunidade,
					rgacomplemento,
					rgafiltro,
					regtabela
				from 
					painel.regagreg rega
				inner join
					painel.regionalizacao reg ON reg.regid = rega.regid 
				where 
					rega.regid = {$arrResultadoFinal['regid']}";
		$arrReg = $db->pegaLinha($sql);
		$regtabela = $arrReg['regtabela'];
		$regunidade = $arrReg['regunidade'];
		$rgacomplemento = $arrReg['rgacomplemento'];
		$arrCampo = explode(";",$rgacomplemento);
		$coluna_cod = $arrCampo[0];
		$coluna_des = $arrCampo[1];
		$sql = "select $coluna_cod as codigo, $coluna_des as descricao from $regtabela order by descricao";
		$arrCodigo = $db->carregar($sql);
		foreach($arrCodigo as $cod){
			$arrReg[$cod['codigo']] = $cod['descricao'];
		}
		
		unset($arrResultadoFinal['regid']);
		
		echo '<table border="0" cellspacing="0" cellpadding="3" align="left" width="400"  style="margin-top:5px;border-top: none; border-bottom: none;">';
		echo "<tr bgcolor='#D5D5D5' ><td><b>$regunidade</b></td><td><b>Qtde.</b></td></tr>";
		foreach($arrResultadoFinal as $codigo => $valor){
			if($n <= 10){
				$sqlA = $sql."'$muncod'";
				$descricao = $arrReg[$codigo];
				$cor = $n%2 == 1 ? "#f0f0f0" : "#FFFFFF";
				echo "<tr bgcolor='$cor' ><td >".ucwords(strtolower($descricao))."</td><td style='color:blue;text-align:right' >".number_format($valor,2,",",".")."</td></tr>";
			}else{
				exit;
			}
			$n++;
		}
		echo "</table>";
	}else{
		echo "<p style=\"color:#990000\" ><b>Fórmula inválida!</b></p>";
	}
	exit;
}

function trataFormulaSHIndicador($tmaformula,$perid,$regid,$dpeid = null){
	global $db;
	
	$arrItens = array();
	$tmaformula = utf8_decode($tmaformula);
	//Procurar por parenteses
	if(strstr($tmaformula,"(")){
		$arr1 = explode("(",$tmaformula);
		foreach($arr1 as $a1){
			if(strstr($a1,")")){
				$arr2 = explode(")",$a1);
				foreach($arr2 as $a2){
					if(strstr($a2,"{")){
						$arr3 = explode("{",$a2);
						foreach($arr3 as $a3){
							if(strstr($a3,"}")){
								$fim = strpos($a3,"}");
								$item = substr($a3,0,$fim);
								if(!in_array($item,$arrItens)){
									$arrItens[] = $item;
								}
							}
						}
					}
				}
			}
		}
	}else{ //Sem parenteses
		if(strstr($tmaformula,"{")){
			$arr3 = explode("{",$tmaformula);
			foreach($arr3 as $a3){
				if(strstr($a3,"}")){
					$fim = strpos($a3,"}");
					$item = substr($a3,0,$fim);
					if(!in_array($item,$arrItens)){
						$arrItens[] = $item;
					}
				}
			}
		}
	}
	
	if($arrItens[0]){
		foreach($arrItens as $i){
			if(strstr($i,"Indicador:")){
				$indicador = str_replace(array("Indicador:"," "),array("",""),$i);
				$ind = explode("_",$indicador);
				$indid  = $ind[0];
				$dpedsc = $ind[1];
				
				if(!$dpedsc && $dpeid){
					$sql = "select dpedsc from painel.detalheperiodicidade where dpeid = $dpeid and perid = $perid";
					$dpedsc = $db->pegaUm($sql);
				}
				if(!$dpedsc){
					$sql = "select dpedsc from painel.detalheperiodicidade where dpeid = ( select dpeid from painel.seriehistorica where sehstatus = 'A' and indid = $indid limit 1)";
					$dpedsc = $db->pegaUm($sql);
				}
				
				$sql = "select 
							dpedatainicio, 
							dpedatafim 
						from 
							painel.detalheperiodicidade 
						where
							( select dpedatainicio from painel.detalheperiodicidade where dpedsc = '$dpedsc' ) between dpedatainicio and dpedatafim
						and
							( select dpedatafim from painel.detalheperiodicidade where dpedsc = '$dpedsc' ) between dpedatainicio and dpedatafim
						and 
							perid = $perid";
				$arrDpe = $db->pegaLinha($sql);
				
				$sql = "select 
							rgaidentificador,
							regunidade,
							rgacomplemento,
							rgafiltro,
							regtabela
						from 
							painel.regagreg rega
						inner join
							painel.regionalizacao reg ON reg.regid = rega.regid 
						where 
							rega.regid = $regid";
				$arrReg = $db->pegaLinha($sql);
								
				$rgaidentificador = $arrReg['rgaidentificador'];
				$rgacomplemento = $arrReg['rgacomplemento'];
				$rgafiltro = $arrReg['rgafiltro'];
				$regtabela = $arrReg['regtabela'];
				$arrCampo = explode(";",$rgacomplemento);
				$coluna_cod = $arrCampo[0];
				$coluna_desc = $arrCampo[1];
				$group_by = str_replace(";",",",$rgacomplemento);
				$campo_on = explode(";",$rgacomplemento);
				$campOn = explode(".",$campo_on[0]);
				$on = str_replace("AND","ON dsh.dshcod::text = ",$rgafiltro);
				$on = str_replace(array("AND d.","=","'{".$campOn[1]."}'"),array("ON dsh.","::text = ",$campo_on[0]."::text"),$rgafiltro);
				$arrAlias = explode("as",$regtabela);
				$alias = $arrAlias[1];
								
				if($regid == REGIONALIZACAO_BRASIL){
					$sql = "select 
							coalesce(sum(dsh.dshqtde),0) as qtde,
							1 as codigo,
							'Brasil' as descricao
						from 
							painel.detalheseriehistorica dsh
						inner join
							painel.seriehistorica seh ON dsh.sehid = seh.sehid
						inner join
							painel.indicador ind ON seh.indid = ind.indid					
						inner join
							painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
						$inner_join
						where 
							ind.indid = $indid 
						and 
							dpe.dpedatainicio >= '".$arrDpe['dpedatainicio']."'
						and 
							dpe.dpedatafim <= '".$arrDpe['dpedatafim']."'
						$where
						order by
							descricao";
				}else{
					$sql = "select 
							coalesce(sum(dsh.dshqtde),0) as qtde,
							$coluna_cod as codigo,
							$coluna_desc as descricao
						from 
							$regtabela
						inner join
							painel.detalheseriehistorica dsh $on
						inner join
							painel.seriehistorica seh ON dsh.sehid = seh.sehid
						inner join
							painel.indicador ind ON seh.indid = ind.indid					
						inner join
							painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
						$inner_join
						where 
							ind.indid = $indid 
						and 
							dpe.dpedatainicio >= '".$arrDpe['dpedatainicio']."'
						and 
							dpe.dpedatafim <= '".$arrDpe['dpedatafim']."'
						$where
						group by
							$group_by
						order by
							descricao";	
				}
				$arrDados[$i] = $db->carregar($sql);
				if($arrDados[$i]){
					foreach($arrDados[$i] as $a){
						$arrQtde[$i][$a['codigo']] = $a['qtde'];
					}
				}else{
					$arrFormulaRemove[] = "{".$i."}";
				}
			}
			
		}
	}
	
	$sql = "select $coluna_cod from $regtabela";
	$arrCodigo = $db->carregarColuna($sql);
	
	foreach($arrQtde as $chave => $arrC){
		foreach($arrCodigo as $m1){
			if($arrQtde[$chave][$m1]){
				if($arrResultado[$m1]){
					$arrResultado[$m1] = str_replace("{".$chave."}",$arrQtde[$chave][$m1],$arrResultado[$m1]);
				}else{
					$arrResultado[$m1] = str_replace("{".$chave."}",$arrQtde[$chave][$m1],$tmaformula);
				}
			}
		}
	}
	$arrResultado = $arrResultado ? $arrResultado : array();
	foreach($arrResultado as $chave => $valor){
		foreach($arrQtde as $formula => $arrC){
			$arrResultado[$chave] = str_replace(array("+{".$formula."}","-{".$formula."}","/{".$formula."}","*{".$formula."}"),array("+0","-0","/1","*1"),$arrResultado[$chave]);
			if($arrFormulaRemove){
				foreach($arrFormulaRemove as $form_rem){
					$arrResultado[$chave] = str_replace(array("+".$form_rem."","-".$form_rem."","/".$form_rem."","*".$form_rem.""),array("+0","-0","/1","*1"),$arrResultado[$chave]);
				}
			}
		}
	}
	if($arrResultado){
		foreach($arrResultado as $chave => $res){
			if(!strstr($res,"Tema") && !strstr($res,"Indicador")){
				$val = false;
				eval('$val = '.$res.';');
				$arrResultadoFinal[$chave] = $val;
				//dbg($res." = ".$val);
			}
		}
	}
	foreach($arrCodigo as $cod){
		if(!$arrResultadoFinal[$cod]){
			$arrResultadoFinal[$cod] = 0;
		}
	}
	$arrResultadoFinal["regid"] = $regid;
	return $arrResultadoFinal;
}

function executarFormulaSH($dados)
{
	global $db;
	ini_set("memory_limit", "3000M");
	set_time_limit(0);
	
	$sql = "select perid,regid,formulash from painel.indicador where indid = ".$dados['indid'];
	$arrDados = $db->pegaLinha($sql);
	$formulash = $arrDados['formulash'];
	if(!$formulash){
		exit;
	}
	$arrResultadoFinal = trataFormulaSHIndicador($formulash,$arrDados['perid'],$arrDados['regid'],$dados['dpeid']);
	
	$sql = "select rgaidentificador,rgafiltro from painel.regagreg where regid = ".$arrResultadoFinal['regid'];
	$arrReg = $db->pegaLinha($sql);
	unset($arrResultadoFinal['regid']);
	//"AND d.dshcodmunicipio='{muncod}'"
	$arrCampo = str_replace(array("AND d."),array(""),$arrReg['rgafiltro']);
	$campo1 = explode("=",$arrCampo);
	$campo = $campo1[0];
	
	$formatoinput = pegarFormatoInput($dados['indid']);
	
	$sql = "select sehid from painel.seriehistorica where dpeid = {$dados['dpeid']} and indid = {$dados['indid']}";
	$sehid = $db->pegaUm($sql);
	if(!$sehid){
		$sql = "insert into painel.seriehistorica 
					(indid,sehvalor,sehstatus,sehqtde,dpeid,sehdtcoleta,regid,sehbloqueado,dmiid) 
				values 
					({$dados['indid']},0,'A',0,{$dados['dpeid']},NOW(),{$arrDados['regid']},false,NULL)
				returning 
					sehid";
		$sehid = $db->pegaUm($sql);
	}
	$sql = "update painel.seriehistorica set sehstatus = 'H' where sehstatus = 'A' and sehid != $sehid and indid = {$dados['indid']};
			delete from painel.detalheseriehistorica where sehid = $sehid";
	$db->executar($sql);
	
	foreach($arrResultadoFinal as $identificador => $valor){
		$sqlI .= "insert into painel.detalheseriehistorica
					(sehid,dshqtde,".str_replace("::integer","",$campo).")
				values
					($sehid,$valor,'$identificador');";
		$sehqtde+=$valor;
	}
	$sqlI .= "update painel.seriehistorica set sehqtde = $sehqtde, sehstatus = 'A' where sehid = $sehid;";
	$db->executar($sqlI);
	$db->commit();
	
}

function popupImportarTerritorios($arrDados)
{
	global $db; 
	extract($arrDados);
	?>
	<head>
		<script language="JavaScript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
		<script>
			function verificaImportar()
			{
				var erro = 0;
				if(!jQuery("[name='ddsid']").val()){
					alert('Selecione o indicador.');
					erro = 1;
					return false;
				}
				if(!jQuery("[name='dpeid']").val()){
					alert('Selecione o período.');
					erro = 1;
					return false;
				}
				if(erro == 0){
					jQuery("#div_bt_acao").html("");
					jQuery("#div_bt_acao2").show();
					jQuery("#form_importar").submit();
				}
			}
		</script>
	</head>
	<body>
		<?php monta_titulo("Importar","Selecione o indicador e o período para importar a série histórica"); ?>
		<form name="form_importar" id="form_importar"  method="post" action="" >
			<input type="hidden" name="requisicao" value="importarTerritorios" >
			<input type="hidden" name="indid" value="<?php echo $indid ?>" >
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr>
					<td width="25%" class="SubtituloDireita" >Indicador:</td>
					<td>
						<?php
							$sql = "select
										dds.ddsid as codigo,
										trim(ddsdescricao) as descricao
									from
										territoriosgeo.descricaodadosetor dds
									inner join
										territoriosgeo.dadosetor ds ON ds.ddsid = dds.ddsid 
									where
										ddsstatus = 'A'
									group by 
										dds.ddsid,ddsdescricao
									order by
										descricao";
							$db->monta_combo('ddsid',$sql,'S','Selecione...','','','','','S','','','');
						?>
					</td>
				</tr>
				<tr>
					<td class="SubtituloDireita" >Período:</td>
					<td>
						<?php
							$sql = "select
										dpe.dpeid as codigo,
										dpe.dpedsc as descricao
									from
										painel.detalheperiodicidade dpe
									inner join
										painel.periodicidade per ON per.perid = dpe.perid
									inner join
										painel.indicador ind ON ind.perid = per.perid
									where
										ind.indid = $indid
									order by
										dpedatainicio,dpeordem";
							$db->monta_combo('dpeid',$sql,'S','Selecione...','','','','','S','','','');
						?>
					</td>
				</tr>
				<tr>
					<td class="SubtituloDireita" ></td>
					<td class="SubtituloEsquerda" >
						<div id="div_bt_acao" >
							<input type="button" onclick="verificaImportar()" name="btn_importar" value="Importar" />
							<input type="button" name="btn_cancelar" onclick="window.close();" value="Cancelar" />
						</div>
						<div id="div_bt_acao2" style="display:none" >
							<input type="button" name="btn_aguarde" disabled="disabled" value="Aguarde... Importando" />
						</div>
					</td>
				</tr>
			</table>
		</form>
	</body>
	<?php
}

function importarTerritorios($arrDados)
{
	global $db;
	extract($arrDados);
	
	$sql = "select sehid from painel.seriehistorica where dpeid = $dpeid and indid = $indid";
	$sehid = $db->pegaUm($sql);
	if(!$sehid){
		$sql = "insert into painel.seriehistorica (indid,sehqtde,dpeid) values ($indid,0,$dpeid) returning sehid";
		$sehid = $db->pegaUm($sql);
	}
	$sql = "delete from painel.detalheseriehistorica where sehid = $sehid;";
	$sql.= "insert into painel.detalheseriehistorica (sehid,dshcod,dshqtde)
			select $sehid, setid, qtde from territoriosgeo.dadosetor where ddsid = $ddsid;";
	$sql.= "update painel.seriehistorica set sehstatus = 'A' where indid = $indid and sehid = $sehid;";
	$sql.= "update painel.seriehistorica set sehstatus = 'H' where indid = $indid and sehid != $sehid and sehstatus != 'I';";
	$sql.= "update painel.seriehistorica set sehqtde = (select coalesce(sum(dshqtde),0) from painel.detalheseriehistorica dsh2 where dsh2.sehid = $sehid) where sehid = $sehid;";
	$db->executar($sql);
	$db->commit();
	echo "<script>alert('Operação realizada com sucesso!');opener.location.reload();</script>";
}

function atualizarSHFormula()
{
	global $db;
	ini_set("memory_limit", "3000M");
	set_time_limit(0);
	extract($_GET);
	if($arrIndid){
		foreach($arrIndid as $indid){
			$sql = "select dpeid from painel.seriehistorica where indid = $indid and sehstatus != 'I' order by sehdtcoleta";
			$arrDpe = $db->carregarColuna($sql);
			foreach($arrDpe as $dpeid){
				$arrDados = array("indid" => $indid, "dpeid" => $dpeid);
				executarFormulaSH($arrDados);	
			}
		}
	}
	echo "<script>alert('Operação realizada com sucesso!');window.location.href='painel.php?modulo=principal/dependenciaIndicador&acao=A';</script>";
}

function trataFormulaPorcentagem($formula,$aliasSerieHistorica = "seh",$where_reg = "",$regionalizadorMinimo = false)
{
	global $db;
		
	$hdn_indid = explode("{Indicador:",str_replace(array(" ","-","+","*"),array("","","",""),$formula));
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
						$arrIndFormula[$valor] = $dado[0]."_".$dpeid__;
					}
				}else{
					if($dado[0]){
						$arrIndFormula[$valor] = $dado[0];
					}
				}
			}
		}
	}
	$indid = "";
	if($arrIndFormula){
		$reg = explode(".",$where_reg);
		
		$n=1;
		foreach($arrIndFormula as $chave => $ind){
			if(strstr($ind,"_")){
				$d = explode("_",$ind);
				$indid = $d[0];
				$dpeid = $d[1];
			}else{
				$indid = $ind;
				$dpeid = false;
			}
			if(strstr($formula,"/{Indicador: $chave}")){
				if(!$regionalizadorMinimo){
					$arrIndFormula[$chave] = "(select 
													coalesce(sum(dsh$n.qtde),1) 
												from 
													painel.v_detalheindicadorsh dsh$n 
												where 
													dsh$n.indid = $indid ".
												($where_reg ? " and ".$where_reg." = dsh$n.".$reg[1]." " : "")." 
												and 
													dsh$n.dpeid = ".($dpeid ? "$dpeid" : "$aliasSerieHistorica.dpeid").")
												";
				}else{
					$arrIndFormula[$chave] = "(coalesce(dsh$n.qtde,1))";
					$arrIndInner[$chave] = "left join painel.v_detalheindicadorsh dsh$n ON ".$where_reg." = dsh$n.".$reg[1]." and dsh$n.dpeid = ".($dpeid ? "$dpeid" : "$aliasSerieHistorica.dpeid")." and dsh$n.indid = $indid and dsh$n.sehstatus = 'A' and dsh$n.qtde > 0";
				}
			}else{
				if(!$regionalizadorMinimo){
					$arrIndFormula[$chave] = "(coalesce( (select 
													coalesce(sum(dsh$n.qtde),0) 
												from 
													painel.v_detalheindicadorsh dsh$n 
												where 
													dsh$n.indid = $indid ".
												($where_reg ? " and ".$where_reg." = dsh$n.".$reg[1]." " : "")." 
												and 
													dsh$n.dpeid = ".($dpeid ? "$dpeid" : "$aliasSerieHistorica.dpeid")."),1))";
				}else{
					$arrIndFormula[$chave] = "(coalesce(dsh$n.qtde,0))";
					$arrIndInner[$chave] = "left join painel.v_detalheindicadorsh dsh$n ON ".$where_reg." = dsh$n.".$reg[1]." and dsh$n.dpeid = ".($dpeid ? "$dpeid" : "$aliasSerieHistorica.dpeid")." and dsh$n.indid = $indid and dsh$n.sehstatus = 'A' and dsh$n.qtde > 0";
				}
			}
			$n++;
		}
	}
	
	foreach($arrIndFormula as $chave => $f){
		$formula = str_replace("{Indicador: ".$chave."}",$f,$formula);
		$join.= $arrIndInner[$chave]." ";
	}
	if(!$regionalizadorMinimo){
		return $formula; 
	}else{
		return array("coluna" => $formula, "join" => $join);
	}
	
}

function geraGraficoHighCharts($dados)
{
	global $db;
	/* INÍCIO - Recupera os parametros via $_GET e cria variáveis com os nomes e valores */
//	dbg($dados);
	$arrPar = explode(";",$dados['dados']);
	foreach($arrPar as $dado){
		$d = explode("=",$dado);
		$arrparametros[ $d[0] ] = $d[1]; 
	}
	
	extract($arrparametros);
	/* FIM - Recupera os parametros via $_GET e cria variáveis com os nomes e valores */
        
	if($tipo){
		
		/* ********* *  INICIO QUERY PARA PEGAR OS DADOS DO INDICADOR * ********* */
		if($indid) {
			$sql = "select 
						ind.indid,
						ind.perid,
						ume.umedesc,
						ind.indcumulativo,
						ind.indcumulativovalor,
						ind.unmid,
						ind.regid,
						ind.indqtdevalor,
						ind.indshformula,
						ind.formulash
					from
						painel.indicador ind
					inner join
						painel.periodicidade per ON per.perid = ind.perid
					inner join
						painel.unidademeta ume ON ind.umeid = ume.umeid
					inner join
						painel.regionalizacao reg ON reg.regid = ind.regid
					where
						ind.indid = ".$arrparametros['indid']."
					and
						ind.indstatus = 'A'";
			$arrDadosIndicador = $db->pegaLinha($sql);
		} else {
			echo "<center>Nao Atendido.</center>";
			exit;
		}
		
		/* ********* *  INICIO QUERY PARA PEGAR OS DADOS DO INDICADOR * ********* */
		
		/* INÍCIO - REGRAS DE CONSULTA */
	
	/* Início - Adiciona exibição de quantidade e valor quando não houver nenhuma delas selecionada */
	if(!$finac_valor && !$finac_qtde){
		$finac_valor  = 1;
		$finac_qtde  = 1;
	}
	/* Fim - Adiciona exibição de quantidade e valor quando não houver nenhuma delas selecionada */
	
	/* Início - Limita a 12 períodos caso não exista data inicial e final selecionada */
	if(!$dpeid && !$dpeid2){
		$limit = " limit 12";
	}
	/* Fim - Limita a 12 períodos caso não exista data inicial e final selecionada */
	
	/* Início - Indica o nome do índice aplicado aos gráficos */
	switch($indice_moeda){
		case "ipca":
			$nome_indice = "IPCA";
		break;
		default:
			$nome_indice = "Indice";
	}
	/* Fim - Indica o nome do índice aplicado aos gráficos */
	
	/* Início - Se o indicador for monetário, a quantidade é a representação financeira */
	$arrDadosIndicador['unmid'] = (int)$arrDadosIndicador['unmid'];
	if($arrDadosIndicador['unmid'] == UNIDADEMEDICAO_MOEDA)
		$arrQtdVlr = array("quantidade");
	/* Fim - Se o indicador for monetário, a quantidade é a representação financeira */
	
	/* Início - Se o indicador for inteiro, deve-se pegar apenas valores antes da ',' */
	$arrDadosIndicador['unmid'] = (int)$arrDadosIndicador['unmid'];
	if($arrDadosIndicador['unmid'] == UNIDADEMEDICAO_NUM_INTEIRO)
		$tipoUnmid = "::bigint ";
	/* Fim - Se o indicador for inteiro, deve-se pegar apenas valores antes da ',' */
		
	
	/* Início - Campo para coleta de quantidade */
	if($finac_qtde)
		$arrCampos[] = "sum(d.qtde)$tipoUnmid as dshqtde";
	/* Fim - Campo para coleta de quantidade */
	
	/* Início - Campo para coleta de valor */
	if($finac_valor)
		$arrCampos[] = "sum(d.valor) as dshvalor";
	/* Fim - Campo para coleta de valor */
	
	
	/* Início - Quando a exibição não for em gráfico de Pizza */
	if($tipo != "pizza"){
		$wherePeriodo = "d1.dpedatainicio>=dp.dpedatainicio 
							and 
						d1.dpedatafim<=dp.dpedatafim 
							and ";
	}
	/* Fim - Quando a exibição não for em gráfico de Pizza */
	
	/* FIM - REGRAS DE CONSULTA */
		
	
	/* INÍCIO - FILTROS POR PARAMETRO */
	
	/* Início - Filtro por detalhe (entid,polid,etc) e valor do detalhe */
	if($detalhe && $detalhe != "" && $valorDetalhe && $valorDetalhe != ""){
		if($detalhe == "muncod"){
			$arrFiltros[] = "d.dshcodmunicipio = '$valorDetalhe'";
		}elseif($detalhe == "estuf"){
			$arrFiltros[] = "d.dshuf = '$valorDetalhe'";
		}elseif($detalhe == "estado"){
			$arrFiltros[] = "d.dshuf = '$valorDetalhe'";
		}elseif($detalhe == "esccodinep"){
			$arrFiltros[] = "d.dshcod = '$valorDetalhe'";
		}elseif($detalhe == "paiid"){
			
		}elseif($detalhe == "tpmid"){
			$arrFiltros[] = "d.dshcodmunicipio in (	select muncod from territorios.muntipomunicipio where tpmid = '$valorDetalhe' )";
		}elseif($detalhe == "miccod"){
			$arrFiltros[] = "d.dshcodmunicipio in (	select muncod from territorios.municipio where miccod = '$valorDetalhe' )";
		}elseif($detalhe == "regcod"){
			$arrFiltros[] = "d.dshuf in ( select estuf from territorios.estado where regcod = '$valorDetalhe' ) ";
		}elseif($detalhe == "mescod"){
			$arrFiltros[] = "d.dshcodmunicipio in (select distinct muncod from territorios.municipio where mescod  = '$valorDetalhe') ";
		}elseif($detalhe == "iesid"){
			$arrFiltros[] = "d.dshcod = '$valorDetalhe'";
		}else{
			$arrFiltros[] = "d.$detalhe = '$valorDetalhe'";
		}
	}
	/* Fim - Filtro por detalhe (entid,polid,etc) e valor do detalhe */
	
	/* Início - Filtro por tidid*/
	if($tidid && $tidid != "todos"){
		$td = explode("_",$tidid);
		$tdiid = $td[1];
		
		$sqlTdiNumero = "select tdinumero from painel.detalhetipoindicador where tdiid = $tdiid";
		$tdinumero = $db->pegaUm($sqlTdiNumero);
		
		$arrCampos[] = "d.tidid$tdinumero";
		$arrGroup[] = "d.tidid$tdinumero";
	}
	/* Fim - Filtro por nível 1 de detalhe */
	
	/* Início - Filtro por nível 1 de detalhe*/
	if($tidid1 && $tidid1 != "todos"){
		if(strstr($tidid1,",")){
			$arrTidid1 = explode(",",$tidid1);
			foreach($arrTidid1 as $td){
				if($td){
					$arrTid1[] = $td;
				}
			}
		}else{
			$arrTid1[] = $tidid1;
		}
		$sql = "select 
					tdinumero 
				from 
					painel.detalhetipoindicador dti 
				inner join
					painel.detalhetipodadosindicador dtdi ON dtdi.tdiid = dti.tdiid 
				where 
					dtdi.tidid in(".implode(",",$arrTid1).")";
		$tdinumero1 = $db->pegaUm($sql);
		$arrFiltros[] ="d.tidid$tdinumero1 in(".implode(",",$arrTid1).")";
		$arrGroup[] = "d.tidid$tdinumero1";
	}
	/* Fim - Filtro por nível 1 de detalhe */
	
	/* Início - Filtro por nível 2 de detalhe*/
	if($tidid2 && $tidid2 != "todos"){
		if(strstr($tidid2,",")){
			$arrTidid2 = explode(",",$tidid2);
			foreach($arrTidid2 as $td){
				if($td){
					$arrTid2[] = $td;
				}
			}
		}else{
			$arrTid2[] = $tidid2;
		}
		$sql = "select 
					tdinumero 
				from 
					painel.detalhetipoindicador dti 
				inner join
					painel.detalhetipodadosindicador dtdi ON dtdi.tdiid = dti.tdiid 
				where 
					dtdi.tidid in(".implode(",",$arrTid2).")";
		$tdinumero2 = $db->pegaUm($sql);
		$arrFiltros[] ="d.tidid$tdinumero2 in(".implode(",",$arrTid2).")";
		$arrGroup[] = "d.tidid$tdinumero2";
	}
	/* Fim - Filtro por nível 2 de detalhe */
	
	/* Início - Filtro por período inicial*/
	if($dpeid){
		$arrFiltros[] = "d.dpedatainicio >= ( select dpedatainicio from painel.detalheperiodicidade where dpeid = $dpeid)";
		$andQntValor = " AND d1.dpedatainicio >= ( select dpedatainicio from painel.detalheperiodicidade where dpeid = $dpeid) ";	
	}
	/* Fim - Filtro por período inicial*/
	
	/* Início - Filtro por período final*/
	if($dpeid2){
		$arrFiltros[] = "d.dpedatainicio <= ( select dpedatafim from painel.detalheperiodicidade where dpeid = $dpeid2)";
		$andQntValor.= " AND d1.dpedatainicio <= ( select dpedatafim from painel.detalheperiodicidade where dpeid = $dpeid2) ";	
	}
	/* Fim - Filtro por período final*/
	
	/* Início - Filtro por região*/
	if($regcod && $regcod != "" && $regcod != "todos"){
		if($arrDadosIndicador['indshformula'] == "t"){
			$arrFiltros[] = "d.estuf in ( select estuf from territorios.estado where regcod = '$regcod' )";
		}else{
			$arrFiltros[] = "d.dshuf in ( select estuf from territorios.estado where regcod = '$regcod' )";
		}
	}
	/* Fim - Filtro por região*/
	
	/* Início - Filtro por mesoregião*/
	if($mescod && $mescod != "" && $mescod != "todos"){
		$arrFiltros[] = "d.dshcodmunicipio in (select distinct muncod from territorios.municipio where mescod  = '$mescod') ";
	}
	/* Fim - Filtro por mesoregião*/
	
	/* Início - Filtro por grupo de municípios*/
	if($gtmid && $gtmid != "todos"){
		$arrFiltros[] = "d.dshcodmunicipio in (select muncod from territorios.muntipomunicipio where tpmid in (select tpmid from territorios.tipomunicipio where gtmid = $gtmid) )";
	}
	/* Fim - Filtro por grupo de municípios*/
	
	/* Início - Filtro por tipo de municípios*/
	if($tpmid && $tpmid != "todos"){
		$arrFiltros[] = "d.dshcodmunicipio in (select muncod from territorios.muntipomunicipio where tpmid = $tpmid )";
	}
	/* Fim - Filtro por tipo de municípios*/
	
	/* Início - Filtro por estado*/
	if($estuf && $estuf != "" && $estuf != "todos"){
		if($arrDadosIndicador['indshformula'] == "t"){
			$arrFiltros[] = "d.estuf = '$estuf'";
		}else{
			$arrFiltros[] = "d.dshuf = '$estuf'";
		}
	}
	/* Fim - Filtro por estado*/
	
	/* Início - Filtro por municipio*/
	if($muncod && $muncod != "" && $muncod != "todos"){
		$arrFiltros[] = "d.dshcodmunicipio = '$muncod'";
	}
	/* Fim - Filtro por municipio*/
	//Filtro por entid
	if($entid && $entid != ""){
		$arrFiltros[] = "d.entid = '$entid' ";
	}
	
	/* Fim - Filtro por zona*/
	//Filtro por entid
	if($zonid && $zonid != ""){
		$arrFiltros[] = "d.zonid = '$zonid' ";
	}
	/* Fim - Filtro por subprefeitura*/
	//Filtro por entid
	if($subid && $subid != ""){
		$arrFiltros[] = "d.subid = '$subid' ";
	}
	/* Fim - Filtro por distrito*/
	//Filtro por entid
	if($disid && $disid != ""){
		$arrFiltros[] = "d.disid = '$disid' ";
	}
	/* Fim - Filtro por setor*/
	//Filtro por entid
	if($setid && $setid != ""){
		$arrFiltros[] = "d.setid = '$setid' ";
	}
	
	/* Início - Filtro por regvalue*/
	//Filtro por regvalue
	if($regvalue && $regvalue != "" && $regvalue != "todos"){
		$sql = "select rgaidentificador,rgafiltro from painel.regagreg where regid = {$arrDadosIndicador['regid']}";
		$campoReg = $db->pegaLinha($sql);
		$arrFiltros[] = str_replace(array("AND","and","{".$campoReg['rgaidentificador']."}"),array("","",$regvalue),$campoReg['rgafiltro']);
	}
	/* Fim - Filtro por regvalue*/
	
	/* FIM - FILTROS POR PARAMETRO */
	
	
	/* INÍCIO - QUERY PARA CONSULTA DOS PERIODOS */
	
	/* Início - Filtro por periodicidade*/
	$periodicidade =  strtoupper($periodicidade) == "ANUAL" || strtoupper($periodicidade) == "ANO" ? PERIODO_ANUAL : $periodicidade;
	$perid = !$periodicidade ? $arrDadosIndicador['perid'] : $periodicidade;
	/* Fim - Filtro por periodicidade*/
	
	/* Início - SQL para criação dos períodos*/
	$sql = "select 
				dpe.dpeid,
				dpe.dpedsc,
				dpe.dpedatainicio,
				dpe.dpedatafim
			from
				painel.detalheperiodicidade dpe
			where
				dpe.perid = $perid
			and
				dpe.dpestatus = 'A'
			order by
				dpe.dpedatainicio asc";
	/* Início - SQL para criação dos períodos*/
	
	//Array para armazenamento dos periodos
	$arrPeriodos = $db->carregar($sql);
	
	/* FIM - QUERY PARA CONSULTA DOS PERIODOS */
		
	/* INÍCIO - QUERY PARA CONSULTA DOS VALORES E QUANTIDADES POR PERÍODO */
	//Se houver períodos para consulta
	if(is_array($arrPeriodos)){
		//percorre os dados de período
		foreach($arrPeriodos as $arrDetPer){
			
			/* Início SQL Novo */
						
			if(($arrDadosIndicador['unmid'] == UNIDADEMEDICAO_PERCENTUAL || $arrDadosIndicador['unmid'] == UNIDADEMEDICAO_RAZAO) && $arrDadosIndicador['indshformula'] == "t" && $arrDadosIndicador['formulash']){
				if($regcod && $regcod != "todos"){
					$where_reg = "d.regcod";
					$campo = "regcod,";
				}
				if($estuf && $estuf != "todos"){
					$where_reg = "d.dshuf";
					$campo = "dshuf,";
				}
				if($muncod && $muncod != "todos"){
					$where_reg = "d.muncod";
					$campo = "muncod,";
				}
				if($detalhe == "zonid"){
					$where_reg = "d.$detalhe";
					$campo = "$detalhe,";
				}
				if($detalhe == "subid"){
					$where_reg = "d.$detalhe";
					$campo = "$detalhe,";
				}
				if($detalhe == "disid"){
					$where_reg = "d.$detalhe";
					$campo = "$detalhe,";
				}
				if($detalhe == "setid"){
					$where_reg = "d.$detalhe";
					$campo = "$detalhe,";
				}
				if($zonid){
					$where_reg = "d.zonid";
					$campo = "zonid,";
				}
				if($subid){
					$where_reg = "d.subid";
					$campo = "subid,";
				}
				if($disid){
					$where_reg = "d.disid";
					$campo = "disid,";
				}
				if($setid){
					$where_reg = "d.setid";
					$campo = "setid,";
				}
				$qtde = trataFormulaPorcentagem($arrDadosIndicador['formulash'],"d",$where_reg);
			}else{
				$qtde = "sum(d.qtde)";
			}
			
			$sql = "select 
						dpeid,
						dpedsc,
						dpedatainicio,
						dpedatafim
						".(count($arrCampos) ? "," . implode(" , ",$arrCampos) : "" )."
					from (
							select $campo
								dp.dpeid,
								dp.dpedatainicio,
								dp.dpedatafim,
								d.indid,
								dp.dpedsc,
								".( in_array("d.tidid1",$arrCampos) ? "tidid1," : "" )."
								".( in_array("d.tidid2",$arrCampos) ? "tidid2," : "" )."
								case when d.indcumulativo = 'N' then
					        			case when (
							                        select 
							                        	d1.dpeid 
							                        from 
							                        	painel.detalheperiodicidade d1
													inner join 
														painel.seriehistorica sh on sh.dpeid=d1.dpeid
													where 
														$wherePeriodo
														sh.indid=d.indid
														$andQntValor
													order by 
				                                		d1.dpedatainicio desc 
				                                	limit 
				                                		1
					                				)=d.dpeid then ($qtde) 
					                	else 0 end
									else ($qtde)
								end as qtde,
								case when d.indcumulativovalor = 'N' then
					        			case when (
					                        		select 
					                        			d1.dpeid 
					                        		from 
					                        			painel.detalheperiodicidade d1
					                                inner join 
					                                	painel.seriehistorica sh on sh.dpeid=d1.dpeid
					                                where 
					                                	$wherePeriodo
					                                	sh.indid=d.indid
					                                	$andQntValor
					                                order by 
				                                		d1.dpedatainicio desc 
				                                	limit 
				                                		1
				                					)=d.dpeid then sum(d.valor)
					                			else 0 end
										else sum(d.valor)
								end as valor
							from 
								painel.v_detalheindicadorsh d
							inner join 
								painel.detalheperiodicidade dp on d.dpedatainicio>=dp.dpedatainicio and d.dpedatafim<=dp.dpedatafim
							-- periodo que vc quer exibir
							where 
								dp.perid = $perid
							-- indicador que vc quer exibir
							and 
								d.indid = $indid
							".(count($arrFiltros) ? " and ".implode(" and ",$arrFiltros) : "" )."
							--range de data compreendida no periodo
							and 
								d.dpedatainicio >= '".$arrDetPer['dpedatainicio']."'
							and
								d.dpedatafim <= '".$arrDetPer['dpedatafim']."'
							and
								sehstatus <> 'I'
							group by $campo
								d.indid,
								d.dpeid,
								dp.dpedsc,
								dp.dpeid,
								dp.dpedatainicio,
								dp.dpedatafim,
								d.indcumulativo,
								d.indcumulativovalor
								".( in_array("d.tidid1",$arrCampos) ? ",tidid1" : "" )."
								".( in_array("d.tidid2",$arrCampos) ? ",tidid2" : "" )."
								".(count($arrGroup) ? ",".implode(",",$arrGroup) : "" )."
						) d
					group by ".($campo && ($arrDadosIndicador['unmid'] == UNIDADEMEDICAO_PERCENTUAL || $arrDadosIndicador['unmid'] == UNIDADEMEDICAO_RAZAO) && $arrDadosIndicador['indshformula'] == "t" && $arrDadosIndicador['formulash'] && $arrDadosIndicador['regid'] != REGIONALIZACAO_UF ? $campo : "")."
						dpedatainicio,
						dpedatafim,
						indid,
						dpeid,
						dpedsc
						".( in_array("d.tidid1",$arrCampos) ? ",tidid1" : "" )."
						".( in_array("d.tidid2",$arrCampos) ? ",tidid2" : "" )."
					order by 
						dpedatainicio
						".( in_array("d.tidid1",$arrCampos) ? ",tidid1" : "" )."
						".( in_array("d.tidid2",$arrCampos) ? ",tidid2" : "" )."";
//			ver($sql,d);
			/* Fim SQL Novo */

			if($tidid && $tidid != "todos"){
				
				//Array para armazenamento dos valores e quantidades do indicador no período
				$arrDados = $db->carregar($sql);
				
				//dbg($sql);
				if($arrDados[0]){
					
					foreach($arrDados as $dados){
						
						//Array para armazenamento dos valores e quantidades do indicador no período com chave no periodo
						$arrValor[ $arrDetPer['dpeid'] ] [ $dados["tidid$tdinumero"] ] = array( "dpedatainicio" => $arrDetPer['dpedatainicio'] , "dpedatafim" => $arrDetPer['dpedatafim'], "periodo" => $arrDetPer['dpedsc'] , "qtde" => $dados['dshqtde'], "valor" => $dados['dshvalor'] );
						//Array para armazenamento dos períodos com chave no periodo
						$arrPeriodo[ $arrDetPer['dpeid'] ] = $arrDetPer['dpedsc'];
						//Array para armazenamento dos valores e quantidades para verificação
						$arrVerificaQtde[] = $dados['dshqtde'];
						$arrVerificaValor[] = $dados['dshvalor'];
					}
					
				}

			}else{
				
				//Array para armazenamento dos valores e quantidades do indicador no período
				$arrDados = $db->pegaLinha($sql);
				
				//Se houver quantidade ou valor adicionamos os dados no array que irá compor o gráfico
				if($arrDados['dshvalor'] || $arrDados['dshqtde']){
				
					//Array para armazenamento dos valores e quantidades do indicador no período com chave no periodo
					$arrValor[ $arrDetPer['dpeid'] ] = array( "dpedatainicio" => $arrDetPer['dpedatainicio'], "dpedatafim" => $arrDetPer['dpedatafim'] , "periodo" => $arrDetPer['dpedsc'] , "qtde" => $arrDados['dshqtde'], "valor" => $arrDados['dshvalor'] );
					//Array para armazenamento dos períodos com chave no periodo
					$arrPeriodo[ $arrDetPer['dpeid'] ] = $arrDetPer['dpedsc'];
					//Array para armazenamento dos valores e quantidades para verificação
					$arrVerificaQtde[] = $arrDados['dshqtde'];
					$arrVerificaValor[] = $arrDados['dshvalor'];
				}
			}
		}
	}
	/* FIM - QUERY PARA CONSULTA DOS VALORES E QUANTIDADES POR PERÍODO */
//dbg($sql);
	//Filtro por regvalue
	$sql = "select 
				regdescricao, 
				rgaidentificador,
				rgafiltro 
			from 
				painel.regagreg reg1
			inner join
				painel.regionalizacao reg2 ON reg1.regid = reg2.regid 
			where 
				reg1.regid = {$arrDadosIndicador['regid']} 
			and 
				regsqlcombo is not null";
	$campoReg = $db->pegaLinha($sql);
	/* Fim - Filtro por regvalue*/
	
	if($campoReg && $arrDadosIndicador['unmid'] == UNIDADEMEDICAO_RAZAO && (!$valorDetalhe && !$regvalue)){
		echo "<center>Indicador de razão! Por favor, selecione o(a) {$campoReg['regdescricao']} e tente novamente!</center>";
		die;
	}
	
	/* INÍCIO - CHAMADA DA FUNÇÃO PARA CRIAÇÃO DO GRÁFICO COM OS PARAMETROS DE TIPO DE GRÁFICO, PERÍODO E VALORES / QUANTIDADES */
	if($arrValor && ( array_sum($arrVerificaQtde) != 0 || array_sum($arrVerificaValor) != 0 ))
		criaGraficoHighCharts($tipo,$arrDadosIndicador,$arrValor,$arrparametros,$arrPeriodo);
	else
		echo "<center>Não Atendido.</center>";
	/* FIM - CHAMADA DA FUNÇÃO PARA CRIAÇÃO DO GRÁFICO COM OS PARAMETROS DE TIPO DE GRÁFICO, PERÍODO E VALORES / QUANTIDADES */
		
	}
	
}

function criaGraficoHighCharts($tipoGrafico,$arrDadosIndicador = array(),$arrValor = array(),$arrparametros = array(),$arrPeriodo = array())
{
	global $db;
	
	$div_grafico = $_POST['div'] ? $_POST['div'] : "grafico_painel_high_charts";
	?> <div id="<?php echo $div_grafico ?>" style="min-width: 400px; height:400px; margin: 0 auto"></div> <?php
	if(is_array($arrparametros)) //Se houver parametros cria-se as variaveis com seus valores através de extract
		extract($arrparametros); // executa extract para disponibilizar os parametros
	if($tidid && $tidid != "todos"){
		$td = explode("_",$tidid);
		$tdiid = $td[1];
		$sql = "select distinct 
					tidid,tiddsc
				from
					painel.detalhetipodadosindicador
				where
					tdiid = $tdiid
				and
					tidstatus = 'A'
				order by
					tidid";
		$arrDetalhes = $db->carregar($sql);
		if($arrDetalhes){
			foreach($arrDetalhes as $det){
				$arrDet[$det['tidid']] = $det['tiddsc'];
			}
		}
	}
		
	/* Início - Adiciona exibição de quantidade e valor quando não houver nenhuma delas selecionada */
	if(!$finac_valor && !$finac_qtde){
		$finac_valor  = 1;
		$finac_qtde  = 1;
	}
	
	if($arrDadosIndicador['indqtdevalor'] == "f")
		$finac_valor  = false;
	
	/* Fim - Adiciona exibição de quantidade e valor quando não houver nenhuma delas selecionada */
	
	/* Início - Aplica a escala */
	$escala = $unidade_inteiro && $unidade_inteiro != "null" ? $unidade_inteiro : 1;
	$escala = $unidade_moeda && $unidade_moeda != "null" ? $unidade_moeda : $escala;
	switch($escala){
		case 1:
			$EscalaTooltip = "";
		break;
		case 1000:
			$EscalaTooltip = " (Milhares)";
		break;
		case 1000000:
			$EscalaTooltip = " (Milhões)";
		break;
		case 1000000000:
			$EscalaTooltip = " (Bilhões)";
		break;
		default:
			$EscalaTooltip = "";
	}
	/* Início - Aplica a escala */ 
	
	/* Início - Aplicação de índices */
	$indice_moeda = $indice_moeda && $indice_moeda != "null" ? $indice_moeda : false;
	if($indice_moeda){
		switch($indice_moeda){
			case "ipca":
				$tooltipIndice = " Índice IPCA";
			break;
			default:
				$tooltipIndice = "";
		}
	}
	/* Fim - Aplicação de índices */
	
	switch($tipoGrafico)
	{
		/* Início - Gráfico Tipo = Linha */
		case "linha":
			
			$arrSerie[1]; //Cria-se uma no série
			$arrSerie[1]['cor'] = "#6495ED"; //atribui-se uma cor para a linha
			
			/* Início - Aplica a exibição de 'R$' para dados monetários */
			if($unidade_moeda && $unidade_moeda != "null"){
				$arrSerie[1]['tooltip'] = "R$ {value}"; //Tooltip para o valor acumulado
			}
			/* Fim - Aplica a exibição de 'R$' para dados monetários */
			
			/* Início - Se o indicador possibilitar cumulatividade (S ou A) cria-se mais uma linha*/
			if($arrDadosIndicador['indcumulativo'] == "S" || $arrDadosIndicador['indcumulativo'] == "A"){
				$arrSerie[2]; //Cria-se uma no série
				$arrSerie[2]['cor'] = "#191970"; //atribui-se umacor para a linha
				$arrSerie[2]['tooltip'] = " {value} (valor acumulado)"; //Tooltip para o valor acumulado
			}
			/* Fim - Se o indicador possibilitar cumulatividade (S ou A) cria-se mais uma linha*/
			
			/* Início - Se o indicador possibilitar aplicação de índices*/
			if($indice_moeda){
				$arrSerie[3]; //Cria-se uma no série
				$arrSerie[3]['cor'] = "#191970"; //atribui-se umacor para a linha
				$arrSerie[3]['tooltip'] = $EscalaTooltip." ".$tooltipIndice; //Tooltip para o valor com índice
			}
			/* Fim - Se o indicador possibilitar aplicação de índices*/
			
			/* Início - Criação do tipo de linha e linha para valor monetário*/
			if($finac_valor){
				$arrSerie[4]; //Cria-se uma no série
				$arrSerie[4]['cor'] = "#3CB371"; //atribui-se umacor para a linha
				$arrSerie[4]['tooltip'] = "R$ {value}"; //Tooltip para o valor em R$
				
				/* Início - Se o indicador possibilitar cumulatividade (S ou A) cria-se mais uma linha*/
				if($arrDadosIndicador['indcumulativovalor'] == "S" || $arrDadosIndicador['indcumulativovalor'] == "A"){
					$arrSerie[5]; //Cria-se uma no série
					$arrSerie[5]['cor'] = "#006400"; //atribui-se umacor para a linha
					$arrSerie[5]['tooltip'] = "R$ {value} (valor acumulado)"; //Tooltip para o valor em R
				}
				/* Fim - Se o indicador possibilitar cumulatividade (S ou A) cria-se mais uma linha*/
				
				/* Início - Se o indicador possibilitar aplicação de índices*/
				if($indice_moeda){
					$arrSerie[6]; //Cria-se uma no série
					$arrSerie[6]['cor'] = "#3CB371"; //atribui-se umacor para a linha
					$arrSerie[6]['tooltip'] = "R$ {value} $EscalaTooltip $tooltipIndice"; //Tooltip para o valor em R$ com índice
				}
				/* Fim - Se o indicador possibilitar aplicação de índices*/
				
			}
			/* Fim - Criação do tipo de linha e linha para valor monetário*/
			
			/* Início - cria as variáveis usadas no foreach com valor zero*/
			$valorAcumulado = 0;
			$valorMonetarioAcumulado = 0;
			$valorIndiceAcumulado = 0;
			$valorIndiceMonetarioAcumulado = 0;
			/* Fim - cria as variáveis usadas no foreach com valor zero*/
			
			/* Início - Percorre o array de dados para criar os valores para o gráfico */
			foreach($arrValor as $arrV){
				
				//array para armazenar os valores do indicador e atribuí-los a 1ª linha do gráfico
				$arrQtdeIndicador[] = round( (float)$arrV['qtde'] / $escala , 2) ;
				
				/* Início - Aplica cumulatividade Anual */
				if($arrDadosIndicador['indcumulativo'] == "A"){
					if(!$anoCorrente){
						$data = explode("-",$arrV['dpedatainicio']);
						$anoCorrente = $data[0];
					}else{
						$data = explode("-",$arrV['dpedatainicio']);
						if($anoCorrente != $data[0]){
							$valorAcumulado = 0;
							$anoCorrente = $data[0];
						}
					}
				}
				/* Fim - Aplica cumulatividade Anual */
				
				//variável para acumular as quantidades
				$valorAcumulado += round( (float)$arrV['qtde'] / $escala , 2) ;
				
				//array para armazenar os valores acumulados do indicador e atribuí-los a linha do gráfico
				$arrQtdeAcumuladoIndicador[] = round( $valorAcumulado , 2) ;
				
				/* Início - Aplica cumulatividade Anual */
				if($arrDadosIndicador['indcumulativovalor'] == "A"){
					if(!$anoCorrenteValor){
						$data = explode("-",$arrV['dpedatainicio']);
						$anoCorrenteValor = $data[0];
					}else{
						$data = explode("-",$arrV['dpedatainicio']);
						if($anoCorrenteValor != $data[0]){
							$valorMonetarioAcumulado = 0;
							$anoCorrenteValor = $data[0];
						}
					}
				}
				/* Fim - Aplica cumulatividade Anual */
				
				//variável para acumular as quantidades	
				$valorMonetarioAcumulado += round( (float)$arrV['valor'] / $escala , 2) ;
				
				//array para armazenar os valores acumulados do indicador e atribuí-los a linha do gráfico
				$arrValorAcumuladoIndicador[] = round( $valorMonetarioAcumulado , 2) ;
				
				//array para armazenar os valores monetários do indicador e atribuí-los a 2ª linha do gráfico
				$arrValorIndicador[] = round( (float)$arrV['valor']  / $escala , 2);
				
				//array para armazenar os valores monetários do indicador e atribuí-los a 2ª linha do gráfico
				$arrPeriodos[] = $arrV['periodo'];
				
				/* Início - Se o indicador possibilitar aplicação de índices*/
				if($indice_moeda){
					$sql = "select ipcindice from painel.ipca where ipcstatus = 'A' and ipcano = '".date("Y",strtotime($arrV['dpedatainicio']))."'";
					$ipca = $db->pegaUm($sql);
					$ipca = !$ipca ? 1 : $ipca;
					
					$arrQtdeIndiceIndicador[] = round( (float)$arrV['qtde'] * $ipca / $escala , 2) ;
					
					/* Início - Aplica cumulatividade Anual */
					if($arrDadosIndicador['indcumulativo'] == "A"){
						if(!$anoCorrente){
							$data = explode("-",$arrV['dpedatainicio']);
							$anoCorrente = $data[0];
						}else{
							$data = explode("-",$arrV['dpedatainicio']);
							if($anoCorrente != $data[0]){
								$valorIndiceAcumulado = 0;
								$anoCorrente = $data[0];
							}
						}
					}
					/* Fim - Aplica cumulatividade Anual */
					
					//variável para acumular as quantidades
					$valorIndiceAcumulado += round( (float)$arrV['qtde'] * $ipca  / $escala , 2) ;
					
					//array para armazenar os valores acumulados do indicador e atribuí-los a linha do gráfico
					$arrQtdeIndiceAcumuladoIndicador[] = round( $valorIndiceAcumulado , 2) ;
					
					/* Início - Aplica cumulatividade Anual */
					if($arrDadosIndicador['indcumulativovalor'] == "A"){
						if(!$anoCorrente){
							$data = explode("-",$arrV['dpedatainicio']);
							$anoCorrente = $data[0];
						}else{
							$data = explode("-",$arrV['dpedatainicio']);
							if($anoCorrente != $data[0]){
								$valorMonetarioIndiceAcumulado = 0;
								$anoCorrente = $data[0];
							}
						}
					}
					/* Fim - Aplica cumulatividade Anual */
					
					//variável para acumular as quantidades
					$valorMonetarioIndiceAcumulado += round( (float)$arrV['valor'] * $ipca  / $escala , 2) ;
					
					//array para armazenar os valores acumulados do indicador e atribuí-los a linha do gráfico
					$arrValorAcumuladoIndiceIndicador[] = round( $valorMonetarioIndiceAcumulado , 2) ;
					
					//array para armazenar os valores monetários do indicador e atribuí-los a 2ª linha do gráfico
					$arrValorIndiceIndicador[] = round( (float)$arrV['valor'] * $ipca   / $escala , 2);
					
				}
				/* Fim - Se o indicador possibilitar aplicação de índices*/
				
				
			}
			/* Fim - Percorre o array de dados para criar os valores para o gráfico */
			
			?>
			<script>
			Highcharts.setOptions({
				lang: {
					numericSymbols: [' mil',' milhões',' bilhões',' trilhões'],
					months: ['Janeiro', 'Fevereiro', 'Março', 'Abril', 'Maio', 'Junho', 'Julho', 'Agosto', 'Setembro', 'Outubro', 'Novembro', 'Dezembro'],
					weekdays: ['Domingo', 'Segunda', 'Terça', 'Quarta', 'Quinta', 'Sexta', 'Sábado'],
					downloadPDF: 'Exportar em PDF',
					downloadJPEG: 'Exportar em JPG',
					downloadPNG: 'Exportar em PNG',
					downloadSVG: 'Exportar em SVG',
					printChart: 'Imprimir',
					decimalPoint: ',',
        			thousandsSep: '.'
				},
				colors: ['#6495ED','#66CDAA','#990000','#FFD700','#CDC8B1',' #000000','#FF0000','#008B45','#8B008B','#FFE4E1','#0000FF',' #7CFC00','#8B4513','#FF1493','#00FF00','#00008B','#7FFFD4','#8B8B00','#FF6A6A','#8B1A1A','#8B0A50','#828282']
			});
			jQuery(function () {
		        jQuery('#<?php echo $div_grafico ?>').highcharts({
		            chart: {
		                type: 'line'
		            },
		            credits: {
			            enabled: false
			        },
		            title: {
		                text: '',
		                x: -20 //center
		            },
		            tooltip: {
	                    formatter: function() {
	                        <?php if($arrDadosIndicador['unmid'] == UNIDADEMEDICAO_PERCENTUAL): ?>
					   			return '<b>'+ this.series.name +' / '+this.x +'</b>:<br/> '+ Highcharts.numberFormat(this.y,2,',','.')+'%'
							<?php elseif($arrDadosIndicador['unmid'] == UNIDADEMEDICAO_RAZAO || $arrDadosIndicador['unmid'] == UNIDADEMEDICAO_NUM_INDICE): ?>
					   			return '<b>'+ this.series.name +' / '+this.x +'</b>:<br/> '+ Highcharts.numberFormat(this.y,2,',','.')
					   		<?php elseif($arrDadosIndicador['unmid'] == UNIDADEMEDICAO_MOEDA): ?>
					   			return '<b>'+ this.series.name +' / '+this.x +'</b>:<br/> R$ '+ Highcharts.numberFormat(this.y,2,',','.')
					   		<?php else: ?>
					   			return '<b>'+ this.series.name +' / '+this.x +'</b>:<br/> '+ Highcharts.numberFormat(this.y,0,',','.')
					   		<?php endif; ?>
	                    }
	                },
		            xAxis: {
		                categories: ['<?php echo implode("','",$arrPeriodos) ?>']
		            },
			        <?php if($finac_valor && $arrDadosIndicador['unmid'] != UNIDADEMEDICAO_MOEDA): ?>
			        	yAxis: [
			        		{
			                title: {
			                    text: '<?php echo $arrDadosIndicador['umedesc'] ?>'
			                },
			                plotLines: [{
			                    value: 0,
			                    width: 1,
			                    color: '#808080'
			                }],
			                minPadding: 0
			            	}
			            	,
			            	{
			                title: {
			                    text: 'R$ (em reais)'
			                },
			                plotLines: [{
			                    value: 0,
			                    width: 1,
			                    color: '#808080'
			                }],
			                minPadding: 0,
			                opposite: true
			            	}
			            ]
			        <?php else: ?>
			        	 yAxis: {
			                title: {
			                    text: '<?php echo $arrDadosIndicador['umedesc'] ?>'
			                },
			                plotLines: [{
			                    value: 0,
			                    width: 1,
			                    color: '#808080'
			                }],
			                minPadding: 0
			            }
			        <?php endif; ?>
			        ,
		            /*exporting: {
			            buttons: {
			                contextButton: {
			                    enabled: false
			                },
			                exportButton: {
			                    text: 'Exportar',
			                    // Use only the download related menu items from the default context button
			                    menuItems: Highcharts.getOptions().exporting.buttons.contextButton.menuItems.splice(2)
			                },
			                printButton: {
			                    text: 'Imprimir',
			                    onclick: function () {
			                        this.print();
			                    }
			                }
			            }
			        },*/
		            legend: {
		                align: 'center',
			            verticalAlign: 'top'
		            },
		            series: [
		            <?php if($finac_qtde): ?>
			            {
			                name: '<?php echo $arrDadosIndicador['umedesc'].$EscalaTooltip ?>',
			                data: [<?php echo implode(",",$arrQtdeIndicador) ?>],
			                /*tooltip: {
		                   		 formatter: function() {
			                        <?php if($arrDadosIndicador['unmid'] == UNIDADEMEDICAO_PERCENTUAL): ?>
							   			return Highcharts.numberFormat(this.y,2,',','.')+'%'
							   		<?php elseif($arrDadosIndicador['unmid'] == UNIDADEMEDICAO_MOEDA): ?>
							   			return Highcharts.numberFormat(this.y,2,',','.')
							   		<?php else: ?>
							   			return Highcharts.numberFormat(this.y,0,',','.')
							   		<?php endif; ?>
			                    }
		                	}*/
			            }
			            <?php if($indice_moeda && $arrDadosIndicador['unmid'] == UNIDADEMEDICAO_MOEDA): ?>
			            ,{
			                name: '<?php echo $arrDadosIndicador['umedesc'].$EscalaTooltip." ".$tooltipIndice ?>',
			                data: [<?php echo implode(",",$arrQtdeIndiceIndicador) ?>],
			                /*tooltip: {
			                    formatter: function() {
			                        return '<b>'+ this.series.name +' / '+this.x +'</b>:<br/> R$ '+ Highcharts.numberFormat(this.value,2,',','.');
			                    }
			                }*/
			            }
			            <?php endif; ?>
			            <?php if($arrDadosIndicador['indcumulativo'] == "S" || $arrDadosIndicador['indcumulativo'] == "A"): ?>
			            ,{
			                name: '<?php echo $arrDadosIndicador['umedesc']." Acumulado(s)".$EscalaTooltip ?>',
			                data: [<?php echo implode(",",$arrQtdeAcumuladoIndicador) ?>],
			                /*tooltip: {
			                    formatter: function() {
			                        <?php if($arrDadosIndicador['unmid'] == UNIDADEMEDICAO_PERCENTUAL): ?>
							   			return '<b>'+ this.series.name +' / '+this.x +'</b>:<br/> '+ Highcharts.numberFormat(this.value,2,',','.')+'%';
							   		<?php elseif($arrDadosIndicador['unmid'] == UNIDADEMEDICAO_MOEDA): ?>
							   			return '<b>'+ this.series.name +' / '+this.x +'</b>:<br/> R$ '+ Highcharts.numberFormat(this.value,2,',','.');
							   		<?php else: ?>
							   			return '<b>'+ this.series.name +' / '+this.x +'</b>:<br/> '+ Highcharts.numberFormat(this.value,0,',','.');
							   		<?php endif; ?>
			                    }
			                }*/
			            }
			            <?php endif; ?>
		            <?php endif; ?>
		            <?php if($finac_valor && $arrDadosIndicador['unmid'] != UNIDADEMEDICAO_MOEDA): ?>
		            	,{
			                name: '<?php echo "Valor Monetário (R$)".$EscalaTooltip ?>',
			                yAxis: 1,
			                data: [<?php echo implode(",",$arrValorIndicador) ?>],
			                /*tooltip: {
			                    formatter: function() {
			                        return '<b>'+ this.series.name +' / '+this.x +'</b>:<br/> R$ '+ Highcharts.numberFormat(this.y,2,',','.');
			                    }
			                }*/
			            }
			            <?php if($arrDadosIndicador['indcumulativovalor'] == "S" || $arrDadosIndicador['indcumulativovalor'] == "A"): ?>
			            ,{
			                name: '<?php echo "Valor Monetário Acumulado (R$)".$EscalaTooltip ?>',
			                yAxis: 1,
			                data: [<?php echo implode(",",$arrValorAcumuladoIndicador) ?>],
			                /*tooltip: {
			                    formatter: function() {
			                        return '<b>'+ this.series.name +' / '+this.x +'</b>:<br/> R$ '+ Highcharts.numberFormat(this.y,2,',','.');
			                    }
			                }*/
			            }
			            <?php endif; ?>
			            <?php if($indice_moeda && $arrDadosIndicador['indqtdevalor'] == "t" && $arrDadosIndicador['unmid'] != UNIDADEMEDICAO_MOEDA): ?>
			            ,{
			                name: '<?php echo "Valor Monetário (R$)".$EscalaTooltip." (".$tooltipIndice.")" ?>',
			                yAxis: 1,
			                data: [<?php echo implode(",",$arrValorIndiceIndicador) ?>],
			                /*tooltip: {
			                    formatter: function() {
			                        return '<b>'+ this.series.name +' / '+this.x +'</b>:<br/> R$ '+ Highcharts.numberFormat(this.y,2,',','.');
			                    }
			                }*/
			            }
			            <?php endif; ?>
		            <?php endif; ?>
		           ]
		        });
		    });
		    </script>
			<?php
			break;
		/* Fim - Gráfico Tipo = Linha */
			
		/* Início - Gráfico Tipo = Meta */
		case "meta":
			
			$sql = "select 
						dpeid,
						dpedsc,
						dpedatainicio,
						dpedatafim
					from
						painel.detalheperiodicidade
					where
						perid = {$arrparametros['periodicidade']}
					and
						dpedatainicio >= (
									select 
										dpedatainicio
									from
										painel.detalheperiodicidade
									where
										perid = {$arrparametros['periodicidade']}
									and
										(
											select  
												dpedatainicio
											from
												painel.detalheperiodicidade
											where
												dpeid = {$arrparametros['dpeid']}
										) between dpedatainicio and dpedatafim limit 1
								)
					and
						dpedatafim <= (
								select 
									dpedatafim
								from
									painel.detalheperiodicidade
								where
									perid = {$arrparametros['periodicidade']}
								and
									(
										select  
											dpedatafim
										from
											painel.detalheperiodicidade
										where
											dpeid = {$arrparametros['dpeid2']}
									) between dpedatainicio and dpedatafim limit 1
								)
					order by
						dpedatainicio";
			
			$arrPeriodos = $db->carregar($sql);
			
			if($arrPeriodos){
				unset($arrPeriodo);
				foreach($arrPeriodos as $periodo){
					$arrPeriodo[$periodo['dpeid']] = $periodo['dpedsc'];
				}
			
				foreach($arrPeriodos as $periodo){
					if($arrValor[$periodo['dpeid']]){
						$arrDados[$periodo['dpeid']] = $arrValor[$periodo['dpeid']];
					}else{
						$arrDados[$periodo['dpeid']] = array("dpedatainicio" => $periodo['dpedatainicio'],
															 "dpedatafim" => $periodo['dpedatafim'], 
															 "periodo" => $periodo['dpedsc'] , 
															 "qtde" => null, 
															 "valor" => null);
					}
				}
				unset($arrPeriodos);
				unset($arrValor);
				$arrValor = $arrDados;
			}
			
			$num_serie = 1;
			
			$arrSerie[$num_serie]; //Cria-se uma no série
			$arrSerie[$num_serie]['cor'] = "#6495ED"; //atribui-se uma cor para a linha
			
			/* Início - Aplica a exibição de 'R$' para dados monetários */
			if($unidade_moeda && $unidade_moeda != "null"){
				$arrSerie[$num_serie]['tooltip'] = "R$ {value}"; //Tooltip para o valor acumulado
			}
			/* Fim - Aplica a exibição de 'R$' para dados monetários */
			$num_serie+=1;
			
			$arrSerie[$num_serie]; //Cria-se uma série para meta da qtde
			$arrSerie[$num_serie]['cor'] = "#FF0000"; //atribui-se uma cor para a linha
			
			/* Início - Aplica a exibição de 'R$' para dados monetários */
			if($unidade_moeda && $unidade_moeda != "null"){
				$arrSerie[$num_serie]['tooltip'] = "R$ {value}"; //Tooltip para o valor acumulado
			}
			/* Fim - Aplica a exibição de 'R$' para dados monetários */
			$num_serie+=1;
			
			/* Início - Se o indicador possibilitar cumulatividade (S ou A) cria-se mais uma linha*/
			if($arrDadosIndicador['indcumulativo'] == "S" || $arrDadosIndicador['indcumulativo'] == "A"){
				$arrSerie[$num_serie]; //Cria-se uma no série
				$arrSerie[$num_serie]['cor'] = "#191970"; //atribui-se umacor para a linha
				$arrSerie[$num_serie]['tooltip'] = " {value} (valor acumulado)"; //Tooltip para o valor acumulado
				$num_serie+=1;
			}
			/* Fim - Se o indicador possibilitar cumulatividade (S ou A) cria-se mais uma linha*/
			
			/* Início - Se o indicador possibilitar aplicação de índices*/
			if($indice_moeda){
				$arrSerie[$num_serie]; //Cria-se uma no série
				$arrSerie[$num_serie]['cor'] = "#191970"; //atribui-se umacor para a linha
				$arrSerie[$num_serie]['tooltip'] = $EscalaTooltip." ".$tooltipIndice; //Tooltip para o valor com índice
				$num_serie+=1;
			}
			/* Fim - Se o indicador possibilitar aplicação de índices*/
			
			/* Início - Criação do tipo de linha e linha para valor monetário*/
			if($finac_valor){
				$arrSerie[$num_serie]; //Cria-se uma no série
				$arrSerie[$num_serie]['cor'] = "#3CB371"; //atribui-se umacor para a linha
				$arrSerie[$num_serie]['tooltip'] = "R$ {value}"; //Tooltip para o valor em R$
				$num_serie+=1;
				
				/* Início - Se o indicador possibilitar cumulatividade (S ou A) cria-se mais uma linha*/
				if($arrDadosIndicador['indcumulativovalor'] == "S" || $arrDadosIndicador['indcumulativovalor'] == "A"){
					$arrSerie[$num_serie]; //Cria-se uma no série
					$arrSerie[$num_serie]['cor'] = "#006400"; //atribui-se umacor para a linha
					$arrSerie[$num_serie]['tooltip'] = "R$ {value} (valor acumulado)"; //Tooltip para o valor em R
					$num_serie+=1;
				}
				/* Fim - Se o indicador possibilitar cumulatividade (S ou A) cria-se mais uma linha*/
				
				/* Início - Se o indicador possibilitar aplicação de índices*/
				if($indice_moeda){
					$arrSerie[$num_serie]; //Cria-se uma no série
					$arrSerie[$num_serie]['cor'] = "#3CB371"; //atribui-se umacor para a linha
					$arrSerie[$num_serie]['tooltip'] = "R$ {value} $EscalaTooltip $tooltipIndice"; //Tooltip para o valor em R$ com índice
					$num_serie+=1;
				}
				/* Fim - Se o indicador possibilitar aplicação de índices*/
				
			}
			/* Fim - Criação do tipo de linha e linha para valor monetário*/
			
			/* Início - cria as variáveis usadas no foreach com valor zero*/
			$valorAcumulado = 0;
			$valorMonetarioAcumulado = 0;
			$valorIndiceAcumulado = 0;
			$valorIndiceMonetarioAcumulado = 0;
			/* Fim - cria as variáveis usadas no foreach com valor zero*/
			
			/* Início - Percorre o array de dados para criar os valores para o gráfico */
			foreach($arrValor as $arrV){
				
				//array para armazenar os valores do indicador e atribuí-los a 1ª linha do gráfico
				$arrQtdeIndicador[] = round( (float)$arrV['qtde'] / $escala , 2) ;
				
				/* Início - Aplica cumulatividade Anual */
				if($arrDadosIndicador['indcumulativo'] == "A"){
					if(!$anoCorrente){
						$data = explode("-",$arrV['dpedatainicio']);
						$anoCorrente = $data[0];
					}else{
						$data = explode("-",$arrV['dpedatainicio']);
						if($anoCorrente != $data[0]){
							$valorAcumulado = 0;
							$anoCorrente = $data[0];
						}
					}
				}
				/* Fim - Aplica cumulatividade Anual */
				
				//variável para acumular as quantidades
				$valorAcumulado += round( (float)$arrV['qtde'] / $escala , 2) ;
				
				//array para armazenar os valores acumulados do indicador e atribuí-los a linha do gráfico
				$arrQtdeAcumuladoIndicador[] = round( $valorAcumulado , 2) ;
				
				/* Início - Aplica cumulatividade Anual */
				if($arrDadosIndicador['indcumulativovalor'] == "A"){
					if(!$anoCorrenteValor){
						$data = explode("-",$arrV['dpedatainicio']);
						$anoCorrenteValor = $data[0];
					}else{
						$data = explode("-",$arrV['dpedatainicio']);
						if($anoCorrenteValor != $data[0]){
							$valorMonetarioAcumulado = 0;
							$anoCorrenteValor = $data[0];
						}
					}
				}
				/* Fim - Aplica cumulatividade Anual */
				
				//variável para acumular as quantidades	
				$valorMonetarioAcumulado += round( (float)$arrV['valor'] / $escala , 2) ;
				
				//array para armazenar os valores acumulados do indicador e atribuí-los a linha do gráfico
				$arrValorAcumuladoIndicador[] = round( $valorMonetarioAcumulado , 2) ;
				
				//array para armazenar os valores monetários do indicador e atribuí-los a 2ª linha do gráfico
				$arrValorIndicador[] = round( (float)$arrV['valor']  / $escala , 2);
				
				//array para armazenar os valores monetários do indicador e atribuí-los a 2ª linha do gráfico
				$arrPeriodos[] = $arrV['periodo'];
				
				/* Início - Se o indicador possibilitar aplicação de índices*/
				if($indice_moeda){
					$sql = "select ipcindice from painel.ipca where ipcstatus = 'A' and ipcano = '".date("Y",strtotime($arrV['dpedatainicio']))."'";
					$ipca = $db->pegaUm($sql);
					$ipca = !$ipca ? 1 : $ipca;
					
					$arrQtdeIndiceIndicador[] = round( (float)$arrV['qtde'] * $ipca / $escala , 2) ;
					
					/* Início - Aplica cumulatividade Anual */
					if($arrDadosIndicador['indcumulativo'] == "A"){
						if(!$anoCorrente){
							$data = explode("-",$arrV['dpedatainicio']);
							$anoCorrente = $data[0];
						}else{
							$data = explode("-",$arrV['dpedatainicio']);
							if($anoCorrente != $data[0]){
								$valorIndiceAcumulado = 0;
								$anoCorrente = $data[0];
							}
						}
					}
					/* Fim - Aplica cumulatividade Anual */
					
					//variável para acumular as quantidades
					$valorIndiceAcumulado += round( (float)$arrV['qtde'] * $ipca  / $escala , 2) ;
					
					//array para armazenar os valores acumulados do indicador e atribuí-los a linha do gráfico
					$arrQtdeIndiceAcumuladoIndicador[] = round( $valorIndiceAcumulado , 2) ;
					
					/* Início - Aplica cumulatividade Anual */
					if($arrDadosIndicador['indcumulativovalor'] == "A"){
						if(!$anoCorrente){
							$data = explode("-",$arrV['dpedatainicio']);
							$anoCorrente = $data[0];
						}else{
							$data = explode("-",$arrV['dpedatainicio']);
							if($anoCorrente != $data[0]){
								$valorMonetarioIndiceAcumulado = 0;
								$anoCorrente = $data[0];
							}
						}
					}
					/* Fim - Aplica cumulatividade Anual */
					
					//variável para acumular as quantidades
					$valorMonetarioIndiceAcumulado += round( (float)$arrV['valor'] * $ipca  / $escala , 2) ;
					
					//array para armazenar os valores acumulados do indicador e atribuí-los a linha do gráfico
					$arrValorAcumuladoIndiceIndicador[] = round( $valorMonetarioIndiceAcumulado , 2) ;
					
					//array para armazenar os valores monetários do indicador e atribuí-los a 2ª linha do gráfico
					$arrValorIndiceIndicador[] = round( (float)$arrV['valor'] * $ipca   / $escala , 2);
					
				}
				/* Fim - Se o indicador possibilitar aplicação de índices*/
				
				
			}
			/* Fim - Percorre o array de dados para criar os valores para o gráfico */
			
			$bool_exibe = false;
			
			//Pega data inicial da projeção
			$sql = "select 
						dpedatainicio,
						dpedatafim
					from
						painel.detalheperiodicidade
					where
						dpeid = {$arrparametros['projecao']}";
			$arrDataProjecao = $db->pegaLinha($sql);
			
			foreach($arrValor as $dpeid => $valor){
				
				$dtinicio = (int)str_replace("-","",$valor['dpedatainicio']);
				$dtfim 	  = (int)str_replace("-","",$valor['dpedatafim']);
				$dtproj   = (int)str_replace("-","",$arrDataProjecao['dpedatainicio']);
				
				if( ( ($dtproj >= $dtinicio) && ( $dtproj <= $dtfim) ) || $bool_exibe == true){
					$bool_exibe = true;
					if( ($dtproj >= $dtinicio) && ( $dtproj <= $dtfim) ){
						$arrMetasQtdeIndicador[] = round((float)$valor['qtde'] / $escala ,2);
						$arrMetasvalorIndicador[] = round((float)$valor['valor'] / $escala ,2);
					}else{
						$sql = "select
									sum(dmivalor) as valor,
									sum(dmiqtde) as qtde
								from
									painel.detalhemetaindicador dmi
								inner join
									painel.metaindicador met ON met.metid = dmi.metid
								inner join
									painel.detalheperiodicidade dpe ON dmi.dpeid = dpe.dpeid
								where
									met.indid = {$arrDadosIndicador['indid']}
								and
									dmi.dmistatus = 'A'
								and
									met.metstatus = 'A'
								and
									dpedatainicio >= '{$valor['dpedatainicio']}'
								and
									dpedatafim <= '{$valor['dpedatafim']}'
								/*and
									dpe.perid = {$arrparametros['periodicidade']}*/";
						$arrMetaValor = $db->pegaLinha($sql);
						
						$arrMetasQtdeIndicador[]  = $arrMetaValor['qtde']  ? round((float)$arrMetaValor['qtde'] / $escala ,2)  : "num";
						$arrMetasvalorIndicador[] = $arrMetaValor['valor'] ? round((float)$arrMetaValor['valor'] / $escala ,2) : "num";
						
					}
				}else{
					$arrMetasQtdeIndicador[] = null;
					$arrMetasvalorIndicador[] = null;
				}
			}
			
			if($arrMetasQtdeIndicador){
				foreach($arrMetasQtdeIndicador as $chave => $qtde){
					if($qtde != null){
						if($qtde == "num"){
							if($projetar != "nao"){
								$arrChavesQ[] = $chave;
							}else{
								$arrChavesQ[] = "null";
							}
						}else{
							$arrChavesQtde[] = array("valor" => $qtde, "chave" => $chave, "array" => $arrChavesQ);
							unset($arrChavesQ);
						}
					}
				}
			}
			
			if($arrMetasvalorIndicador){
				foreach($arrMetasvalorIndicador as $chave => $valor){
					if($valor != null){
						if($valor == "num"){
							$arrChavesV[] = $chave;
						}else{
							$arrChavesValor[] = array("valor" => $qtde, "chave" => $chave, "array" => $arrChavesV);
							unset($arrChavesV);
						}
					}
				}
			}
			
			$x = 0;
			if($arrChavesQtde){
				foreach($arrChavesQtde as $key => $qtde){
					$arrQ[$x] = $qtde['valor'];
					if($qtde['array']){
						foreach($qtde['array'] as $chave){
							if($arrQ[$x - 1]){
								if($arrQ[$x] >= $arrQ[$x -1]){
									$qtdeQ = ($arrQ[$x] - $arrQ[$x -1]) / (count($qtde['array']) + 1); //aumenta
									$y = 1;
									foreach($qtde['array'] as $q){
										$arrMetasQtdeIndicador[$q] = round((float)$arrQ[$x -1] + ($qtdeQ * $y) / $escala ,2);
										$y++;
									}
								}else{
									$qtdeQ = ($arrQ[$x - 1] - $arrQ[$x]) / (count($qtde['array']) + 1); //diminui
									$y = 1;
									foreach($qtde['array'] as $q){
										$arrMetasQtdeIndicador[$q] = round((float)$arrQ[$x -1] - ($qtdeQ * $y) / $escala ,2);
										$y++;
									}
								}
							}else{
								$qtdeQ = $arrQ[$x] / (count($qtde['array']) + 1); //diminui
								$y = 1;
								foreach($qtde['array'] as $q){
									$arrMetasQtdeIndicador[$q] = round((float)$qtdeQ*$y / $escala ,2);
									$y++;
								}
							}
						}
					}
					$x++;
				}
			}
						
			if($projetar == "nao"){
				if($arrChavesQtde){
					foreach($arrChavesQtde as $qtde){
						$arrFinalMetaQtde[$qtde['chave']] = $qtde['valor'];
						$ultima_chave = $qtde['chave'];
					}
					for($i=0;$i<$ultima_chave;$i++){
						if(!$arrFinalMetaQtde[$i]){
							$arrFinalMetaQtde[$i] = null;
						}
					}
					ksort($arrFinalMetaQtde);
					$arrMetasQtdeIndicador = $arrFinalMetaQtde;
				}
			}
			
			$x = 0;
			if($arrChavesValor){
				foreach($arrChavesValor as $key => $qtde){
					$arrQ[$x] = $qtde['valor'];
					if($qtde['array']){
						foreach($qtde['array'] as $chave){
							if($arrQ[$x - 1]){
								if($arrQ[$x] >= $arrQ[$x -1]){
									$qtdeQ = ($arrQ[$x] - $arrQ[$x -1]) / (count($qtde['array']) + 1); //aumenta
									$y = 1;
									foreach($qtde['array'] as $q){
										$arrMetasvalorIndicador[$q] = round((float)$arrQ[$x -1] + ($qtdeQ * $y) / $escala ,2);
										$y++;
									}
								}else{
									$qtdeQ = ($arrQ[$x - 1] - $arrQ[$x]) / (count($qtde['array']) + 1); //diminui
									$y = 1;
									foreach($qtde['array'] as $q){
										$arrMetasvalorIndicador[$q] = round((float)$arrQ[$x -1] - ($qtdeQ * $y) / $escala ,2);
										$y++;
									}
								}
							}else{
								$qtdeQ = $arrQ[$x] / (count($qtde['array']) + 1); //diminui
								$y = 1;
								foreach($qtde['array'] as $q){
									$arrMetasvalorIndicador[$q] = round((float)$qtdeQ*$y / $escala ,2);
									$y++;
								}
							}
						}
					}
					$x++;
				}
			}
			
			if($projetar == "nao"){
				if($arrChavesValor){
					foreach($arrChavesValor as $valor){
						$arrFinalMetaValor[$valor['chave']] = $valor['valor'];
						$ultima_chave = $valor['chave'];
					}
					for($i=0;$i<$ultima_chave;$i++){
						if(!$arrFinalMetaValor[$i]){
							$arrFinalMetaValor[$i] = null;
						}
					}
					ksort($arrFinalMetaValor);
					$arrMetasvalorIndicador = $arrFinalMetaValor;
				}
			}
			
			
			if($arrQtdeIndicador){
				foreach($arrQtdeIndicador as $chave => $qtde){
					if(!$qtde){
						$arrQtdeIndicador[$chave] = "null";	
					}
				}
			}
			
			if($arrValorIndicador){
				foreach($arrValorIndicador as $chave => $qtde){
					if(!$qtde){
						$arrValorIndicador[$chave] = "null";	
					}
				}
			}
			
			if($arrMetasQtdeIndicador){
				foreach($arrMetasQtdeIndicador as $chave => $qtde){
					if(!$qtde){
						$arrMetasQtdeIndicador[$chave] = "null";	
					}
				}
			}
			
			if($arrMetasvalorIndicador){
				foreach($arrMetasvalorIndicador as $chave => $qtde){
					if(!$qtde){
						$arrMetasvalorIndicador[$chave] = "null";	
					}
				}
			}
			
			?>
			<script>
			Highcharts.setOptions({
				lang: {
					numericSymbols: [' mil',' milhões',' bilhões',' trilhões'],
					months: ['Janeiro', 'Fevereiro', 'Março', 'Abril', 'Maio', 'Junho', 'Julho', 'Agosto', 'Setembro', 'Outubro', 'Novembro', 'Dezembro'],
					weekdays: ['Domingo', 'Segunda', 'Terça', 'Quarta', 'Quinta', 'Sexta', 'Sábado'],
					downloadPDF: 'Exportar em PDF',
					downloadJPEG: 'Exportar em JPG',
					downloadPNG: 'Exportar em PNG',
					downloadSVG: 'Exportar em SVG',
					printChart: 'Imprimir',
					decimalPoint: ',',
        			thousandsSep: '.'
				},
				colors: ['#6495ED','#66CDAA','#990000','#FFD700','#CDC8B1',' #000000','#FF0000','#008B45','#8B008B','#FFE4E1','#0000FF',' #7CFC00','#8B4513','#FF1493','#00FF00','#00008B','#7FFFD4','#8B8B00','#FF6A6A','#8B1A1A','#8B0A50','#828282']
			});
			jQuery(function () {
		        jQuery('#<?php echo $div_grafico ?>').highcharts({
		            chart: {
		                type: 'line'
		            },
		            credits: {
			            enabled: false
			        },
		            title: {
		                text: '',
		                x: -20 //center
		            },
		            tooltip: {
	                    formatter: function() {
	                        <?php if($arrDadosIndicador['unmid'] == UNIDADEMEDICAO_PERCENTUAL): ?>
					   			return '<b>'+ this.series.name +' / '+this.x +'</b>:<br/> '+ Highcharts.numberFormat(this.y,2,',','.')+'%'
							<?php elseif($arrDadosIndicador['unmid'] == UNIDADEMEDICAO_RAZAO || $arrDadosIndicador['unmid'] == UNIDADEMEDICAO_NUM_INDICE): ?>
					   			return '<b>'+ this.series.name +' / '+this.x +'</b>:<br/> '+ Highcharts.numberFormat(this.y,2,',','.')
					   		<?php elseif($arrDadosIndicador['unmid'] == UNIDADEMEDICAO_MOEDA): ?>
					   			return '<b>'+ this.series.name +' / '+this.x +'</b>:<br/> R$ '+ Highcharts.numberFormat(this.y,2,',','.')
					   		<?php else: ?>
					   			return '<b>'+ this.series.name +' / '+this.x +'</b>:<br/> '+ Highcharts.numberFormat(this.y,0,',','.')
					   		<?php endif; ?>
	                    }
	                },
		            xAxis: {
		                categories: ['<?php echo implode("','",$arrPeriodos) ?>']
		            },
			        <?php if($finac_valor && $arrDadosIndicador['unmid'] != UNIDADEMEDICAO_MOEDA): ?>
			        	yAxis: [
			        		{
			                title: {
			                    text: '<?php echo $arrDadosIndicador['umedesc'] ?>'
			                },
			                plotLines: [{
			                    value: 0,
			                    width: 1,
			                    color: '#808080'
			                }],
			                minPadding: 0
			            	}
			            	,
			            	{
			                title: {
			                    text: 'R$ (em reais)'
			                },
			                plotLines: [{
			                    value: 0,
			                    width: 1,
			                    color: '#808080'
			                }],
			                minPadding: 0,
			                opposite: true
			            	}
			            ]
			        <?php else: ?>
			        	 yAxis: {
			                title: {
			                    text: '<?php echo $arrDadosIndicador['umedesc'] ?>'
			                },
			                plotLines: [{
			                    value: 0,
			                    width: 1,
			                    color: '#808080'
			                }],
			                minPadding: 0
			            }
			        <?php endif; ?>
			        ,
		            /*exporting: {
			            buttons: {
			                contextButton: {
			                    enabled: false
			                },
			                exportButton: {
			                    text: 'Exportar',
			                    // Use only the download related menu items from the default context button
			                    menuItems: Highcharts.getOptions().exporting.buttons.contextButton.menuItems.splice(2)
			                },
			                printButton: {
			                    text: 'Imprimir',
			                    onclick: function () {
			                        this.print();
			                    }
			                }
			            }
			        },*/
		            legend: {
		                align: 'center',
			            verticalAlign: 'top'
		            },
		            series: [
		            <?php if($finac_qtde): ?>
			            {
			                name: '<?php echo $arrDadosIndicador['umedesc'].$EscalaTooltip ?>',
			                data: [<?php echo implode(",",$arrQtdeIndicador) ?>],
			                /*tooltip: {
		                   		 formatter: function() {
			                        <?php if($arrDadosIndicador['unmid'] == UNIDADEMEDICAO_PERCENTUAL): ?>
							   			return Highcharts.numberFormat(this.y,2,',','.')+'%'
							   		<?php elseif($arrDadosIndicador['unmid'] == UNIDADEMEDICAO_MOEDA): ?>
							   			return Highcharts.numberFormat(this.y,2,',','.')
							   		<?php else: ?>
							   			return Highcharts.numberFormat(this.y,0,',','.')
							   		<?php endif; ?>
			                    }
		                	}*/
			            },
			            {
			            	name: '<?php echo "Meta - ".$arrDadosIndicador['umedesc'].$EscalaTooltip ?>',
			                data: [<?php echo implode(",",$arrMetasQtdeIndicador) ?>],
			            }
			            <?php if($indice_moeda && $arrDadosIndicador['unmid'] == UNIDADEMEDICAO_MOEDA): ?>
			            ,{
			                name: '<?php echo $arrDadosIndicador['umedesc'].$EscalaTooltip." ".$tooltipIndice ?>',
			                data: [<?php echo implode(",",$arrQtdeIndiceIndicador) ?>],
			                /*tooltip: {
			                    formatter: function() {
			                        return '<b>'+ this.series.name +' / '+this.x +'</b>:<br/> R$ '+ Highcharts.numberFormat(this.value,2,',','.');
			                    }
			                }*/
			            }
			            <?php endif; ?>
			            <?php if($arrDadosIndicador['indcumulativo'] == "S" || $arrDadosIndicador['indcumulativo'] == "A"): ?>
			            ,{
			                name: '<?php echo $arrDadosIndicador['umedesc']." Acumulado(s)".$EscalaTooltip ?>',
			                data: [<?php echo implode(",",$arrQtdeAcumuladoIndicador) ?>],
			                /*tooltip: {
			                    formatter: function() {
			                        <?php if($arrDadosIndicador['unmid'] == UNIDADEMEDICAO_PERCENTUAL): ?>
							   			return '<b>'+ this.series.name +' / '+this.x +'</b>:<br/> '+ Highcharts.numberFormat(this.value,2,',','.')+'%';
							   		<?php elseif($arrDadosIndicador['unmid'] == UNIDADEMEDICAO_MOEDA): ?>
							   			return '<b>'+ this.series.name +' / '+this.x +'</b>:<br/> R$ '+ Highcharts.numberFormat(this.value,2,',','.');
							   		<?php else: ?>
							   			return '<b>'+ this.series.name +' / '+this.x +'</b>:<br/> '+ Highcharts.numberFormat(this.value,0,',','.');
							   		<?php endif; ?>
			                    }
			                }*/
			            }
			            <?php endif; ?>
		            <?php endif; ?>
		            <?php if($finac_valor && $arrDadosIndicador['unmid'] != UNIDADEMEDICAO_MOEDA): ?>
		            	,{
			                name: '<?php echo "Valor Monetário (R$)".$EscalaTooltip ?>',
			                yAxis: 1,
			                data: [<?php echo implode(",",$arrValorIndicador) ?>],
			                /*tooltip: {
			                    formatter: function() {
			                        return '<b>'+ this.series.name +' / '+this.x +'</b>:<br/> R$ '+ Highcharts.numberFormat(this.y,2,',','.');
			                    }
			                }*/
			            },
			            {
			            	name: '<?php echo "Meta - "."Valor Monetário (R$)".$EscalaTooltip ?>',
			                data: [<?php echo implode(",",$arrMetasvalorIndicador) ?>],
			            }
			            <?php if($arrDadosIndicador['indcumulativovalor'] == "S" || $arrDadosIndicador['indcumulativovalor'] == "A"): ?>
			            ,{
			                name: '<?php echo "Valor Monetário Acumulado (R$)".$EscalaTooltip ?>',
			                yAxis: 1,
			                data: [<?php echo implode(",",$arrValorAcumuladoIndicador) ?>],
			                /*tooltip: {
			                    formatter: function() {
			                        return '<b>'+ this.series.name +' / '+this.x +'</b>:<br/> R$ '+ Highcharts.numberFormat(this.y,2,',','.');
			                    }
			                }*/
			            }
			            <?php endif; ?>
			            <?php if($indice_moeda && $arrDadosIndicador['indqtdevalor'] == "t" && $arrDadosIndicador['unmid'] != UNIDADEMEDICAO_MOEDA): ?>
			            ,{
			                name: '<?php echo "Valor Monetário (R$)".$EscalaTooltip." (".$tooltipIndice.")" ?>',
			                yAxis: 1,
			                data: [<?php echo implode(",",$arrValorIndiceIndicador) ?>],
			                /*tooltip: {
			                    formatter: function() {
			                        return '<b>'+ this.series.name +' / '+this.x +'</b>:<br/> R$ '+ Highcharts.numberFormat(this.y,2,',','.');
			                    }
			                }*/
			            }
			            <?php endif; ?>
		            <?php endif; ?>
		           ]
		        });
		    });
		    </script>
			<?php
			break;
		/* Fim - Gráfico Tipo = Meta */		
			
		/* Início - Gráfico Tipo = Barra*/
		case "barra":
			
			$arrSerie[1]; //Cria-se uma no série
			$arrSerie[1]['cor'] = "#6495ED"; //atribui-se uma cor para a linha
			
			/* Início - Aplica a exibição de 'R$' para dados monetários */
			if($unidade_moeda && $unidade_moeda != "null"){
				$arrSerie[1]['tooltip'] = "R$ {value}"; //Tooltip para o valor acumulado
			}
			/* Fim - Aplica a exibição de 'R$' para dados monetários */
			
			/* Início - Se o indicador possibilitar cumulatividade (S ou A) cria-se mais uma linha*/
			if($arrDadosIndicador['indcumulativo'] == "S" || $arrDadosIndicador['indcumulativo'] == "A"){
				$arrSerie[2]; //Cria-se uma no série
				$arrSerie[2]['cor'] = "#191970"; //atribui-se umacor para a linha
				$arrSerie[2]['tooltip'] = " {value} (valor acumulado)"; //Tooltip para o valor acumulado
			}
			/* Fim - Se o indicador possibilitar cumulatividade (S ou A) cria-se mais uma linha*/
			
			/* Início - Se o indicador possibilitar aplicação de índices*/
			if($indice_moeda){
				$arrSerie[3]; //Cria-se uma no série
				$arrSerie[3]['cor'] = "#191970"; //atribui-se umacor para a linha
				$arrSerie[3]['tooltip'] = $EscalaTooltip." ".$tooltipIndice; //Tooltip para o valor com índice
			}
			/* Fim - Se o indicador possibilitar aplicação de índices*/
			
			/* Início - Criação do tipo de linha e linha para valor monetário*/
			if($finac_valor){
				$arrSerie[4]; //Cria-se uma no série
				$arrSerie[4]['cor'] = "#3CB371"; //atribui-se umacor para a linha
				$arrSerie[4]['tooltip'] = "R$ {value}"; //Tooltip para o valor em R$
				
				/* Início - Se o indicador possibilitar cumulatividade (S ou A) cria-se mais uma linha*/
				if($arrDadosIndicador['indcumulativovalor'] == "S" || $arrDadosIndicador['indcumulativovalor'] == "A"){
					$arrSerie[5]; //Cria-se uma no série
					$arrSerie[5]['cor'] = "#006400"; //atribui-se umacor para a linha
					$arrSerie[5]['tooltip'] = "R$ {value} (valor acumulado)"; //Tooltip para o valor em R
				}
				/* Fim - Se o indicador possibilitar cumulatividade (S ou A) cria-se mais uma linha*/
				
				/* Início - Se o indicador possibilitar aplicação de índices*/
				if($indice_moeda){
					$arrSerie[6]; //Cria-se uma no série
					$arrSerie[6]['cor'] = "#3CB371"; //atribui-se umacor para a linha
					$arrSerie[6]['tooltip'] = "R$ {value} $EscalaTooltip $tooltipIndice"; //Tooltip para o valor em R$ com índice
				}
				/* Fim - Se o indicador possibilitar aplicação de índices*/
				
			}
			/* Fim - Criação do tipo de linha e linha para valor monetário*/
			
			/* Início - cria as variáveis usadas no foreach com valor zero*/
			$valorAcumulado = 0;
			$valorMonetarioAcumulado = 0;
			$valorIndiceAcumulado = 0;
			$valorIndiceMonetarioAcumulado = 0;
			/* Fim - cria as variáveis usadas no foreach com valor zero*/
			
			/* Início - Percorre o array de dados para criar os valores para o gráfico */
			foreach($arrValor as $arrV){
				
				//array para armazenar os valores do indicador e atribuí-los a 1ª linha do gráfico
				$arrQtdeIndicador[] = round( (float)$arrV['qtde'] / $escala , 2) ;
				
				/* Início - Aplica cumulatividade Anual */
				if($arrDadosIndicador['indcumulativo'] == "A"){
					if(!$anoCorrente){
						$data = explode("-",$arrV['dpedatainicio']);
						$anoCorrente = $data[0];
					}else{
						$data = explode("-",$arrV['dpedatainicio']);
						if($anoCorrente != $data[0]){
							$valorAcumulado = 0;
							$anoCorrente = $data[0];
						}
					}
				}
				/* Fim - Aplica cumulatividade Anual */
				
				//variável para acumular as quantidades
				$valorAcumulado += round( (float)$arrV['qtde'] / $escala , 2) ;
				
				//array para armazenar os valores acumulados do indicador e atribuí-los a linha do gráfico
				$arrQtdeAcumuladoIndicador[] = round( $valorAcumulado , 2) ;
				
				/* Início - Aplica cumulatividade Anual */
				if($arrDadosIndicador['indcumulativovalor'] == "A"){
					if(!$anoCorrenteValor){
						$data = explode("-",$arrV['dpedatainicio']);
						$anoCorrenteValor = $data[0];
					}else{
						$data = explode("-",$arrV['dpedatainicio']);
						if($anoCorrenteValor != $data[0]){
							$valorMonetarioAcumulado = 0;
							$anoCorrenteValor = $data[0];
						}
					}
				}
				/* Fim - Aplica cumulatividade Anual */
				
				//variável para acumular as quantidades	
				$valorMonetarioAcumulado += round( (float)$arrV['valor'] / $escala , 2) ;
				
				//array para armazenar os valores acumulados do indicador e atribuí-los a linha do gráfico
				$arrValorAcumuladoIndicador[] = round( $valorMonetarioAcumulado , 2) ;
				
				//array para armazenar os valores monetários do indicador e atribuí-los a 2ª linha do gráfico
				$arrValorIndicador[] = round( (float)$arrV['valor']  / $escala , 2);
				
				//array para armazenar os valores monetários do indicador e atribuí-los a 2ª linha do gráfico
				$arrPeriodos[] = $arrV['periodo'];
				
				/* Início - Se o indicador possibilitar aplicação de índices*/
				if($indice_moeda){
					$sql = "select ipcindice from painel.ipca where ipcstatus = 'A' and ipcano = '".date("Y",strtotime($arrV['dpedatainicio']))."'";
					$ipca = $db->pegaUm($sql);
					$ipca = !$ipca ? 1 : $ipca;
					
					$arrQtdeIndiceIndicador[] = round( (float)$arrV['qtde'] * $ipca / $escala , 2) ;
					
					/* Início - Aplica cumulatividade Anual */
					if($arrDadosIndicador['indcumulativo'] == "A"){
						if(!$anoCorrente){
							$data = explode("-",$arrV['dpedatainicio']);
							$anoCorrente = $data[0];
						}else{
							$data = explode("-",$arrV['dpedatainicio']);
							if($anoCorrente != $data[0]){
								$valorIndiceAcumulado = 0;
								$anoCorrente = $data[0];
							}
						}
					}
					/* Fim - Aplica cumulatividade Anual */
					
					//variável para acumular as quantidades
					$valorIndiceAcumulado += round( (float)$arrV['qtde'] * $ipca  / $escala , 2) ;
					
					//array para armazenar os valores acumulados do indicador e atribuí-los a linha do gráfico
					$arrQtdeIndiceAcumuladoIndicador[] = round( $valorIndiceAcumulado , 2) ;
					
					/* Início - Aplica cumulatividade Anual */
					if($arrDadosIndicador['indcumulativovalor'] == "A"){
						if(!$anoCorrente){
							$data = explode("-",$arrV['dpedatainicio']);
							$anoCorrente = $data[0];
						}else{
							$data = explode("-",$arrV['dpedatainicio']);
							if($anoCorrente != $data[0]){
								$valorMonetarioIndiceAcumulado = 0;
								$anoCorrente = $data[0];
							}
						}
					}
					/* Fim - Aplica cumulatividade Anual */
					
					//variável para acumular as quantidades
					$valorMonetarioIndiceAcumulado += round( (float)$arrV['valor'] * $ipca  / $escala , 2) ;
					
					//array para armazenar os valores acumulados do indicador e atribuí-los a linha do gráfico
					$arrValorAcumuladoIndiceIndicador[] = round( $valorMonetarioIndiceAcumulado , 2) ;
					
					//array para armazenar os valores monetários do indicador e atribuí-los a 2ª linha do gráfico
					$arrValorIndiceIndicador[] = round( (float)$arrV['valor'] * $ipca   / $escala , 2);
					
				}
				/* Fim - Se o indicador possibilitar aplicação de índices*/
				
				
			}
			/* Fim - Percorre o array de dados para criar os valores para o gráfico */

			?>
			<script>
			Highcharts.setOptions({
				lang: {
					numericSymbols: [' mil',' milhões',' bilhões',' trilhões'],
					months: ['Janeiro', 'Fevereiro', 'Março', 'Abril', 'Maio', 'Junho', 'Julho', 'Agosto', 'Setembro', 'Outubro', 'Novembro', 'Dezembro'],
					weekdays: ['Domingo', 'Segunda', 'Terça', 'Quarta', 'Quinta', 'Sexta', 'Sábado'],
					downloadPDF: 'Exportar em PDF',
					downloadJPEG: 'Exportar em JPG',
					downloadPNG: 'Exportar em PNG',
					downloadSVG: 'Exportar em SVG',
					printChart: 'Imprimir',
					decimalPoint: ',',
        			thousandsSep: '.'
				},
				colors: ['#6495ED','#66CDAA','#990000','#FFD700','#CDC8B1',' #000000','#FF0000','#008B45','#8B008B','#FFE4E1','#0000FF',' #7CFC00','#8B4513','#FF1493','#00FF00','#00008B','#7FFFD4','#8B8B00','#FF6A6A','#8B1A1A','#8B0A50','#828282']
			});
			jQuery(function () {
		        jQuery('#<?php echo $div_grafico ?>').highcharts({
		            chart: {
		                type: 'column',
		                marginRight: 130,
		                marginBottom: 25
		            },
		            credits: {
			            enabled: false
			        },
		            title: {
		                text: '',
		                x: -20 //center
		            },
		            tooltip: {
	                    formatter: function() {
	                        <?php if($arrDadosIndicador['unmid'] == UNIDADEMEDICAO_PERCENTUAL): ?>
					   			return '<b>'+ this.series.name +' / '+this.x +'</b>:<br/> '+ Highcharts.numberFormat(this.y,2,',','.')+'%'
							<?php elseif($arrDadosIndicador['unmid'] == UNIDADEMEDICAO_RAZAO || $arrDadosIndicador['unmid'] == UNIDADEMEDICAO_NUM_INDICE): ?>
					   			return '<b>'+ this.series.name +' / '+this.x +'</b>:<br/> '+ Highcharts.numberFormat(this.y,2,',','.')
					   		<?php elseif($arrDadosIndicador['unmid'] == UNIDADEMEDICAO_MOEDA): ?>
					   			return '<b>'+ this.series.name +' / '+this.x +'</b>:<br/> R$ '+ Highcharts.numberFormat(this.y,2,',','.')
					   		<?php else: ?>
					   			return '<b>'+ this.series.name +' / '+this.x +'</b>:<br/> '+ Highcharts.numberFormat(this.y,0,',','.')
					   		<?php endif; ?>
	                    }
	                },
		            xAxis: {
		                categories: ['<?php echo implode("','",$arrPeriodos) ?>']
		            },
			        <?php if($finac_valor && $arrDadosIndicador['unmid'] != UNIDADEMEDICAO_MOEDA): ?>
			        	yAxis: [
			        		{
			                title: {
			                    text: '<?php echo $arrDadosIndicador['umedesc'] ?>'
			                },
			                plotLines: [{
			                    value: 0,
			                    width: 1,
			                    color: '#808080'
			                }],
			                minPadding: 0
			            	}
			            	,
			            	{
			                title: {
			                    text: 'R$ (em reais)'
			                },
			                plotLines: [{
			                    value: 0,
			                    width: 1,
			                    color: '#808080'
			                }],
			                minPadding: 0,
			                opposite: true
			            	}
			            ]
			        <?php else: ?>
			        	 yAxis: {
			                title: {
			                    text: '<?php echo $arrDadosIndicador['umedesc'] ?>'
			                },
			                plotLines: [{
			                    value: 0,
			                    width: 1,
			                    color: '#808080'
			                }],
			                minPadding: 0
			            }
			        <?php endif; ?>
			        ,
		             /*exporting: {
			            buttons: {
			                contextButton: {
			                    enabled: false
			                },
			                exportButton: {
			                    text: 'Exportar',
			                    // Use only the download related menu items from the default context button
			                    menuItems: Highcharts.getOptions().exporting.buttons.contextButton.menuItems.splice(2)
			                },
			                printButton: {
			                    text: 'Imprimir',
			                    onclick: function () {
			                        this.print();
			                    }
			                }
			            }
			        },*/
		            legend: {
		                align: 'center',
			            verticalAlign: 'top'
		            },
		            series: [
		            <?php if($finac_qtde): ?>
			            {
			                name: '<?php echo $arrDadosIndicador['umedesc'].$EscalaTooltip ?>',
			                data: [<?php echo implode(",",$arrQtdeIndicador) ?>],
			                /*tooltip: {
		                   		 formatter: function() {
			                        <?php if($arrDadosIndicador['unmid'] == UNIDADEMEDICAO_PERCENTUAL): ?>
							   			return Highcharts.numberFormat(this.y,2,',','.')+'%'
							   		<?php elseif($arrDadosIndicador['unmid'] == UNIDADEMEDICAO_MOEDA): ?>
							   			return Highcharts.numberFormat(this.y,2,',','.')
							   		<?php else: ?>
							   			return Highcharts.numberFormat(this.y,0,',','.')
							   		<?php endif; ?>
			                    }
		                	}*/
			            }
			            <?php if($indice_moeda && $arrDadosIndicador['unmid'] == UNIDADEMEDICAO_MOEDA): ?>
			            ,{
			                name: '<?php echo $arrDadosIndicador['umedesc'].$EscalaTooltip." ".$tooltipIndice ?>',
			                data: [<?php echo implode(",",$arrQtdeIndiceIndicador) ?>],
			                /*tooltip: {
			                    formatter: function() {
			                        return '<b>'+ this.series.name +' / '+this.x +'</b>:<br/> R$ '+ Highcharts.numberFormat(this.value,2,',','.');
			                    }
			                }*/
			            }
			            <?php endif; ?>
			            <?php if($arrDadosIndicador['indcumulativo'] == "S" || $arrDadosIndicador['indcumulativo'] == "A"): ?>
			            ,{
			                name: '<?php echo $arrDadosIndicador['umedesc']." Acumulado(s)".$EscalaTooltip ?>',
			                data: [<?php echo implode(",",$arrQtdeAcumuladoIndicador) ?>],
			                /*tooltip: {
			                    formatter: function() {
			                        <?php if($arrDadosIndicador['unmid'] == UNIDADEMEDICAO_PERCENTUAL): ?>
							   			return '<b>'+ this.series.name +' / '+this.x +'</b>:<br/> '+ Highcharts.numberFormat(this.value,2,',','.')+'%';
							   		<?php elseif($arrDadosIndicador['unmid'] == UNIDADEMEDICAO_MOEDA): ?>
							   			return '<b>'+ this.series.name +' / '+this.x +'</b>:<br/> R$ '+ Highcharts.numberFormat(this.value,2,',','.');
							   		<?php else: ?>
							   			return '<b>'+ this.series.name +' / '+this.x +'</b>:<br/> '+ Highcharts.numberFormat(this.value,0,',','.');
							   		<?php endif; ?>
			                    }
			                }*/
			            }
			            <?php endif; ?>
		            <?php endif; ?>
		            <?php if($finac_valor && $arrDadosIndicador['unmid'] != UNIDADEMEDICAO_MOEDA): ?>
		            	,{
			                name: '<?php echo "Valor Monetário (R$)".$EscalaTooltip ?>',
			                yAxis: 1,
			                data: [<?php echo implode(",",$arrValorIndicador) ?>],
			                /*tooltip: {
			                    formatter: function() {
			                        return '<b>'+ this.series.name +' / '+this.x +'</b>:<br/> R$ '+ Highcharts.numberFormat(this.y,2,',','.');
			                    }
			                }*/
			            }
			            <?php if($arrDadosIndicador['indcumulativovalor'] == "S" || $arrDadosIndicador['indcumulativovalor'] == "A"): ?>
			            ,{
			                name: '<?php echo "Valor Monetário Acumulado (R$)".$EscalaTooltip ?>',
			                yAxis: 1,
			                data: [<?php echo implode(",",$arrValorAcumuladoIndicador) ?>],
			                /*tooltip: {
			                    formatter: function() {
			                        return '<b>'+ this.series.name +' / '+this.x +'</b>:<br/> R$ '+ Highcharts.numberFormat(this.y,2,',','.');
			                    }
			                }*/
			            }
			            <?php endif; ?>
			            <?php if($indice_moeda && $arrDadosIndicador['indqtdevalor'] == "t" && $arrDadosIndicador['unmid'] != UNIDADEMEDICAO_MOEDA): ?>
			            ,{
			                name: '<?php echo "Valor Monetário (R$)".$EscalaTooltip." (".$tooltipIndice.")" ?>',
			                yAxis: 1,
			                data: [<?php echo implode(",",$arrValorIndiceIndicador) ?>],
			                /*tooltip: {
			                    formatter: function() {
			                        return '<b>'+ this.series.name +' / '+this.x +'</b>:<br/> R$ '+ Highcharts.numberFormat(this.y,2,',','.');
			                    }
			                }*/
			            }
			            <?php endif; ?>
		            <?php endif; ?>
		           ]
		        });
		    });
		    </script>
			<?php
			break;
		/* Fim - Gráfico Tipo =  Barra */
			
		/* Início - Gráfico Tipo = barra_fatia */
		case "barra_fatia":
			
			$arrSerie[1]; //Cria-se uma no série
			$arrSerie[1]['cor'] = "#6495ED"; //atribui-se uma cor para a linha
			
			/* Início - Aplica a exibição de 'R$' para dados monetários */
			if($unidade_moeda && $unidade_moeda != "null"){
				$arrSerie[1]['tooltip'] = "R$ {value}"; //Tooltip para o valor acumulado
			}
			/* Fim - Aplica a exibição de 'R$' para dados monetários */
			
			/* Início - Se o indicador possibilitar cumulatividade (S ou A) cria-se mais uma linha*/
			if($arrDadosIndicador['indcumulativo'] == "S" || $arrDadosIndicador['indcumulativo'] == "A"){
				$arrSerie[2]; //Cria-se uma no série
				$arrSerie[2]['cor'] = "#191970"; //atribui-se umacor para a linha
				$arrSerie[2]['tooltip'] = " {value} (valor acumulado)"; //Tooltip para o valor acumulado
			}
			/* Fim - Se o indicador possibilitar cumulatividade (S ou A) cria-se mais uma linha*/
			
			/* Início - Se o indicador possibilitar aplicação de índices*/
			if($indice_moeda){
				$arrSerie[3]; //Cria-se uma no série
				$arrSerie[3]['cor'] = "#191970"; //atribui-se umacor para a linha
				$arrSerie[3]['tooltip'] = $EscalaTooltip." ".$tooltipIndice; //Tooltip para o valor com índice
			}
			/* Fim - Se o indicador possibilitar aplicação de índices*/
			
			/* Início - Criação do tipo de linha e linha para valor monetário*/
			if($finac_valor){
				$arrSerie[4]; //Cria-se uma no série
				$arrSerie[4]['cor'] = "#3CB371"; //atribui-se umacor para a linha
				$arrSerie[4]['tooltip'] = "R$ {value}"; //Tooltip para o valor em R$
				
				/* Início - Se o indicador possibilitar cumulatividade (S ou A) cria-se mais uma linha*/
				if($arrDadosIndicador['indcumulativovalor'] == "S" || $arrDadosIndicador['indcumulativovalor'] == "A"){
					$arrSerie[5]; //Cria-se uma no série
					$arrSerie[5]['cor'] = "#006400"; //atribui-se umacor para a linha
					$arrSerie[5]['tooltip'] = "R$ {value} (valor acumulado)"; //Tooltip para o valor em R
				}
				/* Fim - Se o indicador possibilitar cumulatividade (S ou A) cria-se mais uma linha*/
				
				/* Início - Se o indicador possibilitar aplicação de índices*/
				if($indice_moeda){
					$arrSerie[6]; //Cria-se uma no série
					$arrSerie[6]['cor'] = "#3CB371"; //atribui-se umacor para a linha
					$arrSerie[6]['tooltip'] = "R$ {value} $EscalaTooltip $tooltipIndice"; //Tooltip para o valor em R$ com índice
				}
				/* Fim - Se o indicador possibilitar aplicação de índices*/
				
			}
			/* Fim - Criação do tipo de linha e linha para valor monetário*/
			
			/* Início - Percorre o array de dados para criar os valores para o gráfico */
			foreach($arrValor as $dados){
				
				/* Início - cria as variáveis usadas no foreach com valor zero*/
				$valorAcumulado = 0;
				$valorMonetarioAcumulado = 0;
				$valorIndiceAcumulado = 0;
				$valorIndiceMonetarioAcumulado = 0;
				/* Fim - cria as variáveis usadas no foreach com valor zero*/
				
				foreach($dados as $tdiid => $arrV){
					
					//array para armazenar os valores do indicador e atribuí-los a 1ª linha do gráfico
					$arrQtdeIndicador[ $arrV['periodo'] ][] = array("qtde" => (round( (float)$arrV['qtde'] / $escala , 2)),"tdiid" => $tdiid) ;
					
					//variável para acumular as quantidades
					$valorAcumulado += round( (float)$arrV['qtde'] / $escala , 2) ;
					
					//array para armazenar os valores acumulados do indicador e atribuí-los a linha do gráfico
					$arrQtdeAcumuladoIndicador[ $tdiid ] [ $arrV['periodo'] ][] = round( $valorAcumulado , 2) ;
					
					//variável para acumular as quantidades
					$valorMonetarioAcumulado += round( (float)$arrV['valor'] / $escala , 2) ;
					
					//array para armazenar os valores acumulados do indicador e atribuí-los a linha do gráfico
					$arrValorAcumuladoIndicador[ $tdiid ] [ $arrV['periodo'] ][] = round( $valorMonetarioAcumulado , 2) ;
					
					//array para armazenar os valores monetários do indicador e atribuí-los a 2ª linha do gráfico
					$arrValorIndicador[ $tdiid ] [ $arrV['periodo'] ][] = round( (float)$arrV['valor']  / $escala , 2);
					
					//array para armazenar os valores monetários do indicador e atribuí-los a 2ª linha do gráfico
					$arrPeriodos = !$arrPeriodos ? array() : $arrPeriodos;
					if(!in_array($arrV['periodo'],$arrPeriodos))
						$arrPeriodos[] = $arrV['periodo'];
					
					/* Início - Se o indicador possibilitar aplicação de índices*/
					if($indice_moeda){
						$sql = "select ipcindice from painel.ipca where ipcstatus = 'A' and ipcano = '".date("Y",strtotime($arrV['dpedatainicio']))."'";
						$ipca = $db->pegaUm($sql);
						$ipca = !$ipca ? 1 : $ipca;
						
						//array para armazenar os valores do indicador e atribuí-los a 1ª linha do gráfico
						$arrQtdeIndiceIndicador[ $tdiid ] [ $arrV['periodo'] ][] = round( (float)$arrV['qtde'] * $ipca / $escala , 2) ;
						
						//variável para acumular as quantidades
						$valorIndiceAcumulado += round( (float)$arrV['qtde'] * $ipca  / $escala , 2) ;
						
						//array para armazenar os valores acumulados do indicador e atribuí-los a linha do gráfico
						$arrQtdeIndiceAcumuladoIndicador[ $tdiid ] [ $arrV['periodo'] ][] = round( $valorIndiceAcumulado , 2) ;
						
						//variável para acumular as quantidades
						$valorMonetarioIndiceAcumulado += round( (float)$arrV['valor'] * $ipca  / $escala , 2) ;
						
						//array para armazenar os valores acumulados do indicador e atribuí-los a linha do gráfico
						$arrValorAcumuladoIndiceIndicador[ $tdiid ] [ $arrV['periodo'] ][] = round( $valorMonetarioIndiceAcumulado , 2) ;
						
						//array para armazenar os valores monetários do indicador e atribuí-los a 2ª linha do gráfico
						$arrValorIndiceIndicador[ $tdiid ] [ $arrV['periodo'] ][] = round( (float)$arrV['valor'] * $ipca   / $escala , 2);
						
					}
					
				}
				/* Fim - Se o indicador possibilitar aplicação de índices*/
				
			}
			/* Fim - Percorre o array de dados para criar os valores para o gráfico */
			
			foreach($arrQtdeIndicador as $ano => $arrQtde){
				foreach($arrQtde as $qtde){
					$arrTidid[$qtde['tdiid']] = $qtde['tdiid'];
					$arrAno[$ano][$qtde['tdiid']] = $qtde['qtde'];
				}
			}
			foreach($arrQtdeIndicador as $ano => $arrQtde){
				foreach($arrTidid as $tidid)
				{
					if($arrAno[$ano][$tidid]){
						$arrSeries[$tidid][] =  $arrAno[$ano][$tidid];
					}else{
						$arrSeries[$tidid][] =  0;
					}
				}
				
			}
			?>
			<script>
			Highcharts.setOptions({
				lang: {
					numericSymbols: [' mil',' milhões',' bilhões',' trilhões'],
					months: ['Janeiro', 'Fevereiro', 'Março', 'Abril', 'Maio', 'Junho', 'Julho', 'Agosto', 'Setembro', 'Outubro', 'Novembro', 'Dezembro'],
					weekdays: ['Domingo', 'Segunda', 'Terça', 'Quarta', 'Quinta', 'Sexta', 'Sábado'],
					downloadPDF: 'Exportar em PDF',
					downloadJPEG: 'Exportar em JPG',
					downloadPNG: 'Exportar em PNG',
					downloadSVG: 'Exportar em SVG',
					printChart: 'Imprimir',
					decimalPoint: ',',
        			thousandsSep: '.'
				},
				colors: ['#6495ED','#66CDAA','#990000','#FFD700','#CDC8B1',' #000000','#FF0000','#008B45','#8B008B','#FFE4E1','#0000FF',' #7CFC00','#8B4513','#FF1493','#00FF00','#00008B','#7FFFD4','#8B8B00','#FF6A6A','#8B1A1A','#8B0A50','#828282']
			});
			jQuery(function () {
		        jQuery('#<?php echo $div_grafico ?>').highcharts({
		            chart: {
		                type: 'column',
		                marginRight: 130,
		                marginBottom: 25
		            },
		            credits: {
			            enabled: false
			        },
		            title: {
		                text: '',
		                x: -20 //center
		            },
		            tooltip: {
	                    formatter: function() {
	                        <?php if($arrDadosIndicador['unmid'] == UNIDADEMEDICAO_PERCENTUAL): ?>
					   			return '<b>'+ this.series.name +' / '+this.x +'</b>:<br/> '+ Highcharts.numberFormat(this.y,2,',','.')+'%' +' de ' + Highcharts.numberFormat(this.point.stackTotal,0,',','.')
							<?php elseif($arrDadosIndicador['unmid'] == UNIDADEMEDICAO_RAZAO || $arrDadosIndicador['unmid'] == UNIDADEMEDICAO_NUM_INDICE): ?>
					   			return '<b>'+ this.series.name +' / '+this.x +'</b>:<br/> '+ Highcharts.numberFormat(this.y,2,',','.') +' de ' + Highcharts.numberFormat(this.point.stackTotal,0,',','.')	
					   		<?php elseif($arrDadosIndicador['unmid'] == UNIDADEMEDICAO_MOEDA): ?>
					   			return '<b>'+ this.series.name +' / '+this.x +'</b>:<br/> R$ '+ Highcharts.numberFormat(this.y,2,',','.') +' de ' + Highcharts.numberFormat(this.point.stackTotal,0,',','.')
					   		<?php else: ?>
					   			return '<b>'+ this.series.name +' / '+this.x +'</b>:<br/> '+ Highcharts.numberFormat(this.y,0,',','.') +' de ' + Highcharts.numberFormat(this.point.stackTotal,0,',','.')
					   		<?php endif; ?>
	                    }
	                },
		            xAxis: {
		                categories: ['<?php echo implode("','",$arrPeriodos) ?>']
		            },
		        	 yAxis: {
		                title: {
		                    text: '<?php echo $arrDadosIndicador['umedesc'] ?>'
		                },
		                plotLines: [{
		                    value: 0,
		                    width: 1,
		                    color: '#808080'
		                }],
		                minPadding: 0,
		                stackLabels: {
		                    enabled: false
		                }
		            },
		            plotOptions: {
		                column: {
		                    stacking: 'normal',
		                    dataLabels: {
		                        enabled: false
		                    }
		                }
		            },
		             /*exporting: {
			            buttons: {
			                contextButton: {
			                    enabled: false
			                },
			                exportButton: {
			                    text: 'Exportar',
			                    // Use only the download related menu items from the default context button
			                    menuItems: Highcharts.getOptions().exporting.buttons.contextButton.menuItems.splice(2)
			                },
			                printButton: {
			                    text: 'Imprimir',
			                    onclick: function () {
			                        this.print();
			                    }
			                }
			            }
			        },*/
		            legend: {
		                align: 'center',
			            verticalAlign: 'top'
		            },
		            series: [
		            	<?php foreach($arrSeries as $n => $serie): ?>
		            		{name: '<?php echo $arrDet[$n] ?>',
			            	data: [<?php echo implode(",",$serie) ?>]},
			            <?php endforeach; ?>
		           ]
		        });
		    });
		    </script>
			<?php
			break;
		/* Fim - Gráfico Tipo = Barra_fatia */
			
		/* Início - Gráfico Tipo = barra_comp */
		case "barra_comp":
			
			$arrSerie[1]; //Cria-se uma no série
			$arrSerie[1]['cor'] = "#6495ED"; //atribui-se uma cor para a linha
			
			/* Início - Aplica a exibição de 'R$' para dados monetários */
			if($unidade_moeda && $unidade_moeda != "null"){
				$arrSerie[1]['tooltip'] = "R$ {value}"; //Tooltip para o valor acumulado
			}
			/* Fim - Aplica a exibição de 'R$' para dados monetários */
			
			/* Início - Se o indicador possibilitar cumulatividade (S ou A) cria-se mais uma linha*/
			if($arrDadosIndicador['indcumulativo'] == "S" || $arrDadosIndicador['indcumulativo'] == "A"){
				$arrSerie[2]; //Cria-se uma no série
				$arrSerie[2]['cor'] = "#191970"; //atribui-se umacor para a linha
				$arrSerie[2]['tooltip'] = " {value} (valor acumulado)"; //Tooltip para o valor acumulado
			}
			/* Fim - Se o indicador possibilitar cumulatividade (S ou A) cria-se mais uma linha*/
			
			/* Início - Se o indicador possibilitar aplicação de índices*/
			if($indice_moeda){
				$arrSerie[3]; //Cria-se uma no série
				$arrSerie[3]['cor'] = "#191970"; //atribui-se umacor para a linha
				$arrSerie[3]['tooltip'] = $EscalaTooltip." ".$tooltipIndice; //Tooltip para o valor com índice
			}
			/* Fim - Se o indicador possibilitar aplicação de índices*/
			
			/* Início - Criação do tipo de linha e linha para valor monetário*/
			if($finac_valor){
				$arrSerie[4]; //Cria-se uma no série
				$arrSerie[4]['cor'] = "#3CB371"; //atribui-se umacor para a linha
				$arrSerie[4]['tooltip'] = "R$ {value}"; //Tooltip para o valor em R$
				
				/* Início - Se o indicador possibilitar cumulatividade (S ou A) cria-se mais uma linha*/
				if($arrDadosIndicador['indcumulativovalor'] == "S" || $arrDadosIndicador['indcumulativovalor'] == "A"){
					$arrSerie[5]; //Cria-se uma no série
					$arrSerie[5]['cor'] = "#006400"; //atribui-se umacor para a linha
					$arrSerie[5]['tooltip'] = "R$ {value} (valor acumulado)"; //Tooltip para o valor em R
				}
				/* Fim - Se o indicador possibilitar cumulatividade (S ou A) cria-se mais uma linha*/
				
				/* Início - Se o indicador possibilitar aplicação de índices*/
				if($indice_moeda){
					$arrSerie[6]; //Cria-se uma no série
					$arrSerie[6]['cor'] = "#3CB371"; //atribui-se umacor para a linha
					$arrSerie[6]['tooltip'] = "R$ {value} $EscalaTooltip $tooltipIndice"; //Tooltip para o valor em R$ com índice
				}
				/* Fim - Se o indicador possibilitar aplicação de índices*/
				
			}
			/* Fim - Criação do tipo de linha e linha para valor monetário*/
			
			/* Início - Percorre o array de dados para criar os valores para o gráfico */
			foreach($arrValor as $dados){
				
				/* Início - cria as variáveis usadas no foreach com valor zero*/
				$valorAcumulado = 0;
				$valorMonetarioAcumulado = 0;
				$valorIndiceAcumulado = 0;
				$valorIndiceMonetarioAcumulado = 0;
				/* Fim - cria as variáveis usadas no foreach com valor zero*/
				
				foreach($dados as $tdiid => $arrV){
					
					//array para armazenar os valores do indicador e atribuí-los a 1ª linha do gráfico
					$arrQtdeIndicador[ $arrV['periodo'] ][] = array("qtde" => (round( (float)$arrV['qtde'] / $escala , 2)),"tdiid" => $tdiid) ;
					
					//variável para acumular as quantidades
					$valorAcumulado += round( (float)$arrV['qtde'] / $escala , 2) ;
					
					//array para armazenar os valores acumulados do indicador e atribuí-los a linha do gráfico
					$arrQtdeAcumuladoIndicador[ $tdiid ] [ $arrV['periodo'] ][] = round( $valorAcumulado , 2) ;
					
					//variável para acumular as quantidades
					$valorMonetarioAcumulado += round( (float)$arrV['valor'] / $escala , 2) ;
					
					//array para armazenar os valores acumulados do indicador e atribuí-los a linha do gráfico
					$arrValorAcumuladoIndicador[ $tdiid ] [ $arrV['periodo'] ][] = round( $valorMonetarioAcumulado , 2) ;
					
					//array para armazenar os valores monetários do indicador e atribuí-los a 2ª linha do gráfico
					$arrValorIndicador[ $tdiid ] [ $arrV['periodo'] ][] = round( (float)$arrV['valor']  / $escala , 2);
					
					//array para armazenar os valores monetários do indicador e atribuí-los a 2ª linha do gráfico
					$arrPeriodos = !$arrPeriodos ? array() : $arrPeriodos;
					if(!in_array($arrV['periodo'],$arrPeriodos))
						$arrPeriodos[] = $arrV['periodo'];
					
					/* Início - Se o indicador possibilitar aplicação de índices*/
					if($indice_moeda){
						$sql = "select ipcindice from painel.ipca where ipcstatus = 'A' and ipcano = '".date("Y",strtotime($arrV['dpedatainicio']))."'";
						$ipca = $db->pegaUm($sql);
						$ipca = !$ipca ? 1 : $ipca;
						
						//array para armazenar os valores do indicador e atribuí-los a 1ª linha do gráfico
						$arrQtdeIndiceIndicador[ $tdiid ] [ $arrV['periodo'] ][] = round( (float)$arrV['qtde'] * $ipca / $escala , 2) ;
						
						//variável para acumular as quantidades
						$valorIndiceAcumulado += round( (float)$arrV['qtde'] * $ipca  / $escala , 2) ;
						
						//array para armazenar os valores acumulados do indicador e atribuí-los a linha do gráfico
						$arrQtdeIndiceAcumuladoIndicador[ $tdiid ] [ $arrV['periodo'] ][] = round( $valorIndiceAcumulado , 2) ;
						
						//variável para acumular as quantidades
						$valorMonetarioIndiceAcumulado += round( (float)$arrV['valor'] * $ipca  / $escala , 2) ;
						
						//array para armazenar os valores acumulados do indicador e atribuí-los a linha do gráfico
						$arrValorAcumuladoIndiceIndicador[ $tdiid ] [ $arrV['periodo'] ][] = round( $valorMonetarioIndiceAcumulado , 2) ;
						
						//array para armazenar os valores monetários do indicador e atribuí-los a 2ª linha do gráfico
						$arrValorIndiceIndicador[ $tdiid ] [ $arrV['periodo'] ][] = round( (float)$arrV['valor'] * $ipca   / $escala , 2);
						
					}
					
				}
				/* Fim - Se o indicador possibilitar aplicação de índices*/
				
			}
			/* Fim - Percorre o array de dados para criar os valores para o gráfico */
			
			foreach($arrQtdeIndicador as $ano => $arrQtde){
				foreach($arrQtde as $qtde){
					$arrTidid[$qtde['tdiid']] = $qtde['tdiid'];
					$arrAno[$ano][$qtde['tdiid']] = $qtde['qtde'];
				}
			}
			foreach($arrQtdeIndicador as $ano => $arrQtde){
				foreach($arrTidid as $tidid)
				{
					if($arrAno[$ano][$tidid]){
						$arrSeries[$tidid][] =  $arrAno[$ano][$tidid];
					}else{
						$arrSeries[$tidid][] =  0;
					}
				}
				
			}
			
			?>
			<script>
			Highcharts.setOptions({
				lang: {
					numericSymbols: [' mil',' milhões',' bilhões',' trilhões'],
					months: ['Janeiro', 'Fevereiro', 'Março', 'Abril', 'Maio', 'Junho', 'Julho', 'Agosto', 'Setembro', 'Outubro', 'Novembro', 'Dezembro'],
					weekdays: ['Domingo', 'Segunda', 'Terça', 'Quarta', 'Quinta', 'Sexta', 'Sábado'],
					downloadPDF: 'Exportar em PDF',
					downloadJPEG: 'Exportar em JPG',
					downloadPNG: 'Exportar em PNG',
					downloadSVG: 'Exportar em SVG',
					printChart: 'Imprimir',
					decimalPoint: ',',
        			thousandsSep: '.'
				},
				colors: ['#6495ED','#66CDAA','#990000','#FFD700','#CDC8B1',' #000000','#FF0000','#008B45','#8B008B','#FFE4E1','#0000FF',' #7CFC00','#8B4513','#FF1493','#00FF00','#00008B','#7FFFD4','#8B8B00','#FF6A6A','#8B1A1A','#8B0A50','#828282']
			});
			jQuery(function () {
		        jQuery('#<?php echo $div_grafico ?>').highcharts({
		            chart: {
		                type: 'column',
		                marginRight: 130,
		                marginBottom: 25
		            },
		            credits: {
			            enabled: false
			        },
		            title: {
		                text: '',
		                x: -20 //center
		            },
		            tooltip: {
	                    formatter: function() {
	                        <?php if($arrDadosIndicador['unmid'] == UNIDADEMEDICAO_PERCENTUAL): ?>
					   			return '<b>'+ this.series.name +' / '+this.x +'</b>:<br/> '+ Highcharts.numberFormat(this.y,2,',','.')+'%'
							<?php elseif($arrDadosIndicador['unmid'] == UNIDADEMEDICAO_RAZAO || $arrDadosIndicador['unmid'] == UNIDADEMEDICAO_NUM_INDICE): ?>
					   			return '<b>'+ this.series.name +' / '+this.x +'</b>:<br/> '+ Highcharts.numberFormat(this.y,2,',','.')
					   		<?php elseif($arrDadosIndicador['unmid'] == UNIDADEMEDICAO_MOEDA): ?>
					   			return '<b>'+ this.series.name +' / '+this.x +'</b>:<br/> R$ '+ Highcharts.numberFormat(this.y,2,',','.')
					   		<?php else: ?>
					   			return '<b>'+ this.series.name +' / '+this.x +'</b>:<br/> '+ Highcharts.numberFormat(this.y,0,',','.')
					   		<?php endif; ?>
	                    }
	                },
		            xAxis: {
		                categories: ['<?php echo implode("','",$arrPeriodos) ?>']
		            },
		        	 yAxis: {
		                title: {
		                    text: '<?php echo $arrDadosIndicador['umedesc'] ?>'
		                },
		                plotLines: [{
		                    value: 0,
		                    width: 1,
		                    color: '#808080'
		                }],
		                minPadding: 0,
		                stackLabels: {
		                    enabled: false
		                }
		            },
		             /*exporting: {
			            buttons: {
			                contextButton: {
			                    enabled: false
			                },
			                exportButton: {
			                    text: 'Exportar',
			                    // Use only the download related menu items from the default context button
			                    menuItems: Highcharts.getOptions().exporting.buttons.contextButton.menuItems.splice(2)
			                },
			                printButton: {
			                    text: 'Imprimir',
			                    onclick: function () {
			                        this.print();
			                    }
			                }
			            }
			        },*/
		            legend: {
		                align: 'center',
			            verticalAlign: 'top'
		            },
		            series: [
		            	<?php foreach($arrSeries as $n => $serie): ?>
		            		{name: '<?php echo $arrDet[$n] ?>',
			            	data: [<?php echo implode(",",$serie) ?>]},
			            <?php endforeach; ?>
		           ]
		        });
		    });
		    </script>
			<?php
			break;
		/* Fim - Gráfico Tipo = Barra_Comparativa */
			
		/* Início - Gráfico Tipo = Pizza */
		case "pizza":
		
			/* Início - Percorre o array de dados para criar os valores para o gráfico */
			foreach($arrValor as $dados){
				
				/* Início - cria as variáveis usadas no foreach com valor zero*/
				$valorAcumulado = 0;
				$valorMonetarioAcumulado = 0;
				$valorIndiceAcumulado = 0;
				$valorIndiceMonetarioAcumulado = 0;
				/* Fim - cria as variáveis usadas no foreach com valor zero*/
				
				foreach($dados as $tdiid => $arrV){
					
					//array para armazenar os valores do indicador e atribuí-los a 1ª linha do gráfico
					$arrQtdeIndicador[ $tdiid ] [ $arrV['periodo'] ] = array("qtde" => (round( (float)$arrV['qtde'] / $escala , 2)),"tdiid" => $tdiid) ;
					
					//variável para acumular as quantidades
					$valorAcumulado += round( (float)$arrV['qtde'] / $escala , 2) ;
					
					//array para armazenar os valores acumulados do indicador e atribuí-los a linha do gráfico
					$arrQtdeAcumuladoIndicador[ $tdiid ] [ $arrV['periodo'] ][] = round( $valorAcumulado , 2) ;
					
					//variável para acumular as quantidades
					$valorMonetarioAcumulado += round( (float)$arrV['valor'] / $escala , 2) ;
					
					//array para armazenar os valores acumulados do indicador e atribuí-los a linha do gráfico
					$arrValorAcumuladoIndicador[ $tdiid ] [ $arrV['periodo'] ][] = round( $valorMonetarioAcumulado , 2) ;
					
					//array para armazenar os valores monetários do indicador e atribuí-los a 2ª linha do gráfico
					$arrValorIndicador[ $tdiid ] [ $arrV['periodo'] ][] = round( (float)$arrV['valor']  / $escala , 2);
					
					//array para armazenar os valores monetários do indicador e atribuí-los a 2ª linha do gráfico
					$arrPeriodos = !$arrPeriodos ? array() : $arrPeriodos;
					if(!in_array($arrV['periodo'],$arrPeriodos))
						$arrPeriodos[] = $arrV['periodo'];
					
					/* Início - Se o indicador possibilitar aplicação de índices*/
					if($indice_moeda){
						$sql = "select ipcindice from painel.ipca where ipcstatus = 'A' and ipcano = '".date("Y",strtotime($arrV['dpedatainicio']))."'";
						$ipca = $db->pegaUm($sql);
						$ipca = !$ipca ? 1 : $ipca;
						
						//array para armazenar os valores do indicador e atribuí-los a 1ª linha do gráfico
						$arrQtdeIndiceIndicador[ $tdiid ] [ $arrV['periodo'] ][] = round( (float)$arrV['qtde'] * $ipca / $escala , 2) ;
						
						//variável para acumular as quantidades
						$valorIndiceAcumulado += round( (float)$arrV['qtde'] * $ipca  / $escala , 2) ;
						
						//array para armazenar os valores acumulados do indicador e atribuí-los a linha do gráfico
						$arrQtdeIndiceAcumuladoIndicador[ $tdiid ] [ $arrV['periodo'] ][] = round( $valorIndiceAcumulado , 2) ;
						
						//variável para acumular as quantidades
						$valorMonetarioIndiceAcumulado += round( (float)$arrV['valor'] * $ipca  / $escala , 2) ;
						
						//array para armazenar os valores acumulados do indicador e atribuí-los a linha do gráfico
						$arrValorAcumuladoIndiceIndicador[ $tdiid ] [ $arrV['periodo'] ][] = round( $valorMonetarioIndiceAcumulado , 2) ;
						
						//array para armazenar os valores monetários do indicador e atribuí-los a 2ª linha do gráfico
						$arrValorIndiceIndicador[ $tdiid ] [ $arrV['periodo'] ][] = round( (float)$arrV['valor'] * $ipca   / $escala , 2);
						
					}
					
				}
				/* Fim - Se o indicador possibilitar aplicação de índices*/
				
			}
			/* Fim - Percorre o array de dados para criar os valores para o gráfico */
			
			foreach($arrQtdeIndicador as $chave => $detalhe){
				
				foreach($arrPeriodo as $periodo){
					$arrQtd[$chave][] = !$arrQtdeIndicador[$chave][$periodo]['qtde'] ? 0 : $arrQtdeIndicador[$chave][$periodo]['qtde'];
					$arrValores[] = !$arrQtdeIndicador[$chave][$periodo]['qtde'] ? 0 : $arrQtdeIndicador[$chave][$periodo]['qtde'];
				}
				
			}
			
			foreach($arrQtdeIndicador as $chave => $detalhe){
				$arrSeries[] = array("nome"=>$arrDet[$chave],"qtde"=>round(array_sum($arrQtd[$chave])),"porcentagem"=>round((array_sum($arrQtd[$chave]) * 100) / array_sum($arrValores),2));
				//$arrCores[] = getCorDetalhe($arrDetalhes,$chave);
				//$arrPizza[] = new pie_value( array_sum($arrQtd[$chave]) , round((array_sum($arrQtd[$chave]) * 100) / array_sum($arrValores),2)."%" );
			}
			?>
			<script>
			Highcharts.setOptions({
				lang: {
					numericSymbols: [' mil',' milhões',' bilhões',' trilhões'],
					months: ['Janeiro', 'Fevereiro', 'Março', 'Abril', 'Maio', 'Junho', 'Julho', 'Agosto', 'Setembro', 'Outubro', 'Novembro', 'Dezembro'],
					weekdays: ['Domingo', 'Segunda', 'Terça', 'Quarta', 'Quinta', 'Sexta', 'Sábado'],
					downloadPDF: 'Exportar em PDF',
					downloadJPEG: 'Exportar em JPG',
					downloadPNG: 'Exportar em PNG',
					downloadSVG: 'Exportar em SVG',
					printChart: 'Imprimir',
					decimalPoint: ',',
        			thousandsSep: '.'
				},
				colors: ['#6495ED','#66CDAA','#990000','#FFD700','#CDC8B1',' #000000','#FF0000','#008B45','#8B008B','#FFE4E1','#0000FF',' #7CFC00','#8B4513','#FF1493','#00FF00','#00008B','#7FFFD4','#8B8B00','#FF6A6A','#8B1A1A','#8B0A50','#828282']
			});
			jQuery(function () {
		        jQuery('#<?php echo $div_grafico ?>').highcharts({
		            chart: {
		                plotBackgroundColor: null,
               			plotBorderWidth: null,
                		plotShadow: false
		            },
		            credits: {
			            enabled: false
			        },
		            title: {
		                text: '',
		                x: -20 //center
		            },
		            tooltip: {
		        	    formatter: function() {
                            return '' + this.point.name +' / '+ Highcharts.numberFormat(this.percentage,2,',','.')+'%';
                        }
		            },
		            plotOptions: {
		                pie: {
		                    allowPointSelect: true,
		                    cursor: 'pointer',
		                    dataLabels: {
		                        enabled: true,
		                        color: '#000000',
		                        connectorColor: '#000000',
		                        formatter: function() {
		                            return ''+ this.point.name +' / '+ Highcharts.numberFormat(this.percentage,2,',','.')+'%';
		                        }
		                    },
		                    showInLegend: true
		                }
		            },
		             /*exporting: {
			            buttons: {
			                contextButton: {
			                    enabled: false
			                },
			                exportButton: {
			                    text: 'Exportar',
			                    // Use only the download related menu items from the default context button
			                    menuItems: Highcharts.getOptions().exporting.buttons.contextButton.menuItems.splice(2)
			                },
			                printButton: {
			                    text: 'Imprimir',
			                    onclick: function () {
			                        this.print();
			                    }
			                }
			            }
			        },*/
		            legend: {
		                align: 'center',
			            verticalAlign: 'top'
		            },
		           series: [{
		                type: 'pie',
		                name: '',
		                data: [
		                	<?php $n=1;foreach($arrSeries as $serie): ?>
		                		{
			                        name: '<b><?php echo $serie['nome']?></b> : <?php echo number_format($serie['qtde'],0,',','.') ?>',
			                        y: <?php echo round($serie['porcentagem'],2) ?>
			                    }
		                		<?php echo $n != count($arrSeries) ? "," : "" ?>
		                	<?php $n++;endforeach; ?>
		                ]
		            }]
		        });
		    });
		    </script> <?php
		break;
		/* Fim - Gráfico Tipo = Pizza */
	}
}
?>