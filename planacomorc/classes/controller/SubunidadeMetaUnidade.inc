<?php

class Planacomorc_Controller_SubunidadeMetaUnidade
{
    public function salvar($dados)
    {
        $url = 'planacomorc.php?modulo=configuracao/vincular-meta-unidade&acao=A';

        try {
            $mSubunidadeMeta = new Planacomorc_Model_SubunidadeMetaUnidade();
            $mSubunidadeMeta->excluirPorExercicio($_SESSION['exercicio']);

            if (isset($dados['vinculos']) && is_array($dados['vinculos'])) {
                foreach ($dados['vinculos'] as $meuid => $vinculos) {
                    foreach ($vinculos as $suoid) {
                        $mSubunidadeMeta->meuid = $meuid;
                        $mSubunidadeMeta->suoid = $suoid;
                        $mSubunidadeMeta->salvar();
                        $mSubunidadeMeta->smuid = null;
                    }
                }
            }
            $mSubunidadeMeta->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e) {
            $mSubunidadeMeta->rollback();
            simec_redirecionar($url, 'error');
        }
    }

}
