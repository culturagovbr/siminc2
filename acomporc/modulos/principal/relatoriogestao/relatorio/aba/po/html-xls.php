<?php
/**
 * HTML para preenchimento e processamento do PDF.
 * @version $Id: html-xls.php 95729 2015-03-24 17:43:21Z lindalbertofilho $
 * @see html.php
 */
?>
<div class="quadro-tcu">
    <table>
        <thead>
            <tr>
                <th colspan="7">Identifica��o do Plano Or�ament�rio</th>
            </tr>
        </thead>
        <tbody>
            <tr>
                <td class="titulo">C�digo</td>
                <td colspan="6">%codigo%</td>
            </tr>
            <tr>
                <td class="titulo">T�tulo</td>
                <td colspan="6">%rldtitulo%</td>
            </tr>
            <tr>
                <td class="titulo">Programa</td>
                <td colspan="2">%rldprograma%</td>
                <td colspan="2"><b>C�digo:</b> %rldcodigoprograma%</td>
                <td colspan="2"><b>Tipo:</b> %rldtipoprograma%</td>
            </tr>
            <tr>
                <td class="titulo">Unidade Or�ament�ria</td>
                <td colspan="6" style="text-align:left">%unicod%</td>
            </tr>
            <tr>
                <td class="titulo">A��o Priorit�ria</td>
                <td colspan="2">( %rldacaoprioritaria_t% ) Sim &nbsp;( %rldacaoprioritaria_f% ) N�o</td>
                <td colspan="4"><b>Caso positivo:</b>&nbsp;&nbsp;&nbsp;&nbsp;
                    ( %rldacaoprioritariatipo_p% ) PAC&nbsp;&nbsp;&nbsp;
                    ( %rldacaoprioritariatipo_b% ) Brasil sem Mis�ria&nbsp;&nbsp;&nbsp;
                    ( %rldacaoprioritariatipo_o% ) Outras
                </td>
            </tr>
        </tbody>
    </table>
    <table>
        <thead>
            <tr>
                <th colspan="7">Lei Or�ament�ria %exercicio%</th>
            </tr>
            <tr class="level2">
                <th colspan="7">Execu��o Or�ament�ria e Financeira</th>
            </tr>
            <tr class="leveln">
                <th colspan="2">Dota��o</th>
                <th colspan="3">Despesa</th>
                <th colspan="2">Restos a Pagar<br />inscritos %exercicio%</th>
            </tr>
            <tr class="leveln">
                <th>Inicial</th>
                <th>Final</th>
                <th>Empenhada</th>
                <th>Liquidada</th>
                <th>Paga</th>
                <th>Processados</th>
                <th>N�o<br />Processados</th>
            </tr>
        </thead>
        <tbody>
            <tr>
                <td style="text-align:right">R$ %rlddotacaoinicial%</td>
                <td style="text-align:right">R$ %rlddotacaofinal%</td>
                <td style="text-align:right">R$ %rlddespempenhada%</td>
                <td style="text-align:right">R$ %rlddespliquidada%</td>
                <td style="text-align:right">R$ %rlddesppaga%</td>
                <td style="text-align:right">R$ %rldrapinscprocessado%</td>
                <td style="text-align:right">R$ %rldrapinscnaoprocessado%</td>
            </tr>
        </tbody>
    </table>
    <table>
        <thead>
            <tr>
                <th colspan="7">Execu��o F�sica</th>
            </tr>
            <tr class="level2">
                <th rowspan="2" colspan="2">Descri��o da Meta</th>
                <th rowspan="2" colspan="2">Unidade de Medida</th>
                <th colspan="3">Montante</th>
            </tr>
            <tr class="level2">
                <th>Previsto</th>
                <th>Reprogramado</th>
                <th>Realizado</th>
            </tr>
        </thead>
        <tbody>
            <tr>
                <td colspan="2">%rlddescmeta%</td>
                <td colspan="2">%rldunidademedida%</td>
                <td style="text-align:right">%rldmontanteprevisto%</td>
                <td style="text-align:right">%rldmontantereprogramado%</td>
                <td style="text-align:right">%rldmontanterealizado%</td>
            </tr>
        </tbody>
    </table>
    <table>
        <thead>
            <tr>
                <th colspan="7">Restos a Pagar N�o processados - Exerc�cios Anteriores</th>
            </tr>
            <tr class="level2">
                <th colspan="4">Execu��o Or�ament�ria e Financeira</th>
                <th colspan="3">Execu��o F�sica - Metas</th>
            </tr>
            <tr class="leveln">
                <th colspan="2">Valor em 01/01/%exercicio%</th>
                <th>Valor Liquidado</th>
                <th>Valor Cancelado</th>
                <th>Descri��o da Meta</th>
                <th>Unidade de medida</th>
                <th>Realizada</th>
            </tr>
        </thead>
        <tbody>
            <tr>
                <td style="text-align:right" colspan="2">R$ %rldrapeaem0101%</td>
                <td style="text-align:right">R$ %rldrapeavalorliquidado%</td>
                <td style="text-align:right">R$ %rldrapeavalorcancelado%</td>
                <td>%rldrapeadescricaometa%</td>
                <td>%rldrapeaunidademedida%</td>
                <td style="text-align:right">%rldrapearealizado%</td>
            </tr>
        </tbody>
    </table>
</div>