<?php
/**
 * Sistema SCRUM
 * @package simec
 * @subpackage scrum
 */

/**
 * Processa as requisi��es ($_POST) realizadas para a p�gina e, ap�s validar o tipo
 * da requisi��o, a encaminha para processamento.
 * Quando � feita uma requisi��o do tipo carregar, a fun��o retorna um array com os dados
 * solicitados, nos demais casos ela � finalizada e ocorre um redirecionamento para a p�gina
 * que originou a requisi��o.
 * @param string $posfixo
 *      Classifica��o da requisi��o: programa|subprograma|estoria|entregavel
 * @return null|array
 */
function processaRequisicao($posfixo)
{
    /**
     * Requerindo fun��es adicionais com base na requisi��o recebida.
     */
    require_once("funcoes{$posfixo}.php");
    
    $retorno = null;
    if (isset($_POST['action'])) {
        // -- Limpando parametros de buscas anteriores
        limpaURI();

        // -- Montando o nome da fun��o que ser� executada.
        $nomeFuncao = "{$_POST['action']}{$posfixo}";
        switch ($_POST['action']) {
            case 'carregar':
                // -- no-break
            case 'salvar':
                // -- Fun��o n�o implementada
                if (!is_callable($nomeFuncao)) {
                    $msgAlerta = 'Requisi��o n�o implementada.';
                    continue;
                }
                if ($retorno = $nomeFuncao($_POST)) {
                    $msgAlerta = 'Sua requisi��o foi executada com sucesso.';
                } else {
                    $msgAlerta = 'N�o foi poss�vel executar sua requisi��o.';
                }
                break;
            case 'json':
                $msg = '';
                header('Content-Type: application/json; charset=ISO-8859-1');
                // -- Fun��o n�o implementada
                if (!is_callable($nomeFuncao)) {
                    die(simec_json_encode(array('error' => 'Requisi��o n�o implementada.')));
                }
                $retorno = $nomeFuncao($_POST);
                die(simec_json_encode(array('error' => $msg, 'options' => $retorno)));
            case 'jsonResponsavelTempoExecucao':
                header('Content-Type: application/json; charset=ISO-8859-1');
                $retorno = jsonResponsavelTempoExecucao($_POST);
                die(simec_json_encode($retorno));
                exit;
            case 'jsonUpdateEntregavel':
                header('Content-Type: application/json; charset=ISO-8859-1');
                $retorno = jsonUpdateEntregavel($_POST);
                die(simec_json_encode($retorno));
                exit;
            case 'filtrar':
                // -- Fun��o n�o implementada
                if (!is_callable($nomeFuncao)) {
                    $msgAlerta = "Requisi��o n�o implementada ({$nomeFuncao}).";
                    continue;
                }
                $msgAlerta = false;
                $queryParams = $nomeFuncao($_POST);
                break;
            case 'voltar':
                $msgAlerta = false;
                break;
            default:
                $msgAlerta = 'Sua requisi��o � inv�lida.';
        }
        if (('carregar' != $_POST['action'])) {
    ?>
<script type="text/javascript" language="javascript">
<?php if ($msgAlerta): ?>
alert('<?php echo $msgAlerta; ?>');
<?php endif;?>
window.location = '<?php echo $_SERVER['REQUEST_URI'] . $queryParams; ?>';
</script>
    <?php
            exit();
        }
    }
    return $retorno;
}

/**
 * Remove parametros de busca da URI base do sistema.
 */
function limpaURI()
{
    $tmpURI = explode('&', $_SERVER['REQUEST_URI']);
    $_SERVER['REQUEST_URI'] = "{$tmpURI[0]}&{$tmpURI[1]}";
}

/**
 * Cria uma string de filtro para anexar � URI
 * @param array $campos
 *      Os campos que devem ser avaliados na listagem de dados.
 * @param array $dados
 *      Dados enviados pelo formul�rio.
 * @return string
 */
function criaFiltroURI($campos, $dados)
{
    $params = '';
    foreach ($campos as $campo) {
        if (!empty($dados[$campo])) {
            $params .= "&{$campo}={$dados[$campo]}";
        }
    }
    return $params;
}

function retornaSolicitante($cpf) {
    
    global $db;
    
    $sqlPartial = <<<DML
SELECT DISTINCT u.usucpf AS codigo, u.usunome AS descricao
  FROM seguranca.usuario AS u
    INNER JOIN demandas.usuarioresponsabilidade ur ON u.usucpf = ur.usucpf
    INNER JOIN seguranca.usuario_sistema us ON u.usucpf = us.usucpf
  WHERE us.sisid = 44
    AND us.suscod = 'A'
    AND ur.rpustatus = 'A'
    AND ur.pflcod in (238, 237)
    AND u.usucpf = '%s'
DML;
    
    $strSql = sprintf($sqlPartial, $cpf);
    $return = $db->carregar($strSql);
    return (object)$return[0];
}