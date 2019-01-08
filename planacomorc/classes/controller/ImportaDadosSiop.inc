<?php

class Planacomorc_Controller_ImportaDadosSiop{
    
    public $filtro = '';
    public $pagina = 0;
    public $exercicio = '';
    public $htmlTabela1 = '';
    public $htmlTabela2 = '';
    public $htmlTabela3 = '';
    public $htmlTabela4 = '';
    public $boAlteracao = false;
    public $boAlteracaoCompartilhada = false;
    public $boAlteracaoProvisionado = false;
    public $boDivergenciaPlanejada = false;
    public $corpoEmailV3 = '';
    public $sqls = [];
    public $destinatarios = '';
    
    public function AtualizarDados(){
        ini_set('display_errors', 1);
        ini_set('error_reporting', E_ALL & ~E_NOTICE & ~E_DEPRECATED);
        set_time_limit(0);
        ini_set("memory_limit", "2048M");
        session_start();
        
        for($ano=2018;$ano<=2019;$ano++){
            $mImportaDadosSiop = new Planacomorc_Model_ImportaDadosSiop();
//            $this->exercicio = date('Y');
            $this->exercicio = $ano;
            $this->filtro = array('anoExercicio' => $this->exercicio);

            $mImportaDadosSiop->excluiDadosWsOrcamentario($this->exercicio);
            $this->ConsultaDadosWs();
            $mImportaDadosSiop->commit();
            $mImportaDadosSiop->excluiDadosSiopExecucao($this->exercicio);
            $mImportaDadosSiop->insereDadosSiopExecucao($this->exercicio);
            $mImportaDadosSiop->commit();
        }
    }
    
    private function ConsultaDadosWs(){
        $continuar = TRUE;
        do{
            # Busca dados de Quantitativo operação consultarExecucaoOrcamentaria do webservice do SIOP
            $service = new SiopSoapCurl_Quantitativo_Service_ExecucaoOrcamentaria();
            $service->getXml()
                ->setAnoExercicio($this->exercicio)
                ->setPagina($this->pagina)
                ->setRegistrosPorPagina(2000)
            ;
            $resultados = $service->configure()->request();

            if($resultados){
                include_once APPRAIZ. 'wssof/classes/Ws_ExecucaoOrcamentariaDto.inc';
                foreach ($resultados as $resultado){
                    $model = new Wssof_Ws_ExecucaoOrcamentariaDto();
                    $model->realizarCarga($resultado);
                    unset($model);
                }
            }

            $this->pagina++;
            if(count($resultados) < 2000){
                $continuar = FALSE;
                break;
            }
        } while ($continuar);
    }

    public function AtualizarDotacao(){
        for($ano=2018;$ano<=2019;$ano++){
//            $this->exercicio = date('Y');
            $this->exercicio = $ano;
            $mImportaDadosSiop = new Planacomorc_Model_ImportaDadosSiop();
            $listaResultadoFuncionaisSIOP = $mImportaDadosSiop->RetornaFuncionais($this->exercicio);
            $listaFuncionaisSiop = [];
            foreach($listaResultadoFuncionaisSIOP as $funcionalSIOP){
                $listaFuncionaisSiop[$funcionalSIOP['funcional']][$funcionalSIOP['categoriaeconomica']] = $funcionalSIOP['dotatual'];
            }

            $listaResultadoFuncionaisSubunidadeSiminc = $mImportaDadosSiop->RetornaDadosSiminc($this->exercicio);
            $listaFuncionaisSubunidadeSiminc = [];
            foreach($listaResultadoFuncionaisSubunidadeSiminc as $funcionalSubunidadeSiminc){
                $listaFuncionaisSubunidadeSiminc[$funcionalSubunidadeSiminc['funcional']][] = $funcionalSubunidadeSiminc;
            }
            $this->GeraTabelasDivergentes($listaFuncionaisSubunidadeSiminc, $listaFuncionaisSiop);

            $dadosProvisionado = $mImportaDadosSiop->RetornaDadosProvisionados($this->exercicio);
            $this->GeraTabelaProvisionado($dadosProvisionado);

            $listaDivergenciaPlanejada = $mImportaDadosSiop->RetornaDivergenciaPlanejado($this->exercicio);
            $this->GeraTabelaDivergenciaPlanejado($listaDivergenciaPlanejada);
            if($this->boAlteracao){$this->corpoEmailV3 .= $this->htmlTabela1;}
            if($this->boAlteracaoCompartilhada){$this->corpoEmailV3 .= $this->htmlTabela2;}
            if($this->boAlteracaoProvisionado){$this->corpoEmailV3 .= $this->htmlTabela3;}
            if($this->boDivergenciaPlanejada){$this->corpoEmailV3 .= $this->htmlTabela4;}
        }
        $this->destinatarios = $mImportaDadosSiop->RetornaEmailsSuperUsuarios();
        $this->EnviaEmail();
    }
    
