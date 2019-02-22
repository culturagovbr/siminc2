<?php
/**
 * Classe de mapeamento da entidade spo.ptressubunidade
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 05-09-2017
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Spo_Model_Ptressubunidade
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Spo_Model_PtresSubunidade extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'spo.ptressubunidade';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'psuid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'suoid' => array('tabela' => 'subunidadeorcamentaria', 'pk' => 'suoid'),
        'ptrid' => array('tabela' => 'ptres', 'pk' => 'ptrid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'psuid' => null,
        'ptrid' => null,
        'suoid' => null,
        'ptrdotacaocusteio' => null,
        'ptrdotacaocapital' => null,
    );

    public function recuperarPorExercicio($exercicio)
    {
        $sql = "select psu.*, suo.unofundo
                from spo.ptressubunidade psu
                        inner join monitora.ptres p on p.ptrid = psu.ptrid
                        inner join public.vw_subunidadeorcamentaria suo on suo.suoid = psu.suoid
                where p.ptrano = '$exercicio'
                and p.ptrstatus = 'A'";

        $dados = $this->carregar($sql);
        $dados = $dados ? $dados : [];

        $dadosAgrupados = [];
        foreach($dados as $dado){
            $dadosAgrupados[$dado['ptrid']][] = $dado['suoid'];
            $dadosAgrupados[$dado['ptrid']][$dado['unofundo']] = $dado['unofundo'];
        }
        
        return $dadosAgrupados;

    }
    
    public function recuperarPorUnidade($suoid)
    {
        $sql = "select ptr.funcional, ptr.plodsc
                from spo.ptressubunidade psu
                        inner join monitora.vw_ptres ptr on ptr.ptrid = psu.ptrid
                where suoid = " . (int) $suoid . "
                order by funcional";

        return $this->carregar($sql);
    }

}//end Class
?>