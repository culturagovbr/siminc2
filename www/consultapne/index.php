<?php
error_reporting(1);
error_reporting(E_ALL ^ E_NOTICE);

// set_include_path('.;D:\Workspace\php\pdeinterativo\includes;D:\Workspace\php\pdeinterativo\global;');
// $_SESSION['usucpforigem'] = '';
// $_SESSION['usucpf'] = '';
// $_SESSION['superuser'] = '1';

include "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once "classes/Configuracao.class.inc";
include_once "classes/Participante.class.inc";
include_once "classes/Estado.class.inc";
include_once "classes/Municipio.class.inc";


$participante = new Participante();
$configuracao = new Configuracao();
$estado = new Estado();
$municipio = new Municipio();

$estados = $estado->recuperarTodos('estuf, estdescricao', null, 'estdescricao');

switch ($_REQUEST['action'])
{
    case 'carregar':
    	$participante->recuperarParticipante(preg_replace("/[^0-9]/", "", trim($_REQUEST['usucpf'])));
    	print json_encode(array('parcnpj'=>$participante->parcnpj, 'parrepresentacao'=>$participante->parrepresentacao));
    	die;
    	break;
}

$sql = "select  to_char(dataexpiracao, 'YYYY-MM-DD') data_termino,
                to_char(dataexpiracao, 'YYYYMMDDHH24MISS') data_expiracao,
                to_char(dataexpiracao, 'DD/MM/YYYY \�\s HH24:MI:SS') as data_formatada
        from consultapne.configuracao";

$dados = $configuracao->pegaLinha($sql);

$dataTermino = $dados['data_termino'];
$dataExpiracao = $dados['data_expiracao'];
$dataFormatada = $dados['data_formatada'];
$dias = floor((strtotime($dataTermino) - strtotime(date('Y-m-d'))) / ((60*60*24)+1));

