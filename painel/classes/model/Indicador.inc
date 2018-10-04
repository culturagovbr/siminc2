<?php
/**
 * Classe de mapeamento da entidade painel.indicador
 *
 * @category Class
 * @package  A1
 * @author   <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 20-11-2017
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Painel_Model_Indicador
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Painel_Model_Indicador extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'painel.indicador';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'indid',
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
        'indid' => null,
        'unmid' => null,
        'cliid' => null,
        'tpiid' => null,
        'secid' => null,
        'acaid' => null,
        'perid' => null,
        'estid' => null,
        'colid' => null,
        'usucpf' => null,
        'indnome' => null,
        'indobjetivo' => null,
        'indformula' => null,
        'indtermos' => null,
        'indfontetermo' => null,
        'indnormalinicio' => null,
        'indnormalfinal' => null,
        'indatencaoinicio' => null,
        'indatencaofinal' => null,
        'indcriticoinicio' => null,
        'indcriticofinal' => null,
        'indmetavalor' => null,
        'indmetadata' => null,
        'indstatus' => null,
        'indmetadatalimite' => null,
        'indmetavalorlimite' => null,
        'umeid' => null,
        'regid' => null,
        'exoid' => null,
        'indqtdevalor' => null,
        'indpermiteagregacao' => null,
        'secidgestora' => null,
        'indpublicado' => null,
        'indescala' => null,
        'indobservacao' => null,
        'indvispadrao' => null,
        'indrelatorio' => null,
        'peridatual' => null,
        'indcumulativovalor' => null,
        'indcumulativo' => null,
        'indpublico' => null,
        'mtiid' => null,
        'indhomologado' => null,
        'indordempainel' => null,
        'indapelido' => null,
        'indshformula' => null,
        'formulash' => null,
        'indencerrado' => null,
        'indavalgestor' => null,
        'indobsgestor' => null,
    );

    public function RetornaPerfil(){
        $sql = "select pflcod from seguranca.perfilusuario where usucpf = '".$_SESSION['usucpf']."'";
        $lista = $this->carregar($sql);
        $retorno;
        foreach($lista as $dado){
            $retorno[]=$dado['pflcod'];
        }
        return $retorno;
    }
    
    public function RetornaRegistro($indid){
        $sql = " SELECT * FROM $this->stNomeTabela WHERE {$this->arChavePrimaria[0]} = '$indid'; ";
        return $this->pegaLinha($sql);
    }
}
