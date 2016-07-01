<?php
/**
 *
 */

/**
 *
 */
class Simec_Abas {

    protected $abas = array();
    protected $regras = array();
    protected $requestParam;
    protected $baseUrl;
    protected $abaDefault;
    protected $abaAtiva;
    protected $incluirCssAdicional;

    public function __construct($baseUrl, $requestParam = 'aba', $incluirCssAdicional = false)
    {
        if (empty($baseUrl)) {
            throw new Exception('A URL base das abas n�o pode ser deixada em branco.');
        }
        $this->requestParam = $requestParam;
        $this->baseUrl = "{$baseUrl}&{$this->requestParam}=";
        $this->incluirCssAdicional = $incluirCssAdicional;
    }

    public function getAbaAtiva()
    {
        if (!empty($this->abaAtiva)) {
            return $this->abaAtiva;
        }

        $this->setAbaAtiva();
        return $this->abaAtiva;
    }

    public function setAbaAtiva($abaAtiva = null)
    {
        if (!is_null($abaAtiva) && key_exists($abaAtiva, $this->abas)) {
            $this->abaAtiva = $abaAtiva;

            return $this;
        }
        if (is_null($abaAtiva) && !empty($_REQUEST[$this->requestParam])) {
            $this->abaAtiva = $_REQUEST[$this->requestParam];

            return $this;
        }

        $this->abaAtiva = $this->abaDefault;
    }

    public function adicionarAba($nomeAba, $titulo, $require, $glyphicon = null, array $params = array())
    {
        if (key_exists($nomeAba, $this->abas)) {
            throw new Exception("J� foi criada uma aba com o nome '{$nomeAba}'.");
        }

        if (!empty($require) && !is_file($require)) {
            throw new Exception("O arquivo '{$require}' n�o existe. O caminho est� correto?");
        }

        $this->abas[$nomeAba] = array(
            'titulo' => $titulo,
            'require' => $require,
            'icon' => $glyphicon,
            'params' => $params
        );

        return $this;
    }

    /**
     *
     * @param type $nomeAba
     * @param type $regra
     * @return \Simec_Abas
     * @todo Fazer uma classe de condi��o que pode ser utilizada pelas demais, afim de ficar tudo igual
     */
    public function adicionarCondicaoAba($nomeAba, $regra)
    {
        $this->regras[$nomeAba] = $regra;
//        if (is_string($regra) && is_callable($regra)) { // -- fun��o callback
//        } elseif (is_array($regra)) {
//        } elseif (is_callable($regra)) {
//        }
        //array(array('var' => 'valor', 'valor' => 0.00, 'op' => 'diferente'));
        // -- pode ser array, ou pode ser function
        return $this;
    }

    protected function verificarCondicao($nomeAba)
    {
        if (!isset($this->regras[$nomeAba])) {
            return true;
        }

        foreach ($this->regras[$nomeAba] as $regra) {
            $method = $regra['op'];
            if (!Simec_Operacoes::$method($regra['op1'], $regra['op2'])) {
                return false;
            }
        }

        return true;
    }

    public function definirAbaDefault($abaDefault)
    {
        if (!key_exists($abaDefault, $this->abas)) {
            throw new Exception("A aba '{$abaDefault}' n�o existe na atual lista de abas.");
        }

        $this->abaDefault = $abaDefault;

        return $this;
    }

    /**
     * Faz a renderiza��o das abas na tela, e j� faz o require do arquivo de conte�do da aba.
     * Importante: Quando a aba � inclusa pela fun��o render(), o escopo do arquivo n�o � o global
     * ent�o as vari�veis globais n�o podem ser acessadas no arquivo, a menos que ocorra a declara��o
     * de vari�veis globais (global $var). Para evitar isso, utilize render(true) e d� um require
     * no retorno da fun��o render(true);
     *
     * @param true $retornaCaminhoAba Indica que o arquivo de conte�do da aba n�o deve ser inclu�do e sim retornado (veja observa��es).
     * @return null|string
     */
    public function render($retornaCaminhoAba = false)
    {
        $this->setAbaAtiva();

        $listaAbas = array();
        $i = 0;

        foreach ($this->abas as $aba => $config) {

            if (!$this->verificarCondicao($aba)) {
                continue;
            }

            // -- criando o t�tulo da aba
            $titulo = '';
            if ($config['icon']) {
                $glyphicon = '<span class="glyphicon glyphicon-%s"></span> ';
                $titulo = sprintf($glyphicon, $config['icon']);
            }
            $titulo .= $config['titulo'];
            $listaAbas[] = array(
                'id' => ++$i,
                'descricao' => $titulo,
                'link' => "{$this->baseUrl}{$aba}" . $this->montaParametros()
            );
        }

        // -- Desenhando as abas
        echo montarAbasArray(
            $listaAbas,
            "{$this->baseUrl}{$this->abaAtiva}" . $this->montaParametros(),
            false,
            $this->incluirCssAdicional
        );

        // -- Fazendo o include da aba
        if ($this->abaAtiva) {
            if ($retornaCaminhoAba) {
                return $this->abas[$this->abaAtiva]['require'];
            }

            require $this->abas[$this->abaAtiva]['require'];
        }
    }

    protected function montaParametros()
    {
        $params = $this->abas[$this->abaAtiva]['params'];
        if (!empty($params)) {
            return '&' . http_build_query($params);
        }

        return '';
    }
}
