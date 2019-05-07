BEGIN; --ROLLBACK; COMMIT;

ALTER TABLE planejamento.segmento_cultural
   ADD COLUMN arceid integer NOT NULL DEFAULT NULL;
COMMENT ON COLUMN planejamento.segmento_cultural.arceid
  IS 'Relação com a tabela de Área Cultural. planejamento.area_cultural';

ALTER TABLE planejamento.segmento_cultural
   ADD CONSTRAINT fk_pi_niveletapaensino_reference_segmento_cultural FOREIGN KEY(arceid)
  REFERENCES planejamento.area_cultural(arceid) MATCH SIMPLE
  ON UPDATE CASCADE ON DELETE RESTRICT;

-- DROP TABLE IF EXISTS monitora.pi_objetivoppa_metappa;
CREATE TABLE monitora.pi_objetivo_ppa
(
  oppid SERIAL PRIMARY KEY,
  prsano CHAR(4),
  oppdesc VARCHAR(1000),
  oppnome VARCHAR(200),
  oppcod VARCHAR(4),
  oppstatus char default 'A'::character varying,
  CONSTRAINT fk_objppa_reference_programa FOREIGN KEY (prsano)
  REFERENCES monitora.programacaoexercicio (prsano) MATCH SIMPLE
  ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT ckc_oppstatus_objppa CHECK (oppstatus= ANY (ARRAY['A'::bpchar, 'I'::bpchar]))
);
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE monitora.pi_objetivo_ppa TO usr_simec;

-- DROP TABLE IF EXISTS monitora.pi_metas_ppa;
CREATE TABLE monitora.pi_metas_ppa
(
  mppid SERIAL PRIMARY KEY,
  mppdesc VARCHAR(1000),
  mppcod CHARACTER(4),
  mppnome CHARACTER VARYING(400),
  mppstatus char default 'A'::character varying,
  prsano CHAR(4),
  CONSTRAINT fk_metppa_reference_programa FOREIGN KEY (prsano)
  REFERENCES monitora.programacaoexercicio (prsano) MATCH SIMPLE
  ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT ckc_mppstatus_metppa CHECK (mppstatus = ANY (ARRAY['A'::bpchar, 'I'::bpchar]))
);
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE monitora.pi_metas_ppa TO usr_simec;

-- DROP TABLE IF EXISTS monitora.pi_objetivoppa_metappa;
CREATE TABLE monitora.pi_objetivoppa_metappa
(
  opmid serial NOT NULL,
  oppid integer NOT NULL,
  mppid integer NOT NULL,
  mpodata timestamp without time zone NOT NULL DEFAULT now(),
  CONSTRAINT pk_pi_objetivoppa_metappa PRIMARY KEY (opmid),
  CONSTRAINT fk_objetivoppa_metappa_reference_pi_objetivo_ppa FOREIGN KEY (oppid)
      REFERENCES monitora.pi_objetivo_ppa (oppid) MATCH SIMPLE
      ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT fk_objetivoppa_metappa_reference_metas_ppa FOREIGN KEY (mppid)
      REFERENCES monitora.pi_metas_ppa (mppid) MATCH SIMPLE
      ON UPDATE CASCADE ON DELETE RESTRICT
)
WITH (
  OIDS=FALSE
);
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE monitora.pi_objetivoppa_metappa TO usr_simec;

-- DROP TABLE IF EXISTS monitora.pi_iniciativa_ppa; -- Renomeada pra pi_indicador_ppa
CREATE TABLE monitora.pi_iniciativa_ppa
(
  ippid serial NOT NULL,
  oppid integer NOT NULL,
  ippdesc VARCHAR(1000),
  ippnome CHARACTER VARYING(500),
  ippcod VARCHAR(4),
  ippstatus char default 'A'::character varying,
  prsano CHAR(4),
  PRIMARY KEY (ippid,oppid),
  CONSTRAINT fk_inippa_reference_obj FOREIGN KEY (oppid)
  REFERENCES monitora.pi_objetivo_ppa (oppid) MATCH SIMPLE
  ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT fk_indppa_reference_programa FOREIGN KEY (prsano)
  REFERENCES monitora.programacaoexercicio (prsano) MATCH SIMPLE
  ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT ckc_ippstatus_indppa CHECK (ippstatus = ANY (ARRAY['A'::bpchar, 'I'::bpchar]))
);
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE monitora.pi_iniciativa_ppa TO usr_simec;

