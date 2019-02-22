<?php

class Spo_Controller_PtresSubunidade
{
    public function vincularPtres($dados)
    {
        try {
            $mPtresSubunidade = new Spo_Model_PtresSubunidade();
            $arrSubunidadePtrid = $mPtresSubunidade->recuperarTodos('suoid', ['ptrid='.$dados['ptrid']]);
            $this->vincularTodos($dados['ptrid'], $dados['suoid'], $arrSubunidadePtrid);
            $this->desvincularTodos($dados['ptrid'], $dados['suoid'], $arrSubunidadePtrid);
        } catch (Exception $e){
            $prefeitura->rollback();
        }
    } //end salvar()
    
    public function vincularTodos($ptrid, $suoidForm, $arrSuoid){
        $arrSuoidForm = explode(',',$suoidForm);
        //Varrendo Array que está no banco e procurando no array do form para ver se tem algum no form que não está no banco.
        foreach($arrSuoidForm as $suoidF){
            $achou=false;
            foreach($arrSuoid as $suoidA){
                if ($suoidF==$suoidA['suoid']){
                    $achou = true;
                }
            }
            if (!$achou && $suoidF!='null'){
                $mPtresSubunidade = new Spo_Model_PtresSubunidade();
                $mPtresSubunidade->ptrid = $ptrid;
                $mPtresSubunidade->suoid = $suoidF;
                $mPtresSubunidade->salvar();
                $mPtresSubunidade->commit();
            }
        }
    }  
    
    public function desvincularTodos($ptrid, $suoidForm, $arrSuoid){
        $arrSuoidForm = explode(',',$suoidForm);
        
        foreach($arrSuoid as $suoidA){
            $achou=false;
            foreach($arrSuoidForm as $suoidF){
                if ($suoidF==$suoidA['suoid']){
                    $achou = true;
                }
            }
            if (!$achou){
                $mPtresSubunidade = new Spo_Model_PtresSubunidade();
                $mPtresSubunidade->excluirVarios("ptrid = {$ptrid} and suoid = {$suoidA['suoid']}");
                $mPtresSubunidade->commit();
            }
        }
    }
}