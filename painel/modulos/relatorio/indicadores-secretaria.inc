<?php

    # Consulta de Secretarias
    $sqlConsultaSecretarias = "
        SELECT
            secordem,
            secdsc AS descricao,
            secid AS codigo
        FROM painel.secretaria
        WHERE
            secstatus = 'A'
        ORDER BY
            secordem
    ";
    $listaSecretarias = $db->carregar($sqlConsultaSecretarias);

    # Filtro(s)
    $parametros = (object)$_REQUEST;
    # Monta SQL da consulta de indicadores
    $sqlListaIndicadores = montarSqlRelIndicadoresSecretaria($parametros);
    # Monta lista de Indicadores
    $listaIndicadores = $db->carregar($sqlListaIndicadores);
//ver($listaIndicadores, d);

    # Busca o nome da secretaria
    $secdsc = $db->pegaUm('SELECT secdsc FROM painel.secretaria WHERE secid = '. (int)$parametros->secid);
    
    include APPRAIZ . 'includes/cabecalho.inc';
?>
<!--<link href="../painel/relatorio/css/indicadores-secretaria.css" rel="stylesheet" media="screen">-->
<link href="https://fonts.googleapis.com/css?family=Open+Sans|Roboto|Material+Icons" rel="stylesheet">
<style type="text/css">
    body {
        font-family: "Helvetica Neue",Helvetica,Arial,sans-serif;
        font-size: 14px;
        line-height: 1.42857143;
        color: #333;
        background-color: #fff;
    }
    .container-tabela{
        width:90%;
        margin: 0 auto;
        position: relative;
        -webkit-print-color-adjust:exact;
        color-adjust: exact;
    }
    .container-tabela .table{
      width:100%;
    }
    h1 {
      font-family: Open Sans;
      font-style: normal;
      font-weight: 300;
      line-height: normal;
      font-size: 24px;
      text-transform: uppercase;
      color: #828282;
    }

    h2 {
      font-family: Open Sans;
      font-style: normal;
      font-weight: bold;
      line-height: normal;
      font-size: 18px;
      color: #828282;
    }
    table {
        border-collapse: collapse;
        border-spacing: 0;
    }
    td, th {
        display: table-cell;
        vertical-align: inherit;
    }
    th{
      min-height: 81px;
    }
    .table>thead>tr>th {
        vertical-align: bottom;
        border-bottom: 2px solid #ddd;
    }
    .table>thead>tr>th, .table>tbody>tr>th, .table>tfoot>tr>th, .table>thead>tr>td, .table>tbody>tr>td, .table>tfoot>tr>td {
        padding: 8px;
        line-height: 1.42857143;
        vertical-align: top;
        border-top: 1px solid #ddd;
    }
    .table-striped>tbody>tr:nth-child(odd)>td, .table-striped>tbody>tr:nth-child(odd)>th {
        background-color: #f9f9f9;
    }
    thead {
        background: #DCDCDC;
        font-family: Roboto;
        font-style: normal;
        font-weight: bold;
        line-height: normal;
        font-size: 14px;
        text-transform: uppercase;
        color: #4F4F4F;
        text-align: left;
    }
    a.voltar {
        float: right;
        color: #333333a3;
        position: absolute;
        top: 20px;
        right: 0;
    }
    .progress-bar {
        float: left;
        width: 0;
        height: 100%;
        font-size: 12px;
        line-height: 20px;
        color: #fff;
        text-align: center;
        background-color: #428bca;
        -webkit-box-shadow: inset 0 -1px 0 rgba(0,0,0,.15);
        box-shadow: inset 0 -1px 0 rgba(0,0,0,.15);
        -webkit-transition: width .6s ease;
        transition: width .6s ease;
    }
    .progress {
        overflow: hidden;
        height: 20px;
        margin-bottom: 20px;
        background-color: #f5f5f5;
        border-radius: 4px;
        -webkit-box-shadow: inset 0 1px 2px rgba(0,0,0,.1);
        box-shadow: inset 0 1px 2px rgba(0,0,0,.1);
    }
    #h2_secid, select {
        cursor: hand;
        cursor: pointer;
    }
    
    @media print
    {    
        .noprint, .no-print, .no-print *
        {
            display: none !important;
        }
        #h2_secid {
            display: block;
        }
    }

</style>

