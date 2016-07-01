<?

function carregarProfessorAlfabetizador($dados) {
	global $db;
	$arr = $db->pegaLinha("SELECT u.uncid, 
								  re.reiid, 
								  su.uniuf, 
								  u.curid, 
								  u.docid, 
								  su.unisigla||' - '||su.uninome as descricao
						   FROM sispacto3.universidadecadastro u 
					 	   INNER JOIN sispacto3.universidade su ON su.uniid = u.uniid
						   INNER JOIN sispacto3.reitor re on re.uniid = su.uniid 
						   WHERE u.uncid='".$dados['uncid']."'");
	
	$infprof = $db->pegaLinha("SELECT i.iusd, i.iusnome, i.iuscpf 
							   FROM sispacto3.identificacaousuario i 
							   INNER JOIN sispacto3.tipoperfil t ON t.iusd=i.iusd 
							   WHERE i.iusd='".$dados['iusd']."' AND t.pflcod='".PFL_PROFESSORALFABETIZADOR."'");
	
	$_SESSION['sispacto3']['professoralfabetizador'] = array("descricao" => $arr['descricao']." ( ".$infprof['iusnome']." )",
															"curid" 	=> $arr['curid'], 
															"uncid" 	=> $arr['uncid'], 
															"reiid" 	=> $arr['reiid'], 
															"estuf" 	=> $arr['uniuf'], 
															"docid" 	=> $arr['docid'], 
															"iusd" 	   	=> $infprof['iusd'],
															"iuscpf"    => $infprof['iuscpf']);	
	
	if($dados['direcionar']) {
		$al = array("location"=>"sispacto3.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=principal");
		alertlocation($al);
	}
	
}

function gerenciarMateriaisProfessores($dados) {
	global $db;
	
	if(!$_SESSION['sispacto3']['professoralfabetizador']['iusd']) {
		$al = array("alert"=>"N�o foi poss�vel gravar as informa��es sobre materiais. Tente novamente mais tarde.","location"=>"sispacto3.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=materiais");
		alertlocation($al);
	}
	
	$sql = "UPDATE sispacto3.materiaisprofessores
	   			SET recebeumaterialpacto='".substr($dados['recebeumaterialpacto'],0,1)."',
	       			criadocantinholeitura='".substr($dados['criadocantinholeitura'],0,1)."'
	 			WHERE iusd='".$_SESSION['sispacto3']['professoralfabetizador']['iusd']."'";
	
	$db->executar($sql);
	
	$sql = "INSERT INTO sispacto3.materiaisprofessores(
            	iusd, recebeumaterialpacto,  
            	criadocantinholeitura, mapstatus)
			 	SELECT '".$_SESSION['sispacto3']['professoralfabetizador']['iusd']."', 
			    		'".$dados['recebeumaterialpacto']."', 
			            '".$dados['criadocantinholeitura']."', 'A'
			    WHERE (SELECT mapid FROM sispacto3.materiaisprofessores WHERE iusd='".$_SESSION['sispacto3']['professoralfabetizador']['iusd']."') IS NULL";
	
	$db->executar($sql);
	
	$db->commit();
			
	$sql = "SELECT mapid as t FROM sispacto3.materiaisprofessores WHERE iusd='".$_SESSION['sispacto3']['professoralfabetizador']['iusd']."'";
	$mapid = $db->pegaUm($sql);
	
	if($_FILES['arquivo']['error']=='0') {
		
		$campos	= array("mapid"	 => $mapid,
						"mpfdsc" => "'".$dados['mpfdsc']."'");	
				
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
				
		$file = new FilesSimec("materiaisprofessoresfotos", $campos ,"sispacto3");
				
		$arquivoSalvo = $file->setUpload($dados['mafdsc']);
	}
	
	
	$al = array("alert"=>"Informa��es sobre materiais salvas com sucesso","location"=>"sispacto3.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=atividadesobrigatorias&fpbid=".$dados['fpbid']);
	alertlocation($al);
	
}

function desvincularTurmaProfessor($dados) {
	global $db;
	
	$sql = "UPDATE sispacto3.turmasprofessoresalfabetizadores SET tpastatus='I', tpajustificativadesvinculacao='".$dados['tpajustificativadesvinculacao']."' WHERE tpaid='".$dados['tpaid']."'";
	$db->executar($sql);
	$db->commit();
	
	$al = array("alert"=>"Desvincula��o feita com sucesso","location"=>"sispacto3.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=atividadesobrigatorias&fpbid=".$dados['fpbid']);
	alertlocation($al);
	
}


function pegarTurmasProfessores($dados) {
	global $db;
	
	if($dados['iusd']) $wh[] = "t.iusd='".$dados['iusd']."'";
	if($dados['tpaid']) $wh[] = "t.tpaid='".$dados['tpaid']."'";
	if($dados['tpastatus']) $wh[] = "t.tpastatus='".$dados['tpastatus']."'";
	if($dados['tpaconfirmaregencianulo']) $wh[] = "t.tpaconfirmaregencia IS NULL";
	if($dados['tpaconfirmaregencia']) $wh[] = "t.tpaconfirmaregencia=".$dados['tpaconfirmaregencia'];
	
	$turmasprofessores = $db->carregar("SELECT * 
										FROM sispacto3.turmasprofessoresalfabetizadores t 
										INNER JOIN territorios.municipio m ON m.muncod = t.tpamuncodescola 
										".(($wh)?"WHERE ".implode(" AND ",$wh):"")." ORDER BY t.tpaid");
	
	return $turmasprofessores;
}

function carregarEscolasPorMunicipio($dados) {
	global $db;
	
	$sql = "SELECT pk_cod_entidade as codigo, pk_cod_entidade || ' - ' || no_entidade as descricao FROM educacenso_2014.tab_entidade WHERE fk_cod_municipio='".$dados['muncod']."' ORDER BY no_entidade";
	$combo = $db->monta_combo('tpacodigoescola', $sql, 'S', 'Selecione', 'exibirDadosTurma', '', '', '200', 'S', 'tpacodigoescola', '', '');
	
}

function confirmarRegenciaTurma($dados) {
	global $db;
	if($dados['tpaconfirmaregencia']) {
		foreach($dados['tpaconfirmaregencia'] as $tpaid => $vl) {
			$db->executar("UPDATE sispacto3.turmasprofessoresalfabetizadores SET tpaconfirmaregencia=".$vl." WHERE tpaid='".$tpaid."'");
			$db->commit();			
		}
	}
	
	$al = array("alert"=>"Confirma��o de reg�ncia feita com sucesso","location"=>"sispacto3.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=atividadesobrigatorias&fpbid=".$dados['fpbid']);
	alertlocation($al);
	
	
}

function inserirTurmaProfessor($dados) {
	global $db;
	
	if(!$dados['tpahorarioinicioturma_hr']) $inf_faltando[] = "Hora (Inic�o) em branco";
	if(!$dados['tpahorarioinicioturma_mi']) $inf_faltando[] = "Minuto (Inic�o) em branco";
	if(!$dados['tpahorariofimturma_hr']) $inf_faltando[] = "Hora (Fim) em branco";
	if(!$dados['tpahorariofimturma_mi']) $inf_faltando[] = "Minuto (Fim) em branco";
	if(!$dados['pk_cod_etapa_ensino']) $inf_faltando[] = "Etapa em branco";
	if(!$dados['tpacodigoescola']) $inf_faltando[] = "Escola em branco";
	
	if($inf_faltando) {
		$al = array("alert"=>"Est�o faltando informa��es para o cadastramento da turma : ".'\n'.implode('\n',$inf_faltando).'\n'."Caso o erro persista (mesmo selecionando as informa��es necess�rias), solicitamos que utilize outra m�quina. Sugerimos Sistema Operacional: Window, Linux. Browser: Internet Explorer, Firefox, Google Chrome","location"=>"sispacto3.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=dadosturmas");
		alertlocation($al);
	}
	
	$sql = "INSERT INTO sispacto3.turmasprofessoresalfabetizadores(
            tpacodigoescola, tpanomeescola, tpamuncodescola, tpaemailescola, 
            tpanometurma, tpahorarioinicioturma, tpahorariofimturma, 
            iusd, tpastatus, tpaoriginalcenso, tpaetapaturma, tpaconfirmaregencia)
            SELECT pk_cod_entidade, no_entidade, fk_cod_municipio, no_email,
            	   '".$dados['tpanometurma']."', '".$dados['tpahorarioinicioturma_hr'].":".$dados['tpahorarioinicioturma_mi']."', '".$dados['tpahorariofimturma_hr'].":".$dados['tpahorariofimturma_mi']."',
            	   '".$_SESSION['sispacto3']['professoralfabetizador']['iusd']."', 'A', false, (SELECT no_etapa_ensino FROM educacenso_2014.tab_etapa_ensino WHERE pk_cod_etapa_ensino='".$dados['pk_cod_etapa_ensino']."'), 
            	   true
            FROM educacenso_2014.tab_entidade WHERE pk_cod_entidade='".$dados['tpacodigoescola']."'";
	
	$db->executar($sql);
	$db->commit();
	
	$al = array("alert"=>"Turma do professor inserida com sucesso","location"=>"sispacto3.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=atividadesobrigatorias&fpbid=".$dados['fpbid']);
	alertlocation($al);
	
}

function gerenciarInformacoesTurmasProfessor($dados) {
	global $db;
	
	if($dados['tpaid']) {
		foreach($dados['tpaid'] as $tpaid) {
			
			$sql = "UPDATE sispacto3.turmasprofessoresalfabetizadores SET 
					  tpatotalmeninos=".((is_numeric($dados['tpatotalmeninos'][$tpaid]))?"'".$dados['tpatotalmeninos'][$tpaid]."'":"NULL").",
					  tpatotalmeninas=".((is_numeric($dados['tpatotalmeninas'][$tpaid]))?"'".$dados['tpatotalmeninas'][$tpaid]."'":"NULL").",
					  tpafaixaetariaabaixo6anos=".((is_numeric($dados['tpafaixaetariaabaixo6anos'][$tpaid]))?"'".$dados['tpafaixaetariaabaixo6anos'][$tpaid]."'":"NULL").",
					  tpafaixaetaria6anos=".((is_numeric($dados['tpafaixaetaria6anos'][$tpaid]))?"'".$dados['tpafaixaetaria6anos'][$tpaid]."'":"NULL").",
					  tpafaixaetaria7anos=".((is_numeric($dados['tpafaixaetaria7anos'][$tpaid]))?"'".$dados['tpafaixaetaria7anos'][$tpaid]."'":"NULL").",
					  tpafaixaetaria8anos=".((is_numeric($dados['tpafaixaetaria8anos'][$tpaid]))?"'".$dados['tpafaixaetaria8anos'][$tpaid]."'":"NULL").",
					  tpafaixaetaria9anos=".((is_numeric($dados['tpafaixaetaria9anos'][$tpaid]))?"'".$dados['tpafaixaetaria9anos'][$tpaid]."'":"NULL").",
					  tpafaixaetaria10anos=".((is_numeric($dados['tpafaixaetaria10anos'][$tpaid]))?"'".$dados['tpafaixaetaria10anos'][$tpaid]."'":"NULL").",
					  tpafaixaetaria11anos=".((is_numeric($dados['tpafaixaetaria11anos'][$tpaid]))?"'".$dados['tpafaixaetaria11anos'][$tpaid]."'":"NULL").",
					  tpafaixaetariaacima11anos=".((is_numeric($dados['tpafaixaetariaacima11anos'][$tpaid]))?"'".$dados['tpafaixaetariaacima11anos'][$tpaid]."'":"NULL").",
					  		
					  tpabolsafamiliaabaixo6anos=".((is_numeric($dados['tpabolsafamiliaabaixo6anos'][$tpaid]))?"'".$dados['tpabolsafamiliaabaixo6anos'][$tpaid]."'":"NULL").",
					  tpabolsafamilia6anos=".((is_numeric($dados['tpabolsafamilia6anos'][$tpaid]))?"'".$dados['tpabolsafamilia6anos'][$tpaid]."'":"NULL").",
					  tpabolsafamilia7anos=".((is_numeric($dados['tpabolsafamilia7anos'][$tpaid]))?"'".$dados['tpabolsafamilia7anos'][$tpaid]."'":"NULL").",
					  tpabolsafamilia8anos=".((is_numeric($dados['tpabolsafamilia8anos'][$tpaid]))?"'".$dados['tpabolsafamilia8anos'][$tpaid]."'":"NULL").",
					  tpabolsafamilia9anos=".((is_numeric($dados['tpabolsafamilia9anos'][$tpaid]))?"'".$dados['tpabolsafamilia9anos'][$tpaid]."'":"NULL").",
					  tpabolsafamilia10anos=".((is_numeric($dados['tpabolsafamilia10anos'][$tpaid]))?"'".$dados['tpabolsafamilia10anos'][$tpaid]."'":"NULL").",
					  tpabolsafamilia11anos=".((is_numeric($dados['tpabolsafamilia11anos'][$tpaid]))?"'".$dados['tpabolsafamilia11anos'][$tpaid]."'":"NULL").",
					  tpabolsafamiliaacima11anos=".((is_numeric($dados['tpabolsafamiliaacima11anos'][$tpaid]))?"'".$dados['tpabolsafamiliaacima11anos'][$tpaid]."'":"NULL").",
					  		
					  tpavivemcomunidadeabaixo6anos=".((is_numeric($dados['tpavivemcomunidadeabaixo6anos'][$tpaid]))?"'".$dados['tpavivemcomunidadeabaixo6anos'][$tpaid]."'":"NULL").",
					  tpavivemcomunidade6anos=".((is_numeric($dados['tpavivemcomunidade6anos'][$tpaid]))?"'".$dados['tpavivemcomunidade6anos'][$tpaid]."'":"NULL").",
					  tpavivemcomunidade7anos=".((is_numeric($dados['tpavivemcomunidade7anos'][$tpaid]))?"'".$dados['tpavivemcomunidade7anos'][$tpaid]."'":"NULL").",
					  tpavivemcomunidade8anos=".((is_numeric($dados['tpavivemcomunidade8anos'][$tpaid]))?"'".$dados['tpavivemcomunidade8anos'][$tpaid]."'":"NULL").",
					  tpavivemcomunidade9anos=".((is_numeric($dados['tpavivemcomunidade9anos'][$tpaid]))?"'".$dados['tpavivemcomunidade9anos'][$tpaid]."'":"NULL").",
					  tpavivemcomunidade10anos=".((is_numeric($dados['tpavivemcomunidade10anos'][$tpaid]))?"'".$dados['tpavivemcomunidade10anos'][$tpaid]."'":"NULL").",
					  tpavivemcomunidade11anos=".((is_numeric($dados['tpavivemcomunidade11anos'][$tpaid]))?"'".$dados['tpavivemcomunidade11anos'][$tpaid]."'":"NULL").",
					  tpavivemcomunidadeacima11anos=".((is_numeric($dados['tpavivemcomunidadeacima11anos'][$tpaid]))?"'".$dados['tpavivemcomunidadeacima11anos'][$tpaid]."'":"NULL").",
					  		
					  tpafreqcrecheabaixo6anos=".((is_numeric($dados['tpafreqcrecheabaixo6anos'][$tpaid]))?"'".$dados['tpafreqcrecheabaixo6anos'][$tpaid]."'":"NULL").",
					  tpafreqcreche6anos=".((is_numeric($dados['tpafreqcreche6anos'][$tpaid]))?"'".$dados['tpafreqcreche6anos'][$tpaid]."'":"NULL").",
					  tpafreqcreche7anos=".((is_numeric($dados['tpafreqcreche7anos'][$tpaid]))?"'".$dados['tpafreqcreche7anos'][$tpaid]."'":"NULL").",
					  tpafreqcreche8anos=".((is_numeric($dados['tpafreqcreche8anos'][$tpaid]))?"'".$dados['tpafreqcreche8anos'][$tpaid]."'":"NULL").",
					  tpafreqcreche9anos=".((is_numeric($dados['tpafreqcreche9anos'][$tpaid]))?"'".$dados['tpafreqcreche9anos'][$tpaid]."'":"NULL").",
					  tpafreqcreche10anos=".((is_numeric($dados['tpafreqcreche10anos'][$tpaid]))?"'".$dados['tpafreqcreche10anos'][$tpaid]."'":"NULL").",
					  tpafreqcreche11anos=".((is_numeric($dados['tpafreqcreche11anos'][$tpaid]))?"'".$dados['tpafreqcreche11anos'][$tpaid]."'":"NULL").",
					  tpafreqcrecheacima11anos=".((is_numeric($dados['tpafreqcrecheacima11anos'][$tpaid]))?"'".$dados['tpafreqcrecheacima11anos'][$tpaid]."'":"NULL").",
					  		
					  tpafreqpreescolaabaixo6anos=".((is_numeric($dados['tpafreqpreescolaabaixo6anos'][$tpaid]))?"'".$dados['tpafreqpreescolaabaixo6anos'][$tpaid]."'":"NULL").",
					  tpafreqpreescola6anos=".((is_numeric($dados['tpafreqpreescola6anos'][$tpaid]))?"'".$dados['tpafreqpreescola6anos'][$tpaid]."'":"NULL").",
					  tpafreqpreescola7anos=".((is_numeric($dados['tpafreqpreescola7anos'][$tpaid]))?"'".$dados['tpafreqpreescola7anos'][$tpaid]."'":"NULL").",
					  tpafreqpreescola8anos=".((is_numeric($dados['tpafreqpreescola8anos'][$tpaid]))?"'".$dados['tpafreqpreescola8anos'][$tpaid]."'":"NULL").",
					  tpafreqpreescola9anos=".((is_numeric($dados['tpafreqpreescola9anos'][$tpaid]))?"'".$dados['tpafreqpreescola9anos'][$tpaid]."'":"NULL").",
					  tpafreqpreescola10anos=".((is_numeric($dados['tpafreqpreescola10anos'][$tpaid]))?"'".$dados['tpafreqpreescola10anos'][$tpaid]."'":"NULL").",
					  tpafreqpreescola11anos=".((is_numeric($dados['tpafreqpreescola11anos'][$tpaid]))?"'".$dados['tpafreqpreescola11anos'][$tpaid]."'":"NULL").",
					  tpafreqpreescolaacima11anos=".((is_numeric($dados['tpafreqpreescolaacima11anos'][$tpaid]))?"'".$dados['tpafreqpreescolaacima11anos'][$tpaid]."'":"NULL")." 
					WHERE tpaid='".$tpaid."'";
			
			$db->executar($sql);
			$db->commit();
			
		}
	}
	
	$al = array("alert"=>"Informa��es das Turmas gravadas com sucesso","location"=>"sispacto3.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=atividadesobrigatorias&fpbid=".$dados['fpbid']);
	alertlocation($al);
	
	
}

function excluirMateriaisProfessoresFoto($dados) {
	global $db;
	
	$sql = "DELETE FROM sispacto3.materiaisprofessoresfotos WHERE mpfid='".$dados['mpfid']."'";
	$db->executar($sql);
	$db->commit();
	
	$al = array("alert"=>"Foto exclu�da com sucesso","location"=>"sispacto3.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=materiais");
	alertlocation($al);
	
}

function gravarAprendizagemTurma($dados) {
	global $db;
	
	if(!$_SESSION['sispacto3']['professoralfabetizador']['uncid']) {
		$al = array("alert"=>"Informa��es n�o encontradas. Tente novamente","location"=>"sispacto3.php?modulo=inicio&acao=C");
		alertlocation($al);
	}
	
	if($dados['tpaid']) {
		
		$sql = "SELECT rfuparcela FROM sispacto3.folhapagamentouniversidade WHERE fpbid='".$dados['fpbid']."' AND pflcod='".PFL_PROFESSORALFABETIZADOR."' AND uncid='".$_SESSION['sispacto3']['professoralfabetizador']['uncid']."'";
		$rfuparcela = $db->pegaUm($sql);
		
		foreach($dados['tpaid'] as $tpaid) {
			$sql = "UPDATE sispacto3.turmasprofessoresalfabetizadores SET 
					  tpatotalmeninos{$rfuparcela}=".((is_numeric($dados['tpatotalmeninos'][$tpaid]))?"'".$dados['tpatotalmeninos'][$tpaid]."'":"NULL").",
					  tpatotalmeninas{$rfuparcela}=".((is_numeric($dados['tpatotalmeninas'][$tpaid]))?"'".$dados['tpatotalmeninas'][$tpaid]."'":"NULL").",
					  tpafaixaetariaabaixo6anos{$rfuparcela}=".((is_numeric($dados['tpafaixaetariaabaixo6anos'][$tpaid]))?"'".$dados['tpafaixaetariaabaixo6anos'][$tpaid]."'":"NULL").",
					  tpafaixaetaria6anos{$rfuparcela}=".((is_numeric($dados['tpafaixaetaria6anos'][$tpaid]))?"'".$dados['tpafaixaetaria6anos'][$tpaid]."'":"NULL").",
					  tpafaixaetaria7anos{$rfuparcela}=".((is_numeric($dados['tpafaixaetaria7anos'][$tpaid]))?"'".$dados['tpafaixaetaria7anos'][$tpaid]."'":"NULL").",
					  tpafaixaetaria8anos{$rfuparcela}=".((is_numeric($dados['tpafaixaetaria8anos'][$tpaid]))?"'".$dados['tpafaixaetaria8anos'][$tpaid]."'":"NULL").",
					  tpafaixaetaria9anos{$rfuparcela}=".((is_numeric($dados['tpafaixaetaria9anos'][$tpaid]))?"'".$dados['tpafaixaetaria9anos'][$tpaid]."'":"NULL").",
					  tpafaixaetaria10anos{$rfuparcela}=".((is_numeric($dados['tpafaixaetaria10anos'][$tpaid]))?"'".$dados['tpafaixaetaria10anos'][$tpaid]."'":"NULL").",
					  tpafaixaetaria11anos{$rfuparcela}=".((is_numeric($dados['tpafaixaetaria11anos'][$tpaid]))?"'".$dados['tpafaixaetaria11anos'][$tpaid]."'":"NULL").",
					  tpafaixaetariaacima11anos{$rfuparcela}=".((is_numeric($dados['tpafaixaetariaacima11anos'][$tpaid]))?"'".$dados['tpafaixaetariaacima11anos'][$tpaid]."'":"NULL")."
					WHERE tpaid='".$tpaid."'";
			
			$db->executar($sql);
		}
	}
	
	$db->commit();
	
	if($dados['catid']) {
		
		foreach($dados['catid'] as $tpaid => $arr) {
			
			foreach($arr as $catid) {
				
				$sql = "SELECT actid as id_aprendizagem FROM sispacto3.aprendizagemconhecimentoturma WHERE tpaid='".$tpaid."' AND catid='".$catid."'";
				$id_aprendizagem = $db->pegaUm($sql);
				
				if($id_aprendizagem) {
					$sql = "UPDATE sispacto3.aprendizagemconhecimentoturma SET actsim=".(($dados['actsim'][$tpaid][$catid])?"'".$dados['actsim'][$tpaid][$catid]."'":"NULL").", 
																			   actparcialmente=".(($dados['actparcialmente'][$tpaid][$catid])?"'".$dados['actparcialmente'][$tpaid][$catid]."'":"NULL").",
																			   actnao=".(($dados['actnao'][$tpaid][$catid])?"'".$dados['actnao'][$tpaid][$catid]."'":"NULL")." 
							WHERE actid='".$id_aprendizagem."'";
					
					$db->executar($sql);
					$db->commit();
					
				} else {
					
					$sql = "SELECT count(*) as qtd FROM sispacto3.aprendizagemconhecimentoturma WHERE tpaid='".$tpaid."' AND catid='".$catid."'";
					$qtd = $db->pegaUm($sql);
					
					if(!$qtd) {
						
						$sql = "INSERT INTO sispacto3.aprendizagemconhecimentoturma(
			            		catid, tpaid, actsim, actparcialmente, actnao) 
								SELECT '".$catid."', 
									   '".$tpaid."', 
									   ".(($dados['actsim'][$tpaid][$catid])?"'".$dados['actsim'][$tpaid][$catid]."'":"NULL").", 
									   ".(($dados['actparcialmente'][$tpaid][$catid])?"'".$dados['actparcialmente'][$tpaid][$catid]."'":"NULL").", 
									   ".(($dados['actnao'][$tpaid][$catid])?"'".$dados['actnao'][$tpaid][$catid]."'":"NULL")." 
								FROM coalesce((SELECT actid::text FROM sispacto3.aprendizagemconhecimentoturma WHERE catid='".$catid."' AND tpaid='".$tpaid."'),NULL) as foo WHERE foo IS NULL";
						
						$db->executar($sql);
						$db->commit();
						
					}
					
					
				}
				
			}
		}
	}
	
	$al = array("alert"=>"Dados gravados com sucesso","location"=>"sispacto3.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=atividadesobrigatorias&fpbid=".$dados['fpbid']);
	alertlocation($al);
	
}





