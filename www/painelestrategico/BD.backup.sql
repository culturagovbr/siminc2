--
-- PostgreSQL database dump
--

-- Started on 2009-11-03 10:31:57

SET statement_timeout = 0;
SET client_encoding = 'LATIN1';
SET standard_conforming_strings = off;
SET check_function_bodies = false;
SET client_min_messages = warning;
SET escape_string_warning = off;

--
-- TOC entry 7 (class 2615 OID 43225496)
-- Name: mec_painel; Type: SCHEMA; Schema: -; Owner: -
--

CREATE SCHEMA mec_painel;


SET search_path = mec_painel, pg_catalog;

SET default_tablespace = '';

SET default_with_oids = false;

--
-- TOC entry 2457 (class 1259 OID 43284976)
-- Dependencies: 7
-- Name: tb_acao; Type: TABLE; Schema: mec_painel; Owner: -; Tablespace: 
--

CREATE TABLE tb_acao (
    cd_acao integer NOT NULL,
    nm_acao character varying(255) NOT NULL,
    url_acao_pde character varying(255),
    nm_arquivo_fluxo character varying(255)
);


--
-- TOC entry 2475 (class 1259 OID 54542826)
-- Dependencies: 7
-- Name: tb_acao_ator; Type: TABLE; Schema: mec_painel; Owner: -; Tablespace: 
--

CREATE TABLE tb_acao_ator (
    cd_acao integer NOT NULL,
    cd_ator integer NOT NULL,
    nr_ordem_apresentacao_ator integer NOT NULL
);


--
-- TOC entry 2458 (class 1259 OID 43285027)
-- Dependencies: 7
-- Name: tb_acao_objetivo; Type: TABLE; Schema: mec_painel; Owner: -; Tablespace: 
--

CREATE TABLE tb_acao_objetivo (
    cd_objetivo integer NOT NULL,
    cd_acao integer NOT NULL,
    nr_ordem_apresentacao integer NOT NULL
);


--
-- TOC entry 2474 (class 1259 OID 54371080)
-- Dependencies: 7
-- Name: tb_ator; Type: TABLE; Schema: mec_painel; Owner: -; Tablespace: 
--

CREATE TABLE tb_ator (
    cd_ator integer NOT NULL,
    nm_ator character varying(150) NOT NULL
);


--
-- TOC entry 2456 (class 1259 OID 43226357)
-- Dependencies: 7
-- Name: tb_indicador; Type: TABLE; Schema: mec_painel; Owner: -; Tablespace: 
--

CREATE TABLE tb_indicador (
    cd_objetivo integer NOT NULL,
    nm_indicador character varying(255) NOT NULL,
    dc_meta character varying(255) NOT NULL,
    nr_ordem_apresentacao integer NOT NULL,
    cd_status character(1) NOT NULL,
    url_indicador character varying(255),
    cd_tendencia character(1),
    dc_fonte character varying(80),
    dc_periodicidade character varying(50),
    dc_formula character varying(155),
    dc_apurado character varying(50),
    dc_observacao character varying(255),
    cd_desatualizado character(1)
);


--
-- TOC entry 2479 (class 1259 OID 56718903)
-- Dependencies: 7
-- Name: tb_indicador_funcao; Type: TABLE; Schema: mec_painel; Owner: -; Tablespace: 
--

CREATE TABLE tb_indicador_funcao (
    cd_indicador_funcao integer NOT NULL,
    cd_acao integer NOT NULL,
    cd_ponto_controle integer,
    nm_indicador character varying(255) NOT NULL,
    dc_meta character varying(255) NOT NULL,
    nr_ordem_apresentacao integer NOT NULL,
    cd_status character(1),
    url_indicador character varying(255),
    cd_tendencia character(1),
    dc_apurado character varying(50),
    cd_desatualizado character(1)
);


--
-- TOC entry 2478 (class 1259 OID 56718901)
-- Dependencies: 7 2479
-- Name: tb_indicador_funcao_cd_indicador_funcao_seq; Type: SEQUENCE; Schema: mec_painel; Owner: -
--

CREATE SEQUENCE tb_indicador_funcao_cd_indicador_funcao_seq
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


--
-- TOC entry 2936 (class 0 OID 0)
-- Dependencies: 2478
-- Name: tb_indicador_funcao_cd_indicador_funcao_seq; Type: SEQUENCE OWNED BY; Schema: mec_painel; Owner: -
--

ALTER SEQUENCE tb_indicador_funcao_cd_indicador_funcao_seq OWNED BY tb_indicador_funcao.cd_indicador_funcao;


--
-- TOC entry 2937 (class 0 OID 0)
-- Dependencies: 2478
-- Name: tb_indicador_funcao_cd_indicador_funcao_seq; Type: SEQUENCE SET; Schema: mec_painel; Owner: -
--

SELECT pg_catalog.setval('tb_indicador_funcao_cd_indicador_funcao_seq', 4, true);


--
-- TOC entry 2452 (class 1259 OID 43225697)
-- Dependencies: 7
-- Name: tb_mapa; Type: TABLE; Schema: mec_painel; Owner: -; Tablespace: 
--

CREATE TABLE tb_mapa (
    cd_mapa character(1) NOT NULL,
    in_corporativo character(1) NOT NULL,
    nm_mapa character varying(255) NOT NULL
);


--
-- TOC entry 2454 (class 1259 OID 43225728)
-- Dependencies: 7
-- Name: tb_objetivo; Type: TABLE; Schema: mec_painel; Owner: -; Tablespace: 
--

CREATE TABLE tb_objetivo (
    cd_objetivo integer NOT NULL,
    nm_objetivo character varying(255) NOT NULL,
    nr_ordem_apresentacao integer NOT NULL,
    cd_mapa character(1) NOT NULL,
    cd_perspectiva integer NOT NULL,
    cd_tema character(1)
);


--
-- TOC entry 2453 (class 1259 OID 43225713)
-- Dependencies: 7
-- Name: tb_perspectiva; Type: TABLE; Schema: mec_painel; Owner: -; Tablespace: 
--

CREATE TABLE tb_perspectiva (
    cd_mapa character(1) NOT NULL,
    cd_perspectiva integer NOT NULL,
    nm_perspectiva character varying(256) NOT NULL,
    nr_ordem_apresentacao integer NOT NULL
);


--
-- TOC entry 2473 (class 1259 OID 54371016)
-- Dependencies: 7
-- Name: tb_ponto_controle; Type: TABLE; Schema: mec_painel; Owner: -; Tablespace: 
--

CREATE TABLE tb_ponto_controle (
    cd_ponto_controle integer NOT NULL,
    nm_ponto_controle character varying(150) NOT NULL,
    cd_acao integer NOT NULL,
    cd_ator integer NOT NULL,
    cd_tempo integer NOT NULL,
    nr_ordem_apresentacao integer NOT NULL
);


--
-- TOC entry 2455 (class 1259 OID 43226328)
-- Dependencies: 7
-- Name: tb_tema; Type: TABLE; Schema: mec_painel; Owner: -; Tablespace: 
--

CREATE TABLE tb_tema (
    cd_tema character(1) NOT NULL,
    nm_tema character varying(80) NOT NULL
);



--
-- TOC entry 2897 (class 2604 OID 56718905)
-- Dependencies: 2478 2479 2479
-- Name: cd_indicador_funcao; Type: DEFAULT; Schema: mec_painel; Owner: -
--

