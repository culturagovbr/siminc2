<?php

    include_once APPRAIZ. 'monitora/classes/Pi_PlanoInterno.class.inc';

    # Recuperando SubUnidade
    $suoid = $_REQUEST['suoid'];
    # Recuperando Janela Ativa
    $janelaAtiva = (new Acompanhamento_Model_Janela())->recuperarJanelaAtiva(Acompanhamento_Model_Tipo::K_TIPO_INDICADOR_PNC);
    # Recuperando a Janela de Alteração
    $janid = $_REQUEST['janid']? $_REQUEST['janid']: $janelaAtiva['janid'];
    $janela = new Acompanhamento_Model_Janela($janid);
    # Recuperando Meta PNC
    $mpnid = $_REQUEST['mpnid'];
    
    switch ($_REQUEST['req']) {
        case 'acompanhamento-indicador-pnc-form':
            include APPRAIZ. 'acompanhamento/modulos/principal/acompanhamento_indicador_pnc_form.inc';
        die;
        case 'acompanhamento-atividade-pnc-form':
            include APPRAIZ. 'acompanhamento/modulos/principal/acompanhamento_atividade_pnc_form.inc';
        die;
        case 'acompanhamento-anexo-pnc-form':
            include APPRAIZ. 'acompanhamento/modulos/principal/acompanhamento_anexo_pnc_form.inc';
        die;
        case 'salvar-acompanhamento-indicador-pnc':
            $resultado = Acompanhamento_Controller_Acompanhamentopnc::salvarIndicadorPnc(simec_utf8_decode_recursive($_REQUEST));
            echo simec_json_encode($resultado);
        die;
        case 'enviar-acompanhamento-pnc':
            $resultado = Acompanhamento_Controller_Acompanhamentopnc::enviar(simec_utf8_decode_recursive($_REQUEST));
            echo simec_json_encode(array());
        die;
        case 'salvar-acompanhamento-atividade-pnc':
            $resultado = Acompanhamento_Controller_Acompanhamentopnc::salvarAtividadePnc(simec_utf8_decode_recursive($_REQUEST));
            echo simec_json_encode($resultado);
        die;
        case 'excluir-acompanhamento-atividade-pnc':
            $resultado = Acompanhamento_Controller_Acompanhamentopnc::excluirAtividadePnc(simec_utf8_decode_recursive($_REQUEST));
            echo simec_json_encode($resultado);
        die;
        case 'excluir-acompanhamento-anexo-pnc':
            $resultado = Acompanhamento_Controller_Acompanhamentopnc::excluirAnexoPnc(simec_utf8_decode_recursive($_REQUEST));
            echo simec_json_encode($resultado);
        die;
        case 'listar-indicadores-pnc':
            include APPRAIZ. 'acompanhamento/modulos/principal/listar-indicadores-pnc.inc';
        die;
        case 'listar-atividades-pnc':
            include APPRAIZ. 'acompanhamento/modulos/principal/listar-atividades-pnc.inc';
        die;
        case 'listar-anexos-pnc':
            include APPRAIZ. 'acompanhamento/modulos/principal/listar-anexos-pnc.inc';
        die;
        case 'detalhe-pi':
            include APPRAIZ . 'acompanhamento/modulos/principal/detalhe-pi-pnc.inc';
        die;
        case 'exibir-grafico':
            include APPRAIZ. 'acompanhamento/modulos/principal/modal-grafico-pnc.inc';
        die;
        case 'listar-historico-indicador-pnc':
            include APPRAIZ. "acompanhamento/modulos/principal/listar-historico-indicador-pnc.inc";
        die;
        case 'anexar-arquivos-monitoramento-Pnc':
            $acpid = $_REQUEST['acpid'];
            $_FILES['file']['name']   = utf8_decode($_FILES['file']['name']);
            $_REQUEST['arqdescricao'] = utf8_decode($_REQUEST['arqdescricao']);
            $file = new FilesSimec();
            if($file->setUpload($_REQUEST['arqdescricao'], '', FALSE)){
                $arqid = $file->getIdArquivo();
                $acompanhamentoPncArquivo = New Acompanhamento_Model_Acompanhamentopncarquivo();
                $acompanhamentoPncArquivo->acpid = $acpid;
                $acompanhamentoPncArquivo->arqid = $arqid;
                $acompanhamentoPncArquivo->salvar();
                $acompanhamentoPncArquivo->commit();

                echo simec_json_encode(array(
                    'acpid' => $acpid,
                    'arqid' => $arqid,
                    'arqnome' => $_FILES['file']['name'],
                    'arqdescricao' => $_REQUEST['arqdescricao']
                ));
            }else{
                echo simec_json_encode(array(
                    'acpid' => $acpid,
                    'error' => 1,
                    'errorMensage' => 'Não foi possível enviar o arquivo!'
                ));
            }
        die;
        case'carregar-municipio':
            echo $simec->select('muncod', 'Município', $atividade->muncod, (new Territorios_Model_Municipio())->recuperarSqlCombo(['estuf', 'mundescricao'], ["estuf='". $_REQUEST['estuf']."'"]));
        die;
    }

    if($mpnid){
        # Busca acompanhamento Ativo de acordo com a Unidade, Janela e Meta.
        $stdAcompanhamento = Acompanhamento_Controller_Acompanhamentopnc::buscarAcompanhamentoJanelaSubunidadeMeta($suoid, $janid, $mpnid);

        # Verifica se existe acompanhamento da meta selecionada
        if($stdAcompanhamento->acpid){
            # Carrega dados do acompanhamento da Meta
            $acompanhamentopnc = new Acompanhamento_Model_Acompanhamentopnc($stdAcompanhamento->acpid);
        } else {
            # Cria um novo registro do acompanhamento da Meta
            $acompanhamentopnc = new Acompanhamento_Model_Acompanhamentopnc();
            $acompanhamentopnc->suoid = $suoid;
            $acompanhamentopnc->janid = $janid;
            $acompanhamentopnc->mpnid = $mpnid;
            $acompanhamentopnc->salvar();
            $acompanhamentopnc->commit();
        }

        # Usado no formulário modal de upload de arquivos do acompanhamento da Meta PNC
        $acpid = $acompanhamentopnc->acpid;

        # Verificando se os indicadores do ano atual foi preenchido.
        $indicadoresPreenchidos = $acompanhamentopnc->verificarPreenchimentoIndicadoresAnoAtual();

        # Verifica se existe pelo menos uma atividade inserida no acompanhamento da Meta PNC
        $atividadePreenchida = $acompanhamentopnc->verificarPreenchimentoAtividade();

        # Verifica se o usuário pode modificar o acompanhamento de acordo com o perfil e estado do acompanhamento
        $podeEditar = Acompanhamento_Controller_Acompanhamentopnc::verificarPermissaoEditar($acompanhamentopnc->acpid);
    }

    include APPRAIZ . "includes/cabecalho.inc";

