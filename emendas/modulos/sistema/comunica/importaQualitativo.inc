<?php
/**
 * PLANACOMORC
 * base: Programação Orçamentária
 */

ini_set("memory_limit", "2048M");
set_time_limit(300000);

if ('enviasolicitacao' == $_POST['requisicao']) {

    $arrParam = array(
        'usuario' => $_POST['wsusuario'],
        'senha' => $_POST['wssenha']);

    if (('obterCargaOrcamentaria' != $_POST['documento']) && ('obterCargaOrcamentaria2' != $_POST['documento'])) {
        $arrParam['post'] = $_POST;
        include_once APPRAIZ . 'planacomorc/classes/WSQualitativo.class.inc';
        include_once APPRAIZ . 'planacomorc/classes/SiopQualitativo.class.inc';
        require_once(APPRAIZ . 'planacomorc/modulos/sistema/comunica/funcoesImportacao.inc');
        $oSiop = new SiopQualitativo($arrParam);
    }

  $documento = $_POST['documento'];

  // -- Indica se houve sucesso na requisição ao webservice
  $wsSucesso = false;
  $numProcessados = array();
  $wsErroMessage = null;

  if ('obterProgramacaoCompletaQualitativo' == $documento) {
    $arrResponse = $oSiop->obterProgramacaoCompleta();
    if ($arrResponse->return->sucesso) {
      $wsSucesso = true;
      $numProcessados['Órgãos'] = insereOrgaosDTO($arrResponse->return->orgaosDTO);
      $numProcessados['Programas'] = insereProgramasDTO($arrResponse->return->programasDTO);
      $numProcessados['Objetivos'] = insereObjetivosDTO($arrResponse->return->objetivosDTO, $upsert = true);
      $numProcessados['Iniciativas'] = insereIniciativasDTO($arrResponse->return->iniciativasDTO, $upsert = true); // -- iniciativas
      $numProcessados['Ações'] = insereAcoesDTO($arrResponse->return->acoesDTO);
      $numProcessados['Localizadores'] = insereLocalizadoresDTO($arrResponse->return->localizadoresDTO);
      $numProcessados['POs'] = inserePlanosOrcamentariosDTO($arrResponse->return->planosOrcamentariosDTO, $upsert = true);

    } else {
      $wsErroMessage = $arrResponse->return->mensagensErro;
    }
  } elseif('obterTabelasApoio' == $documento) { // -- Documentos processados de forma comum
    $retornoApoioQualitativoDTO = $oSiop->obterTabelasApoio(); //die(var_dump($retornoApoioQualitativoDTO));
    if ($retornoApoioQualitativoDTO->return->sucesso) {
      $wsSucesso = true;
      $numProcessados['Esferas'] = insereEsferasDTO(
              $retornoApoioQualitativoDTO->return->esferasDTO, true);
      $numProcessados['Produtos'] = insereProdutosDTO(
              $retornoApoioQualitativoDTO->return->produtosDTO, true);
      $numProcessados['Funções'] = insereFuncoesDTO(
              $retornoApoioQualitativoDTO->return->funcoesDTO, true);
      $numProcessados['Unidades de Medida'] = insereUnidadesMedidaDTO(
              $retornoApoioQualitativoDTO->return->unidadesMedidaDTO, true);
      $numProcessados['Subfunções'] = insereSubFuncoesDTO(
              $retornoApoioQualitativoDTO->return->subFuncoesDTO, true);
      $numProcessados['Tipos de Ação'] = insereTiposAcaoDTO(
              $retornoApoioQualitativoDTO->return->tiposAcaoDTO, true);
      $numProcessados['Tipos de Inclusão'] = insereTiposInclusaoDTO(
              $retornoApoioQualitativoDTO->return->tiposInclusaoDTO, true);
    } else {
      $wsErroMessage = $retornoApoioQualitativoDTO->return->mensagensErro;
    }
  }else if (('obterCargaOrcamentaria' == $documento) || ('obterCargaOrcamentaria2' == $documento)) {
      include_once APPRAIZ . 'planacomorc/classes/CargaOrcamentaria.class.inc';
      $cargaOrcamentaria = new CargaOrcamentaria($arrParam);
      $cargaOrcamentaria->obterTabelasDeApoio($documento);
      $msg = '<script>alert("Carga realizada com sucesso.");';
      $msg .= 'window.location.href = "planacomorc.php?modulo=sistema/comunica/importaQualitativo&acao=A";</script>';
      echo $msg;
      exit('finalizou');
  }
  if ($wsSucesso) {
    // -- número de registros processados por tipo
    $msg = '<script>alert("Dados atualizados com sucesso:';
    foreach ($numProcessados as $nome => $qtd) {
      $msg .= '\n' . "* {$nome}: {$qtd}" . '';
    }
    $msg .= '"); window.location.href = "planacomorc.php?modulo=sistema/comunica/importaQualitativo&acao=A";</script>';
  } else {
    $msg = '<script>alert("' . $wsErroMessage . '!");'
            . 'window.location.href = "planacomorc.php?modulo=sistema/comunica/importaQualitativo&acao=A";</script>';
  }
  echo $msg;
  exit();
}

