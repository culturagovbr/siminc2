<?php
/**
 * Sistema Integrado de Monitoramento do Ministério da Educação
 * Setor responsvel: SPO/MEC
 * Analistas: Werter
 * Programadores: Lindalberto Filho
 * Módulo: Usuário
 * Finalidade: Permitir o controle de cadastro de usuários.
 * Data de criação: 25/07/2014
 * Última modificação: 25/07/2014
 */

require_once APPRAIZ . "includes/library/simec/Listagem.php";

include_once APPRAIZ . 'includes/funcoes_espelhoperfil.php';
include_once APPRAIZ . 'includes/_funcoescadastrousuario.php';
include_once( APPRAIZ . "includes/classes/FuncoesBootstrap.class.inc" );

//CONSTANTES
define ( 'SENHA_PADRAO', 'siminc123' );

$arrStatus = array ('A' => 'Ativo','B' => 'Bloqueado','P' => 'Pendente');
$acao = $_REQUEST ['acao'];
$usucpf = '';

//tratamento título da página
if ($acao == 'U') {
    $usucpf = $_SESSION ['usucpf'];
    $titulo_pagina = "Atualização de Dados Cadastrais";
} else {
    $usucpf = $_REQUEST ['usucpf'];
    $titulo_pagina = "Cadastro de Usuários";
}

