<div class="ibox-content">
    <form id="filtroalteracao" name="filtroalteracao" method="POST" role="form" class="form-horizontal">
        <input name="requisicao" id="requisicao" value="" type="hidden">

        <div class="row">
            <div class="col-lg-3">
                <?= $simec->input('altid',  'Nº do Pedido', $filtroalteracao['altid'], ['maxlength' => 12, 'class' => 'inteiro'], ['input-size' => 6, 'label-size' => 5]); ?>
            </div>
            <div class="col-lg-3">
                <?= $simec->select('sitid', 'Situação', $filtroalteracao['sitid'], (new Public_Model_Workflow())->recuperarSqlCombo(['esddsc'], ["tpdid=".$tpdid]), null,  ['input-size' => 9, 'label-size' => 2]); ?>
            </div>
            <div class="col-lg-6">
                <?= $simec->select('suoid[]', 'Unidades', $filtroalteracao['suoid'], (new Public_Model_SubUnidadeOrcamentaria())->queryComboChaveSuoid((object)array('exercicio' => $_SESSION['exercicio'], 'listaSubUnidadeUsuario' => $listaSubUnidadeUsuario)), null, ['input-size' => 9, 'label-size' => 2]); ?>
            </div>
        </div>

        <div class="row">
            <div class="col-lg-3">
                <?= $simec->select('tpaid', 'Tipo', $filtroalteracao['tpaid'], (new Alteracao_Model_Tipo())->recuperarSqlCombo('tpacod'), null,  ['input-size' => 6, 'label-size' => 5]); ?>
            </div>
            <div class="col-lg-3">
                <?= $simec->select('janid', 'Janela', $filtroalteracao['janid'], (new Alteracao_Model_Janela())->recuperarSqlCombo(), null,  ['input-size' => 9, 'label-size' => 2]); ?>
            </div>
            <div class="col-lg-6">
                <?= $simec->input('altid',  'Título', $filtroalteracao['altid'], NULL, ['input-size' => 9, 'label-size' => 2]); ?>
            </div>
        </div>

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