<?php
/**
 * Classe de mapeamento da entidade planejamento.categoria_apropriacao
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 17-08-2017
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Planacomorc_Model_CategoriaApropriacao
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Planacomorc_Model_CategoriaApropriacao extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'planejamento.categoria_apropriacao';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'capid',
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'capid' => null,
        'capcod' => null,
        'capdsc' => null,
        'capano' => null,
        'capstatus' => null,
        'capsiconv' => null,
    );

    public function existeVinculo($dados = null)
    {
        $sql = "
            SELECT
                count(*) qtd
            FROM monitora.pi_planointerno pli
            WHERE capid = {$this->capid}
        ";
        return $this->pegaUm($sql);
    }

}
