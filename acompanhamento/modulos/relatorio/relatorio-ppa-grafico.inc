<?php

    include_once APPRAIZ . "monitora/classes/Pi_PlanoInterno.class.inc";
    include_once APPRAIZ . "www/planacomorc/_funcoespi.php";

    $param = $_REQUEST;

    # Controla requisições extras da tela
    if($param['req']) {
        switch($param['req']) {
            case 'carregarComboUG':
                echo montarComboUG((object) array(
                    'suocod' => $param['suocod'],
                    'exercicio' => $_SESSION['exercicio'],
                    'unicod' => $param['unicod']
                ));
                die();
            break;
            case 'carregarMetasPPA':
                carregarMetasPPA((int)$param['oppid'], null, $param['suocod']);
                die();
            break;
        }
    }

    # Busca lista de metas PPA
    $modelAcompanhamento = new Acompanhamento_Model_Acompanhamento();
    $listaMetaPpa = $modelAcompanhamento->getMetasPpaRelatorio( (object)array(
        'exercicio' => $_SESSION['exercicio'],
        'unicod' => $param['unicod'],
        'suocod' => $param['suocod'],
        'janid' => $param['janid'],
        'oppid' => $param['oppid'],
        'mppid' => $param['mppid'],
    ));
    /**
     * Cabeçalho padrão do sistema.
     */
    include APPRAIZ . "includes/cabecalho.inc";
?>

<script src="../library/liquify/snap.svg.js"></script>
<script src="../library/liquify/liquid.meter.js"></script>

<style>
/*    .tabela-listagem {
        font-size: 1em !important;
    }*/
