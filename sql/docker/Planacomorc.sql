/*
 * Arquivo de Remoção de dados das tabelas domínios do Sistema SIMINC2 - Módulo Planacomorc (Planejamento Orçamentário).
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
UPDATE monitora.pi_categoriaapropriacao SET capstatus = 'I';
UPDATE monitora.pi_modalidadeensino SET mdestatus = 'I';
UPDATE monitora.pi_produto SET pprstatus = 'I';
UPDATE planacomorc.manutencaoitem SET maistatus = 'I';
UPDATE monitora.programa SET prgstatus = 'I';
