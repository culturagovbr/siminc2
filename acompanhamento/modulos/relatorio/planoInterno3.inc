<?php
set_time_limit(120);


if (isset($_REQUEST['req'])){
    switch ($_REQUEST['req']) {
        case 'exibir':
            include "geralResult.inc";
            die;
        case 'xls':
            include "geralResult.inc";
            die;            
        default:
            break;
    }
}

include APPRAIZ . 'includes/cabecalho_bootstrap_v3.inc';
//include APPRAIZ . "includes/cabecalho.inc";
include APPRAIZ . 'includes/AgrupadorBootstrap.php';

?>
<script language="javascript" type="text/javascript" src="../includes/agrupadorBootstrap.js"></script>

<form action="" method="post" name="filtro">
    <input type="hidden" name="req" id="req" value=""/>
    

    <div class="ibox-title">
        <h5>Relatório Geral</h5><br>
    </div>
    <div class="ibox-content">
        <?php
        //$simec->input('titulo', 'Título', null,"{$janela['jandsc']} ({$janela['janinicio']} - {$janela['janfim']})", ['input-size'=>9, 'label-size'=>2]); 
        ?>
        <?php
            $stSql = "SELECT prsano AS codigo, prsano AS descricao FROM emendas.emenda WHERE emestatus = 'A' ORDER BY 1;";
            $preenchidos = null;
            if(!$_REQUEST['prsano'] ){
                $preenchidos = array( array('codigo' => $_SESSION['exercicio'], 'descricao' => $_SESSION['exercicio'] ) );
                $_REQUEST['prsano'][] = $_SESSION['exercicio'];
            }else{
                if( count($_REQUEST['prsano']) > 0){
                    $preenchidos = array();
                    foreach ($_REQUEST['prsano'] AS $ano){
                        $preenchidos[] = array('codigo' => $ano, 'descricao' => $ano );
                    }
                }
            }
            echo $simec->select('filtro[e.prsano]', 'Exercício', $preenchidos, $stSql, null, ['input-size'=>9, 'label-size'=>2]);
        ?>

        <label class="col-sm-2 col-md-2 col-lg-2 control-label">Agrupadores:</label>
        <?php
            // Início dos agrupadores
            $agrupador = new AgrupadorBootstrap('filtro', '');
            // Dados padrão de destino (nulo)
            $destino = isset($agrupador2) ? $agrupador2 : array();
            // Dados padrão de origem
            $origem = array(array('codigo' => 'unonome','descricao' => 'Unidade'),
                            array('codigo' => 'unosigla','descricao' => 'Sigla da Unidade'));
            // exibe agrupador
            $agrupador->setOrigem('naoAgrupador', null, $origem);
            $agrupador->setDestino('agrupador', null, $destino);
            $agrupador->exibir();
        ?>


        <label class="col-sm-2 col-md-2 col-lg-2 control-label">Colunas</label>
        <?php
            unset($agrupador, $destino, $origem);
            // Início das colunas
            $agrupador = new AgrupadorBootstrap('filtro', '');
            $agrupador->width = 345;
            // Dados padrão de destino (nulo)
            $destino = isset($agrupador2) ? $agrupador2 : array();
            // Dados padrão de origem
            $origem = array(array('codigo' => 'unonome','descricao' => 'Unidade'),
                            array('codigo' => 'unosigla','descricao' => 'Sigla da Unidade'),
                            array('codigo' => 'unocod','descricao' => 'unocod'),
                            array('codigo' => 'prsano','descricao' => 'prsano'));
            // exibe agrupador
            $agrupador->setOrigem('naoColuna', null, $origem);
            $agrupador->setDestino('coluna', null, $destino);
            $agrupador->exibir();
        ?>
        <?php
            $strAno = "";
            $strDsc = "";

            if (count($_REQUEST['prsano']) == 1) {
                $strAno = " uno.prsano IN ('" . implode("','", $_REQUEST['prsano']) . "')  AND ";
                $strDsc = " uno.unocod || ' - ' || uno.unonome  ";
            } else {
                $strDsc = " '(Exercicio ' || uno.prsano || ') ' || uno.unocod || ' - ' || uno.unonome ";
                if (count($_REQUEST['prsano']) > 0) {
                    $strAno = " uno.prsano IN ('" . implode("','", $_REQUEST['prsano']) . "')  AND ";
                } else {
                    $strAno = " ";
                }
            }

            $stSql = "  SELECT DISTINCT uno.unoid AS codigo,
                               {$strDsc} AS descricao,
                               uno.prsano
                          FROM public.unidadeorcamentaria uno
                         INNER JOIN public.subunidadeorcamentaria suo ON suo.unoid = uno.unoid
                         WHERE {$strAno} uno.unostatus = 'A'
                        {$whereResponsabilidade}
                         ORDER BY 3,2";
            echo $simec->select("filtro[e.unoid]", 'Unidade', null, $stSql, null, ['input-size'=>9, 'label-size'=>2]);
        ?>
        <div class="text-center">
            <a class="btn-success btn btn-md" href="javascript:exibeRelatorioGeral('exibir');">Visualizar</a>
            <a class="btn-info btn btn-md" href="javascript:exibeRelatorioGeral('xls');">Visualizar XLS</a>
        </div>
    </div>
</form>
<br>
<br>
<!--<script type="text/javascript" src="../includes/prototype.js"></script>-->
<script type="text/javascript">
    function exibeRelatorioGeral(tipo) {

        var formulario = document.filtro;

        selectAllOptions(formulario.agrupador);
        selectAllOptions(formulario.coluna);

        $("#req").val(tipo);
        if (tipo == 'xls') {
            formulario.action = '?modulo=relatorio/planoInterno3&acao=A';
            window.open('', 'relatorio', 'width=800,height=550,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
            formulario.target = 'relatorio';
        } else if (tipo == 'exibir') {
            formulario.action = '?modulo=relatorio/planoInterno3&acao=A';
            window.open('', 'relatorio', 'width=800,height=550,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
            formulario.target = 'relatorio';

        }

        formulario.submit();

    }
</script>
