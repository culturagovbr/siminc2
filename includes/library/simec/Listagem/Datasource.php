<?php
/**
 * Implementa��o de uma classe abstrata de fonte de dados para a Simec_Listagem.
 *
 * $Id: Datasource.php 103935 2015-10-21 16:54:47Z maykelbraz $
 * @filesource
 * @package Simec\Listagem\Datasource
 */

/**
 * Classe abstrata de fonte de dados.
 *
 * @package Simec\Listagem\Datasource
 * @see \Simec_Listagem
 */
abstract class Simec_Listagem_Datasource
{
    /**
     * Quantidade de registros por p�gina.
     */
    const TAMANHO_PADRAO_PAGINA = 100;

    /**
     * @var mixed Fonte de dados.
     */
    protected $source = null;

    /**
     * @var int N�mero total de registros do datasource.
     */
    protected $totalRegistros = null;

    protected $numRegistrosPorPagina = self::TAMANHO_PADRAO_PAGINA;

    /**
     * @var int N�mero da p�gina que dever� ser exibida.
     */
    protected $paginaAtual = 1;

    /**
     * @var int N�mero total de p�ginas contidas no datasource.
     */
    protected $totalPaginas;

    /**
     * Retorna a fonte de dados completa.
     *
     * @return string
     */
    public function getSource()
    {
        return $this->source;
    }

    /**
     * Define a p�gina atualmente selecionada pelo usu�rio.
     *
     * @param int|string $pagina O n�mero da p�gina atual, ou 'all'.
     * @return \Simec_Listagem_Datasource
     */
    public function setPaginaAtual($pagina)
    {
        if (empty($pagina)) {
            $pagina = 1;
        }

        $this->paginaAtual = $pagina;
        return $this;
    }

    /**
     * Retorna a p�gina atualmente selecionada pelo usu�rio.
     *
     * @return int|string
     */
    public function getPaginaAtual()
    {
        return $this->paginaAtual;
    }

    /**
     * Computa, se necess�rio, e retorna a quantidade de registros da fonte de dados.
     * @return int
     */
    public function getTotalRegistros()
    {
        if (is_null($this->totalRegistros)) {
            $this->totalRegistros = $this->contaRegistros();
        }

        return $this->totalRegistros;
    }

    /**
     * Computa, se necess�rio, e retorna a quantidade de p�ginas da fonte de dados.
     * @return int
     */
    public function getTotalPaginas()
    {
        if (is_null($this->totalPaginas)) {
            $this->totalPaginas = ceil($this->getTotalRegistros() / $this->numRegistrosPorPagina);
        }

        return $this->totalPaginas;
    }

    /**
     * Verifica se a fonte de dados tem algum registro.
     *
     * @return bool
     */
    public function estaVazio()
    {
        return 0 === (int)$this->getTotalRegistros();
    }

    /**
     * Verifica se existe mais de uma p�gina de dados.
     *
     * @return bool
     */
    public function paginar()
    {
        return $this->getTotalRegistros() > $this->getRegistrosPorPagina();
    }

    /**
     * Calcula e retorna o offset da p�gina selecionada.
     *
     * @return int
     */
    protected function offset()
    {
        return ($this->paginaAtual - 1) * $this->getRegistrosPorPagina();
    }

    /**
     * Retorna a quantidade de registros exibidos por p�gina.
     * @return int
     */
    protected function getRegistrosPorPagina()
    {
        return $this->numRegistrosPorPagina;
    }

    /**
     * Define uma nova quantidade de registros por p�gina.
     *
     * @param int $numRegistrosPorPagina Quantidade de registros por p�gina.
     */
    public function setRegistrosPorPagina($numRegistrosPorPagina)
    {
        $this->numRegistrosPorPagina = $numRegistrosPorPagina;
    }

    /**
     * Implemente a defini��o de uma fonte de dados e suas op��es.
     *
     * @return \Simec_Listagem_Datasource
     */
    public abstract function setSource($source, array $opcoes = array());

    /**
     * Implementa a forma de retornar a query que originou o conjunto de dados.
     *
     * @returns string
     */
    public abstract function getQuery();

    /**
     * Esta � a fun��o de retorno de dados da p�gina atualmente selecionada.
     * Com base no offset e n�mero de registros a serem retornados, a implmenta��o esta fun��o deve
     * retornar apenas o conjunto de dados a serem exibidos na lista atual.
     *
     * @return mixed[] Lista de dados para exibi��o
     */
    public abstract function getDados();

    /**
     * Descobre a quantidade total de registros.
     *
     * @return int
     */
    protected abstract function contaRegistros();
}
