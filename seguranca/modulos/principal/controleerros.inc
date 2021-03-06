<?php
include_once APPRAIZ . "includes/library/simec/Grafico.php";
include APPRAIZ ."includes/cabecalho.inc";
echo '<br>';

/* montando cabe�alho */
monta_titulo( "Controle de Erros dos �ltimos 2 meses", "");

$sql = "SELECT u.usucpf, u.usunome, u.usuemail, e.eeuid FROM seguranca.envioerrosusuarios e INNER JOIN seguranca.usuario u ON u.usucpf = e.usucpf WHERE eeustatus='A'";
$usus = $db->carregar($sql);


$sql = "
    -- Qtd distintos por dia
    SELECT
        'Erros �nicos' AS descricao,
        DATA AS categoria,
        count(*) AS valor
    FROM (
        SELECT
            DISTINCT errtipo,
            sisid,
            errarquivo,
            errlinha,
            errdata,
            TO_CHAR(errdata, 'YYYY-MM-DD') AS DATA,
            count(*)
        FROM
            seguranca.erro
        GROUP BY
            errtipo,
            sisid,
            errarquivo,
            errlinha,
            errdata,
            DATA
        ORDER BY
            DATA,
            sisid
    ) AS foo
    WHERE
        -- Ultimos 2 meses
        errdata >= DATE_TRUNC('day', NOW() - interval '2 month')
    GROUP BY
        categoria,
        descricao
";

$sql = "
    SELECT
        *
    FROM (
        SELECT
            'Erros �nicos' AS descricao,
            DATA AS categoria,
            count(*) AS valor
        FROM (
            SELECT
                DISTINCT errtipo,
                sisid,
                errarquivo,
                errlinha,
                to_char(errdata, 'YYYY-MM-DD') AS DATA,
                count(*)
            FROM seguranca.erro
            WHERE
                    -- Ultimos 2 meses
                    errdata >= DATE_TRUNC('day', NOW() - interval '2 month')
            GROUP BY
                errtipo,
                sisid,
                errarquivo,
                errlinha,
                DATA
        ) AS foo
        GROUP BY
            descricao,
            categoria

        UNION

        SELECT
            'Erros totais' AS descricao,
            to_char(errdata, 'YYYY-MM-DD') AS DATA,
            count(*)
        FROM seguranca.erro
        WHERE
            -- Ultimos 2 meses
            errdata >= DATE_TRUNC('day', NOW() - interval '2 month')
        GROUP BY
            descricao,
            DATA
    ) AS total
    ORDER BY
        categoria,
        descricao
";

$sql2 = "
    -- Qtd distintos por dia
    SELECT
        CASE WHEN errtipo = 'DB' THEN
            'Banco de dados'
            WHEN errtipo = 'PR' THEN
            'Programa��o'
            WHEN errtipo = 'QB' THEN
            'Queda no banco'
            WHEN errtipo = 'WS' THEN
            'WebService'
            WHEN errtipo = 'EN' THEN
            'Encoding no banco'
            WHEN errtipo = 'PD' THEN
            'Erro na Conex�o'
            WHEN errtipo = 'DC' THEN
            'Diret�rio Cheio'
            WHEN errtipo = 'AI' THEN
            'Arquivo inexistente'
            WHEN errtipo = 'DV' THEN
            'Diversos'
        ELSE
            errtipo
        END AS descricao,
        count(*) AS valor
    FROM (
        SELECT
            DISTINCT errtipo,
            sisid,
            errarquivo,
            errlinha,
            to_char(errdata,
                'YYYY-MM-DD') AS DATA,
            count(*)
        FROM seguranca.erro
        WHERE
            -- Ultimos 3 meses
            errdata >= DATE_TRUNC('day', NOW() - interval '2 month')
        GROUP BY
            errtipo,
            sisid,
            errarquivo,
            errlinha,
            DATA
        ORDER BY
            DATA,
            sisid) AS foo
    GROUP BY
        descricao
    ORDER BY
        valor DESC
";