?>

<script src="js/principal/acompanhamento.js?v=1"></script>

<script src="../library/liquify/snap.svg.js"></script>
<script src="../library/liquify/liquid.meter.js"></script>

<style>
    .no-margin-bottom .form-group{
        margin-bottom: 5px;
    }
</style>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-12">
        <h2>MONITORAMENTO DAS METAS PNC</h2>
    </div>
</div>

<?php echo '<input id="acpid" type="hidden" value="'.((int)$acpid). '" />'; ?>
<?php echo '<input id="pode_editar" type="hidden" value="'.((int)$podeEditar). '" />'; ?>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <div class="col-md-12">
            
            <div class="ibox float-e-margins">
            	<div class="ibox-title">
                    <div class="row">
                        <div class="col-sm-2">
                            <h5>Metas</h5>
                        </div>
                        <div class="col-sm-10">
                            <div style="float: right;">
                                <div class="box-legenda">
                                    Legenda:
                                </div>
                                <div class="box-legenda red-bg">
                                    Pendente
                                </div>
                                <div class="box-legenda navy-bg">
                                    Preenchida
                                </div>
                                <div class="box-legenda blue-bg">
                                    Selecionada
                                </div>
                            </div>
                        </div>
            	    </div>
            	</div>
            	<div class="ibox-content">
                    <div class="row form-horizontal">

                        <div class="col-sm-6">
                            <?php
                                $listaUnidades = (new Public_Model_SubUnidadeOrcamentaria())->recuperarPorUsuario();
                                echo $simec->select('suoid', 'Unidade', $suoid, $listaUnidades, null, ['label-size'=>1]);
                            ?>
                        </div>

                        <div class="clearfix"></div>

                        <?php
                            $listaMetas = Acompanhamento_Model_Acompanhamentopnc::getMetasPnc((object) array(
                                'exercicio' => $_SESSION['exercicio'],
                                'suoid' => $suoid,
                                'janid' => $janid
                            ));
                        
                            foreach($listaMetas as $contador => $meta){
                                $cor = $meta['acompanhamento']? 'navy-bg': 'red-bg';
                                if($mpnid == $meta['mpnid']){
                                    $cor = 'blue-bg';
                                    $mpnid_proximo = $listaMetas[$contador++]['mpnid']? $listaMetas[$contador++]['mpnid']: '';
                                }
                                
                                $urlAcompanhamento = '?modulo=principal/monitorar-pnc&acao=A&suoid='. (int)$suoid. '&janid='. (int)$janid. '&mpnid='. (int)$meta['mpnid'];
                            ?>
                                <a class="selecionar-meta-pnc" href="<?php echo $urlAcompanhamento; ?>">
                                    <div class="col-xs-1" title="<?php echo $meta['mpncod'] . ' - ' . $meta['mpndsc']; ?>">
                                        <div data-cor="<?php echo $cor ?>" class="widget div-indicador <?php echo $cor ?>">
                                            <div class="row">
                                                <div class="col-xs-12">
                                                    <h2><?php echo $meta['mpncod']; ?></h2>
                                                </div>
                                            </div>
                                        </div>
                                    </div>
                                </a>
                            <?php } ?>

                        <div class="clearfix"></div>

                        <?php if($mpnid): ?>
                            <?php
                                $simec->setPodeEditar(0);
                                $valoresOrcamentarios = (new Pi_PlanoInterno())->recuperarValoresPiMetaPncESubunidade($mpnid, $suoid);
                            ?>
                            <input type="hidden" id="mpnid" value="<?php echo $mpnid; ?>" />
                            <input type="hidden" id="mpnid_proximo" value="<?php echo $mpnid_proximo; ?>" />
                            <hr />
                            
                            <div class="col-sm-12">
                                <div style="float: right;">
                                    <?php echo $simec->input('Referência', 'Referência', $janela->jandsc. " (". formata_data($janela->janinicio). " - ". formata_data($janela->janfim). ")"); ?>
                                </div>
                            </div>
                            <div class="col-sm-12">
                                <table class="table">
                                    <thead>
                                        <tr>
                                            <th>
                                                <a class="detalhe-pi" href="?modulo=principal/monitorar-pnc&acao=A&req=detalhe-pi&mpnid=<?php echo $mpnid; ?>&suoid=<?php echo $suoid; ?>" title="Exibir Lista de Planos Internos">
                                                    Planos Internos (PI´s)
                                                </a>
                                            </th>
                                            <th><a class="a_grafico" href="#" title="Exibir Grafico de Planos Internos">Previsto</a></th>
                                            <th><a class="a_grafico" href="#" title="Exibir Grafico de Planos Internos">Empenhado</a></th>
                                            <th><a class="a_grafico" href="#" title="Exibir Grafico de Planos Internos">Liquidado</a></th>
                                            <th><a class="a_grafico" href="#" title="Exibir Grafico de Planos Internos">Pago</a></th>
                                        </tr>
                                    </thead>
                                    <?php if($valoresOrcamentarios): ?>
                                        <tbody>
                                            <tr>
                                                <td>
                                                    <a class="detalhe-pi" href="?modulo=principal/monitorar-pnc&acao=A&req=detalhe-pi&mpnid=<?php echo $mpnid; ?>&suoid=<?php echo $suoid; ?>" title="Exibir Lista de Planos Internos">
                                                        <?php echo number_format($valoresOrcamentarios['qtd'], 0, ',', '.'); ?>
                                                    </a>
                                                </td>
                                                <td><a class="a_grafico" href="#" title="Exibir Grafico de Planos Internos">R$ <?php echo number_format($valoresOrcamentarios['previsto'], 2, ',', '.'); ?></a></td>
                                                <td><a class="a_grafico" href="#" title="Exibir Grafico de Planos Internos">R$ <?php echo number_format($valoresOrcamentarios['empenhado'], 2, ',', '.'); ?></a></td>
                                                <td><a class="a_grafico" href="#" title="Exibir Grafico de Planos Internos">R$ <?php echo number_format($valoresOrcamentarios['liquidado'], 2, ',', '.'); ?></a></td>
                                                <td><a class="a_grafico" href="#" title="Exibir Grafico de Planos Internos">R$ <?php echo number_format($valoresOrcamentarios['pago'], 2, ',', '.'); ?></a></td>
                                            </tr>
                                        </tbody>
                                    <?php else: ?>
                                        <tfoot>
                                            <tr>
                                                <td>
                                                    <div class="alert alert-danger">Nenhum PI cadastrado para essa indicador</div>
                                                </td>
                                            </tr>
                                        </tfoot>
                                    <?php endif; ?>
                                </table>
                            </div>
                        <?php endif; ?>

                    </div>
            	</div>
            </div>
            
        </div>

        <?php if($mpnid): ?>
            <?php $meta = new Public_Model_MetaPnc($mpnid); ?>
            <div class="col-md-12">
                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <div class="row">
                            <div class="col-sm-1" style="text-align: center;">
                                <h1 class="h1_codigo_meta"><?php echo $meta->mpncod; ?></h1>
                            </div>
                            <div class="col-sm-11">
                                <h2><?php echo $meta->mpndsc; ?></h2>
                            </div>
                        </div>
                    </div>
                    <div class="ibox-content">

                        <!-- STEPS INICIO -->
                        <div id="wizard">
                            <h1>INDICADORES</h1>
                            <div class="div_lista_indicadores_pnc" data-mode="async" data-url="?modulo=principal/monitorar-pnc&acao=A&req=listar-indicadores-pnc&acpid=<?php echo $acompanhamentopnc->acpid; ?>">
                                Carregando ...
                                <?php echo '<input id="hid_indicadores_preenchidos" type="hidden" value="'.((int)$indicadoresPreenchidos). '" />'; ?>
                            </div>

                            <h1>ATIVIDADES</h1>
                            <div class="div_lista_atividades_pnc" data-mode="async" data-url="?modulo=principal/monitorar-pnc&acao=A&req=listar-atividades-pnc&acpid=<?php echo $acompanhamentopnc->acpid; ?>">
                                Carregando ...
                                <?php echo '<input id="hid_atividade_preenchida" type="hidden" value="'.((int)$atividadePreenchida). '" />'; ?>
                            </div>

                            <h1>ANEXOS</h1>
                            <div class="div_lista_anexos_pnc" data-mode="async" data-url="?modulo=principal/monitorar-pnc&acao=A&req=listar-anexos-pnc&acpid=<?php echo $acompanhamentopnc->acpid; ?>">
                                Carregando ...
                            </div>
                        </div>
                        <!-- STEPS FIM -->

                    </div>
                </div>
            </div>
        <?php endif; ?>

    </div>