if(!empty($usucpf)){
    // carrega os dados da conta do usuário
    $usuario = carregaDadosUsuario($usucpf);
    //Extrai os dados
    extract ( $usuario );
    $usuario = ( object ) $usuario;
    $cpf = formatar_cpf ( $usuario->usucpf );

    //Verifica se usuário só pode consultar
    $habilitar_edicao = 'S';
    if ($acao == 'C'){ $habilitar_edicao = 'N';}

    //se o sistema for de segurança
    if (($_SESSION ['sisid']) != 4 && ($_SESSION ['usucpforigem'] != $_REQUEST ['usucpf'])) {

        $sql = "
            SELECT
                min(pflnivel) as pflnivel
            FROM seguranca.perfilusuario pu
            INNER JOIN seguranca.perfil p on p.pflcod = pu.pflcod
            WHERE usucpf = '{$_SESSION['usucpforigem']}'
                AND sisid = {$_SESSION['sisid']}";
        $menornivelusuario = $db->pegaUm ( $sql );

        $sql = "
            SELECT
                min(pflnivel) as pflnivel
            FROM seguranca.perfilusuario pu
            INNER JOIN seguranca.perfil p on p.pflcod = pu.pflcod
            WHERE usucpf = '{$_REQUEST['usucpf']}'
                AND sisid = {$_SESSION['sisid']}";
        $menornivelcadastro = $db->pegaUm ( $sql );

        if ($menornivelcadastro && (($menornivelusuario - $menornivelcadastro) > 0)) {
            $habilitar_edicao = 'N';
        }
    }

    /*
     * Verifica se o usuário deve ou não visualizar todos os campos
     */
    $data_atual = NULL;
    $permissao = true;

    if ($acao == 'U') {
        $permissao = false;
        $data_atual = date ( "Y-m-d H:i:s" );
    }

    $_REQUEST['permissao'] = $permissao;
    $usudatanascimento = formata_data ( $usuario->usudatanascimento );

    //------------------------------------REQUISIÇÕES---------------------------------------------//
    /**
     * Captura de requisições
     * @method _funcoescadastrousuario.php
     */
    $_REQUEST['usuario'] = $usuario;
    if ($_REQUEST ['formulario']) {
        !empty($_REQUEST['entid']) ? $_REQUEST['entid'] : $entid;
        alteraDadosUsuario($_REQUEST);
    }
    /**
     * Captura requisições em AJAX
     */
    switch ($_REQUEST['ajax']){
        // Carrega a combo com os orgãos do tipo selecionado
        case 1:
            // Se for estadual verifica se existe estado selecionado
            if ($_REQUEST ["tpocod"] == 2 && empty($_REQUEST["regcod"]) ) {
                echo '<p class="form-control-static">
                        Favor selecionar um Estado.
                    </p>';
                die ();
            }
            // Se for municipal verifica se existe estado selecionado
            if ($_REQUEST ["tpocod"] == 3 && empty($_REQUEST["muncod"]) ) {
                $resultado['msg'] = 'Falha ao inserir Associação.';
                die ();
            }
            //carregando valores para o global FuncoesBootstrap.class.inc
            $tpocod = $_REQUEST ["tpocod"];
            $muncod = $_REQUEST ["muncod"];

            require_once APPRAIZ . "includes/funcoesspo_componentes.php";
            $funcoesBootstrap = new FuncoesBootstrap();
            $funcoesBootstrap->carrega_orgao_bootstrap("S",$usuario->usucpf);
            die();

        case 2:
            // Carrega a combo com as unidades de acordo com orgão selecionado
            require_once APPRAIZ . "includes/funcoesspo_componentes.php";
            $funcoesBootstrap = new FuncoesBootstrap();
            $funcoesBootstrap->carrega_unidade_bootstrap($_REQUEST["entid"], $editavel, $_REQUEST ["usucpf"]);
            die ();

        case 3:	// Carrega a combo com as unidades gestoras da undiade selecionada
            require_once APPRAIZ . "includes/funcoesspo_componentes.php";
            $funcoesBootstrap = new FuncoesBootstrap();
            $funcoesBootstrap->carrega_unidade_gestora_bootstrap($_REQUEST ["unicod"], $editavel, $_REQUEST ["usucpf"]);
            die ();
    }

    //alternando o título da página
    if ( $_REQUEST['acao'] == 'A') {
        $titulo_modulo = 'Alterar o Cadastro de Usuários do Simec';
    }
    if ( $_REQUEST['acao'] == 'C' ) {
        $titulo_modulo = 'Consultar o Cadastro de Usuários do Simec';
    }

//----------------------------------FIM DAS REQUISIÇÕES--------------------------------------------------//

include APPRAIZ . "includes/cabecalho.inc";
require_once APPRAIZ . "includes/funcoesspo_componentes.php";
?>
<!-- INICIO HTML -->
<body>
    <section class="col-md-12">
        <ol class="breadcrumb">
            <li><a href="<?php echo MODULO; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
            <li class="disabled">Sistema</li>
            <li class="disabled">Usuários</li>
            <li class="disabled"><?php echo$titulo_modulo?></li>
            <li class="active"><?php echo$titulo_pagina?></li>
        </ol>
        <section class="form-horizontal well">
    <?php
        if('S' == $habilitar_edicao) { ?>
            <form method="post" name="formulario" id='formulario' class="form-horizontal">
  <?php }
                //istanciando a classe, caso não entre no case.
                if($funcoesBootstrap == null){ $funcoesBootstrap = new FuncoesBootstrap(); } ?>

                <input type="hidden" name="formulario" value="1" />
                <input type="hidden" name="ssd" value="<?php echo (($_REQUEST['ssd'])?"true":""); ?>" />
   <?php
        if ($permissao) { ?>
            <section class="form-group">
                <label for="cpf" class="control-label col-md-2">CPF:</label>
                <section class="col-md-10">
                    <?php echo inputTexto('cpf', '', 'cpf', 14,false, array('masc'=>'###.###.###-##','habil' => 'N', 'evtblur'=>'mostraNomeReceita(this.value);')); ?><!-- cpf não pode ser alterado. -->
                </section>
            </section>
            <section class="form-group">
                <label class="control-label col-md-2" for="usunome">Nome:</label>
                <section class="col-md-10">
                    <?php echo inputTexto('usunome', '', 'usunome', 100,false, array('habil' => $habilitar_edicao)); ?>
                </section>
            </section>
    <?php
        } ?>
            <section class="form-group">
                <label for="apelido" class="control-label col-md-2">Apelido: </label>
                <section class="col-md-10">
                    <?php echo inputTexto('usunomeguerra', '', 'apelido', 20, false, array('habil' => $habilitar_edicao))?>
                </section>
            </section>
            <section class="form-group">
                <label for="" class="control-label col-md-2">Sexo:</label>
                <section class="col-md-2 btn-group" data-toggle="buttons">
                    <label for="sexo_masculino" class="btn btn-default <?php if ($ususexo=='M') { echo 'active';} ?>">
                        <input id="sexo_masculino" type="radio" name="ususexo" value="M" <?php if($habilitar_edicao == 'N' ) echo("disabled='disabled'"); ?><?php echo ($ususexo=='M'?"CHECKED":"") ?> <?php echo $cpf_cadastrado ? 'disabled="disabled"' : '' ?> />
                        Masculino
                    </label>
                    <label for="sexo_feminino" class="btn btn-default <?php if ($ususexo!='M') { echo 'active';} ?>">
                        <input id="sexo_feminino" type="radio" name="ususexo" value="F" <?php echo ($ususexo!='M'?"CHECKED":"") ?> <?php echo $cpf_cadastrado ? 'disabled="disabled"' : '' ?> />
                        Feminino
                    </label>
                </section>
            </section>

    <?php
        $obrigatorio = "N";
        if (! $permissao) { $obrigatorio = "S"; } ?>
                <link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

            <section class="form-group">
                <label for="dt" class="control-label col-md-2">Data de Nascimento <b>(dd/mm/aaaa)</b>:</label>
                <section class="col-md-10">
                    <?php echo inputTexto('usudatanascimento', $valor, 'usudatanascimento', 10,false,array('masc'=>'##/##/####','habil' => $habilitar_edicao, 'size' => 10)) ?>
                </section>
            </section>
            <section class="form-group">
                <label class="control-label col-md-2" for="regcod">Unidade Federal: </label>
                <section class="col-md-10">
                <?php
                    $sql = "SELECT regcod AS codigo, regcod||' - '||descricaouf AS descricao FROM uf WHERE codigoibgeuf IS NOT NULL ORDER BY 2";
                    inputCombo('regcod',$sql,$valor,'regcod',array('arrStyle'=>array('input:read-only'),'habil' => $habilitar_edicao));
                ?>
                </section>
            </section>
            <section class="form-group">
                <label class="control-label col-md-2" for="muncod">Município: </label>
                <section  class="col-md-10" id="municipio">
                </section>
            </section>
            <section class="form-group">
                <label class="control-label col-md-2" for="tpocod">Tipo do Órgão:</label>
                <section class="col-md-10">
                    <?php
                    $tpocod = $usuario->tpocod;
                    $sql = "
                        SELECT
                            tpocod as codigo,
                            tpodsc as descricao
                        FROM public.tipoorgao
                        WHERE tpostatus='A'";
                    inputCombo("tpocod",$sql,'','tpocod');
                    ?>
                </section>
            </section>
            <section class="form-group">
                <label class="control-label col-md-2" for="entid">Órgão:</label>
                <section class="col-md-10" id="spanOrgao">
                    <?php

                    $entid = $usuario->entid;
                    //sobrescrever entid, caso tipo orgao seja 2 ou 3 e o usuário não possua orgão.
                    if (($tpocod == 2 || $tpocod == 3) && !empty($usuario->orgao)) {
                        $entid = 999999;
                    }
                    if(!$entid){
                        $entid = 390374;
                    }
                    //Função utilizada ao carregar a página. Após isso, qualquer tipo de ação no Tipo de Orgão será executada por outra função.
                    $funcoesBootstrap->carrega_orgao_bootstrap("S", $usuario->usucpf);
                    ?>
                </section>
            </section>
            <section class="form-group">
                <label class="col-md-2 control-label" for="unicod">UO:</label>
                <section class="col-md-10" id="spanUnidade">
                    <?php
                    $unicod = $usuario->unicod;
                    if ($entid == 'null' || ($orgao != null && trim($orgao) != "")) {
                        $entid = '';
                    }
                    //Função utilizada ao carregar a página. Após isso, qualquer tipo de ação no Orgão será executada por outra função.
                    $funcoesBootstrap->carrega_unidade_bootstrap($entid, $editavel, $usuario->usucpf);
                    ?>
                </section>
            </section>
            <section class="form-group">
                <label class="control-label col-md-2" for="ungcod">Unidade:</label>
                <section class="col-md-10" id="unidade_gestora">
                <?php
                    $funcoesBootstrap->carrega_unidade_gestora_bootstrap($unicod, $editavel, $usuario->usucpf);
                ?>
                </section>
            </section>
            <section class="form-group">
                <label class="control-label col-md-2" for="usufoneddd">Telefone(DDD) + Telefone:</label>
                <section class="col-md-6">
                    <?php inputTexto("usufoneddd", $valor, "usufoneddd", 3,false,array('masc'=>"##", 'habil' => $habilitar_edicao)); ?>
                    <?php inputTexto("usufonenum", $valor, "usufonenum", 18,false,array('masc'=>'###-####|####-####'));?>
                    <script>
                        jQuery('#usufoneddd').attr('style','width:50px;float:left;');
                        jQuery('#usufonenum').attr('style','width:100px;');
                    </script>
                </section>
            </section>
            <section class="form-group">
                <label for="usuemail" class="control-label col-md-2">E-mail:</label>
                <section class="col-md-10">
                    <?php inputTexto("usuemail", $valor, "usuemail", 100,false, array('habil' => $habilitar_edicao))?>
                    <input type="hidden" name="usuemailconfssd" value="<? echo $usuemail; ?>">
                </section>
            </section>
            <section class="form-group">
                <label class="control-label col-md-2" for="carid">Função/Cargo:</label>
                <section class="col-md-10">
                <?php
                    $sql = "select carid as codigo, cardsc as descricao from public.cargo order by descricao";
                    $carid = $carid? $carid: 9;
                    inputCombo("carid", $sql, $valor, "carid",array('acao'=>'alternarExibicaoCargo','classe'=>'form-control','habil' => $habilitar_edicao));
                    $valor = $valor? $valor: 'Outros';
                    inputTexto('usufuncao', $valor, 'usufuncao', 100,false,array());
                ?>
                    <script>
                        jQuery('#carid').attr('style','width:600px;float:left;');
                        jQuery('#carid').chosen();
                        jQuery('#usufuncao').attr('style','display:none;width:600px;float:left;');
                        if(jQuery('#carid').val() == '9'){
                            jQuery('#usufuncao').show();//usufuncao.style.display = "";
                            jQuery('#linkVoltar').show();//link.style.display = "";
                            jQuery('#carid').next().hide();
                        }
                    </script>

                    <a class="btn btn-primary" onclick="alternarExibicaoCargo('exibirOpcoes');" id="linkVoltar" style="display: none;margin-left:20px;">
                    <span class="glyphicon glyphicon-search"></span>&nbsp;Exibir Opções</a>
                </section>
            </section>
    <?php
        if (($permissao == true) && ($habilitar_edicao == 'S')) {
            if (! AUTHSSD) { ?>
                <section class="form-group">
                    <label class="control-label col-md-2">Senha:</label>
                    <section class="col-md-10">
                        <label class="form-control-static" style="font-weight:lighter;"><input id="senha" type="checkbox" name="senha" style="vertical-align:bottom;"/>&nbsp;Alterar a senha do usuário para a senha padrão: <b class="text-danger"><?php echo SENHA_PADRAO; ?></b>.</label>
                    </section>
                </section>
    <?php
            }//fim (! AUTHSSD)
            if ($_SESSION ['sisid'] == 4){  ?>
            <!--Se o sistema for o SEGURANÇA-->
                <section class="form-group">
                    <label for="" class="control-label col-md-2">Sexo:</label>
                    <section class="control-label col-md-2 btn-group" data-toggle="buttons">
                        <label for="sexo_masculino" class="btn btn-default <?php if ($ususexo=='M') { echo 'active';} ?>">
                            <input id="sexo_masculino" type="radio" name="ususexo" value="M" <? if($habilitar_edicao == 'N' ) echo("disabled='disabled'"); ?><?php echo ($ususexo=='M'?"CHECKED":"") ?> <?php echo $cpf_cadastrado ? 'disabled="disabled"' : '' ?> />
                            Masculino
                        </label>
                        <label for="sexo_feminino" class="btn btn-default <?php if ($ususexo=='F') { echo 'active';} ?>">
                            <input id="sexo_feminino" type="radio" name="ususexo" value="F" <?php echo ($ususexo=='F'?"CHECKED":"") ?> <?php echo $cpf_cadastrado ? 'disabled="disabled"' : '' ?> />
                            Feminino
                        </label>
                    </section>
                </section>
                <section class="form-group">
                    <label class="control-label col-md-2">Status Geral:</label>
                    <section class="col-md-10">
                        <section class="btn-group" data-toggle="buttons">
                            <label for="status_ativo" class="btn btn-default <?php if ($suscod == 'A') { echo 'active';} ?>">
                                <input id="status_ativo" type="radio" name="suscod" value="A" onchange="alterar_status_geral();" <?php echo $suscod == 'A' ? 'checked="checked"' : "" ?> />
                                Ativo
                            </label>
                            <label for="status_pendente" class="btn btn-default <?php if ($suscod == 'P') { echo 'active';} ?>">
                                <input id="status_pendente" type="radio" name="suscod" value="P" onchange="alterar_status_geral();" <?php echo $suscod == 'P' ? 'checked="checked"' : "" ?> />
                                Pendente
                            </label>
                            <label for="status_bloqueado" class="btn btn-default <?php if ($suscod == 'B') { echo 'active';} ?>">
                                <input id="status_bloqueado" type="radio" name="suscod" value="B" onchange="alterar_status_geral();" <?php echo $suscod == 'B' ? 'checked="checked"' : "" ?> />
                                Bloqueado
                            </label>
                        </section>
                        <a href="javascript: exibir_ocultar_historico('historico_geral');" class="btn btn-default">
                            <span class="glyphicon glyphicon-search"></span> Histórico
                        </a>
                    </section>
                    <div class="col-md-10 col-md-offset-2">
                        <div id="historico_geral" style="margin-top: 20px; display: none">
                            <p>
                            <?php
                            $cabecalho = array ("Data","Status","Descrição","CPF" );
                            $sqlHist = sprintf ( "SELECT to_char( hu.htudata, 'dd/mm/YYYY' ) as data, hu.suscod, hu.htudsc, hu.usucpfadm FROM seguranca.historicousuario hu WHERE usucpf = '%s' AND sisid IS NULL ORDER BY hu.htudata DESC", $usucpf );
                            $listHist = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
                            $listHist->setCabecalho($cabecalho);
                            $listHist->setQuery($sqlHist);
                            $listHist->setFormOff();
                            $listHist->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
                            ?>
                            </p>
                        </div>
                    </div>
                </section>
                <section class="form-group">
                    <label class="control-label col-md-2">Justificativa:</label>
                    <section class="col-md-10">
                        <div id="justificativa_on" style="display: none;">
                            <?php inputTextArea('htudsc', $valor, 'htudsc', '3000',array('cols'=>100));?>
                        </div>
                        <div id="justificativa_off" style="display: block; color: #909090;" class="form-control-static">Status não alterado.</div>
                    </section>
                </section>
    <?php
            }else{ ?>
            <!-- fim if ($_SESSION ['sisid'] == 4-->
                <section class="form-group">
                    <label class="control-label col-md-2">Status Geral:</label>
                    <section class="col-md-10" >
                        <p id="tr_status_geral" class="form-control-static text-primary">
                            <?php
                                if(!empty($suscod)){
                                    switch ($suscod) {
                                       case 'A':
                                             echo "<p class=\"form-control-static text-primary\">Ativo</p>";
                                             break;
                                       case 'B':
                                             echo "<p class=\"form-control-static text-primary\">Bloqueado</p>";
                                             break;
                                       case 'P':
                                             echo "<p class=\"form-control-static text-primary\">Pendente</p>";
                                             break;
                                    }
                                }else{?>
                                     <p class="form-control-static text-primary"><?php echo "Sem status" ?></p>
                          <?php }?>

                    </section>
                </section>
                <?php
            } //fim else

            $sistemas = array ();
            if ($_SESSION ['sisid'] == 4) {
                //------------------------CASO O SISTEMA SEJA SEGURANÇA--------------------------
                $funcoesBootstrap->seguranca();
            } else {
                //------------------------SENÃO FOR CAPTURA O ID DO SISTEMA...-------------------
                $sisid = $_SESSION ['sisid'];
                $sql = sprintf ( "SELECT s.* FROM seguranca.sistema s WHERE sisid = %d", $sisid );
                $sistema = ( object ) $db->pegaLinha ( $sql );
                $sql = sprintf ( "SELECT us.*, p.* FROM seguranca.usuario_sistema us LEFT JOIN seguranca.perfil  p USING ( pflcod ) WHERE us.sisid = %d AND usucpf = '%s'", $sistema->sisid, $usucpf );
                $usuariosistema = ( object ) $db->pegaLinha ( $sql );
                $sistema->usuariosistema = $usuariosistema;
                $sistemas [] = $sistema;

                // ----------- VERIFICANDo se o modulo possui as tabelas necessarias e alguma proposta para que seja exibido a lista de propostas------
                $sistema->sisdiretorio = $sistema->sisid == 14 ? 'cte' : $sistema->sisdiretorio;
                $sql = sprintf ("
                    SELECT
                        CASE WHEN (
                            SELECT true FROM pg_tables WHERE schemaname='%s' AND tablename = 'tiporesponsabilidade')
                        THEN true
                        WHEN (SELECT true FROM pg_tables WHERE schemaname='%s' AND tablename = 'tprperfil')
                        THEN true
                        ELSE false
                        END;",
                    strtolower($sistema->sisdiretorio),
                    strtolower($sistema->sisdiretorio));
                $existTable = $db->pegaUm($sql);

                // Somente quem tem cadastro no usuariorespproposta.
                if ($existTable == 't') {
                    $sql_urp = "SELECT urpcampo, pflcod FROM seguranca.usuariorespproposta WHERE sisid = '{$sisid}' AND usucpf = '{$usucpf}'";
                    $urp = $db->pegaLinha($sql_urp);

                    if ($urp) {
                        $sql_tpresp = "SELECT tprdsc, tprtabela, tprcampo, tprcampodsc FROM " .$sistema->sisdiretorio . ".tiporesponsabilidade WHERE tprcampo = '{$urp['urpcampo']}'";
                        $dados_tpresp = $db->pegaLinha($sql_tpresp);
                        $sql_propostas = "SELECT DISTINCT urpcampoid, urpcampo FROM seguranca.usuariorespproposta AS urp WHERE sisid = '{$sisid}' AND usucpf = '{$usucpf}'";
                        $dados_propostas = $db->carregar($sql_propostas);?>

                <section class="form-group">
                    <label class='control-label col-md-2'>
                        Proposto
                    </label>
                </section>

                <section class="form-group">
                    <label class="control-label col-md-2">Perfil:</label>
                    <section class="col-md-10">
                        <p class="form-control-static"><?php echo $usuariosistema->pfldsc?></p>
                    </section>
                </section>
                <section class="form-group">
                    <label class="control-label col-md-2"><? echo($dados_tpresp['tprdsc'].": "); ?></label>
                    <section class="col-md-10">
                        <table class="table table-bordered table-hover">
                            <tr>
                                <td width="100%" bgcolor="#e9e9e9" align="left">Código - <?php echo$dados_tpresp['tprdsc'] ?></td>
                            </tr>
                <?php
                        $count = 0;
                        $dados_propostas = $dados_propostas ? $dados_propostas  : array();
                        foreach ( $dados_propostas as $dado ) {
                            if ($dado ['urpcampoid'] != "") {
                                if ($dados_tpresp ['tprtabela'] == 'planejamento.acao') {
                                    $sql_proposta = "
                                        SELECT
                                            DISTINCT {$dados_tpresp ['tprcampodsc']} as descricao
                                        FROM {$dados_tpresp['tprtabela']}
                                        WHERE {$dados_tpresp['tprcampo']} = '{$dado['urpcampoid']}'
                                            AND prgano = '{$_SESSION ['exercicio']}'";
                                } else {
                                    $sql_proposta = "
                                        SELECT DISTINCT
                                            {$dados_tpresp ['tprcampodsc']} as descricao
                                        FROM {$dados_tpresp ['tprtabela']}
                                        WHERE {$dados_tpresp ['tprcampo']} = '{$dado ['urpcampoid']}'";
                                }
                                $dados_ptoposta = $db->pegaLinha ( $sql_proposta );

                                if ($count % 2)
                                    $backcolor = "class=\"active\"";
                                else
                                    $backcolor = "";
                                $count ++;
                                echo ("<tr " . $backcolor . "><td>" . $dado ['urpcampoid'] . " - " . $dados_ptoposta ['descricao'] . "</td></tr>");
                            }
                        }
                    ?>
                        </table>
                    </section>
                </section>
            <?php
                    }
                }
                ?>

                <section class="col-md-12">
                    <fieldset  style="border-bottom: 1px solid #ccc; font-family: sans-serif; font-size: 16px; color: #999999; margin-bottom: 20px; padding: 8px 15px;">Atribuído</fieldset>
                </section>
                <section class="form-group">
                    <label class="control-label col-md-2" for="sis">Sistema:</label>
                    <section class="col-md-10">
                        <p id="sis" name="sis" class="form-control-static text-info"><strong><?php echo $sistema->sisdsc ?></strong></p>
                    </section>
                </section>
                <section class="form-group">
                    <label class="control-label col-md-2">Status:</label>
                    <section class="col-md-10">
                        <section class="btn-group" data-toggle="buttons">
                            <label for="status_ativo_<?php echo $sistema->sisid ?>"  class="btn btn-default <?php if ($usuariosistema->suscod == 'A' ) { echo 'active';} ?>">
                                <input id="status_ativo_<?php echo $sistema->sisid ?>" type="radio" name="status[<?php echo $sistema->sisid ?>]" value="A" onchange="alterar_status_sistema( <?php echo $sistema->sisid ?> );" />
                                Ativo
                            </label>
                            <label for="status_pendente_<?php echo $sistema->sisid ?>"  class="btn btn-default <?php if ($usuariosistema->suscod == 'P' ) { echo 'active';} ?>">
                                <input id="status_pendente_<?php echo $sistema->sisid ?>" type="radio" name="status[<?php echo $sistema->sisid ?>]" value="P" onchange="alterar_status_sistema( <?php echo $sistema->sisid ?> );"  />
                                Pendente
                            </label>
                            <label for="status_bloqueado_<?php echo $sistema->sisid ?>"  class="btn btn-default <?php if ($usuariosistema->suscod == 'B' ) { echo 'active';} ?>">
                                <input id="status_bloqueado_<?php echo $sistema->sisid ?>" type="radio" name="status[<?php echo $sistema->sisid ?>]" value="B" onchange="alterar_status_sistema( <?php echo $sistema->sisid ?> );" />
                                Bloqueado
                            </label>
                        </section>
                        <a href="javascript: exibir_ocultar_historico('historico_<?php echo $sistema->sisid ?>');" class="btn btn-default">
                            <span class="glyphicon glyphicon-search"></span> Histórico
                        </a>
                    </section>
                    <div class="col-md-10 col-md-offset-2">
                        <div id="historico_<?php echo $sistema->sisid ?>"style="margin-top:20px; display: none">
                            <p>
                        <?php
                        $cabecalho = array ("Data","Status","Descrição","CPF" );
                        $sql = sprintf ( "SELECT
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
                            DESC", $usucpf, $sistema->sisid );
                        $list = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
                        $list->setCabecalho($cabecalho);
                        $list->setQuery($sql);
                        $list->setFormOff();
                        $list->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
                        ?>
                            </p>
                        </div>
                    </div>
                </section>
                <section class="form-group" >
                    <label class="control-label col-md-2">Justificativa:</label>
                    <section class="col-md-10">
                        <div id="justificativa_on_<?php echo $sistema->sisid ?>"
                            style="display: none;" name="justificativa_on_<?php echo $sistema->sisid ?>">
                            <?php inputTextArea('justificativa['.$sistema->sisid.']', $valor, 'justificativa['. $sistema->sisid .']', '3000',array('cols'=>100));?>
                        </div>
                        <div id="justificativa_off_<?php echo $sistema->sisid ?>"style="display: block; color: #909090;" class="form-control-static" >
                            Status não alterado.
                        </div>
                    </section>
                </section>
                <section class="form-group">
                    <label class="control-label col-md-2">Enviar mensagem:</label>
                    <section class="col-md-10">
                        <a class="btn btn-default" title=" Enviar e-mail " style="cursor: pointer;"  onclick="enviarEmail_usu('../geral/envia_email_usuario.php?usuID=',673,515,'<?php echo str_replace(array(".","-"),"",$cpf); ?>')">
                            <span class="glyphicon glyphicon-envelope text-info" ></span>
                            Clique para preencher os dados do email
                        </a>
                    </section>
                </section>
                    <?php
                if ($usuariosistema->pflcod && (!$urp)){ ?>
                <section class="form-group">
                    <label class="control-label col-md-2">Perfil Desejado:</label>
                    <section class="col-md-10">
                        <p class="form-control-static"><?php echo $usuariosistema->pfldsc ?></p>
                    </section>
                </section>
                    <?php
                }
                    ?>
                <section class="form-group">
                    <label class="control-label col-md-2">Perfil:</label>
                    <section class="col-md-10">
                    <?php
                // Se não for Super Usuário, deve obedecer a tabela 'seguranca.perfilpermissao'
                $perfilSuperUser = $db->testa_superuser (); // testa se o usuário é super usuário
                if (!$perfilSuperUser) {
                    $arrPerfil = pegaPerfilgeral (); // pega todos os perfis do usuário
                    $arrPerfil = retornaPflcodFilhos ( $arrPerfil ); // retornar todos os perfis associados (seguranca.perfilpermissao)
                }
                /**
                 * * Sistema ESCOLA **
                 */
                if ($sistema->sisid == 34) {
                    if ($db->testa_superuser ()) {
                        $sql_perfil = sprintf ( "
                            SELECT
                                distinct p.pflcod as codigo,
                                p.pfldsc as descricao
                            FROM seguranca.perfil p
                            LEFT JOIN seguranca.perfilusuario pu on pu.pflcod=p.pflcod
                            WHERE p.pflstatus='A'
                                AND p.sisid = %d
                            ORDER BY descricao", $sistema->sisid );
                    } else {
                        $arrModid = $db->carregarColuna ( "
                            SELECT p.modid FROM seguranca.perfil p
                            INNER JOIN seguranca.perfilusuario pu ON pu.pflcod = p.pflcod AND pu.usucpf = '" . $_SESSION ['usucpf'] . "'
                            WHERE p.sisid = " . $sistema->sisid );
                        if ($arrModid) {
                            $arrAux = array ();
                            $arrSelects = array ();
                            $arrModid = $arrModid ? $arrModid  : array();
                            foreach ( $arrModid as $modid ) {
                                if (! in_array ( $modid, $arrAux )) {
                                    if (is_null ( $modid )) {
                                        $sql = sprintf ( "SELECT p.pflnivel FROM seguranca.perfil p INNER JOIN seguranca.perfilusuario pu ON pu.pflcod = p.pflcod AND pu.usucpf = '%s' AND p.sisid = %d AND p.modid IS NULL ORDER BY p.pflnivel", $_SESSION ['usucpf'], $sistema->sisid);
                                        $nivel = $db->pegaUm($sql);
                                        $arrSelects[] = sprintf ("
                                            SELECT
                                                distinct p.pflcod AS codigo,
                                                p.pfldsc AS descricao
                                            FROM seguranca.perfil p
                                            LEFT JOIN seguranca.perfilusuario pu ON pu.pflcod=p.pflcod
                                            WHERE p.pflstatus='A' AND p.pflnivel >= %d AND p.sisid= %d", $nivel, $sistema->sisid );
                                    } else {
                                            $sql = sprintf ( "select
                                                    p.pflnivel
                                                    from
                                                    seguranca.perfil p
                                                    inner join seguranca.perfilusuario pu on pu.pflcod=p.pflcod
                                                    and pu.usucpf='%s' and p.sisid=%d
                                                    and p.modid='%d'
                                                    order by
                                                    p.pflnivel", $_SESSION ['usucpf'], $sistema->sisid, $modid );
                                            $nivel = $db->pegaUm ( $sql );
                                            $arrSelects [] = sprintf ( "select
                                                    distinct p.pflcod as codigo,
                                                    p.pfldsc as descricao
                                                    from
                                                    seguranca.perfil p
                                                    left join seguranca.perfilusuario pu on pu.pflcod=p.pflcod
                                                    where
                                                    p.pflstatus='A' and
                                                    p.pflnivel > %d and
                                                    p.sisid=%d and
                                                    p.modid=%d", $nivel, $sistema->sisid, $modid );
                                    }

                                    $arrAux [] = $modid;
                                }
                            }

                            $sql_perfil = 'SELECT
                                            codigo,
                                            descricao
                                            FROM
                                            (' . implode ( ' UNION ALL ', $arrSelects ) . ') as foo
                                            ORDER BY
                                            descricao';
                        } else {
                            $sql = sprintf ( "select
                                                p.pflnivel
                                                from
                                                seguranca.perfil p
                                                inner join seguranca.perfilusuario pu on pu.pflcod=p.pflcod
                                                and pu.usucpf='%s' and p.sisid=%d
                                                and p.modid is null
                                                order by
                                                p.pflnivel", $_SESSION ['usucpf'], $sistema->sisid );
                            $nivel = $db->pegaUm ( $sql );
                            $sql_perfil = sprintf ( "select
                                                        distinct p.pflcod as codigo,
                                                        p.pfldsc as descricao
                                                        from
                                                        seguranca.perfil p
                                                        left join seguranca.perfilusuario pu on pu.pflcod=p.pflcod
                                                        where
                                                        p.pflstatus='A' and
                                                        p.pflnivel > %d and
                                                        p.sisid=%d and
                                                        " . ($arrPerfil ? " p.pflcod in('" . implode ( "','", $arrPerfil ) . "') and " : "") . "
                                                        p.modid is null
                                                        order by
                                                        descricao", $nivel, $sistema->sisid );
                        }
                    }
                } else {
                    if ($db->testa_superuser ()) {
                        $sql_perfil = sprintf ( "select
                        distinct p.pflcod as codigo,
                        p.pfldsc as descricao
                        from
                        seguranca.perfil p
                        left join seguranca.perfilusuario pu on pu.pflcod=p.pflcod
                        where
                        p.pflstatus='A' and
                        p.sisid=%d
                        order by
                        descricao", $sistema->sisid );
                    } else {
                            $sql = sprintf ( "SELECT
                                    p.pflnivel
                                    FROM
                                    seguranca.perfil p
                                    INNER JOIN seguranca.perfilusuario pu ON pu.pflcod=p.pflcod AND pu.usucpf='%s' AND p.sisid=%d
                                    ORDER BY
                                    p.pflnivel", $_SESSION ['usucpf'], $sistema->sisid );
                            $nivel = $db->pegaUm ( $sql );
                            $sql_perfil = sprintf ( "SELECT
                                    DISTINCT p.pflcod AS codigo,
                                    p.pfldsc AS descricao
                                    FROM
                                    seguranca.perfil p
                                    LEFT JOIN seguranca.perfilusuario pu ON pu.pflcod=p.pflcod
                                    WHERE
                                    p.pflstatus='A' AND
                                    p.pflnivel >= %d AND
                                    --" . ($arrPerfil ? " p.pflcod in('" . implode ( "','", $arrPerfil ) . "') and " : "") . "
                                    p.sisid=%d
                                    ORDER BY
                                    descricao", $nivel, $sistema->sisid );
                    }
                }
                $sql = sprintf ( "SELECT
                                    DISTINCT p.pflcod as codigo,
                                    p.pfldsc as descricao
                                    FROM
                                    seguranca.perfilusuario pu
                                    INNER JOIN seguranca.perfil p on p.pflcod=pu.pflcod
                                    WHERE
                                    p.pflstatus = 'A' AND
                                    p.sisid=%d AND
                                    pu.usucpf='%s'
                                    ORDER BY
                                    descricao", $sistema->sisid, $usucpf );
                $nome = 'pflcod[' . $sistema->sisid . ']';
                $$nome = $db->carregar( $sql );
                $cons = $db->carregar($sql);
                $perfis = $db->carregar($sql_perfil);
                $selecionados = array();
                if(is_array($cons)){
                    foreach ($cons as $c){
                            $selecionados[] = $c['codigo'];
                    }
                }
                echo inputCombo('pflcod[' . $sistema->sisid . '][]', $sql_perfil, $selecionados, 'pflcod[]', array('multiple'=> 'multiple', 'titulo'=>'Selecione o(s) Perfil(s)'));
                ?>

                        </section>
                    </section>
                    <?php unset ( $sql );
                    // solicitado pelo Henrique Xavier para o ENEM poder funcionar
                    if ($existTable === 't') :
                            $sql = "SELECT
                                    *
                                    FROM
                                    " . $sistema->sisdiretorio . ".tiporesponsabilidade
                                    WHERE
                                    tprsnvisivelperfil = 't'
                                    ORDER BY
                                    tprdsc";
                            $responsabilidades = ( array ) $db->carregar ( $sql );
                            $sqlPerfisUsuario = "SELECT
                                    p.pflcod, p.pfldsc
                                    FROM
                                    seguranca.perfil p INNER JOIN seguranca.perfilusuario pu ON
                                    pu.pflcod = p.pflcod AND
                                    pu.usucpf = '%s' AND
                                    sisid=" . $sistema->sisid . "
                                    WHERE
                                    p.pflstatus='A'
                                    ORDER BY
                                    p.pfldsc";
                            $query = sprintf ( $sqlPerfisUsuario, $usucpf );
                            $perfisUsuario = $db->carregar ( $query );
                            $script = gerFuncResp ( $sistema->sisid, $sistema->sisdiretorio, $usucpf, $responsabilidades );

                            if( $perfisUsuario ): ?>
                                <section class="form-group">
                                    <label class="control-label col-md-2">Associação de Perfil:</label>
                                     <?php mostraResponsabilidades($responsabilidades, $perfisUsuario, $sistema, $exibirBotao = true); ?>
                                </section>
                            <?php endif;
                    endif;// FIM if ($existTable === 't')
                } //FIM else ($_SESSION ['sisid'] == 4)
        } // FIM if ( ($permissao == true) && ($habilitar_edicao == 'S') )

            //exibir estático o perfil e a associação para tela de consulta
            if(($_REQUEST['acao'] == 'C') ){
                //recuperando a unidade vinculada ao perfil
                $sisid = $_SESSION ['sisid'];
                $sql = sprintf ( "SELECT
                                    DISTINCT p.pflcod as codigo,
                                    p.pfldsc as descricao
                                    FROM
                                    seguranca.perfilusuario pu
                                    INNER JOIN seguranca.perfil p on p.pflcod=pu.pflcod
                                    WHERE
                                    p.pflstatus = 'A' AND
                                    p.sisid=%d AND
                                    pu.usucpf='%s'
                                    ORDER BY
                                    descricao", $sisid, $usucpf );
                $cons = $db->carregar($sql);
                //recuperando os perfis cadastrados para o cpf
                $sqlPerfisUsuario = "SELECT
                        p.pflcod, p.pfldsc
                        FROM
                        seguranca.perfil p INNER JOIN seguranca.perfilusuario pu ON
                        pu.pflcod = p.pflcod AND
                        pu.usucpf = '%s' AND
                        sisid=" . $sisid . "
                        WHERE
                        p.pflstatus='A'
                        ORDER BY
                        p.pfldsc";
                $query = sprintf ( $sqlPerfisUsuario, $usucpf );
                $perfisUsuario = $db->carregar ( $query );
                //recuperando o dados do módulo
                $sql = sprintf ( "SELECT
                                    s.*
                                    FROM
                                    seguranca.sistema s
                                    WHERE
                                    sisid = %d", $sisid );
                $sistema = ( object ) $db->pegaLinha ( $sql );
                if( is_array($cons) && $perfisUsuario && $sistema ){
                    foreach ($cons as $c)?>
                        <section class="form-group">
                            <label class="control-label col-md-2">Associação de Perfil:</label>
                            <section class="col-md-10">
                               <?php mostraResponsabilidades($responsabilidades, $perfisUsuario,$sistema,$exibirBotao = false);?>
                            </section>
                        </section><?

                }
            }?>
                <section class="form-group">
                    <section class="col-md-offset-2 col-md-10">
                        <?php
                        if ($habilitar_edicao == 'S') { ?>
                            <input type="button" class="btn btn-primary" name="btalterar" value="Salvar" onclick="enviar_formulario();"><?php
                        }?>
                        <input type="button" class="btn btn-warning" name="btvoltar" value="Voltar"onclick="voltar();">
                    </section>
                </section>
            <?php if('S' == $habilitar_edicao) { ?>
                </form>
            <?php } ?>
        </section>
    </section>
</body>
<?php }else{?>
<script>
    window.location.href = '?modulo=sistema/usuario/consusuario&acao=<?php echo $_REQUEST['acao'] ?>';
</script>
<?php } ?>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="/includes/funcoesspo.js"></script>
<script type="text/javascript" defer="defer">
    //function que carrega as combos Unidade Federal, Municipio e Tipo de Órgão
    jQuery(function(){
        jQuery("#usudatanascimento").datepicker({
            changeMonth: true,//exibir combo de mês
            changeYear: true,//exibir combo de ano
            yearRange: '1900:c',//limitando ano mínimo e ano atual como máximo
        });

        combo_entid = jQuery('#entid');
        combo_regcod = jQuery('#regcod');
        combo_municipio = jQuery('#municipio');
        combo_tpocod = jQuery('#tpocod');

        combo_regcod.change(function(){//carregar municipio de acordo com UF selecionada
            if(combo_regcod.val() != ''){
                combo_municipio.html('<select id="muncod" <?php echo $habilitar_edicao == 'N' ? "disabled='disabled'" : ''?> name="muncod" class="form-control"></select>');
                listaMunicipio(combo_regcod.val());
            }else{
                jQuery('#municipio').html('<p id="mun" class="form-control-static" style="color:#428bca;">Selecione uma Unidade Federal.</p>');
            }
        });

        combo_regcod.change();//deixando a combo carregada, caso selecione outra opção, chama a function acima.

        combo_tpocod.change(function(){//de acordo com nova selecao do tipo de órgão deve recarregar
            //forçando o usuário a prencher UF, além de ser campo obrigatório a funcao carregaOrgao
            if(combo_regcod.val() == ''){
                alert("Para prosseguir, selecione Unidade Federal");
                var exibir = setInterval(function() {
                    jQuery('#regcod').trigger('chosen:open');
                }, 1000);
                setInterval(function(){
                    clearInterval(exibir);
                }, 3000);
            }else{
                carregaOrgao(combo_tpocod.val());
            }
        });
    });
    function listaMunicipio(estuf){
        var url;
        <?php if ( $_SESSION['sisid'] == 147 ){ ?>
                url = '<?php echo$_SESSION['sisarquivo'] ?>.php?modulo=sistema2/usuario2/cadusuario&acao=<?php echo$_REQUEST['acao'] ?>';
        <?php }else{ ?>
                url = '<?php echo$_SESSION['sisarquivo'] ?>.php?modulo=sistema/usuario/cadusuario&acao=<?php echo$_REQUEST['acao'] ?>',
        <?php } ?>

        jQuery.ajax({
            url: url,
            type: "post",
            dataType: "html",
            data: {"servico" : "listar_mun","estuf" : estuf},
            success: function(res){
                atualizaComboMunicipio(res);
            },
            error: function(e){
                alert(e);
            }
         });
    }

    function atualizaComboMunicipio(html){
        //jogando retorno do ajax na combo via html
        jQuery('#muncod').html(html);
        //mantendo carregada a opção pre selecionada
        jQuery('#muncod > option').each(function(){
            <?php if( !empty($usuario->muncod) ): ?>
                if(this.value == <?php echo $usuario->muncod; ?>){
                    jQuery(this).attr('selected','selected');
                }
            <?php endif; ?>
        });
        jQuery('#muncod').chosen();
    }

    function carregaOrgao(tpocod){
	if (tpocod) {
            if ((jQuery('#muncod').val() == '') && (jQuery('#regcod').val() == '') ){
                var url = location.href + '&ajax=1&tpocod=' + tpocod;
            }else {
                var url = location.href + '&ajax=1&tpocod=' + tpocod
                    + '&muncod=' + jQuery('#muncod').val()
                    + '&regcod=' + jQuery('#regcod').val();
            }

            if (jQuery('#unicod')){
                jQuery('#unicod').val('');
                carregarUnidadeGestora(jQuery('unicod').val());
            }

//            if ( (typeof jQuery('#entid').val() != 'undefined') || (jQuery('#entid').val() != '') ){
//                ($('entid').val());
//            }

            //ajax que popula combo tipo de órgão
            $.ajax({
                url: url,
                type: "post",
                dataType: "html",
                async :false,
                //data: {"servico" : "listar_mun","estuf" : estuf},
                success: function(res){
                    jQuery('#spanOrgao').html(res);
                    jQuery('#spanOrgao').find('select').addClass('form-control').chosen();
                },
                error: function(e){
                    alert(e);
                }
            });

            if((typeof jQuery('#entid').val() != 'undefined') && (jQuery('#entid').val() != '') ){
                carregarUnidade(jQuery('#entid').val());
            }else{
               carregarUnidade('');
            }

        }else{
            jQuery('#spanOrgao').html('<p id="org" class="form-control-static" style="color:#428bca;">Selecione o Tipo do Órgão.</p>');
            carregarUnidade('');
        }
    }

    //carrega combo Unidade Orçamentária
    function carregarUnidade(entid){

        $.ajax({
            url: location.href + '&ajax=2&entid=' + entid,
            type: "post",
            dataType: "html",
            async :false,
            //data: {"servico" : "listar_mun","estuf" : estuf},
            success: function(res){
                jQuery('#spanUnidade').html(res);//substituindo html através do id da section
                $('#unicod').find('select').addClass('form-control').chosen();//jQuery('.chosen-select').chosen();//instanciando o chosen novamente para montar a combo.
            },
            error: function(e){
                alert(e);
            }
        });
        if(jQuery('#unicod').val() != "" && jQuery('#unicod').val() != 'undefined'){
            carregarUnidadeGestora(jQuery('#unicod').val());
        }
    }



    function carregarUnidadeGestora(unicod){
        var url = location.href + '&ajax=3&unicod=' + unicod;
        $.ajax({
            url: url,
            type: "post",
            dataType: "html",
            async :false,
            //data: {"servico" : "listar_mun","estuf" : estuf},
            success: function(res){
                jQuery('#unidade_gestora').html(res);
                jQuery('#unidade_gestora').find('select').addClass('form-control').chosen();
            },
            error: function(e){
                alert(e);
            }
        });
    }

    function mostraNomeReceita(cpf){
        //var cpf = jQuery('#cpf').val();
        if (cpf) {
            jQuery.ajax({
            url: '/sisfor/ajax.php?recuperarUsuario=1&iuscpf=' + cpf,
            dataType: 'json',
            success: function(dados) {
                alert(dados);
                if (dados.nome) {
                    cpf = mascaraglobal("###.###.###-##", dados.cpf);
                    jQuery('#cpf').val(cpf);
                    jQuery('#usunome').val(dados.nome);
                    jQuery('#usuemail').val(dados.email);
                    //recuperando CPF digitado, para buscar dados no SIAPE
                    jQuery('#btnConsSiape').attr('onclick', 'visualizaDadosSiape(\''+dados.cpf+'\')');

                } else {
                    alert('O CPF informado é inválido.');
                    jQuery('#cpf').val('');
                    jQuery('#usunome').val('');
                    jQuery('#usuemail').val('');
                }
            }
            });
        }
    }

    <?php echo $script; ?>

    var permissao = <?php echo $permissao ? 'true' : 'false' ?>;

    <?php
        $sql = "select " . " orgcod, " . " unicod, " . " unicod || ' - ' || unidsc as unidsc " . " from unidade " . " where " . " unistatus = 'A' and " . " unitpocod = 'U' " . " order by orgcod, unidsc ";
    ?>
    var lista_uo = new Array();
    <?php $ultimo_cod = null;
          is_array($db->carregar( $sql )) ? $db->carregar( $sql ) : array();
          foreach ( $db->carregar( $sql ) as $unidade ) :
            if ( $ultimo_cod != $unidade['orgcod'] ) : ?>
                    lista_uo['<?php echo $unidade['orgcod'] ?>'] = new Array();
                    <?php $ultimo_cod = $unidade['orgcod'];
            endif; ?>
            lista_uo['<?php echo $unidade['orgcod'] ?>'].push( new Array( '<?php echo $unidade['unicod'] ?>', '<?php echo addslashes( trim( $unidade['unidsc'] ) ) ?>' ) );
    <?php endforeach; ?>

    <?php
    $sql = "SELECT unicod, ungcod, ungcod||' - '||ungdsc as ungdsc FROM unidadegestora WHERE ungstatus = 'A' AND unitpocod = 'U' ORDER BY unicod, ungdsc";
    ?>
    var lista_ug = new Array();
    <?php $ultimo_cod = null;
          $unidades = ($db->carregar( $sql )) ? $db->carregar( $sql ) : array();
          foreach ($unidades as $unidade ) :
            if ( $ultimo_cod != $unidade['unicod'] ) : ?>
                lista_ug['<?php echo $unidade['unicod'] ?>'] = new Array();
                <?php $ultimo_cod = $unidade['unicod'];
            endif; ?>
            lista_ug['<?php echo $unidade['unicod'] ?>'].push( new Array( '<?php echo $unidade['ungcod'] ?>', '<?php echo addslashes( trim( $unidade['ungdsc'] ) ) ?>' ) );
    <?php endforeach; ?>

    var status_geral_alterado = false;
    function alterar_status_geral(){
        var antigo = '<?php echo $suscod ?>';
        var novo = antigo;

        jQuery('[name=suscod]').each(function(){
            if(jQuery(this).parent().hasClass( "active" )){
                novo = this.value;
            }
        });

        var status_geral_alterado = antigo != novo;

        if ( status_geral_alterado ) {
            jQuery( '#justificativa_on' ).show();//vazia.style.display = 'none';
            jQuery( '#justificativa_off' ).hide();//justificativa.style.display = 'block';
        } else {
            jQuery( '#justificativa_on' ).hide();//justificativa.style.display = 'none';
            jQuery( '#justificativa_off' ).show();//vazia.style.display = 'block';
        }
    }


    if (permissao) {
        var antigo = new Array();
        var status_sistema_alterado = new Array();

        <?php
        $sistemas = $sistemas ? $sistemas : array();
        foreach ( $sistemas as $sistema ) {
            if ($sistema instanceof StdClass) {
                echo "antigo[", $sistema->sisid, "] = '", $sistema->usuariosistema->suscod, "';\n";
                echo "status_sistema_alterado[", $sistema->sisid, "] = false;\n";
            } else {
                echo "antigo[", $sistema ['sisid'], "] = '", null, "';\n";
                echo "status_sistema_alterado[", $sistema ['sisid'], "] = false;\n";
            }
        }
        ?>

        function alterar_status_sistema( sisid ){
            //atribuindo selected=selected, porém com active.
            if ( jQuery('#status_ativo_' + sisid).parent().hasClass( "active" ) ) {
                var novo = jQuery('#status_ativo_' + sisid).val();
            }
            if ( jQuery('#status_pendente_' + sisid).parent().hasClass( "active" ) ) {
                var novo = jQuery('#status_pendente_' + sisid).val();
            }
            if ( jQuery('#status_bloqueado_' + sisid).parent().hasClass( "active" ) ) {
                var novo = jQuery('#status_bloqueado_' + sisid).val();
            }

            status_sistema_alterado[sisid] = antigo[sisid] != novo;
            //exibindo campo de justicativa quando status pendente ou bloqueado
            if ( status_sistema_alterado[sisid] ) {
                jQuery( '#justificativa_on_' + sisid ).show();
                jQuery( '#justificativa_off_' + sisid ).hide();
            } else {
                jQuery( '#justificativa_on_' + sisid ).hide();
                jQuery( '#justificativa_off_' + sisid ).show();
            }
        }

    }
    function enviar_formulario(){
        if ( validar_formulario() ) {
            //selectAllOptions( document.getElementById( 'pflcod' ) );
            setTimeout(function(){  jQuery( "#formulario" ).submit(); }, 1000);

        }
    }

    var validar_uo = false;
    function listar_unidades_orcamentarias( orgcod ){
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
        while( campo_select.options.length ){
            campo_select.options[0] = null;
        }
        campo_select.options[0] = new Option( '', '', false, true );

        var div_on = document.getElementById( 'unicod_on' );
        var div_off = document.getElementById( 'unicod_off' );
        if ( !lista_uo[orgcod] ){
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
        for ( var i = 0; i < j; i++ ){
            campo_select.options[campo_select.options.length] = new Option( lista_uo[orgcod][i][1], lista_uo[orgcod][i][0], false, lista_uo[orgcod][i][0] == '<?php echo $unicod ?>' );
        }
        if ( navigator.appName == 'Microsoft Internet Explorer' ) {
            for ( i=0; i< campo_select.length; i++ ){
                if ( campo_select.options(i).value == '<?php echo $unicod ?>' ) {
                        campo_select.options(i).selected = true;
                }
            }
        }
    }

    function listar_municipios2(regcod){
        var campo_select = jQuery('#muncod').get(0);
        while( campo_select.options.length ){
            campo_select.options[0] = null;
        }
        campo_select.options[0] = new Option( '', '', false, true );

        var div_on = document.getElementById( 'muncod_on' );
        var div_off = document.getElementById( 'muncod_off' );

        if ( !lista_mun[regcod] ){
            validar_mun = false;
            div_on.style.display = 'none';
            div_off.style.display = 'block';
            return;
        }

        validar_mun = true;
        div_on.style.display = 'block';
        div_off.style.display = 'none';
        var j = lista_mun[regcod].length;
        for ( var i = 0; i < j; i++ ){
            campo_select.options[campo_select.options.length] = new Option( lista_mun[regcod][i][1], lista_mun[regcod][i][0], false, lista_mun[regcod][i][0] == '<?php echo $muncod ?>' );
        }
        if ( navigator.appName == 'Microsoft Internet Explorer' ) {
            for ( i=0; i< campo_select.length; i++ ){
                if ( campo_select.options(i).value == '<?php echo $muncod ?>' ) {
                    campo_select.options(i).selected = true;
                }
            }
        }
    }


    var validar_ug = false;
    function listar_unidades_gestoras(unicod){
        var campo_select = document.getElementById( 'ungcod' );
        while( campo_select.options.length ){
            campo_select.options[0] = null;
        }
        campo_select.options[0] = new Option( '', '', false, true );
        var div_on = document.getElementById( 'ungcod_on' );
        var div_off = document.getElementById( 'ungcod_off' );
        if ( !lista_ug[unicod] ){
            validar_ug = false;
            div_on.style.display = 'none';
            div_off.style.display = 'block';
            return;
        }
        validar_ug = true;
        div_on.style.display = 'block';
        div_off.style.display = 'none';
        var j = lista_ug[unicod].length;
        for ( var i = 0; i < j; i++ ){
            campo_select.options[campo_select.options.length] = new Option( lista_ug[unicod][i][1], lista_ug[unicod][i][0], false, lista_ug[unicod][i][0] == '<?php echo $ungcod ?>' );
        }
        if ( navigator.appName == 'Microsoft Internet Explorer' ) {
            for ( i=0; i < campo_select.length; i++ ){
                if ( campo_select.options(i).value == '<?php echo $ungcod ?>' ){
                    campo_select.options(i).selected = true;
                }
            }
        }
    }

    function validar_formulario(){
        var validacao = true;
        var mensagem = 'Os seguintes campos não foram preenchidos:';
        jQuery('[name^=status]').each(function(){

            if(jQuery(this).parent().hasClass( "active" ) && jQuery(this).val() == 'B'){
                if(permissao){
                    if ( jQuery('#usunome').val() == '' ) {
                        mensagem += '\nNome \n';
                        validacao = false;
                    }
                }
                if ( !validaEmail( jQuery('#usuemail').val() ) ) {
                    mensagem += '\n E-mail \n';
                    validacao = false;
                }
            }

            if(jQuery(this).parent().hasClass( "active" ) && jQuery(this).val() != 'B'){
                if ( jQuery('#muncod').val() == '' ) {
                    mensagem += '\n Município \n';
                    validacao = false;
                }

                if(permissao){
                    if ( jQuery('#usunome').val() == '' ) {
                        mensagem += '\n Nome \n';
                        validacao = false;
                    }
                }

                if(jQuery('[name=ususexo]').parent().hasClass( "active" ) == false){
                    mensagem += '\n Sexo \n';
                    validacao = false;
                }

                if( ! permissao){
                    if ( jQuery('#usudatanascimento').val().length < 10 ) {
                        mensagem += '\n Data de Nascimento \n';
                        validacao = false;
                    }
                } else if( jQuery('#usudatanascimento').val().length > 0 && jQuery('#usudatanascimento').val().length < 10 ) {
                    mensagem += '\n Data de Nascimento \n';
                    validacao = false;
                }

                if ( jQuery('#regcod').val() == '' ) {
                    mensagem += '\n Unidade Federal \n';
                    validacao = false;
                }

                if ( jQuery('#tpocod').val() == '' ) {
                    mensagem += '\n Tipo do Órgão \n';
                    validacao = false;
                }

                if ( jQuery('#entid') ){
                    if ( jQuery('#entid').val() == '' ) {
                        mensagem += '\n Órgão \n';
                        validacao = false;
                    }
                }

                if ( jQuery('#usufoneddd').val() == '' || jQuery('#usufonenum').val() == '' ) {
                    mensagem += '\n Telefone \n';
                    validacao = false;
                }

                if ( !validaEmail( jQuery('#usuemail').val() ) ) {
                    mensagem += '\n E-mail \n';
                    validacao = false;
                }

                if ( jQuery('#carid') ) {
                    if ( jQuery('#carid').val() == '' ) {
                        mensagem += '\n\t Função/Cargo \n';
                        validacao = false;
                    }else{
                        if( jQuery('#carid').val() == 9 ){
                            if ( jQuery('#usufuncao').val() == '' ) {
                                mensagem += '\n Função \n';
                                validacao = false;
                            }
                        }
                    }
                }

                // verifica a alteração de status
                var status      = true;
                var ativo_geral = jQuery('[name^=status]').parent().hasClass( "active" );
                var htudsc      = jQuery('[name^=justificativa]').val();

                if ( status_geral_alterado && htudsc == '' && ativo_geral == false ) { status = false;}
              }
        });

        if ((jQuery('#entid')[0] && '' === jQuery('#entid').val())
             || (jQuery('#orgao')[0] && '' === jQuery('#orgao').val())) {
            mensagem += 'Órgão';
            validacao = false;
        }

        if ( !validacao ) {
            $('#modal-alert .modal-body').html(mensagem);
            $('#modal-alert').modal();
            return;
        }

        return validacao;
    }

    function voltar(){
        <?php if ( $_SESSION['sisid'] == 147 ): ?>
        window.location.href = '?modulo=sistema2/usuario2/consusuario&acao=<?php echo $_REQUEST['acao'] ?>';
        <?php else: ?>
        window.location.href = '?modulo=sistema/usuario/consusuario&acao=<?php echo $_REQUEST['acao'] ?>';
        <?php endif; ?>
    }

     function exibir_ocultar_historico( id ){
            div = document.getElementById( id );
            if ( div.style.display == 'none' ) {
                    div.style.display = 'block';
            } else {
                    div.style.display = 'none'
            }
    }


    if( jQuery('#carid').val() == 9 ){
            usufuncao.style.display = "";
            linkVoltar.style.display = "";
            carid.style.display = "none";
    }

    function alternarExibicaoCargo( tipo ){

        var carid = document.getElementById( 'carid' );
        //var usufuncao = document.getElementById( 'usufuncao' );
        //var link = document.getElementById( 'linkVoltar' );

        if( tipo != 'exibirOpcoes' ){
            if( jQuery('#carid').val() == 9 ){
                jQuery('#usufuncao').show();//usufuncao.style.display = "";
                jQuery('#linkVoltar').show();//link.style.display = "";
                //carid.style.display = "hidden";
                jQuery('#carid').next().hide();
            }
        }
        else{
            jQuery('#usufuncao').hide();//usufuncao.style.display = "none";
            jQuery('#linkVoltar').hide();//link.style.display = "none";
            jQuery('#carid').next().show();
            //carid.style.display = "";
            //carid.value = "";
        }
    }

    function visualizaDadosSiape(cpf) {

        var url = '../geral/dadosUsuarioSIAPE.php?cpf='+cpf;

        var janela = window.open(url, '_blank', 'width=500,height=400,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
        janela.focus();
    }

</script>
<?php
    function enviarEmailSuperUser($usucpf, $pflcod) {
	global $db;
	$sql = "
            SELECT
                u.usunome,u.usucpf,u.regcod,u.usuemail,u.usufoneddd,u.usufonenum,u.usufuncao,
                p.pflcod,p.pfldsc,
                s.sisid,s.sisdsc,s.sisabrev,
                u2.usunome as usunomeorigem, u2.usucpf as usucpforigem, u2.usuemail as usuemailorigem,  u2.usufoneddd as usufonedddorigem, u2.usufonenum as usufonenumorigem
            FROM seguranca.perfilusuario pu
            INNER JOIN seguranca.perfil p ON p.pflcod = pu.pflcod
            INNER JOIN seguranca.sistema s ON s.sisid = p.sisid
            INNER JOIN seguranca.usuario u ON u.usucpf = pu.usucpf
            INNER JOIN seguranca.usuario u2 ON u2.usucpf = '{$_SESSION['usucpforigem']}'
            WHERE pu.usucpf = '{$usucpf}'
                AND pu.pflcod = '{$pflcod}' ";
	$dados = $db->pegaLinha ($sql);

	$remetente = array (
            "nome" => SIGLA_SISTEMA,
            "email" => $_SESSION['email_sistema']
	);
	$destinatarios = recuperarDestinatariosPadrao ();

	$assunto = "Novo super-usuário atribuído ({$dados['sisabrev']})";
	$mensagem = "
        <pre>
            Prezados,
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

            Link para verificação do usuário: ". URL_SISTEMA. "seguranca/seguranca.php?modulo=sistema/usuario/cadusuario&acao=A&usucpf={$dados['usucpf']}

            Qualquer dúvida, entrar em contato com o gestor do módulo.

            Atenciosamente,
            Equipe ". SIGLA_SISTEMA. "
	</pre>
	";
	$retorno = enviar_email ( $remetente, $destinatarios, $assunto, $mensagem );
    }
    function enviarAlteracaoSuperUser($dados, $tipo) {
	if ($_SESSION ['ambiente'] != 'Ambiente de Produção') {
            // return false;
	}

	$remetente = array (
            "nome" => SIGLA_SISTEMA,
            "email" => $_SESSION['email_sistema']
	);
	$destinatarios = recuperarDestinatariosPadrao ();

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

        $mensagem = "
            <pre>
                Prezados,

                {$texto}

                Módulo: {$_SESSION['sisdsc']} ({$_SESSION['sisabrev']})

                Nome: {$dados['usunome']}
                CPF: {$dados['usucpf']}
                E-mail: {$dados['usuemail']}
                UF: {$dados['regcod']}
                Telefone: ({$dados['usufoneddd']}) {$dados['usufonenum']}
                Função: {$dados['usufuncao']}

                Responsável pela alteracao: {$_SESSION['usunome']} ({$_SESSION['usucpf']}) - {$_SESSION['usuemail']}

                Link para verificação do usuário: ". URL_SISTEMA. "seguranca/seguranca.php?modulo=sistema/usuario/cadusuario&acao=A&usucpf={$dados['usucpf']}

                Qualquer dúvida, entrar em contato com o gestor do módulo.

                Atenciosamente,
                Equipe ". SIGLA_SISTEMA. "
            </pre>
        ";
        return enviar_email($remetente, $destinatarios, $assunto, $mensagem);
    }

    function recuperarDestinatariosPadrao(){
        return array($_SESSION['email_sistema']);
    }
?>

<style>
   .btn-group > .btn:hover, .btn-group-vertical > .btn:hover, .btn-group > .btn:focus, .btn-group-vertical > .btn:focus, .btn-group > .btn:active, .btn-group-vertical > .btn:active, .btn-group > .btn.active, .btn-group-vertical > .btn.active{z-index: 1}
</style>