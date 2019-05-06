<?
/* configurações */
ini_set("memory_limit", "1024M");
set_time_limit(300);
define("COD_FNDE",26298); // código da entidade do FNDE
define("COD_DIRETA",26101); // código da entidade da Direta
define("EXEID_FNDE",42); // código do executor FNDE
define("ENTID_FNDE",391334); // código da entidade do FNDE

if( $_POST ){
	extract( $_POST );
}
/* FIM configurações */

/*
 * Registro tipo 05.:  Cadastramento de Sub Ação
$sql = "SELECT sbacod, to_char(sbadata, 'ddmmYYYY') as sbadata, sbadsc, sbatitulo, eqdcod, sbaobra, prgcod||'.'||acacod||'.'||unicod||'.'||loccod as codigo FROM monitora.pi_subacao AS sba 
		INNER JOIN monitora.pi_subacaodotacao AS sbd ON sbd.sbaid = sba.sbaid 
		INNER JOIN monitora.ptres AS ptr ON ptr.ptrid = sbd.ptrid 
		INNER JOIN planejamento.enquadramento_despesa AS eqd ON eqd.eqdid = sba.eqdid
		INNER JOIN monitora.subacaounidade sbu ON sbu.sbaid = sba.sbaid  
		WHERE sba.sbastatus = 'A' AND ptr.unicod ='".COD_FNDE."' ";
 */

$sql = "SELECT 
			sbacod, to_char(sbadata, 'ddmmYYYY') as sbadata, 
			rtrim(replace(sbadsc,';','')) as sbadsc, 
			rtrim(sbatitulo) as sbatitulo, eqdcod, sbaobras, prgcod||'.'||acacod||'.'||ptr.unicod||'.'||loccod as codigo 
		FROM monitora.pi_subacao AS sba 
		INNER JOIN monitora.pi_subacaodotacao AS sbd ON sbd.sbaid = sba.sbaid 
		INNER JOIN monitora.pi_planointerno AS pli ON pli.sbaid = sba.sbaid
		INNER JOIN monitora.ptres AS ptr ON ptr.ptrid = sbd.ptrid 
		INNER JOIN monitora.pi_subacaoenquadramento sae ON sae.sbaid = sba.sbaid
		INNER JOIN planejamento.enquadramento_despesa as eqd ON eqd.eqdid = sae.eqdid 
		INNER JOIN monitora.pi_subacaounidade sbu ON sbu.sbaid = sba.sbaid  
		WHERE sba.sbastatus = 'A' AND ptr.unicod ='".COD_FNDE."' and ptr.ptrano = '".$_SESSION['exercicio']."'";
if( $plicod != "" ){
	$sql .=" AND sbacod = '".$plicod."'";
}
if( $plititulo != "" ){
	$sql .=" AND sbatitulo ILIKE '%".$plititulo."%'";
}
if( $unidade != "" ){
	$sql .=" AND ptr.unicod = '".$unidade."'";
}
if( $enquadramento != "" ){
	$sql .=" AND eqd.eqdid = '".$enquadramento."'";
}
if( $nivel ){
	$sql .=" AND pli.secid = '".$nivel."'";
}
if( $apropriacao ){
	$sql .=" AND pli.capid = '".$apropriacao."'";
}
if( $modalidade ){
	$sql .=" AND pli.mdeid = '".$modalidade."'";
}
if( $plisituacao ){
	$sql .=" AND pli.plisituacao = '".$plisituacao."'";
}


$dadostipo5 = $db->carregar($sql); 

if($dadostipo5[0]) {
	foreach($dadostipo5 as $tipo5) {
		$csv .= "05;".$tipo5['sbacod'].";".$tipo5['sbadata'].";".substr($tipo5['sbadsc'],0,250).";".substr($tipo5['sbatitulo'],0,25).";".$tipo5['eqdcod'].";".(($tipo5['sbaobra']=="t")?"1":"0").";".$tipo5['codigo']."\n"; 
	}
}

/*
 * Registro tipo 10.:  Vinculação Sub Ação x PTRES
$sql = "SELECT ptr.ptres, to_char(sba.sbadata, 'ddmmYYYY') as sbadata, sba.sbacod, sadvalor, prgcod||'.'||acacod||'.'||unicod||'.'||loccod as codigo FROM monitora.subacao AS sba 
		INNER JOIN monitora.subacaodotacao AS sbd ON sbd.sbaid = sba.sbaid
		INNER JOIN monitora.ptres AS ptr ON ptr.ptrid = sbd.ptrid 
		INNER JOIN monitora.subacaounidade sbu ON sbu.sbaid = sba.sbaid 
		WHERE sba.sbastatus = 'A' AND ptr.unicod ='".COD_FNDE."' ";
 */

