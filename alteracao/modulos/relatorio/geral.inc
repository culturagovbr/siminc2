<?php

    $cabecalho = [
        'ID Pedido',
        'Código da UO',
        'Sigla da UO',
        'Nome da UO',
        'Código da Unidade',
        'Sigla da Unidade',
        'Nome da Unidade',
        'Título',
        'Tipo',
        'Situação',
        'Janela',
        'Data de Cadastro',
        'Total Cancelado',
        'Total Suplementado',
        'Identificador do PI',
        'Funcional do PI',
        'Código PI',
        'Título do PI',
        'Produto do PI',
        'Valor Custeio',
        'Valor Capital',
        'Físico',
        'Suplementar Custeio',
        'Suplementar Capital',
        'Suplementar Físico',
        'Cancelar Custeio',
        'Cancelar Capital',
        'Cancelar Físico',
        'Suplementar por Excesso Custeio',
        'Suplementar por Excesso Capital',
        'Suplementar por Excesso Físico',
        'Suplementar por Superávit Custeio',
        'Suplementar por Superávit Capital',
        'Suplementar por Superávit Físico',
        'Resultado Provável Custeio',
        'Resultado Provável Capital',
        'Resultado Provável Físico',
        'LOA Funcional',
        'LOA Categoria',
        'LOA GND',
        'LOA Modalidade de Aplicação',
        'LOA Fonte',
        'LOA IDOC',
        'LOA IDUSO',
        'LOA RP',
        'LOA Suplementar',
        'LOA Cancelado',
        'LOA Suplementar por excesso',
        'LOA Suplementar por Superávit',
        'Justificativa Necessidade de Alteração',
        'Justificativa Causa da Demanda',
        'Justificativa formas de financiamento de Crédito e adequação da proposta à meta fiscal e ao limite de gastos',
        'Justificativa Verificação das fontes, dos Identificadores de uso - Iduso e dos identificadores de Resultado Primário - RP',
        'Justificativa Medida Provisória',
        'Justificativa Legislação Específica',
        'Justificativa Outras Informações'
    ];

    # Lista de colunas que terão formatação de moeda.
    $listaColunaMoeda = array(
        'cancelado',
        'suplementado',
        
        'vlcusteio',
        'vlcapital',
        'vlfisico',
        'vlsuplementarcusteio',
        'vlsuplementarcapital',
        'vlsuplementarfisico',
        'vlcancelarcusteio',
        'vlcancelarcapital',
        'vlcancelarfisico',
        'vlsuplementarexcessocusteio',
        'vlsuplementarexcessocapital',
        'vlsuplementarexcessofisico',
        'vlsuplementarsuperavitcusteio',
        'vlsuplementarsuperavitcapital',
        'vlsuplementarsuperavitfisico',
        'vldotacaocusteio',
        'vldotacaocapital',
        'vldotacaofisico',
        
        'vlsuplementar',
        'vlcancelado',
        'vlsuplementarexcesso',
        'vlsuplementarsuperavit'
    );
    
    $listaPerfil = pegaPerfilGeral();
    $listaSubUnidadeUsuario = buscarSubUnidadeUsuario((object) array('usucpf' => $_SESSION['usucpf']));
    $parametro = array('pedano' => $_SESSION['exercicio']);
    if(in_array(PFL_SUBUNIDADE, $listaPerfil) && $listaSubUnidadeUsuario){
        if($listaSubUnidadeUsuario){
            $parametro['suocod'] = $listaSubUnidadeUsuario;
        }
    }
    $sql = montarSqlRelatorioGeralAlteracao((object) $parametro);
    $lista = $db->carregar($sql);
//ver($sql, $lista, d);
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
    header( 'Content-Type: application/vnd.ms-excel' );
    header( 'Expires: 0' );
    header( 'Cache-Control: must-revalidate, post-check=0, pre-check=0' );
    header( 'Content-Disposition: attachment; filename="relatorio_geral_pedido_de_alteracao' . '.xls"' );
    die;
