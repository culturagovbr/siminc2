<?php

class Public_Controller_SubUnidadeOrcamentaria
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/unidadeOrcamentaria_form&acao=A&unoid=' . $dados['unoid'];
        try {
            $mUnidadeOrcamentaria = new Public_Model_SubUnidadeOrcamentaria($dados['suoid']);
//            $dados['suonome'] = strtoupper($dados['suonome']);
            $dados['suosigla'] = strtoupper($dados['suosigla']);
            $dados['suocodigopi'] = strtoupper($dados['suocodigopi']);
            $mUnidadeOrcamentaria->popularDadosObjeto($dados);
            $mUnidadeOrcamentaria->salvar(null, null, ['unoid','suocod', 'suonome', 'suostatus','suoflaglimiteliberado','suocodigopi' ]);
            $mUnidadeOrcamentaria->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mUnidadeOrcamentaria->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()

    public function excluir($dados)
    {
        $url = '?modulo=apoio/unidadeOrcamentaria_form&acao=A&unoid='. $dados['unoid'];
        $response = [];
        
        try {
            $mUnidadeOrcamentaria = new Public_Model_SubUnidadeOrcamentaria($dados['suoid']);

            if($mUnidadeOrcamentaria->existeVinculo()){
                $mUnidadeOrcamentaria->rollback();
                $mensagem = 'O registro não pode ser excluído pois possui vínculo com algum PI.';
//                $url = '?modulo=apoio/unidadeOrcamentaria_form&acao=A&suoid='. $dados['unoid'];
//                simec_redirecionar($url, 'error', $mensagem);
                $response['result'] = 'error';
                $response['message'] = $mensagem;
                echo simec_json_encode($response);
                die;
            }

            $mUnidadeOrcamentaria->suostatus = 'I';
            $mUnidadeOrcamentaria->salvar();
            $mUnidadeOrcamentaria->commit();
            $response['result'] = 'success';
            $response['message'] = 'Subunidade excluída com sucesso!';
            echo simec_json_encode($response);
            die;
        } catch (Exception $e){
            $mUnidadeOrcamentaria->rollback();
            $response['result'] = 'error';
            $response['message'] = 'Ocorreu um erro ao tentar excluir a Subunidade.';
            echo simec_json_encode($response);
            die;
        }
    } //end salvar()
}
