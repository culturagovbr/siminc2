<input type="hidden" name="req" id="req" value=""/>
<div class="ibox-title">
    <h5>Relat�rio Geral de PA-FNC</h5><br>
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
    echo $simec->select('filtro[prsano]', 'Exerc�cio', $preenchidos, $stSql, null, ['input-size' => 9, 'label-size' => 2]);
    ?>
    <label class="col-sm-2 col-md-2 col-lg-2 control-label">Colunas</label>
    <?php
    unset($agrupador, $destino, $origem);
    // In�cio das colunas
    $agrupador = new AgrupadorBootstrap('filtro', '');
    // Dados padr�o de destino (nulo)
    $destino = isset($agrupador2) ? $agrupador2 : array();

    # Dados das colunas do Relat�rio
    $arrColunas = montarColunasRelatorioPI();
    if ($retConsultaRelatorio['colunas']){
        foreach($arrColunas as $arrColuna){
            foreach($retConsultaRelatorio['colunas'] as $coluna){
                if ($coluna['dscampo']==$arrColuna['codigo']){
                    $destino[] = $arrColuna;
                }
            }
        }
        foreach($arrColunas as $arrColuna){
            $achou=false;
            foreach($retConsultaRelatorio['colunas'] as $coluna){
                if ($coluna['dscampo']==$arrColuna['codigo']){
                    $achou=true;
                }
            }
            if (!$achou){
                $origem[] = $arrColuna;
            }
        }        
    }else{
        $destino = isset($agrupador2) ? $agrupador2 : array();
        $origem = $arrColunas;
    }
    // exibe agrupador
    $agrupador->setOrigem('naoColuna', null, $origem);
    $agrupador->setDestino('coluna', null, $destino);
    $agrupador->exibir();

    $stSql = "
        SELECT DISTINCT
            suo.suoid AS codigo,
            suo.suocod || ' - ' || suo.suonome AS descricao,
            suo.prsano
        FROM public.vw_subunidadeorcamentaria suo
        WHERE
	        suo.prsano = '".$_REQUEST['filtro']['prsano']."'
	        AND suo.suostatus = 'A'
	        AND suo.unofundo IS TRUE";
        if (count($listaSubUnidades)>0){
            $stSql .= " AND suo.suoid in (".implode(',', $listaSubUnidades).")";
        }        
        $stSql .= " ORDER BY
                descricao
        ";
        
        if (count($listaSubUnidades)>0){
            $suoid = $listaSubUnidades;
            echo $simec->select("filtro[suoid][]", 'Subunidade', $suoid, $stSql, null, ['input-size' => 9, 'label-size' => 2]);
        }else{
            echo $simec->select("filtro[suoid][]", 'Subunidade', $suoid, $stSql, null, ['input-size' => 9, 'label-size' => 2]);
        }

    $stSql = "
        SELECT DISTINCT
            esdid AS codigo,
            esddsc AS descricao
        FROM workflow.estadodocumento
        WHERE
            tpdid = '".WF_TPDID_PLANEJAMENTO_PI_FNC."'
            AND esdstatus = 'A'
        ORDER BY
            1
    ";
    echo $simec->select("filtro[esdid][]", 'Situa��o', $esdid, $stSql, null, ['input-size' => 9, 'label-size' => 2]);

    $stSql = "
        SELECT DISTINCT
            eqd.eqdid AS codigo,
            eqd.eqddsc AS descricao
        FROM monitora.pi_enquadramentodespesa eqd
        WHERE eqd.eqdano = '".$_REQUEST['filtro']['prsano']."'
            AND eqdstatus = 'A'
        ORDER BY
            2,1
    ";
    echo $simec->select("filtro[eqdid][]", 'Enquadramento', $eqdid, $stSql, null, ['input-size' => 9, 'label-size' => 2]);

    # Traz o Todos por default
    $picted=$picted?$picted:'';
    echo $simec->radio("filtro[picted]", 'TED', $picted, ['t' => 'Sim', 'f' => 'N�o', '' => 'Todos'], null, ['input-size' => 9, 'label-size' => 2]);

    $stSql = "
        SELECT DISTINCT
            irp.irpcod AS codigo,
            irp.irpcod || ' - ' || irp.irpdsc AS descricao
        FROM public.identresultprimario irp
        WHERE
            irp.irpstatus = 'A'
        ORDER BY
            2,1
    ";
    echo $simec->select("filtro[irpcod][]", 'RP', $irpcod, $stSql, null, ['input-size' => 9, 'label-size' => 2]);

    echo"<div class='div_acaid'>";
    $stSql = "
        SELECT DISTINCT
            acao.acaid AS codigo,
            prgcod || ' - ' || acacod || ' - ' || COALESCE(acaobjetivocod, '') || ' - ' || loccod || ' - ' || acatitulo AS descricao
        FROM monitora.acao acao
        JOIN public.vw_subunidadeorcamentaria suo ON(
            suostatus = 'A'
            AND acao.unicod = suo.unocod
            AND suo.prsano = prgano
        )
        WHERE
            acastatus = 'A'
            AND prgano = '".$_REQUEST['filtro']['prsano']."'
        ORDER BY
            descricao
    ";
    echo $simec->select("filtro[acaid][]", 'A��o', $acaid, $stSql, null, ['input-size' => 9, 'label-size' => 2]);
    echo"</div>";

    $stSql = "
            SELECT DISTINCT
                oppid as codigo,
                oppcod || ' - ' || oppnome as descricao,
                prsano as ano
            FROM public.objetivoppa
            WHERE
                prsano = '".$_REQUEST['filtro']['prsano']."'
                AND oppstatus = 'A'
            ORDER BY
                3
    ";
    echo $simec->select("filtro[oppnome][]", 'Objetivo', $oppnome, $stSql, null, ['input-size' => 9, 'label-size' => 2]);

    $stSql = "SELECT
                mdeid AS codigo,
                mdedsc AS descricao
            FROM monitora.pi_modalidadeensino
            WHERE
                mdestatus = 'A'
                AND mdeano = '".$_REQUEST['filtro']['prsano']."'
            ORDER BY
                descricao ASC
    ";
    echo $simec->select("filtro[mdeid][]", '�rea Cultural', $mdeid, $stSql, null, ['input-size' => 9, 'label-size' => 2]);

    $stSql = "
              SELECT DISTINCT
                 capid AS codigo,
                 capano as ano,
                 capdsc AS descricao
             FROM monitora.pi_categoriaapropriacao
             WHERE
                 capano = '".$_REQUEST['filtro']['prsano']."'
                 AND capstatus = 'A'
             ORDER BY
                 3,2
    ";
    echo $simec->select("filtro[capid][]", 'Modalidade de Pactua��o', $capid, $stSql, null, ['input-size' => 9, 'label-size' => 2]);

    $stSql = "
              SELECT
                  mescod AS codigo,
                  mesdsc AS descricao
              FROM public.meses
              ORDER BY
                  codigo
    ";
    echo $simec->select("filtro[mescod][]", 'Edital', $mescod, $stSql, null, ['input-size' => 9, 'label-size' => 2]);

    $stSql = "
              SELECT
                  claid AS codigo,
                  cladsc AS descricao
              FROM acompanhamento.classificacao
              WHERE
                  tipid = '".TPDID_PLANO_INTERNO."'
                  AND clastatus = 'A'
              ORDER BY
                  2,1
    ";
    echo $simec->select("filtro[claid][]", 'Classifica��o', $claid, $stSql, null, ['input-size' => 9, 'label-size' => 2]);
    ?>
</div>