<?php
ini_set( 'display_errors', 1 );
ini_set("memory_limit", "9024M");
ini_set("default_socket_timeout", "70000000");

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));

$_REQUEST['baselogin'] = "simec_espelho_producao";//simec_desenvolvimento
// carrega as fun��es gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/human_gateway_client_api/HumanClientMain.php";

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';
$_SESSION['sisid'] = '147';

$db = new cls_banco();

include_once APPRAIZ . 'www/obras2/_constantes.php';
include_once APPRAIZ . 'www/obras2/_funcoes.php';
include_once APPRAIZ . 'www/obras2/_componentes.php';
include_once APPRAIZ . "www/autoload.php";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/Obras.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/DestinatarioEmail.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/AnexoEmail.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/Email.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/ContatosObra.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/Restricao.class.inc";
include_once APPRAIZ . "includes/classes/dateTime.inc";
require_once APPRAIZ . "includes/classes/entidades.class.inc";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include_once APPRAIZ . "includes/dompdf/dompdf_config.inc.php";

if(!isset($_GET['limit']))
    exit;

$email = new Email();
$data = new Data();
$data = $data->formataData($data->dataAtual(), 'Bras�lia, DD de mesTextual de YYYY.');

$conteudo = '
                        <html>
                            <head>
                                <title></title>
                            </head>
                            <body>
                                <table style="width: 100%;">
                                    <thead>
                                        <tr>
                                            <td style="text-align: center; font-size: 12px;">
                                                <p><img  src="data:image/png;base64,' . $email->getBrasao() . '" width="70"/><br/>
                                                <b>MINIST�RIO DA EDUCA��O</b><br/>
                                                FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCA��O - FNDE<br/>
                                                DIRETORIA DE GEST�O, ARTICULA��O E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                                COORDENA��O GERAL DE IMPLEMENTA��O E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                                SBS Q.2 Bloco F Edif�cio FNDE - 70.070-929 - Bras�lia, DF - E-mail: monitoramento.obras@fnde.gov.br<br/>
                                            </td>
                                        </tr>
                                    </thead>
                                    <tbody>
                                        <tr>
                                            <td style=" text-align:justify">
                                                <p style="text-align: right;">' . $data . '</p>
                                                <p>Assunto: <b>Informa��o sobre solicita��o e deferimento de repasse de parcelas.</b></p>

                                                <p>Senhor(a) Prefeito(a),</p>

                                                <p>1. Informamos que o art. 10, da Resolu��o CD/FNDE n� 13, de 08 de junho de 2012, que estabelece os crit�rios de transfer�ncia de recursos para execu��o das obras no �mbito do PAC 2, foi alterado pela Resolu��o CD/FNDE n� 07, de 05 de agosto de 2015, passando a ter a seguinte reda��o:</p>

                                                <p><i>Art. 10�. Os recursos ser�o transferidos em parcelas, de acordo com a execu��o da obra, sendo a primeira no montante de at� 15%, ap�s inser��o da ordem de servi�o de in�cio de execu��o da obra, no Sistema Integrado de Monitoramento Execu��o e Controle do Minist�rio da Educa��o - Simec, m�dulo Obras 2.0.</i></p>

                                                <p><i>Par�grafo �nico. As demais parcelas ser�o transferidas ap�s a aferi��o da evolu��o f�sica da obra, comprovada mediante o relat�rio de vistoria inserido no Sistema Integrado de Monitoramento Execu��o e Controle do Minist�rio da Educa��o - Simec, m�dulo Obras.2.0, e aprovado pela equipe t�cnica do FNDE.</i></p>

                                                <p>2. Em raz�o das novas regras de transfer�ncia de recursos aos munic�pios, estados e Distrito Federal, o FNDE, com escopo de aprimorar o processo de integra��o entre os entes federados e esta Autarquia, criou ferramenta no m�dulo Obras 2.0 (SIMEC), na qual ocorrer� a solicita��o do desembolso, bem como o acompanhamento de todo o procedimento at� a delibera��o do pedido. Outrossim, � importante que o ente observe, desde j�, as respectivas orienta��es  de modo a cumpri-las no transcorrer das etapas, at� o repasse final do recurso. A prop�sito, segue, abaixo, as correspondentes orienta��es:</p>

                                                <p style="margin:0 0 0 40px">1�) A solicita��o de libera��o de parcelas passa a depender do preenchimento completo e obrigat�rio dos documentos solicitados nas abas "Contrata��o", "Cronograma", "Vistorias" e "Execu��o Or�ament�ria" do Simec - Obras 2.0 (Vide Anexo I).</p>

                                                <p style="margin:0 0 0 40px">2�) Caso a solicita��o de libera��o das parcelas comporte evolu��o f�sica de obra inferior a 10% do percentual repassado na �ltima libera��o, dever� ser apresentada justificativa pelos Munic�pios, Estados e Distrito Federal (atrav�s de boletim de medi��o, verifica��o de saldo banc�rio, dentre outros);</p>

                                                <p style="margin:0 0 0 40px">3�) O cronograma de execu��o da obra deve estar atualizado em todas as suas etapas. (vide Anexo I).</p>

                                                <p style="margin:0 0 0 40px">4�) O boletim de medi��o dos servi�os executados deve ser compat�vel com o percentual solicitado na libera��o da parcela, podendo ser apresentado, nesse caso, o boletim de medi��o acumulada (vide Anexo I).</p>

                                                <p style="margin:0 0 0 40px">5�) Para que a solicita��o de libera��o de parcela seja submetida � an�lise dos t�cnicos do FNDE, todas obras pactuadas com os Munic�pios, Estados e Distrito Federal devem apresentar seus dados atualizados no Simec - Obras 2.0, ou seja, com vistorias inseridas h� menos de 60 dias;</p>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td>
                                                <p style="margin:0 0 0 40px">6�) A exist�ncia de restri��es na obra, sob a responsabilidade dos Munic�pios, Estados e Distrito Federal, enquanto n�o superadas, impede a libera��o de parcelas para esta, salvo se providenciada sua corre��o e forem tramitadas para an�lise do t�cnico do FNDE;</p>

                                                <p style="margin:0 0 0 40px">7�) Se, durante a an�lise da solicita��o de libera��o de parcelas, forem cadastradas restri��es na obra pactuada com o Munic�pio/Estado/DF solicitante, ser�, para esta, indeferido o pedido de repasse de recursos at� que os problemas apontados sejam sanados;</p>

                                                <p style="margin:0 0 0 40px">8�) O Munic�pio/Estado/DF dever� aguardar a delibera��o do pedido de desembolso para que novos pedidos sejam solicitados;</p>

                                                <p style="margin:0 0 0 40px">9�) O acompanhamento da solicita��o de desembolso de parcelas ser� disponibilizado no SIMEC - OBRAS 2.0;</p>
                                                <p>
                                                        <br /><br /><br />Atenciosamente,
                                                </p>
                                                <p style="text-align: center;">
                                                        <img align="center" style="height:80px;" src="data:image/png;base64,' . $email->getAssinatura() . '" />
                                                        <br />
                                                        <b>Fabr�cio Batista de Ara�jo<b>
                                                        <br />
                                                        Coordenador Geral de Implementa��o e Monitoramento de Projetos Educacionais
                                                        <br />
                                                        CGIMP/DIRPE/FNDE/MEC
                                                </p>
                                             </td>
                                        </tr>
                                    </tbody>
                                </table>
                            </body>
                        </html>
                                    ';
