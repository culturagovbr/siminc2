<?php
/**
 * PLANACOMORC
 * base: Programação Orçamentária
 */

ini_set("memory_limit", "2048M");
set_time_limit(30000);

if($_POST['requisicao'] == 'enviasolicitacao') {
    $arrParam = array(
        'usuario' => $_POST['wsusuario'],
        'senha' => $_POST['wssenha']);

    if ('cargaOrcamentaria' != $_POST['documento']) {
        $arrParam['post'] = $_POST;
        include_once APPRAIZ . 'planacomorc/classes/WSQualitativo.class.inc';
        include_once APPRAIZ . 'planacomorc/classes/SiopQualitativo.class.inc';
        include_once APPRAIZ . 'planacomorc/modulos/sistema/comunica/funcoesImportacao.inc';
        $oSiop = new SiopQualitativo($arrParam);
    }
    $documento = $_POST['documento'];

  // -- Indica se houve sucesso na requisição ao webservice
  $wsSucesso = false;
  $numProcessados = array();
  $wsErroMessage = null;

  if ($documento == 'obterProgramacaoCompletaQualitativo') {
    $arrResponse = $oSiop->obterProgramacaoCompleta();
    if ($arrResponse->return->sucesso) {
      $wsSucesso = true;

      $numProcessados['Ações'] = insereAcoesDTO($arrResponse->return->acoesDTO);
      $numProcessados['Órgãos'] = insereOrgaosDTO($arrResponse->return->orgaosDTO);
      $numProcessados['Programas'] = insereProgramasDTO($arrResponse->return->programasDTO);
      $numProcessados['Localizadores'] = insereLocalizadoresDTO($arrResponse->return->localizadoresDTO);

    } else {
      $wsErroMessage = $arrResponse->return->mensagensErro;
    }
  } elseif ($documento == 'obterTabelasApoio') {
    $retornoApoioQualitativoDTO = $oSiop->obterTabelasApoio();

    if ($retornoApoioQualitativoDTO->return->sucesso) {
      $wsSucesso = true;

      $numProcessados['Bases Geográficas'] = insereBasesGeograficasDTO(
              $retornoApoioQualitativoDTO->return->basesGeograficasDTO);
      $numProcessados['Esferas'] = insereEsferasDTO(
              $retornoApoioQualitativoDTO->return->esferasDTO);
      $numProcessados['Funções'] = insereFuncoesDTO(
              $retornoApoioQualitativoDTO->return->funcoesDTO);
      $numProcessados['Macro Desafios'] = insereMacroDesafiosDTO(
              $retornoApoioQualitativoDTO->return->macroDesafiosDTO);
      $numProcessados['Momentos'] = insereMomentosDTO(
              $retornoApoioQualitativoDTO->return->momentosDTO);
      $numProcessados['Perfis'] = inserePerfisDTO(
              $retornoApoioQualitativoDTO->return->perfisDTO);
      $numProcessados['Periodicidades'] = inserePeriodicidadesDTO(
              $retornoApoioQualitativoDTO->return->periodicidadesDTO);
      $numProcessados['Produtos'] = insereProdutosDTO(
              $retornoApoioQualitativoDTO->return->produtosDTO);
      $numProcessados['Regiões'] = insereRegioesDTO(
              $retornoApoioQualitativoDTO->return->regioesDTO);
      $numProcessados['Subfunções'] = insereSubFuncoesDTO(
              $retornoApoioQualitativoDTO->return->subFuncoesDTO);
      $numProcessados['Tipos de Ação'] = insereTiposAcaoDTO(
              $retornoApoioQualitativoDTO->return->tiposAcaoDTO);
      $numProcessados['Tipos de Inclusão'] = insereTiposInclusaoDTO(
              $retornoApoioQualitativoDTO->return->tiposInclusaoDTO);
      $numProcessados['Tipos de Programa'] = insereTiposProgramaDTO(
              $retornoApoioQualitativoDTO->return->tiposProgramaDTO);
      $numProcessados['Unidades de Medida'] = insereUnidadesMedidaDTO(
              $retornoApoioQualitativoDTO->return->unidadesMedidaDTO);
      $numProcessados['Indicador de Unidades de Medida'] = insereUnidadesMedidaIndicadorDTO(
              $retornoApoioQualitativoDTO->return->unidadesMedidaIndicadorDTO);
    } else {
      $wsErroMessage = $retornoApoioQualitativoDTO->return->mensagensErro;
    }
  } elseif ('cargaOrcamentaria' == $documento) {
      include_once APPRAIZ . 'planacomorc/classes/CargaOrcamentaria.class.inc';
      $cargaOrcamentaria = new CargaOrcamentaria($arrParam);
      $cargaOrcamentaria->obterTabelasDeApoio();
  } else {
    $arDocumentos = array(
        'obterAcoes' => array('desc' => 'Ações', 'func' => 'insereAcoesDTO2'),
        'obterOrgao' => array('desc' => 'Órgãos', 'func' => 'insereOrgaosDTO2'),
        'obterProgramas' => array('desc' => 'Programas', 'func' => 'insereProgramasDTO2'),
        'obterObjetivos' => array('desc' => 'Objetivos', 'func' => 'insereObjetivosDTO'),
        'obterMetas' => array('desc' => 'Metas', 'func' => 'insereMetasDTO'),
        'obterIniciativas' => array('desc' => 'Iniciativas', 'func' => 'insereIniciativasDTO'),
        'obterLocalizadores' => array('desc' => 'Localizadores', 'func' => 'insereLocalizadoresDTO2'),
        'obterRegionalizacoes' => array('desc' => 'Regionalizações', 'func' => 'insereRegionalizacoesDTO'),
        'obterUnidadesOrcamentarias' => array('desc' => 'Unidades Orçamentárias', 'func' => 'insereUnidadesOrcamentariasDTO'),
        'obterIndicadores' => array('desc' => 'Indicadores', 'func' => 'insereIndicadoresDTO'),
        'obterMomentos' => array('desc' => 'Momentos', 'func' => 'insereMomentosDTO'),
        'obterPlanosOrcamentarios' => array('desc' => 'Planos Orçamentários', 'func' => 'inserePlanosOrcamentariosDTO'));
    // -- Processando o tipo de documento enviado pelo formulário
    if (in_array($documento, array_keys($arDocumentos))) {
      $retornoDTO = $oSiop->$documento(); //ver($retornoDTO, $_POST, d);
      if ($retornoDTO->return->sucesso) {
        $wsSucesso = true;
        // -- Tratamento para momentoDTO em obterMomentoCarga
        if (isset($retornoDTO->return->momento)) {
          $numProcessados[$arDocumentos[$documento]['desc']]
                  = $arDocumentos[$documento]['func']($retornoDTO->return->momento); 
        } else {
          $numProcessados[$arDocumentos[$documento]['desc']]
                  = $arDocumentos[$documento]['func']($retornoDTO->return->registros); 
        }
      } else {
        $wsErroMessage = $retornoDTO->return->mensagensErro;
      }
    } else {
      trigger_error("Documento '{$documento}' não está implementado.", E_USER_ERROR);
    }
  }
  if ($wsSucesso) {
    // -- número de registros processados por tipo
    $msg = '<script>alert("Dados atualizados com sucesso:';
    foreach ($numProcessados as $nome => $qtd) {
      $msg .= '\n' . "* {$nome}: {$qtd}" . '';
    }
    $msg .= '"); window.location.href = "planacomorc.php?modulo=sistema/comunica/importaQualitativo2&acao=A";</script>';
  } else {
    $msg = '<script>alert("' . $wsErroMessage . '!");'
            . 'window.location.href = "planacomorc.php?modulo=sistema/comunica/importaQualitativo2&acao=A";'
            . '</script>';
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
            <input type="radio" name="documento" id="documento_obterTabelasApoio"
                   value="obterTabelasApoio" onclick="mostraCombo(this.value);"
                   class="chamadaWs" /><label for="documento_obterTabelasApoio">Tabelas Apoio (Busca dados das tabelas de apoio):</label>
            <div id="obterTabelasApoio" style="display: none;">
              <div class="msg-info" style="width:500px">
                  Observações:<br />
                  <ul><li>Estas importações fazem a carga das Tabelas de Apoio Qualitativas para o esquema <b><u>WSSOF</u></b>.</li>
                      <li>Esta carga é feita <b><u>APAGANDO</u></b> os registros anteriores antes de fazer a nova carga.</li>
                  </ul>
              </div>
              <table align="center" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
                  <?php
                  $arTabelasApoio = array(
                      'retornarBasesGeograficas' => 'Retornar Bases Geograficas',
                      'retornarEsferas' => 'Retornar Esferas', //
                      'retonarFuncoes' => 'Retonar Funções', //
                      'retornarProdutos' => 'Retornar Produtos', //
                      'retornarSubFuncoes' => 'Retornar SubFunções', //
                      'retornarUnidadesMedida' => 'Retornar Unidades Medida', //
                      'retornarMacroDesafios' => 'Retornar Macro Desafios',
                      'retornarMomentos' => 'Retornar Momentos',
                      'retornarPerfis' => 'Retornar Perfis',
                      'retornarPeriodicidades' => 'Retornar Periodicidades',
                      'retornarRegioes' => 'Retornar Regiões',
                      'retornarTiposAcao' => 'Retornar Tipos Ação', //
                      'retornarTiposInclusao' => 'Retornar Tipos Inclusão', //
                      'retornarTiposPrograma' => 'Retornar Tipos Programa',
                      'retornarUnidadesMedidaIndicador' => 'Retornar Unidades Medida Indicador');
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
<!--          <tr><td>
            <input type="radio" name="documento" id="documento_obterMomentos" value="obterMomentos"
                   onClick="mostraCombo(this.value);" class="chamadaWs" />
            <label for="documento_obterMomentos">Momentos</label>
            <div id="obterMomentos" style="display:none"></div>
          </td></tr>-->
<!--          <tr><td>
            <input type="radio" name="documento" id="documento_obterOrgao" value="obterOrgao"
                   onClick="mostraCombo(this.value);" class="chamadaWs" />
            <label for="documento_obterOrgao">Órgão</label>
            <div id="obterOrgao" style="display:none">
              <table align="center" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
                <tr>
                  <td class="subtitulodireita" width="30%">Por: </td>
                  <td width="70%">
                    <span>
                      <input type="radio" name="buscarOrgaoPor" value="codtipo" id="opcBuscaOrgaoTipo" onClick="mostraFiltros(this.id);" />
                      <label for="opcBuscaOrgaoTipo">Código / Tipo</label>
                    </span>
                    <span>
                      <input type="radio" name="buscarOrgaoPor" value="siorg" id="opcBuscaOrgaoSiorg" onClick="mostraFiltros(this.id);" />
                      <label for="opcBuscaOrgaoSiorg">Código Siorg</label>
                    </span>
                  </td>
                </tr>
              </table>
            </div>
          </td></tr>-->
<!--          <tr><td>
            <input type="radio" name="documento" id="documento_obterProgramas" value="obterProgramas"
                   onClick="mostraCombo(this.value);"
                   class="chamadaWs" />
            <label for="documento_obterProgramas">Programas</label>
            <div id="obterProgramas" style="display:none">
              <table align="center" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
                <tr>
                  <td class="subtitulodireita" width="30%">Por: </td>
                  <td width="70%">
                    <span>
                      <input type="radio" name="buscarAcaoPor" value="programa" id="opcBuscaProgOrgao" onClick="mostraFiltros(this.id);" />
                      <label for="opcBuscaProgOrgao">Órgão</label>
                    </span>
                  </td>
                </tr>
              </table>
            </div>
          </td></tr>-->
<!--          <tr><td>
            <input type="radio" name="documento" id="documento_obterUnidadesOrcamentarias" value="obterUnidadesOrcamentarias"
                   onClick="mostraCombo(this.value);"
                   class="chamadaWs" />
            <label for="documento_obterUnidadesOrcamentarias">Unidades Orçamentárias</label>
            <div id="obterUnidadesOrcamentarias" style="display:none">
              <table align="center" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
                <tr>
                  <td class="subtitulodireita" width="30%">Por: </td>
                  <td width="70%">
                    <span>
                      <input type="radio" name="buscarUoPorOrgao" value="programa" id="opcBuscaUoPorOrgao" onClick="mostraFiltros(this.id);" />
                      <label for="opcBuscaUoPorOrgao">Órgão</label>
                    </span>
                  </td>
                </tr>
              </table>
            </div>
          </td></tr>-->
<!--          <tr><td>
            <input type="radio" name="documento" id="documento_obterObjetivos" value="obterObjetivos"
                   onClick="mostraCombo(this.value);" class="chamadaWs" />
            <label for="documento_obterObjetivos">Objetivos</label>
            <div id="obterObjetivos" style="display:none">
              <table align="center" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
                <tr>
                  <td class="subtitulodireita" width="30%">Por: </td>
                  <td width="70%">
                    <span>
                      <input type="radio" name="buscarObjetivosPorProg" value="programa" id="opcBuscaObjProg" onClick="mostraFiltros(this.id);" />
                      <label for="opcBuscaObjProg">Programa</label>
                    </span>
                  </td>
                </tr>
              </table>
            </div>
          </td></tr>-->
<!--          <tr><td>
            <input type="radio" name="documento" id="documento_obterIndicadores" value="obterIndicadores"
                   onClick="mostraCombo(this.value);" class="chamadaWs" />
            <label for="documento_obterIndicadores">Indicadores</label>
            <div id="obterIndicadores" style="display:none">
              <table align="center" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
                <tr>
                  <td class="subtitulodireita" width="30%">Por: </td>
                  <td width="70%">
                    <span>
                      <input type="radio" name="buscarIndicadoresPorProg" value="programa" id="opcBuscaIndProg" onClick="mostraFiltros(this.id);" />
                      <label for="opcBuscaIndProg">Programa</label>
                    </span>
                  </td>
                </tr>
              </table>
            </div>
          </td></tr>-->
<!--          <tr><td>
            <input type="radio" name="documento" id="documento_obterMetas" value="obterMetas"
                   onClick="mostraCombo(this.value);" class="chamadaWs" />
            <label for="documento_obterMetas">Metas</label>
            <div id="obterMetas" style="display:none">
              <table align="center" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
                <tr>
                  <td class="subtitulodireita" width="30%">Por: </td>
                  <td width="70%">
                    <span>
                      <input type="radio" name="buscarMetasPorObjetivo" value="programa" id="opcBuscaMetObjetivo" onClick="mostraFiltros(this.id);" />
                      <label for="opcBuscaMetObjetivo">Objetivo</label>
                    </span>
                  </td>
                </tr>
              </table>
            </div>
          </td></tr>-->
<!--          <tr><td>
            <input type="radio" name="documento" id="documento_obterIniciativas" value="obterIniciativas"
                   onClick="mostraCombo(this.value);" class="chamadaWs" />
            <label for="documento_obterIniciativas">Iniciativas</label>
            <div id="obterIniciativas" style="display:none">
              <table align="center" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
                <tr>
                  <td class="subtitulodireita" width="30%">Por: </td>
                  <td width="70%">
                    <span>
                      <input type="radio" name="buscarIniciativasPorObjetivo" value="programa" id="opcBuscaInicObjetivo" onClick="mostraFiltros(this.id);" />
                      <label for="opcBuscaInicObjetivo">Objetivo</label>
                    </span>
                  </td>
                </tr>
              </table>
            </div>
          </td></tr>-->
<!--          <tr><td>
            <input type="radio" name="documento" id="documento_obterRegionalizacoes" value="obterRegionalizacoes"
                   onClick="mostraCombo(this.value);" class="chamadaWs" />
            <label for="documento_obterRegionalizacoes">Regionalizações</label>
            <div id="obterRegionalizacoes" style="display:none">
              <table align="center" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
                <tr>
                  <td class="subtitulodireita" width="30%">Por: </td>
                  <td width="70%">
                    <span>
                      <input type="radio" name="buscarRegionalizacoesPorMetas" value="programa" id="opcBuscaRegMetas" onClick="mostraFiltros(this.id);" />
                      <label for="opcBuscaRegMetas">Metas</label>
                    </span>
                  </td>
                </tr>
              </table>
            </div>
          </td></tr>          -->
<!--          <tr><td>
            <input type="radio" name="documento" id="documento_obterAcoes" value="obterAcoes"
                   onClick="mostraCombo(this.value);"
                   class="chamadaWs" />
            <label for="documento_obterAcoes">Ações</label>
            <div id="obterAcoes" style="display:none">
              <table align="center" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
                <tr>
                  <td class="subtitulodireita" width="30%">Por: </td>
                  <td width="70%">
                    <span>
                      <input type="radio" name="buscarAcaoPor" value="programa" id="opcBuscaProg" onClick="mostraFiltros(this.id);" />
                      <label for="opcBuscaProg">Programa</label>
                    </span>
                    <span>
                      <input type="radio" name="buscarAcaoPor" value="iniciativa" id="opcBuscaInic" onClick="mostraFiltros(this.id);" />
                      <label for="opcBuscaInic">Iniciativa</label>
                    </span>
                    <span>
                      <input type="radio" name="buscarAcaoPor" value="identunico" id="opcBuscaIdenUnic" onClick="mostraFiltros(this.id);" />
                      <label for="opcBuscaIdenUnic">Identificador Único</label>
                    </span>
                  </td>
                </tr>
              </table>
            </div>
          </td></tr>-->
<!--          <tr><td>
            <input type="radio" name="documento" id="documento_obterLocalizadores" value="obterLocalizadores"
                   onClick="mostraCombo(this.value);" class="chamadaWs" />
            <label for="documento_obterLocalizadores">Localizadores</label>
            <div id="obterLocalizadores" style="display:none">
              <table align="center" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
                <tr>
                  <td class="subtitulodireita" width="30%">Por: </td>
                  <td width="70%">
                    <span>
                      <input type="radio" name="buscarLocalizadoresPorAcao" value="programa" id="opcBuscaLocAcao" onClick="mostraFiltros(this.id);" />
                      <label for="opcBuscaLocAcao">Identificador único da Ação</label>
                    </span>
                  </td>
                </tr>
              </table>
            </div>
          </td></tr>-->
<!--          <tr><td>
            <input type="radio" name="documento" id="documento_obterPlanosOrcamentarios" value="obterPlanosOrcamentarios"
                   onClick="mostraCombo(this.value);" class="chamadaWs" />
            <label for="documento_obterPlanosOrcamentarios">Planos Orçamentários</label>
            <div id="obterPlanosOrcamentarios" style="display:none">
              <table align="center" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
                <tr>
                  <td class="subtitulodireita" width="30%">Por: </td>
                  <td width="70%">
                    <span>
                      <input type="radio" name="buscarPlanosOrcamentariosPorAcao" value="programa" id="opcBuscaPlanOrcAcao" onClick="mostraFiltros(this.id);" />
                      <label for="opcBuscaPlanOrcAcao">Identificador único da Ação</label>
                    </span>
                  </td>
                </tr>
              </table>
            </div>
          </td></tr>-->
        </table>
      </td>
    </tr>
    <tr>
      <td class="subtitulodireita"><b>Código do Momento:</b></td>
      <td><? echo campo_texto("codigomomento", "S", "S", "codigomomento", "22", "", "[#]", "", "", "", "", "id='codigomomento'", '', '2000'); ?></td>
    </tr>
    <tr style="display:none" class="filtroWs opcBuscaOrgaoTipo">
      <td class="subtitulodireita" width="30%"><label for="filtroCodigoOrgaoValor">Código Órgão: </label></td>
      <td width="70%"><input type="text" name="filtro[codigoOrgao]" id="filtroCodigoOrgaoValor" value="" /></td>
    </tr>
    <tr style="display:none" class="filtroWs opcBuscaOrgaoTipo">
      <td class="subtitulodireita" width="30%"><label for="filtroTipoOrgaoValor">Tipo Órgão: </label></td>
      <td width="70%">
        <select name="filtro[tipoOrgao]" id="filtroTipoOrgaoValor">
          <option></option>
          <option value="U">Unidade Orçamentária</option>
          <option value="O">Órgão Setorial</option>
          <option value="S">Secretaria</option>
        </select>
      </td>
    </tr>
    <tr style="display:none" class="filtroWs opcBuscaOrgaoSiorg">
      <td class="subtitulodireita" width="30%"><label for="filtroCodigoSiorgValor">Código Siorg: </label></td>
      <td width="70%"><input type="text" name="filtro[codigoSiorg]" id="filtroCodigoSiorgValor" value="" /></td>
    </tr>
    <tr style="display:none" class="filtroWs opcBuscaProgOrgao opcBuscaUoPorOrgao">
      <td class="subtitulodireita" width="30%"><label for="filtroOrgaoValor">Órgão: </label></td>
      <td width="70%"><input type="text" name="filtro[codigoOrgao]" id="filtroOrgaoValor" value="" /></td>
    </tr>
    <tr style="display:none" class="filtroWs opcBuscaProg opcBuscaInic opcBuscaObjProg opcBuscaMetObjetivo opcBuscaInicObjetivo opcBuscaRegMetas opcBuscaIndProg">
      <td class="subtitulodireita" width="30%"><label for="filtroProgramaValor">Programa: </label></td>
      <td width="70%"><input type="text" name="filtro[codigoPrograma]" id="filtroProgramaValor" value="" /></td>
    </tr>
    <tr style="display:none" class="filtroWs opcBuscaInic opcBuscaMetObjetivo opcBuscaInicObjetivo opcBuscaRegMetas">
      <td class="subtitulodireita" width="30%"><label for="filtroObjetivoValor">Objetivo: </label></td>
      <td width="70%"><input type="text" name="filtro[codigoObjetivo]" id="filtroObjetivoValor" value="" /></td>
    </tr>
    <tr style="display:none" class="filtroWs opcBuscaInic">
      <td class="subtitulodireita" width="30%"><label for="filtroIniciativaValor">Iniciativa: </label></td>
      <td width="70%"><input type="text" name="filtro[codigoIniciativa]" id="filtroIniciativaValor" value="" /></td>
    </tr>
    <tr style="display:none" class="filtroWs opcBuscaIdenUnic">
      <td class="subtitulodireita" width="30%"><label for="filtroIdentUnicoValor">Identificador Único: </label></td>
      <td width="70%"><input type="text" name="filtro[identificadorUnico]" id="filtroIdentUnicoValor" value="" /></td>
    </tr>
    <tr style="display:none" class="filtroWs opcBuscaLocAcao opcBuscaPlanOrcAcao">
      <td class="subtitulodireita" width="30%"><label for="filtroAcaoValor">Ação: </label></td>
      <td><?php echo campo_texto("filtro[identificadorUnicoAcao]", "S", "S", "Ação", "22", "", "", "", "", "", "", "id='filtroAcaoValor'", '', ''); ?></td>
    </tr>
    <tr style="display:none" class="filtroWs opcBuscaRegMetas">
      <td class="subtitulodireita" width="30%"><label for="filtroMetaValor">Meta: </label></td>
      <td><?php echo campo_texto("filtro[codigoMeta]", "N", "S", "Meta", "22", "", "", "", "", "", "", "id='filtroMetaValor'", '', ''); ?></td>
    </tr>
    <tr>
      <td colspan="2" align="center" style="background-color:#c0c0c0;"><input type="button" class="botao" value="Enviar Solicitação" onclick="solicitarExecucao();"></td>
    </tr>
  </table>
  <br>
  <div id="erro"><?= $html; ?></div>

  <input type="hidden" value="" name="requisicao" id="requisicao">
  <?php require(dirname(__FILE__) . '/popup.inc'); ?>
</form>
<script type="text/javascript">
/**
 * Validação adicional para localizadores e planos orçamentários.
 */
function validacaoAdicional() {
  if (($('#documento_obterLocalizadores').attr('checked') || $('#documento_obterPlanosOrcamentarios').attr('checked'))
          && (!$('#filtroAcaoValor').attr('value'))) {
    alert('É obrigatório informar uma ação.');
    return false;
  }
  return true;
}
</script>