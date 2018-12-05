/*
 * Arquivo de Remoção de dados das tabelas domínios do Sistema SIMINC2 - Módulo Planacomorc (Planejamento Orçamentário).
 *
 * @since 2018/12/05
 * @author Douglas Santana <douglas.fontes@cultura.gov.br>
 */


UPDATE proposta.preplanointerno SET plistatus = 'I';
UPDATE public.metappa SET mppstatus = 'I';
UPDATE public.indicadorpnc SET ipnstatus = 'I';
UPDATE monitora.pi_enquadramentodespesa SET eqdstatus = 'I';
UPDATE public.iniciativappa SET ippstatus = 'I';
UPDATE monitora.acao SET acastatus = 'I';
