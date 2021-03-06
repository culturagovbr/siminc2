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
    const DESC_NECESSIDADE = 'Indicar a necessidade da altera��o or�ament�ria, informando a destina��o da proposi��o.';
    const DESC_FINANCIAMENTO = 'Indicar as compensa��es or�ament�rias oferecidas e/ou a utiliza��o de excesso de arrecada��o ou super�vit financeiro, al�m de verificar a adequa��o das altera��es � meta fiscal do exerc�cio e ao limite de gastos definido pela EC 95/2016, e que os cancelamentos oferecidos n�o trazem preju�zo � execu��o das programa��es canceladas.';
    const DESC_MEDIDA = 'Indicar a urg�ncia, a relev�ncia e a imprevisibilidade da proposi��o.';
    const DESC_CAUSA = 'Indicar a causa para a demanda, o que provocou a necessidade do ajuste e dos novos recursos e explicitar, caso haja, a altera��o nas metas de atendimento da pol�tica p�blica envolvida.';
    const DESC_FONTES = 'Verificar o balanceamento dessas classifica��es or�ament�rias, justificando, caso estejam desbalanceadas.';
    const DESC_LEGISLACAO = 'Informar, quando for o caso, a legisla��o \'espec�fica\' da qual decorre ou se baseia a altera��o or�ament�ria solicitada.';
    const DESC_OUTROS = 'Al�m de outras informa��es que forem julgadas como necess�rias, registrar que n�o h� �bices no prosseguimento da proposta/solicita��o.';
    
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
     * Busca a Justificativa pelo n�mero do Pedido de Altera��o Or�ament�ria.
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
