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
    $nuanoreferencia = $_REQUEST['nuanoreferencia']?$_REQUEST['nuanoreferencia']:date("Y");
    $numesreferencia = $_REQUEST['numesreferencia']?$_REQUEST['numesreferencia']:date("m");
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
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
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
    .h2_hand, select {
        cursor: hand;
        cursor: pointer;
    }
    
    @media print
    {    
        .noprint, .no-print, .no-print *
        {
            display: none !important;
        }
        #h2_secid, h2_nuanoreferencia {
            display: block;
        }
    }

</style>

<div class="alert alert-warning alert-dismissable aria-hidden noprint" align="center" style="font-size: 14px">
    Para uma melhor impress�o dos dados utilize o navegador FireFox
</div>
<div class="container-tabela">
    <h1>
        Indicadores por Secretarias
    </h1>
    
    <hr/>
    <div class="row">
        <div class="col-lg-4">
            <h2 id="h2_secid" class="h2_hand" title="Clique aqui para escolher uma Secretaria">
                <?=($secdsc? $secdsc: 'Todas Secretarias');?>
            </h2>
            <span class="span_secid noprint">
                <?=$db->monta_combo('secid', $sqlConsultaSecretarias, 'S', 'Todas Secretarias', 'filtraSecretaria', '', '', '', 'S', '', '', $parametros->secid, '', '', 'noprint');?>
            </span>            
        </div>
        <div class="col-lg-4">
            <h2 id="h2_nuanoreferencia" class="h2_hand" title="Clique aqui para escolher o ano">
                <?=($nuanoreferencia);?>
            </h2>
            <span class="span_nuanoreferencia noprint">
                <?php
                    $anoInicial = date("Y")-7;
                    $anoFinal = date("Y")+5;
                    $j=0;
                    for ($i=$anoInicial;$i<=$anoFinal;$i++){
                        $listaAnos[$j]['codigo']=$i;
                        $listaAnos[$j]['descricao']=$i;
                        $j++;
                    }
                    $db->monta_combo('nuanoreferencia', $listaAnos, 'S', 'Selecione', 'filtraSecretaria', '', '', '200', 'S', 'nuanoreferencia','',$nuanoreferencia);
                ?>
            </span>                
        </div>
        <div class="col-lg-2">
            <?php
                $listaMeses = Array(
                                    array('codigo' => 1, 'descricao'=>'Janeiro'),
                                    array('codigo' => 2, 'descricao'=>'Fevereiro'),
                                    array('codigo' => 3, 'descricao'=>'Mar�o'),
                                    array('codigo' => 4, 'descricao'=>'Abril'),
                                    array('codigo' => 5, 'descricao'=>'Maio'),
                                    array('codigo' => 6, 'descricao'=>'Junho'),
                                    array('codigo' => 7, 'descricao'=>'Julho'),
                                    array('codigo' => 8, 'descricao'=>'Agosto'),
                                    array('codigo' => 9, 'descricao'=>'Setembro'),
                                    array('codigo' => 10, 'descricao'=>'Outubro'),
                                    array('codigo' => 11, 'descricao'=>'Novembro'),
                                    array('codigo' => 12, 'descricao'=>'Dezembro')
                                   );            
            ?>
            <h2 id="h2_numesreferencia" class="h2_hand" title="Clique aqui para escolher o m�s">
                <?php
                foreach($listaMeses as $value){
                    if ($value['codigo']==$numesreferencia){
                        echo $value['descricao'];
                    }
                }
                ?>
            </h2>  
            <span class="span_numesreferencia noprint">
                <?php
                    $db->monta_combo('numesreferencia', $listaMeses, 'S', 'Selecione', 'filtraSecretaria', '', '', '200', 'S', 'numesreferencia','',$numesreferencia);
                ?>
            </span>
        </div>
    </div>

    <a href="javascript:window.history.go(-1)" class="voltar" >  <i class="material-icons">arrow_back</i> </a>
    <?php if($parametros->secid): ?>
        <table class="table table-striped">
            <thead>
                <tr>
                    <tr>
                        <th scope="col" width="20%">Nome</th>
                        <th scope="col" width="10%"style="text-align: center;">Produto</th>
                        <th scope="col" width="5%"style="text-align: center;">Or�amento</th>
                        <th scope="col" width="5%"style="text-align: center;">Previsto</th>
                        <th scope="col" width="5%"style="text-align: center;">Realizado</th>
                        <th scope="col" width="40%"style="text-align: left;">Observa��es</th>
                    </tr>
                </tr>
            </thead>
            <?php if($listaIndicadores): ?>
                <tbody>
                    <?php foreach($listaIndicadores as $indicadores):?>
                        <?php
                            if (($indicadores['meta']-($indicadores['meta']*0.15))>$indicadores['realizado']){
                                //vermelho
                                $bgColor = '#E92922';
                            }else if (($indicadores['meta']-($indicadores['meta']*0.15))>$indicadores['realizado'] || $indicadores['meta']>$indicadores['realizado']){
                                //marrom
                                $bgColor = '#bf662b'; 
                            }else{
                                //verde
                                $bgColor = '#1db954';                    
                            }
                        ?>   
                        <tr>
                            <td>
                                <?=$indicadores['nome']; ?>
                            </td>
                            <td align="center">
                                <?=$indicadores['produto']; ?>
                            </td>
                            <td align="center">
                                <?=number_format($indicadores['orcamento'], 0, ',', '.')?>
                            </td>
                            <td align="center">
                                <?=$indicadores['meta']; ?>
                            </td>
                            <td style='color: <?= $bgColor;?>' align="center">
                                <?=$indicadores['realizado']; ?>
                                <br/>
                                <div class="progress" style="height: 6px;">
                                    <div class="progress-bar" role="progressbar" style="width: <?=$indicadores['realizado']; ?>%; background-color: <?= $bgColor;?>;" aria-valuenow="<?=$indicadores['realizado']; ?>" aria-valuemin="0" aria-valuemax="100"></div>
                                </div>                                
                            </td>
                            <td>
                                <?php
                                $class='';
                                if ($indicadores['indavalgestor']=='S'){
                                    $class='fa-thumbs-up';
                                }else if ($indicadores['indavalgestor']=='N'){
                                    $class='fa-thumbs-down';
                                }else if ($indicadores['indavalgestor']=='E'){
                                    $class='fa-exclamation-circle';
                                }
                                ?>
                                <i style="font-size:2.5em" class="fa <?= $class?>"></i>                                
                                <?=$indicadores['observacao_gestor_i']; ?>
                            </td>
                        </tr>
                    <?php endforeach; ?>
                </tbody>
            <?php else: ?>
                <tfoot>
                    <tr>
                        <td colspan="6">N�o existem indicadores cadastrados</td>
                    </tr>
                </tfoot>
            <?php endif; ?>
        </table>
    <?php else: ?>
        <hr />
        <?php
            if($listaSecretarias) :
                foreach($listaSecretarias as $secretaria):
                $sqlListaIndicadores = montarSqlRelIndicadoresSecretaria((object)array('secid' => $secretaria['codigo'], 'nuanoreferencia' => $nuanoreferencia, 'numesreferencia' => $numesreferencia));
                $listaIndicadores = $db->carregar($sqlListaIndicadores);
        ?>
            <h2>
                <?=$secretaria['descricao']?>
            </h2>
            <table class="table table-striped">
                <thead>
                    <tr>
                        <!--<th >indid</th>-->
                        <th >Nome</th>
                        <th style="text-align: center;">Produto</th>
                        <th style="text-align: center;">Or�amento</th>
                        <th style="text-align: center;">Previsto</th>
                        <th style="text-align: center;">Realizado</th>
                        <th style="text-align: left;">Observa��es</th>
                    </tr>
                </thead>
                <?php if($listaIndicadores): ?>
                    <tbody>
                        <?php foreach($listaIndicadores as $indicadores): ?>
                            <?php
                            if (($indicadores['meta']-($indicadores['meta']*0.15))>$indicadores['realizado']){
                                //vermelho
                                $bgColor = '#E92922';
                            }else if (($indicadores['meta']-($indicadores['meta']*0.15))>$indicadores['realizado'] || $indicadores['meta']>$indicadores['realizado']){
                                //marrom
                                $bgColor = '#bf662b'; 
                            }else{
                                //verde
                                $bgColor = '#1db954';                    
                            }
                            ?>                        
                            <tr>
