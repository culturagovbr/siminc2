<?php

class Planacomorc_Controller_UnidadegestoraLimite
{
    public function salvar($dados)
    {
        try {
            $mUnidadegestoralimite = new Planacomorc_Model_UnidadegestoraLimite($dados['lmuid']);

            $dados['lmudtcadastro'] = date('Y-m-d H:i:s');
            $dados['usucpf'] = $_SESSION['usucpforigem'];
            $dados['lmustatus'] = 'A';

            $mUnidadegestoralimite->popularDadosObjeto($dados);
            $mUnidadegestoralimite->prsano = $mUnidadegestoralimite->prsano ? $mUnidadegestoralimite->prsano : $_SESSION['exercicio'];

            $mUnidadegestoralimite->salvar(null, null, ['lmuvlr', 'lmudtcadastro', 'usucpf', 'lmuflgliberado', ]);
            $mUnidadegestoralimite->commit();
            return true;
        } catch (Exception $e){
            $mUnidadegestoralimite->rollback();
            return false;
        }
    }
}