function verificarAprendizagemTurma($dados) {
	global $db;
	
	$tot = $db->pegaUm("SELECT count(actid) as t FROM sispacto3.aprendizagemconhecimentoturma a 
				 	    INNER JOIN sispacto3.turmasprofessoresalfabetizadores t ON a.tpaid = t.tpaid 
				 	    WHERE t.iusd='".$dados['iusd']."'");
	
	if($tot) {
		echo 'TRUE';
	} else {
		echo 'FALSE';
	}
}

function gerenciarUsoMateriaisDidaticos($dados) {
	global $db;
	
	if($dados['usomaterialdidatico']) {
		foreach($dados['usomaterialdidatico'] as $catid => $umdopcao) {
			
			$umdid = $db->pegaUm("SELECT umdid FROM sispacto3.usomateriaisdidaticos WHERE catid='{$catid}' AND iusd='".$_SESSION['sispacto3']['professoralfabetizador']['iusd']."'");
			
			if($umdid) {
				
				$sql = "UPDATE sispacto3.usomateriaisdidaticos SET umdopcao='{$umdopcao}' WHERE umdid='{$umdid}'";
				
				$db->executar($sql);
				
			} else {
				
				$sql = "INSERT INTO sispacto3.usomateriaisdidaticos(
            			iusd, catid, umdopcao)
    					VALUES ('".$_SESSION['sispacto3']['professoralfabetizador']['iusd']."', '{$catid}', '{$umdopcao}');";
				
				$db->executar($sql);
				
			}
			
		}
	}
	
	$db->commit();

	$al = array("alert"=>"Dados gravados com sucesso","location"=>"sispacto3.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=atividadesobrigatorias&fpbid=".$dados['fpbid']);
	alertlocation($al);
	
	
}

