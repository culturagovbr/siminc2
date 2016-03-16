<?php
$_SESSION['sisid'] = 88;
$_SESSION['sisbaselogin'] = 'simec_desenvolvimento'; // base
$_SESSION['baselogin'] = 'simec_desenvolvimento'; // base
extract($_POST);
if( isset($cpf) ){
$_SESSION['usucpf'] = $cpf != '' ? $cpf : ( $_SESSION['usucpf'] ? $_SESSION['usucpf'] : '');
    $_SESSION['usucpf'] = str_replace('.', '', $_SESSION['usucpf']);
    $_SESSION['usucpf'] = str_replace('-', '', $_SESSION['usucpf']);

}
$_SESSION['usucpforigem'] = $_SESSION['usucpf'];

require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
include APPRAIZ . "includes/Snoopy.class.php";
include APPRAIZ . "includes/classes/Modelo.class.inc";

$db = new cls_banco();

//Validar C�digo INEP
if($_REQUEST['validar_inep']){
	$sql_inep = "SELECT entid, entnome
				FROM entidade.entidade 
				WHERE entcodent IS NOT NULL AND entstatus = 'A' AND entcodent = '{$_REQUEST['inep']}'";
	$rs_inep = $db->pegaLinha($sql_inep);
	if(empty($rs_inep['entid'])){
		echo "1";
	} else {
		echo $rs_inep['entnome'];
	}
	exit();
}

// CPF do administrador de sistemas
//$_SESSION['usucpf'] = '';
//$_SESSION['usucpforigem'] = '';

include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";
include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";

function pegaQuantidadeRespondida( $qrpid ){
	
	global $db;
	
	$sql = "select 
					COUNT(DISTINCT r.perid) as perguntas
				from 
					livro.questionario lq
				INNER JOIN questionario.questionarioresposta qr ON qr.qrpid = lq.qrpid
				INNER JOIN questionario.grupopergunta gp ON gp.queid = qr.queid
				INNER JOIN questionario.pergunta p ON p.grpid = gp.grpid
				INNER JOIN questionario.itempergunta ip ON ip.perid = p.perid
				INNER JOIN questionario.resposta r ON r.itpid = ip.itpid AND r.qrpid = lq.qrpid
				WHERE
					qr.queid = 80 AND qr.qrpid = ".$qrpid;
		
		return $db->pegaUm($sql);
}

function pegaQrpid( $usuario, $inep, $disc ){

	global $db;

    include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
    
    $queid = 80;
    
    $usuario = str_replace('.', '', $usuario);
    $usuario = str_replace('-', '', $usuario);
	$inep = preg_replace("/[^0-9]/","",$inep);
			 
    $sql = "SELECT
            	q.qrpid
            FROM
            	livro.questionario q
            INNER JOIN questionario.questionarioresposta qr ON qr.qrpid = q.qrpid
            WHERE
            	q.qplcpf = '{$usuario}'
            	AND q.qplinep = '{$inep}'
            	AND q.discid = {$disc}
            	AND qr.queid = {$queid}";

    $qrpid = $db->pegaUm( $sql );

    if(!$qrpid){
    	
    	$arParam = array ( "queid" => $queid, "titulo" => "PNLD (Usu: ".$usuario.", Inep: ".$inep.", Disc: ".$disc.")" );
        $qrpid = GerenciaQuestionario::insereQuestionario( $arParam );
        $sql = "INSERT INTO livro.questionario (qplcpf, qplinep, discid, qrpid) VALUES ('{$usuario}', '{$inep}', {$disc}, {$qrpid})";
        $db->executar( $sql );
        $db->commit();
    }
    return $qrpid;
}

