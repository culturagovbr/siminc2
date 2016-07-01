<?php
// Diretorio do SAP
define('APPSAP', APPRAIZ . 'sap/');

// N�mero de casas decimais que ser�o usadas no valores do patrimonio
define('APPSAP_CASASDECIMAIS', 8);

#Situa��es do Bem Estaticos
define('SBM_ENTRADA', 1);
define('SBM_NORMAL', 2);
define('SBM_MOVIMENTACAO', 3);
define('SBM_USO', 4);
define('SBM_DEVOLUCAO', 5);
define('SBM_BAIXA', 6);
define('SBM_BAIXADO', 7);
define('SBM_NAO_LOCALIZADO', 8);

#Tipos Movimenta��o Estaticos
define('TMV_DEVOLUCAO', 4);
define('TMV_INTERNA', 1);

#Tipos Entrada/Saida Estaticos
define('TES_SAIDA_DOACAO', 15);
define('TES_ENTRADA_DOACAO', 10);

#Estados de Conserva��o Estaticos
define('ECO_INSERVIVEL', 3);

#C�digo de menu/abas
define( 'ABA_ADICIONA_EMPENHO',  7569);
define( 'ABA_EDITAR_EMPENHO',  7570);
define( 'ABA_ADICIONA_BENS_BAIXA',  7599);
define( 'ABA_EDITA_BENS_BAIXA',  7600);
define('ABA_EDITAR_CATALOGO',7559);
define('ABA_EDITAR_SITUACAO_BEM',7437);
define('ABA_EDITAR_FORNECEDOR',7619);
define('ABA_EDITAR_ESTADO_CONSERVACAO',7438);
define('ABA_EDITAR_TIPO_DOCUMENTO',7534);
define('ABA_EDITAR_TIPO_MOVIMENTACAO',7537);
define('ABA_EDITAR_TIPO_ENTRADASAIDA',7542);
define('ABA_EDITAR_MATERIAL',7572);
define('ABA_EDITAR_MOTIVO_ESTADO',7718);

define('ABA_EDITAR_BEM',7583);
define('ABA_EDITAR_BEM_ETAPA2',8202);
define('ABA_ADICIONAR_BEM_MATERIAL',7589);
define('ABA_ADICIONAR_BEM_RGP',7602);

define('ABA_EDITAR_BEM_ABERTO',8197);

define('ABA_INCLUIR_BEM',7582);

define('ABA_ADICIONA_BENS_BAIXA_ABERTO',8201);

define('ABA_EDITAR_MOVIMENTACAO_GERAL',8190);
define('ABA_EDITAR_MOVIMENTACAO_LOCALIDADE',8191);
define('ABA_EDITAR_MOVIMENTACAO_SIAPE',8192);


define('ABA_INCLUSAO_EDICAO_ITEM_PROCESSO',8214);
define('ABA_INCLUSAO_EDICAO_TOMBAMENTO',8216);
define('ABA_EDICAO_ITEM_PROCESSO',8217);
define('ABA_TOMBAMENTO_ITEM',8218);
define('ABA_EDICAO_TOMBAMENTO',8219);
define('ABA_INCLUIR_BEM_REDIRECIONA',8220);
define('ABA_EDITAR_BEM_REDIRECIONA',8222);

