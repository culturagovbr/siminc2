<?php

ini_set("memory_limit", "3024M");
set_time_limit(0);
define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));

$_REQUEST['baselogin'] = "simec_espelho_producao"; //simec_desenvolvimento
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
include_once APPRAIZ . "includes/classes/modelo/obras2/Empreendimento.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/DestinatarioEmail.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/AnexoEmail.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/Email.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/Restricao.class.inc";
include_once APPRAIZ . "includes/classes/modelo/entidade/Endereco.class.inc";
include_once APPRAIZ . "includes/classes/entidades.class.inc";
include_once APPRAIZ . "includes/classes/dateTime.inc";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";


$sql = "
            SELECT
                o.*,
                ep.prfid,
                s.supdata,
                b.dcodatafim < NOW() conv_vencido,
                v.\"Fim Vig�ncia Termo\" as fim_termo,
                DATE_PART('days', NOW() - eml.data) dias,
                (
                    SELECT
                        CASE
                        WHEN SUM(icovlritem) > 0 THEN
                            ROUND( (SUM( spivlrfinanceiroinfsupervisor ) /  SUM(icovlritem)) * 100, 2)
                        ELSE
                            0
                        END AS total
                    FROM
                        obras2.cronograma cro
                        JOIN obras2.itenscomposicaoobra i ON cro.croid = i.croid
                    LEFT JOIN
                        obras2.supervisaoitem sic ON sic.icoid = i.icoid
                        AND sic.supid = s.supid
                        AND sic.icoid IS NOT NULL
                        AND sic.ditid IS NULL
                        JOIN obras2.supervisao su ON su.supid = sic.supid
                    WHERE
                        i.icostatus = 'A' AND
                        i.relativoedificacao = 'D' AND
                        i.obrid = o.obrid AND
                        i.croid = s.croid AND
                        cro.crostatus IN ('A', 'H') AND su.croid = cro.croid
                ) as percentual,
                CASE WHEN r.obrid IS NOT NULL THEN 'S' ELSE 'N' END possui_restricao

                FROM obras2.obras o
                JOIN obras2.empreendimento ep ON ep.empid = o.empid
                JOIN workflow.documento d ON d.docid = o.docid
                JOIN workflow.estadodocumento e ON e.esdid = d.esdid
                JOIN (SELECT MAX(s.supid) as supid, s.obrid
                    FROM
                        obras2.supervisao s
                    WHERE s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A' AND s.validadaPeloSupervisorUnidade = 'S'
                    GROUP BY s.obrid) as ult_sup ON ult_sup.obrid = o.obrid
                JOIN obras2.supervisao s ON s.supid = ult_sup.supid
                LEFT JOIN (SELECT
                            r.obrid
                        FROM obras2.restricao  r
                        JOIN workflow.documento d ON d.docid = r.docid
                        JOIN workflow.estadodocumento e ON e.esdid = d.esdid
                        WHERE r.rststatus = 'A' AND d.esdid != 1142 AND r.obrid IS NOT NULL
                        GROUP BY r.obrid) r ON r.obrid = o.obrid
                LEFT JOIN (SELECT MAX(emldata) as data, obrid FROM obras2.email e WHERE temid IN (25, 26) AND e.emlstatus NOT IN ('I') GROUP BY obrid) as eml ON eml.obrid = o.obrid
                LEFT JOIN painel.dadosconvenios b on b.dcoprocesso = Replace(Replace(Replace(o.obrnumprocessoconv,'.',''),'/',''),'-','')
                LEFT JOIN obras2.vm_termo_obras v ON v.\"ID Obra\" = o.obrid
                WHERE
                e.esdid = 690 AND
                o.obridpai IS NULL AND
                o.obrstatus = 'A' AND
                o.obrpercentultvistoria > 93 AND
                DATE_PART('days', NOW() - s.supdata) > 59 AND
                o.obrid = 28255

";

// Obras com mais de 59 dias acima de 93% de execu��o
$obras = $db->carregar($sql);
$arrObras1 = array();
$arrObras2 = array();
$arrObras3 = array();
$arrObras4 = array();

