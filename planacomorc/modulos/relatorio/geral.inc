<?php

include_once APPRAIZ. 'monitora/classes/Pi_PlanoInterno.class.inc';

$cabecalho = [
    'ID Planejamento', 'Situação', 'Código', 'Código da UO', 'UO', 'Código da Unidade', 'Unidade', 'Unidade Delegada', 'Unidade Delegada', 'Enquadramento', 'RP - Resultado Primário',
    'Item de Manutenção', 'Subitem de Manutenção', 'Código do Programa', 'Programa', 'Código do Objetivo', 'Objetivo', 'Código da Iniciativa', 'Iniciativa', 'Código da Ação', 'Ação', 'Código do Localizador',
    'Localizador', 'Código do PO', 'Plano Orçamentário', 'PTRES', 'Área', 'Meta PNC', 'Meta', 'Título', 'Descrição', 'Segmento Cultural', 'Tipo de Instrumento / Modalidade de Pactuação',
    'Emenda parlamentar', 'Forma de execução - Edital', 'Localização da ação', 'País', 'Unidade Federativa', 'Município', 'Código do Objetivo Estratégico', 'Objetivo Estratégico',
    'Código da Meta Estratégica', 'Meta Estratégica', 'Código da Diretriz Estratégica', 'Diretriz Estratégica',
    'Responsável', 'TED', 'Edital', 'Previsão Edital',
    'Convênio', 'SNIIC', 'Processo', 'PRONAC', 'Produto', 'Unidade de Medida', 'Quantidade', 'Valor Total', 'Valor Total Custeio', 'Valor Total Capital',

    'Cronograma Físico<br />Capital Janeiro', 'Cronograma Físico<br />Capital Fevereivo', 'Cronograma Físico<br />Capital Março', 'Cronograma Físico<br />Capital Abril',
    'Cronograma Físico<br />Capital Maio', 'Cronograma Físico<br />Capital Junho', 'Cronograma Físico<br />Capital Julho', 'Cronograma Físico<br />Capital Agosto',
    'Cronograma Físico<br />Capital Setembro', 'Cronograma Físico<br />Capital Outubro', 'Cronograma Físico<br />Capital Novembro', 'Cronograma Físico<br />Capital Dezembro',

    'Cronograma Financeiro<br />Capital Janeiro', 'Cronograma Financeiro<br />Capital Fevereivo', 'Cronograma Financeiro<br />Capital Março', 'Cronograma Financeiro<br />Capital Abril',
    'Cronograma Financeiro<br />Capital Maio', 'Cronograma Financeiro<br />Capital Junho', 'Cronograma Financeiro<br />Capital Julho', 'Cronograma Financeiro<br />Capital Agosto',
    'Cronograma Financeiro<br />Capital Setembro', 'Cronograma Financeiro<br />Capital Outubro', 'Cronograma Financeiro<br />Capital Novembro', 'Cronograma Financeiro<br />Capital Dezembro',

    'Cronograma Financeiro<br />Custeio Janeiro', 'Cronograma Financeiro<br />Custeio Fevereivo', 'Cronograma Financeiro<br />Custeio Março', 'Cronograma Financeiro<br />Custeio Abril',
    'Cronograma Financeiro<br />Custeio Maio', 'Cronograma Financeiro<br />Custeio Junho', 'Cronograma Financeiro<br />Custeio Julho', 'Cronograma Financeiro<br />Custeio Agosto',
    'Cronograma Financeiro<br />Custeio Setembro', 'Cronograma Financeiro<br />Custeio Outubro', 'Cronograma Financeiro<br />Custeio Novembro', 'Cronograma Financeiro<br />Custeio Dezembro',

    'Cronograma Orçamentário<br />Capital Janeiro', 'Cronograma Orçamentário<br />Capital Fevereivo', 'Cronograma Orçamentário<br />Capital Março', 'Cronograma Orçamentário<br />Capital Abril',
    'Cronograma Orçamentário<br />Capital Maio', 'Cronograma Orçamentário<br />Capital Junho', 'Cronograma Orçamentário<br />Capital Julho', 'Cronograma Orçamentário<br />Capital Agosto',
    'Cronograma Orçamentário<br />Capital Setembro', 'Cronograma Orçamentário<br />Capital Outubro', 'Cronograma Orçamentário<br />Capital Novembro', 'Cronograma Orçamentário<br />Capital Dezembro',

    'Cronograma Orçamentário<br />Custeio Janeiro', 'Cronograma Orçamentário<br />Custeio Fevereivo', 'Cronograma Orçamentário<br />Custeio Março', 'Cronograma Orçamentário<br />Custeio Abril',
    'Cronograma Orçamentário<br />Custeio Maio', 'Cronograma Orçamentário<br />Custeio Junho', 'Cronograma Orçamentário<br />Custeio Julho', 'Cronograma Orçamentário<br />Custeio Agosto',
    'Cronograma Orçamentário<br />Custeio Setembro', 'Cronograma Orçamentário<br />Custeio Outubro', 'Cronograma Orçamentário<br />Custeio Novembro', 'Cronograma Orçamentário<br />Custeio Dezembro',

    'Valor Provisionado', 'Valor Empenhado', 'Valor Liquidado', 'Valor Pago',
    
    'Executado', 'Análise Situacional', 'Classificação', 'Medidas a serem adotadas', 'Detalhamento das providências a serem adotadas'
];

