<?php
/**
 * Implementa��o da classe de cria��o de listagens.
 *
 * @version $Id: Listagem.php 103935 2015-10-21 16:54:47Z maykelbraz $
 * @filesource
 */

/**
 * Construtor de a��es.
 * @see Simec_Listagem_FactoryAcao
 */
require_once dirname(__FILE__) . '/Listagem/FactoryAcao.php';
/**
 * Renderizador HTML.
 * @see Simec_Listagem_Renderer
 */
require_once dirname(__FILE__) . '/Listagem/Renderer/Html.php';
/**
 * Renderizador HTML.
 * @see Simec_Listagem_Renderer
 */
require_once dirname(__FILE__) . '/Listagem/Renderer/Xls.php';
/**
 * Classe com opera��es matem�ticas para a renderiza��o da listagem e a��es.
 * @see Simec_Operacoes
 */
require_once dirname(__FILE__) . '/Operacoes.php';
/**
 * Classe de encapsulamento de dados utilizados na listagem.
 * @see Simec_Listagem_Datasource
 */
require_once dirname(__FILE__) . '/Listagem/Datasource.php';
/**
 * Armazenamento de configura��es da listagem.
 * @see Simec_Listagem_Config
 */
require_once dirname(__FILE__) . '/Listagem/Config.php';

require_once dirname(__FILE__) . '/Listagem/Datasource.php';
require_once dirname(__FILE__) . '/Listagem/Datasource/Array.php';
require_once dirname(__FILE__) . '/Listagem/Datasource/Query.php';

/**
 * Classe de cria��o de relat�rios.
 *
 * *Importante*: Algumas op��es avan�adas s�o disponibilizadas pelos renderizadores. Para maiores informa��es,
 * ou op��es extras, veja a documenta��o de cada um deles.
 *
 * Lista de funcionalidades
 *
 * * Sa�da: HTML, XLS;
 * * Suporte a arrays de dados e queries.
 * * Suporte a totalizadores e somat�rios;
 * * A��es;
 * * A��es condicionais;
 * * Fun��es de callback para tratamento de dados do relat�rio (m�scara, formata��o, etc);
 * * Formata��o condicional de linhas;
 * * Retorno bufferizado;
 * * Pagina��o;
 * * Suporte � customiza��o de pagina��o;
 * * Exporta��o autom�tica de XLSs;
 * * Debug de query dependente de n�vel de usu�rio;
 * * Suporte a implementa��o de novos renderizadores e fontes de dados;
 * * Integra��o com o workflow.
 *
 * @package Simec\View\Listagem
 * @example
 * <code>
 * $dados = array(
 *     array('id' => 1, 'valor' => 3.00),
 *     array('id' => 2, 'valor' => 0.00),
 * );
 * $listagem = new Simec_Listagem();
 * $listagem->setDados($dados);
 * $listagem->setCabecalho(array('Valor'));
 * $listagem->addAcao('edit', 'editarValor');
 * $listagem->setAcaoComoCondicional('edit', array(
 *     array('campo' => 'valor', 'valor' => 0.00, 'op' => 'diferente'))
 * );
 * $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
 * </code>
 *
 * @author Maykel S. Braz <maykelbraz@mec.gov.br>
 * @todo Implementa��o de renderizadores PDF, CSV e IMPRESS�O.
 */
class Simec_Listagem
{
    /**
     * Indica que o relat�rio HTML deve ser paginado.
     */
    const RELATORIO_PAGINADO = 1;
    /**
     * Indica que o relat�rio HTML n�o deve ser paginado.
     */
    const RELATORIO_CORRIDO = 2;
//    const RELATORIO_IMPRESSAO = 3;
//    const RELATORIO_CSV = 4;
    /**
     * Indica que o relat�rio deve ser renderizado no formato XLS.
     */
    const RELATORIO_XLS = 5;
//    const RELATORIO_PDF = 6;

    /**
     * Indica que a chamada de self::render() ir� imprimir o conte�do do relat�rio.
     */
    const RETORNO_PADRAO = false; // -- SAIDA_PRINT
    /**
     * Indica que a chamada de self::render() ir� retornar uma string com o conte�do do relat�rio.
     */
    const RETORNO_BUFFERIZADO = true; // -- SAIDA_RETORNO

    /**
     * N�o inclui um rodap� na listagem.
     */
    const TOTAL_SEM_TOTALIZADOR = 1;
    /**
     * Inclui um rodap� no relat�rio com a quantidade de registros.
     */
    const TOTAL_QTD_REGISTROS = 2;
    /**
     * Inclui um rodap� no relat�rio com o somat�rio das colunas indicadas.
     */
    const TOTAL_SOMATORIO_COLUNA = 3;

    /**
     * Identifica que a query deve ser retornada com um SELECT COUNT(1) externo.
     * @todo N�o foi movido para o config?
     */
    const QUERY_COUNT = 1;
    /**
     * Identifica que a query deve ser retornada sem altera��es.
     * @todo N�o foi movido para o config?
     */
    const QUERY_NORMAL = 2;

