<?php

    $suoid = array();
    if ($_REQUEST['pedid']){
        $suoid = (new Alteracao_Model_PedidoUnidade())->carregarPorPedido($_REQUEST['pedid']);
        foreach($suoid as $value){
            $arrSuoid[]=$value['suoid'];
        }
    }
?>
<div class="wrapper wrapper-content animated fadeInRight alteracao-wrapper">
    <div class="col-lg-12">
        <div class="float-e-margins">
            <h2>Verifique se as informações estão corretas e clique no botão enviar</h2>
            <hr class="ruptura_tematica">
        </div>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight row">
    <div class="col-lg-12">
        <div class="float-e-margins">
            <div class="ibox-content">
                <?php
                    include_once APPRAIZ. 'alteracao/modulos/principal/espelho_pedido.inc';
                ?>
            </div>
        </div>
    </div>
</div>
<div class="wrapper wrapper-content animated fadeInRight row">
    <div class="col-lg-12">
        <div class="float-e-margins">
            <div class="ibox-content">
                <?php
                    include_once APPRAIZ.'alteracao/modulos/principal/espelho_pi_selecionados.inc';
                ?>
            </div>
        </div>
    </div>
</div>
<div class="wrapper wrapper-content animated fadeInRight row">
    <div class="col-lg-12">
        <div class="float-e-margins">
            <div class="ibox-content">
                <?php
                    include_once APPRAIZ.'alteracao/modulos/principal/espelho_remanejamento_loa.inc';
                ?>
            </div>
        </div>
    </div>
</div>
<!--<div class="wrapper wrapper-content animated fadeInRight row">
    <div class="col-lg-12">
        <div class="float-e-margins">
            <div class="ibox-content">
                <?php /*
                    # Natureza de Despesa
                    include_once APPRAIZ.'alteracao/modulos/principal/espelho_natureza_receita.inc';
                */?>
            </div>
        </div>
    </div>
</div>-->
<div class="wrapper wrapper-content animated fadeInRight row">
    <div class="col-lg-12">
        <div class="float-e-margins">
            <div class="ibox-content">
                <?php
                    include_once APPRAIZ.'alteracao/modulos/principal/espelho_justificativa.inc';
                ?>
            </div>
        </div>
    </div>
</div>
