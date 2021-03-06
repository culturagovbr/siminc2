<?php
/**
 * Arquivo de implementa��o do datasource da listagem do tipo Array.
 *
 * $Id: Array.php 102684 2015-09-22 13:29:15Z maykelbraz $
 * @filesource
 * @package Simec\Listagem\Datasource
 */

/**
 * Implementa uma fonte de dados do tipo array.
 *
 * Da mesma forma que antes da sua implementa��o, pode ser fornecido
 * um array com as linhas da lista, no entanto, agora � poss�vel passar
 * um array de configura��o, que permite, inclusive, emular a pagina��o.
 *
 * @package Simec\Listagem\Datasource
 * @see Simec\Listagem
 * @see Simec_Listagem_Datasource_Array::setSource()
 */
class Simec_Listagem_Datasource_Array extends Simec_Listagem_Datasource
{
    /**
     * @var string Query que originou o array de dados.
     */
    protected $query;

    /**
     * @var bool Indica se o $source deve ser cortado, ou se ele tem apenas a qtd necess�ria de dados.
     */
    protected $slice = true;

    /**
     * Carrega a fonte de dados.
     *
     * O array $source pode ser tanto uma lista simples com as linhas do relat�rio, quanto o seguinte
     * conjunto de configura��es:
     * dados: Lista de dados;
     * query: query que originou os dados - utilizado externamento;
     * registros: quantidade total de registros (ao informar, permite simular uma pagina��o);
     * pagina: a pagina solicitada pelo usu�rio - utilizado externamente.
     *
     * @param array $source Array de configura��o ou de dados da listagem.
     * @param array $opcoes Op��es extras de configura��o do source.
     * @return \Simec_Listagem_Datasource_Array
     */
    public function setSource($source, array $opcoes = array())
    {
        if (!is_array($source)) {
            $this->source = array();
        } elseif (array_key_exists('dados', $source)) {
            $this->source = $source['dados']?$source['dados']:array();

            if (array_key_exists('query', $source)) {
                $this->query = $source['query'];
            }

            if (array_key_exists('registros', $source)) {
                $this->totalRegistros = $source['registros'];
                // -- Apenas uma p�gina de registros foi retornada
                $this->slice = false;
            }

            if (array_key_exists('pagina', $source)) {
                $this->paginaAtual = $source['pagina'];
            }
        } else {
            $this->source = $source;
        }
        return $this;
    }

    /**
     * Retorna o conjunto de registros da p�gina (ou todos os registros).
     *
     * @return array
     */
    public function getDados()
    {
        if (('all' == $this->getPaginaAtual()) || (!$this->slice)) {
            return $this->source;
        }

        return array_slice($this->source, $this->offset(), $this->getRegistrosPorPagina());
    }

    /**
     * Retorna a query que originou as linhas do array ($source).
     *
     * @return string
     */
    public function getQuery()
    {
        if (empty($this->query)) {
            return 'QUERY N�O DISPON�VEL PARA ESTE CONJUNTO DE DADOS.';
        }

        return $this->query;
    }

    /**
     * Realiza a contagem dos registros atuais.
     *
     * @return int
     * @see Simec_Listagem_Datasource::getTotalRegistros()
     */
    protected function contaRegistros()
    {
        return count($this->source);
    }
}