    /**
     * Indica ao renderizador que deve imprimir mensagem de aviso se n�o houver registros.
     */
    const SEM_REGISTROS_MENSAGEM = 1;
    /**
     * Indica ao renderizador que deve retornar false se n�o houver registros.
     */
    const SEM_REGISTROS_RETORNO = 2;
    /**
     * Indica ao renderizador que a tabela principal deve ser mostrada, com uma mensagem interna.
     * Utilizado para listas din�micas, que receber�o elementos posteriormente.
     */
    const SEM_REGISTROS_LISTA_VAZIA = 3;

    /**
     * @var integer Indica o tipo de sa�da do relat�rio.
     * @uses Simec_Listagem::RELATORIO_PAGINADO Relat�rio HTML paginado.
     * @uses Simec_Listagem::RELATORIO_CORRIDO Relat�rio HTML sem pagina��o.
     * @uses Simec_Listagem::RELATORIO_XLS Relat�rio XLS.
     */
    protected $tipoRelatorio;

    /**
     * @var bool Armazena o tipo de sa�da do relat�rio.
     * @uses Simec_Listagem::RETORNO_PADRAO
     * @uses Simec_Listagem::RETORNO_BUFFERIZADO
     */
    protected $bufferizarRetorno;

    /**
     * @var Simec_Listagem_Renderer_Abstract Inst�ncia do renderer respons�vel por criar/formatar o conte�do do relat�rio.
     */
    protected $renderer;

    /**
     * Indica o tipo de totalizador do relat�rio.
     *
     * @var int
     * @see Simec_Listagem::setTotalizador()
     * @see Simec_Listagem::totalizarColunas()
     * @see Simec_Listagem::TOTAL_SEM_TOTALIZADOR
     * @see Simec_Listagem::TOTAL_QTD_REGISTROS
     * @see Simec_Listagem::TOTAL_SOMATORIO_COLUNA
     */
    protected $totalizador = Simec_Listagem::TOTAL_SEM_TOTALIZADOR;

    /**
     * Configura��es de legenda do relat�rio.
     * @var string
     * @todo Implementar a cria��o de legendas no relat�rio.
     */
    protected $legenda;

    /**
     * N�mero m�ximo de p�ginas que ser�o exibidas no seletor de p�ginas.
     * @var int
     */
    protected $numPaginasSeletor = 7;

    /**
     * @var \Simec_Listagem_Config Configura��es da listagem.
     */
    protected $config;

    /**
     * @var \Simec_Listagem_Datasource Fonte de dados da listagem.
     */
    protected $datasource;

    protected static $monitorarExport = false;
    protected static $namespaceExport;

    /**
     * @var int Armazenamento inicial para repassar para o datasource, depois de inici�-lo.
     */
    protected $paginaAtual;

    /**
     * Inicia o monitoramento da exporta��o da Simec_Listagem.
     *
     * Modo de uso: Inclua a chamada Simec_Listagem::monitorarExport('id') no arquivo {NOME_MODULO}.php acima
     * das chamadas do controle de acesso. IMPORTANTE: As fun��es de callback s� ser�o encontradas se estiverem
     * declaradas no arquivo _funcoes.php do m�dulo em quest�o.
     *
     * @param string $namespace Identificador onde ser�o armazenadas as configura��es da listagem.
     */
    public static function monitorarExport($namespace)
    {
        self::$monitorarExport = true;
        self::$namespaceExport = $namespace;

        self::exportarXLS();
    }

    protected static function exportarXLS()
    {
        if (isset($_POST['listagem']['requisicao'])
            && ('exportar-xls' == $_POST['listagem']['requisicao'])) {

            $config = Simec_Listagem_Config::carregar(self::$namespaceExport);
            $list = new self(self::RELATORIO_XLS);
            $list->setConfig($config);

            $datasource = $config->getDatasource();
            if (is_array($datasource)) {
                $list->setDados($datasource);
            } else {
                $list->setQuery($datasource);
            }

            $list->render();
            die();
        }
    }

    /**
     * Criar uma nova listagem com configura��o de tipo de relat�rio (paginado ou n�o) e o tipo de <br />
     * retorno (padr�o ou bufferizado).
     *
     * @param integer $tipoRelatorio Tipo de listagem que ser� criada.
     * @param int $tipoRetorno
     *      Indica se a sa�da da listagem deve ser retornada em uma vari�vel (self::RETORNO_BUFFERIZADO) ou
     *      deve ser exibida diretamente na tela (self::RETORNO_PADRAO).
     */
    public function __construct($tipoRelatorio = self::RELATORIO_PAGINADO, $tipoRetorno = self::RETORNO_PADRAO)
    {
        $this->setTipoRelatorio($tipoRelatorio);
        $this->setTipoRetorno($tipoRetorno);

        switch ($tipoRelatorio) {
            case Simec_Listagem::RELATORIO_XLS:
                $this->renderer = new Simec_Listagem_Renderer_Xls();
                break;
            default:
                $this->renderer = new Simec_Listagem_Renderer_Html();
        }

        // -- Se a exporta��o estiver ativada, adiciona o bot�o de exportar XLS
        if ((self::$monitorarExport) && ($this->renderer instanceof Simec_Listagem_Renderer_Html)) {
            $this->renderer->getToolbar()->add(Simec_Listagem_Renderer_Html_Toolbar::EXPORTAR_XLS);
        }

        $this->setConfig(new Simec_Listagem_Config());

        // -- Carrega os dados externos enviados � listagem (pagina��o, filtros e ordena��o)
        $this->carregarDadosExternos();
    }

