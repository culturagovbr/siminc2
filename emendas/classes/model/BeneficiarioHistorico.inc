<?php
/**
 * Classe de mapeamento da entidade emendas.beneficiario_historico
 *
 * @category Class
 * @package  A1
 * @author   DOUGLAS SANTANA FONTES <douglas.fontes@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 2018-08-23
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Emendas_Model_BeneficiarioObservacao
 *
 * @category Class
 * @package  A1
 * @author   DOUGLAS SANTANA FONTES <douglas.fontes@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 2018-08-23
 * @link     no link
 */
class Emendas_Model_BeneficiarioHistorico extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'emendas.beneficiario_historico';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'behid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'usucpf' => array('tabela' => 'seguranca.usuario', 'pk' => 'usucpf'),
        'benid' => array('tabela' => 'emendas.beneficiario', 'pk' => 'benid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'behid' => null,
        'benid' => null,
        'usucpf' => null,
        'behdata' => null,
    );

    /**
     * Retorna lista com usuários que fizeram alterações no cadastro de beneficiário.
     *
     * @return mixed
     */
    public function recuperarAlteracoes($benid)
    {
        global $db;

        $sql = "SELECT
                    beh.behid,
                    to_char(beh.behdata, 'DD/MM/YYYY HH24:MI:SS') as behdata,
                    seg.usucpf,
                    seg.usunome,
                    seg.usuemail,
                    '(' || seg.usufoneddd || ') ' || seg.usufonenum as telefone
                FROM emendas.beneficiario_historico beh
                JOIN seguranca.usuario seg ON ( seg.usucpf = beh.usucpf )
                WHERE
                    beh.benid = '".$benid ."'
        ";

        return $db->carregar($sql);
    }

}
