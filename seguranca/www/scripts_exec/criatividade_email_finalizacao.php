<?php
set_time_limit(0);

define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );

$_REQUEST['baselogin']  = "simec_espelho_producao";//simec_desenvolvimento
// $_REQUEST['baselogin']  = "simec_desenvolvimento";//simec_desenvolvimento

// carrega as fun��es gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
// require_once "../../global/config.inc";

require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";

//eduardo - envio SMS pendecias de obras - PAR
//http://simec-local/seguranca/scripts_exec/par_enviaSMS_pendenciasAtualizacaoObras.php
// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';
$_SESSION['sisid'] = 98;

$db = new cls_banco();

$sql = "select queid, quesituacao, parnome, parcpf, paremail, parsexo
        from criatividadeeducacao.questionario  q
            inner join criatividadeeducacao.participante p on p.parid = q.parid
        where coalesce(paremail, '') != ''
        and quesituacao = 'A'
        ";

$dados = $db->carregar($sql);
$dados = $dados ? $dados : array();

foreach ($dados as $dado) {

    $saudacao = $dado['parsexo'] == 'F' ? 'Prezada Sra.' : 'Prezado Sr.';

    $conteudo = "<pre>{$saudacao} {$dado['parnome']},

Percebemos que voc� iniciou sua inscri��o na Chamada P�blica Inova��o e Criatividade na Educa��o B�sica (http://siscriatividade.mec.gov.br), mas n�o a completou.
Alertamos que as tr�s p�ginas do formul�rio devem ser preenchidas para que sua inscri��o seja analisada. Ao final, clique em FINALIZAR.

Atenciosamente,

A Equipe do
Inova��o e Criatividade na Educa��o B�sica
</pre>";

    $assunto = "Inscri��o na Chamada P�blica Inova��o e Criatividade na Educa��o B�sica";

    $destinatario = trim($dado['paremail']);

    simec_email(array('nome'=>'Inova��o e Criatividade na Educa��o B�sica', 'email'=>$_SESSION['email_sistema']), $destinatario, $assunto, $conteudo);
}

echo 'FIM';