?>
<!DOCTYPE html>
<html lang="pt-BR">
    <?php require "head.php"; ?>

    <style>
        body {
            background-image: url("imagens/bg.png");
        }
    </style>

    <body>
	<input type="hidden" id="datatermino" value="<?php echo $dataTermino; ?>" />
    <header>
        <div class="row">
            <div class="col-lg-12 col-sm-12 col-xs-12">
            	<div style="width: 180px; float: left;">
                    <img src="imagens/logo-simec.png" class="res" width="150">
                </div>
                <div style="width: 300px; float: left; text-align: left">
                	<div class="countdown-container data-termino"></div>
                </div>
                <div style="width: 180px; float: right;">
                	<a class="brasil pull-right" href="http://www.brasil.gov.br/"><img alt="Brasil - Governo Federal" src="/estrutura/temas/default/img/brasil.png" style="margin-right: 10px;"></a>
                </div>
            </div>
        </div>
    </header>

    <div class="container">
        <div class="row">
            <div class="col-lg-12 col-sm-12 col-xs-12">
                <h2 style="text-align: center;">Consulta P�blica sobre os Indicadores para Acompanhamento das Metas do PNE</h2>
            </div>
        </div>

        <div class="row">
            <?php if(date('YmdHis') >= $dataExpiracao) : ?>
                <div class="col-lg-12 col-sm-12 col-xs-12">
                    <div class="alert alert-success" role="alert">
                        <p>Envio de receitas encerrado em <?php echo $dataFormatada; ?>.</p>
                        <p>Obrigado pela colabora��o!</p>
                    </div>
                </div>
            <?php else : ?>
                <div class="col-lg-7 col-sm-7 col-xs-7" id="informativos">
                    <div class="alert alert-success">

                        <div class="text-center">
                            <strong>
                                Consulta P�blica <br/>
                                Plano Nacional de Educa��o PNE 2014-2014: Linha de Base<br/>
                                Consulta p�blica sobre os indicadores selecionados para o monitoramento e avalia��o do PNE<br/>
                                13 de outubro a 11 de dezembro de 2015
                            </strong>
                        </div>

                        <p>O Plano Nacional de Educa��o (PNE) prev� em seu artigo 5� que, a cada 2 (dois) anos ao longo de sua vig�ncia, o Instituto Nacional de Estudos e Pesquisas Educacionais An�sio Teixeira (Inep) publique estudos para aferir a evolu��o no cumprimento de suas metas, com informa��es organizadas por ente federado e consolidadas em �mbito nacional. Esses estudos dever�o servir como subs�dio para o monitoramento cont�nuo e avalia��es peri�dicas da execu��o do PNE e do cumprimento de suas metas, a serem realizadas pelas seguintes inst�ncias: Minist�rio da Educa��o - MEC; Comiss�o de Educa��o da C�mara dos Deputados e Comiss�o de Educa��o, Cultura e Esporte do Senado Federal; Conselho Nacional de Educa��o - CNE; F�rum Nacional de Educa��o.</p>
                        <p>Visando a atender a essa determina��o, o Inep, por meio de sua Diretoria de Estudos Educacionais, disponibiliza � sociedade o documento "Plano Nacional de Educa��o: Linha de Base" que apresenta, em car�ter preliminar, indicadores selecionados pelo Inep e pelo MEC para o monitoramento do PNE 2014-2024.</p>
                        <p>O documento consiste de an�lises descritivas das s�ries hist�ricas dos indicadores, bem como de recortes (por exemplo, regionais, sexo, renda e localiza��o de resid�ncia e da escola). As informa��es foram extra�das dos dados provenientes das pesquisas do Inep (Censo da Educa��o B�sica, Censo da Educa��o Superior, Saeb, Ideb), do IBGE (Pnad e Censo Demogr�fico) e da Capes (dados da p�s-gradua��o), dispon�veis na data de promulga��o da Lei do PNE, em 25 de junho de 2014. Al�m de estabelecer uma linha de base para o acompanhamento das Metas, esse documento objetiva desencadear o debate a respeito dos indicadores escolhidos para a aferi��o do cumprimento das metas estabelecidas no Plano durante sua vig�ncia.</p>
                        <p>Nesse sentido, esta consulta p�blica tem como objetivo coletar contribui��es sistematizadas de indiv�duos e institui��es, visando ao aprimoramento dos indicadores selecionados e, eventualmente, ao desenvolvimento de outros indicadores para acompanhamento das metas. S�o aceitas participa��es individuais ou institucionais. Sugest�es, cr�ticas e propostas poder�o ser enviadas ao Inep no per�odo de 13 de outubro a 11 de dezembro de 2015.</p>
                        <p><strong><a target="_blank" href="http://www.publicacoes.inep.gov.br/portal/download/1362">Clique aqui para acessar o documento "Plano Nacional de Educa��o: Linha de Base".</a></strong></p>
                        <p><strong><a target="_blank" href="http://pesquisa.in.gov.br/imprensa/jsp/visualiza/index.jsp?jornal=1&pagina=15&data=13/10/2015">Clique aqui para acessar a Portaria n� 424, de 09 de outubro de 2015, que regulamenta esta consulta p�blica.</a></strong></p>
                    </div>
                </div>
                <div class="col-lg-5 col-sm-5 col-xs-5">
                    <form method="post" id="form" action="controlador.php" class="well">
                    	<div class="row">
                            <div class="col-lg-12 col-sm-12 col-xs-12">
                                <div class="form-group <?php echo ( isset($_GET['usucpf']) ? ' has-error' : ''); ?>">
                                    <label for="usucpf">CPF: </label>
                                    <input required type="text" class="form-control cpf" id="usucpf" name="usucpf" placeholder="CPF">
                                    <?php echo ( (isset($_GET['usucpf']) and $_GET['usucpf'] == 1) ? ' <p class="help-block has-error">Campo Obrigat�rio</p>' : ''); ?>
                                    <?php echo ( (isset($_GET['usucpf']) and $_GET['usucpf'] == 2) ? ' <p class="help-block has-error">CPF Incorreto!</p>' : ''); ?>
                                </div>
                                <div class="form-group">
                                    <input type="hidden" name="parrepresentacao" id="parrepresentacao" value="<?php echo $_GET['parrepresentacao'];?>">
                                    <label for="parrepresentacaoCombo">Tipo de Representa��o: </label>
                                    <select class="form-control" name="parrepresentacaoCombo" id="parrepresentacaoCombo" required>
                                        <option value="">Selecione</option>
                                        <?php foreach (Participante::$tiposRepresentacao as $id => $label): ?>
                                        <?php $selected = $_GET['parrepresentacao'] == $id ? 'selected="selected"' : null; ?>
                                            <option <?php echo $selected; ?> value="<?php echo $id; ?>"><?php echo $label; ?></option>
                                        <?php endforeach; ?>
                                    </select>
                                    <?php echo ( (isset($_GET['flagrepresentacao']) and $_GET['flagrepresentacao'] == 1) ? ' <p class="help-block has-error">Campo Obrigat�rio</p>' : ''); ?>
                                </div>
                                <div class="form-group representante_cnpj <?php echo ( isset($_GET['usucnpj']) ? ' has-error' : ''); ?>" style="display: none;">
                                    <input type="hidden" id="usucnpj" name="usucnpj" value="<?php echo $_GET['usucnpj'];?>">
                                    <label for="usucnpjCombo">CNPJ: </label>
                                        <input type="text" class="form-control cnpj" id="usucnpjCombo" name="usucnpjCombo" placeholder="CNPJ">
                                        <?php echo ( (isset($_GET['usucnpj']) and $_GET['usucnpj'] == 1) ? ' <p class="help-block has-error">Campo Obrigat�rio</p>' : ''); ?>
                                        <?php echo ( (isset($_GET['usucnpj']) and $_GET['usucnpj'] == 2) ? ' <p class="help-block has-error">CNPJ Incorreto!</p>' : ''); ?>
                                    </div>
                                    <div class="form-group <?php echo ( isset($_GET['captcha']) ? ' has-error' : ''); ?>">
                                        <img src="captcha.php" width="113" height="49">
                                        <input type="text" class="form-control" name="captcha" id="captcha" maxlength="4" size="20" placeholder="Digite os caracteres da imagem"/>
                                        <?php echo ( (isset($_GET['captcha']) and $_GET['captcha'] == 1) ? ' <p class="help-block has-error">Campo Obrigat�rio</p>' : ''); ?>
                                        <?php echo ( (isset($_GET['captcha']) and $_GET['captcha'] == 2) ? ' <p class="help-block has-error">Caracteres Incorretos - digite novamente!</p>' : ''); ?>
                                    </div>
                                    <button type="submit" class="btn btn-primary"><i class="fa fa-check-square-o"></i> Participar</button>
                                </div>
                            </div>
                        </form>
                    <?php endif; ?>
                </div>
            </div>
            <hr>
            <?php require "footer.php"; ?>
            <?php if ($_GET['parrepresentacao']) : ?>
                <script>$(document).ready(function() { $('#representacao').trigger('change'); }) </script>
            <?php endif; ?>
        </div>
    </body>
</html>