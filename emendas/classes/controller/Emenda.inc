<?php

require_once APPRAIZ . 'includes/php-excel/PHPExcel.php';

class Emendas_Controller_Emenda
{
    public function salvar($dados)
    {
        $url = '?modulo=principal/emenda_form&acao=A';

        try {
            $mEmenda = new Emendas_Model_Emenda($dados['emeid']);
            $mEmenda->popularDadosObjeto($dados);
            $mEmenda->prsano = $mEmenda->prsano ? $mEmenda->prsano : $_SESSION['exercicio'];
                
            $mEmenda->salvar(null, null, ['emejustificativa', 'emeimpositiva', ]);
            $mEmenda->commit();
            simec_redirecionar($url . '&emeid=' . $mEmenda->emeid, 'success');
        } catch (Exception $e){
            $mEmenda->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
    
    public function excluir($emeid)
    {
        $url = '?modulo=inicio&acao=C';

        try {
            $mEmenda = new Emendas_Model_Emenda($emeid);
            
            if($mEmenda->existeVinculo()){
                $mensagem = 'O registro não pode ser excluído pois possui vínculo com algum PI.';
                $url = '?modulo=apoio/modalidade-pactuacao-form&acao=A&capid=' . $mEmenda->emeid;
                simec_redirecionar($url, 'error', $mensagem);
            }            
            
            $mEmenda->emestatus = 'I';
            $mEmenda->salvar();
            $mEmenda->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mEmenda->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()

    public function importar($arquivo)
    {
        $url = '?modulo=apoio/importar_emendas&acao=A';
        $modelEmenda = new Emendas_Model_Emenda();
        try {
            if(!self::validarCarregamentoArquivo($arquivo['error'])){
                throw new Exception('Não foi possivel fazer o upload do arquivo, verifique sua conexão e tente novamente.');
            }
            
            if(!self::validarTipoArquivoXlsx($arquivo['type'])){
                throw new Exception('O arquivo da planilha precisa ser do tipo Planilha do Excel 2007(.xls).');
            }
            
            $arquivoDestino = self::copiarArquivoServidor($arquivo['tmp_name'], $arquivo['name']);
            if(!$arquivoDestino){
                throw new Exception('Não foi possível salvar o arquivo na pasta do Sistema.');
            }
            
            # Lista de emendas contida no arquivo da Planilha de importação
            $listaEmendas = self::listarEmendasArquivo($arquivoDestino);
            
            $modelEmenda->importar($listaEmendas, (int)$_SESSION['exercicio']);
            $modelEmenda->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $modelEmenda->rollback();
            simec_redirecionar($url, 'error', $e->getMessage());
        }
    }
    
    /**
     * Retorna uma lista de emendas contida no arquivo da Planilha de importação
     * 
     * @param string $arquivoDestino
     * @return array
     */
    public static function listarEmendasArquivo($arquivoDestino){
        $listaEmendas = array();
        $reader = PHPExcel_IOFactory::createReader('Excel2007');
        $reader->setReadDataOnly(true);
        $excel = $reader->load($arquivoDestino);
        $worksheet = $excel->getActiveSheet();

        $highestRow = $worksheet->getHighestRow();
        for($row = 2; $row <= $highestRow; ++$row) {
            # Retira codificação UTF8 dos textos importados
            $emenda = array_map('utf8_decode', array(
                'autor' => $worksheet->getCellByColumnAndRow(0, $row)->getValue(),
                'partido' => $worksheet->getCellByColumnAndRow(1, $row)->getValue(),
                'emenda' => $worksheet->getCellByColumnAndRow(2, $row)->getValue(),
                'esfera' => $worksheet->getCellByColumnAndRow(3, $row)->getValue(),
                'orgao' => $worksheet->getCellByColumnAndRow(4, $row)->getValue(),
                'unidadeOrcamentaria' => $worksheet->getCellByColumnAndRow(5, $row)->getValue(),
                'programa' => $worksheet->getCellByColumnAndRow(6, $row)->getValue(),
                'acao' => $worksheet->getCellByColumnAndRow(7, $row)->getValue(),
                'localizador' => $worksheet->getCellByColumnAndRow(8, $row)->getValue(),
                'fonte' => $worksheet->getCellByColumnAndRow(9, $row)->getValue(),
                'idoc' => $worksheet->getCellByColumnAndRow(10, $row)->getValue(),
                'iduso' => $worksheet->getCellByColumnAndRow(11, $row)->getValue(),
                'gnd' => $worksheet->getCellByColumnAndRow(12, $row)->getValue(),
                'modalidade' => $worksheet->getCellByColumnAndRow(13, $row)->getValue(),
                'beneficiario' => $worksheet->getCellByColumnAndRow(14, $row)->getValue(),
                'beneficiarioNome' => $worksheet->getCellByColumnAndRow(15, $row)->getValue(),
                'funcao' => $worksheet->getCellByColumnAndRow(16, $row)->getValue(),
                'subFuncao' => $worksheet->getCellByColumnAndRow(17, $row)->getValue(),
                'loa' => $worksheet->getCellByColumnAndRow(18, $row)->getValue(),
                'dotacaoAtual' => $worksheet->getCellByColumnAndRow(19, $row)->getValue(),
                'autorizado' => $worksheet->getCellByColumnAndRow(20, $row)->getValue(),
                'valorIndicado' => $worksheet->getCellByColumnAndRow(21, $row)->getValue(),
                'valorPriorizado' => $worksheet->getCellByColumnAndRow(22, $row)->getValue(),
            ));
            # Retira espaços
            $listaEmendas[] = array_map('trim', $emenda);
        }
//ver($listaEmendas, d);
        return $listaEmendas;
    }
    
    /**
     * Copia o arquivo pra pasta de arquivos do sistema.
     * 
     * @param string $origem
     * @param string $nome
     * @return string
     */
    public static function copiarArquivoServidor($origem, $nome) {
        $extensao = substr($nome, strrpos($nome, '.'));
        $pastaDestino = APPRAIZ. 'arquivos/emendas';
        $arquivoDestino = $pastaDestino. '/planilha-importacao-'. date('YmdHis'). $extensao;
        if(!(is_dir($pastaDestino) && move_uploaded_file($origem, $arquivoDestino))){
            $arquivoDestino = NULL;
        }
        
        return $arquivoDestino;
    }
    
    /**
     * Verifica se o arquivo é do tipo Xlsx.
     * 
     * @param string $tipo
     * @return boolean
     */
    public static function validarTipoArquivoXlsx($tipo){
        $valido = FALSE;
        if($tipo == 'application/vnd.openxmlformats-officedocument.spreadsheetml.sheet'){
            $valido = TRUE;
        }
        
        return $valido;
    }
    
    /**
     * Verifica se o arquivo é do tipo Xlsx.
     * 
     * @param string $status
     * @return boolean
     */
    public static function validarCarregamentoArquivo($status){
        $valido = FALSE;
        if($status == UPLOAD_ERR_OK){
            $valido = TRUE;
        }
        
        return $valido;
    }

    public function montarFormularioDetalhes($emeid = null, $emdid = null)
    {
        global $simec;
        
        $perfis = pegaPerfilGeral();
        $podeEditar = !in_array(PFL_SUBUNIDADE, $perfis)? TRUE: FALSE;
        $simec->setPodeEditar($podeEditar);
        
        $listaEmendadetalhe = (new Emendas_Model_EmendaDetalhe)->recuperarTodos('*', ['emeid = ' . (int)$emeid]);
        
        if(!$podeEditar && $listaEmendadetalhe){
            $emdid = $emdid? $emdid: $listaEmendadetalhe[0]['emdid'];
        }
        
        $mEmendadetalhe = new Emendas_Model_EmendaDetalhe($emdid);

        if(!$mEmendadetalhe->emeid){ $mEmendadetalhe->emeid = $emeid; }
        include_once APPRAIZ . 'emendas/modulos/principal/emendadetalhe_form.inc';
    }

    public function montarListagemDetalhes($emeid)
    {
        $perfis = pegaPerfilGeral();
        $podeEditar = !in_array(PFL_SUBUNIDADE, $perfis)? TRUE: FALSE;
        $mEmendaDetalhe = new Emendas_Model_EmendaDetalhe();
        $mEmendaDetalhe->emeid = $emeid;
        $aEmendadetalhe = $mEmendaDetalhe->recuperarTodosPorEmeid();
        include_once APPRAIZ . 'emendas/modulos/principal/emendadetalhe.inc';
    }

}
