<?php
/**
 * $Id: Workflow.php 97549 2015-05-19 21:13:18Z maykelbraz $
 */

/**
 * A��o de exibi��o da barra do workflow.
 * O nome da fun��o, ao inv�s de ser utilizado como callback javascript,
 * � utilizado como o par�metro 'requisicao' e enviado ao servidor, para
 * executar a cria��o da barra de workflow.
 *
 * Exemplo:
 * <pre>
 * array(2) {
 *   ["requisicao"]=>
 *     string(12) "drawWorkflow"
 *   ["params"]=>
 *     array(2) {
 *       [0]=>
 *         string(5) "70549"
 *       [1]=>
 *         string(8) "30430693"
 *   }
 * }
 * </pre>
 */
class Simec_Listagem_Acao_Workflow extends Simec_Listagem_Acao
{
    protected $icone = 'transfer';
    protected $titulo = 'Situa��o';
    protected $callbackJS = 'showWorkflow';

    protected function renderAcao()
    {
        // -- A��o avan�ada
        $acao = <<<HTML
<a href="#" class="workflow" data-action="%s" data-params="%s">%s</a>
HTML;
        return sprintf(
            $acao,
            $this->callbackJS,
            $this->getCallbackParams(true),
            $this->renderGlyph()
        );
    }
}
