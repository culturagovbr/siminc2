<?php

/**
 * Classe de mapeamento da entidade acompanhamento.indicador_pnc
 */

require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Acompanhamento_Model_Indicadorpnc
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Acompanhamento_Model_Indicadorpnc extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'acompanhamento.indicador_pnc';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'idpid',
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'acpid' => array('tabela' => 'acompanhamento.acompanhamento_pnc', 'pk' => 'acpid')
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'idpid' => null,
        'acpid' => null,
        'ipncod' => null,
        'unocod' => null,
        'suocod' => null,
        'prsano' => null,
        'idpquantidade' => null,
        'idpdata' => null,
        'idpjustificativa' => null,
        'idpstatus' => null,
    );

    /**
     * Monta consulta pra buscar acompanhamento de Indicador por Indicador Meta, UO, Subnidade, Indicador e Ano.
     * 
     * @param stdClass $filtro
     * @return string
     */
    public function montarSqlbuscarPorAcompanhamentoPncUnidadeIndicadorAno(){
        $sql = "
            SELECT DISTINCT
                idp.idpid,
                idp.acpid,
                acp.suoid,
                acp.janid,
                acp.mpnid,
                idp.unocod,
                idp.suocod,
                idp.ipncod,
                idp.prsano,
                idp.idpquantidade
            FROM acompanhamento.indicador_pnc idp
                JOIN acompanhamento.acompanhamento_pnc acp ON(idp.acpid = acp.acpid)
            WHERE
                idp.idpstatus = 'A'
                AND acp.acpstatus = 'A'
                AND idp.acpid = ". (int)$this->acpid. "
                AND idp.unocod = '". (int)$this->unocod. "'
                AND idp.suocod = '". (int)$this->suocod. "'
                AND idp.ipncod = '". $this->ipncod. "'
                AND idp.prsano = '". (int)$this->prsano. "'
        ";
//ver($sql, d);
        return $sql;
    }

    /**
     * Buscar acompanhamento de Indicador por Indicador Meta, UO, Subnidade, Indicador e Ano.
     * 
     * @param stdClass $filtro
     * @return stdClass
     */
    public function buscarPorAcompanhamentoPncUnidadeIndicadorAno() {
        $sql = $this->montarSqlbuscarPorAcompanhamentoPncUnidadeIndicadorAno();
        $resultado = $this->pegaLinha($sql);
        if($resultado){
            $this->popularDadosObjeto($resultado);
        }
        
        return $this;
    }
    
}
