<?php
/**
 * Funções de inserção de dados na base de dados.
 * PLANACOMORC
 * $Id: funcoesImportacao.inc 72940 2014-01-03 17:55:18Z maykelbraz $
 */

/**
 * Salva o debug em um arquivo.
 * @param mixed $content
 */
function dbif($content, $stop = true) {
  ob_start();
  var_dump($content);
  file_put_contents('debug.txt', ob_get_contents() . (!$stop?"\n":''), (!$stop?FILE_APPEND:null));
  ob_end_clean();
  if ($stop) { exit; }
}

/**
 * Normalmente este método faz a carga no WSSOF, apagando todas as ocorrências
 * e adicionando novas. Quando utilizada como upsert, ele verifica a existência
 * dos elementos antes da inserção no esquema PLANACOMORC.
 * @param Array $produtosDTO Lista de produtos para inserção / atualização;
 * @param bool $upsert Flag indicando o comportamento da função.
 * @return int Quantidade de registros processados.
 * @see insereItemTabelaApoio
 * @see upsertInTable
 * @see produtoDTO
 */
function insereProdutosDTO($produtosDTO, $upsert = false) {
  $arConfig = array(
      'descricao' => array('tipo' => 'texto', 'func' => array(/*'__UTF8_ENC__', '__UTF8_DEC__', */'__ESCAPE_ASPAS__')),
      'codigoProduto' => array('tipo' => 'numerico'),
      'dataHoraAlteracao' => array('tipo' => 'texto'),
      'snAtivo' => array('tipo' => 'texto'));
  if (!$upsert) {
    return insereItemTabelaApoio('ws_produtosdto', $arConfig, $produtosDTO);
  }
  return upsertInTable('planacomorc_old.produto', $arConfig, $produtosDTO, $primaryKeys = array(
      'codigoProduto'));
}

/**
 * Normalmente este método faz a carga no WSSOF, apagando todas as ocorrências
 * e adicionando novas. Quando utilizada como upsert, ele verifica a existência
 * dos elementos antes da inserção no esquema PLANACOMORC.
 * @param Array $funcoesDTO Lista de funções para inserção / atualização;
 * @param bool $upsert Flag indicando o comportamento da função.
 * @return int Quantidade de registros processados.
 * @see insereItemTabelaApoio
 * @see upsertInTable
 * @see FuncoesDTO
 */
function insereFuncoesDTO($funcoesDTO, $upsert = false) {
  $arConfig = array(
      'codigoFuncao' => array('tipo' => 'numerico'),
      'exercicio' => array('tipo' => 'texto'),
      'dataHoraAlteracao' => array('tipo' => 'texto'),
      'descricao' => array('tipo' => 'texto'),
      'descricaoAbreviada' => array('tipo' => 'texto'),
      'snAtivo' => array('tipo' => 'texto'));
  if (!$upsert) {
    return insereItemTabelaApoio('ws_funcoesdto', $arConfig, $funcoesDTO);
  }
  $arConfig['codigoFuncao']['tipo'] = 'texto';
  $arConfig['codigoFuncao']['size'] = 5;
  $arConfig['codigoFuncao']['func'][] = '__PAD__';
  $arConfig['exercicio']['tipo'] = 'numerico';
  return upsertInTable('planacomorc_old.funcao', $arConfig, $funcoesDTO, $primaryKeys = array(
      'codigoFuncao', 'exercicio'));
}

/**
 * Normalmente este método faz a carga no WSSOF, apagando todas as ocorrências
 * e adicionando novas. Quando utilizada como upsert, ele verifica a existência
 * dos elementos antes da inserção no esquema PLANACOMORC.
 * @param Array $unidadesMedidaDTO Lista de unidades de medida para inserção / atualização;
 * @param bool $upsert Flag indicando o comportamento da função.
 * @return int Quantidade de registros processados.
 * @see insereItemTabelaApoio
 * @see upsertInTable
 * @see unidadeMedidaDTO
 */
function insereUnidadesMedidaDTO($unidadesMedidaDTO, $upsert = false) {
  $arConfig = array(
      'codigoUnidadeMedida' => array('tipo' => 'texto'),
      'dataHoraAlteracao' => array('tipo' => 'texto'),
      'descricao' => array('tipo' => 'texto', 'func' => array(
          '__UTF8_ENC__', '__UTF8_DEC__')),
      'snAtivo' => array('tipo' => 'texto'));
  if (!$upsert) {
    return insereItemTabelaApoio('ws_unidadesmedidadto', $arConfig, $unidadesMedidaDTO);
  }
  $arConfig['codigoUnidadeMedida']['size'] = 20;
  $arConfig['codigoUnidadeMedida']['func'][] = '__PAD__';
  return upsertInTable('planacomorc_old.unidademedida', $arConfig, $unidadesMedidaDTO, $primaryKeys = array(
      'codigoUnidadeMedida'));
}

/**
 * Normalmente este método faz a carga no WSSOF, apagando todas as ocorrências
 * e adicionando novas. Quando utilizada como upsert, ele verifica a existência
 * dos elementos antes da inserção no esquema PLANACOMORC.
 * @param Array $subfuncoesDTO Lista de subfunções para inserção / atualização;
 * @param bool $upsert Flag indicando o comportamento da função.
 * @return int Quantidade de registros processados.
 * @see insereItemTabelaApoio
 * @see upsertInTable
 * @see subFuncaoDTO
 */
function insereSubFuncoesDTO($subfuncoesDTO, $upsert = false) {
  $arConfig = array(
      'codigoSubFuncao' => array('tipo' => 'texto'),
      'codigoFuncao' => array('tipo' => 'texto'),
      'exercicio' => array('tipo' => 'texto'),
      'dataHoraAlteracao' => array('tipo' => 'texto'),
      'descricao' => array('tipo' => 'texto'),
      'descricaoAbreviada' => array('tipo' => 'texto'),
      'snAtivo' => array('tipo' => 'texto'));
  if (!$upsert) {
    return insereItemTabelaApoio('ws_subfuncoesdto', $arConfig, $subfuncoesDTO);
  }
  $arConfig['codigoSubFuncao']['size'] = 20;
  $arConfig['codigoSubFuncao']['func'][] = '__PAD__';
  $arConfig['codigoFuncao']['size'] = 20;
  $arConfig['codigoFuncao']['func'][] = '__PAD__';
  $arConfig['exercicio']['tipo'] = 'numerico';
  return upsertInTable('planacomorc_old.subfuncao', $arConfig, $subfuncoesDTO, $primaryKeys = array(
      'codigoSubFuncao', 'codigoFuncao', 'exercicio'));
}

/**
 * Normalmente este método faz a carga no WSSOF, apagando todas as ocorrências
 * e adicionando novas. Quando utilizada como upsert, ele verifica a existência
 * dos elementos antes da inserção no esquema PLANACOMORC.
 * @param Array $objetivosDTO Lista de objetivos para inserção / atualização;
 * @param bool $upsert Flag indicando o comportamento da função.
 * @return int Quantidade de registros processados.
 * @see insereItemTabelaApoio
 * @see upsertInTable
 * @see objetivosDTO
 */
function insereObjetivosDTO($objetivosDTO, $upsert = false) {
  $arConfig = array(
      'identificadorUnico' => array('tipo' => 'numerico'),
      'codigoMomento' => array('tipo' => 'numerico'),
      'exercicio' => array('tipo' => 'numerico'),
      'codigoObjetivo' => array('tipo' => 'texto'),
      'codigoOrgao' => array('tipo' => 'texto'),
      'codigoPrograma' => array('tipo' => 'texto'),
      'enunciado' => array('tipo' => 'texto'),
      'snExclusaoLogica' => array('tipo' => 'texto'));
  if (!$upsert) {
    return insereItemTabelaApoio('ws_objetivosdto', $arConfig, $objetivosDTO);
  }
  $arConfig['identificadorUnico']['renomear'] = 'identificadorUnicoSiop';
  return upsertInTable('planacomorc_old.objetivo', $arConfig, $objetivosDTO, $primaryKeys = array(
      'identificadorUnicoSiop'));
}

/**
 * Normalmente este método faz a carga no WSSOF, apagando todas as ocorrências
 * e adicionando novas. Quando utilizada como upsert, ele verifica a existência
 * dos elementos antes da inserção no esquema PLANACOMORC.
 * @param Array $iniciativasDTO Lista de iniciativas para inserção / atualização;
 * @param bool $upsert Flag indicando o comportamento da função.
 * @return int Quantidade de registros processados.
 * @see insereItemTabelaApoio
 * @see upsertInTable
 * @see iniciativasDTO
 */