foreach ($obras as $obra) {
    if($obra['possui_restricao'] == 'N'){
        // Disparar o email 1 se n�o existirem restri��es ou incoformidades ou todas estiverem superadas. 30 dias ap�s esse disparo verificar novamente e disparar o email 1, desde que a condi��o de n�o existirem as restri��es/inconformidades ou estarem superadas estiver obedecida e n�o conclus�o da obra.
        $arrObras1[] = $obra['obrid'];
    } else {
        //Se ocorrer de existir restri��o ou inconformidade n�o superada (ag providencia, analise fnde, ag corre��o), disparar o email 2. e 30 dias ap�s fazer o mesmo se mantiver a conclus�o de n�o supera��o e n�o conclus�o da obra.
        $arrObras2[] = $obra['obrid'];
    }

    if($obra['prfid'] == '41' && ( $obras['conv_vencido'] == 't' || verificaDataTermoVencido($obra['fim_termo']))){
        $arrObras3[] = $obra['obrid'];
    }

    if ($obra['dias'] > 0) {
        if($obra['dias'] % 7 == 0)
            $arrObras4[] = $obra['obrid'];
    }
}
//ver($arrObras1,$arrObras2,$arrObras3,$arrObras4,d);
foreach ($arrObras1 as $key => $obrid) {

    $conteudo = geraConteudo1($obrid);
    if($conteudo['prazo']){
        if(enviaEmail($obrid, $conteudo['conteudo'], 25))
            criaRestricao($obrid, $conteudo['descricao'], $conteudo['providencia'], $conteudo['prazo']);
    }
}

foreach ($arrObras2 as $key => $obrid) {

    $conteudo = geraConteudo2($obrid);
    if($conteudo['prazo']) {
        if(enviaEmail($obrid, $conteudo['conteudo'], 26))
            criaRestricao($obrid, $conteudo['descricao'], $conteudo['providencia'], $conteudo['prazo']);
    }
}

foreach ($arrObras3 as $key => $obrid) {

    $conteudo = geraConteudo3($obrid);
    if($conteudo['prazo']) {
        if(enviaEmail($obrid, $conteudo['conteudo'], 27))
            criaRestricao($obrid, $conteudo['descricao'], $conteudo['providencia'], $conteudo['prazo']);
    }
}






foreach ($arrObras4 as $key => $obrid) {

    $conteudo = geraConteudo4($obrid);
    enviaEmail($obrid, $conteudo['conteudo'], 28, false);
}

function criaRestricao($obrid, $descricao, $providencia, $prazo)
{
    // Criar inconformidade
    $obra = new Obras($obrid);

    // CPF 21269017500
    $dados = array(
        'rstid' => null,
        'tprid' => 16,
        'fsrid' => 1,
        'empid' => $obra->empid,
        'obrid' => $obra->obrid,
        'usucpf' => '21269017500',
        'rstdsc' => $descricao,
        'rstdscprovidencia' => $providencia,
        'rstitem' => 'I',
        'rstdtprevisaoregularizacao' => "NOW() + interval '{$prazo['n']}' day",
        'rstdtinclusao' => 'NOW()',
        'rststatus' => 'A',
    );


    $sql = "insert into obras2.restricao ( tprid, fsrid, empid, usucpf, rstdsc, rstdtprevisaoregularizacao, rstdscprovidencia, rstdtinclusao, rststatus, obrid, rstitem )
              values ( {$dados['tprid']}, {$dados['fsrid']}, {$dados['empid']},  {$dados['usucpf']}, '{$dados['rstdsc']}', {$dados['rstdtprevisaoregularizacao']}, '{$dados['rstdscprovidencia']}', {$dados['rstdtinclusao']}, '{$dados['rststatus']}', {$dados['obrid']}, '{$dados['rstitem']}' )
					 returning rstid";
    $restricao = new Restricao();
    $rstid = $restricao->pegaUm($sql);
    $restricao->commit();

    $restricao->atualizaDocidNullRetricao($rstid, 1);
    $restricao->commit();
}