include APPRAIZ . "includes/cabecalho.inc";
monta_titulo('Comunicação de Dados - SIOP', 'Qualitativos');

$wsusuario = WEB_SERVICE_SIOP_USUARIO;
$wssenha = WEB_SERVICE_SIOP_SENHA;

$largura = "300px";
$altura = "215px";
$id = "div_auth";
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="/planacomorc/js/importa.js"></script>
<link rel="stylesheet" type="text/css" href="/planacomorc/css/importa.css"/>
<style>
.popup_alerta{
  width:<?php echo $largura ?>;
  height:<?php echo $altura ?>;
  position:absolute;
  z-index:0;
  top:50%;
  left:50%;
  margin-top:-<?php echo $altura/2 ?>;
  margin-left:-<?php echo $largura/2 ?>;
  border:solid 2px black;
  background-color:#FFFFFF;
  display:none;
  overflow:auto}
label{cursor:pointer}
</style>
<form method="post" name="formulario" id="formulario">
  <table align="center" width="95%" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
    <tr>
      <td class="subtitulodireita" width="30%"><b>Funções:</b></td>
      <td style="padding: 0 20px 20px 20px;" width="70%">
        <table align="center" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
          <tr><td>
            <input type="radio" name="documento" value="obterCargaOrcamentaria"
                   class="chamadaWs" id="documento_cargaOrcamentaria"
                   onclick="mostraCombo(this.value);" />
            <label for="documento_cargaOrcamentaria"><b>Carga Orçamentária</b></label>
            <div id="obterCargaOrcamentaria" style="display: none;">
              <div class="msg-info" style="width:500px">
                  Observações:<br />
                  <ul><li>Estas importações fazem a carga das Tabelas de Apoio Qualitativas para o esquema <b><u>PLANACOMORC</u></b>.</li>
                      <li>Os updates/inserts fazem avaliação de chaves primárias e estrangeiras para inserir os novos itens encontradas.</li>
                      <li>Todas elas são feitas <b><u>ATUALIZANDO</u></b> os registros inseridos previamente.</li>
                      <li>Itens importados:
                          <ul><li>Tipos de Inclusão</li>
                              <li>Macro Desafios</li>
                              <li>Tipos de Programa</li>
                              <li>Funções</li>
                              <li>Esferas</li>
                              <li>Produtos</li>
                              <li>Unidades de Medida</li>
                              <li>Tipos de Ação</li>
                              <li>Subfunções</li></ul>
                      </li>
                  </ul>
              </div>
            </div>
          </td></tr>
          <tr><td>
            <input type="radio" name="documento" value="obterCargaOrcamentaria2"
                   class="chamadaWs" id="documento_cargaOrcamentaria2"
                   onclick="mostraCombo(this.value);" />
            <label for="documento_cargaOrcamentaria2"><b>Carga Orçamentária - Etapa 2</b></label>
            <div id="obterCargaOrcamentaria2" style="display: none;">
              <div class="msg-info" style="width:500px">
                  Observações:<br />
                  <ul><li>Estas importações fazem a carga da Programação Completa Qualitativa para o esquema <b><u>PLANACOMORC</u></b>.</li>
                      <li>Os updates/inserts fazem avaliação de chaves primárias e estrangeiras para inserir os novos itens encontradas.</li>
                      <li>Todas elas são feitas <b><u>ATUALIZANDO</u></b> os registros inseridos previamente.</li>
                      <li>Itens importados:
                          <ul><li>Orgãos</li>
                              <li>Localizadores</li>
                              <li>Programas</li>
                              <li>Objetivos</li>
                              <li>Iniciativas</li>
                              <li>Ações</li>
                              <li>Planos Orçamentários</li></ul>
                      </li>
                      <li><b><u>IMPORTANTE</u></b>: Atualmente, as tabelas <b><u>PLANACOMORC.ACAO_PROGRAMATICA</u></b> e <b><u>PLANACOMORC.DADOS_ACAO_EXERCICIO</u></b>
                      são apenas incrementadas.</li>
                  </ul>
              </div>
            </div>
          </td></tr>
          <tr><td>
            <input type="radio" name="documento" value="obterProgramacaoCompletaQualitativo"
                   class="chamadaWs" id="documento_obterProgramacaoCompletaQualitativo"
                   onclick="mostraCombo('obterProgramacaoCompletaQualitativo');" />
            <label for="documento_obterProgramacaoCompletaQualitativo">Obter Programação Completa Qualitativo:</label>
            <div id="obterProgramacaoCompletaQualitativo" style="display: none;">
              <div class="msg-info" style="width:500px">
                  Observações:<br />
                  <ul><li>Estas importações fazem a carga de alguns itens da Programação Completa Qualitativa para o esquema <b><u>PLANACOMORC_OLD</u></b>.</li>
                      <li>Todas elas são feitas <b><u>ATUALIZANDO</u></b> os registros inseridos previamente.</li>
                      <li><b><u>IMPORTANTE</u></b>: <b><u>Retornar PO</u></b> insere na tabela <b><u>WSSOF.WS_PLANOSORCAMENTARIOSDTO</b></u> após <b><u>APAGAR</b></u> seus registros.</li>
                  </ul>
              </div>
              <table align="center" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
                <?php $arProgComp = array(
                    'retornarOrgaos' => 'Retornar Órgãos',
                    'retornarProgramas' => 'Retornar Programas',
                    'retornarObjetivos' => 'Retornar Objetivos',
                    'retornarIniciativas' => 'Retornar Iniciativas',
                    'retornarAcoes' => 'Retornar Ações',
                    'retornarLocalizadores' => 'Retornar Localizadores',
                    'retornarPlanosOrcamentarios' => 'Retornar Planos Orçamentários (PO)');
                  foreach ($arProgComp as $key => $desc) {
                    echo <<<HTML
                  <tr>
                    <td class="subtitulodireita" width="30%">{$desc}:</td>
                    <td width="70%">
                      <input type="radio" name="{$key}" value="1" id="qtba{$key}s" />
                      <label for="qtba{$key}s">Sim</label>
                      <input type="radio" name="{$key}" value="0" id="qtba{$key}n" checked="checked" />
                      <label for="qtba{$key}n">Não</label>
                    </td>
                  </tr>
HTML;
                  }
                ?>
              </table>
            </div>
          </td></tr>
          <tr><td>
            <input type="radio" name="documento" id="documento_obterTabelasApoio"
                   value="obterTabelasApoio" onclick="mostraCombo(this.value);"
                   class="chamadaWs" /><label for="documento_obterTabelasApoio">Tabelas Apoio (Busca dados das tabelas de apoio):</label>
            <div id="obterTabelasApoio" style="display:none;">
              <div class="msg-info" style="width:500px">
                  Observações:<br />
                  <ul><li>Estas importações fazem a carga de alguns itens das Tabelas de Apoio Qualitativas para o esquema <b><u>PLANACOMORC_OLD</u></b>.</li>
                      <li>Todas elas são feitas <b><u>ATUALIZANDO</u></b> os registros inseridos previamente.</li>
                  </ul>
              </div>
              <table align="center" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
                  <?php
                  $arTabelasApoio = array(
//                      'retornarEsferas' => 'Retornar Esferas',
                      'retornarProdutos' => 'Retornar Produtos',
                      'retonarFuncoes' => 'Retonar Funções',
                      'retornarUnidadesMedida' => 'Retornar Unidades Medida',
                      'retornarSubFuncoes' => 'Retornar SubFunções',
//                      'retornarTiposAcao' => 'Retornar Tipos Ação',
//                      'retornarTiposInclusao' => 'Retornar Tipos Inclusão',
                      );
                  foreach ($arTabelasApoio as $key => $desc) {
                    echo <<<HTML
                  <tr>
                    <td class="subtitulodireita" width="30%">{$desc}:</td>
                    <td width="70%">
                      <input type="radio" name="{$key}" value="1" id="qtba{$key}s" />
                      <label for="qtba{$key}s">Sim</label>
                      <input type="radio" name="{$key}" value="0" id="qtba{$key}n" checked="checked" />
                      <label for="qtba{$key}n">Não</label>
                    </td>
                  </tr>
HTML;
                  }
                  ?>
              </table>
            </div>
          </td></tr>
        </table>
      </td>
    </tr>
    <tr>
      <td class="subtitulodireita"><b>Código do Momento:</b></td>
      <td><? echo campo_texto("codigomomento","S","S","codigomomento","22","","[#]","","","","","id='codigomomento'", '', '2000'); ?></td>
    </tr>
    <tr>
      <td colspan="2" align="center" style="background-color:#c0c0c0;"><input type="button" class="botao" value="Enviar Solicitação" onclick="solicitarExecucao();"></td>
    </tr>
