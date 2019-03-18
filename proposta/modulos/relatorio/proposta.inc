<?php

    $cabecalho = [
        'ID Proposta',
        'Subunidade',
        'Enquadramento da Despesa',
        'RP',
        'Funcional',
        'Ação',
        'PO',
        'Quantidade Localizador',
        'Quantidade PO',
        'Quantidade Expansão PO',
        'Justificativa',
        'Justificativa Expansão',
        'Nat. Desp.',
        'IDUSO',
        'Fonte',
        'IDOC',
        'Valor',
        'Valor Expansão',
    ];

    # Lista de colunas que terão formatação de moeda.
    $listaColunaMoeda = array(
        'prdvalor',
        'prdvalorexpansao'
    );
    $listaPerfil = pegaPerfilGeral();
    $listaSubUnidadeUsuario = buscarSubUnidadeUsuario((object) array('usucpf' => $_SESSION['usucpf']));
    if((in_array(PFL_CONSULTA_UNIDADE, $listaPerfil)) || in_array(PFL_SUBUNIDADE, $listaPerfil) && ($listaSubUnidadeUsuario)){
        $filtro = array('prsano' => $_SESSION['exercicio']);
        if($listaSubUnidadeUsuario){
            $filtro['suocod'] = $listaSubUnidadeUsuario;
        }
    } else if (in_array(PFL_ADMINISTRADOR, $listaPerfil) || in_array(PFL_SUPERUSUARIO, $listaPerfil)){
        $filtro = array('prsano' => $_SESSION['exercicio']);
    }
    $sql = montarSqlRelatorioGeralProposta((object) $filtro);
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

    header('Content-Type: application/vnd.ms-excel');
    header('Expires: 0');
    header('Cache-Control: must-revalidate, post-check=0, pre-check=0');
    header('Content-Disposition: attachment; filename="relatorio-proposta'. '.xls"');
    die;
