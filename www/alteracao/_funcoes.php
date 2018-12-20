<?php

/**
 * Monta consulta do relatório geral de Propostas
 * 
 * @param stdClass $parametro
 * @return string
 */
function montarSqlRelatorioGeralAlteracao(stdClass $parametro){
    # Filtros
    $where = '';
    $where .= $parametro->pedano? "\n AND ped.pedano = '". (int)$parametro->pedano. "'": NULL;
    $where .= $parametro->suocod? "\n AND suo.suocod IN(".join($parametro->suocod, ','). ")": NULL;
//    $where .= $parametro->suoid? "\n AND suo.suoid IN(".join($parametro->suoid, ','). ")": NULL;
//    $where .= $parametro->eqdid? "\n AND pro.eqdid IN(".join($parametro->eqdid, ','). ")": NULL;
//    $where .= $parametro->irpcod? "\n AND ptr.irpcod::INTEGER IN(".join($parametro->irpcod, ','). ")": NULL;

    $sql = "
        SELECT DISTINCT
            ped.pedid,
            suo.unocod,
            suo.unosigla,
            suo.unonome,
            suo.suocod,
            suo.suosigla,
            suo.suonome,
            ped.pedtitulo,
            tpa.tpacod,
            esd.esddsc,
            jan.jannome,
            to_char(ped.dtalteracao, 'dd/mm/yyyy hh:mi') as data_cadastro,
            COALESCE((
                SELECT
                    SUM(COALESCE(pisvalor.vlcancelarcusteio, 0)+ COALESCE(pisvalor.vlcancelarcapital, 0)) AS cancelado
                FROM alteracao.plano_interno_selecionado pisvalor
                WHERE
                    pisvalor.pedid = ped.pedid
            ), 0) AS cancelado,
            COALESCE((
                SELECT
                    SUM(
                        COALESCE(pisvalor.vlsuplementarcusteio, 0)+
                        COALESCE(pisvalor.vlsuplementarcapital, 0)+
                        COALESCE(pisvalor.vlsuplementarexcessocusteio, 0)+
                        COALESCE(pisvalor.vlsuplementarexcessocapital, 0)+
                        COALESCE(pisvalor.vlsuplementarsuperavitcusteio, 0)+
                        COALESCE(pisvalor.vlsuplementarsuperavitcapital, 0)
                    ) AS suplementado
                FROM alteracao.plano_interno_selecionado pisvalor
                WHERE
                    pisvalor.pedid = ped.pedid
            ), 0) AS suplementado,
            pli.pliid,
            pli.funcional,
            pli.plicod,
            pli.plititulo,
            pli.pprnome as produto,
            pis.vlcusteio,
            pis.vlcapital,
            pis.vlfisico,
            pis.vlsuplementarcusteio,
            pis.vlsuplementarcapital,
            pis.vlsuplementarfisico,
            pis.vlcancelarcusteio,
            pis.vlcancelarcapital,
            pis.vlcancelarfisico,
            pis.vlsuplementarexcessocusteio,
            pis.vlsuplementarexcessocapital,
            pis.vlsuplementarexcessofisico,
            pis.vlsuplementarsuperavitcusteio,
            pis.vlsuplementarsuperavitcapital,
            pis.vlsuplementarsuperavitfisico,
            pis.vldotacaocusteio,
            pis.vldotacaocapital,
            pis.vldotacaofisico,
            rl_ptr.funcional AS loa_funcional,
            rl.ctecod,
            rl.gndcod,
            rl.mapcod,
            rl_fo.foncod,
            rl_ido.idocod,
            rl_idu.iducod,
            rl.rpcod,
            rl.vlsuplementar,
            rl.vlcancelado,
            rl.vlsuplementarexcesso,
            rl.vlsuplementarsuperavit,
            jst.jstnecessidade,
            jst.jstcausa,
            jst.jstfinanciamento,
            jst.jstfontes,
            jst.jstmedida,
            jst.jstlegislacao,
            jst.jstoutros
        FROM alteracao.pedido AS ped
            JOIN alteracao.tipo AS tpa ON ped.tpaid = tpa.tpaid
            JOIN alteracao.janela AS jan ON ped.janid = jan.janid
            LEFT JOIN alteracao.pedido_unidade pu on ped.pedid = pu.pedid
            LEFT JOIN public.vw_subunidadeorcamentaria suo ON pu.suoid = suo.suoid
            LEFT JOIN alteracao.plano_interno_selecionado AS pis ON ped.pedid = pis.pedid -- SELECT * FROM alteracao.plano_interno_selecionado
            LEFT JOIN monitora.vw_planointerno pli ON pis.pliid = pli.pliid -- SELECT * FROM monitora.vw_planointerno 
            LEFT JOIN alteracao.remanejamento_loa rl ON ped.pedid = rl.pedid -- SELECT * FROM alteracao.remanejamento_loa 
            LEFT JOIN monitora.vw_ptres rl_ptr ON rl.ptrid = rl_ptr.ptrid
            LEFT JOIN public.fonterecurso rl_fo ON rl.fonid = rl_fo.fonid
            LEFT JOIN public.idoc rl_ido ON rl.idoid = rl_ido.idoid
            LEFT JOIN public.identifuso rl_idu ON rl.iduid = rl_idu.iduid
            LEFT JOIN alteracao.justificativa jst ON ped.pedid = jst.pedid

            LEFT JOIN workflow.documento AS doc ON ped.docid = doc.docid
            LEFT JOIN workflow.estadodocumento AS esd ON esd.esdid = doc.esdid
        WHERE
            ped.pedstatus = 'A'
            AND doc.tpdid IS NOT NULL
            $where
        ORDER BY
            ped.pedid,
            suo.unocod,
            suo.unosigla,
            suo.unonome,
            suo.suocod,
            suo.suosigla,
            suo.suonome,
            ped.pedtitulo,
            tpa.tpacod,
            esd.esddsc,
            jan.jannome
    ";
//ver($sql, d);
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