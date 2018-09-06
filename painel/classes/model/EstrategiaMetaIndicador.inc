<?php
/**
 * Classe de mapeamento da entidade painel.estrategia_meta_indicador
 *
 * @category Class
 * @package  A1
 * @author   RAFAEL FREITAS CARNEIRO <rafael.carneiro@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 03-09-2018
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Public_Model_Gnd
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Painel_Model_Estrategia_Meta_Indicador extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'painel.estrategia_meta_indicador';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'emiid',
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
        'emiid' => null,
        'indid' => null,
        'emidsc' => null,
    );

    public function retornaSqlEtrategiaMetas($indid){
        $sql = "SELECT DISTINCT '<center>
                                     <a href=\"painel.php?modulo=principal/cadastroEstrategiaMetas&acao=A&req=alterar&emiid='||emiid||'&indid='||indid||'&evento=A\"><img src=\"/imagens/alterar.gif \" border=0 title=\"Visualizar\"></a>
                                     <a href=\"painel.php?modulo=principal/cadastroEstrategiaMetas&acao=A&req=excluir&emiid='||emiid||'&evento=A\"><img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\"></a>
                                 </center>',
		       emidsc
	    FROM painel.estrategia_meta_indicador
	    where indid = $indid
	    ORDER BY emidsc";       
        return $sql;
    }
    
    public function carregarPorIndid($indid){
        $sql = 'select emidsc from painel.estrategia_meta_indicador where indid = '.$indid;
        return $this->carregar($sql);
    }
}//end Class
?>