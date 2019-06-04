<?php
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Simec
   Analista: Alexandre Soares Diniz
   Programador: Alexandre Soares Diniz
   Módulo:sistemas.inc
   Finalidade: permitir o cadastro dos sistemas
   */

if ($_REQUEST['pesquisarUsuario']) {
    $usucpf = str_replace(array('.',',', '-'), "", $_REQUEST['usucpf']);
    $sql = "select usunome, usufoneddd, usufonenum, usufuncao  from seguranca.usuario where usucpf = '{$usucpf}'";
    $dados = $db->pegaLinha($sql);

    if($dados){
        echo '
            <div style="margin-top: 10px;">
                <span>Nome: ' . $dados['usunome'] . '</span><br />
                <span>Telefone: (' . $dados['usufoneddd'] . ') ' . $dados['usufonenum'] . '</span><br />
                <span>Função: ' . $dados['usufuncao'] . '</span><br />
            </div>
        ';
    } else {
        echo '';
    }

    die;
}

### function, Copia diretório ###
function copyDir ($source, $dest) {
    $arq = (array) scandir($source);

    // Caso não exista o diretório de destino cria o mesmo
    if (!is_dir($dest))
            mkdir($dest);

    // LOOP nos arquivos do diretório que será copiado
    foreach ($arq as $arq):

            ## Caso seja um diretório aplica recursividade ##
            if (strpos($arq, ".") === false && is_dir($source."/".$arq)):
                    copyDir($source."/".$arq, $dest."/".$arq);
                    continue;
            ## Caso seja um arquivo ".php ou .inc" copia para o novo diretório
            elseif (strrpos($arq, ".php") || strrpos($arq, ".inc")):
                    copy($source."/".$arq, $dest."/".$arq);
                    continue;
            endif;
    endforeach;
    return true;
}

########### AÇÕES Cadastro/Atualização de sistema ###########
/*
 * Será feita uma CÓPIA do sistema RAIZ => sisid = 256
 * Criado com a finalidade de ser o sistema padrão
*/
if (($_REQUEST['act'] == 'inserir') and (! is_array($msgerro))):

   $sql= "
       SELECT
            sisid 
        FROM seguranca.sistema 
        WHERE
            sisdsc = '".$_REQUEST['dsc']."' OR
            sisabrev = '".$_REQUEST['abrev']."' OR
            sisdiretorio = '".$_REQUEST['diret']."';";
   $usu = $db->recuperar($sql);
   unset($sql);

   if(is_array($usu)) {
	   // existe perfil identico, logo, tem que bloquear
	   ?>
	      <script>
	         alert ('O Sistema: <?=$_REQUEST['sisdsc']?> já se encontra cadastrado no sistema.');
	         history.back();
	      </script>
	   <?php
	     exit();
    }

