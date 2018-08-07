psql --port 5432 --username "postgres" --dbname "siminc2_desenvolvimento" -f /var/www/db-init/users.sql

psql --port 5432 --username "postgres" -c "ALTER DATABASE siminc2_desenvolvimento SET datestyle TO European;"

pg_restore --port 5432 --username "postgres" --dbname "siminc2_desenvolvimento" --disable-triggers -O -x --verbose /var/www/db-init/bkp_prod_dbsiminc.backup

# Executar cria��o de estrutura de auditoria e mudan�a de senhas, emails
psql --port 5432 --username "postgres" --dbname "siminc2_desenvolvimento" -f /var/www/db-init/create_auditoria.sql

# Executar permiss��es nas tabelas
psql --port 5432 --username "postgres" --dbname "siminc2_desenvolvimento" -f /var/www/db-init/grants.sql