ALTER TABLE tb_indicador_funcao ALTER COLUMN cd_indicador_funcao SET DEFAULT nextval('tb_indicador_funcao_cd_indicador_funcao_seq'::regclass);


--
-- TOC entry 2927 (class 0 OID 43284976)
-- Dependencies: 2457
-- Data for Name: tb_acao; Type: TABLE DATA; Schema: mec_painel; Owner: -
--

INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (2, 'Avalia��o no Sistema Nacional de Ensino Superior - SINAES', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (5, 'Cat�logo Nacional dos Cursos Superiores de Tecnologia', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (6, 'Cat�logo Nacional dos Cursos T�cnicos', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (7, 'CENSO da Educa��o Superior', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (8, 'Compromisso Todos pela Educa��o', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (9, 'Conex�es de saberes', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (10, 'Cria��o dos institutos Federais', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (11, 'Curr�culo em Movimento', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (12, 'EDUCACENSO', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (13, 'e-MEC', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (14, 'ENCCEJA Exame Nacional para Certifica��o de Compet�ncias de Jovens e Adultos', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (15, 'Ensino M�dio Inovador', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (16, 'Escola Aberta', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (17, 'Escola Acess�vel', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (18, 'Escola Ativa', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (19, 'Escola de Altos Estudos', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (20, 'Escola de Gestores', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (21, 'Escola que protege', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (22, 'Escola T�cnica Aberta do Brasil (e Tec Brasil)', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (23, 'Exame Nacional do Ensino M�dio ENEM', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (24, 'Expans�o da Rede Federal de EPT Fase I', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (25, 'Expans�o da Rede Federal de EPT Fase II', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (26, 'Expans�o do PDDE para a Educa��o Infantil', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (27, 'Extens�o do PDDE para o Ensino M�dio', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (28, 'Extens�o do Programa Nacional de Alimenta��o Escolar (PNAE) para o Ensino M�dio', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (29, 'Extens�o do Programa Nacional de Apoio ao Transporte Escolar (PNATE) para a Educa��o B�sica', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (30, 'Forma��o pelas escolas', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (31, 'Frequ�ncia Escolar PBF', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (32, 'Gera��o de Trabalho e Renda (Associativismo, cooperativismo e empreendedorismo)', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (33, 'GESTAR', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (34, 'GESTAR II', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (35, 'Hospitais Universit�rios Federais HUF''s', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (36, 'Implanta��o do PDDE da Escola', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (37, 'Implementa��o da Lei de incentivos Empresa ICT', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (38, 'INCLUIR Acessibilidade no Ensino Superior', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (39, 'Instrumento de Avalia��o', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (40, 'Livro Did�tico Para Alfabetiza��o de Jovens e Adultos PNLA', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (41, 'MAB', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (42, 'Mais Educa��o', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (43, 'MEC', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (44, 'Observat�rio da Educa��o', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (45, 'Observat�rio Mundo do Trabalho', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (46, 'Olimp�ada Brasileira da L�ngua Portuguesa', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (47, 'Olimp�ada Brasileira de Matem�tica das Escolas P�blicas', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (48, 'Orienta��es Curriculares sobre o Ensino de Nove Anos', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (49, 'Parcela Extra do PDDE para as Escolas Rurais', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (50, 'Parcela Extra do PDDE para as Escolas Urbanas', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (51, 'PDDE', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (52, 'PDDE financiando a educa��o integral', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (53, 'PDDE investindo na Acessibilidade �s Escolas', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (54, 'PEC-G', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (55, 'PET', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (56, 'Piso salarial do Magist�rio', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (57, 'PNLD - Contemplando Diferentes Formatos e Acessibilidades', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (58, 'Portais Educacionais', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (59, 'Portal do Professor', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (60, 'Portal dos Perio�dicos', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (61, 'PRADINE', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (62, 'Pr� Campo', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (63, 'Pro conselho Programa de Forma��o de Conselheiros Municipais de Educa��o', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (64, 'Pr� Letramento', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (65, 'PRO Lind', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (66, 'Pro Doc�ncia - Programa de Consolida��o das Licenciaturas', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (67, 'PROEJA Programa Nacional de Integra��o da Educa��o Profissional com a Educa��o B�sica na Modalidade de Educa��o de Jovens e Adultos', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (68, 'Professor Equivalente/Tec. Admin. Equivalente', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (69, 'Profuncion�rio', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (70, 'Programa Brasil Profissionalizado', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (71, 'Programa de Acompanhamento dos PME, PEE e PNE', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (85, 'PROINFO Integrado - Infra-estrutura', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (73, 'Programa de Apoio � Extens�o Universit�ria PROEXT', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (74, 'Programa de Forma��o Continuada de Professores na Educa��o Especial', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (75, 'Programa de Forma��o de Conselheiros Escolares', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (76, 'Programa Institucional de Bolsas de Inicia��o � Doc�ncia - PIBID', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (77, 'Programa Nacional da Biblioteca Escolar para a Educa��o infantil e o Ensino M�dio', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (78, 'Programa Nacional de Assist�ncia Estudantil PNAES', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (79, 'Programa Nacional de P�s Doutorado PNPD', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (80, 'Programa Sa�de na Escola PSE', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (81, 'ProInf�ncia', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (82, 'Proinfantil', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (83, 'PROINFO Integrado - Aluno', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (84, 'PROINFO Integrado - Capacita�� no uso das TIC nas escolas', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (86, 'PROINFO Integrado - Projeto um Computador por Aluno (UCA)', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (87, 'PROINFO Integrado (Luz para Todos)', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (88, 'PROINFO Integrado - Conte�dos medi�ticos, TV Escola e Portal do Professor', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (89, 'ProJovem Campo Saberes da Terra', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (90, 'PROUNI FIES', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (91, 'Prova Brasil', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (92, 'Provinha Brasil', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (93, 'Rede Nacional de Informa��o', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (94, 'Rede UAB', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (95, 'Regula��o', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (96, 'REUNI / Expans�o das Universidades Federais Fase I', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (97, 'RNP', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (98, 'Sala de Recursos Multifuncionais', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (99, 'SINAES', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (100, 'SIOPE', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (101, 'SIOPE - FNDE', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (102, 'SISTEC', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (103, 'Sistema Nacional de Forma��o', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (104, 'Sistema UAB Universidade Aberta do Brasil', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (105, 'Supervis�o', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (106, 'TECNEP', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (107, 'Tecnologias Educacionais', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (108, 'TV Escola', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (109, 'UNIAFRO', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (1, 'Acompanhamento da frequ�ncia escolar', 'xxxxxxxxxxxxx', 'fluxo/fluxo.jpg');
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (72, 'Programa de Acompanhamento e Monitoramento BPC na Escola', 'xxxxxxxxxxxxx', NULL);
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (4, 'Caminho da Escola', 'xxxxxxxxxxx', 'fluxo/Caminho da escola.pdf');
INSERT INTO tb_acao (cd_acao, nm_acao, url_acao_pde, nm_arquivo_fluxo) VALUES (3, 'Brasil Alfabetizado', 'xxxxxxxxxxxxxxxxxxxx', 'fluxo/Programa Brasil Alfabetizado_validado.pdf');


--
-- TOC entry 2931 (class 0 OID 54542826)
-- Dependencies: 2475
-- Data for Name: tb_acao_ator; Type: TABLE DATA; Schema: mec_painel; Owner: -
--

INSERT INTO tb_acao_ator (cd_acao, cd_ator, nr_ordem_apresentacao_ator) VALUES (1, 1, 1);
INSERT INTO tb_acao_ator (cd_acao, cd_ator, nr_ordem_apresentacao_ator) VALUES (1, 2, 2);
INSERT INTO tb_acao_ator (cd_acao, cd_ator, nr_ordem_apresentacao_ator) VALUES (1, 3, 3);
INSERT INTO tb_acao_ator (cd_acao, cd_ator, nr_ordem_apresentacao_ator) VALUES (1, 4, 4);
INSERT INTO tb_acao_ator (cd_acao, cd_ator, nr_ordem_apresentacao_ator) VALUES (3, 7, 1);
INSERT INTO tb_acao_ator (cd_acao, cd_ator, nr_ordem_apresentacao_ator) VALUES (3, 8, 2);
INSERT INTO tb_acao_ator (cd_acao, cd_ator, nr_ordem_apresentacao_ator) VALUES (3, 9, 3);
INSERT INTO tb_acao_ator (cd_acao, cd_ator, nr_ordem_apresentacao_ator) VALUES (3, 10, 4);
INSERT INTO tb_acao_ator (cd_acao, cd_ator, nr_ordem_apresentacao_ator) VALUES (3, 11, 5);
INSERT INTO tb_acao_ator (cd_acao, cd_ator, nr_ordem_apresentacao_ator) VALUES (4, 7, 1);
INSERT INTO tb_acao_ator (cd_acao, cd_ator, nr_ordem_apresentacao_ator) VALUES (4, 12, 2);
INSERT INTO tb_acao_ator (cd_acao, cd_ator, nr_ordem_apresentacao_ator) VALUES (4, 13, 3);
INSERT INTO tb_acao_ator (cd_acao, cd_ator, nr_ordem_apresentacao_ator) VALUES (4, 14, 4);
INSERT INTO tb_acao_ator (cd_acao, cd_ator, nr_ordem_apresentacao_ator) VALUES (4, 9, 5);
INSERT INTO tb_acao_ator (cd_acao, cd_ator, nr_ordem_apresentacao_ator) VALUES (4, 15, 6);


--
-- TOC entry 2928 (class 0 OID 43285027)
-- Dependencies: 2458
-- Data for Name: tb_acao_objetivo; Type: TABLE DATA; Schema: mec_painel; Owner: -
--

INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (30, 72, 1);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (30, 3, 2);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (31, 72, 1);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (31, 80, 2);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (31, 1, 3);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (32, 89, 1);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (32, 42, 2);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (32, 22, 3);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (32, 70, 4);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (32, 24, 5);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (32, 25, 6);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (32, 67, 7);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (32, 18, 8);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (32, 83, 9);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (33, 59, 1);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (33, 74, 2);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (33, 82, 3);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (33, 84, 4);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (33, 104, 5);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (33, 103, 6);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (33, 76, 7);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (33, 66, 8);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (33, 33, 9);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (33, 34, 10);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (33, 64, 11);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (33, 93, 12);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (34, 56, 1);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (34, 100, 2);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (35, 30, 1);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (35, 69, 2);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (42, 98, 1);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (42, 108, 2);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (42, 88, 3);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (42, 86, 4);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (42, 85, 5);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (42, 77, 6);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (42, 40, 7);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (42, 57, 8);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (40, 28, 1);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (40, 80, 2);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (41, 107, 1);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (41, 44, 2);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (41, 46, 3);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (41, 47, 4);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (41, 66, 5);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (41, 58, 6);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (41, 85, 7);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (36, 29, 1);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (36, 4, 2);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (39, 42, 1);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (39, 87, 2);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (39, 17, 3);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (39, 70, 4);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (39, 81, 5);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (39, 24, 6);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (39, 25, 7);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (39, 53, 8);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (45, 23, 1);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (47, 14, 1);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (48, 22, 1);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (48, 85, 2);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (48, 88, 3);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (49, 68, 1);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (49, 10, 2);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (51, 20, 1);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (51, 69, 2);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (51, 22, 3);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (52, 67, 1);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (53, 39, 1);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (53, 13, 2);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (53, 5, 3);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (54, 13, 1);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (55, 69, 1);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (55, 22, 2);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (56, 86, 1);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (56, 84, 2);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (57, 102, 1);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (59, 90, 1);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (59, 109, 2);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (59, 65, 3);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (59, 62, 4);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (59, 38, 5);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (59, 78, 6);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (60, 90, 1);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (60, 109, 2);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (60, 65, 3);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (60, 62, 4);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (60, 38, 5);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (60, 78, 6);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (66, 73, 1);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (66, 55, 2);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (66, 44, 3);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (69, 76, 1);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (69, 66, 2);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (69, 103, 3);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (70, 104, 1);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (72, 54, 1);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (75, 60, 1);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (75, 79, 2);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (75, 37, 3);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (79, 9, 1);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (79, 14, 2);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (79, 3, 3);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (79, 31, 4);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (79, 21, 5);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (79, 49, 6);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (79, 62, 7);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (79, 4, 8);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (79, 18, 9);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (80, 16, 1);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (80, 51, 2);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (81, 89, 1);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (81, 3, 2);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (81, 31, 3);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (81, 12, 4);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (85, 50, 1);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (84, 77, 1);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (84, 40, 2);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (84, 67, 3);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (84, 94, 4);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (85, 62, 1);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (85, 65, 2);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (85, 44, 3);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (85, 104, 4);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (85, 109, 5);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (85, 74, 6);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (85, 103, 7);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (86, 8, 1);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (89, 107, 1);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (90, 50, 1);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (90, 42, 2);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (90, 36, 3);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (90, 27, 4);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (90, 85, 5);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (90, 52, 6);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (1, 30, 3);
INSERT INTO tb_acao_objetivo (cd_objetivo, cd_acao, nr_ordem_apresentacao) VALUES (30, 1, 3);


--
-- TOC entry 2930 (class 0 OID 54371080)
-- Dependencies: 2474
-- Data for Name: tb_ator; Type: TABLE DATA; Schema: mec_painel; Owner: -
--

INSERT INTO tb_ator (cd_ator, nm_ator) VALUES (1, 'Estados / Munic�pios');
INSERT INTO tb_ator (cd_ator, nm_ator) VALUES (8, 'Diretoria de pol�ticas de educa��o de jovens e adultos - SECAD');
INSERT INTO tb_ator (cd_ator, nm_ator) VALUES (9, 'Diretoria de Programas e Projetos Educacionais - FNDE');
INSERT INTO tb_ator (cd_ator, nm_ator) VALUES (10, 'Diretoria Financeira - FNDE');
INSERT INTO tb_ator (cd_ator, nm_ator) VALUES (11, 'Procuradoria Federal - FNDE');
INSERT INTO tb_ator (cd_ator, nm_ator) VALUES (7, 'Estado/Munic�pios');
INSERT INTO tb_ator (cd_ator, nm_ator) VALUES (12, 'Secretaria Executiva - SE');
INSERT INTO tb_ator (cd_ator, nm_ator) VALUES (13, 'Diretoria de Administra��o e Tecnologia - FNDE');
INSERT INTO tb_ator (cd_ator, nm_ator) VALUES (14, 'Diretoria de A��es Educacionais � DIRAE/FNDE');
INSERT INTO tb_ator (cd_ator, nm_ator) VALUES (15, 'INMETRO');
INSERT INTO tb_ator (cd_ator, nm_ator) VALUES (3, 'Secretaria Executiva - SE');
INSERT INTO tb_ator (cd_ator, nm_ator) VALUES (4, 'Unidade Gestora do Projeto');
INSERT INTO tb_ator (cd_ator, nm_ator) VALUES (6, 'Diretoria Financeira');
INSERT INTO tb_ator (cd_ator, nm_ator) VALUES (5, 'Consultoria Jur�dica');
INSERT INTO tb_ator (cd_ator, nm_ator) VALUES (2, 'Comit� Gestor');


--
-- TOC entry 2926 (class 0 OID 43226357)
-- Dependencies: 2456
-- Data for Name: tb_indicador; Type: TABLE DATA; Schema: mec_painel; Owner: -
--

INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (28, 'Taxa de escolariza��o na Educa��o B�sica', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, '1');
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (31, 'Taxa de evas�o', '4000', 4, '1', 'xxxxxxxxxxxxx', '3', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (29, 'IDEB', '4000', 1, '2', 'xxxxxxxxxxxxx', '2', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (2, 'IDEB, ENEM, Sinaes, Prova Brasil, Provinha Brasil', '4000', 1, '2', 'xxxxxxxxxxxxx', '2', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, '1');
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (34, '�ndice salarial regional dos professores ', '4000', 2, '2', 'xxxxxxxxxxxxx', '3', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (31, 'Taxa de frequ�ncia bruta', '4000', 1, '1', 'xxxxxxxxxxxxx', '5', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (24, 'N�mero �ndice de escolas adotando conte�do MEC
 Secret�rias envolvidas: SEB, SETEC. SEESP', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (72, 'N�mero de professores estrangeiros visitantes', '4000', 3, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (72, 'N�mero de professores em atividade no exterior', '4000', 4, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (74, 'N�mero de pontos de acesso � RNP', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (75, 'N�mero de programas de p�s-gradua��o por conceito (de 1 a 7)', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (76, 'Taxa de credenciamento (eMEC)', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (76, 'Avalia��o de cursos', '4000', 2, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (76, 'Termos de  saneamento de defici�ncias', '4000', 3, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (80, '�ndice de escolas abertas final de semana', '4000', 2, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (31, 'N�mero m�dio de s�ries conclu�das da popula��o de 10-14 anos', '4000', 5, '2', 'xxxxxxxxxxxxx', '2', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (79, 'Taxa de cobertura', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (80, '�ndice de escolas com conselho escolar atuante', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (78, '�ndice de Igualdade da Educa��o do Campo ', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (78, '�ndice de Igualdade da Educa��o Escolar Ind�gena', '4000', 2, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (77, 'Taxa de cobertura', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (78, '�ndice de Igualdade das A��es Educativas Complementares ', '4000', 3, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (78, '�ndice de Igualdade das Diversidades �tnico-Raciais ', '4000', 4, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (78, '�ndice de Igualdade de G�nero', '4000', 5, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (81, 'Taxa de matricula', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (25, 'Percentual de cursos supervisionados', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (31, 'Taxa de abandono', '4000', 3, '1', 'xxxxxxxxxxxxx', '4', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, '1');
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (81, 'Taxa de conclus�o (com foco nas popula��es exclu�das)', '4000', 2, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (82, 'N�mero de cursos com forma��o espec�fica', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (54, 'Participa��o em reuni�es e comit�s e CTA quadro superior / Reuni�es ordin�rias do CONAC ', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (55, 'Taxa de Professores Capacitados', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (83, 'N�mero de escolas para a diversidade com equipamentos adequado (luz, biblioteca, sala de computadores, saneamento', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (84, 'N�mero de alunos beneficiados', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (85, 'Taxa de forma��o de professores para a diversidade', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (86, 'Propor��o de solicita��es dos munic�pios e estados no PAR para as tem�ticas da diversidade (forma��o de professores e material did�tico', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (3, 'Propor��o de vagas preenchidas pelos segmentos espec�ficos da popula��o, em todos os n�veis e modalidades de ensino, em rela��o ao pr�prio segmento e em rela��o � popula��o total', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (7, 'Rela��o desempenho dos estudantes das popula��es espec�ficas (segmentos) x desempenho de todos os estudantes do pa�s', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (8, 'Percentual de atendimento da demanda de segmentos espec�ficos da popula��o, em todos os n�veis e modalidades de ensino', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (16, 'Taxa de oferta de vagas � ocupadas x ofertadas', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (17, 'Percentual de escolas com recursos adequados', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (18, 'N�mero �ndice de professores com especializa��o para a diversidade', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (30, 'Taxa de matriculas na educa��o do campo, ind�genas, quilombolas e na educa��o de jovens e adultos, nos n�veis e modalidades de ensino', '4000', 2, '1', 'xxxxxxxxxxxxx', '3', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (31, 'Taxa de frequ�ncia escolar mensal no ensino fundamental e ensino m�dio de estudantes de fam�lias pobres e extremamente pobres � Taxa de defasagem  idade � s�rie', '4000', 6, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (32, 'Taxa de sucesso nos exames de refer�ncia', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (33, 'Percentual de professores com forma��o adequada / Inicial ao n�vel, modalidade e etapa', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (33, 'Rela��o aluno/professor', '4000', 2, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (31, 'Taxa de frequ�ncia l�quida', '4000', 2, '1', 'xxxxxxxxxxxxx', '2', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (35, 'Percentual de n�o-docentes com forma��o adequada', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (35, 'Rela��o aluno / n�o-docente', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (36, 'Percentual de alunos atendidos com transporte escolar urbano e rural (INEP)', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (37, 'Rela��o investimento em educa��o e PIB', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (37, 'Taxa de crescimento do gasto por aluno ao ano', '4000', 2, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (39, 'Percentual de escolas atendidas', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (40, 'Percentual de alunos atendidos com alimenta��o ', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (40, 'Percentual de conselhos de alimenta��o  capacitados', '4000', 2, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (41, 'Percentual de escolas atendidas com tecnologia', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (41, 'N�mero de projetos desenvolvidos', '4000', 2, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (30, 'Taxa de matr�culas inclusivas na educa��o especial', '4000', 1, '2', 'xxxxxxxxxxxxx', '5', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (58, 'Qualidade dos egressos da p�s-gradua��o', '4000', 2, '2', 'xxxxxxxxxxxxx', '2', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (44, 'Percentual de egressos inseridos no mercado de trabalho', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (45, 'Indicador de diversidade (total de necessidades espec�ficas / total de matr�culas de EPT)', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (61, 'Vagas oferecidas (p�blicas e privadas)', '4000', 1, '3', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (48, 'Percentual de oferta CST (matr�culas CST / Total egressos do EM)', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (48, 'Percentual de oferta CT (matr�culas CT / Total egressos do EF)', '4000', 2, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (48, 'Percentual de oferta FIC (matr�culas FIC / Total egressos do EJA)', '4000', 3, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (49, 'Indicador de  atendimento', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (50, 'Percentual de atendimento', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (51, 'Percentual de gestores capacitados', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (57, 'Taxa de gratuidade e custo aluno', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (58, 'Qualidade dos egressos da gradua��o', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (58, 'Qualidade dos egressos da produ��o cient�fica', '4000', 3, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (59, 'N�vel de inclus�o de alunos das classes C, D e E nas IES (inclusive ind�genas)', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (59, 'N�vel de inclus�o de alunos com defici�ncia funcional nas IES', '4000', 2, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (60, 'Taxa de sucesso', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (61, 'Taxa de ocupa��o (p�blica e privada)', '4000', 2, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (62, 'Qualidade de forma��o do egresso', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (62, 'Demanda regional', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (63, 'N�mero de publica��es em revistas nacionais e internacionais indexadas (ponderada)', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (63, 'Recursos do FINEP destinado ao financiamento', '4000', 2, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (64, 'Grau de satisfa��o do usu�rio', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (64, 'Produ��o cient�fica', '4000', 2, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (64, 'Qualidade da gest�o', '4000', 3, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (65, 'Numero de bolsas gradua��o (PROUNI,PET,etc)', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (65, 'Numero de bolsas p�s-gradua��o (CAPES+CNPQ', '4000', 2, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (66, 'Propor��o de estudantes de gradua��o e p�s-gradua��o beneficiados pelos programas acad�micos', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (67, 'Propor��o de estudantes de gradua��o e p�s-gradua��o beneficiados pelos programas acad�micos', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (68, 'N�mero de acordos nacionais firmados', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (68, 'N�mero de acordos internacionais firmados', '4000', 2, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (68, 'N�mero de pessoas beneficiadas', '4000', 3, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (69, 'N�mero de professores graduados', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (69, 'N�mero de professores concluintes  de programas de educa��o continuada', '4000', 2, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (70, 'N�mero de professores formados', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (70, 'N�mero de cursos por men��o (1 a 5)', '4000', 2, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (71, 'Participa��o de mestres/ doutores por curso/IES', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (71, 'Sal�rio real m�dio do prof. Universit�rio', '4000', 2, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (71, 'Carga hor�ria', '4000', 3, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (72, 'N�mero de projetos de pesquisa com o exterior', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (72, 'Publica��o de pesquisas em revistas indexadas', '4000', 2, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (86, 'Taxa de atendimento das demandas para as tem�ticas da diversidade atendidas', '4000', 2, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (87, 'Propor��o dos alunos egressos dos cursos de alfabetiza��o que ingressam no EJA', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (89, 'Taxa de crescimento da diversifica��o dos materiais para diversidade', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (89, 'Taxa de crescimento do n�mero de escolas atendidas pelos recursos pedag�gicos, did�ticos e tecnol�gicos para diversidade', '4000', 2, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (90, 'Propor��o de escolas com amplia��o de jornada', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (88, 'Taxa de crescimento do n�mero cursos de forma��o para as tem�ticas da diversidade ofertados pelas universidades p�blicas', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (88, 'Taxa de crescimento do n�mero de grupos de pesquisa para diversidade nas universidades p�blicas', '4000', 2, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (19, 'Taxa de expans�o da rede', '4000', 1, '3', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, '1');
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (67, 'Campi implantados/ munic�pios; quantidade de cursos', '4000', 2, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (11, 'Taxa de disponibiliza��o de insumos educacionais  (por n�veis e modalidades)', '4000', 1, '2', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (26, 'Percentual de escolas qualificadas para diversidade', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (15, 'Taxa de escolas com projeto pol�tico pedag�gico', '4000', 1, '2', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (6, 'Taxa de aprova��o', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (25, 'N�mero �ndice de diretrizes estabelecidas', '4000', 1, '3', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (27, 'N�mero �ndice de pol�ticas publicadas', '4000', 1, '2', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (20, 'Percentual de pedidos das escolas atendidos', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, '1');
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (5, 'Taxa de evas�o', '4000', 2, '2', 'xxxxxxxxxxxxx', '3', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, '1');
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (1, 'Taxa de escolariza��o da popula��o (por n�veis e modalidades)', '4000', 1, '1', 'xxxxxxxxxxxxx', '4', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (4, 'Taxa de cobertura ', '4000', 1, '1', 'xxxxxxxxxxxxx', '2', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (5, 'Taxa de assiduidade', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (9, 'Taxa de oferta de vagas (por n�veis e modalidades) � matr�cula x oferta', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (10, 'R$ por aluno  (per capta e por regi�o)', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (12, 'Percentual de pedidos atendidos por aluno por disciplina (indicadores devem ser fornecidos pelo FNDE, diretamente do MEC e de acervo.', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (42, 'Indice de atendimento com recursos did�ticos �s escolas ', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (21, 'Taxa de ades�o aos programas do MEC', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (22, 'Taxa aluno por professor por n�vel/modalidade e regi�o', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (13, 'Gestores capacitados', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (14, 'Percentual de professores licenciados', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (23, 'N�mero de tecnologias disponibilizadas', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (43, '�ndice de atendimento da popula��o em EPT', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (43, '�ndice de atendimento da popula��o interessada em EPT ', '4000', 2, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (38, '�ndice de atendimento do PAR', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (46, '�ndice de concluintes dos cursos de EPT', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (56, '�ndice de escolas com laborat�rios, equipamentos e tecnologias adequadas ', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (52, '�ndice de matr�culas em PROEJA', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (47, '�ndice de reconhecimentos', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (53, '�ndice de supervis�o', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);
INSERT INTO tb_indicador (cd_objetivo, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_fonte, dc_periodicidade, dc_formula, dc_apurado, dc_observacao, cd_desatualizado) VALUES (34, '�ndice salarial nacional dos professores', '4000', 1, '1', 'xxxxxxxxxxxxx', '1', 'INEP', 'Bianual', 'N�mero de Escolas qualificadas / N�mero de Escolas', '3000', NULL, NULL);


--
-- TOC entry 2932 (class 0 OID 56718903)
-- Dependencies: 2479
-- Data for Name: tb_indicador_funcao; Type: TABLE DATA; Schema: mec_painel; Owner: -
--

INSERT INTO tb_indicador_funcao (cd_indicador_funcao, cd_acao, cd_ponto_controle, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_apurado, cd_desatualizado) VALUES (1, 1, 1, 'indicador pc 1', '100', 1, '1', 'xxxxxx', '2', '100', '1');
INSERT INTO tb_indicador_funcao (cd_indicador_funcao, cd_acao, cd_ponto_controle, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_apurado, cd_desatualizado) VALUES (3, 1, NULL, 'indicador acao 1', '50', 3, '2', 'xxxx', '1', '110', NULL);
INSERT INTO tb_indicador_funcao (cd_indicador_funcao, cd_acao, cd_ponto_controle, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_apurado, cd_desatualizado) VALUES (4, 1, NULL, 'indicador acao 2', '230', 2, '1', 'xxxx', '1', '110', NULL);
INSERT INTO tb_indicador_funcao (cd_indicador_funcao, cd_acao, cd_ponto_controle, nm_indicador, dc_meta, nr_ordem_apresentacao, cd_status, url_indicador, cd_tendencia, dc_apurado, cd_desatualizado) VALUES (2, 1, 1, 'indicador pc 2', '200', 2, '3', 'xxxxx', '3', '100', NULL);


--
-- TOC entry 2922 (class 0 OID 43225697)
-- Dependencies: 2452
-- Data for Name: tb_mapa; Type: TABLE DATA; Schema: mec_painel; Owner: -
--

INSERT INTO tb_mapa (cd_mapa, in_corporativo, nm_mapa) VALUES ('3', 'N', 'Educa��o Profissional e Tecnol�gica');
INSERT INTO tb_mapa (cd_mapa, in_corporativo, nm_mapa) VALUES ('5', 'N', 'Alfabetiza��o, Educa��o Continuada e Diversidade');
INSERT INTO tb_mapa (cd_mapa, in_corporativo, nm_mapa) VALUES ('1', 'S', 'MEC');
INSERT INTO tb_mapa (cd_mapa, in_corporativo, nm_mapa) VALUES ('2', 'N', 'Educa��o B�sica');
INSERT INTO tb_mapa (cd_mapa, in_corporativo, nm_mapa) VALUES ('4', 'N', 'Educa��o Superior');


--
-- TOC entry 2924 (class 0 OID 43225728)
-- Dependencies: 2454
-- Data for Name: tb_objetivo; Type: TABLE DATA; Schema: mec_painel; Owner: -
--

INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (4, 'Atender a demanda (matr�cula)', 1, '1', 2, '1');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (1, 'Garantir o direito � educa��o a toda a popula��o', 1, '1', 1, '1');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (2, 'Atingir qualidade na educa��o', 2, '1', 1, '2');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (3, 'Universalizar o ensino', 3, '1', 1, '3');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (5, 'Atingir frequ�ncia e perman�ncia', 2, '1', 2, '1');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (6, 'Alcan�ar a aprendizagem e a capacita��o do aluno', 3, '1', 2, '2');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (7, 'Atender com qualidade as especificidades dos diversos segmentos da popula��o', 4, '1', 2, '3');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (8, 'Atender a demanda da popula��o exclu�da', 5, '1', 2, '3');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (9, 'Ofertar vagas que assegurem o acesso da popula��o', 1, '1', 3, '1');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (10, 'Propiciar alimenta��o', 2, '1', 3, '1');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (11, 'Propiciar transporte', 3, '1', 3, '1');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (12, 'Disponibilizar recursos did�ticos', 4, '1', 3, '1');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (13, 'Capacitar gestores', 5, '1', 3, '2');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (14, 'Formar professores', 6, '1', 3, '2');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (15, 'Desenvolver o projeto pedag�gico', 7, '1', 3, '2');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (16, 'Propiciar acesso � popula��o exclu�da', 8, '1', 3, '3');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (17, 'Disponibilizar recursos pedag�gicos did�ticos e tecnol�gicos destinados �s especificidades', 9, '1', 3, '3');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (18, 'Formar professores para a diversidade', 10, '1', 3, '3');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (19, 'Expandir rede f�sica', 1, '1', 4, '1');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (20, 'Disponibilizar equipamentos', 2, '1', 4, '1');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (21, 'Articular com as inst�ncias estaduais e municipais ', 3, '1', 4, '1');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (22, 'Disponibilizar professores', 4, '1', 4, '1');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (23, 'Desenvolver e disponibilizar tecnologia educacionais', 5, '1', 4, '2');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (24, 'Disponibilizar conte�do did�tico', 6, '1', 4, '2');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (25, 'Normatizar e supervisionar diretrizes curriculares', 7, '1', 4, '2');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (26, 'Qualificar a rede f�sica com cobertura regional', 8, '1', 4, '3');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (27, 'Elaborar pol�ticas para atender demandas diferenciadas', 9, '1', 4, '3');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (28, 'Assegurar acesso � Educa��o B�sica', 1, '2', 1, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (29, 'Assegurar qualidade na educa��o b�sica', 2, '2', 1, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (30, 'Atender a demanda de matr�culas considerando as especificidades frequentes na diversidade humana (inclus�o)', 1, '2', 2, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (31, 'Garantir frequ�ncia e perman�ncia', 2, '2', 2, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (32, 'Promover aprendizagem e forma��o do estudante', 3, '2', 2, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (33, 'Garantir professores qualificados e capacitados em quantidade suficiente', 1, '2', 3, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (34, 'Implantar o piso salarial para os professores', 2, '2', 3, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (35, 'Garantir funcion�rios n�o-docentes, qualificados e capacitados em quantidade suficiente', 3, '2', 3, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (36, 'Garantir meio de transporte acess�vel � escola', 4, '2', 3, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (37, 'Ampliar o financiamento da educa��o b�sica', 5, '2', 3, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (38, 'Aperfei�oar o planejamento e gest�o educacional articulada', 6, '2', 3, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (39, 'Garantir rede f�sica adequada', 7, '2', 3, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (40, 'Promover educa��o alimentar e ofertar alimenta��o saud�vel nas escolas de educa��o b�sica', 8, '2', 3, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (41, 'Desenvolver e fornecer tecnologia educacional a todos', 9, '2', 3, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (42, 'Garantir recursos did�ticos e pedag�gicos', 10, '2', 3, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (43, 'Garantir EPT ao p�blico de jovens,  adultos  e trabalhadores', 1, '3', 1, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (44, 'Assegurar qualidade  na EPT', 2, '3', 1, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (45, 'Democratizar o acesso �s institui��es de EPT', 1, '3', 2, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (46, 'Elevar a frequ�ncia e reduzir a evas�o', 2, '3', 2, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (47, 'Garantir condi��es para reconhecimento de saberes/ compet�ncias n�o formais para trabalho', 3, '3', 2, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (48, 'Prover EPT nas modalidades presencial e EAD', 1, '3', 3, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (49, 'Reordenar e reorientar o papel da Rede Federal de EPT', 2, '3', 3, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (50, 'Ofertar est�gios EPT', 3, '3', 3, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (51, 'Capacitar gestores de EPT nos diversos sistemas de ensino', 4, '3', 3, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (52, 'Ampliar a oferta de PROEJA', 5, '3', 3, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (53, 'Regular e supervisionar oferta de cursos superiores  de tecnologia', 6, '3', 3, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (54, 'Colaborar e cooperar com sistemas ensino<br>-Aprimorar a regula��o e supervis�o da EP<br>- Aprimorar a regula��o e supervis�o da ET', 7, '3', 3, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (55, 'Assegurar a forma��o inicial e continuada de professores e servidores para a EPT', 8, '3', 3, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (56, 'Disponibilizar conte�dos, laborat�rios, equipamentos e tecnologias', 9, '3', 3, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (57, 'Monitorar acordo Sistema S', 10, '3', 3, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (58, 'Garantir a forma��o qualificada de pessoas para o desenvolvimento nacional', 1, '4', 1, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (59, 'Promover  acesso e inclus�o', 2, '4', 1, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (60, 'Promover perman�ncia', 1, '4', 2, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (61, 'Ampliar vagas', 2, '4', 2, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (62, 'Implementar resid�ncias em sa�de de acordo com as necessidades regionais do pa�s', 3, '4', 2, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (63, 'Ampliar produ��o cient�fica- tecnol�gica', 4, '4', 2, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (64, 'Garantir HU de qualidade', 5, '4', 2, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (65, 'Disponibilizar bolsas', 1, '4', 3, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (66, 'Ampliar programas acad�micos (Gradua��o e p�s', 2, '4', 3, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (67, 'Expandir a rede f�sica federal (IPES) (Rede de Ensino Superior', 3, '4', 3, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (68, 'Estabelecer parcerias  Estados  e Munic�pios', 4, '4', 3, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (69, 'Fortalecer Sistema Nacional de Forma��o de Professores', 5, '4', 3, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (70, 'Estabelecer um  sistema de EAD para forma��o de professores com qualidade', 6, '4', 3, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (71, 'Qualificar e valorizar professores', 7, '4', 3, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (72, 'Promover  projetos conjuntos de pesquisa com o exterior', 8, '4', 3, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (73, 'Estabelecer parcerias nacional e internacional', 9, '4', 3, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (74, 'Ampliar a infra-estrutura tecnol�gica da RNP', 10, '4', 3, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (75, 'Fomentar um sistema de p�s-gradua��o de qualidade', 11, '4', 3, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (76, 'Realizar processos de avalia��o, supervis�o e regula��o do ensino superior', 12, '4', 3, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (77, 'Garantir o direito  � educa��o, �s popula��es ind�genas, quilombolas e em vulnerabilidade social-educacional ', 1, '5', 1, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (78, 'Promover a equidade pela valoriza��o da diversidade �tnica, racial, cultural, de g�nero, de orienta��o sexual, geracional, territorial', 2, '5', 1, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (79, 'Atender a demanda da popula��o pouco contemplada em pol�ticas educacionais', 1, '5', 2, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (80, 'Estabelecer rela��o escola - comunidade', 2, '5', 2, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (81, 'Ofertar vagas �s popula��es exclu�das do acesso � escolaridade e garantir sua perman�ncia', 1, '5', 3, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (82, 'Implementar nos cursos de forma��o (inicial e continuada) as diretrizes espec�ficas para a diversidade (Ex.: EJA, povos ind�genas, dentre outros)', 2, '5', 3, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (83, 'Expandir  e qualificar a rede f�sica', 3, '5', 3, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (84, 'Disponibilizar material espec�fico, adequado �s popula��es, neoleitores jovens e adultos', 4, '5', 3, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (85, 'Formar professores para a diversidade', 5, '5', 3, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (86, 'Articular entre as esferas federativas para o enfrentamento dos temas da desigualdade e da diversidade', 6, '5', 3, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (87, 'Promover a continuidade da escolariza��o dos jovens e adultos egressos dos cursos de alfabetiza��o', 7, '5', 3, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (88, 'Ampliar as tem�ticas da diversidade como campos de conhecimento e forma��o nas universidades p�blicas', 8, '5', 3, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (89, 'Disponibilizar os recursos pedag�gicos, did�ticos e tecnol�gicos para a diversidade', 9, '5', 3, '4');
INSERT INTO tb_objetivo (cd_objetivo, nm_objetivo, nr_ordem_apresentacao, cd_mapa, cd_perspectiva, cd_tema) VALUES (90, 'Consolidar educa��o integral como pol�tica p�blica', 10, '5', 3, '4');


--
-- TOC entry 2923 (class 0 OID 43225713)
-- Dependencies: 2453
-- Data for Name: tb_perspectiva; Type: TABLE DATA; Schema: mec_painel; Owner: -
--

INSERT INTO tb_perspectiva (cd_mapa, cd_perspectiva, nm_perspectiva, nr_ordem_apresentacao) VALUES ('1', 1, 'Resultados Estrat�gicos', 1);
INSERT INTO tb_perspectiva (cd_mapa, cd_perspectiva, nm_perspectiva, nr_ordem_apresentacao) VALUES ('1', 2, 'P�blico-alvo', 2);
INSERT INTO tb_perspectiva (cd_mapa, cd_perspectiva, nm_perspectiva, nr_ordem_apresentacao) VALUES ('1', 3, 'Processos Final�sticos', 3);
INSERT INTO tb_perspectiva (cd_mapa, cd_perspectiva, nm_perspectiva, nr_ordem_apresentacao) VALUES ('1', 4, 'Processos Meio', 4);
INSERT INTO tb_perspectiva (cd_mapa, cd_perspectiva, nm_perspectiva, nr_ordem_apresentacao) VALUES ('2', 5, 'Resultados Estrat�gicos', 1);
INSERT INTO tb_perspectiva (cd_mapa, cd_perspectiva, nm_perspectiva, nr_ordem_apresentacao) VALUES ('2', 6, 'P�blico-alvo', 2);
INSERT INTO tb_perspectiva (cd_mapa, cd_perspectiva, nm_perspectiva, nr_ordem_apresentacao) VALUES ('2', 7, 'Processos', 3);
INSERT INTO tb_perspectiva (cd_mapa, cd_perspectiva, nm_perspectiva, nr_ordem_apresentacao) VALUES ('3', 8, 'Resultados Estrat�gicos', 1);
INSERT INTO tb_perspectiva (cd_mapa, cd_perspectiva, nm_perspectiva, nr_ordem_apresentacao) VALUES ('3', 9, 'P�blico-alvo', 2);
INSERT INTO tb_perspectiva (cd_mapa, cd_perspectiva, nm_perspectiva, nr_ordem_apresentacao) VALUES ('3', 10, 'Processos', 3);
INSERT INTO tb_perspectiva (cd_mapa, cd_perspectiva, nm_perspectiva, nr_ordem_apresentacao) VALUES ('4', 11, 'Resultados Estrat�gicos', 1);
INSERT INTO tb_perspectiva (cd_mapa, cd_perspectiva, nm_perspectiva, nr_ordem_apresentacao) VALUES ('4', 12, 'P�blico-alvo', 2);
INSERT INTO tb_perspectiva (cd_mapa, cd_perspectiva, nm_perspectiva, nr_ordem_apresentacao) VALUES ('4', 13, 'Processos', 3);
INSERT INTO tb_perspectiva (cd_mapa, cd_perspectiva, nm_perspectiva, nr_ordem_apresentacao) VALUES ('5', 14, 'Resultados Estrat�gicos', 1);
INSERT INTO tb_perspectiva (cd_mapa, cd_perspectiva, nm_perspectiva, nr_ordem_apresentacao) VALUES ('5', 15, 'P�blico-alvo', 2);
INSERT INTO tb_perspectiva (cd_mapa, cd_perspectiva, nm_perspectiva, nr_ordem_apresentacao) VALUES ('5', 16, 'Processos', 3);


--
-- TOC entry 2929 (class 0 OID 54371016)
-- Dependencies: 2473
-- Data for Name: tb_ponto_controle; Type: TABLE DATA; Schema: mec_painel; Owner: -
--

INSERT INTO tb_ponto_controle (cd_ponto_controle, nm_ponto_controle, cd_acao, cd_ator, cd_tempo, nr_ordem_apresentacao) VALUES (1, '1. Providencia Documenta��o', 1, 1, 1, 1);
INSERT INTO tb_ponto_controle (cd_ponto_controle, nm_ponto_controle, cd_acao, cd_ator, cd_tempo, nr_ordem_apresentacao) VALUES (2, '6. Providencia Ajustes', 1, 1, 4, 1);
INSERT INTO tb_ponto_controle (cd_ponto_controle, nm_ponto_controle, cd_acao, cd_ator, cd_tempo, nr_ordem_apresentacao) VALUES (4, '2. Acessa Sistema de Conv�nios', 1, 3, 1, 1);
INSERT INTO tb_ponto_controle (cd_ponto_controle, nm_ponto_controle, cd_acao, cd_ator, cd_tempo, nr_ordem_apresentacao) VALUES (5, '3. Cadastra Proposta', 1, 3, 2, 1);
INSERT INTO tb_ponto_controle (cd_ponto_controle, nm_ponto_controle, cd_acao, cd_ator, cd_tempo, nr_ordem_apresentacao) VALUES (6, '4. Verifica Conformidade', 1, 3, 2, 2);
INSERT INTO tb_ponto_controle (cd_ponto_controle, nm_ponto_controle, cd_acao, cd_ator, cd_tempo, nr_ordem_apresentacao) VALUES (8, '7. Registra no SIMEC Projetos', 1, 3, 5, 1);
INSERT INTO tb_ponto_controle (cd_ponto_controle, nm_ponto_controle, cd_acao, cd_ator, cd_tempo, nr_ordem_apresentacao) VALUES (7, '5. Comunica n�o Conformidade', 1, 3, 3, 1);
INSERT INTO tb_ponto_controle (cd_ponto_controle, nm_ponto_controle, cd_acao, cd_ator, cd_tempo, nr_ordem_apresentacao) VALUES (9, '8. Emite Parecer', 1, 3, 5, 2);
INSERT INTO tb_ponto_controle (cd_ponto_controle, nm_ponto_controle, cd_acao, cd_ator, cd_tempo, nr_ordem_apresentacao) VALUES (10, '9. Identifica Unidade', 1, 3, 6, 3);
INSERT INTO tb_ponto_controle (cd_ponto_controle, nm_ponto_controle, cd_acao, cd_ator, cd_tempo, nr_ordem_apresentacao) VALUES (11, '10. Analisa Proposta', 1, 4, 6, 1);
INSERT INTO tb_ponto_controle (cd_ponto_controle, nm_ponto_controle, cd_acao, cd_ator, cd_tempo, nr_ordem_apresentacao) VALUES (12, '11. Solicita Ajustes', 1, 4, 7, 1);
INSERT INTO tb_ponto_controle (cd_ponto_controle, nm_ponto_controle, cd_acao, cd_ator, cd_tempo, nr_ordem_apresentacao) VALUES (3, '12. Providencia Ajustes', 1, 1, 7, 1);
INSERT INTO tb_ponto_controle (cd_ponto_controle, nm_ponto_controle, cd_acao, cd_ator, cd_tempo, nr_ordem_apresentacao) VALUES (13, '13. Elabora Parecer', 1, 4, 8, 1);


--
-- TOC entry 2925 (class 0 OID 43226328)
-- Dependencies: 2455
-- Data for Name: tb_tema; Type: TABLE DATA; Schema: mec_painel; Owner: -
--

INSERT INTO tb_tema (cd_tema, nm_tema) VALUES ('1', 'Acesso e Perman�ncia');
INSERT INTO tb_tema (cd_tema, nm_tema) VALUES ('2', 'Qualidade');
INSERT INTO tb_tema (cd_tema, nm_tema) VALUES ('3', 'Equidade');


--
-- TOC entry 2917 (class 2606 OID 54542831)
-- Dependencies: 2475 2475 2475
-- Name: tb_acao_ator_pkey; Type: CONSTRAINT; Schema: mec_painel; Owner: -; Tablespace: 
--

ALTER TABLE ONLY tb_acao_ator
    ADD CONSTRAINT tb_acao_ator_pkey PRIMARY KEY (cd_acao, cd_ator);


--
-- TOC entry 2911 (class 2606 OID 43285030)
-- Dependencies: 2458 2458 2458
-- Name: tb_acao_objetivo_pkey; Type: CONSTRAINT; Schema: mec_painel; Owner: -; Tablespace: 
--

ALTER TABLE ONLY tb_acao_objetivo
    ADD CONSTRAINT tb_acao_objetivo_pkey PRIMARY KEY (cd_objetivo, cd_acao);


--
-- TOC entry 2909 (class 2606 OID 43284979)
-- Dependencies: 2457 2457
-- Name: tb_acao_pkey; Type: CONSTRAINT; Schema: mec_painel; Owner: -; Tablespace: 
--

ALTER TABLE ONLY tb_acao
    ADD CONSTRAINT tb_acao_pkey PRIMARY KEY (cd_acao);


--
-- TOC entry 2915 (class 2606 OID 54371083)
-- Dependencies: 2474 2474
-- Name: tb_ator_pkey; Type: CONSTRAINT; Schema: mec_painel; Owner: -; Tablespace: 
--

ALTER TABLE ONLY tb_ator
    ADD CONSTRAINT tb_ator_pkey PRIMARY KEY (cd_ator);


--
-- TOC entry 2919 (class 2606 OID 56718907)
-- Dependencies: 2479 2479
-- Name: tb_indicador_funcao_pkey; Type: CONSTRAINT; Schema: mec_painel; Owner: -; Tablespace: 
--

ALTER TABLE ONLY tb_indicador_funcao
    ADD CONSTRAINT tb_indicador_funcao_pkey PRIMARY KEY (cd_indicador_funcao);


--
-- TOC entry 2907 (class 2606 OID 43226360)
-- Dependencies: 2456 2456 2456
-- Name: tb_indicador_pkey; Type: CONSTRAINT; Schema: mec_painel; Owner: -; Tablespace: 
--

ALTER TABLE ONLY tb_indicador
    ADD CONSTRAINT tb_indicador_pkey PRIMARY KEY (cd_objetivo, nm_indicador);


--
-- TOC entry 2899 (class 2606 OID 43225700)
-- Dependencies: 2452 2452
-- Name: tb_mapa_pkey; Type: CONSTRAINT; Schema: mec_painel; Owner: -; Tablespace: 
--

ALTER TABLE ONLY tb_mapa
    ADD CONSTRAINT tb_mapa_pkey PRIMARY KEY (cd_mapa);


--
-- TOC entry 2903 (class 2606 OID 43225731)
-- Dependencies: 2454 2454
-- Name: tb_objetivo_pkey; Type: CONSTRAINT; Schema: mec_painel; Owner: -; Tablespace: 
--

ALTER TABLE ONLY tb_objetivo
    ADD CONSTRAINT tb_objetivo_pkey PRIMARY KEY (cd_objetivo);


--
-- TOC entry 2901 (class 2606 OID 43225716)
-- Dependencies: 2453 2453 2453
-- Name: tb_perspectiva_pkey; Type: CONSTRAINT; Schema: mec_painel; Owner: -; Tablespace: 
--

ALTER TABLE ONLY tb_perspectiva
    ADD CONSTRAINT tb_perspectiva_pkey PRIMARY KEY (cd_mapa, cd_perspectiva);


--
-- TOC entry 2913 (class 2606 OID 54371019)
-- Dependencies: 2473 2473
-- Name: tb_ponto_controle_pkey; Type: CONSTRAINT; Schema: mec_painel; Owner: -; Tablespace: 
--

ALTER TABLE ONLY tb_ponto_controle
    ADD CONSTRAINT tb_ponto_controle_pkey PRIMARY KEY (cd_ponto_controle);


--
-- TOC entry 2905 (class 2606 OID 43226331)
-- Dependencies: 2455 2455
-- Name: tb_tema_pkey; Type: CONSTRAINT; Schema: mec_painel; Owner: -; Tablespace: 
--

ALTER TABLE ONLY tb_tema
    ADD CONSTRAINT tb_tema_pkey PRIMARY KEY (cd_tema);


-- Completed on 2009-11-03 10:32:06

--
-- PostgreSQL database dump complete
--