//////// Verifica ESQUEMA /////////
/*
 * Se há esquema == diretório novo, interrompe cadastro
 */
	$sql = sprintf("
            SELECT
                TRUE
            FROM pg_tables 
            WHERE
                schemaname = '%s'",
        $_REQUEST['diret']);
	if ($db->pegaUm($sql) == "t" ){
            die('<script>
                    alert(\'Já existe o esquema "'.$_REQUEST['diret'].'" no Banco de Dados.\nOperação não realizada!\');
                    history.back();
             </script>');
	}

///////// Cadastro do sistema //////////

    $usucpf = ereg_replace( "[^0-9]", "", $_REQUEST['usucpfgestor']);

   $sql = "
        INSERT INTO seguranca.sistema (sisdsc,sisurl,sisabrev,sisdiretorio,sisarquivo,sisfinalidade,sisrelacionado,sispublico,paginainicial,sisexercicio,sisstatus, sisemail, sistel, sisfax, usucpfgestor, usucpfanalista) VALUES (".
            "'".$_REQUEST['dsc']."',".
            "'http://',".
            "'".$_REQUEST['abrev']."',".
            "'".$_REQUEST['diret']."',".
            "'".$_REQUEST['diret']."',".
            "'".$_REQUEST['finalidade']."',".
            "'".$_REQUEST['relacionado']."',".
            "'".$_REQUEST['publico']."',".
            "'inicio&acao=C',".
            "'".$_REQUEST['exerc']."',".
            "'".$_REQUEST['status']."',".
            "'".$_REQUEST['sisemail']."',".
            "'".$_REQUEST['sistel']."',".
            "'".$_REQUEST['sisfax']."',".
            "'".$usucpf."',".
            "'".$_REQUEST['usucpfanalista']."'
             ) RETURNING sisid";
   $sisidNew = $db->pegaUm($sql);
  # $db->commit();
   unset($sql);

///////// Cadastro dos PERFIS (Super Usuário) //////////
   $sql = sprintf("
        INSERT INTO seguranca.perfil
        (
            pfldsc, pflstatus, pflresponsabilidade,
            pflsncumulativo, pflfinalidade, pflnivel,
            pfldescricao, sisid, pflsuperuser
        )
        SELECT
            pfldsc, pflstatus, pflresponsabilidade,
            pflsncumulativo, pflfinalidade, pflnivel, 
            pfldescricao, %d as sisid, pflsuperuser 
        FROM seguranca.perfil 
        WHERE 
            sisid = 256
            AND pflsuperuser = 't';
    ", $sisidNew);

	$db->executar($sql);
	unset($sql);

//////// Cadastro dos MENUS (Menus padrão) ///////////
	$sql = sprintf("
            INSERT INTO seguranca.menu(
                mnucod, mnucodpai, mnudsc, 
                mnustatus, mnulink, mnutipo, 
                mnustile, mnuhtml, mnusnsubmenu, 
                mnutransacao, mnushow, abacod, 
                mnuhelp, sisid, mnuordem
            )
            SELECT
                mnucod, mnucodpai, mnudsc,
                mnustatus, mnulink, mnutipo,
                mnustile, mnuhtml, mnusnsubmenu,
                mnutransacao, mnushow, abacod,
                mnuhelp, %d AS sisid, mnuordem
            FROM seguranca.menu
            WHERE
                sisid = 256
        ", $sisidNew);
	$db->executar($sql);
	unset($sql);

//////// Atualiza a hierarquia do menu ///////////
  	$sql = sprintf("
            UPDATE
                seguranca.menu 
            SET 
                mnuidpai = ( 
                    SELECT
                           mnuid 
                    FROM
                           seguranca.menu m2 
                    WHERE
                           m2.mnucod = menu.mnucodpai AND
                           m2.sisid = %d 
                )
            WHERE 
                menu.sisid = %d;",
        $sisidNew,
        $sisidNew);
  	$db->executar($sql);
  	unset($sql);

/////// Cadastro de ABA ////////
  	$sql = sprintf("
            INSERT INTO seguranca.aba
            (
                abadsc,
                sisid
            )
            SELECT
                abadsc,
                %d 
            FROM seguranca.aba 
            WHERE 
                sisid = 256;", $sisidNew);
  	//die($sql);
  	$db->executar($sql);
  	unset($sql);

/////// Atualiza o MENU associando-o a ABA => abacod ///////
 	$sql = sprintf("
            UPDATE seguranca.menu 
            SET 
                abacod = (
                    SELECT 
                           abacod 
                    FROM 
                           seguranca.aba 
                    WHERE 
                           sisid = %d
                )
            WHERE 
                abacod IS NOT NULL AND
                sisid = %d",
            $sisidNew,
            $sisidNew);
 	$db->executar($sql);
 	unset($sql);

/////// Cadastro que associa o MENU à ABA ///////
 	$sql = sprintf("
            INSERT INTO seguranca.aba_menu(
                abacod,
                mnuid
            )
            SELECT 
                abacod,
                mnuid 
            FROM seguranca.menu 
            WHERE 
                abacod IS NOT NULL AND 
                sisid = %d", $sisidNew);
 	$db->executar($sql);
 	unset($sql);

/////// Associa PERFIL ao MENU ///////
 	$sql = sprintf("
            INSERT INTO seguranca.perfilmenu(
                pflcod,
                mnuid
            )
            SELECT DISTINCT
                pflcod, 
                mnuid 
            FROM seguranca.menu m
                JOIN seguranca.perfil p ON p.sisid = m.sisid
            WHERE 
                m.sisid = %d",$sisidNew);
 	$db->executar($sql);
 	unset($sql);

/////// Atualiza campo com o arquivo padrão de abertura ////////
## DE raiz.php PARA arquivo novo ##
	$sql = sprintf("
            UPDATE seguranca.menu
            SET
                mnulink = replace(mnulink,'raiz.php','%s.php') 
            WHERE
                sisid = %d
                AND mnulink IS NOT NULL",
            $_REQUEST['diret'],
            $sisidNew);
 	$db->executar($sql);
 	unset($sql);

//////// Cadastra usuario no sistema /////////
	$sql = sprintf("
            INSERT INTO seguranca.usuario_sistema(
                usucpf, 
                sisid, 
                susstatus, 
                suscod
            ) VALUES (
                '%s', %d, 'A', 'A'
            );",
        $_SESSION['usucpf'],
        $sisidNew);
 	$db->executar($sql);
	unset($sql);

/////// Cadastra perfil ao usuário /////////
	$sql = sprintf("
            INSERT INTO seguranca.perfilusuario(
                usucpf,
                pflcod
            ) VALUES (
                '%s',
                ( SELECT pflcod FROM seguranca.perfil WHERE sisid = %d limit 1 )
            );",
            $_SESSION['usucpf'],
            $sisidNew);
 	$db->executar($sql);
	unset($sql);

/////// Cria esquema no banco /////////
	$sql = sprintf("
            CREATE SCHEMA %s AUTHORIZATION postgres;
            GRANT ALL ON SCHEMA %s TO postgres;
            GRANT USAGE ON SCHEMA %s TO usr_simec;
        ",
        $_REQUEST['diret'],
        $_REQUEST['diret'],
        $_REQUEST['diret']);
	#dbg($sql,1);
 	$db->executar($sql);
	unset($sql);

	if ($_POST['exerc'] == 'true'):

/////// Criar tabela "programacaoexercicio" dentro do esquema ////////
		$sql = sprintf("
                    CREATE TABLE %s.programacaoexercicio
                    (
                        prsano character(4) NOT NULL,
                        prsdata_inicial date,
                        prsdata_termino date,
                        prsexerccorrente boolean DEFAULT false,
                        prsstatus character(1) DEFAULT 'A'::bpchar,
                        prsativo smallint DEFAULT 1,
                        prsexercicioaberto boolean DEFAULT true,
                        CONSTRAINT pk_programacaosimec PRIMARY KEY (prsano)
                    ) 
                    WITH OIDS;
                    ALTER TABLE %s.programacaoexercicio OWNER TO postgres;
                    GRANT ALL ON TABLE %s.programacaoexercicio TO postgres;
                    GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE %s.programacaoexercicio TO usr_simec;
                    ",
                    $_REQUEST['diret'],
                    $_REQUEST['diret'],
                    $_REQUEST['diret'],
                    $_REQUEST['diret'],
                    $_REQUEST['diret']);
		$db->executar($sql);
		unset($sql);

	//////// Cadastrar exercicio => ano corrente /////////
		$sql = sprintf("
                    INSERT INTO %s.programacaoexercicio
                    (
                        prsano,
                        prsdata_inicial,
                        prsdata_termino,
                        prsexerccorrente,
                        prsstatus,
                        prsativo,
                        prsexercicioaberto
                    )VALUES(
                        date_part('year', timestamp 'now()'),
                        DATE(date_part('year', timestamp 'now()') || '-01-01'),
                        DATE(date_part('year', timestamp 'now()') || '-12-31'),
                        TRUE,
                        'A',
                        1,
                        TRUE
                    )",
                    $_REQUEST['diret']);
		$db->executar($sql);
		unset($sql);
	endif;

#################################### Criar tabelas de segurança #################################
	if ($_REQUEST['tblseg'] == 'A'):
	//////// Criar no esquema a tabela tiporesponsabilidade ///////////
		$sql = sprintf("
                    CREATE TABLE %s.tiporesponsabilidade
                    (
                        tprcod serial NOT NULL, -- Código único da tabela tiporesponsabilidade.
                        tprdsc character varying(100) NOT NULL, -- Descrição do tipo de responsabilidade.
                        tprsnvisivelperfil boolean NOT NULL, -- Indica se o tipo é visível no perfil.
                        tprsigla character(1) NOT NULL, -- Sigla do tipo de responsabilidade.
                        tprurl character varying(255), -- www/sisDiretorio/cadastro_responsabilidade_\"tprurl\".php Será passado como parametro desta url os campos \"pflcod\" e \"usucpf\";		  
                        CONSTRAINT pk_tiporesponsabilidade PRIMARY KEY (tprcod)
                    )
                    WITH (OIDS=FALSE);
                    ALTER TABLE %s.tiporesponsabilidade OWNER TO postgres;
                    GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE %s.tiporesponsabilidade TO usr_simec;
                    COMMENT ON TABLE %s.tiporesponsabilidade IS 'Tabela contendo os tipos possíveis a serem associados a um usuário.';
                    COMMENT ON COLUMN %s.tiporesponsabilidade.tprcod IS 'Código único da tabela tiporesponsabilidade.';
                    COMMENT ON COLUMN %s.tiporesponsabilidade.tprdsc IS 'Descrição do tipo de responsabilidade.';
                    COMMENT ON COLUMN %s.tiporesponsabilidade.tprsnvisivelperfil IS 'Indica se o tipo é visível no perfil.';
                    COMMENT ON COLUMN %s.tiporesponsabilidade.tprsigla IS 'Sigla do tipo de responsabilidade.';
                    COMMENT ON COLUMN %s.tiporesponsabilidade.tprurl IS 'www/sisDiretorio/cadastro_responsabilidade_\"tprurl\".php Será passado como parametro desta url os campos \"pflcod\" e \"usucpf\"';",
                    $_REQUEST['diret'],
                    $_REQUEST['diret'],
                    $_REQUEST['diret'],
                    $_REQUEST['diret'],
                    $_REQUEST['diret'],
                    $_REQUEST['diret'],
                    $_REQUEST['diret'],
                    $_REQUEST['diret'],
                    $_REQUEST['diret']);
		$db->executar($sql);
		unset($sql);

	////////// Criar no esquema a tabela tprperfil //////////
		$sql = sprintf("
                    CREATE TABLE %s.tprperfil
                    (
                        prfid serial NOT NULL, -- Identificador único da tabela tprperfil.
                        tprcod integer NOT NULL, -- Código único da tabela tiporesponsabilidade.
                        pflcod integer NOT NULL, -- Código único da tabela perfil.
                        CONSTRAINT pk_tprperfil PRIMARY KEY (prfid),
                        CONSTRAINT fk_tprperfi_reference_perfil FOREIGN KEY (pflcod)
                            REFERENCES seguranca.perfil (pflcod) MATCH SIMPLE
                            ON UPDATE CASCADE ON DELETE RESTRICT,
                        CONSTRAINT fk_tprperfi_reference_tiporesp FOREIGN KEY (tprcod)
                            REFERENCES %s.tiporesponsabilidade (tprcod) MATCH SIMPLE
                            ON UPDATE CASCADE ON DELETE RESTRICT
                    )
                    WITH (OIDS=FALSE);
                    ALTER TABLE %s.tprperfil OWNER TO postgres;
                    GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE %s.tprperfil TO usr_simec;
                    COMMENT ON TABLE %s.tprperfil IS 'Tabela associativa entre a tiporesponsabilidade e a perfil.';
                    COMMENT ON COLUMN %s.tprperfil.prfid IS 'Identificador único da tabela tprperfil.';
                    COMMENT ON COLUMN %s.tprperfil.tprcod IS 'Código único da tabela tiporesponsabilidade.';
                    COMMENT ON COLUMN %s.tprperfil.pflcod IS 'Código único da tabela perfil.';",
                $_REQUEST['diret'],
                $_REQUEST['diret'],
                $_REQUEST['diret'],
                $_REQUEST['diret'],
                $_REQUEST['diret'],
                $_REQUEST['diret'],
                $_REQUEST['diret'],
                $_REQUEST['diret']);
		$db->executar($sql);
		unset($sql);

	////////// Criar no esquema a tabela usuarioresponsabilidade //////////
		$sql = sprintf("
                    CREATE TABLE %s.usuarioresponsabilidade
                    (
                        rpuid serial NOT NULL, -- Identificador único da tabela usuarioresponsabilidade.
                        pflcod integer,
                        usucpf character(11), -- CPF do usuário.
                        rpustatus character(1) DEFAULT 'A'::bpchar, -- Status do registro (A - Ativo, I - Inativo).
                        rpudata_inc timestamp without time zone DEFAULT '2008-09-02 17:46:42.244463'::timestamp without time zone, -- Data de inclusão do registro.
                        CONSTRAINT pk_usuarioresponsabilidade PRIMARY KEY (rpuid),
                        CONSTRAINT fk_usuarior_reference_perfil FOREIGN KEY (pflcod)
                            REFERENCES seguranca.perfil (pflcod) MATCH SIMPLE
                            ON UPDATE RESTRICT ON DELETE RESTRICT,
                        CONSTRAINT fk_usuarior_reference_usuario FOREIGN KEY (usucpf)
                            REFERENCES seguranca.usuario (usucpf) MATCH SIMPLE
                            ON UPDATE RESTRICT ON DELETE RESTRICT
                    )
                    WITH (OIDS=FALSE);
                    ALTER TABLE %s.usuarioresponsabilidade OWNER TO simec;
                    GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE %s.usuarioresponsabilidade TO usr_simec;
                    COMMENT ON TABLE %s.usuarioresponsabilidade IS 'Tabela contendo a relação dos usuários com acesso ao sistema por perfil e unidade.';
                    COMMENT ON COLUMN %s.usuarioresponsabilidade.rpuid IS 'Identificador único da tabela usuarioresponsabilidade.';
                    COMMENT ON COLUMN %s.usuarioresponsabilidade.usucpf IS 'CPF do usuário.';
                    COMMENT ON COLUMN %s.usuarioresponsabilidade.rpustatus IS 'Status do registro (A - Ativo, I - Inativo).';
                    COMMENT ON COLUMN %s.usuarioresponsabilidade.rpudata_inc IS 'Data de inclusão do registro.';

                    CREATE INDEX ix_usurespons_pflcod
                      ON %s.usuarioresponsabilidade
                      USING btree
                      (pflcod);

                    CREATE INDEX ix_usurespons_usucpf
                      ON %s.usuarioresponsabilidade
                      USING btree
                      (usucpf);",
                    $_REQUEST['diret'],
                    $_REQUEST['diret'],
                    $_REQUEST['diret'],
                    $_REQUEST['diret'],
                    $_REQUEST['diret'],
                    $_REQUEST['diret'],
                    $_REQUEST['diret'],
                    $_REQUEST['diret'],
                    $_REQUEST['diret'],
                    $_REQUEST['diret']);
		$db->executar($sql);
		unset($sql);
	endif;

	$db->commit();
//	$db->rollback();

        # Criação de estrutura padrão de arquivos e pastas do novo módulo/Sistema.
//	$pathDef = $_SERVER['DOCUMENT_ROOT'];
//	copyDir($pathDef.'/raiz', $pathDef.'/'.$_REQUEST['diret']);
//	copyDir($pathDef.'/../raiz', $pathDef.'/../'.$_REQUEST['diret']);
//	rename($pathDef.'/'.$_REQUEST['diret'].'/raiz.php', $pathDef.'/'.$_REQUEST['diret'].'/'.$_REQUEST['diret'].'.php');

    ?>
    	<script>
            alert('Operação realizada com sucesso!');
            location.href='seguranca.php?modulo=sistema/cadastro/sistemas&acao=I';
        </script>
    <?php
	exit();
endif;

if ( ($_REQUEST['act']=='alterar') and (! is_array($msgerro)) ):

    $usucpf = ereg_replace( "[^0-9]", "", $_REQUEST['usucpfgestor']);

   // fazer alteração de perfil na base de dados.
   	$sql = "
            UPDATE 
                seguranca.sistema 
            SET 
                sisdsc 		= '".$_REQUEST['dsc']."',
                sisurl 		= '".$_REQUEST['surl']."',
                sisabrev 		= '".$_REQUEST['abrev']."',
                sisfinalidade 	= '".$_REQUEST['finalidade']."',
                sisrelacionado = '".$_REQUEST['relacionado']."',
                sispublico 	= '".$_REQUEST['publico']."',
                sisexercicio 	= '".$_REQUEST['exerc']."',
                sisstatus 		= '".$_REQUEST['status']."', 
                sisemail 		= '".$_REQUEST['sisemail']."', 
                sistel 		= '".$_REQUEST['sistel']."', 
                sisfax 		= '".$_REQUEST['sisfax']."',
                usucpfgestor 		= '".$usucpf."',
                usucpfanalista	= '".$_REQUEST['usucpfanalista']."'
                WHERE
                sisid = ".$_POST['sisidEdit'];
    $saida = $db->executar($sql);
	$db->commit();
	?>
    	<script>
            alert('Operação realizada com sucesso!');
            location.href='seguranca.php?modulo=sistema/cadastro/sistemas&acao=I';
        </script>
    <?php
	exit();
endif;


include APPRAIZ."includes/cabecalho.inc";
?>
<script language="JavaScript" src="../includes/calendario.js"></script>
<br>
<?php
$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo='Cadastro de Sistema';
monta_titulo($titulo_modulo,'<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.');
?>
<div align="center">
<center>

<?php
//atribui valores as variaveis
$modulo = $_REQUEST['modulo'] ;//
$sisid = md5_decrypt($_REQUEST['sisidEdit'],'');

if($sisid):
	$sql= "
            SELECT
                sisid, 
                sisdsc AS dsc, 
                sisemail,
                sistel,
                sisfax,
                --sisurl AS surl, 
                sisabrev AS abrev, 
                sisdiretorio AS diret, 
                sisfinalidade AS finalidade, 
                sisrelacionado AS relacionado, 
                sispublico AS publico,
                sisexercicio AS exerc, 
                usucpfgestor,
                usucpfanalista,
                sisstatus AS status--,
                --paginainicial 
                FROM seguranca.sistema 
                WHERE 
                    sisid = ".$sisid."
                ORDER BY
		     sisdsc";
	$dados = (array) $db->carregar($sql);

	#### Transforma $dados[0] em Variáveis ####
	extract($dados[0]);

else:
    $sisid 		   = "";
    $dsc 		   ="";
    $surl 		   = "";
    $abrev     	   = "";
    $diret 		   = "";
    $finalidade    = "";
    $relacionado   = "";
    $publico	   = "";
    $paginainicial = "";
    $exerc 		   = 'f';
    $status 	   = 'I';
    $tblseg 	   = 'I';
endif;
    $act = '';

?>
<form method="POST"  name="formulario">
    <input type='hidden' name="modulo" value="<?=$modulo?>">
    <input type='hidden' name="sisidEdit" value="<?=$sisid?>">
    <input type='hidden' name="act" value=<?=$act?>>
    <center>
        <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
            <tr>
                <td align='right' class="SubTituloDireita">Descrição:</td>
                <td><?=campo_texto('dsc','S','S','',50,100,'','');?></td>
            </tr>
            <tr>
                <td align='right' class="SubTituloDireita">abreviação:</td>
                <td><?=campo_texto('abrev','S','S','',25,100,'','');?></td>
            </tr>
            <tr>
                <td align='right' class="SubTituloDireita">Finalidade:</td>
                <td><?=campo_textarea('finalidade','S','S','',75,3,'');?></td>
            </tr>
            <tr>
                <td align='right' class="SubTituloDireita">Relacionado:</td>
                <td><?=campo_textarea('relacionado','S','S','',75,3,'');?></td>
            </tr>
            <tr>
                <td align='right' class="SubTituloDireita">Público:</td>
                <td><?=campo_textarea('publico','S','S','',75,3,'');?></td>
            </tr>
            <tr>
                <td align='right' class="SubTituloDireita">Diretório:</td>
                <td><?=campo_texto('diret','S',$sisid ? 'N' : 'S','',50,100,'','');?></td>
            </tr>
            <tr>
                <td align='right' class="SubTituloDireita">Analista Resposável:</td>
                <td>
                <?php
                    $sql = "
                        SELECT DISTINCT
                            usuario.usucpf as codigo,
                            INITCAP(usuario.usunome) AS descricao
                        FROM seguranca.perfil perfil
                            JOIN seguranca.perfilusuario perfilusuario ON(
                            perfil.pflcod = perfilusuario.pflcod
                            AND perfil.pflcod = 25
                            )
                            RIGHT JOIN seguranca.usuario usuario ON usuario.usucpf = perfilusuario.usucpf
                            JOIN seguranca.usuario_sistema usuariosistema ON usuario.usucpf = usuariosistema.usucpf
                        WHERE
                            usunome IS NOT NULL
                            AND usuariosistema.suscod = 'A'
                            AND usuariosistema.sisid = '4'
                            AND (perfil.pflcod = 25)
                        GROUP BY
                            usuario.usucpf,
                            usuario.usunome
                        ORDER BY
                            descricao
                    ";
                    $db->monta_combo("usucpfanalista", $sql, 'S',"-- Selecione o Analista --",'','','','','S');
                ?>
                </td>
            </tr>
            <tr>
                <td align='right' class="SubTituloDireita">Gestor responsável:</td>
                <td>
                    <?=campo_texto('usucpfgestor','','','',16,14,'###.###.###-##','', '', '', '', 'id="usucpfgestor"', '', '', "this.value=mascaraglobal('###.###.###-##',this.value);");?>
                    <span id="dados-gestor"></span>
                </td>
            </tr>
            <tr>
                <td align='right' class="SubTituloDireita">Exercício:</td>
                <td>
                    <input type="radio" name="exerc" value="true" <?=($exerc=='t'?"CHECKED":"")?>> Ativo
                    <input type="radio" name="exerc" value="false" <?=($exerc=='f'?"CHECKED":"")?>> Inativo
                </td>
            </tr>
            <tr>
                <td align='right' class="SubTituloDireita">Status:</td>
                <td>
                    <input type="radio" name="status" value="A" <?=($status=='A'?"CHECKED":"")?>> Ativo
                    <input type="radio" name="status" value="I" <?=($status=='I'?"CHECKED":"")?>> Inativo
                </td>
            </tr>
            <tr>
                <td align='right' class="SubTituloDireita">Tabelas de segurança:</td>
                <td>
                    <input type="radio" name="tblseg" <?=$sisid ? 'disabled="disabled"' : '';?> value="A" <?=($tblseg=='A'?"CHECKED":"")?>> Ativo
                    <input type="radio" name="tblseg" <?=$sisid ? 'disabled="disabled"' : '';?> value="I" <?=($tblseg=='I'?"CHECKED":"")?>> Inativo
                </td>
            </tr>

            <?php if ($sisid) { ?>
                <tr bgcolor="#CCCCCC">
                    <td></td>
                    <td><input type="button" name="btalterar" value="Alterar" onclick="validar_cadastro('A')" class="botao">
                    <input type="button" name="btcancela" value="Cancelar" onclick="inicio();" class="botao"></td>
                </tr>
            <?php } else { ?>
                <tr bgcolor="#CCCCCC">
                    <td></td>
                    <td><input type="button" name="btinserir" value="Incluir" onclick="validar_cadastro('I')" class="botao">
                    <input type="button" name="btcancela" value="Cancelar" onclick="inicio();" class="botao"></td>
                </tr>
            <?php } ?>
        </table>
        <br><br>

        <table width='95%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
            <thead>
                <tr>
                  <td valign="top" class="title"><strong>Ação</strong></td>
                  <td valign="top" class="title"><strong>Sistema</strong></td>
                  <td valign="top" class="title"><strong>Abreviação</strong></td>
                  <td valign="top" class="title"><strong>Analista Resposável</strong></td>
                  <td valign="top" class="title"><strong>Status</strong></td>
                </tr>
            </thead>
            <tbody>
            <?php

            //exibe listagem dos sistemas cadastrados
            $sql= "
                SELECT
                    sisid AS codigo2,
                    sisdsc AS descricao2,
                    sisabrev AS abreviacao2,
                    sisstatus AS status2,
                    u.usunome
                FROM sistema s
                LEFT JOIN seguranca.usuario u ON u.usucpf = s.usucpfanalista
                WHERE
                    s.sisstatus = 'A'
                ORDER BY
                    sisdsc";
            $RS2 = $db->record_set($sql);
            $nlinhas = $db->conta_linhas($RS2);
            if ($nlinhas >= 0) {
            for ($i=0; $i<=$nlinhas;$i++){
              $res = $db->carrega_registro($RS2,$i);
            // a linha abaixo transforma em variáveis todos os campos do array
              if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
                if (fmod($i,2) == 0) $marcado = '' ; else $marcado='marcado';
                    print '<tr class="'.$marcado.'">
                                <td>
                                    <img border="0" src="../imagens/alterar.gif" title="Alterar o perfil." onclick="altera_cadastro('."'".md5_encrypt($codigo2,'')."'".')">&nbsp;&nbsp;&nbsp;
                                </td>
                                <td>'.$descricao2.'</td>
                                <td>'.$abreviacao2.'</td>
                                <td>'.$usunome.'</td>
                                <td>'.($status2 == 'A'?'Ativo':'Inativo').'</td>
                            </tr>';

            } }
            else
            {
                print '<tr class="'.$marcado.'"><td></td><td>Não há registros de sistemas </td></tr>';
            }
            ?>
            </tbody>
        </table>
    </center>

</form>

<script>
    function strSpecial(string){
        var search = 'áâàãÁÂÀÃéêèÉÊÈíîìÍÎÌóõòõÓÔÌÕúûùÚÛÙ!@#$%¨&*()-+][}{~^´`?:;,"';

        for (i = 0; i < search.length; i++){
            if ( string.indexOf(search.substr(i,1)) >= 0 ){
                alert('Esse caractere é inválido para o campo: " '+search.substr(i,1)+' "');
                return false;
            }
        }
        return true;
    }



    function altera_cadastro(cod) {
        document.formulario.sisidEdit.value = cod;
        document.formulario.submit();
    }

    function inicio() {
        document.formulario.act.value = '';
        location.href='seguranca.php?modulo=sistema/cadastro/sistemas&acao=I';
    }
    jQuery(function(){
        jQuery('#usucpfgestor').change(function(){
            jQuery('#dados-gestor').load('seguranca.php?modulo=sistema/cadastro/sistemas&acao=I&pesquisarUsuario=1&usucpf='+jQuery(this).val());
        }).change().keyup();
    });

    function validar_cadastro(cod) {

        if (document.formulario.dsc.value == "")
        {
            alert("Sistema sem descrição.");
            document.formulario.dsc.focus();
            return;
        }

        if (document.formulario.abrev.value == "")
        {
            alert("Sistema sem abreviação.");
            document.formulario.abrev.focus();
            return;
        }

        if (document.formulario.finalidade.value == "")
        {
            alert("Sistema sem finalidade.");
            document.formulario.finalidade.focus();
            return;
        }

        if (document.formulario.relacionado.value == "")
        {
            alert("Sistema sem relacionamento.");
            document.formulario.relacionado.focus();
            return;
        }

        if (document.formulario.publico.value == "")
        {
            alert("Sistema sem público alvo.");
            document.formulario.publico.focus();
            return;
        }

        if (document.formulario.diret.value == "")
        {
            alert("Sistema sem diretorio.");
            document.formulario.diret.focus();
            return;
        }

        if (!strSpecial(document.formulario.diret.value)){
            document.formulario.diret.focus();
            return;
        }

        if (document.formulario.usucpfanalista.value == "")
        {
            alert("Informe o Analista.");
            document.formulario.usucpfanalista.focus();
            return;
        }

        if ((document.formulario.exerc[0].checked == 0) && (document.formulario.exerc[1].checked == 0))
        {
            alert("Sistema sem exercício.");
            document.formulario.exerc.focus();
            return;
        }

        if ((document.formulario.status[0].checked == 0) && (document.formulario.status[1].checked == 0))
        {
            alert("Sistema sem status.");
            document.formulario.status.focus();
            return;
        }

        if (cod == 'I') document.formulario.act.value = 'inserir'; else document.formulario.act.value = 'alterar';
        document.formulario.submit();
    }
</script>