<?php
/**
 * Classe de mapeamento da entidade alteracao.pedido
 *
 * @category Class
 * @package  A1
 * @author   RAFAEL FREITAS CARNEIRO <rafael.carneiro@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 13-04-2018
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Alteracao_Model_PlanoInternoSelecionado
 *
 * @category Class
 * @package  A1
 * @author   RAFAEL FREITAS CARNEIRO <rafael.carneiro@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Alteracao_Model_PlanoInternoSelecionado extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'alteracao.plano_interno_selecionado';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'pliselid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'pedid' => array('tabela' => 'alteracao.pedido', 'pk' => 'pedid'),
        'pliid' => array('tabela' => 'monitora.pi_planointerno', 'pk' => 'pliid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'pliselid' => null,
        'pedid' => null,
        'pliid' => null,
        'vlsaldoempenhocusteio' => null,
        'vlsaldoempenhocapital' => null,
        'vlsuplementarcusteio' => null,
        'vlsuplementarcapital' => null,
        'vlcancelarcusteio' => null,
        'vlcancelarcapital' => null,
        'vlsuplementarexcessocusteio' => null,
        'vlsuplementarexcessocapital' => null,
        'vlsuplementarsuperavitcusteio' => null,
        'vlsuplementarsuperavitcapital' => null,
        'vldotacaocusteio' => null,
        'vldotacaocapital' => null,
        'plisuplementarquantidade' => null,
        'plicancelarquantidade' => null,
    );
}
