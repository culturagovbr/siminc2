<?php

/**
 * Implementação da classe base de modelo.
 *
 * @version $Id: Modelo.class.inc 103541 2015-10-09 20:36:35Z mariluciaqueiroz $
 * @filesource
 */

/**
 * Classe pai para as classes espelhos de banco.
 *
 * @package Simec\Db
 * @author Orion Teles de Mesquita
 * @since 12/02/2009
 */
class Modelo extends cls_banco {

    protected $tabelaAssociativa = false;

    /**
     * Armazena configurações de chave estrangeira no seguinte formato:
     * array('esquema.tabela' => array('fk' => array('coluna'), 'pk' => array('coluna')));
     * @var array
     */
    protected $arChaveEstrangeira = array();

    public function __construct($id = null) {

        parent::__construct();

        if ($id) {
            $this->carregarPorId($id);
        }
    }

    public function __destruct() {
//		$this->close();
    }

    /**
     * Função __get
     * Método mágico do PHP. Recupera o valor do atributo da classe caso não haja método get implementado.
     * @param string $stAtributo - Nome do atributo que se deseja retornar
     * @return string - Retorna o valor do atributo
     * @access public
     * @author Orion Teles de Mesquita
     * @since 12/02/2009
     * @final
     */
    final public function __get($stAtributo) {

        if (property_exists(get_class($this), $stAtributo))
            return $this->$stAtributo;
        elseif (array_key_exists($stAtributo, $this->arAtributos))
            return $this->arAtributos[$stAtributo];
        else
            trigger_error("O atributo (" . $stAtributo . ") não existe!");
    }

    /**
     * Função __set
     * Método mágico do PHP. Seta o valor do atributo da classe caso não haja método set implementado.
     * @param string $stAtributo - Nome do atributo que se deseja setar
     * @param string $valor - Valor do atributo
     * @access public
     * @author Orion Teles de Mesquita
     * @since 12/02/2009
     * @final
     */
    final public function __set($stAtributo, $valor) {

        if (array_key_exists($stAtributo, $this->arAtributos))
            $this->arAtributos[$stAtributo] = $valor;
        else
            trigger_error("O atributo ({$stAtributo}) não existe!");
    }

    public function __isset($atributo) {
        return isset($this->arAtributos[$atributo]);
    }

    public function antesSalvar() {
        return true;
    }

    public function depoisSalvar() {
        return true;
    }

    /**
     * Verifica se o registro existe na base de dados.
     * 
     * @param string $id
     * @return boolean
     */
    public function verificarRegistroBd($id) {
        $id = trim(str_replace("'", "", (string) $id));
        $sql = " SELECT {$this->arChavePrimaria[0]} FROM $this->stNomeTabela WHERE {$this->arChavePrimaria[0]} = '$id'; ";

        $idBase = $this->pegaUm($sql);
        return $idBase? TRUE: FALSE;
    }
    