    public function setId($id)
    {
        $this->config->setId($id);
        return $this;
    }

    public function getId()
    {
        return $this->config->getId();
    }

    protected function setConfig(Simec_Listagem_Config $config)
    {
        $this->config = $config;
        $this->renderer->setConfig($this->config);

        return $this;
    }

    /**
     * Fun��o de cria��o de conte�do para a coluna da listagem.
     *
     * @param string|function $callback
     */
    public function addColunaVirtual($callback)
    {
        $this->config->addColunaVirtual($callback);
    }

    /**
     * Define um novo renderizador para o compomente de listagem de dados
     * @param Simec_Listagem_Renderer_Abstract $renderer
     * @return $this
     */
    public function setRenderer(Simec_Listagem_Renderer_Abstract $renderer)
    {
        $this->renderer = $renderer;
        return $this;
    }

    /**
     * Define uma nova quantidade de registros para compor uma p�gina da listagem (n�mero de registros por p�gina).
     * @param int $tamanhoPagina Novo tamanho da p�gina.
     * @return Simec_Listagem
     */
    public function setTamanhoPagina($tamanhoPagina)
    {
        $this->datasource->setRegistrosPorPagina($tamanhoPagina);
        return $this;
    }

    /**
     * Retorna a quantidade de registros de uma pagina.
     * @return int
     * @todo transpor para datasource. tah usando?
     */
    public function getTamanhoPagina()
    {
        return $this->tamanhoPagina;
    }

    /**
     * Transfere as chamadas de m�todos n�o definidos para o renderizador.
     *
     * @param string $name O Nome do m�todo chamado.
     * @param string $arguments Lista de argumentos do m�todo chamado.
     * @return mixed|Simec_Listagem
     * @throws Exception Se o m�todo n�o est� implementado no renderizador, lan�a uma exce��o.
     */
    public function __call($name, $arguments)
    {
        // -- Verifica se a fun��o solicitada est� dispon�vel no renderer
        if (!is_callable(array($this->renderer, $name)) || !method_exists($this->renderer, $name)) {
            $rendererClass = get_class($this->renderer);
            throw new Exception("O m�todo '{$name}' n�o est� implementado no renderizador '{$rendererClass}'.");
        }
        $retorno = call_user_func_array(array($this->renderer, $name), $arguments);

        if (is_null($retorno)) {
            return $this;
        }
    }

    protected function setTipoRelatorio($tipoRelatorio)
    {
        if ($tipoRelatorio != self::RELATORIO_PAGINADO
            && $tipoRelatorio != self::RELATORIO_CORRIDO
            && $tipoRelatorio != self::RELATORIO_XLS
                ) {
            throw new Exception(
                'Tipo de relat�rio inv�lido. Tipos v�lidos: Simec_Listagem:RELATORIO_PAGINADO ou Simec_Listagem::RELATORIO_CORRIDO.'
            );
        }

        $this->tipoRelatorio = $tipoRelatorio;
        return $this;
    }

    public function getPaginaAtual()
    {
        return $this->paginaAtual;
    }

    protected function setPaginaAtual($novaPaginaAtual)
    {
        $this->paginaAtual = $novaPaginaAtual;
        return $this;
    }

    /**
     * Carrega informa��es de p�gina��o, filtros e ordena��o (todos considerando o tipo do relat�rio).
     */
    protected function carregarDadosExternos()
    {
        if (Simec_Listagem::RELATORIO_PAGINADO == $this->tipoRelatorio) {
            $novaPagina = $_POST['listagem']['p'];
            if ('0' == $novaPagina || !$novaPagina) {
                $novaPagina = 1;
            }

            if ('all' == $novaPagina) {
                $this->paginaAtual = 'all';
            } else {
                $this->paginaAtual = (int)$novaPagina;
            }
        }

        // -- Carregando filtros, ordena��o e n�mero de p�gina para o relat�rio renderizado em HTML
        if ($this->renderer instanceof Simec_Listagem_Renderer_Html) {
            $this->renderer->setFiltros($_POST['filtro']);
            $this->renderer->setOrdenacao($_POST['campo_ordenacao']);
            $this->renderer->setPaginaAtual($novaPagina);
        }
    }

    /**
     * Troca o tipo de sa�da do relat�rio.
     * Por padr�o, o relat�rio � impresso na tela, mas a sa�da pode ser mudada para retorno do HTML.
     *
     * @param int $tipoRetorno Um dos tipos v�lidos de sa�da de relat�rio.
     * @throws Exception Lan�a exce��o quando tipo informado � inv�lido.
     * @return \Simec_Listagem
     * @see Simec_Listagem::RETORNO_PADRAO
     * @see Simec_Listagem::RETORNO_BUFFERIZADO
     */
    public function setTipoRetorno($tipoRetorno)
    {
        if ($tipoRetorno != self::RETORNO_PADRAO && $tipoRetorno != self::RETORNO_BUFFERIZADO) {
            throw new Exception(
                'Tipo de sa�da inv�lido. Tipos v�lidos: Simec_Listagem:RETORNO_PADRAO ou Simec_Listagem::RETORNO_BUFFERIZADO.'
            );
        }
        $this->bufferizarRetorno = $tipoRetorno;
        return $this;
    }

