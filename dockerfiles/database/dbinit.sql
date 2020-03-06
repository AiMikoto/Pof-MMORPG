CREATE DATABASE pof;
\c pof
CREATE TABLE userinfo (
	ID serial NOT NULL PRIMARY KEY, 
	usercard json NOT NULL,
	username varchar(64) UNIQUE NOT NULL,
	passwordhash varchar(256) NOT NULL
);
