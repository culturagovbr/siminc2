<?php
require_once APPRAIZ . "includes/library/simec/Helper/FlashMessage.php";
require_once APPRAIZ . "www/planacomorc/_funcoesVinculacaoOrcamentaria.php";
require_once "vinculacaoOrcamentariaAcao.inc";

$urlCall = 'planacomorc.php?modulo=sistema/tabelasapoio/cadastrarVinculacaoOrcamentaria&acao=A';
$fm = new Simec_Helper_FlashMessage('planacomorc/tc/vinculacaoOrcamentaria');

if ($_REQUEST['vacid'] && $_REQUEST['execucao'] != 'editar') {
    $strSql = <<<DML
        SELECT 
            *, 
            vac.vaetituloorcamentario 
        FROM planacomorc.vinculacaoacaoestrategica vac 
        JOIN planacomorc.vinculacaoacaoestrategicaexercicio vae ON vac.vacid = vae.vacid 
        WHERE vac.vacid = {$_REQUEST['vacid']} AND exercicio = {$_REQUEST['clicouexercicio']}
DML;
    $resultSet = $db->pegaLinha($strSql);
}

if ($_REQUEST['execucao'] == 'excluir') {
    switch ($_REQUEST['tipo']) {
        case 'acao':
            $strSQL = <<<DML
                DELETE FROM planacomorc.vinculacaoestrategicaacoes WHERE veaid = %d
DML;
            break;
        case 'subacao':
            $sql = <<<DML
                SELECT sbacod FROM planacomorc.vinculacaoestrategicasubacoes WHERE vesid = {$_REQUEST['registro']}
DML;
            $sbacod = $db->pegaUm($sql);
            $strSQL = <<<DML
                DELETE FROM planacomorc.vinculacaoestrategicasubacoes WHERE sbacod = '%s' AND vaeid='{$_REQUEST['vaeid']}'
DML;
            break;
        case 'pi';
            $strSQL = <<<DML
                DELETE FROM planacomorc.vinculcaoestrategicapis WHERE vepid = %d AND vaeid='{$_REQUEST['vaeid']}'
DML;
            break;
        case 'po';
            $strSQL = <<<DML
                DELETE FROM planacomorc.vinculacaoestrategicapos WHERE vpoid = %d AND vaeid='{$_REQUEST['vaeid']}'
DML;
            break;
        default:
            throw new Exception('nenhuma ação foi passada via requisição');
    }

    if ($_REQUEST['tipo'] == 'subacao') {
        $stmt = sprintf($strSQL, $sbacod);
    } else {
        $stmt = sprintf($strSQL, $_GET['registro']);
    }

    $db->executar($stmt);
    $db->commit();
    $fm->addMensagem('Registro excluído com sucesso.', Simec_Helper_FlashMessage::SUCESSO);

    $vacid = $_REQUEST['vacid'];
    $exercicio = $_REQUEST['clicouexercicio'];
    echo <<<JAVASCRIPT
        <script>
            window.location = 'http:planacomorc.php?modulo=sistema/tabelasapoio/cadastrarVinculacaoOrcamentaria&acao=A&clicouexercicio={$exercicio}&vacid={$vacid}';
        </script>
JAVASCRIPT;

}

if ($_REQUEST['execucao'] == 'editar') {
    if($_REQUEST['vacid'] != null && $_REQUEST['vacid'] != ''){
        $vacid = <<<DML
            vac.vacid= {$_REQUEST['vacid']} AND
DML;
    }
    $strSql = <<<DML
        SELECT 
            *, 
            vac.vaetituloorcamentario 
        FROM planacomorc.vinculacaoacaoestrategica vac 
        JOIN planacomorc.vinculacaoacaoestrategicaexercicio vae on vac.vacid = vae.vacid 
        WHERE {$vacid}
            exercicio = {$_REQUEST['clicouexercicio']}
DML;
    
    $resultSet = $db->pegaLinha($strSql);
    #ver($strSql,d);
}

