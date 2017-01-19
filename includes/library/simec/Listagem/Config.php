<?php
/**
 * Arquivo de implementa��o da classe de configura��es da Listagem.
 *
 * $Id: Config.php 103935 2015-10-21 16:54:47Z maykelbraz $
 * @filesource
 */

/**
 *
 */
require_once dirname(__FILE__) . '/../Listagem.php';

/**
 *
 *
 * @package Simec\View\Listagem
 */
class Simec_Listagem_Config
{
    /**
     * @var string Id do relat�rio.
     */
    protected $id = 'tb_render';

    /**
     * @var mixed[] Lista de a��es exibidas no relat�rio.
     */
    protected $acoes = array();
    /**
     * @var mixed[] Lista de condi��es para exibi��o das a��es no relat�rio.
     */
    protected $acoesCondicionais = array();
    /**
     * @var mixed[] Lista de campos de agrupamento das a��es no relat�rio.
     */
    protected $acoesAgrupadas = array();

    /**
     * @var string[] Lista de colunas que n�o s�o exibidas na listagem.
     */
    protected $colunasOcultas = array();
    /**
     * @var string[] Lista de callbacks de cria��o de colunas virtuais.
     */
    protected $colunasVirtuais = array();
    /**
     * @var string[] Lista com o nome das colunas da listagem, utilizado em conjunto com Simec_Listagem::SEM_RETORNO_LISTA_VAZIA.
     * @see \Simec_Listagem::SEM_RETORNO_LISTA_VAZIA
     */
    protected $listaColunas = array();

    /**
     * @var int Tipo de totalizador da listagem.
     *
     * @uses Simec_Listagem::TOTAL_SOMATORIO_COLUNA;
     * @uses Simec_Listagem::TOTAL_QTD_REGISTROS;
     */
    protected $totalizador;

    /**
     * @var string[] Lista de colunas totalizadas.
     */
    protected $colunasTotalizadas = array();

    /**
     * @var array Cada elemento � uma callback para um campo. Toda vez que o campo for ser impresso na listagem, ele
     * primeiro � processado pela callback registrada e o resultado do processamento � impresso no lugar do valor do
     * campo. Ex: $callbacksDeCampo = array('nome_do_campo' => nome_da_callback);
     */
    protected $callbacksDeCampo = array();

    /**
     * @var array Cada elemento define uma regra de formata��o de linha. As regras s�o compostas pelos seguintes
     * elementos: $regrasDeLinha = array(
     *     array('campo' => nome_do_campo, 'op' => igual|diferente|contem, 'valor' => valor_para_comparacao_com_campo, 'classe' => nome_da_classe_css_de_modificacao),
     * );
     */
    protected $regrasDeLinha = array();

    /**
     * @var string|mixed Conjunto de dados da lista.
     */
    protected $datasource;

    /**
     * @var string[] Armazena mensagens de valida��o de callbacks.
     */
    protected $mensagensCallback = array();

    public function salvar($namespace)
    {
        $_SESSION['simec-listagem'][$namespace] = serialize($this);
        return $this;
    }

    public static function carregar($namespace)
    {
        return unserialize($_SESSION['simec-listagem'][$namespace]);
    }

    // -- ID -----------------------------------------------------------------------------------------------------------
    /**
     * Define um ID para a listagem.
     *
     * @param string $id Id da listagem
     * @throws Exception N�o permite que o ID seja vazio.
     */
    public function setId($id)
    {
        if (empty($id)) {
            throw new Exception('O ID da listagem n�o pode ser definido como vazio.');
        }

        $this->id = $id;
    }

    /**
     * Retorna o ID da listagem.
     *
     * @return string Id da listagem
     */
    public function getId()
    {
        return $this->id;
    }

    // -- A��es --------------------------------------------------------------------------------------------------------
    /**
     * Retorna a lista de a��es adicionadas � listagem.
     *
     * @return mixed[] Lista de a��es.
     */
    public function getAcoes()
    {
        return $this->acoes;
    }

    /**
     * Retorna a quantidade de a��es adicionadas � listagem.
     *
     * @return int Quantidade de a��es.
     */
    public function getNumeroAcoes()
    {
        return count($this->acoes);
    }

    /**
     * Retorna as condi��es de exibi��o de uma a��o da listagem.
     *
     * @param string $acao Nome de uma a��o adicionada � listagem.
     * @return mixed[] Lista de condi��es de exibi��o da a��o.
     */
    public function getCondicaoAcao($acao)
    {
        return $this->acoesCondicionais[$acao];
    }

