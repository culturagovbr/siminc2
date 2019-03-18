<?php

    $cabecalho = Acompanhamento_Model_Acompanhamento::montarColunasRelatorioPPA();

    # Lista de colunas que terão formatação de moeda.
    $listaColunaMoeda = Acompanhamento_Model_Acompanhamento::montarColunasFormatoMoedaRelatorioPPA();

    # Busca lista de Subunidades do usuário logado pra aplicar filtro em caso de usuário do perfil Unidade
//    $listaSubUnidadeUsuario = buscarSubUnidadeUsuario((object) array('usucpf' => $_SESSION['usucpf']));

    # Busca lista de metas PPA
    $modelAcompanhamento = new Acompanhamento_Model_Acompanhamento();
    $listaAcompanhamento = $modelAcompanhamento->consultarRelatorioAcompanhamentoPPA((object)array(
        'exercicio' => $_SESSION['exercicio'],
        'unicod' => $param['unicod'],
        'suocod' => $param['suocod'],
        'janid' => $param['janid'],
        'oppid' => $param['oppid'],
        'mppid' => $param['mppid'],
    ));

//ver($listaAcompanhamento, d);
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
    <?php if($listaAcompanhamento): ?>
        <?php foreach($listaAcompanhamento as $acompanhamento){ ?>
            <tr>
                <?php foreach($acompanhamento as $coluna => $valor){ ?>
                    <td style="border: 1px #e5d9c5 solid;"><?php echo (in_array($coluna, $listaColunaMoeda)? formata_valor($valor): $valor)?(in_array($coluna, $listaColunaMoeda)? formata_valor($valor): $valor): '-'; ?></td>
                <?php } ?>
            </tr>
        <?php } ?>
    <?php endif; ?>
    </tbody>
</table>

<?php

    header( 'Content-Type: application/vnd.ms-excel' );
    header( 'Expires: 0' );
    header( 'Cache-Control: must-revalidate, post-check=0, pre-check=0' );
    header( 'Content-Disposition: attachment; filename="relatorio-monitoramento-ppa' . '.xls"' );
    die;
