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
