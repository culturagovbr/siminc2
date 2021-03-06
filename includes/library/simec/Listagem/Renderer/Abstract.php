<?php
/**
 * Interface de defini��o de Renderers de Simec_Listagem.
 *
 * $Id: Abstract.php 103403 2015-10-06 21:10:22Z maykelbraz $
 * @filesource
 */

require_once dirname(__FILE__) . '/../Config.php';


/**
 *
 * @package Simec\View\Listagem\Renderer
 */
abstract class Simec_Listagem_Renderer_Abstract
{
    /**
     *
     * @var Simec_Listagem_Config Configura��es de exibi��o da listagem.
     */
    protected $config;

    /**
     * Armazena os dados utilizados para montar a listagem.
     * @var array
     * @see Simec_Listagem_Renderer_Abstract::setDados()
     */
    protected $dados;

    /**
     * Armazena o t�tulo do relat�rio.
     * Se estiver vazio, nenhum t�tulo � exibido.
     * @var string
     * @see Simec_Listagem::setTitulo()
     * @See Simec_Listagem::renderTitulo()
     */
    protected $titulo = '';

    /**
     * @var array Lista de colunas totalizadas com a soma dos valores processados at� o momento.
     */
    protected $totalColunas = array();

    /**
     * Atributo de ajuda para renderiza��o do titulo.
     *
     * @var bool
     * @todo Remover a utiliza��o deste campo.
     * @see Simec_Listagem::setDados()
     * @see Simec_Listagem::renderCabecalho()
     */
    protected $renderPrimeiroItem = true;

    public function __construct(array $dados = null)
    {
        if (!empty($dados)) {
            $this->setDados($dados);
        }
    }

    /**
     * Retorna a configura��o do cabecalho da listagem.
     *
     * @return string|string[]
     */
    protected function getCabecalho()
    {
        return $this->config->getCabecalho();
    }

    protected function setCabecalho($cabecalho)
    {
        $this->config->setCabecalho($cabecalho);
    }

    /**
     * Atribui ao render o objeto de renderiza��o da listagem.
     *
     * @param Simec_Listagem_Config $config Configura��es de exibi��o da listagem.
     * @return \Simec_Listagem_Renderer_Abstract
     */
    public function setConfig(Simec_Listagem_Config $config)
    {
        $this->config = $config;
        return $this;
    }

    /**
     * Retorna uma refer�ncia ao objeto de configura��o de exibi��o da listagem.
     *
     * @return \Simec_Listagem_Config
     */
    public function getConfig()
    {
        return $this->config;
    }

    /**
     * Retorna uma refer�ncia ao total das colunas totalizadas.
     *
     * @return array
     *
     * @uses \Simec_Listagem_Config::getColunasTotalizadas()
     */
    public function getColunasTotalizadas()
    {
        if (empty($this->totalColunas) && 0 !== $this->config->getColunasTotalizadas()) {
            $this->totalColunas = array_combine(
                $this->config->getColunasTotalizadas(),
                array_fill(0, count($this->config->getColunasTotalizadas()), 0)
            );
        }
        return $this->totalColunas;
    }

    /**
     * Carrega no objeto os dados que ser�o utilizados para criar a listagem.
     *
     * @param array $dados Dados para cria��o da listagem.
     */
    public function setDados($dados)
    {
        if (!is_array($dados)) {
            return false;
        }

        $this->dados = $dados;

        // -- Limpando indicador do primeiro campo a ser renderizado
        $this->renderPrimeiroItem = true;

        return $this;
    }

    /**
     * Adiciona um valor ao valor atual de uma coluna totalizada.
     *
     * @param string $nomeColuna Nome da coluna a ter o valor atualizado.
     * @param int|float $valor Valor a ser adicionado ao total da coluna.
     * @return \Simec_Listagem_Renderer_Abstract
     */
    public function somarColuna($nomeColuna, $valor)
    {
        if ($this->config->colunaEhTotalizada($nomeColuna)) {
            if (strpos($valor, '.')) {
                $valor = (double)$valor;
            } else {
                $valor = (int)$valor;
            }
            $this->totalColunas[$nomeColuna] += $valor;
        }

        return $this;
    }

    /**
     * Retorna o somat�rio de uma coluna.
     *
     * @param string $nomeColuna Nome da coluna.
     * @return int|float
     */
    public function getTotalColuna($nomeColuna)
    {
        return $this->totalColunas[$nomeColuna];
    }

    public function semDados()
    {
        return empty($this->dados);
    }

    /**
     * Define um t�tulo para o relat�rio.
     * @param string $titulo T�tulo a ser exibido acima do relat�rio.
     */
    public function setTitulo($titulo)
    {
        if (!empty($titulo)) {
            $this->titulo = $titulo;
        }
    }

    /**
     * Verifica se uma coluna precisa de callback e, se necess�rio, aplica a fun��o de callback associada.
     *
     * @param string $nomeColuna Nome da coluna para verifica��o.
     * @param mixed $valor Valor da coluna, par�metro principal da callback.
     * @param mixed[] $parametros Par�metros adicionais da callback: dados da linha, id da linha, array variado.
     * @return mixed
     */
    protected function aplicarCallback($nomeColuna, $valor, array $parametros = array())
    {
        if ($this->config->colunaTemCallback($nomeColuna)) {
            array_unshift($parametros, $valor);
            $valor = call_user_func_array(
                $this->config->getCallback($nomeColuna),
                $parametros
            );
        }
        return $valor;
    }


    /**
     * Faz a contagem de colunas da listagem, inclu�ndo colunas de a��es (quando presentes).
     * @todo Precisa disso? Precisa ser assim??? oO
     * @return integer
     */
    protected function quantidadeDeColunasExibidas()
    {
        $numColunasOcultas = $this->config->getNumeroColunasOcultas();

        if ($numColunasOcultas != 0) {
            $qtdColunasExibidas = count(
                array_diff_key( // -- Criar um array tempor�rio com os campos dados que n�o est�o inclusos na listagem de colunas ocultas
                    $this->dados[0],
                    array_combine( // -- Cria um array tempor�rio baseado nas colunas ocultas
                        $this->config->getColunasOcultas(),
                        array_fill(0, $numColunasOcultas, null)
                    )
                )
            );
        }
        // -- Ajuste da quantidade de colunas da query mediante contagem de a��es
        $numAcoes = $this->config->getNumeroAcoes();
        if ($numAcoes > 1) {
            // -- -1 pq a coluna de ID j� � contada em $qtdColunasExibidas
            $qtdColunasExibidas += $numAcoes - 1;
        }

        return $qtdColunasExibidas;
    }

    /**
     * Executa a cria��o da listagem de acordo com o Delegate implementado.
     */
    abstract public function render();

    abstract protected function renderTitulo();

    abstract protected function renderCabecalho();

    abstract protected function renderRodape();
}