function insereIniciativasDTO($iniciativasDTO, $upsert = false) {
  $arConfig = array(
      'identificadorUnico' => array('tipo' => 'numerico'),
      'codigoMomento' => array('tipo' => 'numerico'),
      'exercicio' => array('tipo' => 'numerico'),
      'codigoIniciativa' => array('tipo' => 'texto'),
      'codigoObjetivo' => array('tipo' => 'texto'),
      'codigoOrgao' => array('tipo' => 'texto'),
      'codigoPrograma' => array('tipo' => 'texto'),
      'snIndividualizada' => array('tipo' => 'texto'),
      'titulo' => array('tipo' => 'texto'),
      'snExclusaoLogica' => array('tipo' => 'texto'));
  if (!$upsert) {
    return insereItemTabelaApoio('ws_iniciativasdto', $arConfig, $iniciativasDTO);
  }
  $arConfig['identificadorUnico']['renomear'] = 'identificadorUnicoSiop';
  return upsertInTable('planacomorc_old.iniciativa', $arConfig, $iniciativasDTO, $primaryKeys = array(
      'identificadorUnicoSiop'));
}

/**
 * Este método faz a carga no PLANACOMORC, quando utilizada como upsert, ele verifica
 * a existência dos elementos antes da inserção.
 * @param Array $fontesDTO Lista de fontes para inserção / atualização;
 * @param bool $upsert Flag indicando o comportamento da função.
 * @return int Quantidade de registros processados.
 * @see insereItemTabelaApoio
 * @see upsertInTable
 * @see fontesDTO
 */
function insereFontesDTO($fontesDTO, $upsert = false) {
  $arConfig = array(
      'codigoFonte' => array('tipo' => 'texto', 'size' => 3, 'func' => array('__PAD__')),
      'exercicio' => array('tipo' => 'numerico'),
      'descricao' => array('tipo' => 'texto'),
      'snAtivo' => array('tipo' => 'texto'));
  if (!$upsert) {
    trigger_error('Não implementado', E_USER_ERROR);
  }
  return upsertInTable('planacomorc_old.fonte', $arConfig, $fontesDTO->fonte, $primaryKeys = array(
      'codigoFonte', 'exercicio'));
}

/**
 * Normalmente este método faz a carga no WSSOF, apagando todas as ocorrências
 * e adicionando novas. Quando utilizada como upsert, ele verifica a existência
 * dos elementos antes da inserção no esquema PLANACOMORC.
 * @param Array $esferasDTO Lista de esferas para inserção / atualização;
 * @param bool $upsert Flag indicando o comportamento da função.
 * @return int Quantidade de registros processados.
 * @see insereItemTabelaApoio
 * @see upsertInTable
 * @see esferasDTO
 */
function insereEsferasDTO($esferasDTO, $upsert = false) {
    $arConfig = array(
        'codigoEsfera' => array('tipo' => 'texto'),
        'dataHoraAlteracao' => array('tipo' => 'texto'),
        'descricao' => array('tipo' => 'texto'),
        'descricaoAbreviada' => array('tipo' => 'texto'),
        'snAtivo' => array('tipo' => 'texto'),
        'snValorizacao' => array('tipo' => 'texto'));
    if (!$upsert) {
      return insereItemTabelaApoio('ws_esferasdto', $arConfig, $esferasDTO);
    }
    $arConfig['codigoEsfera']['size'] = 3;
    $arConfig['codigoEsfera']['func'] = array('__PAD__');
    $arConfig['codigoEsfera']['renomear'] = 'codigoesfera';
    return upsertInTable('planacomorc.esfera', $arConfig, $esferasDTO, $primaryKeys = array(
        'codigoesfera'));
}

/**
 * Normalmente este método faz a carga no WSSOF, apagando todas as ocorrências
 * e adicionando novas. Quando utilizada como upsert, ele verifica a existência
 * dos elementos antes da inserção no esquema PLANACOMORC.
 * @param Array $tiposAcaoDTO Lista de tipos de ação para inserção / atualização;
 * @param bool $upsert Flag indicando o comportamento da função.
 * @return int Quantidade de registros processados.
 * @see insereItemTabelaApoio
 * @see upsertInTable
 * @see tiposAcaoDTO
 */
function insereTiposAcaoDTO($tiposAcaoDTO, $upsert = true) {
  $arConfig = array(
      'codigoTipoAcao' => array('tipo' => 'texto'),
      'descricao' => array('tipo' => 'texto'),
      'snAtivo' => array('tipo' => 'texto'));
  if (!$upsert) {
    return insereItemTabelaApoio('ws_tiposacaodto', $arConfig, $tiposAcaoDTO);
  }
  $arConfig['codigoTipoAcao']['size'] = 2;
  $arConfig['codigoTipoAcao']['func'] = array('__PAD__');
  $arConfig['codigoTipoAcao']['renomear'] = 'codigotipoacao';
  return upsertInTable('planacomorc_old.tipoacao', $arConfig, $tiposAcaoDTO, $primaryKeys = array(
      'codigotipoacao'));
}

function inserePlanosOrcamentariosDTO($planosOrcamentariosDTO) {
    $arConfig = array(
        'identificadorUnico' => array('tipo' => 'numerico'),
        'identificadorUnicoAcao' => array('tipo' => 'numerico'),
        'codigoMomento' => array('tipo' => 'numerico'),
        'exercicio' => array('tipo' => 'texto'),
        'planoOrcamentario' => array('tipo' => 'texto'),
        'titulo' => array('tipo' => 'texto'),
        'detalhamento' => array('tipo' => 'texto'),
        'codigoUnidadeMedida' => array('tipo' => 'texto'),
        'codigoProduto' => array('tipo' => 'numerico'),
        'codigoIndicadorPlanoOrcamentario' => array('tipo' => 'texto'),
        'dataHora' => array('tipo' => 'texto'));
    return insereItemTabelaApoio('ws_planosorcamentariosdto', $arConfig, $planosOrcamentariosDTO);
}

/**
 * Normalmente este método faz a carga no WSSOF, apagando todas as ocorrências
 * e adicionando novas. Quando utilizada como upsert, ele verifica a existência
 * dos elementos antes da inserção no esquema PLANACOMORC.
 * @param Array $tiposInclusaoDTO Lista de tipos de inclusão para inserção / atualização;
 * @param bool $upsert Flag indicando o comportamento da função.
 * @return int Quantidade de registros processados.
 * @see insereItemTabelaApoio
 * @see upsertInTable
 * @see tiposInclusaoDTO
 */
function insereTiposInclusaoDTO($tiposInclusaoDTO, $upsert = true) {
  $arConfig = array(
      'codigoTipoInclusao' => array('tipo' => 'numerico'),
      'dataHoraAlteracao' => array('tipo' => 'texto'),
      'descricao' => array('tipo' => 'texto'),
      'snAtivo' => array('tipo' => 'texto'));
  if (!$upsert) {
    return insereItemTabelaApoio('ws_tiposinclusaodto', $arConfig, $tiposInclusaoDTO);
  }
  $arConfig['codigoTipoInclusao']['renomear'] = 'codigotipoinclusao';
  return upsertInTable('planacomorc_old.tipoinclusao', $arConfig, $tiposInclusaoDTO, $primaryKeys = array(
      'codigotipoinclusao'));
}

