<?php
/**
 * Classe de mapeamento da entidade acompanhamento.historicopnc
 *
 */
require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Acompanhamento_Model_Historicopnc
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Acompanhamento_Model_Historicoindicadorpnc extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'acompanhamento.historicoindicadorpnc';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'hipid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'usucpf' => array('tabela' => 'usuario', 'pk' => 'usucpf'),
        'idpid' => array('tabela' => 'acompanhamento.indicador_pnc', 'pk' => 'idpid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'hipid' => null,
        'idpid' => null,
        'usucpf' => null,
        'hipdata' => null,
        'hipquantidadeantigo' => null,
        'hipquantidadenovo' => null,
    );

    public function buscarPorIndicador()
    {
        $sql = "
            SELECT
                his.*,
                usu.usunome,
		usu.usuemail,
		usu.usufoneddd,
		usu.usufonenum,
                to_char(hipdata, 'DD/MM/YYYY HH24:MI:SS') AS hipdata
            FROM acompanhamento.historicoindicadorpnc his
                JOIN seguranca.usuario usu ON usu.usucpf = his.usucpf
            WHERE
                his.idpid = ". (int)$this->idpid. "
            ORDER BY
                his.hipdata DESC
        ";

        return $this->carregar($sql);
    }

}
