<?php
/**
 * Classe de mapeamento da entidade territorios.esfera
 *
 * @category Class
 * @package  A1
 * @author   EDUARDO DUNICE NETO <eduardo.neto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 13-10-2015
 * @link     no link
 */



/**
 * Territorios_Model_Regiao
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release:
 * @link     no link
 */
class Territorios_Model_Esfera extends Modelo
{
    const K_FEDERAL = 1;
    const K_ESTADUAL = 2;
    const K_MUNICIPAL = 3;
    const K_EXTERIOR = 4;

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'territorios.esfera';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'esfid',
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'esfid' => null,
        'esfdsc' => null,
    );
}