function gravarRelatoExperiencia($dados) {
	global $db;
	if(!$dados['reeturma']) $err[] = "Preencha as turmas";	
	if(!$dados['reeobjetivo']) $err[] = "Preencha os Objetivos principais da experi�ncia";
	if(!$dados['reetecnicas']) $err[] = "Preencha as T�cnicas utilizadas";
	if(!$dados['reedificuldades']) $err[] = "Preencha as Dificuldades na realiza��o da atividade";
	$dt_val = explode("/",$dados['reeperiodoexperienciaini']);
	if(!checkdate((($dt_val[1])?$dt_val[1]:"0"),(($dt_val[0])?$dt_val[0]:"0"),(($dt_val[2])?$dt_val[2]:"0"))) $err[] = "Formato da data in�cio inv�lida";
	$dt_val = explode("/",$dados['reeperiodoexperienciafim']);
	if(!checkdate((($dt_val[1])?$dt_val[1]:"0"),(($dt_val[0])?$dt_val[0]:"0"),(($dt_val[2])?$dt_val[2]:"0"))) $err[] = "Formato da data fim inv�lida";
	
	if($err) {
		$al = array("alert"=>"Foram encontrados problemas :".'\n'.implode('\n',$err),"javascript"=>"window.history.back();");
		alertlocation($al);
	}
	
	if(!$_SESSION['sispacto3']['professoralfabetizador']['iusd']) {
		$al = array("alert"=>"Foram encontrados problemas internos. Por favor tente novamente.","location"=>"sispacto3.php?modulo=inicio&acao=C");
		alertlocation($al);
	}
	
	$reeid = $db->pegaUm("SELECT reeid FROM sispacto3.relatoexperiencia WHERE iusd='".$_SESSION['sispacto3']['professoralfabetizador']['iusd']."'");
	
	$idx = array_keys($dados);
	
	foreach($idx as $ix)
	if(is_array($dados[$ix]))
	foreach($dados[$ix] as $key => $vlr)
		$dados[$ix][$key] = $vlr.(($dados['tx_'.$ix.'_'.$vlr])?"||".$dados['tx_'.$ix.'_'.$vlr]:"");
	else $dados[$ix] = $dados[$ix].(($dados['tx_'.$ix.'_'.$dados[$ix]])?"||".$dados['tx_'.$ix.'_'.$dados[$ix]]:"");
	
	
	if($reeid) {
		
		$sql = "UPDATE sispacto3.relatoexperiencia SET 
						reeareatematica       ='".$dados['reeareatematica']."', 
						reeturma			  ='".implode(";",$dados['reeturma'])."', 
						reeperiodoexperienciaini ='".formata_data_sql($dados['reeperiodoexperienciaini'])."', 
						reeperiodoexperienciafim ='".formata_data_sql($dados['reeperiodoexperienciafim'])."',
			            reeobjetivo			  ='".addslashes(implode(";",$dados['reeobjetivo']))."', 
						reetecnicas			  ='".addslashes(implode(";",$dados['reetecnicas']))."', 
						reetempoduracao		  ='".$dados['reetempoduracao']."', 
						reeorganizacao		  ='".$dados['reeorganizacao']."', 
						reemateriaisutilizados='".$dados['reemateriaisutilizados']."', 
			            reelocal			  ='".$dados['reelocal']."', 
						reedificuldades		  ='".implode(";",$dados['reedificuldades'])."', 
						reeenvolvimento		  ='".$dados['reeenvolvimento']."', 
						reetitulo			  ='".addslashes($dados['tx_reetitulo'])."', 
						reeresumo			  ='".substr(addslashes($dados['tx_reeresumo']),0,1000)."', 
			            reeobjetivosalcancados='".$dados['reeobjetivosalcancados']."', 
			            reerepetirexperiencia ='".$dados['reerepetirexperiencia']."' 
			    WHERE reeid='".$reeid."'";
		
	} else {
		
		$sql = "INSERT INTO sispacto3.relatoexperiencia(
			            iusd, 
						reeareatematica, 
						reeturma, 
						reeperiodoexperienciaini, 
						reeperiodoexperienciafim,
			            reeobjetivo, 
						reetecnicas, 
						reetempoduracao, 
						reeorganizacao, 
						reemateriaisutilizados, 
			            reelocal, 
						reedificuldades, 
						reeenvolvimento, 
						reetitulo, 
						reeresumo, 
			            reeobjetivosalcancados, reerepetirexperiencia)
			    VALUES ('".$_SESSION['sispacto3']['professoralfabetizador']['iusd']."', 
			    		'".$dados['reeareatematica']."', 
			    		'".implode(";",$dados['reeturma'])."', 
			    		'".formata_data_sql($dados['reeperiodoexperienciaini'])."', 
			    		'".formata_data_sql($dados['reeperiodoexperienciafim'])."',
			            '".addslashes(implode(";",$dados['reeobjetivo']))."', 
			            '".addslashes(implode(";",$dados['reetecnicas']))."', 
			            '".addslashes($dados['reetempoduracao'])."', 
			            '".addslashes($dados['reeorganizacao'])."', 
			            '".addslashes($dados['reemateriaisutilizados'])."', 
			            '".addslashes($dados['reelocal'])."', 
			            '".implode(";",$dados['reedificuldades'])."', 
			            '".addslashes($dados['reeenvolvimento'])."', 
			            '".addslashes($dados['tx_reetitulo'])."', 
			            '".substr(addslashes($dados['tx_reeresumo']),0,1000)."', 
			            '".addslashes($dados['reeobjetivosalcancados'])."', 
			            '".addslashes($dados['reerepetirexperiencia'])."');";
	}
	
	$db->executar($sql);
	$db->commit();
	
	$al = array("alert"=>"Dados gravados com sucesso","location"=>"sispacto3.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=atividadesobrigatorias&fpbid=".$dados['fpbid']);
	alertlocation($al);
	
	
}

