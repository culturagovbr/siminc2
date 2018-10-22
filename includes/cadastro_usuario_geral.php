<?php
/**
 * Sistema Integrado de Monitoramento do Ministério da Educação
 * Setor responsvel: SPO/MEC
 * Analistas: Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>
 * Programadores: Renan de Lima Barbosa <renandelima@gmail.com>, Renê de Lima Barbosa <renedelima@gmail.com>
 * Módulo: Usuário
 * Finalidade: Permitir o controle de cadastro de usuários.
 * Data de criação: 19/11/2006
 * Última modificação: 21/11/2006
 */


//ver ( '1 ------ ' . time() );

include_once APPRAIZ . 'includes/funcoes_espelhoperfil.php';


// Carrega a combo com os orgãos do tipo selecionado
if( $_REQUEST["ajax"] == 1 ){

	// Se for estadual verifica se existe estado selecionado
	if ( $_REQUEST["tpocod"] == 2 && !$_REQUEST["regcod"]  ){

		echo '<font style="color:#909090;">
					Favor selecionar um Estado.
				  </font>';
		die;

	}

	// Se for municipal verifica se existe estado selecionado
	if ( $_REQUEST["tpocod"] == 3 && !$_REQUEST["muncod"]  ){

		echo '<font style="color:#909090;">
					Favor selecionar um município.
				  </font>';
		die;

	}

	$tpocod =  $_REQUEST["tpocod"];
	$muncod =  $_REQUEST["muncod"];

	carrega_orgao($editavel, $usucpf);
	die;

}

// Carrega a combo com as unidades do orgão selecionado
if( $_REQUEST["ajax"] == 2 ){

	carrega_unidade($_REQUEST["entid"], $editavel, $usuario->usucpf);
	die;

}

// Carrega a combo com as unidades gestoras da undiade selecionada
if( $_REQUEST["ajax"] == 3 ){

	carrega_unidade_gestora($_REQUEST["unicod"], $editavel, $usuario->usucpf);
	die;

}


// controle o cache do navegador
header( "Cache-Control: no-store, no-cache, must-revalidate" );
header( "Cache-Control: post-check=0, pre-check=0", false );
header( "Cache-control: private, no-cache" );
header( "Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT" );
header( "Pragma: no-cache" );

/*
 * Fucntion que gera código "function" de javascript
 */
