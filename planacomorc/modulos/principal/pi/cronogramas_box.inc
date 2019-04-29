<div id="div_cronogramas">
    <div class="ibox-title">
        <h5>Cronogramas </h5>
    </div>
    <div class="ibox-content">

        <div class="form-group">
            <div class="col-lg-12">
                <?php
                $sql = "SELECT mescod, mesdsc FROM public.meses ORDER BY mescod";
                $meses = $db->carregar($sql);

                $modelPiCronograma =  new Pi_Cronograma();

                $piCronogramasAgrupado = [];
                if(!empty($dadosPI['pliid'])){
                    $aPiCronograma = $modelPiCronograma->recuperarTodos('*', array('pliid = ' . $dadosPI['pliid']));

                    foreach($aPiCronograma as $piCronograma){
                        $piCronogramasAgrupado[$piCronograma['mescod']][$piCronograma['crvid']] = $piCronograma;
                    }
                }

                $aCronogramas = $modelPiCronograma->recuperarCronogramas();

                $cronogramasAgrupado = [];
                foreach($aCronogramas as $cronograma){
                    $cronogramasAgrupado[$cronograma['crodsc']][] = $cronograma;
                }
                ?>
                <table cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover table-input">
                    <tr>
                        <td rowspan="2" style="text-align: center; border-right: 1px black solid;">
                            <b>Mês</b>
                        </td>
                        <?php 
                            $colunaCronograma = 0;
                            foreach($cronogramasAgrupado as $cronograma => $descricaoValores){ ?>
                                <td style="text-align: center; border-right: 1px black solid;" colspan="<?php echo count($descricaoValores); ?>" class="<?php echo buscarClasseCronograma($colunaCronograma, TRUE); ?>">
                                    <b><?php echo $cronograma; ?></b>
                                </td>
                        <?php
                            $colunaCronograma++;
                        }
                        ?>
                    </tr>
                    <tr style="height: 30px;">
                        <?php
                            $colunaCronograma = 0;
                            foreach($cronogramasAgrupado as $cronograma => $descricaoValores){
                                foreach($descricaoValores as $cronogramaValor){
                                    $bordaPreta = in_array($cronogramaValor['crvid'], array(Cronograma::K_FISICO, Cronograma::ORCAMENTO_CAPITAL, Cronograma::FINANCEIRO_CAPITAL))? 'border-right: 1px black solid': NULL;
                            ?>
                                <td style="text-align: center; <?php echo $bordaPreta; ?>" class="<?php echo buscarClasseCronograma($colunaCronograma, TRUE); ?>">
                                    <?php echo $cronogramaValor['crvdsc']; ?>
                                </td>
                            <?php
                                    $colunaCronograma++;
                                }
                        }
                        ?>
                    </tr>
                    <?php foreach($meses as $mes){ ?>
                        <tr style="height: 30px;">
                            <td style="text-align: left; border-right: 1px black solid;">
                                <?php echo $mes['mesdsc']; ?>
                            </td>
                            <?php foreach($aCronogramas as $cronogramaValor){
                                switch($cronogramaValor['croid']){
                                    case Cronograma::K_FISICO:          $class = 'input_fisico inteiro';        break;
                                    case Cronograma::K_ORCAMENTARIO:    $class = 'input_orcamentario';  break;
                                    case Cronograma::K_FINANCEIRO:      $class = 'input_financeiro';    break;
                                    default: $class = '';
                                }
                                $bordaPreta = in_array($cronogramaValor['crvid'], array(Cronograma::K_FISICO, Cronograma::ORCAMENTO_CAPITAL, Cronograma::FINANCEIRO_CAPITAL))? 'border-right: 1px black solid': NULL;
                            ?>
                            <td class="<?php echo buscarClasseCronograma($cronogramaValor['croid']); ?>" style="<?php echo $bordaPreta; ?>">
                                    <?php
                                        $pcrid = $pcrvalor = '';
                                        if(isset($piCronogramasAgrupado[$mes['mescod']][$cronogramaValor['crvid']])){
                                            $pcrid = $piCronogramasAgrupado[$mes['mescod']][$cronogramaValor['crvid']]['pcrid'];
                                            $pcrvalor = $piCronogramasAgrupado[$mes['mescod']][$cronogramaValor['crvid']]['pcrvalor'];

                                            if($cronogramaValor['croid'] != Cronograma::K_FISICO){
                                                $pcrvalor = number_format($pcrvalor, 2, ',', '.');
                                            }
                                        }

                                        switch ($cronogramaValor['crvid']) {
                                            case Cronograma::ORCAMENTO_CUSTEIO:
                                                $class .= ' custeio';
                                            break;
                                            case Cronograma::FINANCEIRO_CUSTEIO:
                                                $class .= ' custeio';
                                            break;
                                            case Cronograma::ORCAMENTO_CAPITAL:
                                                $class .= ' capital';
                                            break;
                                            case Cronograma::FINANCEIRO_CAPITAL:
                                                $class .= ' capital';
                                            break;
                                        }
                                    ?>
                                    <input name="cronograma[<?php echo $mes['mescod']; ?>][<?php echo $cronogramaValor['crvid']; ?>][pcrid]" value="<?php echo $pcrid; ?>" type="hidden">
                                    <input name="cronograma[<?php echo $mes['mescod']; ?>][<?php echo $cronogramaValor['crvid']; ?>][pcrvalor]" value="<?php echo $pcrvalor; ?>" class="CampoEstilo normal form-control obrigatorio <?php echo $class ?>" type="text">
                                </td>
                            <?php } ?>
                        </tr>
                    <?php } ?>
                    <tr style="height: 30px; ">
                        <td style="text-align: left; border-right: 1px black solid;">Total:</td>
                        <td style="font-weight: bold; text-align: center; border-right: 1px black solid;" id="td_total_fisico" class="td_cronograma_fisico">0</td>
                        <td style="font-weight: bold; text-align: center;" id="td_total_orcamentario_custeio" class="td_cronograma_orcamentario">R$ 0,00</td>
                        <td style="font-weight: bold; text-align: center; border-right: 1px black solid;" id="td_total_orcamentario_capital" class="td_cronograma_orcamentario">R$ 0,00</td>
                        <td style="font-weight: bold; text-align: center;" id="td_total_financeiro_custeio" class="td_cronograma_financeiro">R$ 0,00</td>
                        <td style="font-weight: bold; text-align: center; border-right: 1px black solid;" id="td_total_financeiro_capital" class="td_cronograma_financeiro">R$ 0,00</td>
                    </tr>
                </table>
            </div>
        </div>

    </div>
</div>
