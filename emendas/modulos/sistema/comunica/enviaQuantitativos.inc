<?php
include_once APPRAIZ . 'includes/funcoesspo.php';
/**
 * Arquivo de métodos de envios de dados usando o webservice.
 * @see enviaQuantitativo.inc
 * $Id: enviaQuantitativos.inc 93020 2015-01-22 17:53:33Z werteralmeida $
 */
ini_set('memory_limit', '1048M');
set_time_limit(30000);

if ($_REQUEST['requisicao']) {
    $_REQUEST['requisicao']($_REQUEST);
    exit;
}

function montaTabelaRetornoAcompanhamento(array $arrResponse = array(), $funcao = '')
{
	global $db;
    $html = <<<HTML
<table class="table table-striped table-hover">
    <thead>
        <tr>
            <th width="10%">Programática</th>
            <th width="10%">Detalhes</th>
            <th width="30%">Descrição</th>
            <th width="50%">Mensagem Retorno</th>
        </tr>
    </thead>
HTML;

	$sql = <<<DML
SELECT COALESCE(max(l.loglote), 0)
  FROM elabrev.logerrows l
  WHERE l.logtipo = 'cadastrarAcompanhamentoOrcamentario'
DML;
	$lote = $db->pegaUm($sql);
	$lote = $lote + 1;

	if ($_SESSION['elabrev']['logidarr'][0]) {
        $logids = implode(', ', $_SESSION['elabrev']['logidarr']);

		$sql = <<<DML
UPDATE elabrev.logerrows
  SET loglote = {$lote}
  WHERE logid IN ({$logids})
DML;

	  	$db->executar($sql);
	  	$db->commit();
	}

	foreach ($arrResponse as $key => $retorno) {

        $sql = <<<DML
SELECT a.codigo AS codigo,
       a.codigo || ' - ' || dae.titulo AS descricao,
       org.codigo AS unicod
  FROM planacomorc.acao a
    INNER JOIN planacomorc.acao_programatica apr ON apr.id_acao = a.id_acao
    INNER JOIN planacomorc.orgao org USING(id_orgao)
    INNER JOIN planacomorc.dados_acao_exercicio dae ON dae.id_acao = a.id_acao
      AND dae.id_exercicio = apr.id_exercicio
    LEFT JOIN planacomorc.programa prg USING(id_programa)
    LEFT JOIN planacomorc.esfera esf USING(id_esfera)
    LEFT JOIN planacomorc.subfuncao sfu USING(id_subfuncao)
  WHERE a.codigo = '{$retorno['acacod']}'
    AND org.codigo = '{$retorno['unicod']}'
DML;
		$arrAcao = $db->pegaLinha( $sql );

		$mensagem = '';
		if (!$retorno['retorno']->return->sucesso){
			if( is_array($retorno['retorno']->return->mensagensErro) ){
				$strMensagem = implode('<br>', $retorno['retorno']->return->mensagensErro);
			} else {
				$strMensagem = $retorno['retorno']->return->mensagensErro;
			}
			$mensagem = '<strong>Detalhe do erro:</strong> <br><br>'.$strMensagem;
			$corTD = 'red';
		} else {
			if ($funcao == 'cadastrarAcompanhamento') {
                $strMensagem = $mensagem = '';
                if (is_array($retorno['retorno']->return->alertas)){
                        $strMensagem .= implode('<br>', $retorno['retorno']->return->alertas);
                        $mensagem .= '<strong>Acompanhamento cadastrado com sucesso com ALERTA:</strong> <br><br>'.$strMensagem;
                } elseif (is_object($retorno['retorno']->return->alertas) || is_object($retorno['retorno']->return->pendencias)) {
                    if (is_array($retorno['retorno']->return->alertas->alerta)) {
                        $strMensagem .= implode('<br />', $retorno['retorno']->return->alertas->alerta);
                    } elseif ($retorno['retorno']->return->alertas->alerta) {
                        $strMensagem .= $retorno['retorno']->return->alertas->alerta;
                    }
                    if ($strMensagem) {
                        $mensagem .= '<strong>Acompanhamento cadastrado com sucesso com ALERTA:</strong> <br><br>'.$strMensagem . '<br /><br/>';
                        $strMensagem = '';
                    }
                    if (is_array($retorno['retorno']->return->pendencias->pendencia)) {
                        $strMensagem .= implode('<br />', $retorno['retorno']->return->pendencias->pendencia);
                    } elseif($retorno['retorno']->return->pendencias->pendencia) {
                        $strMensagem .= $retorno['retorno']->return->pendencias->pendencia;
                    }
                    if ($strMensagem) {
                        $mensagem .= '<strong>Acompanhamento cadastrado com sucesso com PENDÊNCIAS:</strong> <br><br>'.$strMensagem;
                    }
                } else {
                        $strMensagem .= $retorno['retorno']->return->mensagensErro;
                        if( $strMensagem ){
                                $mensagem .= '<strong>Acompanhamento cadastrado com sucesso com ALERTA:</strong> <br><br>'.$strMensagem;
                        } else {
                                $mensagem .= 'Acompanhamento cadastrado com sucesso!<br>';
                        }
                }
                include_once APPRAIZ . "includes/workflow.php";

                $sql = <<<DML
SELECT acc.docid
  FROM planacomorc.acompanhamento_acao acc
    INNER JOIN planacomorc.acao_programatica apr USING(id_acao_programatica)
    INNER JOIN planacomorc.localizador_programatica lpr USING(id_acao_programatica, id_localizador)
    INNER JOIN planacomorc.quantitativo_sof qsf USING(id_acao_programatica, loccod)
    INNER JOIN workflow.documento wfd USING(docid)
  WHERE id_acao_programatica = {$retorno['acaid']}
    AND esdid = 753
    AND qsf.quantidade_fisico != 0
DML;

                foreach ($db->carregar($sql) as $programatica) {
                    $aedid = 1767;
                    $dados = array();
                    $result = wf_alterarEstado(
                        $programatica['docid'],
                        $aedid,
                        $cmddsc = 'Tramitação feita em lote - SIOP',
                        $dados
                    );
                }
			}
			$corTD = 'blue';
		}
        //$key % 2 ? $cor = "#dedfde" : $cor = "";
        $html.= <<<HTML
    <tr id="tr_{$key}">
        <td>{$arrAcao['unicod']}.{$retorno['prgcod']}.{$arrAcao['codigo']}</td>
        <td>
            Função: {$retorno['funcao']}<br />
            Subfunção: {$retorno['subfuncao']}<br />
            Esfera: {$retorno['esfera']}<br />
            Tipo inclusão: {$retorno['tipoinclusao']}<br />
        </td>
        <td>{$arrAcao['descricao']}</td>
        <td style="color:$corTD">{$mensagem}</td>
    </tr>
HTML;
	}
    $totRegistros = sizeof($arrResponse);
	$html.= <<<HTML
    <tr>
        <td colspan="4">
            <table class="table table-striped table-hover">
                <tbody>
                    <tr>
                        <td>
                            <strong>Total de Registros: {$totRegistros}</strong>
                        </td>
                    </tr>
                </tbody>
            </table>
        </td>
    <tr>
</table>
HTML;
	return $html;
}