function geraConteudo1($obrid)
{
    $obra = new Obras($obrid);
    $empreendimento = new Empreendimento($obra->empid);
    $vistoria = pegaDadosVistoria($obrid);
    $endereco = new Endereco(($obra->endid ? $obra->endid : $empreendimento->endid));

    $prazoTotal = array(
        1 => array('n' => 5, 'e' => 'cinco'),
        2 => array('n' => 20, 'e' => 'vinte'),
        3 => array('n' => 30, 'e' => 'trinta'),
    );

    if ($vistoria['percentual'] >= 97) $prazo = $prazoTotal[1];
    if ($vistoria['percentual'] >= 94 && $vistoria['percentual'] <= 96.9) $prazo = $prazoTotal[2];
    if ($vistoria['percentual'] >= 90 && $vistoria['percentual'] <= 93.9) $prazo = $prazoTotal[3];

    $conteudo = '<p>
                    1. Em decorr�ncia do acompanhamento realizado por meio do Sistema Integrado de
                    Monitoramento, Execu��o e Controle do Minist�rio da Educa��o (SIMEC), no M�dulo OBRAS 2.0, a obra
                    em epigrafe encontra-se com o percentual de ' . $vistoria["percentual"] . '% h� ' . $vistoria["dias"] . ' dias. Assim, de modo a viabilizar os demais
                    repasses do MEC/ FNDE, como o E.I Manuten��o, que pode chegar ao montante de R$ ' . pegaAlunos($endereco->estuf) . ' aluno/m�s
                    (Resolu��o/CD/ FNDE/MEC n� 15 e 16/2013), � condi��o que a obra j� esteja conclu�da.
                </p>

                <p>
                    2. Solicitamos que a inser��o de vistoria de conclus�o seja realizada no prazo m�ximo de ' . $prazo['n'] . '
                    (' . $prazo['e'] . ') dias, conforme manual de orienta��o de preenchimento do SIMEC OBRAS 2.0, disponibilizado
                    no sitio do FNDE, (link: http://www.fnde.gov.br/programas/proinfancia/proinfancia-manuais/item/4967-monitoramento),
                    e quando conclu�da, seja informadas a esta Autarquia, por meio da supera��o da respectiva
                    inconformidade, na aba Restri��o e Inconformidade. O n�o atendimento da provid�ncia poder� acarretar
                    san��es administrativas, conforme previsto no instrumento pactuado.
                </p>';

    $descricao = 'A obra em epigrafe encontra-se com o percentual de ' . $vistoria["percentual"] . '% h� ' . $vistoria["dias"] . ' dias.';
    $providencia = 'Solicitamos que a inser��o de vistoria de conclus�o seja realizada no prazo m�ximo de ' . $prazo['n'] . ' (' . $prazo['e'] . ') dias, conforme manual de orienta��o de preenchimento do SIMEC OBRAS 2.0, disponibilizado no sitio do FNDE, (link: http://www.fnde.gov.br/programas/proinfancia/proinfancia-manuais/item/4967-monitoramento)';

    return array(
        'conteudo' => $conteudo,
        'descricao' => $descricao,
        'providencia' => $providencia,
        'prazo' => $prazo,
    );
}

function geraConteudo2($obrid)
{
    $obra = new Obras($obrid);
    $empreendimento = new Empreendimento($obra->empid);
    $vistoria = pegaDadosVistoria($obrid);
    $endereco = new Endereco(($obra->endid ? $obra->endid : $empreendimento->endid));

    $prazoTotal = array(
        1 => array('n' => 5, 'e' => 'cinco'),
        2 => array('n' => 20, 'e' => 'vinte'),
        3 => array('n' => 30, 'e' => 'trinta'),
    );
    if ($vistoria['percentual'] >= 97) $prazo = $prazoTotal[1];
    if ($vistoria['percentual'] >= 94 && $vistoria['percentual'] <= 96.9) $prazo = $prazoTotal[2];
    if ($vistoria['percentual'] >= 90 && $vistoria['percentual'] <= 93.9) $prazo = $prazoTotal[3];

    $conteudo = '<p>
                    1. Em decorr�ncia do acompanhamento realizado por meio do Sistema Integrado de
                    Monitoramento, Execu��o e Controle do Minist�rio da Educa��o (SIMEC), no M�dulo OBRAS 2.0, verificamos que a obra
                    supracitada encontra-se sem a vistoria de conclus�o, embora encontre-se com o percentual de ' . $vistoria["percentual"] . '% h� ' . $vistoria["dias"] . ' dias.
                </p>

                <p>
                    2. Ressaltamos que essa situa��o pode estar inviabilizando repasses do FNDE ao munic�pio, como o E I
                    Manuten��o, que podem chegar ao montante de R$ ' . pegaAlunos($endereco->estuf) . ' aluno/m�s (Resolu��es FNDE nr. 15, 16 e 17/2013).
                </p>

                <p>
                    3. Quanto �s restri��es e inconformidades, que comprometem a boa execu��o e a plena realiza��o do
                    objeto pactuado, identificadas em visita de supervis�o realizada in loco por empresa contratada pelo FNDE,
                    e registradas no Simec- M�dulo Obras 2.0 - na aba "Restri��es e Inconformidades" -, devem ser sanadas
                    seguindo as orienta��es constantes na mesma aba. As resolu��es dessas pend�ncias podem ocorrer mesmo
                    ap�s a obra ter sido dada como conclu�da no sistema.
                </p>

                <p>
                    4. Quando houver a supera��o das restri��es e/ou inconformidades, o fato deve ser informado ao
                    FNDE atrav�s da tramita��o pela barra de trabalho, localizada no canto direito da tela (orienta��o encontra-se na p�gina de acesso inicial do SIMEC-M�dulo Obras 2.0, do fiscal ou do gestor da obra), para que n�o
                    impactem quando da presta��o de contas do instrumento pactuado.
                </p>

                <p>
                    5. Solicitamos que a inser��o de vistoria de conclus�o seja realizado no prazo m�ximo de ' . $prazo['n'] . ' (' . $prazo['e'] . ') dias, e quando conclu�da, seja informadas a esta Autarquia, por meio de e-mail, endere�ado para
                    fabio.cardoso@fnde.gov.br e monitoramento.obras@fnde.gov.br . O n�o atendimento da provid�ncia poder�
                    acarretar san��es administrativas, conforme previsto no instrumento pactuado.
                </p>

                ';

    $descricao = 'Verificamos que a obra supracitada encontra-se sem a vistoria de conclus�o, embora encontre-se com o percentual de ' . $vistoria["percentual"] . '% h� ' . $vistoria["dias"] . ' dias';
    $providencia = 'Solicitamos que a inser��o de vistoria de conclus�o seja realizada no prazo m�ximo de ' . $prazo['n'] . ' (' . $prazo['e'] . ') dias, conforme manual de orienta��o de preenchimento do SIMEC OBRAS 2.0, disponibilizado no sitio do FNDE, (link: http://www.fnde.gov.br/programas/proinfancia/proinfancia-manuais/item/4967-monitoramento)';

    return array(
        'conteudo' => $conteudo,
        'descricao' => $descricao,
        'providencia' => $providencia,
        'prazo' => $prazo,
    );
}

function geraConteudo3($obrid)
{

    $vistoria = pegaDadosVistoria($obrid);
    $prazoTotal = array(
        1 => array('n' => 5, 'e' => 'cinco'),
        2 => array('n' => 20, 'e' => 'vinte'),
        3 => array('n' => 30, 'e' => 'trinta'),
    );
    if ($vistoria['percentual'] >= 97) $prazo = $prazoTotal[1];
    if ($vistoria['percentual'] >= 94 && $vistoria['percentual'] <= 96.9) $prazo = $prazoTotal[2];
    if ($vistoria['percentual'] >= 90 && $vistoria['percentual'] <= 93.9) $prazo = $prazoTotal[3];

    $conteudo = '<p>
                    1. Em decorr�ncia do acompanhamento realizado atrav�s do Sistema Integrado de Monitoramento,
                    Execu��o e Controle do Minist�rio da Educa��o (SIMEC), m�dulo Obras2.0, verificamos que a obra
                    supracitada encontra-se sem a vistoria de conclus�o, embora o instrumento pactuado j� esteja vencido, e em
                    fase de prepara��o para a presta��o de contas.
                </p>

                <p>
                    2. Com o intuito de regularizar a situa��o, orientamos que seja inserida a vistoria de conclus�o de obra,
                    conforme manual de orienta��es de preenchimento do SIMEC OBRAS 2.0, disponibilizado atrav�s do link:
                    http://www.fnde.gov.br/programas/proinfancia/proinfancia-manuais/item/4967-monitoramento.
                </p>
                <p>
                    3. Solicitamos que a inser��o de vistoria de conclus�o seja realizado no prazo m�ximo de ' . $prazo['n'] . ' (' . $prazo['e'] . ') dias,
                    e quando conclu�da, seja informada a esta Autarquia, por meio da supera��o da respectiva inconformidade, na
                    aba Restri��o e Inconformidade.
                </p>
                <p>
                    4. O n�o atendimento da provid�ncia poder� acarretar san��es administrativas, conforme previsto no
                    instrumento pactuado, o que pode incluir a glosa total do objeto pactuado (por falta de seu cumprimento) e a
                    instaura��o de Tomada de Contas Especial (TCE).
                </p>
                ';

    $descricao = 'A obra supracitada encontra-se sem a vistoria de conclus�o, embora o instrumento pactuado j� esteja vencido.';
    $providencia = 'Inserir a vistoria de conclus�o de obra, conforme manual de orienta��es de preenchimento do SIMEC OBRAS 2.0, disponibilizado atrav�s do link: http://www.fnde.gov.br/programas/proinfancia/proinfancia-manuais/item/4967-monitoramento';

    return array(
        'conteudo' => $conteudo,
        'descricao' => $descricao,
        'providencia' => $providencia,
        'prazo' => $prazo,
    );
}


function geraConteudo4($obrid)
{
    $obra = new Obras($obrid);

    $conteudo = '<p>
                    1. Em raz�o de estar pr�xima a conclus�o da obra '.$obra->obrnome.', apresentamos as orienta��es que se seguem.
                </p>

                <p>
                    2. Quando da conclus�o da obra e seu recebimento provis�rio, cabe ao respons�vel por seu acompanhamento e fiscaliza��o, a verifica��o do cumprimento do objeto contratado em conformidade com o projeto e de acordo com as normas da Associa��o Brasileira de Normas T�cnicas - ABNT, que refletem os requisitos m�nimos de qualidade, utilidade, resist�ncia e seguran�a, conforme determina a Lei n� 4.150, de 1962, o inciso X do art. 6� e o art. 66 da Lei n� 8.666, de 1993.
                </p>

                <p>
                    3. Caso seja verificada a exist�ncia de v�cios, defeitos ou incorre��es em raz�o da execu��o ou da qualidade dos materiais utilizados, cabe � empresa contratada adotar as provid�ncias necess�rias � supera��o dessas irregularidades, caso contr�rio, a Administra��o poder� rejeitar a obra no todo ou em parte, se executada em desacordo com o contrato, conforme previs�o legal constante nos artigos 69, 70 e 76 da Lei n� 8.666, de 1993.
                </p>

                <p>
                    4. Esclarecemos que, mesmo ap�s o recebimento provis�rio ou definitivo da obra, a empresa contratada continua sendo respons�vel civilmente pela solidez e seguran�a do empreendimento pelo prazo de cinco anos, devendo apresentar a corre��o dos v�cios que surgirem nesse per�odo, nos termos do �2� do art. 73 da Lei n� 8.666/93 c/c art. 618 da Lei n� 10.406, de 2012. H� que se observar, ainda, a orienta��o do Tribunal de Contas da Uni�o constante no sum�rio do Relat�rio de Auditoria (TC 018.842/2013-5 - Ac�rd�o n� 1816/2014- Plen�rio) sobre a necessidade de acompanhamento peri�dico de obra conclu�da, nos seguintes termos:
                    <p style="padding-left:200px">
                        � recomend�vel a realiza��o de acompanhamento peri�dico da obra conclu�da, mormente nos cinco anos posteriores ao seu t�rmino, com a finalidade de identificar falhas que devam ser corrigidas pelo executor sem �nus para a Administra��o P�blica, bem como de garantir o seu adequado funcionamento durante a vida �til de projeto, sendo a boa pr�tica a elabora��o de um manual de utiliza��o, inspe��o e manuten��o para empreendimento em quest�o.
                    </p>
                </p>

                <p>
                    5. Quando da conclus�o da obra, a unidade gestora dever� garantir condi��es de funcionamento e habitabilidade da edifica��o com as liga��es definitivas de energia el�trica e �gua pot�vel. Caso haja atraso das concession�rias de servi�os p�blicos em fornecer esses servi�os, orientamos o gestor p�blico a acionar as Ag�ncias Reguladoras respons�veis.
                </p>

                <p>
                    6. Isto posto, colocamo-nos � disposi��o em caso de d�vidas atrav�s do email atendimento.monitora@fnde.gov.br.
                </p>
                ';


    return array(
        'conteudo' => $conteudo
    );
}


function enviaEmail($obrid, $conteudo, $temid, $verifica_email = true)
{
    global $db;

    $obra = new Obras($obrid);
    $email = new Email();

    if($verifica_email) {
        if ($email->verificaEmailEnviado($temid, $obrid, 30)) {
            return false;
        }
    }

    $esfera = $db->pegaUm("select empesfera from obras2.empreendimento where empid = " . $obra->empid);

    // Por enquanto e-mail enviado somente para prefeitura
    if ($esfera != 'M')
        return;

    $entPrefeito = $email->pegaEntidadePar($obrid, 2);
    $entPrefeitura = $email->pegaEntidadePar($obrid, 7);

    $enderecoPrefeitura = current($entPrefeitura->enderecos);

    $data = new Data();
    $data = $data->formataData($data->dataAtual(), 'Bras�lia, DD de mesTextual de YYYY.');
    $dados = array(
        'usucpf' => $_SESSION['usucpf'],
        'emlconteudo' => '
                        <html>
                            <head>
                                <title></title>
                            </head>
                            <body>
                                <table style="width: 100%;">
                                    <thead>
                                        <tr>
                                            <td style="text-align: center; font-size:12px">
                                                <p><img  src="data:image/png;base64,' . base64_encode(file_get_contents(APPRAIZ . '/www/' . 'imagens/brasao.gif')) . '" width="70"/><br/>
                                                <b>MINIST�RIO DA EDUCA��O</b><br/>
                                                FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCA��O - FNDE<br/>
                                                DIRETORIA DE GEST�O, ARTICULA��O E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                                COORDENA��O GERAL DE IMPLEMENTA��O E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                                SBS Q.2 Bloco F Edif�cio FNDE - 70.070-929 - Bras�lia, DF - E-mail: monitoramento.obras@fnde.gov.br<br/>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td>
                                                <p style="float:left; text-align: left; padding: 40px 0 0 0;">Comunicado N� __RGAID__ - CGIMP/DIGAP/FNDE</p>
                                                <p style="float-right; text-align: right; padding: 40px 0 0 0;">' . $data . '</p>
                                            </td>
                                        </tr>
                                    </thead>
                                    <tbody>
                                        <tr>
                                            <td style="line-height: 20px;">
                                                A Sua Excel�ncia a Senhor(a)
                                                <br />
                                                ' . $entPrefeito->getEntnome() . '
                                                <br />
                                                Prefeito(a) do Munic�pio de ' . $enderecoPrefeitura['mundescricao'] . ' - ' . $enderecoPrefeitura['estuf'] . '
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="padding:10px 0 20px 0;">
                                              Assunto: <b>Inconformidades na obra (' . $obrid . ') ' . $obra->obrnome . '</b>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="line-height: 20px; text-align:justify">
                                                <p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Senhor Prefeito(a),</p>

                                                ' . $conteudo . '

                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="padding: 1px 0 0 0;">
                                                    Atenciosamente,
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="text-align: center; padding: 1px 0 0 0;">
                                                    <img align="center" style="height:80px;margin-top:5px;margin-bottom:5px;" src="data:image/png;base64,' . base64_encode(file_get_contents(APPRAIZ . 'www/imagens/obras/assinatura-fabio.png')) . '" />
                                                    <br />
                                                    <b>F�bio L�cio de Almeida Cardoso<b>
                                                    <br />
                                                    Coordenador Geral de Implementa��o e Monitoramento de Projetos Educacionais
                                                    <br />
                                                    CGIMP/DIRPE/FNDE/MEC
                                            </td>
                                        </tr>
                                    </tbody>
                                    <tfoot>

                                    </tfoot>
                                </table>
                            </body>
                        </html>
                                    ',
        'emlassunto' => 'Inconformidades na obra (' . $obrid . ') ' . $obra->obrnome,
        'temid' => $temid,
        'emlregistroatividade' => true,
        'obrid' => $obrid
    );

    $dadosRemetentes = array($entPrefeito->getEntEmail(), $entPrefeitura->getEntEmail());
//    $dadosRemetentes = array($_SESSION['email_sistema']);

    $email->popularDadosObjeto($dados);
    $email->salvar($dadosRemetentes);
    $email->enviar();

    return true;
}

function pegaAlunos($uf)
{
    $alunos = array(
        'AC' => '3.622,85',
        'AL' => '2.971,24',
        'AM' => '2.971,24',
        'AP' => '4.362,13',
        'BA' => '2.971,24',
        'CE' => '2.971,24',
        'DF' => '3.230,31',
        'ES' => '3.548,72',
        'GO' => '3.533,72',
        'MA' => '2.971,24',
        'MG' => '3.131,44',
        'MS' => '3.483,45',
        'MT' => '3.030,59',
        'PA' => '2.971,24',
        'PB' => '2.971,24',
        'PE' => '2.971,24',
        'PI' => '2.971,24',
        'PR' => '3.088,41',
        'RJ' => '3.395,17',
        'RN' => '2.971,24',
        'RO' => '3.265,40',
        'RR' => '5.105,31',
        'RS' => '3.863,42',
        'SC' => '3.527,49',
        'SE' => '3.571,18',
        'SP' => '3.944,06',
        'TO' => '3.839,87'
    );

    return $alunos[$uf];
}

function pegaDadosVistoria($obrid)
{
    global $db;
    $sql = "SELECT s.supdata, date_part('day',now() - s.supdata) as dias, ( SELECT
                                  CASE
                                      WHEN sum(i.icovlritem) > 0::numeric THEN round(sum(sic.spivlrfinanceiroinfsupervisor) / sum(i.icovlritem) * 100::numeric, 2)
                                      ELSE 0::numeric
                                  END AS total
                                   FROM obras2.itenscomposicaoobra i
                                   JOIN obras2.cronograma cro ON cro.croid = i.croid AND cro.crostatus = 'A'
                                    LEFT JOIN obras2.supervisaoitem sic ON sic.icoid = i.icoid AND sic.supid = s.supid AND sic.icoid IS NOT NULL AND sic.ditid IS NULL
                                   WHERE i.icostatus = 'A'::bpchar AND i.relativoedificacao = 'D'::bpchar AND cro.obrid = $obrid AND i.obrid = cro.obrid) AS percentual
            FROM obras2.supervisao s
            WHERE s.obrid = $obrid AND s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar
            ORDER BY s.supdata DESC LIMIT 1";

    return $db->pegaLinha($sql);
}


function verificaDataTermoVencido($data){
    require_once APPRAIZ . "includes/classes/dateTime.inc";

    $data = trim($data);

    $m = '';
    $d = '';
    $a = '';

    if(strlen($data) == 7){
        $data = explode('/', $data);
        $d = '01';
        $m = $data[0];
        $a = $data[1];
    } else if(strlen($data) == 10){
        $data = explode('/', $data);
        $d = $data[0];
        $m = $data[1];
        $a = $data[2];
    } else {
        return false;
    }
    $data = new Data();
    return $data->diferencaEntreDatas($data->dataAtual(), "$d/$m/$a 00:00:00", 'maiorDataBolean', 'string', '');
}