</style>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <div class="col-md-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5><?php echo $titulo_modulo; ?></h5>
                </div>
                <div class="ibox-content noprint">
                    <form id="filtropi" name="filtropi" method="POST" role="form" class="form-horizontal">
                        <input name="requisicao" id="requisicao" value="" type="hidden">
                        <?php
                            echo $simec->select('unicod', 'Unidade', $param['unicod'], Spo_Model_Unidade::queryCombo((object)array(
                                    'prsano::INTEGER = '. (int)$_SESSION['exercicio'],
                                    'suo.unofundo = FALSE')));
                            echo '<div id=div_ungcod>';
                            echo $simec->select('suocod', 'SubUnidade', $param['suocod'], Public_Model_SubUnidadeOrcamentaria::queryCombo((object)array(
                                    'unofundo' => 'FALSE',
                                    'exercicio' => $_SESSION['exercicio'])));
                            echo '</div>';
                            echo $simec->select('janid', 'Janela', $param['janid'], (new Acompanhamento_Model_Janela())->recuperarSqlCombo(["prsano = '{$_SESSION['exercicio']}'", 'tipid = ' . Acompanhamento_Model_Tipo::K_TIPO_META_PPA], 'janinicio DESC'));
                        ?>
                        <div class="form-group">
                            <div class="col-md-3 text-right">
                                <label for="oppid" class=" control-label">Objetivo PPA: <span title="Campo obrigatório" class="link campo-obrigatorio">*</span></label>
                            </div>
                            <div class="col-lg-9">
                                <?php (new Public_Model_ObjetivoPpa())->monta_combo($param['oppid']); ?>
                            </div>
                        </div>
                        <div class="form-group">
                            <div class="col-md-3 text-right">
                                <label for="mppid" class=" control-label">Metas PPA: <span title="Campo obrigatório" class="link campo-obrigatorio">*</span></label>
                            </div>
                            <div class="col-lg-9 div_mppid">
                                <?php carregarMetasPPA($param['oppid'], $param['mppid']); ?>
                            </div>
                        </div>

                        <div class="text-center">
                            <button class="btn btn-warning btn-limpar" type="button">
                                <span class="glyphicon glyphicon-remove-circle"></span> Limpar
                            </button>
                            <button class="btn btn-primary btn-buscar" type="submit">
                                <span class="glyphicon glyphicon-search"></span> Buscar
                            </button>
                        </div>

                    </form>
                </div>

                <div class="ibox-content">
                    <?php foreach($listaMetaPpa as $contador => $meta): ?>
                    <?php
                        $infoMeta = (new Public_Model_MetaPpa($meta['mppid']))->recuperarInformacoes();
                        $mJanela = new Acompanhamento_Model_Janela($meta['janid']);
                        $infoPI = (new Pi_PlanoInterno())->recuperarResumoPorMetaESubunidade($meta['mppid'], $meta['suoid']);
                        $mAcompanhamento = new Acompanhamento_Model_Acompanhamento($meta['acoid']);
                        $listaDeAnexos = (new Acompanhamento_Model_AcompanhamentoArquivo())->buscarPorAcompanhamento($mAcompanhamento->acoid);
                        $subUnidade = new Public_Model_SubUnidadeOrcamentaria($meta['suoid']);
                        $dadosGrafico = [];
                        if(count($infoPI)){
                            foreach ($infoPI as $key => $valor) {
                                if(in_array($key, ['previsto', 'provisionado', 'empenhado', 'liquidado', 'pago'])){
                                    $dadosGrafico[$key]['descricao'] = ucfirst($key);
                                    $dadosGrafico[$key]['valor'] = $valor;
                                    $dadosGrafico[$key]['categoria'] = $infoMeta['mppcod'];
                                }
                            }
                        }
                    ?>

                        <div class="row form-horizontal">
                            <hr />
                            <div class="col-md-5 no-margin-bottom">
                                <?php
                                    $compartilhada =  $infoMeta['compartilhada'] == 't' ? ' <i style="font-size: 15px; color: #6892ff;" class="fa fa-handshake-o link" title="Meta Compartilhada com outras Subunidades."></i>' : '';

                                    $simec->setPodeEditar(0);

                                    $janinicio = Simec_Util::formatarData($mJanela->janinicio, 'dd/MM/YYYYY');
                                    $janfim = Simec_Util::formatarData($mJanela->janfim, 'dd/MM/YYYYY');
                                    echo $simec->input('Referência', 'Referência', "{$mJanela->jandsc} ({$janinicio} - {$janfim})");
                                    echo $simec->select('suocod', 'Subunidade', $subUnidade->suocod, (new Public_Model_SubUnidadeOrcamentaria())->queryCombo((object)['exercicio' => $_SESSION['exercicio']]));
                                    echo $simec->input('Objetivo', 'Objetivo', "{$infoMeta['oppcod']} - {$infoMeta['oppdsc']}");
                                    echo $simec->input('Meta', 'Meta', "{$infoMeta['mppcod']} - {$infoMeta['mppnome']} {$compartilhada}");
                                    echo $simec->input('Previsto', 'Previsto PPA', number_format($infoMeta['mppquantidade'], 2, ',', '.'));
                                    if($infoMeta['mppanualizavel'] == 't'){
                                        echo $simec->input('Previstoa', 'Previsto Anualizado', number_format($infoMeta['mppquantidadeanualizada'], 2, ',', '.'));
                                    }
                                    echo $simec->input('qtd_pis', 'Planos Internos (PI´s)', '<a class="detalhe-pi" title="PI\'s Vinculados" href="?modulo=principal/monitorar-ppa&acao=A&req=detalhe-pi&mppid=' . $mppid . '&suoid=' . $suoid . '">' . number_format($infoPI['qtd'], 0, ',', '.') . '</a>');
                                    $simec->setLabelSize(3);
                                ?>

                            </div>
                            <div class="col-md-5">
                                <h3 class="text-center">PI's Associados à Meta</h3>
                                <?php
                                $colors = "'#55BF3B', '#eeaaee', '#00BFFF', '#aaeeee', '#7798BF', '#DDDF0D', '#7CCD7C', '#DF5353', '#008000', '#CD0000', '#FF4500', '#ff0066', '#4B0082', '#808000', '#800000', '#2F4F4F', '#006400', '#FFA500'";
                                $grafico = new Grafico(Grafico::K_TIPO_COLUNA, false);
                                $grafico->setFormatoTooltip(Grafico::K_TOOLTIP_DECIMAL_0)
                                        ->setColors($colors)
                                        ->setHeight('250px')
                                        ->gerarGrafico($dadosGrafico);
                                ?>
                            </div>
                            <div class="col-sm-2">
                                <?php $percentual = $infoPI['previsto'] ? $infoPI['empenhado'] * 100 / $infoPI['previsto'] : 0; ?>
                                <h3 class="text-center">Percentual de Empenho</h3>
                                <meter style="display: none;" max="1000" value="<?php echo $percentual; ?>" class="percentual-empenhado"></meter>
                            </div>

                            <!-- Monitoramento -->
                            <div class="col-md-8">

                                    <div class="form-group ">
                                        <label for="acoquantidade" class="col-sm-2 col-md-2 col-lg-2 control-label">Executado: <span class="campo-obrigatorio" title="Campo obrigatório">*</span></label>
                                        <div class="col-sm-2 col-md-2 col-lg-2 ">
                                            <?php $acoquantidade = is_numeric($mAcompanhamento->acoquantidade) ? number_format($mAcompanhamento->acoquantidade, 2, ',', '.') : null; ?>
                                            <?php if($podeEditar){ ?>
                                                <input name="acoquantidade" id="acoquantidade" type="text" class="form-control  valor " value="<?php echo $acoquantidade; ?>" required="required">
                                            <?php } else { ?>
                                                <p class="form-control-static" id="acoquantidade"><?php echo $acoquantidade; ?></p>
                                            <?php } ?>

                                        </div>
                                        <label for="acodata" class="col-sm-2 col-md-2 col-lg-2 control-label">Dt. Referência: <span class="campo-obrigatorio" title="Campo obrigatório">*</span></label>
                                        <div class="col-sm-2 col-md-2 col-lg-2 <?php echo $podeEditar ? 'date' : '' ; ?>">
                                            <div class="input-group">
                                                <?php if($podeEditar){ ?>
                                                    <span class="input-group-addon"><span class="fa fa-calendar"></span></span>
                                                    <input name="acodata" id="acodata" type="text" value="<?php echo Simec_Util::formatarData($mAcompanhamento->acodata, Zend_Date::DATES); ?>" class="datemask form-control" required="required">
                                                <?php } else { ?>
                                                    <p class="form-control-static" id="acodata"><?php echo Simec_Util::formatarData($mAcompanhamento->acodata, Zend_Date::DATES); ?></p>
                                                <?php } ?>

                                            </div>
                                        </div>
                                        <div style="clear:both"></div>
                                    </div>

                                    <?php
                                        echo $simec->textarea('acoanalise', 'Análise Situacional', $mAcompanhamento->acoanalise, ['rows' => 4], ['label-size' => 2]);
                                        echo $simec->select('claid', 'Classificação', $mAcompanhamento->claid, (new Acompanhamento_Model_Classificacao())->recuperarSqlCombo(null, ['tipid = ' . Acompanhamento_Model_Tipo::K_TIPO_META_PPA]), ['required'], ['label-size' => 2]);

                                        $aMedidasSelecionadas = $mAcompanhamento->recuperarMedidas();
                                        echo '<span class="span-medida" id="span-medida" ' . (!count($aMedidasSelecionadas) ? 'style="display: none"' : '') . '>' . $simec->select('medidas[]', 'Medidas a serem adotadas', $aMedidasSelecionadas, (new Acompanhamento_Model_Medida())->recuperarSqlCombo(null, ['claid = ' . (int)$mAcompanhamento->claid]), [], ['label-size' => 2]) . '</span>';
                                        echo '<span class="span-medida" ' . (!count($aMedidasSelecionadas) ? 'style="display: none"' : '') . '>' . $simec->textarea('acoprovidencias', 'Detalhamento das providências a serem adotadas', $mAcompanhamento->acoprovidencias, ['rows' => 4], ['label-size' => 2]) . '</span>';
                                    ?>

                            </div>

                            <div class="col-md-4">

                                <div class="form-group">
                                    <div class="col-lg-12">
                                        <h5>Anexos</h5>
                                    </div>
                                    <div class="col-lg-12">
                                        <table id="table_anexos" cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                                            <tr>
                                                <td style="text-align: left;"><b>Nome do arquivo</b></td>
                                                <td style="text-align: left;"><b>Descrição</b></td>
                                            </tr>
                                            <?php if($listaDeAnexos): ?>
                                                <?php foreach($listaDeAnexos as $anexo): ?>
                                                    <tr style="height: 30px;" class="tr_anexos_<?php echo $anexo['arqid']; ?>" >
                                                        <td style="text-align: left;"><a href="#" onclick="javascript:abrirArquivo('<?php echo $anexo['arqid']; ?>'); return false;" ><?php echo $anexo['arqnome']. '.'. $anexo['arqextensao']; ?></a></td>
                                                        <td style="text-align: left;"><?php echo $anexo['arqdescricao']; ?></td>
                                                    </tr>
                                                <?php endforeach; ?>
                                            <?php else: ?>
                                                <tr style="height: 30px;" >
                                                    <td style="text-align: left;" colspan="2">Não foi anexado nenhum arquivo.</td>
                                                </tr>
                                            <?php endif; ?>
                                        </table>
                                    </div>
                                </div>
                            </div>


                        </div>
                    <?php endforeach; ?>
                </div>
            </div>
        </div>
    </div>