    private function GeraTabelasDivergentes($listaFuncionaisSubunidadeSiminc, $listaFuncionaisSiop){
        $this->sqls = [];
        $this->htmlTabela1 = "
        <h3>Foram realizadas alterações na Dotação das seguintes unidades:</h3>
        <table border='1' width='100%' style='font-size: 10px;'>
            <thead>
            <tr>
                <th>Funcional</th>
                <th>Unidade</th>
                <th>Tipo</th>
                <th style='text-align: right;'>Vl. Antigo</th>
                <th style='text-align: right;'>Vl. Atual</th>
            </tr>   
            </thead>
            <tbody>       
        ";
        $this->htmlTabela2 = "
        <h3>As seguintes funcionais compartilhadas estão com valores divergentes:</h3>
        <style>
               /* classe mid para alterar a formatação das colunas para o meio/centro */
            .mid{
                vertical-align: middle;
        }
        </style>
        <table border='1' width='100%' style='font-size: 10px;'>
            <thead>
            <tr>
                <th>Funcional</th>
                <th>Unidade</th>
                <th>Tipo</th>
                <th style='text-align: right;'>Soma</th>
                <th style='text-align: right;'>Dotação Atual</th>
            </tr>   
            </thead>
            <tbody>";  

        foreach($listaFuncionaisSubunidadeSiminc as $codigoFuncional => $funcionalSubunidadeSiminc){

            $listaFuncionaisSiop[$codigoFuncional][3] = !empty($listaFuncionaisSiop[$codigoFuncional][3]) ? $listaFuncionaisSiop[$codigoFuncional][3] : 0;
            $listaFuncionaisSiop[$codigoFuncional][4] = !empty($listaFuncionaisSiop[$codigoFuncional][4]) ? $listaFuncionaisSiop[$codigoFuncional][4] : 0;

            if(isset($listaFuncionaisSiop[$codigoFuncional]) && count($funcionalSubunidadeSiminc) == 1){

                // Alteração do valor de Custeio
                if(isset($listaFuncionaisSiop[$codigoFuncional][3]) && $funcionalSubunidadeSiminc[0]['ptrdotacaocusteio'] != $listaFuncionaisSiop[$codigoFuncional][3]){
                    $this->htmlTabela1 .= "
                        <tr>
                            <td>{$funcionalSubunidadeSiminc[0]['funcionalptres']}</td>
                            <td>{$funcionalSubunidadeSiminc[0]['suocod']} - {$funcionalSubunidadeSiminc[0]['suosigla']}({$funcionalSubunidadeSiminc[0]['unosigla']})</td>
                            <td>3 - CUSTEIO</td>
                            <td style='text-align: right; color: red;'>" . number_format($funcionalSubunidadeSiminc[0]['ptrdotacaocusteio'], 0, ',', '.') . "</td>
                            <td style='text-align: right; color: green;'>" . number_format($listaFuncionaisSiop[$codigoFuncional][3], 0, ',', '.') . "</td>
                        </tr>
                    ";

                    $this->sqls[] = "UPDATE spo.ptressubunidade psu SET ptrdotacaocusteio = {$listaFuncionaisSiop[$codigoFuncional][3]} WHERE psuid = {$funcionalSubunidadeSiminc[0]['psuid']}";
                    $this->boAlteracao = true;
                }

                // Alteração do valor de Capital
                if(isset($listaFuncionaisSiop[$codigoFuncional][4]) && $funcionalSubunidadeSiminc[0]['ptrdotacaocapital'] != $listaFuncionaisSiop[$codigoFuncional][4]){

                    $this->htmlTabela1 .= "
                        <tr>
                            <td>{$funcionalSubunidadeSiminc[0]['funcionalptres']}</td>
                            <td>{$funcionalSubunidadeSiminc[0]['suocod']} - {$funcionalSubunidadeSiminc[0]['suosigla']}({$funcionalSubunidadeSiminc[0]['unosigla']})</td>
                            <td>4 - CAPITAL</td>
                            <td style='text-align: right; color: red;'>" . number_format($funcionalSubunidadeSiminc[0]['ptrdotacaocapital'], 0, ',', '.') . "</td>
                            <td style='text-align: right; color: green;'>" . number_format($listaFuncionaisSiop[$codigoFuncional][4], 0, ',', '.') . "</td>
                        </tr>
                    ";

                    $this->sqls[] = "UPDATE spo.ptressubunidade psu SET ptrdotacaocapital = {$listaFuncionaisSiop[$codigoFuncional][4]} WHERE psuid = {$funcionalSubunidadeSiminc[0]['psuid']}";
                    $this->boAlteracao = true;
                }
            } elseif(count($funcionalSubunidadeSiminc) > 1){
                # Busca o código da UO(unicod) no número da funcional
                $unicod = substr($codigoFuncional, 15, 22);
                # Verifica se a UO é o Fundo Nacional de Cultura-FNC
                if($unicod == UNICOD_FNC){
                    # Alteração do valor de Custeio
                    if(isset($listaFuncionaisSiop[$codigoFuncional][3]) && $funcionalSubunidadeSiminc[0]['ptrdotacaocusteio'] != $listaFuncionaisSiop[$codigoFuncional][3]){
                        $this->htmlTabela1 .= "
                            <tr>
                                <td>{$funcionalSubunidadeSiminc[0]['funcionalptres']}</td>
                                <td>{$funcionalSubunidadeSiminc[0]['unocod']} - {$funcionalSubunidadeSiminc[0]['unonome']}({$funcionalSubunidadeSiminc[0]['unosigla']})</td>
                                <td>3 - CUSTEIO</td>
                                <td style='text-align: right; color: red;'>" . number_format($funcionalSubunidadeSiminc[0]['ptrdotacaocusteio'], 0, ',', '.') . "</td>
                                <td style='text-align: right; color: green;'>" . number_format($listaFuncionaisSiop[$codigoFuncional][3], 0, ',', '.') . "</td>
                            </tr>
                        ";

                        # Atualiza a dotação de todas as Subunidades do FNC
                        foreach($funcionalSubunidadeSiminc as $subunidade){
                            $this->sqls[] = "UPDATE spo.ptressubunidade psu SET ptrdotacaocusteio = {$listaFuncionaisSiop[$codigoFuncional][3]} WHERE psuid = {$subunidade['psuid']}";
                        }
                        # Atualiza a dotação de Custeio da Funcional do FNC
                        $this->sqls[] = "UPDATE monitora.ptres SET ptrdotacaocusteio = {$listaFuncionaisSiop[$codigoFuncional][3]} WHERE ptrid = {$funcionalSubunidadeSiminc[0]['ptrid']}";
                        $this->boAlteracao = true;
                    }

                    # Alteração do valor de Capital
                    if(isset($listaFuncionaisSiop[$codigoFuncional][4]) && $funcionalSubunidadeSiminc[0]['ptrdotacaocapital'] != $listaFuncionaisSiop[$codigoFuncional][4]){

                        $this->htmlTabela1 .= "
                            <tr>
                                <td>{$funcionalSubunidadeSiminc[0]['funcionalptres']}</td>
                                <td>{$funcionalSubunidadeSiminc[0]['unocod']} - {$funcionalSubunidadeSiminc[0]['unonome']}({$funcionalSubunidadeSiminc[0]['unosigla']})</td>
                                <td>4 - CAPITAL</td>
                                <td style='text-align: right; color: red;'>" . number_format($funcionalSubunidadeSiminc[0]['ptrdotacaocapital'], 0, ',', '.') . "</td>
                                <td style='text-align: right; color: green;'>" . number_format($listaFuncionaisSiop[$codigoFuncional][4], 0, ',', '.') . "</td>
                            </tr>
                        ";

                        # Atualiza a dotação de todas as Subunidades do FNC
                        foreach($funcionalSubunidadeSiminc as $subunidade){
                            $this->sqls[] = "UPDATE spo.ptressubunidade psu SET ptrdotacaocapital = {$listaFuncionaisSiop[$codigoFuncional][4]} WHERE psuid = {$subunidade['psuid']}";
                        }
                        # Atualiza a dotação de Capital da Funcional do FNC
                        $this->sqls[] = "UPDATE monitora.ptres SET ptrdotacaocapital = {$listaFuncionaisSiop[$codigoFuncional][4]} WHERE ptrid = {$funcionalSubunidadeSiminc[0]['ptrid']}";
                        $this->boAlteracao = true;
                    }
                } else {
                    $total[3] = $total[4] = 0;
                    $unidades[3] = $unidades[4] = NULL;
                    foreach($funcionalSubunidadeSiminc as $subunidade){
                        $total[3] += $subunidade['ptrdotacaocusteio'];
                        $total[4] += $subunidade['ptrdotacaocapital'];

                        $unidades[3][] .= "{$subunidade['suocod']} - {$subunidade['suosigla']}({$subunidade['unosigla']}) (R$ " . number_format($subunidade['ptrdotacaocusteio'], 0, ',', '.') . ")";
                        $unidades[4][] .= "{$subunidade['suocod']} - {$subunidade['suosigla']}({$subunidade['unosigla']}) (R$ " . number_format($subunidade['ptrdotacaocapital'], 0, ',', '.') . ")";
                    }

                    if(isset($listaFuncionaisSiop[$codigoFuncional][3]) && $total[3] != $listaFuncionaisSiop[$codigoFuncional][3]){
                        $this->htmlTabela2 .= "
                                <tr>
                                    <td class='mid'>{$funcionalSubunidadeSiminc[0]['funcionalptres']}</td>
                                    <td>" . implode('<hr />', $unidades[3]) . "</td>
                                    <td class='mid'>3 - CUSTEIO</td>
                                    <td class='mid' style='text-align: right; color: red;'>" . number_format($total[3], 0, ',', '.') . "</td>
                                    <td class='mid' style='text-align: right; color: green;'>" . number_format($listaFuncionaisSiop[$codigoFuncional][3], 0, ',', '.') . "</td>
                                </tr>
                            ";
                        $this->boAlteracaoCompartilhada = true;
                    }

                    if(isset($listaFuncionaisSiop[$codigoFuncional][4]) && $total[4] != $listaFuncionaisSiop[$codigoFuncional][4]){
                        $this->htmlTabela2 .= "
                                <tr>
                                    <td>{$funcionalSubunidadeSiminc[0]['funcionalptres']}</td>
                                    <td>" . implode('<hr />', $unidades[4]) . "</td>
                                    <td>4 - CAPITAL</td>
                                    <td style='text-align: right; color: red;'>" . number_format($total[4], 0, ',', '.') . "</td>
                                    <td style='text-align: right; color: green;'>" . number_format($listaFuncionaisSiop[$codigoFuncional][4], 0, ',', '.') . "</td>
                                </tr>
                            ";
                        $this->boAlteracaoCompartilhada = true;
                    }
                }
            }
        }
        $this->htmlTabela1 .= "</tbody></table>";
        $this->htmlTabela2 .= "</tbody></table>";        
    }
    