function estruturaImpressaoANA($dados) {

	$es['imaparticiparam'] = array('texto' => 'Nesta escola uma ou mais crian�as participaram da Avalia��o Nacional de Aprendizagem (ANA)?',
			'tipo' => 'radio',
			'opcoes' => array(
					array('valor'=>'S','descricao'=>'Sim'),
					array('valor'=>'N','descricao'=>'N�o')
			)
	);
	
	$es['imaacessoresultados'] = array('texto' => 'Voc� teve acesso aos resultados da ANA de sua escola?',
			'tipo' => 'radio',
			'opcoes' => array(
					array('valor'=>'S','descricao'=>'Sim'),
					array('valor'=>'N','descricao'=>'N�o')
			)
	);
	
	$es['imaresultadosescola'] = array('texto' => 'Os resultados da ANA na sua escola:',
			'tipo' => 'radio',
			'opcoes' => array(
					array('valor'=>'1','descricao'=>'Est�o dentro de sua expectativa'),
					array('valor'=>'2','descricao'=>'Est�o acima de sua expectativa'),
					array('valor'=>'3','descricao'=>'Est�o abaixo de sua expectativa'),
					array('valor'=>'4','descricao'=>'N�o sabe avaliar'),
			)
	);
	
	
	$es['imaaspectos'] = array('texto' => 'Qual a sua avalia��o sobre os seguintes aspectos da ANA:',
			'tipo' => 'gridradio',
			'colunas' => array(
							array('codigo'=>'P','descricao'=>'P�ssimo'),
							array('codigo'=>'U','descricao'=>'Ruim'),
							array('codigo'=>'R','descricao'=>'Regular'),
							array('codigo'=>'B','descricao'=>'Bom'),
							array('codigo'=>'O','descricao'=>'�timo'),
							array('codigo'=>'N','descricao'=>'N�o sei informar')
						 ),
				
			'linhas' => array(
					array('codigo'=>'orientacoes','descricao'=>'Orienta��es previas a aplica��o'),
					array('codigo'=>'tempoaplicacao','descricao'=>'Tempo de aplica��o da avalia��o'),
					array('codigo'=>'horarioaplicacao','descricao'=>'Hor�rio de aplica��o da avalia��o'),
					array('codigo'=>'quantidadequestoes','descricao'=>'Quantidade de quest�es'),
					array('codigo'=>'clarezaquestoes','descricao'=>'Clareza na apresenta��o das quest�es'),
					array('codigo'=>'aplicadorexterno','descricao'=>'Necessidade de aplicador externo'),
					array('codigo'=>'localavaliacao','descricao'=>'Local de aplica��o da avalia��o'),
					array('codigo'=>'apresentacaoavaliacao','descricao'=>'Forma de apresenta��o da avalia��o'),
					array('codigo'=>'apresentacaoresultados','descricao'=>'Forma de apresenta��o dos resultados da escola')
						
			)
	);
	
	$es['imafatores'] = array('texto' => 'Avalie como os fatores abaixo interferiram nos resultados de alfabetiza��o das crian�as:',
			'tipo' => 'gridradio',
			'colunas' => array(
					array('codigo'=>'I','descricao'=>'N�o interferiu'),
					array('codigo'=>'P','descricao'=>'Interferiu pouco'),
					array('codigo'=>'M','descricao'=>'Interferiu muito'),
					array('codigo'=>'N','descricao'=>'N�o sei informar')
			),
	
			'linhas' => array(
					array('codigo'=>'gestaoescolar','descricao'=>'Gest�o escolar'),
					array('codigo'=>'formacaoprofessores','descricao'=>'Forma��o dos professores'),
					array('codigo'=>'praticaspedagogicas','descricao'=>'Pr�ticas pedag�gicas de sala de aula'),
					array('codigo'=>'perfilalunos','descricao'=>'Perfil dos  alunos'),
					array('codigo'=>'recursosdidaticos','descricao'=>'Recursos did�ticos'),
					array('codigo'=>'estruturafisica','descricao'=>'Estrutura f�sica da escola'),
					array('codigo'=>'participacaofamilia','descricao'=>'Participa��o da fam�lia na vida escolar da crian�a'),
					array('codigo'=>'relacoesinterpessoais','descricao'=>'Rela��es interpessoais da escola')
	
			)
	);
	
	return $es;
	
	
}