?>
<html>
	<head>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		<script src="../includes/funcoes.js" type="text/javascript"></script>
		<script type="text/javascript" src="../includes/webservice/cpf.js"></script>
		<script type="text/javascript" src="/includes/prototype.js"></script>
		<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<script>
			jQuery.noConflict();
			function buscaCPFReceita(cpf){
				if(cpf){		
					var valor = cpf.replace(".", "");	
					valor = valor.replace(".", "");
					valor = valor.replace("-", "");
					
					if( validar_cpf(valor) ){		
						var comp = new dCPF();
						comp.buscarDados( valor );
						jQuery('#mostra_nome').html(comp.dados.no_pessoa_rf);
						jQuery('#mostra_nome').show();
					} else {
						alert('CPF informado � inv�lido');
						jQuery('[name=cpf]').val();
						jQuery('[name=cpf]').focus();
						return false;
					}
				}
			}
	
			function validarInep(inep){
				jQuery.post("questionario.php",{  validar_inep: "true", inep: inep }, function(data) {
					if(data == 1){
						jQuery('[name=inep]').val('');
						jQuery('[name=inep]').focus();
						alert("O c�digo do INEP informado n�o foi encontrado na Base do sistema.");
					} else {
						jQuery('#mostra_inep').html(data);
						jQuery('#mostra_inep').show();
					}
				});
			} 			
		
			jQuery(function(){
				
				jQuery('#enviarDados').click(function(){
					if(jQuery('[name=cpf]').val() == ''){
						alert('O campo CPF � obrigat�rio!');
						jQuery('[name=cpf]').focus();
						return false;
					}
					if(!validar_cpf(jQuery('[name=cpf]').val())){
						alert('CPF inv�lido!');
						jQuery('[name=cpf]').focus().val('');
						return false;
					}
					if(jQuery('[name=inep]').val() == ''){
						alert('O campo Inep � obrigat�rio!');
						jQuery('[name=inep]').focus();
						return false;
					}
					if(jQuery('[name=disciplina]').val() == ''){
						alert('O campo Disciplina � obrigat�rio!');
						jQuery('[name=disciplina]').focus();
						return false;
					}
					jQuery('[name=formulario]').submit();
				});
			});
		</script>
	</head>
	<body>	
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class='SubTituloEsquerda'><img src = "../includes/layout/verde/img/logo.png"></td>
		</tr>
		<tr>
			<td class='SubTituloCentro'>Question�rio PNLD</td>
		</tr>
		<tr>
			<td>
				<p><h2>Prezado(a) professor(a),</h2>
				<p>Tendo em vista a melhoria do Programa Nacional do Livro Did�tico - PNLD, apresentamos abaixo um breve question�rio 
				com o objetivo de compreender a opini�o dos docentes do ensino m�dio sobre o processo de escolha e utiliza��o dos livros
				did�ticos distribu�dos pelo PNLD 2012/Ensino M�dio, bem como sobre os aspectos qualitativos dessas obras.</p>
				<p>Para acessar o question�rio, insira o seu CPF, o C�digo Identificador da Escola (Inep), bem como a disciplina que leciona, 
				nos campos abaixo. Caso n�o conhe�a esse C�digo, solicite-o ao Diretor e/ou ao Coordenador Pedag�gico. Na hip�tese de voc� 
				lecionar mais de um componente curricular, preencha um question�rio para cada um desses componentes. Desde j�, agradecemos a
				sua colabora��o no processo de melhoria das pol�ticas p�blicas destinadas ao ensino m�dio.</p>
			</td>	
		</tr>
	</table>
	<br>
	<form name="formulario" method="post" action="">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
				<?php if(!isset($_POST['cpf']) && !isset($_POST['inep']) && !isset($_POST['disciplina'])): ?>
		<tr>	
			<td bgcolor="#DCDCDC" align="center">Informe os dados abaixo para preencher o question�rio:</td>
		</tr>
		<tr>
			<td class='SubTituloCentro'>
				<table>
					<tr>
						<td>CPF:</td>
						<td>
							<?php echo campo_texto('cpf', 'S', 'S', '', 20, 255, '###.###.###-##', '','','','','','','','buscaCPFReceita(this.value)'); ?>
						</td>
						<td id="mostra_nome"></td>
					</tr>
					<tr>
						<td>C�digo Identificador da Escola (Inep):</td>
						<td><?php echo campo_texto('inep', 'S', 'S', '', 20, 255, '########', '','','','','','','','validarInep(this.value)'); ?></td>
						<td id="mostra_inep"></td>
					</tr>
					<tr>
						<td>Disciplina:</td>
						<td colspan="2">
							<?php 
							$sql = "SELECT discid as codigo, discdsc as descricao
									FROM livro.disciplina
									ORDER BY discdsc";
							$db->monta_combo('disciplina',$sql,'S',"Selecione...",'','','','','S'); ?>
						</td>
					</tr>
					<tr>
						<td>
						<td colspan="2"><input type="button" value="Entrar" id="enviarDados"></td>
					</tr>
				</table>
				<tr>
					<td><?php else: ?></td>
					<td bgcolor="#DCDCDC" align="center"><input type="button" value="Voltar" onClick="location.href='questionario.php'"></td>
				</tr>
			<tr>
				<td>
					<?php
					$usuario = $_POST['cpf'];
					$inep = $_POST['inep'];
					$disc = $_POST['disciplina'];
					
					if($disc == ""){
						echo "<script>alert('Favor informar a disciplina.'); history.back(-1); </script>";
						die();
					}
					
					$qrpid = pegaQrpid($usuario, $inep, $disc);
					$qtdRespondidas = pegaQuantidadeRespondida($qrpid);
					$tela = new Tela( array("qrpid" => $qrpid, 'tamDivArvore' => 25, 'habilitado' => 'S') );
					endif;?>
				</td>
			</tr>
	</table>
</form>
</body>
</html>