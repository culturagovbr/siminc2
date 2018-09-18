<?php

    # Incluindo arquivo do Módulo que possui a implementação da busca
    include_once APPRAIZ. 'www/proposta/_funcoes.php';

    /**
     * Busca a descricao da coluna pelo nome interno/Código
     * 
     * @param string $codigo
     * @param array $listaColunas
     * @return string
     */
    function buscarDescricaoPorCodigo($codigo, $listaColunas){
        $resultado = '';
        if($listaColunas){
            foreach($listaColunas as $coluna){
                if($codigo == $coluna['codigo']){
                    $resultado = $coluna['descricao'];
                    break;
                }
            }
        }
        
        return $resultado;
    }
    
    # Filtros do relatório
    $listaFiltros = $_POST['filtro'];
    
    # Colunas do relatorio
    $colunasPadroes = $_SESSION['relatorio_gerencial_pre_pi']['colunas'];
    $colunasSelecionadas = $_POST['coluna'];
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
    if($_REQUEST['req'] == 'xls'){
        ob_clean();
        $nomeDoArquivoXls = 'relatorio';
    //    echo $rel->getRelatorioXls();
        die();
    }

    include APPRAIZ . "includes/cabecalho_bootstrap_v3_popup.inc";
?>

<div class="row" style="margin-top: 5px;">
    <div class="col-md-12">
        <div class="ibox float-e-margins">
            <div class="ibox-title">
                <h5>Relatório Geral de Pré-PI</h5>
            </div>
            <div class="ibox-content">
                <div class="table-responsive">

                    <table class="table table-bordered table-hover dataTablesSP">
                        <thead>
                            <tr class="text-center">
                                <?php foreach($colunasSelecionadas as $colunaSelecionada): ?>
                                    <th><?=buscarDescricaoPorCodigo($colunaSelecionada, $colunasPadroes)?></th>
                                <?php endforeach; ?>
                            </tr>
                        </thead>
                        <?php if($listaRelatorio): ?>
                            <tbody>
                                <?php foreach($listaRelatorio as $itemRelatorio): ?>
                                    <tr>
                                        <?php foreach($colunasSelecionadas as $colunaSelecionada): ?>
                                            <td>
                                                <?php echo $itemRelatorio[$colunaSelecionada]; ?>
                                            </td>
                                        <?php endforeach; ?>
                                    </tr>
                                <?php endforeach; ?>
                            </tbody>
                        <?php else: ?>
                            <tfoot>
                                <tr>
                                    <td colspan="<?php echo count($colunasSelecionadas)-1; ?>">
                                        Não existem dados para serem mostrados
                                    </td>
                                </tr>
                            </tfoot>
                        <?php endif; ?>
                    </table>

                </div>
            </div>
        </div>
    </div>
</div>