function estruturaContribuicaoPacto($dados) {


	$es['cpacontribuicao'] = array('texto' => 'Informe qual foi a contribui��o da Forma��o do Pacto para:',
			'tipo' => 'gridradio',
			'colunas' => array(
					array('codigo'=>'M','descricao'=>'Contribuiu muito'),
					array('codigo'=>'P','descricao'=>'Contribuiu pouco'),
					array('codigo'=>'C','descricao'=>'N�o contribuiu')
			),

			'linhas' => array(
					array('codigo'=>'1','descricao'=>'a reflex�o sobre a pr�tica pedag�gica'),
					array('codigo'=>'2','descricao'=>'o aprofundamento da compreens�o sobre o curr�culo nos anos iniciais do Ensino Fundamental e os direitos de aprendizagem'),
					array('codigo'=>'3','descricao'=>'a amplia��o de conhecimentos sobre avalia��o no ciclo de alfabetiza��o'),
					array('codigo'=>'4','descricao'=>'a amplia��o de estrat�gias de inclus�o de crian�as com defici�ncias'),
					array('codigo'=>'5','descricao'=>'o planejamento de mais estrat�gias para lidar com a heterogeneidade presente nas salas de aula quanto aos processos de aprendizagem'),
					array('codigo'=>'6','descricao'=>'a an�lise e cria��o de propostas de organiza��o de rotinas da alfabetiza��o na perspectiva do letramento'),
					array('codigo'=>'7','descricao'=>'o planejamento de projetos did�ticos e sequ�ncias did�ticas, integrando diferentes componentes curriculares'),
					array('codigo'=>'8','descricao'=>'o planejamento de aulas por meio de situa��es diferenciadas de ensino'),
					array('codigo'=>'9','descricao'=>'o uso de jogos e recursos did�ticos diversificados'),
					array('codigo'=>'10','descricao'=>'o uso de recursos did�ticos distribu�dos pelo Minist�rio da Educa��o (livros did�ticos e obras complementares aprovados no PNLD; livros do PNBE e PNBE Especial; jogos did�ticos)')

			)
	);
	
	$es['cpadificuldade'] = array('texto' => 'Informe a dificuldade encontrada para:',
			'tipo' => 'gridradio',
			'colunas' => array(
					array('codigo'=>'M','descricao'=>'Muita dificuldade'),
					array('codigo'=>'P','descricao'=>'Pouca dificuldade'),
					array('codigo'=>'N','descricao'=>'Nenhuma dificuldade')
			),
	
			'linhas' => array(
					array('codigo'=>'1','descricao'=>'comunicar-se com o Minist�rio da Educa��o pelo e-mail '. $_SESSION['email_sistema']),
					array('codigo'=>'2','descricao'=>'utilizar o Sispacto 2014')
	
			)
	);
	

	return $es;


}

