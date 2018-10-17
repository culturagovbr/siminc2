<?php

    include_once APPRAIZ. "includes/library/simec/Grafico.php";

    require_once APPRAIZ. 'includes/workflow.php';

    # Lista de perfis vinculados ao usuário logado
    $perfis = pegaPerfilGeral();

    # Filtro por Subunidades do perfil do usuário logado.
    $listaSubUnidadeUsuario = buscarSubUnidadeUsuario((object) array('usucpf' => $_SESSION['usucpf']));

    # Filtro de Situação na sessão do usuário
    $filtroPainelAlteracao = $_SESSION['filtroPainelAlteracao'] = (isset($_REQUEST['esdid'])? $_REQUEST: $_SESSION['filtroPainelAlteracao']);

    /**
     * Cabeçalho padrão do sistema.
     * @see cabecalho.inc
     */
    include APPRAIZ. "includes/cabecalho.inc";
?>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <div class="col-md-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Pagina Inicial</h5>
                </div>
                <div class="ibox-content">
                    <form id="filtroalteracao" name="filtroalteracao" method="POST" role="form" class="form-horizontal">
                        <input name="requisicao" id="requisicao" value="" type="hidden">

                        <?php
//                            echo $simec->select('suoid','SubUnidade',$filtroalteracao['suoid'],(new Public_Model_SubUnidadeOrcamentaria())->queryComboChaveSuoid((object)array('unofundo' => 'FALSE','exercicio' => $_SESSION['exercicio'],'listaSubUnidadeUsuario' => $listaSubUnidadeUsuario)));
//                            echo $simec->select('eqdid', 'Enquadramento da Despesa', $filtroalteracao['eqdid'], (new Monitora_Model_PiEnquadramentoDespesa())->recuperarSqlCombo());
//                            echo '<span id="span-funcional">'.$simec->select('ptrid', 'Funcional', $filtroalteracao['ptrid'], (new Proposta_Model_Proposta())->recuperarSqlFuncional($filtroalteracao['suoid'], '')).'</span>';
                        ?>

                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-warning btn-limpar" type="button">
                                    <span class="glyphicon glyphicon-remove-circle"></span> Limpar
                                </button>
                                <button class="btn btn-primary btn-buscar" type="submit">
                                    <span class="glyphicon glyphicon-search"></span> Buscar
                                </button>
                                <button class="btn btn-success btn-novo" type="button">
                                    <span class="glyphicon glyphicon-plus"></span>
                                    Novo
                                </button>
                            </div>
                        </div>

                    </form>
                </div>
            </div>
        </div>
    </div>
    <div class="row">
        <div class="col-md-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Dados Gerais</h5>
                </div>
                <div class="ibox-content">
                    <div class="table-responsive">
                        
                        <table class="table table-striped table-bordered table-hover dataTables" >
                            <thead>
                                <tr class="text-center">
                                    <th width="4%">Ações</th>
                                    <th>Id</th>
                                    <th>Subunidade</th>
                                    <th>Funcional</th>
                                </tr>
                            </thead>
                        </table>

                    </div>
                </div>
            </div>
        </div>
    </div>
</div>

<script>
    $(document).ready(function(){

        $("#btn-exportar-xls").click(function(){
//            window.open('?modulo=inicio&acao=C&req=lista-inicio-xls&tipo='+ $("#tipo").val());
        });

    });

</script>