    /**
     * Verifica se uma a��o � agrupada.
     *
     * @param string $acao Nome de uma a��o.
     * @return bool
     */
    public function acaoEhAgrupada($acao)
    {
        return array_key_exists($acao, $this->acoesAgrupadas);
    }

    /**
     * Retorna a configura��o de agrupamento de uma a��o.
     *
     * @param string $acao Nome de uma a��o.
     * @return mixed[]
     */
    public function getAgrupamentoAcao($acao)
    {
        return $this->acoesAgrupadas[$acao];
    }

    /**
     * Adiciona uma nova a��o � listagem.
     *
     * @param string $acao Nome da a��o.
     * @param mixed[] $config Configura��es da a��o.
     * @param array|null $condicoes Lista de condi��es de exibi��o da a��o.
     * @param array|null $agrupamentos Lista de colunas de agrupamento da a��o.
     * @return \Simec_Listagem_Config
     *
     * @uses \Simec_Listagem_Config::setAcaoComoCondicional()
     * @uses \Simec_Listagem_Config::setAcaoComoAgrupada()
     */
    public function addAcao($acao, $config, array $condicoes = null, array $agrupamentos = null)
    {
        $this->acoes[$acao] = $config;

        if (!empty($condicoes)) {
            $this->setAcaoComoCondicional($acao, $condicoes);
        }

        if (!empty($agrupamentos)) {
            $this->setAcaoComoAgrupada($acao, $agrupamentos);
        }

        return $this;
    }

    /**
     * Define uma condi��o de exibi��o de uma a��o.
     *
     * @param string $acao Nome de uma a��o.
     * @param mixed[] $condicoes Lista de condi��es de exibi��o da a��o.
     * @return \Simec_Listagem_Config
     */
    public function setAcaoComoCondicional($acao, array $condicoes)
    {
        if (is_array($acao)) {
            foreach ($acao as $acao_) {
                $this->acoesCondicionais[$acao_] = $condicoes;
            }
        } else {
            $this->acoesCondicionais[$acao] = $condicoes;
        }

        return $this;
    }

    /**
     * Define um conjunto de colunas de agrupamento de uma a��o.
     *
     * @param string $acao Nome de uma a��o.
     * @param string[] $nomeColunas Lista de nome de campos do conjunto de dados para agrupamento da a��o.
     * @return \Simec_Listagem_Config
     */
    public function setAcaoComoAgrupada($acao, array $nomeColunas)
    {
        if (is_array($acao)) {
            foreach ($acao as $acao_) {
                $this->acoesAgrupadas[$acao_] = $nomeColunas;
            }
        } else {
            $this->acoesAgrupadas[$acao] = $nomeColunas;
        }

        return $this;
    }

    // -- Colunas ocultas ----------------------------------------------------------------------------------------------
    /**
     * Indica que uma, ou mais, colunas(s) da query n�o ser�(�o) exibida(s).
     *
     * @param string|array $nomeColuna Nome do campo da query que n�o ser� exibido na listagem.
     * @return \Simec_Listagem_Config
     */
    public function ocultarColunas($nomeColuna)
    {
        if (!is_array($nomeColuna)) {
            $this->colunasOcultas[] = $nomeColuna;
        } else {
            // -- Recebendo um array de campo para esconder as acolunas
            foreach ($nomeColuna as $coluna) {
                $this->colunasOcultas[] = $coluna;
            }
        }

        return $this;
    }

    /**
     * Retorna a lista de colunas ocultas.
     *
     * @return string[] Lista de colunas ocultas.
     */
    public function getColunasOcultas()
    {
        return $this->colunasOcultas;
    }

    /**
     * Quantidade de colunas ocultas.
     * @return int
     */
    public function getNumeroColunasOcultas()
    {
        return count($this->colunasOcultas);
    }

    /**
     * Indica se uma coluna est� na lista de colunas ocultas.
     * @param string $nomeColuna Nome da colunas para verifica��o.
     * @return bool
     */
    public function colunaEstaOculta($nomeColuna)
    {
        return in_array($nomeColuna, $this->colunasOcultas);
    }

    // -- Colunas virtuais ---------------------------------------------------------------------------------------------
    /**
     * Cria uma lista de colunas virtuais com a fun��o callback de gera��o da mesma.
     *
     * @param string|function $callback Fun��o de gera��o do valor da coluna;
     * @return \Simec_Listagem_Config
     */
    public function addColunaVirtual($callback)
    {
        $this->colunasVirtuais[] = $callback;
        return $this;
    }

