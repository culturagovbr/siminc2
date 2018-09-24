<?php
$_SESSION['sislayoutbootstrap'] = false;

/**
 * PLANACOMORC
 * base: Programação Orçamentária
 */
ini_set("memory_limit", "2048M");
set_time_limit(50000);

if ($_GET['clear']) { unset($_SESSION['planacomorc']); }


if ($_POST['requisicao'] == 'enviasolicitacao') {
  include_once APPRAIZ . 'planacomorc/classes/WSQuantitativo.class.inc';
  include_once APPRAIZ . 'planacomorc/classes/SiopQuantitativo.class.inc';
  include_once APPRAIZ . 'planacomorc/modulos/sistema/comunica/funcoesImportacao.inc';

  $arrParam = array('usuario' => $_POST['wsusuario'],
      'senha' => $_POST['wssenha'],
      'post' => $_POST);

  $oSiop = new SiopQuantitativo($arrParam);
  $documento = $_POST['documento'];

  // -- Indica se houve sucesso na requisição ao webservice
  $wsSucesso = false;
  $numProcessados = array();
  $wsErroMessage = null;

  if ($documento == 'obterProgramacaoCompletaQuantitativo') {
    $arrResponse = $oSiop->obterProgramacaoCompletaQuantitativo();
    if ($arrResponse->return->sucesso) {
      $wsSucesso = true;
      if (is_array($arrResponse->return->proposta)) {
        // -- Limpando os elementos da tabela de acordo com o momento
        $sql = <<<DML
DELETE FROM planacomorc.financeirosquantitativosof
  WHERE EXISTS (SELECT NULL
                  FROM planacomorc.quantitativosof
                  WHERE quantitativosof.quancod = financeirosquantitativosof.quancod
                    AND quantitativosof.qtscodigomomento = '{$_POST['codigomomento']}')
DML;
        $db->executar($sql);
        $sql = <<<DML
DELETE FROM planacomorc.quantitativosof
  WHERE quantitativosof.qtscodigomomento = '{$_POST['codigomomento']}'
DML;
        $db->executar($sql);
        // -- Inserindo os quantitativos relacionados ao momento
        $numProcessados['Registros importados na programação'] = 0;
        foreach ($arrResponse->return->proposta as $propostaDTO) {

          if (is_null($propostaDTO->quantidadeFisico)) { $propostaDTO->quantidadeFisico = 'null'; }
          if (is_null($propostaDTO->valorFisico)) { $propostaDTO->valorFisico = 'null'; }

          $sql = <<<DML
INSERT INTO planacomorc.quantitativosof(acaidentificadorunicosiop, loccod, qtsquantidadefisico,
                                        qtsvalorfisico, qtscodigotipodetalhamento, qtscodigomomento)
  VALUES({$propostaDTO->identificadorUnicoAcao}, '{$propostaDTO->codigoLocalizador}', {$propostaDTO->quantidadeFisico},
         {$propostaDTO->valorFisico}, '{$propostaDTO->codigoTipoDetalhamento}', '{$propostaDTO->codigoMomento}')
  RETURNING quancod
DML;
          $quanCode = $db->pegaUm($sql);

          // -- Inserindo os financeiros de cada quantitativo
          if ($quanCode) {
            $financeiros = $propostaDTO->financeiros;
            if (!is_array($financeiros)) {
              $financeiros = array($financeiros);
            }
            foreach ($financeiros as $financeiroDTO) {
              if (is_null($financeiroDTO->identificadorPlanoOrcamentario)) { $financeiroDTO->identificadorPlanoOrcamentario = 'null'; }
              if (is_null($financeiroDTO->valor)) { $financeiroDTO->valor = 'null'; }

              $sql = <<<DML
INSERT INTO planacomorc.financeirosquantitativosof(fqsfonte, fqsidoc, fqsiduso, fqsidentifplanoorcamentario,
                                                   fqsnaturezadespesa, fqsresultadoprimarioatual, fqsresultadoprimariolei,
                                                   fqsvalor, quancod)
  VALUES('{$financeiroDTO->fonte}', '{$financeiroDTO->idOC}', '{$financeiroDTO->idUso}', {$financeiroDTO->identificadorPlanoOrcamentario},
         '{$financeiroDTO->naturezaDespesa}', '{$financeiroDTO->resultadoPrimarioAtual}', '{$financeiroDTO->resultadoPrimarioLei}',
         {$financeiroDTO->valor}, {$quanCode})
DML;
              $db->executar($sql);
            }
          }
          $numProcessados['Registros importados na programação']++;
        }
        if (!$db->commit()) {
          $numProcessados['Registros importados na programação'] = 0;
//          $html = 'Dados carregados com sucesso. Total de Registros: ' . $totalRegistro;
        }
      }
    } else {
      $wsErroMessage = $arrResponse->return->mensagensErro;
    }
  } else if ('exibicaoExecucaoOrcamentaria' == $documento) {
      $arrCabecalho = array('acao','acompanhamentopo', 'anoexercicio', 'autorizado', 'bloqueadoremanejamento',
                            'bloqueadosof', 'categoriaeconomica', 'creditocontidosof', 'detalheacompanhamentopo', 'disponivel',
                            'dotatual', 'dotinicialsiafi', 'dotacaoantecipada', 'dotacaoinicial', 'elementodespesa', 'empliquidado',
                            'empenhadoaliquidar', 'esfera', 'fonte', 'funcao', 'gruponaturezadespesa', 'identificadoracompanhamentopo',
                            'idoc', 'iduso', 'indisponivel', 'localizador', 'modalidadeaplicacao', 'natureza', 'pago', 'planointerno',
                            'planoorcamentario', 'programa', 'projetolei', 'rapapagarnaoprocessado', 'rapapagarprocessado', 'rapcanceladosnaoprocessados',
                            'rapcanceladosprocessados', 'rapexerciciosanteriores', 'rapinscritonaoprocessado', 'rapinscritoprocessado',
                            'rapnaoprocessadoaliquidar', 'rapnaoprocessadobloqueado', 'rapnaoprocessadoliquidadoapagar', 'rappagonaoprocessado',
                            'rappagoprocessado', 'resultadoprimarioatual', 'resultadoprimariolei', 'subelementodespesa', 'subfuncao', 'tematicapo',
                            'tipoapropriacaopo', 'tipocredito', 'unidadegestoraresponsavel', 'unidadeorcamentaria', 'anoreferencia',
                            'executadoporinscricaoderap', 'origem', 'numeroptres', 'mes');

      $select = <<<DML
SELECT *
  FROM wssof.ws_execucaoorcamentariadto
DML;





      $db->monta_lista_simples($select,$arrCabecalho,150,5,'N','100%','N',false,null/*array('5','15','35','45')*/,70);

      exit;
  } else {
    $arDocumentos = array(
        'consultarAcompanhamentoOrcamentario' => array(
            'desc' => 'Consultar Acompanhamento Orçamentário',
            'func' => 'insereAcompanhamentoOrcamentario',
            'return' => 'acompanhamentosAcoes'),
        'consultarExecucaoOrcamentaria' => array(
            'desc' => 'Consultar Execução Orçamentária',
            'func' => 'insereExecucaoOrcamentariaDTO',
            'return' => 'execucoesOrcamentarias'));
    if (in_array($documento, array_keys($arDocumentos))) { //ver($_POST, d);
        unset($_SESSION['planacomorc']);
        foreach ($_POST as $key => $var) {
            if (is_array($var)) {
                foreach ($var as $_key => $_var) {
                    $_SESSION['planacomorc'][$key][$_key] = $_var;
                }
            } else {
                $_SESSION['planacomorc'][$key] = $var;
            }
        }

      $retornoDTO = $oSiop->$documento(); // echo '<pre>'; ver($retornoDTO, $_POST, d);
      if ($_POST['send-email']) {
          ob_start();
          echo '<pre>';
          var_dump($retornoDTO);
          enviar_email(array('nome' => SIGLA_SISTEMA, 'email' => $_SESSION['email_sistema']),
                  array($_SESSION['email_sistema']), 'Resposta da Requisição à SOF', ob_get_contents());
          ob_end_clean();
      }
      if ($retornoDTO->return->sucesso) {
        $wsSucesso = true;
        $numProcessados[$arDocumentos[$documento]['desc']]
                = $arDocumentos[$documento]['func']($retornoDTO->return->$arDocumentos[$documento]['return']);
      } else {
        $wsErroMessage = $retornoDTO->return->mensagensErro;
      }
    } else {
      trigger_error("Documento '{$documento}' não está implementado.", E_USER_ERROR);
    }
  }

  // -- Feedback para o usuário sobre a operação com o webservice
  if ($wsSucesso) {
    // -- número de registros processados por tipo
    $msg = '<script>alert("Dados atualizados com sucesso:';
    foreach ($numProcessados as $nome => $qtd) {
      $msg .= '\n' . "* {$nome}: {$qtd}" . '';
    }
    $msg .= '"); window.location.href = "' . $_SERVER['REQUEST_URI'] . '";</script>';
  } else {
    $msg = '<script>alert("' . $wsErroMessage . '!");'
            . 'window.location.href = "' . $_SERVER['REQUEST_URI'] . '";'
            . '</script>';
  }
  echo $msg;
  exit();
}
// Cabecalho padrão do sistema
include APPRAIZ . "includes/cabecalho.inc";
monta_titulo('Comunicação de Dados - SIOP', 'Quantitativos');

