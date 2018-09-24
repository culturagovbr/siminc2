<?php
/**
 * PLANACOMORC
 * base: Programação Orçamentária
 * $Id: importaQuantitativos.inc 96801 2015-04-30 19:32:45Z maykelbraz $
 */
ini_set("memory_limit", "2048M");
set_time_limit(300000);

if ($_POST['requisicao'] == 'enviasolicitacao') {
    $arrParam = array(
        'usuario' => $_POST['wsusuario'],
        'senha' => $_POST['wssenha']);

    if ('cargaOrcamentaria' != $_POST['documento']) {
        $arrParam['post'] = $_POST;
        include_once APPRAIZ . 'planacomorc/classes/WSQuantitativo.class.inc';
        include_once APPRAIZ . 'planacomorc/classes/SiopQuantitativo.class.inc';
        include_once APPRAIZ . 'planacomorc/modulos/sistema/comunica/funcoesImportacao.inc';
        $oSiop = new SiopQuantitativo($arrParam);
    }
    $documento = $_POST['documento'];

    // -- Indica se houve sucesso na requisição ao webservice
    $wsSucesso = false;
    $numProcessados = array();
    $wsErroMessage = null;

    if ('obterAcoesDisponiveisAcompanhamentoOrcamentario' == $documento) {
        $data = $oSiop->obterAcoesDisponiveisAcompanhamentoOrcamentario();
        if (!$data->return->sucesso) {
            die('Sem registros');
        }
        echo <<<HTML
<table border="1">
    <thread>
        <tr>
            <th>Programática (Uo.Prg.Aca)</th>
            <th>A.O. Ação</th>
            <th>Localizador</th>
            <th>A.O. Loc.</th>
        </tr>
    </thead>
    <tbody>
HTML;
        $data = $data->return->acoes->acao;
        foreach ($data as $acao) {
            $programatica = "{$acao->codigoOrgao}.{$acao->codigoPrograma}.{$acao->codigoAcao}";
            $acaAcompOp = $acao->snAcompanhamentoOpcional?'S':'N';
            $localizadores = $acao->localizadores->localizador;
            if (is_object($localizadores)) { // -- Apenas um localizador
                $locAcompOp = $localizadores->snAcompanhamentoOpcional?'S':'N';
                echo <<<HTML
        <tr>
            <td>{$programatica}</td>
            <td>{$acaAcompOp}</td>
            <td>{$localizadores->codigoLocalizador}</td>
            <td>{$locAcompOp}</td>
        </tr>
HTML;
            } else { // -- Lista de localizadores
                foreach ($localizadores as $localizador) {
                    $locAcompOp = $localizador->snAcompanhamentoOpcional?'S':'N';
                    echo <<<HTML
        <tr>
            <td>{$programatica}</td>
            <td>{$acaAcompOp}</td>
            <td>{$localizador->codigoLocalizador}</td>
            <td>{$locAcompOp}</td>
        </tr>
HTML;
                }
            }
        }
        echo <<<HTML
    </tbody>
</table>
HTML;
        die();
    } elseif ('obterProgramacaoCompletaQuantitativoMetaPO' == $documento) {
        $response = $response = $oSiop->obterProgramacaoCompletaQuantitativo();
        if ($response->return->sucesso) {
            if (is_array($response->return->proposta)) {
                // -- Limpando a tabela
                $sql = <<<SQL
DELETE FROM planacomorc.meta_plano_orcamentario
SQL;
                $db->executar($sql);
                // -- Inserindo os registros em planacomorc.meta_plano_orcamentario
                $totalRegistro = 0;
                $insertSQL = <<<DML
INSERT INTO planacomorc.meta_plano_orcamentario(
    exercicio,
    funcional_concatenada,
    funcional_concatenada_completa,
    codigo_momento,
    identificador_unico_plano_orcamentario,
    quantidade_fisico,
    expansao_fisica_solicitada,
    expansao_fisica_concedida
) VALUES('%s', '%s', '%s', '%s', %d, %d, %d, %d)
DML;
                foreach ($response->return->proposta as $propostaDTO) {
                    if (is_null($propostaDTO->metaPlanoOrcamentario)) {
                        continue;
                    }
                    $funcional = "{$propostaDTO->codigoEsfera}.{$propostaDTO->codigoOrgao}.{$propostaDTO->codigoFuncao}."
                        . "{$propostaDTO->codigoSubFuncao}.{$propostaDTO->codigoPrograma}.{$propostaDTO->codigoAcao}."
                        . "{$propostaDTO->codigoLocalizador}";
                    $funcionalCompleta = "{$propostaDTO->codigoEsfera}.{$propostaDTO->codigoOrgao}.{$propostaDTO->codigoFuncao}."
                        . "{$propostaDTO->codigoSubFuncao}.{$propostaDTO->codigoPrograma}.{$propostaDTO->codigoAcao}."
                        . "{$propostaDTO->codigoTipoInclusaoAcao}.{$propostaDTO->codigoLocalizador}."
                        . "{$propostaDTO->codigoTipoInclusaoLocalizador}";

                    if (is_array($propostaDTO->metaPlanoOrcamentario)) {
                        foreach ($propostaDTO->metaPlanoOrcamentario as $metaPlanoOrcamentarioDTO) {
                            $insertStmt = sprintf(
                                $insertSQL,
                                $propostaDTO->exercicio,
                                $funcional,
                                $funcionalCompleta,
                                $propostaDTO->codigoMomento,
                                $metaPlanoOrcamentarioDTO->identificadorUnicoPlanoOrcamentario,
                                $metaPlanoOrcamentarioDTO->quantidadeFisico,
                                $metaPlanoOrcamentarioDTO->expansaoFisicaSolicitada,
                                $metaPlanoOrcamentarioDTO->expansaoFisicaConcedida
                            );
                            $db->executar($insertStmt);
                            $totalRegistro++;
                        }
                    } elseif ($propostaDTO->metaPlanoOrcamentario instanceof metaPlanoOrcamentarioDTO) {
                        $insertStmt = sprintf(
                            $insertSQL,
                            $propostaDTO->exercicio,
                            $funcional,
                            $funcionalCompleta,
                            $propostaDTO->codigoMomento,
                            $propostaDTO->metaPlanoOrcamentario->identificadorUnicoPlanoOrcamentario,
                            $propostaDTO->metaPlanoOrcamentario->quantidadeFisico,
                            $propostaDTO->metaPlanoOrcamentario->expansaoFisicaSolicitada,
                            $propostaDTO->metaPlanoOrcamentario->expansaoFisicaConcedida
                        );
                        $db->executar($insertStmt);
                        $totalRegistro++;
                    }
                }
                if ($db->commit()) {
                    $wsSucesso = true;
                    $numProcessados['Registros'] = $totalRegistro;
                } else {
                    $wsErroMessage = 'Não foi possível realizar a carga dos registros.';
                }
            }
        } else {
            $wsErroMessage = $response->return->mensagensErro;
        }
    } elseif ($documento == 'obterProgramacaoCompletaQuantitativo') {
        $response = $oSiop->obterProgramacaoCompletaQuantitativo();

        if ($response->return->sucesso) {
            if (is_array($response->return->proposta)) {
//                // -- Limpando os elementos da tabela de acordo com o momento
//                $sql = <<<DML
//DELETE FROM planacomorc_old.financeirosquantitativosof
//  WHERE EXISTS (SELECT 1
//                  FROM planacomorc_old.quantitativosof
//                  WHERE quantitativosof.quancod = financeirosquantitativosof.quancod
//                    AND quantitativosof.qtscodigomomento = '{$_POST['codigomomento']}')
//DML;
//                $db->executar($sql);
//                $sql = <<<DML
//DELETE FROM planacomorc_old.quantitativosof
//  WHERE quantitativosof.qtscodigomomento = '{$_POST['codigomomento']}'
//DML;
//                $db->executar($sql);
                // -- Inserindo os quantitativos relacionados ao momento
                $totalRegistro = 0;
                foreach ($response->return->proposta as $propostaDTO) {

                    if (is_null($propostaDTO->quantidadeFisico)) {
                        $propostaDTO->quantidadeFisico = 'null';
                    }
                    if (is_null($propostaDTO->valorFisico)) {
                        $propostaDTO->valorFisico = 'null';
                    }

                    $sql = <<<DML
INSERT INTO planacomorc_old.quantitativosof(acaidentificadorunicosiop, loccod, qtsquantidadefisico,
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
                            if (is_null($financeiroDTO->identificadorPlanoOrcamentario)) {
                                $financeiroDTO->identificadorPlanoOrcamentario = 'null';
                            }
                            if (is_null($financeiroDTO->valor)) {
                                $financeiroDTO->valor = 'null';
                            }

                            $sql = <<<DML
INSERT INTO planacomorc_old.financeirosquantitativosof(fqsfonte, fqsidoc, fqsiduso, fqsidentifplanoorcamentario,
                                                       fqsnaturezadespesa, fqsresultadoprimarioatual, fqsresultadoprimariolei,
                                                       fqsvalor, quancod)
  VALUES('{$financeiroDTO->fonte}', '{$financeiroDTO->idOC}', '{$financeiroDTO->idUso}', {$financeiroDTO->identificadorPlanoOrcamentario},
         '{$financeiroDTO->naturezaDespesa}', '{$financeiroDTO->resultadoPrimarioAtual}', '{$financeiroDTO->resultadoPrimarioLei}',
         {$financeiroDTO->valor}, {$quanCode})
DML;
                            $db->executar($sql);
                        }
                    }
                    $totalRegistro++;
                }
                if ($db->commit()) {
                    $wsSucesso = true;
                    $numProcessados['Registros'] = $totalRegistro;
                } else {
                    $wsErroMessage = 'Não foi possível realizar a carga dos registros.';
                }
            }
        } else {
            $wsErroMessage = $response->return->mensagensErro;
        }
    } elseif ('obterTabelasApoioQuantitativo' == $documento) {
        $retornoDTO = $oSiop->$documento();
        if ($retornoDTO->return->sucesso) {
            $wsSucesso = true;
            $numProcessados['Fontes'] = insereFontesDTO($retornoDTO->return->fontes, $upsert = true);
        } else {
            $wsErroMessage = $retornoDTO->return->mensagensErro;
        }
    }/* elseif ('cargaOrcamentaria' == $documento) {
      include_once APPRAIZ . 'planacomorc/classes/CargaOrcamentaria.class.inc';
      $cargaOrcamentaria = new CargaOrcamentaria($arrParam);
      $cargaOrcamentaria->obterTabelasDeApoio();
      } */ else {
        $arDocumentos = array(
            'consultarExecucaoOrcamentaria' => array(
                'desc' => 'Consultar Execução Orçamentária',
                'func' => 'insereExecucaoOrcamentariaDTO_planacomorc',
                'return' => 'execucoesOrcamentarias'),
            'consultarExecucaoOrcamentaria_cf' => array(
                'desc' => 'Carga financeira - Execução Orçamentária',
                'func' => 'insereExecucaoOrcamentariaDTO_cf',
                'call' => 'consultarExecucaoOrcamentaria',
                'posfixo' => '_cf',
                'return' => 'execucoesOrcamentarias'),
            'consultarExecucaoOrcamentaria_pi' => array(
                'desc' => 'Execução Orçamentária PI',
                'func' => 'insereExecucaoOrcamentariaDTO_pi',
                'call' => 'consultarExecucaoOrcamentaria',
                'posfixo' => '_pi',
                'return' => 'execucoesOrcamentarias'));
        if (in_array($documento, array_keys($arDocumentos))) {
            // 'Call deve ser definido sempre que o valor armazenado em $documento seja diferente do
            // nome da função que é chamada para consultar o webservice. Veja 'consultarExecucaoOrcamentaria'
            // e 'consultarExecucaoOrcamentaria_pi'. São cargas diferentes que chamam a mesma função de execucao.
            if (!isset($arDocumentos[$documento]['call'])) {
                $retornoDTO = $oSiop->$documento();
            } else {
                $retornoDTO = $oSiop->$arDocumentos[$documento]['call']($arDocumentos[$documento]['posfixo']);
            }

            if ($retornoDTO->return->sucesso) {
                $wsSucesso = true;
                $numProcessados[$arDocumentos[$documento]['desc']]
                    = $arDocumentos[$documento]['func']($retornoDTO->return->$arDocumentos[$documento]['return'], $upsert = true);
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
        $msg .= '"); window.location.href = "planacomorc.php?modulo=sistema/comunica/importaQuantitativos&acao=A";</script>';
    } else {
        $msg = '<script>alert("' . $wsErroMessage . '!");'
            . 'window.location.href = "planacomorc.php?modulo=sistema/comunica/importaQuantitativos&acao=A";</script>';
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
  overflow: auto;
}
label{cursor: pointer;}
</style>
<form method="post" name="formulario" id="formulario">
  <table align="center" width="95%" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
    <tr>
      <td class="subtitulodireita" style="width:30%">Funções:</td >
      <td style="padding: 0 20px 20px 20px;">
        <table align="center" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
          <tr><td>
            <input type="radio" name="documento" value="consultarExecucaoOrcamentaria_cf"
                   class="chamadaWs" id="documento_consultarExecucaoOrcamentaria_cf"
                   onclick="mostraCombo(this.value);" />
            <label for="documento_consultarExecucaoOrcamentaria_cf"><b><u>Carga financeira</u> - Execução Orçamentária</b></label>
            <div id="consultarExecucaoOrcamentaria_cf" style="display: none;">
              <?php
              $optQuebra = array(
                  'saida_cf[anoReferencia]' => 'Exercício',
                  'saida_cf[acao]' => 'Ação',
                  'saida_cf[programa]' => 'Programa',
                  'saida_cf[unidadeOrcamentaria]' => 'UO',
                  'saida_cf[planoOrcamentario]' => 'PO',
                  'saida_cf[localizador]' => 'Localizador');
              $optRetorno = array(
                  'saida_cf[numeroptres]' => 'PTRES',
                  'saida_cf[dotacaoInicial]' => 'Dotação Inicial',
                  'saida_cf[dotAtual]' => 'Dotação Atual',
                  'saida_cf[empenhadoALiquidar]' => 'Empenhado a Liquidar',
                  'saida_cf[empLiquidado]' => 'Empenhado Liquidado',
                  'saida_cf[pago]' => 'Pago',
                  'saida_cf[rapInscritoNaoProcessado]' => 'rapInscritoNaoProcessado',
                  'saida_cf[rapInscritoProcessado]' => 'rapInscritoProcessado',
                  'saida_cf[rapAPagarProcessado]' => 'rapAPagarProcessado',
                  'saida_cf[rapPagoProcessado]' => 'rapPagoProcessado',
                  'saida_cf[rapCanceladosProcessados]' => 'rapCanceladosProcessados',
                  'saida_cf[rapNaoProcessadoLiquidadoAPagar]' => 'rapNaoProcessadoLiquidadoAPagar',
                  'saida_cf[rapPagoNaoProcessado]' => 'rapPagoNaoProcessado');
              foreach (array_merge($optQuebra, $optRetorno) as $key => $value): ?>
                <input type="hidden" value="on" name="<?php echo $key; ?>" />
              <?php endforeach;
              $ultimo = array_pop($optQuebra);
              $strQuebra = implode(', ', $optQuebra) . " e {$ultimo}";
              $ultimo = array_pop($optRetorno);
              $strRetorno = implode(', ', $optRetorno) . " e {$ultimo}";
              ?>
                <div class="msg-info" style="width:500px">
                  Observações:<br />
                  <ul><li>Esta importação faz a carga da Execução orçamentária na tabela <b><u>PLANACOMORC.CARGA_FINANCEIRA</u></b>.</li>
                    <li>O conteúdo da tabela não é removido, apenas atualizado e incrementado.</li>
                    <li>A carga é feita detalhando valores por: <?php echo $strQuebra; ?>.</li>
                    <li>Os valores carregados são: <?php echo $strRetorno; ?>.</li>
                    <li>Esta carga é feita <b><u>ATUALIZANDO</u></b> os registros inseridos previamente.</li>
                  </ul>
                </div>
            </div>
          </td></tr>
          <tr><td>
            <input type="radio" name="documento" value="consultarExecucaoOrcamentaria"
                   class="chamadaWs" id="documento_consultarExecucaoOrcamentaria"
                   onclick="mostraCombo(this.value);" />
            <label for="documento_consultarExecucaoOrcamentaria">Execução Orçamentária por PO</label>
            <div id="consultarExecucaoOrcamentaria" style="display: none;">
              <?php
              $optQuebra = array(
                  'saida[anoReferencia]' => 'Exercício',
                  'saida[unidadeOrcamentaria]' => 'UO',
                  'saida[programa]' => 'Programa',
                  'saida[acao]' => 'Ação',
                  'saida[localizador]' => 'Localizador',
                  'saida[planoOrcamentario]' => 'PO');
              $optRetorno = array(
                  'saida[numeroptres]' => 'PTRES',
                  'saida[dotacaoInicial]' => 'Dotação Inicial',
                  'saida[dotAtual]' => 'Dotação Atual',
                  'saida[empenhadoALiquidar]' => 'Empenhado a Liquidar',
                  'saida[empLiquidado]' => 'Empenhado Liquidado',
                  'saida[pago]' => 'Pago',
                  'saida[rapInscritoNaoProcessado]' => 'rapInscritoNaoProcessado',
                  'saida[rapNaoProcessadoLiquidadoAPagar]' => 'rapNaoProcessadoLiquidadoAPagar',
                  'saida[rapPagoNaoProcessado]' => 'rapPagoNaoProcessado');
              foreach (array_merge($optQuebra, $optRetorno) as $key => $value): ?>
                <input type="hidden" value="on" name="<?php echo $key; ?>" />
              <?php  endforeach;
              $ultimo = array_pop($optQuebra);
              $strQuebra = implode(', ', $optQuebra) . " e {$ultimo}";
              $ultimo = array_pop($optRetorno);
              $strRetorno = implode(', ', $optRetorno) . " e {$ultimo}";
              ?>
                <div class="msg-info" style="width:500px">
                  Observações:<br />
                  <ul><li>Esta importação faz a carga da Execução orçamentária na tabela <b><u>PLANACOMORC_OLD.DADOSFINANCEIROSSIAFI</u></b>.</li>
                    <li>O conteúdo da tabela não é removido, apenas atualizado e incrementado.</li>
                    <li>A carga é feita detalhando valores por: <?php echo $strQuebra; ?>.</li>
                    <li>Os valores carregados são: <?php echo $strRetorno; ?>.</li>
                    <li>Esta carga é feita <b><u>ATUALIZANDO</u></b> os registros inseridos previamente.</li>
                  </ul>
                </div>
            </div>
          </td></tr>
          <tr><td>
            <input type="radio" name="documento" value="consultarExecucaoOrcamentaria_pi"
                   class="chamadaWs" id="documento_consultarExecucaoOrcamentaria_pi"
                   onclick="mostraCombo(this.value);" />
            <label for="documento_consultarExecucaoOrcamentaria_pi">Execução Orçamentária por PI</label>
            <div id="consultarExecucaoOrcamentaria_pi" style="display: none;">
              <?php
              $optQuebra = array(
                  'saida_pi[anoReferencia]' => 'Exercício',
                  'saida_pi[unidadeOrcamentaria]' => 'UO',
                  'saida_pi[programa]' => 'Programa',
                  'saida_pi[acao]' => 'Ação',
                  'saida_pi[localizador]' => 'Localizador',
                  'saida_pi[planoInterno]' => 'PI',
                  'saida_pi[planoOrcamentario]' => 'PO',
                  'saida_pi[numeroptres]' => 'PTRES');
              $optRetorno = array(
                  'saida_pi[empenhadoALiquidar]' => 'Empenhado a Liquidar',
                  'saida_pi[empLiquidado]' => 'Empenhado Liquidado',
                  'saida_pi[pago]' => 'Pago',
                  'saida_pi[rapInscritoNaoProcessado]' => 'rapInscritoNaoProcessado',
                  'saida_pi[rapNaoProcessadoLiquidadoAPagar]' => 'rapNaoProcessadoLiquidadoAPagar',
                  'saida_pi[rapPagoNaoProcessado]' => 'rapPagoNaoProcessado');
              foreach (array_merge($optQuebra, $optRetorno) as $key => $value): ?>
                <input type="hidden" value="on" name="<?php echo $key; ?>" />
              <?php endforeach;
              $ultimo = array_pop($optQuebra);
              $strQuebra = implode(', ', $optQuebra) . " e {$ultimo}";
              $ultimo = array_pop($optRetorno);
              $strRetorno = implode(', ', $optRetorno) . " e {$ultimo}";
              ?>
                <div class="msg-info" style="width:500px">
                  Observações:<br />
                  <ul><li>Esta importação faz a carga da Execução orçamentária na tabela <b><u>PLANACOMORC_OLD.EXECUCAOPORPI</u></b>.</li>
                    <li>O conteúdo da tabela não é removido, apenas atualizado e incrementado.</li>
                    <li>A carga é feita detalhando valores por: <?php echo $strQuebra; ?>.</li>
                    <li>Os valores carregados são: <?php echo $strRetorno; ?>.</li>
                    <li>Esta carga é feita <b><u>ATUALIZANDO</u></b> os registros inseridos previamente.</li>
                  </ul>
                </div>
            </div>
          </td></tr>
          <tr><td>
            <input type="radio" name="documento" value="obterProgramacaoCompletaQuantitativo"
                   class="chamadaWs" id="documento_obterProgramacaoCompletaQuantitativo"
                   onclick="mostraCombo(this.value);" />
            <label for="documento_obterProgramacaoCompletaQuantitativo">Obter Programação Completa Quantitativo:</label>
            <div id="obterProgramacaoCompletaQuantitativo" style="display: none;">
              <div class="msg-info" style="width:500px">
                Observações:<br />
                <ul><li>A carga da programação completa quantitativa é feita nas duas tabelas <b><u>PLANACOMORC_OLD.QUANTITATIVOSOF</u></b> e
                        <b><u>PLANACOMORC_OLD.FINANCEIROSQUANTITATIVOSOF</u></b>.</li>
                    <li>Esta carga é feita <b><u>APAGANDO</u></b> os registros anteriores antes de fazer a nova carga.</li>
                </ul>
              </div>
            </div>
          </td></tr>
          <tr><td>
            <input type="radio" name="documento" value="obterProgramacaoCompletaQuantitativoMetaPO"
                   class="chamadaWs" id="documento_obterProgramacaoCompletaQuantitativoMetaPO"
                   onclick="mostraCombo(this.value);" />
            <label for="documento_obterProgramacaoCompletaQuantitativoMetaPO">Obter Metas dos POs:</label>
            <div id="obterProgramacaoCompletaQuantitativoMetaPO" style="display:none">
              <div class="msg-info" style="width:500px">
                Observações:<br />
                <ul><li>A carga das metas do POs é feita na tabela <b><u>PLANACOMORC.META_PLANO_ORCAMENTARIO</u></b>.</li>
                    <li>Esta carga é feita <b><u>APAGANDO</u></b> os registros anteriores antes de fazer a nova carga.</li>
                </ul>
              </div>
            </div>
          </td></tr>
          <tr><td>
            <input type="radio" name="documento" value="obterTabelasApoioQuantitativo"
                   class="chamadaWs" id="documento_obterTabelasApoioQuantitativo"
                   onclick="mostraCombo(this.value);" />
            <label for="documento_obterTabelasApoioQuantitativo">Obter Tabelas Apoio Quantitativo:</label>
            <div id="obterTabelasApoioQuantitativo" style="display: none;">
              <div class="msg-info" style="width:500px">
                Observações:<br />
                <ul><li>A carga de fontes é feita na tabela <b><u>PLANACOMORC_OLD.FONTE</u></b>.</li>
                    <ul><li>Esta carga é feita <b><u>ATUALIZANDO</u></b> os registros inseridos previamente.</li></ul>
                </ul>
              </div>
              <table align="center" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
                <?php $arTabelasApoio = array(
                    'retornarFontes' => 'Retornar Fontes',
//                    'retornarNaturezas' => 'Retornar Naturezas',
//                    'retornarIdOcs' => 'Retornar IdOcs',
//                    'retornarIdUsos' => 'Retornar IdUsos',
//                    'retornarRPs' => 'Retornar RPs'
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
          <tr><td>
            <input type="radio" name="documento" value="obterAcoesDisponiveisAcompanhamentoOrcamentario"
                   class="chamadaWs" id="documento_obterAcoesDisponiveisAcompanhamentoOrcamentario"
                   onclick="mostraCombo(this.value);" />
            <label for="documento_obterAcoesDisponiveisAcompanhamentoOrcamentario">Ações disponíveis para AcompOrc</label>
            <div id="obterAcoesDisponiveisAcompanhamentoOrcamentario" style="display: none;">
            </div>
          </td></tr>
        </table>
      </td>
    </tr>
    <tr class="filtroWs obterTabelasApoioQuantitativo obterProgramacaoCompletaQuantitativo obterProgramacaoCompletaQuantitativoMetaPO">
      <td class="subtitulodireita"><b>Código do Momento:</b></td>
      <td><? echo campo_texto("codigomomento","S","S","codigomomento","22","","[#]","","","","","id='codigomomento'", '', '9000'); ?></td>
    </tr>
    <tr>
        <td class="subtitulodireita"><b>Página:</b></td>
      <td><? echo campo_texto("pagina","S","S","pagina","22","","[#]","","","","","id='pagina'", '', '1'); ?></td>
    </tr>
    <tr class="filtroWs consultarExecucaoOrcamentaria consultarExecucaoOrcamentaria_pi">
      <td class="subtitulodireita"><b>Ano de Referência:</b></td>
      <td><?php echo campo_texto("filtro[anoReferencia]", "S", "S", "Ano de Referência", "5", "4", "[#]", "", "", "", "", "id='anoReferencia'", '', '2013'); ?></td>
    </tr>
    <tr>
      <td colspan="2" align="center" style="background-color:#c0c0c0;">
        <input type="button" class="botao" value="Enviar Solicitação" onclick="solicitarExecucao();">
      </td>
    </tr>
  </table>
<br />
<script type="text/javascript">
/**
 * Validações adicionais da importação de quantitativos.
 */
function validacaoAdicional() {
  // -- Validação adicional da execução orçamentária
  if ($('#documento_consultarExecucaoOrcamentaria').attr('checked')
          && (!$('#anoReferencia').attr('value'))) {
    return alert('É obrigatório informar um ano de referência.');
  }
  // -- Validação adicional das tabelas de apoio e do programa quantitativo completo
  if (($('#documento_obterProgramacaoCompletaQuantitativo').attr('checked')
          ||$('#documento_obterTabelasApoioQuantitativo').attr('checked')) && !$('#anoReferencia').attr('value')) {
    return alert('É obrigatório informar um código de momento.');
  }
  return true;
}

/**
 * Indica para importa.js que a validação do momento deve ser ignorada.
 * Ela é feita manualmente na função validacaoAdicional.
 */
function ignorarMomento() { }


//  if (($('#documento_obterLocalizadores').attr('checked') || $('#documento_obterPlanosOrcamentarios').attr('checked'))
//          && (!$('#filtroAcaoValor').attr('value'))) {
//    alert('É obrigatório informar uma ação.');
//    return false;
//  }
//  return true;

</script>
<input type="hidden" value="" name="requisicao" id="requisicao">
<?php require(dirname(__FILE__) . '/popup.inc'); ?>