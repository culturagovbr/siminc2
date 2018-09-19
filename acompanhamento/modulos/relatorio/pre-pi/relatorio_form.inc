<input type="hidden" name="req" id="req" value=""/>
<div class="ibox-title">
    <h5>Relatório Geral de Pré-PI</h5><br>
</div>
<div class="ibox-content">
    <?php
        $stSql = "SELECT DISTINCT prsano AS codigo, prsano AS descricao FROM proposta.preplanointerno WHERE plistatus = 'A' ORDER BY 1";

    $preenchidos = null;
    if(!$_REQUEST['filtro']['prsano']) {
        $_REQUEST['filtro']['prsano'] = $preenchidos = $_SESSION['exercicio'];
    } else {
        $preenchidos = $_REQUEST['filtro']['prsano'];
    }
    echo $simec->select('filtro[prsano]', 'Exercício', $preenchidos, $stSql, null, ['input-size' => 9, 'label-size' => 2]);
    ?>
    <label class="col-sm-2 col-md-2 col-lg-2 control-label">Colunas</label>
    <?php
    unset($agrupador, $destino, $origem);
    // Início das colunas
    $agrupador = new AgrupadorBootstrap('filtro', '');
    // Dados padrão de destino (nulo)
    $destino = isset($agrupador2) ? $agrupador2 : array();
    // Dados padrão de origem
    $origem = array(
        array('codigo' => 'pliid', 'descricao' => 'Id Pré-PI'),
        array('codigo' => 'plititulo', 'descricao' => 'Título'),
        array('codigo' => 'plidsc', 'descricao' => 'Descrição/Finalidade'),
        array('codigo' => 'subunidade', 'descricao' => 'Subunidade'),
        array('codigo' => 'eqddsc', 'descricao' => 'Enquadramento da Despesa'),
        array('codigo' => 'irpcod', 'descricao' => 'RP'),
        array('codigo' => 'mainome', 'descricao' => 'Item'),
        array('codigo' => 'masnome', 'descricao' => 'Subitem'),
        array('codigo' => 'funcional', 'descricao' => 'Funcional'),
        array('codigo' => 'acatitulo', 'descricao' => 'Ação'),
        array('codigo' => 'plodsc', 'descricao' => 'PO'),
        array('codigo' => 'esddsc', 'descricao' => 'Situação'),
        array('codigo' => 'pprnome', 'descricao' => 'Produto'),
        array('codigo' => 'pumdescricao', 'descricao' => 'Unidade de Medida'),
        array('codigo' => 'pliquantidade', 'descricao' => 'Quantidade'),
        array('codigo' => 'oppcod', 'descricao' => 'Código Objetivo PPA'),
        array('codigo' => 'oppdsc', 'descricao' => 'Objetivo PPA'),
        array('codigo' => 'mppcod', 'descricao' => 'Código Metas PPA'),
        array('codigo' => 'mppnome', 'descricao' => 'Metas PPA'),
        array('codigo' => 'ippcod', 'descricao' => 'Código Iniciativa PPA'),
        array('codigo' => 'ippnome', 'descricao' => 'Iniciativa PPA'),
        array('codigo' => 'mpncod', 'descricao' => 'Código Meta PNC'),
        array('codigo' => 'mpnnome', 'descricao' => 'Meta PNC'),
        array('codigo' => 'ipncod', 'descricao' => 'Código Indicador PNC'),
        array('codigo' => 'ipndsc', 'descricao' => 'Indicador PNC'),
        array('codigo' => 'mdedsc', 'descricao' => 'Área Cultural'),
        array('codigo' => 'needsc', 'descricao' => 'Segmento Cultural'),
        array('codigo' => 'esfdsc', 'descricao' => 'Localização'),
        array('codigo' => 'paidescricao', 'descricao' => 'País'),
        array('codigo' => 'estuf', 'descricao' => 'UF'),
        array('codigo' => 'estdescricao', 'descricao' => 'Estado'),
        array('codigo' => 'munestuf', 'descricao' => 'UF do Município'),
        array('codigo' => 'mundescricao', 'descricao' => 'Município'),
        array('codigo' => 'plivalorcusteio', 'descricao' => 'Custeio'),
        array('codigo' => 'plivalorcapital', 'descricao' => 'Capital'),
        array('codigo' => 'pliquantidadeadicional', 'descricao' => 'Adicional de Quantidade'),
        array('codigo' => 'plivalorcusteioadicional', 'descricao' => 'Adicional de Valor Custeio'),
        array('codigo' => 'plivalorcapitaladicional', 'descricao' => 'Adicional de Valor Capital'),
        array('codigo' => 'plijustificativaadicional', 'descricao' => 'Justificativa do Adicional')
    );
    
    # Grava informações das colunas do relatorio especifico na sessao
    $_SESSION['relatorio_gerencial_pre_pi']['listaTodasColunas'] = $origem;
    
    // exibe agrupador
    $agrupador->setOrigem('naoColuna', null, $origem);
    $agrupador->setDestino('coluna', null, $destino);
    $agrupador->exibir();
    $stSql = "SELECT DISTINCT
                  suo.suoid AS codigo,
                  suo.suonome AS descricao,
                  suo.prsano
              FROM public.vw_subunidadeorcamentaria suo
              WHERE suo.prsano='".$_REQUEST['filtro']['prsano']."' AND suo.suostatus = 'A'
              ORDER BY 3,2";

    echo $simec->select("filtro[suoid][]", 'Unidade', $suoid, $stSql, null, ['input-size' => 9, 'label-size' => 2]);
    $stSql = "SELECT DISTINCT
                  eqd.eqdid AS codigo,
                  eqd.eqddsc AS descricao
              FROM monitora.pi_enquadramentodespesa eqd
              WHERE eqd.eqdano='".$_REQUEST['filtro']['prsano']."'
                  AND eqdstatus = 'A'
              ORDER BY 2,1";

    echo $simec->select("filtro[eqdid][]", 'Enquadramento', null, $stSql, null, ['input-size' => 9, 'label-size' => 2]);    
    $stSql = "  SELECT DISTINCT irp.irpcod AS codigo,
                       irp.irpcod || ' - ' || irp.irpdsc AS descricao
                  FROM public.identresultprimario irp
                 WHERE irp.irpstatus = 'A'
                 ORDER BY 2,1";

    echo $simec->select("filtro[irpcod][]", 'RP', null, $stSql, null, ['input-size' => 9, 'label-size' => 2]);
    $stSql = "SELECT DISTINCT
                  esdid AS codigo,
                  esddsc AS descricao
              FROM workflow.estadodocumento
              WHERE
                  tpdid = ".TPDID_PROPOSTA."
              ORDER BY
                  1";
//ver($stSql, d);
    echo $simec->select("filtro[tpdid][]", 'Situação', null, $stSql, null, ['input-size' => 9, 'label-size' => 2]);
    $stSql = "SELECT DISTINCT
                  tpd.tpdid AS codigo,
                  tpd.tpdcod || ' - ' || tpd.tpdnome AS descricao 
              FROM proposta.tipodetalhamento tpd
              WHERE
                  tpd.tpdstatus = 'A'
              ORDER BY
                  2,1";

    echo $simec->select("filtro[tpdid][]", 'Tipo de Detalhamento', null, $stSql, null, ['input-size' => 9, 'label-size' => 2]);    
    ?>    
    <div class="text-center">
        <a class="btn-success btn btn-md" href="javascript:exibeRelatorioGeral('exibir');"><i class="fa fa-eye"></i> Visualizar</a>
        <a class="btn btn-primary btn-buscar" href="javascript:exibeRelatorioGeral('xls');"><i class="fa fa-file-excel-o"></i> Exportar XLS</a>
    </div>
</div>