    private function GeraTabelaProvisionado($dadosProvisionado){
        $this->htmlTabela3 = "
        <h3>Lista de PIs com valor Provisionado maior do que o valor Previsto:</h3>
        <style>
               /* classe mid para alterar a formatação das colunas para o meio/centro */
            .mid{
                vertical-align: middle;
        }
        </style>
        <table border='1' width='100%' style='font-size: 10px;'>
            <thead>
            <tr>
                <th>Funcional</th>
                <th style='width: 75px'>Unidade</th>
                <th>PI</th>
                <th>Título</th>
                <th style='text-align: right; color: green;'>Previsto</th>
                <th style='text-align: right; color: red;'>Provisionado</th>
                <th style='text-align: right;'>Empenhado</th>
                <th style='text-align: right;'>Liquidado</th>
                <th style='text-align: right;'>Pago</th>
            </tr>   
            </thead>
            <tbody>";

        foreach($dadosProvisionado as $dado){
            $this->htmlTabela3 .= "
                <tr>
                    <td class='mid'>{$dado['funcional']}</td>
                    <td class='mid'>{$dado['suocod']} - {$dado['suosigla']}({$dado['unosigla']})</td>
                    <td class='mid'>{$dado['plicod']}</td>
                    <td class='mid'>{$dado['plititulo']}</td>
                    <td style='text-align: right; color: green;'>". number_format($dado['previsto'], 0, ',', '.') . "</td>
                    <td style='text-align: right; color: red;'>". number_format($dado['autorizado'], 0, ',', '.') . "</td>
                    <td style='text-align: right;'>". number_format($dado['empenhado'], 0, ',', '.') . "</td>
                    <td style='text-align: right;'>". number_format($dado['liquidado'], 0, ',', '.') . "</td>
                    <td style='text-align: right;'>". number_format($dado['pago'], 0, ',', '.') . "</td>
                </tr>
            ";
            $this->boAlteracaoProvisionado = true;
        }     
        $this->htmlTabela3 .= "</tbody></table>";
    }
    
