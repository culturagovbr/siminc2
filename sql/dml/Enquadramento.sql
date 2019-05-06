BEGIN; -- ROLLBACK;

/*
	SELECT
		*
	FROM information_schema.constraint_table_usage
	WHERE
		table_name = 'previsaoorcamentaria'
*/

DELETE FROM monitora.pi_planointernohistorico;

DELETE FROM monitora.pi_planointernoptres;

DELETE FROM monitora.previsaoorcamentaria;
DELETE FROM ted.previsaoorcamentaria;

DELETE FROM monitora.pi_planointerno;

DELETE FROM monitora.pi_subacaoenquadramento;

DELETE FROM planacomorc.solicitacaopidotacao;

DELETE FROM planacomorc.solicitacaocriacaopi;


SELECT
	*
/*
	eqdid AS codigo,
	eqdcod || ' - ' || eqddsc AS descricao
*/
FROM planejamento.enquadramento_despesa
WHERE eqdano = '2016'
;

--de -- SELECT * FROM planointerno.enquadramentodespesa WHERE prsano = '2016';
-- para -- SELECT * FROM planejamento.enquadramento_despesa WHERE eqdano = '2016';
-- DELETE FROM planejamento.enquadramento_despesa;
-- TRUNCATE TABLE planointerno.enquadramentodespesa;
BEGIN; -- ROLLBACK;
INSERT INTO planejamento.enquadramento_despesa(
	eqdcod,
	eqddsc,
	eqdano
)
SELECT
	eqdsigla,
	eqdnome,
	'2017'
FROM planointerno.enquadramentodespesa
WHERE prsano = '2017'
;

UPDATE planejamento.enquadramento_despesa SET eqddsc = 'Emendas', eqdstatus = 'I' WHERE eqdid = 355;

-- COMMIT; ROLLBACK;