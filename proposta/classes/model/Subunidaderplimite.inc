<?php
/**
 * Classe de mapeamento da entidade proposta.subunidaderplimite
 *
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Proposta_Model_Subunidaderplimite
 *
 */
class Proposta_Model_Subunidaderplimite extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'proposta.subunidaderplimite';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'srlid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'suoid' => array('tabela' => 'public.subunidadeorcamentaria', 'pk' => 'suoid'),
        'irpcod' => array('tabela' => 'public.identresultprimario', 'pk' => 'irpcod'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'srlid' => null,
        'suoid' => null,
        'irpcod' => null,
        'srllimite' => null,
        'srldtcadastro' => null,
        'srlstatus' => null
    );

    public function montarSqlBuscarPorSuoidIrpcod(){
        $sql = "
            SELECT
                *
            FROM $this->stNomeTabela
            WHERE
                srlstatus = 'A'
                AND suoid = ". (int)$this->suoid. "
                AND irpcod = '". (int)$this->irpcod. "'
        ";
        return $sql;
    }
    
    public function buscarPorSuoidIrpcod() {
        $sql = $this->montarSqlBuscarPorSuoidIrpcod();
        $resultado = $this->pegaLinha($sql);
        if($resultado){
            $this->popularDadosObjeto($resultado);
        }
        
        return $this;
    }
    
    public function recuperarLimitePorSuoid(stdClass $dto)
    {
        if($dto->eqdid){
            $whereEqdid = "
                SELECT DISTINCT
                    irpcod
                FROM
                    planejamento.enquadramentorp
                WHERE
                    eqdid = ". (int)$dto->eqdid;
            # Filtro por RP no disponivel
            $whereDisponivelUnidade = "
                AND ptres.irpcod IN(
                    $whereEqdid
                )";
            # Filtro por RP no limite
            $where = "
                AND irpcod IN(
                    $whereEqdid
                )";
        }
        
        $sql = "
            SELECT
                srl.suoid,
                MIN(srl.irpcod) AS irpcod,
                SUM(COALESCE(srl.srllimite, 0)) AS limite_unidade,
                (
                    SELECT
                        SUM(COALESCE(plivalorcapital, 0) + COALESCE(plivalorcusteio, 0)) AS total_pre_pi
                    FROM proposta.preplanointerno
			JOIN monitora.ptres ON preplanointerno.ptrid = ptres.ptrid
                    WHERE
                        plistatus = 'A'
                        AND suoid = '". (int)$dto->suoid. "'
                        AND pliid <> ". (int)$dto->noPliid. "
                        $whereDisponivelUnidade
                    GROUP BY
                        suoid
                ) AS total_pre_pi
            FROM proposta.subunidaderplimite srl
            WHERE
                srlstatus = 'A'
                AND suoid = '". (int)$dto->suoid. "'
                $where
            GROUP BY
                srl.suoid,
                total_pre_pi
        ";
//ver($sql, d);
        $limite = $this->pegaLinha($sql);
        return $limite;
    }
}//end Class

