<?php
/**
 * HTML para preenchimento e processamento do PDF.
 * @version $Id: html-pdf.php 89593 2014-11-03 18:34:19Z maykelbraz $
 * @see html.php
 */
?>
<div class="quadro-tcu">
    <p>Quadro A.5.2.3.1 - A��es de responsabilidade da UJ - OFSS</p>
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
                <th colspan="5">Execu��o F�sica</th>
            </tr>
            <tr class="level2">
                <th rowspan="2">Descri��o da Meta</th>
                <th rowspan="2">Unidade de Medida</th>
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
                <td>%rlddescmeta%</td>
                <td>%rldunidademedida%</td>
                <td style="text-align:right">%rldmontanteprevisto%</td>
                <td style="text-align:right">%rldmontantereprogramado%</td>
                <td style="text-align:right">%rldmontanterealizado%</td>
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