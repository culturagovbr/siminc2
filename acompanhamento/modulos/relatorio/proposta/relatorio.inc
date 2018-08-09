<?php
ini_set("memory_limit","1024M");
set_time_limit(0);

// Inclui componente de relatórios
include APPRAIZ. 'includes/classes/relatorio.simple.class.inc';

// instancia a classe de relatório
$rel = new relatorioSimple();

$rel->setColunas($_REQUEST['coluna'], $_REQUEST['naoColuna']);
$rel->setAgrupadores($_REQUEST['agrupador'], $_REQUEST['naoAgrupador']);

//Lista de colunas, Agrupadores e joins
$col = array();
$col["locquantidadeproposta"] = array("campo" => "locquantidadeproposta", "alias" => "aca", "label" => "Quantidade Localizador", "type"	=> "numeric");
$col["prdvalor"] = array("campo" => "prdvalor", "alias" => "prd", "label" => "Valor", "type"	=> "numeric");
$col["prdvalorexpansao"] = array("campo" => "prdvalorexpansao", "alias" => "prd", "label" => "Valor Expansão", "type" => "numeric");
$col["proquantidade"] = array("campo" => "proquantidade", "alias" => "pro", "label" => "Quantidade PO", "type"	=> "numeric");
$col["proquantidadeexpansao"] = array("campo" => "proquantidadeexpansao", "alias" => "pro", "label" => "Quantidade Expansão PO", "type"	=> "numeric");
$col["projustificativa"] = array("campo" => "projustificativa", "alias" => "pro", "label" => "Justificativa", "type"	=> "string");
$col["projustificativaexpansao"] = array("campo" => "projustificativaexpansao", "alias" => "pro", "label" => "Justificativa Expansão", "type"	=> "string");
$agp = array();
$agp[] = array("campo" => "unosigla", "alias" => "suo", "label" => "Subunidade", "type" => "string");
$agp[] = array("campo" => "eqddsc", "alias" => "eqd", "label" => "Enquadramento da Despesa", "type" => "string");
$agp[] = array("campo" => "irpcod", "alias" => "ptr", "label" => "RP", "type" => "string");
$agp[] = array("campo" => "funcional", "alias" => "ptr", "label" => "Funcional", "type" => "string");
$agp[] = array("campo" => "acatitulo", "alias" => "ptr", "label" => "Ação", "type" => "string");
$agp[] = array("campo" => "plodsc", "alias" => "ptr", "label" => "PO", "type" => "string");
$agp[] = array("campo" => "ndpid", "alias" => "prd", "label" => "Natureza de Despesa", "type" => "string");
$agp[] = array("campo" => "iducod", "alias" => "idu", "label" => "IDUSO", "type" => "string");
$agp[] = array("campo" => "foncod", "alias" => "fr", "label" => "Fonte", "type" => "string");
$agp[] = array("campo" => "idoid", "alias" => "prd", "label" => "IDOC", "type" => "string");
$agp[] = array("campo" => "proid", "alias" => "pro", "label" => "ID Proposta", "type"	=> "string");
$join = array();
$join[] = array("campo" => "locquantidadeproposta", "join" => array("JOIN monitora.vw_ptres ptr ON pro.ptrid = ptr.ptrid",
                                                                    "JOIN monitora.acao aca ON ptr.acaid = aca.acaid"), "where" => array(" AND pro.prostatus='A'"));
$join[] = array("campo" => "prdvalor", "join" => array("LEFT JOIN proposta.propostadetalhe prd ON prd.proid = pro.proid"), "where" => array(" AND prd.prdstatus='A'",
                                                                                                                                            " AND pro.prostatus='A'"));
$join[] = array("campo" => "prdvalorexpansao", "join" => array("LEFT JOIN proposta.propostadetalhe prd ON prd.proid = pro.proid"), "where" => array(" AND prd.prdstatus='A'",
                                                                                                                                                    " AND pro.prostatus='A'"));