<div class="container-tabela">
    <h1>
        Indicadores por Secretarias
    </h1>
    
    <hr/>
    
    <h2 id="h2_secid" title="Clique aqui para escolher uma Secretaria">
        <?=($secdsc? $secdsc: 'Todas Secretarias');?>
    </h2>
    <span class="span_secid noprint">
        <?=$db->monta_combo('secid', $sqlConsultaSecretarias, 'S', 'Todas Secretarias', 'filtraSecretaria', '', '', '', 'S', '', '', $parametros->secid, '', '', 'noprint');?>
    </span>

    <a href="javascript:window.history.go(-1)" class="voltar" >  <i class="material-icons">arrow_back</i> </a>
    <?php if($parametros->secid): ?>
        <table class="table table-striped">
            <thead>
                <tr>
                    <th scope="col">Nome</th>
                    <th scope="col"style="text-align: left;">Unidade </th>
                    <th scope="col"style="text-align: left;">Produto</th>
                    <th scope="col"style="text-align: left;">Observações</th>
                    <th scope="col"style="text-align: left;">Meta atual</th>
                    <th scope="col"style="text-align: left;">Realizado</th>
                    <th scope="col">Orçamento</th>
                </tr>
            </thead>
            <?php if($listaIndicadores): ?>
                <tbody>
                    <?php foreach($listaIndicadores as $indicadores):?>
                        <?php
                        if (($indicadores['meta']-($indicadores['meta']*0.15))>$indicadores['realizado']){
                            $bgColor = 'red';
                        }else if (($indicadores['meta']-($indicadores['meta']*0.15))>=$indicadores['realizado']){
                            $bgColor = 'blue';
                        }else{
                            $bgColor = 'green';                     
                        }
                        ?>   
                        <tr bgcolor='<?=$bgColor;?>'>
                            <td>
                                <?=$indicadores['nome']; ?>
                            </td>
                            <td>
                                <?=$indicadores['unidade']; ?>
                            </td>
                            <td>
                                <?=$indicadores['produto']; ?>
                            </td>
                            <td>
                                <?=$indicadores['parecer_gestor_a']; ?>
                            </td>
                            <td>
                                <?=$indicadores['meta']; ?>
                            </td>
                            <td style='color: <?= $bgColor;?>'>
                                <?=$indicadores['realizado']; ?>
                            </td>
                            <td>
                                <?=number_format($indicadores['orcamento'], 0, ',', '.')?>
                                <br/>
                                <div class="progress" style="height: 2px;">
                                    <div class="progress-bar" role="progressbar" style="width: <?=$indicadores['realizado']; ?>%;" background-color: <?= $bgColor;?>; aria-valuenow="<?=$indicadores['realizado']; ?>" aria-valuemin="0" aria-valuemax="100"></div>
                                </div>
                            </td>
                        </tr>
                    <?php endforeach; ?>
                </tbody>
            <?php else: ?>
                <tfoot>
                    <tr>
                        <td colspan="7">Não existem indicadores cadastrados</td>
                    </tr>
                </tfoot>
            <?php endif; ?>
        </table>
    <?php else: ?>
        <hr />
        <?php
            foreach($listaSecretarias as $secretaria):
            $sqlListaIndicadores = montarSqlRelIndicadoresSecretaria((object)array('secid' => $secretaria['codigo']));
            $listaIndicadores = $db->carregar($sqlListaIndicadores);
        ?>
            <h2>
                <?=$secretaria['descricao']?>
            </h2>
            <table class="table table-striped">
                <thead>
                    <tr>
                        <th scope="col">Nome</th>
                        <th scope="col"style="text-align: left;">Unidade </th>
                        <th scope="col"style="text-align: left;">Produto</th>
                        <th scope="col"style="text-align: left;">Observações</th>
                        <th scope="col"style="text-align: left;">Meta atual</th>
                        <th scope="col"style="text-align: left;">Realizado</th>
                        <th scope="col">Orçamento</th>
                    </tr>
                </thead>
                <?php if($listaIndicadores): ?>
                    <tbody>
                        <?php foreach($listaIndicadores as $indicadores): ?>
                            <?php
                            if (($indicadores['meta']-($indicadores['meta']*0.15))>$indicadores['realizado']){
                                $bgColor = 'red';
                            }else if (($indicadores['meta']-($indicadores['meta']*0.15))>=$indicadores['realizado']){
                                $bgColor = 'blue';
                            }else{
                                $bgColor = 'green';                     
                            }
                            ?>                        
                            <tr>
                                <td>
                                    <?=$indicadores['nome']; ?>
                                </td>
                                <td>
                                    <?=$indicadores['unidade']; ?>
                                </td>
                                <td>
                                    <?=$indicadores['produto']; ?>
                                </td>
                                <td>
                                    <?=$indicadores['parecer_gestor_a']; ?>
                                </td>
                                <td>
                                    <?=$indicadores['meta']; ?>
                                </td>
                                <td style='color: <?= $bgColor;?>'>
                                    <?=$indicadores['realizado']; ?>
                                </td>
                                <td>
                                    <?=number_format($indicadores['orcamento'], 0, ',', '.')?>
                                    <br/>
                                    <div class="progress" style="height: 2px;">
                                        <div class="progress-bar" role="progressbar" style="width: <?=$indicadores['realizado']; ?>%; background-color: <?= $bgColor;?>; aria-valuenow="<?=$indicadores['realizado']; ?>" aria-valuemin="0" aria-valuemax="100"></div>
                                    </div>
                                </td>
                            </tr>
                        <?php endforeach; ?>
                    </tbody>
                <?php else: ?>
                    <tfoot>
                        <tr>
                            <td colspan="7">Não existem indicadores cadastrados</td>
                        </tr>
                    </tfoot>
                <?php endif; ?>
            </table>
            <!-- Quebra de pagina no documento impresso. -->
            <br style='page-break-before:always' />
        <?php endforeach; ?>
    <?php endif; ?>
</div>

<script>
    
    jQuery(document).ready(function(){
        
        jQuery('.span_secid').hide();
        
        jQuery('#h2_secid').click(function(){
            jQuery(this).hide();
            jQuery('.span_secid').show();
        });
        
    });
    
    function filtraSecretaria(){
        var secid = jQuery('select[name=secid]').val();
        window.location.href = '?modulo=relatorio/indicadores-secretaria&acao=A&secid='+ secid;
    }
</script>