$wsusuario = WEB_SERVICE_SIOP_USUARIO;
$wssenha = WEB_SERVICE_SIOP_SENHA;

$largura = "300px";
$altura = "215px";
$id = "div_auth";
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="/planacomorc/js/importa.js"></script>
<script type="text/javascript" src="/planacomorc/js/planacomorc.js"></script>
<link rel="stylesheet" type="text/css" href="/planacomorc/css/importa.css"/>
<style>
.popup_alerta {
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
  overflow: auto;
}
label { cursor: pointer; }
</style>
<?php
$arFuncoes = array(
    'exibicaoExecucaoOrcamentaria' => array(
        'det' => false,
        'desc' => 'Conteúdo da Execução Orçamentária'),
    'consultarExecucaoOrcamentaria' => array(
        'det' => true,
        'desc' => 'Execução Orçamentária',
        // -- Opções de agrupamento de retorno
        'options-agrupamento' => array(
            'saida[anoReferencia]' => 'Ano de Referência',
            'saida[anoExercicio]' => 'Ano de Exercício',
            'saida[programa]' => 'Programa',
            'saida[acao]' => 'Ação',
            'saida[localizador]' => 'Localizador',
            'saida[unidadeOrcamentaria]' => 'Unidade Orçamentária',
            'saida[planoOrcamentario]' => 'Plano Orçamentário',
            'saida[planoInterno]' => 'Plano Interno',
            'saida[numeroptres]' => 'PTRES',
            'saida[esfera]' => 'Esfera',
            'saida[funcao]' => 'Função',
            'saida[subFuncao]' => 'Subfunção',
            'saida[natureza]' => 'Natureza',
            'saida[unidadeGestoraResponsavel]' => 'Unidade Gestora Responsável',
            'saida[categoriaEconomica]' => 'Categoria Econômica',
            'saida[grupoNaturezaDespesa]' => 'grupoNaturezaDespesa',
            'saida[modalidadeAplicacao]' => 'Modalidade de Aplicação',
            'saida[elementoDespesa]' => 'Elemento de Despesa',
            'saida[subElementoDespesa]' => 'Subelemento de Despesa',
            'saida[idoc]' => 'Ientificadores de Doação e Op. Credito (idoc)',
            'saida[iduso]' => 'Identificadores de Uso (iduso)',
            'saida[fonte]' => 'Fonte',
            'saida[resultadoPrimarioLei]' => 'Cód. Resultado Primários na LOA',
            'saida[resultadoPrimarioAtual]' => 'Cód. Resultado Primários Atuais',
            'saida[tipoCredito]' => 'Tipo de Crédito',
            'saida[tipoApropriacaoPO]' => 'Tipo de Apropriação (PO)',
            'saida[tematicaPO]' => 'Tematicas (PO)',
            'saida[identificadorAcompanhamentoPO]' => 'Identificadores de Acompanhamento (PO)',
            'saida[acompanhamentoPO]' => 'Acompanhamentos (PO)',
            'saida[detalheAcompanhamentoPO]' => 'Detalhes de Acompanhamento (PO)',
        ),
        // -- Opções de retorno que variam de acordo com o agrupamento
        'options-retorno' => array(
            'saida[dotacaoAntecipada]' => 'Dotação Antecipada',
            'saida[dotacaoInicial]' => 'Dotação Inicial',
            'saida[dotInicialSiafi]' => 'Dotação Inicial Siafi',
            'saida[dotAtual]' => 'Dotação Atual',
            'saida[empLiquidado]' => 'Empenhado Liquidado',
            'saida[empenhadoALiquidar]' => 'Empenhado a Liquidar',
            'saida[pago]' => 'Pago',
            'saida[disponivel]' => 'Disponível',
            'saida[indisponivel]' => 'Indisponível',
            'saida[autorizado]' => 'Autorizado',
            'saida[projetoLei]' => 'projetoLei',
            'saida[rapInscritoProcessado]' => 'rapInscritoProcessado',
            'saida[rapExerciciosAnteriores]' => 'rapExerciciosAnteriores',
            'saida[rapCanceladosNaoProcessados]' => 'rapCanceladosNaoProcessados',
            'saida[rapCanceladosProcessados]' => 'rapCanceladosProcessados',
            'saida[rapAPagarNaoProcessado]' => 'rapAPagarNaoProcessado',
            'saida[rapAPagarProcessado]' => 'rapAPagarProcessado',
            'saida[rapPagoNaoProcessado]' => 'rapPagoNaoProcessado',
            'saida[rapPagoProcessado]' => 'rapPagoProcessado',
            'saida[rapInscritoNaoProcessado]' => 'rapInscritoNaoProcessado',
            'saida[rapNaoProcessadoBloqueado]' => 'rapNaoProcessadoBloqueado',
            'saida[rapNaoProcessadoALiquidar]' => 'rapNaoProcessadoALiquidar',
            'saida[rapNaoProcessadoLiquidadoAPagar]' => 'rapNaoProcessadoLiquidadoAPagar',
            'saida[creditoContidoSOF]' => 'creditoContidoSOF',
            'saida[bloqueadoSOF]' => 'bloqueadoSOF',
            'saida[bloqueadoRemanejamento]' => 'bloqueadoRemanejamento'),
        'options-check' => array(
            'saida[tipoPrecatorio]' => 'tipoPrecatorio')),
//    'consultarExecucaoOrcamentariaMensal' => array('desc' => 'Consultar Execução Orçamentária Mensal'),
//    'consultarExecucaoOrcamentariaEstataisMensal' => array('desc' => 'Consultar Execução Orçamentária Mensal com Estatais'),
//    'obterExecucaoOrcamentariaSam' => array('desc' => 'Obter Execução Orçamentária SAM'),
//    'consultarProposta' => array('desc' => 'Consultar Proposta'),
//    'obterProgramacaoCompletaQuantitativo' => array('desc' => 'Obter Programação Completa Quantitativo'),
//    'obterTabelasApoioQuantitativo' => array('desc' => 'Obter Tabelas Apoio Quantitativo')
    );
