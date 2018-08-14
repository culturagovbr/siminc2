<?php

define('SISID_EMENDAS', 251);
define('SISID_PLANEJAMENTO', 157);

# ESFERA DA AวรO
define( 'ESFERA_FEDERAL_BRASIL', 1 );
define( 'ESFERA_ESTADUAL_DISTRITO_FEDERAL', 2 );
define( 'ESFERA_MUNICIPAL', 3 );
define( 'ESFERA_EXTERIOR', 4 );

# Tipo de Documento do Planejamento
define("WF_TPDID_PLANEJAMENTO_PI", 265);
define("WF_TPDID_PLANEJAMENTO_PI_FNC", 266);
define("ESD_PI_CADASTRAMENTO", 1769);
define("ESD_FNC_PI_CADASTRAMENTO", 1774);
define("ESD_PI_AGUARDANDO_APROVACAO", 1770);
define("AED_ENVIAR_APROVACAO", 4298);
define("AED_APRESENTAR_PROJETO_FNC", 4307);
define("AED_SELECIONAR_PROJETO_EMENDAS_FNC", 4326);
define("AED_ENVIAR_DELIBERACAO_FNC", 4308);
define("AED_SELECIONAR_PROJETO_FNC", 4310);

# Tipo de Documento do Emendas
define("WF_TPDID_BENEFICIARIO", 267);
define("ESD_BENEFICIARIO_CADASTRAMENTO", 1781);
define("ESD_BENEFICIARIO_PREENCHIMENTO_UNIDADE", 1782);
define("ESD_BENEFICIARIO_CORRECAO", 1783);
define("ESD_BENEFICIARIO_PI_GERADO", 1784);
define("ESD_BENEFICIARIO_PI_APROVADO", 1785);

# Perfis de Usuแrio
define('PFL_SUPERUSUARIO', 1501);
define("PFL_ADMINISTRADOR", 1507);
define("PFL_ASPAR", 1508);
define("PFL_SUBUNIDADE", 1506);
define("PFL_PLANEJAMENTO", 956);

//Erros retornados pelo webservice SICONV
define('ERR_USUARIO_INCORRETO', 'web.service.erro.nao.foi.encontrado.um.usuario.no.sistema.com.as.credenciais.informadas');
define('ERR_QUANTIDADE_VEZES_EXCEDIDA', 'br.gov.mp.siconv.ws.exception.WSException: "Deve haver um intervalo de 5 segundos entre duas chamadas seguidas ao Web Service"');