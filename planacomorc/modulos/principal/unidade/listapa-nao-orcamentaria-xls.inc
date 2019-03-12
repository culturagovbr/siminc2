<?php

header( 'Content-Type: application/vnd.ms-excel' );
header( 'Expires: 0' );
header( 'Cache-Control: must-revalidate, post-check=0, pre-check=0' );
header( 'Content-Disposition: attachment; filename="relatorio_geral_pa_nao_orcamentario' . '.xls"' );

# Colunas do relatório de Planos de Ação Não Orçamentários
$cabecalho = [
    'Id',
    'Unidade',
    'Título',
    'Situação',
    'Quantidade',
    'Unidade Medida',
    'Produto',
];

?>

    <table width="100%" cellspacing="0">
        <thead>
            <tr>
                <?php foreach($cabecalho as $coluna){ ?>
                    <th style="border: 1px #e5d9c5 solid; background-color: #f4f4f4"><?= $coluna; ?></th>
                <?php } ?>
            </tr>
        </thead>
        <tbody>
        <?php if($listaPa):
            foreach($listaPa as $pa): ?>
                <tr>
                    <td style="border: 1px #e5d9c5 solid;">
                        <?= $pa['id']; ?>
                    </td>
                    <td style="border: 1px #e5d9c5 solid;">
                        <?= $pa['sub_unidade']; ?>
                    </td>
                    <td style="border: 1px #e5d9c5 solid;">
                        <?= strip_tags($pa['plititulo']); ?>
                    </td>
                    <td style="border: 1px #e5d9c5 solid;">
                        <?= $pa['situacao']; ?>
                    </td>
                    <td style="border: 1px #e5d9c5 solid; text-align: left">
                        <?= $pa['quantidade']; ?>
                    </td>
                    <td style="border: 1px #e5d9c5 solid;">
                        <?= $pa['unidademedida']; ?>
                    </td>
                    <td style="border: 1px #e5d9c5 solid;">
                        <?= $pa['produto']; ?>
                    </td>
                </tr>
            <?php endforeach;
        endif; ?>
        </tbody>
    </table>

<?php
die;

