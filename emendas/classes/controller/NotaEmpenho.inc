<?php

class Emendas_Controller_NotaEmpenho
{
    public function salvar($dados)
    {
        try {
            $mNotaEmpenho = new Emendas_Model_NotaEmpenho($dados['neid']);

            $mNotaEmpenho->benid = $dados['benid'];
            $mNotaEmpenho->nestatus = 'A';
            $mNotaEmpenho->numnotaempenho = $dados['numnotaempenho'];
            $mNotaEmpenho->salvar(null, null, []);
            $mNotaEmpenho->commit();
            return true;
        } catch (Exception $e){
            $mNotaEmpenho->rollback();
            return false;
        }
    }
    
    public function excluir($neid)
    {
        try {
            $mNotaEmpenho = new Emendas_Model_NotaEmpenho($neid);

            $mNotaEmpenho->nestatus = 'I';
            $mNotaEmpenho->salvar();
            $mNotaEmpenho->commit();
            return $mNotaEmpenho->benid;
        } catch (Exception $e){
            $mNotaEmpenho->rollback();
            return false;
        }
    }
}
