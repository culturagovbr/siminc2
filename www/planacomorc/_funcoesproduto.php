<?
function exibirProduto($dados) {
	global $db;
	if($dados['id_produto_spo']) {

		$sql = "SELECT id_produto_spo, codigo, nome, descricao FROM planacomorc.produto_spo WHERE id_produto_spo='".$dados['id_produto_spo']."'";
		$produtosubacao = $db->pegaLinha($sql);

		echo '<input type="hidden" name="requisicao" value="atualizarProduto">';
		echo '<input type="hidden" name="id_produto_spo" value="'.$produtosubacao['id_produto_spo'].'">';

		global $id_subacao;

        //adicionando ano de exercicio, para carregar combo popup de suba��es correspondentes.
		$id_subacao = $db->carregar("SELECT s.id_subacao as codigo, s.codigo || ' - ' || s.descricao as descricao
	                                 FROM planacomorc.subacao s
	                                   INNER JOIN planacomorc.produto_spo_subacao p ON p.id_subacao = s.id_subacao
	                                 WHERE p.id_produto_spo='".$dados['id_produto_spo']."'");

	} else {
		echo '<input type="hidden" name="requisicao" value="inserirProduto">';
	}
    //dados carregados na Modal com dados da suba��o.
    ?>
    <form class="form-horizontal" method="post" name="formproduto" id="formproduto">
    	<section class="form-group">
    		<label class="col-md-2 control-label" for="id_exercicio">Ano Exerc�cio:</label>
    		<section class="col-md-10">
    			<?php //echo campo_texto('id_exercicio', "N", "N", "Ano Exerc�cio", 4, 4, "", "", '', '', 0, 'id="id_exercicio"', '',  );?>
    			<?php inputTexto('id_exercicio', $_SESSION['exercicio'], 'id_exercicio', 4,false,array('habil' => 'N', 'obrig' => 'N'));?>
    		</section>
    	</section>
    	
    	<section class="form-group">
    		<label class="col-md-2 control-label" for="codigo">C�digo:</label>
    		<section class="col-md-10">
    			<?php //echo campo_texto('codigo', "N", "S", "Nome", 12, 10, "", "", '', '', 0, 'id="codigo"', '', $produtosubacao['codigo'] );?>
    			<?php inputTexto('codigo', $produtosubacao['codigo'], 'codigo', 10,false,array('obrig'=>'N'))?>
    		</section>
    	</section>
    	
    	<section class="form-group">
    		<label class="col-md-2 control-label" for="nome">Nome:</label>
    		<section class="col-md-10">
    			<?php //echo campo_texto('nome', "S", "S", "Nome", 62, 100, "", "", '', '', 0, 'id="nome"', '',  );?>
    			<?php inputTexto('nome', $produtosubacao['nome'], 'nome', 100,false,array('habil'=>'S','obrig'=>'S'));?>
    		</section>
    	</section>
    	
    	<section class="form-group">
    		<label class="col-md-2 control-label" for="descricao">Descri��o:</label>
    		<section class="col-md-10">
    			<?php //echo campo_textarea( 'descricao', 'S', 'S', '', '70', '4', '500', '', '', '', '', '', $produtosubacao['descricao']);?>
    			<?php inputTextArea('descricao', $produtosubacao['descricao'], 'descricao', 500,array('obrig'=>'S'));?>
    		</section>
    	</section>
    	
    	<section class="form-group">
    		<label class="col-md-2 control-label" for="subacoes">Suba��es:</label>
    		<section class="col-md-10">
    			<?php 
    			$sql = "
					SELECT 
						id_subacao as codigo, 
						codigo || ' - ' || descricao as descricao
                  	FROM planacomorc.subacao s
                  	WHERE id_exercicio='".$_SESSION['exercicio']."'
                    	AND EXISTS (SELECT 1
                        	FROM planacomorc.snapshot_dotacao_subacao sds
                            WHERE s.id_subacao = sds.id_subacao)
				";    			
    			//combo_popup( "id_subacao", $sql, "A��es", "400x600", 0, array(), "", "S", false, false, 5, 400 );
    			inputCombo('id_subacao', $sql, $valor, 'subacoes',array('multiple'=>'multiple'));   			    			
    			?>
    		</section>
    	</section>    	
	</form>	
	<?php 

}

function gravarMetodologia($dados) {
	global $db;
	if ($dados['texto_metodologia']) {

		$sql = "DELETE FROM planacomorc.metodologia_conversao WHERE id_acao='".$dados['id_acao']."' AND id_subacao IN('".implode("','",array_keys($dados['texto_metodologia']))."')";
		$db->executar($sql);
        $db->commit();

		foreach($dados['texto_metodologia'] as $id_subacao => $text) {
			$sql = "INSERT INTO planacomorc.metodologia_conversao(
            		id_periodo_referencia, id_acao, texto_metodologia, id_subacao)
    				VALUES ('".$dados['id_periodo_referencia']."', '".$dados['id_acao']."', '".$text."', '".$id_subacao."');";

			$db->executar($sql);
            $db->commit();
		}

        $al = array();
        $al['message'] = utf8_encode('Metodologia salva com sucesso!');
	} else {
        $al = array();
        $al['message'] = utf8_encode('Falha ao tentar salvar metodologia!');
    }

    echo simec_json_encode($al);
}

