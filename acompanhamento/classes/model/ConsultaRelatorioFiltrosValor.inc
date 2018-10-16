<?php
/**
 * Classe de mapeamento da entidade acompanhamento.consulta_relatorio_filtros_valor
 *
 * @category Class
 * @package  A1
 * @author   RAFAEL FREITAS CARNEIRO<rafael.carneiro@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 11-10-2018
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Acompanhamento_Model_Consulta_Relatorio_Filtros_Valor
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Acompanhamento_Model_ConsultaRelatorioFiltrosValor extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'acompanhamento.consulta_relatorio_filtros_valor';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'crfvid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'crfid' => array('tabela' => 'acompanhamento.consulta_relatorio_filtros', 'pk' => 'crfid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'crfvid' => null,
        'crfid' => null,
        'vlcampo' => null,
    );

    public function selecionaValores($crid){
        $sql = "select dscampo, vlcampo 
                  from acompanhamento.consulta_relatorio_filtros_valor crfv
                 inner join acompanhamento.consulta_relatorio_filtros crf
                    on crfv.crfid = crf.crfid
                 where crf.crid = $crid";
        $arrRetorno = Array();
        $result = $this->carregar($sql);

        foreach($result as $key=>$value){
            $arrRetorno[$value['dscampo']][]=$value['vlcampo'];
        }
        return $arrRetorno;
    }
    
    public function removeFiltrosValores($crid){
        $sql = "delete from acompanhamento.consulta_relatorio_filtros_valor where crfid in (select crfid from acompanhamento.consulta_relatorio_filtros where crid = $crid)";
        $this->executar($sql);
    }
}