# Lista de colunas que terão formatação de moeda.
$listaColunaMoeda = array(
    'valortotal',
    'picvalorcusteio',
    'picvalorcapital',
    'fin_capital_1',
    'fin_capital_2',
    'fin_capital_3',
    'fin_capital_4',
    'fin_capital_5',
    'fin_capital_6',
    'fin_capital_7',
    'fin_capital_8',
    'fin_capital_9',
    'fin_capital_10',
    'fin_capital_11',
    'fin_capital_12',
    'fin_custeio_1',
    'fin_custeio_2',
    'fin_custeio_3',
    'fin_custeio_4',
    'fin_custeio_5',
    'fin_custeio_6',
    'fin_custeio_7',
    'fin_custeio_8',
    'fin_custeio_9',
    'fin_custeio_10',
    'fin_custeio_11',
    'fin_custeio_12',
    'orc_capital_1',
    'orc_capital_2',
    'orc_capital_3',
    'orc_capital_4',
    'orc_capital_5',
    'orc_capital_6',
    'orc_capital_7',
    'orc_capital_8',
    'orc_capital_9',
    'orc_capital_10',
    'orc_capital_11',
    'orc_capital_12',
    'orc_custeio_1',
    'orc_custeio_2',
    'orc_custeio_3',
    'orc_custeio_4',
    'orc_custeio_5',
    'orc_custeio_6',
    'orc_custeio_7',
    'orc_custeio_8',
    'orc_custeio_9',
    'orc_custeio_10',
    'orc_custeio_11',
    'orc_custeio_12',
    'vlrautorizado',
    'vlrempenhado',
    'vlrliquidado',
    'vlrpago'
);

# Verifica se o usuário tem vinculo de Sub-Unidades no seu Perfil
$listaSubunidade = buscarSubUnidadeUsuario((object)array('usucpf' => $_SESSION['usucpf']));

$dto = (object) array(
    'exercicio' => $_SESSION['exercicio'],
    'listaSubunidade' => $listaSubunidade
);
# Filtro do FNC
if($_REQUEST['fundo'] === 't'){
    $dto->fundo = TRUE;
}
else if($_REQUEST['fundo'] === 'f'){
    $dto->fundo = FALSE;
}
$dados = (new Pi_PlanoInterno())->listarRelatorioGeralPi($dto);

?>

<table width="100%" cellspacing="0">
    <thead>
        <tr>
            <?php foreach($cabecalho as $coluna){ ?>
                <th style="border: 1px #e5d9c5 solid; background-color: #f4f4f4"><?php echo $coluna; ?></th>
            <?php } ?>
        </tr>
    </thead>
    <tbody>
        <?php foreach($dados as $dado){ ?>
            <tr>
                <?php foreach($dado as $coluna => $valor){ ?>
                    <td style="border: 1px #e5d9c5 solid;"><?php echo in_array($coluna, $listaColunaMoeda)? formata_valor($valor): $valor; ?></td>
                <?php } ?>
            </tr>
        <?php } ?>
    </tbody>
</table>

<?php

header( 'Content-Type: application/vnd.ms-excel' );
header( 'Expires: 0' );
header( 'Cache-Control: must-revalidate, post-check=0, pre-check=0' );
header( 'Content-Disposition: attachment; filename="relatorio_geral_pi' .($_REQUEST['fundo'] === 't'? '_fnc': NULL). '.xls"' );
die;
