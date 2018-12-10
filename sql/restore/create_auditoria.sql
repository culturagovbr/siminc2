
CREATE SCHEMA auditoria
  AUTHORIZATION postgres;

GRANT ALL ON SCHEMA auditoria TO postgres;
GRANT USAGE ON SCHEMA auditoria TO usr_simec;

CREATE TABLE auditoria.auditoria
(
  audid serial NOT NULL,
  usucpf character(11),
  mnuid integer,
  audsql text,
  audtabela character varying(100),
  audtipo character(1),
  audip character varying(20),
  auddata timestamp without time zone DEFAULT now(),
  audmsg text,
  sisid integer,
  audscript character varying(5000),
  CONSTRAINT pk_auditoria_11_2014 PRIMARY KEY (audid)
)
WITH (
  OIDS=FALSE
);
ALTER TABLE auditoria.auditoria
  OWNER TO postgres;
GRANT ALL ON TABLE auditoria.auditoria TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE auditoria.auditoria TO usr_simec;

-- Index: auditoria.idx_auddata

-- DROP INDEX auditoria.idx_auddata;

CREATE INDEX idx_auddata
  ON auditoria.auditoria
  USING btree
  (auddata);

-- Index: auditoria.idx_audip_audata

-- DROP INDEX auditoria.idx_audip_audata;

CREATE INDEX idx_audip_audata
  ON auditoria.auditoria
  USING btree
  (audip COLLATE pg_catalog."default", auddata);

-- Index: auditoria.idx_audtabela_auddata

-- DROP INDEX auditoria.idx_audtabela_auddata;

CREATE INDEX idx_audtabela_auddata
  ON auditoria.auditoria
  USING btree
  (audtabela COLLATE pg_catalog."default", auddata);

-- Index: auditoria.idx_audtipo_auddata

-- DROP INDEX auditoria.idx_audtipo_auddata;

CREATE INDEX idx_audtipo_auddata
  ON auditoria.auditoria
  USING btree
  (audtipo COLLATE pg_catalog."default", auddata);

-- Index: auditoria.idx_sisid_auddata

-- DROP INDEX auditoria.idx_sisid_auddata;

CREATE INDEX idx_sisid_auddata
  ON auditoria.auditoria
  USING btree
  (sisid, auddata);

-- Index: auditoria.idx_usucpf_auddata

-- DROP INDEX auditoria.idx_usucpf_auddata;

CREATE INDEX idx_usucpf_auddata
  ON auditoria.auditoria
  USING btree
  (usucpf COLLATE pg_catalog."default", auddata);

-- Atualizando senhas e e-mail pra evitar spam e fishing atraves da base de dados
UPDATE
    seguranca.usuario u
SET
    regcod = 'DF',
    usunome = CASE WHEN LENGTH(SUBSTR(usunome, 1, POSITION(' ' IN usunome))) > 0 THEN SUBSTR(usunome, 1, POSITION(' ' IN usunome)) ELSE usunome END,
    usuemail = 'teste@teste.com.br',
    usufoneddd = '99',
    usufonenum = '9999-9999',
    ususenha = 'o/0m5tlONgaBe9NwzktC4uUvv+26NqEE6YAJmOz4Qn4=', -- 123456
    usufuncao = 'N/A',
    usunomeguerra = SUBSTR(usunome, 1, POSITION(' ' IN usunome)),
    muncod = '5300108',
    usudatanascimento = '1984-01-01',
    entid = 390374,
    carid = 16
;

-- Criando Usuário Padrão/Default para demostração do sistema

-- Ativa o usuário de teste
UPDATE seguranca.usuario SET usustatus = 'A', suscod = 'A' where usucpf ='86274565426';
-- Vincula usuário aos principais módulos ativos na versão de demonstração
UPDATE seguranca.usuario_sistema SET suscod = 'A' where sisid in(4, 157, 251, 255, 256, 48) and usucpf ='86274565426';
-- Deleta todos os perfis de usuário
DELETE FROM seguranca.perfilusuario where usucpf = '86274565426';
-- Insere Perfis de Usuário aos módulos ativos
INSERT INTO seguranca.perfilusuario ( usucpf, pflcod ) values ( '86274565426', 25), ( '86274565426', 955), ( '86274565426', 349), ( '86274565426', 1501), ( '86274565426', 1502), ( '86274565426', 1512);