    /**
     * Retorna a lista de colunas virtuais.
     * @return string[]
     */
    public function getColunasVirtuais()
    {
        return $this->colunasVirtuais;
    }

    // -- Lista de colunas ---------------------------------------------------------------------------------------------
    public function setListaColunas(array $listaColunas)
    {
        $this->listaColunas = $listaColunas;
        return $this;
    }

    public function getListaColunas()
    {
        return $this->listaColunas;
    }

    // -- Totalizadores ------------------------------------------------------------------------------------------------
    /**
     * Configura o tipo de totalizador da listagem, al�m de definir quais colunas s�o totalizadas, qdo aplic�vel.
     *
     * @param int $totalizador Tipo de totalizador que ser� utilizado (Simec_Listagem::TOTAL_QTD_REGISTROS, Simec_Listagem::TOTAL_SOMATORIO_COLUNAS).
     * @param string|array $colunas Coluna(s) que ser�(�o) totalizada(s).
     * @return \Simec_Listagem_Config
     * @throws Exception Lan�ada qdo um tipo inv�lido de totalizador � informado.
     *
     * @uses \Simec_Listagem::TOTAL_QTD_REGISTROS
     * @uses \Simec_Listagem::TOTAL_SOMATORIO_COLUNA
     */
    public function setTotalizador($totalizador, $colunas = null)
    {
        if ($totalizador != Simec_Listagem::TOTAL_QTD_REGISTROS && $totalizador != Simec_Listagem::TOTAL_SOMATORIO_COLUNA) {
            throw new Exception(
                'Tipo de totalizador inv�lido. Tipos v�lidos: Simec_Listagem:TOTAL_QTD_REGISTROS ou Simec_Listagem::TOTAL_SOMATORIO_COLUNA.'
            );
        }

        if (Simec_Listagem::TOTAL_SOMATORIO_COLUNA == $totalizador && !is_null($colunas)) {
            $this->totalizarColunas($colunas);
        }

        $this->totalizador = $totalizador;
        return $this;
    }

    public function getTotalizador()
    {
        return $this->totalizador;
    }

    /**
     * Lista de colunas que ser�o totalizadas.
     *
     * @param string|array $colunas Nome(s) da(s) coluna(s) que ser�o totalizadas.
     * @return \Simec_Listagem_Config
     */
    public function totalizarColunas($colunas)
    {
        if (empty($colunas)) {
            return $this;
        }

        if (!is_array($colunas)) {
            $this->colunasTotalizadas[] = $colunas;
        } else {
            foreach ($colunas as $coluna) {
                $this->colunasTotalizadas[] = $coluna;
            }
        }
    }

    /**
     * Retorna a lista de colunas totalizadas.
     *
     * @return string[]
     */
    public function getColunasTotalizadas()
    {
        return $this->colunasTotalizadas;
    }

    /**
     * Verifica se uma coluna � totalizada.
     * @param string $nomeColuna Nome da coluna totalizada.
     * @return bool
     */
    public function colunaEhTotalizada($nomeColuna)
    {
        return in_array($nomeColuna, $this->colunasTotalizadas);
    }

    // -- Cabecalho ----------------------------------------------------------------------------------------------------
    /**
     * Lista de t�tulos das colunas do relat�rio. Tamb�m cria t�tulos de duas colunas,
     * para isso, passe o nome da coluna principal como chave do array e as colunas filhas como
     * itens deste array.
     * Exemplo cabecalho simples:
     * $list = new Simec_Listagem();
     * $list->setCabecalho(array('Coluna 1', 'Coluna 2'));
     * Exemplo cabecalho de dois n�veis:
     * $list = new Simec_Listagem();
     * $list->setCabecalho(array('Grupo de colunas' => array('Coluna 1', 'Coluna 2'));
     * Obs: Se passar a string 'auto', ser�o utilizados os nomes das colunas presentes
     * no conjunto de dados da listagem.
     *
     * @param array|string $cabecalho Array com o t�tulo de cada coluna, ou a palavra 'auto'.
     * @todo Transformar o valor 'auto' no padr�o da classe.
     */
    public function setCabecalho($cabecalho)
    {
        if (('auto' != $cabecalho) && !is_array($cabecalho)) {
            throw new Exception("\$cabecalho deve ser um array, ou o valor 'auto'.");
        }
        $this->cabecalho = $cabecalho;
    }