    /**
     * Função salvar
     * Método usado para inserção ou alteração de um registro do banco
     * @return int|bool - Retorna um inteiro correspondente ao resultado ou false se hover erro
     * @access public
     * @author Orion Teles de Mesquita
     * @since 12/02/2009
     */
    public function salvar($boAntesSalvar = true, $boDepoisSalvar = true, $arCamposNulo = array(), $manterAspas = false) {
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

        if ($this->$stChavePrimaria && !$this->tabelaAssociativa) {
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
     * @return int|bool - Retorna um inteiro correspondente ao resultado ou false se hover erro
     * @access public
     * @author Orion Teles de Mesquita
     * @since 12/02/2009
     */
    public function inserir($arCamposNulo = array()) {
        $arCamposNulo = is_array($arCamposNulo) ? $arCamposNulo : array();
        if (count($this->arChavePrimaria) > 1)
            trigger_error("Favor sobreescrever método na classe filha!");

        $arCampos = array();
        $arValores = array();
        $arSimbolos = array();

        $troca = array("'", "\\");
        foreach ($this->arAtributos as $campo => $valor) {
            if ($campo == $this->arChavePrimaria[0] && !$this->tabelaAssociativa)
                continue;
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
            $sql = " insert into $this->stNomeTabela ( " . implode(', ', $arCampos) . " )
											  values ( '" . implode("', '", $arValores) . "' )
					 returning {$this->arChavePrimaria[0]}";
            $stChavePrimaria = $this->arChavePrimaria[0];
            return $this->$stChavePrimaria = $this->pegaUm($sql);
        }
    }

    /**
     * Função _alterar
     * Método usado para alteração de um registro do banco
     * @return int|bool - Retorna um inteiro correspondente ao resultado ou false se hover erro
     * @access private
     * @author Orion Teles de Mesquita
     * @since 12/02/2009
     */
    public function alterar($arCamposNulo = array()) {
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

        $sql = " UPDATE $this->stNomeTabela SET $campos WHERE {$this->arChavePrimaria[0]} = $valorCampoChave ";

        return $this->executar($sql);
    }

    /**
     * Função antesExcluir
     * Método usado para fazer alguma funcionalidade antes da exclusão dos dados
     * @return bool - Retorna true se não houve nada para impedir a exclusão e false caso contrário
     * @access public
     * @author Orion Teles de Mesquita
     * @since 12/02/2009
     */
    public function antesExcluir($id = null) {
        return true;
    }

    /**
     * Função excluir
     * Método usado para excluir registro do banco
     * @param int $id - Identificador do registro a ser excluído ( se não for passado valor, exclui-se o registro do objeto carregado )
     * @return int|bool - Retorna um inteiro correspondente ao resultado ou false se hover erro
     * @access public
     * @author Orion Teles de Mesquita
     * @since 12/02/2009
     */
    public function excluir($id = null, $retorno = null) {
        $complemento = ";";
        if (count($this->arChavePrimaria) > 1)
            trigger_error("Favor sobreescrever método na classe filha!");

        if ($retorno) {
            $complemento = "returning $retorno;";
        }

        if (!$this->antesExcluir($id))
            return false;

        $stChavePrimaria = $this->arChavePrimaria[0];
        $id = $id ? $id : $this->$stChavePrimaria;

        $sql = " DELETE FROM $this->stNomeTabela WHERE $stChavePrimaria = $id $complemento";
        if ($retorno) {
            return $this->pegaUm($sql);
        } else {
            return $this->executar($sql);
        }
    }

    /**
     * Função excluirVarios
     * Método usado para excluir registros da tabela por parâmetro
     * @param string - cláusula where
     * @return int|bool - Retorna um inteiro correspondente ao resultado ou false se hover erro
     * @access public
     * @author Orion Teles de Mesquita
     * @since 20/02/2014
     */
    public function excluirVarios($where, $retorno = null) {
        $complemento = ";";
        if (count($this->arChavePrimaria) > 1)
            trigger_error("Favor sobreescrever método na classe filha!");

        if ($retorno) {
            $complemento = "returning $retorno;";
        }

        $stChavePrimaria = $this->arChavePrimaria[0];
        $id = $id ? $id : $this->$stChavePrimaria;

        $sql = " DELETE FROM $this->stNomeTabela WHERE $where";

        if ($retorno) {
            return $this->pegaUm($sql);
        } else {
            return $this->executar($sql);
        }
    }

    /**
     * Função carregarPorId
     * Método usado para carregar um Objeto pelo ID
     * @param int $id - Identificador do objeto a ser carregado
     * @access public
     * @author Orion Teles de Mesquita
     * @since 12/02/2009
     */
    public function carregarPorId($id) {
        $id = trim(str_replace("'", "", (string) $id));
        $sql = " SELECT * FROM $this->stNomeTabela WHERE {$this->arChavePrimaria[0]} = '$id'; ";

        $arResultado = $this->pegaLinha($sql);
        $this->popularObjeto(array_keys($this->arAtributos), $arResultado);
    }

    /**
     * Função recuperarTodos
     * Método usado para recuperar todos os registros do banco, seguinto parâmetros
     * @param string $stCampos - String contendo nomes dos campos a serem carregados
     * @param array $arClausulas - Array contendo dados da cláusula where
     * @param string $stOrdem - String contendo dados da cláusula ordey by
     * @param array $opcoes Opções adicionais do método.
     * @example - $obNomeObjeto->recuperarTodos( 'campo1, campo2', array( "id = 123" ), nome )
     * @example - $obNomeObjeto->recuperarTodos() --> Se não passar parâmetros reconhece como todos os registros do banco.
     * @access public
     * @author Orion Teles de Mesquita
     * @since 12/02/2009
     */
    public function recuperarTodos($stCampos = '*', $arClausulas = null, $stOrdem = null, array $opcoes = array()) {
        // -- Retornar a query ao invés dos dados
        $opcoes['query'] = isset($opcoes['query']) ? $opcoes['query'] : false;
        // -- Faz join com as tabelas mapeadas nas foreing keys
        $opcoes['join'] = isset($opcoes['join']) ? $opcoes['join'] : false;

        $sql = "SELECT DISTINCT $stCampos FROM $this->stNomeTabela t1";

        $contagemTabelas = 2;
        if ($opcoes['join']) {
            if (empty($this->arChaveEstrangeira)) {
                throw new Exception("Não existe nenhuma chave estranjeira definida para a tabela {$this->stNomeTabela}.");
            }
            $sql .= $this->gerarJoin($contagemTabelas, $opcoes['join']);
        }
        if ($opcoes['leftjoin']) {
            if (empty($this->arChaveEstrangeira)) {
                throw new Exception("Não existe nenhuma chave estranjeira definida para a tabela {$this->stNomeTabela}.");
            }
            $sql .= $this->gerarJoin($contagemTabelas, $opcoes['leftjoin'], 'LEFT');
        }

        $sql .= $arClausulas ? " WHERE " . implode(" AND ", $arClausulas) : "";
        $sql .= $stOrdem ? " ORDER BY $stOrdem" : "";

        if ($opcoes['query']) {
            return $sql;
        }

        $resultado = $this->carregar($sql);
        return $resultado ? $resultado : array();
    }

    protected function gerarJoin(&$i, $joins, $join = 'INNER') {
        if (!is_array($joins)) {
            $joins = array($joins);
        }

        $sql = '';
        foreach ($joins as $fk) {
            $dadosFk = $this->arChaveEstrangeira[$fk];
            $fk = explode(', ', $fk);
            $pk = explode(', ', $dadosFk['pk']);

            $sql .= " {$join} JOIN {$dadosFk['tabela']} t{$i} ON (";

            foreach ($fk as $index => $coluna) {
                $sql .= "t1.{$coluna} = t{$i}.{$pk[$index]} ";
            }

            $sql .= ") ";
            $i++;
        }
        return $sql;
    }

    /**
     * Executa o método Modelo::recuperarTodos() formatando o resultado para utilização com as funções de combo do Simec.
     * @param string $campoDescricao Nome do campo que servirá como descrição para os itens do combo.
     * @return array
     */
    public function recuperarTodosFormatoInput($campoDescricao, array $arClausulas = array(), $stOrdem = null, array $opcoes = array()) {
        $stCampos = "{$this->arChavePrimaria[0]} AS codigo, {$campoDescricao} AS descricao";
        return $this->recuperarTodos($stCampos, $arClausulas, $stOrdem, $opcoes);
    }

    public function popularObjeto($arCampos, $arDados = null) {

        $arDados = $arDados ? $arDados : $_REQUEST;

        foreach ($arCampos as $campo) {

            if (key_exists($campo, $arDados)) {
                $this->$campo = $arDados[$campo];
            }
        }
    }

    public function popularDadosObjeto($arDados = null, $utf8_decode = false) {

        $arDados = $arDados ? array_change_key_case($arDados, CASE_LOWER) : $_REQUEST;
        foreach ($this->arAtributos as $campo => $valor) {

            if (key_exists($campo, $arDados)) {
                $this->$campo = $arDados[$campo];
                $this->$campo = ($utf8_decode && is_string($arDados[$campo]))? str_replace(' ? ', ' - ', utf8_decode($arDados[$campo])): $arDados[$campo];
            }
        }

        return $this;
    }

    public function getDados() {
        return $this->arAtributos;
    }

    public function clearDados() {
        foreach ($this->arAtributos as $k => $v) {
            $this->arAtributos[$k] = null;
        }
        return $this;
    }

    function lista($coluna = null, Array $arParamWhere = null, Array $arParamJoin = null, Array $arParam = null) {
        $where = "";
        $arJoin = array();
        $coluna = implode(', ', (is_null($coluna) ? array_keys($this->arAtributos) : ((array) $coluna)));
        // Parametros auxiliares
        $alias = $arParam['alias'];
        $order = $arParam['order'] ? " ORDER BY {$arParam['order']}" : "";

        // Separa os JOINs (inner, left)
        $arInner = is_array($arParamJoin['inner']) ? $arParamJoin['inner'] : array();
        $arLeft = is_array($arParamJoin['left']) ? $arParamJoin['left'] : array();

        // Monta os INNER JOINs
        foreach ($arInner as $tabela => $on) {
            $arJoin[] = 'JOIN ' . $tabela . ' ON (' . $on . ')';
        }
        // Monta os LEFT JOINs
        foreach ($arLeft as $tabela => $on) {
            $arJoin[] = 'LEFT JOIN ' . $tabela . ' ON (' . $on . ')';
        }

        // Monta string WHERE
        $where = (count($arParamWhere) > 0 ? "WHERE (" . implode(') AND (', $arParamWhere) . ")" : "");
        // Monta string JOIN
        $join = count($arJoin) ? implode(' ', $arJoin) : '';

        $sql = "SELECT
				   {$coluna}
			  	FROM
			  	   {$this->stNomeTabela} $alias
			  	{$join}
			  	{$where}
			  	{$order};";
//		dbg($sql, 1);
        return $this->carregar($sql);
    }

    /**
     * Método estático para validação de parâmetros.
     *
     * Utilize este método para verificar se um parâmetro OBRIGATÓRIO existe em uma lista de parâmetros.
     * Exemplo: self::checarParametros(['a' => 1, 'b' => 2], ['a', 'b', 'c']);
     * Se o parâmetro não existir na lista, ou estiver vazio, uma exception é lançada.
     *
     * @param mixed[] $params Lista de parâmetros. Veja a documentação do método para o formato do array.
     * @param string|string[] $nome O nome, ou uma lista de nomes para verificação.
     * @throws Exception Lançada qdo o parâmetro informado para validação não existe ou está vazio na lista de parâmetros.
     */
    protected static function checarParametros(array $params, $nome) {
        if (!is_array($nome)) {
            $nome = array($nome);
        }

        foreach ($nome as $_nome) {
            if (!isset($params[$_nome]) || empty($params[$_nome])) {
                throw new Exception("O parâmetro '{$_nome}' é obrigatório e não pode ser deixado em branco.");
            }
        }
    }

    protected function validar(array $dados) {
        $this->validarCampos($dados);
    }

    public function existeVinculo($dados = null) {
        return false;
    }

    public function getCamposValidacao($dados = array()) {
        return array();
    }

    final protected function validarCampos(array $dados) {
        return true;

        $fields = $this->getCamposValidacao($dados);
        $validate = new Zend_Filter_Input(array(), $fields, $dados);

        // Se não for válido lança a exception
        $aMensagem = $aCampo = array();
        if (!$validate->isValid()) {
            foreach ($validate->getMessages() as $campo => $mensagem) {
                $aMensagem[$campo][] = current($mensagem);
            }
            // -- Campos do formulário que apresentaram erro de validação
            $_SESSION['form_validate']['dados'] = $dados;
            $_SESSION['form_validate']['erros'] = $aMensagem;
            throw new Simec_Db_Exception('Não foi possível realizar a operação.');
        }
        return true;
    }

    /**
     * Função _inserir mantendo aspas
     * Método usado para inserção de um registro do banco
     * @return int|bool - Retorna um inteiro correspondente ao resultado ou false se hover erro
     * @access private
     * @author 
     * @since 09/10/2015
     */
    public function inserirManterAspas($arCamposNulo = array()) {
        $arCamposNulo = is_array($arCamposNulo) ? $arCamposNulo : array();
        if (count($this->arChavePrimaria) > 1)
            trigger_error("Favor sobreescrever método na classe filha!");

        $arCampos = array();
        $arValores = array();
        $arSimbolos = array();

        foreach ($this->arAtributos as $campo => $valor) {
            if ($campo == $this->arChavePrimaria[0] && !$this->tabelaAssociativa)
                continue;
            if ($valor !== null) {
                if (!$valor && in_array($campo, $arCamposNulo)) {
                    continue;
                }
                $arCampos[] = $campo;
                $arValores[] = trim(pg_escape_string($valor));
            }
        }

        if (count($arValores)) {
            $sql = " insert into $this->stNomeTabela ( " . implode(', ', $arCampos) . " )
											  values ( '" . implode("', '", $arValores) . "' )
					 returning {$this->arChavePrimaria[0]}";
            $stChavePrimaria = $this->arChavePrimaria[0];
            return $this->$stChavePrimaria = $this->pegaUm($sql);
        }
    }

    /**
     * Função monta_combo
     * Esta função sobreescreve a função pai passando valores padrões.
     * OBS: Foi alterada a ordem do parâmetro $valor para a primeira posição para facilitar a escrita do método (não foi alterada a assinatura do método pai)
     * @since 31/05/2017
     */
    public function monta_combo($value = null, $var = null, $sql = null, $habil = 'S', $titulo = 'Selecione', $acao = '', $opc = '', $txtdica = '', $size = '', $obrig = 'N', $id = '', $return = false, $title = null, $complemento = 'class="form-control chosen-select" style="width=100%;"', $classe = null, $complementoOption = array(array('codigo' => array(), 'complemento' => '')), $percentual = null) {
        $pk = $this->arChavePrimaria[0];
        $sql = $sql ? $sql : $this->recuperarSqlCombo();

        parent::monta_combo($pk, $sql, $habil, $titulo, $acao, $opc, $txtdica, $size, $obrig, $pk, $return, $value, $title, $complemento, $classe, $complementoOption, $percentual);
    }

    public function recuperarSqlCombo($descricao = null, $where = [], $order = '') {
        $pk = $this->arChavePrimaria[0];

        foreach (array_keys($this->arAtributos) as $atributo) {
            if (substr($atributo, -6) == 'status') {
                $where[] = "{$atributo} = 'A'";
            } elseif (substr($atributo, -3) == 'ano') {
                $where[] = "$atributo = '{$_SESSION['exercicio']}'";
            }
            if ($descricao) {
                $descricao = implode(" || ' - ' || ", (array)$descricao);
            } else {
                $aPossibilidades = ['dsc', 'desc', 'nome', 'descricao'];
                foreach ($aPossibilidades as $possibilidade) {
                    if (substr($atributo, -(strlen($possibilidade))) == $possibilidade) {
                        $descricao = $atributo;
                        break;
                    }
                }
            }
        }
        $order = $order ? $order : $descricao;
        $where = count($where) ? " where  " . implode(' and ', $where) : '';

        $sql = "select {$pk} AS codigo, {$descricao} AS descricao
                from {$this->stNomeTabela}
                $where
                order by $order   
        ";

        return $sql;
    }

    public function realizarCarga($dados = array(), $utf8_decode = false) {
        $this->popularDadosObjeto((array) $dados, $utf8_decode);

        $arCampos = array();
        $arValores = array();
        $arSimbolos = array();

        foreach ($this->arAtributos as $campo => $valor) {

            if ($valor !== null) {
                $arCampos[] = $campo;
                $arValores[] = trim(pg_escape_string($valor));
            }
        }

        if (count($arValores)) {
            $sql = " insert into $this->stNomeTabela ( " . implode(', ', $arCampos) . " )
											  values ( '" . implode("', '", $arValores) . "' ) ";
            return $this->executar($sql);
        }
        return false;
    }

    /**
     * Monta filtros para pesquisas dinamicamente.
     * 
     * @example Exemplos de utilização: array( 'suo.unocod' => UNICOD_FNC, 'suo.unocod != '. UNICOD_FNC, 'suo.unocod' => array(UNICOD_FNC, 42101)
     * 
     * @param array $aFiltro
     * @param boolean $where Informa se é necessário construir com a palavra reservada WHERE.
     * @return string
     */
    public function montarFiltros($aFiltro = [], $where = FALSE){
        $filtros = '';
        if($where){
            $filtros = "\n WHERE";
        }
        $listaFiltros = self::montarListaFiltros($aFiltro);
        $filtros .= join("\n AND ", $listaFiltros);
        
        return $filtros;
    }
    
    /**
     * Monta lista de filtros.
     * 
     * @param array $aFiltro
     * @return string
     */
    public function montarListaFiltros($aFiltro = []){
        $listaFiltros = [];
        if($aFiltro){
            foreach($aFiltro as $campo => $valor){
                if(is_int($campo)){
                    $listaFiltros[] = $valor;
                } else {
                    if(is_array($valor)){
                        $strValor = "'". join("','", $valor). "'";
                        $listaFiltros[] = $campo. " IN(". $strValor. ")";
                    } else {
                        $listaFiltros[] = $campo. " = '". pg_escape_string($valor). "'";
                    }
                }
            }
        }
        
        return $listaFiltros;
    }

    public function gerarDocid($tipoFluxo, $docdsc)
    {
        $chavePrimaria = $this->arChavePrimaria[0];
        $id = $this->{$chavePrimaria};

        if ($id && !$this->docid) {
            $docdsc = $docdsc . ' - ' . $id;
            $docid = wf_cadastrarDocumento($tipoFluxo, $docdsc);

            $this->executar("UPDATE {$this->stNomeTabela} SET docid = $docid where {$chavePrimaria} = {$id}");
            $this->commit();

            $this->docid = $docid;
        }

        return wf_pegarEstadoAtual($this->docid);
    }
    
    function carregar($SQL, $var = null, $tempocache = null){
        $dados = parent::carregar($SQL, $var, $tempocache);
        return $dados ? $dados : [];
    }

}
