<?php
/**
 * Classe de mapeamento da entidade territorios.estado.
 *
 * $Id$
 */

/**
 * Mapeamento da entidade territorios.estado.
 *
 * @see Modelo
 */
class Territorios_Model_Estado extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'territorios.estado';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'estuf',
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'regcod' => array(
            'tabela' => 'territorios.regiao', 'pk' => 'regcod'
        ),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'estuf' => null,
        'muncodcapital' => null,
        'regcod' => null,
        'estcod' => null,
        'estdescricao' => null,
    );


    /**
     * Função pegarSQLSelect
     *
     * @return query para montar componentes.
     */
    public static function pegarSQLSelect($arrPost)
    {
        $where = '';

        if ($arrPost['regcod'][0] != '') $where = "WHERE e.regcod IN ('".implode("', '", $arrPost['regcod'])."')";

        $sql = "
            SELECT
                e.estuf as codigo,
                e.estdescricao as descricao
            FROM territorios.estado e
            $where
            ORDER BY
                e.estdescricao ASC
        ";

        return $sql;

    }


}