</div>

<?php

    if($mpnid){
        bootstrapPopup(
            "Grafico de execução dos Planos Internos(PIs) da Unidade vinculados a essa Meta",
            'modal_grafico',
            '',
            array('fechar'),
            array('tamanho' => 'xl')
        );

        bootstrapPopup(
            "Planos Internos(PIs) da Unidade vinculados a essa Meta",
            'detalhe-pi',
            '',
            array('fechar'),
            array('tamanho' => 'lg')
        );
        
        bootstrapPopup(
            "Cadastro de acompanhamento do Indicador PNC ". '<a style="margin-left: 10px;" class="fa fa-history modal-historico-indicador-pnc" title="Histórico de alterações do indicador"></a>',
            'div_modal_acompanhamento_indicador',
            '',
            array('salvar', 'fechar'),
            array('tamanho' => 'lg')
        );

        bootstrapPopup(
            "Histórico de Alterações do Indicador PNC",
            'div_modal_lista_historico_indicador_pnc',
            '',
            array('fechar'),
            array('tamanho' => 'lg')
        );
        
        bootstrapPopup(
            "Atividade",
            'div_modal_acompanhamento_atividade',
            '',
            array('salvar', 'fechar'),
            array('tamanho' => 'lg')
        );
        
        bootstrapPopup(
            "Anexar arquivos(Documentos)",
            'div_modal_acompanhamento_anexo',
            APPRAIZ. 'acompanhamento/modulos/principal/acompanhamento_anexo_pnc_form.inc',
            array('fechar'),
            array(
                'tamanho' => 'xl',
                'acpid' => $acpid
            )
        );
    }
