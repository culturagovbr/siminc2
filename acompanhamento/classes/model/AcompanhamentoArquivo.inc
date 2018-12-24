<?php
/**
 * Classe de mapeamento da entidade acompanhamento.acompanhamentoarquivo
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 23-01-2018
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Acompanhamento_Model_Acompanhamentoarquivo
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Acompanhamento_Model_AcompanhamentoArquivo extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'acompanhamento.acompanhamentoarquivo';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'acaid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'arqid' => array('tabela' => 'arquivo', 'pk' => 'arqid'),
        'acoid' => array('tabela' => 'acompanhamento.acompanhamento', 'pk' => 'acoid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'acaid' => null,
        'acoid' => null,
        'arqid' => null,
    );

    public function buscarPorAcompanhamento($acoid)
    {
        $sql = "
            SELECT
                a.arqid,
                a.arqnome || '.' || a.arqextensao AS arqnome,
                a.arqdescricao
            FROM {$this->stNomeTabela} entanexo
                JOIN public.arquivo a ON(entanexo.arqid = a.arqid)
            WHERE
                entanexo.acoid = ". (int)$acoid;

        $dados = $this->carregar($sql);
        return $dados ? $dados : [];
    }

}