function insereExecucaoOrcamentariaDTO_planacomorc($execucaoOrcamentariaDTO, $upsert = false) {
  $arConfig = array(
      'anoReferencia' => array('tipo' => 'texto', 'renomear' => 'dfsexercicio'),
      'dfsptres' => array('tipo' => 'texto', 'virtual' => true, 'concatenado' => array(
          'programa', 'acao', 'unidadeOrcamentaria', 'localizador'), 'separador' => '.'),
      'dotacaoInicial' => array('tipo' => 'numerico', 'renomear' => 'dfsdotacaoinicial'),
      'dotAtual' => array('tipo' => 'numerico', 'renomear' => 'dfsdotacaoatual'),
      'empenhadoALiquidar' => array('tipo' => 'numerico', 'renomear' => 'dfsempenhado'),
      'empLiquidado' => array('tipo' => 'numerico', 'renomear' => 'dfsliquidado'),
      'pago' => array('tipo' => 'numerico', 'renomear' => 'dfspago'),
      'rapInscritoNaoProcessado' => array('tipo' => 'numerico', 'renomear' => 'dfsrapnaoprocessadoinscritoliquido'),
      'rapNaoProcessadoLiquidadoAPagar' => array('tipo' => 'numerico', 'renomear' => 'dfsrapnaoprocessadoliquidadoapagar'),
      'rapPagoNaoProcessado' => array('tipo' => 'numerico', 'renomear' => 'dfsrapnaoprocessado'),
      'acao' => array('tipo' => 'texto', 'size' => 4, 'func' => array('__PAD__')),
      'unidadeOrcamentaria' => array('tipo' => 'texto', 'renomear' => 'unidade',
          'size' => 5, 'func' => array('__PAD__')),
      'localizador' => array('tipo' => 'texto', size => 4, 'func' => array('__PAD__')),
      'programa' => array('tipo' => 'texto', 'size' => 4, 'func' => array('__PAD__')),
      'numeroptres' => array('tipo' => 'texto', 'renomear' => 'ptres'),
      'planoOrcamentario' => array('tipo' => 'texto', 'size' => 4, 'renomear' => 'planoorcamentario',
          'func' => array('__PAD__')));
  if (!$upsert) {
    trigger_error('Não implementado', E_USER_ERROR);
  }
  return upsertInTable('planacomorc_old.dadosfinanceirossiafi', $arConfig,
          is_array($execucaoOrcamentariaDTO->execucaoOrcamentaria)
          ?$execucaoOrcamentariaDTO->execucaoOrcamentaria
          :array($execucaoOrcamentariaDTO->execucaoOrcamentaria), $primaryKeys = array(
              'acao', 'unidade', 'localizador', 'programa', 'dfsexercicio', 'planoorcamentario'));
}

function insereExecucaoOrcamentariaDTO_pi($execucaoOrcamentariaDTO, $upsert = false) {
    $arConfig = array(
      'anoReferencia' => array('tipo' => 'texto', 'renomear' => 'eppexercicio'),
      'eppptres' => array('tipo' => 'texto', 'virtual' => true, 'concatenado' => array(
          'programa', 'acao', 'unidadeOrcamentaria', 'localizador','planoOrcamentario'), 'separador' => '.'),
      'empenhadoALiquidar' => array('tipo' => 'numerico', 'renomear' => 'eppempenhado'),
      'empLiquidado' => array('tipo' => 'numerico', 'renomear' => 'eppliquidado'),
      'pago' => array('tipo' => 'numerico', 'renomear' => 'epppago'),
      'rapInscritoNaoProcessado' => array('tipo' => 'numerico', 'renomear' => 'epprapnaoprocessadoinscritoliquido'),
      'rapNaoProcessadoLiquidadoAPagar' => array('tipo' => 'numerico', 'renomear' => 'epprapnaoprocessadoliquidadoapagar'),
      'rapPagoNaoProcessado' => array('tipo' => 'numerico', 'renomear' => 'epprapnaoprocessado'),
      'acao' => array('tipo' => 'texto', 'size' => 4, 'func' => array('__PAD__')),
      'unidadeOrcamentaria' => array('tipo' => 'texto', 'renomear' => 'unidade',
          'size' => 5, 'func' => array('__PAD__')),
      'localizador' => array('tipo' => 'texto', 'size' => 4, 'func' => array('__PAD__')),
      'programa' => array('tipo' => 'texto', 'size' => 4, 'func' => array('__PAD__')),
      'numeroptres' => array('tipo' => 'texto'),
      'planoInterno' => array('tipo' => 'texto', 'size' => 12, 'renomear' => 'planointerno',
          'func' => array('__PAD__')));
    if (!$upsert) {
        trigger_error('Não implementado', E_USER_ERROR);
    }
    return upsertInTable('planacomorc_old.execucaoporpi', $arConfig,
            is_array($execucaoOrcamentariaDTO->execucaoOrcamentaria)
            ?$execucaoOrcamentariaDTO->execucaoOrcamentaria
            :array($execucaoOrcamentariaDTO->execucaoOrcamentaria), $primaryKeys = array(
                'acao', 'unidade', 'localizador', 'programa', 'eppexercicio', 'planointerno'));
}

function insereExecucaoOrcamentariaDTO_cf($execucaoOrcamentariaDTO, $upsert = false) { //ver($execucaoOrcamentariaDTO, d);
    $arConfig = array(
        'numeroptres' => array('tipo' => 'texto', 'renomear' => 'ptres'),
        'anoReferencia' => array('tipo' => 'numerico', 'renomear' => 'id_exercicio'),
        'id_ptres' => array('tipo' => 'sql', 'query' => <<<QUERY
SELECT ptr.id_ptres
  FROM planacomorc.ptres ptr
    INNER JOIN planacomorc.localizador loc USING (id_localizador)
    INNER JOIN planacomorc.acao_programatica apr USING (id_acao_programatica)
    INNER JOIN planacomorc.acao aca USING (id_acao)
    INNER JOIN planacomorc.programa prg USING (id_programa)
    INNER JOIN planacomorc.orgao org ON (apr.id_orgao = org.id_orgao)
    INNER JOIN planacomorc.plano_orcamentario plo USING (id_acao_programatica, id_plano_orcamentario)
  WHERE loc.codigo = '__LOC_CODIGO__' AND aca.codigo = '__ACA_CODIGO__'
    AND prg.codigo = '__PRG_CODIGO__' AND org.codigo = '__ORG_CODIGO__'
    AND plo.codigo = '__PLO_CODIGO__' AND apr.id_exercicio = {$_SESSION['exercicio']}
QUERY
),
        'dotacaoInicial' => array('tipo' => 'numerico', 'renomear' => 'dotacao_inicial'),
        'dotAtual' => array('tipo' => 'numerico', 'renomear' => 'dotacao_atual'),
        'empenhado' => array('tipo' => 'numerico_sum', 'sum' => array('empenhadoALiquidar', 'empLiquidado')),
//        'empenhadoALiquidar' => array('tipo' => 'numerico', 'renomear' => 'empenhado'),
        'empLiquidado' => array('tipo' => 'numerico', 'renomear' => 'liquidado'),
        'pago' => array('tipo' => 'numerico', 'renomear' => 'pago'),
        'rapInscritoNaoProcessado' => array('tipo' => 'numerico', 'renomear' => 'rap_np_inscrito_liquido'),
        'rapNaoProcessadoLiquidadoAPagar' => array('tipo' => 'numerico', 'renomear' => 'rap_np_liquidado_pagar'),
        'rapPagoNaoProcessado' => array('tipo' => 'numerico', 'renomear' => 'rap_np_pago'),
        'rapInscritoProcessado' => array('tipo' => 'numerico', 'renomear' => 'rap_proc_inscrito'),
        'rapCanceladosProcessados' => array('tipo' => 'numerico', 'renomear' => 'rap_proc_cancelado'),
        'rapAPagarProcessado' => array('tipo' => 'numerico', 'renomear' => 'rap_proc_pagar'),
        'rapPagoProcessado' => array('tipo' => 'numerico', 'renomear' => 'rap_proc_pago'));
    if (!$upsert) {
        trigger_error('Não implementado', E_USER_ERROR);
    }
    return upsertInTable('planacomorc.carga_financeira', $arConfig,
            is_array($execucaoOrcamentariaDTO->execucaoOrcamentaria)
            ?$execucaoOrcamentariaDTO->execucaoOrcamentaria
            :array($execucaoOrcamentariaDTO->execucaoOrcamentaria), $primaryKeys = array(
                'id_ptres'));
}


/**
 * Faz todo o tratamento dos campos para transferir do objeto de retorno para a
 * tabela de armazenamento. Todo o tratamento do campo é baseado na configuração
 * recebida em $configCampos. Verifica a existência do registro com base nos campos
 * informados em $primaryKeys.
 * Exemplo de $configCampos:
 * $configCampos = array(
 *  'nomeCampo1' => array('tipo' => 'texto'),
 *  'nomeCampo2' => array('tipo' => 'numerico'),
 *  'nomeCampo3' => array('tipo' => 'texto', 'size' => 3, 'func' => array('__PAD__')),
 *  'nomeCampo4' => array('tipo' => 'texto', 'renomear' => 'novoNomeCampo4'),
 *  'nomeCampo5' => array('tipo' => 'texto', 'func' => array('__UFT8_ENC__', '__UTF8_DEC__', '__ESCAPE_ASPAS__'),
 *  'nomeCampo6' => array('tipo' => 'virtual', 'concatenado' => array('campo1', 'campo2'), 'separador' => '.')
 * 
 * Observação: 'concatenado' não funciona com 'renomeado'.
 * @global cls_banco $db Classe de conexão com o banco.
 * @param string $nomeTabela Nome da tabela que receberá os dados. Deve incluiro schema: "scheme.tabela".
 * @param array $configCampos Configuração dos campos: funções de tratamento, tipo, tamanho, etc.
 * @param array $listaDTO Lista de dados para inserção / atualização;
 * @param array $primaryKeys Lista de primary keys da tabela, para verificação de existência do registro.
 * @return int Quantidade de registros processados.
 */
