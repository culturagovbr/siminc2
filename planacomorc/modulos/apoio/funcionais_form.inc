<?php

    $cPtres = new Monitora_Controller_Ptres();
    switch ($_REQUEST['req']) {
        case 'salvar':
            $cPtres->salvar($_REQUEST);
            die;
        case 'excluir':
            $cPtres->excluir($_REQUEST['ptrid']);
            die;
        case 'carregarAcaoPorUo':
            echo $simec->select('acaid', 'Ação', $mPtres->acaid, (new Monitora_Model_Acao())->recuperarSqlCombo(['prgcod', 'acacod', 'acaobjetivocod', 'loccod', 'acatitulo'], ["unicod = '". $_REQUEST['unicod']. "'"]), ['required']);
            die;
    }

    $mPtres = new Monitora_Model_Ptres($_REQUEST['ptrid']);

    include APPRAIZ . "includes/cabecalho.inc";
?>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-10">
        <h2><?php echo $titulo_modulo; ?></h2>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <div class="col-md-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Dados Gerais</h5>
                </div>
                <div class="ibox-content">
                    <form id="formulario" name="formulario" method="post" class="form-horizontal">
                        <input type="hidden" name="req" id="req" value="salvar" />
                        <input name="ptrid" id="ptrid" type="hidden" value="<?php echo $mPtres->ptrid; ?>">

                        <?php
                            echo $simec->select('unicod', 'Unidade Orçamentária', $mPtres->unicod, (new Public_Model_UnidadeOrcamentaria())->recuperarSqlComboPorUnocod(['unocod', 'unonome'], null, 'unonome'), ['required']);
                            # select de unidade para filtrar a ação com base do unicod.
                            echo '<span id="span-acao">';
                            echo $simec->select('acaid', 'Ação', $mPtres->acaid, (new Monitora_Model_Acao())->recuperarSqlCombo(['prgcod', 'acacod', 'acaobjetivocod', 'loccod', 'acatitulo'], ["unicod = '". $mPtres->unicod. "'"]), ['required']);
                            echo '</span>';
                            echo $simec->input('ptres', 'PTRES', $mPtres->ptres, ['required','maxlength' => 50]);
                            echo $simec->input('plocod', 'Código PO', $mPtres->plocod, ['maxlength' => 4]);
                            echo $simec->input('plodsc', 'Descrição PO', $mPtres->plodsc, ['maxlength' => 500]);
                            echo $simec->input('irpcod', 'RP', $mPtres->irpcod, ['required', 'maxlength' => 1]);
//                            echo $simec->valor('ptrdotacaocusteio', 'Custeio', $mPtres->ptrdotacaocusteio, ['required']);
//                            echo $simec->valor('ptrdotacaocapital', 'Capital', $mPtres->ptrdotacaocapital, ['required']);
                            echo $simec->textarea('ptrnomeprodutopo', 'Nome do Produto do PO', $mPtres->ptrnomeprodutopo, ['maxlength' => 200, 'cols'=>30, 'rows'=>3]);
                            echo $simec->textarea('ptrnomeunidadepo', 'Nome da Unidade de Medida do PO', $mPtres->ptrnomeunidadepo, ['maxlength' => 200, 'cols'=>30, 'rows'=>3]);
                        ?>
                        
                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-primary" type="submit" id="btn-salvar"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                                <a href="?modulo=apoio/funcionais&acao=A" class="btn btn-warning" id="btn-voltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
                                <?php if($mPtres->ptrid){ ?>
                                    <a href="?modulo=apoio/funcionais_form&acao=A&req=excluir&ptrid=<?php echo $mPtres->ptrid; ?>" class="btn btn-danger link-excluir" id="btn-excluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
                                <?php } ?>                                
                            </div>
                        </div>                        
                    </form>
                </div>
            </div>
        </div>
    </div>
</div>

<script>

    $(document).ready(function() {

        $('#unicod').change(function () {
            $('#span-acao').load('?modulo=apoio/funcionais_form&acao=A&req=carregarAcaoPorUo&unicod=' + $(this).val());
        });

    });

</script>