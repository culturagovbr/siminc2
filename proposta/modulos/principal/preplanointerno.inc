<?php

$requisicao = $_REQUEST['requisicao'];
$filtroprepi = $_SESSION['filtroprepi'] = ($_POST? $_POST: $_SESSION['filtroprepi']);
if($requisicao) {
    switch ($requisicao) {
        case 'limpar':
            $filtroprepi = $_SESSION['filtroprepi'] = array();
        break;
    }
}
$listaSubUnidadeUsuario = buscarSubUnidadeUsuario((object) array('usucpf' => $_SESSION['usucpf']));
$aPrePlanoInterno = (new Proposta_Model_PrePlanoInterno)->recuperarListagem((object) array(
    'exercicio' => $_SESSION['exercicio'],
    'listaSubUnidadeUsuario' => $listaSubUnidadeUsuario
), $filtroprepi);

include APPRAIZ . "includes/cabecalho.inc";
?>


<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <div class="col-md-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Pesquisa avançada por PI</h5>
                </div>
                <div class="ibox-content">
                    <form id="filtropi" name="filtropi" method="POST" role="form" class="form-horizontal">
                        <input name="requisicao" id="requisicao" value="" type="hidden">

                        <?php 
                        echo $simec->select('suoid','SubUnidade',$filtroprepi['suoid'],(new Public_Model_SubUnidadeOrcamentaria())->queryComboChaveSuoid((object)array('unofundo' => 'FALSE','exercicio' => $_SESSION['exercicio'],'listaSubUnidadeUsuario' => $listaSubUnidadeUsuario)));
                        echo $simec->select('eqdid', 'Enquadramento da Despesa', $filtroprepi['eqdid'], (new Monitora_Model_PiEnquadramentoDespesa())->recuperarSqlCombo());
                        echo $simec->select('esdid', 'Situação', $filtroprepi['esdid'], Spo_Model_Unidade::queryComboEstados((object) array('tpdid' => 268)), array('multiple' => 'true'));
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
                                <!--<a class="btn btn-small btn-warning" href="?modulo=principal/preplanointerno_form&acao=A">Novo</a>-->
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
                                $totalQtd       = 0;
                                $totalVCusteio  = 0;
                                $totalVCapital  = 0;
                                $totalCustExp   = 0;
                                $totalVCapExp   = 0;
                            ?>
                            <?php foreach($aPrePlanoInterno as $dados){ ?>
                                <?php
                                    # Soma dos Totalizadores.
                                    $totalQtd       += (int)$dados['pliquantidade'];
                                    $totalVCusteio  += (int)$dados['plivalorcusteio'];
                                    $totalVCapital  += (int)$dados['plivalorcapital'];
                                    $totalCustExp   += (int)$dados['plivalorcusteioadicional'];
                                    $totalVCapExp   += (int)$dados['plivalorcapitaladicional'];
                                ?>
                                <tr>
                                    <td class="text-center">
                                        <a title="Alterar" href="?modulo=principal/preplanointerno_form&acao=A&pliid=<?php echo $dados['pliid']; ?>"><i class="btn btn-primary btn-sm glyphicon glyphicon-pencil"></i></a>
                                    </td>
                                    <td><?php echo $dados['pliid']; ?></td>
                                    <td><?php echo $dados['subunidade']; ?></td>
                                    <td><?php echo $dados['funcional'] . ' - ' . $dados['acatitulo'] . ' - ' . $dados['plodsc']; ?></td>
                                    <td><?php echo $dados['plititulo']; ?></td>
                                    <td><?php echo $dados['eqddsc']; ?></td>
                                    <td><?php echo $dados['esddsc']; ?></td>
                                    <td align="right"><?php echo number_format($dados['pliquantidade'],0,',', '.'); ?></td>
                                    <td align="right"><?php echo number_format($dados['plivalorcusteio'],0,',', '.'); ?></td>
                                    <td align="right"><?php echo number_format($dados['plivalorcapital'],0,',', '.'); ?></td>
                                    <td align="right"><?php echo number_format($dados['plivalorcusteioadicional'],0,',', '.'); ?></td>
                                    <td align="right"><?php echo number_format($dados['plivalorcapitaladicional'],0,',', '.'); ?></td>
                                </tr>

                            <?php } ?>
                            </tbody>
                            <thead>
                                <tr class="text-center">
                                    <th width="5%">Ação</th>
                                    <th>Id</th>
                                    <th>Subunidade</th>
                                    <th>Funcional</th>
                                    <th>Título</th>
                                    <th>Enquadramento</th>
                                    <th>Estado Atual</th>
                                    <th>Quantidade</th>
                                    <th>Valor Custeio</th>
                                    <th>Valor Capital</th>
                                    <th>Valor Custeio Exp.</th>
                                    <th>Valor Capital Exp.</th>
                                </tr>
                                <tr style="vertical-align: middle" class="text-right">
                                    <th colspan="7" style="text-align: left"><h5>Total</h5></th>
                                    <th><?php echo number_format($totalQtd , 0, ',', '.'); ?></th>
                                    <th><?php echo number_format($totalVCusteio , 0, ',', '.'); ?></th>
                                    <th><?php echo number_format($totalVCapital,0,',','.'); ?></th>
                                    <th><?php echo number_format($totalCustExp,0,',','.'); ?></th>
                                    <th><?php echo number_format($totalVCapExp,0,',','.'); ?></th>
                                </tr>
                            </thead>
                            <tfoot>
                                <tr style="vertical-align: middle" class="text-right">
                                    <th colspan="7" style="text-align: left"><h5>Total</h5></th>
                                    <th><?php echo number_format($totalQtd , 0, ',', '.'); ?></th>
                                    <th><?php echo number_format($totalVCusteio , 0, ',', '.'); ?></th>
                                    <th><?php echo number_format($totalVCapital,0,',','.'); ?></th>
                                    <th><?php echo number_format($totalCustExp,0,',','.'); ?></th>
                                    <th><?php echo number_format($totalVCapExp,0,',','.'); ?></th>
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
    sufixoUrl = '';

    $(document).ready(function(){
        $(".btn-limpar").click(function(){
            $('#requisicao').val('limpar');
            $('#filtropi').submit();            
        });
        $(".btn-novo").click(function(){
            window.location.href='?modulo=principal/preplanointerno_form&acao=A';
        });
    });
</script>
