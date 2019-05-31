<input type="hidden" name="req" id="req" value=""/>

<div class="ibox-title">
    <h5>Relatório Geral de Emendas</h5><br>
</div>
<div class="ibox-content">
    <?php
        $stSql = "SELECT DISTINCT prsano AS codigo, prsano AS descricao FROM emendas.emenda WHERE emestatus = 'A' ORDER BY 1";

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

        # Dados das colunas do Relatório
        $arrColunas = Emendas_Model_Emenda::montarColunasRelatorioEmendas();
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
            // Dados padrão de origem
    //        $origem = 
        }
        // exibe agrupador
        $agrupador->setOrigem('naoColuna', null, $origem);
        $agrupador->setDestino('coluna', null, $destino);
        $agrupador->exibir();

        $stSql = "
            SELECT DISTINCT
                suo.suoid AS codigo,
                suo.suocod || ' - ' || suo.suonome AS descricao
            FROM public.vw_subunidadeorcamentaria suo
            WHERE
                suo.prsano = '".$_REQUEST['filtro']['prsano']."'
                AND suo.suostatus = 'A'";
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
                suo.suoid AS codigo,
                suo.suocod || ' - ' || suo.suonome AS descricao
            FROM public.vw_subunidadeorcamentaria suo
            WHERE
                suo.prsano = '".$_REQUEST['filtro']['prsano']."'
                AND suo.suostatus = 'A'
            ORDER BY
                descricao
        ";
        $suoid_delegada;
        echo $simec->select("filtro[suoid_delegada][]", 'Subunidade Delegada', $suoid_delegada, $stSql, null, ['input-size' => 9, 'label-size' => 2]);

        $stSql = "
            SELECT DISTINCT
                sit.sitid AS codigo,
                sit.sitdsc AS descricao
            FROM emendas.siconvsituacao sit
            WHERE
                sit.sitstatus = 'A'
            ORDER BY
                descricao
        ";
        $sitid;
        echo $simec->select("filtro[sitid][]", 'Situação SICONV', $sitid, $stSql, NULL, ['input-size' => 9, 'label-size' => 2]);

        $stSql = "
            SELECT DISTINCT
                esdid AS codigo,
                esddsc AS descricao
            FROM workflow.estadodocumento
            WHERE
                tpdid = '".WF_TPDID_BENEFICIARIO."'
                AND esdstatus = 'A'
            ORDER BY
                esddsc
        ";
        $esdid;
        echo $simec->select("filtro[esdid][]", 'Situação', $esdid, $stSql, NULL, ['input-size' => 9, 'label-size' => 2]);

        $stSql = "
            SELECT
                autid AS codigo,
                a.autcod || ' - ' || a.autnome || ' - ' || aut.atpdsc AS descricao
            FROM emendas.autor a -- SELECT * FROM emendas.autor a
                JOIN emendas.autortipo aut ON a.atpid = aut.atpid -- SELECT * FROM emendas.autortipo
                JOIN emendas.partido p ON a.parid = p.parid -- SELECT * FROM emendas.partido
            WHERE
                autstatus = 'A'
            ORDER BY
                descricao
        ";
        $autid;
        echo $simec->select("filtro[autid][]", 'Autor', $autid, $stSql, NULL, ['input-size' => 9, 'label-size' => 2]);

        $stSql = "
            SELECT DISTINCT
                estuf AS codigo,
                estdescricao || ' - ' || estuf AS descricao
            FROM territorios.estado
            ORDER BY
                descricao
        ";
        $estuf;   
        echo $simec->select("filtro[estuf][]", 'UF Parlamentar', $estuf, $stSql, NULL, ['input-size' => 9, 'label-size' => 2]);

        $stSql = "
            SELECT DISTINCT
                parid AS codigo,
                pardsc || ' - ' || parsigla AS descricao
            FROM emendas.partido
            WHERE
                parstatus = 'A'
            ORDER BY
                descricao
        ";
        $parid;
        echo $simec->select("filtro[parid][]", 'Partido', $parid, $stSql, NULL, ['input-size' => 9, 'label-size' => 2]);

        $stSql = "
            SELECT DISTINCT
                capid AS codigo,
                capdsc AS descricao
            FROM monitora.pi_categoriaapropriacao
            WHERE
                capano = '".$_REQUEST['filtro']['prsano']."'
                AND capstatus = 'A'
            ORDER BY
                descricao
        ";
        $parid;
        echo $simec->select("filtro[capid][]", 'Modalidade de Pactuação', NULL, $stSql, NULL, ['input-size' => 9, 'label-size' => 2]);

        # Traz o Todos por default
        $bented = $bented?$bented:'';
        echo $simec->radio("filtro[bented]", 'TED', $bented, ['t' => 'Sim', 'f' => 'Não', '' => 'Todos'], NULL, ['input-size' => 9, 'label-size' => 2]);

        $stSql = "
            SELECT
                mdeid AS codigo,
                mdedsc AS descricao
            FROM monitora.pi_modalidadeensino
            WHERE
                mdestatus = 'A'
                AND mdeano = '".$_REQUEST['filtro']['prsano']."'
            ORDER BY
                descricao ASC
        ";
        $mdeid;
        echo $simec->select("filtro[mdeid][]", 'Área Cultural', $mdeid, $stSql, NULL, ['input-size' => 9, 'label-size' => 2]);

        $stSql = "
            SELECT DISTINCT
                aloid AS codigo,
                alodsc AS descricao
            FROM emendas.alteracaoorcamentaria
            WHERE
                alostatus = 'A'
            ORDER BY
                descricao
        ";
        $aloid;  
        echo $simec->select("filtro[aloid][]", 'Alterações', $aloid, $stSql, NULL, ['input-size' => 9, 'label-size' => 2]);

        # Traz o Todos por default
        $benparecertecnico=$benparecertecnico?$benparecertecnico:'';
        if ($retConsultaRelatorio['valores']['benparecertecnico']){
            $parecerTecn = $retConsultaRelatorio['valores']['benparecertecnico'];
        }  
        echo $simec->radio("filtro[benparecertecnico]", 'Parecer Técnico', $benparecertecnico, ['t' => 'Sim', 'f' => 'Não', '' => 'Todos'], NULL, ['input-size' => 9, 'label-size' => 2]);

        # Traz o Todos por default
        $benparecerjuridico=$benparecerjuridico?$benparecerjuridico:'';
        echo $simec->radio("filtro[benparecerjuridico]", 'Parecer Jurídico', $benparecerjuridico, ['t' => 'Sim', 'f' => 'Não', '' => 'Todos'], NULL, ['input-size' => 9, 'label-size' => 2]);

        $stSql = "
            SELECT DISTINCT
                impid AS codigo,
                impdsc AS descricao
            FROM emendas.impedimento
            WHERE
                impstatus = 'A'
            ORDER BY
                descricao
            ";
        $impid; 
        echo $simec->select("filtro[impid][]", 'Impedimento', $impid, $stSql, NULL, ['input-size' => 9, 'label-size' => 2]);
    ?>
</div>
