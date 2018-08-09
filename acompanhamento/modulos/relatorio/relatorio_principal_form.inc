<?php
set_time_limit(120);


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

        var formulario = document.filtro;

        selectAllOptions(formulario.agrupador);
        selectAllOptions(formulario.naoAgrupador);
        selectAllOptions(formulario.coluna);
        selectAllOptions(formulario.naoColuna);

        $("#req").val(tipo);
        if (tipo == 'xls') {
            formulario.action = '?modulo=relatorio/relatorio_principal_form&acao=A';
            window.open('', 'relatorio', 'width=800,height=550,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
            formulario.target = 'relatorio';
        } else if (tipo == 'exibir') {
            formulario.action = '?modulo=relatorio/relatorio_principal_form&acao=A';
            window.open('', 'relatorio', 'width=800,height=550,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
            formulario.target = 'relatorio';

        }

        formulario.submit();

    }
    $(document).ready(function(){
       $("#tipo_relatorio").change(function(){
           document.filtro.submit();
       });
    });
</script>