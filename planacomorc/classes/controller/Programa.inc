<?php

class Planacomorc_Controller_Programa
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/programa&acao=A';
        try {
            $mPrograma = new Planacomorc_Model_Programa($dados['prgid']);
            $mPrograma->popularDadosObjeto($dados);
            $mPrograma->prgano = $mPrograma->prgano ? $mPrograma->prgano : $_SESSION['exercicio'];
            $mPrograma->salvar(null, null);
            $mPrograma->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e) {
            $mPrograma->rollback();
            simec_redirecionar($url, 'error');
        }
    }

    public function excluir($prgid)
    {
        $url = '?modulo=apoio/programa&acao=A';
        try {
            $mPrograma = new Planacomorc_Model_Programa($prgid);
            $mPrograma->prgstatus = 'I';
            $mPrograma->salvar();
            $mPrograma->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e) {
            $mPrograma->rollback();
            simec_redirecionar($url, 'error');
        }
    }
}