function estruturaRelatoExperiencia($dados) {
	
	$es['reeareatematica'] = array('texto' => '1. �rea tem�tica',
			'tipo' => 'radio',
			'opcoes' => array(
					array('valor'=>'P','descricao'=>'L�ngua Portuguesa'),
					array('valor'=>'M','descricao'=>'Matem�tica')
			)
	);
		
	$es['reeturma'] = array('texto' => '2. Turma',
			'tipo' => 'checkbox',
			'opcoes' => array(
					array('valor'=>'1','descricao'=>'1� ano'),
					array('valor'=>'2','descricao'=>'2� ano/ 1� s�rie'),
					array('valor'=>'3','descricao'=>'2� ano/ 1� s�rie'),
					array('valor'=>'4','descricao'=>'3� ano/ 2� s�rie'),
					array('valor'=>'5','descricao'=>'3� s�rie'),
					array('valor'=>'6','descricao'=>'Multisseriada')
			)
	);
		
	$es['reeperiodoexperiencia'] = array('texto' => '3. Per�odo em que a experi�ncia foi realizada',
			'tipo' => 'data',
			'datas' => array(
					array('valor'=>'ini','descricao'=>'In�cio (dd/mm/aaaa)'),
					array('valor'=>'fim','descricao'=>'T�rmino (dd/mm/aaaa)')
			)
	);
		
		
	$es['reeobjetivo'] = array('texto' => '4. Objetivo principal da experi�ncia',
			'tipo' => 'checkbox',
			'opcoes' => array(
					array('valor'=>'1','descricao'=>'Apropriar-se do Sistema de Escrita Alfab�tica (SEA)'),
					array('valor'=>'2','descricao'=>'Reconhecer a fun��o social de um texto'),
					array('valor'=>'3','descricao'=>'Identificar e utilizar diferentes suportes textuais'),
					array('valor'=>'4','descricao'=>'Produzir textos utilizando diversos g�neros'),
					array('valor'=>'5','descricao'=>'Conhecer e fazer uso da norma padr�o na escrita de textos'),
					array('valor'=>'6','descricao'=>'Outro objetivo','complementotexto' => 'Qual?'),
			)
	);
		
	$es['reetecnicas'] = array('texto' => '5. T�cnicas utilizadas',
			'tipo' => 'checkbox',
			'opcoes' => array(
					array('valor'=>'1','descricao'=>'Brincadeira'),
					array('valor'=>'2','descricao'=>'Jogo'),
					array('valor'=>'3','descricao'=>'Dramatiza��o'),
					array('valor'=>'4','descricao'=>'Exposi��o dialogada'),
					array('valor'=>'5','descricao'=>'Exerc�cio escrito'),
					array('valor'=>'6','descricao'=>'Leitura em voz alta'),
					array('valor'=>'7','descricao'=>'Recorte e colagem'),
					array('valor'=>'8','descricao'=>'Outra t�cnica','complementotexto' => 'Qual?'),
			)
	);
		
		
	$es['reetempoduracao'] = array('texto' => '6. Tempo de dura��o da experi�ncia',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Menos de 20 minutos'),
					array('valor'=>'2','descricao'=>'Entre 20 e 40 minutos'),
					array('valor'=>'3','descricao'=>'Mais de 40 minutos')
			)
	);
		
	$es['reeorganizacao'] = array('texto' => '7. Organiza��o',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Individual'),
					array('valor'=>'2','descricao'=>'2 pessoas'),
					array('valor'=>'3','descricao'=>'3 pessoas'),
					array('valor'=>'4','descricao'=>'Mais de 3 pessoas')
			)
	);
		
	$es['reemateriaisutilizados'] = array('texto' => '8. Materiais utilizados',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Obras complementares do PNLD'),
					array('valor'=>'2','descricao'=>'Obras liter�rias do PNBE'),
					array('valor'=>'3','descricao'=>'Outras obras liter�rias'),
					array('valor'=>'4','descricao'=>'Livros did�ticos do PNLD'),
					array('valor'=>'5','descricao'=>'Jogos de alfabetiza��o'),
					array('valor'=>'6','descricao'=>'Jogos de matem�tica'),
					array('valor'=>'7','descricao'=>'Revistas, jornais, gibis e outros suportes textuais'),
					array('valor'=>'8','descricao'=>'Caixa matem�tica'),
					array('valor'=>'9','descricao'=>'Outros materiais','complementotexto' => 'Qual?'),
			)
	);
		
	$es['reelocal'] = array('texto' => '9. Local em que a atividade foi realizada',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Em sala de aula (Na escola)'),
					array('valor'=>'2','descricao'=>'No p�tio (Na escola)'),
					array('valor'=>'3','descricao'=>'Outro ambiente (Na escola)', 'complementotexto' => 'Qual?'),
					array('valor'=>'4','descricao'=>'Pra�a, parque ou jardim (Fora da escola)'),
					array('valor'=>'5','descricao'=>'Teatro/ cinema (Fora da escola)'),
					array('valor'=>'6','descricao'=>'Biblioteca (Fora da escola)'),
					array('valor'=>'7','descricao'=>'Quadras esportivas ou similares (Fora da escola)'),
					array('valor'=>'8','descricao'=>'Outro espa�o (Fora da escola)','complementotexto' => 'Qual?'),
			)
	);
		
	$es['reedificuldades'] = array('texto' => '10. Dificuldades na realiza��o da atividade',
			'tipo' => 'checkbox',
			'opcoes' => array(array('valor'=>'1','descricao'=>'N�o houve dificuldade para realizar a atividade'),
					array('valor'=>'2','descricao'=>'Incompreens�o da atividade por parte das crian�as'),
					array('valor'=>'3','descricao'=>'Dificuldade das crian�as em realizar as atividades propostas'),
					array('valor'=>'4','descricao'=>'Desinteresse da maioria das crian�as pela atividade'),
					array('valor'=>'5','descricao'=>'Tempo escasso para concluir a atividade'),
					array('valor'=>'6','descricao'=>'Falta de materiais apropriados para realizar a atividade'),
					array('valor'=>'7','descricao'=>'Espa�o inadequado para realizar as atividades'),
					array('valor'=>'8','descricao'=>'Outra dificuldade','complementotexto' => 'Qual?'),
			)
	);
		
	$es['reeenvolvimento'] = array('texto' => '11. Como voc� avalia o grau de envolvimento das crian�as?',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Todas as crian�as participaram da atividade'),
					array('valor'=>'2','descricao'=>'Mais da metade das crian�as participou da atividade'),
					array('valor'=>'3','descricao'=>'Metade das crian�as participou da atividade'),
					array('valor'=>'4','descricao'=>'Menos da metade as crian�as participou da atividade'),
					array('valor'=>'5','descricao'=>'Nenhuma crian�a participou da atividade')
			)
	);
		
	$es['reetitulo'] = array('texto' => '12. Titulo da experi�ncia',
			'text' => array('maxsize'=>'100','rows'=>'4','cols'=>'40')
	);
		
	$es['reeresumo'] = array('texto' => '13. Resumo da experi�ncia',
			'text' => array('maxsize'=>'1000','rows'=>'4','cols'=>'40','dica'=>'Escreva uma s�ntese objetiva da atividade, considerando que as caracter�sticas gerais j� foram indicadas nos itens anteriores. Valorize as informa��es essenciais que permitam a qualquer leitor entender o que foi feito.')
	);
		
	$es['reeobjetivosalcancados'] = array('texto' => '14. Os objetivos principais foram alcan�ados?',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Sim'),
					array('valor'=>'2','descricao'=>'N�o'),
					array('valor'=>'3','descricao'=>'Parcialmente','complementotexto' => 'Por qu�?'),
			)
	);
		
	$es['reerepetirexperiencia'] = array('texto' => '15. Voc� pretende repetir essa experi�ncia futuramente?',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Sim'),
					array('valor'=>'2','descricao'=>'N�o')
			)
	);
	
	return $es;
}

function gravarImpressoesANA($dados) {
	global $db;
	
	if($dados['tpaid']) {
		foreach($dados['tpaid'] as $tpaid) {
			$imaid = $db->pegaUm("SELECT imaid FROM sispacto3.impressoesana WHERE iusd='".$_SESSION['sispacto3']['professoralfabetizador']['iusd']."' AND tpaid='{$tpaid}'");
			
			if($imaid) $sql = "UPDATE sispacto3.impressoesana SET imaparticiparam='".$dados['imaparticiparam'][$tpaid]."', imaacessoresultados='".$dados['imaacessoresultados'][$tpaid]."', imaresultadosescola='".$dados['imaresultadosescola'][$tpaid]."', 
					            imaaspectosorientacoes='".$dados['imaaspectosorientacoes'][$tpaid]."', imaaspectostempoaplicacao='".$dados['imaaspectostempoaplicacao'][$tpaid]."', imaaspectoshorarioaplicacao='".$dados['imaaspectoshorarioaplicacao'][$tpaid]."', 
					            imaaspectosquantidadequestoes='".$dados['imaaspectosquantidadequestoes'][$tpaid]."', imaaspectosclarezaquestoes='".$dados['imaaspectosclarezaquestoes'][$tpaid]."', imaaspectosaplicadorexterno='".$dados['imaaspectosaplicadorexterno'][$tpaid]."', 
					            imaaspectoslocalavaliacao='".$dados['imaaspectoslocalavaliacao'][$tpaid]."', imaaspectosapresentacaoavaliacao='".$dados['imaaspectosapresentacaoavaliacao'][$tpaid]."', 
					            imaaspectosapresentacaoresultados='".$dados['imaaspectosapresentacaoresultados'][$tpaid]."', imafatoresgestaoescolar='".$dados['imafatoresgestaoescolar'][$tpaid]."', imafatoresformacaoprofessores='".$dados['imafatoresformacaoprofessores'][$tpaid]."', 
					            imafatorespraticaspedagogicas='".$dados['imafatorespraticaspedagogicas'][$tpaid]."', imafatoresperfilalunos='".$dados['imafatoresperfilalunos'][$tpaid]."', imafatoresrecursosdidaticos='".$dados['imafatoresrecursosdidaticos'][$tpaid]."', 
					            imafatoresestruturafisica='".$dados['imafatoresestruturafisica'][$tpaid]."', imafatoresparticipacaofamilia='".$dados['imafatoresparticipacaofamilia'][$tpaid]."', imafatoresrelacoesinterpessoais='".$dados['imafatoresrelacoesinterpessoais'][$tpaid]."' 
					            WHERE imaid='{$imaid}'";
			else $sql = "INSERT INTO sispacto3.impressoesana(
					            iusd, imaparticiparam, imaacessoresultados, imaresultadosescola, 
					            imaaspectosorientacoes, imaaspectostempoaplicacao, imaaspectoshorarioaplicacao, 
					            imaaspectosquantidadequestoes, imaaspectosclarezaquestoes, imaaspectosaplicadorexterno, 
					            imaaspectoslocalavaliacao, imaaspectosapresentacaoavaliacao, 
					            imaaspectosapresentacaoresultados, imafatoresgestaoescolar, imafatoresformacaoprofessores, 
					            imafatorespraticaspedagogicas, imafatoresperfilalunos, imafatoresrecursosdidaticos, 
					            imafatoresestruturafisica, imafatoresparticipacaofamilia, imafatoresrelacoesinterpessoais, 
					            tpaid)
					    VALUES ('".$_SESSION['sispacto3']['professoralfabetizador']['iusd']."', '".$dados['imaparticiparam'][$tpaid]."', '".$dados['imaacessoresultados'][$tpaid]."', '".$dados['imaresultadosescola'][$tpaid]."', 
					            '".$dados['imaaspectosorientacoes'][$tpaid]."', '".$dados['imaaspectostempoaplicacao'][$tpaid]."', '".$dados['imaaspectoshorarioaplicacao'][$tpaid]."', 
					            '".$dados['imaaspectosquantidadequestoes'][$tpaid]."', '".$dados['imaaspectosclarezaquestoes'][$tpaid]."', '".$dados['imaaspectosaplicadorexterno'][$tpaid]."', 
					            '".$dados['imaaspectoslocalavaliacao'][$tpaid]."', '".$dados['imaaspectosapresentacaoavaliacao'][$tpaid]."', 
					            '".$dados['imaaspectosapresentacaoresultados'][$tpaid]."', '".$dados['imafatoresgestaoescolar'][$tpaid]."', '".$dados['imafatoresformacaoprofessores'][$tpaid]."', 
					            '".$dados['imafatorespraticaspedagogicas'][$tpaid]."', '".$dados['imafatoresperfilalunos'][$tpaid]."', '".$dados['imafatoresrecursosdidaticos'][$tpaid]."', 
					            '".$dados['imafatoresestruturafisica'][$tpaid]."', '".$dados['imafatoresparticipacaofamilia'][$tpaid]."', '".$dados['imafatoresrelacoesinterpessoais'][$tpaid]."', 
					            '{$tpaid}');";
			
			$db->executar($sql);
			$db->commit();
			
		}
	}
	
	$al = array("alert"=>"Dados gravados com sucesso","location"=>"sispacto3.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=atividadesobrigatorias&fpbid=".$dados['fpbid']);
	alertlocation($al);
	

}

