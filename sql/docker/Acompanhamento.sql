/*
 * Arquivo de Remoção de dados das tabelas domínios do Sistema SIMINC2 - Módulo Acompanhamento (Monitoramento).
 *
 * @since 2018/12/05
 * @author Douglas Santana <douglas.fontes@cultura.gov.br>
 */


UPDATE public.metapnc SET mpnstatus = 'I';
UPDATE monitora.pi_planointerno SET plistatus = 'I';
UPDATE public.objetivoppa SET oppstatus = 'I'
UPDATE acompanhamento.acompanhamento SET acostatus = 'I';