<?php
/**
 * Arquivo de renderiza��o da toolbar da listagem HTML.
 *
 * @version $Id: Toolbar.php 103888 2015-10-20 19:54:03Z maykelbraz $
 * @filesource
 */

/**
 * Classe respons�vel por renderizar a toolbar da listagem.
 *
 * @package Simec\View\Listagem\Renderer\Html
 * @author Maykel S. Braz <maykelbraz@mec.gov.br>
 */
class Simec_Listagem_Renderer_Html_Toolbar
{
    const PESQUISATOR = 1;
    /**
     * Adicionar um novo item.
     */
    const ADICIONAR = 2;
    /**
     * Inverter sele��o.
     */
    const INVERTER = 3;
    /**
     * Exibir a query que deu origem aos dados.
     */
    const QUERY = 4;
    /**
     * Separador de bot�es.
     */
    const SEPARADOR = 5;
    /**
     * Exporta o conte�do listado no formato XLS.
     */
    const EXPORTAR_XLS = 6;

    /**
     * @var int[] Lista de itens que ser�o exibidas na toolbar.
     */
    protected $itens = array();

    /**
     * @var string Armazena a query utilizada para gerar a listagem.
     */
    protected $query;


    /**
     * @var bool Indica se j� foi criada uma modal de exibi��o da query.
     */
    protected static $modalQueryJaCriada = false;

    /**
     * Adiciona funcionalidades � toolbar.
     *
     * @param int[]|int,... $item Item ou lista de itens a serem adicionados.
     * @return \Simec_Listagem_Renderer_Html_Toolbar
     * @todo Validar se o item � v�lido
     */
    public function add($item)
    {
        if (!is_array($item)) {
            $item = func_get_args ();
        }

        foreach ($item as $_item) {
            $this->itens[] = $_item;
        }

        return $this;
    }

    /**
     * Ativa a exibi��o da query na toolbar e define a query que ser� exibida.
     *
     * @param string $query
     * @return \Simec_Listagem_Renderer_Html_Toolbar
     */
    public function setQuery($query)
    {
        if (!in_array(self::QUERY, $this->itens)) {
            $this->itens[] = self::QUERY;
        }

        $this->query = $query;
        return $this;
    }

    public function render()
    {
        if (empty($this->itens)) {
            return '';
        }

        // -- Preparando a modal de exibi��o das queries
        $html = $this->renderModalQuery();

        $html .= <<<HTML
<nav class="navbar navbar-default navbar-listagem" role="navigation">
HTML;
        $itens = $this->itens;

        if (false !== array_search(self::PESQUISATOR, $itens)) {
            unset($itens[array_search(self::PESQUISATOR, $itens)]);
        }

        if (!empty($itens)) {
            $html .= <<<HTML
    <div class="navbar-form navbar-left btn-group">
HTML;

            foreach ($itens as $item) {
                switch ($item) {
                    case self::ADICIONAR:
                        $html .= $this->renderAdicionar();
                        break;
                    case self::INVERTER:
                        $html .= $this->renderInverter();
                        break;
                    case self::QUERY:
                        $html .= $this->renderQuery();
                        break;
                    case self::EXPORTAR_XLS:
                        $html .= $this->renderExportarXls();
                        break;
                }
            }
            $html .= <<<HTML
    </div>
HTML;
        }

        if (in_array(self::PESQUISATOR, $this->itens)) {
            $html .= $this->renderPesquisator();
        }

        $html .= <<<HTML
</nav>
HTML;
        return $html;
    }

    public function __toString()
    {
        return $this->render();
    }

    protected function renderPesquisator()
    {
        return <<<HTML
        <div class="navbar-form navbar-left" style="text-align:right;font-weight:bold">
            <span>Pesquisa<br />r�pida:</span>
        </div>
        <div class="navbar-form navbar-pesquisa">
            <div class="input-group">
                <div class="input-group-addon" data-toggle="popover" style="cursor:pointer">
                    <span class="glyphicon glyphicon-info-sign" style="color:#428bca"></span>
                </div>
                <input class="form-control busca-listagem" type="text" id="textFind"
                       placeholder="Digite o texto para busca" />
            </div>
        </div>
HTML;
    }

    protected function renderAdicionar()
    {
        return <<<HTML
        <button type="button" class="btn btn-default btn-adicionar" data-title='Ajuda - Adicionar'
                data-content="Clique para adicionar um novo<br />item � lista.">
            <span class="glyphicon glyphicon-plus"></span>
        </button>
HTML;
    }

    protected function renderInverter()
    {
        return <<<HTML
        <button type="button" class="btn btn btn-default btn-marcar" data-title='Ajuda - Inverter sele��o'
                data-content="Clique para selecionar todos<br />os elementos exibidos na lista.">
            <span class="glyphicon glyphicon-ok"></span>
        </button>
HTML;
    }

    protected function renderQuery()
    {
        $query = simec_htmlspecialchars($this->query);
        return <<<HTML
        <button type="button" class="btn btn btn-default btn-query" data-title='Ajuda - Query'
                data-content="Exibe a query utilizada na listagem." data-query="{$query}">
            <span class="glyphicon glyphicon-cog"></span>
        </button>
HTML;
    }

    /**
     * Faz a inclus�o do html da modal de exibi��o do script SQL da listagem.
     *
     * @return string
     * @uses Simec_Listagem_Renderer_Html_Toolbar::$modalQueryJaCriada
     */
    protected function renderModalQuery()
    {
        if (!in_array(self::QUERY, $this->itens)) {
            return '';
        }

        if (!self::$modalQueryJaCriada) {
            self::$modalQueryJaCriada = true;

            return <<<HTML
<div class="modal fade" id="listagem-query" style="z-index:99999">
  <div class="modal-dialog modal-lg">
    <div class="modal-content">
      <div class="modal-header">
        <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
        <h4 class="modal-title">Visualiza��o de query</h4>
      </div>
      <div class="modal-body">
      </div>
      <div class="modal-footer">
        <button type="button" class="btn btn-success" data-dismiss="modal">Ok</button>
      </div>
    </div>
  </div>
</div>
<script type="text/javascript">
$(function(){
    $(document).on('click', '.navbar-listagem .btn-query', function(){
        $('#listagem-query.modal .modal-body').empty().html(
            '<pre>' + $(this).attr('data-query') + '</pre>'
        );
        $('#listagem-query').modal();
    });
});
</script>
HTML;
        }

        return '';
    }

    protected function renderExportarXls()
    {
        return <<<HTML
        <button type="button" class="btn btn btn-default btn-xls" data-title='Ajuda - Exportar XLS'
                data-content="Exporta o conte�do do relat�rio para o formato XLS (Excel).">
            <span class="glyphicon glyphicon-download-alt"></span>
        </button>
HTML;
    }
}