</table>
<br />
<input type="hidden" value="" name="requisicao" id="requisicao">
<?php require(dirname(__FILE__) . '/popup.inc'); ?>
</form>
<script type="text/javascript">
</script>
<?
/**
 * Insere novas ações na base e atualiza as existentes.
 *
 * @global cls_banco $db
 * @param array $arrDados Lista de acaoDTO retornado pelo webservice.
 * @return null|boolean
 * @see cls_banco
 * @see acaoDTO
 */
 function insereAcoesDTO($arrDados) {
  global $db;

  if (is_array($arrDados)) {
    // -- Lista as acoes já cadastradas
    $sql = <<<QUERY
SELECT a.acaidentificadorunicosiop
  FROM planacomorc_old.acao a
QUERY;

    $arrAcoes = $db->carregarColuna($sql);
    if (!is_array($arrAcoes)) {
      $arrAcoes = array();
    }

    foreach ($arrDados as $acaoDTO) {
      // -- tratamento de aspas simples que quebram o upsert
      $acaoDTO->detalhamentoImplementacao = str_replace("'", "''", $acaoDTO->detalhamentoImplementacao);
      $acaoDTO->descricao = str_replace("'", "''", $acaoDTO->descricao);
      $acaoDTO->baseLegal = str_replace("'", "''", $acaoDTO->baseLegal);

      // -- Tratamento de campo booleano
      $snAcompanhamentoOpcional = $acaoDTO->snAcompanhamentoOpcional?'t':'f';

      // -- Verificando se a acao já está cadastrada no sistema
      if (in_array($acaoDTO->identificadorUnico, $arrAcoes)) {

        $sql = <<<DML
UPDATE planacomorc_old.acao
  SET acacod = '{$acaoDTO->codigoAcao}',
      esfcod = '{$acaoDTO->codigoEsfera}',
      procod = '{$acaoDTO->codigoPrograma}',
      orgcod = '{$acaoDTO->codigoOrgao}',
      acabaselegal = '{$acaoDTO->baseLegal}',
      acacodigofuncao = '{$acaoDTO->codigoFuncao}',
      acacodigoiniciativa = '{$acaoDTO->codigoIniciativa}',
      acacodigomomento = '{$acaoDTO->codigoMomento}',
      acacodigoobjetivo = '{$acaoDTO->codigoObjetivo}',
      acacodigoproduto = '{$acaoDTO->codigoProduto}',
      acacodigosubfuncao = '{$acaoDTO->codigoSubFuncao}',
      acacodigotipoacao = '{$acaoDTO->codigoTipoAcao}',
      acacodigotipoinclusaoacao = '{$acaoDTO->codigoTipoInclusaoAcao}',
      acacodigounidademedida = '{$acaoDTO->codigoUnidadeMedida}',
      acadescricao = '{$acaoDTO->descricao}',
      acadetalhamentoimplementacao = '{$acaoDTO->detalhamentoImplementacao}',
      acaespecificacaoproduto = '{$acaoDTO->especificacaoProduto}',
      acafinalidade = '{$acaoDTO->finalidade}',
      acaformaacompanhamento = '{$acaoDTO->formaAcompanhamento}',
      acaidentificacaosazonalidade = '{$acaoDTO->identificacaoSazonalidade}',
      acainsumosutilizados = '{$acaoDTO->insumosUtilizados}',
      acasndescentralizada = '{$acaoDTO->snDescentralizada}',
      acasndireta = '{$acaoDTO->snDireta}',
      acaativo = '{$acaoDTO->snExclusaoLogica}',
      acasnlinhacredito = '{$acaoDTO->snLinhaCredito}',
      acasntransferenciaobrigatoria = '{$acaoDTO->snTransferenciaObrigatoria}',
      acasntransferenciavoluntaria = '{$acaoDTO->snTransferenciaVoluntaria}',
      acatitulo = '{$acaoDTO->titulo}',
      acaunidaderesponsavel = '{$acaoDTO->unidadeResponsavel}',
      acaexercicio = '{$acaoDTO->exercicio}',
      dtalteracao = NOW()::date,
      snacompanhamentoopcional = '{$snAcompanhamentoOpcional}'
  WHERE acaidentificadorunicosiop = '{$acaoDTO->identificadorUnico}'
DML;
      } else {
        $sql = <<<DML
INSERT INTO planacomorc_old.acao(acacod, acaidentificadorunicosiop, esfcod, procod,
                             orgcod, acabaselegal, acacodigofuncao, acacodigoiniciativa,
                             acacodigomomento, acacodigoobjetivo, acacodigoproduto, acacodigosubfuncao,
                             acacodigotipoacao, acacodigotipoinclusaoacao, acacodigounidademedida, acadescricao,
                             acadetalhamentoimplementacao, acaespecificacaoproduto, acafinalidade, acaformaacompanhamento,
                             acaidentificacaosazonalidade, acainsumosutilizados, acasndescentralizada, acasndireta,
                             acaativo, acasnlinhacredito, acasntransferenciaobrigatoria, acasntransferenciavoluntaria,
                             acatitulo, acaunidaderesponsavel, acaexercicio, dtcriacao, snacompanhamentoopcional)
  VALUES('{$acaoDTO->codigoAcao}', '{$acaoDTO->identificadorUnico}', '{$acaoDTO->codigoEsfera}', '{$acaoDTO->codigoPrograma}',
         '{$acaoDTO->codigoOrgao}', '{$acaoDTO->baseLegal}', '{$acaoDTO->codigoFuncao}', '{$acaoDTO->codigoIniciativa}',
         '{$acaoDTO->codigoMomento}', '{$acaoDTO->codigoObjetivo}', '{$acaoDTO->codigoProduto}', '{$acaoDTO->codigoSubFuncao}',
         '{$acaoDTO->codigoTipoAcao}', '{$acaoDTO->codigoTipoInclusaoAcao}', '{$acaoDTO->codigoUnidadeMedida}', '{$acaoDTO->descricao}',
         '{$acaoDTO->detalhamentoImplementacao}', '{$acaoDTO->especificacaoProduto}', '{$acaoDTO->finalidade}', '{$acaoDTO->formaAcompanhamento}',
         '{$acaoDTO->identificacaoSazonalidade}', '{$acaoDTO->insumosUtilizados}', '{$acaoDTO->snDescentralizada}', '{$acaoDTO->snDireta}',
         '{$acaoDTO->snExclusaoLogica}', '{$acaoDTO->snLinhaCredito}', '{$acaoDTO->snTransferenciaObrigatoria}', '{$acaoDTO->snTransferenciaVoluntaria}',
         '{$acaoDTO->titulo}', '{$acaoDTO->unidadeResponsavel}', '{$acaoDTO->exercicio}', NOW()::date, '{$snAcompanhamentoOpcional}')
DML;
      }
      $db->executar($sql);
    }
    $db->commit();
    return count($arrAcoes);
  }
  return 0;
}

