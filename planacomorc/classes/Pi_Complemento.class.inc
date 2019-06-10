<?php
/**
 * Classe de mapeamento da entidade planejamento.complemento.
 *
 * @version $Id$
 * @see Pi_PlanoInterno
 */

/**
 *
 */
require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 *
 */
class Pi_Complemento extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "planejamento.complemento";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("pliid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'pliid' => null,
        'oppid' => null,
        'mppid' => null,
        'ippid' => null,
        'mpnid' => null,
        'ipnid' => null,
        'pprid' => null,
        'pumid' => null,
        'pijid' => null,
        'ptaid' => null,
        'meuid' => null,
        'picpriorizacao' => null,
        'picquantidade' => null,
        'picted' => null,
        'picedital' => null,
        'esfid' => null,
        'picpublico' => null,
        'picexecucao' => null,
        'picvalorcusteio' => null,
        'picvalorcapital' => null,
        'mescod' => null,
        'acuid' => null,
        'secid' => null,
        'maiid' => null,
        'masid' => null,
        'prgid' => null,
    );
    
    /**
     * Função salvar
     * Método usado para inserção ou alteração de um registro do banco
     * @return int|bool - Retorna um inteiro correspondente ao resultado ou false se hover erro
     */
    public function salvar($boAntesSalvar = true, $boDepoisSalvar = true, $arCamposNulo = array(), $manterAspas = false) {
        $arCamposNulo = is_array($arCamposNulo) ? $arCamposNulo : array();

        if($boAntesSalvar) {
            if (!$this->antesSalvar()) {
                return false;
            }
        }

        if (count($this->arChavePrimaria) > 1)
            trigger_error("Favor sobreescrever método na classe filha!");

        $stChavePrimaria = $this->arChavePrimaria[0];
        $this->validar($this->arAtributos);

        if($this->verificarRegistroBd($this->arAtributos[$this->arChavePrimaria[0]])) {
            $this->alterar($arCamposNulo);
            $resultado = $this->$stChavePrimaria;
        } else {
            if ($manterAspas === false) {
                $resultado = $this->inserir($arCamposNulo);
            } else {
                $resultado = $this->inserirManterAspas($arCamposNulo);
            }
        }
        if ($resultado) {
            if ($boDepoisSalvar) {
                $this->depoisSalvar();
            }
        }
        return $resultado;
    }

    /**
     * Função _inserir
     * Método usado para inserção de um registro do banco
     * 
     * @return boolean
     */
    public function inserir($arCamposNulo = array()) {
        $arCamposNulo = is_array($arCamposNulo) ? $arCamposNulo : array();
        $arCampos = array();
        $arValores = array();
        $arSimbolos = array();

        $troca = array("'", "\\");
        foreach ($this->arAtributos as $campo => $valor) {
            if ($valor !== null) {
                if (!$valor && in_array($campo, $arCamposNulo)) {
                    continue;
                }
                $arCampos[] = $campo;
                $valor = str_replace($troca, "", $valor);
                $arValores[] = trim(pg_escape_string($valor));
            }
        }

        if(count($arValores)) {
            $sql = " insert into $this->stNomeTabela ( " . implode(', ', $arCampos) . " )
            values ( '" . implode("', '", $arValores) . "' )";
            $stChavePrimaria = $this->arChavePrimaria[0];
            return $this->executar($sql);
        }
    }

}