$sql3 = "
    -- Qtd distintos por dia
    SELECT
        'Sistema' AS descricao,
        sisabrev AS categoria,
        count(*) AS valor
    FROM (
        SELECT DISTINCT
            errtipo,
            s.sisabrev,
            e.sisid,
            errarquivo,
            errlinha,
            to_char(errdata, 'YYYY-MM-DD') AS DATA,
            count(*)
        FROM seguranca.erro e
        LEFT JOIN seguranca.sistema s ON s.sisid = e.sisid
        WHERE
            -- Ultimos 3 meses
            errdata >= DATE_TRUNC('day', NOW() - interval '2 month')
        GROUP BY
                errtipo,
                s.sisabrev,
                e.sisid,
                errarquivo,
                errlinha,
                DATA
        ORDER BY
                DATA,
                e.sisid
    ) AS foo
    GROUP BY
        categoria,
        descricao
    ORDER BY
        valor DESC
";

$grafico = new Grafico();
$grafico->setTitulo('Qtd por dia')->setTipo(Grafico::K_TIPO_LINHA)->gerarGrafico($sql);
$grafico->setTitulo('Qtd �nica por tipo')->setTipo(Grafico::K_TIPO_PIZZA)->gerarGrafico($sql2);
$grafico->setTitulo('Qtd �nica por m�dulo')->setTipo(Grafico::K_TIPO_BARRA)->setLabelX(array())->setHeight('500px')->gerarGrafico($sql3);


$sql = "
    SELECT
        DISTINCT sisabrev,
        e.sisid,
        errdescricao,
        CASE WHEN errtipo = 'DB' THEN
            'Banco de dados'
            WHEN errtipo = 'PR' THEN
            'Programa��o'
            WHEN errtipo = 'QB' THEN
            'Queda no banco'
            WHEN errtipo = 'WS' THEN
            'WebService'
            WHEN errtipo = 'EN' THEN
            'Encoding no banco'
            WHEN errtipo = 'PD' THEN
            'Erro na Conex�o'
            WHEN errtipo = 'DC' THEN
            'Diret�rio Cheio'
            WHEN errtipo = 'AI' THEN
            'Arquivo inexistente'
            WHEN errtipo = 'DV' THEN
            'Diversos'
        ELSE
            errtipo
        END AS descricao,
        errarquivo,
        errlinha,
        count(*)
    FROM seguranca.erro e
        LEFT JOIN seguranca.sistema s ON s.sisid = e.sisid
    WHERE
        -- Hoje
        --to_char(errdata, 'YYYY-MM-DD') = to_char(now(), 'YYYY-MM-DD')
        -- Ultimas 2 semanas
        errdata >= DATE_TRUNC('day', NOW() - interval '2 week')
    GROUP BY
        errdescricao,
        errtipo,
        sisabrev,
        e.sisid,
        errarquivo,
        errlinha,
        errdata
    ORDER BY
        sisabrev,
        errarquivo,
        errlinha
        ";

$dados = $db->carregar($sql);

$aDados = array();
if($dados && is_array($dados)){
    foreach($dados as $count => $dado){
        $chave = $dado['sisid'] . $dado['errlinha'] . $dado['errarquivo'];
        $qtd[$chave]++;
        $aDados[$chave] = $dado;
        $aDados[$chave]['errlinha'] = $dado['errlinha'] . '&nbsp;';
        $aDados[$chave]['count'] = $qtd[$chave] . '&nbsp;';
        unset($aDados[$chave]['sisid']);
    }
}
$aDados = array_values($aDados);

//echo '<h3 style="margin-top: 20px; color: red; text-align: center;">Erros �NICOS de hoje (' . date('d/m/Y') . ')</h3>';
echo '<h3 style="margin-top: 20px; color: red; text-align: center;">Erros �NICOS das �ltimas 2 semanas</h3>';
$cabecalho = array('M�dulo', 'Erro', 'Tipo', 'Arquivo', 'Linha', 'Qtd.');
$db->monta_lista($aDados, $cabecalho, 50000000, 4, 'N','Center');
?>