/**
 * Insere novos localizadores na base e atualiza os existentes.
 *
 * @global cls_banco $db Conexão com o banco.
 * @param array $arrDados Lista de localizadorDTO retornado pelo webservice.
 * @return null|boolean
 * @see cls_banco
 * @see localizadorDTO
 */
function insereLocalizadoresDTO($arrDados) {
  global $db;

  if (is_array($arrDados)) {
    $sql = <<<QUERY
SELECT l.acaidentificadorunicosiop,
       l.loccod
  FROM planacomorc_old.localizador l
QUERY;
    $arrLocalizadores = $db->carregar($sql);
    if (!is_array($arrLocalizadores)) {
      $arrLocalizadores = array();
    }

    foreach ($arrDados as $localizadorDTO) {
      if (is_null($localizadorDTO->anoInicio)) { $localizadorDTO->anoInicio = 'null'; }
      if (is_null($localizadorDTO->anoTermino)) { $localizadorDTO->anoTermino = 'null'; }
      if (is_null($localizadorDTO->exercicio)) {$localizadorDTO->exercicio = 'null'; }
      if (is_null($localizadorDTO->totalFinanceiro)) { $localizadorDTO->totalFinanceiro = 'null'; }
      if (is_null($localizadorDTO->totalFisico)) { $localizadorDTO->totalFisico = 'null'; }
      if (is_null($localizadorDTO->codigoMomento)) { $localizadorDTO->codigoMomento = 'null'; }
      if (is_null($localizadorDTO->identificadorunicosiop)) { $localizadorDTO->identificadorunicosiop = 'null'; }
      $localizadorDTO->descricao = str_replace("'", "''", $localizadorDTO->descricao);
      $localizadorDTO->municipio = str_replace("'", "''", $localizadorDTO->municipio);

      // -- Verificando se o orgao já está cadastrado no sistema
      if (in_array(array(
          'acaidentificadorunicosiop' => $localizadorDTO->identificadorUnicoAcao,
          'loccod' => $localizadorDTO->codigoLocalizador), $arrLocalizadores)) {
        $sql = <<<DML
UPDATE planacomorc_old.localizador
  SET acaidentificadorunicosiop = '{$localizadorDTO->identificadorUnicoAcao}',
      loccod = '{$localizadorDTO->codigoLocalizador}',
      locanoinicio = {$localizadorDTO->anoInicio},
      locanotermino = {$localizadorDTO->anoTermino},
      loccodigotipoinclusao = '{$localizadorDTO->codigoTipoInclusao}',
      locdescricao = '{$localizadorDTO->descricao}',
      locexercicio = {$localizadorDTO->exercicio},
      locmunicipio = '{$localizadorDTO->municipio}',
      locregiao = '{$localizadorDTO->regiao}',
      locsiglaregiao = '{$localizadorDTO->siglaRegiao}',
      locativo = '{$localizadorDTO->snExclusaoLogica}',
      loctotalfinanceiro = {$localizadorDTO->totalFinanceiro},
      loctotalfisico = {$localizadorDTO->totalFisico},
      locuf = '{$localizadorDTO->uf}',
      loccodigomomento = {$localizadorDTO->codigoMomento},
      identificadorunicosiop = {$localizadorDTO->identificadorunicosiop}
  WHERE acaidentificadorunicosiop = {$localizadorDTO->identificadorUnicoAcao}
    AND loccod = '{$localizadorDTO->codigoLocalizador}'
DML;
      } else {
        $sql = <<<DML
INSERT INTO planacomorc_old.localizador(acaidentificadorunicosiop, loccod, locanoinicio, locanotermino,
                                    loccodigotipoinclusao, locdescricao, locexercicio, locmunicipio,
                                    locregiao, locsiglaregiao, locativo, loctotalfinanceiro,
                                    loctotalfisico, locuf, loccodigomomento, dtcriacao, identificadorunicosiop)
  VALUES($localizadorDTO->identificadorUnicoAcao, '{$localizadorDTO->codigoLocalizador}', $localizadorDTO->anoInicio, $localizadorDTO->anoTermino,
         '{$localizadorDTO->codigoTipoInclusao}', '{$localizadorDTO->descricao}', $localizadorDTO->exercicio, '{$localizadorDTO->municipio}',
         '{$localizadorDTO->regiao}', '{$localizadorDTO->siglaRegiao}', '{$localizadorDTO->snExclusaoLogica}', $localizadorDTO->totalFinanceiro,
         $localizadorDTO->totalFisico, '{$localizadorDTO->uf}', {$localizadorDTO->codigoMomento}, NOW()::date, {$localizadorDTO->identificadorunicosiop})
DML;
      }
      $db->executar($sql);
    }
    return $db->commit();
  }
  return 0;
}

