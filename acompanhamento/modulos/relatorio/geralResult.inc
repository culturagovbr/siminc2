<?php
ini_set("memory_limit","1024M");
set_time_limit(0);

// Inclui componente de relatórios
include APPRAIZ. 'includes/classes/relatorio.class.inc';

// instancia a classe de relatório
$rel = new montaRelatorio();

$monstrarCabecalho = true;

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
            <?php echo monta_cabecalho_relatorio( '100',$monstrarCabecalho ); ?>
        </center>

        <!--  Monta o Relatório -->
        <?php echo $rel->getRelatorio(); ?>

    </body>
</html>
<?php
function monta_sql(){
    $where = array();
    global $db;

    $join = monta_join();
    $sql = "SELECT *
              FROM emendas.emenda e
              ".implode(' ', $join)."
             where e.emestatus = 'A'". monta_where();
    return $sql;
}

function monta_where(){
    $where = '';
    foreach($_REQUEST['filtro'] as $key=>$value){
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
            case "unonome":
                array_push($agp['agrupador'], array("campo" => "unonome","label" => "Unidade","type" => "string"));
            break;
            case "unosigla":
                array_push($agp['agrupador'], array("campo" => "unosigla","label" => "Sigla","type" => "string"));
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
            case "unonome":
                array_push($col,array("campo" => "unonome","label" => "Unidade","type"	=> "string"));
            break;
            case "unosigla":
                array_push($col, array("campo" => "unosigla","label" => "Sigla","type"=> "string"));
            break;
            case "unocod":
                array_push($col, array("campo" => "unocod","label" => "unocod","type"=> "string"));
            break;
            case "prsano":
                array_push($col, array("campo" => "prsano","label" => "prsano","type"=> "string"));
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
            case "unonome":
                $novoJoin = "inner join public.unidadeorcamentaria uo on e.unoid = uo.unoid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;
            case "unosigla":
                $novoJoin = "inner join public.unidadeorcamentaria uo on e.unoid = uo.unoid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;
            case "unocod":
                $novoJoin = "inner join public.unidadeorcamentaria uo on e.unoid = uo.unoid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;
            case "prsano":
                $novoJoin = "inner join public.unidadeorcamentaria uo on e.unoid = uo.unoid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;
        }
    }    
    return $join;
}

function verificaJoin($join, $novoJoin){
    foreach($join as $key=> $value){
        if ($value==$novoJoin){
            return false;
        }
    }
    return true;
}

?>