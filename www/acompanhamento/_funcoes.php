<?php

    /**
     * Monta a combo de UGs filtrando por UO
     *
     * @param $filtros
     * @return VOID
     */
    function montarComboUG(stdClass $filtros) {
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
