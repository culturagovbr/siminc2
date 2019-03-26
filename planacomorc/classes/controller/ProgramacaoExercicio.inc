<?php

class Planacomorc_Controller_ProgramacaoExercicio
{
    protected $schema = 'planacomorc';

    public function salvar($dados)
    {
        $url = '?modulo=sistema/exercicio/admexerc&acao=A';
        try {
            $mExercicio = new Spo_Model_Programacaoexercicio($this->schema, $dados['prsano']);
            $mExercicio->popularDadosObjeto($dados);
            $mExercicio->prsano = $dados['prsano'];
            $mExercicio->prsdata_inicial = formata_data_sql($dados['prsdata_inicial']);
            $mExercicio->prsdata_termino = formata_data_sql($dados['prsdata_termino']);
            $mExercicio->prsexerccorrente = $dados['prsexerccorrente'];
            $mExercicio->salvar();
            $mExercicio->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e) {
            $mExercicio->rollback();
            simec_redirecionar($url, 'error');
        }
    }

    public function excluir($prsano)
    {
        $url = '?modulo=sistema/exercicio/admexerc&acao=A';
        try {
            $mExercicio = new Spo_Model_Programacaoexercicio($this->schema, $prsano);
            $mExercicio->prsstatus = 'I';
            $mExercicio->salvar();
            $mExercicio->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e) {
            $mExercicio->rollback();
            simec_redirecionar($url, 'error');
        }
    }

}