function upsertInTable($nomeTabela, $configCampos, $listaDTO, $primaryKeys) { //ver($configCampos, $listaDTO[0], d);
  global $db;
  
  $numRegistros = 0;
  $sqlInsert = "INSERT INTO {$nomeTabela}(__CAMPOS__) VALUES(__VALORES__)";
  $sqlUpdate = "UPDATE {$nomeTabela} SET __ATUALIZACOES__ WHERE __RESTRICOES__";

  if (!is_array($listaDTO)) {
    return $numRegistros;
  }
  // -- Consultando os itens já existentes na base de dados
  $sql = "SELECT " . implode(', ', $primaryKeys) . " FROM {$nomeTabela}";
  $dbDados = $db->carregar($sql);
  if (!$dbDados) { $dbDados = array(); }

  foreach ($listaDTO as $itemDTO) {
    $sqlCampos = $sqlValores = array();
    // -- Armazena os valores da primary key para verificação posterior
    $camposPk = array_fill_keys(array_map('strtolower', $primaryKeys), null);

    // -- Para cada registro, processamos suas colunas executando os tratamentos necessários    
    foreach ($configCampos as $campoWS => $config) {
      // -- Aplicando funções de processamento aos campos
      if (isset($config['func'])) {
        if (in_array('__PAD__', $config['func'])) {
          $itemDTO->$campoWS = padString($itemDTO->$campoWS, $config['size']);
        }
        if (in_array('__ESCAPE_ASPAS__', $config['func'])) {
          $itemDTO->$campoWS = escaparAspas($itemDTO->$campoWS);
        }
        if (in_array('__UTF8_ENC__', $config['func'])) {
          $itemDTO->$campoWS = utf8_encode($itemDTO->$campoWS);
        }
        if (in_array('__UTF8_DEC__', $config['func'])) {
          $itemDTO->$campoWS = utf8_decode($itemDTO->$campoWS);
        }
      }
      // -- Verificando se a coluna é concatenada
      if (isset($config['concatenado'])) {
        $separador = (isset($config['separador'])?$config['separador']:'');
        $config['concatenado'] = array_fill_keys($config['concatenado'], null);
        foreach ($config['concatenado'] as $key => $valor) {
          $config['concatenado'][$key] = $itemDTO->$key;
        }
        $valorVirtual = implode($separador, $config['concatenado']);
      }
      // Verificando se a coluna é uma consulta SQL
      if (isset($config['query'])) {
         $valorVirtual = '(' . str_replace(
                 array('__LOC_CODIGO__', '__ACA_CODIGO__', '__PRG_CODIGO__', '__ORG_CODIGO__', '__PLO_CODIGO__'),
                 array($itemDTO->localizador, $itemDTO->acao, $itemDTO->programa, $itemDTO->unidadeOrcamentaria, $itemDTO->planoOrcamentario),
                 $config['query']) . ')';
      }
      // -- Verificando se a coluna faz parte da chave primária
      if (isset($config['renomear']) && in_array($config['renomear'], $primaryKeys)) {
        $camposPk[strtolower($config['renomear'])] = $itemDTO->$campoWS;
      } elseif (in_array($campoWS, $primaryKeys)) {
        $camposPk[strtolower($campoWS)] = $itemDTO->$campoWS;
      }
      
      
      
      // -- Tratamento de conteúdo baseado no tipo do campo
      switch ($config['tipo']) {
        case 'numerico': if (is_null($itemDTO->$campoWS)) { $itemDTO->$campoWS = "null"; } break;
        case 'texto': $itemDTO->$campoWS = "'{$itemDTO->$campoWS}'"; break;
        case 'data': break;
      }
      $sqlCampos[] = strtolower(isset($config['renomear'])?"{$config['renomear']}":"{$campoWS}");

      
      if (!isset($config['virtual'])) {
          if (!isset($config['query'])) {
              if ('numerico_sum' == $config['tipo']) {
                  $sqlValores[] = $itemDTO->$config['sum'][0] + $itemDTO->$config['sum'][1];
              } else {
                  $sqlValores[] = $itemDTO->$campoWS;
              }
          } else {
              $sqlValores[] = "{$valorVirtual}";
          }
      } else {
          $sqlValores[] = "'{$valorVirtual}'";
      }
      }
    
    
    // -- Verificação dos dados para decidir por um update ou insert
    if (in_array($camposPk, $dbDados)) { // -- update
      $arAtualizacoes = array();
      // -- O combine é para criar um único array composto por campos como chave
      // -- e valores como valores do array.
      $arDadosWS = array_combine($sqlCampos, $sqlValores);
      foreach ($arDadosWS as $campo => $valor) {
        $arAtualizacoes[] = "{$campo} = {$valor}";
      }
      $arRestricoes = array();
      // -- O intersect é para pegar os nomes e valores dos campos da chave primária
      // -- já com o tratamento para utilização na query.
      $arDadosWS = array_intersect_key($arDadosWS, $camposPk);
      foreach ($arDadosWS as $campo => $valor) {
        $arRestricoes[] = "{$campo} = {$valor}";
      }
      $db->executar(str_replace(
              array('__ATUALIZACOES__', '__RESTRICOES__'),
              array(implode(', ', $arAtualizacoes), implode(' AND ', $arRestricoes)),
              $sqlUpdate));
    } else { // -- insert
      $db->executar(str_replace(
              array('__CAMPOS__', '__VALORES__'),
              array(implode(', ', $sqlCampos), implode(', ', $sqlValores)),
              $sqlInsert));
    }
    $numRegistros++;
  }
  if (!$db->commit()) { $numRegistros = 0; }
  return $numRegistros;
}

/**
 * Insert genérico na carga do wssof, precedido de um delete.
 * @global cls_banco $db
 * @param type $nomeTabela
 * @param type $configCampos
 * @param type $listaDTO
 * @param type $bDeleteAll
 * @return int
 */
function insereItemTabelaApoio($nomeTabela, $configCampos, $listaDTO, $bDeleteAll = true) {
  global $db;
  $numInserts = 0;
  if (is_array($listaDTO)) {
    if ($bDeleteAll) {
        if (2 != $_POST['apagar_execucao']) {
            $sql = <<<DML
DELETE FROM wssof.{$nomeTabela}
DML;
      $db->executar($sql);
        }
    }
    $sqlBase = <<<DML
INSERT INTO wssof.{$nomeTabela}(__CAMPOS__)
  VALUES(__VALORES__)
DML;

    // -- Processando o retorno do webservice
    foreach ($listaDTO as $classeDTO) {
      $sqlCampos = array();
      $sqlValores = array();
      // -- Processando cada campo do item
      foreach ($configCampos as $campoWS => $config) {
        // -- Funções de processamento dos campos
        if (isset($config['func'])) {
          if (in_array('__ESCAPE_ASPAS__', $config['func'])) {
            $classeDTO->$campoWS = str_replace("'", "''", $classeDTO->$campoWS);
          }
          if (in_array('__UTF8_ENC__', $config['func'])) {
            $classeDTO->$campoWS = utf8_encode($classeDTO->$campoWS);
          }
          if (in_array('__UTF8_DEC__', $config['func'])) {
            $classeDTO->$campoWS = utf8_decode($classeDTO->$campoWS);
          }
        }
        switch ($config['tipo']) {
          case 'numerico':
            if (is_null($classeDTO->$campoWS)) {
              $classeDTO->$campoWS = "null";
            }
            break;
          case 'texto': $classeDTO->$campoWS = "'{$classeDTO->$campoWS}'"; break;
          case 'variavel': $classeDTO->$campoWS = ($_POST['filtro']['mes'])?$_POST['filtro']['mes']:'null'; break;
          case 'data': break;
        }
        $sqlCampos[] = "{$campoWS}";
        $sqlValores[] = $classeDTO->$campoWS;
      }

      $db->executar(str_replace(
              array('__CAMPOS__', '__VALORES__'),
              array(implode(', ', $sqlCampos), implode(', ', $sqlValores)),
              $sqlBase));
      $numInserts++;
    }
    if (!$db->commit()) { $numInserts = 0; }
  }
  return $numInserts;
}

function escaparAspas($texto) {
  return str_replace("'", "''", $texto);
}
function padString($texto, $tamanho) {
  return str_pad($texto, $tamanho, ' ');
}


