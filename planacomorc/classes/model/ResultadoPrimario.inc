<?php

/**
 * Classe de mapeamento da entidade planacomorc.pi_delegacao
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 21-08-2017
 * @link     no link
 */

require_once APPRAIZ .'includes/classes/Modelo.class.inc';

/**
 * Planacomorc_Model_Pi_delegacao
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release:
 * @link     no link
 */
class Planacomorc_Model_ResultadoPrimario extends Modelo
{
    const FINANCEIRA = 0;
    
    const PRIMARIA_OBRIGATORIA = 1;
    
    const PRIMARIA_DISCRICIONARIA = 2;
    
    const PAC = 3;
    
    const EMENDA = 6;
    
    const NAO_IMPACTA = 9;
    
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'planacomorc.resultadoprimario';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'rpcod',
    );
    protected $arChaveEstrangeira = array(
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'rpcod' => null,
        'redsc' => null,
    );

}//end Class
?>
