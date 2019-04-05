
    /**
     * Ações efetuadas quando a tela de lista de PI é iniciada.
     * 
     * @returns VOID
     */
    function initListaEmendas(){
        
        $('.dataTablesSP').DataTable({
            bPaginate: false,
            responsive: true,
            dom: '<"html5buttons"B>lTfgitp',
            "language": {
                "url": "/zimec/public/temas/simec/js/plugins/dataTables/Portuguese-Brasil.json"
            }
        });

        $('.editar_situacao').click(function(){
            $('#proposta_modal .modal-body').load('emendas.php?modulo=inicio&acao=C&req=proposta_modal&benid='+ $(this).data('benid'));
            $('#proposta_modal').modal();
        });
        
        $('.btn-salvar').click(function(){
            $('#formulario-selecionar-proposta').submit();
        });
        
        $('#btn_novo').click(function(){
            window.location.href = '?modulo=principal/emenda_form&acao=A';
        });
        
        $('#btn-limpar').click(function(){
            $('#req').val('limpar');
            $('#formFiltroEmenda').submit();
        });

        $('#btn-exportar-xls').click(function(){
            $('#req').val('listaremendas-xls');
            $('#formFiltroEmenda').submit();
            $('#req').val('');
        });
        
        $('.a_espelho').click(function(){
            var pliid = $(this).attr('data-pi');
            exibirEspelhoPi(pliid);
            return false;
        });

        // Evento ao mudar opção de UO
        $('#unicod').change(function(){
            carregarUG($(this).val());
            $('#span-funcional').load('?modulo=inicio&acao=C&req=carregarAcaoPorUo&unicod=' + $(this).val());
        });

    }

    /**
     * Exibe popup com Detalhes do pi. Tela de Espelho de PI.
     *
     * @returns VOID
     */
    function exibirEspelhoPi(pliid){
        window.open(
            '?modulo=principal/beneficiario_form&acao=A&req=espelho-pi&pliid='+ pliid,
            'popup_espelho_pi',
            'width=780,height=1000,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
    }

    /**
     * Carrega novo conteúdo para a opções de Sub-Unidade via requisição ajax.
     *
     */
    function carregarUG(unicod) {
        $.post(
            '?modulo=inicio&acao=C&req=carregarComboUG', { unicod: unicod}, function(response) {
                $('#div_ungcod').remove('slow');
                $('#div_ungcod').html(response);
                $(".chosen-select").chosen();
        });
    }



