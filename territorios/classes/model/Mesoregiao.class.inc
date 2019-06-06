<?php
/**
 * Classe de mapeamento da entidade territorios.mesoregiao
 *
 * @category Class
 * @package  A1
 * @author   EDUARDO DUNICE NETO <eduardo.neto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 13-10-2015
 * @link     no link
 */



/**
 * Territorios_Model_Mesoregiao
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release:
 * @link     no link
 */
class Territorios_Model_Mesoregiao extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'territorios.mesoregiao';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'mescod',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'estuf' => array('tabela' => 'territorios.estado', 'pk' => 'estuf'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'mescod' => null,
        'estuf' => null,
        'mesdsc' => null,
    );
    /**
     * Atributos
     * @var $dados array
     * @access protected
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'mescod' => array(  new Zend_Validate_StringLength(array('max' => 4))  ),
            'estuf' => array(  new Zend_Validate_StringLength(array('max' => 2))  ),
            'mesdsc' => array(  new Zend_Validate_StringLength(array('max' => 255))  ),
        );
    }//end getCamposValidacao($dados)


    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function gravar()
    {
        global $url;
        $this->popularDadosObjeto();
        $url .= '&mescod=' . $this->mescod;

        try{
            $sucesso = $this->salvar();
            $this->commit();
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }

        if($sucesso){
            simec_redirecionar($url, 'success');
        }
        simec_redirecionar($url, 'error');
    }//end gravar()


   /**
     * Função excluir
     * - grava os dados
     *
     */
    public function excluir()
    {
        global $url;
       // $url = 'aspar.php?modulo=principal/proposicao/index&acao=A';
        try{
            $this->excluir();
            $this->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }//end excluir()


    /**
     * Função pegarSQLSelect
     *
     * @return query para montar componentes.
     */
    public function pegarSQLSelect($arrPost)
    {
        $where = '';

        if ($arrPost['estuf'][0] != '') $where = "WHERE estuf IN ('".implode("', '", $arrPost['estuf'])."')";

        $sql = "SELECT mescod as codigo, mesdsc as descricao
                FROM territorios.mesoregiao
                $where
                ORDER BY mesdsc ASC";

        return $sql;

    }//end pegarSQLSelect()



}
