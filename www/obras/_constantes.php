<?php

// �rg�os do m�dulo de obras
define('ORGAO_SESU', 1);
define('ORGAO_SETEC', 2);
define('ORGAO_FNDE', 3);
define('ORGAO_ADM', 4);
define('ORGAO_REHUF', 5);
define('ORGAO_MILITAR', 6);


// Constantes dos ID's dos m�dulos que usam o m�dulo de obras
define('ID_OBRAS', 15);
define('ID_PARINDIGENA', 32);

// Constantes dos tipos de forma de repasse de recursos
define('TFR_CONVENIO', 2);
define('TFR_DESCENTRALIZACAO', 3);
define('TFR_REC_PROPRIO', 4);

// Constantes das fun��es das entidades
define('ID_UNIVERSIDADE', 12);
define('ID_HOSPITAL', 16);
define('ID_ESCOLAS_TECNICAS', 11);
define('ID_ESCOLAS_AGROTECNICAS', 14);
define('ID_ADM', 34);
define('ID_INSTMILITAR', 118);

// Constantes situa��o da obra
define('EM_CONSTRUCAO', 1);
define('PARALIZADA', 2);
define('FINALIZADA', 3);
define('EM_ELABORACAO_DE_PROJETOS', 4);
define('EM_LICITACAO', 5);
define('NAO_CONCLUIDA', 6);

// ids das unidades
define("ID_UNIDADEIMPLANTADORA",44);
define("ID_CAMPUS",18);
define("ID_UNED",17);
define("ID_SUPERVISIONADA",35);
define("ID_REITORIA",75);
define("ID_UNIESTADUAL",42);

// ids situa��es de supervis�o
define( "OBRSITSUPREPOSITORIO", 1 );
define( "OBRSITSUPDISTRIBUIDA", 2 );
define( "OBRSITSUPROTAEMPRESA", 4 );
define( "OBRSITSUPVISTORIA", 	5 );

// execu��o or�ament�ria
define( "OBRAS_TIPO_EXECORC_OBRAS", 1 );
define( "OBRAS_TIPO_EXECORC_EQUIPAMENTO", 2 );

// constantes workflow (POR OBRA)
if($_SESSION['baselogin'] == 'simec_desenvolvimento'){
	define( "OBR_TIPO_DOCUMENTO_OBRA", 34 );
}else{
	define( "OBR_TIPO_DOCUMENTO_OBRA", 23 );
}

define( "OBRASUPERVISAO", 		   227 );
define( "OBRAANALISEMEC", 	   	   228 );
define( "OBRADEVOLVIDOSUPERVISAO", 229 );
define( "OBRAANALISEMECCORRECAO",  230 );
define( "OBRASUPERVISAOAPROVADA",  231 );
define( "OBRAVISTORIAAPROVADA",    234 );

// constantes workflow
define( "OBR_TIPO_DOCUMENTO", 18);
define( "OBRDISTRIBUIDO", 156 );
define( "OBRREDISTRIBUIDO", 209 );
define( "OBREMDEFINROTA", 157 );
define( "OBREMAVALIAMEC", 158 );
define( "OBREMAPROVAMEC", 159 );
define( "OBREMSUPERVISAO", 159 );
define( "OBREMAVALIASUPERVMEC", 171 );
define( "OBRAVALIAFINALSAA", 172 );
define( "OBRSUPFINALIZADA", 173 );
define( "OBRREAVSUPVISAO", 239 );
define( "OBRREAJSUPVISAOEMP", 280 );
define( "OBRENVREAVALSUPMEC", 174 );
define( "OBREMSUPERVISAOIND", 240 );
define( "OBRAAVALIACAOSUPERVISAO_MEC", 241 );
define( "OBRAAJUSTESUPERVISAO_EMPRESA", 242 );
define( "OBRAREAVALIACAOSUPERVISAO_MEC", 243 );
define( "OBRASUPERVISAOAPROVADAOBRA", 244 );
define( "OBRAREAJUSTESUPERVISAO_EMPRESA", 279 );
define( "GRUPOEMSUPERVISAO", 297 );
define( "GRUPOAGUARDANDOINICIOSUPERVISAO", 216 );
define( "GRUPOLIBERADOPARASUPERVISAO", 336 );
define( "GRUPOCONTRATONAORENOVADO", 316 );
define( "GRUPOCONTRATOCANCELADO"  , 317 );
define( "OBRACONTRATOCANCELADO"   , 320 );
define( "OBRACONTRATONAORENOVADO" , 321 );


/*
// constantes workflow (DESENV)
define( "OBR_TIPO_DOCUMENTO", 14);

define( "OBRDISTRIBUIDO", 147 );
define( "OBREMDEFINROTA", 148 );
define( "OBREMAVALIAMEC", 149 );
define( "OBREMAPROVAMEC", 150 );
*/


//constantes supervis�o empresas
define('OBRSITROTADEFINIDA', 4);

// Constantes de perfis do m�dulo
define('PERFIL_SUPERUSUARIO', 160);
define('PERFIL_SUPERVISORUNIDADE', 163);
define('PERFIL_GESTORUNIDADE', 164);
define('PERFIL_SUPERVISORMEC', 165);
define('PERFIL_ADMINISTRADOR', 166);
define('PERFIL_CONSULTAGERAL', 174);
define('PERFIL_CONSULTAESTADUAL', 177);
define('PERFIL_GESTORMEC', 162);
define('PERFIL_AUDITORINTERNO', 387);
define('PERFIL_SAA', 425);
define('PERFIL_EMPRESA', 426);
define('PERFIL_CONSULTATIPOENSINO', 230);
define('PERFIL_CONSULTAUNIDADE', 231);
define('PERFIL_CADASTRADOR_INSTITUCIONAL', 542);

// Constantes de perfis do m�dulo
define('ADM', 391281);
define('ADM_UNICOD', 26101);

// Tipo do Aditivo
define('ADITIVO_PRAZO',		  1);
define('ADITIVO_VALOR', 	  2);
define('ADITIVO_PRAZO_VALOR', 3);

// Tipos de Arquivo
define('TIPO_ARQUIVO_FOTO_VISTORIA', 23);

// workflow do grupo da obra
define('TPDID_GRUPO', 18);
define('ESDID_SUPERVISAO_FINALIZADA', 173);

// org�os relacionados as obras
define('ORGID_EDUCACAO_SUPERIOR'	, 1);
define('ORGID_EDUCACAO_PROFISSIONAL', 2);
define('ORGID_EDUCACAO_BASICA'		, 3);
define('ORGID_ADMINSTRATIVO'		, 4);
define('ORGID_HOSPITAIS'			, 5); 

?>