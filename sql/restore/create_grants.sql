SELECT
	linha_comando
FROM(
	-- PERMISSÕES PARA USAR TODOS OS ESQUEMAS
	SELECT DISTINCT
		'GRANT USAGE ON SCHEMA "' || schemaname || '" TO "usr_simec";' AS linha_comando
	FROM
		pg_catalog.pg_tables -- 6230
	WHERE
		schemaname NOT IN('topology', 'tiger', 'pg_catalog', 'information_schema', 'pg_toast')


	UNION ALL

	-- PERMISSÕES EM TODAS AS TABELAS DE TODOS OS ESQUEMAS
	SELECT DISTINCT
		'GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE "' || schemaname || '"."' || tablename || '" TO "usr_simec";' AS linha_comando
	FROM
		pg_catalog.pg_tables -- 6230
	WHERE
		schemaname NOT IN('topology', 'tiger', 'pg_catalog', 'information_schema', 'pg_toast')

	UNION

	-- PERMISSÕES EM TODAS AS SEQUENCES DE ESQUEMAS
	SELECT DISTINCT
		'GRANT SELECT ON ALL SEQUENCES IN SCHEMA "' || schemaname || '" TO "usr_simec";' AS linha_comando
	FROM
		pg_catalog.pg_tables -- 6230
	WHERE
		schemaname NOT IN('topology', 'tiger', 'pg_catalog', 'information_schema', 'pg_toast')

	UNION

	-- PERMISSÕES EM TODAS AS SEQUENCES DE TODOS OS ESQUEMAS
	SELECT
		'GRANT USAGE, SELECT ON SEQUENCE "' || sequence_schema || '"."' || sequence_name || '" TO "usr_simec";' AS linha_de_comando
	FROM information_schema.sequences

	UNION

	-- PERMISSÕES EM TODAS AS SEQUENCES
	SELECT DISTINCT
		'GRANT SELECT ON TABLE "' || table_schema || '"."' || table_name || '" TO "usr_simec";' AS linha_comando
	FROM INFORMATION_SCHEMA.views
	WHERE
		table_schema NOT IN('topology', 'tiger', 'pg_catalog', 'information_schema', 'pg_toast')
) obj_db
ORDER BY
	linha_comando
;
