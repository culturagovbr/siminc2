<?php
/* configura��es */
ini_set("memory_limit", "3000M");
set_time_limit(0);
/* FIM configura��es */

// carrega as fun��es gerais
include_once "/var/www/simec/global/config.inc";
//include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes/Sms.class.inc";

// Pull in the NuSOAP code
require_once APPRAIZ . "/www/webservice/painel/nusoap.php";

// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
    $_SESSION['usucpforigem'] = '00000000191';

// abre conex�o com o servidor de banco de dados
$db = new cls_banco();

require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';

$mensagem = new PHPMailer();
$mensagem->persistencia = $db;
$mensagem->Host         = "localhost";
$mensagem->Mailer       = "smtp";
$mensagem->FromName		= SIGLA_SISTEMA. " Painel - Atualiza��o dos dados";
$mensagem->From 		= $_SESSION['email_sistema'];
$mensagem->Subject      = "Informa��es desatualizadas no Painel de Controle";
$mensagem->IsHTML( true );

$sms = new Sms();

$sql = "SELECT i.indid, i.indnome, i.secid, p.dpediasverificacao
		FROM painel.indicador i 
		INNER JOIN painel.periodicidade p ON i.peridatual = p.perid 
		WHERE i.indstatus = 'A'
		--AND i.indhomologado = true
		AND i.indencerrado = false
		AND i.secid <> 17
		ORDER BY i.secid, i.indid";
$indicadores = $db->carregar($sql);

if(date('l')!='Saturday' && date('l')!='Sunday'){
    if($indicadores[0]) {
        foreach($indicadores as $in) {
            $sql = "SELECT s.indid, '<center>'||to_char(sehdtcoleta, 'dd/mm/YYYY HH24:MI')||'</center>' as data
					FROM painel.indicador i
					LEFT JOIN painel.seriehistorica s ON s.indid = i.indid
					WHERE i.indid = '".$in['indid']."'
					AND ((s.sehdtcoleta IS NOT NULL	AND s.sehstatus <> 'I' AND now() > (SELECT sehdtcoleta + interval '".$in['dpediasverificacao']." day' FROM painel.seriehistorica WHERE indid = '".$in['indid']."' AND sehdtcoleta IS NOT NULL AND sehstatus <> 'I' ORDER BY sehdtcoleta DESC LIMIT 1))
					OR (s.indid IS NULL))
					ORDER BY s.sehdtcoleta DESC
					LIMIT 1";
            $indids = $db->pegaLinha($sql);
            if($indids) {
                $_ATRASO[$in['secid']][] = array("id" => $in['indid'], "nome" => $in['indnome'], "ultdata" => (($indids['indid'])?$indids['data']:"<center>Nunca foi atualizado</center>"));
            }
        }

        if($_ATRASO) {
            foreach($_ATRASO as $secid => $at) {

                $sql = "SELECT
							r.respdddcelular::varchar||r.respcelular::varchar as telefone,
							r.respemail,
							r.respnome,
							s.secdsc
						FROM painel.responsavelsecretaria r
						INNER JOIN painel.secretaria s ON s.secid = r.secid
						WHERE r.respstatus = 'A'
						AND r.secid='".$secid."'";
                $resp = $db->carregar($sql);

                unset($mensagem->to);

                if($resp[0]) {
                    //$contador=0;
                    foreach($resp as $r) {
                        $mensagem->AddAddress($r['respemail'], $r['respnome']);
                        if ($r['secdsc'] == 'FNDE'){
                            $mensagem->AddAddress($_SESSION['email_sistema'], SIGLA_SISTEMA);
                        }
                        if(strlen($r['telefone']) >= 10) {
                            $aCelularEnvio = array('55' . $r['telefone']);
                            //$aCelularEnvio = array('556181221163'); //Vitor Sad
                            //if($contador==0){
                            //    $aCelularEnvio[] = '556181221163'; //Vitor Sad
                            //}
                            $conteudo = 'Existem '.count($at).' indicadores da secretaria '.$r['secdsc'].' sob sua responsabilidade desatualizados, acesse simec.mec.gov.br e proceda a atualiza��o. MEC';

                            $sms->enviarSms($aCelularEnvio, $conteudo, null, 48);
                            //$contador++;
                        }
                    }

                    // enviando email
                    ob_start();
                    $cabecalho = array("ID", "Nome do indicador", "�ltima atualiza��o");
                    $db->monta_lista_simples($at,$cabecalho,1000,5,'N','100%');
                    $dadosserv = ob_get_contents();
                    ob_end_clean();
                    $mensagem->AddAddress($_SESSION['email_sistema'], SIGLA_SISTEMA);
                    $mensagem->IsHTML(true);
                    $mensagem->Body = "<style>table.listagem  {border-bottom:3px solid #DFDFDF;border-collapse:collapse;border-top:2px solid #404040;font-size:11px;padding:3px;font:8pt Arial,verdana;}body {font:12px Arial,verdana;}</style>";
                    $mensagem->Body .= "<p>Prezado(a) <b>".$r['respnome']."</b>,</p>";
                    $mensagem->Body .= "<p>Existem <b>".count($at)."</b> indicadores desatualizados referente a secretaria <b>".$r['secdsc']."</b>. Acesse o <a href=http://simec.mec.gov.br/ target=_blank>SIMEC - Painel</a> (http://simec.mec.gov.br/) e realize as atualiza��es das informa��es.</p>";
                    $mensagem->Body .= "<p>Segue abaixo a lista de indicadores desatualizados:</p>";
                    $mensagem->Body .= $dadosserv;
                    $mensagem->Body .= "<p>Agradecemos a colabora��o,<br/>MEC</p>";

                    $enviosmtp = $mensagem->Send();

                    if($enviosmtp) {
                        $_LOG .= "Email enviado para ".$d['respnome']." <br /> ";
                    } else {
                        $_LOG .= "Problemas para enviar email ".$d['respemail']." <br /> ";
                    }
                    // fim enviando email
                }
            }
        }
    }
}
?>