<?php
/**
 * Abstração do mapeamento da entidade {esquema}.programacaoexercicio.
 *
 * $Id: Programacaoexercicio.php 100919 2015-08-06 19:01:37Z maykelbraz $
 * @filesource
 */

/**
 * Spo_Model_Programacaoexercicio
 *
 * @category Class
 * @package  A1
 * @author   DOUGLAS SANTANA FONTES <douglas.fontes@cultura.gov.br>
 * @license  GNU siminc2.cultura.gov.br
 * @version  Release:
 * @link     no link
 */
class Spo_Model_Programacaoexercicio extends Modelo
{
    /**
     * @var string Nome da tabela especificada
     */
    protected $stNomeTabela;

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'prsano',
    );

    /**
     * @var string[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array();

    /**
     * @var mixed[] Atributos
     */
    protected $arAtributos = array(
        'prsano' => null,
        'prsdata_inicial' => null,
        'prsdata_termino' => null,
        'prsexerccorrente' => null,
        'prsstatus' => null,
        'prsativo' => null,
        'prsexercicioaberto' => null,
    );

    /**
     * Construtor da classe.
     *
     * @param string $esquema O Nome do esquema da tabela.
     * @param int|null $id O ID do registro a ser consultado.
     * @throws Exception Lançada se o esquema informado for vazio.
     */
    public function __construct($esquema, $id = null)
    {
        if (empty($esquema)) {
            throw new Exception('O valor de "$esquema" não pode ser vazio.');
        }

        $this->stNomeTabela = "{$esquema}.programacaoexercicio";
        parent::__construct($id);
    }

    public function queryCombo()
    {
        $opcoes = array('query' => true);
        return $this->recuperarTodosFormatoInput('prsano', array(), 'descricao DESC', $opcoes);
    }

    /**
     * Função salvar sobreescrevendo o pai por conta do id não ser auto incremento.
     * Método usado para inserção ou alteração de um registro do banco
     * @return int|bool - Retorna um inteiro correspondente ao resultado ou false se hover erro
     * @param bool $boAntesSalvar
     * @param bool $boDepoisSalvar
     * @param array $arCamposNulo
     * @param bool $manterAspas
     * @throws Exception
     */
    public function salvar($boAntesSalvar = true, $boDepoisSalvar = true, $arCamposNulo = array(), $manterAspas = false)
    {
        $arCamposNulo = is_array($arCamposNulo) ? $arCamposNulo : array();

        if ($boAntesSalvar) {
            if (!$this->antesSalvar()) {
                return false;
            }
        }

        if (count($this->arChavePrimaria) > 1)
            trigger_error("Favor sobreescrever método na classe filha!");

        $stChavePrimaria = $this->arChavePrimaria[0];
        $this->validar($this->arAtributos);

        $sql = "SELECT * FROM " . $this->stNomeTabela . " WHERE prsano = '" . $this->prsano . "' ";

        $existe = $this->pegaUm($sql);

        if ($this->$stChavePrimaria && $existe) {
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
     * Função inserir
     * Método usado para inserção de um registro do banco
     * @return int|bool - Retorna um inteiro correspondente ao resultado ou false se hover erro
     * @param array $arCamposNulo
     */
    public function inserir($arCamposNulo = array())
    {
        $arCamposNulo = is_array($arCamposNulo) ? $arCamposNulo : array();
        if (count($this->arChavePrimaria) > 1)
            trigger_error("Favor sobreescrever método na classe filha!");

        $arCampos = array();
        $arValores = array();

        $troca = array("'", "\\");
        foreach ($this->arAtributos as $campo => $valor) {
            /*if ($campo == $this->arChavePrimaria[0] && !$this->tabelaAssociativa)
                continue;*/
            if ($valor !== null) {
                if (!$valor && in_array($campo, $arCamposNulo)) {
                    continue;
                }
                $arCampos[] = $campo;
                $valor = str_replace($troca, "", $valor);
                $arValores[] = trim(pg_escape_string($valor));
            }
        }

        if (count($arValores)) {
            $sql = "
                INSERT INTO
                    $this->stNomeTabela ( " . implode(', ', $arCampos) . " )
                VALUES
                    ( '" . implode("', '", $arValores) . "' )
					 returning {$this->arChavePrimaria[0]}
            ";

            $stChavePrimaria = $this->arChavePrimaria[0];
            return $this->$stChavePrimaria = $this->pegaUm($sql);
        }
    }

    /**
     * /**
     * Função alterar
     * Método usado para alteração de um registro do banco
     * @return int|bool - Retorna um inteiro correspondente ao resultado ou false se hover erro
     * @author Douglas Santana Fontes
     * @since 25/03/2019
     * @param array $arCamposNulo
     * @throws Exception
     */
    public function alterar($arCamposNulo = array())
    {
        $arCamposNulo = is_array($arCamposNulo) ? $arCamposNulo : array();
        if (count($this->arChavePrimaria) > 1)
            trigger_error("Favor sobreescrever método na classe filha!");

        $campos = "";
        foreach ($this->arAtributos as $campo => $valor) {
            if ($valor != null) {
                if ($campo == $this->arChavePrimaria[0]) {
                    $valorCampoChave = $valor;
                    continue;
                }

                $valor = pg_escape_string($valor);

                $campos .= $campo . " = '" . $valor . "', ";
            } else {
                if (in_array($campo, $arCamposNulo)) {
                    $campos .= $campo . " = null, ";
                }
            }
        }

        $campos = substr($campos, 0, -2);

        $sql = "UPDATE {$this->stNomeTabela} SET {$campos} WHERE {$this->arChavePrimaria[0]} = '$valorCampoChave' ";

        return $this->executar($sql);
    }

    /**
     * Busca todos os registros do exercício.
     * @return array|string
     */
    public function buscarTodos()
    {
        $sql = "
            SELECT DISTINCT
                prsano,
                prsdata_inicial,
                prsdata_termino,
                CASE
                    WHEN prsexerccorrente = 't' THEN 'Sim'
                    WHEN prsexerccorrente = 'f' THEN 'Não'
                END AS prsexerccorrente
            FROM {$this->stNomeTabela}
            WHERE
                prsstatus = 'A'
            ORDER BY
                prsano desc
        ";

        return $this->carregar($sql);
    }
}
