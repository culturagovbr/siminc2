<?php
/**
 * $Id: remaneja.inc 102359 2015-09-11 18:26:07Z maykelbraz $
 */

include_once '_funcoesremanejavalorsubacao.php';

if (isset($_REQUEST['requisicao']) && 'remanejamentoDeSubacao' == $_REQUEST['requisicao']) {
    $url = 'planacomorc.php?modulo=principal/remanejavalorsubacao/listaptres&acao=A';
    $msg = 'N�o foi poss�vel executar sua requisi��o.';
    if (remanejamentoDeSubacao($_POST)) {
        $msg = 'Sua requisi��o foi executada com sucesso.';
        if ('E' == $_POST['tipotransacao']) {
            //atendendo demanda 225075 - Ao confirmar o remanejamento de suba��o, retornar para a listagem de PTRES para remanejar.
            //$url = 'planacomorc.php?modulo=principal/remanejavalorsubacao/listartransacoes&acao=A&tipoTransacao=E';
        }
    }
    ?>
    <script type="text/javascript" language="javascript">
        alert('<?php echo $msg; ?>');
        location.href = '<?php echo $url; ?>';
    </script>
    <?php
    exit();
} elseif ($_REQUEST['requisicao']) {
    $_REQUEST['requisicao']($_REQUEST);
    exit;
}

//Chamada de programa
include APPRAIZ . "includes/cabecalho.inc";
echo "<br>";
/* Variaveis gerais */
$obrigatorias = UNIDADES_OBRIGATORIAS;
if (isset($_REQUEST['rmsid'])) {
    $sql = "
		SELECT rsb.ptrid,
	       	rsb.tipotransacao,
	       	rsb.ptrid,
	       	rsb.funcprogramatica,
	       	rsb.po,
	       	rsb.podsc,
	       	rsb.ptrdotacao,
	       	rsb.ptrempenhado,
	       	rsb.detalhadosubacao,
	       	rsb.ptrsaldo,
	       	rsb.datatransacao,
	       	rsb.usucpf,
	       	rsb.rmpsaldosubtraido,
	       	rsb.rmpsaldoadicionado,
	       	rsb.rmsidorigem,
	       	rsb.dscalteracao,
	       	rsb.dscjustificativa,
			rsb.dscexecucao,
	       	ptr.ptres
  		FROM planacomorc.remanejamentosubacao rsb
    	LEFT JOIN monitora.ptres ptr USING(ptrid)
  		WHERE rmsid = %d
	";
    $stmt = sprintf($sql, $_REQUEST['rmsid']);
    $dadosTransacao = $db->carregar($stmt);
    if ($dadosTransacao) {
        $dadosTransacao = array_shift($dadosTransacao);
    }
    $_REQUEST['ptrid'] = $dadosTransacao['ptrid'];
}
$ptrid = $_REQUEST['ptrid'];

/* Pegando dados do PTRES */
$sql = "
	SELECT ptr.funcod,
    	ptr.sfucod,
       	ptr.prgcod,
       	ptr.acacod,
       	ptr.loccod,
       	ptr.unicod,
       	uni.unidsc,
       	ptr.plocod,
       	plo.plotitulo,
       	ptr.ptres,
       	COALESCE(ptr.ptrdotacao, 0.00) AS ptrdotacao,
       	COALESCE(pte.total, 0.00) AS empenhado,
       	COALESCE(sadvalor, 0.00) AS detalhadosubacao,
       	COALESCE(ptr.ptrdotacao, 0.00) - COALESCE(sadvalor, 0.00) AS saldo
  	FROM monitora.ptres ptr
    LEFT JOIN public.unidade uni USING(unicod)
    LEFT JOIN monitora.planoorcamentario plo ON (plo.prgcod = ptr.prgcod AND plo.acacod = ptr.acacod AND plo.plocodigo = ptr.plocod AND plo.unicod = ptr.unicod)
    LEFT JOIN (SELECT ptrid, SUM(sadvalor) AS sadvalor FROM monitora.pi_subacaodotacao GROUP BY ptrid) dt USING(ptrid)
    LEFT JOIN siafi.ptrempenho pte ON ptr.ptres = pte.ptres AND pte.exercicio = '{$_SESSION['exercicio']}'
  	WHERE ptr.ptrid = {$ptrid}
";

$dadosPtres = $db->pegaLinha($sql);

// -- Identificando se o usu�rio atual faz parte do CGSO/CPMO
$pulaSolicitacao = pulaSolicitacao();

// -- Definindo o tipo da transa��o que ser� criada
$tipoTransacao = ($pulaSolicitacao || isset($_REQUEST['rmsid']) ? 'E' : 'S');
// -- Definindo a funcional program�tica
$funcProgramatica = $dadosPtres['funcod'] . '.' . $dadosPtres['sfucod'] . '.' . $dadosPtres['prgcod'] . '.'
        . $dadosPtres['acacod'] . '.' .  $dadosPtres['unicod'] . '.' . $dadosPtres['loccod'];



