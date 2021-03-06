<?php
/**
 * Implementa��o do renderizador da a��o Select, utilizada na listagem.
 *
 * @version $Id: Select.php 98338 2015-06-08 20:47:48Z maykelbraz $
 * @see Simec_Listagem
 */

/**
 * Esta a��o se parece com um checkbox, que pode ser marcado e desmarcado. Sempre que acontece a<br />
 * transi��o de estado, uma callback registrada no momento da cria��o da a��o � chamada. � poss�vel<br />
 * definir o estado inicial de todos os itens da lista, al�m de definir uma condi��o para marca��o, ou n�o.<br />
 * Exemplo 1: Uso b�sico, os itens aparecem marcados.<br />
 * <pre>
 * $list = new Simec_Listagem(...);
 * ...
 * $list->addAcao('select', 'selecionarLinha');
 * ...
 * </pre>
 * Exemplo 2: Todos os itens aparecem desmarcados.<br />
 * <pre>
 * ...
 * $config = array();
 * $config['func'] = 'selecionarLinha';
 * $config['desmarcado'] = true;
 * $list->addAcao('select', $config);
 * ...
 * </pre>
 * Exemplo 3: Os itens s�o marcados condicionalmente.<br />
 * <pre>
 * ...
 * $config = array();
 * $config['func'] = 'selecionarLinha';
 * $config['verifica'] = array('campo' => 'diferenca', 'valor' => 0.00, 'op' => 'igual');
 * $list->addAcao('select', $config);
 * ...
 * </pre>
 * Para maiores detalhes da callback executada, veja o arquivo listagem.js e a fun��o delegateAcaoSelect().
 * @see listagem.js
 */
class Simec_Listagem_Acao_Select extends Simec_Listagem_AcaoComID
{
    protected $icone = 'ok';
    protected $cor = 'green';
    protected $titulo = 'Selecionar item';

    protected $icone2 = 'remove';
    protected $cor2 = 'gray';
    protected $titulo2 = 'Remover item';

    protected function renderAcao()
    {
        // -- Status inicial da a��o: marcado ou desmarcado.
        $marcado = true;
        if (isset($this->config['desmarcado']) || $this->config['desmarcado']) {
            $marcado = false;
        }

        // -- Verifica��o de status da a��o com base no valor de um campo
        if (isset($this->config['verifica'])) {
            $func = 'checa' . ucfirst($this->config['verifica']['op']?$this->config['verifica']['op']:'igual');
            $marcado = $this->$func(
                $this->dados[$this->config['verifica']['campo']],
                $this->config['verifica']['valor']
            );
        }

        $acao = <<<HTML
<span title="%s" class="glyphicon glyphicon-%s" style="cursor:pointer;color:%s" data-id="%s" data-cb="%s" data-exp="%s"></span>
HTML;
        return sprintf(
            $acao,
            $marcado?$this->titulo:$this->titulo2,
            $marcado?$this->icone:$this->icone2,
            $marcado?$this->cor:$this->cor2,
            $this->getAcaoID(),
            $this->callbackJS,
            str_replace('"', "'", simec_json_encode($this->paramsAsArray()))
        );
    }

    protected function paramsAsArray()
    {
        $parametros = array();
        foreach ($this->parametrosExternos as $param) {
            $parametros[$param] = $this->dados[$param];
        }
        foreach ($this->parametrosExtra as $param) {
            $parametros[$param] = $this->dados[$param];
        }
        return $parametros;
    }
}
