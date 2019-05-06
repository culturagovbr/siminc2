<?php

class Proposta_Controller_PrePlanoInterno
{
    public function salvar($dados)
    {
        $url = '?modulo=principal/preplanointerno_form&acao=A';

        try {
            $mPrePlanoInterno = new Proposta_Model_PrePlanoInterno($dados['pliid']);
            $mPrePlanoInterno->popularDadosObjeto($dados);

            $camposNumericos = ['pliquantidade', 'plivalorcusteio', 'plivalorcapital', 'plivalorcusteioadicional', 'plivalorcapitaladicional', 'pliquantidadeadicional'];
            foreach ($camposNumericos as $campo){
                $mPrePlanoInterno->{$campo} = $mPrePlanoInterno->{$campo} ? str_replace(array('.', ','), array('', '.'), $mPrePlanoInterno->{$campo}) : 0;
            }

            $mPrePlanoInterno->prsano = $mPrePlanoInterno->prsano ? $mPrePlanoInterno->prsano : $_SESSION['exercicio'];
//ver($dados, d);
            $mPrePlanoInterno->salvar(null, null, ['esfid', 'plidsc', 'maiid', 'masid', 'oppid', 'mppid', 'ippid', 'mpnid', 'ipnid', 'pprid', 'arceid', 'secid', 'pumid', 'pliquantidade', 'plivalorcusteio', 'plivalorcapital', 'docid', 'plivalorcusteioadicional', 'plivalorcapitaladicional', 'pliquantidadeadicional', 'plijustificativaadicional']);


            // Salvando dados da Localização
            $dados['pliid'] = $mPrePlanoInterno->pliid;
            (new Proposta_Controller_PrePlanoInternoLocalizacao())->salvar($dados);

            $mPrePlanoInterno->commit();
            simec_redirecionar($url . '&pliid=' . $mPrePlanoInterno->pliid, 'success');
        } catch (Exception $e){
            $mPrePlanoInterno->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()

    public function excluir($pliid)
    {
        $url = '?modulo=principal/preplanointerno&acao=A';

        try {
            $mPrePlanoInterno = new Proposta_Model_PrePlanoInterno($pliid);

            if($mPrePlanoInterno->existeVinculo()){
                $mensagem = 'O registro não pode ser excluído pois possui vínculo com algum PI.';
                $url = '?modulo=apoio/modalidade-pactuacao-form&acao=A&capid=' . $mPrePlanoInterno->pliid;
                simec_redirecionar($url, 'error', $mensagem);
            }

            $mPrePlanoInterno->plistatus = 'I';
            $mPrePlanoInterno->salvar();
            $mPrePlanoInterno->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mPrePlanoInterno->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
}            