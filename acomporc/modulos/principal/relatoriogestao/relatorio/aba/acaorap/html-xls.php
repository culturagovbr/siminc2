<?php
/**
 * HTML para preenchimento e processamento do XLS.
 * @version $Id: html-xls.php 95729 2015-03-24 17:43:21Z lindalbertofilho $
 * @see html.php
 */
?>
<div class="quadro-tcu">
    <table>
        <tr>
            <td style="border-color:white!important" colspan="7">
                <p>Quadro A.5.2.3.3 - A��es n�o Previstas LOA %exercicio% - Restos a Pagar - OFSS</p>
            </td>
        </tr>
    </table>
    <table border="1">
        <thead>
            <tr style="background-color:#bbbbbb">
                <th colspan="7">Identifica��o da A��o</th>
            </tr>
        </thead>
        <tbody>
            <tr>
                <td class="titulo" style="background-color:#eeeeee">C�digo</td>
                <td colspan="2" style="border:0">%codigo%</td>
                <td style="border:0;font-weight:bold">Tipo:</td>
                <td colspan="3" style="border:0">%rldtipocod%</td>
            </tr>
            <tr>
                <td class="titulo" style="background-color:#eeeeee">T�tulo</td>
                <td colspan="6" style="text-align:left;">%rldtitulo%</td>
            </tr>
            <tr>
                <td class="titulo" style="background-color:#eeeeee">Iniciativa</td>
                <td colspan="6">%rldiniciativa%</td>
            </tr>
            <tr>
                <td class="titulo" style="background-color:#eeeeee">Objetivo</td>
                <td colspan="4" style="border:0">%rldobjetivo%</td>
                <td style="border:0;font-weight:bold">C�digo:</td>
                <td style="border:0">%rldcodigoobjetivo%</td>
            </tr>
            <tr>
                <td class="titulo" style="background-color:#eeeeee">Programa</td>
                <td colspan="2" style="border-left:0;border-right:0">%rldprograma%</td>
                <td style="border-left:0;border-right:0;font-weight:bold">C�digo:</td>
                <td style="border-left:0;border-right:0">%rldcodigoprograma%</td>
                <td style="border-left:0;border-right:0;font-weight:bold">Tipo:</td>
                <td style="border-left:0">%rldtipoprograma%</td>
            </tr>
            <tr>
                <td class="titulo" style="background-color:#eeeeee">Unidade Or�ament�ria</td>
                <td colspan="6">%unicod%</td>
            </tr>
            <tr>
                <td class="titulo" style="background-color:#eeeeee">A��o Priorit�ria</td>
                <td style="border:0">( %rldacaoprioritaria_t% ) Sim</td>
                <td style="border:0">( %rldacaoprioritaria_f% ) N�o</td>
                <td style="border:0;font-weight:bold">Caso positivo:</td>
                <td style="border:0">( %rldacaoprioritariatipo_p% ) PAC</td>
                <td style="border:0">( %rldacaoprioritariatipo_b% ) Brasil sem Mis�ria</td>
                <td style="border:0">( %rldacaoprioritariatipo_o% ) Outras</td>
            </tr>
        </tbody>
    </table>
    <table border="1">
        <thead>
            <tr style="background-color:#bbbbbb">
                <th colspan="7">Restos a Pagar N�o processados - Exerc�cios Anteriores</th>
            </tr>
            <tr class="level2" style="background-color:#cccccc">
                <th colspan="3">Execu��o Or�ament�ria e Financeira</th>
                <th colspan="4">Execu��o F�sica - Metas</th>
            </tr>
            <tr class="leveln" style="background-color:#dddddd">
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