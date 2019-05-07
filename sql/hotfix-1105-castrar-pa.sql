-- DROP SCHEMA planejamento CASCADE;
CREATE SCHEMA planejamento AUTHORIZATION postgres;

/**

	ALTERAÇÕES DE SCHEMA DO MONITORA PARA PLANEJAMENTO.


	Total Tabelas do schema monitora : 112

	    Utilizadas => 18
	Não Utilizadas => 94

	Total VIEWS do schema monitora : 20

	    Utilizadas => 2
	Não Utilizadas => 18

**/


GRANT ALL ON SCHEMA planejamento TO postgres;
GRANT USAGE ON SCHEMA planejamento TO usr_simec;

-- DROP TABLE planejamento.programa; -- antiga monitora.programa
CREATE TABLE planejamento.programa
(
  prgid serial NOT NULL,
  prgcod character(4) NOT NULL,
  prgano character(4) NOT NULL,
  tpocod character(1),
  tprcod character(1),
  orgcod character(5),
  organo character(4),
  prgdsc character varying(200) NOT NULL,
  prgmesinicio character(2),
  prganoinicio character(4),
  prgmestermino character(2),
  prganotermino character(4),
  prgdscobjetivo text,
  prgdscpublicoalvo text,
  prgdscjustificativa text,
  prgdscestrategia text,
  prgstatus character(1) DEFAULT 'A'::bpchar,
  CONSTRAINT pk_programa_er PRIMARY KEY (prgano, prgcod, prgid),
  CONSTRAINT fk_programa_orgcodprg_orgao FOREIGN KEY (orgcod, organo)
      REFERENCES public.orgao (orgcod, organo) MATCH SIMPLE
      ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT fk_programa_tpocodprg_tipoorga FOREIGN KEY (tpocod)
      REFERENCES public.tipoorgao (tpocod) MATCH SIMPLE
      ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT fk_programa_tprcodprg_tipoprog FOREIGN KEY (tprcod)
      REFERENCES public.tipoprograma (tprcod) MATCH SIMPLE
      ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT prgid_un UNIQUE (prgid)
)
WITH (
  OIDS=TRUE
);
ALTER TABLE planejamento.programa OWNER TO postgres;
GRANT ALL ON TABLE planejamento.programa TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planejamento.programa TO usr_simec;

GRANT ALL ON SEQUENCE planejamento.programa_prgid_seq TO postgres;
GRANT SELECT, USAGE ON SEQUENCE planejamento.programa_prgid_seq TO usr_simec;

-- DROP INDEX planejamento.orgcodprg_fk;
CREATE INDEX orgcodprg_fk
  ON planejamento.programa
  USING btree
  (orgcod COLLATE pg_catalog."default", organo COLLATE pg_catalog."default");

-- DROP INDEX planejamento.orgcodprg_fk_mn;
CREATE INDEX orgcodprg_fk_mn
  ON planejamento.programa
  USING btree
  (orgcod COLLATE pg_catalog."default", organo COLLATE pg_catalog."default");

-- DROP INDEX planejamento.prgid_ind;
CREATE UNIQUE INDEX prgid_ind
  ON planejamento.programa
  USING btree
  (prgid);

-- DROP INDEX planejamento.prgid_ind_mn;
CREATE UNIQUE INDEX prgid_ind_mn
  ON planejamento.programa
  USING btree
  (prgid);

-- DROP INDEX planejamento.tpocodprg_fk;
CREATE INDEX tpocodprg_fk
  ON planejamento.programa
  USING btree
  (tpocod COLLATE pg_catalog."default");

-- DROP INDEX planejamento.tpocodprg_fk_mn;
CREATE INDEX tpocodprg_fk_mn
  ON planejamento.programa
  USING btree
  (tpocod COLLATE pg_catalog."default");

-- DROP INDEX planejamento.tprcodprg_fk_mn;
CREATE INDEX tprcodprg_fk_mn
  ON planejamento.programa
  USING btree
  (tprcod COLLATE pg_catalog."default");

-- DROP TABLE planejamento.acao; -- antiga monitora.acao
CREATE TABLE planejamento.acao
(
  acaid serial NOT NULL,
  prgid integer,
  acacod character(4) NOT NULL,
  acadsc character varying(255),
  saccod character(4) NOT NULL,
  loccod character(4) NOT NULL,
  esfcod integer,
  unicod character(5),
  unitpocod character(1),
  funcod character(2),
  fundsc character varying(100),
  sfucod character(3),
  sfundsc character varying(100),
  prgano character(4),
  prgcod character(4),
  sacdsc character varying(1000),
  acasnmetanaocumulativa boolean,
  acadscsituacaoatual text,
  acasnrap boolean,
  acadescricao text,
  acabaselegal text,
  acasntransfoutras boolean,
  acadetalhamento text,
  acastatus character(1) DEFAULT 'A'::bpchar,
  acasnestrategica boolean DEFAULT false,
  acasnbgu boolean DEFAULT false,
  acadataatualizacao date DEFAULT ('now'::text)::timestamp(6) without time zone,
  irpcod character(1),
  acatipoinclusao character varying(1),
  acatipoinclusaolocalizador character varying(1),
  descricaomomento character varying(1000),
  acatitulo character varying(500),
  ididentificadorunicosiop character varying(500),
  acainiciativacod character varying(5),
  acainiciativadsc character varying(1000),
  acaobjetivocod character varying(5),
  acaobjetivodsc character varying(1000),
  prgdsc character varying(300),
  codtipoinclusaoacao integer,
  codtipoinclusaolocalizador integer,
  locquantidadeproposta numeric(15,2), -- Quantidade informada para o Localizador na tela de Proposta Orçamentária
  acanomeproduto character varying(200), -- Nome do Produto
  acanomeunidade character varying(200), -- Nome da Unidade
  CONSTRAINT pk_acao_mn PRIMARY KEY (acaid, acacod, saccod, loccod),
  CONSTRAINT fk_acao_esfcodaca_esfera FOREIGN KEY (esfcod)
      REFERENCES public.esfera (esfcod) MATCH SIMPLE
      ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT fk_acao_funcodaca_funcao FOREIGN KEY (funcod)
      REFERENCES public.ppafuncao (funcod) MATCH SIMPLE
      ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT fk_acao_identresultprimario FOREIGN KEY (irpcod)
      REFERENCES public.identresultprimario (irpcod) MATCH SIMPLE
      ON UPDATE RESTRICT ON DELETE RESTRICT,
  CONSTRAINT fk_acao_prgcodaca_programa FOREIGN KEY (prgano, prgcod, prgid)
      REFERENCES planejamento.programa (prgano, prgcod, prgid) MATCH SIMPLE
      ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT fk_acao_unicodaca_unidade FOREIGN KEY (unicod, unitpocod)
      REFERENCES public.unidade (unicod, unitpocod) MATCH SIMPLE
      ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT sfucod FOREIGN KEY (sfucod)
      REFERENCES public.ppasubfuncao (sfucod) MATCH SIMPLE
      ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT acao_acaid_unique UNIQUE (acaid)
)
WITH (
  OIDS=TRUE
);
ALTER TABLE planejamento.acao OWNER TO postgres;
GRANT ALL ON TABLE planejamento.acao TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planejamento.acao TO usr_simec;
COMMENT ON COLUMN planejamento.acao.locquantidadeproposta IS 'Quantidade informada para o Localizador na tela de Proposta Orçamentária';
COMMENT ON COLUMN planejamento.acao.acanomeproduto IS 'Nome do Produto';
COMMENT ON COLUMN planejamento.acao.acanomeunidade IS 'Nome da Unidade';

GRANT ALL ON SEQUENCE planejamento.acao_acaid_seq TO postgres;
GRANT SELECT, USAGE ON SEQUENCE planejamento.acao_acaid_seq TO usr_simec;

-- DROP INDEX planejamento.acacod_ind;
CREATE INDEX acacod_ind
  ON planejamento.acao
  USING btree
  (acacod COLLATE pg_catalog."default");

-- DROP INDEX planejamento.acaid_ind;
CREATE INDEX acaid_ind
  ON planejamento.acao
  USING btree
  (acaid);

-- DROP INDEX planejamento.esfcodaca_fk;
CREATE INDEX esfcodaca_fk
  ON planejamento.acao
  USING btree
  (esfcod);

-- DROP INDEX planejamento.funcodaca_fk;
CREATE INDEX funcodaca_fk
  ON planejamento.acao
  USING btree
  (funcod COLLATE pg_catalog."default");

-- DROP INDEX planejamento.idx_acao_01;
CREATE INDEX idx_acao_01
  ON planejamento.acao
  USING btree
  (prgcod COLLATE pg_catalog."default", acacod COLLATE pg_catalog."default", unicod COLLATE pg_catalog."default", loccod COLLATE pg_catalog."default");

-- DROP INDEX planejamento.idx_prgano;
CREATE INDEX idx_prgano
  ON planejamento.acao
  USING btree
  (prgano COLLATE pg_catalog."default");

-- DROP INDEX planejamento.ix_acao_irpcod;
CREATE INDEX ix_acao_irpcod
  ON planejamento.acao
  USING btree
  (irpcod COLLATE pg_catalog."default");

-- DROP INDEX planejamento.ix_acao_sfucod;
CREATE INDEX ix_acao_sfucod
  ON planejamento.acao
  USING btree
  (sfucod COLLATE pg_catalog."default");

-- DROP INDEX planejamento.ix_planejamento_acao_01;
CREATE INDEX ix_planejamento_acao_01
  ON planejamento.acao
  USING btree
  (prgcod COLLATE pg_catalog."default", acacod COLLATE pg_catalog."default", unicod COLLATE pg_catalog."default", loccod COLLATE pg_catalog."default");

-- DROP INDEX planejamento.prgcodacao_fk;
CREATE INDEX prgcodacao_fk
  ON planejamento.acao
  USING btree
  (prgano COLLATE pg_catalog."default", prgcod COLLATE pg_catalog."default", prgid);

-- DROP INDEX planejamento.unicodaca_fk;
CREATE INDEX unicodaca_fk
  ON planejamento.acao
  USING btree
  (unicod COLLATE pg_catalog."default", unitpocod COLLATE pg_catalog."default");

