<?php

class Public_Controller_ManutencaoItem
{
	public function salvar($dados)
	{
		$url = '?modulo=apoio/item-manutencao-form&acao=A&maiid=';
		try {
			$mManutencaoItem = new Planacomorc_Model_ManutencaoItem($dados['maiid']);
			$mManutencaoItem->popularDadosObjeto($dados);
			$mManutencaoItem->prsano = $mManutencaoItem->prsano ? $mManutencaoItem->prsano : $_SESSION['exercicio'];

			$mManutencaoItem->salvar(null, null, ['maiid', 'maidescricao', 'eqdid']);
			$mManutencaoItem->commit();
			simec_redirecionar($url . $mManutencaoItem->maiid, 'success');
		} catch (Exception $e){
			$mManutencaoItem->rollback();
			simec_redirecionar($url, 'error');
		}
	}

	public function excluir($maiid)
	{
		$url = '?modulo=apoio/item-manutencao&acao=A';

		try {
			$mManutencaoItem = new Planacomorc_Model_ManutencaoItem($maiid);
			$mManutencaoItem->maistatus = 'I';
            $mManutencaoItem->salvar(null, null, ['maiid', 'maidescricao', 'eqdid']);
			$mManutencaoItem->commit();
			simec_redirecionar($url, 'success');
		} catch (Exception $e){
			$mManutencaoItem->rollback();
			simec_redirecionar($url, 'error');
		}
	}

	public function montarFormulario($maiid = null, $masid = null)
	{
		global $simec;

		$perfis = pegaPerfilGeral();
		/**
		 * @todo colcoar a constante correta do acompanhamento para atualizar a regra.
		 */
		$podeEditar = !in_array(PFL_SUBUNIDADE, $perfis)? TRUE: FALSE;
		$simec->setPodeEditar($podeEditar);

		$listaSubItens = (new Planacomorc_Model_ManutencaoSubItem())->recuperarTodos('*', ['maiid = ' . (int)$maiid]);
		if(!$podeEditar && $listaSubItens){
			$masid = $masid? $masid: $listaSubItens[0]['masid'];
		}

		$mSubItem = new Planacomorc_Model_ManutencaoSubItem($masid);

		if(!$mSubItem->maiid){ $mSubItem->maiid = $maiid; }
		include_once APPRAIZ. 'planacomorc/modulos/apoio/subitem-manutencao-form.inc';
	}

	public function montarListagem($maiid)
	{
		$perfis = pegaPerfilGeral();
		/**
		 * @todo colcoar a constante correta do acompanhamento para atualizar a regra.
		 */
		$podeEditar = !in_array(PFL_SUBUNIDADE, $perfis)? TRUE: FALSE;

		$lista = (new Planacomorc_Model_ManutencaoSubItem)->recuperarTodos('*', ['maiid = ' . (int)$maiid, "masstatus = 'A'"]);
		include_once APPRAIZ . 'planacomorc/modulos/apoio/subitem-manutencao.inc';
	}
}
