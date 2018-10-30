<?php

$perfis = pegaPerfilGeral();

$mEmenda = new Emendas_Model_Emenda($_REQUEST['emeid']);

$podeEditar = !in_array(PFL_SUBUNIDADE, $perfis)? TRUE: FALSE;

$cEmenda = new Emendas_Controller_Emenda();
switch ($_REQUEST['req']) {
    case 'salvar':
        $cEmenda->salvar($_REQUEST);
        die;
    case 'excluir':
        $cEmenda->excluir($_REQUEST['emeid']);
        die;
    case 'carregarAcaoPorUo':
        $sqlPtres = (new Monitora_Model_Ptres())->recuperarSqlCombo([
            "'(Ptres: ' || ptres.ptres || ')'",
            "coalesce(ptres.prgcod, '')",
            "coalesce(ptres.acacod, '')",
            "coalesce(ptres.loccod, '')",
            "coalesce(acaobjetivocod, '')",
            "coalesce(ptres.plocod, '')",
            "acatitulo"
            ], 
            [
                "plocod = 'EIND'",
                "unoid = '". (int)$_REQUEST['unoid']. "'"
            ]);
        echo $simec->select('ptrid', 'Funcional <span class="campo-obrigatorio" title="Campo obrigatório">*</span>', null, $sqlPtres);
        die;
    case 'formulario-detalhe':
        $cEmenda->montarFormularioDetalhes($_REQUEST['emeid'], $_REQUEST['emdid']);
        die;
    case 'listar-detalhe':
        $cEmenda->montarListagemDetalhes($mEmenda->emeid);
        die;
    case 'salvar-detalhe':
        $cEmendaDetalhe = new Emendas_Controller_EmendaDetalhe();
        $cEmendaDetalhe->salvar($_POST);
        die;
    case 'excluir-detalhe':
        $cEmendaDetalhe = new Emendas_Controller_EmendaDetalhe();
        echo $cEmendaDetalhe->excluir($_REQUEST['emdid']);
        die;
}

