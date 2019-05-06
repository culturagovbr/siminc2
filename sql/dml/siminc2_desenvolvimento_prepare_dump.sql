
-- APAGANDO DE EXECUÇÃO ORÇAMENTÁRIA DO WEBSERVICE DO SIOP
DELETE FROM wssof.ws_execucaoorcamentariadto;
DELETE FROM spo.siopexecucao;

VACUUM FULL VERBOSE wssof.ws_execucaoorcamentariadto;
VACUUM FULL VERBOSE spo.siopexecucao;

-- APAGANDO ARQUIVOS DE AUDITORIA DE USO DA BASE DE DADOS
DELETE FROM auditoria.auditoria;
VACUUM FULL VERBOSE auditoria.auditoria;

/*
 * Arquivo de Remo��o de dados das tabelas dom�nios do Sistema SIMINC2 - M�dulo Acompanhamento (Monitoramento).
 *
 * @since 2018/12/05
 * @author Douglas Santana <douglas.fontes@cultura.gov.br>
 */


UPDATE public.metapnc SET mpnstatus = 'I';
UPDATE monitora.pi_planointerno SET plistatus = 'I';
UPDATE public.objetivoppa SET oppstatus = 'I';
UPDATE acompanhamento.acompanhamento SET acostatus = 'I';
UPDATE acompanhamento.janela SET janstatus = 'I';

/*
 * Arquivo de Remo��o de dados das tabelas dom�nios do Sistema SIMINC2 - M�dulo Emendas Parlamentares.
 *
 * @since 2018/12/05
 * @author Douglas Santana <douglas.fontes@cultura.gov.br>
 */

UPDATE emendas.emenda SET emestatus = 'I';
UPDATE public.subunidadeorcamentaria SET suostatus = 'I';
UPDATE public.unidadeorcamentaria SET unostatus = 'I';
UPDATE emendas.beneficiario SET benstatus = 'I';
UPDATE emendas.beneficiariodetalhe SET bedstatus = 'I';
DELETE FROM emendas.usuario_siconv;
UPDATE emendas.proponente SET prostatus = 'I';
UPDATE emendas.autor SET autstatus = 'I';
UPDATE emendas.autorgrupo SET agrstatus = 'I';
UPDATE emendas.partido SET parstatus = 'I';
UPDATE emendas.impedimento SET impstatus = 'I';
UPDATE emendas.alteracaoorcamentaria SET alostatus = 'I';
UPDATE emendas.siconvsituacao SET sitstatus = 'I';
UPDATE emendas.proponentetipo SET prtstatus = 'I';
UPDATE emendas.autortipo SET atpstatus = 'I';

/*
VACUUM FULL VERBOSE emendas.usuario_siconv;
*/

/*
 * Arquivo de Remo��o de dados das tabelas dom�nios do Sistema SIMINC2 - M�dulo Painel (Gest�o Estrat�gica).
 *
 * @since 2018/12/05
 * @author Douglas Santana <douglas.fontes@cultura.gov.br>
 */

UPDATE painel.secretaria SET secstatus = 'I';
UPDATE painel.agendagoverno SET aggstatus = 'I';
UPDATE painel.acao SET acastatus = 'I';
UPDATE painel.indicador SET indstatus = 'I';
UPDATE painel.metaindicador SET metstatus = 'I';
UPDATE seguranca.menu SET mnushow = 'f' WHERE mnucod = 1960;
UPDATE seguranca.menu SET mnushow = 'f' WHERE mnucod = 1961;
UPDATE seguranca.menu SET mnushow = 'f' WHERE mnucod = 1940;

/*
 * Arquivo de Remo��o de dados das tabelas dom�nios do Sistema SIMINC2 - M�dulo Planacomorc (Planejamento Or�ament�rio).
 *
 * @since 2018/12/05
 * @author Douglas Santana <douglas.fontes@cultura.gov.br>
 */


UPDATE proposta.preplanointerno SET plistatus = 'I';
UPDATE public.metappa SET mppstatus = 'I';
UPDATE public.indicadorpnc SET ipnstatus = 'I';
UPDATE planejamento.enquadramento_despesa SET eqdstatus = 'I';
UPDATE public.iniciativappa SET ippstatus = 'I';
UPDATE monitora.acao SET acastatus = 'I';
UPDATE monitora.ptres SET ptrstatus = 'I';
UPDATE public.metapnc SET mpnstatus = 'I';

UPDATE planacomorc.pi_janela SET pijstatus = 'I';
UPDATE planejamento.categoria_apropriacao SET capstatus = 'I';
UPDATE planejamento.area_cultural SET mdestatus = 'I';
UPDATE monitora.pi_produto SET pprstatus = 'I';
UPDATE planacomorc.manutencaoitem SET maistatus = 'I';
UPDATE monitora.programa SET prgstatus = 'I';

/*
 * Arquivo de Remo��o de dados das tabelas dom�nios do Sistema SIMINC2 - M�dulo Proposta Or�ament�ria.
 *
 * @since 2018/12/05
 * @author Leonardo dos Santos <leonardo.barreiros@cultura.gov.br>
 */

UPDATE proposta.proposta SET prostatus = 'I';
UPDATE proposta.preplanointerno SET plistatus = 'I';
UPDATE proposta.propostadetalhe SET prdstatus = 'I';
UPDATE public.idoc SET idostatus = 'I';
UPDATE wssof.ws_momentosdto SET snativo = '0';
UPDATE public.naturezadespesa SET ndpstatus = 'I';
UPDATE public.fonterecurso SET fonstatus = 'I';

DELETE FROM wssof.ws_acoesdto;
DELETE FROM wssof.ws_planosorcamentariosdto;
DELETE FROM wssof.ws_localizadoresdto;

/*

VACUUM FULL VERBOSE wssof.ws_acoesdto;
VACUUM FULL VERBOSE wssof.ws_planosorcamentariosdto;
VACUUM FULL VERBOSE wssof.ws_localizadoresdto;

*/