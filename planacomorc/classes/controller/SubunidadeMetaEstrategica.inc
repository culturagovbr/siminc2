<?php

class Planacomorc_Controller_SubunidadeMetaEstrategica
{
    public function salvar($dados)
    {
        ver($dados, d);
        $url = 'planacomorc.php?modulo=apoio/vincular-meta-ppa&acao=A';

        try {
            $mSubunidadeMetas = new Planacomorc_Model_SubunidadeMetaEstrategica();
            $mSubunidadeMetas->excluirPorExercicio($_SESSION['exercicio']);

            if (isset($dados['vinculos']) && is_array($dados['vinculos'])) {
                foreach ($dados['vinculos'] as $mppid => $vinculos) {
                    foreach ($vinculos as $suoid) {
                        $mSubunidadeMetas->mppid = $mppid;
                        $mSubunidadeMetas->suoid = $suoid;
                        $mSubunidadeMetas->salvar();
                        $mSubunidadeMetas->smpid = null;
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