include APPRAIZ . "includes/cabecalho.inc";
?>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-10">
        <h2><?php echo $titulo_modulo; ?></h2>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <div class="col-md-8">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Dados Gerais</h5>
                </div>
                <div class="ibox-content">
                    <form id="formulario" name="formulario" method="post" class="form-horizontal">
                        <input type="hidden" name="req" id="req" value="salvar" />
                        <input name="emeid" id="emeid" type="hidden" value="<?php echo $mEmenda->emeid; ?>">

                        <?php
                            $simec->setPodeEditar($podeEditar);
                            echo $simec->input('emenumero', 'Número', $mEmenda->emenumero, ['class'=>'inteiro']);
                            echo $simec->select('autid', 'Autor', $mEmenda->autid, (new Emendas_Model_Autor())->recuperarSqlCombo(['autcod', 'autnome'], null, 'autnome'));
                            echo $simec->select('unoid', 'Unidade <span class="campo-obrigatorio" title="Campo obrigatório">*</span>', $mEmenda->unoid, (new Public_Model_UnidadeOrcamentaria())->recuperarSqlCombo(['unocod', 'unonome'], null, 'unonome'));
                            $sqlPtres = (new Monitora_Model_Ptres())->recuperarSqlCombo([
                                "'(Ptres: ' || ptres.ptres || ')'",
                                "coalesce(ptres.prgcod, '')",
                                "coalesce(ptres.acacod, '')",
                                "coalesce(ptres.loccod, '')",
                                "coalesce(acaobjetivocod, '')",
                                "coalesce(ptres.plocod, '')",
                                "acatitulo"
                                ], [
                                    "plocod = 'EIND'",
                                    "unoid = '". (int)$mEmenda->unoid. "'"
                                ]);
                            echo '<span id="span-funcional">'.
                                $simec->select('ptrid', 'Funcional <span class="campo-obrigatorio" title="Campo obrigatório">*</span>', $mEmenda->ptrid, $sqlPtres).
                            '</span>';
                            echo $simec->radio('emeimpositiva', 'Impositiva', $mEmenda->emeimpositiva, ['t'=>'Sim', 'f'=>'Não']);
                            echo $simec->textarea('emejustificativa', 'Justificativa', $mEmenda->emejustificativa, ['maxlength' => 2000]);
                        ?>

                        <?php if($podeEditar): ?>
                            <div class="form-group">
                                <div class="text-center">
                                    <button class="btn btn-primary" type="submit" id="btn-salvar"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                                    <a href="?modulo=principal/emenda&acao=A" class="btn btn-warning" id="btn-voltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
                                    <?php if($mEmenda->emeid){ ?>
                                        <a href="?modulo=principal/emenda_form&acao=A&req=excluir&emeid=<?php echo $mEmenda->emeid; ?>" class="btn btn-danger link-excluir" id="btn-excluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
                                    <?php } ?>
                                </div>
                            </div>
                        <?php endif; ?>

                    </form>
                </div>
            </div>
        </div>
        <div class="col-md-4">
            <div class="ibox float-e-margins">
            	<div class="ibox-title">
            		<h5>Dados Financeiros</h5>
            	</div>
            	<div class="ibox-content" id="">
                    <div id="div_formulario_detalhe">
                        <?php
                            if($mEmenda->emeid){
                                $cEmenda->montarFormularioDetalhes($mEmenda->emeid);
                            }
                        ?>
                    </div>
                    <div id="div_listagem_detalhe">
                        <?php $cEmenda->montarListagemDetalhes($mEmenda->emeid); ?>
                    </div>
            	</div>
            </div>
        </div>

        <div class="ibox float-e-margins">
        	<div class="ibox-title">
                    <h5>Beneficiários</h5>
        	</div>
        	<div class="ibox-content">
                <?php if($mEmenda->emeid){ ?>
                    <?php if($podeEditar): ?>
                        <a class="btn-small btn btn-success" href="?modulo=principal/beneficiario_form&acao=A&emeid=<?php echo $mEmenda->emeid; ?>">
                            <span class="glyphicon glyphicon-plus"></span>
                            Novo
                        </a>
                    <?php endif; ?>

                    <?php $aBeneficiario = (new Emendas_Model_Beneficiario())->recuperarTodos('*', ['emeid = ' . $mEmenda->emeid, "benstatus = 'A'"]);
                    if(count($aBeneficiario)){ ?>

                        <div class="table-responsive">
                            <table class="table table-striped table-bordered table-hover" >
                                <thead>
                                <tr class="text-center">
                                    <th width="10%">Ações</th>
                                    <th>Proponente</th>
                                    <th>Unidade</th>
                                    <th>N° Proposta</th>
                                    <th>Situação Siconv</th>
                                    <th>Custeio</th>
                                    <th>Capital</th>
                                    <th>Empenhado</th>
                                    <th>Pago</th>
                                </tr>
                                </thead>
                                <tbody>
                                <?php foreach($aBeneficiario as $dados){ ?>
                                    <tr>
                                        <td class="text-center">
                                            <a title="Alterar" href="?modulo=principal/beneficiario_form&acao=A&benid=<?php echo $dados['benid']; ?>"><i class="btn btn-primary btn-sm glyphicon glyphicon-pencil"></i></a>
                                        </td>
                                        <td><?php echo $dados['procnpj'] . ' - ' . $dados['pronome']; ?></td>
                                        <td><?php echo $dados['suocod'] . ' - ' . $dados['suonome']; ?></td>
                                        <td><?php echo $dados['numero_proposta']; ?></td>
                                        <td><?php echo $dados['sitdsc']; ?></td>
                                        <td><?php echo number_format($dados['custeio'], 2, ',', '.'); ?></td>
                                        <td><?php echo number_format($dados['capital'], 2, ',', '.'); ?></td>
                                        <td><?php echo number_format($dados['empenhado'], 2, ',', '.'); ?></td>
                                        <td><?php echo number_format($dados['pago'], 2, ',', '.'); ?></td>
                                    </tr>
                                    <?php
                                    $totalcusteio += $dados['custeio'];
                                    $totalcapital += $dados['capital'];
                                    $totalempenhado += $dados['empenhado'];
                                    $totalpago += $dados['pago'];
                                    ?>
                                <?php } ?>
                                <td colspan="5" style="text-align: right"><h5>Total</h5></td>
                                <td style="vertical-align: middle"><?php echo number_format($totalcusteio, 2, ',', '.'); ?></td>
                                <td style="vertical-align: middle"><?php echo number_format($totalcapital, 2, ',', '.'); ?></td>
                                <td style="vertical-align: middle"><?php echo number_format($totalempenhado, 2, ',', '.'); ?></td>
                                <td style="vertical-align: middle"><?php echo number_format($totalpago, 2, ',', '.'); ?></td>
                                </tbody>
                            </table>
                        </div>
                    <?php } else {
                        echo '<div class="alert alert-danger">Nenhum beneficiário cadastrado</div>';
                    }
                } else {
                    echo '<div class="alert alert-danger">Nenhum beneficiário cadastrado</div>';
                }
            ?>

            </div>
        </div>
    </div>
</div>

<script>
    $(function(){
        $('#unoid').change(function(){
            $('#span-funcional').load('?modulo=principal/emenda_form&acao=A&req=carregarAcaoPorUo&unoid=' + $(this).val());
        });
    })
</script>