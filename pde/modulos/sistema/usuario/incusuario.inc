<?php

	/**
	 * Sistema Integrado de Monitoramento do Ministério da Educação
	 * Setor responsvel: SPO/MEC
	 * Desenvolvedor: Desenvolvedores Simec
	 * Analistas: Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>, Alexandre Soares Diniz
	 * Programadores: Renê de Lima Barbosa <renedelima@gmail.com>, Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>
	 * Módulo: Monitoramento e Avaliação
	 * Finalidade: Controla as especificidades do cadastro de usuários do Sistema de Monitoramento e Avaliação.
	 * Data de criação:
	 * Última modificação: 05/09/2006
	 */

	$pflcod = $_REQUEST['pflcod'];

?>
<tr>
	<td align="right" class="SubTituloDireita">Perfil desejado:</td>
	<td>
		<?php include APPRAIZ .'seguranca/modulos/sistema/usuario/incperfilusuario.inc'; ?>
	</td>
</tr>
<?php
	$sql = sprintf(
		"select distinct * from monitora.tiporesponsabilidade tr inner join  monitora.tprperfil tp on tr.tprcod = tp.tprcod where tp.pflcod = %d and tr.tprsnvisivelperfil = 't'",
		$pflcod
	);
	$responsabilidades = $db->carregar( $sql );
?>
<?php if ( $responsabilidades ): ?>
	<?php foreach( $responsabilidades as $responsabilidade ): ?>
		<tr>
			<td align="right" class="subtitulodireita"><?= $responsabilidade['tprdsc'] ?>:</td>
			<td>
				<input type="hidden" name="perfil" value="<?= $responsabilidade['tprsigla'] ?>"/>
				<?php
					switch ( $responsabilidade['tprsigla'] ) {
						case 'A':
							$sql = sprintf(
								"select distinct
									pap.acacod,
									a.prgcod||'.'||pap.acacod||'.'||pap.unicod||' - '||a.acadsc as dscacao
								from monitora.progacaoproposto as pap
									inner join monitora.acao as a on pap.acacod = a.acacod and pap.prgid = a.prgid and pap.unicod = a.unicod
								where
									a.acasnrap = false and
									pap.usucpf =  '%s'",
								$cpf
							);
							break;
						case 'P':
							$sql = sprintf(
								"select
									pap.prgid,
									p.prgcod||' - '||p.prgdsc as dscprg
								from monitora.progacaoproposto as pap
									left join planejamento.programa as p on pap.prgid = p.prgid
								where
									pap.acacod = '' and
									pap.usucpf = '%s'",
								$cpf
							);
							break;
						case 'U':
						case 'E':
						case 'N':
						default:
							break;
					}
					if ( $sql ) {
						$opcoes = $db->carregar( $sql );
					}
				?>
				<?php if ( !is_array( $opcoes ) OR $_SESSION['sisid'] == 4 ): ?>
					<select multiple size="5" name="proposto[<?= $responsabilidade['tprsigla'] ?>][]" id="proposto_<?= $responsabilidade['tprsigla'] ?>" style="width:500px;" onclick="especificar_perfil( '<?= $responsabilidade['tprsigla'] ?>' )" class="CampoEstilo">
						<option value="">Clique Aqui para Selecionar</option>
					</select>
					<?=obrigatorio();?>
					<br/>
				<?php else: ?>
					<select multiple size="5" name="proposto[<?= $responsabilidade['tprsigla'] ?>][]" id="proposto_<?= $responsabilidade['tprsigla'] ?>" style="width:500px;" onclick="" class="CampoEstilo">
						<?php
							foreach ( $opcoes as $opcao ) {
								extract( $opcao );
								print '<option value='. $prgid .'>'. $dscprg .'</option>';
							}
						?>
					</select>
					<?=obrigatorio();?>
				<?php endif; ?>
			</td>
		</tr>
	<?php endforeach; ?>
<?php endif; ?>
<script type="text/javascript">

	function selecionar_perfil(){
		document.formulario.formulario.value = "";
		document.formulario.submit();
	}

	/**
	 * Exibe opções relacionadas ao perfil. (ação, programa e projeto especial )
	 */
	function especificar_perfil( tipo ){
		document.getElementById( "proposto_"+ tipo ).selectedIndex = -1;
		switch ( tipo ) {
			case 'A':
				especifica_perfil = window.open( "../monitora/geral/seleciona_aca.php?campo=proposto_"+ tipo, "especifica_perfil", "menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=500,height=480" );
				break;
			case 'P':
				especifica_perfil = window.open( "../monitora/geral/seleciona_prg.php?campo=proposto_"+ tipo, "especifica_perfil", "menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=500,height=480" );
				break;
			case 'U':
				especifica_perfil = window.open( "../elabrev/geral/seleciona_unid.php?campo=proposto_"+ tipo, "especifica_perfil", "menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=500,height=480" );
				break;
			case 'E':
			case 'N':
			defaul:
				break;
		}
	}

	/**
	 * Recebe os itens selecionados pelo usuário na lista exibida pelo método especificar_perfil()
	 */
	function retorna( objeto, tipo ) {
		campo = document.getElementById( "proposto_"+ tipo );
		tamanho = campo.options.length;
		if ( campo.options[0].value == '' ) {
			tamanho--;
		}
		if ( especifica_perfil.document.formulario.prgid[objeto].checked == true ){
			campo.options[tamanho] = new Option( especifica_perfil.document.formulario.prgdsc[objeto].value, especifica_perfil.document.formulario.prgid[objeto].value, false, false );
		} else {
			for( var i=0; i <= campo.length-1; i++ ) {
				if ( campo.options[i].value == especifica_perfil.document.formulario.prgid[objeto].value ) {
					campo.options[i] = null;
				}
			}
			if ( campo.options[0] ) {
			} else {
				campo.options[0] = new Option( 'Clique Aqui para Selecionar', '', false, false );
			}
		}
	}

</script>