    private function GeraTabelaDivergenciaPlanejado($listaDivergenciaPlanejada){
        $this->htmlTabela4 = "
            <h3>Valor sem PI por Funcional:</h3>
            <style>
                /* classe mid para alterar a formatação das colunas para o meio/centro */
                .mid {
                    vertical-align: middle;
                }
            </style>
            <table border='1' width='100%' style='font-size: 10px;'>
                <thead>
                <tr>
                    <th>PTRES</th>
                    <th>Funcional</th>
                    <th>Unidade</th>
                    <th style='text-align: right;'>Empenhado</th>
                    <th style='text-align: right;'>Liquidado</th>
                    <th style='text-align: right;'>Pago</th>
                </tr>
                </thead>
                <tbody>";

        foreach($listaDivergenciaPlanejada as $dado){
            $this->htmlTabela4 .= "
                <tr>
                    <td class='mid'>{$dado['ptres']}</td>
                    <td class='mid'>{$dado['funcional']}</td>
                    <td class='mid'>{$dado['subunidade']}</td>
                    <td style='text-align: right;'>". number_format($dado['empenhado'], 0, ',', '.') . "</td>
                    <td style='text-align: right;'>". number_format($dado['liquidado'], 0, ',', '.') . "</td>
                    <td style='text-align: right;'>". number_format($dado['pago'], 0, ',', '.') . "</td>
                </tr>
            ";
            $this->boDivergenciaPlanejada = true;
        }  
        $this->htmlTabela4 .= "</tbody></table>";
    }
    
    private function EnviaEmail(){
        $mImportaDadosSiop = new Planacomorc_Model_ImportaDadosSiop();
        if($this->corpoEmailV3){
            $this->corpoEmailV3 = "<h2 style='color: red; text-align: center'>Relatório de Alterações e Divergências</h2>" . $this->corpoEmailV3;
            if(count($this->sqls)){
                $this->sqls = implode('; ', $this->sqls);
                $mImportaDadosSiop->executar($this->sqls);
                $mImportaDadosSiop->commit();
            }
            $corpoEmailV3 = $this->corpoEmailV3;
            include_once APPRAIZ. "includes/email-template.php";
            $remetente = '';
            $assunto = '[SIMINC2] Alterações de Dotação';
            $conteudo = $textoEmailV3;

            simec_email($remetente, $this->destinatarios, $assunto, $conteudo);
        }
    }

}