if (isset($_POST['codigomomento']) && count($_POST['acaid_cadastrarAcompanhamento'])) {

    /**
     * Conexão com o webservice quantitativo do SIOP.
     * @see SiopQuantitativo
     */
    require(APPRAIZ . 'planacomorc/classes/SiopQuantitativo.class.inc');

    $wsusuario = WEB_SERVICE_SIOP_USUARIO;
    $wssenha = WEB_SERVICE_SIOP_SENHA;

    // -- credenciais e dados
    $arrParam = array(
        'usuario' => $wsusuario,
        'senha' => $wssenha,
        'post' => $_POST
    );

    $obResponse = new SiopQuantitativo($arrParam);
    if (isset($_POST['acaid_cadastrarAcompanhamento'][0])) {
        $arrResponse = $obResponse->cadastrarAcompanhamento();
        $error = false;
        if ($arrResponse) {
            $html = montaTabelaRetornoAcompanhamento($arrResponse, 'cadastrarAcompanhamento');
        } else {
            $error = true;
            $html = 'erro';
        }
    }
}

// -- Cabeçalho padrão do sistema
include APPRAIZ . "includes/cabecalho.inc";

?>
<!-- incluindo scripts para formatação de interface -->
<link rel="stylesheet" type="text/css" href="/library/bootstrap-switch/stylesheets/bootstrap-switch.css">
<script src="/library/bootstrap-switch/js/bootstrap-switch.min.js" type="text/javascript"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script type="text/javascript">
$(function(){
    /**
     * Ação que dispara evento submit do formulário principal
     */
    jQuery(".btn-consulta-siop").on("click", function(){
        jQuery("#formSiop").submit();
    });
});
/**
 * Abre uma popup com uma busca ao log de envio daquela acaoprogramatica.
 * @param int idAcaoProgramatica ID da ação programática;
 */
function exibirLogEnvio(idAcaoProgramatica)
{
    $.post(window.location, {requisicao: 'exibirLogEnvio',id_acao_programatica: idAcaoProgramatica}, function(content){
        alert(content);
    });
}
</script>

<!-- breadCrumb da page -->
<div class="row">
    <div class="col-lg-12">
        <ol class="breadcrumb">
            <li><a href="/planacomorc/planacomorc.php?modulo=inicio&acao=C"><?= $_SESSION['sisdsc']; ?></a></li>
            <li class="">Ações <?= $_SESSION['exercicio']; ?></li>
            <li class="active">Enviar Acompanhamento para o SIOP em LOTE</li>
        </ol>
    </div>