</div>

<script>

    $(document).ready(function(){

        $('.btn-limpar').click(function(){
            window.location = window.location;
        });

        // Evento ao mudar opção de UO
        $('#unicod').change(function(){
            carregarUG($(this).val());
        });

        // Evento ao mudar opção de Objetivos PPA
        $('#oppid').change(function(){
            carregarMetasPPA($(this).val(), null, $('#ungcod').val());
        });

        $('.percentual-empenhado').liquidMeter({
            shape: 'circle',
            color: '#0088cc',
            background: '#F9F9F9',
            fontSize: '24px',
            fontWeight: '600',
            stroke: '#F2F2F2',
            textColor: '#333',
            liquidOpacity: 0.9,
            liquidPalette: ['#333'],
            speed: 3000,
        });

    });

    /**
     * Carrega novo conteúdo para a opções de Sub-Unidade via requisição ajax.
     *
     */
    function carregarUG(unicod) {
        $.post(
            '?modulo=relatorio/relatorio-ppa&acao=A&req=carregarComboUG', { unicod: $('#unicod').val() }, function(response) {
                $('#div_ungcod').remove('slow');
                $('#div_ungcod').html(response);
                $(".chosen-select").chosen();
        });
    }

    /**
     * Carrega novo conteúdo para o select de Metas PPA via requisição ajax.
     */
    function carregarMetasPPA(oppid, mppid, suocod) {
        $.post('?modulo=relatorio/relatorio-ppa&acao=A&req=carregarMetasPPA&oppid=' +oppid +'&suocod='+ suocod, function(response) {
            $('#mppid').remove();
            $('.div_mppid').html(response);
            $(".chosen-select").chosen();
        });
    }

</script>
