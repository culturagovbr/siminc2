<?php

if(isset($_REQUEST['req'])){
    switch ($_REQUEST['req']) {
        case 'exibir':
            include $_REQUEST['tipo_relatorio']."/relatorio.inc";
            die;
        case 'xls':
            include $_REQUEST['tipo_relatorio']."/relatorio.inc";
            die;            
        default:
            break;
    }
}

/**
 * Cabeçalho padrão do sistema.
 */
include APPRAIZ . "includes/cabecalho.inc";

include APPRAIZ . 'includes/AgrupadorBootstrap.php';

?>

<style>
.tabela tr:hover {
    background: #fff9b2;
}

.tabela tr {
    line-height: 30px;
}
table.tabela {
    font-size: 12px;
}
.table-responsive {
    display: block;
    width: 100%;
    overflow-x: auto;
    -webkit-overflow-scrolling: touch;
    -ms-overflow-style: -ms-autohiding-scrollbar;
}
#page-wrapper.gray-bg {
    position: inherit;
    margin: 0 0 0 240px;
    min-height: inherit;
}
</style>

<script language="javascript" type="text/javascript" src="../includes/agrupadorBootstrap.js"></script>

<form action="" method="post" name="filtro">
    <div class="wrapper wrapper-content animated fadeInRight">
        <div class="row">
            <div class="col-md-12">
                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <h5><?php echo $titulo_modulo; ?></h5>
                    </div>
                    <div class="ibox-content noprint">
                        <?php
                            $preenchidos = $_REQUEST['tipo_relatorio']? $_REQUEST['tipo_relatorio']: array();
                            $base = array(
                                'proposta' => 'Relatório de Propostas',
                                'emendas'=> 'Relatório de Beneficiários/Emenda',
                                'pre-pi' => 'Relatório de Pré-PI',
                                'pi' => 'Relatório de PI',
                                'pi-fnc' => 'Relatório de PI do FNC',
                                'funcional' => 'Relatório de Funcionais',
                            );
                            echo $simec->select('tipo_relatorio', 'Relatório', $preenchidos, $base, null, ['input-size'=>9, 'label-size'=>2]);
                            switch ($_REQUEST['tipo_relatorio']) {
                                case 'emendas':
                                    include_once APPRAIZ.'acompanhamento/modulos/relatorio/emendas/relatorio_form.inc';
                                    break;
                                case 'proposta':
                                    include_once APPRAIZ.'acompanhamento/modulos/relatorio/proposta/relatorio_form.inc';
                                    break;        
                                default:
                                    break;
                            }
                        ?>
                    </div>
                    <div class="ibox-content">

                    </div>
                </div>
            </div>
        </div>
    </div>
</form>

<script>

    $(document).ready(function(){
        $("#tipo_relatorio").change(function(){
            document.filtro.target = "";
            $("#req").val("");
            document.filtro.submit();
        });
        $("#filtro-prsano").change(function(){
            $("#req").val("");
            document.filtro.submit();
        });
    });
    
    function exibeRelatorioGeral(tipo) {
        
        if (document.filtro.coluna.length==0){
            alert("Selecione pelo menos um campo para exibir o relatório!");
            return;
        }
        var formulario = document.filtro;

        selectAllOptions(formulario.agrupador);
        selectAllOptions(formulario.naoAgrupador);
        selectAllOptions(formulario.coluna);
        selectAllOptions(formulario.naoColuna);
        var largura = $(window).width();
        $("#req").val(tipo);
        if (tipo == 'xls') {
            formulario.action = '?modulo=relatorio/relatorio_principal_form&acao=A';
        } else if (tipo == 'exibir') {
            formulario.action = '?modulo=relatorio/relatorio_principal_form&acao=A';
            window.open('', 'relatorio', 'width='+largura+',height=750,status=1,menubar=1,toolbar=0,scrollbars=no,resizable=1');
            formulario.target = 'relatorio';

        }

        formulario.submit();
    }

</script>