/**
 * 
 * @param array $basesGeograficasDTO BGs retornadas pelo sistema.
 * @return int Quantidade de inserções
 * @see BaseGeograficaDTO
 */
function insereBasesGeograficasDTO($basesGeograficasDTO) {
  $arConfig = array(
      'codigoBaseGeografica' => array('tipo' => 'numerico'),
      'descricao' => array('tipo' => 'texto'),
      'snAtivo' => array('tipo' => 'texto'),
      'snExclusaoLogica' => array('tipo' => 'texto'));
  return insereItemTabelaApoio('ws_basesgeograficasdto', $arConfig, $basesGeograficasDTO);
}

function insereMacroDesafiosDTO($macroDesafiosDTO) {
  $arConfig = array(
      'codigoMacroDesafio' => array('tipo' => 'texto'),
      'descricao' => array('tipo' => 'texto'),
      'titulo' => array('tipo' => 'texto'));
  return insereItemTabelaApoio('ws_macrodesafiosdto', $arConfig, $macroDesafiosDTO);
}

function insereMomentosDTO($momentosDTO) {
  if ($momentosDTO instanceof momentoDTO) {
    $momentosDTO = array($momentosDTO);
  }
  $arConfig = array(
      'codigoMomento' => array('tipo' => 'numerico'),
      'dataHoraAlteracao' => array('tipo' => 'texto'),
      'descricao' => array('tipo' => 'texto'),
      'snAtivo' => array('tipo' => 'texto'));
  return insereItemTabelaApoio('ws_momentosdto', $arConfig, $momentosDTO);
}

function inserePerfisDTO($perfisDTO) {
  $arConfig = array(
      'perfilId' => array('tipo' => 'numerico'),
      'descricao' => array('tipo' => 'texto'));
  return insereItemTabelaApoio('ws_perfisdto', $arConfig, $perfisDTO);
}
  
function inserePeriodicidadesDTO($periodicidadesDTO) {
  $arConfig = array(
      'codigoPeriodicidade' => array('tipo' => 'numerico'),
      'descricao' => array('tipo' => 'texto'),
      'snAtivo' => array('tipo' => 'texto'));
  return insereItemTabelaApoio('ws_periodicidadesdto', $arConfig, $periodicidadesDTO);
}
  
function insereRegioesDTO($regioesDTO) {
  $arConfig = array(
      'codigoRegiao' => array('tipo' => 'numerico'),
      'descricao' => array('tipo' => 'texto'),
      'sigla' => array('tipo' => 'texto'));
  return insereItemTabelaApoio('ws_regioesdto', $arConfig, $regioesDTO);
}
  
function insereTiposProgramaDTO($tiposProgramaDTO) {
  $arConfig = array(
      'codigoTipoPrograma' => array('tipo' => 'texto'),
      'descricao' => array('tipo' => 'texto'),
      'exercicio' => array('tipo' => 'numerico'),
      'snAtivo' => array('tipo' => 'texto'));
  return insereItemTabelaApoio('ws_tiposprogramadto', $arConfig, $tiposProgramaDTO);
}
  
function insereUnidadesMedidaIndicadorDTO($unidadesMedidaIndicadorDTO) {
  $arConfig = array(
      'codigoUnidadeMedidaIndicador' => array('tipo' => 'numerico'),
      'descricao' => array('tipo' => 'texto'),
      'exercicio' => array('tipo' => 'texto'),
      'snAtivo' => array('tipo' => 'texto'));
  return insereItemTabelaApoio('ws_unidadesmedidaindicadordto', $arConfig, $unidadesMedidaIndicadorDTO);
}

function insereMetasDTO($metasDTO) {
  $arConfig = array(
      'identificadorUnico' => array('tipo' => 'numerico'),
      'codigoMomento' => array('tipo' => 'numerico'),
      'exercicio' => array('tipo' => 'numerico'),
      'codigoMeta' => array('tipo' => 'numerico'),
      'codigoObjetivo' => array('tipo' => 'texto'),
      'codigoPrograma' => array('tipo' => 'texto'),
      'descricao' => array('tipo' => 'texto'));
  return insereItemTabelaApoio('ws_metasdto', $arConfig, $metasDTO);
}

function insereLocalizadoresDTO2($localizadoresDTO) {
  $arConfig = array(
      'identificadorUnicoAcao' => array('tipo' => 'numerico'),
      'exercicio' => array('tipo' => 'numerico'),
      'codigoMomento' => array('tipo' => 'numerico'),
      'codigoLocalizador' => array('tipo' => 'texto'),
      'codigoTipoInclusao' => array('tipo' => 'numerico'),
      'anoInicio' => array('tipo' => 'numerico'),
      'anoTermino' => array('tipo' => 'numerico'),
      'descricao' => array('tipo' => 'texto'),
      'justificativaRepercussao' => array('tipo' => 'texto'),
      'municipio' => array('tipo' => 'numerico'),
      'regiao' => array('tipo' => 'texto'),
      'siglaRegiao' => array('tipo' => 'texto'),
      'totalFinanceiro' => array('tipo' => 'numerico'),
      'totalFisico' => array('tipo' => 'numerico'),
      'uf' => array('tipo' => 'texto'),
      'snExclusaoLogica' => array('tipo' => 'texto'));
  return insereItemTabelaApoio('ws_localizadoresdto', $arConfig, $localizadoresDTO);
}

function insereRegionalizacoesDTO($regionalizacoesDTO) {
  $arConfig = array(
      'regionalizacaoId' => array('tipo' => 'numerico'),
      'exercicio' => array('tipo' => 'numerico'),
      'codigoMomento' => array('tipo' => 'numerico'),
      'identificadorUnicoMeta' => array('tipo' => 'numerico'),
      'codigoMeta' => array('tipo' => 'numerico'),
      'codigoObjetivo' => array('tipo' => 'texto'),
      'codigoPrograma' => array('tipo' => 'texto'),
      'codigoUnidadeMedida' => array('tipo' => 'texto'),
      'descricao' => array('tipo' => 'texto'),
      'exercicio' => array('tipo' => 'numerico'),
      'sigla' => array('tipo' => 'texto'),
      'valor' => array('tipo' => 'numerico'));
  return insereItemTabelaApoio('ws_regionalizacoesdto', $arConfig, $regionalizacoesDTO);
}

function insereUnidadesOrcamentariasDTO($unidadesOrcamentaisDTO) {
  $arConfig = array(
      'codigoOrgao' => array('tipo' => 'texto'),
      'exercicio' => array('tipo' => 'numerico'),
      'tipoOrgao' => array('tipo' => 'texto'),
      'codigoOrgaoPai' => array('tipo' => 'texto'),
      'descricao' => array('tipo' => 'texto'),
      'descricaoAbreviada' => array('tipo' => 'texto'),
      'orgaoId' => array('tipo' => 'numerico'),
      'orgaoSiorg' => array('tipo' => 'texto'),
      'snAtivo' => array('tipo' => 'texto'));
  return insereItemTabelaApoio('ws_unidadesorcamentariasdto', $arConfig, $unidadesOrcamentaisDTO);
}

function insereIndicadoresDTO($indicadoresDTO) {
  $arConfig = array(
      'identificadorUnico' => array('tipo' => 'numerico'),
      'codigoMomento' => array('tipo' => 'numerico'),
      'exercicio' => array('tipo' => 'numerico'),
      'codigoBaseGeografica' => array('tipo' => 'numerico'),
      'codigoIndicador' => array('tipo' => 'numerico'),
      'codigoPeriodicidade' => array('tipo' => 'numerico'),
      'codigoPrograma' => array('tipo' => 'texto'),
      'codigoUnidadeMedidaIndicador' => array('tipo' => 'numerico'),
      'dataApuracao' => array('tipo' => 'texto'),
      'descricao' => array('tipo' => 'texto'),
      'fonte' => array('tipo' => 'texto'),
      'formula' => array('tipo' => 'texto'),
      'snApuracaoReferencia' => array('tipo' => 'texto'),
      'valorReferencia' => array('tipo' => 'numerico'),
      'snExclusaoLogica' => array('tipo' => 'texto'));
  return insereItemTabelaApoio('ws_indicadoresdto', $arConfig, $indicadoresDTO);
}


