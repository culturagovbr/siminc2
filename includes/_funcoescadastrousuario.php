<?php
/**
 * Arquivo criado no intuito de alocar funções da página de cadastro de usuário bootstrap
 * @author Lindalberto Filho
 */


/**
 * Função para gerar funções em javascript
 * @param unknown $sisid
 * @param unknown $sisDir
 * @param unknown $usucpf
 * @param unknown $dados
 * @return string
 */
function gerFuncResp($sisid, $sisDir, $usucpf, $dados = array()) {
    $script = sprintf ( "function popresp_%s( pflcod, tprsigla ) {switch( tprsigla ){", $sisid );

    foreach ( $dados as $dado ) {
            $script .= sprintf ( " case '%s':
                                   abreresp%s = window.open(
                                   '../%s/geral/cadastro_responsabilidade_%s.php%spflcod='+pflcod+'&usucpf=%s',
                                   'popresp_%s',
                                   'menubar=no,location=no,resizable=no,scrollbars=no,status=yes,width=500,height=540');
                                   break;", $dado ['tprsigla'], $sisid, $sisDir, $dado ['tprurl'], (strpos ( $dado ['tprurl'], '?' ) !== false ? '&' : '?'), $usucpf, $sisid );
    }

    $script .= "\n }\n";
    $script .= sprintf ( " abreresp%s.focus();\n", $sisid );
    $script .= " }\n\n";
    return $script;

}

function carregaDadosUsuario($usucpf){
    global $db;

    $sql = sprintf ("SELECT
                        u.*,
                        e.entid
                    FROM
                        seguranca.usuario u
                    LEFT JOIN
                        entidade.entidade e ON
                        u.entid = e.entid
                    WHERE
                        usucpf = '%s'", $usucpf );

    $usuario = $db->pegaLinha ( $sql );

    if (! $usuario) {
            $_REQUEST ['acao'] = "A";
            if ($_SESSION ['sisid'] == 147) { // obras 2
                    $db->insucesso ( "Usuário Não Encontrado", "&acao=A", "sistema2/usuario2/consusuario" );
            } else {
                    $db->insucesso ( "Usuário Não Encontrado", "&acao=A", "sistema/usuario/consusuario" );
            }
    }
    return $usuario;
}

if (isset ( $_REQUEST ['servico'] ) && $_REQUEST ['servico'] == 'listar_mun') {
	$sql = "SELECT muncod, mundescricao as mundsc
                FROM territorios.municipio
                WHERE estuf = '" . $_REQUEST ['estuf'] . "' ORDER BY mundsc";
	$dados = $db->carregar ( $sql );

	$enviar = '';
	if ($dados)
            $dados = $dados;
	else
            $dados = array ();
	foreach ( $dados as $data ) {
            $enviar .= "<option value= " . $data ['muncod'] . ">  " . simec_htmlentities ( $data ['mundsc'] ) . " </option> \n";
	}

	die ( $enviar );
}


function alteraDadosUsuario($info){
    global $db;
	// Só quem pode alterar senha ou email (em caso de super-usuário) é a própria pessoa
    if ($_SESSION ['usucpforigem'] != $info ['usucpf'] || $_SESSION ['usucpforigem'] != $_SESSION ['usucpf']) {
        $usucpf = $info['usucpf'];
        $sql = "SELECT * FROM seguranca.usuario WHERE usucpf = '{$usucpf}'";
        $dadosUsuario = $db->pegaLinha ( $sql );

        $sql = "SELECT 
                    count(*) as qtd
                FROM 
                    seguranca.perfil p
                INNER JOIN 
                    seguranca.perfilusuario pu on p.pflcod = pu.pflcod
                WHERE 
                    p.pflsuperuser = 't'
                AND 
                    usucpf = '{$info['usucpf']}'";
        $qtdDestino = $db->pegaUm ( $sql );

        if ($qtdDestino) {
            if ($info ['senha']) {
                enviarAlteracaoSuperUser ( $dadosUsuario, 'senha' );
                echo "<script>
                        alert('Você não tem permissão para alterar a senha deste usuário.');
                        window.location = '?modulo=sistema/usuario/consusuario&acao=A';
                      </script>";
                exit ();
            }

            if ($dadosUsuario ['usuemail'] != $info ['usuemail']) {
                $dadosUsuario ['usuemaildestino'] = $info ['usuemail'];
                enviarAlteracaoSuperUser ( $dadosUsuario, 'email' );
                echo "<script>
                        alert('Você não tem permissão para alterar o email deste usuário.');
                        window.location = '?modulo=sistema/usuario/consusuario&acao=A';
                      </script>";
                exit ();
            }
        }
    }
    // $tpocod_banco = $_REQUEST['tpocod'] ? (integer) $_REQUEST['tpocod'] : "null";

    // data de nascimento
    $dataBanco = formata_data_sql ( $info ['usudatanascimento'] );
    $dataBanco = $dataBanco ? "'" . $dataBanco . "'" : "null";
    
    // Caso tenha entidade 
    $entid = isset( $info['entid'] ) && !isset($info['orgao'])  ? $info['entid'] : 'NULL';
    $entid = $entid == 999999 ? 'NULL' : $entid;
       
    // arrumando problema de slashes excessivos causados pela diretiva magic codes do php
    $orgao = (str_replace ( "\\", "", $info ['orgao'] ));
    $orgao = stripcslashes ( $orgao );
    $orgao = str_replace ( "'", "", $orgao );

    /*
    * Integração com o SSD Atualizando os possiveis novos dados Desenvolvido por Alexandre Dourado
    */
    if (AUTHSSD) {
        // Definindo o local dos certificados
        // define("PER_PATH", "../");
        include_once (APPRAIZ . "www/connector.php");
        $SSDWs = new SSDWsUser ( $tmpDir, $clientCert, $privateKey, $privateKeyPassword, $trustedCaChain );
        // Efetuando a conexão com o servidor (produção/homologação)
        if ($GLOBALS ['USE_PRODUCTION_SERVICES']) {
            $SSDWs->useProductionSSDServices ();
        } else {
            $SSDWs->useHomologationSSDServices ();
        }

        $SSD_tipo_pessoa = @utf8_encode ( "F" );
        $SSD_nome = @utf8_encode ( $_POST ["usunome"] );
        $SSD_cpf = @utf8_encode ( $info['usuario']['usucpf'] );
        $SSD_data_nascimento = @utf8_encode ( formata_data_sql ( $_POST ["usudatanascimento"] ) );

        if ($_POST ['usuemailconfssd'] != $_POST ["usuemail"]) {
            $SSD_email = @utf8_encode ( $_POST ["usuemail"] );
        }

        $SSD_ddd_telefone = @utf8_encode ( $_POST ["usufoneddd"] );
        $SSD_telefone = @utf8_encode ( $_POST ["usufonenum"] );

        $userInfo = "$SSD_tipo_pessoa||$SSD_nome||$nome_mae||$SSD_cpf||$rg||$sigla_orgao_expedidor||$orgao_expedidor||$nis||" . "$SSD_data_nascimento||$codigo_municipio_naturalidade||$codigo_nacionalidade||$SSD_email||$email_alternativo||" . "$cep||$endereco||$sigla_uf_cep||$localidade||$bairro||$complemento||$endereco||$SSD_ddd_telefone||$SSD_telefone||" . "$ddd_telefone_alternativo||$telefone_alternativo||$ddd_celular||$celular||$instituicao_trabalho||$lotacao||" . "$justificativa||$cpf_responsavel||ssd";

        $resposta = $SSDWs->updateUser ( $userInfo );

        if ($resposta != "true") {
            if ($_SESSION ['sisid'] == 147) { // obras 2
                echo "<script>
                        alert('" . addslashes ( $resposta ["erro"] ) . "');
                        window.location = '?modulo=sistema2/usuario2/consusuario&acao=A';
                      </script>";
                exit ();
            } else {
                echo "<script>
                            alert('" . addslashes ( $resposta ["erro"] ) . "');
                            window.location = '?modulo=sistema/usuario/consusuario&acao=A';
                      </script>";
                exit ();
            }
        }
    }
    //moldando objetos
    $usuario = new stdClass();
    $usuario = $info['usuario'];
    /*
    * FIM Integração com o SSD Atualizando os possiveis novos dados Desenvolvido por Alexandre Dourado
    */
    // variável permissão cadastro_usuario_geral_bootstrap
    $info ['carid'] = ($info ['carid'] != '') ? $info ['carid'] : 'NULL';
    if (!empty($info['permissao'])) {
        
        $sql = sprintf ( "
            UPDATE seguranca.usuario 
                SET
                usunome           = '" . $_POST ['usunome'] . "',
                usuemail          = '%s',
                usufoneddd        = '%s',
                usufonenum        = '%s',
                usufuncao         = '%s',
                carid             = %s,
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
                pg_escape_string ( $info['usuemail'] ), 
                pg_escape_string ( $info['usufoneddd'] ), 
                str_replace ( "\\", "", substr ( $info['usufonenum'], 0, 10 ) ), 
                pg_escape_string ( substr ( $info['usufuncao'], 0, 100 ) ), 
                pg_escape_string ( $info['carid'] ), 
                pg_escape_string ( $entid ), 
                pg_escape_string ( $info['unicod'] ), 
                pg_escape_string ( $info['regcod'] ), 
                pg_escape_string ( $info['ungcod'] ), 
                pg_escape_string ( $info['ususexo'] ), 
                $dataBanco, 
                pg_escape_string ( $info['usunomeguerra'] ), 
                pg_escape_string ( $info['muncod'] ), 
                str_replace ( "'", "", $orgao ), 
                pg_escape_string ( $info['tpocod'] ), 
                pg_escape_string ( $usuario->usucpf ) );
    } else {
        $data_atual = $data_atual ? "'" . $data_atual . "'" : "null";
        // atualiza dados gerais do usuário
        $sql = sprintf ( "
            UPDATE seguranca.usuario 
                SET
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
                pg_escape_string ( $info['usuemail'] ), 
                pg_escape_string ( $info['usufoneddd'] ), 
                str_replace ( "\\", "", substr ( $info['usufonenum'], 0, 10 ) ), 
                pg_escape_string ( $info['usufuncao'] ), 
                pg_escape_string ( $info['carid'] ), 
                pg_escape_string ( $entid ), 
                pg_escape_string ( $info['unicod'] ), 
                pg_escape_string ( $info['regcod'] ), 
                pg_escape_string ( $info['ungcod'] ), 
                pg_escape_string ( $info['ususexo'] ), 
                $dataBanco, 
                pg_escape_string ( $info['usunomeguerra'] ), 
                pg_escape_string ( $info['muncod'] ), 
                str_replace ( "'", "", $orgao ), 
                pg_escape_string ( $info['tpocod'] ), 
                $data_atual, 
                pg_escape_string ( $info['usucpf'] ) 
        );
    }
    $booAlterar = $db->executar( $sql );
    // altera a senha do usuário com o valor padrão
    if ($info['senha']) {
        /*
        * Integração com o SSD Atualizando nova senha por padrão Desenvolvido por Alexandre Dourado
        */
        if (AUTHSSD) {
            // Definindo o local dos certificados
            // define("PER_PATH", "../");

            include_once (APPRAIZ . "www/connector.php");
            $SSDWs = new SSDWsUser ( $tmpDir, $clientCert, $privateKey, $privateKeyPassword, $trustedCaChain );
            // Efetuando a conexão com o servidor (produção/homologação)
            if ($GLOBALS ['USE_PRODUCTION_SERVICES']) {
                    $SSDWs->useProductionSSDServices ();
            } else {
                    $SSDWs->useHomologationSSDServices ();
            }
            $cpfOrCnpj = $usuario->usucpf;
            $oldPassword = base64_encode ( md5_decrypt_senha ( $usuario->ususenha, '' ) );
            $newPassword = base64_encode ( SENHA_PADRAO );
            $resposta = $SSDWs->changeUserPasswordByCPFOrCNPJ ( $cpfOrCnpj, $oldPassword, $newPassword );
            if ($resposta != "true") {
                if ($_SESSION ['sisid'] == 147) { // obras 2
                    echo "<script>
                            alert('" . addslashes ( $resposta ["erro"] ) . "');
                            window.location = '?modulo=sistema2/usuario2/consusuario&acao=A';
                            </script>";
                    exit ();
                } else {
                    echo "<script>
                            alert('" . addslashes ( $resposta ["erro"] ) . "');
                            window.location = '?modulo=sistema/usuario/consusuario&acao=A';
                            </script>";
                    exit ();
                }
            }
        }
        /*
         * FIM Integração com o SSD Atualizando nova senha por padrão Desenvolvido por Alexandre Dourado
        */
        $sql = sprintf ( "UPDATE
                            seguranca.usuario
                        SET
                            ususenha         = '%s',
                            usuchaveativacao = 'f'
                        WHERE
                            usucpf = '%s'", md5_encrypt_senha ( SENHA_PADRAO, '' ), $_REQUEST ['usucpf'] );

        $db->executar ( $sql );
    }
        // aplica as alterações de status nos sistemas
        foreach ( ( array ) $_REQUEST ['status'] as $sisid => $suscod ) {
                $sql = sprintf ( "SELECT us.* FROM seguranca.usuario_sistema us WHERE sisid = %d AND usucpf = '%s'", $sisid, $_REQUEST['usucpf'] );
                $usuariosistema = ( object ) $db->pegaLinha ( $sql );
                if (! $usuariosistema->sisid) {
                    $sql = sprintf ( "INSERT INTO seguranca.usuario_sistema ( sisid, usucpf ) VALUES ( %d, '%s' )", $sisid, $_REQUEST['usucpf'] );
                    $db->executar ( $sql );
                }

                if ($usuariosistema->suscod != $suscod) {
                    $justificativa = $_REQUEST ['justificativa'] [$sisid];
                    $db->alterar_status_usuario ( $_REQUEST['usucpf'], $suscod, $justificativa, $sisid );
                    $email_aprovacao = $usuariosistema->suscod == 'P' && $suscod == 'A' ? true : $email_aprovacao;
                }
        }

        // executa rotina para alteração do status geral no sistema
        if ($_SESSION ['sisid'] == 4 /*in_array( 'geral', $configuracao )*/ ) {
                if ($usuario->suscod != $_REQUEST ['suscod']) {
                        $db->alterar_status_usuario ( $_REQUEST['usucpf'], $_REQUEST ['suscod'], $_REQUEST ['htudsc'] );
                        $email_aprovacao = $usuario->suscod == 'P' && $_REQUEST ['suscod'] == 'A';
                }
        }

        // envia o email de confirmação caso a conta seja aprovada
        if ($email_aprovacao) {
            $remetente = array (
                "nome" => $_SESSION ['usunome'],
                "email" => $_SESSION ['usuemail']
            );
            $destinatario = $_REQUEST ['usuemail'];
            $assunto = "Aprovação do Cadastro no ".SIGLA_SISTEMA;
            $conteudo = "
                <br />
                <span style='background-color: red;'><b>Esta é uma mensagem gerada automaticamente pelo sistema. </b></span>
                <br />
                <span style='background-color: red;'><b>Por favor, não responda. Pois, neste caso, a mesma será descartada.</b></span>
                <br />
                <br />
            ";
            $conteudo .= sprintf ( "%s %s<p>Sua conta está ativa. Sua Senha de acesso é: %s</p>", $_REQUEST ['ususexo'] == 'M' ? 'Prezado Sr.' : 'Prezada Sra.', $_REQUEST ['usunome'], md5_decrypt_senha ( $usuario->ususenha, '' ) );
            $conteudo .= "
                <br />
                <a style='color: #0000ff;' href='". URL_SISTEMA. "' title='".NOME_SISTEMA ."'>". URL_SISTEMA. "</a>
                <br />
                <br />
            ";
            $corpoEmailV3 = '<p>'. $conteudo. '</p>';
            include APPRAIZ . "includes/email-template.php";
//ver($textoEmailV3, d);
            enviar_email ( $remetente, $destinatario, $assunto, $textoEmailV3 );
        }
        // cadastra os perfils selecionados
        if (is_array($_REQUEST ['pflcod'])) {
            foreach ($_REQUEST ['pflcod'] as $sisid => $perfis) {
                if ($perfis <> '') {
                    //varrendo se já existe perfil cadastrado para o cpf informado
                    $sqlPerfilAntigo = sprintf ( "SELECT * FROM seguranca.perfilusuario
                                      WHERE usucpf = '%s' AND pflcod IN ( SELECT p.pflcod FROM seguranca.perfil p WHERE p.sisid = %d )", $_REQUEST ['usucpf'], $sisid );
                    $perfisAntigos = $db->carregar ( $sqlPerfilAntigo );
                    $aPerfisAntigos = array ();
                    //armazenando os perfis antigos
                    if ($perfisAntigos && is_array ( $perfisAntigos )) {
                        foreach ( $perfisAntigos as $perfilAntigo ) {
                            $aPerfisAntigos [] = $perfilAntigo ['pflcod'];
                        }
                    }

                    // deleta os perfis encontrados, mas já estão salvos na variável $aPerfisAntigos
                    $sql = sprintf ( "DELETE FROM seguranca.perfilusuario
                                      WHERE usucpf = '%s' AND pflcod IN ( SELECT p.pflcod FROM seguranca.perfil p WHERE p.sisid = %d )", $_REQUEST ['usucpf'], $sisid );
                    $db->executar ( $sql );
                    removerPerfisSlaves ( $_REQUEST ['usucpf'], $sisid );

                    /**
                     * * REGRA DO ENEM **
                    */
                    if ($email_aprovacao && $_SESSION ['sisid'] == '24') {
                        $sql = "SELECT fun.funid FROM entidade.entidade ent
                                INNER JOIN entidade.funcaoentidade fue ON fue.entid = ent.entid AND fue.fuestatus = 'A'
                                INNER JOIN entidade.funcao fun ON fun.funid = fue.funid AND fun.funstatus = 'A'
                                WHERE ent.entstatus = 'A'
                                AND ent.entnumcpfcnpj = '" . $_REQUEST ['usucpf'] . "'";
                        $funid_enem = $db->carregarColuna ( $sql );

                        if ($funid_enem) {
                            foreach ( $funid_enem as $funid ) {
                                /**
                                 * * executor **
                                 */
                                if ($funid == 83 && ! in_array ( "518", $perfis )) {
                                        $sql = sprintf ( "INSERT INTO seguranca.perfilusuario ( usucpf, pflcod ) VALUES ( '%s', 518 )", $_REQUEST ['usucpf'] );
                                        $db->executar ( $sql );
                                }
                                /**
                                 * * validador **
                                 */
                                if ($funid == 84 && ! in_array ( "519", $perfis )) {
                                        $sql = sprintf ( "INSERT INTO seguranca.perfilusuario ( usucpf, pflcod ) VALUES ( '%s', 519 )", $_REQUEST ['usucpf'] );
                                        $db->executar ( $sql );
                                }
                                /**
                                 * * certificador **
                                 */
                                if ($funid == 85 && ! in_array ( 520, $perfis )) {
                                        $sql = sprintf ( "INSERT INTO seguranca.perfilusuario ( usucpf, pflcod ) VALUES ( '%s', 520 )", $_REQUEST ['usucpf'] );
                                        $db->executar ( $sql );
                                }
                            }
                        }//fim if($funid_enem)
                    }//fim if($email_aprovacao && $_SESSION ['sisid'] == '24')

                    //tratando os perfis recuperados
                    $perfis = is_array( $perfis ) ? $perfis : array();  
                    foreach ( $perfis as $pflcod ) {
                        if (empty ( $pflcod )) {
                                continue;
                        }
                        // verifica se existe os perfis para o cpf
                        $sqlPerfilExiste = "select usucpf from seguranca.perfilusuario where usucpf = '{$_REQUEST ['usucpf']}' and pflcod = {$pflcod}";
                        $existePerfis    = $db->pegaUm ( $sqlPerfilExiste );

                        if ( !$existePerfis ) {
                                // inclui os perfis
                                $sqlInsert = sprintf ( "INSERT INTO seguranca.perfilusuario ( usucpf, pflcod ) VALUES ( '%s', %d )", $_REQUEST ['usucpf'], $pflcod );
                                $db->executar ( $sqlInsert );

                                $sql = "SELECT pflcod FROM perfil WHERE pflcod = $pflcod AND pflsuperuser = 't' ";
                                $perfilSuperUser = $db->pegaUm ( $sql );

                                if ($_SESSION ['ambiente'] == 'Ambiente de Produção' && $perfilSuperUser && ! in_array ( $perfilSuperUser, $aPerfisAntigos )) {
                                        enviarEmailSuperUser ( $_REQUEST ['usucpf'], $pflcod );
                                }
                        }
                        inserirPerfisSlaves ( $_REQUEST ['usucpf'], $pflcod );
                        atualizarResponsabilidadesSlaves ( $_REQUEST ['usucpf'], $pflcod );
                    }

                    // INATIVA AS RESPONSABILIDADES (ENTIDADE,ESTADOS,MUNICIPIOS)
                    if ($sisid != 4) { // 4=Módulo de Segurança
                        // pega o nome do sistema tabela
                        $sql = sprintf ( "SELECT s.* FROM seguranca.sistema s WHERE sisid = %d", $sisid );
                        $sistema = ( object ) $db->pegaLinha ( $sql );
                        $tabela_aux = $sistema->sisdiretorio;
                        if ($sisid == 14)
                            $tabela_aux = "cte";
                        if ($tabela_aux == 'pde') {
                            // 10=Monitoramento do Plano de Desenvolvimento da Educação / 11=gerenciamento projetos
                            // pde
                            // $sisidaux = "10,11";
                            $sisidaux = "select sisid from seguranca.sistema  where sisdiretorio = 'pde'";
                        } elseif ($sisid == 13 || $sisid == 14) {
                            // 13=PAR - plano de metas / 14=Brasilpro
                            // cte
                            $sisidaux = "13,14";
                        } elseif ($sisid == 1 || $sisid == 6) {
                            // 1="PPA-Monitoramento e Avaliação" / 6="Projetos Especiais"
                            // monitora
                            $sisidaux = "1,6";
                        } elseif ($sisid == 2 || $sisid == 5) {
                            // 2="Programação Orçamentária" / 5="PPA-Elaboração e Revisão"
                            // elabrev
                            $sisidaux = "2,5";
                        } else {
                            $sisidaux = $sisid;
                        }
                        $sql = "SELECT true FROM pg_tables
                                WHERE schemaname = '" . $tabela_aux . "' 
                                AND tablename  = 'usuarioresponsabilidade'";

                        if ($db->pegaUm ( $sql ) == 't') {
                                $sqlr = "UPDATE " . $tabela_aux . ".usuarioresponsabilidade SET rpustatus='I'
                                        WHERE usucpf='" . $_REQUEST ['usucpf'] . "' and rpustatus='A' and pflcod not in( 
                                              SELECT pu.pflcod from seguranca.perfilusuario pu, seguranca.perfil p 
                                              WHERE pu.pflcod=p.pflcod and p.sisid in (" . $sisidaux . ")
                                              AND pu.usucpf ='" . $_REQUEST ['usucpf']  . "')";
                                $db->executar ( $sqlr );
                        }
                    }//fim modulo seguranca
                    // FIM INATIVA

                        $db->commit ();
                        //$parametros = '&usucpf=' . $_REQUEST ['usucpf'];
                        die ( '<script>
                                alert(\'Operação realizada com sucesso!\');
                                location.href = window.location;
                               </script>');

                }//fim foreach ($_REQUEST ['pflcod'] as $sisid => $pflcod)
            }
        }//fim function alterarDadosUsuarios
        if($booAlterar){//submetendo dados para alteração.
        $db->commit ();
            //$parametros = '&usucpf=' . $_REQUEST ['usucpf'];
            die ( '<script>
                    alert(\'Operação realizada com sucesso!\');
                    location.href = window.location;
                   </script>');
        }//fim booAlterar
}

//------------------------------------------- INICIO DA FUNCTION MOSTRARESPONSABILIDADES -----------------------------------------------------
function mostraResponsabilidades($responsabilidades = array(), $perfisUsuario, $sistema, $exibirBotao = true){
    global $db;
    $responsabilidades = is_array($responsabilidades) ? $responsabilidades : array();
    $colspan = @count($responsabilidades)
    ?>
    <section class="col-md-10">
        <table class="table table-bordered table-condensed">
            <thead>
                <tr class="active">
                    <th rowspan="2" colspan="2">Descrição</th>
                    <th colspan="<?=$colspan?>">Responsabilidades</th>
                </tr>
                <tr class="active">
                    <?php foreach ($responsabilidades as $responsabilidade):
                    echo <<<HTML
                        <th>{$responsabilidade["tprdsc"]}</th>
HTML;
                    endforeach; ?>
                </tr>
            </thead>
            <tbody style="background-color:white;">
		    <?php
		    	foreach( $perfisUsuario as $perfil ):
       				$sqlResponsabilidadesPerfil = <<<DML
				        SELECT
				            p.*,
				            tr.tprdsc,
				            tr.tprsigla
				        FROM (SELECT * FROM {$sistema->sisdiretorio}.tprperfil
				        WHERE pflcod = '%s') p
				        RIGHT JOIN {$sistema->sisdiretorio}.tiporesponsabilidade tr ON p.tprcod = tr.tprcod
				        WHERE tprsnvisivelperfil = TRUE
				        ORDER BY tr.tprdsc
DML;

			        $query = sprintf($sqlResponsabilidadesPerfil, $perfil ["pflcod"]);
			        $responsabilidadesPerfil = (array) $db->carregar($query);
			
			        // Esconde a imagem + para perfis sem responsabilidades
			        $mostraMais = false;
			
			        foreach($responsabilidadesPerfil as $resPerfil) {
			            if ((boolean) $resPerfil ["tprcod"]) {
			                $mostraMais = true;
			                break;
			            }
			        }
			    ?>
                <tr>
                    <td width="10px" align="center">
			            <? if ($mostraMais): ?>
							<a href="Javascript:abreconteudo('../<?=$sistema->sisdiretorio; ?>/geral/cadastro_responsabilidades.php?usucpf=<?=$sistema->usuariosistema->usucpf?>&pflcod=<?=$perfil["pflcod"]?>','<?=$perfil["pflcod"]?>')">
			                	<img src="../imagens/mais.gif" name="+" border="0"	id="img<?=$perfil["pflcod"]?>" />
							</a>
			            <? endif; ?>
                    </td>
                    <td><strong><?=$perfil["pfldsc"]?></strong></td>
		            <? foreach( $responsabilidadesPerfil as $resPerfil ): ?>
					<td align="center">
		            	<? if ( (boolean) $resPerfil["tprcod"]  && $exibirBotao == true): ?>
	                        <button type="button" class="btn btn-primary" name="btnAbrirResp<?=$perfil["pflcod"]?>" onclick="popresp_<?= $sistema->sisid ?>(<?=$perfil["pflcod"]?>, '<?=$resPerfil["tprsigla"]?>')" data-toggle="modal" data-target="#myModal">Atribuir</button>
		                <? else: ?>
		                	-
		                <? endif; ?>
					</td>
		            <? endforeach; ?>
                </tr>
                <tr>
					<td style="display: none;" colspan="<?=$colspan+2?>" id="td<?=$perfil["pflcod"]?>"></td>
                </tr>
			<? endforeach; ?>
            </tbody>
        </table>
    </section><?
    }
 //------------------------------------------- FIM DA FUNCTION MOSTRARESPONSABILIDADES -----------------------------------------------------