</div>

<?php if (!empty($html) && $error) : ?>
    <!-- Se houver erro na chamada ao ws, aqui sera o output -->
    <div class="row">
        <div class="col-lg-2"></div>
        <div class="col-lg-8">
            <div id="erro" class="alert alert-danger" role="alert"><?php echo $html; ?></div>
        </div>
        <div class="col-lg-2"></div>
    </div>
<?php endif; ?>

<!-- formulário com grid de opções para envio ao ws -->
<div class="col-lg-12">
    <form class="form-horizontal" role="form" name="formSiop" id="formSiop" action="" method="post">
        <div class="row">
            <div class="form-group">
                <label class="control-label col-md-2" for="codigomomento">Código do momento</label>
                <div class="col-md-10">
                    <?php echo inputTexto('codigomomento', isset($_POST['codigomomento']) ? $_POST['codigomomento'] : '2000', 'codigomomento', '', false); ?>
                </div>
            </div>
            <?php if (!empty($html) && !$error) : ?>
             <!-- output para chamada de sucesso ao ws -->
                <div class="row">
                    <div class="col-lg-12">
                        <?php echo $html; ?>
                    </div>
                </div>
            <?php endif; ?>
            <div class="form-group">
                <div class="col-md-12">
                <?php
                $sql = <<<DML
                        SELECT DISTINCT
                            apr.id_acao_programatica AS codigo,
                            org.codigo || '.' || prg.codigo  || '.' || a.codigo || ' - ' || dae.titulo AS descricao,
                            ultimoretornosiop AS retorno_siop
                        FROM planacomorc.acao a
                            INNER JOIN planacomorc.acao_programatica apr ON apr.id_acao = a.id_acao
                            INNER JOIN planacomorc.orgao org USING(id_orgao)
                            INNER JOIN planacomorc.localizador_programatica lpr USING(id_acao_programatica)
                            INNER JOIN planacomorc.quantitativo_sof qsf USING(id_acao_programatica, loccod)
                            INNER JOIN planacomorc.dados_acao_exercicio dae ON
                                        dae.id_acao = a.id_acao AND dae.id_exercicio = apr.id_exercicio
                            INNER JOIN planacomorc.acompanhamento_acao acc USING(id_acao_programatica)
                            INNER JOIN workflow.documento wfd USING(docid)
                            LEFT JOIN planacomorc.programa prg USING(id_programa)
                            LEFT JOIN planacomorc.produto prd USING(id_produto)
                            LEFT JOIN planacomorc.unidade_medida u ON u.id_unidade_medida = dae.id_unidade_medida
                        WHERE
                            apr.id_exercicio = {$_SESSION['exercicio']}
                            AND qsf.quantidade_fisico != 0
                            AND esdid = 753
                            ORDER BY 2
DML;

                require_once(APPRAIZ . 'includes/library/simec/Listagem.php');

                $colunms = array(
                    'Ações',
                    'Descrição',
                    'Situação'
                );

                $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
                $listagem->addCallbackDeCampo(array('descricao' ), 'alinhaParaEsquerda');
                $listagem->setCabecalho($colunms)
                     ->addCallbackDeCampo('codigo', 'formatarComoCheckbox')
                     ->setQuery($sql);
                $listagem->addCallbackDeCampo('retorno_siop', 'statusNoSIOP');
                $listagem->turnOnPesquisator();
                $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
                $listagem->render();
                
                    function statusNoSIOP($status, $dados, $id) {
        $html = <<<HTML
<span class="glyphicon glyphicon-%s" style="color:%s" %s></span>
HTML;
                    switch ($status) {
                        case 'E':
                            return sprintf($html, 'thumbs-down', '#D9534F;cursor:pointer', ' onclick="exibirLogEnvio(' . $id . ')"');
                        case 'S':
                            return sprintf($html, 'thumbs-up', '#5CB85C', ' onclick="exibirLogEnvio(' . $id . ')"');
                        default:
                            return sprintf($html, 'minus', '#F0AD4E', '');
                    }
                }
                ?>
                </div>
            </div>
            <div class="form-group">
                <div class="col-md-12">
                    <button type="button" class="btn btn-success btn-consulta-siop">Enviar Solicitação</button>
                </div>
            </div>
        </div>
    </form>
</div>

<?php
/**
 * Função de callback que auxilia o componente listagem simec
 * a apresentar um checkbox personalizado
 * @param $codigo
 * @param $dados
 * @return string
 */
function formatarComoCheckbox($codigo, $dados) {
        return <<<HTML
<div class="make-switch switch-mini" data-on-label="S" data-off-label="N" data-off="danger">
    <input type="checkbox" name="acaid_cadastrarAcompanhamento[]" value="{$codigo}" />
</div>
HTML;
}

?>