function exibirMetodologia($dados) {
	global $db;

    if (isset($dados['cod'])) {
        $dados = $db->pegaLinha("select * from planacomorc.metodologia_conversao where id_metodologia_conversao = {$dados['cod']}");
    }

    echo '<form class="form-horizontal" name="formmetodologia" id="formmetodologia" action="" method="POST" role="form">';
    echo '<div class="row">';

	echo '<input type="hidden" name="requisicao" value="gravarMetodologia">';

	echo '<div class="form-group">';
	echo '<label class="control-label col-md-2" for="id_periodo_referencia">Per�odo de refer�ncia</label>';
	echo '<div class="col-md-10">';
	$sql = "SELECT id_periodo_referencia AS codigo, titulo || ' : ' || inicio_validade || ' � ' || fim_validade AS descricao FROM planacomorc.periodo_referencia WHERE id_exercicio = '".$_SESSION['exercicio']."'";
    echo inputCombo('id_periodo_referencia', $sql, $dados['id_periodo_referencia'], 'id_periodo_referencia');
	echo '</div>';
	echo '</div>';

	echo '<div class="form-group">';
    echo '<label class="control-label col-md-2" for="id_acao">A��o</label>';
    echo '<div class="col-md-10">';
	$sql = "SELECT id_acao AS codigo, codigo AS descricao FROM planacomorc.acao ORDER BY descricao";
    echo inputCombo('id_acao', $sql, $dados['id_acao'], 'id_acao', array('acao' => 'selecionarAcaoMetologia'));
    echo '</div>';
	echo '</div>';

	echo '<div class="form-group">';
	echo '<div class="col-md-12" id="div_subacao">';
	if($dados['id_acao']) {
		carregarSubacao(array('id_acao' => $dados['id_acao'],'id_periodo_referencia' => $dados['id_periodo_referencia']));
	}
	echo '</div>';
	echo '</div>';
	echo '</form>';
}

function carregarSubacao($dados) {
	global $db;

	$sql = "SELECT distinct
                   s.codigo || ' - ' || s.sigla as titulo,
                  (array_to_string(
                    array(
                        select p.nome from planacomorc.produto_spo p
                        inner join planacomorc.produto_spo_subacao ps on ps.id_produto_spo = p.id_produto_spo
                        where ps.id_subacao = s.id_subacao), ','
                        )
                    ) as produtos,
                  '<input type=\"text\" style=\"text-align:;\" class=\"field-subacao\" id=\"'||s.id_subacao||'\" name=\"texto_metodologia['||s.id_subacao||']\" size=\"40\" maxlength=\"200\"
                  value=\"'||COALESCE((
                        SELECT texto_metodologia
                        FROM planacomorc.metodologia_conversao
                        WHERE id_acao = apr.id_acao
                        AND id_subacao = s.id_subacao
                        AND id_periodo_referencia=".$dados['id_periodo_referencia']."
                  ),'')||'\" onmouseover=\"MouseOver(this);\"
                  onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"
                  id=\"texto_metodologia_'||s.id_subacao||'\" title=\"Nome\" class=\"obrigatorio normal\">' as input
            FROM planacomorc.dotacao_subacao d
                INNER JOIN planacomorc.ptres p ON p.id_ptres = d.id_ptres
                INNER JOIN planacomorc.acao_programatica apr ON p.id_acao_programatica = apr.id_acao_programatica
                INNER JOIN planacomorc.subacao s ON s.id_subacao = d.id_subacao
			WHERE apr.id_acao='".$dados['id_acao']."'
			AND s.id_exercicio='".$_SESSION['exercicio']."' ";
    //ver($sql, d);

    $colunms = array(
        'Suba��o',
        'Produtos',
        'Metodologia de convers�o do(s) produto(s) da suba��o no produto da a��o'
    );

    require(APPRAIZ . 'includes/library/simec/Listagem.php');

    $list = new Simec_Listagem();
    $list->setCabecalho($colunms)->setQuery($sql)->setFormOff();

    $list->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
}