    /**
     * Carrega na listagem o conjunto de dados para exibi��o.
     *
     * Geralmente o array ir� conter apenas a lista de linhas do relat�rio,
     * no entanto, em casos especiais � poss�vel adicionar chaves especiais
     * neste array de forma a mudar o seu comportamente:
     * **dados**: Ao utilizar um array do tipo configura��o, as linhas de dados
     * devem vir entro desta chave. Sua exist�ncia � que muda o funcionamento
     * do datasource.
     * query: Armazena a query utilizada para a cria��o do array de dados, �
     * a �nica chave com impacto fora do datasource e serve para ser exibida na toolbar
     * da listagem.
     *
     * @param array $dados Array de dados/configura��o do datasource do tipo array.
     * @return \Simec_Listagem
     * @uses \Simec\Listagem\Simec_Listagem_Datasource_Array
     * @uses \Simec_Listagem::showQuery()
     */
    public function setDados($dados)
    {
        if (!is_array($dados)) {
            $dados = array();
        }

        $this->datasource = new Simec_Listagem_Datasource_Array();
        $this->datasource->setSource($dados);

        // -- Salvando para execu��o posterior autom�tica, ex: xls
        $this->config->setDatasource($this->datasource->getSource());
        $this->datasource->setPaginaAtual($this->paginaAtual);

        if (array_key_exists('query', $dados)) {
            // -- Adicionando bot�o de visualiza��o da query na toolbar da listagem
            $this->showQuery($dados['query']);
        }
        if (array_key_exists('pagina', $dados)) {
            $this->setPaginaAtual($dados['pagina']);
        }

        return $this;
    }

    /**
     * Carrega no objeto a query respons�vel por recuperar os dados que ser�o listados.
     * Esta fun��o � uma alternativa a Simec_Listagem::setDados().
     *
     * @param string $query String SQL para carregar os dados da listagem.
     * @param int $queryTimeout N�mero de segundos que a query deve ficar armazenada em cache. O valor 0 faz com que seja armazenado para sempre.
     * @return \Simec_Listagem
     * @see Simec_Listagem::setDados()
     */
    public function setQuery($query, $queryTimeout = null)
    {
        $this->datasource = new Simec_Listagem_Datasource_Query();
        $this->datasource->setSource($query, array('timeout' => $queryTimeout));
        $this->datasource->setPaginaAtual($this->paginaAtual);

        // -- Salvando para execu��o posterior autom�tica, ex: xls
        $this->config->setDatasource($this->datasource->getSource());

        // -- Adicionando bot�o de visualiza��o da query na toolbar da listagem
        $this->showQuery($query);

        return $this;
    }

    /**
     * Adiciona � toolbar da listagem o bot�o de exibi��o da query.
     * Para adicionar o bot�o, verifica se o usu�rio atualmente � superusu�rio e se
     * o renderizador � um Simec_Listagem_Renderer_Html.
     *
     * @param string $query Query para exibi��o no debug da toolbar.
     * @return \Simec_Listagem
     */
    protected function showQuery($query)
    {
        // -- Adicionando bot�o de visualiza��o da query na toolbar da listagem
        if ($_SESSION['superuser'] && $this->renderer instanceof Simec_Listagem_Renderer_Html) {
            $this->renderer->getToolbar()->add(Simec_Listagem_Renderer_Html_Toolbar::QUERY);
            $this->renderer->getToolbar()->setQuery($query);
        }

        return $this;
    }

    /**
     * Retorna a query utilizada pelo relat�rio. Se o relat�rio for paginado, retorna a query para pagina��o.
     * @return string
     *
     * @todo N�o foi removido?
     */
    protected function getQuery($formatoQuery = null)
    {
        if (empty($this->query)) {
            throw new Exception('Nenhuma query foi definida para a listagem.');
        }
        if (self::QUERY_COUNT == $formatoQuery) {
            return <<<DML
SELECT COUNT(1) FROM ({$this->query}) lst
DML;
        }
        if (self::QUERY_NORMAL == $formatoQuery || 'all' == $this->getPaginaAtual()) {
            return $this->query;
        }

        // -- Relat�rio sem pagina��o
        if ($this->tipoRelatorio != Simec_Listagem::RELATORIO_PAGINADO) {
            return $this->query;
        }

        // -- Relat�rio paginado
        return $this->query . ' OFFSET ' . $this->calculaOffset() . " LIMIT {$this->tamanhoPagina}";
    }

    /**
     * Define um t�tulo para o relat�rio.
     *
     * Exemplo de utiliza��o:
     * $list = new Simec_Listagem();
     * $list->setTitulo('Relat�rio de movimenta��o');
     *
     * @param string $titulo T�tulo a ser exibido acima do relat�rio.
     * @return \Simec_Listagem
     */
    public function setTitulo($titulo)
    {
        $this->renderer->setTitulo($titulo);
        return $this;
    }

    /**
     * Lista de t�tulos das colunas do relat�rio.
     *
     * O t�tulo pode ter elementos em dois n�veis, para isso, passe o nome da coluna
     * principal como chave do array e as colunas filhas como itens deste array.
     * Exemplo cabecalho simples:<pre>
     * $list = new Simec_Listagem();
     * $list->setCabecalho(array('Coluna 1', 'Coluna 2'));
     * Exemplo cabecalho de dois n�veis:
     * $list = new Simec_Listagem();
     * $list->setCabecalho(array('Grupo de colunas' => array('Coluna 1', 'Coluna 2'));</pre>
     *
     * @param mixed $cabecalho
     * @return \Simec_Listagem
     */
    public function setCabecalho($cabecalho)
    {
        $this->config->setCabecalho($cabecalho);
        return $this;
    }