if ($resultSet['vaeid']) {
    #$resultAcao = resultado_soma_acoes($resultSet['vaeid'], $resultSet['exercicio']);
    #$resultPo = carrega_soma_ptres($resultSet['vaeid'], $resultSet['exercicio']);
    #$resultPi = carrega_soma_pi($resultSet['vaeid'], $resultSet['exercicio']);
    #$resultSubacao = carrega_soma_subacoes($resultSet['vaeid'], $resultSet['exercicio']);
    
    /* Retorna o Total de Execução para o Ano */
    /*$sql = <<<DML
        SELECT
            aet.ano,
            aet.empenhado,
            aet.pago,
            aet.rap_npp,
            aet.rap_pp
        FROM planacomorc.vwacaoestrategicatotal aet
        WHERE vaeid = {$resultSet['vaeid']}
DML;
    
    $execucaoTotal = $db->pegaLinha($sql);
    $DespesasEmpenhadas = $execucaoTotal['empenhado'];
    $ValoresPagos = $execucaoTotal['pago'];
    $RAPnaoProcessadosPagos = $execucaoTotal['rap_npp'];
    $RAPProcessadosPagosNE = $execucaoTotal['rap_pp'];*/
    $cotacaoTotal = cotacaoTotal($resultSet['vaeid'], $_REQUEST['clicouexercicio']);
}

require_once APPRAIZ . "includes/cabecalho.inc";
#loadStaticScripts();
?>
<style>
    .red {
        color: #B60F2E;
        font-weight:bold;
    }
    .bold {
        font-weight:bold;
    }     
    .blue{
        color:#073E87;
    }
    .marcado {
        background-color: #C1FFC1 !important;
    }
    .remover {
        display: none;
    }
