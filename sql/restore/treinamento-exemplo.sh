#!/bin/bash
# Substituir dados [USUÁRIO_SERVIDOR_BKP], [IP_SERVIDOR_BKP], [IP_SERVIDOR_BANCO]

# Copia o arquivo do servidor para a pasta local
scp [USUÁRIO_SERVIDOR_BKP]@[IP_SERVIDOR_BKP]:/home/bkp_siminc/bkp_prod_dbsiminc.backup .

# Criar banco siminc2_tr_new
psql --host [IP_SERVIDOR_BANCO] --port 5432 --username "postgres" -c "CREATE DATABASE siminc2_tr_new;"

psql --host [IP_SERVIDOR_BANCO] --port 5432 --username "postgres" -c "ALTER DATABASE siminc2_tr_new SET datestyle TO European; ALTER DATABASE siminc2_tr_new SET timezone TO 'America/Sao_Paulo';"

pg_restore --host [IP_SERVIDOR_BANCO] --port 5432 --username "postgres" --dbname "siminc2_tr_new" --disable-triggers -O -x --verbose bkp_prod_dbsiminc.backup

# Executar criação de estrutura de auditoria e mudança de senhas, emails
psql --host [IP_SERVIDOR_BANCO] --port 5432 --username "postgres" --dbname "siminc2_tr_new" -f create_auditoria.sql

# Executar permissções nas tabelas
psql --host [IP_SERVIDOR_BANCO] --port 5432 --username "postgres" --dbname "siminc2_tr_new" -f grants.sql

# Deleta base de dados de backup
psql --host [IP_SERVIDOR_BANCO] --port 5432 --username "postgres" -c "DROP DATABASE IF EXISTS siminc2_treinamento_bkp;"

# Finalizar processos de banco e Mudar nome de bases
psql --host [IP_SERVIDOR_BANCO] --port 5432 --username "postgres" -c "
SELECT
    pg_terminate_backend(pid)
FROM
    pg_stat_activity
WHERE
    datname = 'siminc2_treinamento';

SELECT
    pg_terminate_backend(pid)
FROM
    pg_stat_activity
WHERE
    datname = 'siminc2_tr_new';

ALTER DATABASE siminc2_treinamento RENAME TO siminc2_treinamento_bkp;
ALTER DATABASE siminc2_tr_new RENAME TO siminc2_treinamento;
"

# Apagando tabelas de logs que não são necessárias
psql --host [IP_SERVIDOR_BANCO] --port 5432 --username "postgres" --dbname "siminc2_treinamento" -c "
TRUNCATE TABLE acomporc.mensagensretorno;
DELETE FROM spo.logws;
VACUUM FULL VERBOSE acomporc.mensagensretorno;
VACUUM FULL VERBOSE spo.logws;"

# Criando arquivo de dump pra o ambiente de desenvolvimento atualizado com permissões pra o usuário usr_simec e tabela de auditoria vazia
pg_dump -v -h [IP_SERVIDOR_BANCO] -p 5432 -W -Fc -U postgres siminc2_treinamento > siminc2_desenvolvimento.bkp
