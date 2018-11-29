<?php

    # Carrega as bibliotecas internas do sistema
    include "config.inc";
    require APPRAIZ . "includes/classes_simec.inc";
    include APPRAIZ . "includes/funcoes.inc";
    include_once APPRAIZ . "includes/classes/Sms.class.inc";

    # Abre conexão com o servidor de banco de dados
    $db = new cls_banco();
    
    // Carrega a combo com os municípios
    if ($_REQUEST["ajaxRegcod"]) {
        header('content-type: text/html; charset=ISO-8859-1');

        $sql = "
            SELECT
                muncod AS codigo,
                mundescricao AS descricao
            FROM territorios.municipio
            WHERE
                estuf = '{$_REQUEST['ajaxRegcod']}'
            ORDER BY
                mundescricao ASC";

        die($db->monta_combo("muncod", $sql, 'S', 'Selecione um município', '', '', '', '200', 'S', 'muncod', null, null, null, null, 'chosen-select'));
    }

    // Carrega a combo com os orgãos do tipo selecionado
    if ($_REQUEST["ajax"] == 1) {
        // Se for estadual verifica se existe estado selecionado
        if ($_REQUEST["tpocod"] == 2 && empty($_REQUEST["regcod"])) {
            echo '<font style="color:#909090;">
                                            Favor selecionar um Estado.
                                      </font>';
            die;
        }
        // Se for municipal verifica se existe estado selecionado
        if ($_REQUEST["tpocod"] == 3 && empty($_REQUEST["muncod"])) {
            echo '<font style="color:#909090;">
                                            Favor selecionar um município.
                                      </font>';
            die;
        }
        $tpocod = $_REQUEST["tpocod"] ? $_REQUEST["tpocod"] : 1;
        $muncod = $_REQUEST["muncod"];
        $regcod = $_REQUEST["regcod"];

        carrega_orgao($editavel, $usucpf);
        die;
    }

    // Carrega a combo com os orgãos do tipo selecionado
    if ($_REQUEST["ajax"] == 2) {
        carrega_unidade($_REQUEST["entid"], $editavel, $usuario->usucpf);
        die;
    }

    if ($_REQUEST["ajax"] == 3) {
        carrega_subunidade_orcamentaria($_REQUEST["unicod"], date('Y'));
        die;
    }

    $_SESSION['mnuid'] = 10;
    $_SESSION['sisid'] = 4;

    // captura os dados informados no primeiro passo
    $sisid = $_REQUEST['sisid'];
    $modid = $_REQUEST['modid'];
    $usucpf = $_REQUEST['usucpf'];

    // Verifica se o CPF digitado é válido.
    if (!validaCPF($usucpf)) {
        die('
            <script>
                alert(\'CPF inválido!\');
                history.go(-1);
            </script>');
    }

    // atribui o ano atual para o exercício das tarefas
    // ultima modificação: 05/01/2007
    $_SESSION['exercicio_atual'] = $db->pega_ano_atual();
    $_SESSION['exercicio'] = $db->pega_ano_atual();

    // captura os dados do formulário
    $usunome = $_REQUEST['usunome'];
    $usuemail = $_REQUEST['usuemail'];
    $usuemail_c = $_REQUEST['usuemail_c'];
    $usufoneddd = $_REQUEST['usufoneddd'];
    $usufonenum = $_REQUEST['usufonenum'];

    // Verifica a entidade
    $entid = isset($_REQUEST['entid']) ? $_REQUEST['entid'] : null;
    $entid = $entid == 999999 ? 'null' : $entid;

    // captura os dados do formulário
    $usufuncao = $_REQUEST['usufuncao'];
    $carid = $_REQUEST['carid'];
    $unicod = $_REQUEST['unicod'];
    $ungcod = $_REQUEST['ungcod'];
    $regcod = $_REQUEST['regcod'];
    $ususexo = $_REQUEST['ususexo'];
    $htudsc = $_REQUEST['htudsc'];
    $pflcod = $_REQUEST['pflcod'];
    $orgao = $_REQUEST['orgao'];
    $muncod = $_REQUEST['muncod'];
    $tpocod = $_REQUEST['tpocod'];

    // prepara o cpf para ser usado nos comandos sql
    $cpf = corrige_cpf($usucpf);

    // verifica se o cpf já está cadastrado no sistema
    $sql = sprintf("
        SELECT
            u.ususexo,
            u.usucpf,
            u.regcod,
            u.usunome,
            u.usuemail,
            u.usustatus,
            u.usufoneddd,
            u.usufonenum,
            u.ususenha,
            u.usudataultacesso,
            u.usunivel,
            u.usufuncao,
            u.ususexo,
            u.entid,
            u.unicod,
            u.usuchaveativacao,
            u.usutentativas,
            u.usuobs,
            u.ungcod,
            u.usudatainc,
            u.usuconectado,
            u.suscod,
            u.muncod,
            u.tpocod,
            u.orgao,
            u.carid
        FROM
            seguranca.usuario u
        WHERE
            u.usucpf = '%s'", $cpf);

    $usuario = (object) $db->pegaLinha($sql);

    if ($usuario->usucpf) {
        // Verifica se todos os dados obrigatórios estão preenchidos
        if (
            !$usuario->usunome || !$usuario->ususexo || !$usuario->regcod || (!$usuario->entid && !$usuario->orgao) ||
            !$usuario->usufoneddd || !$usuario->usufonenum || !$usuario->usuemail
        ) {
            $sql = "
                SELECT
                    count(*)
                FROM seguranca.perfilusuario up
                    LEFT JOIN seguranca.perfil p ON p.pflcod = up.pflcod
                    LEFT JOIN seguranca.sistema s ON s.sisid = p.sisid
                WHERE
                    up.usucpf = '{$usuario->usucpf}'
                    AND sisstatus = 'A'
            ";

            $qtdModulos = $db->pegaUm($sql);

            if ($qtdModulos) {
                $_SESSION['MSG_AVISO'] = 'Você já possui acesso ao SIMEC e seu cadastro está incompleto. <br />Entre em contato com o gestor do seu sistema ou preencha os dados em seu cadastro antes de solicitar acesso à outro módulo.';
                echo '<script>
                        window.location = "cadastrar_usuario.php";
                      </script>
                ';
            }
        }

        foreach ($usuario as $atributo => $valor) {
            $$atributo = $valor;
        }

        $usucpf = formatar_cpf($usuario->usucpf);
        $cpf_cadastrado = true;
        $editavel = 'N';
    } else {
        $cpf_cadastrado = false;
        $editavel = 'S';
    }

    // verifica se o usuário já está cadastrado no módulo selecionado
    $sql = sprintf("SELECT usucpf, sisid, suscod FROM usuario_sistema WHERE usucpf = '%s' AND sisid = %d", $cpf, $sisid);

    $usuario_sistema = (object) $db->pegaLinha($sql);

    if ($usuario_sistema->sisid) {
        if ($usuario_sistema->suscod == 'B') {
            $_SESSION['MSG_AVISO'] = array("Sua conta está bloqueada neste sistema. Para solicitar a ativação da sua conta justifique o pedido no formulário abaixo.");
            header("Location: solicitar_ativacao_de_conta_sigest.php?sisid=$sisid&modid=$modid&usucpf=$usucpf");
            exit();
        }
        $_SESSION['MSG_AVISO'] = array("Atenção. CPF já cadastrado no módulo solicitado.");
        header("Location: cadastrar_usuario_sigest.php?sisid=$sisid&modid=$modid&usucpf=$usucpf");
        exit();
    }

    $cpf_cadastrado_sistema = (boolean) $db->pegaUm($sql);

    $sql = sprintf("select sisid, sisdsc, sisfinalidade, sispublico, sisrelacionado, sisdiretorio from sistema where sisid = %d", $sisid);
    $sistema = (object) $db->pegaLinha($sql);

    // efetiva cadastro se o formulário for submetido
    if ($_POST['formulario']) {

        // Gerando a senha que poderá ser usada no SSD e no simec
        $senhageral = $db->gerar_senha();

        /*
         *  Código feito para integrar a autenticação do SIMEC com o SSD
         *  Inserir o usuário no BD do SSD e inserir a permissão
         *  Desenvolvido por Alexandre Dourado
         */
        if (AUTHSSD) {
            include_once("connector.php");
            /*
             *  Código feito para integrar a autenticação do SIMEC com o SSD
             *  Verifica se o cpf ja esta cadastrado no SSD
             *  Desenvolvido por Alexandre Dourado
             */

            // Instanciando Classe de conexão
            $SSDWs = new SSDWsUser($tmpDir, $clientCert, $privateKey, $privateKeyPassword, $trustedCaChain);

            // Efetuando a conexão com o servidor (produção/homologação)
            if ($GLOBALS['USE_PRODUCTION_SERVICES']) {
                $SSDWs->useProductionSSDServices();
            } else {
                $SSDWs->useHomologationSSDServices();
            }

            $cpfOrCnpj = str_replace(array(".", "-"), array("", ""), $_REQUEST["usucpf"]);
            $resposta = $SSDWs->getUserInfoByCPFOrCNPJ($cpfOrCnpj);

            // 	Se retornar a classe padrão, o cpf esta cadastrado
            if ($resposta instanceof stdClass) {
                $ssd_cpf_cadastrado = true;
            } else {
                $ssd_cpf_cadastrado = false;
            }

            /*
             *  FIM
             *  Código feito para integrar a autenticação do SIMEC com o SSD
             *  Verifica se o cpf ja esta cadastrado no SSD
             *  Desenvolvido por Alexandre Dourado
             */

            if (!$ssd_cpf_cadastrado) {
                header("Content-Type: text/html; charset=utf-8");
                ob_start();
                // Instanciando Classe de conexão
                $SSDWs = new SSDWsUser($tmpDir, $clientCert, $privateKey, $privateKeyPassword, $trustedCaChain);
                // Efetuando a conexão com o servidor (produção/homologação)
                if ($GLOBALS['USE_PRODUCTION_SERVICES']) {
                    $SSDWs->useProductionSSDServices();
                } else {
                    $SSDWs->useHomologationSSDServices();
                }

                $SSD_senha = @utf8_encode(base64_encode($senhageral));
                $SSD_tipo_pessoa = @utf8_encode("F");
                $SSD_nome = @utf8_encode($_POST["usunome"]);
                $SSD_cpf = @utf8_encode(str_replace(array(".", "-"), array("", ""), $_POST["usucpf"]));
                $SSD_data_nascimento = @utf8_encode("0000-00-00");
                $SSD_email = @utf8_encode($_POST["usuemail"]);
                $SSD_ddd_telefone = @utf8_encode($_POST["usufoneddd"]);
                $SSD_telefone = @utf8_encode($_POST["usufonenum"]);

                // Variavel para inserir os dados no SSD
                $userInfo = "$SSD_senha||$SSD_tipo_pessoa||$SSD_nome||$nome_mae||$SSD_cpf||$rg||$sigla_orgao_expedidor||$orgao_expedidor||$nis||" .
                        "$SSD_data_nascimento||$codigo_municipio_naturalidade||$codigo_nacionalidade||$SSD_email||$email_alternativo||" .
                        "$cep||$endereco||$sigla_uf_cep||$localidade||$bairro||$complemento||$endereco||$SSD_ddd_telefone||$SSD_telefone||" .
                        "$ddd_telefone_alternativo||$telefone_alternativo||$ddd_celular||$celular||$instituicao_trabalho||$lotacao||ssd";

                // Inserindo usuario no SSD
                $resposta = $SSDWs->signUpUser($userInfo);

                if ($resposta != "true") {
                    session_unset();
                    $_SESSION['MSG_AVISO'] = $resposta["erro"];
                    header('location: sigest.php');
                    exit;
                }

                // Incluindo a permissão
                $permissionId = PER_SIMEC;
                $cpfOrCnpj = str_replace(array(".", "-"), array("", ""), $_POST["usucpf"]);

                // $responsibleForChangeCpfOrCnpj deve ser vazio
                $resposta = $SSDWs->includeUserPermissionByCPFOrCNPJ($cpfOrCnpj, $permissionId, $responsibleForChangeCpfOrCnpj);

                if ($resposta != "true") {
                    session_unset();
                    $_SESSION['MSG_AVISO'] = $resposta["erro"];
                    header('location: sigest.php');
                    exit;
                }
            }
        }
        /*
         *  FIM
         *  Código feito para integrar a autenticação do SIMEC com o SSD
         *  Inserir o usuário no BD do SSD e inserir a permissão
         *  Desenvolvido por Alexandre Dourado
         */

        // atribuições requeridas para que a auditoria do sistema funcione
        $_SESSION['sisid'] = 4; # seleciona o sistema de segurança
        $_SESSION['usucpf'] = $cpf;
        $_SESSION['usucpforigem'] = $cpf;

        $tpocod_banco = $tpocod ? (integer) $tpocod : "null";

        if (!$cpf_cadastrado) {
            // insere informações gerais do usuário
            $sql = sprintf(
                    "INSERT INTO seguranca.usuario (
                                            usucpf, usunome, usuemail, usufoneddd, usufonenum,
                                            usufuncao, carid, unicod, usuchaveativacao, regcod,
                                            ususexo, ungcod, ususenha, suscod, orgao,
                                            muncod, tpocod
                                    ) values (
                                            '%s', '%s', '%s', '%s', '%s',
                                            '%s', '%s', '%s', '%s', '%s',
                                            '%s', '%s', '%s', '%s', '%s',
                                            '%s', %s
                                    )", $cpf, str_to_upper($usunome), strtolower($usuemail), $usufoneddd, $usufonenum, $usufuncao, $carid, $unicod, 'f', $regcod, $ususexo, $_POST['ungcod_disable'], md5_encrypt_senha($senhageral, ''), 'P', $orgao, $muncod, $tpocod_banco
            );

            $db->executar($sql);
        }

        // vincula o usuário com o módulo
        $sql = sprintf("INSERT INTO seguranca.usuario_sistema ( usucpf, sisid, pflcod ) values ( '%s', %d, %d )", $cpf, $sisid, $pflcod);
        $db->executar($sql);

        // modifica o status do usuário (no módulo) para pendente
        $descricao = "Usuário solicitou cadastro e apresentou as seguintes observações: " . $htudsc;
        $db->alterar_status_usuario($cpf, 'P', $descricao, $sisid);

        $sql = "SELECT
                             s.sisid, lower(s.sisdiretorio) as sisdiretorio
                            FROM
                             seguranca.sistema s
                            WHERE
                             sisid = " . $sisid . "";

        $sistema = (object) $db->pegaLinha($sql);

        $sistema->sisdiretorio = $sistema->sisid == 14 ? 'cte' : $sistema->sisdiretorio;

        $sql = sprintf("
            SELECT
                CASE WHEN (
                    SELECT
                        TRUE
                    FROM pg_tables
                    WHERE
                        schemaname = '%s' AND
                        tablename  = 'tiporesponsabilidade')
                    THEN
                        TRUE
                    WHEN (
                        SELECT
                            TRUE
                        FROM pg_tables
                        WHERE
                            schemaname='%s'
                            AND tablename = 'tprperfil'
                        )
                    THEN
                        TRUE
                    ELSE 
                        FALSE
                    END
            ;", $sistema->sisdiretorio, $sistema->sisdiretorio);

        $existeTabela = $db->pegaUm($sql);

        if ($existeTabela == 't') {
            $propostos = (array) $_REQUEST["proposto"];

            foreach ($propostos as $chave => $valores) {
                $sql_tpr = "select tprcampo from " . $sistema->sisdiretorio . ".tiporesponsabilidade where tprsigla = '" . $chave . "'";
                $tprcampo = $db->pegaUm($sql_tpr);

                foreach ($valores as $chave => $valor) {
                    $sql_proposta = "insert into seguranca.usuariorespproposta ( urpcampoid, urpcampo, pflcod, usucpf, sisid ) values ( '" . $valor . "', '" . $tprcampo . "', '" . $pflcod . "', '" . $cpf . "', " . $sisid . " )";
                    $db->executar($sql_proposta, false);
                }
            }
        }

        $sql = sprintf("SELECT pflcod FROM seguranca.perfil WHERE sisid=%s and pflpadrao='t'", $sisid);
        $pflcodpadrao = (array) $db->carregarColuna($sql);

        // Se for o Demandas, então envia e-mail para o gestor. -- Atílio Emanuel
        if ($sisid == 44) {
            //dados do usuário solicitante
            $sqlUsu = sprintf("
                SELECT
                    usucpf,
                    usuemail,
                    ususexo,
                    usunome,
                    ususenha
                FROM seguranca.usuario
                WHERE
                    usucpf = '%s'
                ", $cpf);

            $usuariod = (object) $db->pegaLinha($sqlUsu);

            //validando e recuperando descrição do perfil solicitado no cadastro.
            if (!empty($_REQUEST['pflcod'])) {
                $nmPerfil = $db->pegaUm("SELECT pfldsc FROM seguranca.perfil WHERE pflcod = " . $_REQUEST['pflcod']);
            }

            //recuperando dados para enviar email para o gestor #Atilio, somente o mesmo poderá ativar o usuário.
            $emailCopia = "";
            $remetente = array("nome" => SIGLA_SISTEMA, "email" => 'noreply@mec.gov.br');
            $destinatario = array("nome" => SIGLA_SISTEMA, "email" => $_SESSION['email_sistema']);
            $assunto = "Solicitação de acesso";
            $nmusu = !empty($_REQUEST['usunome']) ? ", <b>" . $usuariod->usunome . "</b>" : "";
            $perfil = !empty($nmPerfil) ? ", para o perfil <b>" . $nmPerfil . "</b>" : "";
            $conteudo = "Houve uma solicitação de acesso ao demandas para o CPF: <b>" . formatar_cpf($cpf) . "</b>" . $nmusu . "" . $perfil . " em <b>" . date('d/m/Y H:i:s') . "</b>.";
            # $corpoEmailV3 Variavel inserida dentro do template.
            $corpoEmailV3 = '<p>' . $conteudo . '</p>';
            # $textoEmailV3 é a variavel que terá o template com a msg principal do e-mail.
            include APPRAIZ . "includes/email-template.php";
            enviar_email($remetente, $destinatario, $assunto, $textoEmailV3, $emailCopia);
        }

        // VERIFICA SE HÁ REGRA PARA ENVIO DE EMAIL/SMS
        if ($_REQUEST['pflcod']) {
            $sql = "select r.mreid, r.sisid, mretextoemail, mretextocelular, mreenviaemail, mreenviasms, mretituloemail, mrestatus, mredescricao,
                           p.pfldsc, s.sisabrev, pu.pflcod, pu.usucpf, u.usunome, usuemail, 55 || usufoneddd || usufonenum as celular, usufoneddd, usufonenum
                    from seguranca.mensagemregra r
                        inner join seguranca.mensagemcampo cs on cs.mreid  = r.mreid and cs.mctid = 2 -- Perfil solicitado
                        inner join seguranca.mensagemcampo ca on ca.mreid  = r.mreid and ca.mctid = 1 -- Perfil a ser avisado
                        inner join seguranca.perfil         p on p.pflcod::text  = cs.mcavalor
                        inner join seguranca.sistema        s on s.sisid   = r.sisid
                        inner join seguranca.perfilusuario pu on pu.pflcod::text = ca.mcavalor
                        inner join seguranca.usuario        u on u.usucpf  = pu.usucpf
                    where r.sisid = {$_REQUEST['sisid']}
                    and cs.mcavalor = '{$_REQUEST['pflcod']}'
                    and mretipo = 'A'
                    union
                    select r.mreid, r.sisid, mretextoemail, mretextocelular, mreenviaemail, mreenviasms, mretituloemail, mrestatus, mredescricao,
                           p.pfldsc, s.sisabrev, 0, u.usucpf, u.usunome, usuemail, 55 || usufoneddd || usufonenum as celular, usufoneddd, usufonenum
                    from seguranca.mensagemregra r
                        inner join seguranca.mensagemcampo cs on cs.mreid  = r.mreid and cs.mctid = 2 -- Perfil solicitado
                        inner join seguranca.mensagemcampo ca on ca.mreid  = r.mreid and ca.mctid = 3 -- Usuário a ser avisado
                        inner join seguranca.perfil         p on p.pflcod::text  = cs.mcavalor
                        inner join seguranca.sistema        s on s.sisid   = r.sisid
                        inner join seguranca.usuario        u on u.usucpf  = ca.mcavalor
                    where r.sisid = {$_REQUEST['sisid']}
                    and cs.mcavalor = '{$_REQUEST['pflcod']}'
                    and mretipo = 'A';
                    ";

            $mensagemRegra = $db->carregar($sql);

            $aEnvio = array();
            if ($mensagemRegra) {
                foreach ($mensagemRegra as $regra) {
                    $aEnvio[$regra['mreid']]['assunto'] = $regra['mretituloemail'];
                    $aEnvio[$regra['mreid']]['email'] = $regra['mretextoemail'];
                    $aEnvio[$regra['mreid']]['sms'] = $regra['mretextocelular'];
                    $aEnvio[$regra['mreid']]['sistema'] = $regra['sisabrev'];
                    $aEnvio[$regra['mreid']]['perfil'] = $regra['pfldsc'];

                    if ($regra['mreenviaemail'] == 't') {
                        $aEnvio[$regra['mreid']]['emails'][$regra['usuemail']] = $regra['usuemail'];
                    }

                    if ($regra['mreenviasms'] == 't') {
                        // Verifica se é número de celular
                        $inicioTelefone = substr(trim($regra['usufonenum']), 0, 1);
                        if (in_array($inicioTelefone, array(7, 8, 9))) {
                            $celular = str_replace(array('-', '.', ' '), '', $regra['celular']);
                            $aEnvio[$regra['mreid']]['celular'][$celular] = $celular;
                        }
                    }
                }
            }

            if (count($aEnvio)) {
                foreach ($aEnvio as $envioRegra) {
                    if (isset($envioRegra['emails'])) {
                        $remetente = array("nome" => SIGLA_SISTEMA, "email" => "noreply@mec.gov.br");

                        $destinatariosBcc = $envioRegra['emails'];
                        $assunto = $envioRegra['assunto'];
                        $mensagem = '<p>' . $envioRegra['email'] . '</p><br /><br /><br />';
                        $linkUsuario = URL_SISTEMA . "planacomorc/planacomorc.php?modulo=sistema/usuario/cadusuario&acao=A&usucpf=" . str_replace(array(".", "-"), '', $_POST['usucpf']);

                        $mensagem .= "
                            <p><b>Dados da Solicitação:</b> <a title='Clique aqui para ir a tela de aprovar o usuário.' href='{$linkUsuario}'>{$linkUsuario}</a></p>
                            <br />
                            <p><b>Módulo:</b> {$envioRegra['sistema']}</p>
                            <p><b>Perfil Desejado:</b> {$envioRegra['perfil']}</p>
                            <br />
                            <p><b>Nome:</b> {$_POST['usunome']}</p>
                            <p><b>CPF:</b> {$_POST['usucpf']}</p>
                            <p><b>E-mail:</b> {$_POST['usuemail']}</p>
                            <p><b>Telefone:</b> ({$_POST['usufoneddd']}) {$_POST['usufonenum']}</p>
                            <br />
                            <p>
                            Atenciosamente,
                            <br />Equipe " . SIGLA_SISTEMA . ".
                            </p>
                        ";
                        # $corpoEmailV3 Variavel inserida dentro do template.
                        $corpoEmailV3 = $mensagem;
                        # $textoEmailV3 é a variavel que terá o template com a msg principal do e-mail.
                        include APPRAIZ . "includes/email-template.php";

                        $aDestinatarios = array();
                        enviar_email($remetente, $aDestinatarios, $assunto, $textoEmailV3, null, $destinatariosBcc);
                    }
                    if (isset($envioRegra['celular'])) {

                        $aCelularEnvio = $envioRegra['celular'];
                        $conteudo = $envioRegra['sms'];

                        $sms = new Sms();
                        $sms->enviarSms($aCelularEnvio, $conteudo);
                    }
                }
            }
        }

        if ($pflcodpadrao) {
            // carrega os dados da conta do usuário
            $sql = sprintf("
                SELECT
                    usucpf, usuemail, ususexo, usunome, ususenha
                FROM seguranca.usuario
                WHERE
                    usucpf = '%s'", $cpf
            );

            $usuariod = (object) $db->pegaLinha($sql);

            $justificativa = "Ativação automática de usuário pelo sistema";
            $suscod = "A";
            $db->alterar_status_usuario($usuariod->usucpf, $suscod, $justificativa, $sisid);

            //deleta os perfis
            $sql = sprintf("DELETE FROM seguranca.perfilusuario WHERE usucpf = '%s' AND pflcod IN ( SELECT p.pflcod FROM seguranca.perfil p WHERE p.sisid = %d )", $usuariod->usucpf, $sisid);
            $db->executar($sql);

            // inclui o perfil
            foreach ($pflcodpadrao as $p) {
                $sql = sprintf("INSERT INTO seguranca.perfilusuario ( usucpf, pflcod ) VALUES ( '%s', %d )", $usuariod->usucpf, $p);
                $db->executar($sql);
            }

            $db->commit();

            $_REQUEST['usucpf'] = formatar_cpf($usuariod->usucpf);
            $_POST['ususenha'] = md5_decrypt_senha($usuariod->ususenha, '');
            $_SESSION['logincadastro'] = true;

            include APPRAIZ . "includes/autenticar.inc";
            exit();
        } else {
            // obtém dados da instituição
            $sql = "select ittcod, ittemail_inclusao_usuario, ittemail, itttelefone1, itttelefone2, ittddd, ittfax, ittsistemasigla from public.instituicao where ittstatus = 'A'";
            $instituicao = (object) $db->pegaLinha($sql);
            if ($instituicao->ittcod) {
                $sqlPegaEmailSistema = "select sisemail	from seguranca.sistema where sisid = " . ( (integer) $sisid );
                $emailCopia = trim($db->pegaUm($sqlPegaEmailSistema));
                $sql = "SELECT sisemail, sistel, sisfax from seguranca.sistema s where s.sisstatus='A' and sismostra='t' AND sisid = $sisid";
                $sistema = (object) $db->pegaLinha($sql);

                // envia email de confirmação
                $remetente = array("nome" => $instituicao->ittsistemasigla, "email" => $emailCopia);
                $destinatario = $usuemail;
                $assunto = "Solicitação de Cadastro no " . SIGLA_SISTEMA;
                $conteudo = sprintf("%s<p>%s %s ou no(s) telefone(s): %s Fax %s</p>%s", $ususexo == 'M' ? 'Prezado Sr.' : 'Prezada Sra.', $instituicao->ittemail_inclusao_usuario, " este mesmo endereço ", $sistema->sistel, $sistema->sisfax, $cpf_cadastrado ? '*Usuário já cadastrado' : '*Novo Usuário'
                );
                # $corpoEmailV3 Variavel inserida dentro do template.
                $corpoEmailV3 = '<p>' . $conteudo . '</p>';
                # $textoEmailV3 é a variavel que terá o template com a msg principal do e-mail.
                include APPRAIZ . "includes/email-template.php";
                enviar_email($remetente, $destinatario, $assunto, $textoEmailV3, $emailCopia);
            }
            // leva o usuário para a página de login e exibe confirmação
            $db->commit();

            $sisabrev = $db->pegaUm("SELECT sisabrev FROM seguranca.sistema WHERE sisid = " . $sisid);
            $mensagem = sprintf("Sua solicitação de cadastro para acesso ao módulo %s foi registrada e será analisada pelo setor responsável. Em breve você receberá maiores informações.", $sisabrev);
            $_SESSION['MSG_AVISO'][] = $mensagem;
            header("Location: sigest.php");

            exit();
        }
    }
?>
<!doctype html>
<html lang="pt-BR">
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
        <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
        <link rel="profile" href="http://gmpg.org/xfn/11">
        <link href="https://fonts.googleapis.com/css?family=Open+Sans:400,800" rel="stylesheet">

        <script>var et_site_url = 'http://gestaoestrategica.cultura.gov.br'; var et_post_id = '39'; function et_core_page_resource_fallback(a, b){"undefined" === typeof b && (b = a.sheet.cssRules && 0 === a.sheet.cssRules.length); b && (a.onerror = null, a.onload = null, a.href?a.href = et_site_url + "/?et_core_page_resource=" + a.id + et_post_id:a.src && (a.src = et_site_url + "/?et_core_page_resource=" + a.id + et_post_id))}
        </script><title>Gestão Estratégica &#8211; Acompanhar e avaliar a execução das ações pertinentes ao MinC</title>
        <link rel='dns-prefetch' href='//base-wp.cultura.gov.br' />
        <link rel='dns-prefetch' href='//barra.brasil.gov.br' />
        <link rel='dns-prefetch' href='//s.w.org' />
        <link rel="alternate" type="application/rss+xml" title="Feed para Gestão Estratégica &raquo;" href="http://gestaoestrategica.cultura.gov.br/feed/" />
        <link rel="alternate" type="application/rss+xml" title="Feed de comentários para Gestão Estratégica &raquo;" href="http://gestaoestrategica.cultura.gov.br/comments/feed/" />
        <script type="text/javascript">
            window._wpemojiSettings = {"baseUrl":"https:\/\/s.w.org\/images\/core\/emoji\/11\/72x72\/", "ext":".png", "svgUrl":"https:\/\/s.w.org\/images\/core\/emoji\/11\/svg\/", "svgExt":".svg", "source":{"concatemoji":"http:\/\/gestaoestrategica.cultura.gov.br\/wp-includes\/js\/wp-emoji-release.min.js?ver=4.9.8"}};
            !function(a, b, c){function d(a, b){var c = String.fromCharCode; l.clearRect(0, 0, k.width, k.height), l.fillText(c.apply(this, a), 0, 0); var d = k.toDataURL(); l.clearRect(0, 0, k.width, k.height), l.fillText(c.apply(this, b), 0, 0); var e = k.toDataURL(); return d === e}function e(a){var b; if (!l || !l.fillText)return!1; switch (l.textBaseline = "top", l.font = "600 32px Arial", a){case"flag":return!(b = d([55356, 56826, 55356, 56819], [55356, 56826, 8203, 55356, 56819])) && (b = d([55356, 57332, 56128, 56423, 56128, 56418, 56128, 56421, 56128, 56430, 56128, 56423, 56128, 56447], [55356, 57332, 8203, 56128, 56423, 8203, 56128, 56418, 8203, 56128, 56421, 8203, 56128, 56430, 8203, 56128, 56423, 8203, 56128, 56447]), !b); case"emoji":return b = d([55358, 56760, 9792, 65039], [55358, 56760, 8203, 9792, 65039]), !b}return!1}function f(a){var c = b.createElement("script"); c.src = a, c.defer = c.type = "text/javascript", b.getElementsByTagName("head")[0].appendChild(c)}var g, h, i, j, k = b.createElement("canvas"), l = k.getContext && k.getContext("2d"); for (j = Array("flag", "emoji"), c.supports = {everything:!0, everythingExceptFlag:!0}, i = 0; i < j.length; i++)c.supports[j[i]] = e(j[i]), c.supports.everything = c.supports.everything && c.supports[j[i]], "flag" !== j[i] && (c.supports.everythingExceptFlag = c.supports.everythingExceptFlag && c.supports[j[i]]); c.supports.everythingExceptFlag = c.supports.everythingExceptFlag && !c.supports.flag, c.DOMReady = !1, c.readyCallback = function(){c.DOMReady = !0}, c.supports.everything || (h = function(){c.readyCallback()}, b.addEventListener?(b.addEventListener("DOMContentLoaded", h, !1), a.addEventListener("load", h, !1)):(a.attachEvent("onload", h), b.attachEvent("onreadystatechange", function(){"complete" === b.readyState && c.readyCallback()})), g = c.source || {}, g.concatemoji?f(g.concatemoji):g.wpemoji && g.twemoji && (f(g.twemoji), f(g.wpemoji)))}(window, document, window._wpemojiSettings);
        </script>
        <style type="text/css">
            img.wp-smiley,
            img.emoji {
                display: inline !important;
                border: none !important;
                box-shadow: none !important;
                height: 1em !important;
                width: 1em !important;
                margin: 0 .07em !important;
                vertical-align: -0.1em !important;
                background: none !important;
                padding: 0 !important;
            }
        </style>
        
        <link rel='stylesheet' id='twitter-track-fix-css'  href='http://gestaoestrategica.cultura.gov.br/wp-content/mu-plugins/css/twitter-tracker.css?ver=4.9.8' type='text/css' media='all' />
        <link rel='stylesheet' id='contact-form-7-css'  href='http://gestaoestrategica.cultura.gov.br/wp-content/plugins/contact-form-7/includes/css/styles.css?ver=4.9.1' type='text/css' media='all' />
        <link rel='stylesheet' id='WpBarraBrasil-css'  href='http://gestaoestrategica.cultura.gov.br/wp-content/plugins/wp-barra-brasil//frontend/css/WpBarraBrasil.css?ver=4.9.8' type='text/css' media='all' />
        <link rel='stylesheet' id='gewp-styles-css'  href='http://gestaoestrategica.cultura.gov.br/wp-content/plugins/ge-wp/assets/gewp-styles.css?ver=4.9.8' type='text/css' media='all' />
        <link rel='stylesheet' id='pp-wp-style-css'  href='http://gestaoestrategica.cultura.gov.br/wp-content/themes/pp-wp/assets/stylesheets/dist/bundle.min.css?ver=4.9.8' type='text/css' media='all' />
        <link rel='stylesheet' id='et-builder-modules-style-css'  href='http://gestaoestrategica.cultura.gov.br/wp-content/plugins/divi-builder/includes/builder/styles/frontend-builder-plugin-style.min.css?ver=2.0.67' type='text/css' media='all' />
        <link rel='stylesheet' id='dashicons-css'  href='http://gestaoestrategica.cultura.gov.br/wp-includes/css/dashicons.min.css?ver=4.9.8' type='text/css' media='all' />
        <script type='text/javascript' src='http://gestaoestrategica.cultura.gov.br/wp-includes/js/jquery/jquery.js?ver=1.12.4'></script>
        <script type='text/javascript' src='http://gestaoestrategica.cultura.gov.br/wp-includes/js/jquery/jquery-migrate.min.js?ver=1.4.1'></script>
        <script type='text/javascript' src='http://base-wp.cultura.gov.br/wp-content/mu-plugins/includes/widgets/js/facebook-like.js?ver=4.9.8'></script>
        <script type='text/javascript' src='http://gestaoestrategica.cultura.gov.br/wp-content/plugins/ge-wp/assets/masonry.pkgd.min.js?ver=4.9.8'></script>
        <script type='text/javascript' src='http://gestaoestrategica.cultura.gov.br/wp-content/plugins/ge-wp/assets/gewp-scripts.js?ver=4.9.8'></script>
        <script type='text/javascript'>
                            /* <![CDATA[ */
                            var campaign_common = {"label":{"MeusProjetos":"Nome de listagem dos blogs por usu\u00e1rio na barra superior administrativa", "AdministrarProjetos":""}, "value":{"MeusProjetos":"Meus projetos", "AdministrarProjetos":"Administrar projetos"}};
                            /* ]]> */
        </script>
        <script type='text/javascript' src='http://gestaoestrategica.cultura.gov.br/wp-content/mu-plugins/js/campaign_common.js?ver=4.9.8'></script>
        <link rel='https://api.w.org/' href='http://gestaoestrategica.cultura.gov.br/wp-json/' />
        <link rel="EditURI" type="application/rsd+xml" title="RSD" href="http://gestaoestrategica.cultura.gov.br/xmlrpc.php?rsd" />
        <link rel="wlwmanifest" type="application/wlwmanifest+xml" href="http://gestaoestrategica.cultura.gov.br/wp-includes/wlwmanifest.xml" /> 
        <link rel="canonical" href="http://gestaoestrategica.cultura.gov.br/" />
        <link rel='shortlink' href='http://gestaoestrategica.cultura.gov.br/' />
        <link rel="alternate" type="application/json+oembed" href="http://gestaoestrategica.cultura.gov.br/wp-json/oembed/1.0/embed?url=http%3A%2F%2Fgestaoestrategica.cultura.gov.br%2F" />
        <link rel="alternate" type="text/xml+oembed" href="http://gestaoestrategica.cultura.gov.br/wp-json/oembed/1.0/embed?url=http%3A%2F%2Fgestaoestrategica.cultura.gov.br%2F&#038;format=xml" />
        <script src='http://base-wp.cultura.gov.br/?dm=523d84ef05dfbcc2d4b9334fd6576d26&amp;action=load&amp;blogid=33&amp;siteid=1&amp;t=170047204&amp;back=http%3A%2F%2Fgestaoestrategica.cultura.gov.br%2F' type='text/javascript'></script>		<!-- Piwik -->
        <script type="text/javascript">
            var _paq = _paq || [];
            _paq.push(['trackPageView']);
            _paq.push(['enableLinkTracking']);
            (function() {
            var u = "//analise.cultura.gov.br/";
            _paq.push(['setTrackerUrl', u + 'piwik.php']);
            _paq.push(['setSiteId', 36]);
            var d = document, g = d.createElement('script'), s = d.getElementsByTagName('script')[0];
            g.type = 'text/javascript'; g.async = true; g.defer = true; g.src = u + 'piwik.js'; s.parentNode.insertBefore(g, s);
            })();
        </script>
        
	<!-- Custom Scripts -->
        <script type="text/javascript" src="../includes/funcoes.js?v=1"></script>
        
    <noscript><p><img src="//analise.cultura.gov.br/piwik.php?idsite=36" style="border:0;" alt="" /></p></noscript>
    <!-- End Piwik Code -->
    <link rel="icon" href="http://gestaoestrategica.cultura.gov.br/wp-content/uploads/sites/33/2018/02/cropped-strategy-32x32.png" sizes="32x32" />
    <link rel="icon" href="http://gestaoestrategica.cultura.gov.br/wp-content/uploads/sites/33/2018/02/cropped-strategy-192x192.png" sizes="192x192" />
    <link rel="apple-touch-icon-precomposed" href="http://gestaoestrategica.cultura.gov.br/wp-content/uploads/sites/33/2018/02/cropped-strategy-180x180.png" />
    <meta name="msapplication-TileImage" content="http://gestaoestrategica.cultura.gov.br/wp-content/uploads/sites/33/2018/02/cropped-strategy-270x270.png" />
    <!-- Custom Fonts -->
    <link href="library/font-awesome/css/font-awesome.min.css" rel="stylesheet" type="text/css">
    <link href="http://fonts.googleapis.com/css?family=Source+Sans+Pro:300,400,700,300italic,400italic,700italic" rel="stylesheet" type="text/css">
    
    <!-- Custom CSS dessa tela -->
    <link rel='stylesheet' id="wp-custom-css" href='<?php echo URL_SISTEMA. 'painel/css/sisgest.css'; ?>' type='text/css' />
</head>
<style>
label {
    font-size: 14px !important;
    font-weight: bold !important;
}
font {
    font-size: 14px;
    font-weight: bold;
}
</style>
<body class="home page-template page-template-page-fluid page-template-page-fluid-php page page-id-39 green-theme  et-pb-theme-portal padrão wp et_minified_js et_minified_css et_divi_builder">

    <div id="page" class="site">

        <a class="skip-link screen-reader-text sr-only" href="#content">Pular para o conteúdo</a>

        <header id="header" class="site-header">
            <div class="container">
                <div class="row">
                    <div class="col-md-6 col-lg-8 cf-1">

                        <ul id="shortcut-bar" class="d-none d-md-block">
                            <li>
                                <a accesskey="1" href="#main" id="main-link">
                                    Ir para o conteúdo
                                    <span>1</span>
                                </a>
                            </li>
                            <li>
                                <a accesskey="2" href="#main-navbar" id="navigation-link">
                                    Ir para o menu
                                    <span>2</span>
                                </a>
                            </li>
                            <li>
                                <a accesskey="3" href="#main-search" id="main-search-link">
                                    Ir para a busca
                                    <span>3</span>
                                </a>
                            </li>
                            <li class="last-item">
                                <a accesskey="4" href="#footer" id="footer-link">
                                    Ir para o rodapé
                                    <span>4</span>
                                </a>
                            </li>
                        </ul>

                        <h1 class="site-title">
                            <a href="http://gestaoestrategica.cultura.gov.br/" rel="home">
                                Gestão Estratégica                        </a>
                        </h1>
                        <span class="site-description">Acompanhar e avaliar a execução das ações pertinentes ao minc</span>
                    </div>

                    <div class="col-md-6 col-lg-4 cf-2">

                        <ul id="accessibility">
                            <li>
                                <a href="http://gestaoestrategica.cultura.gov.br/acessibilidade" title="Acessibilidade" accesskey="5">Acessibilidade</a>
                            </li>
                            <li>
                                <a href="#" title="Alto Contraste" accesskey="6" id="high-contrast">Alto Contraste</a>
                            </li>
                            <li>
                                <a href="http://gestaoestrategica.cultura.gov.br/mapa-do-site" title="Mapa do Site" accesskey="7">Mapa do Site</a>
                            </li>
                        </ul>

                    </div>
                </div>
            </div>

            <div class="service-bar-container">
                <div class="container">
                    <div class="row">
                        <div class="col">
                            <nav class="navbar navbar-expand-md navbar-dark">
                                <a class="navbar-brand invisible d-md-none" href="#">Menu de navegação</a>
                                <button class="navbar-toggler" type="button" data-toggle="collapse" data-target="#main-navbar" aria-controls="main-navbar" aria-expanded="false" aria-label="Toggle navigation">
                                    <span class="navbar-toggler-icon"></span>
                                </button>
                                <div id="main-navbar" class="collapse navbar-collapse"><ul id="menu-menu-principal" class="service-bar ml-auto nav navbar-nav"><li itemscope="itemscope" itemtype="https://www.schema.org/SiteNavigationElement" id="menu-item-45" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-home current-menu-item page_item page-item-39 current_page_item menu-item-45 active"><a title="Início" href="http://gestaoestrategica.cultura.gov.br/" class="nav-link">Início</a></li>
                                        <li itemscope="itemscope" itemtype="https://www.schema.org/SiteNavigationElement" id="menu-item-66" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-66"><a title="Ações Estratégicas" href="http://gestaoestrategica.cultura.gov.br/acoes-estrategicas/" class="nav-link">Ações Estratégicas</a></li>
                                        <li itemscope="itemscope" itemtype="https://www.schema.org/SiteNavigationElement" id="menu-item-65" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-has-children menu-item-65 dropdown"><a title="Gerenciamento de Processo" href="#" data-toggle="dropdown" class="dropdown-toggle" aria-haspopup="true">Gerenciamento de Processo <span class="caret"></span></a>
                                            <ul role="menu" class=" dropdown-menu" >
                                                <li itemscope="itemscope" itemtype="https://www.schema.org/SiteNavigationElement" id="menu-item-61" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-61"><a title="Modelo de Gestão" href="http://gestaoestrategica.cultura.gov.br/modelo-de-gestao/" class="nav-link">Modelo de Gestão</a></li>
                                                <li itemscope="itemscope" itemtype="https://www.schema.org/SiteNavigationElement" id="menu-item-106" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-106"><a title="Metodologia" href="http://gestaoestrategica.cultura.gov.br/metodologia/" class="nav-link">Metodologia</a></li>
                                                <li itemscope="itemscope" itemtype="https://www.schema.org/SiteNavigationElement" id="menu-item-105" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-105"><a title="Cadeia de Valor" href="http://gestaoestrategica.cultura.gov.br/cadeia-de-valor/" class="nav-link">Cadeia de Valor</a></li>
                                                <li itemscope="itemscope" itemtype="https://www.schema.org/SiteNavigationElement" id="menu-item-104" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-104"><a title="Estratégias" href="http://gestaoestrategica.cultura.gov.br/estrategias/" class="nav-link">Estratégias</a></li>
                                            </ul>
                                        </li>
                                        <li itemscope="itemscope" itemtype="https://www.schema.org/SiteNavigationElement" id="menu-item-73" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-has-children menu-item-73 dropdown"><a title="Plano de Diretrizes e Metas" href="#" data-toggle="dropdown" class="dropdown-toggle" aria-haspopup="true">Plano de Diretrizes e Metas <span class="caret"></span></a>
                                            <ul role="menu" class=" dropdown-menu" >
                                                <li itemscope="itemscope" itemtype="https://www.schema.org/SiteNavigationElement" id="menu-item-103" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-103"><a title="Mapa Estratégico" href="http://gestaoestrategica.cultura.gov.br/mapa-estrategico/" class="nav-link">Mapa Estratégico</a></li>
                                                <li itemscope="itemscope" itemtype="https://www.schema.org/SiteNavigationElement" id="menu-item-101" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-101"><a title="Fonte de recursos" href="http://gestaoestrategica.cultura.gov.br/orcamento/" class="nav-link">Fonte de recursos</a></li>
                                                <li itemscope="itemscope" itemtype="https://www.schema.org/SiteNavigationElement" id="menu-item-808" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-808"><a title="Plano de Ação" href="http://gestaoestrategica.cultura.gov.br/plano-de-acao/" class="nav-link">Plano de Ação</a></li>
                                            </ul>
                                        </li>
                                        <li itemscope="itemscope" itemtype="https://www.schema.org/SiteNavigationElement" id="menu-item-842" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-has-children menu-item-842 dropdown"><a title="Resultados" href="#" data-toggle="dropdown" class="dropdown-toggle" aria-haspopup="true">Resultados <span class="caret"></span></a>
                                            <ul role="menu" class=" dropdown-menu" >
                                                <li itemscope="itemscope" itemtype="https://www.schema.org/SiteNavigationElement" id="menu-item-102" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-102"><a title="Plano de Ação &#8211; Acompanhamento" href="http://gestaoestrategica.cultura.gov.br/resultados/" class="nav-link">Plano de Ação &#8211; Acompanhamento</a></li>
                                                <li itemscope="itemscope" itemtype="https://www.schema.org/SiteNavigationElement" id="menu-item-843" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-843"><a title="Indicadores" href="http://gestaoestrategica.cultura.gov.br/indicadores/" class="nav-link">Indicadores</a></li>
                                            </ul>
                                        </li>
                                        <li itemscope="itemscope" itemtype="https://www.schema.org/SiteNavigationElement" id="menu-item-62" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-has-children menu-item-62 dropdown"><a title="Fontes de Recursos" href="#" data-toggle="dropdown" class="dropdown-toggle" aria-haspopup="true">Fontes de Recursos <span class="caret"></span></a>
                                            <ul role="menu" class=" dropdown-menu" >
                                                <li itemscope="itemscope" itemtype="https://www.schema.org/SiteNavigationElement" id="menu-item-680" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-680"><a title="Orçamento Geral da União (OGU)" href="http://gestaoestrategica.cultura.gov.br/orcamento-e-gestao/" class="nav-link">Orçamento Geral da União (OGU)</a></li>
                                                <li itemscope="itemscope" itemtype="https://www.schema.org/SiteNavigationElement" id="menu-item-99" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-99"><a title="Lei Federal de Incentivo à Cultura" href="http://gestaoestrategica.cultura.gov.br/renuncia-lei-rouanet/" class="nav-link">Lei Federal de Incentivo à Cultura</a></li>
                                                <li itemscope="itemscope" itemtype="https://www.schema.org/SiteNavigationElement" id="menu-item-98" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-98"><a title="Fundo Setorial do Audiovisual (FSA)" href="http://gestaoestrategica.cultura.gov.br/fsa/" class="nav-link">Fundo Setorial do Audiovisual (FSA)</a></li>
                                                <li itemscope="itemscope" itemtype="https://www.schema.org/SiteNavigationElement" id="menu-item-96" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-96"><a title="Emendas Parlamentares e  PAC ? Céu das Artes" href="http://gestaoestrategica.cultura.gov.br/pac-ceu-das-artes/" class="nav-link">Emendas Parlamentares e  PAC ? Céu das Artes</a></li>
                                                <li itemscope="itemscope" itemtype="https://www.schema.org/SiteNavigationElement" id="menu-item-97" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-97"><a title="PAC ? Cidades Históricas (Iphan)" href="http://gestaoestrategica.cultura.gov.br/pac-iphan/" class="nav-link">PAC ? Cidades Históricas (Iphan)</a></li>
                                            </ul>
                                        </li>
                                        <li itemscope="itemscope" itemtype="https://www.schema.org/SiteNavigationElement" id="menu-item-493" class="menu-item menu-item-type-custom menu-item-object-custom menu-item-493"><a title="Adicionar nova ação" href="http://siminc2.cultura.gov.br/" class="nav-link">Adicionar nova ação</a></li>
                                    </ul>
                                </div>
                            </nav>
                        </div>
                    </div>
                </div>
            </div>
            <div class="custom-header-bg">
                <img src="http://gestaoestrategica.cultura.gov.br/wp-content/uploads/sites/33/2018/02/Group-2-3.png" width="556" height="168" alt="Gestão Estratégica" srcset="http://gestaoestrategica.cultura.gov.br/wp-content/uploads/sites/33/2018/02/Group-2-3.png 556w, http://gestaoestrategica.cultura.gov.br/wp-content/uploads/sites/33/2018/02/Group-2-3-300x91.png 300w" sizes="(max-width: 556px) 100vw, 556px" />
            </div>
        </header>

        <main id="main" class="site-main">

            <div class="container-fluid">
                <div class="row ">
                </div>
                <div class="row ">
                    <div class="col-lg-12">
                        <article id="login-form-page" class="hentry" style="margin: 30px 0;">
                            <div class="entry-content">
                                <div class="container">
                                    <div class="row">
                                        <div class="col-md-12">
                                            <h2 class="arrow-down-blue">SIGEST</h2>
                                        </div>
                                        <div class="col-md-12">
                                            <div class="box-area">
                                                <!-- Mensagens de retorno de autenticação para os usuários -->
                                                <?php if ($_SESSION['MSG_AVISO']): ?>
                                                    <div class="row">
                                                        <div class="col-md-10 offset-md-1">
                                                            <div class="alert alert-danger" style="font-size: 14px; line-height: 20px;">
                                                                <button type="button" class="close" data-dismiss="alert" aria-hidden="true">&times;</button>
                                                                <i class="fa fa-bell"></i> <?php echo implode("<br />", (array) $_SESSION['MSG_AVISO']); ?>
                                                            </div>
                                                        </div>
                                                        <?php $_SESSION['MSG_AVISO'] = array(); ?>
                                                    </div>
                                                <?php endif; ?>
                                                <div class="row">
                                                    <div class="col-md-10 offset-md-1 box-area-inner">
                                                        <h3><span class="glyphicon glyphicon-user"></span> Ficha de solicitação de cadastro de usuários</h3>
                                                        <hr />
                                                        
                                                        <form class="" method="post" name="formulario">
                                                            <input type="hidden" name="formulario" value="1" />
                                                            <div class="form-group">
                                                                <div class="col-sm-12">
                                                                    <label class="control-label" for="sisid_modid">Módulo:</label>
                                                                    <?php
                                                                        # Recupera todos os sistemas cadastrados
                                                                        $sql = "
                                                                            SELECT
                                                                                s.sisid AS codigo,
                                                                                s.sisabrev AS descricao
                                                                            FROM seguranca.sistema s
                                                                            WHERE
                                                                                s.sisstatus = 'A'
                                                                                AND sismostra = 't'
                                                                            ORDER BY
                                                                                descricao";

                                                                        $sistemas = $db->carregar($sql);

                                                                        $select = '';

                                                                        if ($sistemas) {
                                                                            $disabled = 'disabled="disabled"';

                                                                            $select .= '<select name="sisid_modid" ' . $disabled . ' class="chosen-select" style="width:100%;" onchange="sel_modulo(this);">';
                                                                            $select .= '<option value="">Selecione...</option>';

                                                                            foreach ($sistemas as $sis) {
                                                                                $sql = "
                                                                                    SELECT
                                                                                        m.modid AS codigo,
                                                                                        m.modtitulo as descricao
                                                                                    FROM seguranca.modulo m
                                                                                    WHERE
                                                                                        m.sisid = {$sis['codigo']}
                                                                                        AND m.modstatus = 'A'";
                                                                                $modulos = $db->carregar($sql);

                                                                                if ($modulos) {
                                                                                    $select .= '<optgroup id="' . $sis['codigo'] . '" label="' . $sis['descricao'] . '">';

                                                                                    foreach ($modulos as $modulo) {
                                                                                        $selected = '';

                                                                                        if ($modid) {
                                                                                            if ($modid == $modulo['codigo']) {
                                                                                                $selected = 'selected="selected"';
                                                                                            }
                                                                                        }

                                                                                        $select .= '<option value="' . $modulo['codigo'] . '" ' . $selected . '>' . $modulo['descricao'] . '</option>';
                                                                                    }

                                                                                    $select .= '</optgroup>';
                                                                                } else {
                                                                                    $selected = '';

                                                                                    if (!$modid && $sisid) {
                                                                                        if ($sisid == $sis['codigo']) {
                                                                                            $selected = 'selected="selected"';
                                                                                        }
                                                                                    }

                                                                                    $select .= '<optgroup id="" label="' . $sis['descricao'] . '">';
                                                                                    $select .= '<option value="' . $sis['codigo'] . '" ' . $selected . '>' . $sis['descricao'] . '</option>';
                                                                                    $select .= '</optgroup>';
                                                                                }
                                                                            }
                                                                            $select .= '</select>';
                                                                        }

                                                                        echo $select;
                                                                    ?>
                                                                    <input type="hidden" name="sisid" id="sisid" value="<?= $sisid ?>" />
                                                                    <input type="hidden" name="modid" id="modid" value="<?= $modid ?>" />
                                                                </div>
                                                            </div>
                                                            <?php if ($sistema->sisid) : ?>
                                                                <div class="form-group">
                                                                    <div class="col-sm-12">
                                                                        <div class="sistema-texto"  style="text-align: justify">
                                                                            <h2><?php echo $sistema->sisdsc ?></h2><br/>
                                                                            <p><?php echo $sistema->sisfinalidade ?></p>
                                                                            <ul style="margin-top: 10px;">
                                                                                <li><i class="fa fa-bullseye"></i>&nbsp;&nbsp;&nbsp;Público-Alvo: <?php echo $sistema->sispublico ?><br></li>
                                                                                <li><i class="fa fa-cubes"></i> Sistemas Relacionados: <?php echo $sistema->sisrelacionado ?></li>
                                                                            </ul>
                                                                        </div>
                                                                    </div>
                                                                </div>
                                                                <input type="hidden" name="usucpf" value="<?php echo $usucpf; ?>" />
                                                            <?php endif; ?>

                                                            <div class="form-group">
                                                                <div class="col-sm-12">
                                                                    <div class="control-label">
                                                                        <label class="control-label" for="pflcod">Perfil</label>
                                                                    </div>
                                                                    <div class="control-input">
                                                                        <?php
                                                                            $pflcod = $_REQUEST['pflcod'];
                                                                            require_once APPRAIZ. 'seguranca/modulos/sistema/usuario/incperfilusuario.inc';
                                                                        ?>
                                                                    </div>
                                                                </div>
                                                            </div>

                                                            <div class="form-group">
                                                                <div class="col-sm-12">
                                                                    <div class="control-label">
                                                                        <label class="control-label" for="usunome">Nome</label>
                                                                    </div>
                                                                    <div class="control-input">
                                                                        <?php
                                                                            $options = array
                                                                                (
                                                                                'value' => $usunome,
                                                                                'name' => 'usunome',
                                                                                'obrig' => $obrig,
                                                                                'complemento' => 'class="form-control"',
                                                                                'habil' => $editavel,
                                                                                'size' => '50',
                                                                                'max' => '50'
                                                                            );
                                                                            echo campo_texto($options);
                                                                        ?>
                                                                    </div>
                                                                </div>
                                                            </div>

                                                            <div class="form-group">
                                                                <div class="col-sm-12">
                                                                    <div class="control-input">
                                                                        <input id="ususexo" type="radio" name="ususexo" value="M" <?= ($ususexo == 'M' ? "CHECKED" : "") ?> <?= $cpf_cadastrado ? 'disabled="disabled"' : '' ?> /> <label style="margin-top: -1px; vertical-align: top; padding-right: 20px;"> Masculino</label>
                                                                        <input id="ususexo" type="radio" name="ususexo" value="F" <?= ($ususexo == 'F' ? "CHECKED" : "") ?>	<?= $cpf_cadastrado ? 'disabled="disabled"' : '' ?> /> <label style="margin-top: -1px; vertical-align: top;"> Feminino</label>
                                                                    </div>
                                                                </div>
                                                            </div>

                                                            <div class="form-group">
                                                                <div class="col-sm-12">
                                                                    <div class="control-label">
                                                                        <label class="control-label" for="regcod">UF</label>
                                                                    </div>
                                                                    <div class="control-input">
                                                                        <?php
                                                                            $sql = "SELECT regcod AS codigo, regcod||' - '||descricaouf AS descricao FROM uf WHERE codigoibgeuf IS NOT NULL ORDER BY 2";
                                                                            $regcod = $regcod ? $regcod : $configPadrao->estufPadrao;
                                                                            $muncod = $muncod ? $muncod : $configPadrao->muncodPadrao;
                                                                            $db->monta_combo("regcod", $sql, $editavel, "&nbsp;", 'listar_municipios', '', '', '', 'S', 'regcod', '', '', '', '', 'chosen-select');
                                                                        ?>
                                                                    </div>
                                                                </div>
                                                            </div>

                                                            <div class="form-group">
                                                                <div class="col-sm-12">
                                                                    <div class="control-label">
                                                                        <label for="regcod">Município</label>
                                                                    </div>
                                                                    <div class="control-input" id="muncod_on" style="display:<?= (($regcod && $muncod) ? 'block' : 'none') ?>;">
                                                                        <?php
                                                                        if ($regcod && $muncod) {
                                                                            $sql = "
                                                                                SELECT
                                                                                    muncod AS codigo,
                                                                                    mundescricao AS descricao
                                                                                FROM territorios.municipio
                                                                                WHERE
                                                                                    estuf = '{$regcod}'
                                                                                ORDER BY
                                                                                    mundescricao ASC";

                                                                            $db->monta_combo("muncod", $sql, 'S', 'Selecione um município', '', '', '', '200', 'S', 'muncod', '', '', '', '', 'chosen-select');
                                                                        } else {
                                                                            echo '<select name=\'muncod\' id=\'muncod\' class=\'chosen-select\' style=\'width:170px;\'><option value="">Selecione um município</option></select>';
                                                                        }
                                                                        ?>
                                                                    </div>
                                                                    <div id="muncod_off" style="display:<?= (($regcod && $muncod) ? 'none' : 'block') ?>;">
                                                                        <font style="color:#909090;">A Unidade Federal selecionada não possui municípios.</font>
                                                                    </div>
                                                                </div>
                                                            </div>

                                                            <div class="form-group" style="display: none;">
                                                                <div class="col-sm-12">
                                                                    <div class="control-label">
                                                                        <label for="tpocod">Tipo do Órgão</label>
                                                                    </div>
                                                                    <div class="control-input">
                                                                        <?php
                                                                            if ($usuario->usucpf) {
                                                                                $sql = "
                                                                                    SELECT
                                                                                        tp.tpocod as codigo,
                                                                                        tp.tpodsc as descricao
                                                                                    FROM public.tipoorgao tp
                                                                                        INNER JOIN public.tipoorgaofuncao tpf ON tp.tpocod = tpf.tpocod
                                                                                        INNER JOIN entidade.funcaoentidade e ON tpf.funid = e.funid
                                                                                        INNER JOIN seguranca.usuario u ON u.entid = e.entid
                                                                                    WHERE
                                                                                        u.usucpf = '{$usuario->usucpf}'
                                                                                        AND tp.tpostatus='A'";
                                                                                $descricao_tipo = "";

                                                                                if (!$db->carregar($sql)) {
                                                                                    $sql = "
                                                                                        SELECT
                                                                                            tpocod as codigo,
                                                                                            tpodsc as descricao
                                                                                        FROM public.tipoorgao
                                                                                        WHERE
                                                                                            tpostatus='A'";

                                                                                    $editavelTipoOrgao = 'S';
                                                                                    $descricao_tipo = "&nbsp;";
                                                                                }
                                                                            } else {
                                                                                $sql = "
                                                                                    SELECT
                                                                                        tpocod as codigo,
                                                                                        tpodsc as descricao
                                                                                    FROM public.tipoorgao
                                                                                    WHERE
                                                                                        tpostatus='A'
                                                                                ";

                                                                                $descricao_tipo = "&nbsp;";
                                                                                $tpocod = $tpocod ? $tpocod : $configPadrao->tpocodPadrao;
                                                                            }

                                                                            $editavelTipoOrgao = ($editavelTipoOrgao) ? $editavelTipoOrgao : $editavel;
                                                                            $tpocod = 1;
                                                                            $db->monta_combo("tpocod", $sql, $editavelTipoOrgao, $descricao_tipo, 'carrega_orgao', '', '', '170', 'S', 'tpocod', '', '', '', '', 'chosen-select');
                                                                        ?>
                                                                    </div>
                                                                </div>
                                                            </div>

                                                            <div class="form-group">
                                                                <div class="col-sm-12">
                                                                    <div class="control-label">
                                                                        <label for="unicod">UO:</label>
                                                                    </div>
                                                                    <div class="control-input">
                                                                        <span id="unidade">
                                                                        <?php
                                                                        $sql = "select unocod as codigo, unonome descricao
                                                                                    from public.unidadeorcamentaria uno
                                                                                    where prsano = '" . date('Y') . "'
                                                                                    order by descricao";

                                                                        $db->monta_combo("unicod", $sql, $editavel, "&nbsp;", 'ajax_unidade_gestora', '', '', '', 'S', 'unicod', '', '', '', '', 'chosen-select');
                                                                        ?>
                                                                        </span>
                                                                    </div>
                                                                </div>
                                                            </div>

                                                            <div class="form-group">
                                                                <div class="col-sm-12">
                                                                    <div class="control-label">
                                                                        <label for="unicod">Unidade:</label>
                                                                    </div>
                                                                    <div class="control-input">
                                                                        <span id="unidade_gestora">
                                                                            <?php
                                                                            $editavelUG = ($editavelUG) ? $editavelUG : $editavel;

                                                                            carrega_subunidade_orcamentaria($unicod, date('Y'));
                                                                            ?>
                                                                        </span>
                                                                    </div>
                                                                </div>
                                                            </div>

                                                            <div class="form-group">
                                                                <div class="row">
                                                                    <div class="col-sm-2">
                                                                        <div class="control-label">
                                                                            <label for="usufoneddd">Telefone:</label>
                                                                        </div>
                                                                        <div class="control-input">
                                                                            <?php echo campo_texto('usufoneddd', 'S', $editavel, '', 3, 2, '##', '', '', '', '', 'class="form-control"'); ?>
                                                                        </div>
                                                                    </div>
                                                                    <div class="col-sm-4">
                                                                        <div class="control-label">&nbsp;</div>
                                                                        <div class="control-input">
                                                                            <?php echo campo_texto('usufonenum', 'S', $editavel, '', 18, 15, '###-####|####-####|#####-####', '', '', '', '', 'class="form-control"'); ?>
                                                                        </div>
                                                                    </div>
                                                                </div>
                                                            </div>

                                                            <div class="form-group">
                                                                <div class="col-sm-12">
                                                                    <div class="control-label">
                                                                        <label for="usuemail">E-mail:</label>
                                                                    </div>
                                                                    <div class="control-input">
                                                                        <?php echo campo_texto('usuemail', 'S', $editavel, '', 50, 100, '', '', 'left', '', 0, 'class="form-control"'); ?>
                                                                    </div>
                                                                </div>
                                                            </div>

                                                                            <?php if (!$cpf_cadastrado): ?>
                                                                <div class="form-group">
                                                                    <div class="col-sm-12">
                                                                        <div class="control-label">
                                                                            <label for="usuemail_c">Confirme e-mail:</label>
                                                                        </div>
                                                                        <div class="control-input">
                                                                            <?php echo campo_texto('usuemail_c', 'S', '', '', 50, 100, '', '', '', '', '', 'class="form-control"'); ?>
                                                                            <font color="#c09853">Este e-mail é para uso individual, <b>não utilize endereço coletivo</b>.</font>
                                                                        </div>
                                                                    </div>
                                                                </div>
                                                                        <?php endif; ?>

                                                            <div class="form-group">
                                                                <div class="col-sm-12">
                                                                    <div class="control-label">
                                                                        <label for="usufuncao">Função/Cargo:</label>
                                                                    </div>
                                                                    <div class="control-input">
                                                                        <?php
                                                                            if ($editavel == 'N' && $usuario->carid == 9) {
                                                                                echo campo_texto('usufuncao', 'S', $editavel, '', 50, 100, '', '', '', '', '', 'class="form-control" id="usufuncao" style="display: none;"');
                                                                                echo '<script>document.getElementById(\'usufuncao\').style.display = "";</script>';
                                                                            } else {
                                                                                $sql = "select carid as codigo, cardsc as descricao from public.cargo where carid not in (26, 27) order by cardsc";
                                                                                $db->monta_combo("carid", $sql, 'S', 'Selecione', 'alternarExibicaoCargo', '', '', '', 'N', "carid", '', '', '', 'class="form-control"');
                                                                                echo campo_texto('usufuncao', 'S', $editavel, '', 50, 100, '', '', '', '', '', 'class="form-control" id="usufuncao"');
                                                                            }
                                                                        ?>
                                                                    </div>
                                                                </div>
                                                            </div>

                                                            <?php if ($_REQUEST['sisid'] != 57) : ?>
                                                                <div class="form-group">
                                                                    <div class="col-sm-12">
                                                                        <div class="control-label">
                                                                            <label for="htudsc">Observações:</label>
                                                                        </div>
                                                                        <div class="control-input">
                                                                            <?php echo campo_textarea('htudsc', 'N', 'S', '', 100, 3, '', 'class="form-control" style="width: 100%"'); ?>
                                                                        </div>
                                                                    </div>
                                                                </div>
                                                            <?php endif; ?>

                                                            <div class="form-group" style="font-size: 14px;">
                                                                <div class="col-sm-3"></div>
                                                                <div class="col-sm-9">
                                                                <?php if ($sisid == 44) : ?>
                                                                    <a class="btn btn-success" href="javascript:enviar_formulario()">
                                                                        <span class="glyphicon glyphicon glyphicon glyphicon-ok"></span> Cadastrar
                                                                    </a>
                                                                <?php else : ?>
                                                                    <a class="btn btn-success" href="javascript:enviar_formulario()">
                                                                        <span class="glyphicon glyphicon glyphicon glyphicon-ok"></span> Enviar Solicita&ccedil;&atilde;o
                                                                    </a>
                                                                <?php endif; ?>
                                                                <a class="btn btn-danger" href="./cadastrar_usuario_sigest.php?sisid=<?= $sisid ?>&modid=<?= $modid ?>&usucpf=<?= $usucpf ?>">
                                                                    <span class="glyphicon glyphicon glyphicon glyphicon-remove"></span> Cancelar
                                                                </a>						
                                                                </div>
                                                            </div>                                                           
                                                        </form>
                                                        
                                                    </div>
                                                </div>
                                            </div>
                                        </div>
                                    </div>
                                </div>
                            </div>
                        </article>
                    </div>
                </div>
            </div>

        </main>

        <footer id="footer" class="site-footer">
            <div class="container site-info">
                <div class="row">
                    <div class="footer-widget col 0 ">
                        <section id="nav_menu-2" class="widget widget_nav_menu">
                            <div class="menu-rodape-1-container">
                                <ul id="menu-rodape-1" class="menu">
                                    <li id="menu-item-231" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-home current-menu-item page_item page-item-39 current_page_item menu-item-231"><a href="http://gestaoestrategica.cultura.gov.br/">Início</a></li>
                                    <li id="menu-item-232" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-232"><a href="http://gestaoestrategica.cultura.gov.br/acoes-estrategicas/">Ações Estratégicas</a></li>
                                </ul>
                            </div>
                        </section>
                    </div>
                    <div class="footer-widget col 1 ">
                        <section id="nav_menu-3" class="widget widget_nav_menu">
                            <div class="menu-rodape-2-container">
                                <ul id="menu-rodape-2" class="menu">
                                    <li id="menu-item-234" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-has-children menu-item-234"><a href="http://gestaoestrategica.cultura.gov.br/gerenciamento-de-processo/">Gerenciamento de Processo</a>
                                        <ul class="sub-menu">
                                            <li id="menu-item-236" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-236"><a href="http://gestaoestrategica.cultura.gov.br/modelo-de-gestao/">Modelo de Gestão</a></li>
                                            <li id="menu-item-235" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-235"><a href="http://gestaoestrategica.cultura.gov.br/metodologia/">Metodologia</a></li>
                                            <li id="menu-item-237" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-237"><a href="http://gestaoestrategica.cultura.gov.br/cadeia-de-valor/">Cadeia de Valor</a></li>
                                            <li id="menu-item-233" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-233"><a href="http://gestaoestrategica.cultura.gov.br/estrategias/">Estratégias</a></li>
                                        </ul>
                                    </li>
                                </ul>
                            </div>
                        </section>
                    </div>
                    <div class="footer-widget col 2 ">
                        <section id="nav_menu-4" class="widget widget_nav_menu">
                            <div class="menu-rodape-3-container">
                                <ul id="menu-rodape-3" class="menu">
                                    <li id="menu-item-240" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-has-children menu-item-240"><a href="http://gestaoestrategica.cultura.gov.br/plano-de-diretrizes-e-metas/">Plano de Diretrizes e Metas</a>
                                        <ul class="sub-menu">
                                            <li id="menu-item-238" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-238"><a href="http://gestaoestrategica.cultura.gov.br/mapa-estrategico/">Mapa Estratégico</a></li>
                                            <li id="menu-item-239" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-239"><a href="http://gestaoestrategica.cultura.gov.br/orcamento/">Orçamento</a></li>
                                        </ul>
                                    </li>
                                    <li id="menu-item-869" class="menu-item menu-item-type-custom menu-item-object-custom menu-item-has-children menu-item-869"><a href="#">Resultados</a>
                                        <ul class="sub-menu">
                                            <li id="menu-item-870" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-870"><a href="http://gestaoestrategica.cultura.gov.br/indicadores/">Indicadores</a></li>
                                            <li id="menu-item-871" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-871"><a href="http://gestaoestrategica.cultura.gov.br/resultados/">Plano de Ação ? Acompanhamento</a></li>
                                        </ul>
                                    </li>
                                </ul>
                            </div>
                        </section>
                    </div>
                    <div class="footer-widget col 3 last ">
                        <section id="nav_menu-5" class="widget widget_nav_menu">
                            <div class="menu-rodape-4-container">
                                <ul id="menu-rodape-4" class="menu">
                                    <li id="menu-item-244" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-has-children menu-item-244"><a href="http://gestaoestrategica.cultura.gov.br/orcamentacao/">Orçamentação</a>
                                        <ul class="sub-menu">
                                            <li id="menu-item-243" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-243"><a href="http://gestaoestrategica.cultura.gov.br/loa/">LOA</a></li>
                                            <li id="menu-item-247" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-247"><a href="http://gestaoestrategica.cultura.gov.br/renuncia-lei-rouanet/">Lei Federal de Incentivo à Cultura (Lei Roaunet)</a></li>
                                            <li id="menu-item-242" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-242"><a href="http://gestaoestrategica.cultura.gov.br/fsa/">Fundo Setorial do Audiovisual (FSA)</a></li>
                                            <li id="menu-item-246" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-246"><a href="http://gestaoestrategica.cultura.gov.br/pac-iphan/">PAC ? Iphan</a></li>
                                            <li id="menu-item-245" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-245"><a href="http://gestaoestrategica.cultura.gov.br/pac-ceu-das-artes/">PAC ? Ceu das Artes</a></li>
                                        </ul>
                                    </li>
                                </ul>
                            </div>
                        </section>
                    </div>
                </div>
            </div>
        </footer>

    </div>
    
    <script src="/includes/prototype.js"></script>
    <script type="text/javascript">
            jQuery(function(){
                    jQuery('#pflcod').chosen({ width: '50%' });
                    jQuery('#tpocod').chosen({ width: '50%' });
                    jQuery('#regcod').chosen({ width: '50%' });
                    jQuery('#entid').chosen({ width: '50%' });
                    jQuery('#muncod').chosen({ width: '50%' });
                    jQuery('#carid').chosen({ width: '100%' });
                    jQuery('.chosen-select').chosen();
            });

            function selecionar_perfil(){
                    document.formulario.formulario.value = "";
                    document.formulario.submit();
            }

            function listar_municipios( regcod )
            {
                    var url = location.href + '&ajaxRegcod=' + regcod;
                    var div_on = document.getElementById( 'muncod_on' );
                    var div_off = document.getElementById( 'muncod_off' );

                    jQuery.post( url, function(html) {
                            div_on.style.display = 'block';
                            div_off.style.display = 'none';

                            div_on.innerHTML = html;
                ajax_carrega_orgao(document.formulario.tpocod.value);
                jQuery('#muncod').chosen({ width: '100%' });
                    });
            }

            function carrega_orgao( cod )
            {
            ajax_carrega_orgao(cod);
            jQuery('#unicod').chosen({ width: '100%' });
            jQuery('#entid').chosen({ width: '100%' });
            }

            function trim( valor )
            {
                    return valor.replace( /^\s+|\s+$/g,"" );
            }

            function selecionar_orgao( valor ) {
                    document.formulario.formulario.value = "";
                    document.formulario.submit();
            }

            function selecionar_unidade_orcamentaria() {
                    document.formulario.formulario.value = "";
                    document.formulario.submit();
            }

            function enviar_formulario() {
                    if ( validar_formulario() ) {
                            document.formulario.submit();
                    }
            }

        function validar_formulario() {

            //alert('tamanho do nome'+ document.formulario.usunome.value.length);

            var validacao = true;
            var mensagem = 'Os seguintes campos não foram preenchidos corretamente:\n';
            if ( document.formulario.sisid.value == '' || !validar_cpf( document.getElementsByName("usucpf")[0].value ) ) {
                // TODO: voltar para o primeiro formulário
            }

        <?php if (!$cpf_cadastrado): ?>
                document.formulario.usunome.value = trim( document.formulario.usunome.value );
                if ( ( document.formulario.usunome.value == '')  || (document.formulario.usunome.value.length < 5 )) {
                    mensagem += '\n\tNome';
                    validacao = false;
                }
                if ( !validar_radio( document.formulario.ususexo, 'Sexo' ) ) {
                    mensagem += '\n\tSexo';
                    validacao = false;
                }
                if ( document.formulario.regcod.value == '' ) {
                    mensagem += '\n\tUnidade Federal';
                    validacao = false;
                } else if ( document.formulario.muncod.value == '' ) {
                    mensagem += '\n\tMunicípio';
                    validacao = false;
                }

                /*** Tipo do Órgão / Instituição ***/
                if( document.formulario.tpocod )
                {
                    if( document.formulario.tpocod.value == '' )
                    {
                        mensagem += '\n\tTipo do Órgão / Instituição';
                        validacao = false;
                    }
                }
                /*** Órgão / Instituição ***/
                if( document.formulario.entid )
                {
                    if ( document.formulario.tpocod.value != 4 && document.formulario.entid.value == '' )
                    {
                        mensagem += '\n\tÓrgão / Instituição';
                        validacao = false;
                    }
                }
                /*** Órgão / Instituição(Outros) ***/
                if( document.formulario.orgao )
                {
                    if ( document.formulario.tpocod.value == 4 && document.formulario.orgao.value == '' )
                    {
                        mensagem += '\n\tÓrgão / Instituição';
                        validacao = false;
                    }
                }
                /*** Se for federal, valida o preenchimento da UO e UG ***/
                if( document.formulario.tpocod )
                {
                    if( document.formulario.tpocod.value == 1 )
                    {
                        if( document.formulario.unicod )
                        {
                            if ( document.formulario.unicod.value == '' )
                            {
                                mensagem += '\n\tUnidade Orçamentária';
                                validacao = false;
                            }
                        }
                        if( document.formulario.ungcod )
                        {
                            if ( document.formulario.ungcod.value == '' )
                            {
                                mensagem += '\n\tUnidade Gestora';
                                validacao = false;
                            }
                        }
                    }
                }

            <?php if ($uo_total > 0): ?>
                    /*if ( document.formulario.unicod.value == '' ) {
                     mensagem += '\n\tUnidade Orçamentária';
                     validacao = false;
                     }*/
            <?php endif; ?>
                /*
                 if ( document.formulario.orgao ){
                 document.formulario.orgao.value = trim( document.formulario.orgao.value );
                 if (    document.formulario.orgao.value == '' ||
                 document.formulario.orgao.value.length < 5
                 )
                 {
                 mensagem += '\n\tNome do Órgão';
                 validacao = false;
                 }
                 }*/

                if ( document.formulario.entid ) {
                    if ( document.formulario.entid.value == '390360' && document.formulario.unicod.value == '26101' ) {
                        if ( document.formulario.ungcod.value == '' ) {
                            mensagem += '\n\tUnidade Gestora';
                            validacao = false;
                        }
                    }
                }

                document.formulario.usufoneddd.value = trim( document.formulario.usufoneddd.value );
                document.formulario.usufonenum.value = trim( document.formulario.usufonenum.value );
                if (
                    document.formulario.usufoneddd.value == '' ||
                    document.formulario.usufonenum.value == '' ||

                    document.formulario.usufoneddd.value.length < 2 ||
                    document.formulario.usufonenum.value.length < 7
                )
                {
                    mensagem += '\n\tTelefone';
                    validacao = false;
                }
                document.formulario.usuemail.value = trim( document.formulario.usuemail.value );
                if ( !validaEmail( document.formulario.usuemail.value ) ) {
                    mensagem += '\n\tEmail';
                    validacao = false;
                }
                document.formulario.usuemail_c.value = trim( document.formulario.usuemail_c.value );
                if ( !validaEmail( document.formulario.usuemail_c.value ) ) {
                    mensagem += '\n\tConfirmação do Email';
                    validacao = false;
                }
                if ( validaEmail( document.formulario.usuemail.value ) && validaEmail( document.formulario.usuemail_c.value ) && document.formulario.usuemail.value != document.formulario.usuemail_c.value ) {
                    mensagem += '\n\tOs campos Email e Confirmação do Email não coincidem.';
                    validacao = false;
                }

                if ( document.formulario.carid ) {
                    if ( document.formulario.carid.value == '' ) {
                        mensagem += '\n\tFunção/Cargo';
                        validacao = false;
                    }
                    else{
                        if( document.formulario.carid.value == 9 ){
                            document.formulario.usufuncao.value = trim( document.formulario.usufuncao.value );
                            if (
                                document.formulario.usufuncao.value == '' ||
                                document.formulario.usufuncao.value.length < 5
                            )
                            {
                                mensagem += '\n\tFunção';
                                validacao = false;
                            }
                        }
                    }
                }
        <?php endif; ?>

            if ( document.formulario.pflcod )
            {
                /*
                 if ( document.formulario.pflcod.value == '' ) {
                 mensagem += '\n\tPerfil';
                 validacao = false;
                 }
                 */
                // seleciona todos as ações
                var acoes = document.getElementById( "proposto_A" );
                if ( acoes ) {
                    if ( acoes.options.length == 1 && acoes.options[0].value == '' ) {
                        mensagem += '\n\tAções';
                        validacao = false;
                    } else {
                        for ( var i=0; i < acoes.options.length; i++ ) {
                            acoes.options[i].selected = true;
                        }
                    }
                }

                // seleciona todos os programas
                var programas = document.getElementById( "proposto_P" );
                if ( programas ) {
                    if ( programas.options.length == 1 && programas.options[0].value == '' ) {
                        mensagem += '\n\tProgramas';
                        validacao = false;
                    } else {
                        for ( var i=0; i < programas.options.length; i++ ) {
                            programas.options[i].selected = true;
                        }
                    }
                }

                // seleciona todas as unidades
                var unidades = document.getElementById( "proposto_U" );
                if ( unidades ) {
                    if ( unidades.options.length == 0 && unidades.options[0].value == '' ) {
                        mensagem += '\n\tUnidades';
                        validacao = false;
                    } else {
                        for ( var i=0; i < unidades.options.length; i++ ) {
                            unidades.options[i].selected = true;
                        }
                    }
                }
            }

            if ( !validacao ) {
                alert( mensagem );
            }
            return validacao;
        }

        document.formulario.usufuncao.style.display = 'none'
        function alternarExibicaoCargo( tipo ){

            var carid = document.getElementById( 'carid' );
            var usufuncao = document.getElementById( 'usufuncao' );
            var link = document.getElementById( 'linkVoltar' );

            if( carid.value == 9 || carid.value == ''){
                usufuncao.style.display = "";
                //usufuncao.className = "";
                link.style.display = "";
                carid.style.display = "none";
                //link.className = "";
            } else{
                document.formulario.usufuncao.style.display = 'none'
                document.formulario.usufuncao.value = ''
                link.style.display = "none";
                //link.className = "objetoOculto";
                carid.style.display = "";
                carid.value = "";
            }
        }
    </script>

    <div id="footer-brasil" class="verde"></div>
    <script type="text/javascript">
        var et_animation_data = [];
    </script>
    <script type='text/javascript'>
        /* <![CDATA[ */
        var wpcf7 = {"apiSettings":{"root":"http:\/\/gestaoestrategica.cultura.gov.br\/wp-json\/contact-form-7\/v1", "namespace":"contact-form-7\/v1"}, "recaptcha":{"messages":{"empty":"Verifique se voc\u00ea n\u00e3o \u00e9 um rob\u00f4."}}};
        /* ]]> */
    </script>
    <script type='text/javascript' src='http://gestaoestrategica.cultura.gov.br/wp-content/plugins/contact-form-7/includes/js/scripts.js?ver=4.9.1'></script>
    <script type='text/javascript'>
        /* <![CDATA[ */
        var WpBarraBrasil = {"element_to_prepend":"BODY"};
        /* ]]> */
    </script>
    <script type='text/javascript' src='http://gestaoestrategica.cultura.gov.br/wp-content/plugins/wp-barra-brasil//frontend/js/WpBarraBrasil.js?ver=0.1.0'></script>
    <script type='text/javascript' src='//barra.brasil.gov.br/barra.js?ver=0.1.0'></script>
    <script type='text/javascript' src='http://gestaoestrategica.cultura.gov.br/wp-content/themes/pp-wp/assets/js/dist/bundle.min.js?ver=4.9.8'></script>
    <script type='text/javascript'>
        /* <![CDATA[ */
        var et_pb_custom = {"ajaxurl":"http:\/\/gestaoestrategica.cultura.gov.br\/wp-admin\/admin-ajax.php", "images_uri":"http:\/\/gestaoestrategica.cultura.gov.br\/wp-content\/themes\/pp-wp\/images", "builder_images_uri":"http:\/\/gestaoestrategica.cultura.gov.br\/wp-content\/plugins\/divi-builder\/includes\/builder\/images", "et_frontend_nonce":"d898d06b89", "subscription_failed":"Por favor, verifique os campos abaixo para verifique se voc\u00ea digitou as informa\u00e7\u00f5es corretas.", "et_ab_log_nonce":"60a1555eed", "fill_message":"Por favor, preencha os seguintes campos:", "contact_error_message":"Por favor, corrija os seguintes erros:", "invalid":"E-mail inv\u00e1lido", "captcha":"Captcha", "prev":"Anterior", "previous":"Anterior", "next":"Pr\u00f3ximo", "wrong_captcha":"Voc\u00ea digitou o n\u00famero captcha errado.", "is_builder_plugin_used":"1", "ignore_waypoints":"no", "is_divi_theme_used":"", "widget_search_selector":".widget_search", "is_ab_testing_active":"", "page_id":"39", "unique_test_id":"", "ab_bounce_rate":"5", "is_cache_plugin_active":"no", "is_shortcode_tracking":""};
        var et_pb_box_shadow_elements = [];
        /* ]]> */
    </script>
    <script type='text/javascript' src='http://gestaoestrategica.cultura.gov.br/wp-content/plugins/divi-builder/js/divi-builder.min.js?ver=2.0.67'></script>
    <script type='text/javascript' src='http://gestaoestrategica.cultura.gov.br/wp-content/plugins/divi-builder/core/admin/js/common.js?ver=3.0.105'></script>
    <script type='text/javascript'>
            var mejsL10n = {"language":"pt", "strings":{"mejs.install-flash":"Voc\u00ea est\u00e1 usando um navegador que n\u00e3o tem Flash ativo ou instalado. Ative o plugin do Flash player ou baixe a \u00faltima vers\u00e3o em https:\/\/get.adobe.com\/flashplayer\/", "mejs.fullscreen-off":"Desativar tela cheia", "mejs.fullscreen-on":"Tela cheia", "mejs.download-video":"Baixar o v\u00eddeo", "mejs.fullscreen":"Tela inteira", "mejs.time-jump-forward":["Avan\u00e7ar 1 segundo", "Avan\u00e7ar %1 segundos"], "mejs.loop":"Alternar repeti\u00e7\u00e3o", "mejs.play":"Reproduzir", "mejs.pause":"Pausar", "mejs.close":"Fechar", "mejs.time-slider":"Tempo do slider", "mejs.time-help-text":"Use as setas esquerda e direita para avan\u00e7ar um segundo. Acima e abaixo para avan\u00e7ar dez segundos.", "mejs.time-skip-back":["Voltar 1 segundo", "Retroceder %1 segundos"], "mejs.captions-subtitles":"Transcri\u00e7\u00f5es\/Legendas", "mejs.captions-chapters":"Cap\u00edtulos", "mejs.none":"Nenhum", "mejs.mute-toggle":"Alternar mudo", "mejs.volume-help-text":"Use as setas para cima ou para baixo para aumentar ou diminuir o volume.", "mejs.unmute":"Desativar mudo", "mejs.mute":"Mudo", "mejs.volume-slider":"Controle de volume", "mejs.video-player":"Tocador de v\u00eddeo", "mejs.audio-player":"Tocador de \u00e1udio", "mejs.ad-skip":"Pular an\u00fancio", "mejs.ad-skip-info":["Pular em 1 segundo", "Pular em %1 segundos"], "mejs.source-chooser":"Seletor de origem", "mejs.stop":"Parar", "mejs.speed-rate":"Taxa de velocidade", "mejs.live-broadcast":"Transmiss\u00e3o ao vivo", "mejs.afrikaans":"Afric\u00e2ner", "mejs.albanian":"Alban\u00eas", "mejs.arabic":"\u00c1rabe", "mejs.belarusian":"Bielorrusso", "mejs.bulgarian":"B\u00falgaro", "mejs.catalan":"Catal\u00e3o", "mejs.chinese":"Chin\u00eas", "mejs.chinese-simplified":"Chin\u00eas (simplificado)", "mejs.chinese-traditional":"Chin\u00eas (tradicional)", "mejs.croatian":"Croata", "mejs.czech":"Checo", "mejs.danish":"Dinamarqu\u00eas", "mejs.dutch":"Holand\u00eas", "mejs.english":"Ingl\u00eas", "mejs.estonian":"Estoniano", "mejs.filipino":"Filipino", "mejs.finnish":"Finland\u00eas", "mejs.french":"Franc\u00eas", "mejs.galician":"Galega", "mejs.german":"Alem\u00e3o", "mejs.greek":"Grego", "mejs.haitian-creole":"Crioulo haitiano", "mejs.hebrew":"Hebraico", "mejs.hindi":"Hindi", "mejs.hungarian":"H\u00fangaro", "mejs.icelandic":"Island\u00eas", "mejs.indonesian":"Indon\u00e9sio", "mejs.irish":"Irland\u00eas", "mejs.italian":"Italiano", "mejs.japanese":"Japon\u00eas", "mejs.korean":"Coreano", "mejs.latvian":"Let\u00e3o", "mejs.lithuanian":"Lituano", "mejs.macedonian":"Maced\u00f4nio", "mejs.malay":"Malaio", "mejs.maltese":"Malt\u00eas", "mejs.norwegian":"Noruegu\u00eas", "mejs.persian":"Persa", "mejs.polish":"Polon\u00eas", "mejs.portuguese":"Portugu\u00eas", "mejs.romanian":"Romeno", "mejs.russian":"Russo", "mejs.serbian":"S\u00e9rvio", "mejs.slovak":"Eslovaco", "mejs.slovenian":"Esloveno", "mejs.spanish":"Espanhol", "mejs.swahili":"Sua\u00edli", "mejs.swedish":"Sueco", "mejs.tagalog":"Tagalo", "mejs.thai":"Tailand\u00eas", "mejs.turkish":"Turco", "mejs.ukrainian":"Ucraniano", "mejs.vietnamese":"Vietnamita", "mejs.welsh":"Gal\u00eas", "mejs.yiddish":"I\u00eddiche"}};</script>
    <script type='text/javascript' src='http://gestaoestrategica.cultura.gov.br/wp-includes/js/mediaelement/mediaelement-and-player.min.js?ver=4.2.6-78496d1'></script>
    <script type='text/javascript' src='http://gestaoestrategica.cultura.gov.br/wp-includes/js/mediaelement/mediaelement-migrate.min.js?ver=4.9.8'></script>
    <script type='text/javascript'>
        /* <![CDATA[ */
        var _wpmejsSettings = {"pluginPath":"\/wp-includes\/js\/mediaelement\/", "classPrefix":"mejs-", "stretching":"responsive"};
        /* ]]> */
    </script>
    <script type='text/javascript' src='http://gestaoestrategica.cultura.gov.br/wp-includes/js/wp-embed.min.js?ver=4.9.8'></script>
    <style id="et-core-unified-cached-inline-styles-2">.et_divi_builder #et_builder_outer_content .et_pb_image_2{margin-left:0}.et_divi_builder #et_builder_outer_content .et_pb_image_1{margin-left:0}.et_divi_builder #et_builder_outer_content .et_pb_image_3{margin-left:0}.et_divi_builder #et_builder_outer_content .et_pb_image_4{margin-left:0}.et_divi_builder #et_builder_outer_content .et_pb_section_6.et_pb_section{background-color:#ffffff!important}.et_divi_builder #et_builder_outer_content .et_pb_section_5.et_pb_section{background-color:#ffffff!important}.et_divi_builder #et_builder_outer_content .et_pb_image_0{margin-left:0}.et_divi_builder #et_builder_outer_content .et_pb_section_4.et_pb_section{background-color:#ffffff!important}.et_divi_builder #et_builder_outer_content .et_pb_slide_0.et_pb_slider_with_text_overlay .et_pb_slide_content{-webkit-border-bottom-right-radius:3px;-webkit-border-bottom-left-radius:3px;-moz-border-radius-bottomright:3px;-moz-border-radius-bottomleft:3px;border-bottom-right-radius:3px;border-bottom-left-radius:3px}.et_divi_builder #et_builder_outer_content .et_pb_slide_0.et_pb_slider_with_text_overlay h2.et_pb_slide_title,.et_divi_builder #et_builder_outer_content.et_pb_slide_0.et_pb_slider_with_text_overlay .et_pb_slide_title{-webkit-border-top-left-radius:3px;-webkit-border-top-right-radius:3px;-moz-border-radius-topleft:3px;-moz-border-radius-topright:3px;border-top-left-radius:3px;border-top-right-radius:3px}.et_divi_builder #et_builder_outer_content .et_pb_section_1.et_pb_section{background-color:#ffffff!important}.et_divi_builder #et_builder_outer_content .et_pb_section_2.et_pb_section{background-color:#ffffff!important}.et_divi_builder #et_builder_outer_content .et_pb_section_3.et_pb_section{background-color:#ffffff!important}.et_divi_builder #et_builder_outer_content .et_pb_slider .et_pb_slide_0{background-image:url(http://gestaoestrategica.cultura.gov.br/wp-content/uploads/sites/33/2018/02/Rectangle-8.png)}@media only screen and (min-width:981px){.et_divi_builder #et_builder_outer_content .et_pb_image_4{display:none!important}.et_divi_builder #et_builder_outer_content .et_pb_image_0{display:none!important}}@media only screen and (min-width:768px) and (max-width:980px){.et_divi_builder #et_builder_outer_content .et_pb_image_4{display:none!important}}@media only screen and (max-width:767px){.et_divi_builder #et_builder_outer_content .et_pb_image_3{display:none!important}.et_divi_builder #et_builder_outer_content .et_pb_image_2{display:none!important}.et_divi_builder #et_builder_outer_content .et_pb_image_1{display:none!important}}</style>

</body>
</html>