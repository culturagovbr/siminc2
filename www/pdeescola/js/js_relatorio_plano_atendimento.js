/*** Quando o DOM estiver pronto ***/
$(document).ready(function()
{ 
	/*** Verifica/Gera por AJAX o relat�rio para o lote de escolas. ***/
	$("#bt_gerar_relatorio").click(function()
	{
		if( confirm('Deseja gerar o relat�rio para todas as escolas?') )
		{
			$.ajax
			({
			  type: "post",
			  data: "ajax=1",
			  dataType: "json",
			  url: 'pdeescola.php?modulo=earelatorio/relatorio_plano_atendimento&acao=A',
			  success: function(data)
			  {
				 /*** Tudo OK, gerou o lote... ***/
				 if(data.valida)
				 {
					 /*** Exibe a janela com o Rel. Geral Consolidado ***/
					 var janela = window.open('pdeescola.php?modulo=earelatorio/pop_relatorio_plano_atendimento&acao=A&lote=' + data.lote, 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
					 janela.focus();
					 /*** Recarrega a tela ***/
					 window.location.reload();
				 }
				 /*** N�o conseguiu gerar o lote de escolas ***/
				 else
				 {
					/*** Exibe a mensagem de erro ***/
					alert("N�o foi poss�vel gerar o Rel.Consolidado para as escolas.\n" +
						  "Todas as escolas devem estar no estado 'Finalizado' para que esta a��o ocorra.");
				 }
			  }
			});
		}
	});
	
	/*** Redireciona para a p�gina do relat�rio ***/
	$("#bt").click(function()
	{
		
	});
});

/*** Fun��o para carregar o combo do Munic�pios atrav�s do combo de estados ***/
function filtraMunicipio(estuf)
{
	$("#muncod").attr("disabled", true);

	if(estuf)
	{
		$.ajax
		({
		  type: "post",
		  data: "ajaxestuf=" + estuf,
		  url: 'pdeescola.php?modulo=earelatorio/relatorio_plano_atendimento&acao=A',
		  success: function(data)
		  {
			$("#tdMunicipio").html('');
			$("#tdMunicipio").html(data);
		  }
		});
	}
	else
	{
		$("#tdMunicipio").html = "<select name='muncod' id='muncod'><option value=''>Selecione...</option></select>";
	}
}

/*** Submete o formul�rio da pesquisa ***/
function submeteFiltro()
{
	$("#btFiltroRelatorio").attr("disabled", true);
	$("#formFiltroRelatorio").submit();
}

function visualizaRelatorio(lote, muncod, estuf)
{
	var janela = window.open('pdeescola.php?modulo=earelatorio/pop_relatorio_plano_atendimento&acao=A&lote='+lote+'&muncod='+muncod+'&estuf='+estuf, 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
	janela.focus();
}

function realizaPagamento(lote)
{
	var janela = window.open('pdeescola.php?modulo=earelatorio/pop_realizar_pagamento&acao=A&lote='+lote, 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
	janela.focus();
}