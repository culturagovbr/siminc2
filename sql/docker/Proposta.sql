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
DELETE FROM wssof.ws_acoesdto;
DELETE FROM wssof.ws_planosorcamentariosdto;
DELETE FROM wssof.ws_localizadoresdto;