</style>
<script>
    /** Salva o formulário principal da página.*/
    function salvafrmVinculacao()
    {
        error = true;
        if (!$("select[name='acaid']").val()) {                    
            $("select[name='acaid']").parent().parent().removeClass('has-success');
            $("select[name='acaid']").parent().parent().addClass('has-warning');
            error = false;
        }else{
            $("select[name='acaid']").parent().parent().removeClass('has-warning');
            $("select[name='acaid']").parent().parent().addClass('has-success');
        }

        if (!$("input[name='vaetituloorcamentario']").val()) {                    
            $('input[name="vaetituloorcamentario"]').parent().parent().removeClass('has-success');
            $('input[name="vaetituloorcamentario"]').parent().parent().addClass('has-warning');
            error = false;
        }else{
            $('input[name="vaetituloorcamentario"]').parent().parent().removeClass('has-warning');
            $('input[name="vaetituloorcamentario"]').parent().parent().addClass('has-success');
        }                

        $('select[name="exercicio"]').parent().parent().addClass('has-success');

        if (!$("textarea[name='vaedescricao']").val()) {                    
            $('textarea[name="vaedescricao"]').parent().parent().removeClass('has-success');
            $('textarea[name="vaedescricao"]').parent().parent().addClass('has-warning');
            error = false;
        }else{
            $('textarea[name="vaedescricao"]').parent().parent().removeClass('has-warning');
            $('textarea[name="vaedescricao"]').parent().parent().addClass('has-success');
        }

        if(error){
            document.forms["frmVinculacao"].submit();
        }else{
            bootbox.alert("Campo(s) de preenchimento obrigatório").focus();
            return false;
        }
    };

    /**
     * Vincular Ações, Subações, Planos Internos e Planos Orçamentários.
     * @param {object} objeto (o próprio botão)
     * @returns false para evitar disparo do html
     */
    function vincular(objeto)
    {
        var _remote = $(objeto).attr("data-remote");
        var _title = $(objeto).attr("data-title");
        var _tipo = $(objeto).attr("data-tipo");

        abrirModal(_remote, _title, _tipo);
        return false;
    }

    /**
     * Abre Modal do cabeçalho.
     * @param {type} objeto
     * @returns {Boolean}
     */        
    function abrirModal(url, titulo, tipo) 
    {
        $.post(url, function(html) 
        {
            $('#modal-confirm .btn-primary').html('Salvar')
            $('#modal-confirm .modal-body').html(html);
            $('.modal-dialog').css('width', '90%');
            $('#modal-confirm .modal-title').html(titulo);
            $('#modal-confirm .btn-primary').attr('class','btn btn-primary btnAcao '+tipo);
            $('#modal-confirm .btn-primary').attr('onclick','return confirmaSelecao(this);');                
            $('#modal-confirm .btn-default').html('Fechar');

            $('.modal-dialog').show();
            $('#modal-confirm').modal();
        });
    };

    /**
     * Primeiro passo: Ao clicar em Adicionar Ações será aberto uma modal. O Botão para finalizar a seleção das ações irá chamar esta função.     
     * @param {object} objeto do input
     * @returns Nova listagem de dados de acordo com o valor anteriormente selecionado.
     */      
    function confirmaSelecao(objeto) 
    {
        var section = $(objeto).attr("class").split(" ")[3]
            , url = "planacomorc.php?modulo=sistema/tabelasapoio/cadastrarVinculacaoOrcamentaria&acao=A&id="+$("input[name='vaeid']").val()+"&getGrid=" + section
            , btnAcao = $(objeto);

        if (section !== 'pi') {

            //Valida Checkbox da listagem
            if ($("input.uoCheck").length && !$("input.uoCheck").is(":checked")) {
                bootbox.alert("É necessário selecionar ao menos uma UO!").focus();
                return false;
            }

            /** Volta para passo anterior - selecionar as UO's*/                 
            if ($(".step1").css("display") === "none") {
                $(".step1").show();
                $(".step2").hide().html("");                    
                return false;
            }

            /**
             * Avança um passo - selecionar as ações para as UO's
             */
            if (typeof section === 'string') {
                url += "&clicouexercicio=<?= $_REQUEST["clicouexercicio"] ?>&values=" + getValuesUOs().join(",");
                $.post(url, function(data) {
                    $(".step1").hide();
                    btnAcao.attr("value", "Voltar");
                    $(".step2").show().html(data);
                });
            }
        } else {

            if ($("input.piUoCheck").length && !$("input.piUoCheck").is(":checked")) {
                var notPiUoCheck = true;
            }

            if ($("input.piSbaCheck").length && !$("input.piSbaCheck").is(":checked")) {
                var notPiSbaCheck = true;
            }

            if (notPiUoCheck && notPiSbaCheck) {
                bootbox.alert("É necessário selecionar ao menos uma UO ou Subação!").focus();
                return false;
            }

            var uos, sba;
            uos = (!notPiUoCheck) ? getValuesPIUOs().join(",") : false;
            sba = (!notPiSbaCheck) ? getValuesPIsba().join(",") : false;

            /* Avança um passo - selecionar as ações para as UO's */
            if (typeof section === 'string') {
                url += "&clicouexercicio=<?= $_REQUEST["clicouexercicio"] ?>&uos=" + uos + "&sba=" + sba;
                $.post(url, function(data) {
                    $('.filter-right').parent().hide();
                    $(".pi-container").show().html(data);
                });
            }
        }
        return false;
    };

    /**
    * Faz associação das UO com as Ações
    */
    function confirmaSelecaoFinal(objeto)
    {
        var section = $(objeto).attr("class").split(" ")[3]
            , vaeid = $("input[name='vaeid']").val()
            , queryString;

        if (section === 'acao') {
            if ($("input.acaoUoCheck").length && !$("input.acaoUoCheck").is(":checked")) {
                bootbox.alert("É necessário selecionar ao menos uma ação!").focus();
                return false;
            }

            var acoes = getValuesAcoes().join(",");
            queryString = "&clicouexercicio=<?= $_REQUEST["clicouexercicio"] ?>&acoes=" + acoes + "&vaeid=" + vaeid;
        }

        if (section === 'subacao') {
            if ($("input.subacaoCheck").length && !$("input.subacaoCheck").is(":checked")) {
                bootbox.alert("é necessário selecionar ao menos uma subação!").focus();
                return false;
            }

            var uos = getValuesUOs().join(",")
                    , acoes = getValuesSubAcoes().join(",");
            queryString = "&uos=" + uos + "&acoes=" + acoes + "&vaeid=" + vaeid;
        }
        if (section === 'pi') {
            if ($("input.plicodCheck").length && !$("input.plicodCheck").is(":checked")) {
                bootbox.alert("É necessário selecionar ao menos uma PI!").focus();
                return false;
            }
            uos = getValuesPIUOs().join(",");
            queryString = "&acoes=" + getValuesPIs().join(",") + "&uos="+ uos +"&vaeid=" + vaeid;
        }

        if (section === 'po') {
            if ($("input.ptresCheck").length && !$("input.ptresCheck").is(":checked")) {
                bootbox.alert("É necessário selecionar ao menos um PO!").focus();
                return false;
            }
            var pos = getValuesPOs().join(",");
            queryString = "&acoes=" + pos + "&vaeid=" + vaeid;
        }

        location.href = "<?= $urlCall; ?>&clicouexercicio=<?= $_REQUEST["clicouexercicio"] ?>&section=" + section + queryString;
        return false;
    };

    /*------------------------------- Funções para captura de valores de Checkboxes ----------------------------------*/
    function getValuesUOs() 
    {
        var values = [];
        $("input[type=checkbox][name='uo[]']:checked").each(function() {
            values.push($(this).val());
        });
        return values;
    };

    function getValuesAcoes() 
    {
        var values = [];
        $("input[type=checkbox][name='acao[]']:checked").each(function() {
            values.push($(this).val());
        });
        return values;
    };

    function getValuesSubAcoes () 
    {
        var values = [];
        $("input[type=checkbox][name='subacao[]']:checked").each(function() {
            values.push($(this).val());
        });
        return values;
    };

    function getValuesPIs() 
    {
        var values = [];
        $("input[type=checkbox][name='plicod[]']:checked").each(function() {
            values.push($(this).val());
        });
        return values;
    };

    function getValuesPOs() 
    {
        var values = [];
        $("input[type=checkbox][name='ptres[]']:checked").each(function() {
            values.push($(this).val());
        });
        return values;
    };

    function getValuesPIUOs() 
    {
        var values = [];
        $("input[type=checkbox][name='pi_uo[]']:checked").each(function() {
            values.push($(this).val());
        });
        return values;
    };

    function getValuesPIsba() 
    {
        var values = [];
        $("input[type=checkbox][name='pi_sba[]']:checked").each(function() {
            values.push($(this).val());
        });
        return values;
    };
    /*------------------------------- FIM das Funções para captura de valores em Checkboxes ----------------------------------*/

    function voltar()
    {
        window.location = 'planacomorc.php?modulo=sistema/tabelasapoio/vinculacaoOrcamentaria&acao=A';
    }

    function deleteAcoes(registro)
    {
        bootbox.confirm("Deseja realmente excluir este registro?",function(result){
            if(result){
                window.location = '<?php echo $urlCall; ?>&execucao=excluir&tipo=acao&registro=' + registro + '&vaeid=<?php echo $resultSet['vaeid']; ?>&clicouexercicio=<?= $_REQUEST["clicouexercicio"] ?>&vacid=<?php echo $resultSet['vacid']; ?>';
            }
        }).focus();            
        
    }

    function deleteSubacoes(registro)
    {
        bootbox.confirm("Deseja realmente excluir este registro?", function(result){
            if(result){
                window.location = '<?php echo $urlCall; ?>&execucao=excluir&tipo=subacao&registro=' + registro + '&vaeid=<?php echo $resultSet['vaeid']; ?>&clicouexercicio=<?= $_REQUEST["clicouexercicio"] ?>&vacid=<?php echo $resultSet['vacid']; ?>';
            }
        }).focus();            
        
    }

    function deletePlanoInterno(registro)
    {
        bootbox.confirm("Deseja realmente excluir este registro?", function(result){
            if(result){
                window.location = '<?php echo $urlCall; ?>&execucao=excluir&tipo=pi&registro=' + registro + '&vaeid=<?php echo $resultSet['vaeid']; ?>&clicouexercicio=<?= $_REQUEST["clicouexercicio"] ?>&vacid=<?php echo $resultSet['vacid']; ?>';
            }
        }).focus();
        
    }

    function deletePlanoOrcamentario(registro)
    {
        bootbox.confirm("Deseja realmente excluir este registro?", function(result){
            if(result){
                window.location = '<?php echo $urlCall; ?>&execucao=excluir&tipo=po&registro=' + registro + '&vaeid=<?php echo $resultSet['vaeid']; ?>&clicouexercicio=<?= $_REQUEST["clicouexercicio"] ?>&vacid=<?php echo $resultSet['vacid']; ?>';
            }
        }).focus();
        
    }
