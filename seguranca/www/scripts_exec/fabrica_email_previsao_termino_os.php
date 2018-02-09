<?php

$_REQUEST['baselogin'] = "simec_espelho_producao";
$simecPath             = realpath( dirname( __FILE__ ) . '/../../../' );

// carrega as fun��es gerais
include_once "/var/www/simec/global/config.inc";
// include_once $simecPath . '/global/config.inc';

include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "www/fabrica/_constantes.php";

// carrega as fun��es EMAIL
require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';

error_reporting( E_ALL &~E_NOTICE );
ini_set( 'display_errors', 1 );
// abre conex�o com o servidor de banco de dados
$db = new cls_banco();


$sqlPrepostoSquadra = "SELECT usu.usucpf, usu.usunome, usu.usuemail
                    FROM seguranca.usuario usu
                    INNER JOIN seguranca.perfilusuario pu
                        ON usu.usucpf = pu.usucpf	
                    INNER JOIN seguranca.perfil per
                        ON per.pflcod = pu.pflcod
                    WHERE per.pflcod = " . PERFIL_PREPOSTO . "  
                    ORDER BY pu.pflcod;";

$sqlPrepostoEficacia = "SELECT usu.usucpf, usu.usunome, usu.usuemail
                    FROM seguranca.usuario usu
                    INNER JOIN seguranca.perfilusuario pu
                        ON usu.usucpf = pu.usucpf	
                    INNER JOIN seguranca.perfil per
                        ON per.pflcod = pu.pflcod
                    WHERE per.pflcod = " . PERFIL_CONTAGEM_PF . "
                    ORDER BY pu.pflcod;";


$arrPrepostoSquadra = $db->carregar( $sqlPrepostoSquadra );
$arrPrepostoEficacia = $db->carregar( $sqlPrepostoEficacia );

$sqlSquadraOS = "SELECT os.odsid, os.scsid, os.odsdetalhamento 
                    ,  to_char(os.odsdtprevinicio, 'dd/mm/YYYY') as odsdtprevinicio
                    ,  to_char(os.odsdtprevtermino, 'dd/mm/YYYY') as odsdtprevtermino
            FROM fabrica.ordemservico os
            LEFT JOIN ( 
                    select odsidpai,  count(odsid) as contador 
                    from fabrica.ordemservico 
                    group by odsidpai 
                    ) osp 
                ON osp.odsidpai = os.odsid
            LEFT JOIN fabrica.tipoordemservico tos 
                ON tos.tosid = os.tosid
            LEFT JOIN fabrica.solicitacaoservico ss 
                ON ss.scsid = os.scsid
            LEFT JOIN workflow.documento d 
                ON d.docid = os.docid
            LEFT JOIN workflow.estadodocumento ed 
                ON ed.esdid = d.esdid
            LEFT JOIN workflow.documento d2 
                ON d2.docid = os.docidpf
            LEFT JOIN workflow.estadodocumento ed2 
                ON ed2.esdid = d2.esdid
            LEFT JOIN demandas.sistemadetalhe sid 
                ON sid.sidid = ss.sidid
            LEFT JOIN  workflow.documento as wkd 
                ON wkd.docid = ss.docid
            LEFT JOIN  fabrica.analisesolicitacao as fas 
                ON fas.scsid = ss.scsid
            WHERE tos.tosdsc = 'Gen�rico'
            AND ( date_part('day', odsdtprevtermino)  - date_part('day', current_date ) )  BETWEEN 0 AND 2
            AND date_part('year', odsdtprevtermino) = date_part('year', current_date)
            ORDER BY os.odsid";

$sqlEficaciaOS = "SELECT os.odsid, os.scsid, os.odsdetalhamento 
                    ,  to_char(os.odsdtprevinicio, 'dd/mm/YYYY') as odsdtprevinicio
                    ,  to_char(os.odsdtprevtermino, 'dd/mm/YYYY') as odsdtprevtermino
                    , tos.tosdsc
            FROM fabrica.ordemservico os
            LEFT JOIN ( 
                    select odsidpai,  count(odsid) as contador 
                    from fabrica.ordemservico 
                    group by odsidpai 
                    ) osp 
                ON osp.odsidpai = os.odsid
            LEFT JOIN fabrica.tipoordemservico tos 
                ON tos.tosid = os.tosid
            LEFT JOIN fabrica.solicitacaoservico ss 
                ON ss.scsid = os.scsid
            LEFT JOIN workflow.documento d 
                ON d.docid = os.docid
            LEFT JOIN workflow.estadodocumento ed 
                ON ed.esdid = d.esdid
            LEFT JOIN workflow.documento d2 
                ON d2.docid = os.docidpf
            LEFT JOIN workflow.estadodocumento ed2 
                ON ed2.esdid = d2.esdid
            LEFT JOIN demandas.sistemadetalhe sid 
                ON sid.sidid = ss.sidid
            LEFT JOIN  workflow.documento as wkd 
                ON wkd.docid = ss.docid
            LEFT JOIN  fabrica.analisesolicitacao as fas 
                ON fas.scsid = ss.scsid
            WHERE tos.tosdsc <> 'Gen�rico'
            AND ( date_part('day', odsdtprevtermino)  - date_part('day', current_date ) )  BETWEEN 0 AND 2
            AND date_part('year', odsdtprevtermino) = date_part('year', current_date)
            ORDER BY os.odsid";

$arrOrdemServicoSquadra     = $db->carregar($sqlSquadraOS);
$arrOrdemServicoEficacia    = $db->carregar($sqlEficaciaOS);

