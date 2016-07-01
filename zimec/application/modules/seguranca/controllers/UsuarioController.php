<?php

include_once APPLICATION_PATH . '/../library/Simec/legacy/Listagem.php';

class Seguranca_UsuarioController extends Simec_Controller_Action
{
    public function indexAction()
    {
        $model = new Model_Par_Demandatipo();
        $this->view->rowSet = $model->fetchAll();
        
        $dados = $this->_getAllParams();

        $sql = "
        select 'Orion' as descricao, '132' as codigo, 1 as teste
        union
        select 'Maykel' as descricao, '32' as codigo, 1 as teste
        union
        select 'Maykel' as descricao, '12' as codigo, 1 as teste
        ";

        $sql = $model->getQuery($dados);
        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO, Simec_Listagem::RETORNO_BUFFERIZADO);
        $listagem->setQuery((string)$sql)
                 ->setCabecalho(array('ID', 'Descri��o'))
//                 ->addAcao('view', 'orion')
//                 ->addAcao('send', 'maykel')
//                 ->esconderColunas('codigo')
//                 ->setOrdenacao(true)
//                 ->setFiltro(true)
                 ;

        $this->view->listagem = $listagem->render(
            Simec_Listagem::SEM_REGISTROS_MENSAGEM,
            Simec_Listagem::RETORNO_BUFFERIZADO
        );
        //$listagem->setQuery($sql)->render();



    }

    public function formularioAction()
    {
        $model = new Model_Seguranca_Usuario();
        $modelUnidade = new Model_Public_Unidade();

        $this->view->row = $model->getRow('');
        $this->view->unidades = $modelUnidade->getUnidadesAtivas();

        $modelUf = new Model_Territorios_Estado();
        $rowSetEstados = $modelUf->fetchAll(null, 'estuf');

        $estados = array();
        foreach ($rowSetEstados as $row) {
            $estados[$row->estuf] = $row->estuf . ' - ' . $row->estdescricao;
        }
        $this->view->estados = $estados;

        $this->view->camposComErro = Simec_Util::getSession('form_validation_error');
        Simec_Util::clear('form_validation_error');
    }

    public function gravarAction()
    {
        $dados = $this->_getAllParams();
        $model = new Model_Seguranca_Usuario();

        try {
            Zend_Db_Table::getDefaultAdapter()->beginTransaction();

            $filter = new Zend_Filter_Digits();
            $dados['usucpf'] = $filter->filter('007.049.631-59');
            $id = $model->gravar($dados);

            Zend_Db_Table::getDefaultAdapter()->commit();

            // -- Redirecionando
            $this->_redirect('seguranca/usuario/demanda-tipo/', 'Opera��o realizada com sucesso.', 'success');

        } catch (Simec_Db_Exception $e) {
            Zend_Db_Table::getDefaultAdapter()->rollBack();
            $this->_redirect('seguranca/usuario/formulario/usucpf/' . $id, $e->getDetalhe(), 'error');
        }
    }

    public function excluirAction()
    {
        $dmtid = $this->_getParam('dmtid');
        $model = new Model_Par_Demandatipo();

        try {
            Zend_Db_Table::getDefaultAdapter()->beginTransaction();

            $dmtid = $model->excluir(array('dmtid = ?' => $dmtid));

            Zend_Db_Table::getDefaultAdapter()->commit();

            $this->_redirect('par/demanda-tipo/');

        } catch (Ev_Exception $e) {
            Zend_Db_Table::getDefaultAdapter()->rollBack();
        }
    }
}