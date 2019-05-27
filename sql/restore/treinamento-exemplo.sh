#!/bin/bash
# Substituir dados [USUÁRIO_SERVIDOR_BKP], [IP_SERVIDOR_BKP], [IP_SERVIDOR_BANCO], [SENHA_BD]

# Copia o arquivo do servidor para a pasta local
scp [USUÁRIO_SERVIDOR_BKP]@[IP_SERVIDOR_BKP]:/home/bkp_siminc/bkp_prod_dbsiminc.backup .

# Criar banco siminc2_tr_new
PGPASSWORD=[SENHA_BD] psql --host [IP_SERVIDOR_BANCO] --port 5432 --username "postgres" -c "CREATE DATABASE siminc2_tr_new;"

PGPASSWORD=[SENHA_BD] psql --host [IP_SERVIDOR_BANCO] --port 5432 --username "postgres" -c "ALTER DATABASE siminc2_tr_new SET datestyle TO European; ALTER DATABASE siminc2_tr_new SET timezone TO 'America/Sao_Paulo';"

pg_restore --dbname=postgres://postgres:[SENHA_BD]@[IP_SERVIDOR_BANCO]:5432/siminc2_tr_new --disable-triggers -O -x --verbose bkp_prod_dbsiminc.backup

# Executar criação de estrutura de auditoria e mudança de senhas, emails
PGPASSWORD=[SENHA_BD] psql --host [IP_SERVIDOR_BANCO] --port 5432 --username "postgres" --dbname "siminc2_tr_new" -f create_auditoria.sql

# Executar permissções nas tabelas
PGPASSWORD=[SENHA_BD] psql --host [IP_SERVIDOR_BANCO] --port 5432 --username "postgres" --dbname "siminc2_tr_new" -f grants.sql

# Deleta base de dados de backup
PGPASSWORD=[SENHA_BD] psql --host [IP_SERVIDOR_BANCO] --port 5432 --username "postgres" -c "DROP DATABASE IF EXISTS siminc2_treinamento_bkp;"

# Finalizar processos de banco e Mudar nome de bases
PGPASSWORD=[SENHA_BD] psql --host [IP_SERVIDOR_BANCO] --port 5432 --username "postgres" -c "
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

# Configurando usuário inicial/padrão pra acesso ao sistema
PGPASSWORD=[SENHA_BD] psql --host [IP_SERVIDOR_BANCO] --port 5432 --username "postgres" --dbname "siminc2_treinamento" -c "
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

# Criando arquivo de dump pra o ambiente de desenvolvimento atualizado com permissões e tabela de auditoria vazia
# pg_dump -v --dbname=postgres://postgres:[SENHA_BD]@[IP_SERVIDOR_BANCO]:5432/siminc2_treinamento -Fc > siminc2_desenvolvimento.bkp
