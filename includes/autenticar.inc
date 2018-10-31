<?php

	/**
	 * Sistema Integrado de Monitoramento do Ministério da Educação
	 * Setor responsvel: SPO/MEC
	 * Desenvolvedor: Desenvolvedores Simec
	 * Analistas: Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>
	 * Programadores: Renê de Lima Barbosa <renedelima@gmail.com>, Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>
	 * Módulo: Autenticação
	 * Finalidade: Rotina de autenticação de usuários.
	 * Data de criação: 24/06/2005
	 * Última modificação: 28/08/2006
	 */
	// BIBLIOTECA //////////////////////////////////////////////////////////////
	/**
	 * Exibe mensagens de erro.
	 *
	 * @return void
	 */

	function erro(){
		global $db;
		$db->commit();
		$_SESSION = array();
		$_SESSION['MSG_AVISO'] = func_get_args();
		header( "Location: ". $_SERVER['PHP_SELF'] );
		exit();
	}

	// CONFIGURAÇÃO ////////////////////////////////////////////////////////////

	// quantidade limite de tentativas de acesso com senha inválida
	define( 'LIMITE_DE_TENTATIVAS', 4 );

	// lista de status de usuário
	define( 'STATUS_ATIVO', 'A' );
	define( 'STATUS_PENDENTE', 'P' );
	define( 'STATUS_BLOQUEADO', 'B' );

	// mensagens de alera do sistema
	$alerta = array(
		'cpf' => "O cpf informado não está cadastrado.",
		'status_pendente' => "Sua conta está pendente para aprovação, aguarde a avaliação dos administradores.",
		'status_bloqueado' => "Sua conta está bloqueada, clique <a href='solicitar_ativacao_de_conta.php' title='solicitar ativação de conta'>aqui</a> para solicitar a ativação.",
		'senha' => "A senha informada não é válida. Tentativas restantes: %s",
		'tentativas' => "Você excedeu a quantidade de tentativas.",
		'permissao' => "Você não possui permissão de acesso em nenhum dos módulos.",
	);

	// VALIDAÇÃO ///////////////////////////////////////////////////////////////



	$_SESSION["NUM_IP_CLIENTE"] = $_SERVER["REMOTE_ADDR"];
	$_SESSION["DES_BROWSER"] = $_SERVER["HTTP_USER_AGENT"];

	// carrega os dados do formulário

	$cpf = corrige_cpf( $_REQUEST['usucpf'] );
	$senha = trim( $_POST['ususenha'] );


	// carrega os dados da conta do usuário a partir do cpf informado
	$sql = sprintf("
            SELECT
                u.usucpf,
                u.ususenha,
                u.suscod,
                u.usutentativas,
                u.usunome,
                u.usuemail
            FROM seguranca.usuario u
            WHERE
                u.usucpf = '%s'",
            $cpf
	);

	$usuario = (object) $db->recuperar( $sql );
	
	// verifica se o cpf existe
	if ( !$usuario->usucpf ) {
		erro( $alerta['cpf'] );
	}

	// verifica o status do usuário
	switch ( $usuario->suscod ) {
            case STATUS_ATIVO:
                break;
            case STATUS_PENDENTE:
                erro( $alerta['status_pendente'] );
            case STATUS_BLOQUEADO:
            default:
                erro( $alerta['status_bloqueado'] );
	}

	// atribuições requeridas para que a auditoria do sistema funcione
	$_SESSION['sisid'] = 4; # seleciona o sistema de segurança
	$_SESSION['usucpf'] = $usuario->usucpf;
	$_SESSION['usucpforigem'] = $usuario->usucpf;


	// valida a senha informada
	if (md5_decrypt_senha( $usuario->ususenha, '' ) != $senha ) {
		$usuario->usutentativas += 1;
		$alerta['senha'] = sprintf( $alerta['senha'], LIMITE_DE_TENTATIVAS - $usuario->usutentativas + 1 );
		if ( $usuario->usutentativas > LIMITE_DE_TENTATIVAS ) {
			$sql = sprintf( "UPDATE seguranca.usuario SET usutentativas = 0 WHERE usucpf = '%s'", $usuario->usucpf );
			$db->executar( $sql );
			$db->alterar_status_usuario( $usuario->usucpf, STATUS_BLOQUEADO, "Usuário bloqueado por exceder a quantidade de tentativas de login com senha inválida." );
			erro( $alerta['senha'], $alerta['tentativas'], $alerta['status_bloqueado'] );
		} else {
			$sql = sprintf(
				"UPDATE seguranca.usuario SET usutentativas = %d WHERE usucpf = '%s'",
				$usuario->usutentativas,
				$usuario->usucpf
			);
			$db->executar( $sql );
			erro( $alerta['senha'] );
		}
	}
	if ( $usuario->usutentativas > 0 ) {
		$sql = sprintf( "UPDATE seguranca.usuario SET usutentativas = 0 WHERE usucpf = '%s'", $usuario->usucpf );
		$db->executar( $sql );
	}

	// carrega os dados do usuário para a sessão
	unset( $usuario->ususenha );
	foreach ( $usuario as $attribute => $value ) {
		$_SESSION[$attribute] = $value;
	}
	$_SESSION['usucpforigem'] = $usuario->usucpf;

	// verifica permissão de acesso aos módulos
	$sql = sprintf("
            SELECT
                s.sisid,
                s.sisdiretorio,
                s.sisarquivo,
                s.sisdsc,
                s.sisurl,
                s.sisabrev,
                s.sisexercicio,
                s.paginainicial,
                p.pflnivel AS usunivel,
                us.susdataultacesso,
                s.sissnalertaajuda,
                s.sislayoutbootstrap
            FROM seguranca.sistema s
                INNER JOIN seguranca.usuario_sistema us USING (sisid)
                INNER JOIN seguranca.usuario u USING (usucpf)
                INNER JOIN seguranca.perfilusuario pu USING (usucpf)
                INNER JOIN seguranca.perfil p ON(
                    pu.pflcod = p.pflcod
                    AND p.sisid = s.sisid
                )
            WHERE
                us.suscod = 'A'
                AND u.usucpf = '%s'
                AND u.suscod = 'A'
                AND p.pflstatus = 'A'
                AND s.sisstatus = 'A'
            GROUP BY
                s.sisid,
                s.sisdiretorio,
                s.sisarquivo,
                s.sisdsc,
                s.sisurl,
                s.sisabrev,
                s.sisexercicio,
                s.paginainicial,
                p.pflnivel,
                us.susdataultacesso,
                s.sissnalertaajuda,
                s.sislayoutbootstrap
            ORDER BY
                us.susdataultacesso DESC
            LIMIT 1",
            $usuario->usucpf
	);

	$sistema = (object) $db->pegaLinha( $sql );
	if ( !$sistema->sisid ) {
		erro( $alerta['permissao'] );
	}
	// carrega os dados do módulo para a sessão
	foreach ( $sistema as $attribute => $value ) {
		$_SESSION[$attribute] = $value;
	}

	// atribui o ano atual para o exercício das tarefas
	$_SESSION['exercicio_atual'] = $db->pega_ano_atual();
	$_SESSION['exercicio'] = $db->pega_ano_atual();

	unset($_SESSION['superuser']);
	$_SESSION['superuser'] = $db->testa_superuser( $usuario->usucpf );

	//dbg($_SESSION['superuser'],1,'00267967160');
	// atualiza a data de último acesso no módulo selecionado
	$_SESSION['usuacesso'] = date( 'Y/m/d H:i:s' );
	$sql = sprintf(
            $sql = "UPDATE seguranca.usuario_sistema SET susdataultacesso = '%s' where usucpf = '%s' and sisid = %d",
            $_SESSION['usuacesso'],
            $usuario->usucpf,
            $sistema->sisid
	);
	$db->executar( $sql );

	// TO DO: verificar se há mensagens na caixa de entrada (aprovar e implementar fluxo)
	/*	o sistema deverá disparar o processo automático.
	Para tanto o sistema verfica se algume já disparou o processo para o dia escolhido. se sim, não faz nada, se não então atualiza a tabela e dispara o processo.
	*/
	$sql = "select ctrdata from controleprocesso where ctrdata=current_date";
	if  (! $db->pegaUm($sql) )//or $_SESSION['usucpf']=='')
	{
		// dispara os processos
		include APPRAIZ."includes/dispara_processo.inc";
	}

	$totmsg=0;
	unset($_SESSION['mostramsg']);
	$sql = "select count(msgid) as totmsg from mensagemusuario where usucpf = '".$_SESSION['usucpf']."' and msulida ='f' and msuexcluida='f'";
	$_SESSION['mostramsg']=$db->pegaUm($sql);

	/*
	 * O contador de tempo online na tela deve ser atualizado toda vez que o
	 * usuário carregar uma tela do sistema. Ele é utilizado pelo "estou vivo"
	 * de acordo com a constante MAXONLINETIME, definido no config.inc.
	 */
	$_SESSION["evHoraUltimoAcesso"] = time();

	// leva o usuário para a tela inicial do módulo selecionado
	$db->commit();
	$db->close();

	if($_POST['hdn_location']){
		header("Location:{$_POST['hdn_location']}");
		exit;
	}
	if($_SESSION['link_tentativa']){
		$url_tentativa = $_SESSION['link_tentativa'];
		unset($_SESSION['link_tentativa']);
		header("Location: http://$url_tentativa");
		exit;
	}

	$header = sprintf(
		"Location: ../%s/%s.php?modulo=%s",
		$sistema->sisdiretorio,
		$sistema->sisarquivo,
		$sistema->paginainicial
	);

	header( $header );
	exit();

?>