<!--                                <td style="width: 400px;">
                                    <?=$indicadores['indid']; ?>
                                </td>-->
                                <td style="width: 400px;">
                                    <?=$indicadores['nome']; ?>
                                </td>
                                <td style="width: 400px;" align="center">
                                    <?= $indicadores['produto'] ? $indicadores['produto'] : '-' ;?>
                                </td>
                                <td style="width: 50px;" align="center">
                                    <?=number_format($indicadores['orcamento'], 0, ',', '.')?>
                                </td>
                                <td style="width: 50px;" align="center">
                                    <?=$indicadores['meta']; ?>
                                </td>
                                <td style='color: <?= $bgColor;?> width: 50px;' align="center">
                                    <?=$indicadores['realizado']; ?>
                                    <br/>
                                    <div class="progress" style="height: 6px;">
                                        <div class="progress-bar" role="progressbar" style="width: <?=$indicadores['realizado']; ?>%; background-color: <?= $bgColor;?>; aria-valuenow="<?=$indicadores['realizado']; ?>" aria-valuemin="0" aria-valuemax="100"></div>
                                    </div>                                    
                                </td>
                                <td style="width: 400px;">
                                    <?php
                                    $class='';
                                    if ($indicadores['parecer_gestor_a']=='S'){
                                        $class='fa-thumbs-up';
                                        $color='#1db954';
                                    }else if ($indicadores['parecer_gestor_a']=='N'){
                                        $class='fa-thumbs-down';
                                        $color='#E92922';
                                    }else if ($indicadores['parecer_gestor_a']=='E'){
                                        $class='fa-exclamation-circle';
                                        $color='#bf662b';
                                    }
                                    ?>
                                    <i style="font-size:2.5em; color: <?=$color?>" id="indthumbs" class="fa <?= $class?>"></i>                                                                        
                                    <?=$indicadores['observacao_gestor_i']; ?>
                                </td>
                            </tr>
                        <?php endforeach; ?>
                    </tbody>
                <?php else: ?>
                    <tfoot>
                        <tr>
                            <td colspan="6">N�o existem indicadores cadastrados</td>
                        </tr>
                    </tfoot>
                <?php endif; ?>
            </table>
            <!-- Quebra de pagina no documento impresso. -->
            <br style='page-break-before:always; page-break-inside: avoid;' />        
            <?php endforeach; ?>
        <?php endif; ?>
    <?php endif; ?>
</div>

<script>
    
    jQuery(document).ready(function(){
        
        jQuery('.span_secid').hide();
        jQuery('.span_nuanoreferencia').hide();
        jQuery('.span_numesreferencia').hide();
        
        jQuery('#h2_secid').click(function(){
            jQuery(this).hide();
            jQuery('.span_secid').show();
        });
        jQuery('#h2_nuanoreferencia').click(function(){
            jQuery(this).hide();
            jQuery('.span_nuanoreferencia').show();
        });
        jQuery('#h2_numesreferencia').click(function(){
            jQuery(this).hide();
            jQuery('.span_numesreferencia').show();
        });
        
    });
    
    function filtraSecretaria(){
        var secid = jQuery('select[name=secid]').val();
        var nuanoreferencia = jQuery('select[name=nuanoreferencia]').val();
        var numesreferencia = jQuery('select[name=numesreferencia]').val();
        window.location.href = '?modulo=relatorio/indicadores-secretaria&acao=A&secid='+ secid+'&nuanoreferencia='+ nuanoreferencia+'&numesreferencia='+ numesreferencia;
    }
</script>