function insereExecucaoOrcamentariaDTO($execucaoOrcamentariaDTO) {
  $arConfig = array(
      'numeroptres' => array('tipo' => 'texto'),
      'acao' => array('tipo' => 'texto'),
      'acompanhamentoPO' => array('tipo' => 'texto'),
      'anoExercicio' => array('tipo' => 'numerico'),
      'anoReferencia' => array('tipo' => 'numerico'),
      'autorizado' => array('tipo' => 'texto'),
      'bloqueadoRemanejamento' => array('tipo' => 'texto'),
      'bloqueadoSOF' => array('tipo' => 'texto'),
      'categoriaEconomica' => array('tipo' => 'texto'),
      'creditoContidoSOF' => array('tipo' => 'texto'),
      'detalheAcompanhamentoPO' => array('tipo' => 'texto'),
      'disponivel' => array('tipo' => 'texto'),
      'dotAtual' => array('tipo' => 'texto'),
      'dotInicialSiafi' => array('tipo' => 'texto'),
      'dotacaoAntecipada' => array('tipo' => 'texto'),
      'dotacaoInicial' => array('tipo' => 'texto'),
      'elementoDespesa' => array('tipo' => 'texto'),
      'empLiquidado' => array('tipo' => 'texto'),
      'empenhadoALiquidar' => array('tipo' => 'texto'),
      'esfera' => array('tipo' => 'texto'),
      'executadoPorInscricaoDeRAP' => array('tipo' => 'texto'),
      'fonte' => array('tipo' => 'texto'),
      'funcao' => array('tipo' => 'texto'),
      'grupoNaturezaDespesa' => array('tipo' => 'texto'),
      'identificadorAcompanhamentoPO' => array('tipo' => 'texto'),
      'idoc' => array('tipo' => 'texto'),
      'iduso' => array('tipo' => 'texto'),
      'indisponivel' => array('tipo' => 'texto'),
      'localizador' => array('tipo' => 'texto'),
      'modalidadeAplicacao' => array('tipo' => 'texto'),
      'natureza' => array('tipo' => 'texto'),
      'origem' => array('tipo' => 'texto'),
      'pago' => array('tipo' => 'texto'),
      'planoInterno' => array('tipo' => 'texto'),
      'planoOrcamentario' => array('tipo' => 'texto'),
      'programa' => array('tipo' => 'texto'),
      'projetoLei' => array('tipo' => 'texto'),
      'rapAPagarNaoProcessado' => array('tipo' => 'texto'),
      'rapAPagarProcessado' => array('tipo' => 'texto'),
      'rapCanceladosNaoProcessados' => array('tipo' => 'texto'),
      'rapCanceladosProcessados' => array('tipo' => 'texto'),
      'rapExerciciosAnteriores' => array('tipo' => 'texto'),
      'rapInscritoNaoProcessado' => array('tipo' => 'texto'),
      'rapInscritoProcessado' => array('tipo' => 'texto'),
      'rapNaoProcessadoALiquidar' => array('tipo' => 'texto'),
      'rapNaoProcessadoBloqueado' => array('tipo' => 'texto'),
      'rapNaoProcessadoLiquidadoAPagar' => array('tipo' => 'texto'),
      'rapPagoNaoProcessado' => array('tipo' => 'texto'),
      'rapPagoProcessado' => array('tipo' => 'texto'),
      'resultadoPrimarioAtual' => array('tipo' => 'texto'),
      'resultadoPrimarioLei' => array('tipo' => 'texto'),
      'subElementoDespesa' => array('tipo' => 'texto'),
      'subFuncao' => array('tipo' => 'texto'),
      'tematicaPO' => array('tipo' => 'texto'),
      'tipoApropriacaoPO' => array('tipo' => 'texto'),
      'tipoCredito' => array('tipo' => 'texto'),
      'unidadeGestoraResponsavel' => array('tipo' => 'texto'),
      'unidadeOrcamentaria' => array('tipo' => 'texto'),
      'mes' => array('tipo' => 'variavel', 'name' => 'mes'));
  return insereItemTabelaApoio('ws_execucaoorcamentariadto', $arConfig,
          is_array($execucaoOrcamentariaDTO->execucaoOrcamentaria)
            ?$execucaoOrcamentariaDTO->execucaoOrcamentaria
            :array($execucaoOrcamentariaDTO->execucaoOrcamentaria));
}

function insereAcompanhamentoOrcamentario($acompanhamentoOrcamentarioDTO) {
  ver($acompanhamentoOrcamentarioDTO, d);
}


if (!function_exists('insereAcoesDTO')) {
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
ver('ok inserir ações!', d);
    if (is_array($arrDados)) {
      // -- Lista as acoes já cadastradas
      $sql = <<<QUERY
  SELECT a.acaidentificadorunicosiop
    FROM planacomorc.acao a
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

        // -- Verificando se a acao já está cadastrada no sistema
        if (in_array($acaoDTO->identificadorUnico, $arrAcoes)) {

          $sql = <<<DML
  UPDATE planacomorc.acao
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
        dtalteracao = NOW()::date
    WHERE acaidentificadorunicosiop = '{$acaoDTO->identificadorUnico}'
DML;
        } else {
          $sql = <<<DML
  INSERT INTO planacomorc.acao(acacod, acaidentificadorunicosiop, esfcod, procod,
                               orgcod, acabaselegal, acacodigofuncao, acacodigoiniciativa,
                               acacodigomomento, acacodigoobjetivo, acacodigoproduto, acacodigosubfuncao,
                               acacodigotipoacao, acacodigotipoinclusaoacao, acacodigounidademedida, acadescricao,
                               acadetalhamentoimplementacao, acaespecificacaoproduto, acafinalidade, acaformaacompanhamento,
                               acaidentificacaosazonalidade, acainsumosutilizados, acasndescentralizada, acasndireta,
                               acaativo, acasnlinhacredito, acasntransferenciaobrigatoria, acasntransferenciavoluntaria,
                               acatitulo, acaunidaderesponsavel, acaexercicio, dtcriacao)
    VALUES('{$acaoDTO->codigoAcao}', '{$acaoDTO->identificadorUnico}', '{$acaoDTO->codigoEsfera}', '{$acaoDTO->codigoPrograma}',
           '{$acaoDTO->codigoOrgao}', '{$acaoDTO->baseLegal}', '{$acaoDTO->codigoFuncao}', '{$acaoDTO->codigoIniciativa}',
           '{$acaoDTO->codigoMomento}', '{$acaoDTO->codigoObjetivo}', '{$acaoDTO->codigoProduto}', '{$acaoDTO->codigoSubFuncao}',
           '{$acaoDTO->codigoTipoAcao}', '{$acaoDTO->codigoTipoInclusaoAcao}', '{$acaoDTO->codigoUnidadeMedida}', '{$acaoDTO->descricao}',
           '{$acaoDTO->detalhamentoImplementacao}', '{$acaoDTO->especificacaoProduto}', '{$acaoDTO->finalidade}', '{$acaoDTO->formaAcompanhamento}',
           '{$acaoDTO->identificacaoSazonalidade}', '{$acaoDTO->insumosUtilizados}', '{$acaoDTO->snDescentralizada}', '{$acaoDTO->snDireta}',
           '{$acaoDTO->snExclusaoLogica}', '{$acaoDTO->snLinhaCredito}', '{$acaoDTO->snTransferenciaObrigatoria}', '{$acaoDTO->snTransferenciaVoluntaria}',
           '{$acaoDTO->titulo}', '{$acaoDTO->unidadeResponsavel}', '{$acaoDTO->exercicio}', NOW()::date)
DML;
        }
        $db->executar($sql);
      }
      if ($db->commit()) {
        return count($arrDados);
      }
      return 0;
    }
  }
}

if (!function_exists('inserelocalizadoresdto')) {
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
    FROM planacomorc.localizador l
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
        $localizadorDTO->descricao = str_replace("'", "''", $localizadorDTO->descricao);
        $localizadorDTO->municipio = str_replace("'", "''", $localizadorDTO->municipio);

        // -- Verificando se o orgao já está cadastrado no sistema
        if (in_array(array(
            'acaidentificadorunicosiop' => $localizadorDTO->identificadorUnicoAcao,
            'loccod' => $localizadorDTO->codigoLocalizador), $arrLocalizadores)) {
          $sql = <<<DML
  UPDATE planacomorc.localizador
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
        loccodigomomento = $localizadorDTO->codigoMomento
    WHERE acaidentificadorunicosiop = {$localizadorDTO->identificadorUnicoAcao}
      AND loccod = '{$localizadorDTO->codigoLocalizador}'
DML;
        } else {
          $sql = <<<DML
  INSERT INTO planacomorc.localizador(acaidentificadorunicosiop, loccod, locanoinicio, locanotermino,
                                      loccodigotipoinclusao, locdescricao, locexercicio, locmunicipio,
                                      locregiao, locsiglaregiao, locativo, loctotalfinanceiro,
                                      loctotalfisico, locuf, loccodigomomento, dtcriacao)
    VALUES($localizadorDTO->identificadorUnicoAcao, '{$localizadorDTO->codigoLocalizador}', $localizadorDTO->anoInicio, $localizadorDTO->anoTermino,
           '{$localizadorDTO->codigoTipoInclusao}', '{$localizadorDTO->descricao}', $localizadorDTO->exercicio, '{$localizadorDTO->municipio}',
           '{$localizadorDTO->regiao}', '{$localizadorDTO->siglaRegiao}', '{$localizadorDTO->snExclusaoLogica}', $localizadorDTO->totalFinanceiro,
           $localizadorDTO->totalFisico, '{$localizadorDTO->uf}', $localizadorDTO->codigoMomento, NOW()::date)
DML;
        }
        $db->executar($sql);
      }
      if ($db->commit()) {
        return count($arrDados);
      }
    }
    return 0;
  }
}

