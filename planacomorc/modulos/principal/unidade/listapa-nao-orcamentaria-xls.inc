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
        <?php if($listaPi):
            foreach($listaPi as $pi): ?>
                <tr>
                    <td style="border: 1px #e5d9c5 solid;">
                        <?= $pi['id']; ?>
                    </td>
                    <td style="border: 1px #e5d9c5 solid;">
                        <?= $pi['sub_unidade']; ?>
                    </td>
                    <td style="border: 1px #e5d9c5 solid;">
                        <?= strip_tags($pi['plititulo']); ?>
                    </td>
                    <td style="border: 1px #e5d9c5 solid;">
                        <?= $pi['situacao']; ?>
                    </td>
                    <td style="border: 1px #e5d9c5 solid; text-align: left">
                        <?= $pi['quantidade']; ?>
                    </td>
                    <td style="border: 1px #e5d9c5 solid;">
                        <?= $pi['unidademedida']; ?>
                    </td>
                    <td style="border: 1px #e5d9c5 solid;">
                        <?= $pi['produto']; ?>
                    </td>
                </tr>
            <?php endforeach;
        endif; ?>
        </tbody>
    </table>

<?php
die;

