#!/bin/bash
# Substituir dados [USU�RIO_SERVIDOR_BKP], [IP_SERVIDOR_BKP], [IP_SERVIDOR_BANCO]

# Copia o arquivo do servidor para a pasta local
scp [USU�RIO_SERVIDOR_BKP]@[IP_SERVIDOR_BKP]:/home/bkp_siminc/bkp_prod_dbsiminc.backup .

# Criar banco siminc2_hom_new
psql --host [IP_SERVIDOR_BANCO] --port 5432 --username "postgres" -c "CREATE DATABASE siminc2_hom_new;"

psql --host [IP_SERVIDOR_BANCO] --port 5432 --username "postgres" -c "ALTER DATABASE siminc2_hom_new SET datestyle TO European; ALTER DATABASE siminc2_hom_new SET timezone TO 'America/Sao_Paulo';"

pg_restore --host [IP_SERVIDOR_BANCO] --port 5432 --username "postgres" --dbname "siminc2_hom_new" --disable-triggers -O -x --verbose bkp_prod_dbsiminc.backup

# Executar cria��o de estrutura de auditoria e mudan�a de senhas, emails
psql --host [IP_SERVIDOR_BANCO] --port 5432 --username "postgres" --dbname "siminc2_hom_new" -f create_auditoria.sql

# Executar permiss��es nas tabelas
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

# Configurando usu�rio inicial/padr�o pra acesso ao sistema
psql --host [IP_SERVIDOR_BANCO] --port 5432 --username "postgres" --dbname "siminc2_homologacao" -c "
DELETE FROM seguranca.usuario_sistema WHERE usucpf = '86274565426';
DELETE FROM seguranca.perfilusuario WHERE usucpf = '86274565426';
UPDATE seguranca.usuario SET suscod = 'A' WHERE usucpf = '86274565426';
INSERT INTO seguranca.usuario_sistema(
    usucpf,
    sisid,
    susstatus,
    pflcod,
    susdataultacesso,
    suscod
)
SELECT
    '86274565426',
    sisid,
    susstatus,
    pflcod,
    susdataultacesso,
    suscod
FROM seguranca.usuario_sistema
WHERE
    usucpf = '00764786105'
    AND suscod = 'A'
    AND susstatus = 'A'
;
INSERT INTO seguranca.perfilusuario(
    usucpf,
    pflcod
)
SELECT
    '86274565426',
    pflcod
FROM seguranca.perfilusuario
WHERE
    usucpf = '00764786105'
;
"

# Apagando tabelas de logs que n�o s�o necess�rias
psql --host [IP_SERVIDOR_BANCO] --port 5432 --username "postgres" --dbname "siminc2_homologacao" -c "
TRUNCATE TABLE acomporc.mensagensretorno;
DELETE FROM spo.logws;
VACUUM FULL VERBOSE acomporc.mensagensretorno;
VACUUM FULL VERBOSE spo.logws;"