</script>

<!-- FIM DO SCRIPT, INICIO DO CORPO -->

<div class="col-lg-12">
    <ol class="breadcrumb">
        <li><a href="<?php echo MODULO; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li>Tabelas de Apoio</li>
        <li><a href="planacomorc.php?modulo=sistema/tabelasapoio/vinculacaoOrcamentaria&acao=A&listar=true">Vinculação Orçamentária da Ação Estratégica</a></li>
        <li class="active">Cadastro de Vinculação Orçamentária da Ação Estratégica</li>
    </ol>
    <?php 
    /* Apresenta quaisquer tipo de mensagens referente a esta página. */
    echo $fm->getMensagens(); 
    ?>
    <div class="well">
        <div class="form-group">
            <table class="tabela table table-striped table-bordered table-hover" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
                <tr>
                    <td><span class="blue"><b>Despesas Empenhadas:</b></span> <span><b><?= number_format($cotacaoTotal['empenhado'], 2, ',', '.') ?></b></span></td>
                    <td><span class="blue"><b>Valores Pagos:</b></span> <span><b><?= number_format($cotacaoTotal['pago'], 2, ',', '.') ?></b></span></td>
                </tr>
                <tr>
                    <td><span class="blue"><b>RAP não-Proc. Pagos:</b></span> <span><b><?= number_format($cotacaoTotal['rap_npp'], 2, ',', '.') ?></b></span></td>
                    <td><span class="blue"><b>RAP Processados Pagos:</b></span> <span><b><?= number_format($cotacaoTotal['rap_pp'], 2, ',', '.') ?></b></span></td>
                </tr>
            </table>
        </div>

        <fieldset>
            <form id="frmVinculacao" name="frmVinculacao" method="POST" class="form-horizontal" enctype="multipart/form-data">
                <div class="form-group">
                    <div class="col-lg-10">
                        <input type="hidden" name="execucao" id="execucao" value="cadastrar">
                        <input type="hidden" name="vacid" id="vacid" value="<?= isset($resultSet['vacid']) ? $resultSet['vacid'] : null; ?>" />
                        <input type="hidden" name="vaeid" id="vaeid" value="<?= isset($resultSet['vaeid']) ? $resultSet['vaeid'] : null; ?>" />
                    </div>
                </div>
                <div class="form-group">
                    <label for="acaid" class="col-lg-2 control-label">Ação Estratégica:</label>
                    <div class="col-lg-10">
                        <?php
                        $strSQL = <<<DML
                            SELECT 
                                acaid AS codigo, 
                                acadsc AS descricao 
                            FROM painel.acao 
                            WHERE acastatus = 'A' 
                                ORDER BY acadsc
