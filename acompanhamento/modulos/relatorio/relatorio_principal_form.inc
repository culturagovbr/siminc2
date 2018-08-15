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
<?php
set_time_limit(120);
//ver($_REQUEST,d);
if (isset($_REQUEST['req'])){
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


include APPRAIZ . 'includes/cabecalho_bootstrap_v3.inc';
include APPRAIZ . 'includes/AgrupadorBootstrap.php';

?>

<script language="javascript" type="text/javascript" src="../includes/agrupadorBootstrap.js"></script>
<form action="" method="post" name="filtro">
<div class="ibox-title">
    <h5>Relatório Geral</h5><br>
</div>
<div class="ibox-content">
                
<?php
    $preenchidos = $_REQUEST['tipo_relatorio']?$_REQUEST['tipo_relatorio']:array();
    $base = array('emendas'=> 'Relatório de Emendas',
                  'proposta' => 'Relatório de Propostas');
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
</form>
<br>
<br>
<!--<script type="text/javascript" src="../includes/prototype.js"></script>-->
<script type="text/javascript">
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
</script>