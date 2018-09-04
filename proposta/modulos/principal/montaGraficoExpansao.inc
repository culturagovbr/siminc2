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
                                <li class="active"><a data-toggle="tab" href="#tab-4" tipo="Expansao" data-url="?modulo=inicio&acao=C&req=carregarGraficoUnidade&tipo=Expansao"> Gráficos das Unidades</a></li>
                                <li class=""><a data-toggle="tab" href="#tab-5" tipo="Expansao" data-url="?modulo=inicio&acao=C&req=carregarGraficoDireta&tipo=Expansao"> Gráficos da Direta</a></li>
                                <li class=""><a data-toggle="tab" href="#tab-6" tipo="Expansao" data-url="?modulo=inicio&acao=C&req=carregarGraficoCgconCogep&tipo=Expansao"> Gráficos da CGCON/COGEP</a></li>
                            </ul>
                            <div class="tab-content">
                                <div id="tab-4" class="tab-pane active">
                                    <?php $mPrePlanoInterno->carregarGrafico('Expansao',null, true, $filtroPainelProposta['esdid']); ?>
                                </div>
                                <div id="tab-5" class="tab-pane">
                                    <!--O grafico 2 será carregado aqui via ajax-->
                                </div>
                                <div id="tab-6" class="tab-pane">
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
