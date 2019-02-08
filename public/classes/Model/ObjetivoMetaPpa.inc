<?php
/**
 * Classe de mapeamento da entidade public.objetivometappa
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 31-05-2017
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Public_Model_Objetivometappa
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Public_Model_ObjetivoMetaPpa extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'public.objetivometappa';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'opmid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'oppid' => array('tabela' => 'objetivoppa', 'pk' => 'oppid'),
        'mppid' => array('tabela' => 'metappa', 'pk' => 'mppid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'opmid' => null,
        'oppid' => null,
        'mppid' => null,
        'mpodata' => null,
    );

    public function recuperarPorExercicio($exercicio, $oppid)
    {
        $sql = "
            SELECT
                m.mppid as codigo
            FROM public.metappa AS m
            JOIN public.objetivometappa AS om ON (m.mppid = om.mppid)
            JOIN public.objetivoppa AS o on (om.oppid = o.oppid)
            WHERE
                o.prsano = '". (int)$exercicio ."'
                AND o.oppid = '". (int)$oppid ."'
        ";
        $dados = $this->carregar($sql);
        $dados = $dados ? $dados : [];

        foreach ($dados as $value){
            $dadosAgrupados[] = $value['codigo'];
        }
        return $dadosAgrupados;
    }

    public function excluirPorObjetivo($oppid)
    {
        $sql = "delete from ".$this->stNomeTabela." where oppid = ".$oppid;
        return $this->executar($sql);
    }
    
    public function salvarMetas($mppid, $oppid){
        $sql = "insert into ".$this->stNomeTabela." (oppid, mppid, mpodata) values(".$oppid.", ".$mppid.", now())";
        return $this->executar($sql);
    }
}//end Class
?>