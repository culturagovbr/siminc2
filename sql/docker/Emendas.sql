/*
 * Arquivo de Remoção de dados das tabelas domínios do Sistema SIMINC2 - Módulo Emendas Parlamentares.
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