    /**
     * Configura o tipo de totalizador da listagem, adicionalmente, informa quais colunas ser�o totalizadas.
     *
     * @param int $totalizador Tipo de totalizador da listagem (Simec_Listagem::TOTAL_SOMATORIO_COLUNA e Simec_Listagem::TOTAL_QTD_REGISTROS)
     * @param string|null|array $colunas Lista de colunas que ser�o totalizadas.
     * @return \Simec_Listagem
     *
     * @uses Simec_Listagem::TOTAL_SOMATORIO_COLUNA;
     * @uses Simec_Listagem::TOTAL_QTD_REGISTROS;
     */
    public function setTotalizador($totalizador, $colunas = null)
    {
        $this->config->setTotalizador($totalizador, $colunas);
        return $this;
    }

    /**
     * Define a lista de colunas que a listagem receber� do conunto de dados.
     *
     * Este m�todo � utilizado em conjunto com a op��o Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA, geralmente
     * utilizado quando a funcionalidade de adicionar novas linhas � listagem est� ativa.
     * @param string[] $listaColunas Lista com as colunas da listagem.
     * @return \Simec_Listagem
     * @see \Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA
     * @uses \Simec_Listagem_Config::setListaColunas()
     */
    public function setListaColunas(array $listaColunas)
    {
        $this->config->setListaColunas($listaColunas);
        return $this;
    }

    /**
     * Define a lista de campos que a listagem ir� receber da consulta ou no array de dados.
     *
     * Este m�todo � utilizado em conjunto com a op��o Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA, geralmente
     * utilizado quando a funcionalidade de adicionar novas linhas � listagem est� ativa.
     *
     * @param string[] $listaColunas Lista com as colunas da listagem.
     * @return \Simec_Listagem
     * @deprecated Utilize \Simec_Listagem::setListaColunas();
     */
    public function setCampos(array $listaColunas)
    {
        $this->setListaColunas($listaColunas);
        return $this;
    }

    /**
     * Indica que um, ou mais, campo(s) da query n�o ser�(�o) exibido(s) na listagem.
     *
     * @param string|string[]|string,... $nomeColuna Nome do campo da query que n�o ser� exibida na listagem.
     * @return \Simec_Listagem
     */
    public function esconderColunas($nomeColuna)
    {
        foreach (func_get_args() as $arg) {
            $this->config->ocultarColunas($arg);
        }
        return $this;
    }

    /**
     * Adiciona a��es �s linhas da listagem.
     *
     * Ao adicionar uma a��o, voc� pode utilizar o formato simples ou, para maior controle,
     * o formato avan�ado.
     *
     * O formato simples � composto pelo nome da a��o e o nome da callback js que ser�
     * invocada. O valor passado para todas a��es simples � o da primeira coluna da listagem
     * e esta coluna deixa de ser exibida na listagem. Ex:<pre>
     * $listagem = new Simec_Listagem();
     * $listagem->addAcao('plus', 'detalharItem');
     * ...
     * $listagem->render();</pre>
     *
     * O formato avan�ado � composto pelo nome da a��o e um array de configura��o, onde,
     * os pares chave/valor definem as op��o daquela a��o. Ex:<pre>
     * $listagem = new Simec_Listagem();
     * $listagem->addAcao('plus', array('func' => 'detalharItem', 'extra-params' => array('idLinha', 'exercicio')));
     * ...
     * $listagem->render();</pre>
     *
     * Para uma lista completa de configura��es da a��o, veja a class Simec_Listagem_Acao.
     * Para uma lista completa das a��es dispon�veis, veja as classes filhas de Simec_Listagem_Acao.
     *
     * @param string $acao Identificador de uma a��o
     * @param string|mixed[] $config Nome de fun��o callback js, ou array de configura��o da a��o.
     * @return \Simec_Listagem
     * @see \Simec_Listagem_Acao
     * @todo Verificar se o tipo de renderer suportar a��es
     */
    public function addAcao($acao, $config)
    {
        $this->config->addAcao($acao, $config);
        return $this;
    }

    /**
     * Define condi��es para que uma a��o seja exibida em uma listagem. Ao definir uma condi��o, mais de uma a��o pode
     * ser informada.
     *
     * A a��o s� ser� exibida se atender a todas as condi��es atribu�das. A condi��o � criada verificando valores do
     * conjunto de dados da listagem. Se mais de uma condi��o for definida para a a��o, esta s� ser� exibida se todas
     * as condi��es forem atendidas. Exemplo de utiliza��o:<pre>
     *
     * $dados = array(array('valor' => 3.00), array('valor' => 0.00));
     * $listagem = new Simec_Listagem();
     * $listagem->setDados($dados);
     * $listagem->setCabecalho(array('Valor'));
     * $listagem->addAcao('edit', 'editarValor');
     * $listagem->setAcaoComoCondicional('edit', array(array('campo' => 'valor', 'valor' => 0.00, 'op' => 'diferente')));
     * $listagem->render();</pre>
     *
     * Desta forma, a a��o de edi��o s� ser� exibida se o valor do campo 'valor' for igual a 0.00.
     *
     * @param string|string[] $acao Nome da a��o, ou a��es, que ser�o exibidas de acordo com a condi��o definida.
     * @param array $condicoes Array de configura��o da(s) condi��o(�es) de exibi��o da a��o.
     * @return \Simec_Listagem
     */
    public function setAcaoComoCondicional($acao, array $condicoes)
    {
        $this->config->setAcaoComoCondicional($acao, $condicoes);
        return $this;
    }

