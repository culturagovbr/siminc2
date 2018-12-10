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
