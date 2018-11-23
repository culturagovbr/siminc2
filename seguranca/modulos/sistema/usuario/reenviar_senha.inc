<?php

function reenvia_senha_acesso(){
	
	global $db;
	
	$remetente = array( "nome"  => $_SESSION['usunome'],
						"email" => $_SESSION['usuemail'] );
	
	if ( $_REQUEST['usunome'] ) $texto  = " AND upper(u.usunome) LIKE replace('%".str_to_upper($_REQUEST['usunome'])."%', ' ' , '%')";
	if ( $_REQUEST['usucpf'] )  $texto .= " and u.usucpf like '%".str_to_upper(corrige_cpf($_REQUEST['usucpf']))."%'";
	if ( $_REQUEST['suscod'] ) 	$texto .= " and us.suscod = '".$_REQUEST['suscod']."'";
	if ( $_REQUEST['codigo'] )  $texto .= " and us.sisid = '".$_REQUEST['codigo']."'";
	
	/*UF e Municipio*/
	if ( $_REQUEST['regcod_disable'] ) $texto .= " and u.regcod = '".$_REQUEST['regcod_disable']."'";
	if ( $_REQUEST['muncod'] ) 		   $texto .= " and u.muncod = '".$_REQUEST['muncod']."'";
	
	//Unidade Orcamentaria
	if ( $_REQUEST['unidade'] ) $texto .= " and uni.unicod = '" . (integer) $_REQUEST['unidade'] . "'" ;	
	
	// Se já acessou o sistema
	if ( $_REQUEST["usuchaveativacao"] ) $texto .= " and u.usuchaveativacao = '" . $_REQUEST["usuchaveativacao"] . "'";
	
	if ( $_REQUEST['pflcod'] ) {
		
		$join_perfil = sprintf(
			"inner join seguranca.perfilusuario pu on pu.usucpf = u.usucpf 
			inner join seguranca.perfil p on p.pflcod = pu.pflcod and p.pflcod = %d",
			$_REQUEST['pflcod']
			
		);
	}
	
	$destinatario = $db->carregar(" SELECT DISTINCT 
										 u.usucpf, u.usuemail
									FROM 
										seguranca.usuario u ". $join_perfil ."
									LEFT JOIN 
										territorios.municipio m on m.muncod=u.muncod
									INNER JOIN 
										seguranca.usuario_sistema AS us ON u.usucpf = us.usucpf
									LEFT JOIN (
											SELECT 
												usucpf, sisid 
											FROM 
												seguranca.historicousuario
											WHERE 
												suscod = 'P'
											GROUP BY 
												usucpf, sisid
										    ) h ON 
										    	h.usucpf = u.usucpf AND us.sisid = h.sisid		
									LEFT JOIN 
										public.unidade uni on u.unicod=uni.unicod and uni.unitpocod='U'
									LEFT JOIN 
										public.orgao o on u.orgcod=o.orgcod
									WHERE 
										u.orgcod=o.orgcod and 
										( uni.uniano is null or uni.uniano = o.organo )" . $texto);

    $assunto = "Aprovação do Cadastro no ".SIGLA_SISTEMA;
		
	$total_usuarios = count( $destinatario );
	
	$ok = true;
	
	for ( $i = 0; $i < $total_usuarios; $i++ ){
		
		$destino  = $destinatario[$i]["usuemail"];
		$usucpf   = $destinatario[$i]["usucpf"];
		$senha    = strtoupper(senha());
		
		$sql = sprintf(
			"UPDATE seguranca.usuario SET ususenha = '%s', usuchaveativacao = 'f' WHERE usucpf = '%s'",
			md5_encrypt_senha( $senha, '' ),
			$usucpf
		);
		$db->executar( $sql );
		
		$conteudo = "
			<br/>
			<span style='background-color: red;'>
				<b>Esta é uma mensagem gerada automaticamente pelo sistema. </b>
			</span>
			<br/>
			<span style='background-color: red;'>
				<b>Por favor, não responda. Pois, neste caso, a mesma será descartada.</b>
			</span>
			<br/>
			";
		
		$conteudo .= sprintf(
			"%s %s<p>Sua conta está ativa. Sua Senha de acesso é: %s</p>",
			'Prezado Sr(a).',
			$_REQUEST['usunome'],
			$senha
			);
			
		if ( !enviar_email( $remetente, $destino, $assunto, $conteudo ) ){
			$ok = false;
		}
		
	}
	return $ok;
}

	if ( $_REQUEST["ajax"] ){
		
		if ( reenvia_senha_acesso() ){
			echo 'E-mails Enviados com Sucesso!';
			$db->commit();	
		}else{
			echo 'Ocorreu erro ao tentar enviar o(s) e-mail(s)!';
			$db->rollback();	
		}
		
		die;
		
	}

	/**
	 * Sistema Integrado de Monitoramento do Ministério da Educação
	 * Setor responsvel: SPO/MEC
	 * Desenvolvedor: Desenvolvedores Simec
	 * Analistas: Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>, Alexandre Soares Diniz
	 * Programadores: Renê de Lima Barbosa <renedelima@gmail.com>, Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>
	 * Módulo: segurança
	 * Finalidade: Consultar informações a resppeito das contas dos usuários
	 * Data de criação:
	 * Última modificação: 29/08/2006
	 */

	$modulo = $_REQUEST['modulo'] ;
	$encontrou = false;
	
if ($_SESSION['sisid']!=4) {$habil = 'N'; $_REQUEST['codigo']=$_SESSION['sisid'];} else $habil = 'S';

if ( $_REQUEST['formulario'] ) {
		$usucpf  = $_REQUEST['usucpf'];
		$usunome = $_REQUEST['usunome'];
		$codigo  = $_REQUEST['codigo'];
		$suscod  = $_REQUEST['suscod'];
		$pflcod  = $_REQUEST['pflcod'];
		$muncod  = $_REQUEST['muncod'];
		$usuchaveativacao = $_REQUEST['usuchaveativacao'];
	} else {
		$codigo = $_SESSION['sisid'];
	}

if(isset($_REQUEST['servico']) &&  $_REQUEST['servico']== 'listar_mun'){	
	$sql = "SELECT muncod, mundescricao as mundsc 
		FROM territorios.municipio 
		WHERE estuf = '".$_REQUEST['estuf']."' ORDER BY mundsc";
	$dados = $db->carregar($sql);
	
	$enviar = '';
	if($dados) $dados = $dados; else $dados = array();
	foreach ($dados as $data) {
		$enviar .= "<option value= ".$data['muncod'].">  ".simec_htmlentities($data['mundsc'])." </option> \n";
	}
			
	die($enviar);
}
	
?>
<?php include APPRAIZ . "includes/cabecalho.inc"; ?>
<br/>
<?php

	$db->cria_aba( $abacod_tela, $url, $parametros );
	
	$titulo_modulo = 'Reenviar Senha de Acesso';
	monta_titulo( $titulo_modulo, '' );

?>
<form method="POST" name="formulario">
<script type="text/javascript" src="../includes/prototype.js"></script>
	<input type="hidden" name="formulario" value="1"/>
	<input type=hidden name="modulo" value="<?= $modulo ?>"/>
	<input type=hidden name="act" value="0"/>
	
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td align='right' class="SubTituloDireita">CPF (ou parte do CPF):</td>
			<td><?=campo_texto('usucpf','','','',16,14,'###.###.###-##','', 'left', '', 0, 'id="usucpf"');?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Nome completo (ou parte do nome):</td>
			<td><?=campo_texto('usunome','','','',50,50,'','', 'left', '', 0, 'id="usunome"');?></td>
		</tr>
		<tr bgcolor="#F2F2F2">
			<td align='right' class="SubTituloDireita">Unidade Federativa:</td>
			<td>
				<?php
					$sql = "SELECT regcod AS codigo, regcod||' - '||descricaouf AS descricao FROM uf WHERE codigoibgeuf IS NOT NULL ORDER BY 2";
					$db->monta_combo("regcod",$sql,"","&nbsp;",'listar_municipios','', '', '', '', 'regcod');
				?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Município:</td>
			<td>
				<div id="muncod_on" style="display:none;">
					<select
						id="muncod"
						name="muncod"
						onchange=""
						class="CampoEstilo"
					></select>
				</div>
				<div id="muncod_off" style="color:#909090;">Selecione uma Unidade Federativa.</div>
			</td>
		</tr>
		
		<tr>
			<td align='right' class="SubTituloDireita">Sistema:</td>
			<td>
				<?php
					$sql = "select s.sisid as codigo, s.sisdsc as descricao from seguranca.sistema s where s.sisstatus='A'";
					$db->monta_combo( "codigo", $sql,$habil, 'Qualquer (selecione)', '', '', '', '', '', 'codigo' );
				?>	
			</td>
		</tr>
		<?php if( $codigo ): ?>
		<tr>
			<td align='right' class="SubTituloDireita">Perfil:</td>
			<td>
				<?php
					$pflnivel = $db->pegaUm( "select min( pflnivel ) from seguranca.perfil p inner join seguranca.perfilusuario pu on pu.pflcod = p.pflcod and pu.usucpf = '". $_SESSION['usucpf'] ."'" );
					$sql = sprintf(
						"select p.pflcod as codigo, p.pfldsc as descricao from seguranca.perfil p where p.sisid = %d and p.pflstatus = 'A' and p.pflnivel >= %d",
						$codigo,
						$pflnivel
					);
					$db->monta_combo( "pflcod", $sql, 'S', 'Todos', '', '', '','','','pflcod' );
				?>	
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Unidade Orçamentária:</td>
			<td>
			<?php

				//SELECT VÁLIDO SE O BANCO ESTIVESSE CONSISTENTE
				/*
				$sql = "select u2.unicod as codigo, u.unidsc || ' - ' || u2.unidsc as descricao
					from public.unidade u
					inner join public.unidade u2 on
						u2.orgcod = u.unicod
					where u2.unistatus='A' and u2.uniano = ". $_SESSION['exercicio'] ." order by u.unidsc, u2.unidsc";
				*/
				// SELECT PALEATIVO QUE PEGAR TODAS AS UNIDADES INDEPENDENTE DO EXERCÍCIO
				$sql = "select u2.unicod as codigo, u.unidsc || ' - ' || u2.unidsc as descricao
					from public.unidade u
					inner join public.unidade u2 on
						u2.orgcod = u.unicod
					inner join seguranca.usuario usu on
						usu.unicod = u2.unicod
					inner join seguranca.usuario_sistema us on
						us.usucpf = usu.usucpf
					where u2.unistatus='A' and u2.unitpocod = 'U' and us.sisid = ". $_SESSION['sisid'] ."
					group by u2.unicod, u.unidsc, u2.unidsc
					order by u2.unicod, u.unidsc, u2.unidsc";
				$db->monta_combo( "unidade", $sql, 'S', '&nbsp;', '', '', '', '', '','unidade' );
			?>
			</td>
		</tr>		
		<?php endif; ?>
		<? if(!isset($_REQUEST['formulario'])) $suscod = 'A'; ?>
		<?php if ( $_REQUEST['acao'] == 'A' || $_REQUEST['acao'] == 'C' ): ?>
			<tr bgcolor="#F2F2F2">			
				<td align='right' class="SubTituloDireita">Status geral do usuário:</td>
				<td>
					<input id="status_qualquer" type="radio" name="suscod" value="" <?= $suscod == '' ? 'checked="checked"' : "" ?>/>
					<label for="status_qualquer">Qualquer</label>
					
					<input id="status_ativo" type="radio" name="suscod" value="A" <?= $suscod == 'A' ? 'checked="checked"' : "" ?>/>
					<label for="status_ativo">Ativo</label>
					
					<input id="status_pendente" type="radio" name="suscod" value="P" <?= $suscod == 'P' ? 'checked="checked"' : "" ?>/>
					<label for="status_pendente">Pendente</label>
					
					<input id="status_bloqueado" type="radio" name="suscod" value="B" <?= $suscod == 'B' ? 'checked="checked"' : "" ?>/>
					<label for="status_bloqueado">Bloqueado</label>
				</td>
			</tr>
		<?php endif; ?>
		<tr bgcolor="#F2F2F2">			
			<td align='right' class="SubTituloDireita">Já acessou o sistema:</td>
			<td>
				<input id="acesso_qualquer" type="radio" name="usuchaveativacao" value="" <?= $usuchaveativacao == '' ? 'checked="checked"' : "" ?>/>
				<label for="acesso_sim">Qualquer</label>
			
				<input id="acesso_sim" type="radio" name="usuchaveativacao" value="t" <?= $usuchaveativacao == 't' ? 'checked="checked"' : "" ?>/>
				<label for="acesso_sim">Sim</label>
				
				<input id="acesso_nao" type="radio" name="usuchaveativacao" value="f" <?= $usuchaveativacao == 'f' ? 'checked="checked"' : "" ?>/>
				<label for="acesso_nao">Não</label>
			</td>
		</tr>
		<tr bgcolor="#C0C0C0">
			<td ></td>
			<td><input type='button' class="botao" name='consultar' value='Consultar' onclick="ProcuraUsuario()"/></td>
		</tr>
	</table>
</form>
<?php

	unset( $nlinhas );
	unset( $sql );

	$cabecalho = array('CPF', 'Nome Completo', 'Telefone', 'UF', 'Município', 'Órgão', 'Unidade',  'Data da Ultima Atualização' );
	
	// insere filtros de consulta
	if ($_REQUEST['usunome']) $texto = " and upper(u.usunome) like replace('%".str_to_upper($_REQUEST['usunome'])."%', ' ' , '%')";
	if ($_REQUEST['usucpf']) $texto.= " and u.usucpf like '%".str_to_upper(corrige_cpf($_REQUEST['usucpf']))."%'";
	if ($_REQUEST['suscod']) $texto.= " and us.suscod = '".$_REQUEST['suscod']."'";
	if ($_REQUEST['codigo']) $texto.= " and us.sisid = '".$_REQUEST['codigo']."'";
	/*UF e Municipio*/
	if ($_REQUEST['regcod_disable']) $texto.= " and u.regcod = '".$_REQUEST['regcod_disable']."'";
	if ($_REQUEST['muncod']) $texto.= " and u.muncod = '".$_REQUEST['muncod']."'";
	//Unidade Orcamentaria
	if ( $_REQUEST['unidade'] ) $texto.= " and uni.unicod = '" . (integer) $_REQUEST['unidade'] . "'" ;	
	
	// Se já acessou o sistema
	if ( $_REQUEST["usuchaveativacao"] ) $texto .= " and u.usuchaveativacao = '" . $_REQUEST["usuchaveativacao"] . "'";
	
	if ( $_REQUEST['pflcod'] ) {
		$join_perfil = sprintf(
			"inner join seguranca.perfilusuario pu on pu.usucpf = u.usucpf 
			inner join seguranca.perfil p on p.pflcod = pu.pflcod and p.pflcod = %d",
			$_REQUEST['pflcod']
		);
	}
	
	// seleciona qual ação o botão que aparece na listagem deve acionar
	if ( $_REQUEST['acao'] == 'A' ) {
		$funcao = "alterar_usu";
	} else {
		$funcao = "consultar_usu";
	}
		
	$sql= "SELECT DISTINCT 
			 u.usucpf, u.usunome, '(' || u.usufoneddd || ') ' || u.usufonenum as fone , u.regcod, m.mundescricao, trim(o.orgdsc),
		CASE WHEN uni.unidsc IS NULL THEN u.orgao
		     else uni.unidsc END AS unidsc, to_char(u.usudataatualizacao,'dd/mm/YYYY HH24:MI') as data 
		FROM seguranca.usuario u ". $join_perfil ."
		LEFT JOIN territorios.municipio m on m.muncod=u.muncod
		INNER JOIN seguranca.usuario_sistema AS us ON u.usucpf = us.usucpf
		LEFT JOIN (
				SELECT usucpf, sisid 
				FROM seguranca.historicousuario
				WHERE suscod = 'P'
				GROUP BY usucpf, sisid
			    ) h ON h.usucpf = u.usucpf 
			    	AND us.sisid = h.sisid		

		LEFT JOIN public.unidade uni on u.unicod=uni.unicod and uni.unitpocod='U'
		LEFT JOIN public.orgao o on u.orgcod=o.orgcod
		WHERE u.orgcod=o.orgcod and ( uni.uniano is null or uni.uniano = o.organo ) $texto
		GROUP BY u.usucpf, u.usunome, u.usufoneddd, u.usufonenum, u.regcod, o.orgdsc, uni.unidsc,u.orgao,  u.usudataatualizacao , m.mundescricao
		ORDER BY u.usunome";	
	
	// exibe o resultado da consulta
	if ( $_REQUEST['act'] == procura ) {
		$db->monta_lista( $sql, $cabecalho, 100, 20, '', '' ,'' );
?>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr bgcolor="#C0C0C0">
		<td>
			<input type="button" name="reenviar_senha" id="reenviar_senha" 
			 value="Reenviar Senha" style="cursor: pointer;" onclick="ajax_reenvia_email();"/>
		</td>
	</tr>
</table>

<?php } ?>

<script language="javascript"><!--
	
	function ajax_reenvia_email(){
	
		var url = location.href + '&ajax=1';
	
		var myAjax = new Ajax.Request(
				url,
				{
					 method: 'post',
					 parameters:   '&usunome=<?php echo $_REQUEST["usunome"] ?>'
					 			 + '&usucpf=<?php echo $_REQUEST["usucpf"] ?>'
					 			 + '&suscod=<?php echo $_REQUEST["suscod"] ?>'
					 			 + '&codigo=<?php echo $_REQUEST["codigo"] ?>'
					 			 + '&regcod_disable=<?php echo $_REQUEST["regcod_disable"] ?>'
					 			 + '&muncod=<?php echo $_REQUEST["muncod"] ?>'
					 			 + '&unidade=<?php echo $_REQUEST["unidade"] ?>'
					 			 + '&usuchaveativacao=<?php echo $_REQUEST["usuchaveativacao"] ?>'
					 			 + '&pflcod=<?php echo $_REQUEST["pflcod"] ?>',
                     onComplete: function(res)
                     {
                     	alert(res.responseText);
                     }
			});
	
	}
	
	function listar_municipios( estuf )
    {
    	validar_mun = true;    
        var div_on = document.getElementById( 'muncod_on' );
		var div_off = document.getElementById( 'muncod_off' );        
		div_on.style.display = 'block';
		div_off.style.display = 'none';
				
        return new Ajax.Updater('muncod', '<?=$_SESSION['sisdiretorio'] ?>.php?modulo=sistema/usuario/consusuario&acao=<?=$_REQUEST['acao'] ?>',
         {
         
            method: 'post',
            parameters: '&servico=listar_mun&estuf=' + estuf,
            onComplete: function(res)
            {	
            // alert(res.responseText);                   
            }
        });
    }
	
	function ProcuraUsuario() {
		if ( document.formulario.usunome ) {
			document.formulario.act.value = "procura";
			document.formulario.submit()
		}
	}

	function consultar_usu( usucpf ){
		window.location = "<?=$_SESSION['sisarquivo']?>.php?modulo=sistema/usuario/cadusuario&acao=C&usucpf="+ usucpf;
	}

	function alterar_usu( usucpf ){
		window.location = "<?=$_SESSION['sisarquivo']?>.php?modulo=sistema/usuario/cadusuario&acao=A&usucpf="+ usucpf;
	}

--></script>