DML;
                        $db->monta_combo('acaid', $strSQL, 'S', 'Selecione', null, null, null, null, 'N', 'acaid', null, isset($resultSet['acaid']) ? $resultSet['acaid'] : '', null, 'class="form-control chosen-select" style="width=100%;""', null, null);
                        ?>
                    </div>
                </div>
                <div class="form-group">
                    <label for="vaetituloorcamentario" class="col-lg-2 control-label">Título orçamentário da ação estratégica:</label>
                    <div class="col-lg-10">
                        <input type="text" class="normal form-control" maxlength="100" name="vaetituloorcamentario" id="vaetituloorcamentario" value="<?php echo $resultSet['vaetituloorcamentario'] ? $resultSet['vaetituloorcamentario'] : '';?>" />
                    </div>
                </div>
                <div class="form-group">
                    <label for="exercicio" class="col-lg-2 control-label">Exercício:</label>
                    <div class="col-lg-10">
                        <select style="width: 100%;" class="form-control chosen-select" id="exercicio" name="exercicio" class="CampoEstilo" style="width: auto;"
                            onchange="javascript:location.href = 'planacomorc.php?modulo=sistema/tabelasapoio/cadastrarVinculacaoOrcamentaria&acao=A&execucao=editar&vacid=<?= $resultSet['vacid'] ?>&vaeid=<?= $_GET['vaeid'] ?>&clicouexercicio=' + this.value">
                            <option value="2020" <?= ($resultSet['exercicio'] == '2020' or $_GET['clicouexercicio'] == '2020') ? 'selected="selected"' : ''; ?>>2020</option>
                            <option value="2019" <?= ($resultSet['exercicio'] == '2019' or $_GET['clicouexercicio'] == '2019') ? 'selected="selected"' : ''; ?>>2019</option>
                            <option value="2018" <?= ($resultSet['exercicio'] == '2018' or $_GET['clicouexercicio'] == '2018') ? 'selected="selected"' : ''; ?>>2018</option>
                            <option value="2017" <?= ($resultSet['exercicio'] == '2017' or $_GET['clicouexercicio'] == '2017') ? 'selected="selected"' : ''; ?>>2017</option>
                            <option value="2016" <?= ($resultSet['exercicio'] == '2016' or $_GET['clicouexercicio'] == '2016') ? 'selected="selected"' : ''; ?>>2016</option>
                            <option value="2015" <?= ($resultSet['exercicio'] == '2015' or $_GET['clicouexercicio'] == '2015') ? 'selected="selected"' : ''; ?>>2015</option>
                            <option value="2014" <?= ($resultSet['exercicio'] == '2014' or $_GET['clicouexercicio'] == '2014') ? 'selected="selected"' : ''; ?>>2014</option>
                            <option value="2013" <?= ($resultSet['exercicio'] == '2013' or $_GET['clicouexercicio'] == '2013') ? 'selected="selected"' : ''; ?>>2013</option>                            
                            <option value="2012" <?= ($resultSet['exercicio'] == '2012' or $_GET['clicouexercicio'] == '2012') ? 'selected="selected"' : ''; ?>>2012</option>
                            <option value="2011" <?= ($resultSet['exercicio'] == '2011' or $_GET['clicouexercicio'] == '2011') ? 'selected="selected"' : ''; ?>>2011</option>                            
                        </select>
                    </div>
                </div>
                <div class="form-group">
                    <label for="justificativa" class="col-lg-2 control-label">Descrição da composição orçamentária:</label>
                    <div class="col-lg-10">
                        <textarea class="form-control" rows="3" id="justificativa" name="vaedescricao" maxlength="500" required><?php echo $resultSet['vaedescricao'] ? $resultSet['vaedescricao'] : '';?></textarea>
                    </div>
                </div>

                <div class="form-group">
                    <div class="col-lg-10 col-lg-offset-2">
                        <button class="btn btn-warning" type="reset" onclick="window.location = window.location;">Limpar</button>
                        <button class="btn btn-warning" id="btnVoltar" type="button" onclick="voltar();">Voltar</button>
                        <button class="btn btn-primary" onclick="return salvafrmVinculacao();" id="inserir" type="submit">
                            <span class="glyphicon glyphicon-upload"></span> Salvar
                        </button>
                    </div>
                </div>
            </form>
        </fieldset>
    </div>
    
