#!/bin/bash

psql --port 5432 --username "postgres" --dbname "siminc2_desenvolvimento" -f /var/www/db-init/users.sql

psql --port 5432 --username "postgres" -c "ALTER DATABASE siminc2_desenvolvimento SET datestyle TO European;"

pg_restore --port 5432 --username "postgres" --dbname "siminc2_desenvolvimento" --disable-triggers -O -x --verbose /var/www/db-init/bkp_prod_dbsiminc.backup