$sql = "SELECT 
			ptr.ptres, to_char(sba.sbadata, 'ddmmYYYY') as sbadata, sba.sbacod, sadvalor, prgcod||'.'||acacod||'.'||ptr.unicod||'.'||loccod as codigo 
		FROM monitora.pi_subacao AS sba 
		INNER JOIN monitora.pi_subacaodotacao AS sbd ON sbd.sbaid = sba.sbaid
		INNER JOIN monitora.pi_planointerno AS pli ON pli.sbaid = sba.sbaid
		INNER JOIN monitora.ptres AS ptr ON ptr.ptrid = sbd.ptrid 
		INNER JOIN monitora.pi_subacaoenquadramento sae ON sae.sbaid = sba.sbaid
		INNER JOIN planejamento.enquadramento_despesa as eqd ON eqd.eqdid = sae.eqdid 
		INNER JOIN monitora.pi_subacaounidade sbu ON sbu.sbaid = sba.sbaid 
		WHERE sba.sbastatus = 'A'  and ptr.ptrano = '".$_SESSION['exercicio']."' ";
if( $plicod != "" ){
	$sql .=" AND ptr.ptres = '".$plicod."'";
}
if( $plititulo != "" ){
	$sql .=" AND sba.sbatitulo ILIKE '%".$plititulo."%'";
}
if( $unidade != "" ){
	$sql .=" AND ptr.unicod = '".$unidade."'";
}
if( $enquadramento != "" ){
	$sql .=" AND eqd.eqdid = '".$enquadramento."'";
}
if( $nivel ){
	$sql .=" AND pli.secid = '".$nivel."'";
}
if( $apropriacao ){
	$sql .=" AND pli.capid = '".$apropriacao."'";
}
if( $modalidade ){
	$sql .=" AND pli.mdeid = '".$modalidade."'";
}
if( $plisituacao ){
	$sql .=" AND pli.plisituacao = '".$plisituacao."'";
}
$dadostipo10 = $db->carregar($sql);

if($dadostipo10[0]) {
	foreach($dadostipo10 as $tipo10) {
		$csv .= "10;".$tipo10['ptres'].";".$tipo10['sbadata'].";".$tipo10['sbacod'].";".(($tipo10['sadvalor'])?str_pad(str_replace(".","",$tipo10['sadvalor']), 15, "0", STR_PAD_LEFT):"000000000000000").";".$tipo10['codigo']."\n"; 
	}
}

/*
 * Registro tipo 20.:  Cadastramento de PI
$sql = "SELECT plicod, to_char(plidata, 'ddmmYYYY') as plidata, ptrano, plititulo, plidsc, plisituacao, prgcod||'.'||acacod||'.'||unicod||'.'||loccod as codigo FROM monitora.planointerno AS pli
		INNER JOIN monitora.subacao AS sba ON sba.sbaid = pli.sbaid 
		INNER JOIN monitora.subacaounidade sbu ON sbu.sbaid = sba.sbaid
		INNER JOIN monitora.ptres AS ptr ON ptr.ptres = pli.pliptres 
		WHERE ptr.unicod ='".COD_FNDE."' AND pli.plistatus = 'A' and (plisituacao = 'C' OR plisituacao = 'S') AND ptrano='".$_SESSION['exercicio']."' 
		GROUP BY plicod, plidata, ptrano, plititulo, plidsc, plisituacao, codigo  
		ORDER BY ptrano";
 */

$sql = "SELECT 
			plicod, to_char(plidata, 'ddmmYYYY') as plidata, ptrano, rtrim(plititulo), trim(replace(plidsc,';','')) as plidsc, 
			rtrim(plisituacao) as plisituacao, 
			prgcod||'.'||acacod||'.'||ptr.unicod||'.'||loccod as codigo 
		FROM monitora.pi_planointerno AS pli
		INNER JOIN monitora.pi_subacao AS sba ON sba.sbaid = pli.sbaid 
		INNER JOIN monitora.pi_subacaoenquadramento sae ON sae.sbaid = sba.sbaid
		INNER JOIN planejamento.enquadramento_despesa as eqd ON eqd.eqdid = sae.eqdid 
		INNER JOIN monitora.pi_subacaounidade sbu ON sbu.sbaid = sba.sbaid
		INNER JOIN monitora.pi_planointernoptres pip ON pip.pliid = pli.pliid
		INNER JOIN monitora.ptres AS ptr ON ptr.ptrid = pip.ptrid
		WHERE  pli.plistatus = 'A' and ( 
				(plisituacao = 'A' OR plisituacao = 'C' OR plisituacao = 'S' OR plisituacao = 'E' OR plisituacao = 'P') or 
				(pli.pliid in ( select pliid from monitora.pi_planointernohistorico p	where pihsituacao = 'H' group by pliid having count(*) > 1 ) )	) AND ptrano='".$_SESSION['exercicio']."'
		";
if( $plicod != "" ){
	$sql .=" AND plicod = '".$plicod."'";
}	
if( $plititulo != "" ){
	$sql .=" AND plititulo ILIKE '%".$plititulo."%'";
}
if( $unidade != "" ){
	$sql .=" AND ptr.unicod = '".$unidade."'";
}
if( $enquadramento != "" ){
	$sql .=" AND eqd.eqdid = '".$enquadramento."'";
}
if( $nivel ){
	$sql .=" AND pli.secid = '".$nivel."'";
}
if( $apropriacao ){
	$sql .=" AND pli.capid = '".$apropriacao."'";
}
if( $modalidade ){
	$sql .=" AND pli.mdeid = '".$modalidade."'";
}
if( $plisituacao ){
	$sql .=" AND plisituacao = '".$plisituacao."'";
}
			$sql .= " GROUP BY plicod, plidata, ptrano, plititulo, plidsc, plisituacao, codigo  
		ORDER BY ptrano";
			