$dompdf = new DOMPDF();
$dompdf->load_html($conteudo);
$dompdf->render();

$pdfoutput = $dompdf->output();

$file = new FilesSimec(null, null, "obras2");
$file->setPasta('obras2');
$arqid = $file->setStream('conteudo_email', $pdfoutput, 'application/pdf', '.pdf', false, 'conteudo_email.pdf');


$sql = "SELECT obrid FROM obras2.obras WHERE obridpai IS NULL AND obrstatus = 'A' AND obrid NOT IN (SELECT obrid FROM obras2.registroatividade  WHERE rgadscsimplificada = 'E-mail enviado (Alerta de Solicita��es de Desembolso)' AND rgadsccompleta = 'E-mail enviado (Alerta de Solicita��es de Desembolso) para: Gestores e Fiscais') LIMIT {$_GET['limit']}";
$obras = $db->carregarColuna($sql);

foreach ($obras as $obrid) {
    registraAtividade($arqid, $obrid);
}
$db->commit();
var_dump($obras);



function registraAtividade($arqidConteudo, $obrid) {
    global $db;

    // Monta o arquivo com corpo
    $sql = "select * from obras2.tipoemail where temid = 44";
    $tipo = $db->pegaLinha($sql);

    $arDado = array();

    $arDado['arqid'] = $arqidConteudo;
    $arDado['obrid'] = $obrid;
    $arDado['rgaautomatica'] = 'true';
    $arDado['rgadscsimplificada'] = 'E-mail enviado (' . $tipo['temnome'] . ')';
    $arDado['rgadsccompleta'] = 'E-mail enviado (' . $tipo['temnome'] . ') para: Gestores e Fiscais';

    if (empty($arDado['arqid']))
        $arDado['arqid'] = 'NULL';

    $sql = "INSERT INTO obras2.registroatividade (arqid, obrid, rgaautomatica, rgadscsimplificada, rgadsccompleta) VALUES (
                  {$arDado['arqid']},
                  {$arDado['obrid']},
                  {$arDado['rgaautomatica']},
                  '{$arDado['rgadscsimplificada']}',
                  '{$arDado['rgadsccompleta']}'
                  )";

    $db->executar($sql);
}