if (!function_exists('insereprogramasdto')) {
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
    FROM planacomorc.programa p
QUERY;

      $arrProgramas = $db->carregarColuna($sql);
      if (!is_array($arrProgramas)) {
        $arrProgramas = array();
      }

      // -- Processando a lista de programas retornada pelo webservice
      foreach ($arrDados as $programaDTO) {
        if (in_array($programaDTO->codigoPrograma, $arrProgramas)) {
          $sql = <<<DML
  UPDATE planacomorc.programa
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
  INSERT INTO planacomorc.programa(procod, procodigomacrodesafio, procodigomomento, procodigoorgao,
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
      if ($db->commit()) {
        return count($arrDados);
      }
    }
    return 0;
  }
}

// ---------------------------------------------- ------------------------------
// -- Funções de carga para análise pelo analista
// ---------------------------------------------- ------------------------------
/**
 * Insere novas ações na base e atualiza as existentes.
 * 
 * @global cls_banco $db
 * @param array $arrDados Lista de acaoDTO retornado pelo webservice.
 * @return null|boolean
 * @see cls_banco
 * @see acaoDTO
 */
 function insereAcoesDTO2($arrDados) {
   
   ver($arrDados);
   
  global $db;
  // -- Pro caso de retornar um único registro (obterAcaoPorIdentificadorUnico)
  if ('acaoDTO' == get_class($arrDados)) {
    $arrDados = array($arrDados);
  }

  if (is_array($arrDados)) {
    // -- Lista as acoes já cadastradas
    $sql = <<<QUERY
SELECT a.identificadorunico
  FROM wssof.ws_acoesdto a
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

      // -- Verificando se a acao já está cadastrada no sistema
      if (in_array($acaoDTO->identificadorUnico, $arrAcoes)) {

        $sql = <<<DML
UPDATE wssof.ws_acoesdto
  SET codigoacao = '{$acaoDTO->codigoAcao}',
      codigoesfera = '{$acaoDTO->codigoEsfera}',
      codigoprograma = '{$acaoDTO->codigoPrograma}',
      codigoorgao = '{$acaoDTO->codigoOrgao}',
      baselegal = '{$acaoDTO->baseLegal}',
      codigofuncao = '{$acaoDTO->codigoFuncao}',
      codigoiniciativa = '{$acaoDTO->codigoIniciativa}',
      codigomomento = '{$acaoDTO->codigoMomento}',
      codigoobjetivo = '{$acaoDTO->codigoObjetivo}',
      codigoproduto = '{$acaoDTO->codigoProduto}',
      codigosubfuncao = '{$acaoDTO->codigoSubFuncao}',
      codigotipoacao = '{$acaoDTO->codigoTipoAcao}',
      codigotipoinclusaoacao = '{$acaoDTO->codigoTipoInclusaoAcao}',
      codigounidademedida = '{$acaoDTO->codigoUnidadeMedida}',
      descricao = '{$acaoDTO->descricao}',
      detalhamentoimplementacao = '{$acaoDTO->detalhamentoImplementacao}',
      especificacaoproduto = '{$acaoDTO->especificacaoProduto}',
      finalidade = '{$acaoDTO->finalidade}',
      formaacompanhamento = '{$acaoDTO->formaAcompanhamento}',
      identificacaosazonalidade = '{$acaoDTO->identificacaoSazonalidade}',
      insumosutilizados = '{$acaoDTO->insumosUtilizados}',
      sndescentralizada = '{$acaoDTO->snDescentralizada}',
      sndireta = '{$acaoDTO->snDireta}',
      snexclusaologica = '{$acaoDTO->snExclusaoLogica}',
      snlinhacredito = '{$acaoDTO->snLinhaCredito}',
      sntransferenciaobrigatoria = '{$acaoDTO->snTransferenciaObrigatoria}',
      sntransferenciavoluntaria = '{$acaoDTO->snTransferenciaVoluntaria}',
      titulo = '{$acaoDTO->titulo}',
      unidaderesponsavel = '{$acaoDTO->unidadeResponsavel}',
      exercicio = '{$acaoDTO->exercicio}'
  WHERE identificadorunico = '{$acaoDTO->identificadorUnico}'
DML;
      } else {
        $sql = <<<DML
INSERT INTO wssof.ws_acoesdto(codigoacao, identificadorunico, codigoesfera, codigoprograma,
                              codigoorgao, baselegal, codigofuncao, codigoiniciativa,
                              codigomomento, codigoobjetivo, codigoproduto, codigosubfuncao,
                              codigotipoacao, codigotipoinclusaoacao, codigounidademedida, descricao,
                              detalhamentoimplementacao, especificacaoproduto, finalidade, formaacompanhamento,
                              identificacaosazonalidade, insumosutilizados, sndescentralizada, sndireta,
                              snexclusaologica, snlinhacredito, sntransferenciaobrigatoria, sntransferenciavoluntaria,
                              titulo, unidaderesponsavel, exercicio, datahorainclusao)
  VALUES('{$acaoDTO->codigoAcao}', '{$acaoDTO->identificadorUnico}', '{$acaoDTO->codigoEsfera}', '{$acaoDTO->codigoPrograma}',
         '{$acaoDTO->codigoOrgao}', '{$acaoDTO->baseLegal}', '{$acaoDTO->codigoFuncao}', '{$acaoDTO->codigoIniciativa}',
         '{$acaoDTO->codigoMomento}', '{$acaoDTO->codigoObjetivo}', '{$acaoDTO->codigoProduto}', '{$acaoDTO->codigoSubFuncao}',
         '{$acaoDTO->codigoTipoAcao}', '{$acaoDTO->codigoTipoInclusaoAcao}', '{$acaoDTO->codigoUnidadeMedida}', '{$acaoDTO->descricao}',
         '{$acaoDTO->detalhamentoImplementacao}', '{$acaoDTO->especificacaoProduto}', '{$acaoDTO->finalidade}', '{$acaoDTO->formaAcompanhamento}',
         '{$acaoDTO->identificacaoSazonalidade}', '{$acaoDTO->insumosUtilizados}', '{$acaoDTO->snDescentralizada}', '{$acaoDTO->snDireta}',
         '{$acaoDTO->snExclusaoLogica}', '{$acaoDTO->snLinhaCredito}', '{$acaoDTO->snTransferenciaObrigatoria}', '{$acaoDTO->snTransferenciaVoluntaria}',
         '{$acaoDTO->titulo}', '{$acaoDTO->unidadeResponsavel}', '{$acaoDTO->exercicio}', NOW()::date)
DML;
      }
      $db->executar($sql);
    }
    if ($db->commit()) {
      return count($arrDados);
    }
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
function insereOrgaosDTO2($arrDados) {
  global $db;

  // -- Pro caso de retornar um único registro (obterAcaoPorIdentificadorUnico)
  if ('orgaoDTO' == get_class($arrDados)) {
    $arrDados = array($arrDados);
  }

  if (is_array($arrDados)) {
    // -- Lista de órgãos já cadastrados
    $sql = <<<QUERY
SELECT o.codigoorgao, o.tipoorgao
  FROM wssof.ws_orgaosdto o
QUERY;
    $arrOrgaos = $db->carregar($sql);
    if (!is_array($arrOrgaos)) {
      $arrOrgaos = array();
    }
    foreach ($arrDados as $orgaoDTO) {
      // -- Verificando se o orgao já está cadastrado no sistema
      if (in_array(array(
          'codigoorgao' => $orgaoDTO->codigoOrgao,
          'tipoorgao' => $orgaoDTO->tipoOrgao), $arrOrgaos)) {
        $sql = <<<DML
UPDATE wssof.ws_orgaosdto
  SET descricaoabreviada = '{$orgaoDTO->descricaoAbreviada}',
      descricao = '{$orgaoDTO->descricao}'
  WHERE codigoorgao = '{$orgaoDTO->codigoOrgao}'
    AND tipoorgao = '{$orgaoDTO->tipoOrgao}'
DML;
      } else {
        $sql = <<<DML
INSERT INTO wssof.ws_orgaosdto(codigoorgao, exercicio, descricaoabreviada, codigoorgaopai,
                               tipoorgao, descricao, orgaoid, orgaosiorg, snativo)
  VALUES('{$orgaoDTO->codigoOrgao}', $orgaoDTO->exercicio, '{$orgaoDTO->descricaoAbreviada}', '{$orgaoDTO->codigoOrgaoPai}',
         '{$orgaoDTO->tipoOrgao}', '{$orgaoDTO->descricao}', $orgaoDTO->orgaoId, '{$orgaoDTO->orgaoSiorg}', '{$orgaoDTO->snAtivo}')
DML;
      }
      $db->executar($sql);
    }
    if ($db->commit()) {
      return count($arrDados);
    }
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
function insereProgramasDTO2($arrDados) {
  global $db;

  // -- Pro caso de retornar um único registro (obterAcaoPorIdentificadorUnico)
  if ('programaDTO' == get_class($arrDados)) {
    $arrDados = array($arrDados);
  }
  
  if (is_array($arrDados)) {
    // -- Lista de programas já cadastrados
    $sql = <<<QUERY
SELECT p.codigoprograma
  FROM wssof.ws_programasdto p
QUERY;

    $arrProgramas = $db->carregarColuna($sql);
    if (!is_array($arrProgramas)) {
      $arrProgramas = array();
    }

    // -- Processando a lista de programas retornada pelo webservice
    foreach ($arrDados as $programaDTO) {
      
      if (is_null($programaDTO->codigoMacroDesafio)) {
        $programaDTO->codigoMacroDesafio = 0;
      }
      if (is_null($programaDTO->horizonteTemporalContinuo)) {
        $programaDTO->horizonteTemporalContinuo = 0;
      }
      
      if (in_array($programaDTO->codigoPrograma, $arrProgramas)) {
        $sql = <<<DML
UPDATE wssof.ws_programasdto
  SET codigomacrodesafio = {$programaDTO->codigoMacroDesafio},
      codigomomento = '{$programaDTO->codigoMomento}',
      codigoorgao = '{$programaDTO->codigoOrgao}',
      codigotipoprograma = '{$programaDTO->codigoTipoPrograma}',
      estrategiaimplementacao = '{$programaDTO->estrategiaImplementacao}',
      exercicio = '{$programaDTO->exercicio}',
      horizontetemporalcontinuo = {$programaDTO->horizonteTemporalContinuo},
      identificadorunico = '{$programaDTO->identificadorUnico}',
      justificativa = '{$programaDTO->justificativa}',
      objetivo = '{$programaDTO->objetivo}',
      objetivogoverno = '{$programaDTO->objetivoGoverno}',
      objetivosetorial = '{$programaDTO->objetivoSetorial}',
      problema = '{$programaDTO->problema}',
      publicoalvo = '{$programaDTO->publicoAlvo}',
      snexclusaologica = '{$programaDTO->snExclusaoLogica}',
      unidaderesponsavel = '{$programaDTO->unidadeResponsavel}',
      titulo = '{$programaDTO->titulo}'
  WHERE codigoprograma = '{$programaDTO->codigoPrograma}'
DML;
      } else {
        $sql = <<<DML
INSERT INTO wssof.ws_programasdto(codigoprograma, codigomacrodesafio, codigomomento, codigoorgao,
                                  codigotipoprograma, estrategiaimplementacao, exercicio, horizontetemporalcontinuo,
                                  identificadorunico, justificativa, objetivo, objetivogoverno,
                                  objetivosetorial, problema, publicoalvo, snexclusaologica,
                                  unidaderesponsavel, titulo)
  VALUES('{$programaDTO->codigoPrograma}', {$programaDTO->codigoMacroDesafio}, '{$programaDTO->codigoMomento}', '{$programaDTO->codigoOrgao}',
         '{$programaDTO->codigoTipoPrograma}', '{$programaDTO->estrategiaImplementacao}', '{$programaDTO->exercicio}', {$programaDTO->horizonteTemporalContinuo},
         '{$programaDTO->identificadorUnico}', '{$programaDTO->justificativa}', '{$programaDTO->objetivo}', '{$programaDTO->objetivoGoverno}',
         '{$programaDTO->objetivoSetorial}', '{$programaDTO->problema}', '{$programaDTO->publicoAlvo}', '{$programaDTO->snExclusaoLogica}',
         '{$programaDTO->unidadeResponsavel}', '{$programaDTO->titulo}')
DML;
      }
      $db->executar($sql);
    }
    if ($db->commit()) {
      return count($arrDados);
    }
  }
  return 0;
}

/**
 * Gera o array de configuração de insert referente ao DTO.
 * @param array $arDTO Array de respostas do webservice.
 */
function toArConfig($arDTO) {
  $propList = get_object_vars($arDTO[0]);
  $phpEOL = PHP_EOL;
  $showValue = false;
  $strValue = '';
  $strConfig = <<<CONFIG
  \$arConfig = array({$phpEOL}
CONFIG;
  foreach ($propList as $prop => $value) {
    $tipo = is_string($value)?'texto':(is_numeric($value)?'numerico':'null');
    if ($showValue) {
      $strValue = <<<VALUE
, 'value' => {$value}
VALUE;
    }
    $strConfig .= <<<CONFIG
      '{$prop}' => array('tipo' => '{$tipo}'{$strValue}),{$phpEOL}
CONFIG;
  }
  $strConfig .= ');';
  print_r($strConfig);
  die();
}

/**
 * Gera o DDL básico para a criação da tabela.
 * Observações para os campos de texto: Todo o conjunto de dados retornados é
 * analisado em busca da maior string entre eles. Depois de analisar todos os
 * registros, os campos de texto recebem um acréscimo de 40% em seu tamanho final.
 * @param mixed $arDTO
 */
function createTableDTO($arDTO) {
  $arPropriedades = get_object_vars($arDTO[0]);
  $arCamposTabela = array_fill_keys(
          array_keys($arPropriedades),
          array('tipo' => null, 'tamanho' => null));

  // -- Percorrendo todos os registros retornados pela consulta
  foreach ($arDTO as $itemDTO) {
    foreach ($arPropriedades as $nomePropriedade => $value) {
      $tipoArmazenado = gettype($arCamposTabela[$nomePropriedade]['tipo']);
      $tipoAtual = gettype($itemDTO->$nomePropriedade);

      switch ($tipoAtual) {
        case 'string':
          switch ($tipoArmazenado) {
            case 'string': // -- Se os dois são strings
              // -- Se o tamanho da string atual for maior que o da string armazenada
              if (strlen($itemDTO->$nomePropriedade) > $arCamposTabela[$nomePropriedade]['tamanho']) {
                // -- Armazena o novo tamanho
                $arCamposTabela[$nomePropriedade]['tamanho'] = strlen($itemDTO->$nomePropriedade);
              }
              break;
            case 'integer':
            case 'NULL':
              $arCamposTabela[$nomePropriedade]['tipo'] = $tipoAtual;
              $arCamposTabela[$nomePropriedade]['tamanho'] = strlen($itemDTO->$nomePropriedade);
              break;
          }
          break;
        case 'integer':
          if (('string' != $tipoArmazenado) && ('integer' != $tipoArmazenado)) {
              $arCamposTabela[$nomePropriedade]['tipo'] = $tipoAtual;
          }
          break;
        case 'NULL':
          break;
      }
    }
  }

  // -- Gerando o create table para execução na base de dados.
  $nomeTabela = 'ws_' . strtolower(get_class($arDTO[0]));
  $sql = array();
  foreach ($arCamposTabela as $campo => $config) {
    $campo = strtolower($campo);
    switch ($config['tipo']) {
      case 'integer': $sql[] = "  {$campo} integer"; break;
      case 'string':
        if (1 == $config['tamanho']) {
          $tipo = 'character';
        } else {
          $tipo = 'character varying';
          $config['tamanho'] += ceil(($config['tamanho']*0.3));
        }
        $sql[] = "  {$campo} {$tipo}({$config['tamanho']})";
        break;
      case 'NULL': break;
    }
  }
  // -- Montando a string completa
  $sql = "CREATE TABLE wssof.{$nomeTabela}(\r"
        . implode(",\r", $sql)
        . ');';
  print_r($sql);
  die();
}
?>
