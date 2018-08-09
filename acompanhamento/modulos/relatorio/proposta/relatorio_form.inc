<input type="hidden" name="req" id="req" value=""/>
<div class="ibox-title">
    <h5>Relatório Geral de Proposta</h5><br>
</div>
<div class="ibox-content">
    <?php
    $stSql = "SELECT prsano AS codigo, prsano AS descricao FROM proposta.proposta WHERE prostatus = 'A' ORDER BY 1;";
    $preenchidos = null;
    if (!$_REQUEST['prsano']) {
        $preenchidos = array(array('codigo' => $_SESSION['exercicio'], 'descricao' => $_SESSION['exercicio']));
        $_REQUEST['prsano'][] = $_SESSION['exercicio'];
    } else {
        if (count($_REQUEST['prsano']) > 0) {
            $preenchidos = array();
            foreach ($_REQUEST['prsano'] AS $ano) {
                $preenchidos[] = array('codigo' => $ano, 'descricao' => $ano);
            }
        }
    }
    echo $simec->select('filtro[pro.prsano][]', 'Exercício', $preenchidos, $stSql, null, ['input-size' => 9, 'label-size' => 2]);
    ?>
    <label class="col-sm-2 col-md-2 col-lg-2 control-label">Colunas</label>
    <?php
    unset($agrupador, $destino, $origem);
    // Início das colunas
    $agrupador = new AgrupadorBootstrap('filtro', '');
    // Dados padrão de destino (nulo)
    $destino = isset($agrupador2) ? $agrupador2 : array();
    // Dados padrão de origem
    $origem = array(array('codigo' => 'locquantidadeproposta', 'descricao' => 'Quantidade Localizador'),
                    array('codigo' => 'proquantidade', 'descricao' => 'Quantidade PO'),
                    array('codigo' => 'proquantidadeexpansao', 'descricao' => 'Quantidade Expansão PO'),
                    array('codigo' => 'projustificativa', 'descricao' => 'Justificativa'),
                    array('codigo' => 'projustificativaexpansao', 'descricao' => 'Justificativa Expansão'),
                    array('codigo' => 'prdvalor', 'descricao' => 'Valor'),
                    array('codigo' => 'prdvalorexpansao', 'descricao' => 'Valor Expansão'),
                    array('codigo' => 'unosigla', 'descricao' => 'Subunidade'),
                    array('codigo' => 'eqddsc', 'descricao' => 'Enquadramento da Despesa'),
                    array('codigo' => 'irpcod', 'descricao' => 'RP'),
                    array('codigo' => 'funcional', 'descricao' => 'Funcional'),
                    array('codigo' => 'acatitulo', 'descricao' => 'Ação'),
                    array('codigo' => 'plodsc', 'descricao' => 'PO'),
                    array('codigo' => 'ndpid', 'descricao' => 'Natureza de Despesa'),
                    array('codigo' => 'iducod', 'descricao' => 'IDUSO'),
                    array('codigo' => 'foncod', 'descricao' => 'Fonte'),
                    array('codigo' => 'idoid', 'descricao' => 'IDOC'),
                    array('codigo' => 'proid', 'descricao' => 'ID Proposta'),
    );
    // exibe agrupador
    $agrupador->setOrigem('naoColuna', null, $origem);
    $agrupador->setDestino('coluna', null, $destino);
    $agrupador->exibir();

    $strAno = "";
    $strDsc = "";

    if (count($_REQUEST['prsano']) == 1) {
        $strAno = " uno.prsano IN ('" . implode("','", $_REQUEST['prsano']) . "')  AND ";
        $strDsc = " uno.unocod || ' - ' || uno.unonome  ";
    } else {
        $strDsc = " '(Exercicio ' || uno.prsano || ') ' || uno.unocod || ' - ' || uno.unonome ";
        if (count($_REQUEST['prsano']) > 0) {
            $strAno = " uno.prsano IN ('" . implode("','", $_REQUEST['prsano']) . "')  AND ";
        } else {
            $strAno = " ";
        }
    }

    $stSql = "  SELECT DISTINCT uno.unoid AS codigo,
                               {$strDsc} AS descricao,
                               uno.prsano
                          FROM public.unidadeorcamentaria uno
                         INNER JOIN public.subunidadeorcamentaria suo ON (suo.unoid = uno.unoid)
                         WHERE {$strAno} uno.unostatus = 'A'
                        {$whereResponsabilidade}
                         ORDER BY 3,2";

    echo $simec->select("filtro[e.unoid]", 'Unidade', null, $stSql, null, ['input-size' => 9, 'label-size' => 2]);
    ?>
    <?php
    $stSql = "  SELECT DISTINCT eqd.eqdid AS codigo,
                                eqd.eqddsc AS descricao
                            from monitora.pi_enquadramentodespesa
                            where eqdano = '".$_SESSION['exercicio']."'
                             and eqdstatus = 'A'
                         ORDER BY 2,1";

    echo $simec->select("filtro[eqd.eqd]", 'Enquadramento', null, $stSql, null, ['input-size' => 9, 'label-size' => 2]);    
    ?>
    <?php
    $stSql = "  SELECT DISTINCT eqd.eqdid AS codigo,
                                eqd.eqddsc AS descricao
                            from monitora.pi_enquadramentodespesa
                            where eqdano = '".$_SESSION['exercicio']."'
                             and eqdstatus = 'A'
                         ORDER BY 2,1";

    echo $simec->select("filtro[eqd.eqd]", 'Enquadramento', null, $stSql, null, ['input-size' => 9, 'label-size' => 2]);    
    ?>
    <div class="text-center">
        <a class="btn-success btn btn-md" href="javascript:exibeRelatorioGeral('exibir');">Visualizar</a>
        <a class="btn-info btn btn-md" href="javascript:exibeRelatorioGeral('xls');">Visualizar XLS</a>
    </div>
</div>