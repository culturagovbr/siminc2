<?php

    /**
     * Monta a combo de UGs filtrando por UO
     *
     * @param $filtros
     * @return VOID
     */
    function montarComboUG(stdClass $filtros)
    {
        global $simec;

        return $simec->select(
            'suocod',
            'Subunidade',
            $filtros->ungcod,
            Public_Model_SubUnidadeOrcamentaria::queryCombo((object) array(
                'exercicio' => $filtros->exercicio,
                'unicod' => $filtros->unicod)));
    }

    /**
     * Monta as Colunas do Relat�rio de Pr� PI do M�dulo Proposta Or�ament�ria.
     *
     * @return array
     */
    function montarColunasRelatorioPrePI()
    {
        $colunas = array(
            array('codigo' => 'pliid', 'descricao' => 'Id Pr�-PI'),
            array('codigo' => 'plititulo', 'descricao' => 'T�tulo'),
            array('codigo' => 'plidsc', 'descricao' => 'Descri��o/Finalidade'),
            array('codigo' => 'subunidade', 'descricao' => 'Subunidade'),
            array('codigo' => 'eqddsc', 'descricao' => 'Enquadramento da Despesa'),
            array('codigo' => 'irpcod', 'descricao' => 'RP'),
            array('codigo' => 'mainome', 'descricao' => 'Item'),
            array('codigo' => 'masnome', 'descricao' => 'Subitem'),
            array('codigo' => 'funcional', 'descricao' => 'Funcional'),
            array('codigo' => 'acatitulo', 'descricao' => 'A��o'),
            array('codigo' => 'plodsc', 'descricao' => 'PO'),
            array('codigo' => 'esddsc', 'descricao' => 'Situa��o'),
            array('codigo' => 'pprnome', 'descricao' => 'Produto'),
            array('codigo' => 'pumdescricao', 'descricao' => 'Unidade de Medida'),
            array('codigo' => 'pliquantidade', 'descricao' => 'Quantidade'),
            array('codigo' => 'oppcod', 'descricao' => 'C�digo Objetivo PPA'),
            array('codigo' => 'oppdsc', 'descricao' => 'Objetivo PPA'),
            array('codigo' => 'mppcod', 'descricao' => 'C�digo Metas PPA'),
            array('codigo' => 'mppnome', 'descricao' => 'Metas PPA'),
            array('codigo' => 'ippcod', 'descricao' => 'C�digo Iniciativa PPA'),
            array('codigo' => 'ippnome', 'descricao' => 'Iniciativa PPA'),
            array('codigo' => 'mpncod', 'descricao' => 'C�digo Meta PNC'),
            array('codigo' => 'mpnnome', 'descricao' => 'Meta PNC'),
            array('codigo' => 'ipncod', 'descricao' => 'C�digo Indicador PNC'),
            array('codigo' => 'ipndsc', 'descricao' => 'Indicador PNC'),
            array('codigo' => 'mdedsc', 'descricao' => '�rea Cultural'),
            array('codigo' => 'needsc', 'descricao' => 'Segmento Cultural'),
            array('codigo' => 'esfdsc', 'descricao' => 'Localiza��o'),
            array('codigo' => 'paidescricao', 'descricao' => 'Pa�s'),
            array('codigo' => 'estuf', 'descricao' => 'UF'),
            array('codigo' => 'estdescricao', 'descricao' => 'Estado'),
            array('codigo' => 'munestuf', 'descricao' => 'UF do Munic�pio'),
            array('codigo' => 'mundescricao', 'descricao' => 'Munic�pio'),
            array('codigo' => 'plivalorcusteio', 'descricao' => 'Custeio'),
            array('codigo' => 'plivalorcapital', 'descricao' => 'Capital'),
            array('codigo' => 'pliquantidadeadicional', 'descricao' => 'Adicional de Quantidade'),
            array('codigo' => 'plivalorcusteioadicional', 'descricao' => 'Adicional de Valor Custeio'),
            array('codigo' => 'plivalorcapitaladicional', 'descricao' => 'Adicional de Valor Capital'),
            array('codigo' => 'plijustificativaadicional', 'descricao' => 'Justificativa do Adicional')
        );
        return $colunas;
    }

    /**
     * Monta configura��o de nome e descricao de colunas para o relat�rio Gerencial de Funcionais.
     * 
     * @return array
     */
    function montarColunasRelatorioFuncional()
    {
        $colunas = array(
            array(
                'codigo' => 'unidade',
                'descricao' => 'Unidade'),
            array(
                'codigo' => 'subunidade',
                'descricao' => 'Subunidade'),
            array(
                'codigo' => 'ptres',
                'descricao' => 'PTRES'),
            array(
                'codigo' => 'funcional',
                'descricao' => 'Funcional'),
            array(
                'codigo' => 'custeio_dotacao',
                'descricao' => 'Dota��o/Custeio'),
            array(
                'codigo' => 'custeio_planejado',
                'descricao' => 'Planejado/Custeio'),
            array(
                'codigo' => 'capital_dotacao',
                'descricao' => 'Dota��o/Capital'),
            array(
                'codigo' => 'capital_planejado',
                'descricao' => 'Planejado/Capital'),
            array(
                'codigo' => 'total_dotacao',
                'descricao' => 'Dota��o/Total'),
            array(
                'codigo' => 'total_planejado',
                'descricao' => 'Planejado/Total'),
            array(
                'codigo' => 'provisionado',
                'descricao' => 'Provisionado'),
            array(
                'codigo' => 'empenhado',
                'descricao' => 'Empenhado'),
            array(
                'codigo' => 'liquidado',
                'descricao' => 'Liquidado'),
            array(
                'codigo' => 'pago',
                'descricao' => 'Pago'),
        );
        
        return $colunas;
    }

    /**
     * Monta as Colunas do Relat�rio de PI e PI-FNC do M�dulo Planejamento Or�ament�rio.
     *
     * @return array
     */
    function montarColunasRelatorioPI()
    {
        $colunas = array(
            array('codigo' => 'pliid',              'descricao' => 'ID Planejamento'),
            array('codigo' => 'esddsc',             'descricao' => 'Situa��o'),
            array('codigo' => 'plicod',             'descricao' => 'C�digo PI'),
            array('codigo' => 'unocod',             'descricao' => 'C�digo da UO'),
            array('codigo' => 'unonome',            'descricao' => 'UO'),
            array('codigo' => 'suocod',             'descricao' => 'C�digo da Unidade'),
            array('codigo' => 'suonome',            'descricao' => 'Unidade'),
            array('codigo' => 'unodelegada',        'descricao' => 'Unidade Delegada'),
            array('codigo' => 'suodelegada',        'descricao' => 'Subunidade Delegada'),
            array('codigo' => 'eqddsc',             'descricao' => 'Enquadramento'),
            array('codigo' => 'resultadoprimario',  'descricao' => 'RP - Resultado Prim�rio'),
            array('codigo' => 'mainome',            'descricao' => 'Item de Manuten��o'),
            array('codigo' => 'masnome',            'descricao' => 'Subitem de Manuten��o'),
            array('codigo' => 'prgcod',             'descricao' => 'C�digo do Programa'),
            array('codigo' => 'prgdsc',             'descricao' => 'Programa'),
            array('codigo' => 'oppcod',             'descricao' => 'C�digo do Objetivo'),
            array('codigo' => 'oppnome',            'descricao' => 'Objetivo'),
            array('codigo' => 'ippcod',             'descricao' => 'C�digo da Iniciativa'),
            array('codigo' => 'ippnome',            'descricao' => 'Iniciativa'),
            array('codigo' => 'acacod',             'descricao' => 'C�digo da A��o'),
            array('codigo' => 'acatitulo',          'descricao' => 'A��o'),
            array('codigo' => 'loccod',             'descricao' => 'C�digo do Localizador'),
            array('codigo' => 'locdsc',             'descricao' => 'Localizador'),
            array('codigo' => 'plocod',             'descricao' => 'C�digo do PO'),
            array('codigo' => 'po',                 'descricao' => 'Plano Or�ament�rio'),
            array('codigo' => 'ptres',              'descricao' => 'PTRES'),
            array('codigo' => 'mdedsc',             'descricao' => '�rea'),
            array('codigo' => 'mpnnome',            'descricao' => 'Meta PNC'),
            array('codigo' => 'mppnome',            'descricao' => 'Meta'),
            array('codigo' => 'plititulo',          'descricao' => 'T�tulo'),
            array('codigo' => 'plidsc',             'descricao' => 'Descri��o'),
            array('codigo' => 'needsc',             'descricao' => 'Segmento Cultural'),
            array('codigo' => 'capdsc',             'descricao' => 'Tipo de Instrumento / Modalidade de Pactua��o'),
            array('codigo' => 'pliemenda',          'descricao' => 'Emenda parlamentar'),
            array('codigo' => 'picedital',          'descricao' => 'Forma de execu��o - Edital'),
            array('codigo' => 'esfdsc',             'descricao' => 'Localiza��o da a��o'),
            array('codigo' => 'pais',               'descricao' => 'Pa�s'),
            array('codigo' => 'estado',             'descricao' => 'Unidade Federativa'),
            array('codigo' => 'municipio',          'descricao' => 'Munic�pio'),
            array('codigo' => 'usuario',            'descricao' => 'Respons�vel'),
            array('codigo' => 'ted',                'descricao' => 'TED'),
            array('codigo' => 'edital',             'descricao' => 'Edital'),
            array('codigo' => 'mesdsc',             'descricao' => 'Previs�o Edital'),
            array('codigo' => 'convenio',           'descricao' => 'Conv�nio'),
            array('codigo' => 'sniic',              'descricao' => 'SNIIC'),
            array('codigo' => 'sei',                'descricao' => 'Processo'),
            array('codigo' => 'pronac',             'descricao' => 'PRONAC'),
            array('codigo' => 'pprnome',            'descricao' => 'Produto'),
            array('codigo' => 'pumnome',            'descricao' => 'Unidade de Medida'),
            array('codigo' => 'picquantidade',      'descricao' => 'Quantidade'),
            array('codigo' => 'valortotal',         'descricao' => 'Valor Total'),
            array('codigo' => 'picvalorcusteio',    'descricao' => 'Valor Total Custeio'),
            array('codigo' => 'picvalorcapital',    'descricao' => 'Valor Total Capital'),
            array('codigo' => 'fisico_1',           'descricao' => 'Cronograma F�sico / Capital Janeiro'),
            array('codigo' => 'fisico_2',           'descricao' => 'Cronograma F�sico / Capital Fevereiro'),
            array('codigo' => 'fisico_3',           'descricao' => 'Cronograma F�sico / Capital Mar�o'),
            array('codigo' => 'fisico_4',           'descricao' => 'Cronograma F�sico / Capital Abril'),
            array('codigo' => 'fisico_5',           'descricao' => 'Cronograma F�sico / Capital Maio'),
            array('codigo' => 'fisico_6',           'descricao' => 'Cronograma F�sico / Capital Junho'),
            array('codigo' => 'fisico_7',           'descricao' => 'Cronograma F�sico / Capital Julho'),
            array('codigo' => 'fisico_8',           'descricao' => 'Cronograma F�sico / Capital Agosto'),
            array('codigo' => 'fisico_9',           'descricao' => 'Cronograma F�sico / Capital Setembro'),
            array('codigo' => 'fisico_10',          'descricao' => 'Cronograma F�sico / Capital Outubro'),
            array('codigo' => 'fisico_11',          'descricao' => 'Cronograma F�sico / Capital Novembro'),
            array('codigo' => 'fisico_12',          'descricao' => 'Cronograma F�sico / Capital Dezembro'),
            array('codigo' => 'fin_capital_1',      'descricao' => 'Cronograma Financeiro / Capital Janeiro'),
            array('codigo' => 'fin_capital_2',      'descricao' => 'Cronograma Financeiro / Capital Fevereiro'),
            array('codigo' => 'fin_capital_3',      'descricao' => 'Cronograma Financeiro / Capital Mar�o'),
            array('codigo' => 'fin_capital_4',      'descricao' => 'Cronograma Financeiro / Capital Abril'),
            array('codigo' => 'fin_capital_5',      'descricao' => 'Cronograma Financeiro / Capital Maio'),
            array('codigo' => 'fin_capital_6',      'descricao' => 'Cronograma Financeiro / Capital Junho'),
            array('codigo' => 'fin_capital_7',      'descricao' => 'Cronograma Financeiro / Capital Julho'),
            array('codigo' => 'fin_capital_8',      'descricao' => 'Cronograma Financeiro / Capital Agosto'),
            array('codigo' => 'fin_capital_9',      'descricao' => 'Cronograma Financeiro / Capital Setembro'),
            array('codigo' => 'fin_capital_10',     'descricao' => 'Cronograma Financeiro / Capital Outubro'),
            array('codigo' => 'fin_capital_11',     'descricao' => 'Cronograma Financeiro / Capital Novembro'),
            array('codigo' => 'fin_capital_12',     'descricao' => 'Cronograma Financeiro / Capital Dezembro'),
            array('codigo' => 'fin_custeio_1',      'descricao' => 'Cronograma Financeiro / Custeio Janeiro'),
            array('codigo' => 'fin_custeio_2',      'descricao' => 'Cronograma Financeiro / Custeio Fevereiro'),
            array('codigo' => 'fin_custeio_3',      'descricao' => 'Cronograma Financeiro / Custeio Mar�o'),
            array('codigo' => 'fin_custeio_4',      'descricao' => 'Cronograma Financeiro / Custeio Abril'),
            array('codigo' => 'fin_custeio_5',      'descricao' => 'Cronograma Financeiro / Custeio Maio'),
            array('codigo' => 'fin_custeio_6',      'descricao' => 'Cronograma Financeiro / Custeio Junho'),
            array('codigo' => 'fin_custeio_7',      'descricao' => 'Cronograma Financeiro / Custeio Julho'),
            array('codigo' => 'fin_custeio_8',      'descricao' => 'Cronograma Financeiro / Custeio Agosto'),
            array('codigo' => 'fin_custeio_9',      'descricao' => 'Cronograma Financeiro / Custeio Setembro'),
            array('codigo' => 'fin_custeio_10',     'descricao' => 'Cronograma Financeiro / Custeio Outubro'),
            array('codigo' => 'fin_custeio_11',     'descricao' => 'Cronograma Financeiro / Custeio Novembro'),
            array('codigo' => 'fin_custeio_12',     'descricao' => 'Cronograma Financeiro / Custeio Dezembro'),
            array('codigo' => 'orc_capital_1',      'descricao' => 'Cronograma Or�amentario / Capital Janeiro'),
            array('codigo' => 'orc_capital_2',      'descricao' => 'Cronograma Or�amentario / Capital Fevereiro'),
            array('codigo' => 'orc_capital_3',      'descricao' => 'Cronograma Or�amentario / Capital Mar�o'),
            array('codigo' => 'orc_capital_4',      'descricao' => 'Cronograma Or�amentario / Capital Abril'),
            array('codigo' => 'orc_capital_5',      'descricao' => 'Cronograma Or�amentario / Capital Maio'),
            array('codigo' => 'orc_capital_6',      'descricao' => 'Cronograma Or�amentario / Capital Junho'),
            array('codigo' => 'orc_capital_7',      'descricao' => 'Cronograma Or�amentario / Capital Julho'),
            array('codigo' => 'orc_capital_8',      'descricao' => 'Cronograma Or�amentario / Capital Agosto'),
            array('codigo' => 'orc_capital_9',      'descricao' => 'Cronograma Or�amentario / Capital Setembro'),
            array('codigo' => 'orc_capital_10',     'descricao' => 'Cronograma Or�amentario / Capital Outubro'),
            array('codigo' => 'orc_capital_11',     'descricao' => 'Cronograma Or�amentario / Capital Novembro'),
            array('codigo' => 'orc_capital_12',     'descricao' => 'Cronograma Or�amentario / Capital Dezembro'),
            array('codigo' => 'orc_custeio_1',      'descricao' => 'Cronograma Or�amentario / Custeio Janeiro'),
            array('codigo' => 'orc_custeio_2',      'descricao' => 'Cronograma Or�amentario / Custeio Fevereiro'),
            array('codigo' => 'orc_custeio_3',      'descricao' => 'Cronograma Or�amentario / Custeio Mar�o'),
            array('codigo' => 'orc_custeio_4',      'descricao' => 'Cronograma Or�amentario / Custeio Abril'),
            array('codigo' => 'orc_custeio_5',      'descricao' => 'Cronograma Or�amentario / Custeio Maio'),
            array('codigo' => 'orc_custeio_6',      'descricao' => 'Cronograma Or�amentario / Custeio Junho'),
            array('codigo' => 'orc_custeio_7',      'descricao' => 'Cronograma Or�amentario / Custeio Julho'),
            array('codigo' => 'orc_custeio_8',      'descricao' => 'Cronograma Or�amentario / Custeio Agosto'),
            array('codigo' => 'orc_custeio_9',      'descricao' => 'Cronograma Or�amentario / Custeio Setembro'),
            array('codigo' => 'orc_custeio_10',     'descricao' => 'Cronograma Or�amentario / Custeio Outubro'),
            array('codigo' => 'orc_custeio_11',     'descricao' => 'Cronograma Or�amentario / Custeio Novembro'),
            array('codigo' => 'orc_custeio_12',     'descricao' => 'Cronograma Or�amentario / Custeio Dezembro'),
            array('codigo' => 'vlrautorizado',      'descricao' => 'Valor Provisionado'),
            array('codigo' => 'vlrempenhado',       'descricao' => 'Valor Empenhado'),
            array('codigo' => 'vlrliquidado',         'descricao' => 'Valor Liquidado'),
            array('codigo' => 'vlrpago',            'descricao' => 'Valor Pago'),
            array('codigo' => 'executado',          'descricao' => 'Executado'),
            array('codigo' => 'analise',            'descricao' => 'An�lise Situacional'),
            array('codigo' => 'classificacao',      'descricao' => 'Classifica��o'),
            array('codigo' => 'medidas',            'descricao' => 'Medidas a serem adotadas'),
            array('codigo' => 'providencias',       'descricao' => 'Detalhamento das provid�ncias a serem adotadas')
        );
        return $colunas;
    }

    /**
     * Monta as Colunas que receber�o formata��o de moeda para o Relat�rio de PI e PI-FNC do M�dulo Planejamento Or�ament�rio.
     *
     * @return array
     */
    function montarColunasFormatoMoeda(){
        $colunas = [
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
            'vlrpago',
        ];

        return $colunas;
    }
