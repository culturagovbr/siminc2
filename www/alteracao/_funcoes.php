<?php

/**
 * Monta consulta do relatório geral de Propostas
 * 
 * @param stdClass $parametro
 * @return string
 */
function montarSqlRelatorioGeralAlteracao(stdClass $parametro){
    $where = '';

    $where .= $parametro->prsano? "\n AND pro.prsano = '". (int)$parametro->prsano. "'": NULL;
    $where .= $parametro->suoid? "\n AND suo.suoid IN(".join($parametro->suoid, ','). ")": NULL;
    $where .= $parametro->eqdid? "\n AND pro.eqdid IN(".join($parametro->eqdid, ','). ")": NULL;
    $where .= $parametro->irpcod? "\n AND ptr.irpcod::INTEGER IN(".join($parametro->irpcod, ','). ")": NULL;
    $where .= $parametro->tpdid? "\n AND eqd.tpdid IN(".join($parametro->tpdid, ','). ")": NULL;
    $where .= $parametro->suocod? "\n AND suo.suocod IN(".join($parametro->suocod, ','). ")": NULL;

    $sql = "
        SELECT
            pro.proid,
            suo.unosigla || ' - ' || suo.suonome subunidade,
            eqd.eqddsc,
	        ptr.irpcod,
            ptr.funcional,
            ptr.acatitulo,
            ptr.plodsc,
            aca.locquantidadeproposta,
            pro.proquantidade,
            pro.proquantidadeexpansao,
            pro.projustificativa,
            pro.projustificativaexpansao,
            ndp.ndpcod,
            idu.iducod,
            fr.foncod,
            prd.idoid,
            prd.prdvalor,
            prd.prdvalorexpansao,
            pro.prsano
        FROM proposta.proposta pro
            JOIN monitora.vw_ptres ptr ON pro.ptrid = ptr.ptrid
	        JOIN monitora.acao aca ON ptr.acaid = aca.acaid
            JOIN public.vw_subunidadeorcamentaria suo ON suo.suoid = pro.suoid
            JOIN monitora.pi_enquadramentodespesa eqd ON eqd.eqdid = pro.eqdid
            LEFT JOIN proposta.propostadetalhe prd ON(
		prd.proid = pro.proid
		AND prd.prdstatus = 'A'
	    )
            LEFT JOIN public.naturezadespesa ndp ON(prd.ndpid = ndp.ndpid)
            LEFT JOIN public.fonterecurso fr on prd.fonid = fr.fonid
            LEFT JOIN public.identifuso idu on prd.iduid = idu.iduid
        WHERE
            pro.prostatus = 'A'
            $where
        ORDER BY
            pro.proid,
            subunidade,
            eqd.eqddsc,
	        ptr.irpcod,
            ptr.funcional,
            ptr.acatitulo,
            ptr.plodsc
    ";
    return $sql;
}

/**
 * Monta lista de options do cadastro de Remanejamento Loa.
 *
 * @param $name   => name do option
 * @param $option => array de lista de valores
 * @return string
 */
function montaListaLoa($name, $option, $default = NULL, $rlid = '', $funcaoJS = '', $title = NULL)
{
    $attrTitle = $title? 'title="'. $title. '"': NULL;
    $select = '<select class="chosen" '. $attrTitle. ' name="'.$name.'" id="'.$name.$rlid.'" rlid="" onchange="javascript:'.$funcaoJS.'">';
    $select .='<option selected disabled></option>';
    foreach ($option as $options):
        if ($options['codigo']==$default){
            $select .='<option value="'.$options['codigo'] .'" selected>'. $options['descricao'].'</option>';
        }else{
            $select .='<option value="'.$options['codigo'] .'">'. $options['descricao'].'</option>';
        }
    endforeach;
    $select .='</select>';

    return $select;
}

function AtualizaValoresPi($pedid)
{
    $mPedido = new Alteracao_Model_Pedido();
    $listaPisSelecionados = $mPedido->listaPisSelecionados($pedid);

    foreach ($listaPisSelecionados as $pis) {
        $mPedido->atualizaValoresPI($pis['pliid'], $pis['vldotacaocusteio'], $pis['vldotacaocapital'], $pis['vldotacaofisico']);
    }
    return true;
}