<?php

class Acompanhamento_Controller_ConsultaRelatorio
{
    public function salvar($dados)
    {
        $url = '?modulo=relatorio/relatorio_principal_form&acao=A';

        try {
            $mConsultaRelatorio = new Acompanhamento_Model_ConsultaRelatorio($dados['crid']);
            $mConsultaRelatorioFiltrosValor = new Acompanhamento_Model_ConsultaRelatorioFiltrosValor();
            $mConsultaRelatorioFiltros = new Acompanhamento_Model_ConsultaRelatorioFiltros();
            $mConsultaRelatorioColunas = new Acompanhamento_Model_ConsultaRelatorioColunas();
            $mConsultaRelatorio->popularDadosObjeto($dados);
            $mConsultaRelatorio->usucpf = $_SESSION['usucpf'];
            $mConsultaRelatorio->tprel = $dados['tipo_relatorio'];
            $mConsultaRelatorio->salvar(null,null);
            if ($dados['crid']){
                $mConsultaRelatorioFiltrosValor->removeFiltrosValores($dados['crid']);            
                $mConsultaRelatorioFiltros->removeFiltros($dados['crid']);            
                $mConsultaRelatorioColunas->removeColunas($dados['crid']);
            }
            
            $this->salvarColunas($mConsultaRelatorio->crid, $dados['coluna']);
            $this->salvarFiltros($mConsultaRelatorio->crid, $dados['filtro']);
            $mConsultaRelatorio->commit();
            simec_redirecionar($url . '&crid='.$mConsultaRelatorio->crid, 'success');
        } catch (Exception $e){
            $mConsultaRelatorio->rollback();
            simec_redirecionar($url, 'error');
        }
    }
    
    private function salvarColunas($crid, $colunas){
        foreach($colunas as $coluna){
            $mConsultaRelatorioColunas = new Acompanhamento_Model_ConsultaRelatorioColunas();
            $mConsultaRelatorioColunas->crid = $crid;            
            $mConsultaRelatorioColunas->dscampo=$coluna;
            $mConsultaRelatorioColunas->salvar(null,null);
        }
    }
    
    private function salvarFiltros($crid, $filtros){
        foreach($filtros as $key=>$value){
            $mConsultaRelatorioFiltros = new Acompanhamento_Model_ConsultaRelatorioFiltros();
            $mConsultaRelatorioFiltros->crid = $crid;            
            $mConsultaRelatorioFiltros->dscampo=$key;
            $mConsultaRelatorioFiltros->salvar(null,null);
            $this->salvarValoresFiltros($mConsultaRelatorioFiltros->crfid, $value);
        }
    }
    
    private function salvarValoresFiltros($crfid, $valores){
        if (is_array($valores)){
            foreach($valores as $valor){
                $mConsultaRelatorioFiltrosValor = new Acompanhamento_Model_ConsultaRelatorioFiltrosValor();
                $mConsultaRelatorioFiltrosValor->crfid = $crfid;
                $mConsultaRelatorioFiltrosValor->vlcampo = $valor;
                $mConsultaRelatorioFiltrosValor->salvar(null, null);
            }            
        }else{
            $mConsultaRelatorioFiltrosValor = new Acompanhamento_Model_ConsultaRelatorioFiltrosValor();
            $mConsultaRelatorioFiltrosValor->crfid = $crfid;            
            $mConsultaRelatorioFiltrosValor->vlcampo = $valores;
            $mConsultaRelatorioFiltrosValor->salvar(null, null);                
        }        
        
    }
    
    public function consultarRelatorio($crid){
        $mConsultaRelatorio = new Acompanhamento_Model_ConsultaRelatorio();
        $mConsultaRelatorioColunas = new Acompanhamento_Model_ConsultaRelatorioColunas();
        $mConsultaRelatorioFiltros = new Acompanhamento_Model_ConsultaRelatorioFiltros();
        $mConsultaRelatorioFiltrosValor = new Acompanhamento_Model_ConsultaRelatorioFiltrosValor();
        $arrRetorno['consulta'] = $mConsultaRelatorio->selecionaConsulta($crid);
        $arrRetorno['colunas'] = $mConsultaRelatorioColunas->SelecionaColunas($crid);
        $arrRetorno['filtros'] = $mConsultaRelatorioFiltros->selecionaFiltros($crid);
        $arrRetorno['valores'] = $mConsultaRelatorioFiltrosValor->selecionaValores($crid);
        return $arrRetorno;
    }
    
    public function excluirRelatorio($crid){
        $url = '?modulo=relatorio/relatorio_principal_form&acao=A';

        try {
            $mConsultaRelatorio = new Acompanhamento_Model_ConsultaRelatorio($crid);
            $mConsultaRelatorioFiltrosValor = new Acompanhamento_Model_ConsultaRelatorioFiltrosValor();
            $mConsultaRelatorioFiltros = new Acompanhamento_Model_ConsultaRelatorioFiltros();
            $mConsultaRelatorioColunas = new Acompanhamento_Model_ConsultaRelatorioColunas();
            $mConsultaRelatorioFiltrosValor->removeFiltrosValores($crid);            
            $mConsultaRelatorioFiltros->removeFiltros($crid);            
            $mConsultaRelatorioColunas->removeColunas($crid);
            $mConsultaRelatorio->excluir();
            $mConsultaRelatorio->commit();
            simec_redirecionar($url , 'success');
        } catch (Exception $e){
            $mConsultaRelatorio->rollback();
            simec_redirecionar($url, 'error');
        }
    }

}
