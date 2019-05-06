<?php
/**
 * Cadastramento de subações.
 * $Id: cadastro.inc 93305 2015-01-30 12:25:02Z lindalbertofilho $
 */

if (isset($_POST['evento']) && ('I' == $_POST['evento'] || 'U' == $_POST['evento'] || 'E' == $_POST['evento'])) {
    require('_funcoessubacoes.php');
    $sucesso = false;
    switch ($_POST['evento']) {
        case 'I':
        case 'U':
            $sucesso = salvarSubacao($_POST);
            break;
        case 'E':        	
            $sucesso = apagarSubacao($_POST['sbaid']);
            break;
    }

    if ($sucesso){ ?>
        <script type="text/javascript" language="javascript">
            alert('Sua requisição foi executada com sucesso.');
            location.href = 'planacomorc.php?modulo=principal/subacoes/listasubacoesmanter&acao=A';
        </script>
    <?php }else{ ?>
        <script type="text/javascript" language="javascript">
            alert('Não foi possível executar sua requisição.');
            location.href = 'planacomorc.php?modulo=principal/subacoes/listasubacoesmanter&acao=A';
        </script>
    <?php
    }
    exit();
}
include APPRAIZ . "includes/cabecalho.inc";
include_once APPRAIZ . "includes/funcoesspo_componentes.php";
if ($_GET['sbaid']) {
    $subacao = $db->carregar("
    	SELECT
    		sba.sbaid,
            sba.sbadsc,
            sba.sbastatus,
            sba.sbadata,
            sba.sbasigla,
            sba.sbatitulo,
            sba.sbacod,
            sba.usucpf,
            sba.sbasituacao,
            sba.sbaobras,
            sba.sbaplanotrabalho,
            sba.pieid,
            sba.pigid,
            sba.sbaano
		FROM
        	monitora.pi_subacao sba
        WHERE
        	sbaid =  {$_GET['sbaid']}");
    $subacao = $subacao[0];
    if (!$subacao) {
        echo "<script>
                alert('A Subação informada não existe!');
                history.back(-1);
            </script>";
        die;
    }
}

if ($subacao['sbaid']) {
    $sql = <<<DML
        SELECT 
            dtl.ptrid,
            dtl.ptres,
            TRIM(aca.prgcod || '.' || aca.acacod || '.' || aca.unicod || '.' || aca.loccod || ' - ' || aca.acatitulo) AS descricao,
            uni.unidsc,
            SUM(dtl.ptrdotacao) AS dotacaoinicial,
            COALESCE(SUM(dt.valor), 0.00) AS det_subacao,       		
            COALESCE(SUM(dtl.ptrdotacao) - COALESCE(SUM(dt.valor), 0.00), 0.00) AS nao_det_subacao,
            (SELECT SUM(pip.pipvalor) AS valorpi FROM monitora.pi_planointerno pli
                INNER JOIN monitora.pi_planointernoptres pip ON pli.pliid = pip.pliid
                WHERE pli.pliano = '{$_SESSION['exercicio']}'
                    AND pli.plistatus = 'A'
                    AND pli.plisituacao IN ('A','C','S','T')
                    AND pli.pliid = dtl.ptrid) AS det_pi,
            emp.total AS empenhado,       	
            (SUM(dtl.ptrdotacao) - COALESCE((emp.total), 0.00)) AS nao_empenhado,
            SUM(sd.sadvalor) AS valorcadastrado
        FROM monitora.acao aca
    	INNER JOIN monitora.ptres dtl ON aca.acaid = dtl.acaid
    	INNER JOIN monitora.pi_subacaodotacao sd ON dtl.ptrid = sd.ptrid
    	INNER JOIN public.unidade uni ON uni.unicod = dtl.unicod
    	LEFT JOIN (SELECT ptrid, SUM(sadvalor) AS valor FROM monitora.pi_subacaodotacao GROUP BY ptrid) dt ON dtl.ptrid = dt.ptrid
    	LEFT JOIN (SELECT sbaid,ptrid, SUM(dtl.valorpi) AS valorpi, dtl.prgano FROM monitora.v_pi_detalhepiptres dtl GROUP BY sbaid, dtl.ptrid, dtl.prgano) dt2 ON dtl.ptrid = dt2.ptrid AND dt2.sbaid = sd.sbaid AND dt2.prgano = aca.prgano
    	LEFT JOIN siafi.ptrempenho AS emp ON (emp.ptres = dtl.ptres AND emp.exercicio = '{$_SESSION['exercicio']}')
        WHERE aca.prgano='{$_SESSION['exercicio']}'
            AND aca.acasnrap = false
            AND sd.sbaid = {$subacao['sbaid']}
            AND dtl.ptrano='{$_SESSION['exercicio']}'    
            AND sd.sadvalor > 0
        GROUP BY 
            dtl.ptrid,
            dtl.ptres,
            descricao,
            uni.unidsc,
            emp.total
        ORDER BY 1
DML;
    $ptresacao = $db->carregar($sql);
    #ver($sql);
    
    $sql = <<<DML
		SELECT pr.codigo,
       		pr.nome,
       		pr.descricao,
       		p.id_produto_spo_subacao,
       		p.meta_fisica,
       		pr.id_produto_spo AS prdid
  		FROM monitora.pi_subacao sba
    	INNER JOIN planacomorc.produto_spo_subacao p ON p.id_subacao = sba.sbaid
    	INNER JOIN planacomorc.produto_spo pr ON p.id_produto_spo = pr.id_produto_spo
  		WHERE sba.sbaano = '{$_SESSION['exercicio']}'
    		AND sba.sbacod = '{$subacao['sbacod']}'
    		AND pr.st_ativo = 'A'
  		ORDER BY 1
DML;

    //$produtos = $db->carregar($sql);
}
?>
<!-- <script language="JavaScript" src="../includes/wz_tooltip.js"></script> -->
<script src="../includes/funcoes.js"></script>
<script src="/includes/funcoesspo.js"></script>
<link rel="stylesheet" href="/library/bootstrap-switch/stylesheets/bootstrap-switch.css">
<script src="/library/bootstrap-switch/js/bootstrap-switch.min.js"></script>

<div class="col-md-12">
    <ol class="breadcrumb">
        <li><a href="/planacomorc/planacomorc.php?modulo=inicio&acao=C"><?=$_SESSION['sisdsc']; ?></a></li>
        <li class="">Subações <?=$_SESSION['exercicio'];?></li>
        <li class="active">Cadastro de Subação</li>
    </ol>    
    <form class="form-horizontal well"  method="POST" name="formulario" id="formulario">
        <input type="hidden" name="evento" id="evento" value="<?php echo (($_REQUEST['sbaid']) ? "U" : "I"); ?>" />
    	<input type="hidden" name="acaoForm" id="acaoForm" value="1" />
    	<input type="hidden" name="sbaid" id="sbaid" value="<?php echo $subacao['sbaid']; ?>" />
    	<input type="hidden" name="sbacod" id="sbacod" value="<?php echo $subacao['sbacod']; ?>" />
    	
        <?php if ($_GET['apagar'] == 'true') { ?>               
        	<input type="button" class="btn btn-danger btn-block" name="btassociar4" value="APAGAR" onclick="removersba(<?php echo $subacao['sbaid']; ?>);">        	
        	<br style="clear:both;">
        <?php } ?>
        <section class="form-group">
        	<label class="control-label col-md-2">Exercício:</label>
        	<section class="col-md-10">
        		<p class="form-control-static"><?php echo $_SESSION['exercicio']; ?></p>
        	</section>
        </section>                               
        <section class="form-group">
        	<div class="col-md-2 text-right">        	
            	<label class="control-label" for="sbatitulo">Título:</label>
            </div>
            <div class="col-md-10">
            	<?php
            	$sbatitulo = trim($subacao['sbatitulo']);
            	inputTexto('sbatitulo', $valor, 'sbatitulo', 250,false);
                echo "<input type='hidden' name='sbatituloantigo' value='" . $sbatitulo . "'>";
                ?>
            </div>
        </section>
        <section class="form-group">
        	<div class="col-md-2 text-right">
            	<label class="control-label" for="sbasigla">Título Abreviado / Sigla:</label>
            </div>
            <div class="col-md-10"><?php
                $sbasigla = trim($subacao['sbasigla']);
                inputTexto('sbasigla', $valor, 'sbasigla', 20,false);
                ?>
            </div>
        </section>
        <section class="form-group">
        	<div class="col-md-2 text-right">
            	<label class="control-label" for="eqdid">Enquadramento da Despesa:</label>
            </div>
            <div class="col-md-10">
                <?php
                if ($subacao['sbaid']) {
                    $sql = "
                        SELECT ed.eqdid AS codigo,
                            ed.eqdcod ||' - '|| ed.eqddsc AS descricao
                        FROM planejamento.enquadramento_despesa ed
                        INNER JOIN monitora.pi_subacaoenquadramento sae ON ed.eqdid = sae.eqdid
                        WHERE sae.sbaid = {$subacao['sbaid']}
                            AND eqdano = '{$_SESSION['exercicio']}'                        
                    ";
                    $eqdid = $db->carregar($sql);
                    $new = array();
                    if($eqdid){
                        foreach ($eqdid as $key => $v){
                            $new[] = $v['codigo'];
                        }
                        $eqdid = $new;
                    }
                }

                $sql = "
                    SELECT eqdid AS codigo,
                        eqdcod ||' - '|| eqddsc AS descricao
                    FROM planejamento.enquadramento_despesa
                    WHERE eqdano = '{$_SESSION['exercicio']}'
                    ORDER BY eqdcod
                ";
                inputCombo('eqdid[]', $sql, $eqdid, 'eqdid',array('multiple'=>'multiple','titulo'=>'Selecione alguma opção'));
                ?>
            </div>
        </section>
        <section class="form-group">
        	<div class="col-md-2 text-right">
            	<label class="control-label" for="pieid">Executor Orçamentário e Financeiro:</label>
            </div>
            <div class="col-md-10">
                <?php
                if ($subacao['sbaid'] && $subacao['pieid']) {
                    $sql = "SELECT piecod ||' - '|| piedsc as descricao FROM  monitora.pi_executor WHERE pieid = {$subacao['pieid']}";
                    echo "<p class=\"form-control-static\">".$db->pegaUm($sql)."</p>";
                } else {
                    $sql = "
                        SELECT 
                            pix.pieid AS codigo,
                            pix.piecod ||' - '|| pix.piedsc AS descricao
                        FROM monitora.pi_executor pix
                        WHERE pix.pieano = '{$_SESSION['exercicio']}'
                        ORDER BY pix.piecod
                    ";
                    inputCombo('pieid', $sql, $valor, 'pieid',array('titulo' => 'Selecione alguma opção'));
                }
                ?>    	
            </div>
        </section>
        <section class="form-group">
        	<div class="col-md-2 text-right">
            	<label class="control-label" for="pigid">Gestor da Subação:</label>
            </div>
            <div class="col-md-10">
                <?php
                if ($subacao['sbaid'] && $subacao['pigid']) {
                    $sql = "
                        SELECT 
                            pigcod ||' - '|| pigdsc as descricao 
                        FROM monitora.pi_gestor 
                        WHERE pigid = {$subacao['pigid']}";
                    echo "<p class=\"form-control-static\">".$db->pegaUm($sql)."</p>";
                } else {
                    $sql = "
                        SELECT 
                            pig.pigid AS codigo,
                            pig.pigcod ||' - '|| pig.pigdsc AS descricao
                        FROM monitora.pi_gestor pig
                        WHERE pig.pigano = '{$_SESSION['exercicio']}'
                        ORDER BY pig.pigcod
                    ";
                    inputCombo('pigid', $sql, $valor, 'pigid',array('titulo' => 'Selecione alguma opção'));
                }
                ?>    	
            </div>
        </section>
        <?php if ($subacao['sbaid']){ ?>
        <section class="form-group">        
            <label class="control-label col-md-2">Código da subação:</label>
            <section class="col-md-10">
                <p class="form-control-static"><?php echo $subacao['sbacod']; ?></p>
            </section>
        </section>
        <?php } ?>
        <section class="form-group">
            <label class="control-label col-md-2">Permite Obras ?</label>
            <section class="col-md-10">
                <div class="make-switch switch-mini" data-on-label="S" data-off-label="N" data-off="danger">
                    <input type="checkbox" id="sbaobras" name="sbaobras"
                        value="T" <?php echo $subacao['sbaobras'] == 't' ? 'checked' : ''; ?> />
                </div>
            </section>
        </section>
        
        <section class="form-group">
            <label class="control-label col-md-2" for="ungcod">Unidades Orçamentárias / Gestoras:</label>
            <section class="col-md-10">
                <?php
                if ($subacao['sbaid']) {
                    $sql = "
                        SELECT 
                            ung.ungcod AS codigo,
                            ung.ungcod ||' - '|| ung.ungdsc AS descricao
                        FROM public.unidadegestora ung
                        INNER JOIN monitora.pi_subacaounidade sau ON ung.ungcod = sau.ungcod
                        WHERE sau.sbaid = {$subacao['sbaid']}
                    ";
                    $ungcod = $db->carregar($sql);
                    
                    if($ungcod){
                        $new = array();
                        foreach ($ungcod as $key => $v){
                            $new[] = $v['codigo'];
                        }
                        $ungcod = $new;
                    }
                }
                $obrigatorias = UNIDADES_OBRIGATORIAS;
                $sql = "
                    SELECT
                        ung.ungcod AS codigo,
                        ung.ungcod ||' - '|| ung.ungdsc AS descricao
                    FROM public.unidadegestora ung
                    INNER JOIN public.unidade uni ON (uni.unicod = ung.unicod)
                    WHERE uni.unicod in ({$obrigatorias})
                        AND uni.orgcod = '". CODIGO_ORGAO_SISTEMA. "'
                        AND uni.unistatus = 'A'
                    ORDER BY ung.ungdsc
                ";
                inputCombo('ungcod[]', $sql, $ungcod, 'ungcod',array('multiple'=>'multiple','titulo'=>'Selecione'));
                ?>
            </section>
        </section>
        <section class="panel panel-default">        
            <div class="panel-heading"><strong>Ações</strong></div>            
            <table class="table table-hover table-bordered table-responsive" id="orcamento" >
                <tr>
                    <td colspan="10" align="center">
                        <b>Detalhamento Orçamentário</b>
                    </td>
                </tr>
                <tr style="text-align:center">
                    <td rowspan="2" nowrap style="font-weight:bold">PTRES</td>
                    <td rowspan="2" style="font-weight:bold" nowrap>Ação</b></td>
                    <td rowspan="2" style="font-weight:bold" nowrap>Dotação (R$)</td>
                    <td colspan="2" style="font-weight:bold" nowrap>Subação (R$)</td>                        
                    <td colspan="2" style="font-weight:bold" nowrap>Empenho (R$)</td>
                    <td rowspan="2" style="font-weight:bold" nowrap>Valor previsto (R$)</td>
                </tr>
                <tr style="text-align:center">
                    <td style="font-weight:bold" nowrap>Detalhado</td>
                    <td style="font-weight:bold" nowrap>Não Detalhado</td>                        
                    <td style="font-weight:bold" nowrap>Empenhado</td>
                    <td style="font-weight:bold" nowrap>Não Empenhado</td>
                </tr>
                <?php
                if ($ptresacao[0]) {
                    $valortotal = $cor = 0;
                    foreach ($ptresacao as $acpl){
                        $detSubacao = mascaraMoeda($acpl['det_subacao']);
                        $naoDetSubacao = mascaraMoeda($acpl['nao_det_subacao']);
                        $empenhado = mascaraMoeda($acpl['empenhado']);
                        $naoEmpenhado = mascaraMoeda($acpl['nao_empenhado']);
                        $valCadastrado = mascaraMoeda($acpl['valorcadastrado'],false);
                ?>
                <tr style="height:30px;" id="ptres_<?php echo $acpl['ptrid']; ?>">
                    <td align="center"><?php echo $acpl['ptres']; ?></td>
                    <td align="left">
                        <?php echo $acpl['descricao']; ?>
                        <input type='hidden' name='ptrid[<?php echo $acpl['ptrid']; ?>]' value='<?php echo $acpl['ptrid']; ?>' />
                    </td>
                    <td align="right"><?php echo mascaraMoeda($acpl['dotacaoinicial']); ?></td>
                    <td align="right"><?php echo $detSubacao; ?></td>
                    <td align="right"><?php echo $naoDetSubacao; ?></td>                       
                    <td align="right"><?php echo $empenhado; ?></td>
                    <td align="right"><?php echo $naoEmpenhado; ?></td>                        
                    <?php /* não dar espaço entre este input e o td */ ?>
                    <td align="center">
                        <input type="text" name="valor[<?php echo $acpl['ptrid']; ?>]" class="form-control somar"
                            value="<?php echo $valCadastrado; ?>" 
                            onKeyUp="this.value = mascaraglobal('###.###.###.###,##', this.value); calculovalorPI()"
                            onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select()"
                            onmouseout="MouseOut(this);" style="text-align:right;width:20ex"
                            data-ptres="<?php echo $ptres['ptres']; ?>"
                            onblur="MouseBlur(this);verificaDisponivel(this, '<?= $acpl['ptrid'] ?>', '<?php echo $valCadastrado; ?>');" />
                    </td>
                </tr>
                <?php
                        $cor++;
                        $valortotal = $valortotal + $acpl['valorcadastrado'];
                    }
                }
                ?>
                <tr style="height:30px">
                    <td align="right" style="vertical-align: middle;" valign="top" colspan="7"><b>TOTAL :</b></td>
                    <td align="center" valign="top">                        	
                        <input type="text" name="valortotal" id="valortotal" class="form-control"
                            value="<?php echo number_format($valortotal, 2, ',', '.'); ?>"
                            onKeyUp="this.value = mascaraglobal('###.###.###.###,##', this.value);"
                            disabled class="disabled"  onmouseover="MouseOver(this);"
                            onfocus="MouseClick(this);this.select()"
                            onmouseout="MouseOut(this);" onblur="MouseBlur(this);"
                            style="text-align:right;width:20ex" />
                    </td>
                </tr>
                <tr>
                    <td align="right" colspan="8">
                        <input class="btn btn-info" type="button" onclick="abrir_lista();" id="btn_selecionar_acaptres" value="Selecionar Ação/PTRES">		
                    </td>
            </table>                
        </section>

        <section class="form-group">
            <div class="col-md-2 text-right">
                <label class="control-label" for="sbadsc">Descrição / Finalidade:</label>
            </div>
            <div class="col-md-10">
            <?php
            $sbadsc = $subacao['sbadsc'];
            inputTextArea('sbadsc', $valor, 'sbadsc', 2500,array('obrig'=>'S'));
            ?>
            </div>
        </section>
        <section class="form-group">            
            <section class="col-md-offset-2">
                <input type="button" class="btn btn-primary" name="btg" value="Gravar" onclick="submeter('<?= $_REQUEST['sbaid'] ?>');">
                <?php if ($_REQUEST['sbaid']) { ?>              
                    <input type="button" class="btn btn-success" name="btassociar2" value="Novo" onclick="validabotao('N');">
                <?php } ?>              
                <input type="button" class="btn btn-warning" name="btassociar3" value="Voltar" onclick="validabotao('V');">
            </section>            
        </section>
    </form>		
</div>
<script type="text/javascript">
    function submeter(pliid) {
        if($('#ungcod').val() == null || $('#ungcod').val() == ""){
            alert('Seleciona uma Unidade Orçamentária / Gestora');
            return false;
        }
    	validarFormulario(['sbatitulo','sbasigla','pieid','pigid','sbadsc'],'formulario','<?php echo (($_REQUEST['sbaid']) ? "U" : "I"); ?>');
    }

    function marcaPermiteObras() {
        var chPermiteObras = document.getElementById("checkPermiteObras");
        var hSbaObra = document.getElementById("sbaobras");

        if (chPermiteObras.checked)
            hSbaObra.value = "true";
        else
            hSbaObra.value = "false";
    }

    function removersba(sbaid) {
        var conf = confirm("Você realmente deseja excluir esta Subação?");
        if (conf) {
            document.getElementById('evento').value = 'E';
            document.getElementById('sbaid').value = sbaid;
            document.formulario.submit();
        }
    }

    function alterarsba(sbaid) {
        document.getElementById('evento').value = 'A';
        document.getElementById('sbaid').value = sbaid;
        document.formulario.submit();
    }

    /* Função para subustituir todos */
    function replaceAll(str, de, para) {
        var pos = str.indexOf(de);
        while (pos > -1) {
            str = str.replace(de, para);
            pos = str.indexOf(de);
        }
        return (str);
    }

    function validar()
    {
        var msg = "";
        if (document.formulario.sbatitulo.value == '') {
            msg += "O preenchimento do campo Título é obrigatório.\n";
        }
        if (document.formulario.sbasigla.value == '') {
            msg += "O preenchimento do campo Sigla é obrigatório.\n";
        }
        var eqdid = document.getElementById('eqdid');
        if (eqdid[0].value == '') {
            msg += 'Você deve selecionar, no mínimo, um Enquadramento da Despesa.\n';
        }
        var pieid = document.getElementById('pieid');
        if (pieid && ('' == pieid[pieid.selectedIndex].value)) {
            msg += 'Você deve selecionar um Executor Orçamentário e Financeiro.\n';
        }
        var pigid = document.getElementById('pigid');
        if (pigid && ('' == pigid[pigid.selectedIndex].value)) {
            msg += 'Você deve selecionar um Gestor da Subação.\n';
        }
        var ungcod = document.getElementById('ungcod');
        if ('' == ungcod[0].value) {
            msg += 'Você deve selecionar, no mínimo, uma Unidade Orçamentária / Gestora.\n';
        }

        // validando se existe ação selecionado/ valor
        var tabela = document.getElementById('orcamento');
        if (tabela.rows.length == 4) {
            msg += "Você deve selecionar, no mínimo, uma ação no Detalhamento Orçamentário.\n";
        } else {
            jQuery('.somar').each(function(){
                var valor = jQuery(this).val();
                if ('' === valor || '0' === valor || '0,00' === valor) {
                    msg += "Valor do PTRES: '" + jQuery(this).attr('data-ptres') + "' é obrigatório.\n";
                }
            });
        }
        var sbadsc = document.getElementById('sbadsc');
        if ('' == sbadsc.value) {
            msg += 'O preenchimento do campo Descrição / Finalidade é obrigatório.\n';
        }

        if (msg != "") {
            alert(msg);
            return false;
        } else {
            return true;
        }
    }

    function abrir_lista_produtos() {
    	var url ='planacomorc.php?modulo=principal/subacoes/listaProdutos&acao=A';
    	$.get(url,function(html) {
        	$('#modal-confirm .modal-body p').html(html);
         	$('.modal-dialog').css('width','70%'); 
         	$('#modal-confirm .modal-title').html('Listagem de Produtos');
         	$('#modal-confirm .btn-primary').remove();
         	$('#modal-confirm .btn-default').html('Fechar');
         	$('.modal-dialog').show();
         	$('#modal-confirm').modal();
     	});          
	}

    function Arredonda(valor, casas) {
        var novo = Math.round(valor * Math.pow(10, casas)) / Math.pow(10, casas);
        var c = novo.toString();
        if (c.indexOf('.') == -1) {
            alert(novo);
            return novo;
        } else {
            return novo.toFixed(casas);
        }
    }

    function calculovalorPI() {
        var tabela = document.getElementById('orcamento');
        var tot = 0;

        jQuery('.somar').each(function(){
            var valor = jQuery(this).val();

            if ('' == valor) {
                valor = '0';
            }

            valor = replaceAll(valor, '.', '');
            valor = replaceAll(valor, ',', '.');
            tot += parseFloat(valor);
        });

        var c = tot.toString();
        if (c.indexOf('.') == -1) {
            document.getElementById('valortotal').value = tot.toFixed(2);
        } else {
            document.getElementById('valortotal').value = Arredonda(tot, 2);
        }
        document.getElementById('valortotal').onkeyup();
    }

    function verificaDisponivel(campo, ptrid, vlold) {        
        var linha = document.getElementById('ptres_' + ptrid);        
        var celNaoDetSubacao = 4;
        console.log(ptrid);
        disp = $(document.getElementById('ptres_'+ptrid).cells[4]).text();
        valordisp = parseFloat(replaceAll(replaceAll(disp, ".", ""), ",", "."));
        valoratual = parseFloat(replaceAll(replaceAll(campo.value, ".", ""), ",", "."));       
        if (valoratual > (valordisp + parseFloat(replaceAll(replaceAll(vlold, ".", ""), ",", ".")))) {
            alert('Valor não pode ser maior que o "Não Detalhado da Subação".');
            campo.value = vlold;
            calculovalorPI();
        }       
    }
    function validabotao(bot) {
        form = document.formulario;
        if (bot == 'N') {
            window.location = '?modulo=principal/subacoes/cadastro&acao=A';
        }
        if (bot == 'V') {
            window.location = '?modulo=principal/subacoes/listasubacoesmanter&acao=A';
        }
    }
    function limitaTitulo() {
        tit = document.formulario.sbatitulo.value;
        tot_tit = tit.length;
        if (tot_tit > 200) {
            alert('O campo Título possui o limite de 25 caracteres.');
            document.formulario.btg.focus();
        }
    }
    function detfin(ptrid) {
        var sbaid = '<?php echo $subacao['sbaid']; ?>';
        if (sbaid) {
            janela = window.open('/monitora/monitora.php?modulo=principal/planotrabalhoUG/detalhafinanceiro_subacaoUG&acao=A&ptrid=' + ptrid + '&sbaid=' + sbaid, 'janela2', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=' + (screen.width - 420) + ',height=280');
            janela.focus();
        }
    }
    
    function abrir_lista() {
        $('#modal-ptres .modal-body').html('');
    	var url ='planacomorc.php?modulo=principal/ptres/modalptres&acao=A';
        $.post(url,function(html) {
            $('#modal-ptres .modal-body').html(html);
            $('#modal-ptres .modal-title').html('Seleção de PTRES');
            $('#modal-ptres .btn-primary').remove();
            $('#modal-ptres .btn-default').html('Fechar');
            $('.modal-dialog-ptres').show();
            $('#modal-ptres').modal();
            $('#modal-ptres .chosen-select').chosen();
            $('#modal-ptres .chosen-container').css('width', '100%');
        });           
    }
</script>
<!-- Início Modal PTRES -->
<div id="modal-ptres" class="modal fade">
    <div class="modal-dialog-ptres" style="width:90%;margin:0 auto;">
        <div class="modal-content">
            <div class="modal-header">
                <button class="close" aria-hidden="true" data-dismiss="modal" type="button">×</button>
                <h4 class="modal-title">Aviso!</h4>
            </div>
            <div class="modal-body">
                <p></p>
            </div>
            <div class="modal-footer">
                <button class="btn btn-default" data-dismiss="modal" type="button">Não</button>
                <button class="btn btn-primary" type="button">Sim</button>
            </div>
        </div>
    </div>
</div>
<!-- FIM Modal PTRES -->