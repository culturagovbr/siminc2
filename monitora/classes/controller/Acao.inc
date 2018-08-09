<?php

class Monitora_Controller_Acao
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/acao&acao=A';

        try {
            $mAcao = new Monitora_Model_Acao($dados['acaid']);
            $mAcao->popularDadosObjeto($dados);
            $mAcao->saccod = $dados['loccod'];
            $mAcao->prgano = $_SESSION['exercicio'];

            $mAcao->salvar(null, null, ['esfcod', 'unitpocod', 'tincod', 'funcod', 'unmcod', 'regcod', 'taccod', 'osicod', 'procod', 'prgano', 'prgid', 'acacodppa', 'sitcodestagio', 'sitcodandamento', 'sitcodcronograma', 'acapercexecucao', 'acadsc', 'sacdsc', 'acadsccomentarios', 'acanomecoordenador', 'acadscunresp', 'acadscunexecutora', 'acasnmedireta', 'acasnmedesc', 'acasnmelincred', 'acasnmetanaocumulativa', 'acamesinicio', 'acaanoinicio', 'acamestermino', 'acaanotermino', 'acavlrrealateanoanterior', 'acadsccomentsituacao', 'acadscsituacaoatual', 'acadscresultadosespobt', 'acamesprevisaoconclusao', 'acaanoprevisaoconclusao', 'acadsccomentexecfisica', 'acadsccomentexecfinanceira', 'acadsccomentexecfisicabgu', 'acadsccomentexecfinanceirabgu', 'acasnrap', 'acadsccomentexecucao', 'acadsccomentexecucaorap', 'acasnfiscalseguridade', 'acasninvestatais', 'acasnoutrasfontes', 'cod_referencia', 'acadscproduto', 'acafinalidade', 'acadescricao', 'acabaselegal', 'acarepercfinanceira', 'acasnpadronizada', 'acasnsetpadronizada', 'acasntransfobrigatoria', 'acasntransfvoluntaria', 'acasntransfoutras', 'acasndespesaobrigatoria', 'acasnbloqueioprogramacao', 'acadetalhamento', 'acavlrcustototal', 'acavlrcustoateanoanterior', 'acaqtdprevistoanocorrente', 'acaordemprioridade', 'acaobs', 'acacodsof', 'acaqtdcustototal', 'acacodreferenciasof', 'acavlrrepercfinanceira', 'sfucod', 'acastatus', 'acasnemenda', 'acasnestrategica', 'acaqtdateanoanterior', 'acavlrcustoprevistoanocorrente', 'acasnbgu', 'acaptres', 'acadataatualizacao', 'irpcod', 'acatipoinclusao', 'acatipoinclusaolocalizador', 'unmdsc', 'prodsc', 'descricaomomento', 'unidaderesponsavel', 'tipoinclusao', 'tipoacao', 'inicioacao', 'terminoacao', 'ididentificadorunicosiop', 'unmcodsof', 'procodsof', 'acatipocod', 'acatipodsc', 'acainiciativacod', 'acainiciativadsc', 'acaobjetivocod', 'acaobjetivodsc', 'esfdsc', 'fundsc', 'sfundsc', 'prgdsc', 'prgtipo', 'codtipoinclusaoacao', 'codtipoinclusaolocalizador', 'metalocalizador', 'financeirolocalizador', 'acanomeunidade', 'acanomeproduto']);
            $mAcao->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mAcao->rollback();
            simec_redirecionar($url, 'error');
        }
    }

    public function excluir($acaid)
    {
        $url = '?modulo=apoio/acao&acao=A';

        try {
            $mAcao = new Monitora_Model_Acao($acaid);

            if($mAcao->existeVinculo()){
                $mensagem = 'O registro não pode ser excluído pois possui vínculo com algum PI.';
                $url = '?modulo=apoio/modalidade-pactuacao-form&acao=A&capid=' . $mAcao->acaid;
                simec_redirecionar($url, 'error', $mensagem);
            }

            $mAcao->acastatus = 'I';
            $mAcao->salvar();
            $mAcao->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mAcao->rollback();
            simec_redirecionar($url, 'error');
        }
    }
}