function estruturaQuestoesDiversas($dados) {
	
	$es['qudformaapoia'] = array('texto' => '1. De que forma a dire��o da escola apoia os professores alfabetizadores que participam do Pacto Nacional pela Alfabetiza��o na Idade Certa?',
			'tipo' => 'checkbox',
			'opcoes' => array(
					array('valor'=>'1','descricao'=>'Disponibilizando meios e espa�os adequados para os estudos.'),
					array('valor'=>'2','descricao'=>'Incentivando a revis�o do Projeto Pol�tico-Pedag�gico.'),
					array('valor'=>'3','descricao'=>'Promovendo reuni�es de pais e mestres e/ou eventos pedag�gicos para apresentar o Pacto.'),
					array('valor'=>'4','descricao'=>'Disponibilizando materiais de apoio � forma��o.'),
					array('valor'=>'5','descricao'=>'A dire��o da escola n�o apoia os professores que participam do Pacto.')
			)
	);
	

	$es['qudperiodicidade'] = array('texto' => '2. Com que periodicidade a escola promove atividades visando envolver as fam�lias dos estudantes no processos de alfabetiza��o e letramento dos filhos?',
			'tipo' => 'radio',
			'opcoes' => array(
					array('valor'=>'S','descricao'=>'Semanal'),
					array('valor'=>'M','descricao'=>'Mensal'),
					array('valor'=>'B','descricao'=>'Bimestral'),
					array('valor'=>'E','descricao'=>'Semestral'),
					array('valor'=>'N','descricao'=>'A escola n�o promove atividades com as fam�lias'),
			)
	);
	
	$es['qudformaparticipa'] = array('texto' => '3. De que forma o Conselho Escolar participa das atividades do Pacto Nacional da Alfabetiza��o na Idade Certa?',
			'tipo' => 'checkbox',
			'opcoes' => array(
					array('valor'=>'1','descricao'=>'Apresenta sugest�es e cr�ticas ao programa.'),
					array('valor'=>'2','descricao'=>'Acompanha o processo formativo dos professores alfabetizadores.'),
					array('valor'=>'3','descricao'=>'Prop�e altera��es no planejamento pedag�gico da escola voltado para as turmas de alfabetiza��o.'),
					array('valor'=>'4','descricao'=>'O Conselho Escolar nunca discutiu sobre alfabetiza��o.'),
					array('valor'=>'5','descricao'=>'Outra.','complementotexto' => 'Qual?'),
					array('valor'=>'6','descricao'=>'A escola n�o possui Conselho Escolar.')
			)
	);
	
	$es['qudmedidaparticipa'] = array('texto' => '4. Em que medida a comunidade escolar participa do Pacto pela Alfabetiza��o na Idade Certa:',
			'tipo' => 'radio',
			'opcoes' => array(
					array('valor'=>'A','descricao'=>'Participa ativamente'),
					array('valor'=>'M','descricao'=>'Participa moderadamente'),
					array('valor'=>'P','descricao'=>'Participa pouco'),
					array('valor'=>'N','descricao'=>'N�o participa')
			)
	);
	
	$es['qudmedidacontribui'] = array('texto' => '5. Em que medida o Pacto contribui para o seu conhecimento acerca do direitos de aprendizagem das crian�as, nos tr�s primeiros anos do ensino fundamental?',
			'tipo' => 'radio',
			'opcoes' => array(
					array('valor'=>'D','descricao'=>'Contribui decisivamente'),
					array('valor'=>'M','descricao'=>'Contribui moderadamente'),
					array('valor'=>'P','descricao'=>'Contribui um pouco'),
					array('valor'=>'N','descricao'=>'N�o contribui')
			)
	);
	


	return $es;
}

function gravarQuestoesDiversas($dados) {
	global $db;
	
	$idx = array_keys($dados);
	
	foreach($idx as $ix)
	if(is_array($dados[$ix]))
	foreach($dados[$ix] as $key => $vlr)
		$dados[$ix][$key] = $vlr.(($dados['tx_'.$ix.'_'.$vlr])?"||".$dados['tx_'.$ix.'_'.$vlr]:"");
	else $dados[$ix] = $dados[$ix].(($dados['tx_'.$ix.'_'.$dados[$ix]])?"||".$dados['tx_'.$ix.'_'.$dados[$ix]]:"");
	
	$qudid = $db->pegaUm("SELECT qudid FROM sispacto3.questoesdiversasatv8 WHERE iusd='".$_SESSION['sispacto3']['professoralfabetizador']['iusd']."'");
	
	if($qudid) $sql = "UPDATE sispacto3.questoesdiversasatv8 SET qudformaapoia='".implode(";",$dados['qudformaapoia'])."', 
														  qudperiodicidade='".$dados['qudperiodicidade']."', 
														  qudformaparticipa='".implode(";",$dados['qudformaparticipa'])."',
														  qudmedidaparticipa='".$dados['qudmedidaparticipa']."',
														  qudmedidacontribui='".$dados['qudmedidacontribui']."' WHERE qudid='{$qudid}'";
		
	else $sql = "INSERT INTO sispacto3.questoesdiversasatv8(
            iusd, qudformaapoia, qudperiodicidade, qudformaparticipa, 
            qudmedidaparticipa, qudmedidacontribui)
    		VALUES ('".$_SESSION['sispacto3']['professoralfabetizador']['iusd']."', '".implode(";",$dados['qudformaapoia'])."', '".$dados['qudperiodicidade']."', '".implode(";",$dados['qudformaparticipa'])."', 
            '".$dados['qudmedidaparticipa']."', '".$dados['qudmedidacontribui']."');";
	
	$db->executar($sql);
	$db->commit();
	
	$al = array("alert"=>"Dados gravados com sucesso","location"=>"sispacto3.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=atividadesobrigatorias&fpbid=".$dados['fpbid']);
	alertlocation($al);
	
}