-- DROP TABLE IF EXISTS monitora.pi_meta_pnc;
CREATE TABLE monitora.pi_meta_pnc
(
  mpnid SERIAL PRIMARY KEY,
  mpndesc VARCHAR(1000),
  mpnstatus char default 'A'::character varying,
  mpncod character(4) NOT NULL,
  mpnnome character varying(400) NOT NULL,
  prsano CHAR(4),
  CONSTRAINT fk_metppa_reference_programa FOREIGN KEY (prsano)
  REFERENCES monitora.programacaoexercicio (prsano) MATCH SIMPLE
  ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT ckc_mpnstatus_metpnc CHECK (mpnstatus = ANY (ARRAY['A'::bpchar, 'I'::bpchar]))
);
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE monitora.pi_meta_pnc TO usr_simec;

-- DROP TABLE monitora.pi_indicador_pnc;
CREATE TABLE monitora.pi_indicador_pnc
(
  ipnid serial NOT NULL,
  mpnid integer NOT NULL,
  ipndesc character varying(1000),
  ipnstatus character(1) DEFAULT 'A'::character varying,
  prsano character(4),
  ipncod character(3),
  CONSTRAINT pi_indicador_pnc_pkey PRIMARY KEY (ipnid, mpnid),
  CONSTRAINT fk_indpnc_reference_meta FOREIGN KEY (mpnid)
      REFERENCES monitora.pi_meta_pnc (mpnid) MATCH SIMPLE
      ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT fk_indppa_reference_programa FOREIGN KEY (prsano)
      REFERENCES monitora.programacaoexercicio (prsano) MATCH SIMPLE
      ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT ckc_ipnstatus_indpnc CHECK (ipnstatus = ANY (ARRAY['A'::bpchar, 'I'::bpchar]))
)
WITH (
  OIDS=FALSE
);
ALTER TABLE monitora.pi_indicador_pnc
  OWNER TO postgres;
GRANT ALL ON TABLE monitora.pi_indicador_pnc TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE monitora.pi_indicador_pnc TO usr_simec;

CREATE TABLE monitora.pi_produto
(
  pprid serial NOT NULL,
  prsano character(4) NOT NULL,
  pprnome character varying(200) NOT NULL,
  pprdescricao character varying(500),
  pprstatus character(1) NOT NULL DEFAULT 'A'::bpchar,
  CONSTRAINT pk_pi_produto PRIMARY KEY(pprid),
  CONSTRAINT ckc_pprstatus_pi_produto CHECK(pprstatus = ANY (ARRAY['I'::bpchar, 'A'::bpchar]))
)
WITH (
  OIDS=FALSE
);
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE monitora.pi_produto TO usr_simec;

-- DROP TABLE monitora.pi_unidade_medida;
CREATE TABLE monitora.pi_unidade_medida
(
  pumid serial NOT NULL,
  prsano character(4) NOT NULL,
  pumnome character varying(100) NOT NULL,
  pumdescricao character varying(200),
  pumstatus character(1) NOT NULL DEFAULT 'A'::bpchar,
  CONSTRAINT pk_pi_unidade_medida PRIMARY KEY(pumid),
  CONSTRAINT ckc_pi_unidade_medida CHECK(pumstatus = ANY (ARRAY['I'::bpchar, 'A'::bpchar]))
)
WITH (
  OIDS=FALSE
);
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE monitora.pi_unidade_medida TO usr_simec;


GRANT SELECT, UPDATE ON TABLE planacomorc.pi_sniic_pisid_seq TO usr_simec;
GRANT SELECT, UPDATE ON TABLE planacomorc.pi_localizacao_pilid_seq TO usr_simec;

GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planacomorc.pi_cronograma TO usr_simec;



-------------------- 24/05/2017 --------------------

-- Table: planacomorc.cronograma

-- DROP TABLE planacomorc.cronograma;

CREATE TABLE planacomorc.cronograma
(
  croid serial NOT NULL, -- Chave Primária
  crodsc character varying(200) NOT NULL, -- Descrição
  CONSTRAINT pk_cronograma_croid PRIMARY KEY (croid)
)
WITH (
  OIDS=FALSE
);
ALTER TABLE planacomorc.cronograma
  OWNER TO postgres;
GRANT ALL ON TABLE planacomorc.cronograma TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planacomorc.cronograma TO usr_simec;
COMMENT ON TABLE planacomorc.cronograma
  IS 'Tabela de Cronogramas';