?>
<form method="post" name="formulario" id="formulario">
  <table align="center" width="95%" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
    <tr>
      <td class="subtitulodireita" style="width:30%">Funções:</td>
      <td style="padding: 0 20px 20px 20px;">
        <table align="center" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
          <?php foreach ($arFuncoes as $func => $funcConfig): ?>
          <tr><td>
            <input type="radio" name="documento" id="documento_<?php echo $func; ?>"
                   class="chamadaWs" value="<?php echo $func; ?>" onclick="mostraCombo(this.value);" />
            <label for="documento_<?php echo $func; ?>"><?php echo $funcConfig['desc']; ?></label>
            <div id="<?php echo $func; ?>" style="overflow:auto;display:none">
              <?php if (1 == $funcConfig['det']): ?>
              <div class="msg-info" style="width:500px">
                  Observações:<br />
                  <ul><li>Esta é a importação de análise da Execução Orçamentária, feita na tabela<b><u>WSSOF.WS_EXECUCAOORCAMENTARIADTO</u></b>.</li>
                      <li>A carga é feita <b><u>APAGANDO</u></b> os registros anteriores antes de fazer a nova carga.</li>
                  </ul>
              </div>
              <div style="text-align:center;border:1px dashed royalblue;margin:5px 20px 10px;padding:15px">
                  <input type="radio" name="execucao_mensal" value="0" id="execucao_mensal_nao"
                         <?php echo ((0 == $_SESSION['planacomorc']['execucao_mensal'])?'checked':'')?> />
                  <label for="execucao_mensal_nao" style="font-size:22px;margin-right:30px">Execução Orçamentária Atual</label>
                  <input type="radio" name="execucao_mensal" value="1" id="execucao_mensal_sim"
                         <?php echo ((1 == $_SESSION['planacomorc']['execucao_mensal'])?'checked':'')?> />
                  <label for="execucao_mensal_sim" style="font-size:22px">Execução Orçamentária Mensal</label>
                  <div style="text-align:center;margin:10px;padding:5px">
                      <input type="radio" name="apagar_execucao" value="2" id="apagar_execucao_nao" checked />
                      <label for="apagar_execucao_nao" style="font-size:15px"><b>NÃO APAGAR</b> execuções anteriores</label>
                      <input type="radio" name="apagar_execucao" value="1" id="apagar_execucao_sim" />
                      <label for="apagar_execucao_sim" style="font-size:15px"><b>APAGAR</b> execuções anteriores</label>
                  </div>
                  <div style="text-align:center;margin:5px;padding:5px">
                      <input type="checkbox" name="send-email" id="send-email" value="1" />
                      <label for="send-email" style="font-size:12px">Notificar requisição (<?php echo $_SESSION['email_sistema']; ?>)</label>
                  </div>
                  <div>
                      <input type="text" name="pagina" id="pagina" size="5" class="normal" />
                      <label for="pagina" style="font-size:12px">Núm. página</label>
                  </div>
              </div>
              <?php endif; ?>
              <?php if (isset($funcConfig['options-agrupamento'])): ?>
              <div style="height:200px;overflow-y:auto;float:left;width:270px">
                <p style="text-align:center;font-size:12px;margin-bottom:0px;font-weight:bold">Agrupamento</p>
                <?php foreach ($funcConfig['options-agrupamento'] as $name => $desc): if (!is_array($desc)): ?>
                <span><input type="checkbox" name="<?php echo $name; ?>" id="<?php echo $name; ?>" class="check_<?php echo $func; ?>"
                             <?php
                                list(,$name_sp) = explode('[', $name);
                                list($name_sp) = explode(']', $name_sp);
                                echo (isset($_SESSION['planacomorc']['saida'][$name_sp])?'checked':''); ?> />
                  <label for="<?php echo $name; ?>"><?php echo $desc; ?></label></span><br />
                <?php else: ?>
                <span><input type="checkbox" name="<?php echo $name; ?>" id="<?php echo $name; ?>" class="check_<?php echo $func; ?>" />
                  <label for="<?php echo $name; ?>"><?php echo $desc['nome']; ?></label></span><br />
                <?php endif; endforeach; ?>
              </div>
              <?php endif; ?>
              <?php if (isset($funcConfig['options-retorno'])): ?>
              <div style="height:200px;overflow-y:auto;float:left;width:270px">
                <p style="text-align:center;font-size:12px;margin-bottom:0px;font-weight:bold">Retornos</p>
                <?php foreach ($funcConfig['options-retorno'] as $name => $desc): ?>
                <span><input type="checkbox" name="<?php echo $name; ?>" id="<?php echo $name; ?>" class="check_<?php echo $func; ?>"
                             <?php
                                list(,$name_sp) = explode('[', $name);
                                list($name_sp) = explode(']', $name_sp);
                                echo (isset($_SESSION['planacomorc']['saida'][$name_sp])?'checked':'')?> />
                  <label for="<?php echo $name; ?>"><?php echo $desc; ?></label></span><br />
                <?php endforeach; ?>
              </div>
              <?php endif; ?>
              <?php if (isset($funcConfig['options-check'])): ?>
              <div style="height:200px;overflow-y:auto;float:left;width:270px">
                <p style="text-align:center;font-size:12px;margin-bottom:0px;font-weight:bold">Não identificado</p>
                <?php foreach ($funcConfig['options-check'] as $name => $desc): ?>
                <span><input type="checkbox" name="<?php echo $name; ?>" id="<?php echo $name; ?>" class="check_<?php echo $func; ?>" />
                  <label for="<?php echo $name; ?>"><?php echo $desc; ?></label></span><br />
                <?php endforeach; ?>
              </div>
              <?php endif; ?>
            </div>
          </td></tr>
          <?php endforeach; ?>
        </table>
        &nbsp;<br/>
      </td>
    </tr>
    <tr class="filtroWs consultarExecucaoOrcamentaria" style="display:none">
      <td colspan="2" align="center" style="background-color:#c0c0c0;">
        <input type="button" class="botao" value="Enviar Solicitação" onclick="solicitarExecucao2();">
      </td>
    </tr>
    <tr class="filtroWs consultarExecucaoOrcamentaria" style="display:none">
      <td class="subtitulodireita"><b>Ano de Referência:</b></td>
      <td><?php echo campo_texto("filtro[anoReferencia]", "N", "S", "Ano de Referência", "5", "4", "[#]", "", "", "", "", "id='anoReferencia'", '', $_SESSION['planacomorc']['filtro']['anoReferencia']); ?></td>
    </tr>
    <tr class="filtroWs consultarExecucaoOrcamentaria" style="display:none">
      <td class="subtitulodireita"><b>Mês:</b></td>
      <td><?php echo campo_texto("filtro[mes]", "N", "S", "Mês", "5", "2", "[#]", "", "", "", "", "id='mes'", '', $_SESSION['planacomorc']['filtro']['mes']); ?></td>
    </tr>
    <?php
    $filtros = array(
        'esferas' => array('desc' => 'Esferas', 'chars' => 2, 'type' => 'textarea',
            'class' => array('consultarExecucaoOrcamentaria'),),
        'unidadesOrcamentarias' => array('desc' => 'Unidades Orçamentárias (UO)', 'chars' => 5, 'type' => 'textarea',
            'class' => array('consultarExecucaoOrcamentaria'), 'value' => '26101'),
        'funcoes' => array('desc' => 'Funções', 'chars' => 2, 'type' => 'textarea',
            'class' => array('consultarExecucaoOrcamentaria')),
        'subFuncoes' => array('desc' => 'Subfunções', 'chars' => 3, 'type' => 'textarea',
            'class' => array('consultarExecucaoOrcamentaria')),
        'programas' => array('desc' => 'Programas', 'chars' => 4, 'type' => 'textarea',
            'class' => array('consultarExecucaoOrcamentaria')),
        'acoes' => array('desc' => 'Ações', 'chars' => 4, 'type' => 'textarea',
            'class' => array('consultarExecucaoOrcamentaria')),
        'localizadores' => array('desc' => 'Localizadores', 'chars' => 4, 'type' => 'textarea',
            'class' => array('consultarExecucaoOrcamentaria')),
        'planosOrcamentarios' => array('desc' => 'Planos Orçamentários (PO)', 'chars' => 4, 'type' => 'textarea',
            'class' => array('consultarExecucaoOrcamentaria')),
        'naturezasDespesas' => array('desc' => 'Naturezas de Despesas', 'chars' => 8, 'type' => 'textarea',
            'class' => array('consultarExecucaoOrcamentaria')),
        'categoriasEconomicas' => array('desc' => 'Categorias Econômicas', 'chars' => 1, 'type' => 'textarea',
            'class' => array('consultarExecucaoOrcamentaria')),
        'gruposNatureza' => array('desc' => 'Grupos de Naturezas de despesa', 'chars' => 1, 'type' => 'textarea',
            'class' => array('consultarExecucaoOrcamentaria')),
        'modalidadesAplicacao' => array('desc' => 'Modalidades de Aplicação', 'chars' => 2, 'type' => 'textarea',
            'class' => array('consultarExecucaoOrcamentaria')),
        'elementosDespesa' => array('desc' => 'Elementos de Despesa', 'chars' => 2, 'type' => 'textarea',
            'class' => array('consultarExecucaoOrcamentaria')),
        'idusos' => array('desc' => 'Identificadores de Uso (idusos)', 'chars' => 1, 'type' => 'textarea',
            'class' => array('consultarExecucaoOrcamentaria')),
        'fontes' => array('desc' => 'Fontes de Recursos', 'chars' => 4, 'type' => 'textarea',
            'class' => array('consultarExecucaoOrcamentaria')),
        'idocs' => array('desc' => 'Identificadores de Doação e Operações de Crédito (idocs)', 'chars' => 4, 'type' => 'textarea',
            'class' => array('consultarExecucaoOrcamentaria')),
        'resultadosPrimariosLei' => array('desc' => 'Resultados Primários na LOA', 'chars' => 1, 'type' => 'textarea',
            'class' => array('consultarExecucaoOrcamentaria')),
        'resultadosPrimariosAtuais' => array('desc' => 'Resultados Primários Atuais', 'chars' => 1, 'type' => 'textarea',
            'class' => array('consultarExecucaoOrcamentaria')),
        'planosInternos' => array('desc' => 'Planos Internos (PI)', 'chars' => 11, 'type' => 'textarea',
            'class' => array('consultarExecucaoOrcamentaria')),
        'unidadesGestorasResponsaveis' => array('desc' => 'Unidades Gestoras Atuais', 'chars' => 6, 'type' => 'textarea',
            'class' => array('consultarExecucaoOrcamentaria')),
        'tipoCredito' => array('desc' => 'Tipos de Inclusão de Ação', 'chars' => 1, 'type' => 'textarea',
            'tip' => '<br />Códigos Válidos:<br />A: LOA;<br />C: Crédito Especial;<br />G: Crédito Extraordinário.', 'type' => 'textarea',
            'class' => array('consultarExecucaoOrcamentaria')),
        'programacoesSelecionadas' => array('desc' => 'Marcações Programações Selecionadas', 'chars' => 1, 'type' => 'textarea',
            'class' => array('consultarExecucaoOrcamentaria')),
        'tiposPrecatorios' => array('desc' => 'Marcadores de Precatório', 'chars' => 1, 'type' => 'textarea',
            'class' => array('consultarExecucaoOrcamentaria')),
        'acompanhamentosIntensivos' => array('desc' => 'Marcadores de Acompanhamentos Intensivos', 'chars' => 2, 'type' => 'textarea',
            'class' => array('consultarExecucaoOrcamentaria')),
        'tiposApropriacaoPO' => array('desc' => 'Tipos de Apropriação PO', 'chars' => 1, 'type' => 'textarea',
            'class' => array('consultarExecucaoOrcamentaria')),
        'tematicasPO' => array('desc' => 'Tipos das Temáticas PO', 'chars' => 1, 'type' => 'textarea',
            'class' => array('consultarExecucaoOrcamentaria')),
        'acompanhamentosPO' => array('desc' => 'Acompanhamentos PO', 'chars' => 1, 'type' => 'textarea',
            'class' => array('consultarExecucaoOrcamentaria')),
        'identificadoresAcompanhamentoPO' => array('desc' => 'Identificadores de Acompanhamento PO', 'chars' => 2, 'type' => 'textarea',
            'class' => array('consultarExecucaoOrcamentaria')),
        'detalhesAcompanhamentoPO' => array('desc' => 'Detalhes de Acompanhamento PO', 'chars' => 2, 'type' => 'textarea',
            'class' => array('consultarExecucaoOrcamentaria')),

        'codigoEsfera' => array('desc' => 'Esfera', 'chars' => 2, 'type' => 'text',
            'class' => array('consultarAcompanhamentoOrcamentario')),
        'codigoUO' => array('desc' => 'Unidade Orçamentária (UO)', 'chars' => 5, 'type' => 'text',
            'class' => array('consultarAcompanhamentoOrcamentario')),
        'codigoFuncao' => array('desc' => 'Função', 'chars' => 2, 'type' => 'text',
            'class' => array('consultarAcompanhamentoOrcamentario')),
        'codigoSubFuncao' => array('desc' => 'Subfunção', 'chars' => 3, 'type' => 'text',
            'class' => array('consultarAcompanhamentoOrcamentario')),
        'codigoPrograma' => array('desc' => 'Programa', 'chars' => 4, 'type' => 'text',
            'class' => array('consultarAcompanhamentoOrcamentario')),
        'codigoAcao' => array('desc' => 'Ação', 'chars' => 4, 'type' => 'text',
            'class' => array('consultarAcompanhamentoOrcamentario')),
        'codigoTipoInclusao' => array('desc' => 'Tipo de Inclusão', 'chars' => 1, 'type' => 'text',
            'class' => array('consultarAcompanhamentoOrcamentario')),
        'codigoLocalizador' => array('desc' => 'Localizador', 'chars' => 4, 'type' => 'text',
            'class' => array('consultarAcompanhamentoOrcamentario')),
        'codigoTipoInclusaoLocalizador' => array('desc' => 'Inclusão do Tipo de Localizador', 'chars' => 1, 'type' => 'text',
            'class' => array('consultarAcompanhamentoOrcamentario')),
    );
    foreach ($filtros as $idFiltro => $filtroConfig):?>
    <tr class="filtroWs <?php echo implode(' ', $filtroConfig['class']); ?>" style="display:none">
      <td class="subtitulodireita"><b><?php echo $filtroConfig['desc']; ?></b></td>
      <?php if ('textarea' == $filtroConfig['type']): ?>
      <td><?php echo campo_textarea("filtro[{$idFiltro}]", 'N', 'S', "{$filtroConfig['label']}", 100, 2, '', '', 0,
                     "Insira um item por linha ({$filtroConfig['chars']} caracteres cada item){$filtroConfig['tip']}", null, null,
                     $_SESSION['planacomorc']['filtro'][$idFiltro])?></td>
      <?php elseif ('text' == $filtroConfig['type']): ?>
      <td><?php echo campo_texto("filtro[{$idFiltro}]", "N", "S", "{$filtroConfig['label']}", "10", "{$filtroConfig['chars']}", "", "", "", "", "", "id='{$idFiltro}'"); ?></td>
      <?php endif; ?>
    </tr>
    <?php endforeach; ?>
    <tr>
      <td colspan="2" align="center" style="background-color:#c0c0c0;">
        <input type="button" class="botao" value="Enviar Solicitação" onclick="solicitarExecucao2();">
      </td>
    </tr>
  </table>
<br />
<div id="mostra_tabela_apoio" style="display:none;height:600px"></div>
<script type="text/javascript">
function validacaoAdicional() {
//  if ($('#documento_consultarExecucaoOrcamentaria').attr('checked') && !$('#anoReferencia').attr('value')) {
//    alert('O Ano de referência é obrigatório.');
//    rolaTela('anoReferencia');
//    $('#anoReferencia').focus();
//    return;
//  }
  return true;
}
</script>
<div id="conteudoExecucao" style="display:block" title="Detalhes da ação"></div>
<input type="hidden" value="" name="requisicao" id="requisicao">
<?php require(dirname(__FILE__) . '/popup.inc'); ?>
<script language="javascript" type="text/javascript">
function solicitarExecucao2() {
    if (jQuery('#documento_exibicaoExecucaoOrcamentaria').attr('checked')) {
        ajaxatualizar('documento=exibicaoExecucaoOrcamentaria&requisicao=enviasolicitacao', 'conteudoExecucao');
        jQuery("#conteudoExecucao").dialog({draggable:true,resizable:true,width:800,height:600,modal:true,create:function(){
            $(this).css('maxHeight', 250);
        }});
    } else {
        solicitarExecucao();
    }
}
</script>