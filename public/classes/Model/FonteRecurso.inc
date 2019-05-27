<?php
/**
 * Classe de mapeamento da entidade public.fonterecurso
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 20-11-2017
 * @link     no link
 */


require_once APPRAIZ . 'includes/classes/Modelo.class.inc';


/**
 * Public_Model_Fonterecurso
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Public_Model_FonteRecurso extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'public.fonterecurso';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'fonid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'fonid' => null,
        'foncod' => null,
        'fondsc' => null,
        'fonstatus' => null,
        'fonabrev' => null,
        'grfid' => null,
        'clasproporc' => null,
    );

    /**
     * Consulta se o código inserido existe e retorna a primeira coluna do primeiro registro.
     * OBS: O código que está sendo passado por parametro está sendo instanciado em Public_Controller_FonteRecurso
     * @return Modelo
     */
    public function carregarPorCodigo()
    {
        $sql = "SELECT
                     foncod,
                     fondsc
                FROM
                    public.fonterecurso
                WHERE
                    fonstatus = 'A'
                    AND foncod = '". $this->foncod ."'
                    AND fonid <> ". (int)$this->fonid;
//ver($sql,d);
        return $this->pegaLinha($sql);
    }

    /**
     * Retorna a linha do registro de Fonte Recurso.
     *
     * @return array|bool
     */
    public function carregarPorFoncod()
    {
        $sql = "
            SELECT
                fonid,
                foncod,
                fondsc,
                fonabrev,
                grfid,
                clasproporc
            FROM public.fonterecurso
            WHERE
                fonstatus = 'A'
                AND foncod = '". (int)$this->foncod ."'
        ";
        return $this->pegaLinha($sql);
    }

}
