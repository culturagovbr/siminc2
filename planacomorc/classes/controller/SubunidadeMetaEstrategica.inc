<?php

class Planacomorc_Controller_SubunidadeMetaEstrategica
{
    public function salvar($dados)
    {
        $url = 'planacomorc.php?modulo=configuracao/vincular-meta-estrategica&acao=A';

        try {
            $mSubunidadeMetas = new Planacomorc_Model_SubunidadeMetaEstrategica();
            $mSubunidadeMetas->excluirPorExercicio($_SESSION['exercicio']);

            if (isset($dados['vinculos']) && is_array($dados['vinculos'])) {
                foreach ($dados['vinculos'] as $meeid => $vinculos) {
                    foreach ($vinculos as $suoid) {
                        $mSubunidadeMetas->meeid = $meeid;
                        $mSubunidadeMetas->suoid = $suoid;
                        $mSubunidadeMetas->salvar();
                        $mSubunidadeMetas->smeid = null;
                    }
                }
            }
            $mSubunidadeMetas->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e) {
            $mSubunidadeMetas->rollback();
            simec_redirecionar($url, 'error');
        }
    }

}
