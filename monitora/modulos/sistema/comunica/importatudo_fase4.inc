<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:importatudo_fase4.inc
   Finalidade: permitir importar os dados do SIGPLAN fase 4
   */
include APPRAIZ.'includes/class.ADODB_XML.inc';
$adodbXML = new adodb_xml("1.0","utf-8");

print 'acao<br>';
$adodbXML->ImportaXML(&$clientedb, "CargaAcao.xml", "monitora.acao",'ACA','sigplan');
$sql='update monitora.acao set prgid=p.prgid from planejamento.programa p where p.prgcod=acao.prgcod and p.prgano=acao.prgano and acao.prgid is null';
$saida = $db->executar($sql);
$db -> commit();

print 'atualiza ppa acao<br>';
$adodbXML->upgradedb_ppa($dbconnection, "CargaPPAAcao.xml", "monitora.acao",'ACA','ppa');


?>
