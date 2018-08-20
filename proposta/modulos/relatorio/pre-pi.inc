<?php

    header( 'Content-Type: application/vnd.ms-excel' );
    header( 'Expires: 0' );
    header( 'Cache-Control: must-revalidate, post-check=0, pre-check=0' );
    header( 'Content-Disposition: attachment; filename="relatorio_geral_pi' . '.xls"' );

    $cabecalho = [
        'Id Pré-PI',
        'Título',
        'Descrição/Finalidade',
        'Subunidade',
        'Enquadramento da Despesa',
        'RP',
        'Item',
        'Subitem',
        'Funcional',
        'Ação',
        'PO',
        'Situação',
        'Produto',
        'Unidade de Medida',
        'Quantidade',
        'Código Objetivo PPA',
        'Objetivo PPA',
        'Código Metas PPA',
        'Metas PPA',
        'Código Iniciativa PPA',
        'Iniciativa PPA',
        'Código Meta PNC',
        'Meta PNC',
        'Código Indicador PNC',
        'Indicador PNC',
        'Área Cultural',
        'Segmento Cultural',
        'Localização',
        'País',
        'UF',
        'Estado',
        'UF do Município',
        'Município',
        'Custeio',
        'Capital',
        'Adicional de Quantidade',
        'Adicional de Valor Custeio',
        'Adicional de Valor Capital',
        'Justificativa do Adicional',
    ];

    # Lista de colunas que terão formatação de moeda.
    $listaColunaMoeda = array(
        'plivalorcusteio',
        'plivalorcapital',
        'plivalorcusteioadicional',
        'plivalorcapitaladicional',
    );
    $listaPerfil = pegaPerfilGeral();
    $listaSubUnidadeUsuario = buscarSubUnidadeUsuario((object) array('usucpf' => $_SESSION['usucpf']));
    $listaSubUnidadeUsuario = implode("', '", $listaSubUnidadeUsuario);
    if((in_array(PFL_CONSULTA_UNIDADE,$listaPerfil)) || in_array(PFL_SUBUNIDADE, $listaPerfil) && ($listaSubUnidadeUsuario)){
        $filtro = array('exercicio' => $_SESSION['exercicio']);
        if($listaSubUnidadeUsuario){
            $filtro['suocod']=$listaSubUnidadeUsuario;
        }
        $sql = montarSqlRelatorioGeralPrePi((object) $filtro);        
    }else if (in_array(PFL_ADMINISTRADOR, $listaPerfil) || in_array(PFL_SUPERUSUARIO, $listaPerfil)){
        $filtro = array('exercicio' => $_SESSION['exercicio']);
        $sql = montarSqlRelatorioGeralPrePi((object) $filtro);                
    }
    $lista = $db->carregar($sql);
?>

<table width="100%" cellspacing="0">
    <thead>
        <tr>
            <?php foreach($cabecalho as $coluna){ ?>
                <th style="border: 1px #e5d9c5 solid; background-color: #f4f4f4"><?php echo $coluna; ?></th>
            <?php } ?>
        </tr>
    </thead>
    <?php if($lista): ?>
    <tbody>
        <?php foreach($lista as $dado){ ?>
            <tr>
                <?php foreach($dado as $coluna => $valor){ ?>
                    <td style="border: 1px #e5d9c5 solid;"><?php echo in_array($coluna, $listaColunaMoeda)? formata_valor($valor): $valor; ?></td>
                <?php } ?>
            </tr>
        <?php } ?>
    </tbody>
    <?php endif; ?>
</table>

<?php
die;
