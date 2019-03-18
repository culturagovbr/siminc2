<?php

    /**
     * Sistema Integrado de Monitoramento do Ministério da Educação
     * Desenvolvedor: Desenvolvedores Simec
     * Analistas: Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>, Alexandre Soares Diniz
     * Programadores: Renê de Lima Barbosa <renedelima@gmail.com>, Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>
     * Módulo: Segurança
     * Finalidade: Solicitação de cadastro de contas de usuário.
     * Data de criação:
     * Última modificação: 31/08/2006
     */
    if (isset($_POST["theme_simec"])) {
        $theme = $_POST["theme_simec"];
        setcookie("theme_simec", $_POST["theme_simec"], time() + 60 * 60 * 24 * 30, "/");
    } else {

        if (isset($_COOKIE["theme_simec"])) {
            $theme = $_COOKIE["theme_simec"];
        }
    }

    // Id da permissão
    define("PER_SIMEC", 154);

    // carrega as bibliotecas internas do sistema
    include "config.inc";
    require APPRAIZ . "includes/classes_simec.inc";
    include APPRAIZ . "includes/funcoes.inc";
    include_once APPRAIZ . "includes/classes/Sms.class.inc";

    // abre conexão com o servidor de banco de dados
    $db = new cls_banco();

    if (!$theme) {
        $theme = $_SESSION['theme_temp'];
    }

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
            header("Location: solicitar_ativacao_de_conta.php?sisid=$sisid&modid=$modid&usucpf=$usucpf");
            exit();
        }
        $_SESSION['MSG_AVISO'] = array("Atenção. CPF já cadastrado no módulo solicitado.");
        header("Location: cadastrar_usuario.php?sisid=$sisid&modid=$modid&usucpf=$usucpf");
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
                    header('location: login.php');
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
                    header('location: login.php');
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
            # Insere informações gerais do usuário
            $sql = sprintf("
                INSERT INTO seguranca.usuario (
                    usucpf,
                    usunome,
                    usuemail,
                    usufoneddd,
                    usufonenum,
                    usufuncao,
                    carid,
                    unicod,
                    usuchaveativacao,
                    regcod,
                    ususexo,
                    ungcod,
                    ususenha,
                    suscod,
                    orgao,
                    muncod,
                    tpocod
                ) VALUES (
                    '%s',
                    '%s',
                    '%s',
                    '%s',
                    '%s',
                    '%s',
                    '%s',
                    '%s',
                    '%s',
                    '%s',
                    '%s',
                    '%s',
                    '%s',
                    '%s',
                    '%s',
                    '%s',
                    %s
                )",
                $cpf, 
                str_to_upper($usunome),
                strtolower($usuemail),
                $usufoneddd,
                $usufonenum, 
                $usufuncao, 
                $carid, 
                $unicod, 
                'f',
                $regcod,
                $ususexo,
                $_POST['ungcod_disable'],
                md5_encrypt_senha($senhageral, ''),
                'P',
                $orgao,
                $muncod,
                $tpocod_banco
            );

            $db->executar($sql);
        }

        // vincula o usuário com o módulo
        $sql = sprintf("INSERT INTO seguranca.usuario_sistema ( usucpf, sisid, pflcod ) values ( '%s', %d, %d )", $cpf, $sisid, $pflcod);
        $db->executar($sql);

        // modifica o status do usuário (no módulo) para pendente
        $descricao = "O usuário que solicitou o acesso apresentou as seguintes observações: " . $htudsc;
        $db->alterar_status_usuario($cpf, 'P', $descricao, $sisid);

        $sql = "
            SELECT
                s.sisid,
                lower(s.sisdiretorio) AS sisdiretorio
            FROM seguranca.sistema s
            WHERE
                sisid = ". $sisid;

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

        $sql = sprintf("SELECT pflcod FROM seguranca.perfil WHERE sisid = %s AND pflpadrao IS TRUE", $sisid);
        $pflcodpadrao = (array) $db->carregarColuna($sql);

        // VERIFICA SE HÁ REGRA PARA ENVIO DE EMAIL/SMS
        if ($_REQUEST['pflcod']) {
            $sql = "
                SELECT
                    r.mreid,
                    r.sisid,
                    mretextoemail,
                    mretextocelular,
                    mreenviaemail,
                    mreenviasms,
                    mretituloemail,
                    mrestatus,
                    mredescricao,
                    p.pfldsc,
                    s.sisabrev,
                    pu.pflcod,
                    pu.usucpf,
                    u.usunome,
                    usuemail,
                    55 || usufoneddd || usufonenum AS celular,
                    usufoneddd,
                    usufonenum
                FROM seguranca.mensagemregra r
                    JOIN seguranca.mensagemcampo cs ON(
                        cs.mreid = r.mreid
                        AND cs.mctid = 2 -- Perfil solicitado
                    )
                    JOIN seguranca.mensagemcampo ca ON(
                        ca.mreid = r.mreid
                        AND ca.mctid = 1
                    ) -- Perfil a ser avisado
                    JOIN seguranca.perfil p ON p.pflcod::text = cs.mcavalor
                    JOIN seguranca.sistema s ON s.sisid = r.sisid
                    JOIN seguranca.perfilusuario pu ON pu.pflcod::text = ca.mcavalor
                    JOIN seguranca.usuario u ON u.usucpf = pu.usucpf
                WHERE
                    r.sisid = {$_REQUEST['sisid']}
                    AND cs.mcavalor = '{$_REQUEST['pflcod']}'
                    AND mretipo = 'A'
                UNION
                SELECT
                    r.mreid,
                    r.sisid,
                    mretextoemail,
                    mretextocelular,
                    mreenviaemail,
                    mreenviasms,
                    mretituloemail,
                    mrestatus,
                    mredescricao,
                    p.pfldsc,
                    s.sisabrev,
                    0,
                    u.usucpf,
                    u.usunome,
                    usuemail,
                    55 || usufoneddd || usufonenum AS celular,
                    usufoneddd,
                    usufonenum
                FROM seguranca.mensagemregra r
                    JOIN seguranca.mensagemcampo cs ON(
                        cs.mreid = r.mreid
                        AND cs.mctid = 2
                    )
                    JOIN seguranca.mensagemcampo ca ON(
                        ca.mreid = r.mreid
                        AND ca.mctid = 3 -- Usuário a ser avisado
                    )
                    JOIN seguranca.perfil p ON p.pflcod::text = cs.mcavalor
                    JOIN seguranca.sistema s ON s.sisid = r.sisid
                    JOIN seguranca.usuario u ON u.usucpf = ca.mcavalor
                WHERE
                    r.sisid = {$_REQUEST['sisid']}
                    AND cs.mcavalor = '{$_REQUEST['pflcod']}'
                    AND mretipo = 'A'
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
                        $remetente = array("nome" => SIGLA_SISTEMA, "email" => EMAIL_SISTEMA_NOREPLY);

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
                            <p><b>O usuário que solicitou o acesso apresentou as seguintes observações:</b> {$htudsc}</p>
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
            header("Location: login.php");

            exit();
        }
    }
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="description" content="">
    <meta name="author" content="">

    <title>Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle</title>

	<!-- Styles Boostrap -->
    <link href="library/bootstrap-3.0.0/css/bootstrap.min.css" rel="stylesheet">
    <link href="library/bootstrap-3.0.0/css/portfolio.css" rel="stylesheet">
    <link href="library/chosen-1.0.0/chosen.css" rel="stylesheet">
    <link href="library/bootstrap-switch/stylesheets/bootstrap-switch.css" rel="stylesheet">
	
    <!-- Custom CSS -->
    <link href="estrutura/temas/default/css/css_reset.css" rel="stylesheet">
    <link href="estrutura/temas/default/css/estilo.css" rel="stylesheet">
	<link href="library/simec/css/custom_login.css" rel="stylesheet">

    <!-- Custom Fonts -->
    <link href="library/font-awesome/css/font-awesome.min.css" rel="stylesheet" type="text/css">
    <link href="http://fonts.googleapis.com/css?family=Source+Sans+Pro:300,400,700,300italic,400italic,700italic" rel="stylesheet" type="text/css">

    <!-- HTML5 shim, for IE6-8 support of HTML5 elements -->
    <!--[if lt IE 9]>
    <script src="estrutura/js/html5shiv.js"></script>
    <![endif]-->
    <!--[if IE]>
    <link href="estrutura/temas/default/css/styleie.css" rel="stylesheet">
    <![endif]-->
	
	<!-- Boostrap Scripts -->
    <script src="library/jquery/jquery-1.10.2.js"></script>
    <script src="library/jquery/jquery.maskedinput.js"></script>
    <script src="library/bootstrap-3.0.0/js/bootstrap.min.js"></script>
    <script src="library/chosen-1.0.0/chosen.jquery.min.js"></script>
    <script src="library/bootstrap-switch/js/bootstrap-switch.min.js"></script>
    
	<!-- Custom Scripts -->
    <script type="text/javascript" src="../includes/funcoes.js"></script>
    
    <!-- FancyBox -->
    <script type="text/javascript" src="library/fancybox-2.1.5/source/jquery.fancybox.js?v=2.1.5"></script>
    <link rel="stylesheet" type="text/css" href="library/fancybox-2.1.5/source/jquery.fancybox.css?v=2.1.5" media="screen" />
    <script type="text/javascript" src="library/fancybox-2.1.5/lib/jquery.mousewheel-3.0.6.pack.js"></script>

    <!-- Add Button helper (this is optional) -->
    <link rel="stylesheet" type="text/css" href="library/fancybox-2.1.5/source/helpers/jquery.fancybox-buttons.css?v=1.0.5" />
    <script type="text/javascript" src="library/fancybox-2.1.5/source/helpers/jquery.fancybox-buttons.js?v=1.0.5"></script>

    <!-- Add Thumbnail helper (this is optional) -->
    <link rel="stylesheet" type="text/css" href="library/fancybox-2.1.5/source/helpers/jquery.fancybox-thumbs.css?v=1.0.7" />
    <script type="text/javascript" src="library/fancybox-2.1.5/source/helpers/jquery.fancybox-thumbs.js?v=1.0.7"></script>

    <!-- Add Media helper (this is optional) -->
    <script type="text/javascript" src="library/fancybox-2.1.5/source/helpers/jquery.fancybox-media.js?v=1.0.6"></script>
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
    <body class="page-index">

        <!-- Login -->
        <section id="register" class="register" style="padding-top: 10px">
            <div class="content">
                <div class="col-md-6 col-md-offset-3">
                    <div class="panel panel-default">
                        <div class="panel-heading">
                            <span class="glyphicon glyphicon-user"></span> Ficha de solicitação de cadastro de usuários
                        </div>
                        <div class="panel-body">
                            <form class="" method="post" name="formulario">
                                <input type="hidden" name="formulario" value="1" />
                                <div class="form-group">
                                    <div class="col-sm-12">
                                        <label class="control-label" for="sisid_modid">Módulo:</label>
                                        <?php
                                        /*                                     * * Recupera todos os sistemas cadastrados ** */
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
                                                $sql = "SELECT
                                                                                                                        m.modid AS codigo,
                                                                                                                        m.modtitulo as descricao
                                                                                                                FROM
                                                                                                                        seguranca.modulo m
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
    require_once APPRAIZ . 'seguranca/modulos/sistema/usuario/incperfilusuario.inc';
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
        $sql = "SELECT
                                                                                                                        muncod AS codigo,
                                                                                                                        mundescricao AS descricao
                                                                                                                FROM
                                                                                                                        territorios.municipio
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
        $sql = "SELECT
                                                                                                                        tp.tpocod as codigo,
                                                                                                                        tp.tpodsc as descricao
                                                                                                                FROM
                                                                                                                        public.tipoorgao tp
                                                                                                                INNER JOIN public.tipoorgaofuncao tpf ON tp.tpocod = tpf.tpocod
                                                                                                                INNER JOIN entidade.funcaoentidade e ON tpf.funid = e.funid
                                                                                                                INNER JOIN seguranca.usuario u ON u.entid = e.entid
                                                                                                                WHERE u.usucpf = '{$usuario->usucpf}' AND tp.tpostatus='A'";
        $descricao_tipo = "";

        if (!$db->carregar($sql)) {
            $sql = "SELECT
                                                                                                                        tpocod as codigo,
                                                                                                                        tpodsc as descricao
                                                                                                                FROM
                                                                                                                        public.tipoorgao
                                                                                                                WHERE tpostatus='A'";

            $editavelTipoOrgao = 'S';
            $descricao_tipo = "&nbsp;";
        }
    } else {
        $sql = "SELECT
                                                                                                                        tpocod as codigo,
                                                                                                                        tpodsc as descricao
                                                                                                                FROM
                                                                                                                        public.tipoorgao
                                                                                                                WHERE tpostatus='A'";

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
                                            /**
                                             * Select feito para retirar duplicados "Ministério da Cidadania - *"
                                             */
                                            $sql = "select distinct unocod as codigo, unonome descricao
                                                        from public.unidadeorcamentaria uno
                                                        where prsano = '" . date('Y') . "' and unocod not in ('55101')
                                                    union 
                                                    select distinct unocod as codigo, unonome descricao
                                                        from public.unidadeorcamentaria uno
                                                        where prsano = '" . date('Y') . "' and unoid = 93
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
                                        <a class="btn btn-danger" href="./cadastrar_usuario.php?sisid=<?= $sisid ?>&modid=<?= $modid ?>&usucpf=<?= $usucpf ?>">
                                            <span class="glyphicon glyphicon glyphicon glyphicon-remove"></span> Cancelar
                                        </a>						
                                    </div>
                                </div>
                            </form>
                        </div>
                    </div>
                </div>
            </div>
        </section>
    </body>

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
</html>