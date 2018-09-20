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
