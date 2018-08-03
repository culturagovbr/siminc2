<?php
ini_set("memory_limit","1024M");
set_time_limit(0);

// Inclui componente de relatórios
include APPRAIZ. 'includes/classes/relatorio.class.inc';

// instancia a classe de relatório
$rel = new montaRelatorio();

// monta o sql, agrupador e coluna do relatório
$sql       = monta_sql();
$agrupador = monta_agp();
$coluna    = monta_col();

$dados     = $db->carregar( $sql );
$dados     = $dados ? $dados : array();

$rel->setAgrupador($agrupador, $dados);
$rel->setColuna($coluna);
$rel->setTotNivel(false);
$rel->setEspandir(false);

// Gera o XLS do relatório
if ( $_REQUEST['req'] == 'xls' ){
    ob_clean();
    $nomeDoArquivoXls = 'relatorio';
    echo $rel->getRelatorioXls();
    die();
}

?>
<!DOCTYPE html>
<html>
    <head>
        <title> Minc </title>
        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
        <link rel="stylesheet" type="text/css" href="../includes/listagem.css">
    </head>
    <body>
        <center>
            <!--  Cabeçalho Brasão -->
            <?php echo monta_cabecalho_relatorio( '100' ); ?>
        </center>

        <!--  Monta o Relatório -->
        <?php echo $rel->getRelatorio(); ?>

    </body>
</html>
<?php

function monta_sql(){
    $where = monta_where();

    $join = monta_join();
    $sql = "SELECT *
              FROM proposta.proposta pro
              ".implode(' ', $join)."
             where prostatus = 'A'". $where;
    return $sql;
}

function monta_where(){
    $where = '';

    foreach($_REQUEST['filtro'] as $key => $value){
        if (!empty($value))
            $where .= " and ".$key." = '".$value."'";
    }
    return $where;
}

function monta_agp(){
    $agrupador = $_REQUEST['agrupadorNovo'] ? $_REQUEST['agrupadorNovo'] : $_REQUEST['agrupador'];
    $agrupador = ($agrupador ? $agrupador : array());

    $agp = array("agrupador"      => array(),
                 "agrupadoColuna" => array("unonome","unosigla"));

    foreach ( $agrupador as $val ){
        switch( $val ){
            case 'unosigla':
                array_push($agp['agrupador'], array("campo" => "unosigla", "label" => "Subunidade", "type" => "string"));
            break;
            case 'eqddsc':
                array_push($agp['agrupador'], array("campo" => "eqddsc", "label" => "Enquadramento da Despesa", "type" => "string"));
            break;
            case 'irpcod':
                array_push($agp['agrupador'], array("campo" => "irpcod", "label" => "RP", "type" => "string"));
            break;
            case 'funcional':
                array_push($agp['agrupador'], array("campo" => "funcional", "label" => "Funcional", "type" => "string"));
            break;
            case 'acatitulo':
                array_push($agp['agrupador'], array("campo" => "acatitulo", "label" => "Ação", "type" => "string"));
            break;
            case 'plodsc':
                array_push($agp['agrupador'], array("campo" => "plodsc", "label" => "PO", "type" => "string"));
            break;
            case 'ndpid':
                array_push($agp['agrupador'], array("campo" => "ndpid", "label" => "Natureza de Despesa", "type" => "string"));
            break;
            case 'iducod':
                array_push($agp['agrupador'], array("campo" => "iducod", "label" => "IDUSO", "type" => "string"));
            break;
            case 'foncod':
                array_push($agp['agrupador'], array("campo" => "foncod", "label" => "Fonte", "type" => "string"));
            break;
            case 'idoid':
                array_push($agp['agrupador'], array("campo" => "idoid", "label" => "IDOC", "type" => "string"));
            break;
        }
    }
    return $agp;
}