$dadostipo20 = $db->carregar($sql);

if($dadostipo20[0]) {
	foreach($dadostipo20 as $tipo20) {
		$csv .= "20;".$tipo20['plicod'].";".$tipo20['plidata'].";".$tipo20['ptrano'].";".preg_replace("/\r\n|\n|\r/", "<br>", addslashes($tipo20['plititulo'])).";".preg_replace("/\r\n|\n|\r/", "<br>", addslashes($tipo20['plidsc'])).";".$tipo20['plisituacao'].";".$tipo20['codigo']."\n"; 
	}
}
/*
 * Registro tipo 30.:  Vinculação de PI x Ptres
$sql = "SELECT plicod, to_char(plidata, 'ddmmYYYY') as plidata, pliptres, plivalor, prgcod||'.'||acacod||'.'||unicod||'.'||loccod as codigo FROM monitora.planointerno AS pli 
		INNER JOIN monitora.acao aca ON aca.acaid = pli.acaid  
		INNER JOIN monitora.subacao AS sba ON sba.sbaid = pli.sbaid 
		INNER JOIN monitora.subacaounidade sbu ON sbu.sbaid = sba.sbaid 
		WHERE ( aca.unicod = '".COD_FNDE."' or aca.unicod = '".COD_DIRETA."' ) AND pli.plistatus = 'A' and (plisituacao = 'C' OR plisituacao = 'S') ";
 */

$sql = "SELECT 
			plicod, to_char(plidata, 'ddmmYYYY') as plidata, rtrim(ptres.ptres) as pliptres, 
			pip.pipvalor as plivalor, aca.prgcod||'.'||aca.acacod||'.'||aca.unicod||'.'||aca.loccod as codigo 
		FROM monitora.pi_planointerno AS pli 
		INNER JOIN monitora.pi_planointernoptres pip ON pip.pliid = pli.pliid 
		INNER JOIN monitora.ptres ptres ON ptres.ptrid = pip.ptrid 
		INNER JOIN monitora.acao aca ON aca.acaid = ptres.acaid 
		INNER JOIN monitora.pi_subacao AS sba ON sba.sbaid = pli.sbaid 
		INNER JOIN monitora.pi_subacaoenquadramento sae ON sae.sbaid = sba.sbaid
		INNER JOIN planejamento.enquadramento_despesa as eqd ON eqd.eqdid = sae.eqdid 
		INNER JOIN monitora.pi_subacaounidade sbu ON sbu.sbaid = sba.sbaid 
		WHERE pli.pliano = '".$_SESSION['exercicio']."' and ( aca.unicod = '".COD_FNDE."' or aca.unicod = '".COD_DIRETA."' ) AND pli.plistatus = 'A' and ( 
				(plisituacao = 'A' OR plisituacao = 'C' OR plisituacao = 'S' OR plisituacao = 'E' OR plisituacao = 'P') or 
				(pli.pliid in ( select pliid from monitora.pi_planointernohistorico p	where pihsituacao = 'H' group by pliid having count(*) > 1 ) )	)";
if( $plicod != "" ){
	$sql .=" AND plicod = '".$plicod."'";
}	
if( $enquadramento != "" ){
	$sql .=" AND eqd.eqdid = '".$enquadramento."'";
}
if( $nivel ){
	$sql .=" AND pli.secid = '".$nivel."'";
}
if( $apropriacao ){
	$sql .=" AND pli.capid = '".$apropriacao."'";
}
if( $modalidade ){
	$sql .=" AND pli.mdeid = '".$modalidade."'";
}
if( $plisituacao ){
	$sql .=" AND pli.plisituacao = '".$plisituacao."'";
}
$dadostipo30 = $db->carregar($sql);

if($dadostipo30[0]) {
	foreach($dadostipo30 as $tipo30) {
		$csv .= "30;".$tipo30['plicod'].";".$tipo30['plidata'].";".$tipo30['pliptres'].";".(($tipo30['plivalor'])?str_pad(str_replace(".","",$tipo30['plivalor']), 15, "0", STR_PAD_LEFT):"000000000000000").";".$tipo30['codigo']."\n"; 
	}
}
/*
 * Registro tipo 99.:  Totalizador
 */
if($csv) {
	$csv .= "99;".str_pad((count($dadostipo5)+count($dadostipo10)+count($dadostipo20)+count($dadostipo30)), 8, "0", STR_PAD_LEFT)."\n"; 
}

header("Content-type: application/octet-stream");
header("Content-Disposition: attachment; filename=\"PI_".date("dmY_hi").".csv \"");
echo $csv;
exit;


?>