function atualizarProduto($dados) {
    global $db;

    $sql = "SELECT count(codigo) FROM planacomorc.produto_spo WHERE codigo = '{$dados['codigo']}' AND id_produto_spo != '{$dados['id_produto_spo']}'";
    $contador = $db->pegaUm($sql);
    if($contador >= 1){
        $al = array(
            'alert'    => 'N�o podem existir dois ou mais produtos com o mesmo c�digo',
            'location' => 'planacomorc.php?modulo=principal/produto/listaproduto&acao=A'
        );
        alertlocation($al);
    }

    $sql = "UPDATE planacomorc.produto_spo SET codigo='".$dados['codigo']."', id_exercicio =".$dados['id_exercicio'].", nome='".$dados['nome']."', descricao='".$dados['descricao']."' WHERE id_produto_spo='".$dados['id_produto_spo']."'";
    $db->executar($sql);

    $db->executar("DELETE FROM planacomorc.produto_spo_subacao WHERE id_produto_spo='".$dados['id_produto_spo']."'");

    if($dados['id_subacao']) {
        foreach($dados['id_subacao'] as $id_subacao) {
            if ($id_subacao) {
                $sql = "
                    INSERT INTO planacomorc.produto_spo_subacao(id_produto_spo, id_subacao)
                        VALUES ('".$dados['id_produto_spo']."', '".$id_subacao."');";
                $db->executar($sql);
            }
        }
    }

    $db->commit();

    $al = array(
        'alert'    => 'Produto atualizado com sucesso',
        'location' => 'planacomorc.php?modulo=principal/produto/listaproduto&acao=A'
    );
    alertlocation($al);
}

function inserirProduto($dados) {
    global $db;
    $sql = "SELECT 1 FROM planacomorc.produto_spo WHERE codigo = '{$dados['codigo']}'";
    if($db->pegaUm($sql)){		
        $al = array(
            'alert'    => 'C�digo do Produto inserido j� est� cadastrado no sistema',
            'location' => 'planacomorc.php?modulo=principal/produto/listaproduto&acao=A'
        );
        alertlocation($al);
    }

    $sql = "
        INSERT INTO planacomorc.produto_spo(
            codigo,id_exercicio, nome, descricao, st_ativo)
        VALUES ('".$dados['codigo']."',".$dados['id_exercicio'].", '".$dados['nome']."', '".$dados['descricao']."', 'A') 
        RETURNING id_produto_spo;
    ";

    $id_produto_spo = $db->pegaUm($sql);

    if($dados['id_subacao']) {
        foreach($dados['id_subacao'] as $id_subacao) {
            if ($id_subacao) {
                $sql = "INSERT INTO planacomorc.produto_spo_subacao(id_produto_spo, id_subacao)
                    VALUES ('".$id_produto_spo."', '".$id_subacao."');";
                $db->executar($sql);
            }
        }
    }
    $db->commit();

    $al = array(
        'alert'    => 'Produto inserido com sucesso',
        'location' => 'planacomorc.php?modulo=principal/produto/listaproduto&acao=A'
    );
    alertlocation($al);
}

function excluirProduto($dados) {
	global $db;
	$sql = "UPDATE planacomorc.produto_spo SET st_ativo='I' WHERE id_produto_spo='".$dados['id_produto_spo']."'";
	$db->executar($sql);
	$db->commit();

	$al = array(
        'alert'    => 'Produto exclu�do com sucesso',
		'location' => 'planacomorc.php?modulo=principal/produto/listaproduto&acao=A'
    );
	alertlocation($al);
}

?>