function ajaxatualizar(params,iddestinatario) {
	jQuery.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: params,
   		async: false,
   		success: function(html){
   			if(iddestinatario!='') {
   				document.getElementById(iddestinatario).innerHTML = html;
   			}
   		}
	});

}

/* Fun��o para subustituir todos */
function replaceAll(str, de, para){
    var pos = str.indexOf(de);
    while (pos > -1){
		str = str.replace(de, para);
		pos = str.indexOf(de);
	}
    return (str);
}

function inserirPeriodo() {

	if(jQuery('#pertitulo').val()=='') {
		alert('T�tulo em branco');
		return false;
	}
	if(jQuery('#perdescricao').val()=='') {
		alert('Descri��o em branco');
		return false;
	}
	if(jQuery('#periniciovalidade').val()=='') {
		alert('Data in�cio validade em branco');
		return false;
	}
	if(jQuery('#perfimvalidade').val()=='') {
		alert('Data fim validade em branco');
		return false;
	}
	if(jQuery('#perinicioaberturapreenchimento').val()=='') {
		alert('Data in�cio preenchimento em branco');
		return false;
	}
	if(jQuery('#perfimaberturapreenchimento').val()=='') {
		alert('Data fim preenchimento em branco');
		return false;
	
	}
	document.getElementById('formulario').submit();
}

function exibirSubacao(idSubacao, codSubacao) {
    url = "planacomorc.php?modulo=principal/acoes/dadosubacao&acao=A&id_subacao="+idSubacao;
    $.post(url, function(html) {
        $('#modal-confirm .modal-body p').html(html);
        $('.modal-dialog').css('width', '90%');
        $('#modal-confirm .modal-title').html('Dados da Suba��o - '+codSubacao);
        $('#modal-confirm .btn-primary').remove();
        $('#modal-confirm .btn-default').html('Fechar');
        $('.modal-dialog').show();
        $('#modal-confirm').modal();
    });
}

/**
 * Rola a tela para poder visualizar o campo indicado em referencia.
 * @param {string} referencia Elemento utilizado como referencia para a rolagem da tela.
 */
function rolaTela(referencia) {
  $('html, body').animate({scrollTop: $('#'+referencia)
    .offset().top - 100}, 500);
}

function altCoordenador(acao,id_acao_programatica,percod,permissao,tipo){
    if(!permissao){
        $('#modal-confirm .modal-body p').html('<section class="alert alert-danger text-center">Seu perfil n�o possui permiss�o para efetuar tal a��o.</section>');
        $('.modal-dialog').css('width', '50%');
        $('#modal-confirm .modal-title').html('A��o: '+ acao);        
        $('#modal-confirm .btn-default').html('Fechar');
        $('.modal-dialog').show();
        $('#modal-confirm').modal();
        return;
    }
    url = "planacomorc.php?modulo=principal/acoes/gerenciarunidades&acao=A&requisicao=coordenadorValidadorAcao&id_acao_programatica="+id_acao_programatica+"&percod="+percod+"&tipo="+tipo;
    $.post(url, function(html) {
        $('#modal-confirm .modal-body p').html(html);
        $('.modal-dialog').css('width', '50%');
        $('#modal-confirm .modal-title').html('Alterar Respons�vel da a��o: '+ acao);        
        $('#modal-confirm .btn-default').html('Fechar');
        $('#modal-confirm .btn-primary').html('Salvar').attr('onclick','validarCoordenadorValidador();');
        $('.modal-dialog').show();
        $('#modal-confirm').modal();
    });
}

function carregaCoordenadorValidador(pflcod) {
    var usucpf = $('#cpf').val();
    if (usucpf.length == 14){
        usucpf = usucpf.replace('-', '').replace('.', '').replace('.', '');        
        url = "planacomorc.php?modulo=principal/acoes/gerenciarunidades&acao=A&requisicao=recuperaCoordenadorValidador&cpf=" + usucpf + "&pflcod="+pflcod;
        $.ajax(url, {async:false,dataType:'json', success:function(data){
            $('#mensagem_coord').html('');    
            if (data) {
                if(data.permissao == true){
                    $('#modal-confirm .btn-primary').show();
                    $('#nome').val(data.nome);
                    $('#foneddd').val(data.ddd);
                    $('#fone').val(data.fone);
                    $('#email').val(data.email);
                    return;
                }else{
                    $('#mensagem_coord').html('<section class="alert alert-danger text-center">Este usu�rio n�o possui permiss�o para ser respons�vel por a��es</section>');
                    $('#modal-confirm .btn-primary').hide();
                }
            }
            
            $('#nome').val('');
            $('#foneddd').val('');
            $('#fone').val('');
            $('#email').val('');
        }, error: function(){
            $('#mensagem_coord').html('<section class="alert alert-danger text-center">Falha ao carregar CPF</section>');
        }});
    }else{
        $('#mensagem_coord').html('<section class="alert alert-danger text-center">CPF Inv�lido</section>');
    }
}

function validarCoordenadorValidador(){    
    if ($('#cpf').val() == '') {        
        $('#mensagem_coord').html('<section class="alert alert-danger text-center">Preencha o CPF</section>');
        return false;
    }
    if ($('#nome').val() == '' || $('#foneddd').val() == '' || $('#email').val() == '') {        
        $('#mensagem_coord').html('<section class="alert alert-danger text-center">CPF inv�lido para a��o</section>');
        return false;
    }    
    $('#cpf').val(mascaraglobal('###.###.###-##', $('#cpf').val()))
    if (!validar_cpf($('#cpf').val())) {
        $('#mensagem_coord').html('<section class="alert alert-danger text-center">CPF Inv�lido</section>');
        return false;
    }    
    url = "planacomorc.php?modulo=principal/acoes/gerenciarunidades&acao=A&requisicao=gravarResponsabilidadeAcao&id_acao_programatica=" + $('#id_acao_programatica').val() + "&id_periodo_referencia=" + $('#id_periodo_referencia').val() + "&cpf=" + $('#cpf').val() + "&pflcod=" + $('#pflcod').val();
    $.ajax(url,
    {async:false,dataType:'json', 
        success:function(data){
            if(data.resultado == true){
                $('#mensagem_coord').html('<section class="alert alert-success text-center">Respons�vel pela A��o atualizado com sucesso. Aguarde, atualizaremos a p�gina.</section>');
                setTimeout(function() {    
                    window.location.href = window.location.href;
                }, 2000);
            }else{
                $('#mensagem_coord').html('<section class="alert alert-danger text-center">Falha ao atualizar Respons�vel pela A��o</section>');
            }
        },error:function(){
            $('#mensagem_coord').html('<section class="alert alert-danger text-center">Falha ao Atualizar Respons�vel</section>'); 
        }
    });    
}