    /**
     * Agrupa uma a��o entre linhas da listagem.
     *
     * � importante que a ordena��o dos dados seja compat�vel com o agrupamento solicitado.
     *
     * @param string $acao O nome de uma a��o adicionada � listagem.
     * @param string[] $campos Lista de colunas que ser�o consideradas para fazer o agrupamento.
     * @return \Simec_Listagem
     * @todo Verificar se o tipo de renderer suportar a��es
     */
    public function setAcaoComoAgrupada($acao, array $campos)
    {
        $this->config->setAcaoComoAgrupada($acao, $campos);
        return $this;
    }

    /**
     * Adiciona uma nova regra de formata��o de linha.
     * A nova regra deve atender ao formato armazenado em self::$regrasDeLinha:
     *
     * @param array $regra
     * @todo validar a estrutura da nova regra a ser adicionada
     * @see Simec_Listagem::$regrasDeLinha
     */
    public function addRegraDeLinha(array $regra)
    {
        $this->config->addRegraDeLinha($regra);
        return $this;
    }

    /**
     * Adiciona uma fun��o callback de processamento de conte�do de campo.
     *
     * Uma a��o comumente realizada com este m�todo � a aplica��o de m�scara em um campo de CPF ou monet�rio.
     * Mais de um campo pode ser informado na mesma chamada, basta utilizar um array com a lista de campos, ao
     * inv�s do nome de um �nico campo. Exemplo de utiliza��o:<pre>
     * function mascaraReal($valor) { return "R$ {$valor}"; }
     * ...
     * $listagem = new Simec_Listagem();
     * $listagem->setQuery("SELECT '3.00' AS valor");
     * $listagem->addCallbackDeCampo('valor', 'mascaraReal');
     * $listagem->render();</pre>
     *
     * Tamb�m � poss�vel utilizar uma fun��o an�nima para executar a formata��o, basta substituir o
     * nome da fun��o pela declara��o da fun��o an�nima.
     *
     * @param string|array $nomeCampo Nome(s) do(s) campo(s) que receber�(�o) o tratamento.
     * @param string $nomeCallback Nome da fun��o de processamento do campo. Ela deve retornar sempre uma string.
     * @return \Simec_Listagem
     * @throws Exception Gerada quando o nome da callback ou a pr�pria fun��o � inv�lida.
     */
    public function addCallbackDeCampo($nomeCampo, $nomeCallback)
    {
        $this->config->addCallbackDeCampo($nomeCampo, $nomeCallback);
        return $this;
    }

    /**
     * Se o renderer suportar, ativa o pesquisator - busca r�pida, na listagem.
     *
     * @return \Simec_Listagem
     */
    public function turnOnPesquisator()
    {
        if (!method_exists($this->renderer, 'turnOnPesquisator')) {
            $renderizador = get_class($this->renderer);
            throw new Exception("O renderizador atual ({$renderizador}) n�o suporta o pesquisator.");
        }
        $this->renderer->turnOnPesquisator();

        return $this;
    }

    /**
     * Adiciona a��es � toolbar da lista. Veja a descri��o das a��es em Simec_Listagem_Renderer_Html_Toolbar.
     *
     * Dispon�vel apenas para o renderizador HTML.
     *
     * @param int $tipo ID do bot�o que ser� adicionado.
     * @return \Simec_Listagem
     * @see \Simec_Listagem_Renderer_Html_Toolbar
     */
    public function addToolbarItem($tipo)
    {
        $this->renderer->addToolbarItem($tipo);
        return $this;
    }

    /**
     * Se o renderizador suportar, ativa o prot�tipo de linha - cria um atributo na
     * listagem com o conte�do de uma linha, para cria��o de novas linhas com js.
     *
     * @return \Simec_Listagem
     */
    public function turnOnPrototipo()
    {
        if (!method_exists($this->renderer, 'turnOnPrototipo')) {
            $renderizador = get_class($this->renderer);
            throw new Exception("O renderizador atual ({$renderizador}) n�o suporta prot�tipo de linha.");
        }
        $this->renderer->turnOnPrototipo();
        return $this;
    }

    /**
     * Desliga o formul�rio da listagem - geralmente utilizado qdo � preciso inserir a listagem dentro de um form.
     * @return \Simec_Listagem
     */
    public function turnOffForm()
    {
        $this->renderer->renderizarForm = false;
        return $this;
    }

