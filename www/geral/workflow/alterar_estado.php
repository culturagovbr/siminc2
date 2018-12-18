<?php

// inicializa sistema
require_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";

# Verifica se a sessão não expirou, se tiver expirada envia pra tela de login.
controlarAcessoSemAutenticacao();

include_once APPRAIZ . "includes/workflow.php";
require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

switch ( $_SESSION['sisdiretorio'] ){
    case 'altorc':
        include_once APPRAIZ. "www/altorc/_funcoesalteracaoorcamentaria.php";
    break;
        case 'recorc':
        include_once APPRAIZ. "www/recorc/_funcoes.php";
    break;
    case 'cap':
        include_once APPRAIZ. "www/cap/_constantes.php";
        include_once APPRAIZ. "www/cap/_funcoes.php";
    break;
    case 'escolasexterior':
        include_once APPRAIZ. "www/escolasexterior/_constantes.php";
        include_once APPRAIZ. "www/escolasexterior/_funcoes.php";
    break;
    case 'planacomorc':
        include_once APPRAIZ. "www/planacomorc/_constantes.php";
        include_once APPRAIZ. "www/planacomorc/_funcoes.php";
        include_once APPRAIZ. "www/planacomorc/_funcoespi.php";
        include_once APPRAIZ. "www/planacomorc/_funcoesacoes.php";
    break;
    case 'emendas':
        include_once APPRAIZ. "www/emendas/_constantes.php";
        include_once APPRAIZ. "www/emendas/_funcoes.php";
        include_once APPRAIZ. 'www/planacomorc/_funcoespi.php';
        include_once APPRAIZ. 'www/planacomorc/_funcoes.php';
        include_once APPRAIZ. 'territorios/classes/model/Esfera.class.inc';
        include_once APPRAIZ. 'emendas/classes/model/EmendaDetalhe.inc';
        include_once APPRAIZ. 'planacomorc/classes/model/PiSei.inc';
        include_once APPRAIZ. 'planacomorc/classes/model/PiPronac.inc';
        include_once APPRAIZ. 'planacomorc/classes/model/PiDelegacao.inc';
        include_once APPRAIZ. 'spo/classes/model/PtresSubunidade.inc';
        include_once APPRAIZ. 'emendas/classes/model/Beneficiario.inc';
        include_once APPRAIZ. 'emendas/classes/model/Proponente.inc';
        include_once APPRAIZ. 'public/classes/Model/SubUnidadeOrcamentaria.inc';
        include_once APPRAIZ. 'emendas/classes/model/Emenda.inc';
        include_once APPRAIZ. 'emendas/classes/model/Autor.inc';
        include_once APPRAIZ. 'emendas/classes/model/Siconv.inc';
        include_once APPRAIZ. 'emendas/classes/model/EmendaDetalhe.inc';
        include_once APPRAIZ. 'monitora/classes/Pi_PlanoInterno.class.inc';
        include_once APPRAIZ. 'planacomorc/classes/model/HistoricoPiUsuario.inc';
    break;
    case 'proporc':
        require_once APPRAIZ. 'includes/funcoesspo.php';
        require_once APPRAIZ. 'www/proporc/_funcoesgestaoploa.php';
        require_once APPRAIZ. 'www/proporc/_funcoes.php';
    break;
    case 'alteracao':
        include_once APPRAIZ. "www/alteracao/_constantes.php";
        require_once APPRAIZ. 'www/alteracao/_funcoes.php';
    break;
    case 'progfin':
        require_once APPRAIZ. 'www/progfin/_funcoessolicitacoesrecursos.php';
        break;
    case 'sismedio':
        include_once APPRAIZ. "www/sismedio/_constantes.php";
        include_once APPRAIZ. "www/sismedio/_funcoes.php";
        include_once APPRAIZ. "www/sismedio/_funcoes_universidade.php";
    break;
    case 'sisindigena':
        include_once APPRAIZ. "www/sisindigena/_constantes.php";
        include_once APPRAIZ. "www/sisindigena/_funcoes.php";
        include_once APPRAIZ. "www/sisindigena/_funcoes_universidade.php";
    break;
    case 'sisindigena2':
        include_once APPRAIZ. "www/sisindigena2/_constantes.php";
        include_once APPRAIZ. "www/sisindigena2/_funcoes.php";
        include_once APPRAIZ. "www/sisindigena2/_funcoes_universidade.php";
    break;
    case 'sispacto2':
        include_once APPRAIZ. "www/sispacto2/_constantes.php";
        include_once APPRAIZ. "www/sispacto2/_funcoes.php";
        include_once APPRAIZ. "www/sispacto2/_funcoes_coordenadorlocal.php";
        include_once APPRAIZ. "www/sispacto2/_funcoes_universidade.php";
        include_once APPRAIZ. "www/sispacto2/_funcoes_orientadorestudo.php";
        include_once APPRAIZ. "www/sispacto2/_funcoes_formadories.php";
    break;
    case 'sispacto3':
        include_once APPRAIZ. "www/sispacto3/_constantes.php";
        include_once APPRAIZ. "www/sispacto3/_funcoes.php";
        include_once APPRAIZ. "www/sispacto3/_funcoes_coordenadorlocal.php";
        include_once APPRAIZ. "www/sispacto3/_funcoes_universidade.php";
        include_once APPRAIZ. "www/sispacto3/_funcoes_orientadorestudo.php";
        include_once APPRAIZ. "www/sispacto3/_funcoes_formadories.php";
    break;
    case 'escolaterra':
        include_once APPRAIZ. "www/escolaterra/_constantes.php";
        include_once APPRAIZ. "www/escolaterra/_funcoes.php";
    break;
    case 'sispacto':
        include_once APPRAIZ. "www/sispacto/_constantes.php";
        include_once APPRAIZ. "www/sispacto/_funcoes.php";
        include_once APPRAIZ. "www/sispacto/_funcoes_coordenadorlocal.php";
        include_once APPRAIZ. "www/sispacto/_funcoes_universidade.php";
        include_once APPRAIZ. "www/sispacto/_funcoes_orientadorestudo.php";
        include_once APPRAIZ. "www/sispacto/_funcoes_formadories.php";
    break;
    case 'brasilpro':
        include_once APPRAIZ. "www/brasilpro/_funcoes.php";
    break;
    case 'par':
        include_once APPRAIZ. "www/par/autoload.php";
        include_once APPRAIZ. "www/par/_funcoes.php";
        include_once APPRAIZ. "www/par/_funcoesPar.php";
        include_once APPRAIZ. "www/par/_constantes.php";
    break;
    case 'cte':
        include_once APPRAIZ. "www/cte/_funcoes.php";
    break;
    case 'demandas':
        include_once APPRAIZ. "www/demandas/_constantes.php";
        include_once APPRAIZ. "www/demandas/_funcoes.php";
    break;
    case 'evento':
        include_once APPRAIZ. "www/evento/_constantes.php";
        include_once APPRAIZ. "www/evento/_funcoes.php";
    break;
    case 'pdeescola':
        include_once APPRAIZ. "www/pdeescola/_constantes.php";
        include_once APPRAIZ. "www/pdeescola/_funcoes.php";
        include_once APPRAIZ. "www/pdeescola/_mefuncoes.php";
    break;
    case 'pdeinterativo':
        include_once APPRAIZ. "www/pdeinterativo/_funcoesplanoestrategico_1.php";
    break;
    case 'emenda':
        include_once APPRAIZ. "www/emenda/_constantes.php";
        include_once APPRAIZ. "www/emenda/_funcoes.php";
        include_once APPRAIZ. "www/emenda/_funcoesWorkflow.php";
    break;
    case 'ies':
        include_once APPRAIZ. "www/ies/_funcoes.php";
    break;
    case 'reuni':
        include_once APPRAIZ. "reuni/www/funcoes.php";
    break;
    case 'fabrica':
        include_once APPRAIZ. "www/fabrica/_constantes.php";
        include_once APPRAIZ. "www/fabrica/_funcoes.php";
        require_once APPRAIZ. 'fabrica/classes/autoload.inc';
    break;
    case 'academico':
        include_once APPRAIZ. "www/academico/_constantes.php";
        include_once APPRAIZ. "www/academico/_componentes.php";
        include_once APPRAIZ. "www/academico/_funcoes.php";
    break;
    case 'demandasfies':
        include_once APPRAIZ. "www/demandasfies/_funcoes.php";
        include_once APPRAIZ. "www/demandasfies/_constantes.php";
    break;
    case 'obras':
        include_once APPRAIZ. "www/obras/_constantes.php";
        include_once APPRAIZ. "www/obras/_funcoes.php";
        include_once APPRAIZ. "www/obras/_componentes.php";
        require_once APPRAIZ. "includes/ActiveRecord/classes/Entidade.php";
    break;
    case 'obras2':
        include_once APPRAIZ. "www/obras2/_constantes.php";
        include_once APPRAIZ. "www/obras2/_funcoes.php";
        include_once APPRAIZ. "www/obras2/_componentes.php";
        include_once APPRAIZ. "includes/classes/modelo/obras2/ModeloRestricaoQuestionario.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/Empreendimento.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/Obras.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/Restricao.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/entidade/Endereco.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/entidade/Entidade.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/SituacaoObra.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/territorios/Municipio.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/territorios/Estado.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/EtapaQuestao.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/Empreendimento.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/Obras.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/ArquivoQuestaoSupervisao.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/ArquivoRespostaSubQuestao.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/Contato.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/RiscoQuestionarioSupervisao.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/ContatosObra.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/Contrato.class.inc";
        include_once APPRAIZ. "includes/classes/dateTime.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/EmpreendimentoSupervisao.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/FaseLicitacao.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/FilaRestricao.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/OrdemServicoMI.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/Cronograma_PadraoMi.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/Itens_Composicao_PadraoMi.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/QtdItensComposicaoObraMi.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/ItensComposicao.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/ItensComposicaoObras.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/ItensExecucaoOrcamentaria.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/Licitacao.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/ObraLicitacao.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/ObrasContrato.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/QuestaoSupervisao.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/Questao.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/SubQuestao.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/Supervisao.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/SupervisaoMi.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/Supervisao_Empenho.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/Supervisao_Grupo.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/Supervisao_Grupo_Empresa.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/Supervisao_Os.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/Supervisao_Grupo_Mesoregiao.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/Supervisao_Os_Obra.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/SupervisaoItem.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/SupervisaoEmpresa.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/TipoAnexoOsMi.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/AnexoOsMi.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/Contato.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/UsuarioResponsabilidade.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/Email.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/DestinatarioEmail.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/EmpresaMi.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/Solicitacao.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/DadosSolicitacao.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/Validador.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/SolicitacaoDesembolso.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/SupervisaoEmpresaRestricao.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/HistoricoRestricao.class.inc";
        include_once APPRAIZ. "includes/classes/modelo/obras2/Cronograma.class.inc";
    break;
    case 'conjur':
        include_once APPRAIZ. "www/conjur/_constantes.php";
        include_once APPRAIZ. "www/conjur/_funcoes.php";
    break;
    case 'snf':
        include_once APPRAIZ. "www/snf/_constantes.php";
        include_once APPRAIZ. "www/snf/_funcoes.php";
    break;
    case 'sic':
        include_once APPRAIZ. "www/sic/_constantes.php";
        include_once APPRAIZ. "www/sic/_funcoes.php";
    break;
    case 'profeinep':
        include_once APPRAIZ. "www/profeinep/_constantes.php";
        include_once APPRAIZ. "www/profeinep/_funcoes.php";
    break;
    case 'monitora':
        include_once APPRAIZ. "monitora/www/_constantes.php";
        include_once APPRAIZ. "monitora/www/_funcoes.php";
    break;
    case 'elabrev':
        include_once APPRAIZ. "elabrev/www/_constantes.php";
        include_once APPRAIZ. "elabrev/www/_funcoes.php";
    break;
    case 'gestaopessoa':
        include_once APPRAIZ. "www/gestaopessoa/_constantes.php";
        include_once APPRAIZ. "www/gestaopessoa/_funcoes.php";
    break;
    case 'gestaodocumentos':
        include_once APPRAIZ. "www/gestaodocumentos/_constantes.php";
        include_once APPRAIZ. "www/gestaodocumentos/_funcoes.php";
        include_once APPRAIZ. "www/gestaodocumentos/ajax.php";
    break;
    case 'sase':
        include_once APPRAIZ. "www/sase/_funcoes.php";
    break;
    case 'sisfor':
        include_once APPRAIZ. "www/sisfor/_constantes.php";
        include_once APPRAIZ. "www/sisfor/_funcoes.php";
    break;
    case 'cproc':
        include_once APPRAIZ. "www/cproc/_constantes.php";
        include_once APPRAIZ. "www/cproc/_funcoes.php";
        include_once APPRAIZ. "includes/classes/Modelo.class.inc";
        include_once APPRAIZ. "cproc/classes/ModeloCproc.class.inc";
        include_once APPRAIZ. "cproc/classes/Processo.class.inc";
        include_once APPRAIZ. "cproc/classes/WorkflowCproc.class.inc";
        include_once APPRAIZ. "cproc/classes/Processo.class.inc";
        include_once APPRAIZ. "cproc/classes/WorkflowCproc.class.inc";
    break;
    case 'progorc':
        include_once APPRAIZ. "www/progorc/_funcoes.php";
    break;
    case 'sicaj':
        include_once APPRAIZ. 'www/sicaj/_constantes.php';
        include_once APPRAIZ. 'www/sicaj/_funcoes.php';
    break;
    case 'ted':
        include_once APPRAIZ. 'spo/autoload.php';
        include_once APPRAIZ. 'www/ted/_constantes.php';
        include_once APPRAIZ. 'www/ted/_funcoes.php';
        include_once APPRAIZ. 'ted/classes/Ted/Connect.php';
        include_once APPRAIZ. 'ted/classes/Ted/Model/Utils.php';
        include_once APPRAIZ. 'ted/classes/Ted/Model/RelatorioCumprimento/Business.php';
        include_once APPRAIZ. 'ted/classes/Ted/Model/Responsabilidade.php';
        include_once APPRAIZ. 'www/ted/_condicao_workflow.inc';
    break;
}