function gerFuncResp ($sisid, $sisDir, $usucpf, $dados = array()) {
	$script = sprintf("
					function popresp_%s( pflcod, tprsigla ) {
						switch( tprsigla ){",
					  $sisid);

	foreach ($dados as $dado):
		$script .= sprintf("
								case '%s':
									abreresp%s = window.open(
										'../%s/geral/cadastro_responsabilidade_%s.php%spflcod='+pflcod+'&usucpf=%s',
										'popresp_%s',
										'menubar=no,location=no,resizable=no,scrollbars=no,status=yes,width=500,height=540');
									break;",
							$dado['tprsigla'],
							$sisid,
							$sisDir,
							$dado['tprurl'],
							(strpos($dado['tprurl'], '?') !== false ? '&' : '?'),
							$usucpf,
							$sisid);
	endforeach;

	$script .= "\n						}\n";
	$script .= sprintf("						abreresp%s.focus();\n",
			   $sisid);
	$script .= "					}\n\n";

	return $script;
}

		define( 'SENHA_PADRAO', 'siminc123' );

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

		$status = array(
				'A' => 'Ativo',
				'B' => 'Bloqueado',
				'P' => 'Pendente'
				);

		$acao = $_REQUEST['acao'];
		$usucpf = '';

		if($acao == 'U'){

			$usucpf = $_SESSION['usucpf'];
			$titulo_pagina = "Atualização de Dados Cadastrais";

		}else{

			$usucpf = $_REQUEST['usucpf'];
			$titulo_pagina = "Cadastro de Usuários";
		}

		// validando cpf
		//if(!validaCPF($usucpf)) {
		//	$db->insucesso("CPF Inválido","&acao=A","sistema/usuario/consusuario");
		//}

		// carrega os dados da conta do usuário
		$sql = sprintf("SELECT
							u.*,
							e.entid
						FROM
							seguranca.usuario u
						LEFT JOIN
							entidade.entidade e ON
							u.entid = e.entid
						WHERE
							usucpf = '%s'",
		$usucpf
		);

		$usuario = $db->pegaLinha( $sql );

        if(!$usuario)
        {
            $_REQUEST['acao'] = "A";
			if ( $_SESSION['sisid'] == 147 ) { // obras 2
	            $db->insucesso("Usuário Não Encontrado","&acao=A","sistema2/usuario2/consusuario");
			}
			else {
	            $db->insucesso("Usuário Não Encontrado","&acao=A","sistema/usuario/consusuario");
			}
        }

		extract( $usuario );

		$usuario = (object) $usuario;
		$cpf = formatar_cpf( $usuario->usucpf );



		/**
		 * Verifica se usuário só pode consultar
		 *
		 */
		$habilitar_edicao = 'S';
		if($acao == 'C') $habilitar_edicao = 'N';

        if (($_SESSION['sisid']) != 4 && ($_SESSION['usucpforigem'] != $_REQUEST['usucpf'])) {

            $sql = "select min(pflnivel) as pflnivel
                        from seguranca.perfilusuario pu
                        inner join seguranca.perfil p on p.pflcod = pu.pflcod
                    where usucpf = '{$_SESSION['usucpforigem']}'
                    and sisid = {$_SESSION['sisid']}";
            $menornivelusuario = $db->pegaUm($sql);

            $sql = "select min(pflnivel) as pflnivel
                        from seguranca.perfilusuario pu
                        inner join seguranca.perfil p on p.pflcod = pu.pflcod
                    where usucpf = '{$_REQUEST['usucpf']}'
                    and sisid = {$_SESSION['sisid']}";
            $menornivelcadastro = $db->pegaUm($sql);

            if ($menornivelcadastro && (($menornivelusuario - $menornivelcadastro) > 0)) {
                $habilitar_edicao = 'N';
            }
        }

		/*
		 * Verifica se o usuário deve ou não visializar todos os campos
		 *
		 */

		$data_atual = NULL;
		$permissao = true;
		if( $acao == 'U') {
			$permissao = false;
			$data_atual = date("Y-m-d H:i:s");
		}

		$usudatanascimento = formata_data($usuario->usudatanascimento);
		//-------------------------------------------------------------------------------

		if ( $_REQUEST['formulario']) {


            // Só quem pode alterar senha ou email (em caso de super-usuário) é a própria pessoa
            if($_SESSION['usucpforigem'] != $_REQUEST['usucpf'] || $_SESSION['usucpforigem'] != $_SESSION['usucpf']){

                $sql = "select * from seguranca.usuario where usucpf = '{$_REQUEST['usucpf']}'";
                $dadosUsuario = $db->pegaLinha($sql);

                $sql = "select count(*) as qtd
                        from seguranca.perfil p
                                inner join seguranca.perfilusuario pu on p.pflcod = pu.pflcod
                        where p.pflsuperuser = 't'
                        and usucpf = '{$_REQUEST['usucpf']}'";
                $qtdDestino = $db->pegaUm($sql);

                if ($qtdDestino) {
                    if ($_REQUEST['senha']) {
                        enviarAlteracaoSuperUser($dadosUsuario, 'senha');
                        echo "<script>
                                alert('Você não tem permissão para alterar a senha deste usuário.');
                                window.location = '?modulo=sistema/usuario/consusuario&acao=A';
                              </script>";
                        exit;
                    }

                    if ($dadosUsuario['usuemail'] != $_REQUEST['usuemail']) {
                        $dadosUsuario['usuemaildestino'] = $_REQUEST['usuemail'];
                        enviarAlteracaoSuperUser($dadosUsuario, 'email');
                        echo "<script>
                                alert('Você não tem permissão para alterar o email deste usuário.');
                                window.location = '?modulo=sistema/usuario/consusuario&acao=A';
                              </script>";
                        exit;
                    }
                }
            }


			//$tpocod_banco = $_REQUEST['tpocod'] ? (integer) $_REQUEST['tpocod'] : "null";

			//data de nascimento
			$dataBanco = formata_data_sql( $_REQUEST['usudatanascimento'] );
			$dataBanco = $dataBanco ? "'" . $dataBanco . "'" : "null";

			// Caso tenha entidade
			$entid = isset($_REQUEST['entid']) ? $_REQUEST['entid'] : 'null';
			$entid = $entid == 999999 ? 'null' : $entid;

			//arrumando problema de slashes excessivos causados pela diretiva magic codes do php
			$orgao = (str_replace( "\\", "",$_REQUEST['orgao']));
			$orgao = stripcslashes( $orgao );
			$orgao = str_replace( "'", "", $orgao );

			/*
			 * Integração com o SSD
			 * Atualizando os possiveis novos dados
			 * Desenvolvido por Alexandre Dourado
			 */
			if(AUTHSSD) {
				// Definindo o local dos certificados
				//define("PER_PATH", "../");

				include_once(APPRAIZ."www/connector.php");
				$SSDWs = new SSDWsUser($tmpDir, $clientCert, $privateKey, $privateKeyPassword, $trustedCaChain);
				// 	Efetuando a conexão com o servidor (produção/homologação)
				if ($GLOBALS['USE_PRODUCTION_SERVICES']) {
					$SSDWs->useProductionSSDServices();
				} else {
					$SSDWs->useHomologationSSDServices();
				}

				$SSD_tipo_pessoa = @utf8_encode("F");
				$SSD_nome = @utf8_encode($_POST["usunome"]);
				$SSD_cpf = @utf8_encode($usuario->usucpf);
				$SSD_data_nascimento = @utf8_encode(formata_data_sql($_POST["usudatanascimento"]));
				if($_POST['usuemailconfssd'] != $_POST["usuemail"]) {
					$SSD_email = @utf8_encode($_POST["usuemail"]);
				}
				$SSD_ddd_telefone = @utf8_encode($_POST["usufoneddd"]);
				$SSD_telefone = @utf8_encode($_POST["usufonenum"]);

				$userInfo = "$SSD_tipo_pessoa||$SSD_nome||$nome_mae||$SSD_cpf||$rg||$sigla_orgao_expedidor||$orgao_expedidor||$nis||" .
							"$SSD_data_nascimento||$codigo_municipio_naturalidade||$codigo_nacionalidade||$SSD_email||$email_alternativo||" .
							"$cep||$endereco||$sigla_uf_cep||$localidade||$bairro||$complemento||$endereco||$SSD_ddd_telefone||$SSD_telefone||" .
							"$ddd_telefone_alternativo||$telefone_alternativo||$ddd_celular||$celular||$instituicao_trabalho||$lotacao||" .
							"$justificativa||$cpf_responsavel||ssd";

				$resposta = $SSDWs->updateUser($userInfo);

				if($resposta != "true") {
					if ( $_SESSION['sisid'] == 147 ) { // obras 2
						echo "<script>
								alert('".addslashes($resposta["erro"])."');
								window.location = '?modulo=sistema2/usuario2/consusuario&acao=A';
							  </script>";
						exit;
					}
					else {
						echo "<script>
								alert('".addslashes($resposta["erro"])."');
								window.location = '?modulo=sistema/usuario/consusuario&acao=A';
							  </script>";
						exit;
					}
				}
			}
			/*
			 * FIM
			 * Integração com o SSD
			 * Atualizando os possiveis novos dados
			 * Desenvolvido por Alexandre Dourado
			 */


			if ($permissao) {
				$sql = sprintf("UPDATE seguranca.usuario SET
                                    usunome           = '".$_POST['usunome']."',
                                    usuemail          = '%s',
                                    usufoneddd        = '%s',
                                    usufonenum        = '%s',
                                    usufuncao         = '%s',
                                    carid         	  = '%s',
                                    entid             = %s,
                                    unicod            = '%s',
                                    regcod            = '%s',
                                    ungcod            = '%s',
                                    ususexo           = '%s',
                                    usudatanascimento =  %s,
                                    usunomeguerra     = '%s',
                                    muncod            = '%s',
                                    orgao             = '%s',
                                    tpocod            = '%s',
                                    usudataatualizacao = 'now()'
                                WHERE
                                    usucpf            = '%s'",
                                pg_escape_string($_REQUEST['usuemail']),
                                pg_escape_string($_REQUEST['usufoneddd']),
                                str_replace( "\\", "", substr( $_REQUEST['usufonenum'], 0, 10 ) ),
                                pg_escape_string(substr($_REQUEST['usufuncao'],0,100)),
                                pg_escape_string($_REQUEST['carid']),
                                pg_escape_string($entid),
                                pg_escape_string($_REQUEST['unicod']),
                                pg_escape_string($_REQUEST['regcod']),
                                pg_escape_string($_REQUEST['ungcod']),
                                pg_escape_string($_REQUEST['ususexo']),
                                $dataBanco,
                                pg_escape_string($_REQUEST['usunomeguerra']),
                                pg_escape_string($_REQUEST['muncod']),
                                str_replace("'","",$orgao),
                                pg_escape_string($_REQUEST['tpocod']),
                                pg_escape_string($usuario->usucpf));
			} else {
				$data_atual = $data_atual ? "'" . $data_atual . "'" : "null";
				// atualiza dados gerais do usuário
				$sql = sprintf("UPDATE seguranca.usuario SET
                                    usuemail = '%s',
                                    usufoneddd = '%s',
                                    usufonenum = '%s',
                                    usufuncao = '%s',
                                    carid = '%s',
                                    entid = '%s',
                                    unicod = '%s',
                                    regcod = '%s',
                                    ungcod = '%s',
                                    ususexo = '%s',
                                    usudatanascimento = %s,
                                    usunomeguerra = '%s',
                                    muncod = '%s',
                                    orgao = '%s',
                                    tpocod = '%s',
                                    usudataatualizacao = %s
                                WHERE
                                    usucpf = '%s'",
                                    pg_escape_string($_REQUEST['usuemail']),
                                    pg_escape_string($_REQUEST['usufoneddd']),
                                    str_replace( "\\", "", substr( $_REQUEST['usufonenum'], 0, 10 ) ),
                                    pg_escape_string($_REQUEST['usufuncao']),
                                    pg_escape_string($_REQUEST['carid']),
                                    pg_escape_string($_REQUEST['entid']),
                                    pg_escape_string($_REQUEST['unicod']),
                                    pg_escape_string($_REQUEST['regcod']),
                                    pg_escape_string($_REQUEST['ungcod']),
                                    pg_escape_string($_REQUEST['ususexo']),
                                	$dataBanco,
                                    pg_escape_string($_REQUEST['usunomeguerra']),
                                    pg_escape_string($_REQUEST['muncod']),
                                    str_replace("'", "",$orgao),
                                    pg_escape_string($_REQUEST['tpocod']),
                                    $data_atual,
                                    pg_escape_string($usuario->usucpf));

			}

			$db->executar($sql);

			// altera a senha do usuário com o valor padrão
			if ($_REQUEST['senha']) {

				/*
			 	 * Integração com o SSD
			 	 * Atualizando nova senha por padrão
			 	 * Desenvolvido por Alexandre Dourado
			 	 */

				if(AUTHSSD) {
					// 	Definindo o local dos certificados
					//define("PER_PATH", "../");

					include_once(APPRAIZ."www/connector.php");
					$SSDWs = new SSDWsUser($tmpDir, $clientCert, $privateKey, $privateKeyPassword, $trustedCaChain);
					// 	Efetuando a conexão com o servidor (produção/homologação)
					if ($GLOBALS['USE_PRODUCTION_SERVICES']) {
						$SSDWs->useProductionSSDServices();
					} else {
						$SSDWs->useHomologationSSDServices();
					}
					$cpfOrCnpj = $usuario->usucpf;
					$oldPassword = base64_encode(md5_decrypt_senha( $usuario->ususenha, '' ));
					$newPassword = base64_encode(SENHA_PADRAO);
					$resposta = $SSDWs->changeUserPasswordByCPFOrCNPJ($cpfOrCnpj, $oldPassword, $newPassword);
					if($resposta != "true") {
						if ( $_SESSION['sisid'] == 147 ) { // obras 2
							echo "<script>
									alert('".addslashes($resposta["erro"])."');
									window.location = '?modulo=sistema2/usuario2/consusuario&acao=A';
								  </script>";
							exit;
						}
						else {
							echo "<script>
									alert('".addslashes($resposta["erro"])."');
									window.location = '?modulo=sistema/usuario/consusuario&acao=A';
								  </script>";
							exit;
						}
					}
				}

				/*
				 * FIM
			 	 * Integração com o SSD
			 	 * Atualizando nova senha por padrão
			 	 * Desenvolvido por Alexandre Dourado
			 	 */


				$sql = sprintf("UPDATE
                                    seguranca.usuario
                                SET
                                    ususenha         = '%s',
                                    usuchaveativacao = 'f'
                                WHERE
                                    usucpf = '%s'",
                                md5_encrypt_senha(SENHA_PADRAO, ''),
                                $usucpf);

				$db->executar($sql);
			}

			// aplica as alterações de status nos sistemas
			foreach ( (array) $_REQUEST['status'] as $sisid => $suscod ) {
				$sql = sprintf(
				"SELECT us.* FROM seguranca.usuario_sistema us WHERE sisid = %d AND usucpf = '%s'",
				$sisid,
				$usuario->usucpf
				);
				$usuariosistema = (object) $db->pegaLinha( $sql );
				if ( !$usuariosistema->sisid ) {
					$sql = sprintf(
					"INSERT INTO seguranca.usuario_sistema ( sisid, usucpf ) VALUES ( %d, '%s' )",
					$sisid,
					$usuario->usucpf
					);
					$db->executar( $sql );
				}
				if ( $usuariosistema->suscod != $suscod ) {
					$justificativa = substr($_REQUEST['justificativa'][$sisid],0,3000);
					$db->alterar_status_usuario( $usuario->usucpf, $suscod, $justificativa, $sisid );
					$email_aprovacao = $usuariosistema->suscod == 'P' && $suscod == 'A' ? true : $email_aprovacao;
				}
			}

			// executa rotina para alteração do status geral no sistema
			if ($_SESSION['sisid'] == 4 /*in_array( 'geral', $configuracao )*/ ) {
				if ( $usuario->suscod != $_REQUEST['suscod'] ) {
					$db->alterar_status_usuario( $usuario->usucpf, $_REQUEST['suscod'], str_replace("'","''",$_REQUEST['htudsc']) );
					$email_aprovacao = $usuario->suscod == 'P' && $_REQUEST['suscod'] == 'A';
				}
			}

			// envia o email de confirmação caso a conta seja aprovada
			if ( $email_aprovacao) {
				$remetente = array("nome" => $_SESSION['usunome'],"email" => $_SESSION['usuemail']);
				$destinatario = $_REQUEST['usuemail'];
                $assunto = "Aprovação do Cadastro no ".SIGLA_SISTEMA;
				$conteudo = "
					<br/>
					<span style='background-color: red;'><b>Esta é uma mensagem gerada automaticamente pelo sistema. </b></span>
					<br/>
					<span style='background-color: red;'><b>Por favor, não responda. Pois, neste caso, a mesma será descartada.</b></span>
					<br/>
					";
				$conteudo .= sprintf(
				"%s %s<p>Sua conta está ativa. Sua Senha de acesso é: %s</p>",
				$_REQUEST['ususexo'] == 'M' ? 'Prezado Sr.' : 'Prezada Sra.',
				$_REQUEST['usunome'],
				md5_decrypt_senha( $usuario->ususenha, '' )
				);

				enviar_email( $remetente, $destinatario, $assunto, $conteudo );
			}


			// cadastra os perfils selecionados
			$_REQUEST['pflcod'] = is_array( $_REQUEST['pflcod'] ) ? $_REQUEST['pflcod'] : array();
			foreach ( $_REQUEST['pflcod'] as $sisid => $perfis ) {

                $sql = sprintf( "select * FROM seguranca.perfilusuario WHERE usucpf = '%s' AND pflcod IN ( SELECT p.pflcod FROM seguranca.perfil p WHERE p.sisid = %d )", $usucpf, $sisid );
                $perfisAntigos = $db->carregar($sql);
                $aPerfisAntigos = array();
                if($perfisAntigos && is_array($perfisAntigos)){
                    foreach ($perfisAntigos as $perfilAntigo){
                        $aPerfisAntigos[] = $perfilAntigo['pflcod'];
                    }
                }

				//deleta os perfis
				$sql = sprintf(
				"DELETE FROM seguranca.perfilusuario WHERE usucpf = '%s' AND pflcod IN ( SELECT p.pflcod FROM seguranca.perfil p WHERE p.sisid = %d )",
				$usucpf,
				$sisid
				);
				$db->executar( $sql );

				removerPerfisSlaves($usucpf, $sisid);

				/*** REGRA DO ENEM ***/
				if( $email_aprovacao && $_SESSION['sisid'] == '24' )
				{
					$sql = "SELECT
								fun.funid
							FROM
								entidade.entidade ent
							INNER JOIN
								entidade.funcaoentidade fue ON fue.entid = ent.entid
														   AND fue.fuestatus = 'A'
							INNER JOIN
								entidade.funcao fun ON fun.funid = fue.funid
												   AND fun.funstatus = 'A'
							WHERE
								ent.entstatus = 'A'
								AND ent.entnumcpfcnpj = '".$usucpf."'";
					$funid_enem = $db->carregarColuna($sql);

					if( $funid_enem )
					{
						foreach($funid_enem as $funid)
						{
							/*** executor ***/
							if( $funid == 83 && !in_array("518", $perfis) )
							{
								$sql = sprintf(
								"INSERT INTO seguranca.perfilusuario ( usucpf, pflcod ) VALUES ( '%s', 518 )",
								$usucpf
								);
								$db->executar( $sql );
							}
							/*** validador ***/
							if( $funid == 84 && !in_array("519", $perfis) )
							{
								$sql = sprintf(
								"INSERT INTO seguranca.perfilusuario ( usucpf, pflcod ) VALUES ( '%s', 519 )",
								$usucpf
								);
								$db->executar( $sql );
							}
							/*** certificador ***/
							if( $funid == 85 && !in_array(520, $perfis) )
							{
								$sql = sprintf(
								"INSERT INTO seguranca.perfilusuario ( usucpf, pflcod ) VALUES ( '%s', 520 )",
								$usucpf
								);
								$db->executar( $sql );
							}
						}
					}
				}

				foreach ( $perfis as $pflcod ) {
					if ( empty( $pflcod ) ) {
						continue;
					}

					//verifica se existe os perfis
					$sql = "select usucpf from seguranca.perfilusuario where usucpf = '{$usucpf}' and pflcod = {$pflcod}";
					$existePerfis = $db->pegaUm($sql);

                    if(!$existePerfis){
                        // inclui os perfis
						$sql = sprintf(
						"INSERT INTO seguranca.perfilusuario ( usucpf, pflcod ) VALUES ( '%s', %d )",
						$usucpf,
						$pflcod
						);
						$db->executar( $sql );

                        $sql = "select pflcod from perfil where pflcod = $pflcod and pflsuperuser = 't' ";
                        $perfilSuperUser = $db->pegaUm($sql);

                        if (IS_PRODUCAO && $perfilSuperUser && !in_array($perfilSuperUser, $aPerfisAntigos)){
                            enviarEmailSuperUser($usucpf, $pflcod);
                        }
					}

					inserirPerfisSlaves($usucpf, $pflcod);
					atualizarResponsabilidadesSlaves($usucpf, $pflcod);

				}


				//INATIVA AS RESPONSABILIDADES (ENTIDADE,ESTADOS,MUNICIPIOS)
				if($sisid != 4){ //4=Módulo de Segurança

					//pega o nome do sistema tabela
					$sql = sprintf("SELECT s.* FROM seguranca.sistema s WHERE sisid = %d",$sisid);
					$sistema = (object) $db->pegaLinha( $sql );
					$tabela_aux = $sistema->sisdiretorio;
					if($sisid == 14) $tabela_aux = "cte";

					if($tabela_aux == 'pde'){
						//10=Monitoramento do Plano de Desenvolvimento da Educação / 11=gerenciamento projetos
						//pde
//						$sisidaux = "10,11";
						$sisidaux = "select sisid from seguranca.sistema  where sisdiretorio = 'pde'";
					}
					elseif($sisid == 13 || $sisid == 14){
						//13=PAR - plano de metas / 14=Brasilpro
						//cte
						$sisidaux = "13,14";
					}
					elseif($sisid == 1 || $sisid == 6){
						//1="PPA-Monitoramento e Avaliação" / 6="Projetos Especiais"
						//monitora
						$sisidaux = "1,6";
					}
					elseif($sisid == 2 || $sisid == 5){
						//2="Programação Orçamentária" / 5="PPA-Elaboração e Revisão"
						//elabrev
						$sisidaux = "2,5";
					}
					else{
						$sisidaux = $sisid;
					}
					$sql = "SELECT
							   true
							  FROM
							   pg_tables
							  WHERE
							   schemaname = '".$tabela_aux."' AND
							   tablename  = 'usuarioresponsabilidade'";
					if ($db->pegaUm($sql) == 't'){
						$sqlr = "UPDATE ".$tabela_aux.".usuarioresponsabilidade SET rpustatus='I' WHERE usucpf='".$usucpf."' and rpustatus='A' and pflcod not in(select pu.pflcod from seguranca.perfilusuario pu, seguranca.perfil p where pu.pflcod=p.pflcod and p.sisid in (".$sisidaux.") and pu.usucpf ='".$usucpf."')";
						$db->executar($sqlr);
					}
				}
				//FIM INATIVA
			}

			$db->commit();
			$parametros = '&usucpf='. $_REQUEST['usucpf'];
			die('<script>
					alert(\'Operação realizada com sucesso!\');
					location.href = window.location;
				 </script>');
		}

		include_once( APPRAIZ . "www/includes/webservice/cpf.php" );
		include APPRAIZ ."includes/cabecalho.inc";
		print '<br>';


		monta_titulo( $titulo_pagina, '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.' );



//ver ( '2 ------ ' . time() );

		?>




<body>
<?php if('S' == $habilitar_edicao) { ?>
    <form method="post" name="formulario">
<?php } ?>
    <input type="hidden" name="formulario" value="1" />
    <input type="hidden" name="ssd" value="<? echo (($_REQUEST['ssd'])?"true":""); ?>" />
    
    <link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
    
    <script type="text/javascript" src="../includes/prototype.js"></script>
    
    <script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
    <script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
    
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<?php
        if ($permissao) { // : 270
    ?>
	<tr id="tr_cpf">
		<td align='right' class="SubTituloDireita">CPF:</td>
		<td>
			<?=campo_texto( 'cpf', 'S', $habilitar_edicao, '', 19, 14, '###.###.###-##', '', '', '', '', 'id="cpf"', '', '', 'mostraNomeReceita(this.value)' ); ?>
			<?
				$sql = "SELECT
							count(*)
						FROM
							siape.tb_siape_cadastro_servidor
						WHERE
							nu_cpf = '".str_replace(array(".","-"),"",$cpf)."'";

				$qtd = $db->pegaUm($sql);

				if((integer)$qtd > 0) {
			 ?>
				&nbsp;
				<a style="cursor: pointer;" onclick="visualizaDadosSiape('<?=str_replace(array(".","-"),"",$cpf)?>');">Consultar dados do SIAPE</a>
			<? } ?>
		</td>
	</tr>
	<tr id="tr_usunome">
		<td align='right' class="SubTituloDireita">Nome:</td>
		<td><?= campo_texto( 'usunome', 'S', $habilitar_edicao, '', 50, 50, '', '', '', '', '', 'id="usunome"' ); ?></td>
	</tr>
	<?php
        } // if ($permissao): 259
    ?>

	<tr>
		<td align='right' class="SubTituloDireita">Apelido:</td>
		<td><?= campo_texto( 'usunomeguerra', 'S', $habilitar_edicao, '', 20, 20, '', '' ); ?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Sexo:</td>
		<td><input id="sexo_masculino" type="radio" name="ususexo" value="M" <? if($habilitar_edicao == 'N' ) echo("disabled='disabled'"); ?>
		<?=($ususexo=='M'?"CHECKED":"")?>
		<?= $cpf_cadastrado ? 'disabled="disabled"' : '' ?> /> <label
			for="sexo_masculino">Masculino</label> <input id="sexo_feminino"
			type="radio" name="ususexo" value="F"
			<?=($ususexo=='F'?"CHECKED":"")?>
			<?= $cpf_cadastrado ? 'disabled="disabled"' : '' ?> /> <label
			for="sexo_feminino">Feminino</label> <?= obrigatorio(); ?></td>
	</tr>

	<tr>
	<?
		$obrigatorio = "N";
		if( ! $permissao){
			$obrigatorio = "S";
		}
	?>
		<td align='right' class="SubTituloDireita"> Data de Nascimento <b>(dd/mm/aaaa)</b>: </td>
		<td>
                    <?PHP 
                    //campo_texto( 'usudatanascimento', $obrigatorio, $habilitar_edicao, '', 20, 20, '##/##/####', '' ); 
                    echo campo_data2('usudatanascimento', $obrigatorio, $habilitar_edicao, 'Data de Nascimento', '##/##/####', '', '', '', '', '', 'usudatanascimento');
                    ?>
                </td>
	</tr>

	<tr>
		<td align='right' class="SubTituloDireita">Unidade Federal:</td>
		<td><?php
		$sql = "SELECT regcod AS codigo, regcod||' - '||descricaouf AS descricao FROM uf WHERE codigoibgeuf IS NOT NULL ORDER BY 2";
		$db->monta_combo( 'regcod', $sql, $habilitar_edicao, '&nbsp;', 'listar_municipios', '', '', '', 'S', 'regcod');
		?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Município:</td>
		<td>
		<div id="muncod_on" style="display: none;">
		<select id="muncod"  <? if($habilitar_edicao == 'N' ) echo("disabled='disabled'"); ?>
			name="muncod" onchange="" class="CampoEstilo"></select> <?= obrigatorio(); ?>
		</div>
		<div id="muncod_off" style="color: #909090;">A Unidade Federal
		selecionada não possui municípios.</div>
		</td>
	</tr>
	<tr>
		<td align='right' class="subtitulodireita">Tipo do Órgão:</td>
		<td>
			<?php

			$tpocod = $usuario->tpocod;

			$sql = "SELECT
						tpocod as codigo,
						tpodsc as descricao
					FROM
						public.tipoorgao
					WHERE
						tpostatus='A'";


			$db->monta_combo("tpocod",$sql,'S','','ajax_carrega_orgao','','','170','S','tpocod');

			?>
		</td>
	</tr>
	<tr>
		<td align='right' class="subtitulodireita">Órgão:</td>
		<td>
			<span id="spanOrgao">
			 	<?php
			 		$entid = $usuario->entid;

					if ( ($tpocod == 2 || $tpocod == 3) && !empty($usuario->orgao) ){
			 			$entid = 999999;
			 		}
			 		carrega_orgao($editavel, $usuario->usucpf);
			 	?>
			</span>
		</td>
	</tr>
	<tr bgcolor="#F2F2F2">
		<td align='right' class="subtitulodireita">Unidade Orçamentária:</td>
		<td>
			<span id="unidade">
				<?php
					$unicod = $usuario->unicod;

					if ( $entid == 'null' ){
						$entid = '';
					}
					carrega_unidade($entid, $editavel, $usuario->usucpf);
				?>
			</span>
		</td>
	</tr>
	<tr bgcolor="#F2F2F2">
		<td align='right' class="subtitulodireita">Unidade Gestora:</td>
		<td>
			<span id="unidade_gestora">
				<?php
					carrega_unidade_gestora($unicod, $editavel, $usuario->usucpf);
				?>
			</span>
		</td>
	</tr>
	<tr>
		<td align='right' class="subtitulodireita">Telefone (DDD) + Telefone:</td>
		<td><?= campo_texto( 'usufoneddd', 'S', $habilitar_edicao, '', 3, 2, '##', '' ); ?>
		<?= campo_texto( 'usufonenum', 'S', $habilitar_edicao, '', 18, 15, '###-####|####-####|#####-####', '' ); ?>
		</td>
	</tr>
	<tr>
		<td align='right' class="subtitulodireita">E-Mail:</td>
		<td><?= campo_texto( 'usuemail', $habilitar_edicao, $habilitar_edicao, '', 50, 100, '', '' ); ?><input type="hidden" name="usuemailconfssd" value="<? echo $usuemail; ?>"></td>
	</tr>
	<tr>
		<td align='right' class="subtitulodireita">Função/Cargo:</td>
		<td>

			<?php
				$sql = "select carid as codigo, cardsc as descricao from public.cargo";
				$db->monta_combo( "carid", $sql, 'S', 'Selecione', 'alternarExibicaoCargo', '', '', '', 'N', "carid", '' );
			?>
			<?= campo_texto( 'usufuncao', $habilitar_edicao, $habilitar_edicao, '', 50, 100, '','', '', '', '', 'id="usufuncao" style="display: none;"' ); ?>
			<a href="javascript: alternarExibicaoCargo( 'exibirOpcoes' )" id="linkVoltar" style="display: none;" > Exibir Opções</a>
		</td>
	</tr>



	<?php
if( ($permissao == true) && ($habilitar_edicao == 'S') ) {
	if(!AUTHSSD) {
	?>
	<tr id="tr_senha">
		<td align='right' class="subtitulodireita">Senha:</td>
		<td><input id="senha" type="checkbox" name="senha" /> <label
			for="senha">Alterar a senha do usuário para a senha padrão: <b><?php echo SENHA_PADRAO; ?></b>.</label>
		</td>
	</tr>
	<?php
	}
	if( $_SESSION['sisid'] == 4/*in_array( 'geral', $configuracao )*/ ): ?>
	<tr id="tr_status">
		<td align='right' class="SubTituloDireita">Status Geral:</td>
		<td><input id="status_ativo" type="radio" name="suscod" value="A"
			onchange="alterar_status_geral();"
			<?= $suscod == 'A' ? 'checked="checked"' : "" ?> /> <label
			for="status_ativo">Ativo</label> <input id="status_pendente"
			type="radio" name="suscod" value="P"
			onchange="alterar_status_geral();"
			<?= $suscod == 'P' ? 'checked="checked"' : "" ?> /> <label
			for="status_pendente">Pendente</label> <input id="status_bloqueado"
			type="radio" name="suscod" value="B"
			onchange="alterar_status_geral();"
			<?= $suscod == 'B' ? 'checked="checked"' : "" ?> /> <label
			for="status_bloqueado">Bloqueado</label>
		&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <a
			href="javascript: exibir_ocultar_historico('historico_geral');"><img
			src="/imagens/mais.gif" style="border: 0" /> histórico</a>
		<div id="historico_geral" style="width: 500px; display: none">
		<p><?php
		$cabecalho = array(
								"Data",
								"Status",
								"Descrição",
								"CPF",
		);
		$sql = sprintf(
								"SELECT to_char( hu.htudata, 'dd/mm/YYYY' ) as data, hu.suscod, hu.htudsc, hu.usucpfadm FROM seguranca.historicousuario hu WHERE usucpf = '%s' AND sisid IS NULL ORDER BY hu.htudata DESC",
		$usucpf
		);
		$db->monta_lista_simples( $sql, $cabecalho, 25, 0 );
		?></p>
		</div>
		</td>
	</tr>
	<tr id="tr_justificativa">
		<td align='right' class="SubTituloDireita">Justificativa:</td>
		<td>
		<div id="justificativa_on" style="display: none;"><?= campo_textarea( 'htudsc', 'N', 'S', '', 100, 3, '3000' ); ?>
		</div>
		<div id="justificativa_off" style="display: block; color: #909090;">
		Status não alterado.</div>
		</td>
	</tr>
	<?php else: ?>
	<tr id="tr_status_geral">
		<td align='right' class="SubTituloDireita">Status Geral:</td>
		<td><?= $status[$suscod] ?></td>
	</tr>
	<?php endif; ?>

	<?php
	//ver ( '3 ------ ' . time() );



	$sistemas = array();

	if ($_SESSION['sisid'] == 4){
		$sql 	= "SELECT
					sisid
				   FROM
				    seguranca.sistema
				   WHERE
				    sisstatus = 'A' AND
				    sisid != 999";
		$dados = $db->carregar($sql);

		foreach ($dados as $sisidArr){
			$sisid = $sisidArr['sisid'];

			$sql = sprintf("SELECT
							  s.sisid,
							  s.sisdsc,
							  s.sisabrev,
							  s.sisdiretorio
							FROM
							 seguranca.sistema s
							WHERE
							 sisid = %d",
						    $sisid);
			$sistema = (object) $db->pegaLinha( $sql );
//if($sistema->sisid == 179) ver($sistema,d);
			$sql = sprintf("SELECT
							 us.suscod ,
							 us.pflcod ,
							 p.pfldsc
							FROM
							 seguranca.usuario_sistema us LEFT JOIN seguranca.perfil p USING ( pflcod )
							WHERE
							 us.sisid = %d AND
							 usucpf = '%s'",
							$sistema->sisid,
							$usucpf);
			$usuariosistema = (object) $db->pegaLinha( $sql );

			$sistema->usuariosistema = $usuariosistema;
			$sistemas[] = $sistema;


			//----------- verificando se o modulo possui as tabelas necessarias e alguma porposta para que seja exibido a lista de propostas
			$sistema->sisdiretorio = $sistema->sisid == 14 ? 'cte' : $sistema->sisdiretorio;
			$sql = sprintf("SELECT CASE
								        WHEN
									 (SELECT
									   true
									  FROM
									   pg_tables
									  WHERE
									   schemaname='%s' AND
									   tablename = 'tiporesponsabilidade')
									THEN true
									WHEN
									  (SELECT
									   true
									  FROM
									   pg_tables
									  WHERE
									   schemaname='%s' AND
									   tablename = 'tprperfil')
									THEN true
									ELSE false
									END;",
							strtolower($sistema->sisdiretorio),
							strtolower($sistema->sisdiretorio));

				$existTable = $db->pegaUm($sql);


				if($existTable == 't'){

					$sql_urp = "select urpcampo, pflcod from seguranca.usuariorespproposta where sisid = '".$sisid."' and usucpf = '".$usucpf."'";
		     		$urp = $db->pegaLinha($sql_urp);
		     		if($urp){

		     			//$sql_perfil = "select pfldsc from seguranca.perfil where pflcod = ".$urp['pflcod']."";

			     		//$dados_perfil  = $db->pegaUm($sql_perfil);

			     		$sql_tpresp = "select tprdsc, tprtabela, tprcampo, tprcampodsc from ".$sistema->sisdiretorio.".tiporesponsabilidade where tprcampo = '".$urp['urpcampo']."'";
			     		$dados_tpresp = $db->pegaLinha($sql_tpresp);

			     		$sql_propostas = "select distinct urpcampoid, urpcampo
											from seguranca.usuariorespproposta as urp
											where sisid = '".$sisid."' and usucpf = '".$usucpf."'";
			     		$dados_propostas = $db->carregar($sql_propostas);

			     		echo("<tr>
								<td class='subtitulodireita' style='text-align: left;font-weight: bold;' colspan='2'>Proposto</td>
							</tr>
							<tr>");


			?>

						<tr>
							<td align='right' class="SubTituloDireita">Perfil: </td>
							<td><?= $usuariosistema->pfldsc ?>
							</td>
						</tr>

						<tr>
							<td align='right' class="SubTituloDireita"><? echo($dados_tpresp['tprdsc'].": "); ?></td>
							<td>
							<table border="0" cellpadding="2" cellspacing="0" width="500" class="listagem" bgcolor="#fefefe">
								<tr>

									<td width="100%" bgcolor="#e9e9e9" align="left">Código - <?=$dados_tpresp['tprdsc'] ?></td>
							     </tr>
							     	<?
							     		$count = 0;
										foreach ($dados_propostas as $dado) {

											 $sql_proposta = "select distinct
											 	".$dados_tpresp['tprcampodsc']." as descricao
												from ".$dados_tpresp['tprtabela']."
												where ".$dados_tpresp['tprcampo']." = '".$dado['urpcampoid']."'";

											$dados_ptoposta = $db->pegaLinha($sql_proposta);

											if ($count %2) $backcolor = "bgcolor=\"#F7F7F7\"";
											else $backcolor = "";
											$count ++;
											echo("<tr ".$backcolor."><td>".$dado['urpcampoid']." - ".$dados_ptoposta['descricao']."</td></tr>");
										}
							     	?>
							</table>
							</td>
						</tr>

			  <?      }
				}
			   ?>
			<tr>
			<td class="subtitulodireita" style="text-align: left;font-weight: bold;" colspan="2">Atribuido</td>
			</tr>

			<tr>
				<td align='right' class="SubTituloDireita">Sistema:</td>
				<td><h5><b><?= $sistema->sisabrev ?></b></h5></td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Status:</td>
				<td>
					<input id="status_ativo_<?= $sistema->sisid ?>" type="radio" name="status[<?= $sistema->sisid ?>]" value="A" onchange="alterar_status_sistema( <?= $sistema->sisid ?> );" <?= $usuariosistema->suscod == 'A' ? 'checked="checked"' : "" ?>/>
					<label for="status_ativo_<?= $sistema->sisid ?>">Ativo</label>
					<input id="status_pendente_<?= $sistema->sisid ?>" type="radio" name="status[<?= $sistema->sisid ?>]" value="P" onchange="alterar_status_sistema( <?= $sistema->sisid ?> );" <?= $usuariosistema->suscod == 'P' ? 'checked="checked"' : "" ?>/>
					<label for="status_pendente_<?= $sistema->sisid ?>">Pendente</label>
					<input id="status_bloqueado_<?= $sistema->sisid ?>" type="radio" name="status[<?= $sistema->sisid ?>]" value="B" onchange="alterar_status_sistema( <?= $sistema->sisid ?> );" <?= $usuariosistema->suscod == 'B' ? 'checked="checked"' : "" ?>/>
					<label for="status_bloqueado_<?= $sistema->sisid ?>">Bloqueado</label>
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<a href="javascript: exibir_ocultar_historico('historico_<?= $sistema->sisid ?>');"><img src="/imagens/mais.gif" style="border: 0"/> histórico</a>
					<div id="historico_<?= $sistema->sisid ?>" style="width: 500px; display: none">
						<p>
						<?php
							$cabecalho = array(
								"Data",
								"Status",
								"Descrição",
								"CPF"
							);
							$sql = sprintf(
								"SELECT
								  to_char( hu.htudata, 'dd/mm/YYYY' ) as data,
								  hu.suscod,
								  hu.htudsc,
								  hu.usucpfadm
								 FROM
								  seguranca.historicousuario hu
								 WHERE
								  usucpf = '%s' AND
								  sisid = %d
								 ORDER BY
								  hu.htudata DESC",
								$usucpf,
								$sistema->sisid
							);
							$db->monta_lista_simples( $sql, $cabecalho, 25, 0 );
						?>
						</p>
					</div>
				</td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Justificativa:</td>
				<td>
					<div id="justificativa_on_<?= $sistema->sisid ?>" style="display: none;">
						<?= campo_textarea( 'justificativa['. $sistema->sisid .']', 'N', 'S', '', 100, 3, '3000' ); ?>
					</div>
					<div id="justificativa_off_<?= $sistema->sisid ?>" style="display: block; color:#909090;">
						Status não alterado.
					</div>
				</td>
			</tr>
			<?php if ( $usuariosistema->pflcod &&  ( ! $urp )): ?>
				<tr>
					<td align='right' class="SubTituloDireita">Perfil Desejado:</td>
					<td><?= $usuariosistema->pfldsc ?></td>
				</tr>
			<?php endif; ?>
			<tr>
				<td align='right' class="SubTituloDireita">Perfil:</td>
				<td>
					<?php
					$sql = sprintf("select
									 p.pflnivel
									from
									 seguranca.perfil p inner join seguranca.perfilusuario pu
									 	on pu.pflcod=p.pflcod and
									 	pu.usucpf='%s' and
									 	p.sisid=%d
									order by
									 p.pflnivel",
							$_SESSION['usucpf'],
							$sistema->sisid);
					$nivel = $db->pegaUm( $sql );
					$nivel = 0;
					$sql_perfil = sprintf("select
											distinct p.pflcod as codigo,
											p.pfldsc as descricao
										   from
										    seguranca.perfil p left join seguranca.perfilusuario pu on pu.pflcod=p.pflcod
										   where
										    p.pflstatus='A' and
										  --  p.pflnivel > %d and
										    p.sisid=%d
										   order by
										    descricao",
								$nivel,
								$sistema->sisid);
					$sql = sprintf("select
									 distinct p.pflcod as codigo,
									 p.pfldsc as descricao
									from
									 seguranca.perfilusuario pu inner join seguranca.perfil p on p.pflcod=pu.pflcod
									where
									 p.pflstatus = 'A' and
									 p.sisid=%d and
									 pu.usucpf='%s'
									order by
									 descricao",
							$sistema->sisid,
							$usucpf);
					$nome = 'pflcod[' . $sistema->sisid . ']';
					$$nome = $db->carregar( $sql );
					combo_popup( 'pflcod['. $sistema->sisid .']', $sql_perfil, 'Selecione o(s) Perfil(s)', '360x460' );
					?>
				</td>
			</tr>
			<?php
				$sistema->sisdiretorio = $sistema->sisid == 14 ? 'cte' : $sistema->sisdiretorio;
				$sql = sprintf("SELECT CASE
									        WHEN
										 (SELECT
										   true
										  FROM
										   pg_tables
										  WHERE
										   schemaname='%s' AND
										   tablename = 'tiporesponsabilidade')
										THEN true
										WHEN
										  (SELECT
										   true
										  FROM
										   pg_tables
										  WHERE
										   schemaname='%s' AND
										   tablename = 'tprperfil')
										THEN true
										ELSE false
										END;",
								$sistema->sisdiretorio,
								$sistema->sisdiretorio);
								//	echo "<pre>".$sql;
				$existTable = $db->pegaUm($sql);
				unset($sql);
			if ($existTable === 't' && $sistema->sisdiretorio != 'pde'):
				$sql = "SELECT
						  	tprsigla,
							tprurl
						FROM
						    ".$sistema->sisdiretorio.".tiporesponsabilidade
						WHERE
						 	tprsnvisivelperfil = 't'
						ORDER BY
						 	tprdsc";
				$responsabilidades = (array) $db->carregar($sql);
				$sqlPerfisUsuario = "SELECT
									  p.pflcod, p.pfldsc
									 FROM
									  seguranca.perfil p
									  INNER JOIN seguranca.perfilusuario pu ON pu.pflcod = p.pflcod AND pu.usucpf = '%s' and sisid=".$sistema->sisid."
									 WHERE
									  p.pflstatus='A'
									 ORDER BY
									  p.pfldsc";
				$query = sprintf($sqlPerfisUsuario, $usucpf);
				$perfisUsuario = $db->carregar($query);

				$script .= gerFuncResp($sistema->sisid, $sistema->sisdiretorio, $usucpf, $responsabilidades);

			//ver ( '4 ------ ' . time() );

			?>
			<?php if( $perfisUsuario ): ?>
				<tr>
					<td align='right' class="SubTituloDireita">Associação de Perfil:</td>
					<td>
						<table border="0" cellpadding="2" cellspacing="0" width="500" class="listagem" bgcolor="#fefefe">
							<tr>
								<td width="12" rowspan="2" bgcolor="#e9e9e9" align="center">&nbsp;</td>
								<td rowspan="2" align="left" bgcolor="#e9e9e9" align="center">Descrição</td>
								<td align="center" colspan="<?=@count($responsabilidades)?>" bgcolor="#e9e9e9" align="center" style="border-bottom: 1px solid #bbbbbb">Responsabilidades</td>
							</tr>
							<tr>
								<?php foreach( $responsabilidades as $responsabilidade ): ?>
									<td align="center" bgcolor="#e9e9e9" align="center"><?= $responsabilidade["tprdsc"] ?></td>
								<? endforeach; ?>
							</tr>
							<?php foreach( $perfisUsuario as $perfil ): ?>
								<?php
									$marcado = $i++ % 2 ? '#F7F7F7' : '';
									$sqlResponsabilidadesPerfil = "SELECT
																	p.*, tr.tprdsc, tr.tprsigla
																   FROM
																    (SELECT
																      *
																     FROM
																      ".$sistema->sisdiretorio.".tprperfil
																     WHERE
																      pflcod = '%s') p
																    RIGHT JOIN ".$sistema->sisdiretorio.".tiporesponsabilidade tr ON p.tprcod = tr.tprcod
																   WHERE
																    tprsnvisivelperfil = TRUE
																   ORDER BY
																    tr.tprdsc";
									$query = sprintf($sqlResponsabilidadesPerfil, $perfil["pflcod"]);

									$responsabilidadesPerfil = (array) $db->carregar($query);

									// Esconde a imagem + para perfis sem responsabilidades
									$mostraMais = false;

									foreach ( $responsabilidadesPerfil as $resPerfil ) {
										if ( (boolean) $resPerfil["tprcod"] ){
											$mostraMais = true;
											break;
										}
									}
								?>
								<tr bgcolor="<?=$marcado?>">
									<td style="color: #003c7b">
										<?php if ($mostraMais): ?>
											<!-- <a href="Javascript:abreconteudo('../geral/cadastro_usuario_cte_responsabilidades.php?usucpf=<?=$usucpf?>&pflcod=<?=$perfil["pflcod"]?>','<?=$perfil["pflcod"]?>')">-->
											<a href="Javascript:abreconteudo('../<?=$sistema->sisdiretorio; ?>/geral/cadastro_responsabilidades.php?usucpf=<?=$usucpf?>&pflcod=<?=$perfil["pflcod"]?>','<?=$perfil["pflcod"]?>')">
												<img src="../imagens/mais.gif" name="+" border="0" id="img<?=$perfil["pflcod"]?>"/>
											</a>
										<?php endif; ?>
									</td>
									<td><?=$perfil["pfldsc"]?></td>
									<?php foreach( $responsabilidadesPerfil as $resPerfil ): ?>
										<td align="center">
											<?php if ( (boolean) $resPerfil["tprcod"] ): ?>
												<input type="button" name="btnAbrirResp<?=$perfil["pflcod"]?>" value="Atribuir" onclick="popresp_<?= $sistema->sisid ?>(<?=$perfil["pflcod"]?>, '<?=$resPerfil["tprsigla"]?>')">
											<?php else: ?>
												-
											<?php endif; ?>
										</td>
									<?php endforeach; ?>
								</tr>
								<tr bgcolor="<?=$marcado?>">
									<td colspan="10" id="td<?=$perfil["pflcod"]?>"></td>
								</tr>
							<?php endforeach; ?>
						</table>
					</td>
				</tr>
			<?php
				endif;
			endif;
		}
	}else{
		$sisid = $_SESSION['sisid'];
						$sql = sprintf("SELECT
										 s.*
										FROM
										 seguranca.sistema s
										WHERE
										 sisid = %d",
								$sisid);
				$sistema = (object) $db->pegaLinha( $sql );

				$sql = sprintf("SELECT
								 us.*,
								 p.*
								FROM
								 seguranca.usuario_sistema us
								 LEFT JOIN seguranca.perfil  p USING ( pflcod )
								WHERE
								 us.sisid = %d AND
								 usucpf = '%s'",
						$sistema->sisid,
						$usucpf);
				$usuariosistema = (object) $db->pegaLinha( $sql );

				$sistema->usuariosistema = $usuariosistema;
				$sistemas[] = $sistema;

				//----------- verificando se o modulo possui as tabelas necessarias e alguma porposta para que seja exibido a lista de propostas
				$sistema->sisdiretorio = $sistema->sisid == 14 ? 'cte' : $sistema->sisdiretorio;
				$sql = sprintf("SELECT CASE
									        WHEN
										 (SELECT
										   true
										  FROM
										   pg_tables
										  WHERE
										   schemaname='%s' AND
										   tablename = 'tiporesponsabilidade')
										THEN true
										WHEN
										  (SELECT
										   true
										  FROM
										   pg_tables
										  WHERE
										   schemaname='%s' AND
										   tablename = 'tprperfil')
										THEN true
										ELSE false
										END;",
								strtolower($sistema->sisdiretorio),
								strtolower($sistema->sisdiretorio));

				$existTable = $db->pegaUm($sql);

				if($existTable == 't'){

					$sql_urp = "select urpcampo, pflcod from seguranca.usuariorespproposta where sisid = '".$sisid."' and usucpf = '".$usucpf."'";
		     		$urp = $db->pegaLinha($sql_urp);

		     		if($urp){

		     			//$sql_perfil = "select pfldsc from seguranca.perfil where pflcod = ".$urp['pflcod']."";

			     		//$dados_perfil  = $db->pegaUm($sql_perfil);

			     		$sql_tpresp = "select tprdsc, tprtabela, tprcampo, tprcampodsc from ".$sistema->sisdiretorio.".tiporesponsabilidade where tprcampo = '".$urp['urpcampo']."'";

			     		$dados_tpresp = $db->pegaLinha($sql_tpresp);

			     		$sql_propostas = "select distinct urpcampoid, urpcampo
											from seguranca.usuariorespproposta as urp
											where sisid = '".$sisid."' and usucpf = '".$usucpf."'";
			     		$dados_propostas = $db->carregar($sql_propostas);


						echo("<tr>
								<td class='subtitulodireita' style='text-align: left;font-weight: bold;' colspan='2'>Proposto</td>
							</tr>
							<tr>");


			?>
						<tr>
							<td align='right' class="SubTituloDireita">Perfil: </td>
							<td><?= $usuariosistema->pfldsc ?>
							</td>
						</tr>

						<tr>
							<td align='right' class="SubTituloDireita"><? echo($dados_tpresp['tprdsc'].": "); ?></td>
							<td>
							<table border="0" cellpadding="2" cellspacing="0" width="500" class="listagem" bgcolor="#fefefe">
								<tr>

									<td width="100%" bgcolor="#e9e9e9" align="left">Código - <?=$dados_tpresp['tprdsc'] ?></td>
							     </tr>
							     	<?
						     		$count = 0;
									foreach ($dados_propostas as $dado)
									{
										if( $dado['urpcampoid'] != "" )
										{
											if($dados_tpresp['tprtabela'] == 'monitora.acao'){
											 	$sql_proposta = "select distinct
											 		".$dados_tpresp['tprcampodsc']." as descricao
													from ".$dados_tpresp['tprtabela']."
													where ".$dados_tpresp['tprcampo']." = '".$dado['urpcampoid']."' AND prgano = '".$_SESSION['exercicio']."'";
											}else{
												$sql_proposta = "select distinct
											 		".$dados_tpresp['tprcampodsc']." as descricao
													from ".$dados_tpresp['tprtabela']."
													where ".$dados_tpresp['tprcampo']." = '".$dado['urpcampoid']."'";
											}
											//dbg($sql_proposta);
											$dados_ptoposta = $db->pegaLinha($sql_proposta);

											if ($count %2) $backcolor = "bgcolor=\"#F7F7F7\"";
											else $backcolor = "";
											$count ++;
											echo("<tr ".$backcolor."><td>".$dado['urpcampoid']." - ".$dados_ptoposta['descricao']."</td></tr>");
										}
									}
							     	?>
							</table>
							</td>
						</tr>

			  <?      }
				}
			   ?>
			<tr>
			<td class="subtitulodireita" style="text-align: left;font-weight: bold;" colspan="2">Atribuido</td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Sistema:</td>
				<td><b><?= $sistema->sisdsc ?></b></td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Status:</td>
				<td>
					<input id="status_ativo_<?= $sistema->sisid ?>" type="radio" name="status[<?= $sistema->sisid ?>]" value="A" onchange="alterar_status_sistema( <?= $sistema->sisid ?> );" <?= $usuariosistema->suscod == 'A' ? 'checked="checked"' : "" ?>/>
					<label for="status_ativo_<?= $sistema->sisid ?>">Ativo</label>
					<input id="status_pendente_<?= $sistema->sisid ?>" type="radio" name="status[<?= $sistema->sisid ?>]" value="P" onchange="alterar_status_sistema( <?= $sistema->sisid ?> );" <?= $usuariosistema->suscod == 'P' ? 'checked="checked"' : "" ?>/>
					<label for="status_pendente_<?= $sistema->sisid ?>">Pendente</label>
					<input id="status_bloqueado_<?= $sistema->sisid ?>" type="radio" name="status[<?= $sistema->sisid ?>]" value="B" onchange="alterar_status_sistema( <?= $sistema->sisid ?> );" <?= $usuariosistema->suscod == 'B' ? 'checked="checked"' : "" ?>/>
					<label for="status_bloqueado_<?= $sistema->sisid ?>">Bloqueado</label>
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<a href="javascript: exibir_ocultar_historico('historico_<?= $sistema->sisid ?>');"><img src="/imagens/mais.gif" style="border: 0"/> histórico</a>
					<div id="historico_<?= $sistema->sisid ?>" style="width: 500px; display: none">
						<p>
						<?php
							$cabecalho = array(
								"Data",
								"Status",
								"Descrição",
								"CPF"
							);
							$sql = sprintf("SELECT
											 to_char( hu.htudata, 'dd/mm/YYYY' ) as data,
											 hu.suscod,
											 hu.htudsc,
											 hu.usucpfadm
										    FROM
										     seguranca.historicousuario hu
										    WHERE
										     usucpf = '%s' AND
										     sisid = %d
										    ORDER BY
										     hu.htudata
										    DESC",
								$usucpf,
								$sistema->sisid
							);
							$db->monta_lista_simples( $sql, $cabecalho, 25, 0 );
						?>
						</p>
					</div>
				</td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Justificativa:</td>
				<td>
					<div id="justificativa_on_<?= $sistema->sisid ?>" style="display: none;">
						<?= campo_textarea( 'justificativa['. $sistema->sisid .']', 'N', 'S', '', 100, 3, '3000' ); ?>
					</div>
					<div id="justificativa_off_<?= $sistema->sisid ?>" style="display: block; color:#909090;">
						Status não alterado.
					</div>
				</td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Enviar mensagem:</td>
				<td>
					<img border="0" onclick="enviarEmail_usu('../geral/envia_email_usuario.php?usuID=',673,515,'<?php echo str_replace(array(".","-"),"",$cpf); ?>')" title=" Enviar e-mail " src="../imagens/email.gif" style="cursor:pointer;" />
					<span style="color:#909090;">Clique para preencher os dados do email</span>
				</td>
			</tr>
			<?php if ( $usuariosistema->pflcod &&  ( ! $urp )): ?>
				<tr>
					<td align='right' class="SubTituloDireita">Perfil Desejado:</td>
					<td><?= $usuariosistema->pfldsc ?></td>
				</tr>
			<?php endif; ?>
			<tr>
				<td align='right' class="SubTituloDireita">Perfil:</td>
				<td>
					<?php

					//Se não for Super Usuário, deve obedecer a tabela 'seguranca.perfilpermissao'

					$perfilSuperUser = $db->testa_superuser(); //testa se o usuário é super usuário
					if ( !$perfilSuperUser ){
						$arrPerfil = pegaPerfilgeral(); //pega todos os perfis do usuário
						$arrPerfil = retornaPflcodFilhos($arrPerfil); //retornar todos os perfis associados (seguranca.perfilpermissao)
					}


					/*** Sistema ESCOLA ***/
					if( $sistema->sisid == 34 )
					{
						if( $db->testa_superuser() )
						{
							$sql_perfil = sprintf("select
													distinct p.pflcod as codigo,
													p.pfldsc as descricao
												   from
												    seguranca.perfil p
												    left join seguranca.perfilusuario pu on pu.pflcod=p.pflcod
												   where
												    p.pflstatus='A' and
												    p.sisid=%d
												   order by
												    descricao",
											$sistema->sisid);
						}
						else
						{
							$arrModid = $db->carregarColuna("SELECT p.modid
												 		     FROM seguranca.perfil p
												 		     INNER JOIN seguranca.perfilusuario pu ON pu.pflcod = p.pflcod AND pu.usucpf = '".$_SESSION['usucpf']."'
												 		     WHERE p.sisid = ".$sistema->sisid);
							if( $arrModid )
							{
								$arrAux = array();
								$arrSelects = array();

								foreach($arrModid as $modid)
								{
									if( !in_array($modid, $arrAux) )
									{
										if( is_null($modid) )
										{
											$sql = sprintf("select
															 p.pflnivel
															from
															 seguranca.perfil p
															 inner join seguranca.perfilusuario pu on pu.pflcod=p.pflcod
															 							and pu.usucpf='%s' and p.sisid=%d
															 							and p.modid is null
															order by
															 p.pflnivel",
													$_SESSION['usucpf'],
													$sistema->sisid);
											$nivel = $db->pegaUm( $sql );

											$arrSelects[] = sprintf("select
																	distinct p.pflcod as codigo,
																	p.pfldsc as descricao
																   from
																    seguranca.perfil p
																    left join seguranca.perfilusuario pu on pu.pflcod=p.pflcod
																   where
																    p.pflstatus='A' and
																    p.pflnivel >= %d and
																    p.sisid=%d",
																	$nivel,
																	$sistema->sisid);
										}
										else
										{
											$sql = sprintf("select
															 p.pflnivel
															from
															 seguranca.perfil p
															 inner join seguranca.perfilusuario pu on pu.pflcod=p.pflcod
															 							and pu.usucpf='%s' and p.sisid=%d
															 							and p.modid='%d'
															order by
															 p.pflnivel",
													$_SESSION['usucpf'],
													$sistema->sisid,
													$modid);
											$nivel = $db->pegaUm( $sql );

											$arrSelects[] = sprintf("select
																	distinct p.pflcod as codigo,
																	p.pfldsc as descricao
																   from
																    seguranca.perfil p
																    left join seguranca.perfilusuario pu on pu.pflcod=p.pflcod
																   where
																    p.pflstatus='A' and
																    p.pflnivel > %d and
																    p.sisid=%d and
																    p.modid=%d",
															$nivel,
															$sistema->sisid,
															$modid);
										}

										$arrAux[] = $modid;
									}
								}

								$sql_perfil = 'SELECT
												codigo,
												descricao
											   FROM
											   ('.implode(' UNION ALL ', $arrSelects).') as foo
											   ORDER BY
											   descricao';
							}
							else
							{
								$sql = sprintf("select
												 p.pflnivel
												from
												 seguranca.perfil p
												 inner join seguranca.perfilusuario pu on pu.pflcod=p.pflcod
												 							and pu.usucpf='%s' and p.sisid=%d
												 							and p.modid is null
												order by
												 p.pflnivel",
										$_SESSION['usucpf'],
										$sistema->sisid);
								$nivel = $db->pegaUm( $sql );

								$sql_perfil = sprintf("select
														distinct p.pflcod as codigo,
														p.pfldsc as descricao
													   from
													    seguranca.perfil p
													    left join seguranca.perfilusuario pu on pu.pflcod=p.pflcod
													   where
													    p.pflstatus='A' and
													    p.pflnivel > %d and
													    p.sisid=%d and
													    ".($arrPerfil ? " p.pflcod in('".implode("','",$arrPerfil)."') and " : "")."
													    p.modid is null
													   order by
													    descricao",
												$nivel,
												$sistema->sisid);
							}
						}
					}
					else
					{
						if( $db->testa_superuser() )
						{
							$sql_perfil = sprintf("select
													distinct p.pflcod as codigo,
													p.pfldsc as descricao
												   from
												    seguranca.perfil p
												    left join seguranca.perfilusuario pu on pu.pflcod=p.pflcod
												   where
												    p.pflstatus='A' and
												    p.sisid=%d
												   order by
												    descricao",
											$sistema->sisid);
						}
						else
						{
							$sql = sprintf("select
											 p.pflnivel
											from
											 seguranca.perfil p
											 inner join seguranca.perfilusuario pu on pu.pflcod=p.pflcod and pu.usucpf='%s' and p.sisid=%d
											order by
											 p.pflnivel",
									$_SESSION['usucpf'],
									$sistema->sisid);

							$nivel = $db->pegaUm( $sql );

							$sql_perfil = sprintf("select
													distinct p.pflcod as codigo,
													p.pfldsc as descricao
												   from
												    seguranca.perfil p
												    left join seguranca.perfilusuario pu on pu.pflcod=p.pflcod
												   where
												    p.pflstatus='A' and
												    p.pflnivel >= %d and
												    --".($arrPerfil ? " p.pflcod in('".implode("','",$arrPerfil)."') and " : "")."
												    p.sisid=%d
												   order by
												    descricao",
											$nivel,
											$sistema->sisid);
						}
					}

					$sql = sprintf("select
									 distinct p.pflcod as codigo,
									 p.pfldsc as descricao
									from
									 seguranca.perfilusuario pu
									 inner join seguranca.perfil p on p.pflcod=pu.pflcod
									where
									 p.pflstatus = 'A' and
									 p.sisid=%d and
									 pu.usucpf='%s'
									order by
									 descricao",
						$sistema->sisid,
						$usucpf
					);
					#echo "<BR><pre>".$sql."<BR>".$sql_perfil;
					$nome = 'pflcod[' . $sistema->sisid . ']';
					$$nome = $db->carregar( $sql );

					combo_popup( 'pflcod['. $sistema->sisid .']', $sql_perfil, 'Selecione o(s) Perfil(s)', '360x460' );
					?>
				</td>
			</tr>
			<?php
/*				$sistema->sisdiretorio = $sistema->sisid == 14 ? 'cte' : $sistema->sisdiretorio;
				$sql = sprintf("SELECT CASE
									        WHEN
										 (SELECT
										   true
										  FROM
										   pg_tables
										  WHERE
										   schemaname='%s' AND
										   tablename = 'tiporesponsabilidade')
										THEN true
										WHEN
										  (SELECT
										   true
										  FROM
										   pg_tables
										  WHERE
										   schemaname='%s' AND
										   tablename = 'tprperfil')
										THEN true
										ELSE false
										END;",
								strtolower($sistema->sisdiretorio),
								strtolower($sistema->sisdiretorio));

				$existTable = $db->pegaUm($sql);*/
				unset($sql);

			//if ($existTable === 't' && $sistema->sisdiretorio != 'pde'):
			// solicitado pelo Henrique Xavier para o ENEM poder funcionar
			if ($existTable === 't'):

				$sql = "SELECT
						 *
						FROM
						 ".$sistema->sisdiretorio.".tiporesponsabilidade
						WHERE
						 tprsnvisivelperfil = 't'
						ORDER BY
						 tprdsc";
				$responsabilidades = (array) $db->carregar($sql);

				$sqlPerfisUsuario = "SELECT
									  p.pflcod, p.pfldsc
									 FROM
									  seguranca.perfil p INNER JOIN seguranca.perfilusuario pu ON
									  	pu.pflcod = p.pflcod AND
									  	pu.usucpf = '%s' AND
									  	sisid=".$sistema->sisid."
									 WHERE
									  p.pflstatus='A'
									 ORDER BY
									  p.pfldsc";
				$query = sprintf($sqlPerfisUsuario, $usucpf);
				$perfisUsuario = $db->carregar($query);

				$script = gerFuncResp($sistema->sisid, $sistema->sisdiretorio, $usucpf, $responsabilidades);
			?>
			<?php if( $perfisUsuario ): ?>
				<tr>
					<td align='right' class="SubTituloDireita">Associação de Perfil:</td>
					<td>
						<table border="0" cellpadding="2" cellspacing="0" width="500" class="listagem" bgcolor="#fefefe">
							<tr>
								<td width="12" rowspan="2" bgcolor="#e9e9e9" align="center">&nbsp;</td>
								<td rowspan="2" align="left" bgcolor="#e9e9e9" align="center">Descrição</td>
								<td align="center" colspan="<?=@count($responsabilidades)?>" bgcolor="#e9e9e9" align="center" style="border-bottom: 1px solid #bbbbbb">Responsabilidades</td>
							</tr>
							<tr>
								<?php
								foreach( $responsabilidades as $responsabilidade ):
								?>
									<td align="center" bgcolor="#e9e9e9" align="center"><?= $responsabilidade["tprdsc"] ?></td>
								<?
									$javascript = "";
								endforeach;
								?>
							</tr>
							<?php foreach( $perfisUsuario as $perfil ): ?>
								<?php
									$marcado = $i++ % 2 ? '#F7F7F7' : '';
									$sqlResponsabilidadesPerfil = "SELECT
																	p.*,
																	tr.tprdsc,
																	tr.tprsigla
																   FROM
																    (
																     SELECT
																      *
																     FROM
																      ".$sistema->sisdiretorio.".tprperfil
																     WHERE
																      pflcod = '%s') p
																      RIGHT JOIN ".$sistema->sisdiretorio.".tiporesponsabilidade tr ON p.tprcod = tr.tprcod
																   WHERE
																    tprsnvisivelperfil = TRUE
																   ORDER BY
																    tr.tprdsc";

									$query = sprintf($sqlResponsabilidadesPerfil, $perfil["pflcod"]);

									$responsabilidadesPerfil = (array) $db->carregar($query);

									// Esconde a imagem + para perfis sem responsabilidades
									$mostraMais = false;

									foreach ( $responsabilidadesPerfil as $resPerfil ) {
										if ( (boolean) $resPerfil["tprcod"] ){
											$mostraMais = true;
											break;
										}
									}
								?>
								<tr bgcolor="<?=$marcado?>">
									<td style="color: #003c7b">
										<? if ($mostraMais): ?>
											<!-- <a href="Javascript:abreconteudo('geral/cadastro_usuario_responsabilidades.php?usucpf=<?=$usucpf?>&pflcod=<?=$perfil["pflcod"]?>','<?=$perfil["pflcod"]?>')"> -->
											<a href="Javascript:abreconteudo('../<?=$sistema->sisdiretorio; ?>/geral/cadastro_responsabilidades.php?usucpf=<?=$usucpf?>&pflcod=<?=$perfil["pflcod"]?>','<?=$perfil["pflcod"]?>')">
												<img src="../imagens/mais.gif" name="+" border="0" id="img<?=$perfil["pflcod"]?>"/>
											</a>
										<?php endif; ?>
									</td>
									<td><?=$perfil["pfldsc"]?></td>
									<?php foreach( $responsabilidadesPerfil as $resPerfil ): ?>
										<td align="center">
											<?php if ( (boolean) $resPerfil["tprcod"] ): ?>
												<input type="button" name="btnAbrirResp<?=$perfil["pflcod"]?>" value="Atribuir" onclick="popresp_<?= $sistema->sisid ?>(<?=$perfil["pflcod"]?>, '<?=$resPerfil["tprsigla"]?>')">
											<?php else: ?>
												-
											<?php endif; ?>
										</td>
									<?php endforeach; ?>
								</tr>
								<tr bgcolor="<?=$marcado?>">
									<td colspan="10" id="td<?=$perfil["pflcod"]?>"></td>
								</tr>
							<?php endforeach; ?>
						</table>
					</td>
				</tr>
			<?php
				endif;
			endif;
			?>
<?php
	}
}

?>

	<tr bgcolor="#C0C0C0">
		<td width="15%">&nbsp;</td>
		<td>
		<? if($habilitar_edicao == 'S'){
		?>
		<input type="button" class="botao" name="btalterar" value="Salvar"
			onclick="enviar_formulario();">
		<?
		}?>
		<input type="button" class="botao"
			name="btvoltar" value="Voltar" onclick="voltar();">
		</td>
	</tr>

</table>
<?php if('S' == $habilitar_edicao) { ?>
    </form>
<?php } ?>
</body>

<script type="text/javascript" defer="defer">

	function mostraNomeReceita(cpf){

		var comp = new dCPF();
		comp.buscarDados(cpf);
		$('usunome').value = comp.dados.no_pessoa_rf;
		$('usunome').readOnly = true;
	}

	<?=$script; ?>

    var permissao = <?php echo $permissao ? 'true' : 'false' ?>;

	<?php
		//$sql = "SELECT estuf, muncod, estuf || ' - ' || mundescricao as mundsc FROM territorios.municipio ORDER BY 3 ";
	?>
	//var lista_mun = new Array();
	<?// $ultimo_cod = null; ?>
	<?// foreach ( $db->carregar( $sql ) as $unidade ) :?>
		<?// if ( $ultimo_cod != $unidade['estuf'] ) : ?>
			//lista_mun['<?= $unidade['estuf'] ?>'] = new Array();
			<?// $ultimo_cod = $unidade['estuf']; ?>
		<?// endif; ?>
		//lista_mun['<?= $unidade['estuf'] ?>'].push( new Array( '<?= $unidade['muncod'] ?>', '<?= addslashes( trim( $unidade['mundsc'] ) ) ?>' ) );
	<? //endforeach; ?>


	<?php
		$sql =
		"select " .
			" orgcod, " .
			" unicod, " .
			" unicod || ' - ' || unidsc as unidsc " .
		" from unidade " .
		" where " .
			" unistatus = 'A' and " .
			" unitpocod = 'U' " .
		" order by orgcod, unidsc ";
	?>
	var lista_uo = new Array();
	<?php $ultimo_cod = null; ?>
	<?php foreach ( $db->carregar( $sql ) as $unidade ) :?>
		<? if ( $ultimo_cod != $unidade['orgcod'] ) : ?>
			lista_uo['<?= $unidade['orgcod'] ?>'] = new Array();
			<? $ultimo_cod = $unidade['orgcod']; ?>
		<? endif; ?>
		lista_uo['<?= $unidade['orgcod'] ?>'].push( new Array( '<?= $unidade['unicod'] ?>', '<?= addslashes( trim( $unidade['unidsc'] ) ) ?>' ) );
	<?php endforeach; ?>

	<?php
		$sql = "SELECT unicod, ungcod, ungcod||' - '||ungdsc as ungdsc FROM unidadegestora WHERE ungstatus = 'A' AND unitpocod = 'U' ORDER BY unicod, ungdsc";
	?>
	var lista_ug = new Array();
	<? $ultimo_cod = null; ?>
	<? foreach ( $db->carregar( $sql ) as $unidade ) :?>
		<? if ( $ultimo_cod != $unidade['unicod'] ) : ?>
			lista_ug['<?= $unidade['unicod'] ?>'] = new Array();
			<? $ultimo_cod = $unidade['unicod']; ?>
		<? endif; ?>
		lista_ug['<?= $unidade['unicod'] ?>'].push( new Array( '<?= $unidade['ungcod'] ?>', '<?= addslashes( trim( $unidade['ungdsc'] ) ) ?>' ) );
	<? endforeach; ?>

	var status_geral_alterado = false;
	function alterar_status_geral(){
		var antigo = '<?= $suscod ?>';
		var novo = antigo;
		if ( document.formulario.suscod[0].checked )
		{
			novo = document.formulario.suscod[0].value;
		}
		else if ( document.formulario.suscod[1].checked )
		{
			novo = document.formulario.suscod[1].value;
		}
		else if ( document.formulario.suscod[2].checked )
		{
			novo = document.formulario.suscod[2].value;
		}
		var justificativa = document.getElementById( 'justificativa_on' );
		var vazia = document.getElementById( 'justificativa_off' );
		status_geral_alterado = antigo != novo;
		if ( status_geral_alterado ) {
			justificativa.style.display = 'block';
			vazia.style.display = 'none';

		} else {
			justificativa.style.display = 'none';
			vazia.style.display = 'block';
		}
	}


	if (permissao) {
		var antigo = new Array();
		var status_sistema_alterado = new Array();

		<?php
            foreach( $sistemas as $sistema ) {
                if ($sistema instanceof StdClass) {
                    echo "antigo[" , $sistema->sisid , "] = '" , $sistema->usuariosistema->suscod ,   "';\n";
                    echo "status_sistema_alterado[" , $sistema->sisid , "] = false;\n";
                } else {
                    echo "antigo[" , $sistema['sisid'] , "] = '" , null , "';\n";
                    echo "status_sistema_alterado[" , $sistema['sisid'] , "] = false;\n";
                }
            }
        ?>

		function alterar_status_sistema( sisid ){
		var ativo = document.getElementById( 'status_ativo_' + sisid );
		if ( ativo.checked ) {
			novo = ativo.value;
		}
		var pendente = document.getElementById( 'status_pendente_' + sisid );
		if ( pendente.checked ) {
			novo = pendente.value;
		}
		var bloqueado = document.getElementById( 'status_bloqueado_' + sisid );
		if ( bloqueado.checked ) {
			novo = bloqueado.value;
		}
		var justificativa = document.getElementById( 'justificativa_on_' + sisid );
		var vazia = document.getElementById( 'justificativa_off_' + sisid );
		status_sistema_alterado[sisid] = antigo[sisid] != novo;
		if ( status_sistema_alterado[sisid] ) {
			justificativa.style.display = 'block';
			vazia.style.display = 'none';
		} else {
			justificativa.style.display = 'none';
			vazia.style.display = 'block';
		}
        }

	}
	function enviar_formulario(){
		if ( validar_formulario() ) {
			prepara_formulario();
			document.formulario.submit();
		}
	}

	var validar_uo = false;
	function listar_unidades_orcamentarias( orgcod )
	{
		var outros = ( orgcod == '99999' );
		document.formulario.orgao.disabled = !outros;
		var sDisplayOn = document.all ? 'block' : 'table-row';
		var sDisplayOff = 'none';
		if ( outros ) {
			document.getElementById( 'nomeorgao' ).style.display = sDisplayOn;
			document.getElementById( 'tipoorgao' ).style.display = sDisplayOn;
			document.getElementById( 'linha_uo' ).style.display = sDisplayOff;
			document.getElementById( 'linha_ug' ).style.display = sDisplayOff;
		} else {
			document.getElementById( 'nomeorgao' ).style.display = sDisplayOff;
			document.getElementById( 'tipoorgao' ).style.display = sDisplayOff;
			document.getElementById( 'linha_uo' ).style.display = sDisplayOn;
			document.getElementById( 'linha_ug' ).style.display = sDisplayOn;
		}

		var campo_select = document.getElementById( 'unicod' );
		while( campo_select.options.length )
		{
			campo_select.options[0] = null;
		}
		campo_select.options[0] = new Option( '', '', false, true );

		var div_on = document.getElementById( 'unicod_on' );
		var div_off = document.getElementById( 'unicod_off' );
		if ( !lista_uo[orgcod] )
		{
			validar_uo = false;
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			listar_unidades_gestoras( '' );
			return;
		}
		validar_uo = true;
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		var j = lista_uo[orgcod].length;
		for ( var i = 0; i < j; i++ )
		{
			campo_select.options[campo_select.options.length] = new Option( lista_uo[orgcod][i][1], lista_uo[orgcod][i][0], false, lista_uo[orgcod][i][0] == '<?= $unicod ?>' );
		}
		if ( navigator.appName == 'Microsoft Internet Explorer' ) {
			for ( i=0; i< campo_select.length; i++ )
			{
				if ( campo_select.options(i).value == '<?= $unicod ?>' ) {
					campo_select.options(i).selected = true;
				}
			}
		}
	}


	function listar_municipios( estuf )
    {
    	validar_mun = true;
        var div_on = document.getElementById( 'muncod_on' );
		var div_off = document.getElementById( 'muncod_off' );
		div_on.style.display = 'block';
		div_off.style.display = 'none';

         //return new Ajax.Updater('muncod', '<?=$_SESSION['sisdiretorio'] ?>.php?modulo=sistema/usuario/cadusuario&acao=<?=$_REQUEST['acao'] ?>',
		<?php if ( $_SESSION['sisid'] == 147 ): ?>
	        return new Ajax.Updater('muncod', '<?=$_SESSION['sisarquivo'] ?>.php?modulo=sistema2/usuario2/cadusuario&acao=<?=$_REQUEST['acao'] ?>',
		<?php else: ?>
	        return new Ajax.Updater('muncod', '<?=$_SESSION['sisarquivo'] ?>.php?modulo=sistema/usuario/cadusuario&acao=<?=$_REQUEST['acao'] ?>',
		<?php endif; ?>
         {
            method: 'post',
            parameters: '&servico=listar_mun&estuf=' + estuf,
            onComplete: function(res)
            {
           	 atualiza_mun();
            }
        });

    }

	function listar_municipios2( regcod )
	{
		var campo_select = document.getElementById( 'muncod' );
		while( campo_select.options.length )
		{
			campo_select.options[0] = null;
		}
		campo_select.options[0] = new Option( '', '', false, true );

		var div_on = document.getElementById( 'muncod_on' );
		var div_off = document.getElementById( 'muncod_off' );

		if ( !lista_mun[regcod] )
		{
			validar_mun = false;
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			return;
		}

		validar_mun = true;
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		var j = lista_mun[regcod].length;
		for ( var i = 0; i < j; i++ )
		{
			campo_select.options[campo_select.options.length] = new Option( lista_mun[regcod][i][1], lista_mun[regcod][i][0], false, lista_mun[regcod][i][0] == '<?= $muncod ?>' );
		}
		if ( navigator.appName == 'Microsoft Internet Explorer' ) {
			for ( i=0; i< campo_select.length; i++ )
			{
				if ( campo_select.options(i).value == '<?= $muncod ?>' ) {
					campo_select.options(i).selected = true;
				}
			}
		}
	}


	var validar_ug = false;
	function listar_unidades_gestoras( unicod )
	{
		var campo_select = document.getElementById( 'ungcod' );
		while( campo_select.options.length )
		{
			campo_select.options[0] = null;
		}
		campo_select.options[0] = new Option( '', '', false, true );
		var div_on = document.getElementById( 'ungcod_on' );
		var div_off = document.getElementById( 'ungcod_off' );
		if ( !lista_ug[unicod] )
		{
			validar_ug = false;
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			return;
		}
		validar_ug = true;
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		var j = lista_ug[unicod].length;
		for ( var i = 0; i < j; i++ )
		{
			campo_select.options[campo_select.options.length] = new Option( lista_ug[unicod][i][1], lista_ug[unicod][i][0], false, lista_ug[unicod][i][0] == '<?= $ungcod ?>' );
		}
		if ( navigator.appName == 'Microsoft Internet Explorer' ) {
			for ( i=0; i < campo_select.length; i++ )
			{
				if ( campo_select.options(i).value == '<?= $ungcod ?>' ) {
					campo_select.options(i).selected = true;
				}
			}
		}
	}

	function validar_formulario(){
		var validacao = true;
		var mensagem = 'Os seguintes campos não foram preenchidos:';

		if ( document.formulario.muncod.value == '' ) {
			mensagem += '\nMunicípio';
			validacao = false;
		}

		if(permissao){
			if ( document.formulario.usunome.value == '' ) {
				mensagem += '\nNome';
				validacao = false;
			}
		}

		if ( !validar_radio( document.formulario.ususexo, 'Sexo' ) ) {
			mensagem += '\nSexo';
			validacao = false;
		}

		if( ! permissao){
			if ( document.formulario.usudatanascimento.value.length < 10 ) {
				mensagem += '\nData de Nascimento';
				validacao = false;
			}
		} else if( document.formulario.usudatanascimento.value.length > 0 && document.formulario.usudatanascimento.value.length < 10 ) {
			mensagem += '\nData de Nascimento';
			validacao = false;
		}

		if ( document.formulario.regcod.value == '' ) {
			mensagem += '\nUnidade Federal';
			validacao = false;
		}

		if ( document.formulario.tpocod.value == '' ) {
				mensagem += '\n\tTipo do Órgão';
				validacao = false;
			}

		if ( document.formulario.entid ){
			if ( document.formulario.entid.value == '' ) {

				mensagem += '\nÓrgão';
				validacao = false;
			}
		}

//		if ( document.formulario.unicod ) {
//			if ( document.formulario.unicod.value == '' ) {
//				mensagem += '\nUnidade Orçamentária';
//				validacao = false;
//			}
//		}
		/*if ( document.formulario.ungcod ) {
			if ( document.formulario.ungcod.value == '' ) {
					mensagem += '\nUnidade Gestora';
					validacao = false;
			}
		}*/

		if ( document.formulario.usufoneddd.value == '' || document.formulario.usufonenum.value == '' ) {
			mensagem += '\nTelefone';
			validacao = false;
		}

		if ( !validaEmail( document.formulario.usuemail.value ) ) {
			mensagem += '\nE-mail';
			validacao = false;
		}

		if ( document.formulario.carid ) {
			if ( document.formulario.carid.value == '' ) {
				mensagem += '\n\tFunção/Cargo';
				validacao = false;
			}
			else{
				if( document.formulario.carid.value == 9 ){
					if ( document.formulario.usufuncao.value == '' ) {
						mensagem += '\nFunção';
						validacao = false;
					}
				}
			}
		}

		// verifica a alteração de status
		status = true;
		var ativo_geral = document.getElementById( 'status_ativo' );
		if ( status_geral_alterado && document.formulario.htudsc.value == '' && ativo_geral.checked == false ) {
			status = false;
		}
		/*
		for ( var sisid in status_sistema_alterado ) {
			if( typeof( status_sistema_alterado[sisid] ) == 'boolean' ) {
				var ativo = document.getElementById( 'status_ativo_' + sisid );
				if ( status_sistema_alterado[sisid] == true && document.formulario.elements['justificativa['+ sisid +']'].value == '' && ativo.checked == false ) {
					status = false;
				}
			}
		}
		*/
//		if ( !status ) {
//			mensagem += '\nJustificativa';
//			validacao = false;
//		}

		if ( !validacao ) {
			alert( mensagem );
		}

		return validacao;
	}

	function voltar(){
		<?php if ( $_SESSION['sisid'] == 147 ): ?>
	        window.location.href = '?modulo=sistema2/usuario2/consusuario&acao=<?= $_REQUEST['acao'] ?>';
		<?php else: ?>
	        window.location.href = '?modulo=sistema/usuario/consusuario&acao=<?= $_REQUEST['acao'] ?>';
		<?php endif; ?>
	}

	<?php if ( $regcod ): ?>
		listar_municipios( '<?= $regcod ?>' );
	<?php endif; ?>




	function exibir_ocultar_historico( id ){
		div = document.getElementById( id );
		if ( div.style.display == 'none' ) {
			div.style.display = 'block';
		} else {
			div.style.display = 'none'
		}
	}

	function atualiza_mun(){

		var campo_select = document.getElementById( 'muncod' );
 		for ( i=0; i< campo_select.length; i++ )
		{
			<? if( !empty($usuario->muncod) ): ?>
			if ( campo_select.options[i].value == <?=$usuario->muncod ?>) {
				campo_select.options[i].selected = 1;
			}
			<? endif; ?>
		}
	}

	var carid = document.getElementById( 'carid' );

	if( carid.value == 9 ){
		usufuncao.style.display = "";
		linkVoltar.style.display = "";
		carid.style.display = "none";
	}

	function alternarExibicaoCargo( tipo ){

		var carid = document.getElementById( 'carid' );
		var usufuncao = document.getElementById( 'usufuncao' );
		var link = document.getElementById( 'linkVoltar' );

		if( tipo != 'exibirOpcoes' ){
			if( carid.value == 9 ){
				usufuncao.style.display = "";
				//usufuncao.className = "";
				link.style.display = "";
				carid.style.display = "none";
				//link.className = "";
			}
		}
		else{
			usufuncao.style.display = "none";
			//usufuncao.value = "";
			link.style.display = "none";
			//link.className = "objetoOculto";
			carid.style.display = "";
			carid.value = "";
		}
	}

	function visualizaDadosSiape(cpf) {
		var url = '../geral/dadosUsuarioSIAPE.php?cpf='+cpf;

		var janela = window.open(url, '_blank', 'width=500,height=400,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
		janela.focus();
	}
</script>

<?php if( $carid == 9 ){
	//echo "<script>alternarExibicaoCargo( 9 );</script>";
}

function enviarEmailSuperUser($usucpf, $pflcod)
{
    if (IS_PRODUCAO) {

        global $db;
        $sql = "select u.usunome, u.usucpf, u.regcod, u.usuemail,  u.usufoneddd, u.usufonenum, u.usufuncao,
                   p.pflcod, p.pfldsc,
                   s.sisid, s.sisdsc, s.sisabrev,
                   u2.usunome as usunomeorigem, u2.usucpf as usucpforigem, u2.usuemail as usuemailorigem,  u2.usufoneddd as usufonedddorigem, u2.usufonenum as usufonenumorigem
            FROM seguranca.perfilusuario pu
                    inner join seguranca.perfil p on p.pflcod = pu.pflcod
                    inner join seguranca.sistema s on s.sisid = p.sisid
                    inner join seguranca.usuario u on u.usucpf = pu.usucpf
                    inner join seguranca.usuario u2 on u2.usucpf = '{$_SESSION['usucpforigem']}'
            WHERE pu.usucpf = '{$usucpf}'
            AND pu.pflcod = '{$pflcod}' ";
        $dados = $db->pegaLinha($sql);

        $remetente = array("nome" => SIGLA_SISTEMA, "email" => "noreply@mec.gov.br");
        $destinatarios = recuperarDestinatariosPadrao();

        $assunto = "Novo super-usuário atribuído ({$dados['sisabrev']})";
        $mensagem = "<pre>Prezados,

Foi vinculado um novo usuário com o perfil Super-Usuário. Seguem os dados:

Módulo: {$dados['sisdsc']} ({$dados['sisabrev']})
Perfil Atribuído: {$dados['pfldsc']} ({$dados['pflcod']})

Nome: {$dados['usunome']}
CPF: {$dados['usucpf']}
E-mail: {$dados['usuemail']}
UF: {$dados['regcod']}
Telefone: ({$dados['usufoneddd']}) {$dados['usufonenum']}
Função: {$dados['usufuncao']}

Responsável pelo cadastro: {$dados['usunomeorigem']} ({$dados['usucpforigem']})
Telefone: ({$dados['usufonedddorigem']}) {$dados['usufonenumorigem']}
E-mail: {$dados['usuemailorigem']}

Link para verificação do usuário: http://simec.mec.gov.br/seguranca/seguranca.php?modulo=sistema/usuario/cadusuario&acao=A&usucpf={$dados['usucpf']}

Qualquer dúvida, entrar em contato com o gestor do módulo.

Atenciosamente,
Equipe ". SIGLA_SISTEMA. ".
</pre>
";
        $retorno = enviar_email($remetente, $destinatarios, $assunto, $mensagem);
    }
}

function enviarAlteracaoSuperUser($dados, $tipo)
{
    if (IS_PRODUCAO) {

        $remetente = array("nome" => SIGLA_SISTEMA, "email" => "noreply@mec.gov.br");
        $destinatarios = recuperarDestinatariosPadrao();

        $assunto = "Tentativa de alteração de super-usuário";
        if ($tipo == 'senha') {
            $assunto = "Tentativa de alteração de super-usuário (SENHA)";
            $texto = 'Houve tentativa de resetar a senha do super-usuário.';
        } elseif ($tipo == 'email') {
            $assunto = "Tentativa de alteração de super-usuário (E-MAIL)";
            $texto = "Houve tentativa de alterar o email do super-usuário.

Email origem: {$dados['usuemail']}
Email destino: {$dados['usuemaildestino']}
        ";
        }

        $mensagem = "<pre>Prezados,

{$texto}

Módulo: {$_SESSION['sisdsc']} ({$_SESSION['sisabrev']})

Nome: {$dados['usunome']}
CPF: {$dados['usucpf']}
E-mail: {$dados['usuemail']}
UF: {$dados['regcod']}
Telefone: ({$dados['usufoneddd']}) {$dados['usufonenum']}
Função: {$dados['usufuncao']}

Responsável pela alteracao: {$_SESSION['usunome']} ({$_SESSION['usucpf']}) - {$_SESSION['usuemail']}

Link para verificação do usuário: http://simec.mec.gov.br/seguranca/seguranca.php?modulo=sistema/usuario/cadusuario&acao=A&usucpf={$dados['usucpf']}

Qualquer dúvida, entrar em contato com o gestor do módulo.

Atenciosamente,
Equipe <?php echo SIGLA_SISTEMA; ?>.
</pre>
";
        return enviar_email($remetente, $destinatarios, $assunto, $mensagem);
    }
}

function recuperarDestinatariosPadrao()
{
    return array($_SESSION['email_sistema']);
}

?>