    /**
     * Cria o output da listagem.
     *
     * O formato gerado depender� do renderer que foi definido para a listagem.
     *
     * @param int $tratamentoListaVazia Indica qual ser� o comportamente da lista caso nenhum registro seja retornado.
     * @return bool|string
     * @throws Exception
     * @todo Refatorar este m�todo.
     */
    public function render($tratamentoListaVazia = Simec_Listagem::SEM_REGISTROS_RETORNO)
    {
        if (is_null($this->datasource)) {
            throw new Exception('A listagem n�o pode ser renderizada sem dados. Utilize Simec_Listagem::setDados(), Simec_Listagem::setQuery() ou Simec_Listagem::setDatasource() para carregar os dados da listagem.');
        }

        if ((Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA == $tratamentoListaVazia) && !$this->config->getListaColunas()) {
            throw new Exception('Para usar "Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA" como op��o de '
                . 'retorno, � necess�rio chamar Simec_Listagem::setCampos() informando a lista de campos. '
                . 'Caso esteja utilizando Simec_View_Form::addInputLista(), inclua a lista de campos no '
                . 'par�metro de op��es utilizando a chave "campos".');
        }

        // -- Armazenando a sa�da em um buffer do relat�rio
        if (self::RETORNO_BUFFERIZADO == $this->bufferizarRetorno) {
            ob_start();
        }

        // -- Mensagens de debug da exporta��o de XLS
        if (self::$monitorarExport && !IS_PRODUCAO && $this->renderer instanceof Simec_Listagem_Renderer_Html) {
            foreach ($this->config->getMensagensCallback() as $mensagem) {
                echo <<<HTML
<div class="col-md-8 col-md-offset-2 listagem-info">
    <div class="alert alert-info" role="alert"><b>Simec_Listagem Info:</b> {$mensagem}</div>
</div>
<br style="clear:both" />
HTML;
            }
        }

        // -- Valida��es de lista vazia
        if ($this->datasource->estaVazio()) {
            switch ($tratamentoListaVazia) {
                case Simec_Listagem::SEM_REGISTROS_MENSAGEM:
                    $idListaVazia = $this->config->getId();
                    echo <<<HTML
<div style="margin-top:20px;" class="alert alert-info col-md-4 col-md-offset-4 text-center nenhum-registro"
    id="{$idListaVazia}">Nenhum registro encontrado</div>
<br style="clear:both" />
<br />
HTML;
                    if (self::RETORNO_BUFFERIZADO == $this->bufferizarRetorno) {
                        return ob_get_clean();
                    }
                    return;
                case Simec_Listagem::SEM_REGISTROS_RETORNO:
                    if (self::RETORNO_BUFFERIZADO == $this->bufferizarRetorno) {
                        ob_end_clean();
                    }
                    return false;
                case Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA:
                    // -- Neste caso, a lista precisa continuar sendo renderizada, mesmo vazia
                    break;
            }
        }

        // -- Verifica se a p�gina atualmente solicitada � v�lida, sen�o, joga para a primeira p�gina
        if ((Simec_Listagem::RELATORIO_PAGINADO == $this->tipoRelatorio)
             && ($this->datasource->getTotalPaginas() < $this->getPaginaAtual())
             && ($this->getPaginaAtual() != 'all')) {
            $this->setPaginaAtual(1);
        }

        $this->renderer->setDados($this->datasource->getDados());

        if (($this->renderer instanceof Simec_Listagem_Renderer_Html) && $this->config->getListaColunas()) {
            $this->renderer->setCampos($this->config->getListaColunas());
        }

        // -- Tratar a pagina��o aqui, fornecendo para o renderer apenas os dados para renderiza��o
        $this->renderer->render();

        if (Simec_Listagem::RELATORIO_PAGINADO == $this->tipoRelatorio) {
            // -- Inclui a sele��o de p�ginas no final da listagem
            $this->renderPaginador();
        }

        if ((self::$monitorarExport) && ($this->renderer instanceof Simec_Listagem_Renderer_Html)) {
            $this->config->salvar(self::$namespaceExport);
        }

        // -- Armazenando a sa�da em um buffer do relat�rio
        if (self::RETORNO_BUFFERIZADO == $this->bufferizarRetorno) {
            $listagem = ob_get_contents();
            ob_end_clean();

            return $listagem;
        }
    }

