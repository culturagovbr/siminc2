<?php
/**
 *
 */

/**
 *
 */
require_once(dirname(__FILE__) . "/../ArrayIterator.php");

/**
 *
 */
class Simec_SoapClient_Options extends Simec_ArrayIterator
{
    /**
     * Adiciona uma op��o do soapcliente � lista de op��es utilizadas na requisi��o.
     *
     * @param string $key Nome da op��o do soapcliente.
     * @param mixed $elem Valor para a op��o.
     * @return \Simec_SoapClient_Options
     */
    public function add($key, $elem = null)
    {
        $this->elements[$key] = $elem;
        return $this;
    }
}
