BEGIN; -- ROLLBACK;

-- DROP TABLE planointerno.tipoinstrumento;
/*
CREATE TABLE planointerno.tipoinstrumento
(
  tpiid serial NOT NULL,
  tpidsc character varying(250) NOT NULL,
  tpistatus character(1) DEFAULT 'A'::bpchar,
  prsano character(4),
  CONSTRAINT pk_planointerno_tipoinstrumento PRIMARY KEY (tpiid)
)
WITH (
  OIDS=FALSE
);
*/

SELECT
	*
FROM planejamento.categoria_apropriacao
WHERE
	capano = '2016'
;

--de -- SELECT * FROM planointerno.tipoinstrumento WHERE prsano = '2017'
-- para -- SELECT * FROM planejamento.categoria_apropriacao
-- DELETE FROM obras.unidadeobrasubacao; -- SELECT * FROM obras.unidadeobrasubacao;
-- DELETE FROM planejamento.categoria_apropriacao;
-- TRUNCATE TABLE planointerno.tipoinstrumento;
BEGIN; -- ROLLBACK;
INSERT INTO planejamento.categoria_apropriacao (
	capcod,
	capdsc,
	capano
)
SELECT
	tpiid,
	tpidsc,
	'2016'
FROM planointerno.tipoinstrumento
WHERE
	prsano = '2017'
;

DELETE FROM planejamento.categoria_apropriacao WHERE capid = 278;

-- COMMIT; ROLLBACK;