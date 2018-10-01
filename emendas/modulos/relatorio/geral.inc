<?php

    $cabecalho = [
        'ID Emenda',
        'Número',
        'Cod. Autor',
        'Autor',
        'Gênero',
        'Tipo Parlamentar',
        'UF Parlamentar',
        'Partido',
        'Cod. Unidade',
        'Unidade',
        'Funcional',
        'Impositiva',
        'Justificativa',
        # DADOS FINANCEIROS
        'Valor Custeio',
        'Valor Capital',
        # BENEFICIARIOS
        'Id Beneficiario',
        'Código Subunidade',
        'Subunidade',
        'Código Subunidade delegada',
        'Subunidade delegada',
        'CNPJ',
        'Proponente',
        'Localização',
        'Paises',
        'UFs',
        'Municipios',
        'Número do Processo',
        'Data de Início da Vigência',
        'Modalidade de Pactuação',
        'TED',
        'Área Cultural',
        'Segmento Cultural',
        'Alterações',
        'Parecer Técnico',
        'Parecer Jurídico',
        'Impedimento',
        'Motivo',
        'Justivicativa',
        # DADOS FINANCEIROS DO BENFICIÁRIO
        'Vl. Beneficiário Custeio',
        'Vl. Beneficiário Capital',
        'Vl. Priorizado Custeio',
        'Vl. Priorizado Capital',
        'Vl. Contingenciado Custeio',
        'Vl. Contingenciado Capital',
        # DADOS SICONV
        'Número Programa - SICONV',
        'Título Programa - SICONV',
        'Objeto Programa',
        'Número Proposta - SICONV',
        'Título Proposta - SICONV',
        'Objeto Proposta',
        'Situação Proposta',
        # SITUAÇÃO WORKFLOW
        'Situação',
        # PLANO INTERNO
        'Id PI',
        'Código PI',
        'Empenhado',
        'Pago',
    ];

    # Lista de colunas que terão formatação de moeda.
    $listaColunaMoeda = Emendas_Model_Emenda::montarColunasFormatoMoedaRelatorioEmendas();

    $listaSubUnidadeUsuario = buscarSubUnidadeUsuario((object) array('usucpf' => $_SESSION['usucpf']));

    $sql = (new Emendas_Model_Emenda)->montarSqlRelatorioGeral( (object) array(
        'exercicio' => $_SESSION['exercicio'],
        'listaSubUnidadeUsuario' => $listaSubUnidadeUsuario
    ));
//ver($sql);
    $lista = $db->carregar($sql);
//ver($lista, d);
?>

<table width="100%" cellspacing="0">
    <thead>
        <tr>
            <?php foreach($cabecalho as $coluna){ ?>
                <th style="border: 1px #e5d9c5 solid; background-color: #f4f4f4"><?php echo $coluna; ?></th>
            <?php } ?>
        </tr>
    </thead>
    <tbody>
        <?php foreach($lista as $dado){ ?>
            <tr>
                <?php foreach($dado as $coluna => $valor){ ?>
                    <td style="border: 1px #e5d9c5 solid;"><?php echo in_array($coluna, $listaColunaMoeda)? formata_valor($valor): $valor; ?></td>
                <?php } ?>
            </tr>
        <?php } ?>
    </tbody>
</table>

<?php

    header( 'Content-Type: application/vnd.ms-excel' );
    header( 'Expires: 0' );
    header( 'Cache-Control: must-revalidate, post-check=0, pre-check=0' );
    header( 'Content-Disposition: attachment; filename="relatorio_geral_emendas_beneficiarios' . '.xls"' );
    die;