if ( !$db )
{
	$db = new cls_banco();
}

if( $_REQUEST['req_ajax_workflow'] ){
	if( $_POST['dados'] != '' ){
		$dados = json_decode(str_replace('\\', '', $_POST['dados']),true);
		$_POST = array_merge($_POST, $dados);
	}

	// if( $_SESSION['sisdiretorio'] == 'cproc' )
		// $_REQUEST['req_ajax_workflow'] = str_replace('form_', '', $_REQUEST['req_ajax_workflow']);

	$_REQUEST['req_ajax_workflow']();
	die();
}

if(!$_REQUEST['docid'] || !$_REQUEST['esdid'] || !$_REQUEST['aedid']) {
	echo "<script>
			alert('Informações não foram passadas corretamente. Refaça o procedimento.');
			window.opener.location='?modulo=inicio&acao=C';
			window.close();
		  </script>";
	exit;
}

$docid = (integer) $_REQUEST['docid'];
$esdid = (integer) $_REQUEST['esdid'];
$aedid = (integer) $_REQUEST['aedid'];
$cmddsc = trim( $_REQUEST['cmddsc'] );
$verificacao = (string) $_REQUEST['verificacao'];

// verifica se precisa de comentário e se comentário está preenchido
if ( wf_acaoNecessitaComentario2( $aedid ) && !$cmddsc )
{
	include "alterar_estado_comentario.php";
	exit();
}