$textoEmail = '<p><strong>Listagem de Ordem de Servi�o</strong><p>';
$textoEmail .= '<p>Prezado(a) Preposto(a),</p>';
$textoEmail .= '<p>As OS relacionadas abaixo, possuem data de encerramento previsto para os pr�ximos 2(dois) dias.</p>';


if ( count( $arrOrdemServicoSquadra ) > 0 )
{
    $listagemOS = '';
    foreach( $arrOrdemServicoSquadra as $ordemServicoSquadra ){
        $osDetalhamento = nl2br($ordemServicoSquadra['odsdetalhamento']);
        $listagemOS .= '<tr>';
        $listagemOS .= "<td>{$ordemServicoSquadra['odsid']}</td>";
        $listagemOS .= "<td>{$ordemServicoSquadra['scsid']}</td>";
        $listagemOS .= "<td>{$osDetalhamento}</td>";
        $listagemOS .= "<td>{$ordemServicoSquadra['odsdtprevinicio']}</td>";
        $listagemOS .= "<td>{$ordemServicoSquadra['odsdtprevtermino']}</td>";
        $listagemOS .= '</tr>';
    }
    
    $counteudoEmail = '<table style="width: 100%" border="1">
                        <thead>
                            <th style="width: 10%">N� OS</th>
                            <th style="width: 10%">N� SS</th>
                            <th style="width: 60%">Detalhamento</th>
                            <th style="width: 10%">Prev. In�cio</th>
                            <th style="width: 10%">Prev. T�rmino</th>
                        </thead>
                        <tbody>'. $listagemOS .'</tbody>
                       <table>';
    
    $counteudoEmail = $textoEmail . $counteudoEmail;
    
    $mailer = new PHPMailer();
    $mailer->Host       = "";
    $mailer->Mailer     = "smtp";
    $mailer->FromName   = SIGLA_SISTEMA. " - F�brica";
    $mailer->From       = "noreply@mec.gov.br";
    $mailer->Subject    = SIGLA_SISTEMA. " - F�brica - Aviso de encerramento de Ordem de Servi�o";
    
    $mailer->AddAddress( $_SESSION['email_sistema'], SIGLA_SISTEMA );
    
    $mailer->Body = $counteudoEmail ;

    foreach( $arrPrepostoSquadra as $prepostoEmpresa )
    {
        $mailer->AddAddress( $prepostoEmpresa['usuemail'], $prepostoEmpresa['usunome'] );
    }


    $mailer->IsHTML( true );
    if( !$mailer->Send()){
        echo "N�o foi poss�vel enviar o e-mail para os prepostos SQUADRA\n";
        exec("echo \"N�o foi poss�vel enviar o e-mail para os prepostos SQUADRA\" >> /tmp/simec_email_fabrica.log");
        exit;
    }
    
    echo "E-mail enviado com sucesso para o preposto SQUADRA\n";
    exec("echo \"E-mail enviado com sucesso para o preposto SQUADRA\" >> /tmp/simec_email_fabrica.log");
}

if ( count( $arrOrdemServicoEficacia ) > 0 )
{
    
    $listagemOS = '';
    foreach( $arrOrdemServicoEficacia as $ordemServicoEficacia ){
        $osDetalhamento = nl2br($ordemServicoEficacia['odsdetalhamento']);
        $listagemOS .= '<tr>';
        $listagemOS .= "<td>{$ordemServicoEficacia['odsid']}</td>";
        $listagemOS .= "<td>{$ordemServicoEficacia['scsid']}</td>";
        $listagemOS .= "<td>{$osDetalhamento}</td>";
        $listagemOS .= "<td>{$ordemServicoEficacia['odsdtprevinicio']}</td>";
        $listagemOS .= "<td>{$ordemServicoEficacia['odsdtprevtermino']}</td>";
        $listagemOS .= "<td>{$ordemServicoEficacia['tosdsc']}</td>";
        $listagemOS .= '</tr>';
    }
    
    $counteudoEmail = '<table style="width: 100%" border="1">
                        <thead>
                            <th style="width: 5%">N� OS</th>
                            <th style="width: 5%">N� SS</th>
                            <th style="width: 50%">Detalhamento</th>
                            <th style="width: 10%">Prev. In�cio</th>
                            <th style="width: 10%">Prev. T�rmino</th>
                            <th style="width: 20%">Tipo de OS</th>
                        </thead>
                        <tbody>'. $listagemOS .'</tbody>
                       <table>';
    
    $counteudoEmail = $textoEmail . $counteudoEmail;
    
    $mailer = new PHPMailer();
    $mailer->Host       = "";
    $mailer->Mailer     = "smtp";
    $mailer->FromName   = SIGLA_SISTEMA. " - F�brica";
    $mailer->From       = "noreply@mec.gov.br";
    $mailer->Subject    = SIGLA_SISTEMA. " - F�brica - Aviso de encerramento de Ordem de Servi�o";
    
    $mailer->AddAddress( $_SESSION['email_sistema'], SIGLA_SISTEMA );
    
    $mailer->Body = $counteudoEmail ;

    foreach( $arrPrepostoEficacia as $prepostoEmpresa )
    {
        $mailer->AddAddress( $prepostoEmpresa['usuemail'], $prepostoEmpresa['usunome'] );
    }


    $mailer->IsHTML( true );
    if( !$mailer->Send()){
        echo "N�o foi poss�vel enviar o e-mail para os prepostos EFIC�CIA\n";
        exec("echo \"N�o foi poss�vel enviar o e-mail para os prepostos EFIC�CIA\" >> /tmp/simec_email_fabrica.log");
        exit;
    }
    
    echo "E-mail enviado com sucesso para o preposto EFIC�CIA\n";
    exec("echo \"E-mail enviado com sucesso para o preposto EFIC�CIA\" >> /tmp/simec_email_fabrica.log");
}