/**
 * Insere novos órgãos na base e atualiza os existentes.
 *
 * @global cls_banco $db Conexão com o banco.
 * @param array $arrDados Lista de orgaoDTO retornado pelo webservice.
 * @return null|boolean
 * @see cls_banco
 * @see orgaoDTO
 */
function insereOrgaosDTO($arrDados) {
  global $db;
  if (is_array($arrDados)) {
    // -- Lista de órgãos já cadastrados
    $sql = <<<QUERY
SELECT o.orgcod, o.orgtipoorgao
  FROM planacomorc_old.orgao o
QUERY;
    $arrOrgaos = $db->carregar($sql);
    if (!is_array($arrOrgaos)) {
      $arrOrgaos = array();
    }
    foreach ($arrDados as $orgaoDTO) {
      // -- Verificando se o orgao já está cadastrado no sistema
      if (in_array(array(
          'orgcod' => $orgaoDTO->codigoOrgao,
          'orgtipoorgao' => $orgaoDTO->tipoOrgao), $arrOrgaos)) {
        $sql = <<<DML
UPDATE planacomorc_old.orgao
  SET orgdescricaoresumida = '{$orgaoDTO->descricaoAbreviada}',
      orgdescricao = '{$orgaoDTO->descricao}'
  WHERE orgcod = '{$orgaoDTO->codigoOrgao}'
    AND orgtipoorgao = '{$orgaoDTO->tipoOrgao}'
DML;
      } else {
        $sql = <<<DML
INSERT INTO planacomorc_old.orgao(orgcod, orgdescricaoresumida, orgtipoorgao, orgdescricao)
  VALUES('{$orgaoDTO->codigoOrgao}', '{$orgaoDTO->descricaoAbreviada}', '{$orgaoDTO->tipoOrgao}', '{$orgaoDTO->descricao}')
DML;
      }
      $db->executar($sql);
    }
    $db->commit();
    return count($arrDados);
  }
  return 0;
}

