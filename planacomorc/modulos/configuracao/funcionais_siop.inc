<?php

    include_once APPRAIZ . "monitora/classes/Pi_PlanoInterno.class.inc";
    
    # Inclusão de classes externas
    include_once APPRAIZ. 'wssof/classes/Ws_MomentosDto.inc';
    
    $mPtres = new Monitora_Model_Ptres();

    # Busca último momento atualizado
    $ultimoMomento = $mPtres->buscarUltimoMomentoAtualizado((int)$_SESSION['exercicio']);

    $momento = $_REQUEST['momento']? $_REQUEST['momento']: ($ultimoMomento? $ultimoMomento: 9000);

    switch ($_REQUEST['req']) {
        case 'importar-siop':
            try{
                $mPtres->importarSiop($_SESSION['exercicio'], $momento);
                $mPtres->commit();
                
                echo simec_json_encode(array(
                    'success' => 1,
                    'message' => 'As informações de Funcionais(Ações, Localizadores e POs) foram Atualizadas com sucesso!'
                ));
            } catch (Exception $e){
                echo simec_json_encode(array(
                    'success' => 0,
                    'message' => 'Não foi possível atualizar as informações de Funcionais(Ações, Localizadores e POs). O sistema não conseguiu acessar o SIOP-Sistema Integrado de Planejamento e Orçamento do Governo Federal. https://www.siop.planejamento.gov.br'
                ));
            }
            die;
        case 'atualizar-sistema':
            try{
                $mPtres->atualizarFuncionaisSiop($_SESSION['exercicio'], $momento);
                $mPtres->commit();
                
                echo simec_json_encode(array(
                    'success' => 1,
                    'message' => 'As informações de Ações e Funcionais do '. SIGLA_SISTEMA. ' foram Atualizadas com sucesso!'
                ));
            } catch (Exception $e){
                echo simec_json_encode(array(
                    'success' => 0,
                    'message' => 'Não foi possível atualizar as informações de Ações e Funcionais do '. SIGLA_SISTEMA. '. O sistema não conseguiu acessar o SIOP-Sistema Integrado de Planejamento e Orçamento do Governo Federal. https://www.siop.planejamento.gov.br'
                ));
            }
            die;
    }

    $quantidadeSiop = $mPtres->recuperarQuantidadesSiop((int)$momento, (int)$_SESSION['exercicio']);
    $listaFuncionaisSiop = $mPtres->listarFuncionaisSiop((int)$momento, (int)$_SESSION['exercicio']);
    $totalFuncionaisSistema = $mPtres->buscarTotalFuncionaisSistema((object)array('exercicio' => (int)$_SESSION['exercicio']));
    $totalFuncionaisSIOP = $mPtres->buscarTotalFuncionaisSIOP((object)array('exercicio' => (int)$_SESSION['exercicio'], 'momento' => (int)$momento));

    /**
     * Cabeçalho padrão do sistema.
     * @see cabecalho.inc
     */
    include APPRAIZ . "includes/cabecalho.inc";
