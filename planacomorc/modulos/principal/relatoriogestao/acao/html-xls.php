<?php
/**
 * HTML para preenchimento e processamento do XLS.
 * @version $Id: html-xls.php 89593 2014-11-03 18:34:19Z maykelbraz $
 * @see html.php
 */
?>
<div class="quadro-tcu">
    <table>
        <tr>
            <td style="border-color:white!important;" colspan="7">
                <p>Quadro A.5.2.3.1 - A��es de responsabilidade da UJ - OFSS</p>
            </td>
        </tr>
    </table>
    <table>
        <thead>
            <tr>
                <th colspan="7">Identifica��o da A��o</th>
            </tr>
        </thead>
        <tbody>
            <tr>
                <td class="titulo">C�digo</td>
                <td colspan="2" style="border:0">%codigo%</td>
                <td style="border:0;font-weight:bold">Tipo:</td>
                <td colspan="3" style="border:0">%rldtipocod%</td>
            </tr>
            <tr>
                <td class="titulo">T�tulo</td>
                <td colspan="6" style="text-align:left;">%rldtitulo%</td>
            </tr>
            <tr>
                <td class="titulo">Iniciativa</td>
                <td colspan="6">%rldiniciativa%</td>
            </tr>
            <tr>
                <td class="titulo">Objetivo</td>
                <td colspan="4" style="border:0">%rldobjetivo%</td>
                <td style="border:0;font-weight:bold">C�digo:</td>
                <td style="border:0">%rldcodigoobjetivo%</td>
            </tr>
            <tr>
                <td class="titulo">Programa</td>
                <td colspan="2" style="border-left:0;border-right:0">%rldprograma%</td>
                <td style="border-left:0;border-right:0;font-weight:bold">C�digo:</td>
                <td style="border-left:0;border-right:0">%rldcodigoprograma%</td>
                <td style="border-left:0;border-right:0;font-weight:bold">Tipo:</td>
                <td style="border-left:0">%rldtipoprograma%</td>
            </tr>
            <tr>
                <td class="titulo">Unidade Or�ament�ria</td>
                <td colspan="6">%unicod%</td>
            </tr>
            <tr>
                <td class="titulo">A��o Priorit�ria</td>
                <td style="border:0">( %rldacaoprioritaria_t% ) Sim</td>
                <td style="border:0">( %rldacaoprioritaria_f% ) N�o</td>
                <td style="border:0;font-weight:bold">Caso positivo:</td>
                <td style="border:0">( %rldacaoprioritariatipo_p% ) PAC</td>
                <td style="border:0">( %rldacaoprioritariatipo_b% ) Brasil sem Mis�ria</td>
                <td style="border:0">( %rldacaoprioritariatipo_o% ) Outras</td>
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
                <th rowspan="2">Descri��o da Meta</th>
                <th rowspan="2">Unidade de Medida</th>
                <th colspan="5">Montante</th>
            </tr>
            <tr class="level2">
                <th colspan="2">Previsto</th>
                <th>Reprogramado</th>
                <th colspan="2">Realizado</th>
            </tr>
        </thead>
        <tbody>
            <tr>
                <td>%rlddescmeta%</td>
                <td>%rldunidademedida%</td>
                <td colspan="2" style="text-align:right">%rldmontanteprevisto%</td>
                <td style="text-align:right">%rldmontantereprogramado%</td>
                <td colspan="2" style="text-align:right">%rldmontanterealizado%</td>
            </tr>
        </tbody>
    </table>
    <table>
        <thead>
            <tr>
                <th colspan="7">Restos a Pagar N�o processados - Exerc�cios Anteriores</th>
            </tr>
            <tr class="level2">
                <th colspan="3">Execu��o Or�ament�ria e Financeira</th>
                <th colspan="4">Execu��o F�sica - Metas</th>
            </tr>
            <tr class="leveln">
                <th>Valor em 01/01/%exercicio%</th>
                <th>Valor Liquidado</th>
                <th>Valor Cancelado</th>
                <th>Descri��o da Meta</th>
                <th>Unidade de medida</th>
                <th colspan="2">Realizada</th>
            </tr>
        </thead>
        <tbody>
            <tr>
                <td style="text-align:right">R$ %rldrapeaem0101%</td>
                <td style="text-align:right">R$ %rldrapeavalorliquidado%</td>
                <td style="text-align:right">R$ %rldrapeavalorcancelado%</td>
                <td>%rldrapeadescricaometa%</td>
                <td>%rldrapeaunidademedida%</td>
                <td colspan="2" style="text-align:right">%rldrapearealizado%</td>
            </tr>
        </tbody>
    </table>
</div>