<?php

    include_once APPRAIZ. "includes/classes/fileSimec.class.inc";

    $caminho_atual = $_SERVER['REQUEST_URI'];
    $esquema = $_REQUEST['esquema'];
    $opcaoDownload = $_REQUEST['download'];
    $arqid = $_REQUEST['arqid'];

    if($opcaoDownload == 'S') {
        $file = new FilesSimec();
        
        # Verifica se o arquivo dever� ser aberto a partir de outro m�dulo
        if($esquema){
            $file->setEsquema($esquema);
        }
        
        ob_clean();
        $arquivo = $file->getDownloadArquivo($arqid);
        exit();
    }

    include APPRAIZ. "includes/cabecalho.inc";
