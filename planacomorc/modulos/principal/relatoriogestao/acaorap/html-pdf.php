<?php
/**
 * HTML para preenchimento e processamento do PDF.
 * @version $Id: html-pdf.php 89593 2014-11-03 18:34:19Z maykelbraz $
 * @see html.php
 */
?>
<div class="quadro-tcu">
    <p>Quadro A.5.2.3.3 - A��es n�o Previstas LOA %exercicio% - Restos a Pagar - OFSS</p>
    <table>
        <thead>
            <tr>
                <th colspan="2">Identifica��o da A��o</th>
            </tr>
        </thead>
        <tbody>
            <tr>
                <td class="titulo">C�digo</td>
                <td>
                    <div style="width:70%;float:left">%codigo%</div>
                    <div style="width:30%;float:left"><b>Tipo:</b> %rldtipocod%</div>
                </td>
            </tr>
            <tr>
                <td class="titulo">T�tulo</td>
                <td>%rldtitulo%</td>
            </tr>
            <tr>
                <td class="titulo">Iniciativa</td>
                <td>%rldiniciativa%</td>
            </tr>
            <tr>
                <td class="titulo">Objetivo</td>
                <td>
                    <div style="width:70%;float:left">%rldobjetivo%</div>
                    <div style="width:30%;float:left"><b>C�digo:</b> %rldcodigoobjetivo%</div>
                </td>
            </tr>
            <tr>
                <td class="titulo">Programa</td>
                <td>
                    <div style="width:55%;float:left">%rldprograma%</div>
                    <div style="width:20%;float:left"><b>C�digo:</b> %rldcodigoprograma%</div>
                    <div style="width:25%;float:left"><b>Tipo:</b> %rldtipoprograma%</div>
                </td>
            </tr>
            <tr>
                <td class="titulo">Unidade Or�ament�ria</td>
                <td>%unicod%</td>
            </tr>
            <tr>
                <td class="titulo">A��o Priorit�ria</td>
                <td>
                    <div style="width:10%;float:left">( %rldacaoprioritaria_t% ) Sim</div>
                    <div style="width:15%;float:left">( %rldacaoprioritaria_f% ) N�o</div>
                    <div style="width:75%;float:left">
                        <div style="width:23%;float:left">Caso positivo:</div>
                        <div style="width:15%;float:left">( %rldacaoprioritariatipo_p% ) PAC</div>
                        <div style="width:35%;float:left">( %rldacaoprioritariatipo_b% ) Brasil sem Mis�ria</div>
                        <div style="width:22%;float:left">( %rldacaoprioritariatipo_o% ) Outras</div>
                    </div>
                </td>
            </tr>
        </tbody>
    </table>
    <table>
        <thead>
            <tr>
                <th colspan="6">Restos a Pagar N�o processados - Exerc�cios Anteriores</th>
            </tr>
            <tr class="level2">
                <th colspan="3">Execu��o Or�ament�ria e Financeira</th>
                <th colspan="3">Execu��o F�sica - Metas</th>
            </tr>
            <tr class="leveln">
                <th>Valor em 01/01/%exercicio%</th>
                <th>Valor Liquidado</th>
                <th>Valor Cancelado</th>
                <th>Descri��o da Meta</th>
                <th>Unidade de medida</th>
                <th>Realizada</th>
            </tr>
        </thead>
        <tbody>
            <tr>
                <td style="text-align:right">R$ %rldrapeaem0101%</td>
                <td style="text-align:right">R$ %rldrapeavalorliquidado%</td>
                <td style="text-align:right">R$ %rldrapeavalorcancelado%</td>
                <td>%rldrapeadescricaometa%</td>
                <td>%rldrapeaunidademedida%</td>
                <td style="text-align:right">%rldrapearealizado%</td>
            </tr>
        </tbody>
    </table>
</div>