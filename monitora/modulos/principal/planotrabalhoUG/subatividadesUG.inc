<?php

// carrega as fun��es de integra��o
include_once "planotrabalhoUG/_constantes.php";
include_once "planotrabalhoUG/unidade_atividade_funcoes.php";
include_once "planotrabalhoUG/_funcoes.php";
include_once "planotrabalhoUG/_componentes.php";

// verifica qual unidade selecionada
if($_REQUEST['ungcod']) {
	$_SESSION['monitora_var']['ungcod']=$_REQUEST['ungcod'];
}

// obt�m dados da atividade vinculada � a��o
$atividade = retornaTarefaUnidade( $_SESSION['monitora_var']['ungcod'], $_SESSION['exercicio'] );

$_SESSION["projeto"] = $atividade["_atiprojeto"];
// N�o se aplica mais ao PDE
//define( "PROJETO", $atividade["_atiprojeto"] );
define( "PROJETO", 0);
projeto_verifica_selecionado( $atividade["atiid"] );


if ($_REQUEST["atiid"]) {
	$atividade = atividade_pegar( $_REQUEST["atiid"] );
} else {
	$atividade = atividade_pegar( $atividade["atiid"] );
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

extract( $atividade ); # mant�m o formul�rio preenchido

if($atividade['_atiprofundidade'] > 1) {
	// montando o menu
	echo montarAbasArray(carregardadosplanotrabalhoUG_sub(), "/monitora/monitora.php?modulo=principal/planotrabalhoUG/subatividadesUG&acao=A&atiid=".$_REQUEST['atiid']);
} else {
	// montando o menu
	echo montarAbasArray(carregardadosplanotrabalhoUG_raiz(), "/monitora/monitora.php?modulo=principal/planotrabalhoUG/subatividadesUG&acao=A&ungcod=".$_SESSION['monitora_var']['ungcod']);

}


monta_titulo("Plano de Trabalho",$atividade['atidescricao']);

?>
<table class="tabela" bgcolor="#fbfbfb" cellspacing="0" cellpadding="2" align="center">
	<tr>
		<td>
			<? 
			echo montar_resumo_atividade( $atividade, $numeracao_relativa = true );
			
			$situacao = array(
				STATUS_NAO_INICIADO,
				STATUS_EM_ANDAMENTO,
				STATUS_SUSPENSO,
				STATUS_CANCELADO,
				STATUS_CONCLUIDO
			);
			$diretorio = $_SESSION["sisdiretorio"] . "/planotrabalhoUG";
			//ver($atividade["atiid"], $profundidade, $situacao, null, $diretorio, $numeracao_relativa,d);
			echo arvore( $atividade["atiid"], $profundidade, $situacao, null, $diretorio, $numeracao_relativa = true );
			?>
		</td>
	</tr>
</table>
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
    <tr style="background-color: #cccccc">	
		<td>
			<input type="hidden" name="atiid" value="<?php echo $_GET[atiid]; ?>">
		</td>
	</tr>		
</table>