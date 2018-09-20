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
