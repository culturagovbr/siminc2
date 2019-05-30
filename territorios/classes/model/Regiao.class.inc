<?php
/**
 * Classe de mapeamento da entidade territorios.regiao
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
class Territorios_Model_Regiao extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'territorios.regiao';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'regcod',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'paiid' => array('tabela' => 'territorios.pais', 'pk' => 'paiid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'regcod' => null,
        'paiid' => null,
        'regdescricao' => null,
    );
    /**
     * Atributos
     * @var $dados array
     * @access protected
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'regcod' => array(  new Zend_Validate_StringLength(array('max' => 2))  ),
            'paiid' => array( 'allowEmpty' => true, 'Digits'  ),
            'regdescricao' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 100))  ),
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
        $url .= '&regcod=' . $this->regcod;

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
        try {
            $this->excluir();
            $this->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }


    /**
     * Função pegarSQLSelect
     *
     * @return query para montar componentes.
     */
    public function pegarSQLSelect()
    {
        $sql = "SELECT regcod as codigo, regdescricao as descricao
                FROM territorios.regiao
                ORDER BY regdescricao ASC";

        return $sql;

    }

}
