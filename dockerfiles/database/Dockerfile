FROM postgres:latest

RUN apt-get update

COPY /dockerfiles/database/dbinit.sql /docker-entrypoint-initdb.d/dbinit.sql 
