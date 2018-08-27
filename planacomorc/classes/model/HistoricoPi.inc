<?php
/**
 * Classe de mapeamento da entidade planacomorc.historico_pi
 *
 * @category Class
 * @package  A1
 * @author   DOUGLAS SANTANA FONTES <douglas.fontes@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 06-07-2018
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Planacomorc_Model_HistoricoPi
 *
 * @category Class
 * @package  A1
 * @author   DOUGLAS SANTANA FONTES <douglas.fontes@cultura.gov.br>
 * @license  GNU siminc2.cultura.gov.br
 * @version  Release: 
 * @link     no link
 */
class Planacomorc_Model_HistoricoPi extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'planacomorc.historico_pi';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'hpiid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'pliid',
        'usucpf',
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'hpiid' => null,
        'pliid' => null,
        'usucpf' => null,
        'hisdata' => null,
        'hpidscantigo' => null,
        'hpidscnovo' => null,

    );
    
    /**
     * Monta consulta pra buscar dados do histórico de alterações do código de PI.
     * 
     * @param $pliid
     * @return string
     */
    public static function recuperarListagem($pliid)
    {
        global $db;

        $sql = "
            SELECT
                to_char(hpi.hisdata, 'DD/MM/YYYY HH24:MI:SS') AS hisdata,
                usu.usunome,
                usu.usuemail,
                '(' || usu.usufoneddd ||') '|| usu.usufonenum as telefone,
                hpi.hpidscantigo,
                hpi.hpidscnovo
            FROM
                planacomorc.historico_pi AS hpi
                JOIN seguranca.usuario AS usu ON (hpi.usucpf = usu.usucpf)
                JOIN monitora.pi_planointerno pi ON (pi.pliid = hpi.pliid)
            WHERE
                pi.pliid = '" . (int)$pliid . "'
                AND pi.plistatus = 'A'
            ORDER BY
                hpi.hisdata DESC
        ";
//ver($sql,d);
        return $db->carregar($sql);
    }

}
