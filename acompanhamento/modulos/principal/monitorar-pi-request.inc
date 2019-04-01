<?php

    $cAcompanhamento = new Acompanhamento_Controller_Acompanhamento();
    switch ($_REQUEST['req']) {
        case 'salvar':
            $cAcompanhamento->salvar($_REQUEST, $urlModulo);
        die;
        case 'excluir':
            $cAcompanhamento->excluir($_REQUEST['acoid']);
        die;
        case 'ver-historico':
            $cAcompanhamento->recuperarHistorico($_REQUEST['acoid']);
        die;
        case 'carregarMedida':
            $mClassificacao = new Acompanhamento_Model_Classificacao($_REQUEST['claid']);
            $retorno['medida'] = $mClassificacao->clamedida;
            $listaMedidas = $db->carregar((new Acompanhamento_Model_Medida())->recuperarSqlCombo(null, ['claid = ' . (int)$_REQUEST['claid']]));
            $listaMedidasUtf8 = array();
            if($listaMedidas){
                foreach ($listaMedidas as $medida){
                    $listaMedidasUtf8[$medida['codigo']] = utf8_encode($medida['descricao']);
                }
            }
            $retorno['select'] = $simec->select('medidas[]', 'Medidas a serem adotadas', null, $listaMedidasUtf8);
            echo json_encode($retorno);
        die;
        case 'uploadAnexoAcompanhamento':
            $_FILES['file']['name'] = utf8_decode($_FILES['file']['name']);
            $_REQUEST['arqdescricao'] = utf8_decode($_REQUEST['arqdescricao']);
            $file = new FilesSimec();
            $file->setEsquema('acompanhamento');
            if($file->setUpload($_REQUEST['arqdescricao'], '', false)){
                echo simec_json_encode(array(
                    'arqid' => $file->getIdArquivo(),
                    'arqnome' => $_FILES['file']['name'],
                    'arqdescricao' => $_REQUEST['arqdescricao']
                ));
            } else {
                echo simec_json_encode(array(
                    'error' => 1,
                    'errorMensage' => 'Não foi possível enviar o arquivo!'
                ));
            }
        die;
        case 'historico-pi':
            include_once APPRAIZ. "planacomorc/modulos/principal/unidade/historico_pi_modal.inc";
        die;
        case 'historico-pi-usuario':
            include_once APPRAIZ. "planacomorc/modulos/principal/unidade/historico_pi_usuario_modal.inc";
        die;
    }

