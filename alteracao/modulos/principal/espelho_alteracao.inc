<?php
    $disabled = ['disabled'];
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
                <h2 style="color: #1AB394">1. Dados do Pedido
                    <a href="javascript:window.location.href='?modulo=principal/cadastro_alteracoes&acao=C&pedid=<?=$_REQUEST['pedid']?>&tpdid=<?=$_REQUEST['tpdid']?>'" tittle="Editar">
                        <span class="btn btn-primary btn-sm glyphicon glyphicon-pencil"></span>
                    </a>
                </h2>                
                <?php
                    include_once APPRAIZ.'alteracao/modulos/principal/cadastro_pedido.inc';
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

