<?php
/**
 * Classe de mapeamento da entidade planejamento.historico_pi_usuario
 *
 * @category Class
 * @package  A1
 * @author   RAFAEL FREITAS CARNEIRO <rafael.carneiro@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 17-07-2018
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


class Planacomorc_Model_HistoricoPiUsuario extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'planejamento.historico_pi_usuario';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'hpuid',
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
        'hpuid' => null,
        'pliid' => null,
        'usucpf' => null,
        'hpudata' => null,
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
                to_char(hpu.hpudata, 'DD/MM/YYYY HH24:MI:SS') AS hpudata,
                usu.usunome,
                '(' || usu.usufoneddd ||') '|| usu.usufonenum AS telefone,
                usu.usuemail
            FROM
                planejamento.historico_pi_usuario AS hpu
                JOIN seguranca.usuario AS usu ON (hpu.usucpf = usu.usucpf)
            WHERE
                hpu.pliid = '" . (int)$pliid . "'
            ORDER BY
                hpu.hpudata DESC limit 1
        ";
        return $db->carregar($sql);
    }

}