?>

<script>

    const numeroPassoIndicadores = 0;
    const numeroPassoAtividades = 1;
    const numeroPassoAnexos = 2;
    
    $(function(){
        
        var wizard = $("#wizard").steps({
            transitionEffect: "slide",
            startIndex: buscarPassoAtual(),
            enableFinishButton: exibirBotaoEnviar(),
            labels: {
                cancel: "Cancelar",
                current: "Passo atual:",
                pagination: "Paginação",
                next: "Próximo",
                previous: "Anterior",
                finish: "Enviar",
                loading: "Carregando ..."
            },
            onStepChanging: function(event, currentIndex, newIndex){
                var permitirAvancarPasso = true;
                switch(currentIndex) {
                    case numeroPassoIndicadores:
                        if(newIndex === numeroPassoAtividades){
                            permitirAvancarPasso = true;
                            if(permitirAvancarPasso === false){
                                swal('', 'Por favor, preencha pelo menos os indicadores do ano de '+ $('#chosen-exercicio').val()+ '.', 'warning');
                            }
                        }
                        break;
                    case numeroPassoAtividades:
                        if(newIndex === numeroPassoAnexos){
                            permitirAvancarPasso = verificarPreenchimentoAtividade();
                            if(permitirAvancarPasso === false){
                                swal('', 'Por favor, insira pelo menos uma atividade.', 'warning');
                            }
                        }
                        break;
                }
                return permitirAvancarPasso;
            },
            onFinished: function(event, currentIndex){
                enviarAcompanhamento();
            }
        });
        
        // Evento de terminar de carregar arquivos
        Dropzone.options.formularioAcompanhamentoAnexo = {
            init: function() {
                
                this.on("success", function(file, response){
                    var jsonResponse = $.parseJSON(response);
                    carregarListaAnexoPnc(jsonResponse.acpid);
                });

                this.on("queuecomplete", function(file){

                    // Armazena o objeto Dropzone para chamar métodos
                    objFormularioAcompanhamentoAnexo = this;
                    // Chama mensagem de sucesso
                    swal({
                      title: "",
                      text: "Arquivos salvos com sucesso!",
                      timer: 2000,
                      showConfirmButton: false,
                      type: "success"
                    }, function(){
                        // Fecha o swal alert
                        swal.close();
                        // limpa campo de upload
                        objFormularioAcompanhamentoAnexo.removeAllFiles();
                        // fecha modal após a seleção
                        $('#div_modal_acompanhamento_anexo').modal('hide');
                    });
                });
            }
        };
        
        // Evento do componente de exibir Número da meta
        $('.div-indicador').mouseenter(function(){
            var cor = $(this).data('cor');
            $(this).removeClass(cor).addClass('blue-bg');
        });

        // Evento do componente de exibir Número da meta
        $('.div-indicador').mouseleave(function(){
            var cor = $(this).data('cor');
            $(this).removeClass('blue-bg').addClass(cor);
        });

        $('.selecionar-meta-pnc').click(function(){
            if(!$('#suoid').val()){
                swal('', 'Por favor, escolha uma Unidade.', 'warning');
                return false;
            }
        });

        $('#suoid').change(function(){
            window.location.href = '?modulo=principal/monitorar-pnc&acao=A&suoid=' + $(this).val();
        });

        $('.detalhe-pi').click(function(){
            var url = $(this).attr('href');
            $('#detalhe-pi .modal-body').load(url);
            $('#detalhe-pi').modal();
            return false;
        });

        $('.a_grafico').click(function(){
            exibirGrafico();
            return false;
        });
        
        $('.modal-historico-indicador-pnc').click(function(){
            var url = '?modulo=principal/monitorar-pnc&acao=A'+
                '&req=listar-historico-indicador-pnc'+
                '&idpid='+ $('input[name=idpid]').val();
            $('#div_modal_lista_historico_indicador_pnc .modal-body').load(url);
            $('#div_modal_lista_historico_indicador_pnc').modal();
            return false;
        });

        // Eventos do ícone(ou número) de edição da lista de indicadores PNC
        $('.div_lista_indicadores_pnc').on('click', '.editar_indicador', function(){
            var idpid = $(this).attr('data-idpid');
            var acpid = $(this).attr('data-acpid');
            var unocod = $(this).attr('data-unocod');
            var suocod = $(this).attr('data-suocod');
            var ipncod = $(this).attr('data-ipncod');
            var prsano = $(this).attr('data-prsano');
            var url = '?modulo=principal/monitorar-pnc&acao=A'+
                '&req=acompanhamento-indicador-pnc-form'+
                '&idpid='+ idpid+
                '&acpid='+ acpid+
                '&unocod='+ unocod+
                '&suocod='+ suocod+
                '&ipncod='+ ipncod+
                '&prsano='+ prsano;

            $('#div_modal_acompanhamento_indicador').find('div.modal-body').empty();
            $('#div_modal_acompanhamento_indicador').find('div.modal-body').load(url);
            $('#div_modal_acompanhamento_indicador').modal();
            return false;
        });
        
        // Evento do botão salvar da janela modal do Cadastro de Indicadores PNC
        $('#div_modal_acompanhamento_indicador').find('.btn-salvar').click(function(){
            $('#formulario-acompanhamento-indicador').find('[type="submit"]').trigger('click');
        });
        
        // Eventos do ícone(ou número) de edição da lista de atividades PNC
        $('.div_lista_atividades_pnc').on('click', '.editar_atividade', function(){
            var atvid = $(this).attr('data-atvid');
            var acpid = $(this).attr('data-acpid');
            var url = '?modulo=principal/monitorar-pnc&acao=A'+
                '&req=acompanhamento-atividade-pnc-form'+
                '&atvid='+ atvid+
                '&acpid='+ acpid;

            $('#div_modal_acompanhamento_atividade').find('div.modal-body').empty();
            $('#div_modal_acompanhamento_atividade').find('div.modal-body').load(url);
            $('#div_modal_acompanhamento_atividade').modal();
            return false;
        });
        
        // Eventos do ícone(ou número) de exclusão da lista de atividades PNC
        $('.div_lista_atividades_pnc').on('click', '.excluir_atividade', function(){
            var acpid = $(this).attr('data-acpid');
            var atvid = $(this).attr('data-atvid');
            var descricaoItem = $(this).attr('data-atvtitulo');
            excluirAtividade(acpid, atvid, descricaoItem);
            return false;
        });
        
        // Eventos do ícone(ou número) de exclusão da lista de atividades PNC
        $('.div_lista_anexos_pnc').on('click', '.excluir_anexo', function(){
            var acpid = $(this).attr('data-acpid');
            var arqid = $(this).attr('data-arqid');
            var descricaoItem = $(this).attr('data-descricao');
            excluirAnexo(acpid, arqid, descricaoItem);
            return false;
        });
        
        // Evento do botão salvar da janela modal do Cadastro de Indicadores PNC
        $('#div_modal_acompanhamento_atividade').find('.btn-salvar').click( function(){
            $('#formulario-acompanhamento-atividade').find('[type="submit"]').trigger('click');
        });

        // Eventos do ícone(ou número) de edição da lista de atividades PNC
        $('.div_lista_anexos_pnc').on('click', '.inserir_anexo', function(){
            $('#div_modal_acompanhamento_anexo').modal();
            return false;
        });

    });

    /**
     * Retorna o número do passo atual de acordo com quais preenchimentos o usuário já fez até o momento.
     * 
     * @returns {integer}
     */
    function buscarPassoAtual(){
        if(verificarPreenchimentoIndicadoresAtual()){
            passoAtual = numeroPassoAtividades;
            if(verificarPreenchimentoAtividade()){
                passoAtual = numeroPassoAnexos;
            }
        }
        else {
            passoAtual = numeroPassoIndicadores;
        }
        
        return passoAtual;
    }

    /**
     * Concluir acompanhamento de Meta PNC.
     * 
     * @returns {void}
     */
    function enviarAcompanhamento(){
        swal({
            title: "Confirmação",
            text: "Você deseja concluir e enviar O PREECHIMENTO de TODOS os Indicadores e da Meta PNC "+ $('.h1_codigo_meta').text()+ '?',
            type: "warning",
            showCancelButton: true,
            confirmButtonColor: '#DD6B55',
            confirmButtonText: 'Sim, eu quero.',
            cancelButtonText: "Não, eu não quero!",
            closeOnConfirm: false,
            closeOnCancel: false
        },
        function(isConfirm){
            if(isConfirm){
                $.ajax({
                    url: '?modulo=principal/monitorar-pnc&acao=A&req=enviar-acompanhamento-pnc&acpid='+ $('#acpid').val(),
                    type: 'POST',
                    dataType: 'json',
                    success: function(response){
                        swal("", "Dados de monitoramento dos indicadores e da Meta PNC enviados!", "success");
                        divCarregando();
                        var mpnidProximo = $('#mpnid_proximo').val();
                        if(mpnidProximo != ''){
                            window.location.href = '?modulo=principal/monitorar-pnc&acao=A&suoid='+ $('#suoid').val()+ '&mpnid='+ mpnidProximo;
                        } else {
                            window.location.href = '?modulo=inicio&acao=C&suoid='+ $('#suoid').val();
                        }
                    }
                });
             } else {
               swal("Envio Cancelado", "Seus dados permanecem disponíveis pra cadastro :)", "warning");
             }
        });
    }

    /**
     * Exclui a atividade selecionada.
     * 
     * @param {int} acpid
     * @param {int} atvid
     * @param {string} descricaoItem
     * @returns {void}
     */
    function excluirAtividade(acpid, atvid, descricaoItem){
        swal({
            title: "Confirmação",
            text: "Você quer excluir essa atividade: "+ descricaoItem+ '?',
            type: "warning",
            showCancelButton: true,
            confirmButtonColor: '#DD6B55',
            confirmButtonText: 'Sim, eu quero.',
            cancelButtonText: "Não, eu não quero!",
            closeOnConfirm: false,
            closeOnCancel: false
        },
        function(isConfirm){
            if(isConfirm){
                $.ajax({
                    url: '?modulo=principal/monitorar-pnc&acao=A&req=excluir-acompanhamento-atividade-pnc&acpid='+ acpid+ '&atvid='+ atvid,
                    type: 'DELETE',
                    dataType: 'json',
                    success: function(response){
                        carregarListaAtividadePnc(response.acpid);
                        swal("Lista atualizada!", "Atividade excluída!", "success");
                    }
                });
             } else {
               swal("Exclusão Cancelada", "Sua Atividade permanece cadastrada :)", "warning");
             }
        });
    }
    
    /**
     * Exclui anexo selecionado.
     * 
     * @param {int} acpid
     * @param {int} arqid
     * @param {string} descricaoItem
     * @returns {void}
     */
    function excluirAnexo(acpid, arqid, descricaoItem){
        swal({
            title: "Confirmação",
            text: "Você quer excluir esse anexo: "+ descricaoItem+ '?',
            type: "warning",
            showCancelButton: true,
            confirmButtonColor: '#DD6B55',
            confirmButtonText: 'Sim, eu quero.',
            cancelButtonText: "Não, eu não quero!",
            closeOnConfirm: false,
            closeOnCancel: false
        },
        function(isConfirm){
            if(isConfirm){
                $.ajax({
                    url: '?modulo=principal/monitorar-pnc&acao=A&req=excluir-acompanhamento-anexo-pnc&acpid='+ acpid+ '&arqid='+ arqid,
                    type: 'DELETE',
                    dataType: 'json',
                    success: function(response){
                        carregarListaAnexoPnc(response.acpid);
                        swal("Lista atualizada!", "Anexo excluído!", "success");
                    }
                });
             } else {
               swal("Exclusão Cancelada", "Seu anexo permanece salvo :)", "warning");
             }
        });
    }
    
    /**
     * Exibe a janela modal do grafico de execução orçamentária Da Meta da Unidade.
     * 
     * @returns {undefined}
     */
    function exibirGrafico(){
        var suoid = $('#suoid').val();
        var mpnid = $('#mpnid').val();
        var url = '?modulo=principal/monitorar-pnc&acao=A&req=exibir-grafico'+ '&suoid='+ suoid+ '&mpnid='+ mpnid;
        $('#modal_grafico .modal-body').load(url);
        $('#modal_grafico').modal();
    }

    /**
     * Carrega a lista de indicadores PNC via AJAX.
     * 
     * @param {type} acpid
     * @returns {undefined}
     */
    function carregarListaIndicadorPnc(acpid){
        var url = '?modulo=principal/monitorar-pnc&acao=A&req=listar-indicadores-pnc'+ '&acpid='+ acpid;
        $('.div_lista_indicadores_pnc').empty();
        $('.div_lista_indicadores_pnc').load(url);
    }
    
    /**
     * Carrega a lista de indicadores PNC via AJAX.
     * 
     * @param {type} acpid
     * @returns {undefined}
     */
    function carregarListaAtividadePnc(acpid){
        var url = '?modulo=principal/monitorar-pnc&acao=A&req=listar-atividades-pnc'+ '&acpid='+ acpid;
        $('.div_lista_atividades_pnc').empty();
        $('.div_lista_atividades_pnc').load(url);
    }
    
    /**
     * Carrega a lista de anexos das metas PNC via AJAX.
     * 
     * @param {type} acpid
     * @returns {undefined}
     */
    function carregarListaAnexoPnc(acpid){
        var url = '?modulo=principal/monitorar-pnc&acao=A&req=listar-anexos-pnc'+ '&acpid='+ acpid;
        $('.div_lista_anexos_pnc').empty();
        $('.div_lista_anexos_pnc').load(url);
    }

    /**
     * Verifica se todos os indicadores do ano atual foram preenchidos
     * 
     * @returns {Boolean}
     */
    function verificarPreenchimentoIndicadoresAtual(){
        var resultado = false;
        var indicadoresPreenchidos = $('#hid_indicadores_preenchidos').val();
        if(indicadoresPreenchidos === "1"){
            resultado = true;
        }
        return resultado;
    }
    
    /**
     * Verifica se pelo menos uma atividade foi inserida
     * 
     * @returns {Boolean}
     */
    function verificarPreenchimentoAtividade(){
        var resultado = false;
        var atividadePreenchida = $('#hid_atividade_preenchida').val();
        if(atividadePreenchida === "1"){
            resultado = true;
        }
        return resultado;
    }
    
    /**
     * Verifica se o acompanhamento da meta ainda não foi enviado e permite exibir o botão de envio.
     * 
     * @returns {Boolean}
     */
    function exibirBotaoEnviar(){
        var resultado = $('#pode_editar').val() == '1'? true: false;
        return resultado;
    }

</script>
