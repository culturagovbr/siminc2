<?php

class Public_Controller_UnidadeOrcamentaria
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/unidadeOrcamentaria_form&acao=A&unoid=';
        try {
            $mUnidadeOrcamentaria = new Public_Model_UnidadeOrcamentaria($dados['unoid']);
//            $dados['unonome'] = strtoupper($dados['unonome']);
            $dados['unosigla'] = strtoupper($dados['unosigla']);
            $mUnidadeOrcamentaria->popularDadosObjeto($dados);
            $mUnidadeOrcamentaria->prsano = $mUnidadeOrcamentaria->prsano ? $mUnidadeOrcamentaria->prsano : $_SESSION['exercicio'];

            $mUnidadeOrcamentaria->salvar(null, null, ['unocod', 'prsano', 'unonome', 'unostatus','unocodigopi','unofundo','unosigla', 'unocodorcamento', ]);
            $mUnidadeOrcamentaria->commit();
            simec_redirecionar($url . $mUnidadeOrcamentaria->unoid, 'success');
        } catch (Exception $e){
            $mUnidadeOrcamentaria->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()

    public function excluir($unoid)
    {
        $url = '?modulo=apoio/unidadeOrcamentaria&acao=A';

        try {
            $mUnidadeOrcamentaria = new Public_Model_UnidadeOrcamentaria($unoid);

            if($mUnidadeOrcamentaria->existeVinculo()){
                $mensagem = 'O registro não pode ser excluído pois possui vínculo com algum PI.';
                $url = '?modulo=apoio/unidadeOrcamentaria_form&acao=A&unoid=' . $mUnidadeOrcamentaria->unoid;
                simec_redirecionar($url, 'error', $mensagem);
            }

            $mUnidadeOrcamentaria->unostatus = 'I';
            $mUnidadeOrcamentaria->salvar();
            $mUnidadeOrcamentaria->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mUnidadeOrcamentaria->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()

    public function montarFormulario($unoid = null, $suoid = null)
    {
        global $simec;

        $perfis = pegaPerfilGeral();
        /**
         * @todo colcoar a constante correta do acompanhamento para atualizar a regra.
         */
        $podeEditar = !in_array(PFL_SUBUNIDADE, $perfis)? TRUE: FALSE;
        $simec->setPodeEditar($podeEditar);

        $listaSubUnidade = (new Public_Model_SubUnidadeOrcamentaria())->recuperarTodos('*', ['unoid = ' . (int)$unoid]);

        if(!$podeEditar && $listaSubUnidade){
            $suoid = $suoid? $suoid: $listaSubUnidade[0]['suoid'];
        }

        $mSubUnidade = new Public_Model_SubUnidadeOrcamentaria($suoid);
        // ver($mSubUnidade, d);

        if(!$mSubUnidade->unoid){ $mSubUnidade->unoid = $unoid; }
        include_once APPRAIZ. 'planacomorc/modulos/apoio/SubUnidadeOrcamentaria_form.inc';
    }
    public function montarListagem($unoid)
    {
        $perfis = pegaPerfilGeral();
        /**
         * @todo colcoar a constante correta do acompanhamento para atualizar a regra.
         */
        $podeEditar = !in_array(PFL_SUBUNIDADE, $perfis)? TRUE: FALSE;

        $lista = (new Public_Model_SubUnidadeOrcamentaria)->recuperarTodos('*', ['unoid = ' . (int)$unoid, "suostatus = 'A'"]);
        include_once APPRAIZ . 'planacomorc/modulos/apoio/SubUnidadeOrcamentaria.inc';
    }
}
