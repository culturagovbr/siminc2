<?php
    $suoid = (int)$_REQUEST['suoid'];
    $mpnid = (int)$_REQUEST['mpnid'];

    $listaPI = (new Pi_PlanoInterno())->recuperarValoresPiMetaPncESubunidade($mpnid, $suoid);
    $metaPnc = new Public_Model_MetaPnc($mpnid);
    $valoresOrcamentarios = (new Pi_PlanoInterno())->recuperarValoresPiMetaPncESubunidade($mpnid, $suoid);

?>

<div class="col-sm-12">
    <div class="col-md-8">
        <h3 class="text-center">PI's Associados à Meta</h3>
        <?php
            $dadosGrafico = [];
            if(count($listaPI)){
                foreach ($listaPI as $key => $valor) {
                    if(in_array($key, ['previsto', 'provisionado', 'empenhado', 'liquidado', 'pago'])){
                        $dadosGrafico[$key]['descricao'] = ucfirst($key);
                        $dadosGrafico[$key]['valor'] = $valor;
                        $dadosGrafico[$key]['categoria'] = $metaPnc->mpncod;
                    }
                }
            }

            $colors = "'#55BF3B', '#eeaaee', '#00BFFF', '#aaeeee', '#7798BF', '#DDDF0D', '#7CCD7C', '#DF5353', '#008000', '#CD0000', '#FF4500', '#ff0066', '#4B0082', '#808000', '#800000', '#2F4F4F', '#006400', '#FFA500'";
            $grafico = new Grafico(Grafico::K_TIPO_COLUNA, false);
            $grafico->setFormatoTooltip(Grafico::K_TOOLTIP_DECIMAL_0)
                ->setColors($colors)
                ->setHeight('250px')
                ->gerarGrafico($dadosGrafico);
        ?>
    </div>
    <div class="col-sm-2">
        <?php $percentual = $valoresOrcamentarios['previsto']? $valoresOrcamentarios['empenhado'] * 100 / $valoresOrcamentarios['previsto'] : 0; ?>
        <h3 class="text-center">Percentual de Empenho</h3>
        <meter style="display: none;" max="1000" value="<?php echo $percentual; ?>" id="percentual-empenhado"></meter>
    </div>
</div>

<script>
    $(function(){

        $('#percentual-empenhado').liquidMeter({
            shape: 'circle',
            color: '#0088cc',
            background: '#F9F9F9',
            fontSize: '24px',
            fontWeight: '600',
            stroke: '#F2F2F2',
            textColor: '#333',
            liquidOpacity: 0.9,
            liquidPalette: ['#333'],
            speed: 3000
        });

    });
</script>