function monta_col(){
    $coluna = $_REQUEST['colunaNovo'] ? $_REQUEST['colunaNovo'] : $_REQUEST['coluna'];
    $coluna = ($coluna ? $coluna : array());
    $col = array();

    foreach ( $coluna as $val ){
        switch( $val ){
            case 'proid':
                array_push($col,array("campo" => "proid","label" => "ID Proposta", "type"	=> "string"));
                break;
            case 'locquantidadeproposta':
                array_push($col,array("campo" => "locquantidadeproposta", "label" => "Quantidade Localizador", "type"	=> "numeric"));
                break;
            case 'prdvalor':
                array_push($col,array("campo" => "prdvalor", "label" => "Valor", "type"	=> "numeric"));
                break;
            case 'prdvalorexpansao':
                array_push($col,array("campo" => "prdvalorexpansao", "label" => "Valor Expansão", "type"	=> "numeric"));
                break;
            case 'proquantidade':
                array_push($col,array("campo" => "proquantidade", "label" => "Quantidade PO", "type"	=> "numeric"));
                break;
            case 'proquantidadeexpansao':
                array_push($col,array("campo" => "proquantidadeexpansao", "label" => "Quantidade Expansão PO", "type"	=> "numeric"));
                break;
            case 'projustificativa':
                array_push($col,array("campo" => "projustificativa", "label" => "Justificativa", "type"	=> "string"));
                break;
            case 'projustificativaexpansao':
                array_push($col,array("campo" => "projustificativaexpansao", "label" => "Justificativa Expansão", "type"	=> "string"));
                break;
        }
    }
    return $col;
}

function monta_join(){
    $coluna = $_REQUEST['colunaNovo'] ? $_REQUEST['colunaNovo'] : $_REQUEST['coluna'];
    $coluna = ($coluna ? $coluna : array());
    $join = array();
    foreach ( $coluna as $val ){
        switch( $val ){
            case 'proid':
                $novoJoin = "JOIN monitora.vw_ptres ptr ON pro.ptrid = ptr.ptrid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
                break;
            case 'locquantidadeproposta':
                $novoJoin = "JOIN monitora.vw_ptres ptr ON pro.ptrid = ptr.ptrid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
                $novoJoin = "JOIN monitora.acao aca ON ptr.acaid = aca.acaid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;
            case 'prdvalor':
                $novoJoin = "LEFT JOIN proposta.propostadetalhe prd ON prd.proid = pro.proid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;
            case 'prdvalorexpansao':
                $novoJoin = "LEFT JOIN proposta.propostadetalhe prd ON prd.proid = pro.proid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;
        }
    }
    $agrupador = $_REQUEST['agrupadorNovo'] ? $_REQUEST['agrupadorNovo'] : $_REQUEST['agrupador'];
    $agrupador = ($agrupador ? $agrupador : array());

    foreach ( $agrupador as $val ){
        switch( $val ){
            case 'unosigla':
                $novoJoin = "JOIN public.vw_subunidadeorcamentaria suo ON suo.suoid = pro.suoid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join, $novoJoin);
                }
            break;
            case 'eqddsc':
                $novoJoin = "JOIN monitora.pi_enquadramentodespesa eqd ON eqd.eqdid = pro.eqdid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join, $novoJoin);
                }
            break;
            case 'irpcod':
                $novoJoin = "JOIN monitora.vw_ptres ptr ON pro.ptrid = ptr.ptrid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join, $novoJoin);
                }
            break;
            case 'funcional':
                $novoJoin = "JOIN monitora.vw_ptres ptr ON pro.ptrid = ptr.ptrid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join, $novoJoin);
                }
            break;
            case 'acatitulo':
                $novoJoin = "JOIN monitora.vw_ptres ptr ON pro.ptrid = ptr.ptrid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join, $novoJoin);
                }
            break;
            case 'plodsc':
                $novoJoin = "JOIN monitora.vw_ptres ptr ON pro.ptrid = ptr.ptrid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join, $novoJoin);
                }
            break;
            case 'ndpid':
                $novoJoin = "LEFT JOIN proposta.propostadetalhe prd ON prd.proid = pro.proid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join, $novoJoin);
                }
            break;
            case 'iducod':
                $novoJoin = "LEFT JOIN proposta.propostadetalhe prd ON prd.proid = pro.proid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
                $novoJoin = "left join public.identifuso idu on prd.iduid = idu.iduid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join, $novoJoin);
                }
            break;
            case 'foncod':
                $novoJoin = "LEFT JOIN proposta.propostadetalhe prd ON prd.proid = pro.proid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
                $novoJoin = "left join public.fonterecurso fr on prd.fonid = fr.fonid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join, $novoJoin);
                }
            break;
            case 'idoid':
                $novoJoin = "LEFT JOIN proposta.propostadetalhe prd ON prd.proid = pro.proid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join, $novoJoin);
                }
            break;
        }
    }
    return $join;
}

function verificaJoin($join, $novoJoin){
    foreach($join as $key => $value){
        if ( $value == $novoJoin ){
            return false;
        }
    }
    return true;
}

?>