<?php

/**
 * Classe para gerar visualização padrão de relatórios dinâmicos
 *
 * @example
 * <code>
 *   $tabela = new relatorio_agrupador_tabela();
 *   echo $tabela->setListaTodasColunas($listaTodasColunas)
 *       ->setListaColunasSelecionadas($listaColunasSelecionadas)
 *       ->setListaColunaFormatoMoeda($listaColunaFormatoMoeda)
 *       ->setListaRelatorio($listaRelatorio)
 *       ->montarTabela()
 *       ->getTabela();
 * </code>
 *
 */
class relatorio_agrupador_tabela {
    
    protected $listaTodasColunas = array();

    protected $listaColunasSelecionadas = array();
    
    protected $listaColunaFormatoMoeda = array();

    protected $listaColunaTotalizador = array();

    protected $listaRelatorio = array();

    protected $tabela = '';

    public function getListaTodasColunas() {
        return $this->listaTodasColunas;
    }

    public function getListaColunasSelecionadas() {
        return $this->listaColunasSelecionadas;
    }

    public function getListaColunaFormatoMoeda() {
        return $this->listaColunaFormatoMoeda;
    }

    public function getListaRelatorio() {
        return $this->listaRelatorio;
    }

    public function getTabela() {
        return $this->tabela;
    }

    public function setListaTodasColunas($listaTodasColunas) {
        $this->listaTodasColunas = $listaTodasColunas;
        return $this;
    }

    public function setListaColunasSelecionadas($listaColunasSelecionadas) {
        $this->listaColunasSelecionadas = $listaColunasSelecionadas;
        return $this;
    }

    public function setListaColunaFormatoMoeda($listaColunaFormatoMoeda) {
        $this->listaColunaFormatoMoeda = $listaColunaFormatoMoeda;
        return $this;
    }

    public function setListaRelatorio($listaRelatorio) {
        $this->listaRelatorio = $listaRelatorio;
        return $this;
    }

    public function setTabela($tabela) {
        $this->tabela = $tabela;
        return $this;
    }
    
    /**
     * Objeto para montar e renderizar a tabela de relatório agrupador
     * 
     * @param array $listaTodasColunas
     * @param array $listaColunasSelecionadas
     * @param array $listaColunaFormatoMoeda
     * @param array $listaRelatorio
     * @param string $tabela
     */
    public function __construct($listaTodasColunas = NULL, $listaColunasSelecionadas = NULL, $listaColunaFormatoMoeda = NULL, $listaRelatorio = NULL, $tabela = NULL) {
        $this->listaTodasColunas = $listaTodasColunas? $listaTodasColunas: array();
        $this->listaColunasSelecionadas = $listaColunasSelecionadas? $listaColunasSelecionadas: NULL;
        $this->listaColunaFormatoMoeda = $listaColunaFormatoMoeda? $listaColunaFormatoMoeda: NULL;
        $this->listaRelatorio = $listaRelatorio? $listaRelatorio: NULL;
        $this->tabela = $tabela? $tabela: '';
    }

    /**
     * Busca a descricao da coluna pelo nome interno/Código
     * 
     * @param string $codigo
     * @param array $listaColunas
     * @return string
     */
    public static function buscarDescricaoPorCodigo($codigo, $listaColunas){
        $resultado = '';
        if($listaColunas){
            foreach($listaColunas as $coluna){
                if($codigo == $coluna['codigo']){
                    $resultado = $coluna['descricao'];
                    break;
                }
            }
        }

        return $resultado;
    }

    /**
     * Monta o código HTML do cabeçalho da tabela
     * 
     * @return relatorio_agrupador_lista
     */
    public function montarTabelaCabecalho(){
        $this->tabela .= '
            <thead>
                <tr>
        ';
        if ($this->listaColunaTotalizador!=null) {
            foreach ($this->listaColunasSelecionadas as $colunaSelecionada) {
                if (array_key_exists($colunaSelecionada, $this->listaColunaTotalizador)) {
                    $this->tabela .= '
                            <th>
                                ' . formata_valor($this->listaColunaTotalizador[$colunaSelecionada]) . '
                            </th>
                    ';
                } else {
                    $this->tabela .= '
                            <th>

                            </th>
                    ';
                }
            }
            $this->tabela .= '
                    </tr>
            ';
        }
        $this->tabela .= '
                <tr>
        ';
        foreach($this->listaColunasSelecionadas as $colunaSelecionada){
            $this->tabela .= '
                <th>'. self::buscarDescricaoPorCodigo($colunaSelecionada, $this->listaTodasColunas). '</th>
            ';
        }

        $this->tabela .= '
                </tr>
            </thead>
        ';

        return $this;
    }

    /**
     * Monta o código HTML do corpo da tabela onde ficam os registros da consulta
     * 
     * @return relatorio_agrupador_lista
     */
    public function montarTabelaCorpo(){
        if($this->listaRelatorio){
            $this->tabela .= '
                <tbody>
            ';
                foreach($this->listaRelatorio as $itemRelatorio){
                    $this->tabela .= '
                        <tr>
                    ';

                    # Monta as linhas do corpo da tabela de acordo com o resultado da pesquisa
                    $this->montarTabelaCorpoLinhas($itemRelatorio);

                    $this->tabela .= '
                        </tr>
                    ';
                }
            $this->tabela .= '
                </tbody>
            ';
        }

        return $this;
    }

    /**
     * Monta as linhas do corpo da tabela de acordo com o resultado da pesquisa
     * 
     * @param array $itemRelatorio
     * @return $this
     */
    public function montarTabelaCorpoLinhas($itemRelatorio){
        if($this->listaColunasSelecionadas){
            foreach($this->listaColunasSelecionadas as $colunaSelecionada){
                $this->tabela .= '
                    <td>
                        '. (in_array($colunaSelecionada, $this->listaColunaFormatoMoeda)? formata_valor($itemRelatorio[$colunaSelecionada]): $itemRelatorio[$colunaSelecionada]). '
                    </td>
                ';
            }
        }

        return $this;
    }

    /**
     * Totaliza colunas no formato moeda
     *
     * @param array $itemRelatorio
     * @return $this
     */
    public function montarTabelaTotalizador(){
        if($this->listaColunaFormatoMoeda && $this->listaRelatorio!=''){
            $arrRetorno = array();
            foreach($this->listaColunaFormatoMoeda as $colunaSelecionada){
                foreach($this->listaRelatorio as $itemRelatorio) {
                    $arrRetorno[$colunaSelecionada] += $itemRelatorio[$colunaSelecionada];
                }
            }
        }
        $this->listaColunaTotalizador = $arrRetorno;
    }
    
    /**
     * Monta o código HTML do rodapé da tabela
     * 
     * @return relatorio_agrupador_lista
     */
    public function montarTabelaRodape(){
        if(!$this->listaRelatorio){
            $this->tabela .= '
                <tfoot>
                    <tr class="text-center validateRedText">
                        <td colspan="'. count($this->listaColunasSelecionadas). '">
                            Não existem dados para serem mostrados
                        </td>
                    </tr>
                </tfoot>
            ';
        }
        
        return $this;
    }
    
    /**
     * Monta o código HTML da tabela
     * 
     * @return relatorio_agrupador_lista
     */
    public function montarTabela(){
        $this->tabela .= '
            <table class="table table-bordered table-hover table-striped">
        ';

        $this->montarTabelaTotalizador();
        $this->montarTabelaCabecalho()
             ->montarTabelaCorpo()
             ->montarTabelaRodape()
        ;
        
        $this->tabela .= '
            </table>
        ';
        
        return $this;
    }

}