?>
<script src="./js/planacomorc.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<!-- <script language="javascript" type="text/javascript" src="../../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script> -->
<!-- <link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/> -->
<!-- <script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script> -->
<script>
        //Fun��o que executa a busca textual na tabela de suba��es do ADICIONAR
	$(document).ready(function() {
    	$('.rem_sba_v').blur(function() {
        	var limite = parseFloat(replaceAll(replaceAll($(this).parent().prev().html(), '.', ''), ',', '.')).toFixed(2);
            var valor = jQuery(this).val();
            valor = valor.replace(/\./g, '').replace(/,/g, '.');
            if (parseFloat(valor) > parseFloat(limite)) {
            	jQuery(this).val(limite);
                jQuery(this).keyup();
            }
		});

    	pesquisaRapida("textFind","primeira-tabela");
    	pesquisaRapida("textFind2","segunda-tabela");
		jQuery('#enviar').click(remanejar);
        jQuery('#tabs').tabs();

        // -- Mostrando apenas remanejamentos preenchidos
        jQuery('#remanejamento_1,#remanejamento_2').click(function() { // -- Para cada checkbox de filtro
            var check = this;
			jQuery(this).closest("table").next().find('.tabela-listagem').each(function() { // -- Pegue a tabela mais pr�ximo
				if (!$(check).hasClass('active')) { // -- Se o checkbox estiver marcado
            		jQuery("input[type='text']", this).each(function() { // -- Pegue todos os inputs do tipo text dentro da tabela
                        var value = jQuery(this).val();
						if (!(value && parseFloat(value) > 0)) { // -- Some toda a tr do cara se ele estiver vazio, ou com zero
                        	jQuery(this).parent().closest('tr').addClass('remover');
                    	}
                	});
                } else { // -- Se n�o estiver marcado, mostra todas as tr
                	jQuery('tr.remover', this).removeClass('remover');
            	}
        	});
        });

		if (<?php echo isset($_REQUEST['rmsid']) ? 'true' : 'false'; ?>) {
		<?php
			$sql = "
				SELECT *
	  			FROM planacomorc.remanejamentosubacao rsb
	   			LEFT JOIN planacomorc.rmsmovimentacao rmv USING(rmsid)
	  			WHERE rmsid = %d
			";
			$stmt = sprintf($sql, $_REQUEST['rmsid']);
			$dataMovimentacao = $db->carregar($stmt);
			if (is_array($dataMovimentacao)) {
				foreach ($dataMovimentacao as $mov) {
					if ('-' == $mov['tipomovimento']) {
				    	$id = "rem_sba_{$mov['sbaid']}";
					} elseif ('+' == $mov['tipomovimento']) {
				        $id = "rem_adc_sba_{$mov['sbaid']}";
				    }
				    if ($id) { // -- Algumas transa��es est�o sem valores de movimenta��o
						echo "jQuery('#{$id}').val(({$mov['vlrmovimento']}).toFixed(2)).keyup();";
				    }
				}
			}
				// -- Bloqueio de campos se as informa��es da transa��o forem do tipo E
			if ('E' == $dadosTransacao['tipotransacao']) {
				echo "jQuery('#remanejar :input').attr('disabled', 'disabled');";
				echo "jQuery(':checkbox, #textFind').removeAttr('disabled');";
			}
			?>
    	}
	});

	function pesquisaRapida(id,idLocal){
   		jQuery.expr[':'].contains = function(a, i, m) {
   	    	return jQuery(a).text().toUpperCase().indexOf(m[3].toUpperCase()) >= 0;
   	 	};

   	 	$("#"+id).keyup(function(){
   			$('#'+idLocal+' table.table tbody tr td').removeClass('marcado');
   	    	$('#'+idLocal+' table.table tbody tr').removeClass('remover');
   	    	stringPesquisa = $("#"+id).val();
   	    	if (stringPesquisa) {
   	    		$('#'+idLocal+' table.table tbody tr td:contains(' + stringPesquisa + ')').addClass('marcado');
   	    		$('#'+idLocal+' table.table tbody tr:not(:contains(' + stringPesquisa + '))').addClass('remover');
   	    	}
		});
   	}

	function calculaSaldo(el) {
    	var somaSBA = 0;
        var saldoRemanejado = 0;

        /*  Valor saindo das Suba��es */
        $('.rem_sba_v').each(function() {
        	if ($(this).val() != '') {
            	somaSBA += parseFloat(replaceAll(replaceAll($(this).val(), '.', ''), ',', '.'));
			}
		});

		/*  Recalculando saldo das Suba��es linha a linha*/
        if (el) {
        	//valorDot = parseFloat(replaceAll(replaceAll($(el).parent().parent().parent().parent().prev().find(':last').html(),'.',''),',','.')).toFixed(2);
        	valorDot = parseFloat(replaceAll(replaceAll($(el).parent().prev().html(), '.', ''), ',', '.')).toFixed(2);
        	valorDotInput = parseFloat(replaceAll(replaceAll($(el).val(), '.', ''), ',', '.')).toFixed(2);

            if (isNaN(valorDotInput)) {
                valorDotInput = 0;
            }

            saldoLinha = valorDot - valorDotInput;
            if (isNaN(saldoLinha)) {
                saldoLinha = valorDot;
            }

            // -- Calcula Or�amento da suba��o ap�s o remanejamento (R$)
            var conteudoSaldo = mascaraglobal('###.###.###.###,##', parseFloat(saldoLinha).toFixed(2));
            if (saldoLinha >= 0) {
            	conteudoSaldo = '<span>' + conteudoSaldo + '</span>';
			} else {
            	conteudoSaldo = '<span style="color:red">-' + conteudoSaldo + '</span>';
            }
			$(':nth-child(10)', $(el).closest('tr')).html(conteudoSaldo);
			//$(el).parent().parent().parent().parent().prev().find(':last').html(conteudoSaldo);
		}

		saldoRemanejado += somaSBA;
        $('#saldo_remanejado_total').html('R$ ' + mascaraglobal('###.###.###.###,##', saldoRemanejado.toFixed(2)));
        calculaDiferenca();
	}

	function calculaSaldoAdic(el) {
    	var somaSBA = saldoRemanejado = 0;
        $('.rem_adc_sba_v').each(function() {
        	if ($(this).val() != '') {
        		somaSBA += parseFloat(replaceAll(replaceAll($(this).val(), '.', ''), ',', '.'));
            }
		});

		if(el){
        	// -- Recalculando saldo SOMADO das Suba��es linha a linha
            valorDot = parseFloat(replaceAll(replaceAll(replaceAll($(el).parent().prev().prev().prev().prev().html(), '<br> ', ''), '.', ''), ',', '.'));
            valorDotInput = parseFloat(replaceAll(replaceAll($(el).val(), '.', ''), ',', '.'));
            saldoLinha = valorDot + valorDotInput;
            if (isNaN(saldoLinha)) {
                saldoLinha = valorDot;
            }
            var conteudoSaldo = mascaraglobal('###.###.###.###,##', saldoLinha.toFixed(2));
            if (saldoLinha >= 0) {
            	conteudoSaldo = '<span>' + conteudoSaldo + '</span>';
            } else {
                conteudoSaldo = '<span style="color:red">-' + conteudoSaldo + '</span>';
            }
            $(':nth-child(9)', $(el).closest('tr')).html(conteudoSaldo);
		}

		saldoRemanejado += somaSBA;
        $('#saldo_remanejado_total_adicionado').html('R$ ' + mascaraglobal('###.###.###.###,##', saldoRemanejado.toFixed(2)));
        calculaDiferenca();
	}

	function calculaDiferenca(){
		var diferenca = 0;
        var saldoRemanejado = jQuery('#saldo_nao_orcado').text();

        saldoRemanejado = saldoRemanejado.replace(/\./g, '').replace(',', '.').replace('R$ ', '');
        saldoRemanejado = parseFloat(saldoRemanejado);

        retirado = parseFloat(replaceAll(replaceAll(replaceAll($('#saldo_remanejado_total').html(), 'R$ ', ''), '.', ''), ',', '.'));

        adicionado = parseFloat(replaceAll(replaceAll(replaceAll($('#saldo_remanejado_total_adicionado').html(), 'R$ ', ''), '.', ''), ',', '.'));
        diferenca = saldoRemanejado + retirado - adicionado;

        if (diferenca >= 0) {
        	$('#diferenca_saldo').html('R$ ' + mascaraglobal('###.###.###.###,##', diferenca.toFixed(2)));
        } else {
			$('#diferenca_saldo').html('<span style="color:red">R$ -' + mascaraglobal('###.###.###.###,##', diferenca.toFixed(2)) + '</span>');
        }
	}

	function detalheSubacao(sbaid) {
		var url ='planacomorc.php?modulo=principal/subacoes/detalhesubacao&acao=A&sbaid=' + sbaid;
        $.post(url,function(html) {
            $('#modal-confirm .modal-body p').html(html);
            $('.modal-dialog').css('width','90%');
            $('#modal-confirm .modal-title').html('Dados da Suba��o');
            $('#modal-confirm .btn-primary').remove();
            $('#modal-confirm .btn-default').html('Fechar');
            $('.modal-dialog').show();
            $('#modal-confirm').modal();
        });
	}

	function remanejar(){
		//inicializando variaveis
        var boVerificaOrigem = false;
        var boVerificaDestino = false;

        //varrendo valores na .class Origem
        $(".rem_sba_v").each(function(elemento, valor) {
        	if ($(valor).val() != '') {
            	//atribuindo "true" caso algum campo da classe origem esteja preenchida.
                boVerificaOrigem = true;
                return;
			}
		});

		//varrendo se algum campo foi preenchido na .class Destino
        $(".rem_adc_sba_v").each(function(elemento, valor) {
        	if ($(valor).val() != '') {
            	//atribuindo "true" caso algum campo da classe Destino esteja preenchida.
                boVerificaDestino = true;
                return;
			}
		});

        //validando com alert, se algum campo ORIGEM DOS RECURSOS ou DESTINO DOS RECURSOS est� preenchido.
        if (!(boVerificaOrigem || boVerificaDestino)) {
			alert("� obrigat�rio preencher um valor no campo Origem ou Destino dos Recursos.");
            return false;
		}

        if ('' == jQuery('#dscalteracao').val() && 'S' == jQuery('#tipotransacao').val()) {
        	alert('O campo "Detalhamento da altera��o" � obrigat�rio e n�o pode ser deixado em branco.');
            jQuery('#dscalteracao').select().focus();
            return;
        }

        if ('' == jQuery('#dscjustificativa').val() && 'S' == jQuery('#tipotransacao').val()) {
            alert('O campo "Justificativa da altera��o" � obrigat�rio e n�o pode ser deixado em branco.');
            jQuery('#dscjustificativa').select().focus();
            return;
        }

        if ('' == jQuery('#dscexecucao').val() && 'E' == jQuery('#tipotransacao').val()) {
			alert('O campo "Detalhamento da execu��o" � obrigat�rio e n�o pode ser deixado em branco.');
            jQuery('#dscexecucao').select().focus();
            return;
        }

        // -- Saldo subtra�do
        var rmpsaldosubtraido = jQuery('#saldo_remanejado_total').text();
        rmpsaldosubtraido = rmpsaldosubtraido.replace(/\./g, '').replace(',', '.').replace('R$ ', '');
        jQuery('#rmpsaldosubtraido').val(rmpsaldosubtraido);
        // -- Saldo adicionado
        var rmpsaldoadicionado = jQuery('#saldo_remanejado_total_adicionado').text();
        rmpsaldoadicionado = rmpsaldoadicionado.replace(/\./g, '').replace(',', '.').replace('R$ ', '');
        jQuery('#rmpsaldoadicionado').val(rmpsaldoadicionado);

		var diferenca_saldo = jQuery('#diferenca_saldo').text();
        diferenca_saldo = parseFloat(diferenca_saldo.replace(/\./g, '').replace(',', '.').replace('R$ ', ''));
        if (diferenca_saldo < 0) {
        	alert('A diferen�a de saldo ap�s o remanejamento n�o pode ser negativa.');
            return;
		}

        jQuery('#dscalteracao').removeAttr('disabled');
        jQuery('#dscjustificativa').removeAttr('disabled');

        // -- Enviar o formul�rio
        jQuery('#remanejar').submit();
	}

