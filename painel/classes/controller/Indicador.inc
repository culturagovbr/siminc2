<?php

class Painel_Controller_Indicador
{
    public function salvar($dados)
    {
        try {
            $mIndicador = new Painel_Model_Indicador($dados['subid']);
            $mIndicador->popularDadosObjeto($dados);

            $mIndicador->subdatainicio = $mIndicador->subdatainicio ? formata_data_sql($mIndicador->subdatainicio) : null;
            $mIndicador->subdatafim = $mIndicador->subdatafim ? formata_data_sql($mIndicador->subdatafim) : null;

            $mIndicador->salvar(null, null, ['subdatainicio', 'subdatafim', 'subtexto']);
            $mIndicador->commit();
            return true;
        } catch (Exception $e){
            $mIndicador->rollback();
            return false;
        }
    } //end salvar()

    public function excluir($subid)
    {
        try {
            $mIndicador = new Painel_Model_Indicador($subid);

            if($mIndicador->existeVinculo()){
                $mensagem = 'O registro não pode ser excluído pois possui vínculo com outro registro.';
                return false;
            }

            $mIndicador->substatus = 'I';
            $mIndicador->salvar();
            $mIndicador->commit();
            return true;
        } catch (Exception $e){
            $mIndicador->rollback();
            return false;
        }
    } //end salvar()
}