// trata dado para verificacao externa
$dadosVerificacao = unserialize( stripcslashes( $verificacao ) );
if ( !is_array( $dadosVerificacao ) )
{
	$dadosVerificacao = array();
}

// realiza alteracao de estado
if ( wf_alterarEstado( $docid, $aedid, $cmddsc, $dadosVerificacao ) )
{
    //var_dump($a);
    //die();
	$mensagem = "Estado alterado com sucesso!";
}
else
{
	$mensagem = wf_pegarMensagem();
	$mensagem = $mensagem ? $mensagem : "Não foi possível alterar estado do documento.";
}

?>
<script type="text/javascript">
var winW = 10;
var winH = 10;
var esdid = '<?php echo $esdid?>';

if (document.body && document.body.offsetWidth) {
	document.body.offsetWidth=winW;
	document.body.offsetHeight=winH;
}
if (document.compatMode=='CSS1Compat' &&
    document.documentElement &&
    document.documentElement.offsetWidth ) {

	document.documentElement.offsetWidth=winW;
	document.documentElement.offsetHeight=winH;
}
if (window.innerWidth && window.innerHeight) {
	window.innerWidth=winW;
	window.innerHeight=winH;
}

	window.opener.wf_atualizarTela( '<?php echo $mensagem ?>', self );
//	if (esdid == '257'){
//		//alert('<?php //echo $mensagem ?>');
//		window.opener.opener.wf_atualizarTela( '<?php echo $mensagem ?>', self );
//	}else{
//		window.opener.wf_atualizarTela( '<?php echo $mensagem ?>', self );
//	}



</script>