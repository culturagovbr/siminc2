<?php

class Municipio extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "territorios.municipio";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("muncod");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'muncod' => null,
        'estuf' => null,
        'miccod' => null,
        'mescod' => null,
        'mundescricao' => null,
        'munprocesso' => null,
        'muncodcompleto' => null,
        'munmedlat' => null,
        'munmedlog' => null,
        'munhemis' => null,
        'munaltitude' => null,
        'munmedarea' => null,
        'muncepmenor' => null,
        'muncepmaior' => null,
        'munmedraio' => null,
        'munmerid' => null,
        'muncodsiafi' => null,
        'munpopulacao' => null,
    );

    function buscaMunicipioPorObra($endid) {
        $sql = "
            SELECT
                    mundescricao
            FROM entidade.endereco e
                JOIN {$this->stNomeTabela} m USING(muncod)
            WHERE
                e.endid = {$endid}
            LIMIT 1";

        return $this->pegaUm($sql);
    }

    function buscaPopulacao(Array $arParam) {
        $where = array();
        if ($arParam['muncod']) {
            $where[] = "muncod = '" . $arParam['muncod'] . "'";
        }

        $sql = "
            SELECT
                munpopulacao
            FROM {$this->stNomeTabela}
            " . (count($where) > 0 ? "WHERE " . implode(' AND ', $where) : "");

        return $this->pegaUm($sql);
    }

    function listaCombo(Array $param = array()) {
        $where = array();
        switch (true) {
            case!empty($param['estuf']):
                $where[] = "estuf = '{$param['estuf']}'";
                break;
        }

        $sql = "
            SELECT
                muncod AS codigo,
                mundescricao AS descricao
            FROM territorios.municipio " .
        (count($where) ? " WHERE " . implode(" AND ", $where) : "") . "
            ORDER BY
                mundescricao";

        $dados = $this->carregar($sql);

        return (is_array($dados) ? $dados : array());
    }

    function listaComboMulti($param) {
        if (!is_array($param)) {
            return array();
        }
        $where = array();

        foreach ($param as $key) {
            $where[] = "estuf = '{$key}'";
        }

        $sql = "
            SELECT
                muncod AS codigo,
                mundescricao AS descricao
            FROM territorios.municipio 
            " .(count($where) ? " WHERE " . implode(" or ", $where) : "") . "
            ORDER BY
                mundescricao";

        $dados = $this->carregar($sql);

        return (is_array($dados) ? $dados : array());
    }

    public function listaMunicipiosMultiplosUfs(Array $params) {
        $where = '';
        if (strlen($params['estuf'])) {
            $params['estuf'] = explode(',', $params['estuf']);
            foreach ($params['estuf'] as $estuf) {
                $where .= "'{$estuf}',";
            }
            $where = substr($where, 0, -1);
        }

        $sql = "
            SELECT
                muncod AS codigo,
                mundescricao AS descricao
            FROM territorios.municipio
            WHERE
                estuf IN ({$where})
            ORDER BY muncod";

        return $sql;
    }

}
