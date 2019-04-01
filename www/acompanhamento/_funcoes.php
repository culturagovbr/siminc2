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
     * Monta as Colunas do Relatório de Pré PI do Módulo Proposta Orçamentária.
     *
     * @return array
     */
    function montarColunasRelatorioPrePI()
    {
        $colunas = array(
            array('codigo' => 'pliid', 'descricao' => 'Id Pré-PI'),
            array('codigo' => 'plititulo', 'descricao' => 'Título'),
            array('codigo' => 'plidsc', 'descricao' => 'Descrição/Finalidade'),
            array('codigo' => 'subunidade', 'descricao' => 'Subunidade'),
            array('codigo' => 'eqddsc', 'descricao' => 'Enquadramento da Despesa'),
            array('codigo' => 'irpcod', 'descricao' => 'RP'),
            array('codigo' => 'mainome', 'descricao' => 'Item'),
            array('codigo' => 'masnome', 'descricao' => 'Subitem'),
            array('codigo' => 'funcional', 'descricao' => 'Funcional'),
            array('codigo' => 'acatitulo', 'descricao' => 'Ação'),
            array('codigo' => 'plodsc', 'descricao' => 'PO'),
            array('codigo' => 'esddsc', 'descricao' => 'Situação'),
            array('codigo' => 'pprnome', 'descricao' => 'Produto'),
            array('codigo' => 'pumdescricao', 'descricao' => 'Unidade de Medida'),
            array('codigo' => 'pliquantidade', 'descricao' => 'Quantidade'),
            array('codigo' => 'oppcod', 'descricao' => 'Código Objetivo PPA'),
            array('codigo' => 'oppdsc', 'descricao' => 'Objetivo PPA'),
            array('codigo' => 'mppcod', 'descricao' => 'Código Metas PPA'),
            array('codigo' => 'mppnome', 'descricao' => 'Metas PPA'),
            array('codigo' => 'ippcod', 'descricao' => 'Código Iniciativa PPA'),
            array('codigo' => 'ippnome', 'descricao' => 'Iniciativa PPA'),
            array('codigo' => 'mpncod', 'descricao' => 'Código Meta PNC'),
            array('codigo' => 'mpnnome', 'descricao' => 'Meta PNC'),
            array('codigo' => 'ipncod', 'descricao' => 'Código Indicador PNC'),
            array('codigo' => 'ipndsc', 'descricao' => 'Indicador PNC'),
            array('codigo' => 'mdedsc', 'descricao' => 'Área Cultural'),
            array('codigo' => 'needsc', 'descricao' => 'Segmento Cultural'),
            array('codigo' => 'esfdsc', 'descricao' => 'Localização'),
            array('codigo' => 'paidescricao', 'descricao' => 'País'),
            array('codigo' => 'estuf', 'descricao' => 'UF'),
            array('codigo' => 'estdescricao', 'descricao' => 'Estado'),
            array('codigo' => 'munestuf', 'descricao' => 'UF do Município'),
            array('codigo' => 'mundescricao', 'descricao' => 'Município'),
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
     * Monta configuração de nome e descricao de colunas para o relatório Gerencial de Funcionais.
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
                'descricao' => 'Dotação/Custeio'),
            array(
                'codigo' => 'custeio_planejado',
                'descricao' => 'Planejado/Custeio'),
            array(
                'codigo' => 'capital_dotacao',
                'descricao' => 'Dotação/Capital'),
            array(
                'codigo' => 'capital_planejado',
                'descricao' => 'Planejado/Capital'),
            array(
                'codigo' => 'total_dotacao',
                'descricao' => 'Dotação/Total'),
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
     * Monta as Colunas do Relatório de PI e PI-FNC do Módulo Planejamento Orçamentário.
     *
     * @return array
     */
    function montarColunasRelatorioPI()
    {
        $colunas = array(
            array('codigo' => 'pliid',              'descricao' => 'ID Planejamento'),
            array('codigo' => 'esddsc',             'descricao' => 'Situação'),
            array('codigo' => 'plicod',             'descricao' => 'Código PI'),
            array('codigo' => 'unocod',             'descricao' => 'Código da UO'),
            array('codigo' => 'unonome',            'descricao' => 'UO'),
            array('codigo' => 'suocod',             'descricao' => 'Código da Unidade'),
            array('codigo' => 'suonome',            'descricao' => 'Unidade'),
            array('codigo' => 'unodelegada',        'descricao' => 'Unidade Delegada'),
            array('codigo' => 'suodelegada',        'descricao' => 'Subunidade Delegada'),
            array('codigo' => 'eqddsc',             'descricao' => 'Enquadramento'),
            array('codigo' => 'resultadoprimario',  'descricao' => 'RP - Resultado Primário'),
            array('codigo' => 'mainome',            'descricao' => 'Item de Manutenção'),
            array('codigo' => 'masnome',            'descricao' => 'Subitem de Manutenção'),
            array('codigo' => 'prgcod',             'descricao' => 'Código do Programa'),
            array('codigo' => 'prgdsc',             'descricao' => 'Programa'),
            array('codigo' => 'oppcod',             'descricao' => 'Código do Objetivo'),
            array('codigo' => 'oppnome',            'descricao' => 'Objetivo'),
            array('codigo' => 'ippcod',             'descricao' => 'Código da Iniciativa'),
            array('codigo' => 'ippnome',            'descricao' => 'Iniciativa'),
            array('codigo' => 'acacod',             'descricao' => 'Código da Ação'),
            array('codigo' => 'acatitulo',          'descricao' => 'Ação'),
            array('codigo' => 'loccod',             'descricao' => 'Código do Localizador'),
            array('codigo' => 'locdsc',             'descricao' => 'Localizador'),
            array('codigo' => 'plocod',             'descricao' => 'Código do PO'),
            array('codigo' => 'po',                 'descricao' => 'Plano Orçamentário'),
            array('codigo' => 'ptres',              'descricao' => 'PTRES'),
            array('codigo' => 'mdedsc',             'descricao' => 'Área'),
            array('codigo' => 'mpnnome',            'descricao' => 'Meta PNC'),
            array('codigo' => 'mppnome',            'descricao' => 'Meta'),
            array('codigo' => 'plititulo',          'descricao' => 'Título'),
            array('codigo' => 'plidsc',             'descricao' => 'Descrição'),
            array('codigo' => 'needsc',             'descricao' => 'Segmento Cultural'),
            array('codigo' => 'capdsc',             'descricao' => 'Tipo de Instrumento / Modalidade de Pactuação'),
            array('codigo' => 'pliemenda',          'descricao' => 'Emenda parlamentar'),
            array('codigo' => 'picedital',          'descricao' => 'Forma de execução - Edital'),
            array('codigo' => 'esfdsc',             'descricao' => 'Localização da ação'),
            array('codigo' => 'pais',               'descricao' => 'País'),
            array('codigo' => 'estado',             'descricao' => 'Unidade Federativa'),
            array('codigo' => 'municipio',          'descricao' => 'Município'),
            array('codigo' => 'usuario',            'descricao' => 'Responsável'),
            array('codigo' => 'ted',                'descricao' => 'TED'),
            array('codigo' => 'edital',             'descricao' => 'Edital'),
            array('codigo' => 'mesdsc',             'descricao' => 'Previsão Edital'),
            array('codigo' => 'convenio',           'descricao' => 'Convênio'),
            array('codigo' => 'sniic',              'descricao' => 'SNIIC'),
            array('codigo' => 'sei',                'descricao' => 'Processo'),
            array('codigo' => 'pronac',             'descricao' => 'PRONAC'),
            array('codigo' => 'pprnome',            'descricao' => 'Produto'),
            array('codigo' => 'pumnome',            'descricao' => 'Unidade de Medida'),
            array('codigo' => 'picquantidade',      'descricao' => 'Quantidade'),
            array('codigo' => 'valortotal',         'descricao' => 'Valor Total'),
            array('codigo' => 'picvalorcusteio',    'descricao' => 'Valor Total Custeio'),
            array('codigo' => 'picvalorcapital',    'descricao' => 'Valor Total Capital'),
            array('codigo' => 'fisico_1',           'descricao' => 'Cronograma Físico / Capital Janeiro'),
            array('codigo' => 'fisico_2',           'descricao' => 'Cronograma Físico / Capital Fevereiro'),
            array('codigo' => 'fisico_3',           'descricao' => 'Cronograma Físico / Capital Março'),
            array('codigo' => 'fisico_4',           'descricao' => 'Cronograma Físico / Capital Abril'),
            array('codigo' => 'fisico_5',           'descricao' => 'Cronograma Físico / Capital Maio'),
            array('codigo' => 'fisico_6',           'descricao' => 'Cronograma Físico / Capital Junho'),
            array('codigo' => 'fisico_7',           'descricao' => 'Cronograma Físico / Capital Julho'),
            array('codigo' => 'fisico_8',           'descricao' => 'Cronograma Físico / Capital Agosto'),
            array('codigo' => 'fisico_9',           'descricao' => 'Cronograma Físico / Capital Setembro'),
            array('codigo' => 'fisico_10',          'descricao' => 'Cronograma Físico / Capital Outubro'),
            array('codigo' => 'fisico_11',          'descricao' => 'Cronograma Físico / Capital Novembro'),
            array('codigo' => 'fisico_12',          'descricao' => 'Cronograma Físico / Capital Dezembro'),
            array('codigo' => 'fin_capital_1',      'descricao' => 'Cronograma Financeiro / Capital Janeiro'),
            array('codigo' => 'fin_capital_2',      'descricao' => 'Cronograma Financeiro / Capital Fevereiro'),
            array('codigo' => 'fin_capital_3',      'descricao' => 'Cronograma Financeiro / Capital Março'),
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
            array('codigo' => 'fin_custeio_3',      'descricao' => 'Cronograma Financeiro / Custeio Março'),
            array('codigo' => 'fin_custeio_4',      'descricao' => 'Cronograma Financeiro / Custeio Abril'),
            array('codigo' => 'fin_custeio_5',      'descricao' => 'Cronograma Financeiro / Custeio Maio'),
            array('codigo' => 'fin_custeio_6',      'descricao' => 'Cronograma Financeiro / Custeio Junho'),
            array('codigo' => 'fin_custeio_7',      'descricao' => 'Cronograma Financeiro / Custeio Julho'),
            array('codigo' => 'fin_custeio_8',      'descricao' => 'Cronograma Financeiro / Custeio Agosto'),
            array('codigo' => 'fin_custeio_9',      'descricao' => 'Cronograma Financeiro / Custeio Setembro'),
            array('codigo' => 'fin_custeio_10',     'descricao' => 'Cronograma Financeiro / Custeio Outubro'),
            array('codigo' => 'fin_custeio_11',     'descricao' => 'Cronograma Financeiro / Custeio Novembro'),
            array('codigo' => 'fin_custeio_12',     'descricao' => 'Cronograma Financeiro / Custeio Dezembro'),
            array('codigo' => 'orc_capital_1',      'descricao' => 'Cronograma Orçamentario / Capital Janeiro'),
            array('codigo' => 'orc_capital_2',      'descricao' => 'Cronograma Orçamentario / Capital Fevereiro'),
            array('codigo' => 'orc_capital_3',      'descricao' => 'Cronograma Orçamentario / Capital Março'),
            array('codigo' => 'orc_capital_4',      'descricao' => 'Cronograma Orçamentario / Capital Abril'),
            array('codigo' => 'orc_capital_5',      'descricao' => 'Cronograma Orçamentario / Capital Maio'),
            array('codigo' => 'orc_capital_6',      'descricao' => 'Cronograma Orçamentario / Capital Junho'),
            array('codigo' => 'orc_capital_7',      'descricao' => 'Cronograma Orçamentario / Capital Julho'),
            array('codigo' => 'orc_capital_8',      'descricao' => 'Cronograma Orçamentario / Capital Agosto'),
            array('codigo' => 'orc_capital_9',      'descricao' => 'Cronograma Orçamentario / Capital Setembro'),
            array('codigo' => 'orc_capital_10',     'descricao' => 'Cronograma Orçamentario / Capital Outubro'),
            array('codigo' => 'orc_capital_11',     'descricao' => 'Cronograma Orçamentario / Capital Novembro'),
            array('codigo' => 'orc_capital_12',     'descricao' => 'Cronograma Orçamentario / Capital Dezembro'),
            array('codigo' => 'orc_custeio_1',      'descricao' => 'Cronograma Orçamentario / Custeio Janeiro'),
            array('codigo' => 'orc_custeio_2',      'descricao' => 'Cronograma Orçamentario / Custeio Fevereiro'),
            array('codigo' => 'orc_custeio_3',      'descricao' => 'Cronograma Orçamentario / Custeio Março'),
            array('codigo' => 'orc_custeio_4',      'descricao' => 'Cronograma Orçamentario / Custeio Abril'),
            array('codigo' => 'orc_custeio_5',      'descricao' => 'Cronograma Orçamentario / Custeio Maio'),
            array('codigo' => 'orc_custeio_6',      'descricao' => 'Cronograma Orçamentario / Custeio Junho'),
            array('codigo' => 'orc_custeio_7',      'descricao' => 'Cronograma Orçamentario / Custeio Julho'),
            array('codigo' => 'orc_custeio_8',      'descricao' => 'Cronograma Orçamentario / Custeio Agosto'),
            array('codigo' => 'orc_custeio_9',      'descricao' => 'Cronograma Orçamentario / Custeio Setembro'),
            array('codigo' => 'orc_custeio_10',     'descricao' => 'Cronograma Orçamentario / Custeio Outubro'),
            array('codigo' => 'orc_custeio_11',     'descricao' => 'Cronograma Orçamentario / Custeio Novembro'),
            array('codigo' => 'orc_custeio_12',     'descricao' => 'Cronograma Orçamentario / Custeio Dezembro'),
            array('codigo' => 'vlrautorizado',      'descricao' => 'Valor Provisionado'),
            array('codigo' => 'vlrempenhado',       'descricao' => 'Valor Empenhado'),
            array('codigo' => 'vlrliquidado',         'descricao' => 'Valor Liquidado'),
            array('codigo' => 'vlrpago',            'descricao' => 'Valor Pago'),
            array('codigo' => 'executado',          'descricao' => 'Executado'),
            array('codigo' => 'analise',            'descricao' => 'Análise Situacional'),
            array('codigo' => 'classificacao',      'descricao' => 'Classificação'),
            array('codigo' => 'medidas',            'descricao' => 'Medidas a serem adotadas'),
            array('codigo' => 'providencias',       'descricao' => 'Detalhamento das providências a serem adotadas')
        );
        return $colunas;
    }

    /**
     * Monta as Colunas que receberão formatação de moeda para o Relatório de PI e PI-FNC do Módulo Planejamento Orçamentário.
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