    /**
     * @todo Mover para o renderer??? Abstract de paginador???
     * @return type
     */
    protected function renderPaginador()
    {
        // -- Se n�o foi preciso paginar, n�o exibe o paginador
        if (!$this->datasource->paginar()) {
            return;
        }

        echo <<<HTML
        <div class="row container-listing">
            <div class="col-lg-12" style="padding-bottom:20px;text-align:center">
HTML;

        if ('all' == $this->datasource->getPaginaAtual()) {
            echo <<<HTML
                <ul class="pagination">
                    <li class="pgd-item" data-pagina="1">
                        <a href="javascript:void(0)">Paginar</a>
                    </li>
                </ul>
HTML;
        } else {

            $paginaAtual = (int)$this->datasource->getPaginaAtual();
            $paginaAnterior = ($paginaAtual - 1);
            $desabilitarAnterior = '';
            if ($paginaAnterior <= 0) {
                $desabilitarAnterior = ' disabled';
            }
            echo <<<HTML
                    <ul class="pagination">
                        <li class="pgd-item{$desabilitarAnterior}" data-pagina="{$paginaAnterior}">
                            <a href="javascript:void(0);">&laquo;</a>
                        </li>
HTML;
            if ((int)$paginaAnterior > 3) {
                echo <<<HTML
                        <li class="pgd-item" data-pagina="1">
                            <a href="javascript:void(0);">&laquo; 1</a>
                        </li>
HTML;
            }
            $listaPaginas = $this->gerarListaPaginas();

            // -- Imprimindo as p�ginas do seletor
            foreach ($listaPaginas as $numPagina) {
                $paginaAtualCSS = '';
                if ($paginaAtual == $numPagina) {
                    $paginaAtualCSS = ' active';
                }
                echo <<<HTML
                        <li class="pgd-item{$paginaAtualCSS}" data-pagina="{$numPagina}">
                            <a href="javascript:void(0)">{$numPagina} </a>
                        </li>
HTML;
            }
            $ultimaPagina = $this->datasource->getTotalPaginas();
            if (!in_array($ultimaPagina, $listaPaginas)) {
                echo <<<HTML
                        <li class="pgd-item" data-pagina="{$ultimaPagina}">
                            <a href="javascript:void(0)">{$ultimaPagina} &raquo;</a>
                        </li>
HTML;
            }
            $desabilitarProxima = '';
            if ($paginaAtual == $ultimaPagina) {
                $desabilitarProxima = ' disabled';
            }
            $proximaPagina = $paginaAtual + 1;
            echo <<<HTML
                    <li class="pgd-item{$desabilitarProxima}" data-pagina="{$proximaPagina}">
                        <a href="javascript:void(0)">&raquo;</a>
                    </li>
                </ul>
                <ul class="pagination">
                    <li class="pgd-item" data-pagina="all">
                        <a href="javascript:void(0)">Mostrar todos</a>
                    </li>
                </ul>
HTML;
        }
        echo <<<HTML
            </div>
        </div>
HTML;
    }

    protected function gerarListaPaginas()
    {
        $metadeDasPaginas = floor($this->numPaginasSeletor / 2);
        $qtdPaginasAnteriores = -1 * $metadeDasPaginas;
        $qtdPaginasPosteriores = $metadeDasPaginas;
        $listaPaginas = array();
        $paginaAtual = $this->datasource->getPaginaAtual();

        // -- A lista de p�ginas que devem ser exibidas
        for ($qtdPaginasAnteriores; $qtdPaginasAnteriores <= 0; $qtdPaginasAnteriores++) {
            // -- Se a p�gina for menor que zero, n�o exibe a p�gina e cria uma nova p�gina posterior
            if ($paginaAtual + $qtdPaginasAnteriores <= 0) {
                $qtdPaginasPosteriores++;
                continue;
            }
            $listaPaginas[] = $paginaAtual + $qtdPaginasAnteriores;
        }

        for ($i = 1; $i < $qtdPaginasPosteriores + ($this->numPaginasSeletor % 2); $i++) {
            if ($paginaAtual +$i > $this->datasource->getTotalPaginas()) {
                break;
            }
            $listaPaginas[] = $paginaAtual + $i;
        }

        return $listaPaginas;
    }

    /**
     * Adiciona um novo campo no formul�rio da listagem.
     * @param array $campos Configura��o do campo com: id, name e type.
     */
    public function addCampo(array $campos)
    {
        $this->renderer->addCampo($campos);
        return $this;
    }

    public function mostrarImportar($mostrar = true)
    {
        $this->renderer->mostrarImportar($mostrar);
        return $this;
    }

    // -- M�todos depreciados ------------------------------------------------------------------------------------------

    /**
     * Utilizar Simec_Listagem:setTotalizador() ou Simec_Listagem::totalizarColunas()
     * para definir quais colunas do relat�rio ser�o totalizadas.
     *
     * @deprecated
     * @param string $nomeCampo
     * @return \Simec_Listagem
     */
    public function totalizarColuna($nomeCampo)
    {
        return $this->totalizarColunas($nomeCampo);
    }

    /**
     * Indica que um campo da query n�o ser� exibido.
     * Utilizar Simec_Listagem::esconderColunas() para esconder uma,
     * ou mais colunas.
     *
     * @deprecated
     * @param string $nomeCampo Nome do campo da query que n�o ser� exibida na listagem.
     * @return \Simec_Listagem
     */
    public function esconderColuna($nomeCampo)
    {
        return $this->esconderColunas($nomeCampo);
    }

    /**
     * Use Simec_Listagem::addAcao no lugar de setAcoes.
     *
     * @deprecated
     * @param type $acoes
     * @return \Simec_Listagem
     */
    public function setAcoes($acoes)
    {
        if (empty($acoes)) {
            return $this;
        }
        foreach ($acoes as $acao => $config) {
            $this->addAcao($acao, $config);
        }
        return $this;
    }

    /**
     * Utilize Simec_Listagem::setTipoRetorno();
     * @deprecated
     */
    public function trocaTipoSaida($tipoRetorno)
    {
        return $this->setTipoRetorno($tipoRetorno);
    }

    /**
     * Utilize Simec_Listagem::turnOffForm();
     * @return \Simec_Listagem
     * @deprecated
     */
    public function setFormOff()
    {
        return $this->turnOffForm();
    }

    /**
     * Define quais colunas ser�o totalizadas.
     *
     * Atualmente utilizada em conjunto com Simec_Listagem::setTotalizador(). Prefira a utiliza��o por l�.
     *
     * @param string|string[] $campos Campo ou lista de campos para totaliza��o.
     * @return \Simec_Listagem
     * @deprecated
     */
    public function totalizarColunas($campos)
    {
        $this->config->totalizarColunas($campos);
        return $this;
    }
}
