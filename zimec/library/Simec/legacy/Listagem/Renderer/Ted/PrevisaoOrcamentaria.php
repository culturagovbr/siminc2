<?php
/**
 * Renderizador de listagens HTML Para Previsoes Orcamentarias do TED.
 *
 * @version $Id: Html.php 84802 2014-08-18 13:41:31Z maykelbraz $
 */

/**
 * Renderizador base.
 * @see Simec_Listagem_Renderer_Html
 */
require_once(dirname(__FILE__) . '/../Html.php');

class Simec_Listagem_Renderer_Ted_PrevisaoOrcamentaria extends Simec_Listagem_Renderer_Html
{
    /**
     * @var
     */
    protected $_subTotalPorLote;

    /**
     * @var
     */
    protected $_loteAnterior;

    /**
     * @var array
     */
    protected $_lotes = array();

    /**
     * inicializa��o do subtotal
     */
    const VALOR_PADRAO_SUBTOTAL = 0;

    /**
     *
     */
    const ULTIMA_POSICAO_TRUE = true;

    /**
     *
     */
    const ULTIMA_POSICAO_FALSE = false;

    /**
     * @param $lote
     * @return bool
     */
    protected function _loteEmCache($lote)
    {
        if (!empty($lote))
            return in_array($lote, $this->_lotes);

        return false;
    }

    /**
     * @param $lote
     */
    protected function _setLote($lote)
    {
        if (!empty($lote)) {
            $this->_loteAnterior = $lote;
        }
    }

    /**
     * @return mixed
     */
    protected function _getLote()
    {
        if (null !== $this->_loteAnterior) {
            return $this->_loteAnterior;
        }
    }

    /**
     * Adiciona o lote, nota de cr�dito
     * @param $lote
     */
    protected function _cacheLotes($lote)
    {
        if (!empty($lote))
            array_push($this->_lotes, $lote);
    }

    /**
     * Reseta o valor da variavel
     * @return void(0)
     */
    protected function _resetSubTotalLote()
    {
        $this->_subTotalPorLote = self::VALOR_PADRAO_SUBTOTAL;
    }

    /**
     * Soma o valor das celulas or�ament�rias
     * @param $valor
     */
    protected function _addSubTotal(array $dadosLinha)
    {
        if (null === $dadosLinha['lote'])
            return false;

        if ($dadosLinha['valor'] > self::VALOR_PADRAO_SUBTOTAL) {
            $this->_subTotalPorLote += $dadosLinha['valor'];
        }
    }

    /**
     * Retorna o subtotal de NC's somadas
     * @return mixed
     */
    protected function _getSubTotalLote()
    {
        return formata_valor($this->_subTotalPorLote);
    }

    /**
     * Imprime o c�digo HTML da listagem.
     */
    public function render()
    {
        // -- Inclu�ndo o arquivo de javascript de fun��es do relat�rio
        echo <<<HTML
<script type="text/javascript" lang="JavaScript">
$(document).ready(function(){
    $.getScript('/library/simec/js/listagem.js');
});
</script>
HTML;
        // -- T�tulo do relat�rio
        $this->renderTitulo();
        if ($this->renderizarForm) {
            echo '<form method="post" class="form-listagem">';
        }
        if ($this->formCampos) {
            foreach ($this->formCampos as $configCampo) {
                echo <<<HTML
<input type="{$configCampo['type']}" name="{$configCampo['name']}" id="{$configCampo['id']}" />
HTML;
            }
        }
        $qtdAcoes = count($this->acoes);
        echo <<<HTML
<table class="table table-striped table-bordered table-hover" id="tb_render" data-qtd-acoes="{$qtdAcoes}">
HTML;

        echo $this->renderCabecalho();

        if (class_exists('Zend_Application',false) ) {
            echo $this->renderFiltro();
        }
        $this->renderDados();
        echo $this->renderRodape();
        echo '</table>';
        if ($this->formImportar) {
            echo <<<HTML
<button type="submit" class="btn btn-primary" id="import-data">Importar</button>
HTML;
        }
        if ($this->renderizarForm) {
            echo '</form>';
        }
    }

    /**
     * Render para previs�es or�ament�rias, agrupado por NC
     */
    protected function renderDados()
    {
        // -- @todo Verificar se o tipo de listagem � de FORM, para incluir os inputs
        if (empty($this->dados)) {
            return;
        }
        echo <<<BODY
    <tbody>
BODY;
        foreach ($this->dados as $linha => $dadosLinha) {
            $classe = $this->getClasseLinha($dadosLinha);

            /**
             * Faz a soma das celulas or�ament�rias que utilizam mesma Nota de credito (NC)
             * E imprime na listagem, logo ap�s a declara��o das celulas usadas como base
             */
            $this->_agrupaRegistroNotaCredito($dadosLinha);

            echo <<<TR
        <tr{$classe}>
TR;
            echo $this->renderAcoes($dadosLinha, $linha);
            // -- @todo: Verificar o tipo da listagem do formulario
            if ($this->acoes) {
                $idLinha = array_shift($dadosLinha);
            } else {
                reset($dadosLinha);
                $idLinha = current($dadosLinha);
            }

            foreach ($dadosLinha as $key => $dado) {
                if (!in_array($key, $this->colunasOcultas)) {
                    // -- Chamando fun��o de callback registrada para o campo da listagem
                    if (array_key_exists($key, $this->callbacksDeCampo)) {
                        $dado = $this->callbacksDeCampo[$key]($dado, $dadosLinha, $idLinha);
                    }
                    echo <<<TD
            <td class="text-center">{$dado}</td>
TD;
                }
            }
            echo <<<TR
        </tr>
TR;

            $this->_addSubTotal($dadosLinha);
            if (count($this->dados) == ($linha+1)) {
                $this->_agrupaRegistroNotaCredito($dadosLinha, self::ULTIMA_POSICAO_TRUE);
            }
        }
        echo <<<BODY
    </tbody>
BODY;
    }

    /**
     * @param array $dadosLinha
     * @param bool $lastPosition
     */
    protected function _agrupaRegistroNotaCredito(array $dadosLinha, $lastPosition = self::ULTIMA_POSICAO_FALSE)
    {
        if (!$this->_loteEmCache($dadosLinha['lote']) || $lastPosition) {

            $numColunas = (count($this->cabecalho) + count($this->acoes));
            $colspan = ceil($numColunas/3);

            if ($this->_subTotalPorLote > self::VALOR_PADRAO_SUBTOTAL) {

                echo '<tr class="info">';
                echo '<td colspan="'.$colspan.'">&nbsp;</td>';
                echo '<td class="text-center" colspan="'.$colspan.'">
                            <span class="glyphicon glyphicon-search linkLote" codigo="'.$this->_getLote().'" alt="Visualizar lote" title="Visualizar lote"></span>
                            <span class="glyphicon-class" codigo="'.$this->_getLote().'">
                                <strong>Nota de cr�dito ('.($this->_getLote() ? $this->_getLote() : 'lote n�o encontrado').')</strong>
                            </span>
                     </td>';
                echo '<td colspan="'.$colspan.'" align="left" id="td_subtotalano_'.($this->_getLote() ? $this->_getLote() : '0000').'" style="font-weight:bold;">
                            R$ '.$this->_getSubTotalLote().'
                     </td>';
                echo '</tr>';
            }

            $this->_cacheLotes($dadosLinha['lote']);
            $this->_resetSubTotalLote();
            $this->_setLote($dadosLinha['lote']);
        }
    }

}