?>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-12">
        <h2>Painel de Ações e Funcionais do SIOP</h2>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">

        <div class="col-md-5">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Funcionais SIOP</h5>
                </div>
                <div class="ibox-content">
                    <form name='form_importar' id='form_importar' method="POST" action="?modulo=configuracao/funcionais_siop&acao=A&req=importar-siop" class="form-horizontal">
                        <?php
                            echo $simec->select(
                                'momento',
                                'Momento',
                                $momento,
                                (new Wssof_Ws_MomentosDto())->recuperarSqlCombo(),
                                ['required']
                            );
                        ?>
                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-warning fa fa-download" type="button" id="btn-importar">&nbsp;Importar SIOP</button>
                                <button class="btn btn-success fa fa-refresh" type="button" id="btn-atualizar">&nbsp;Atualizar <?=SIGLA_SISTEMA?></button>
                            </div>
                        </div>
                    </form>
                    <div class="table-responsive">
                        <table class="table table-bordered table-hover" >
                            <thead>
                            <tr class="text-center">
                                <th>Carga</th>
                                <th>Quantidade</th>
                            </tr>
                            </thead>
                            <tbody>
                                <?php foreach($quantidadeSiop as $funcional){ ?>
                                    <tr>
                                        <td><?php echo $funcional['descricao']; ?></td>
                                        <td align="right"><?php echo $funcional['qtd']; ?></td>
                                    </tr>
                                <?php } ?>
                            </tbody>
                        </table>
                    </div>
                </div>
            </div>
        </div>

        <div class="col-md-3">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Comparativo SIOP/<?=SIGLA_SISTEMA?></h5>
                </div>
                <div class="ibox-content">

                    <div class="table-responsive">
                        <table class="table table-bordered table-hover" >
                            <thead>
                            <tr class="text-center">
                                <th>FUNCIONAIS SIOP</th>
                                <th>FUNCIONAIS <?=SIGLA_SISTEMA;?></th>
                            </tr>
                            </thead>
                            <tbody>
                            <tr>
                                <td align="right"><?=$totalFuncionaisSIOP?></td>
                                <td align="right"><?=$totalFuncionaisSistema?></td>
                            </tr>
                            </tbody>
                        </table>
                    </div>
                </div>
            </div>
        </div>

        <div class="col-md-12 div-lista-funcional-siop">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Funcionais do SIOP</h5>
                </div>
                <div class="ibox-content">
                    
                    <div class="table-responsive">
                        <table class="table table-bordered table-hover dataTables">
                            <thead>
                            <tr class="text-center">
                                <th>Id SIOP</th>
                                <th>Unidade</th>
                                <th>Função</th>
                                <th>Subfunção</th>
                                <th>Programa</th>
                                <th>Ação</th>
                                <th>Localizador</th>
                                <th>Objetivo</th>
                                <th>PO</th>
                                <th>Título Ação</th>
                                <th>Título PO</th>
                            </tr>
                            </thead>
                            <tbody>
                            <?php foreach($listaFuncionaisSiop as $funcional){ ?>
                                <tr>
                                    <td><?php echo $funcional['identificadorunico']; ?></td>
                                    <td><?php echo $funcional['codigoorgao']; ?></td>
                                    <td><?php echo $funcional['codigofuncao']; ?></td>
                                    <td><?php echo $funcional['codigosubfuncao']; ?></td>
                                    <td><?php echo $funcional['codigoprograma']; ?></td>
                                    <td><?php echo $funcional['codigoacao']; ?></td>
                                    <td><?php echo $funcional['codigolocalizador']; ?></td>
                                    <td><?php echo $funcional['codigoobjetivo']; ?></td>
                                    <td><?php echo $funcional['planoorcamentario']; ?></td>
                                    <td><?php echo $funcional['codigoorgao'].'.'. $funcional['codigofuncao']. '.'. $funcional['codigosubfuncao']. '.'. $funcional['codigoprograma']. '.'. $funcional['codigoacao']. '.'. $funcional['codigolocalizador']. '.'. (trim($funcional['codigoobjetivo'])? trim($funcional['codigoobjetivo']): '-'). '.'. $funcional['planoorcamentario']. ' - '. $funcional['acatitulo']; ?></td>
                                    <td><?php echo $funcional['plotitulo']; ?></td>
                                </tr>
                            <?php } ?>
                            </tbody>
                        </table>
                    </div>
                </div>
            </div>
        </div>

    </div>
</div>

<script>

    $(document).ready(function(){
        
        $('#momento').change(function(){
            window.document.location = '?modulo=configuracao/funcionais_siop&acao=A&momento='+ $(this).val();
        });
        
        $('#btn-importar').click(function(){
            $('.div-lista-funcional-siop').hide();
            importarSIOP();
        });
        
        $('#btn-atualizar').click(function(){
            $('.div-lista-funcional-siop').hide();
            atualizarSistema();
        });

    });

    /**
     * Atualiza os dados de Ações, Localizadores e Funcionais
     * 
     * @returns VOID Retorna janela modal com aviso pra o usuário
     */
    function importarSIOP(){
        divCarregando();
        $.ajax({
            type: 'POST',
            url: window.location.href,
            data: {
                req: 'importar-siop',
                momento: $('#momento').val()
            },
            dataType: 'json',
            success: function(resp){
                if(resp.success){
                    swal({
                            title: '',
                            text: resp.message,
                            type: 'success',
                            showCancelButton: false,
                            confirmButtonText: 'OK',
                            closeOnConfirm: true
                        },
                        function(){
                            window.location.href = window.location.href.replace('&req=importar-siop', '');
                        });
                } else {
                    swal({
                            title: 'Erro!',
                            text: resp.message,
                            type: 'error',
                            showCancelButton: false,
                            confirmButtonText: 'OK',
                            closeOnConfirm: true
                        },
                        function(){
                            window.location.href = window.location.href.replace('&req=importar-siop', '');
                        });
                }
            }
        });
    }
    
    /**
     * Atualiza os dados de Ações e Funcionais do Sistema
     * 
     * @returns VOID Retorna janela modal com aviso pra o usuário
     */
    function atualizarSistema(){
        divCarregando();
        $.ajax({
            type: 'POST',
            url: window.location.href,
            data: {
                req: 'atualizar-sistema',
                momento: $('#momento').val()
            },
            dataType: 'json',
            success: function(resp){
                if(resp.success){
                    swal({
                            title: '',
                            text: resp.message,
                            type: 'success',
                            showCancelButton: false,
                            confirmButtonText: 'OK',
                            closeOnConfirm: true
                        },
                        function(){
                            window.location.href = window.location.href.replace('&req=atualizar-sistema', '');
                        });
                } else {
                    swal({
                            title: 'Erro!',
                            text: resp.message,
                            type: 'error',
                            showCancelButton: false,
                            confirmButtonText: 'OK',
                            closeOnConfirm: true
                        },
                        function(){
                            window.location.href = window.location.href.replace('&req=atualizar-sistema', '');
                        });
                }
            }
        });
    }

</script>