function gravarContribuicaoPacto($dados) {
	global $db;
	
	$idx = array_keys($dados);
	
	foreach($idx as $ix)
	if(is_array($dados[$ix]))
	foreach($dados[$ix] as $key => $vlr)
		$dados[$ix][$key] = $vlr.(($dados['tx_'.$ix.'_'.$vlr])?"||".$dados['tx_'.$ix.'_'.$vlr]:"");
	else $dados[$ix] = $dados[$ix].(($dados['tx_'.$ix.'_'.$dados[$ix]])?"||".$dados['tx_'.$ix.'_'.$dados[$ix]]:"");
	
	$cpaid = $db->pegaUm("SELECT cpaid FROM sispacto3.contribuicaopacto WHERE iusd='".$_SESSION['sispacto3']['professoralfabetizador']['iusd']."'");

	if($cpaid) $sql = "UPDATE sispacto3.contribuicaopacto SET cpacontribuicao1='".$dados['cpacontribuicao1']."',
															  cpacontribuicao2='".$dados['cpacontribuicao2']."',
															  cpacontribuicao3='".$dados['cpacontribuicao3']."',
															  cpacontribuicao4='".$dados['cpacontribuicao4']."',
															  cpacontribuicao5='".$dados['cpacontribuicao5']."',
															  cpacontribuicao6='".$dados['cpacontribuicao6']."',
															  cpacontribuicao7='".$dados['cpacontribuicao7']."',
															  cpacontribuicao8='".$dados['cpacontribuicao8']."',
															  cpacontribuicao9='".$dados['cpacontribuicao9']."',
															  cpacontribuicao10='".$dados['cpacontribuicao10']."',
															  cpadificuldade1='".$dados['cpadificuldade1']."',
															  cpadificuldade2='".$dados['cpadificuldade2']."' 
													 WHERE cpaid='{$cpaid}'";
	
	else $sql = "INSERT INTO sispacto3.contribuicaopacto(
            iusd, cpacontribuicao1, cpacontribuicao2, cpacontribuicao3, 
            cpacontribuicao4, cpacontribuicao5, cpacontribuicao6, cpacontribuicao7, 
            cpacontribuicao8, cpacontribuicao9, cpacontribuicao10, cpadificuldade1, 
            cpadificuldade2)
    VALUES ('".$_SESSION['sispacto3']['professoralfabetizador']['iusd']."', '".$dados['cpacontribuicao1']."', '".$dados['cpacontribuicao2']."', '".$dados['cpacontribuicao3']."', 
            '".$dados['cpacontribuicao4']."', '".$dados['cpacontribuicao5']."', '".$dados['cpacontribuicao6']."', '".$dados['cpacontribuicao7']."', 
            '".$dados['cpacontribuicao8']."', '".$dados['cpacontribuicao9']."', '".$dados['cpacontribuicao10']."', '".$dados['cpadificuldade1']."', 
            '".$dados['cpadificuldade2']."');";
	
	$db->executar($sql);
	$db->commit();
	
	$al = array("alert"=>"Dados gravados com sucesso","location"=>"sispacto3.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=atividadesobrigatorias&fpbid=".$dados['fpbid']);
	alertlocation($al);
	
}




function gravarAprendizagemTurma2($dados) {
	global $db;

	if($dados['tpaid']) {

		$sql = "SELECT rfuparcela FROM sispacto3.folhapagamentouniversidade WHERE fpbid='".$dados['fpbid']."' AND pflcod='".PFL_PROFESSORALFABETIZADOR."' AND uncid='".$_SESSION['sispacto3']['professoralfabetizador']['uncid']."'";
		$rfuparcela = $db->pegaUm($sql);

		foreach($dados['tpaid'] as $tpaid) {
			$sql = "UPDATE sispacto3.turmasprofessoresalfabetizadores SET
					tpatotalmeninos{$rfuparcela}=".((is_numeric($dados['tpatotalmeninos'][$tpaid]))?"'".$dados['tpatotalmeninos'][$tpaid]."'":"NULL").",
					tpatotalmeninas{$rfuparcela}=".((is_numeric($dados['tpatotalmeninas'][$tpaid]))?"'".$dados['tpatotalmeninas'][$tpaid]."'":"NULL").",
							tpafaixaetariaabaixo6anos{$rfuparcela}=".((is_numeric($dados['tpafaixaetariaabaixo6anos'][$tpaid]))?"'".$dados['tpafaixaetariaabaixo6anos'][$tpaid]."'":"NULL").",
							  tpafaixaetaria6anos{$rfuparcela}=".((is_numeric($dados['tpafaixaetaria6anos'][$tpaid]))?"'".$dados['tpafaixaetaria6anos'][$tpaid]."'":"NULL").",
							  		tpafaixaetaria7anos{$rfuparcela}=".((is_numeric($dados['tpafaixaetaria7anos'][$tpaid]))?"'".$dados['tpafaixaetaria7anos'][$tpaid]."'":"NULL").",
							  				tpafaixaetaria8anos{$rfuparcela}=".((is_numeric($dados['tpafaixaetaria8anos'][$tpaid]))?"'".$dados['tpafaixaetaria8anos'][$tpaid]."'":"NULL").",
							  				tpafaixaetaria9anos{$rfuparcela}=".((is_numeric($dados['tpafaixaetaria9anos'][$tpaid]))?"'".$dados['tpafaixaetaria9anos'][$tpaid]."'":"NULL").",
							  						tpafaixaetaria10anos{$rfuparcela}=".((is_numeric($dados['tpafaixaetaria10anos'][$tpaid]))?"'".$dados['tpafaixaetaria10anos'][$tpaid]."'":"NULL").",
							  								tpafaixaetaria11anos{$rfuparcela}=".((is_numeric($dados['tpafaixaetaria11anos'][$tpaid]))?"'".$dados['tpafaixaetaria11anos'][$tpaid]."'":"NULL").",
							  										tpafaixaetariaacima11anos{$rfuparcela}=".((is_numeric($dados['tpafaixaetariaacima11anos'][$tpaid]))?"'".$dados['tpafaixaetariaacima11anos'][$tpaid]."'":"NULL")."
							  										WHERE tpaid='".$tpaid."'";
							  											
							  										$db->executar($sql);
		}
	}

	$db->commit();

	if($dados['catid']) {

		foreach($dados['catid'] as $tpaid => $arr) {
			
			foreach($arr as $catid) {
		
				$sql = "SELECT actid as id_aprendizagem FROM sispacto3.aprendizagemconhecimentoturma2 WHERE tpaid='".$tpaid."' AND catid='".$catid."'";
				$id_aprendizagem = $db->pegaUm($sql);

				if($id_aprendizagem) {
					$sql_e[] = "UPDATE sispacto3.aprendizagemconhecimentoturma2 SET actsim=".(($dados['actsim'][$tpaid][$catid])?"'".$dados['actsim'][$tpaid][$catid]."'":"NULL").",
							actparcialmente=".(($dados['actparcialmente'][$tpaid][$catid])?"'".$dados['actparcialmente'][$tpaid][$catid]."'":"NULL").",
																			   actnao=".(($dados['actnao'][$tpaid][$catid])?"'".$dados['actnao'][$tpaid][$catid]."'":"NULL")."
							WHERE actid='".$id_aprendizagem."';";
					
				} else {
						
					$sql = "SELECT count(*) as qtd FROM sispacto3.aprendizagemconhecimentoturma2 WHERE tpaid='".$tpaid."' AND catid='".$catid."'";
					$qtd = $db->pegaUm($sql);
						
					if(!$qtd) {

						$sql_e[] = "INSERT INTO sispacto3.aprendizagemconhecimentoturma2(
			            		catid, tpaid, actsim, actparcialmente, actnao)
								SELECT '".$catid."',
									   '".$tpaid."',
									   ".(($dados['actsim'][$tpaid][$catid])?"'".$dados['actsim'][$tpaid][$catid]."'":"NULL").",
									   ".(($dados['actparcialmente'][$tpaid][$catid])?"'".$dados['actparcialmente'][$tpaid][$catid]."'":"NULL").",
									   ".(($dados['actnao'][$tpaid][$catid])?"'".$dados['actnao'][$tpaid][$catid]."'":"NULL")."
									   		FROM coalesce((SELECT actid::text FROM sispacto3.aprendizagemconhecimentoturma2 WHERE catid='".$catid."' AND tpaid='".$tpaid."'),NULL) as foo WHERE foo IS NULL;";


					}
						
						
				}
		
			}
		}
		
		if($sql_e) {
				
			$db->executar(implode("",$sql_e));
			$db->commit();
			
		}
		
	}

	$al = array("alert"=>"Dados gravados com sucesso","location"=>"sispacto3.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=atividadesobrigatorias&fpbid=".$dados['fpbid']."&cattipo=".$dados['cattipo']);
	alertlocation($al);

}

?>