$join[] = array("campo" => "proquantidade", "join" => array(), "where" => array(" AND pro.prostatus='A'"));
$join[] = array("campo" => "proquantidadeexpansao", "join" => array(), "where" => array(" AND pro.prostatus='A'"));
$join[] = array("campo" => "projustificativa", "join" => array(), "where" => array(" AND pro.prostatus='A'"));
$join[] = array("campo" => "projustificativaexpansao", "join" => array(), "where" => array(" AND pro.prostatus='A'"));    
$join[] = array("campo" => "proid", "join" => array(), "where" => array(" AND pro.prostatus='A'"));    
$join[] = array("campo" => "unosigla", "join" => array("JOIN public.vw_subunidadeorcamentaria suo ON suo.suoid = pro.suoid"), "where" => array(" AND pro.prostatus='A'"));    
$join[] = array("campo" => "eqddsc", "join" => array("JOIN monitora.pi_enquadramentodespesa eqd ON eqd.eqdid = pro.eqdid"), "where" => array(" AND pro.prostatus='A'"));    
$join[] = array("campo" => "irpcod", "join" => array("JOIN monitora.vw_ptres ptr ON pro.ptrid = ptr.ptrid"), "where" => array(" AND pro.prostatus='A'"));    
$join[] = array("campo" => "funcional", "join" => array("JOIN monitora.vw_ptres ptr ON pro.ptrid = ptr.ptrid"), "where" => array(" AND pro.prostatus='A'"));    
$join[] = array("campo" => "acatitulo", "join" => array("JOIN monitora.vw_ptres ptr ON pro.ptrid = ptr.ptrid"), "where" => array(" AND pro.prostatus='A'"));    
$join[] = array("campo" => "ndpid", "join" => array("LEFT JOIN proposta.propostadetalhe prd ON prd.proid = pro.proid"), "where" => array(" AND prd.prdstatus='A'",
                                                                                                                                         " AND pro.prostatus='A'"));    
$join[] = array("campo" => "iducod", "join" => array("LEFT JOIN proposta.propostadetalhe prd ON prd.proid = pro.proid",
                                           "left join public.identifuso idu on prd.iduid = idu.iduid"), "where" => array(" AND prd.prdstatus='A'",
                                                                                                                         " AND pro.prostatus='A'"));    
$join[] = array("campo" => "foncod", "join" => array("LEFT JOIN proposta.propostadetalhe prd ON prd.proid = pro.proid",
                                                     "left join public.fonterecurso fr on prd.fonid = fr.fonid"), "where" => array(" AND pro.prostatus='A'")); 
$join[] = array("campo" => "plodsc", "join" => array("JOIN monitora.vw_ptres ptr ON pro.ptrid = ptr.ptrid"), "where" => array(" AND pro.prostatus='A'"));    
$join[] = array("campo" => "idoid", "join" => array("LEFT JOIN proposta.propostadetalhe prd ON prd.proid = pro.proid"), "where" => array(" AND prd.prdstatus='A'",
                                                                                                                                         " AND pro.prostatus='A'"));
$rel->setTabelaBase('proposta.proposta pro');
$rel->setListaColunas($col);
$rel->setListaAgrupadores($agp);
$rel->setListaJoins($join);
$rel->db=$db;
$rel->MontaSelect();
// monta o sql, agrupador e coluna do relatório
//$sql       = monta_sql();
//$agrupador = monta_agp();
//$coluna    = monta_col();
//
//$dados     = $db->carregar( $sql );
//$dados     = $dados ? $dados : array();
//
//$rel->setAgrupador($agrupador, $dados);
//$rel->setColuna($coluna);
//$rel->setTotNivel(false);
//$rel->setEspandir(false);

// Gera o XLS do relatório
if ( $_REQUEST['req'] == 'xls' ){
    ob_clean();
    $nomeDoArquivoXls = 'relatorio';
    echo $rel->getRelatorioXls();
    die();
}
$where = '';
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
        <?php echo $rel->getRelatorio(); die; ?>

    </body>
</html>
<?php

function monta_sql(){
    global $where;
    $where = monta_where();

    $join = monta_join();
    $campos = monta_colunas();
    $sql = "SELECT ".implode(', ', $campos)."
              FROM proposta.proposta pro
              ".implode(' ', $join)."
             where prostatus = 'A'". $where;
//    ver($sql,d);
    return $sql;
}