#!/bin/bash
# Substituir dados [USUÁRIO_SERVIDOR_BKP], [IP_SERVIDOR_BKP], [IP_SERVIDOR_BANCO]

# Copia o arquivo do servidor para a pasta local
scp [USUÁRIO_SERVIDOR_BKP]@[IP_SERVIDOR_BKP]:/home/bkp_siminc/bkp_prod_dbsiminc.backup .

# Criar banco siminc2_hom_new
psql --host [IP_SERVIDOR_BANCO] --port 5432 --username "postgres" -c "CREATE DATABASE siminc2_hom_new;"

psql --host [IP_SERVIDOR_BANCO] --port 5432 --username "postgres" -c "ALTER DATABASE siminc2_hom_new SET datestyle TO European; ALTER DATABASE siminc2_hom_new SET timezone TO 'America/Sao_Paulo';"

pg_restore --host [IP_SERVIDOR_BANCO] --port 5432 --username "postgres" --dbname "siminc2_hom_new" --disable-triggers -O -x --verbose bkp_prod_dbsiminc.backup

# Executar criação de estrutura de auditoria e mudança de senhas, emails
psql --host [IP_SERVIDOR_BANCO] --port 5432 --username "postgres" --dbname "siminc2_hom_new" -f create_auditoria.sql

# Executar permissções nas tabelas
psql --host [IP_SERVIDOR_BANCO] --port 5432 --username "postgres" --dbname "siminc2_hom_new" -f grants.sql

# Deleta base de dados de backup
psql --host [IP_SERVIDOR_BANCO] --port 5432 --username "postgres" -c "DROP DATABASE IF EXISTS siminc2_homologacao_bkp;"

# Finalizar processos de banco e Mudar nome de bases
psql --host [IP_SERVIDOR_BANCO] --port 5432 --username "postgres" -c "
SELECT
    pg_terminate_backend(pid)
FROM
    pg_stat_activity
WHERE
    datname = 'siminc2_homologacao';

SELECT
    pg_terminate_backend(pid)
FROM
    pg_stat_activity
WHERE
    datname = 'siminc2_hom_new';

ALTER DATABASE siminc2_homologacao RENAME TO siminc2_homologacao_bkp;
ALTER DATABASE siminc2_hom_new RENAME TO siminc2_homologacao;
"

# Apagando tabelas de logs que não são necessárias
psql --host [IP_SERVIDOR_BANCO] --port 5432 --username "postgres" --dbname "siminc2_homologacao" -c "
TRUNCATE TABLE acomporc.mensagensretorno;
DELETE FROM spo.logws;
VACUUM FULL VERBOSE acomporc.mensagensretorno;
VACUUM FULL VERBOSE spo.logws;"

