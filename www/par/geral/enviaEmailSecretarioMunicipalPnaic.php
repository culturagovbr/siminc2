<?php 

die('N�o foi poss�vel executar a a��o!');

include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";
$db = new cls_banco();

ini_set("memory_limit","2000M");

$sql = "select * from entidade.entidade where entid in (select entid1 from (SELECT
							max(ent.entid) as entid1,
							eed2.muncod
						FROM entidade.entidade ent
							INNER JOIN entidade.funcaoentidade 	fue ON fue.entid = ent.entid AND fue.funid = 15 AND fue.fuestatus = 'A'
							INNER JOIN entidade.funcao 			fun ON fun.funid = fue.funid
							LEFT JOIN entidade.funentassoc 		fea ON fea.fueid = fue.fueid
							LEFT JOIN entidade.entidade         ent2 ON ent2.entid = fea.entid 
							LEFT JOIN entidade.endereco         eed2 ON eed2.entid = ent2.entid 
							LEFT JOIN entidade.funcaoentidade 	fue2 ON fue2.entid = ent2.entid AND fue2.funid = 7 AND fue2.fuestatus = 'A'
							LEFT JOIN entidade.funcao 			fun2 ON fun2.funid = fue2.funid
						WHERE (ent.entstatus = 'A' OR ent.entstatus IS NULL)
						AND eed2.muncod in (select muncod from par.pfadesaoprograma adp 
									inner join par.instrumentounidade inu on inu.inuid = adp.inuid
									where tapid in (13,14))
						group by eed2.muncod) as foo )";


$rs = $db->carregar($sql);

echo "######################### IN�CIO DO SCRIPT ######################### <p>&nbsp;</p>";

$x=1;
foreach($rs as $dados){
	
	echo $x." - E-mail enviado para: ".$dados['entemail']." <br/>";
	
	$arEmail = array($dados['entemail']);	

	$remetente 	= '';
	$assunto	= 'Conclus�o de Ades�o ao Pacto';

	$conteudo	= '
					<p>Prezado(a) Secret�rio(a),</p> 
					
					A sua Secretaria de Educa��o acaba de concluir a ades�o ao Pacto Nacional pela Alfabetiza��o na Idade Certa e �s 
					A��es do Pacto. Em breve, o Minist�rio da Educa��o entrar� em contato para informar as pr�ximas etapas. Para saber 
					mais, acesse o portal do MEC: http://www.mec.gov.br.
				  ';
	
	$cc			= array($_SESSION['email_sistema']);
	$cco		= '';
	$arquivos 	= array();

	enviar_email( $remetente, $arEmail, $assunto, $conteudo, $cc, $cco, $arquivos );
	$x++;
}

echo "<p>&nbsp;</p> ######################### FIM DO SCRIPT #########################";

?>