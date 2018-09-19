<?php

    # Incluindo arquivo do Módulo que possui a implementação da busca
    include_once APPRAIZ. 'www/proposta/_funcoes.php';
    include_once APPRAIZ. 'includes/relatorio-agrupador/tabela.php';

    /**
     * Busca a descricao da coluna pelo nome interno/Código
     * 
     * @param string $codigo
     * @param array $listaColunas
     * @return string
     */
//    function buscarDescricaoPorCodigo($codigo, $listaColunas){
//        $resultado = '';
//        if($listaColunas){
//            foreach($listaColunas as $coluna){
//                if($codigo == $coluna['codigo']){
//                    $resultado = $coluna['descricao'];
//                    break;
//                }
//            }
//        }
//        
//        return $resultado;
//    }
    
    # Filtros do relatório
    $listaFiltros = $_POST['filtro'];
    
//ver($colunasPadroes, $colunasSelecionadas, d);
    # Busca lista de perfis do usuário
    $listaPerfil = pegaPerfilGeral();
    
    # Filtros da pesquisa
    $filtro = array('exercicio' => $_SESSION['exercicio']);
    
    if((in_array(PFL_CONSULTA_UNIDADE,$listaPerfil)) || in_array(PFL_SUBUNIDADE, $listaPerfil) && ($listaSubUnidadeUsuario)){
        # Busca lista de subunidades vinculadas ao perfil do usuário
        $listaResultado = buscarSubUnidadeUsuario((object) array('usucpf' => $_SESSION['usucpf']));
        $listaSubUnidadeUsuario = implode("', '", $listaResultado);
        if($listaSubUnidadeUsuario){
            $filtro['suocod'] = $listaSubUnidadeUsuario;
        }
        
        # Busca SQL com filtros pra realizar a consulta
        $sql = montarSqlRelatorioGeralPrePi((object) $filtro);
    }else if (in_array(PFL_ADMINISTRADOR, $listaPerfil) || in_array(PFL_SUPERUSUARIO, $listaPerfil)){
        $sql = montarSqlRelatorioGeralPrePi((object) $filtro);                
    }
    
    # Busca lista pra montar a tabela do relatório
    $listaRelatorio = $db->carregar($sql);
    
    # Gera o XLS do relatório
//    if($_REQUEST['req'] == 'xls'){
//        ob_clean();
//        $nomeDoArquivoXls = 'relatorio';
//        echo $rel->getRelatorioXls();
//        die();
//    }

    include APPRAIZ. "includes/cabecalho_bootstrap_v3_popup.inc";
?>

<div class="row" style="margin-top: 5px;">
    <div class="col-md-12">
        <div class="ibox float-e-margins">
            <div class="ibox-title">
                <h5>Relatório Geral de Pré-PI</h5>
            </div>
            <div class="ibox-content">
                <div class="table-responsive" style="overflow-y: auto;">

                    <?php

                        # Colunas do relatorio
                        $listaTodasColunas = $_SESSION['relatorio_gerencial_pre_pi']['listaTodasColunas'];
                        $listaColunasSelecionadas = $_POST['coluna'];
                        $listaColunaFormatoMoeda = [
                            'plivalorcusteio',
                            'plivalorcapital',
                            'plivalorcusteioadicional',
                            'plivalorcapitaladicional'
                        ];

                        $tabela = new relatorio_agrupador_tabela();
                        echo $tabela->setListaTodasColunas($listaTodasColunas)
                            ->setListaColunasSelecionadas($listaColunasSelecionadas)
                            ->setListaColunaFormatoMoeda($listaColunaFormatoMoeda)
                            ->setListaRelatorio($listaRelatorio)
                            ->montarTabela()
                            ->getTabela();
                    ?>

                </div>
            </div>
        </div>
    </div>
</div>