/**
 * Insere novos programas na base e atualiza os existentes.
 *
 * @global cls_banco $db Conexão com o banco.
 * @param array $arrDados Lista de programasDTO retornado pelo webservice.
 * @return null|boolean
 * @see cls_banco
 * @see programaDTO
 */
function insereProgramasDTO($arrDados) {
  global $db;
  if (is_array($arrDados)) {
    // -- Lista de programas já cadastrados
    $sql = <<<QUERY
SELECT p.procod
  FROM planacomorc_old.programa p
QUERY;

    $arrProgramas = $db->carregarColuna($sql);
    if (!is_array($arrProgramas)) {
      $arrProgramas = array();
    }

    // -- Processando a lista de programas retornada pelo webservice
    foreach ($arrDados as $programaDTO) {
      if (in_array($programaDTO->codigoPrograma, $arrProgramas)) {
        $sql = <<<DML
UPDATE planacomorc_old.programa
  SET procodigomacrodesafio = '{$programaDTO->codigoMacroDesafio}',
      procodigomomento = '{$programaDTO->codigoMomento}',
      procodigoorgao = '{$programaDTO->codigoOrgao}',
      procodigotipoprograma = '{$programaDTO->codigoTipoPrograma}',
      proestrategiaimplementacao = '{$programaDTO->estrategiaImplementacao}',
      proexercicio = '{$programaDTO->exercicio}',
      prohorizontetemporalcontinuo = '{$programaDTO->horizonteTemporalContinuo}',
      proidentificadorunicosiop = '{$programaDTO->identificadorUnico}',
      projustificativa = '{$programaDTO->justificativa}',
      proobjetivo = '{$programaDTO->objetivo}',
      proobjetivogoverno = '{$programaDTO->objetivoGoverno}',
      proobjetivosetorial = '{$programaDTO->objetivoSetorial}',
      proproblema = '{$programaDTO->problema}',
      propublicoalvo = '{$programaDTO->publicoAlvo}',
      proativo = '{$programaDTO->snExclusaoLogica}',
      prounidaderesponsavel = '{$programaDTO->unidadeResponsavel}',
      protitulo = '{$programaDTO->titulo}'
  WHERE procod = '{$programaDTO->codigoPrograma}'
DML;
      } else {
        $sql = <<<DML
INSERT INTO planacomorc_old.programa(procod, procodigomacrodesafio, procodigomomento, procodigoorgao,
                                 procodigotipoprograma, proestrategiaimplementacao, proexercicio, prohorizontetemporalcontinuo,
                                 proidentificadorunicosiop, projustificativa, proobjetivo, proobjetivogoverno,
                                 proobjetivosetorial, proproblema, propublicoalvo, proativo,
                                 prounidaderesponsavel, protitulo)
  VALUES('{$programaDTO->codigoPrograma}', '{$programaDTO->codigoMacroDesafio}', '{$programaDTO->codigoMomento}', '{$programaDTO->codigoOrgao}',
         '{$programaDTO->codigoTipoPrograma}', '{$programaDTO->estrategiaImplementacao}', '{$programaDTO->exercicio}', '{$programaDTO->horizonteTemporalContinuo}',
         '{$programaDTO->identificadorUnico}', '{$programaDTO->justificativa}', '{$programaDTO->objetivo}', '{$programaDTO->objetivoGoverno}',
         '{$programaDTO->objetivoSetorial}', '{$programaDTO->problema}', '{$programaDTO->publicoAlvo}', '{$programaDTO->snExclusaoLogica}',
         '{$programaDTO->unidadeResponsavel}', '{$programaDTO->titulo}')
DML;
      }
      $db->executar($sql);
    }
    return $db->commit();
  }
}
