#!/bin/bash

psql --port 5432 --username "postgres" --dbname "siminc2_desenvolvimento" -f /var/www/db-init/users.sql

psql --port 5432 --username "postgres" -c "ALTER DATABASE siminc2_desenvolvimento SET datestyle TO European; ALTER DATABASE siminc2_desenvolvimento SET timezone TO 'America/Sao_Paulo';"

pg_restore --port 5432 --username "postgres" --dbname "siminc2_desenvolvimento" --disable-triggers --verbose /var/www/db-init/siminc2_desenvolvimento.bkp
