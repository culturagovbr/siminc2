<?php
/**
 * Classe de implementa��o das regras de neg�cio do Per�odo de refer�ncia.
 *
 * $id$
 */

/**
 *
 */
class Proporc_Service_Periodoreferencia extends Spo_Service_Abstract {

    public function salvar()
    {
        $this->validaDados();
        $perReferencia = new Proporc_Model_Periodoreferencia();
        $perReferencia->popularDadosObjeto($this->getDados());
        $perReferencia->prsano = $_SESSION['exercicio'];
        if ($perReferencia->salvar()) {
            $perReferencia->commit();
            $this->flashMessage->addMensagem('Sua solicita��o foi realizada com sucesso.');
        } else {
            $this->flashMessage->addMensagem(
                'N�o foi poss�vel completar sua solicita��o.',
                Simec_Helper_FlashMessage::ERRO
            );
            if (!IS_PRODUCAO) {
                $this->flashMessage->addMensagem(
                pg_last_error(),
                Simec_Helper_FlashMessage::ERRO
                );
            }
        }
    }

    public function duplicar()
    {

    }

    public function visualizarConfig()
    {
        $mdl = new Proporc_Model_Grupodespesa();
        $where = array(sprintf('t1.prfid = %d', $this->prfid));
        $gdpobservacao = <<<DML
CASE WHEN char_length(t1.gdpobservacao) > 50
       THEN substring(t1.gdpobservacao, 0, 50) || '...'
     ELSE t1.gdpobservacao END AS gdpobservacao
DML;
        $sql = $mdl->recuperarTodos("gdpid, gdpnome, {$gdpobservacao}", $where, null, array('query' => true));

        $list = new Simec_Listagem();
        $list->setCabecalho(array('Nome do grupo', 'Descri��o'))
            ->addAcao('plus', 'visualizarConfig')
            ->setQuery($sql)
            ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    }
}