    public function getCabecalho()
    {
        return $this->cabecalho;
    }

    // -- Callbacks de campo -------------------------------------------------------------------------------------------
    /**
     * Adiciona uma fun��o callback de processamento de conte�do de campo.
     * Uma a��o comum que pode ser executada com este m�todo, � a aplica��o de m�scara em um campo de CPF.
     *
     * Exemplo de utiliza��o:<pre>
     * function mascaraReal($valor)
     * {
     * &nbsp;&nbsp;&nbsp;&nbsp;return "R$ {$valor}";
     * }
     * [...]
     * $listagem = new Simec_Listagem();
     * $listagem->setQuery("SELECT '3.00' AS valor");
     * $listagem->setCabecalho(array('Valor'));
     * $listagem->addCallbackDeCampo('valor', 'mascaraReal');
     * $listagem->render();</pre>
     *
     * @param string|array $nomeColuna Nome(s) da(s) colunas(s) que receber�(�o) o tratamento.
     * @param string $nomeCallback Nome da fun��o de processamento do campo. Ela deve retornar sempre uma string.
     * @return \Simec_Listagem_Config
     * @throws Exception Gerada quando o nome da callback ou a pr�pria fun��o � inv�lida.
     */
    public function addCallbackDeCampo($nomeColuna, $nomeCallback)
    {
        if (empty($nomeColuna)) {
            return;
        }

        if (empty($nomeCallback)) { // -- Foi informado o nome da fun��o?
            throw new Exception('O nome da fun��o de callback do campo nao pode ser vazia.');
        }
        if (!is_callable($nomeCallback)) { // -- A fun��o foi declarada??
            throw new Exception("A fun��o '{$nomeCallback}' n�o est� declarada.");
        }

        // -- Preferencialmente (para funcionamento da exporta��o xls), as callbacks devem estar declaradas no arquivo
        // -- _funcoes.php do m�dulo.
        if (is_string($nomeCallback)) {
            $reflection = new ReflectionFunction($nomeCallback);
            $arquivoFuncao = $reflection->getFileName();
            if (!in_array(basename($arquivoFuncao), array('funcoesspo.php', '_funcoes.php'))) {
                $this->mensagensCallback[] = <<<HTML
Para que a exporta��o autom�tica XLS funcionar corretamente, a fun��o de callback <b>{$nomeCallback}()</b> deve ser declarada no arquivo <b>_funcoes.php</b> deste m�dulo. Declarada em: <u>{$arquivoFuncao}</u>.
HTML;
            }
//        } else {
//            throw new Exception('A exporta��o XML n�o funciona com callbacks an�nimas.');
        }

        // -- Recebendo um array de campo para adicionar como callback
        if (!is_array($nomeColuna)) {
            $this->callbacksDeCampo[$nomeColuna] = $nomeCallback;
        } else {
            foreach ($nomeColuna as $campo) {
                $this->callbacksDeCampo[$campo] = $nomeCallback;
            }
        }
        return $this;
    }

    /**
     * Verifica se uma coluna tem uma fun��o de callback associada a ela.
     * @param string $nomeColuna Nome da coluna para verifica��o.
     * @return bool
     */
    public function colunaTemCallback($nomeColuna)
    {
        return array_key_exists($nomeColuna, $this->callbacksDeCampo);
    }

    /**
     * Retorna o nome da fun��o callback associada a uma coluna.
     * @param string $nomeColuna Nome da coluna.
     * @return string
     */
    public function getCallback($nomeColuna)
    {
        return $this->callbacksDeCampo[$nomeColuna];
    }

    public function getMensagensCallback()
    {
        return $this->mensagensCallback;
    }

    // -- Regras de linha ----------------------------------------------------------------------------------------------
    /**
     * Adiciona uma nova regra de formata��o de linha.
     * A nova regra deve atender ao formato armazenado em self::$regrasDeLinha:
     *
     * @param array $regra
     * @return \Simec_Listagem_Config
     *
     * @todo validar a estrutura da nova regra a ser adicionada
     */
    public function addRegraDeLinha(array $regra)
    {
        $this->regrasDeLinha[] = $regra;

        return $this;
    }

    public function getRegrasDeLinha()
    {
        return $this->regrasDeLinha;
    }

    // -- Datasource --------------------------------------------------------------------------------------------------------
    public function setDatasource($datasource)
    {
        $this->datasource = $datasource;
        return $this;
    }

    public function getDatasource()
    {
        return $this->datasource;
    }
}
