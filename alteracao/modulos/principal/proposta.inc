<?php
$filtroprop = $_SESSION['filtroprop'] = ($_POST? $_POST: $_SESSION['filtroprop']);
switch ($_REQUEST['requisicao']) {
    case 'limpar':
        $filtroprop = $_SESSION['filtroprop'] = array();
        break;    
}
$listaSubUnidadeUsuario = buscarSubUnidadeUsuario((object) array('usucpf' => $_SESSION['usucpf']));

$aProposta = (new Proposta_Model_Proposta)->recuperarListagem((object) array(
    'exercicio' => $_SESSION['exercicio'],
    'listaSubUnidadeUsuario' => $listaSubUnidadeUsuario
), $filtroprop);

$requisicao = $_REQUEST['requisicao'];
//ver($_REQUEST,d);



include APPRAIZ . "includes/cabecalho.inc";
?>
<script src="js/principal/proposta.js?v=0"></script>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <div class="col-md-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Pesquisa avançada por Proposta</h5>
                </div>
                <div class="ibox-content">
                    <form id="filtroprop" name="filtroprop" method="POST" role="form" class="form-horizontal">
                        <input name="requisicao" id="requisicao" value="" type="hidden">

                        <?php
                            echo $simec->select('suoid','SubUnidade',$filtroprop['suoid'],(new Public_Model_SubUnidadeOrcamentaria())->queryComboChaveSuoid((object)array('unofundo' => 'FALSE','exercicio' => $_SESSION['exercicio'],'listaSubUnidadeUsuario' => $listaSubUnidadeUsuario)));
                            echo $simec->select('eqdid', 'Enquadramento da Despesa', $filtroprop['eqdid'], (new Planacomorc_Model_EnquadramentoDespesa())->recuperarSqlCombo());
                            echo '<span id="span-funcional">'.$simec->select('ptrid', 'Funcional', $filtroprop['ptrid'], (new Proposta_Model_Proposta())->recuperarSqlFuncional($filtroprop['suoid'], '')).'</span>';
                        ?>

                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-warning btn-limpar" type="button">
                                    <span class="glyphicon glyphicon-remove-circle"></span> Limpar
                                </button>
                                <button class="btn btn-primary btn-buscar" type="submit">
                                    <span class="glyphicon glyphicon-search"></span> Buscar
                                </button>
                                <button class="btn btn-success btn-novo" type="button">
                                    <span class="glyphicon glyphicon-plus"></span>
                                    Novo
                                </button>
                            </div>
                        </div>

                    </form>
                </div>
            </div>
        </div>
    </div>
    <div class="row">
        <div class="col-md-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Dados Gerais</h5>
                </div>
                <div class="ibox-content">
                    <div class="table-responsive">
                        <table class="table table-striped table-bordered table-hover dataTables" >
                            <tbody>
                            <?php
                                # Inicialização das variáveis do totalizador dos dados.
                                $totalQtdPo = 0;
                                $totalQtdExp = 0;
                                $totalCustoT = 0;
                                $totalCustoExp = 0;
                                
                            foreach($aProposta as $dados){ ?>
                                <tr>
                                    <td class="text-center">
                                        <a title="Alterar" href="?modulo=principal/proposta_form&acao=A&proid=<?php echo $dados['proid']; ?>"><i class="fa fa-pencil"></i></a>
                                    </td>
                                    <td><?= $dados['proid']; ?></td>
                                    <td><?= $dados['subunidade']; ?></td>
                                    <td><?= $dados['funcional'] . ' - ' . $dados['acatitulo'] . ' - ' . $dados['plodsc']; ?></td>
                                    <td><?= $dados['eqddsc']; ?></td>
                                    <td align="right"><?= number_format($dados['proquantidade'], 0, ',', '.'); ?></td>
                                    <td align="right"><?= number_format($dados['proquantidadeexpansao'], 0, ',', '.'); ?></td>
                                    <td align="right"><?= number_format($dados['prdvalor'], 0, ',', '.'); ?></td>
                                    <td align="right"><?= number_format($dados['prdvalorexpansao'], 0, ',', '.'); ?></td>
                                </tr>
                            <?php
                                # Soma dos Totalizadores.
                                $totalQtdPo += (int)$dados['proquantidade'];
                                $totalQtdExp += (int)$dados['proquantidadeexpansao'];
                                $totalCustoT += (int)$dados['prdvalor'];
                                $totalCustoExp += (int)$dados['prdvalorexpansao'];
                            ?>
                            <?php } ?>
                            </tbody>
                            <thead>
                            <tr class="text-center">
                                <th width="4%">Ações</th>
                                <th>Id</th>
                                <th>Subunidade</th>
                                <th>Funcional</th>
                                <th>Enquadramento de Despesa</th>
                                <th>Quantidade PO</th>
                                <th>Quantidade Expansão</th>
                                <th>Valor Total</th>
                                <th>Valor Expansão</th>
                            </tr>
                            <tr style="vertical-align: middle" class="text-right">
                                <th colspan="5" style="text-align: left"><h5>Total</h5></th>
                                <th><?= number_format($totalQtdPo , 0, ',', '.'); ?></th>
                                <th><?= number_format($totalQtdExp , 0, ',', '.'); ?></th>
                                <th><?= number_format($totalCustoT,0,',','.'); ?></th>
                                <th><?= number_format($totalCustoExp,0,',','.'); ?></th>
                            </tr>
                            </thead>
                            <tfoot>
                                <tr style="vertical-align: middle" class="text-right">
                                    <th colspan="5" style="text-align: left"><h5>Total</h5></th>
                                    <th><?= number_format($totalQtdPo , 0, ',', '.'); ?></th>
                                    <th><?= number_format($totalQtdExp , 0, ',', '.'); ?></th>
                                    <th><?= number_format($totalCustoT,0,',','.'); ?></th>
                                    <th><?= number_format($totalCustoExp,0,',','.'); ?></th>
                                </tr>
                            </tfoot>
                        </table>

                    </div>
                </div>
            </div>
        </div>
    </div>
</div>

<script type="text/javascript">

    $(document).ready(function(){
        initListaProposta();
        
    });
</script>