<?php
/**
 * Classe de mapeamento da entidade alteracao.justificativa
 *
 * @category Class Justificativa de Alteracao Orcamentaria
 * @package  A1
 * @author   Douglasx <douglas.fontes@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 30-11-2018
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Alteracao_Model_Justificativa
 *
 * @category Class Justificativa de Alteracao Orcamentaria
 * @package  A1
 * @author   DOUGLAS SANTANA FONTES <douglas.fontes@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 30-11-2018
 * @link     no link
 */
class Alteracao_Model_Justificativa extends Modelo
{
    const DESC_NECESSIDADE = 'Indicar a necessidade da alteração orçamentária, informando a destinação da proposição.';
    const DESC_FINANCIAMENTO = 'Indicar as compensações orçamentárias oferecidas e/ou a utilização de excesso de arrecadação ou superávit financeiro, além de verificar a adequação das alterações à meta fiscal do exercício e ao limite de gastos definido pela EC 95/2016, e que os cancelamentos oferecidos não trazem prejuízo à execução das programações canceladas.';
    const DESC_MEDIDA = 'Indicar a urgência, a relevância e a imprevisibilidade da proposição.';
    const DESC_CAUSA = 'Indicar a causa para a demanda, o que provocou a necessidade do ajuste e dos novos recursos e explicitar, caso haja, a alteração nas metas de atendimento da política pública envolvida.';
    const DESC_FONTES = 'Verificar o balanceamento dessas classificações orçamentárias, justificando, caso estejam desbalanceadas.';
    const DESC_LEGISLACAO = 'Informar, quando for o caso, a legislação \'específica\' da qual decorre ou se baseia a alteração orçamentária solicitada.';
    const DESC_OUTROS = 'Além de outras informações que forem julgadas como necessárias, registrar que não há óbices no prosseguimento da proposta/solicitação.';
    
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'alteracao.justificativa';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'jstid',
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'pedid' => array('tabela' => 'alteracao.pedido', 'pk' => 'pedid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'jstid' => null,
        'pedid' => null,
        'jstnecessidade' => null,
        'jstcausa' => null,
        'jstfinanciamento' => null,
        'jstfontes' => null,
        'jstmedida' => null,
        'jstlegislacao' => null,
        'jstoutros' => null,
    );

    /**
     * Busca a Justificativa pelo número do Pedido de Alteração Orçamentária.
     * @param $pedid
     * @return object
     * @since 30-11-2018
     */
    public function recuperaJustificativa($pedid)
    {
        $sql = "SELECT
                    jstid,
                    jstnecessidade,
                    jstcausa,
                    jstfinanciamento,
                    jstfontes,
                    jstmedida,
                    jstlegislacao,
                    jstoutros
                FROM alteracao.justificativa
                WHERE
                    pedid = {$pedid}
        ";

        return (object) $this->pegaLinha($sql);
    }

}