-- DROP TABLE planejamento.enquadramento_despesa; -- antiga monitora.pi_enquadramentodespesa
CREATE TABLE planejamento.enquadramento_despesa
(
  eqdid serial NOT NULL,
  tpdid integer, -- Tipo de Detalhamento/Grupo
  eqdcod character varying(4),
  eqddsc text,
  eqdano character(4),
  eqdstatus character(1) DEFAULT 'A'::bpchar,
  eqdstreduzido character(1) NOT NULL DEFAULT 'N'::bpchar,
  CONSTRAINT pk_enquadramento_despesa_eqdid PRIMARY KEY (eqdid),
  CONSTRAINT fk_enquadramento_despesa_tpdid FOREIGN KEY (tpdid)
      REFERENCES proposta.tipodetalhamento (tpdid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
)
WITH (
  OIDS=TRUE
);
ALTER TABLE planejamento.enquadramento_despesa OWNER TO postgres;
GRANT ALL ON TABLE planejamento.enquadramento_despesa TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planejamento.enquadramento_despesa TO usr_simec;
COMMENT ON COLUMN planejamento.enquadramento_despesa.tpdid IS 'Tipo de Detalhamento/Grupo';

GRANT ALL ON SEQUENCE planejamento.enquadramento_despesa_eqdid_seq TO postgres;
GRANT SELECT, USAGE ON SEQUENCE planejamento.enquadramento_despesa_eqdid_seq TO usr_simec;

-- DROP INDEX planejamento.ix_eqdid;
CREATE UNIQUE INDEX ix_eqdid
  ON planejamento.enquadramento_despesa
  USING btree
  (eqdid);

-- DROP INDEX planejamento.ix_enquadramentodespesa_tpdid;
CREATE INDEX ix_enquadramentodespesa_tpdid
  ON planejamento.enquadramento_despesa
  USING btree
  (tpdid);


-- DROP TABLE planejamento.enquadramentorp; antiga monitora.enquadramentorp
CREATE TABLE planejamento.enquadramentorp
(
  erpid serial NOT NULL, -- Chave Primária
  eqdid integer NOT NULL, -- Enquadramento
  irpcod character(1) NOT NULL, -- RP - Resultado Primário
  CONSTRAINT pk_enquadramentorp_erpid PRIMARY KEY (erpid),
  CONSTRAINT fk_enquadramento_despesa_eqdid FOREIGN KEY (eqdid)
      REFERENCES planejamento.enquadramento_despesa (eqdid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT fk_identresultprimario_irpcod FOREIGN KEY (irpcod)
      REFERENCES public.identresultprimario (irpcod) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
)
WITH (
  OIDS=FALSE
);
ALTER TABLE planejamento.enquadramentorp OWNER TO postgres;
GRANT ALL ON TABLE planejamento.enquadramentorp TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planejamento.enquadramentorp TO usr_simec;
COMMENT ON TABLE planejamento.enquadramentorp IS 'Vínculo de Enquadramento com RP - Resultado Primária';
COMMENT ON COLUMN planejamento.enquadramentorp.erpid IS 'Chave Primária';
COMMENT ON COLUMN planejamento.enquadramentorp.eqdid IS 'Enquadramento';
COMMENT ON COLUMN planejamento.enquadramentorp.irpcod IS 'RP - Resultado Primário';

GRANT ALL ON SEQUENCE planejamento.enquadramentorp_erpid_seq TO postgres;
GRANT SELECT, USAGE ON SEQUENCE planejamento.enquadramentorp_erpid_seq TO usr_simec;

-- DROP TABLE planejamento.categoria_apropriacao;  antiga monitora.pi_categoriaapropriacao
CREATE TABLE planejamento.categoria_apropriacao
(
  capid serial NOT NULL,
  capcod character varying(2),
  capdsc character varying(250),
  capano character(4),
  capstatus character(1) DEFAULT 'A'::bpchar,
  capsiconv boolean, -- Informa se pode ter números de proposta SICONV
  CONSTRAINT pk_categoria_apropriacao_capid PRIMARY KEY (capid)
)
WITH (
  OIDS=TRUE
);
ALTER TABLE planejamento.categoria_apropriacao OWNER TO postgres;
GRANT ALL ON TABLE planejamento.categoria_apropriacao TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planejamento.categoria_apropriacao TO usr_simec;
COMMENT ON COLUMN planejamento.categoria_apropriacao.capsiconv IS 'Informa se pode ter números de proposta SICONV';

GRANT ALL ON SEQUENCE planejamento.categoria_apropriacao_capid_seq TO postgres;
GRANT SELECT, USAGE ON SEQUENCE planejamento.categoria_apropriacao_capid_seq TO usr_simec;

-- DROP INDEX planejamento.index_21;
CREATE UNIQUE INDEX index_21
  ON planejamento.categoria_apropriacao
  USING btree
  (capid);


/*
 Mudado o nome da tabela de Area Cultural de monitora.pi_modalidadeensino para planejamento.area_cultural

 MODALIDADE DE ENSINO => Área Cultural     | Antes => mdeid | Depois => arceid | Antes => monitora.pi_modalidadeensino | Depois => planejamento.area_cultural
 NIVEL ETAPA ENSINO   => Segmento Cultural | Antes => neeid | Depois => secid  | Antes => monitora.pi_niveletapaensino  | Depois => planejamento.segmento_cultural
*/

-- DROP TABLE planejamento.area_cultural; -- antiga
CREATE TABLE planejamento.area_cultural
(
  arceid serial NOT NULL,
  arccod character varying(1),
  arcdsc character varying(250),
  arcano character(4),
  arcstatus character(1) DEFAULT 'A'::bpchar,
  CONSTRAINT pk_area_cultural PRIMARY KEY (arceid)
)
WITH (
  OIDS=TRUE
);
ALTER TABLE planejamento.area_cultural OWNER TO postgres;
GRANT ALL ON TABLE planejamento.area_cultural TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planejamento.area_cultural TO usr_simec;

GRANT ALL ON SEQUENCE planejamento.area_cultural_arceid_seq TO postgres;
GRANT SELECT, USAGE ON SEQUENCE planejamento.area_cultural_arceid_seq TO usr_simec;

-- DROP INDEX planejamento.index_1;
CREATE UNIQUE INDEX index_1
  ON planejamento.area_cultural
  USING btree
  (arceid);

/*
 Mudado o nome da tabela de Segmento Cultural de monitora.pi_niveletapaensino para planejamento.segmento_cultural

 MODALIDADE DE ENSINO => Área Cultural     | Antes => mdeid | Depois => arceid |
 NIVEL ETAPA ENSINO   => Segmento Cultural | Antes => neeid | Depois => secid  |

 Antes => monitora.pi_modalidadeensino  | Depois => planejamento.area_cultural
 Antes => monitora.pi_niveletapaensino  | Depois => planejamento.segmento_cultural

*/
-- DROP TABLE planejamento.segmento_cultural;
CREATE TABLE planejamento.segmento_cultural
(
  secid serial NOT NULL,
  arceid integer NOT NULL,
  secdsc character varying(250),
  secano character(4),
  secstatus character(1) DEFAULT 'A'::bpchar,
  seccod character varying(10),
  CONSTRAINT pk_segmento_cultural_secid PRIMARY KEY (secid),
  CONSTRAINT fk_area_cultural_arceid FOREIGN KEY (arceid)
      REFERENCES planejamento.area_cultural (arceid) MATCH SIMPLE
      ON UPDATE CASCADE ON DELETE RESTRICT
)
WITH (
  OIDS=TRUE
);
ALTER TABLE planejamento.segmento_cultural OWNER TO postgres;
GRANT ALL ON TABLE planejamento.segmento_cultural TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planejamento.segmento_cultural TO usr_simec;
COMMENT ON COLUMN planejamento.segmento_cultural.arceid IS 'Relação com a tabela de Área Cultural. planejamento.area_cultural';

GRANT ALL ON SEQUENCE planejamento.segmento_cultural_secid_seq TO postgres;
GRANT SELECT, USAGE ON SEQUENCE planejamento.segmento_cultural_secid_seq TO usr_simec;

-- DROP INDEX planejamento.index_4;
CREATE UNIQUE INDEX index_4
  ON planejamento.segmento_cultural
  USING btree
  (secid);

-- DROP TABLE planejamento.ptres;
CREATE TABLE planejamento.ptres
(
  ptrid serial NOT NULL,
  acaid integer,
  ptres character varying(50) NOT NULL,
  ptrano character varying(4),
  funcod character varying(2),
  sfucod character varying(3),
  prgcod character varying(4),
  acacod character varying(4),
  loccod character varying(4),
  unicod character varying(5),
  irpcod character varying(1),
  ptrdotacao numeric,
  ptrstatus character(1) DEFAULT 'A'::bpchar,
  ptrdata timestamp without time zone DEFAULT now(),
  plocod character(4),
  esfcod character(2),
  ptrdotacaocapital double precision, -- Dotação de Capital
  ptrdotacaocusteio double precision, -- Dotação de Custeio
  plodsc character varying(500), -- Título do Plano Orçamentário
  ptrnomeprodutopo character varying(200), -- Nome do Produto PO
  ptrnomeunidadepo character varying(200), -- Nome da Unidade PO
  ptrdotacaoinicialcapital numeric(20,0),
  ptrdotacaoinicialcusteio numeric(20,0),
  CONSTRAINT ptres_pkey PRIMARY KEY (ptrid),
  CONSTRAINT ptres_acaid_fkey FOREIGN KEY (acaid)
      REFERENCES planejamento.acao (acaid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
)
WITH (
  OIDS=FALSE
);
ALTER TABLE planejamento.ptres OWNER TO postgres;
GRANT ALL ON TABLE planejamento.ptres TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planejamento.ptres TO usr_simec;
COMMENT ON COLUMN planejamento.ptres.ptrdotacaocapital IS 'Dotação de Capital';
COMMENT ON COLUMN planejamento.ptres.ptrdotacaocusteio IS 'Dotação de Custeio';
COMMENT ON COLUMN planejamento.ptres.plodsc IS 'Título do Plano Orçamentário';
COMMENT ON COLUMN planejamento.ptres.ptrnomeprodutopo IS 'Nome do Produto PO';
COMMENT ON COLUMN planejamento.ptres.ptrnomeunidadepo IS 'Nome da Unidade PO';

GRANT ALL ON SEQUENCE planejamento.ptres_ptrid_seq TO postgres;
GRANT SELECT, USAGE ON SEQUENCE planejamento.ptres_ptrid_seq TO usr_simec;

-- DROP INDEX planejamento.ix_ptres_acaid;
CREATE INDEX ix_ptres_acaid
  ON planejamento.ptres
  USING btree
  (acaid);

-- DROP INDEX planejamento.ix_ptres_ptres;
CREATE INDEX ix_ptres_ptres
  ON planejamento.ptres
  USING btree
  (ptres COLLATE pg_catalog."default");

-- DROP TABLE planejamento.plano_interno;
CREATE TABLE planejamento.plano_interno
(
  pliid serial NOT NULL,
  suoid integer,
  ptrid integer,
  eqdid integer,
  capid integer,
  plisituacao character(1) DEFAULT 'P'::character varying,
  plititulo character varying(250),
  plidata timestamp with time zone DEFAULT now(),
  plistatus character(1) DEFAULT 'A'::character varying,
  plicodsubacao character(4),
  plicod character varying(15),
  plilivre character varying(3),
  plidsc character varying(1000),
  usucpf character varying(11),
  pliano character varying(4),
  plicadsiafi boolean,
  docid integer, -- Número do Documento do WorkFlow
  pliemenda boolean DEFAULT false, -- Indica se a PI veio de Emenda ou não
  plirecursosnecessarios character varying(1000),
  CONSTRAINT pk_plano_interno_pliid PRIMARY KEY (pliid),
  CONSTRAINT fk_categoria_apropriacao_capid FOREIGN KEY (capid)
      REFERENCES planejamento.categoria_apropriacao (capid) MATCH SIMPLE
      ON UPDATE RESTRICT ON DELETE RESTRICT,
  CONSTRAINT fk_subunidadeorcamentaria_suoid FOREIGN KEY (suoid)
      REFERENCES public.subunidadeorcamentaria (suoid) MATCH SIMPLE
      ON UPDATE RESTRICT ON DELETE RESTRICT,
  CONSTRAINT fk_ptres_ptrid FOREIGN KEY (ptrid)
      REFERENCES planejamento.ptres (ptrid) MATCH SIMPLE
      ON UPDATE RESTRICT ON DELETE RESTRICT,
  CONSTRAINT fk_enquadramento_despesa_eqdid FOREIGN KEY (eqdid)
      REFERENCES planejamento.enquadramento_despesa (eqdid) MATCH SIMPLE
      ON UPDATE RESTRICT ON DELETE RESTRICT,
  CONSTRAINT fk_pi_plano_reference_usuario FOREIGN KEY (usucpf)
      REFERENCES seguranca.usuario (usucpf) MATCH SIMPLE
      ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT fk_pi_plano_interno_docid FOREIGN KEY (docid)
      REFERENCES workflow.documento (docid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
)
WITH (
  OIDS=TRUE
);
ALTER TABLE planejamento.plano_interno OWNER TO postgres;
GRANT ALL ON TABLE planejamento.plano_interno TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planejamento.plano_interno TO usr_simec;
COMMENT ON COLUMN planejamento.plano_interno.docid IS 'Número do Documento do WorkFlow';
COMMENT ON COLUMN planejamento.plano_interno.pliemenda IS 'Indica se a PI veio de Emenda ou não';

GRANT ALL ON SEQUENCE planejamento.plano_interno_pliid_seq TO postgres;
GRANT SELECT, USAGE ON SEQUENCE planejamento.plano_interno_pliid_seq TO usr_simec;

-- DROP INDEX planejamento.idx_pliid_docid;
CREATE INDEX idx_pliid_docid
  ON planejamento.plano_interno
  USING btree
  (docid);

-- DROP INDEX planejamento.index_11;
CREATE INDEX index_11
  ON planejamento.plano_interno
  USING btree
  (capid);

-- DROP INDEX planejamento.index_5;
CREATE UNIQUE INDEX index_5
  ON planejamento.plano_interno
  USING btree
  (pliid);

-- DROP INDEX planejamento.index_8;
CREATE INDEX index_8
  ON planejamento.plano_interno
  USING btree
  (eqdid);

-- DROP INDEX planejamento.ix_pi_plano_interno_plicod;
CREATE INDEX ix_pi_plano_interno_plicod
  ON planejamento.plano_interno
  USING btree
  (plicod COLLATE pg_catalog."default");

-- DROP INDEX planejamento.ix_pi_plano_interno_usucpf;
CREATE INDEX ix_pi_plano_interno_usucpf
  ON planejamento.plano_interno
  USING btree
  (usucpf COLLATE pg_catalog."default");

-- DROP TABLE planejamento.produto;
CREATE TABLE planejamento.produto
(
  pprid serial NOT NULL,
  prsano character(4) NOT NULL,
  pprnome character varying(200) NOT NULL,
  pprdescricao character varying(500),
  pprstatus character(1) NOT NULL DEFAULT 'A'::bpchar,
  tipo "char", -- Tipo de dado para aplicar regra de négocio. N = Não se aplica e não necessita de dados complementares de medida e quantidade do produto.
  CONSTRAINT pk_produto_pprid PRIMARY KEY (pprid),
  CONSTRAINT ckc_pprstatus_pi_produto CHECK (pprstatus = ANY (ARRAY['I'::bpchar, 'A'::bpchar]))
)
WITH (
  OIDS=FALSE
);
ALTER TABLE planejamento.produto OWNER TO postgres;
GRANT ALL ON TABLE planejamento.produto TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planejamento.produto TO usr_simec;
COMMENT ON COLUMN planejamento.produto.tipo IS 'Tipo de dado para aplicar regra de négocio. N = Não se aplica e não necessita de dados complementares de medida e quantidade do produto.';

GRANT ALL ON SEQUENCE planejamento.produto_pprid_seq TO postgres;
GRANT SELECT, USAGE ON SEQUENCE planejamento.produto_pprid_seq TO usr_simec;

-- DROP TABLE planejamento.executor; -- Antiga monitora.pi_executor
CREATE TABLE planejamento.executor
(
  pieid serial NOT NULL,
  pieano character(4),
  piestatus character(1) DEFAULT 'A'::bpchar,
  piecod character varying(1),
  piedsc character varying(250),
  CONSTRAINT pk_pi_executor_pieid PRIMARY KEY (pieid)
) WITH (OIDS=TRUE);
ALTER TABLE planejamento.executor OWNER TO postgres;
GRANT ALL ON TABLE planejamento.executor TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planejamento.executor TO usr_simec;

GRANT ALL ON SEQUENCE planejamento.executor_pieid_seq TO postgres;
GRANT SELECT, USAGE ON SEQUENCE planejamento.executor_pieid_seq TO usr_simec;


-- DROP TABLE planejamento.gestor; -- Antiga monitora.pi_gestor
CREATE TABLE planejamento.gestor
(
  pigid serial NOT NULL,
  pigano character(4),
  pigstatus character(1) DEFAULT 'A'::bpchar,
  pigcod character varying(1),
  pigdsc character varying(250),
  CONSTRAINT pk_gestor_pigid PRIMARY KEY (pigid)
) WITH (OIDS=TRUE);
ALTER TABLE planejamento.gestor OWNER TO postgres;
GRANT ALL ON TABLE planejamento.gestor TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planejamento.gestor TO usr_simec;

GRANT ALL ON SEQUENCE planejamento.gestor_pigid_seq TO postgres;
GRANT SELECT, USAGE ON SEQUENCE planejamento.gestor_pigid_seq TO usr_simec;

-- DROP TABLE planejamento.unidade_medida;
CREATE TABLE planejamento.unidade_medida
(
  pumid serial NOT NULL,
  prsano character(4) NOT NULL,
  pumnome character varying(100) NOT NULL,
  pumdescricao character varying(200),
  pumstatus character(1) NOT NULL DEFAULT 'A'::bpchar,
  CONSTRAINT pk_unidade_medida PRIMARY KEY (pumid),
  CONSTRAINT ckc_unidade_medida CHECK (pumstatus = ANY (ARRAY['I'::bpchar, 'A'::bpchar]))
)
WITH (
  OIDS=FALSE
);
ALTER TABLE planejamento.unidade_medida OWNER TO postgres;
GRANT ALL ON TABLE planejamento.unidade_medida TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planejamento.unidade_medida TO usr_simec;


GRANT ALL ON SEQUENCE planejamento.unidade_medida_pumid_seq TO postgres;
GRANT SELECT, USAGE ON SEQUENCE planejamento.unidade_medida_pumid_seq TO usr_simec;

-- DROP TABLE planejamento.plano_orcamentario;
CREATE TABLE planejamento.plano_orcamentario
(
  ploid serial NOT NULL,
  prgcod character varying(5),
  acacod character varying(5),
  unicod character varying(5),
  plocodigo character varying(5),
  ploidentificadorunicosiop character varying(7), -- ID que será enviado para o SIOP
  plotitulo character varying(250),
  plodetalhamento text,
  ploproduto character varying(250),
  plounidademedida character varying(250),
  ploobrigatorio boolean,
  plostatus character(1) DEFAULT 'A'::bpchar,
  acaid integer,
  exercicio character varying(4),
  prddsc character varying(200),
  unmdsc character varying(200),
  metafisica integer,
  CONSTRAINT pk_planoorcamentarioloa PRIMARY KEY (ploid)
)
WITH (
  OIDS=FALSE
);
ALTER TABLE planejamento.plano_orcamentario OWNER TO postgres;
GRANT ALL ON TABLE planejamento.plano_orcamentario TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planejamento.plano_orcamentario TO usr_simec;
COMMENT ON COLUMN planejamento.plano_orcamentario.ploidentificadorunicosiop IS 'ID que será enviado para o SIOP';

GRANT ALL ON SEQUENCE planejamento.plano_orcamentario_ploid_seq TO postgres;
GRANT SELECT, USAGE ON SEQUENCE planejamento.plano_orcamentario_ploid_seq TO usr_simec;


-- DROP TABLE planejamento.unidade_medida_indicador;
CREATE TABLE planejamento.unidade_medida_indicador
(
  umiid serial NOT NULL,
  umidsc character(250),
  umistatus character(1) DEFAULT 'A'::bpchar,
  CONSTRAINT pk_unidade_medida_indicador_umiid PRIMARY KEY (umiid)
)
WITH (
  OIDS=TRUE
);
ALTER TABLE planejamento.unidade_medida_indicador OWNER TO postgres;
GRANT ALL ON TABLE planejamento.unidade_medida_indicador TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planejamento.unidade_medida_indicador TO usr_simec;


GRANT ALL ON SEQUENCE planejamento.unidade_medida_indicador_umiid_seq TO postgres;
GRANT SELECT, USAGE ON SEQUENCE planejamento.unidade_medida_indicador_umiid_seq TO usr_simec;

-- DROP INDEX planejamento.idx_unidade_medida_indicador_umiid;
CREATE UNIQUE INDEX idx_unidade_medida_indicador_umiid
  ON planejamento.unidade_medida_indicador
  USING btree
  (umiid);

/**

	ALTERAÇÕES DE SCHEMA DO PLANACOMORC PARA PLANEJAMENTO.

**/

-- DROP TABLE planejamento.programacao_exercicio; -- antiga planacomorc.programacaoexercicio
CREATE TABLE planejamento.programacao_exercicio
(
  prsano character(4) NOT NULL,
  prsdata_inicial date,
  prsdata_termino date,
  prsexerccorrente boolean DEFAULT false,
  prsstatus character(1) DEFAULT 'A'::bpchar,
  prsativo smallint DEFAULT 1,
  prsexercicioaberto boolean DEFAULT true,
  CONSTRAINT pk_programacao_exercicio_prsano PRIMARY KEY (prsano)
)
WITH (
  OIDS=FALSE
);
ALTER TABLE planejamento.programacao_exercicio OWNER TO postgres;
GRANT ALL ON TABLE planejamento.programacao_exercicio TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planejamento.programacao_exercicio TO usr_simec;

-- DROP TABLE planejamento.objetivo_estrategico;
CREATE TABLE planejamento.objetivo_estrategico
(
  obeid serial NOT NULL, -- Chave Primária
  obecod character varying(4) NOT NULL, -- Código do Objetivo Estratégico
  obenome character varying(200) NOT NULL, -- Nome do Objetivo Estratégico
  obecadastro timestamp without time zone NOT NULL DEFAULT now(), -- Data de cadastro do registro
  prsano character(4) NOT NULL, -- Exercício do ano corrido do registro
  obestatus character(1) NOT NULL DEFAULT 'A'::bpchar,
  CONSTRAINT pk_objetivo_estrategico_obeid PRIMARY KEY (obeid),
  CONSTRAINT fk_programacao_exercicio_prsano FOREIGN KEY (prsano)
      REFERENCES planejamento.programacao_exercicio (prsano) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
)
WITH (
  OIDS=FALSE
);
ALTER TABLE planejamento.objetivo_estrategico OWNER TO postgres;
GRANT ALL ON TABLE planejamento.objetivo_estrategico TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planejamento.objetivo_estrategico TO usr_simec;
COMMENT ON COLUMN planejamento.objetivo_estrategico.obeid IS 'Chave Primária';
COMMENT ON COLUMN planejamento.objetivo_estrategico.obecod IS 'Código do Objetivo Estratégico';
COMMENT ON COLUMN planejamento.objetivo_estrategico.obenome IS 'Nome do Objetivo Estratégico';
COMMENT ON COLUMN planejamento.objetivo_estrategico.obecadastro IS 'Data de cadastro do registro';
COMMENT ON COLUMN planejamento.objetivo_estrategico.prsano IS 'Exercício do ano corrido do registro';


GRANT ALL ON SEQUENCE planejamento.objetivo_estrategico_obeid_seq TO postgres;
GRANT SELECT, USAGE ON SEQUENCE planejamento.objetivo_estrategico_obeid_seq TO usr_simec;


-- DROP TABLE planejamento.diretriz_estrategica;
CREATE TABLE planejamento.diretriz_estrategica
(
  dieid serial NOT NULL, -- Chave Primária
  diecod character varying(4) NOT NULL, -- Código da Diretriz Estratégica
  dienome character varying(200) NOT NULL, -- Nome do Diretriz Estratégica
  diecadastro timestamp without time zone NOT NULL DEFAULT now(), -- Data de cadastro do registro
  prsano character(4) NOT NULL, -- Exercício do ano corrido do registro
  diestatus character(1) NOT NULL DEFAULT 'A'::bpchar,
  obeid integer NOT NULL,
  CONSTRAINT pk_diretriz_estrategica_dieid PRIMARY KEY (dieid),
  CONSTRAINT fk_objetivo_estrategico_obeid FOREIGN KEY (obeid)
      REFERENCES planejamento.objetivo_estrategico (obeid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT fk_programacao_exercicio_prsano FOREIGN KEY (prsano)
      REFERENCES planejamento.programacao_exercicio (prsano) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
)
WITH (
  OIDS=FALSE
);
ALTER TABLE planejamento.diretriz_estrategica OWNER TO postgres;
GRANT ALL ON TABLE planejamento.diretriz_estrategica TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planejamento.diretriz_estrategica TO usr_simec;
COMMENT ON COLUMN planejamento.diretriz_estrategica.dieid IS 'Chave Primária';
COMMENT ON COLUMN planejamento.diretriz_estrategica.diecod IS 'Código da Diretriz Estratégica';
COMMENT ON COLUMN planejamento.diretriz_estrategica.dienome IS 'Nome do Diretriz Estratégica';
COMMENT ON COLUMN planejamento.diretriz_estrategica.diecadastro IS 'Data de cadastro do registro';
COMMENT ON COLUMN planejamento.diretriz_estrategica.prsano IS 'Exercício do ano corrido do registro';

GRANT ALL ON SEQUENCE planejamento.diretriz_estrategica_dieid_seq TO postgres;
GRANT SELECT, USAGE ON SEQUENCE planejamento.diretriz_estrategica_dieid_seq TO usr_simec;

-- DROP TABLE planejamento.meta_estrategica;
CREATE TABLE planejamento.meta_estrategica
(
  meeid serial NOT NULL, -- Chave Primária
  meecod character varying(4) NOT NULL, -- Código da Meta Estratégica
  meenome character varying(200) NOT NULL, -- Nome da Meta Estratégica
  meecadastro timestamp without time zone NOT NULL DEFAULT now(), -- Data de cadastro do registro
  prsano character(4) NOT NULL, -- Exercício do ano corrido do registro
  meestatus character(1) NOT NULL DEFAULT 'A'::bpchar,
  obeid integer NOT NULL, -- Chave Estrangeira de Objetivo Estratégico
  CONSTRAINT pk_meeid PRIMARY KEY (meeid),
  CONSTRAINT fk_objetivo_estrategico_obeid FOREIGN KEY (obeid)
      REFERENCES planejamento.objetivo_estrategico (obeid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT fk_programacao_exercicio_prsano FOREIGN KEY (prsano)
      REFERENCES planejamento.programacao_exercicio (prsano) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
)
WITH (
  OIDS=FALSE
);
ALTER TABLE planejamento.meta_estrategica OWNER TO postgres;
GRANT ALL ON TABLE planejamento.meta_estrategica TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planejamento.meta_estrategica TO usr_simec;
COMMENT ON COLUMN planejamento.meta_estrategica.meeid IS 'Chave Primária';
COMMENT ON COLUMN planejamento.meta_estrategica.meecod IS 'Código da Meta Estratégica';
COMMENT ON COLUMN planejamento.meta_estrategica.meenome IS 'Nome da Meta Estratégica';
COMMENT ON COLUMN planejamento.meta_estrategica.meecadastro IS 'Data de cadastro do registro';
COMMENT ON COLUMN planejamento.meta_estrategica.prsano IS 'Exercício do ano corrido do registro';
COMMENT ON COLUMN planejamento.meta_estrategica.obeid IS 'Chave Estrangeira de Objetivo Estratégico';

GRANT ALL ON SEQUENCE planejamento.meta_estrategica_meeid_seq TO postgres;
GRANT SELECT, USAGE ON SEQUENCE planejamento.meta_estrategica_meeid_seq TO usr_simec;


-- DROP TABLE planejamento.manutencao_item;
CREATE TABLE planejamento.manutencao_item
(
  maiid serial NOT NULL,
  eqdid integer,
  prsano character(4) NOT NULL,
  mainome character varying(200) NOT NULL,
  maidescricao character varying(500),
  maistatus character(1) NOT NULL DEFAULT 'A'::bpchar,
  CONSTRAINT pk_manutencao_item_maiid PRIMARY KEY (maiid),
  CONSTRAINT fk_enquadramento_despesa_eqdid FOREIGN KEY (eqdid)
      REFERENCES planejamento.enquadramento_despesa (eqdid) MATCH SIMPLE
      ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT ckc_manutencao_item_maistatus CHECK (maistatus = ANY (ARRAY['I'::bpchar, 'A'::bpchar]))
)
WITH (
  OIDS=FALSE
);
ALTER TABLE planejamento.manutencao_item OWNER TO postgres;
GRANT ALL ON TABLE planejamento.manutencao_item TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planejamento.manutencao_item TO usr_simec;

GRANT ALL ON SEQUENCE planejamento.manutencao_item_maiid_seq TO postgres;
GRANT SELECT, USAGE ON SEQUENCE planejamento.manutencao_item_maiid_seq TO usr_simec;

-- DROP TABLE planejamento.manutencao_subitem;
CREATE TABLE planejamento.manutencao_subitem
(
  masid serial NOT NULL,
  maiid integer NOT NULL,
  prsano character(4) NOT NULL,
  masnome character varying(200) NOT NULL,
  masdescricao character varying(500),
  masstatus character(1) NOT NULL DEFAULT 'A'::bpchar,
  CONSTRAINT pk_manutencao_subitem_masid PRIMARY KEY (masid),
  CONSTRAINT fk_manutencao_item_maiid FOREIGN KEY (maiid)
      REFERENCES planejamento.manutencao_item (maiid) MATCH SIMPLE
      ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT ckc_manutencao_subitem_masstatus CHECK (masstatus = ANY (ARRAY['I'::bpchar, 'A'::bpchar]))
) WITH (OIDS=FALSE);
ALTER TABLE planejamento.manutencao_subitem OWNER TO postgres;
GRANT ALL ON TABLE planejamento.manutencao_subitem TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planejamento.manutencao_subitem TO usr_simec;


GRANT ALL ON SEQUENCE planejamento.manutencao_subitem_masid_seq TO postgres;
GRANT SELECT, USAGE ON SEQUENCE planejamento.manutencao_subitem_masid_seq TO usr_simec;


-- DROP TABLE planejamento.complemento; -- antiga planacomorc.pi_complemento;
CREATE TABLE planejamento.complemento
(
  pliid integer NOT NULL, -- Identificador do Plano Interno - PI
  oppid integer, -- Identificador do Objetivo PPA
  mppid integer, -- Metas PPA
  ippid integer, -- Iniciativa PPA
  mpnid integer, -- Meta PNC
  ipnid integer, -- Indicador PNC
  pprid integer, -- Produto
  pumid integer, -- Unidade de Medida
  arceid integer, -- Área Cultural
  secid integer, -- Segmento Cultural
  picquantidade double precision, -- Quantidade do Produto
  picted boolean, -- TED
  picedital boolean, -- Edital
  esfid integer, -- Tipo de localização
  picvalorcusteio double precision, -- Valor do Custeio
  picvalorcapital double precision, -- Valor Capital
  mescod character(2), -- Código do Mês de Previsão do Lançamento do Edital
  maiid integer, -- Matutenção de Item
  masid integer, -- Manutenção de Sub Item
  picexecucao double precision, -- Valor do percentual de execução do projeto.
  picpublico character varying(1000),
  picpriorizacao integer, -- Grau de priorização para a unidade
  pijid integer, -- Janela
  ptaid integer, -- Plano de trabalho Anual - FNC
  prgid integer,
  obeid integer,
  meeid integer,
  dieid integer,
  CONSTRAINT pi_complemento_pkey PRIMARY KEY (pliid),
  CONSTRAINT fk_complemento_ipnid FOREIGN KEY (ipnid)
      REFERENCES public.indicadorpnc (ipnid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT fk_iniciativappa_ippid FOREIGN KEY (ippid)
      REFERENCES public.iniciativappa (ippid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT fk_metapnc_mpnid FOREIGN KEY (mpnid)
      REFERENCES public.metapnc (mpnid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT fk_metappa_mppid FOREIGN KEY (mppid)
      REFERENCES public.metappa (mppid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT fk_objetivoppa_oppid FOREIGN KEY (oppid)
      REFERENCES public.objetivoppa (oppid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT fk_diretriz_estrategica_dieid FOREIGN KEY (dieid)
      REFERENCES planejamento.diretriz_estrategica (dieid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT fk_meta_estrategica_meeid FOREIGN KEY (meeid)
      REFERENCES planejamento.meta_estrategica (meeid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT fk_objetivo_estrategico_obeid FOREIGN KEY (obeid)
      REFERENCES planejamento.objetivo_estrategico (obeid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT fk_esfera_esfid FOREIGN KEY (esfid)
      REFERENCES territorios.esfera (esfid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT fk_meses_mescod FOREIGN KEY (mescod)
      REFERENCES public.meses (mescod) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT fk_plano_interno_pliid FOREIGN KEY (pliid)
      REFERENCES planejamento.plano_interno (pliid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT fk_produto_pprid FOREIGN KEY (pprid)
      REFERENCES planejamento.produto (pprid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT fk_planejamento_pumid FOREIGN KEY (pumid)
      REFERENCES planejamento.unidade_medida (pumid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT fk_manutencao_item_maiid FOREIGN KEY (maiid)
      REFERENCES planejamento.manutencao_item (maiid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT fk_picid_masid FOREIGN KEY (masid)
      REFERENCES planejamento.manutencao_subitem (masid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT fk_area_cultural_arceid FOREIGN KEY (arceid)
      REFERENCES planejamento.area_cultural (arceid) MATCH SIMPLE
      ON UPDATE RESTRICT ON DELETE RESTRICT,
  CONSTRAINT fk_segmento_cultural_secid FOREIGN KEY (secid)
      REFERENCES planejamento.segmento_cultural (secid) MATCH SIMPLE
      ON UPDATE RESTRICT ON DELETE RESTRICT,
  CONSTRAINT fk_planotrabalhoanual_ptaid FOREIGN KEY (ptaid)
      REFERENCES spo.planotrabalhoanual (ptaid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
)
WITH (
  OIDS=FALSE
);
ALTER TABLE planejamento.complemento OWNER TO postgres;
GRANT ALL ON TABLE planejamento.complemento TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planejamento.complemento TO usr_simec;
COMMENT ON TABLE planejamento.complemento IS 'Dados complementares da tabela planejamento.plano_interno';
COMMENT ON COLUMN planejamento.complemento.pliid IS 'Identificador do Plano Interno - PI';
COMMENT ON COLUMN planejamento.complemento.oppid IS 'Identificador do Objetivo PPA';
COMMENT ON COLUMN planejamento.complemento.mppid IS 'Metas PPA';
COMMENT ON COLUMN planejamento.complemento.ippid IS 'Iniciativa PPA';
COMMENT ON COLUMN planejamento.complemento.mpnid IS 'Meta PNC';
COMMENT ON COLUMN planejamento.complemento.ipnid IS 'Indicador PNC';
COMMENT ON COLUMN planejamento.complemento.pprid IS 'Produto';
COMMENT ON COLUMN planejamento.complemento.pumid IS 'Unidade de Medida';
COMMENT ON COLUMN planejamento.complemento.picquantidade IS 'Quantidade do Produto';
COMMENT ON COLUMN planejamento.complemento.picted IS 'TED';
COMMENT ON COLUMN planejamento.complemento.picedital IS 'Edital';
COMMENT ON COLUMN planejamento.complemento.esfid IS 'Tipo de localização';
COMMENT ON COLUMN planejamento.complemento.picvalorcusteio IS 'Valor do Custeio';
COMMENT ON COLUMN planejamento.complemento.picvalorcapital IS 'Valor Capital';
COMMENT ON COLUMN planejamento.complemento.mescod IS 'Código do Mês de Previsão do Lançamento do Edital';
COMMENT ON COLUMN planejamento.complemento.maiid IS 'Matutenção de Item';
COMMENT ON COLUMN planejamento.complemento.masid IS 'Manutenção de Sub Item';
COMMENT ON COLUMN planejamento.complemento.picexecucao IS 'Valor do percentual de execução do projeto.';
COMMENT ON COLUMN planejamento.complemento.picpriorizacao IS 'Grau de priorização para a unidade';
COMMENT ON COLUMN planejamento.complemento.pijid IS 'Janela';
COMMENT ON COLUMN planejamento.complemento.ptaid IS 'Plano de trabalho Anual - FNC';


-- DROP TABLE planejamento.cronograma;
CREATE TABLE planejamento.cronograma
(
  croid serial NOT NULL, -- Chave Primária
  crodsc character varying(200) NOT NULL, -- Descrição
  CONSTRAINT pk_cronograma_croid PRIMARY KEY (croid)
)
WITH (
  OIDS=FALSE
);
ALTER TABLE planejamento.cronograma OWNER TO postgres;
GRANT ALL ON TABLE planejamento.cronograma TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planejamento.cronograma TO usr_simec;
COMMENT ON TABLE planejamento.cronograma IS 'Tabela de Cronogramas';
COMMENT ON COLUMN planejamento.cronograma.croid IS 'Chave Primária';
COMMENT ON COLUMN planejamento.cronograma.crodsc IS 'Descrição';


GRANT ALL ON SEQUENCE planejamento.cronograma_croid_seq TO postgres;
GRANT SELECT, USAGE ON SEQUENCE planejamento.cronograma_croid_seq TO usr_simec;


-- DROP TABLE planejamento.cronograma_valor;
CREATE TABLE planejamento.cronograma_valor
(
  crvid serial NOT NULL, -- Chave Primária
  crvdsc character varying(200) NOT NULL, -- Descrição
  croid integer NOT NULL, -- Identificador do Cronograma
  crvtipo character(3), -- Tipo de Valores:...
  CONSTRAINT pk_cronograma_valor_crvid PRIMARY KEY (crvid),
  CONSTRAINT fk_cronograma_valor_croid FOREIGN KEY (croid)
      REFERENCES planejamento.cronograma (croid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
)
WITH (
  OIDS=FALSE
);
ALTER TABLE planejamento.cronograma_valor OWNER TO postgres;
GRANT ALL ON TABLE planejamento.cronograma_valor TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planejamento.cronograma_valor TO usr_simec;
COMMENT ON TABLE planejamento.cronograma_valor IS 'Valores do Cronograma';
COMMENT ON COLUMN planejamento.cronograma_valor.crvid IS 'Chave Primária';
COMMENT ON COLUMN planejamento.cronograma_valor.crvdsc IS 'Descrição';
COMMENT ON COLUMN planejamento.cronograma_valor.croid IS 'Identificador do Cronograma';
COMMENT ON COLUMN planejamento.cronograma_valor.crvtipo IS 'Tipo de Valores:
CUS - Custeio
CAP - Capital
FIS - Físico';

GRANT ALL ON SEQUENCE planejamento.cronograma_valor_crvid_seq TO postgres;
GRANT SELECT, USAGE ON SEQUENCE planejamento.cronograma_valor_crvid_seq TO usr_simec;

-- DROP TABLE planejamento.esfera;
CREATE TABLE planejamento.esfera
(
  id_esfera serial NOT NULL,
  codigo integer, -- Código utilizado pela SOF
  descricao character varying(100),
  descricao_abreviada character varying(50), -- Descrição resumida da esfera
  instante_alteracao timestamp(6) without time zone,
  CONSTRAINT pk_planejamento_esfera PRIMARY KEY (id_esfera)
)
WITH (
  OIDS=FALSE
);
ALTER TABLE planejamento.esfera OWNER TO postgres;
GRANT ALL ON TABLE planejamento.esfera TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planejamento.esfera TO usr_simec;
COMMENT ON TABLE planejamento.esfera IS 'Origem dos dados: WSQualitativo -> obterTabelasApoio -> esferas';
COMMENT ON COLUMN planejamento.esfera.codigo IS 'Código utilizado pela SOF';
COMMENT ON COLUMN planejamento.esfera.descricao_abreviada IS 'Descrição resumida da esfera';

GRANT ALL ON SEQUENCE planejamento.esfera_id_esfera_seq TO postgres;
GRANT SELECT, USAGE ON SEQUENCE planejamento.esfera_id_esfera_seq TO usr_simec;

-- DROP TABLE planejamento.etapas;
CREATE TABLE planejamento.etapas
(
  etaid serial NOT NULL, -- Chave Primária
  pliid integer NOT NULL, -- ID do PI
  etadsc character varying(250), -- Descrição da Etapa
  etadata date DEFAULT now(), -- Data da Etapa
  etastatus character(1) NOT NULL DEFAULT 'A'::bpchar,
  CONSTRAINT pk_etapas_etaid PRIMARY KEY (etaid),
  CONSTRAINT fk_plano_interno_pliid FOREIGN KEY (pliid)
      REFERENCES planejamento.plano_interno (pliid) MATCH SIMPLE
      ON UPDATE RESTRICT ON DELETE RESTRICT
)
WITH (
  OIDS=FALSE
);
ALTER TABLE planejamento.etapas OWNER TO postgres;
GRANT ALL ON TABLE planejamento.etapas TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planejamento.etapas TO usr_simec;
COMMENT ON COLUMN planejamento.etapas.etaid IS 'Chave Primária';
COMMENT ON COLUMN planejamento.etapas.etadsc IS 'Descrição da Etapa';
COMMENT ON COLUMN planejamento.etapas.etadata IS 'Data da Etapa';
COMMENT ON COLUMN planejamento.etapas.pliid IS 'ID do PI';

GRANT ALL ON SEQUENCE planejamento.etapas_etaid_seq TO postgres;
GRANT SELECT, USAGE ON SEQUENCE planejamento.etapas_etaid_seq TO usr_simec;


-- DROP TABLE planejamento.funcao;
CREATE TABLE planejamento.funcao
(
  id_funcao serial NOT NULL,
  instante_alteracao timestamp(6) without time zone DEFAULT now(),
  descricao character varying(100), -- Descrição completa da função
  descricao_abreviada character varying(50), -- Descrição resumida da função
  codigo character(2), -- Código da SOF para o Ptres
  CONSTRAINT pk_funcao_id_funcao PRIMARY KEY (id_funcao)
)
WITH (
  OIDS=FALSE
);
ALTER TABLE planejamento.funcao OWNER TO postgres;
GRANT ALL ON TABLE planejamento.funcao TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planejamento.funcao TO usr_simec;
COMMENT ON TABLE planejamento.funcao IS 'Origem dos dados: WSQualitativo -> obterTabelasApoio-> funcoes';
COMMENT ON COLUMN planejamento.funcao.descricao IS 'Descrição completa da função';
COMMENT ON COLUMN planejamento.funcao.descricao_abreviada IS 'Descrição resumida da função';
COMMENT ON COLUMN planejamento.funcao.codigo IS 'Código da SOF para o Ptres';

GRANT ALL ON SEQUENCE planejamento.funcao_id_funcao_seq TO postgres;
GRANT SELECT, USAGE ON SEQUENCE planejamento.funcao_id_funcao_seq TO usr_simec;

-- DROP TABLE planejamento.historico_pi;
CREATE TABLE planejamento.historico_pi
(
  hpiid serial NOT NULL, -- Chave Primária
  pliid integer NOT NULL, -- Plano Interno
  usucpf character(11) NOT NULL, -- CPF do Usuário
  hisdata timestamp without time zone NOT NULL DEFAULT now(), -- Data da Modificação
  hpidscantigo character(15) NOT NULL, -- Nº do PI antigo
  hpidscnovo character(15) NOT NULL, -- Nº do PI novo
  CONSTRAINT pk_historico_pi_hpiid PRIMARY KEY (hpiid),
  CONSTRAINT fk_historico_pi_usucpf FOREIGN KEY (usucpf)
      REFERENCES seguranca.usuario (usucpf) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT fk_plano_interno_pliid FOREIGN KEY (pliid)
      REFERENCES planejamento.plano_interno (pliid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
)
WITH (
  OIDS=FALSE
);
ALTER TABLE planejamento.historico_pi OWNER TO postgres;
GRANT ALL ON TABLE planejamento.historico_pi TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planejamento.historico_pi TO usr_simec;
COMMENT ON TABLE planejamento.historico_pi IS 'Histórico do PI';
COMMENT ON COLUMN planejamento.historico_pi.hpiid IS 'Chave Primária';
COMMENT ON COLUMN planejamento.historico_pi.pliid IS 'Plano Interno';
COMMENT ON COLUMN planejamento.historico_pi.usucpf IS 'CPF do Usuário';
COMMENT ON COLUMN planejamento.historico_pi.hisdata IS 'Data da Modificação';
COMMENT ON COLUMN planejamento.historico_pi.hpidscantigo IS 'Nº do PI antigo';
COMMENT ON COLUMN planejamento.historico_pi.hpidscnovo IS 'Nº do PI novo';

GRANT ALL ON SEQUENCE planejamento.historico_pi_hpiid_seq TO postgres;
GRANT SELECT, USAGE ON SEQUENCE planejamento.historico_pi_hpiid_seq TO usr_simec;

-- DROP TABLE planejamento.historico_pi_usuario;
CREATE TABLE planejamento.historico_pi_usuario
(
  hpuid serial NOT NULL, -- Chave Primária
  pliid integer NOT NULL, -- Plano Interno
  usucpf character(11) NOT NULL, -- CPF do Usuário
  hpudata timestamp without time zone NOT NULL DEFAULT now(), -- Data da Modificação
  CONSTRAINT pk_historico_pi_usuario_hpuid PRIMARY KEY (hpuid),
  CONSTRAINT fk_historico_pi_usuario_usucpf FOREIGN KEY (usucpf)
      REFERENCES seguranca.usuario (usucpf) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT fk_plano_interno_pliid FOREIGN KEY (pliid)
      REFERENCES planejamento.plano_interno (pliid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
)
WITH (
  OIDS=FALSE
);
ALTER TABLE planejamento.historico_pi_usuario OWNER TO postgres;
GRANT ALL ON TABLE planejamento.historico_pi_usuario TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planejamento.historico_pi_usuario TO usr_simec;
COMMENT ON TABLE planejamento.historico_pi_usuario IS 'Histórico do PI';
COMMENT ON COLUMN planejamento.historico_pi_usuario.hpuid IS 'Chave Primária';
COMMENT ON COLUMN planejamento.historico_pi_usuario.pliid IS 'Plano Interno';
COMMENT ON COLUMN planejamento.historico_pi_usuario.usucpf IS 'CPF do Usuário';
COMMENT ON COLUMN planejamento.historico_pi_usuario.hpudata IS 'Data da Modificação';


GRANT ALL ON SEQUENCE planejamento.historico_pi_usuario_hpuid_seq TO postgres;
GRANT SELECT, USAGE ON SEQUENCE planejamento.historico_pi_usuario_hpuid_seq TO usr_simec;

-- DROP TABLE planejamento.anexo; -- antigo planacomorc.pi_anexo;
CREATE TABLE planejamento.anexo
(
  piaid serial NOT NULL,
  arqid integer NOT NULL,
  pliid integer NOT NULL,
  piastatus character(1) NOT NULL DEFAULT 'A'::bpchar,
  CONSTRAINT pk_anexo_piaid PRIMARY KEY (piaid),
  CONSTRAINT fk_arquivo_arqid FOREIGN KEY (arqid)
      REFERENCES public.arquivo (arqid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT fk_plano_interno_pliid FOREIGN KEY (pliid)
      REFERENCES planejamento.plano_interno (pliid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
)
WITH (
  OIDS=FALSE
);
ALTER TABLE planejamento.anexo OWNER TO postgres;
GRANT ALL ON TABLE planejamento.anexo TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planejamento.anexo TO usr_simec;
COMMENT ON TABLE planejamento.historico_pi_usuario IS 'Anexos do Plano de Ação, antigo Plano Interno.';

GRANT ALL ON SEQUENCE planejamento.anexo_piaid_seq TO postgres;
GRANT SELECT, USAGE ON SEQUENCE planejamento.anexo_piaid_seq TO usr_simec;

-- DROP TABLE planejamento.convenio;
CREATE TABLE planejamento.convenio
(
  pcoid serial NOT NULL, -- Chave Primária
  pliid integer NOT NULL, -- Plano Interno - PI
  pcoconvenio bigint NOT NULL, -- Número do Convênio
  CONSTRAINT pk_convenio_pcoid PRIMARY KEY (pcoid),
  CONSTRAINT fk_plano_interno_pliid FOREIGN KEY (pliid)
      REFERENCES planejamento.plano_interno (pliid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
)
WITH (
  OIDS=FALSE
);
ALTER TABLE planejamento.convenio OWNER TO postgres;
GRANT ALL ON TABLE planejamento.convenio TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planejamento.convenio TO usr_simec;
COMMENT ON TABLE planejamento.convenio IS 'Vínculo do Plano Interno com Convênio';
COMMENT ON COLUMN planejamento.convenio.pcoid IS 'Chave Primária';
COMMENT ON COLUMN planejamento.convenio.pliid IS 'Plano Interno - PI';
COMMENT ON COLUMN planejamento.convenio.pcoconvenio IS 'Número do Convênio';

GRANT ALL ON SEQUENCE planejamento.convenio_pcoid_seq TO postgres;
GRANT SELECT, USAGE ON SEQUENCE planejamento.convenio_pcoid_seq TO usr_simec;




--- ==============================--- ==============================--- ==============================
--- ============================== Executado até aqui
--- ==============================--- ==============================--- ==============================


-- DROP TABLE planejamento.cronograma_plano_interno;  -- antiga planacomorc.pi_cronograma
CREATE TABLE planejamento.cronograma_plano_interno
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
ALTER TABLE planejamento.cronograma_plano_interno OWNER TO postgres;
GRANT ALL ON TABLE planejamento.cronograma_plano_interno TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planejamento.cronograma_plano_interno TO usr_simec;
COMMENT ON TABLE planejamento.cronograma_plano_interno IS 'Cronograma Físico/Financeiro do Plano Interno';
COMMENT ON COLUMN planejamento.cronograma_plano_interno.pcrid IS 'Chave Primária';
COMMENT ON COLUMN planejamento.cronograma_plano_interno.pliid IS 'Plano Interno - PI';
COMMENT ON COLUMN planejamento.cronograma_plano_interno.mescod IS 'Mês';
COMMENT ON COLUMN planejamento.cronograma_plano_interno.pcrvalor IS 'Valor do Cronograma';
COMMENT ON COLUMN planejamento.cronograma_plano_interno.crvid IS 'Identificador do Cronograma Valor';

GRANT ALL ON SEQUENCE planejamento.cronograma_plano_interno_pcrid_seq TO postgres;
GRANT SELECT, USAGE ON SEQUENCE planejamento.cronograma_plano_interno_pcrid_seq TO usr_simec;

-- DROP TABLE planejamento.delegacao;  -- antigo planacomorc.pi_delegacao
CREATE TABLE planejamento.delegacao
(
  pdeid serial NOT NULL, -- Chave Primária
  pliid integer NOT NULL, -- Plano Interno
  suoid integer NOT NULL, -- Sub-Unidade Orçamentária
  CONSTRAINT pk_delegacao_pdeid PRIMARY KEY (pdeid),
  CONSTRAINT fk_plano_interno_pliid FOREIGN KEY (pliid)
      REFERENCES planejamento.plano_interno (pliid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT fk_subunidadeorcamentaria_suoid FOREIGN KEY (suoid)
      REFERENCES public.subunidadeorcamentaria (suoid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
)
WITH (
  OIDS=FALSE
);
ALTER TABLE planejamento.delegacao OWNER TO postgres;
GRANT ALL ON TABLE planejamento.delegacao TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planejamento.delegacao TO usr_simec;
COMMENT ON TABLE planejamento.delegacao IS 'Delegações de Plano Interno';
COMMENT ON COLUMN planejamento.delegacao.pdeid IS 'Chave Primária';
COMMENT ON COLUMN planejamento.delegacao.pliid IS 'Plano Interno';
COMMENT ON COLUMN planejamento.delegacao.suoid IS 'Sub-Unidade Orçamentária';

GRANT ALL ON SEQUENCE planejamento.delegacao_pdeid_seq TO postgres;
GRANT SELECT, USAGE ON SEQUENCE planejamento.delegacao_pdeid_seq TO usr_simec;


-- DROP TABLE planejamento.janela_plano_interno;  -- antiga planacomorc.pi_janela
CREATE TABLE planejamento.janela_plano_interno
(
  pijid serial NOT NULL, -- Chave Primária
  pijdsc character varying(500) NOT NULL, -- Descrição
  pijinicio timestamp with time zone NOT NULL, -- Data início da Janela
  pijfim timestamp with time zone NOT NULL, -- Data fim da Janela
  pijcadastro timestamp with time zone NOT NULL DEFAULT now(), -- Data de cadastro
  usucpf character(11) NOT NULL, -- CPF do usuário que cadastrou a janela
  prsano character(4) NOT NULL, -- Ano de Referência
  pijstatus character(1) NOT NULL DEFAULT 'A'::bpchar, -- Status: A - Ativo / I - Inativo
  CONSTRAINT pk_janela_plano_interno_pijid PRIMARY KEY (pijid)
)
WITH (
  OIDS=FALSE
);
ALTER TABLE planejamento.janela_plano_interno OWNER TO postgres;
GRANT ALL ON TABLE planejamento.janela_plano_interno TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planejamento.janela_plano_interno TO usr_simec;
COMMENT ON TABLE planejamento.janela_plano_interno IS 'Janelas para cadastro de PI FNC';
COMMENT ON COLUMN planejamento.janela_plano_interno.pijid IS 'Chave Primária';
COMMENT ON COLUMN planejamento.janela_plano_interno.pijdsc IS 'Descrição';
COMMENT ON COLUMN planejamento.janela_plano_interno.pijinicio IS 'Data início da Janela';
COMMENT ON COLUMN planejamento.janela_plano_interno.pijfim IS 'Data fim da Janela';
COMMENT ON COLUMN planejamento.janela_plano_interno.pijcadastro IS 'Data de cadastro';
COMMENT ON COLUMN planejamento.janela_plano_interno.usucpf IS 'CPF do usuário que cadastrou a janela';
COMMENT ON COLUMN planejamento.janela_plano_interno.prsano IS 'Ano de Referência';
COMMENT ON COLUMN planejamento.janela_plano_interno.pijstatus IS 'Status: A - Ativo / I - Inativo';

GRANT ALL ON SEQUENCE planejamento.janela_plano_interno_pijid_seq TO postgres;
GRANT SELECT, USAGE ON SEQUENCE planejamento.janela_plano_interno_pijid_seq TO usr_simec;


-- DROP TABLE planejamento.localizacao;  -- antiga planacomorc.pi_localizacao
CREATE TABLE planejamento.localizacao
(
  pilid serial NOT NULL, -- Chave Primária
  pliid integer NOT NULL, -- Plano Interno - PI
  estuf character(2), -- UF
  muncod character(7), -- Código IBGE do Município
  paiid integer, -- País
  CONSTRAINT pk_localizacao_pilid PRIMARY KEY (pilid),
  CONSTRAINT fk_plano_interno_pliid FOREIGN KEY (pliid)
      REFERENCES planejamento.plano_interno (pliid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
)
WITH (
  OIDS=FALSE
);
ALTER TABLE planejamento.localizacao OWNER TO postgres;
GRANT ALL ON TABLE planejamento.localizacao TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planejamento.localizacao TO usr_simec;
COMMENT ON TABLE planejamento.localizacao IS 'Vínculo do Plano Interno com Localização do Projeto';
COMMENT ON COLUMN planejamento.localizacao.pilid IS 'Chave Primária';
COMMENT ON COLUMN planejamento.localizacao.pliid IS 'Plano Interno - PI';
COMMENT ON COLUMN planejamento.localizacao.estuf IS 'UF';
COMMENT ON COLUMN planejamento.localizacao.muncod IS 'Código IBGE do Município';
COMMENT ON COLUMN planejamento.localizacao.paiid IS 'País';


GRANT ALL ON SEQUENCE planejamento.localizacao_pilid_seq TO postgres;
GRANT SELECT, USAGE ON SEQUENCE planejamento.localizacao_pilid_seq TO usr_simec;

-- DROP TABLE planejamento.responsavel_plano_interno; -- antigo planacomorc.pi_responsavel
CREATE TABLE planejamento.responsavel_plano_interno
(
  pirid serial NOT NULL, -- Chave Primária
  pliid integer NOT NULL, -- Plano Interno - PI
  usucpf character(11) NOT NULL, -- CPF do usuário Responsável
  CONSTRAINT pk_responsavel_plano_interno_pirid PRIMARY KEY (pirid),
  CONSTRAINT fk_plano_interno_pliid FOREIGN KEY (pliid)
      REFERENCES planejamento.plano_interno (pliid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT fk_usuario_usucpf FOREIGN KEY (usucpf)
      REFERENCES seguranca.usuario (usucpf) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
)
WITH (
  OIDS=FALSE
);
ALTER TABLE planejamento.responsavel_plano_interno OWNER TO postgres;
GRANT ALL ON TABLE planejamento.responsavel_plano_interno TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planejamento.responsavel_plano_interno TO usr_simec;
COMMENT ON TABLE planejamento.responsavel_plano_interno IS 'Vínculo do Plano Interno com Usuário';
COMMENT ON COLUMN planejamento.responsavel_plano_interno.pirid IS 'Chave Primária';
COMMENT ON COLUMN planejamento.responsavel_plano_interno.pliid IS 'Plano Interno - PI';
COMMENT ON COLUMN planejamento.responsavel_plano_interno.usucpf IS 'CPF do usuário Responsável';

GRANT ALL ON SEQUENCE planejamento.responsavel_plano_interno_pirid_seq TO postgres;
GRANT SELECT, USAGE ON SEQUENCE planejamento.responsavel_plano_interno_pirid_seq TO usr_simec;

-- DROP TABLE planejamento.sei_plano_interno; -- antiga planacomorc.pi_sei
CREATE TABLE planejamento.sei_plano_interno
(
  pseid serial NOT NULL, -- Chave Primária
  pliid integer NOT NULL, -- Plano Interno - PI
  psesei character varying(25) NOT NULL, -- Número único do produto principal do sistema Mapas Culturais
  CONSTRAINT pk_sei_plano_interno_pseid PRIMARY KEY (pseid),
  CONSTRAINT fk_plano_interno_pliid FOREIGN KEY (pliid)
      REFERENCES planejamento.plano_interno (pliid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
)
WITH (
  OIDS=FALSE
);
ALTER TABLE planejamento.sei_plano_interno OWNER TO postgres;
GRANT ALL ON TABLE planejamento.sei_plano_interno TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planejamento.sei_plano_interno TO usr_simec;
COMMENT ON TABLE planejamento.sei_plano_interno IS 'Vínculo do Plano Interno com sei';
COMMENT ON COLUMN planejamento.sei_plano_interno.pseid IS 'Chave Primária';
COMMENT ON COLUMN planejamento.sei_plano_interno.pliid IS 'Plano Interno - PI';
COMMENT ON COLUMN planejamento.sei_plano_interno.psesei IS 'Número único do produto principal do sistema Mapas Culturais';

GRANT ALL ON SEQUENCE planejamento.sei_plano_interno_pseid_seq TO postgres;
GRANT SELECT, USAGE ON SEQUENCE planejamento.sei_plano_interno_pseid_seq TO usr_simec;


-- DROP TABLE planejamento.resultado_primario;  -- antiga planacomorc.resultadoprimario
CREATE TABLE planejamento.resultado_primario
(
  rpcod integer NOT NULL, -- Código RP
  redsc character varying(500) NOT NULL, -- Descrição
  CONSTRAINT pk_resultado_primario_rpcod PRIMARY KEY (rpcod)
)
WITH (
  OIDS=FALSE
);
ALTER TABLE planejamento.resultado_primario OWNER TO postgres;
GRANT ALL ON TABLE planejamento.resultado_primario TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planejamento.resultado_primario TO usr_simec;
COMMENT ON TABLE planejamento.resultado_primario IS 'RP - Resultado Primário';
COMMENT ON COLUMN planejamento.resultado_primario.rpcod IS 'Código RP';
COMMENT ON COLUMN planejamento.resultado_primario.redsc IS 'Descrição';

-- DROP TABLE planejamento.subunidade_meta_estrategica; -- antiga planacomorc.subunidademeta_estrategica
CREATE TABLE planejamento.subunidade_meta_estrategica
(
  smeid serial NOT NULL, -- Chave Primária
  suoid integer NOT NULL, -- Sub-Unidade Orçamentária
  meeid integer NOT NULL, -- Metas Estratégicas
  CONSTRAINT pk_subunidade_meta_estrategica_smeid PRIMARY KEY (smeid),
  CONSTRAINT fk_meta_estrategica_meeid FOREIGN KEY (meeid)
      REFERENCES planejamento.meta_estrategica (meeid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT fk_subunidade_orcamentaria_suoid FOREIGN KEY (suoid)
      REFERENCES public.subunidadeorcamentaria (suoid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
)
WITH (
  OIDS=FALSE
);
ALTER TABLE planejamento.subunidade_meta_estrategica OWNER TO postgres;
GRANT ALL ON TABLE planejamento.subunidade_meta_estrategica TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planejamento.subunidade_meta_estrategica TO usr_simec;
COMMENT ON TABLE planejamento.subunidade_meta_estrategica IS 'Vínculo de Metas Estratégicas com Sub-Unidades';
COMMENT ON COLUMN planejamento.subunidade_meta_estrategica.smeid IS 'Chave Primária';
COMMENT ON COLUMN planejamento.subunidade_meta_estrategica.suoid IS 'Sub-Unidade Orçamentária';
COMMENT ON COLUMN planejamento.subunidade_meta_estrategica.meeid IS 'Metas Estratégicas';

GRANT ALL ON SEQUENCE planejamento.subunidade_meta_estrategica_smeid_seq TO postgres;
GRANT SELECT, USAGE ON SEQUENCE planejamento.subunidade_meta_estrategica_smeid_seq TO usr_simec;


-- DROP TABLE planejamento.tipo_acao; -- antiga planacomorc.tipo_acao
CREATE TABLE planejamento.tipo_acao
(
  id_tipo_acao serial NOT NULL, -- Essa coluna é carregada diretamente do webservice
  descricao character varying(25),
  CONSTRAINT pk_planejamento_tipo_acao PRIMARY KEY (id_tipo_acao)
)
WITH (
  OIDS=FALSE
);
ALTER TABLE planejamento.tipo_acao OWNER TO postgres;
GRANT ALL ON TABLE planejamento.tipo_acao TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planejamento.tipo_acao TO usr_simec;
COMMENT ON TABLE planejamento.tipo_acao IS 'Origem dos dados: WSQualitativo -> obterTabelasApoio -> tiposAcao';
COMMENT ON COLUMN planejamento.tipo_acao.id_tipo_acao IS 'Essa coluna é carregada diretamente do webservice';

GRANT ALL ON SEQUENCE planejamento.tipo_acao_id_tipo_acao_seq TO postgres;
GRANT SELECT, USAGE ON SEQUENCE planejamento.tipo_acao_id_tipo_acao_seq TO usr_simec;


-- DROP TABLE planejamento.tipo_inclusao;
CREATE TABLE planejamento.tipo_inclusao
(
  id_tipo_inclusao serial NOT NULL, -- Essa coluna é carregada diretamente do webservice
  instante_alteracao timestamp(6) without time zone,
  descricao character varying(30),
  CONSTRAINT pk_placomorc_tipo_inclusao PRIMARY KEY (id_tipo_inclusao)
)
WITH (
  OIDS=FALSE
);
ALTER TABLE planejamento.tipo_inclusao OWNER TO postgres;
GRANT ALL ON TABLE planejamento.tipo_inclusao TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planejamento.tipo_inclusao TO usr_simec;
COMMENT ON TABLE planejamento.tipo_inclusao IS 'Origem dos dados: WSQualitativo -> obterTabelasApoio -> tiposInclusao';
COMMENT ON COLUMN planejamento.tipo_inclusao.id_tipo_inclusao IS 'Essa coluna é carregada diretamente do webservice';

GRANT ALL ON SEQUENCE planejamento.tipo_inclusao_id_tipo_inclusao_seq TO postgres;
GRANT SELECT, USAGE ON SEQUENCE planejamento.tipo_inclusao_id_tipo_inclusao_seq TO usr_simec;


-- DROP TABLE planejamento.tipo_programa; -- antiga planacomorc.tipo_programa
CREATE TABLE planejamento.tipo_programa
(
  id_tipo_programa serial NOT NULL, -- Essa coluna é carregada diretamente do webservice
  descricao character varying(25),
  CONSTRAINT pk_placomorc_tipo_programa PRIMARY KEY (id_tipo_programa)
)
WITH (
  OIDS=FALSE
);
ALTER TABLE planejamento.tipo_programa OWNER TO postgres;
GRANT ALL ON TABLE planejamento.tipo_programa TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planejamento.tipo_programa TO usr_simec;
COMMENT ON TABLE planejamento.tipo_programa IS 'Origem dos dados: WSQualitativo -> obterTabelasApoio -> tipoprograma';
COMMENT ON COLUMN planejamento.tipo_programa.id_tipo_programa IS 'Essa coluna é carregada diretamente do webservice';

GRANT ALL ON SEQUENCE planejamento.tipo_programa_id_tipo_programa_seq TO postgres;
GRANT SELECT, USAGE ON SEQUENCE planejamento.tipo_programa_id_tipo_programa_seq TO usr_simec;

/*
    Verificar necessidade de Usar.
-- DROP TABLE planejamento.tiporesponsabilidade; -- antiga planacomorc.tiporesponsabilidade
CREATE TABLE planejamento.tiporesponsabilidade
(
  tprcod serial NOT NULL,
  tprdsc character varying(100) NOT NULL,
  tprsnvisivelperfil boolean NOT NULL,
  tprsigla character(1) NOT NULL,
  tprurl character varying(255),
  CONSTRAINT pk_tiporesponsabilidade_tprcod PRIMARY KEY (tprcod)
)
WITH (
  OIDS=FALSE
);
ALTER TABLE planejamento.tiporesponsabilidade OWNER TO postgres;
GRANT ALL ON TABLE planejamento.tiporesponsabilidade TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planejamento.tiporesponsabilidade TO usr_simec;

GRANT ALL ON SEQUENCE planejamento.tiporesponsabilidade_tprcod_seq TO postgres;
GRANT SELECT, USAGE ON SEQUENCE planejamento.tiporesponsabilidade_tprcod_seq TO usr_simec;
*/


-- DROP TABLE planejamento.unidadegestora_limite; -- antiga planacomorc.unidadegestora_limite
CREATE TABLE planejamento.unidadegestora_limite
(
  lmuid serial NOT NULL,
  usucpf character(11),
  lmuvlr numeric(15,2),
  lmudtcadastro timestamp without time zone DEFAULT now(),
  lmustatus character(1) NOT NULL DEFAULT 'A'::bpchar,
  lmuflgliberado boolean DEFAULT false,
  prsano character(4) NOT NULL,
  ungcod character(10),
  unoid integer, -- Identificador da Unidade Orçamentária - Deverá ser utilizado somente em caso do FNC
  CONSTRAINT pk_unidadegestora_limite_lmuid PRIMARY KEY (lmuid),
  CONSTRAINT fk_unidadegestora_limite_unoid FOREIGN KEY (unoid)
      REFERENCES public.unidadeorcamentaria (unoid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
)
WITH (
  OIDS=FALSE
);
ALTER TABLE planejamento.unidadegestora_limite OWNER TO postgres;
GRANT ALL ON TABLE planejamento.unidadegestora_limite TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planejamento.unidadegestora_limite TO usr_simec;
COMMENT ON COLUMN planejamento.unidadegestora_limite.unoid IS 'Identificador da Unidade Orçamentária - Deverá ser utilizado somente em caso do FNC';


GRANT ALL ON SEQUENCE planejamento.unidadegestora_limite_lmuid_seq TO postgres;
GRANT SELECT, USAGE ON SEQUENCE planejamento.unidadegestora_limite_lmuid_seq TO usr_simec;

-- DROP TABLE planejamento.usuarioresponsabilidade; -- antiga planacomorc.usuarioresponsabilidade
CREATE TABLE planejamento.usuarioresponsabilidade
(
  rpuid serial NOT NULL,
  pflcod integer,
  suoid integer, -- Código da SubUnidade
  id_subacao integer,
  id_acao_programatica integer,
  id_periodo_referencia integer,
  usucpf character(11),
  rpustatus character(1) DEFAULT 'A'::bpchar,
  rpudata_inc timestamp without time zone DEFAULT '2008-09-02 17:46:42.244463'::timestamp without time zone,
  ungcod character(6),
  unicod character(5),
  CONSTRAINT pk_usuarioresponsabilidade_rpuid PRIMARY KEY (rpuid),
  CONSTRAINT fk_perfil_pflcod FOREIGN KEY (pflcod)
      REFERENCES seguranca.perfil (pflcod) MATCH SIMPLE
      ON UPDATE RESTRICT ON DELETE RESTRICT,
  CONSTRAINT fk_usuario_usucpf FOREIGN KEY (usucpf)
      REFERENCES seguranca.usuario (usucpf) MATCH SIMPLE
      ON UPDATE RESTRICT ON DELETE RESTRICT
)
WITH (
  OIDS=FALSE
);
ALTER TABLE planejamento.usuarioresponsabilidade OWNER TO postgres;
GRANT ALL ON TABLE planejamento.usuarioresponsabilidade TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planejamento.usuarioresponsabilidade TO usr_simec;
COMMENT ON COLUMN planejamento.usuarioresponsabilidade.suoid IS 'Código da SubUnidade';

GRANT ALL ON SEQUENCE planejamento.usuarioresponsabilidade_rpuid_seq TO postgres;
GRANT SELECT, USAGE ON SEQUENCE planejamento.usuarioresponsabilidade_rpuid_seq TO usr_simec;



/**

    EXECUÇÃO DE VIEWS DO monitora

 */


-- DROP VIEW planejamento.vw_ptres;
CREATE OR REPLACE VIEW planejamento.vw_ptres AS
    SELECT DISTINCT
        ptr.ptrid,
        ptr.ptres,
        ptr.acaid,
        ptr.ptrano,
        aca.funcod,
        aca.sfucod,
        ptr.prgcod,
        ptr.acacod,
        ptr.loccod,
        ptr.unicod,
        ptr.irpcod,
        ptr.ptrdotacaoinicialcusteio,
        ptr.ptrdotacaoinicialcapital,
        (psu.ptrdotacaocapital + psu.ptrdotacaocusteio)::NUMERIC AS ptrdotacao,
        ptr.ptrstatus,
        ptr.ptrdata,
        ptr.plocod,
        ptr.esfcod,
        prg.prgdsc::CHARACTER VARYING(300) AS prgdsc,
        aca.acatitulo,
        loc.locdsc,
        aca.acaobjetivocod,
        (((((((btrim(aca.prgcod::TEXT) || '.'::TEXT) || btrim(aca.acacod::TEXT)) || '.'::TEXT) || btrim(aca.loccod::TEXT)) || '.'::TEXT) ||
            CASE
                WHEN length(btrim(aca.acaobjetivocod::TEXT)) <= 0 THEN '-'::TEXT
                ELSE COALESCE(btrim(aca.acaobjetivocod::TEXT), ''::TEXT)
            END) || '.'::TEXT) || btrim(ptr.plocod::TEXT) AS funcional,
        psu.ptrdotacaocapital,
        psu.ptrdotacaocusteio,
        ptr.plodsc
    FROM planejamento.ptres ptr
        JOIN planejamento.acao aca ON ptr.acaid = aca.acaid
        LEFT JOIN localizador loc ON ptr.loccod::bpchar = loc.loccod
        LEFT JOIN planejamento.programa prg ON ptr.prgcod::bpchar = prg.prgcod AND ptr.ptrano::bpchar = prg.prgano AND prg.prgstatus = 'A'::bpchar
        LEFT JOIN (
            SELECT psu_1.ptrid,
                SUM(psu_1.ptrdotacaocusteio) AS ptrdotacaocusteio,
                SUM(psu_1.ptrdotacaocapital) AS ptrdotacaocapital
            FROM spo.ptressubunidade psu_1
            GROUP BY
                psu_1.ptrid
        ) psu ON ptr.ptrid = psu.ptrid;

ALTER TABLE planejamento.vw_ptres OWNER TO postgres;
GRANT ALL ON TABLE planejamento.vw_ptres TO postgres;
GRANT SELECT ON TABLE planejamento.vw_ptres TO usr_simec;


 -- DROP VIEW planejamento.vw_plano_interno;
CREATE OR REPLACE VIEW planejamento.vw_plano_interno AS
    SELECT
        pli.pliid,
        pli.plicod,
        pli.plititulo,
        pli.plidsc,
        pli.pliano,
        pic.picquantidade,
        COALESCE(pic.picvalorcapital, 0::DOUBLE PRECISION) AS picvalorcapital,
        COALESCE(pic.picvalorcusteio, 0::DOUBLE PRECISION) AS picvalorcusteio,
        COALESCE(pic.picvalorcapital, 0::DOUBLE PRECISION) + COALESCE(pic.picvalorcusteio, 0::DOUBLE PRECISION) AS previsto,
        COALESCE(sex.vlrautorizado, 0::numeric) AS autorizado,
        COALESCE(sex.vlrempenhado, 0::numeric) AS empenhado,
        COALESCE(sex.vlrliquidado, 0::numeric) AS liquidado,
        COALESCE(sex.vlrpago, 0::numeric) AS pago,
        aco.acoid,
        aco.acoquantidade,
        aco.acodata,
        esd.esddsc,
        suo.unoid,
        suo.unocod,
        suo.unonome,
        suo.unosigla,
        suo.suoid,
        suo.suocod,
        suo.suonome,
        suo.suosigla,
        eqd.eqddsc,
        REPLACE(REPLACE(REPLACE((ARRAY(
            SELECT DISTINCT (((' '::TEXT || erp.irpcod::TEXT) || ' - '::TEXT) || rp.irpdsc::TEXT) || ' '::TEXT
            FROM planejamento.enquadramentorp erp
                JOIN identresultprimario rp ON erp.irpcod = rp.irpcod
            WHERE erp.eqdid = eqd.eqdid))::TEXT, '{'::TEXT, ''::TEXT), '}'::TEXT, ''::TEXT), '"'::TEXT, ''::TEXT) AS resultadoprimario,
        mai.mainome,
        mas.masnome,
        ptr.prgcod,
        ptr.prgdsc,
        opp.oppcod,
        opp.oppnome,
        ipp.ippcod,
        ipp.ippnome,
        ptr.acacod,
        ptr.acatitulo,
        ptr.loccod,
        ptr.locdsc,
        ptr.plocod,
        ptr.plodsc AS po,
        ptr.ptres,
        arc.arcdsc,
        (mpn.mpncod::TEXT || ' - '::TEXT) || mpn.mpnnome::TEXT AS mpnnome,
        (mpp.mppcod::TEXT || ' - '::TEXT) || mpp.mppnome::TEXT AS mppnome,
        sec.secdsc,
        cap.capdsc,
        ptr.ptrid,
        ptr.funcional,
        ptr.acaid,
        pli.pliemenda,
        pic.picedital,
        pum.pumnome,
        ppr.pprid,
        ppr.pprnome,
        CASE
            WHEN pli.pliemenda = true THEN 'Sim'::TEXT
            ELSE 'Não'::TEXT
        END AS emenda,
        CASE
            WHEN pic.picedital = true THEN 'Sim'::TEXT
            ELSE 'Não'::TEXT
        END AS edital,
        esf.esfdsc,
        pic.mppid,
        pic.mpnid,
        pic.oppid,
        pic.pumid,
        pic.ipnid,
        pic.ippid
    FROM planejamento.plano_interno pli
        JOIN planejamento.complemento pic ON pic.pliid = pli.pliid
        JOIN vw_subunidadeorcamentaria suo ON suo.suoid = pli.suoid AND suo.prsano = pli.pliano::BPCHAR
        JOIN planejamento.enquadramento_despesa eqd ON eqd.eqdid = pli.eqdid
        LEFT JOIN planejamento.manutencao_item mai ON mai.maiid = pic.maiid -- mudar para planejamento
        LEFT JOIN planejamento.manutencao_subitem mas ON mas.masid = pic.masid -- mudar para planejamento
        LEFT JOIN planejamento.vw_ptres ptr ON ptr.ptrid = pli.pliid AND ptr.ptrano::TEXT = pli.pliano::TEXT
        LEFT JOIN objetivoppa opp ON opp.oppid = pic.oppid
        LEFT JOIN iniciativappa ipp ON ipp.ippid = pic.ippid
        LEFT JOIN planejamento.area_cultural arc ON arc.arceid = pic.arceid
        LEFT JOIN planejamento.segmento_cultural sec ON sec.secid = pic.secid
        LEFT JOIN metappa mpp ON mpp.mppid = pic.mppid
        LEFT JOIN metapnc mpn ON mpn.mpnid = pic.mpnid
        LEFT JOIN planejamento.categoria_apropriacao cap ON cap.capid = pli.capid
        LEFT JOIN planejamento.produto ppr ON ppr.pprid = pic.pprid
        LEFT JOIN planejamento.unidade_medida pum ON pum.pumid = pic.pumid
        LEFT JOIN workflow.documento doc ON doc.docid = pli.docid
        LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
        LEFT JOIN territorios.esfera esf ON esf.esfid = pic.esfid
        LEFT JOIN meses mes ON mes.mescod = pic.mescod
        LEFT JOIN acompanhamento.acompanhamento aco ON aco.pliid = pli.pliid
        LEFT JOIN(
            SELECT
                 sex_1.exercicio,
                 sex_1.plicod,
                 SUM(sex_1.vlrautorizado) AS vlrautorizado,
                 SUM(sex_1.vlrempenhado) AS vlrempenhado,
                 SUM(sex_1.vlrliquidado) AS vlrliquidado,
                 SUM(sex_1.vlrpago) AS vlrpago
            FROM spo.siopexecucao sex_1
            WHERE
                COALESCE(sex_1.plicod, ''::BPCHAR) <> ''::BPCHAR
            GROUP BY
                sex_1.exercicio, sex_1.plicod) sex ON sex.plicod = pli.plicod::BPCHAR AND sex.exercicio = pli.pliano::BPCHAR
    WHERE pli.plistatus = 'A'::BPCHAR;
ALTER TABLE planejamento.vw_plano_interno OWNER TO postgres;
GRANT ALL ON TABLE planejamento.vw_plano_interno TO postgres;
GRANT SELECT ON TABLE planejamento.vw_plano_interno TO usr_simec;