COMMENT ON COLUMN planacomorc.cronograma.croid IS 'Chave Primária';
COMMENT ON COLUMN planacomorc.cronograma.crodsc IS 'Descrição';


-- Table: planacomorc.cronograma_valor

-- DROP TABLE planacomorc.cronograma_valor;

CREATE TABLE planacomorc.cronograma_valor
(
  crvid serial NOT NULL, -- Chave Primária
  crvdsc character varying(200) NOT NULL, -- Descrição
  croid integer NOT NULL, -- Identificador do Cronograma
  CONSTRAINT pk_cronograma_valor_crvid PRIMARY KEY (crvid),
  CONSTRAINT fk_cronograma_valor_croid FOREIGN KEY (croid)
      REFERENCES planacomorc.cronograma (croid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
)
WITH (
  OIDS=FALSE
);
ALTER TABLE planacomorc.cronograma_valor
  OWNER TO postgres;
GRANT ALL ON TABLE planacomorc.cronograma_valor TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planacomorc.cronograma_valor TO usr_simec;
COMMENT ON TABLE planacomorc.cronograma_valor
  IS 'Valores do Cronograma';
COMMENT ON COLUMN planacomorc.cronograma_valor.crvid IS 'Chave Primária';
COMMENT ON COLUMN planacomorc.cronograma_valor.crvdsc IS 'Descrição';
COMMENT ON COLUMN planacomorc.cronograma_valor.croid IS 'Identificador do Cronograma';




-- Table: planacomorc.pi_convenio

-- DROP TABLE planacomorc.pi_convenio;

CREATE TABLE planacomorc.pi_convenio
(
  pcoid serial NOT NULL, -- Chave Primária
  pliid integer NOT NULL, -- Plano Interno - PI
  pcoconvenio integer NOT NULL, -- Número do Convênio
  CONSTRAINT pk_pi_convenio_pcoid PRIMARY KEY (pcoid),
  CONSTRAINT fk_pi_complemento_pliid FOREIGN KEY (pliid)
      REFERENCES monitora.pi_planointerno (pliid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
)
WITH (
  OIDS=FALSE
);
ALTER TABLE planacomorc.pi_convenio
  OWNER TO postgres;
GRANT ALL ON TABLE planacomorc.pi_convenio TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planacomorc.pi_convenio TO usr_simec;
COMMENT ON TABLE planacomorc.pi_convenio
  IS 'Vínculo do Plano Interno com Convênio';
COMMENT ON COLUMN planacomorc.pi_convenio.pcoid IS 'Chave Primária';
COMMENT ON COLUMN planacomorc.pi_convenio.pliid IS 'Plano Interno - PI';
COMMENT ON COLUMN planacomorc.pi_convenio.pcoconvenio IS 'Número do Convênio';


-- Table: planacomorc.pi_cronograma

-- DROP TABLE planacomorc.pi_cronograma;

CREATE TABLE planacomorc.pi_cronograma
(
  pcrid serial NOT NULL, -- Chave Primária
  pliid integer NOT NULL, -- Plano Interno - PI
  mescod character(2) NOT NULL, -- Mês
  pcrvalor double precision, -- Valor do Cronograma
  crvid integer NOT NULL -- Identificador do Cronograma Valor
)
WITH (
  OIDS=FALSE
);
ALTER TABLE planacomorc.pi_cronograma
  OWNER TO postgres;
GRANT ALL ON TABLE planacomorc.pi_cronograma TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planacomorc.pi_cronograma TO usr_simec;
COMMENT ON TABLE planacomorc.pi_cronograma
  IS 'Cronograma Físico/Financeiro do Plano Interno';
COMMENT ON COLUMN planacomorc.pi_cronograma.pcrid IS 'Chave Primária';
COMMENT ON COLUMN planacomorc.pi_cronograma.pliid IS 'Plano Interno - PI';
COMMENT ON COLUMN planacomorc.pi_cronograma.mescod IS 'Mês';
COMMENT ON COLUMN planacomorc.pi_cronograma.pcrvalor IS 'Valor do Cronograma';
COMMENT ON COLUMN planacomorc.pi_cronograma.crvid IS 'Identificador do Cronograma Valor';

-- Table: planacomorc.pi_localizacao

-- DROP TABLE planacomorc.pi_localizacao;

CREATE TABLE planacomorc.pi_localizacao
(
  pilid serial NOT NULL, -- Chave Primária
  pliid integer NOT NULL, -- Plano Interno - PI
  estuf character(2), -- UF
  muncod character(7), -- Código IBGE do Município
  paiid integer, -- País
  CONSTRAINT pk_pi_localizacao_pilid PRIMARY KEY (pilid),
  CONSTRAINT fk_pi_complemento_pliid FOREIGN KEY (pliid)
      REFERENCES monitora.pi_planointerno (pliid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
)
WITH (
  OIDS=FALSE
);
ALTER TABLE planacomorc.pi_localizacao
  OWNER TO postgres;
GRANT ALL ON TABLE planacomorc.pi_localizacao TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planacomorc.pi_localizacao TO usr_simec;
COMMENT ON TABLE planacomorc.pi_localizacao
  IS 'Vínculo do Plano Interno com Localização do Projeto';
COMMENT ON COLUMN planacomorc.pi_localizacao.pilid IS 'Chave Primária';
COMMENT ON COLUMN planacomorc.pi_localizacao.pliid IS 'Plano Interno - PI';
COMMENT ON COLUMN planacomorc.pi_localizacao.estuf IS 'UF';
COMMENT ON COLUMN planacomorc.pi_localizacao.muncod IS 'Código IBGE do Município';
COMMENT ON COLUMN planacomorc.pi_localizacao.paiid IS 'País';


-- Table: planacomorc.pi_responsavel

-- DROP TABLE planacomorc.pi_responsavel;

CREATE TABLE planacomorc.pi_responsavel
(
  pirid serial NOT NULL, -- Chave Primária
  pliid integer NOT NULL, -- Plano Interno - PI
  usucpf character(11) NOT NULL, -- CPF do usuário Responsável
  CONSTRAINT pk_pi_responsavel_pirid PRIMARY KEY (pirid),
  CONSTRAINT fk_pi_complemento_pliid FOREIGN KEY (pliid)
      REFERENCES monitora.pi_planointerno (pliid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT fk_pi_complemento_usucpf FOREIGN KEY (usucpf)
      REFERENCES seguranca.usuario (usucpf) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
)
WITH (
  OIDS=FALSE
);
ALTER TABLE planacomorc.pi_responsavel
  OWNER TO postgres;
GRANT ALL ON TABLE planacomorc.pi_responsavel TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planacomorc.pi_responsavel TO usr_simec;
COMMENT ON TABLE planacomorc.pi_responsavel
  IS 'Vínculo do Plano Interno com Usuário';
COMMENT ON COLUMN planacomorc.pi_responsavel.pirid IS 'Chave Primária';
COMMENT ON COLUMN planacomorc.pi_responsavel.pliid IS 'Plano Interno - PI';
COMMENT ON COLUMN planacomorc.pi_responsavel.usucpf IS 'CPF do usuário Responsável';

-- Table: planacomorc.pi_sniic

-- DROP TABLE planacomorc.pi_sniic;

CREATE TABLE planacomorc.pi_sniic
(
  pisid serial NOT NULL, -- Chave Primária
  pliid integer NOT NULL, -- Plano Interno - PI
  pissniic integer NOT NULL, -- Número único do produto principal do sistema Mapas Culturais
  CONSTRAINT pk_pi_sniic_pisid PRIMARY KEY (pisid),
  CONSTRAINT fk_pi_complemento_pliid FOREIGN KEY (pliid)
      REFERENCES monitora.pi_planointerno (pliid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
)
WITH (
  OIDS=FALSE
);
ALTER TABLE planacomorc.pi_sniic
  OWNER TO postgres;
GRANT ALL ON TABLE planacomorc.pi_sniic TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planacomorc.pi_sniic TO usr_simec;
COMMENT ON TABLE planacomorc.pi_sniic
  IS 'Vínculo do Plano Interno com SNIIC';
COMMENT ON COLUMN planacomorc.pi_sniic.pisid IS 'Chave Primária';
COMMENT ON COLUMN planacomorc.pi_sniic.pliid IS 'Plano Interno - PI';
COMMENT ON COLUMN planacomorc.pi_sniic.pissniic IS 'Número único do produto principal do sistema Mapas Culturais';



-------------------- 31/05/2017 --------------------

-- Table: public.objetivoppa

-- DROP TABLE public.objetivoppa;

CREATE TABLE public.objetivoppa
(
  oppid serial NOT NULL,
  prsano character(4),
  oppdsc character varying(1000),
  oppnome character varying(200),
  oppcod character varying(4),
  oppstatus character(1) DEFAULT 'A'::character varying,
  CONSTRAINT objetivoppa_pkey PRIMARY KEY (oppid),
  CONSTRAINT ckc_oppstatus_objppa CHECK (oppstatus = ANY (ARRAY['A'::bpchar, 'I'::bpchar]))
)
WITH (
  OIDS=FALSE
);
ALTER TABLE public.objetivoppa
  OWNER TO postgres;
GRANT ALL ON TABLE public.objetivoppa TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE public.objetivoppa TO usr_simec;

insert into public.objetivoppa
select * from monitora.pi_objetivo_ppa

-- Table: public.metappa

-- DROP TABLE public.metappa;

CREATE TABLE public.metappa
(
  mppid serial NOT NULL,
  mppdsc character varying(1000),
  mppcod character(4),
  mppnome character varying(400),
  mppstatus character(1) DEFAULT 'A'::character varying,
  prsano character(4),
  CONSTRAINT metappa_pkey PRIMARY KEY (mppid),
  CONSTRAINT ckc_mppstatus_metppa CHECK (mppstatus = ANY (ARRAY['A'::bpchar, 'I'::bpchar]))
)
WITH (
  OIDS=FALSE
);
ALTER TABLE public.metappa
  OWNER TO postgres;
GRANT ALL ON TABLE public.metappa TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE public.metappa TO usr_simec;

insert into public.metappa
select * from monitora.pi_metas_ppa

-- Table: public.iniciativappa

-- DROP TABLE public.iniciativappa;

CREATE TABLE public.iniciativappa
(
  ippid serial NOT NULL,
  oppid integer NOT NULL,
  ippdsc character varying(1000),
  ippnome character varying(500),
  ippcod character varying(4),
  ippstatus character(1) DEFAULT 'A'::character varying,
  prsano character(4),
  CONSTRAINT pk_iniciativappa PRIMARY KEY (ippid),
  CONSTRAINT fk_inippa_reference_opp FOREIGN KEY (oppid)
      REFERENCES public.objetivoppa (oppid) MATCH SIMPLE
      ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT ckc_ippstatus_indppa CHECK (ippstatus = ANY (ARRAY['A'::bpchar, 'I'::bpchar]))
)
WITH (
  OIDS=FALSE
);
ALTER TABLE public.iniciativappa
  OWNER TO postgres;
GRANT ALL ON TABLE public.iniciativappa TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE public.iniciativappa TO usr_simec;

insert into public.iniciativappa
select * from monitora.pi_iniciativa_ppa

-- Table: public.objetivometappa

-- DROP TABLE public.objetivometappa;

CREATE TABLE public.objetivometappa
(
  opmid serial NOT NULL,
  oppid integer NOT NULL,
  mppid integer NOT NULL,
  mpodata timestamp without time zone NOT NULL DEFAULT now(),
  CONSTRAINT pk_objetivometappa PRIMARY KEY (opmid),
  CONSTRAINT fk_objetivometappa_reference_metas_ppa FOREIGN KEY (mppid)
      REFERENCES public.metappa (mppid) MATCH SIMPLE
      ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT fk_objetivometappareference_objetivo_ppa FOREIGN KEY (oppid)
      REFERENCES public.objetivoppa (oppid) MATCH SIMPLE
      ON UPDATE CASCADE ON DELETE RESTRICT
)
WITH (
  OIDS=FALSE
);
ALTER TABLE public.objetivometappa
  OWNER TO postgres;
GRANT ALL ON TABLE public.objetivometappa TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE public.objetivometappa TO usr_simec;



insert into public.objetivometappa
select * from monitora.pi_objetivoppa_metappa;

-- Table: public.metapnc

-- DROP TABLE public.metapnc;

CREATE TABLE public.metapnc
(
  mpnid serial NOT NULL,
  mpndsc character varying(1000),
  mpnstatus character(1) DEFAULT 'A'::character varying,
  mpncod character(4) NOT NULL,
  mpnnome character varying(1000) NOT NULL,
  prsano character(4),
  CONSTRAINT metapnc_pkey PRIMARY KEY (mpnid),
  CONSTRAINT ckc_mpnstatus_metpnc CHECK (mpnstatus = ANY (ARRAY['A'::bpchar, 'I'::bpchar]))
)
WITH (
  OIDS=FALSE
);
ALTER TABLE public.metapnc
  OWNER TO postgres;
GRANT ALL ON TABLE public.metapnc TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE public.metapnc TO usr_simec;



insert into public.metapnc
select * from monitora.pi_meta_pnc;


-- Table: public.indicadorpnc

-- DROP TABLE public.indicadorpnc;

CREATE TABLE public.indicadorpnc
(
  ipnid serial NOT NULL,
  mpnid integer NOT NULL,
  ipndsc character varying(1000),
  ipnstatus character(1) DEFAULT 'A'::character varying,
  prsano character(4),
  ipncod character(3) DEFAULT NULL::bpchar,
  CONSTRAINT pk_indicadorpnc PRIMARY KEY (ipnid),
  CONSTRAINT fk_indpnc_reference_meta FOREIGN KEY (mpnid)
      REFERENCES public.metapnc (mpnid) MATCH SIMPLE
      ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT ckc_ipnstatus_indpnc CHECK (ipnstatus = ANY (ARRAY['A'::bpchar, 'I'::bpchar]))
)
WITH (
  OIDS=FALSE
);
ALTER TABLE public.indicadorpnc
  OWNER TO postgres;
GRANT ALL ON TABLE public.indicadorpnc TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE public.indicadorpnc TO usr_simec;


insert into public.indicadorpnc
select * from monitora.pi_indicador_pnc;

-- Table: public.areacultural

-- DROP TABLE public.areacultural;

CREATE TABLE public.areacultural
(
  acuid serial NOT NULL,
  acucod character varying(1),
  acudsc character varying(250),
  acuano character(4),
  acustatus character(1) DEFAULT 'A'::bpchar,
  CONSTRAINT pk_areacultural PRIMARY KEY (acuid)
)
WITH (
  OIDS=TRUE
);
ALTER TABLE public.areacultural
  OWNER TO postgres;
GRANT ALL ON TABLE public.areacultural TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE public.areacultural TO usr_simec;

insert into public.areacultural
select * from planejamento.area_cultural


-- Table: public.segmentocultural

-- DROP TABLE public.segmentocultural;

CREATE TABLE public.segmentocultural
(
  secid serial NOT NULL,
  secdsc character varying(250),
  secano character(4),
  secstatus character(1) DEFAULT 'A'::bpchar,
  acuid integer NOT NULL, -- Relação com a tabela de Área Cultural. public.segmento_cultural
  seccod character varying(10),
  CONSTRAINT pk_segmentocultural PRIMARY KEY (secid),
  CONSTRAINT fk_segmentocultural_reference_areacultural FOREIGN KEY (acuid)
      REFERENCES public.areacultural (acuid) MATCH SIMPLE
      ON UPDATE CASCADE ON DELETE RESTRICT
)
WITH (
  OIDS=TRUE
);
ALTER TABLE public.segmentocultural
  OWNER TO postgres;
GRANT ALL ON TABLE public.segmentocultural TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE public.segmentocultural TO usr_simec;
COMMENT ON COLUMN public.segmentocultural.acuid IS 'Relação com a tabela de Área Cultural. public.segmento_cultural';


insert into public.segmentocultural
select * from planejamento.segmento_cultural;


ALTER TABLE planacomorc.pi_complemento
  ADD CONSTRAINT fk_complemento_oppid FOREIGN KEY (oppid) REFERENCES public.objetivoppa (oppid) ON UPDATE NO ACTION ON DELETE NO ACTION;

ALTER TABLE planacomorc.pi_complemento
  ADD CONSTRAINT fk_complemento_mppid FOREIGN KEY (mppid) REFERENCES public.metappa (mppid) ON UPDATE NO ACTION ON DELETE NO ACTION;

ALTER TABLE planacomorc.pi_complemento
  ADD CONSTRAINT fk_complemento_mpnid FOREIGN KEY (mpnid) REFERENCES public.metapnc (mpnid) ON UPDATE NO ACTION ON DELETE NO ACTION;

ALTER TABLE planacomorc.pi_complemento
  ADD CONSTRAINT fk_complemento_ipnid FOREIGN KEY (ipnid) REFERENCES public.indicadorpnc (ipnid) ON UPDATE NO ACTION ON DELETE NO ACTION;

ALTER TABLE planacomorc.pi_complemento
  ADD CONSTRAINT fk_complemento_ippid FOREIGN KEY (ippid) REFERENCES public.iniciativappa (ippid) ON UPDATE NO ACTION ON DELETE NO ACTION;

ALTER TABLE planacomorc.pi_complemento
  ADD CONSTRAINT fk_complemento_acuid FOREIGN KEY (acuid) REFERENCES public.areacultural (acuid) ON UPDATE NO ACTION ON DELETE NO ACTION;

ALTER TABLE planacomorc.pi_complemento
  ADD CONSTRAINT fk_complemento_secid FOREIGN KEY (secid) REFERENCES public.segmentocultural (secid) ON UPDATE NO ACTION ON DELETE NO ACTION;


-------------------- 02/06/2017 --------------------

CREATE SEQUENCE monitora.seq_codigo_pi_2016;
GRANT SELECT, USAGE ON SEQUENCE monitora.seq_codigo_pi_2016 TO usr_simec;

CREATE SEQUENCE monitora.seq_codigo_pi_2017;
GRANT SELECT, USAGE ON SEQUENCE monitora.seq_codigo_pi_2017 TO usr_simec;

CREATE SEQUENCE monitora.seq_codigo_pi_2018;
GRANT SELECT, USAGE ON SEQUENCE monitora.seq_codigo_pi_2018 TO usr_simec;

CREATE SEQUENCE monitora.seq_codigo_pi_2019;
GRANT SELECT, USAGE ON SEQUENCE monitora.seq_codigo_pi_2019 TO usr_simec;

CREATE SEQUENCE monitora.seq_codigo_pi_2020;
GRANT SELECT, USAGE ON SEQUENCE monitora.seq_codigo_pi_2020 TO usr_simec;

CREATE SEQUENCE monitora.seq_codigo_pi_2021;
GRANT SELECT, USAGE ON SEQUENCE monitora.seq_codigo_pi_2021 TO usr_simec;

CREATE SEQUENCE monitora.seq_codigo_pi_2022;
GRANT SELECT, USAGE ON SEQUENCE monitora.seq_codigo_pi_2022 TO usr_simec;

CREATE SEQUENCE monitora.seq_codigo_pi_2023;
GRANT SELECT, USAGE ON SEQUENCE monitora.seq_codigo_pi_2023 TO usr_simec;

CREATE SEQUENCE monitora.seq_codigo_pi_2024;
GRANT SELECT, USAGE ON SEQUENCE monitora.seq_codigo_pi_2024 TO usr_simec;


-- Table: planacomorc.manutencaoitem

-- DROP TABLE planacomorc.manutencaoitem;

CREATE TABLE planacomorc.manutencaoitem
(
  maiid serial NOT NULL,
  prsano character(4) NOT NULL,
  mainome character varying(200) NOT NULL,
  maidescricao character varying(500),
  maistatus character(1) NOT NULL DEFAULT 'A'::bpchar,
  eqdid integer,
  CONSTRAINT pk_manutencaoitem PRIMARY KEY (maiid),
  CONSTRAINT fk_manutencaoitem_reference_enquadramentodespesa FOREIGN KEY (eqdid)
      REFERENCES planejamento.enquadramento_despesa (eqdid) MATCH SIMPLE
      ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT ckc_maistatus_manutencaoitem CHECK (maistatus = ANY (ARRAY['I'::bpchar, 'A'::bpchar]))
);

GRANT ALL ON TABLE planacomorc.manutencaoitem TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planacomorc.manutencaoitem TO usr_simec;

-- Table: planacomorc.manutencaosubitem

-- DROP TABLE planacomorc.manutencaosubitem;

CREATE TABLE planacomorc.manutencaosubitem
(
  masid serial NOT NULL,
  maiid integer NOT NULL,
  prsano character(4) NOT NULL,
  masnome character varying(200) NOT NULL,
  masdescricao character varying(500),
  masstatus character(1) NOT NULL DEFAULT 'A'::bpchar,
  CONSTRAINT pk_manutencaosubitem PRIMARY KEY (masid),
  CONSTRAINT fk_manutencaosubitem_reference_manutencaoitem FOREIGN KEY (maiid)
      REFERENCES planacomorc.manutencaoitem (maiid) MATCH SIMPLE
      ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT ckc_masstatus_manutencaosubitem CHECK (masstatus = ANY (ARRAY['I'::bpchar, 'A'::bpchar]))
);


GRANT ALL ON TABLE planacomorc.manutencaosubitem TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planacomorc.manutencaosubitem TO usr_simec;