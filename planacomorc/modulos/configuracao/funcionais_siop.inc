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
            $mPtres->importarSiop($_SESSION['exercicio'], $momento);
        case 'atualizar-sistema':
            $mPtres->atualizarFuncionaisSiop($_SESSION['exercicio'], $momento);
    }

    $quantidadeSiop = $mPtres->recuperarQuantidadesSiop((int)$momento, (int)$_SESSION['exercicio']);
    $listaFuncionaisSiop = $mPtres->listarFuncionaisSiop((int)$momento, (int)$_SESSION['exercicio']);
    $comparacaoSiop = $mPtres->recuperarComparacaoSiop((int)$momento, (int)$_SESSION['exercicio']);

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
                                ['required']);
                        ?>
                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-danger fa fa-download" type="submit" id="btn-importar">&nbsp;Importar SIOP</button>
                                <button class="btn btn-danger fa fa-refresh" type="button" id="btn-importar">&nbsp;Atualizar <?=SIGLA_SISTEMA?></button>
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
                                <?php foreach($comparacaoSiop as $funcional){ ?>
                                    <th><?php echo $funcional['descricao']; ?></th>
                                <?php } ?>
                            </tr>
                            </thead>
                            <tbody>
                            <tr>
                                <?php foreach($comparacaoSiop as $funcional){ ?>
                                    <td  align="right"><?php echo $funcional['qtd']; ?></td>
                                <?php } ?>
                            </tr>
                            </tbody>
                        </table>
                    </div>
                </div>
            </div>
        </div>

        <div class="col-md-12">
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

    });

</script>