</script>
<style>
    .linkSubacao{font-weight:bold;color:#00529b;cursor:pointer}
    #menubackmenu1{z-index:20}
    .listagem thead:first-child td{vertical-align:middle;text-align:center}
    .tabela-listagem{border-left: 0 !important;border-right: 0!important;}
	.marcado {background-color: #C1FFC1 !important;}
	.remover {display: none;}
</style>
    <div id="dialog" title="Dados da Suba��o">

    </div>

<!-- ORIGEM -->
<div class="col-md-12">
	<ol class="breadcrumb">
		<li><a href="/planacomorc/planacomorc.php?modulo=inicio&acao=C"><?=$_SESSION['sisdsc']; ?></a></li>
		<li class="">Suba��es <?=$_SESSION['exercicio'];?></li>
		<li class="active">Remanejamento de valor de Suba��o por PTRES</li>
	</ol>
</div>
<div class="col-md-12">

	<form class="form-horizontal" id="remanejar" name="remanejar" method="POST">
	    <input type="hidden" name="requisicao" value="remanejamentoDeSubacao" />

	    <input type="hidden" name="rmsidorigem" value="<?php echo $_GET['rmsid']; ?>" />
	    <input type="hidden" name="tipotransacao" id="tipotransacao" value="<?php echo($tipoTransacao); ?>" />
	    <input type="hidden" name="ptrid" value="<?php echo $_REQUEST['ptrid']; ?>" />
	    <input type="hidden" name="funcprogramatica" value="<?php echo $funcProgramatica; ?>" />
	    <input type="hidden" name="ptrdotacao" value="<?php echo $dadosPtres['ptrdotacao']; ?>" />
	    <input type="hidden" name="ptrempenhado" value="<?php echo $dadosPtres['empenhado']; ?>" />
	    <input type="hidden" name="ptrsaldo" value="<?php echo $dadosPtres['saldo']; ?>" />
	    <input type="hidden" name="detalhadosubacao" value="<?php echo $dadosPtres['detalhadosubacao']; ?>" />
	    <input type="hidden" name="po" value="<?php echo $dadosPtres['plocod']; ?>" />
	    <input type="hidden" name="podsc" value="<?php echo $dadosPtres['plotitulo']; ?>" />

	    <input type="hidden" name="rmpsaldosubtraido" id="rmpsaldosubtraido" />
	    <input type="hidden" name="rmpsaldoadicionado" id="rmpsaldoadicionado" />

	    <section class="panel panel-default">
	    	<section class="panel-heading"><strong>INFORMA��ES</strong></section>
	    	<section class="panel-body">
	    		<div class="form-group">
	    			<label class="control-label col-md-2">Unidade:</label>
	    			<section class="col-md-10">
	    				<p class="form-control-static"><?php echo("{$dadosPtres['unicod']} - {$dadosPtres['unidsc']}"); ?></p>
	    			</section>
				</div>
				<div class="form-group">
					<label class="control-label col-md-2">Funcional Program�tica:</label>
					<div class="col-md-10">
						<p class="form-control-static"><?php echo($funcProgramatica); ?></p>
					</div>
				</div>
	            <div class="form-group">
	            	<label class="control-label col-md-2">Data da solicita��o:</label>
	            	<div class="col-md-10">
	            		<p class="form-control-static"><?php echo date('d/m/Y'); ?></p>
	            	</div>
	            </div>
	            <div class="form-group">
	            	<label class="control-label col-md-2">Detalhamento:</label>
	            	<div class="col-md-10">
	            		<div id="tabs">
	            			<ul>
		                        <?php if (isset($_REQUEST['rmsid'])){ ?>
	                            <li><a href="#previamente">No Momento da Solicita��o</a></li>
	                        	<?php } ?>
	                        	<li><a href="#atualmente">Atualmente</a></li>
	                    	</ul>
		                    <?php if (isset($_REQUEST['rmsid'])){ ?>
		                        <div id="previamente">
		                            <div class="cols">
		                                <div class="col1">
		                                    <table class="table table-striped">
		                                        <tr>
		                                            <td class="text-right">PTRES:</td>
		                                            <td><b style="font-size:14px"><?php echo $dadosTransacao['ptres']; ?></b></td>
		                                        </tr>
		                                        <tr>
		                                            <td class="text-right">PO:</td>
		                                            <td><?php echo "{$dadosTransacao['po']} - {$dadosTransacao['podsc']}"; ?></td>
		                                        </tr>
		                                        <tr>
		                                            <td class="text-right">Dota��o do PTRES:</td>
		                                            <td><b><?php echo number_format2($dadosTransacao['ptrdotacao']); ?></b></td>
		                                        </tr>
		                                        <tr>
		                                            <td class="text-right">Empenhado no PTRES:</td>
		                                            <td><b><?php echo number_format2($dadosTransacao['ptrempenhado']); ?></b></td>
		                                        </tr>
		                                        <tr>
		                                            <td class="text-right">Detalhado em Suba��o:</td>
		                                            <td><b><?php echo number_format2($dadosTransacao['detalhadosubacao']); ?></b></td>
		                                        </tr>
		                                        <tr>
		                                            <td class="text-right">N�o Detalhado em Suba��o:</td>
		                                            <td style="color:<?php echo(($dadosTransacao['ptrsaldo'] >= 0) ? 'black' : 'red'); ?>">
		                                                <b><span><?php echo number_format2($dadosTransacao['ptrsaldo']); ?></span></b>
		                                            </td>
		                                        </tr>
		                                    </table>
		                                </div>
		                            </div>
		                        </div>
		                    <?php } ?>
		                    <div id="atualmente">
		                        <div class="cols">
		                            <div class="col1">
		                                <table class="table table-striped">
		                                    <tr>
		                                        <td class="text-right">PTRES:</td>
		                                        <td><b style="font-size:14px"><?php echo $dadosPtres['ptres']; ?></b></td>
		                                    </tr>
		                                    <?php
		                                        //atribuindo texto "0000" quando ptres.plocod = '' e  texto "valor n�o informado" quando plo.plotitulo = '';
		                                        $dadosPtres['plocod']    = (!empty($dadosPtres['plocod'])) ? ($dadosPtres['plocod']) : '0000';
		                                        $dadosPtres['plotitulo'] = (!empty($dadosPtres['plotitulo'])) ? $dadosPtres['plotitulo'] : 'Valor n�o detalhado';
		                                    ?>
		                                    <tr>
		                                        <td class="text-right">PO:</td>
		                                        <td><?php echo "{$dadosPtres['plocod']} - {$dadosPtres['plotitulo']}"; ?></td>
		                                    </tr>
		                                    <tr>
		                                        <td class="text-right">Dota��o do PTRES:</td>
		                                        <td><b><?php echo number_format2($dadosPtres['ptrdotacao']); ?></b></td>
		                                    </tr>
		                                    <tr>
		                                        <td class="text-right">Empenhado no PTRES:</td>
		                                        <td><b><?php echo number_format2($dadosPtres['empenhado']); ?></b></td>
		                                    </tr>
		                                    <tr>
		                                        <td class="text-right">Detalhado em Suba��o:</td>
		                                        <td><b><?php echo number_format2($dadosPtres['detalhadosubacao']); ?></b></td>
		                                    </tr>
		                                    <tr>
		                                        <td class="text-right">N�o Detalhado em Suba��o:</td>
		                                        <td style="color:<?php echo(($dadosPtres['saldo'] >= 0) ? 'black' : 'red'); ?>">
		                                            <b><span id="saldo_nao_orcado"><?php echo number_format2($dadosPtres['saldo']); ?></span></b>
		                                        </td>
		                                    </tr>
		                                </table>
		                            </div>
		                        </div>
		                    </div>
		                </div>
	            	</div>
	            </div>
	    	</section>
	    </section>
	    <br>
	    <section class="panel panel-success">
	    	<section class="panel-heading">
	    		<strong>ORIGEM DOS RECURSOS</strong> <span class="glyphicon glyphicon-info-sign" title="As informa��es na tabela abaixo dizerm respeito apenas aos dados da SUBA��O vinculada ao PTRES." class="initialism"></span>
	        </section>
	        <table class="table table-striped">
				<tr>
					<td>
						<strong>SUBA��ES VINCULADAS AO PTRES</strong>
					</td>
	            	<td  style="font-size: 14px; text-align: right; padding-right: 3em;">
	        	    	Saldo Subtra�do:
	    	        	<div style="float: right; color: seagreen; font-weight: bold; " id="saldo_remanejado_total">
		            	R$ 0,00
	                    </div>
					</td>
	            </tr>
	            <tr>
	            	<td>
	                    <div class="btn-group" data-toggle="buttons">
	                    	<label class="btn btn-default" id="remanejamento_1">
	                    		<input type="checkbox"  />
	                    		Mostrar apenas preenchidos
	                    	</label>
	                   	</div>
					</td>
				</tr>
			</table>
			<section class="col-md-12" id="primeira-tabela">
	        	<label for="textFind" style="margin-bottom: 3px">Pesquisa r�pida: </label>
				<input class="normal form-control" type="text" id="textFind"></input>
				<br>
				<?php
			    $cabecalho = array(
					"C�digo",
				    "Suba��o",
				    "Or�amento total da Suba��o (R$)"
			    );
				if (!(isset($_REQUEST['rmsid']) && 'E' == $dadosTransacao['tipotransacao'])) {
					$cabecalho[] = "Or�amento da Suba��o neste PTRES (R$)";
				    $cabecalho[] = "Detalhado em PI nesta Suba��o e neste PTRES (R$)";
				    $cabecalho[] = "Empenhado nesta Suba��o e neste PTRES(R$)";
				    $cabecalho[] = "N�o Detalhado em PI nesta Suba��o e neste PTRES (R$)";
				    $cabecalho[] = "Recursos a SUBTRAIR com o remanejamento (R$)";
				    $cabecalho[] = "Or�amento da suba��o ap�s o remanejamento (R$)";
			    } else {
				    $cabecalho[] = "Or�amento da Suba��o neste PTRES (R$)";
				    $cabecalho[] = "Detalhado em PI nesta Suba��o e neste PTRES(R$)";
				    $cabecalho[] = "Empenhado nesta Suba��o e neste PTRES(R$)";
				    $cabecalho[] = "N�o Detalhado em PI nesta Suba��o e neste PTRES (R$)";
				    $cabecalho[] = "Recursos SUBTRA�DOS com o remanejamento (R$)";
				    $cabecalho[] = "Or�amento da suba��o ap�s o remanejamento (R$)";
			    }

				$select = "
					SELECT
						foo.sbaid,
						foo.codigo AS sbacod,
	        			foo.sbatitulo,
	        			COALESCE(foo.dotacao, 0.00) AS dotacao,
	        			COALESCE((SELECT sadvalor FROM monitora.pi_subacaodotacao WHERE sbaid = foo.sbaid AND ptrid = {$ptrid} ),0.00)  as dotacao_no_ptres,
	       				COALESCE((SELECT COALESCE(SUM(dtl.valorpi),0.00) FROM monitora.v_pi_detalhepiptres dtl WHERE prgano = '{$_SESSION['exercicio']}' AND ptrid = {$ptrid} AND sbaid = foo.sbaid), '0.00') AS detalhado_pi_no_ptres,
	       				COALESCE((SELECT SUM(total) FROM siafi.pliptrempenho WHERE exercicio = '{$_SESSION['exercicio']}' AND SUBSTR(plicod, 2,4) = foo.codigo AND ptres = '{$dadosPtres['ptres']}'), '0.00') AS empenhado_no_ptres,
	       				COALESCE((SELECT sadvalor FROM monitora.pi_subacaodotacao WHERE sbaid = foo.sbaid AND ptrid = {$ptrid} ) -
	       				(SELECT  COALESCE(SUM(dtl.valorpi),0.00) FROM monitora.v_pi_detalhepiptres dtl WHERE prgano = '{$_SESSION['exercicio']}' AND ptrid = {$ptrid} AND sbaid = foo.sbaid), '0.00') AS nao_detalhado_pi_no_ptres,
							'<input type=\"text\" class=\"rem_sba_v normal form-control\" onkeyup=\"this.value=mascaraglobal(''###.###.###.###,##'', this.value); calculaSaldo(this);\" '
	           				|| 'id=\"rem_sba_' || foo.sbaid || '\" size=\"15\" name=\"rem_sba[' || foo.sbaid || ']\" />' AS remanejar,
						COALESCE(((SELECT COALESCE(sadvalor, 0.00) FROM monitora.pi_subacaodotacao WHERE sbaid = foo.sbaid AND ptrid = {$ptrid} ) -
	       					(SELECT COALESCE(SUM(dtl.valorpi), 0.00) AS valorpi FROM monitora.v_pi_detalhepiptres dtl WHERE prgano = '{$_SESSION['exercicio']}' AND ptrid = {$ptrid} AND sbaid = foo.sbaid) ), '0.00') AS novo_saldo
				";
				$groupby = array('foo.codigo','foo.sbatitulo','foo.dotacao','foo.sbaid','foo.empenhado','foo.detalhado_pi','foo.codigo');
				$orderby = array('1');
	            $where = "AND ptr.ptrid = {$ptrid}";
	            $sql = retornaConsultaSubacao_bootstrap(array('SELECT' => $select,'groupby' => $groupby,'orderby' => $orderby,'where' => $where));

	            $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
	            $listagem->setCabecalho($cabecalho);
	            $listagem->setFormOff();
	            $listagem->addAcao('view', 'detalheSubacao');
	            $listagem->addCallbackDeCampo(array('dotacao','dotacao_no_ptres','detalhado_pi_no_ptres','empenhado_no_ptres','nao_detalhado_pi_no_ptres','novo_saldo'), 'mascaraMoeda');
	            $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
	            $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA,array('dotacao','dotacao_no_ptres','detalhado_pi_no_ptres','empenhado_no_ptres','nao_detalhado_pi_no_ptres','novo_saldo'));
	            $listagem->setQuery($sql);
	            $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
	            ?>
	    	</section>
	    </section>

	    <br>
	    <!--DESTINO-->
	    <section class="panel panel-primary">
	    	<section class="panel-heading">
	    		<strong>DESTINO DOS RECURSOS</strong>
	    		<span class="glyphicon glyphicon-info-sign" title="As informa��es na tabela abaixo dizem respeito a SUBA��O como um todo, independente do PTRES selecionado." class="initialism"></span>
	        </section>
	        <table class="table table-striped" cellSpacing="1" cellPadding="3">
				<tr>
					<td><strong>SUBA��ES <?php echo $_SESSION['exercicio']; ?></strong></td>

	            	<td style="font-size: 14px; text-align: right; padding-right: 3em;">
	                	Saldo Adicionado:
	                	<div style="float:right;color:#00529b;font-weight:bold" id="saldo_remanejado_total_adicionado">
	                    	R$ 0,00
	                    </div>
					</td>
				</tr>
				<tr>
					<td colspan="2">
						<div class="btn-group" data-toggle="buttons">
	                    	<label class="btn btn-default" id="remanejamento_2">
								<input type="checkbox"  />
				            	Mostrar apenas preenchidos
							</label>
						</div>
					</td>
				</tr>
			</table>
			<section class="col-md-12" id="segunda-tabela">
		        <label for="textFind" style="margin-bottom: 3px">Pesquisa r�pida: </label>
				<input class="normal form-control" type="text" id="textFind2"></input>
				<br>
				<div id="div_subacoesAdicionar" style="height:400px;overflow:auto">
		        <?php
		        $cabecalho = array("C�digo","Suba��o","Or�amento total da Suba��o (R$)");
				if (!(isset($_REQUEST['rmsid']) && 'E' == $dadosTransacao['tipotransacao'])) {
		        	$cabecalho[] = "Or�amento da Suba��o neste PTRES (R$)";
		        	$cabecalho[] = "Detalhado em PI nesta Suba��o e neste PTRES (R$)";
		            $cabecalho[] = "Empenhado nesta Suba��o e neste PTRES(R$)";
		            $cabecalho[] = "Recursos a ADICIONAR com o remanejamento (R$)";
		            $cabecalho[] = "Or�amento da suba��o ap�s o remanejamento (R$)";
				} else {
		            $cabecalho[] = "Or�amento da Suba��o neste PTRES (R$)";
		            $cabecalho[] = "Detalhado em PI nesta Suba��o e neste PTRES(R$)";
		            $cabecalho[] = "Empenhado nesta Suba��o e neste PTRES(R$)";
		            $cabecalho[] = "Recursos ADICIONADOS com o remanejamento (R$)";
		            $cabecalho[] = "Or�amento da suba��o ap�s o remanejamento (R$)";
				}

		        $select = "
					SELECT
						foo.sbaid,
						foo.codigo AS sbacod,
		        		foo.sbatitulo,
		        		COALESCE(foo.dotacao, 0.00) AS dotacao,
		        		COALESCE((SELECT sadvalor FROM monitora.pi_subacaodotacao WHERE sbaid = foo.sbaid AND ptrid = {$ptrid} ),0.00)  as dotacao_no_ptres,
		       			COALESCE((SELECT SUM(dtl.valorpi) AS valorpi FROM monitora.v_pi_detalhepiptres dtl WHERE prgano = '{$_SESSION['exercicio']}' AND ptrid = {$ptrid} AND sbaid = foo.sbaid), '0.00') AS detalhado_pi_no_ptres,
						COALESCE((SELECT SUM(total) FROM siafi.pliptrempenho WHERE exercicio = '{$_SESSION['exercicio']}' AND SUBSTR(plicod, 2,4) = foo.codigo AND ptres = '{$dadosPtres['ptres']}'), '0.00') AS empenhado_no_ptres,
						foo.sbaid AS orcar,
		        		COALESCE(foo.dotacao, 0.00) AS novo_saldo
				";
		        $groupby = array(
					'foo.codigo',
			        'foo.sbatitulo',
			        'foo.dotacao',
			        'foo.sbaid',
			        'foo.empenhado',
			        'foo.detalhado_pi',
			        'foo.codigo'
		        );
function orcaAParada($orcar){
	$html = "<input type=\"text\" class=\"rem_adc_sba_v form-control\" id=\"rem_adc_sba_{$orcar}\" size=\"15\" name=\"rem_adc_sba[{$orcar}]\"'
	|| 'onkeyup=\"this.value=mascaraglobal(''###.###.###.###,##'', this.value); calculaSaldoAdic(this);\" /> ";
	return $html;
}
				$orderby = array('1');
		        $where = "AND ptr.ptrid = {$ptrid}";
				$sql = retornaConsultaSubacao_bootstrap(array('SELECT' => $select,'groupby' => $groupby,'orderby' => $orderby));
				$listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
				$listagem->setQuery($sql);
				$listagem->addAcao('view', 'detalheSubacao');
				$listagem->addCallbackDeCampo('orcar', 'orcaAParada');
				$listagem->setCabecalho($cabecalho);
				$listagem->setFormOff();
				$listagem->addCallbackDeCampo(array('dotacao','dotacao_no_ptres','detalhado_pi_no_ptres','empenhado_no_ptres','novo_saldo'), 'mascaraMoeda');
				$listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA,array('dotacao','dotacao_no_ptres','detalhado_pi_no_ptres','empenhado_no_ptres','novo_saldo'));
				$listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);

		        ?>
		        </div>
			</section>

	        <section class="panel-body">
	        	<section class="form-group">
	        		<label class="control-label col-md-3" for="dsc">Detalhamento da altera��o (500 caracteres):</label>
	        		<section class="col-md-9">
	        			<?php
						$dscalteracao = $dadosTransacao['dscalteracao'];
	                    //echo campo_textarea('dscalteracao', 'S', $pulaSolicitacao || isset($_REQUEST['rmsid']) ? 'N' : 'S', '', '85', '5', 500);
	                    inputTextArea('dscalteracao', $valor, 'dsc', 500,array('habil'=>$pulaSolicitacao || isset($_REQUEST['rmsid']) ? 'N' : 'S','obrig'=>'S'));
	                    ?>
	        		</section>
	        	</section>
	        	<section class="form-group">
	        		<label class="control-label col-md-3" for="justificativa">Justificativa da altera��o (500 caracteres):</label>
	        		<section class="col-md-9">
	        			<?php
	        			$dscjustificativa = $dadosTransacao['dscjustificativa'];
	        			//echo campo_textarea('dscjustificativa', 'S', $pulaSolicitacao || isset($_REQUEST['rmsid']) ? 'N' : 'S', '', '85', '5', 500);
	        			inputTextArea('dscjustificativa', $valor, 'justificativa', 500, array('habil' => $pulaSolicitacao || isset($_REQUEST['rmsid']) ? 'N' : 'S','obrig'=>'S'));
	        			?>
	        		</section>
	        	</section>
	            <?php
				if ($pulaSolicitacao || isset($_REQUEST['rmsid'])){
	            ?>
	            <section class="form-group">
	            	<label class="control-label col-md-3" for="detalhamento">Detalhamento da execu��o (500 caracteres):</label>
	            	<section class="col-md-9">
	            	<?php
	                	$dscexecucao = $dadosTransacao['dscexecucao'];
	                    //echo campo_textarea('dscexecucao', 'S', 'S', '', '85', '5', 500);
	                    inputTextArea('dscexecucao', $valor, 'detalhamento', 500, array('habil'=>'S','obrig'=>'S'));
	                 ?>
	            	</section>
	            </section>
	            <?php
				}
	            ?>
				<section class="form-group" >
					<label class="control-label col-md-3 col-md-offset-7">Saldo do PTRES ap�s remanejamento: </label>
		            <p class="form-control-static" style="font-weight:bold<?php echo($dadosPtres['saldo'] > 0 ? 'color:black;' : ';color:red'); ?>" id="diferenca_saldo"><?php echo number_format2($dadosPtres['saldo']); ?></p>
	            </section>
	            <section class="col-md-12">
	            	<input type="button" class="btn btn-primary" id="enviar" value="<?php echo ($pulaSolicitacao || isset($_REQUEST['rmsid']) ? 'Confirmar Remanejamento' : 'Solicitar Remanejamento'); ?>" />
	                <input type="button" class="btn btn-warning" value="Voltar" onclick="history.go(-1);" />
	            </section>
	        </section>
	    </section>
	</form>
</div>