<?php 
    if ($resultSet) {
?>
    <div class="">
        <form>
            <div class="panel panel-default">
                <div class="panel-heading"><b style="font-size: 16px;">Ações</b></div>
                <?php
                if (isset($resultSet['vaeid'])) {
                    $strSqlGrid = <<<DML
                        SELECT veaid, acacod, unicod || ' - ' || unidsc
                        FROM planacomorc.vinculacaoestrategicaacoes
                        JOIN unidade USING(unicod)
                        WHERE vaeid = {$resultSet['vaeid']}
                        ORDER BY veaid DESC
DML;
                    $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
                    $listagem->setCabecalho(array("Ação Orçamentária","Unidade Orçamentária"));
                    $listagem->setQuery($strSqlGrid);
                    $listagem->setAcoes(array('delete' => 'deleteAcoes'));
                    $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
                    
                    $html = cotacaoAcoes($resultSet['vaeid']);
                }
                ?>
                <ul class="list-group">
                    <li class="list-group-item">
                        <? echo $html;?>
                    </li>
                    <li class="list-group-item">
                        <input type="button" class="btn btn-success btnVinc acao text-center" data-title="Escolha as Ações:" onclick="return vincular(this);"
                            data-remote="<?= $urlCall; ?>&selecionaUo=acao&clicouexercicio=<?= $_REQUEST["clicouexercicio"] ?>"
                            value="Adicionar Ações" data-tipo="acao"/>
                    </li>
                </ul>
            </div>
            <div class="panel panel-default">
                <div class="panel-heading"><b style="font-size: 16px;">Subações</b></div>
                <?php
                if (isset($resultSet['vaeid'])) {
                    $strSqlGrid = <<<DML
                        SELECT vesid, sbacod, sbacod, unicod || ' - ' || unidsc
                        FROM planacomorc.vinculacaoestrategicasubacoes
                        JOIN unidade USING(unicod)
                        WHERE vaeid = {$resultSet['vaeid']}
                        ORDER BY vesid DESC
DML;
                    $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
                    $listagem->setCabecalho(array("Subação Orçamentária","Unidade Orçamentária"));
                    $listagem->setQuery($strSqlGrid);
                    $listagem->setAcoes(array('delete' => 'deleteSubacoes'));
                    $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
                    
                    $html = cotacaoSubacoes($resultSet['vaeid']);
                }   
                ?>
                <ul class="list-group">
                    <li class="list-group-item">
                        <? echo $html; ?>
                    </li>
                    <li class="list-group-item">
                        <input type="button" class="btn btn-success btnVinc subacao text-center" data-title="Escolha as Subações:" onclick="return vincular(this);"
                            data-remote="<?= $urlCall; ?>&selecionaUo=subacao&clicouexercicio=<?= $_REQUEST["clicouexercicio"] ?>" 
                            value="Adicionar Subações" data-tipo="subacao"/>
                    </li>                    
                </ul>
            </div>             

            <div class="panel panel-default">
                <div class="panel-heading"><b style="font-size: 16px;">Planos Internos</b></div>
                <?php
                if (isset($resultSet['vaeid'])) {
                    $strSqlGrid = <<<DML
                        SELECT vp.vepid, vp.plicod, uni.unicod|| ' - ' ||uni.unidsc as unidade, plititulo
                        FROM planacomorc.vinculcaoestrategicapis vp
                        JOIN monitora.pi_planointerno USING(plicod)
                        LEFT JOIN public.unidade uni on vp.unicod = uni.unicod
                        WHERE vaeid = {$resultSet['vaeid']} AND pliano = '{$_GET['clicouexercicio']}'
                        ORDER BY vepid DESC
DML;
                    $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
                    $listagem->setCabecalho(array("Plano Interno","Unidade Orçamentária","Título"));
                    $listagem->setQuery($strSqlGrid);
                    $listagem->setAcoes(array('delete' => 'deletePlanoInterno'));
                    $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
                    
                    $html = cotacaoPI($resultSet['vaeid']);
                }
                ?>
                <ul class="list-group">
                    <li class="list-group-item">
                        <? echo $html; ?>
                    </li>
                    <li class="list-group-item">
                        <input type="button" class="btn btn-success btnVinc pi text-center" data-title="Escolha os Planos Internos:" onclick="return vincular(this);"
                            data-remote="<?= $urlCall; ?>&selecionaUo=pi&clicouexercicio=<?= $_REQUEST["clicouexercicio"] ?>" 
                            value="Adicionar Planos Internos" data-tipo="pi"/>
                    </li>
                </ul>
            </div>
            
            <div class="panel panel-default">
                <div class="panel-heading"><b style="font-size: 16px;">Planos Orçamentários</b></div>
                <?php
                if (isset($resultSet['vaeid'])){
                    $strSqlGrid = <<<DML
                        SELECT vep.vpoid, vep.plocod || ' - ' || plo.plotitulo, pt.acacod, uni.unicod|| ' - ' ||uni.unidsc as unidade,  pt.ptres
                        FROM planacomorc.vinculacaoestrategicapos vep
                        JOIN unidade uni USING(unicod)
                        JOIN monitora.ptres pt ON (pt.ptres=vep.ptres)
                        JOIN monitora.planoorcamentario plo ON plo.acaid = pt.acaid AND plo.plocodigo = pt.plocod
                        WHERE vaeid = {$resultSet['vaeid']} AND pt.ptrano = '{$_SESSION['exercicio']}'
                        ORDER BY vpoid DESC
DML;
                    $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
                    $listagem->setCabecalho(array("Plano Orçamentário","Ação Orçamentária","Unidade Orçamentária","PTRES"));
                    $listagem->setQuery($strSqlGrid);
                    $listagem->setAcoes(array('delete' => 'deletePlanoOrcamentario'));
                    $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
                    
                    $html = cotacaoPTRES($resultSet['vaeid']);
                }
                ?>
                <ul class="list-group">
                    <li class="list-group-item">
                        <? echo $html; ?>
                    </li>
                    <li class="list-group-item">
                        <input type="button" class="btn btn-success btnVinc po text-center" data-title="Escolha os Planos Orçamentários:" onclick="return vincular(this);"
                            data-remote="<?= $urlCall; ?>&selecionaUo=po&clicouexercicio=<?= $_REQUEST["clicouexercicio"] ?>" 
                            value="Adicionar Planos Orçamentários" data-tipo="po"/>
                    </li>                    
                </ul>                
            </div>            
        </form>        
    </div>
<?php 
    }
?>
</div>