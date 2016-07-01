<?php
/**
 * Implementa uma vers�o simplificada do zend flash message.
 * $Id: FlashMessage.php 97955 2015-05-28 18:07:18Z maykelbraz $
 */

/**
 * Utilize para armazenar mensagens de aviso, sucesso, informa��es
 * ou erros e exib�-las posteriamente. Assim que uma mensagem �
 * exibida, ela � removida do armazenamento.
 * Depois de instanciar a classe definindo o identificador de onde
 * sua mensagem ser� armazenada, basta chamar addMensagem para
 * adicionar uma nova mensagem. Quando for exibir as mensagens,
 * basta chamar getMensagens, que elas ser�o retornadas j� formatadas.
 *
 * Exemplo de adi��o de mensagem:
 * $fm = new Simec_Helper_FlashMessage('elabrev/tc/formnc');
 * $fm->addMensagem(
 *      'N�o foi poss�vel processar sua requisi��o.',
 *      Simec_Helper_FlashMessage::ERRO
 * );
 *
 * Exemplo de exibi��o de mensagem:
 * $fm = new Simec_Helper_FlashMessage('elabrev/tc/formnc');
 * echo $fm->getMensagens();
 *
 */
class Simec_Helper_FlashMessage
{
    /**
     * Use para criar uma mensagem de sucesso. Bal�o verde.
     * Este � o tipo de mensagem padr�o.
     */
    const SUCESSO = 'success';
    /**
     * Use para criar uma mensagem de informa��o. Bal�o azul.
     */
    const INFO = 'info';
    /**
     * Use para criar uma mensagem de aviso. Bal�o amarelo.
     */
    const AVISO = 'warning';
    /**
     * Use para criar uma mensagem de erro. Bal�o vermelho.
     */
    const ERRO = 'danger';

    /**
     * Identificador de armazenamento das mensagens na sess�o.
     * Ser� cria na vari�vel de sess�o uma entrada do tipo:
     * $_SESSION[$identificador]['msg'], onde as mensagens
     * ser�o armazenadas.
     * @var string
     */
    protected $identificador;

    /**
     * Cria uma inst�ncia de gerenciamento de mensagens com
     * um identificador pr�prio. � recomendado a utiliza��o
     * de identificadores compostos para evitar conflitos
     * com outras �reas da sess�o.
     * Exemplo: "elabrev/termocooperacao".
     *
     * @param string $identificador
     *          Identificador de armazenamento das mensagens.
     */
    public function __construct($identificador)
    {
        if (empty($identificador)) {
            trigger_error('O identificador n�o pode ser vazio.');
        }
        $this->identificador = $identificador;

        if (!isset($_SESSION[$this->identificador]['msg'])) {
            $_SESSION[$this->identificador]['msg'];
        }
    }

    /**
     * Adiciona uma nova mensagem � lista de mensagens do identificador.
     * Mais de uma mensagem pode ser adicionada.
     * @param string $mensagem O texto a mensagem a ser inclu�da na lista.
     * @param string $tipo O Tipo da mensagem a ser inclu�da.
     * @see Simec_Helper_FlashMessage::SUCESSO
     * @see Simec_Helper_FlashMessage::INFO
     * @see Simec_Helper_FlashMessage::AVISO
     * @see Simec_Helper_FlashMessage::ERRO
     */
    public function addMensagem($mensagem, $tipo = self::SUCESSO)
    {
        if (empty($mensagem)) {
            trigger_error('A mensagem n�o pode ser vazia.');
        }
        $_SESSION[$this->identificador]['msg'][] = array(
            'tipo' => $tipo,
            'texto' => $mensagem
        );
    }

    /**
     * Retorna um HTML com todas as mensagens armazenadas e
     * as remove da lista do identificador.
     * @return string
     */
    public function getMensagens()
    {

        $msgs = $_SESSION[$this->identificador];

        if (!isset($msgs['msg'])
            || empty($msgs['msg'])
            || !is_array($msgs['msg'])) {
            return;
        }

        // -- imprimindo mensagens
        $html = '';
        foreach ($msgs['msg'] as $msg) {
            $html .= <<<DML
<div class="alert alert-{$msg['tipo']} text-center col-md-8 col-md-offset-2">
    <button type="button" class="close" data-dismiss="alert" aria-hidden="true">&times;</button>
{$msg['texto']}
</div>
<br style="clear:both" />
DML;
        }

        // -- Limpando o identificador
        unset($_SESSION[$this->identificador]);
        return $html;
    }

    public function __toString()
    {
        $retorno = $this->getMensagens();
        return $retorno?$retorno:'';
    }
}
