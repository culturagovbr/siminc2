jQuery.noConflict();

/*** Quando o DOM estiver pronto ***/
jQuery(document).ready(function()
{
	/*** Redireciona para a p�gina do relat�rio ***/
	jQuery("#btGeralConsolidado").click(function()
	{
		window.location = 'pdeescola.php?modulo=earelatorio/relatorio_plano_atendimento&acao=A';
	});
}); 