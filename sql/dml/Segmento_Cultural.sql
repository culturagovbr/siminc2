BEGIN; -- ROLLBACK;

/*
-- DROP TABLE planointerno.segmento;

CREATE TABLE planointerno.segmento
(
  segid serial NOT NULL,
  segdsc character varying(60) NOT NULL,
  segstatus character(1) NOT NULL DEFAULT 'A'::bpchar,
  areid integer,
  CONSTRAINT pk_segmento PRIMARY KEY (segid),
  CONSTRAINT ck_segmento_status CHECK (segstatus = ANY (ARRAY['A'::bpchar, 'I'::bpchar]))
)
WITH (
  OIDS=FALSE
);

*/

SELECT
	*
FROM planejamento.segmento_cultural
WHERE
	neeano = '2016'
;

--de -- SELECT * FROM planointerno.segmento;
-- para -- SELECT * FROM planejamento.segmento_cultural WHERE neeano = '2016';
-- DELETE FROM planejamento.segmento_cultural;
-- TRUNCATE TABLE planointerno.segmento;
BEGIN; -- ROLLBACK;
INSERT INTO planejamento.segmento_cultural(
	arceid, -- Área Cultural -- SELECT * FROM planejamento.area_cultural WHERE arcstatus = 'A' AND arcano = '2016'
	neecod,
	needsc,
	neeano
)
SELECT
	p_m.arceid AS arceid,
	s.segid,
	s.segdsc,
	'2016'
FROM planointerno.segmento s
	JOIN planointerno.areacultural a ON(s.areid = a.areid AND a.prsano = '2017') -- SELECT * FROM planointerno.areacultural
	JOIN planejamento.area_cultural p_m ON(a.arenome = p_m.arcdsc AND p_m.arcano = '2016') -- SELECT * FROM planejamento.area_cultural WHERE arcstatus = 'A' AND arcano = '2016'
;

-- ALTER TABLE planejamento.segmento_cultural DROP COLUMN neecod;

ALTER TABLE planejamento.segmento_cultural ADD COLUMN neecod character varying(10);

-- COMMIT; ROLLBACK;