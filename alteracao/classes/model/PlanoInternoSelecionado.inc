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
        'vlcusteio' => null,
        'vlcapital' => null,
        'vlfisico' => null,
        'vlsuplementarcusteio' => null,
        'vlsuplementarcapital' => null,
        'vlsuplementarfisico' => null,
        'vlcancelarcusteio' => null,
        'vlcancelarcapital' => null,
        'vlcancelarfisico' => null,
        'vlsuplementarexcessocusteio' => null,
        'vlsuplementarexcessocapital' => null,
        'vlsuplementarexcessofisico' => null,
        'vlsuplementarsuperavitcusteio' => null,
        'vlsuplementarsuperavitcapital' => null,
        'vlsuplementarsuperavitfisico' => null,
        'vldotacaocusteio' => null,
        'vldotacaocapital' => null,
        'vldotacaofisico' => null,
        'pliselstatus' => null,
    );
    
    function RetornaValoresPI($pliid){
        $sql = "SELECT pc.picvalorcusteio AS custeio,
		       pc.picvalorcapital AS capital,
                       pc.picquantidade as fisico 
                  from planacomorc.pi_complemento pc
                 where pc.pliid = ".$pliid;
        return $this->pegaLinha($sql);
    }
}
