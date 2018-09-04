<?php if (!$listaSubUnidadeUsuario): ?>
    <div class="wrapper wrapper-content animated fadeInRight">
        <div class="row">
            <div class="col-md-12">
                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <h5>Valores por Unidades</h5>
                    </div>
                    <div class="ibox-content">
                        <div class="tabs-container tabs">
                            <ul class="nav nav-tabs prodTabs">
                                <li class="active"><a data-toggle="tab" href="#tab-1" tipo="Normal" data-url="?modulo=inicio&acao=C&req=carregarGraficoUnidade&tipo=Normal"> Gráficos das Unidades</a></li>
                                <li class=""><a data-toggle="tab" href="#tab-2" tipo="Normal" data-url="?modulo=inicio&acao=C&req=carregarGraficoDireta&tipo=Normal"> Gráficos da Direta</a></li>
                                <li class=""><a data-toggle="tab" href="#tab-3" tipo="Normal" data-url="?modulo=inicio&acao=C&req=carregarGraficoCgconCogep&tipo=Normal"> Gráficos da CGCON/COGEP</a></li>
                            </ul>
                            <div class="tab-content">
                                <div id="tab-1" class="tab-pane active">
                                    <?php $mPrePlanoInterno->carregarGrafico($_REQUEST['tipo'], null, true, $filtroPainelProposta['esdid']); ?>
                                </div>
                                <div id="tab-2" class="tab-pane">
                                    <!--O grafico 2 será carregado aqui via ajax-->
                                </div>
                                <div id="tab-3" class="tab-pane">
                                    <!--O grafico 3 será carregado aqui via ajax-->
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
<?php else: ?>
    <div class="wrapper wrapper-content animated fadeInRight">
        <div class="row">
            <div class="col-md-12">
                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <h5>Valores por Unidades</h5>
                    </div>
                    <div class="ibox-content">
                        <div class="tabs-container">
                            <div class="tab-content">
                                <div class="panel-body">
                                    <?php
                                        $codigoUnidadesWhere = "'". join($listaSubUnidadeUsuario, "','"). "'";
                                        $mPrePlanoInterno->carregarGrafico($_REQUEST['tipo'], ['unidades' => "suo.suocod IN(". $codigoUnidadesWhere. ")"], true, $filtroPainelProposta['esdid']);
                                    ?>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
<?php endif; ?>
