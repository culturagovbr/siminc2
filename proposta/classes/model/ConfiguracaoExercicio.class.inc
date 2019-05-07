<?php

include_once APPRAIZ . "wssof/classes/Ws_UnidadesOrcamentariasDto.inc";
include_once APPRAIZ . "wssof/classes/Ws_ProgramasDto.inc";

class ConfiguracaoExercicio extends Modelo {

    public function verificarExercicios()
    {
        $sql = "select prsano from planointerno.programacaoexercicio where prsstatus = 'A' order by prsano;";
        $exercicios = $this->carregar($sql);

        include_once APPRAIZ . 'planointerno/modulos/principal/configuracaoExercicio/exercicios.inc';
    }

    public function verificarProgramacaoSiop($exercicio)
    {
        $sql = "select 'Bases Geográficas' as descricao, count(*) qtd from wssof.ws_basesgeograficasdto
                union all 
                select 'Esferas' as descricao, count(*) from wssof.ws_esferasdto
                union all 
                select 'Funções' as descricao, count(*) from wssof.ws_funcoesdto where exercicio = '$exercicio'
                union all 
                select 'Macro Desafios' as descricao, count(*) from wssof.ws_macrodesafiosdto
                union all 
                select 'Momentos' as descricao, count(*) from wssof.ws_momentosdto
                union all 
                select 'Perfis' as descricao, count(*) from wssof.ws_perfisdto
                union all 
                select 'Periodicidade' as descricao, count(*) from wssof.ws_periodicidadesdto
                union all 
                select 'Produtos' as descricao, count(*) from wssof.ws_produtosdto
                union all 
                select 'Regiões' as descricao, count(*) from wssof.ws_regioesdto
                union all 
                select 'Subfunções' as descricao, count(*) from wssof.ws_subfuncoesdto where exercicio = '$exercicio'
                union all 
                select 'Tipos de ação' as descricao, count(*) from wssof.ws_tiposacaodto
                union all 
                select 'Tipos de inclusão' as descricao, count(*) from wssof.ws_tiposinclusaodto
                union all 
                select 'Tipos de programa' as descricao, count(*) from wssof.ws_tiposprogramadto where exercicio = '$exercicio'
                union all 
                select 'Unidades de medida' as descricao, count(*) from wssof.ws_unidadesmedidadto
                union all 
                select 'Unidades de medida Indicador' as descricao, count(*) from wssof.ws_unidadesmedidaindicadordto where exercicio = '$exercicio'";
        $aTabelasSIOP = $this->carregar($sql);

        $sql = "select 'Área Cultural' as descricao, count(*) qtd from planointerno.areacultural where prsano = '$exercicio'
                union all
                select 'Segmento' as descricao, count(*) from planointerno.segmento where areid in (select areid  from planointerno.areacultural where prsano = '$exercicio')
                union all
                select 'Enquadramento de Despesa' as descricao, count(*) from planointerno.enquadramentodespesa where prsano = '$exercicio'
                union all
                select 'Iniciativa PPA' as descricao, count(*) from planointerno.ppainiciativa where prsano = '$exercicio'
                union all
                select 'Item Manutenção' as descricao, count(*) from planointerno.manutencaoitem where prsano = '$exercicio'
                union all
                select 'Sub-Item Manutenção' as descricao, count(*) from planointerno.manutencaosubitem where prsano = '$exercicio'
                union all
                select 'Modalidade de Pactuação' as descricao, count(*) from planointerno.tipoinstrumento where prsano = '$exercicio'
                union all
                select 'Prioridade Ministerial' as descricao, count(*) from planointerno.prioridadeministra where prsano = '$exercicio'
                union all
                select 'Objetivo PPA' as descricao, count(*) from planointerno.ppaobjetivo where prsano = '$exercicio'
                union all
                select 'Programa MinC' as descricao, count(*) from planointerno.programaprioritario where prsano = '$exercicio'
                union all
                select 'Produto PO' as descricao, count(*) from planointerno.produtopo where prsano = '$exercicio'
                union all
                select 'Unidade de Medida PO' as descricao, count(*) from planointerno.unidademedidapo where prsano = '$exercicio'
                union all
                select 'Produto PI' as descricao, count(*) from planointerno.produtopi where prsano = '$exercicio'
                union all
                select 'Unidade de Medida PI' as descricao, count(*) from planointerno.unidademedidapi where prsano = '$exercicio'
                union all
                select 'Tipo de Detalhamento PLOA' as descricao, count(*) from planointerno.tipodetalhamentoploa where prsano = '$exercicio'
                union all
                select 'Unidade Orçamentária' as descricao, count(*) from planointerno.unidadeorcamentaria where prsano = '$exercicio'
                union all
                select 'Sub-Unidade Orçamentária' as descricao, count(*) from planointerno.subunidadeorcamentaria where unoid in (select unoid from planointerno.unidadeorcamentaria where prsano = '$exercicio')
                union all
                select 'Vínculo de Usuário com Sub-Unidade' as descricao, count(*) from planointerno.usuarioresponsabilidade where suoid in (select suoid from planointerno.subunidadeorcamentaria where unoid in (select unoid from planointerno.unidadeorcamentaria where prsano = '$exercicio'))
                union all
                select 'Meta MinC' as descricao, count(*) from planointerno.iniciativaestrategica where prsano = '$exercicio'
                union all
                select 'Meta PNC' as descricao, count(*) from planointerno.metapnc where prsano = '$exercicio'
                union all
                select 'Meta PPA' as descricao, count(*) from planointerno.metappa where prsano = '$exercicio'
                union all
                select 'Vínculo de Meta PPA com Objetivo' as descricao, count(*) from planointerno.metappaobjetivo where mppid in (select mppid from planointerno.metappa where prsano = '$exercicio')
                union all
                select 'Vínculo de Meta PPA com Unidade' as descricao, count(*) from planointerno.metappaunidade where mppid in (select mppid from planointerno.metappa where prsano = '$exercicio')";
        $aTabelasReplicar = $this->carregar($sql);

        $sql = "select 'Ações' as descricao, count(*) qtd from planointerno.ppaacao where prsano = '$exercicio'
                union all
                select 'Localizadores' as descricao, count(*) from planointerno.ppalocalizador where prsano = '$exercicio'
                union all
                select 'Planos Orçamentários' as descricao, count(*) from planointerno.ppaplanoorcamentario where prsano = '$exercicio'
                union all
                select 'Programas' as descricao, count(*) from planointerno.ppaprograma where prsano = '$exercicio'
                union all
                select 'Indicadores' as descricao, 0
                union all
                select 'MOMENTO UNIDADE' as descricao, count(*) from planointerno.ppamomentounidade where prsano = '$exercicio'";
        $aExtratoSIOP = $this->carregar($sql);

        include_once APPRAIZ . 'planointerno/modulos/principal/configuracaoExercicio/programacao-siop.inc';
    }

    public function verificarMomentoUnidade($exercicio, $momento)
    {
        $momid = $this->pegaUm("select momid from planointerno.momento where prsano = '{$exercicio}' and momstatus = 'A' order by momid limit 1");

        $sql = "select uno.unocod || ' - ' || uno.unonome unidade, suo.suocod || ' - ' || suo.suonome subunidade, aca.acacod, prg.prgcod, loc.loccod, plo.plocod
                from planointerno.ppamomentounidade pmu
                    inner join planointerno.ppaprograma prg on prg.prgid = pmu.prgid
                    inner join planointerno.ppaacao aca on aca.acaid = pmu.acaid and aca.prsano = '$exercicio'
                    inner join planointerno.ppalocalizador loc on loc.locid = pmu.locid and loc.prsano = '$exercicio'
                    left join planointerno.ppaplanoorcamentario plo on plo.ploid = pmu.ploid and plo.prsano = '$exercicio'
                    inner join planointerno.subunidadeorcamentaria suo on suo.suoid = pmu.suoid 
                    inner join planointerno.unidadeorcamentaria uno on uno.unoid = suo.unoid AND uno.prsano = '$exercicio'
                where momid = $momid
                and pmu.prsano = '$exercicio'
                order by uno.unonome, suo.suonome, aca.acacod, plo.plocod, prg.prgcod, loc.loccod";

        $dadosSiop = $this->carregar($sql);
        $dadosSiop = $dadosSiop ? $dadosSiop : [];

        include_once APPRAIZ . 'planointerno/modulos/principal/configuracaoExercicio/momento-unidade.inc';
    }

    public function verificarLimiteMomentoUnidade($exercicio)
    {
        $sql = "select uno.unocod || ' - ' || uno.unonome as unidade, suo.suosigla || ' - ' || suo.suonome as subunidade
                from planointerno.subunidadeorcamentaria suo
                        INNER JOIN planointerno.unidadeorcamentaria uno ON uno.unoid = suo.unoid and uno.prsano = '$exercicio' and unostatus = 'A'
                where suo.suostatus = 'A'
                and uno.unocod != '42902'
                order by uno.unonome, suo.suonome;";

        $dadosUnidades = $this->carregar($sql);
        $dadosUnidades = $dadosUnidades ? $dadosUnidades : [];
        include_once APPRAIZ . 'planointerno/modulos/principal/configuracaoExercicio/limite-momento-unidade.inc';
    }

    public function verificarAcaoEnquadramento($exercicio)
    {
        $sql = "select acacod, acaid, acanome from planointerno.ppaacao where prsano = '$exercicio' and acastatus = 'A' order by acacod, acanome";
        $dadosAcoes = $this->carregar($sql);
        $dadosAcoes = $dadosAcoes ? $dadosAcoes : [];

        $sql = "select  eqdnome, eqdid,
                        case 
                            when eqdsigla = 'F' or eqdsigla = 'M' then '1'
                            else 2
                        end ordem
                from planointerno.enquadramentodespesa 
                where prsano = '$exercicio' 
                and eqdstatus = 'A' 
                order by ordem, eqdnome";

        $dadosEnquadramento = $this->carregar($sql);
        $dadosEnquadramento = $dadosEnquadramento ? $dadosEnquadramento : [];

        $dadosAcaoEnquadramento = (new AcaoEnquadramento())->recuperarPorExercicio($exercicio);

        include_once APPRAIZ . 'planointerno/modulos/principal/configuracaoExercicio/acao-enquadramento.inc';
    }

    public function importarTabelasExercicio($exercicio, $momento)
    {
        set_time_limit(0);

        $siopQualitativo = new SiopWebServiceQualitativo();

        // Importando Tabelas de Apoio
        // @TODO Verificar se para esse momento de proposta é necessário fazer essa carga. Se sim, quais tabelas, uma vez que o método abaixo traz todas que o WS fornece
//        $this->importarTabelasApoio($exercicio);

        // Importando Indicadores
        // @TODO Verificar se serão resgatados os indicadores
//        $this->importarIndicadores($exercicio, 3000);

        /*
         * TESTAR A PARTIR DAQUI
         * */

        // Importando Unidades Orçamentárias
        $this->importatUnidadeOrcamentaria($exercicio, 42000);

        // Importando Usuário Responsabilidade
        $this->importatUsuarioResponsabilidade($exercicio);

        // Importando Dados Ano Anterior
        $this->importatDadosAnoAnterior($exercicio);

        // Importando Programação Completa
        $this->importarProgramacaoCompleta($exercicio, $momento);

        die('FIM');

//        $dadosImportar = $siopQualitativo->obterObjetivosPorPrograma($exercicio, 9000, 2069);
//        $dadosImportar = $siopQualitativo->obterMomentoCarga(2017);
    }

    public function importarTabelasSof($exercicio, $dadosImportar, $codigoMomento = null)
    {
        $aTabelasCarga = [
            'basesGeograficasDTO' => 'Wssof_Ws_BasesGeograficasDto',
            'esferasDTO' => 'Wssof_Ws_EsferasDto',
            'funcoesDTO' => 'Wssof_Ws_FuncoesDto',
            'macroDesafiosDTO' => 'Wssof_Ws_MacroDesafiosDto',
            'momentosDTO' => 'Wssof_Ws_MomentosDto',
            'perfisDTO' => 'Wssof_Ws_PerfisDto',
            'periodicidadesDTO' => 'Wssof_Ws_PeriodicidadesDto',
            'produtosDTO' => 'Wssof_Ws_ProdutosDto',
            'regioesDTO' => 'Wssof_Ws_RegioesDto',
            'subFuncoesDTO' => 'Wssof_Ws_SubFuncoesDto',
            'tiposAcaoDTO' => 'Wssof_Ws_TiposAcaoDto',
            'tiposInclusaoDTO' => 'Wssof_Ws_TiposInclusaoDto',
            'tiposProgramaDTO' => 'Wssof_Ws_TiposProgramaDto',
            'unidadesMedidaDTO' => 'Wssof_Ws_UnidadesMedidaDto',
            'unidadesMedidaIndicadorDTO' => 'Wssof_Ws_UnidadesMedidaIndicadorDto',

            'agendasSamDTO' => 'Wssof_Ws_AgendaSamDto',
            'orgaosDTO' => 'Wssof_Ws_OrgaosDto',
            'acoesDTO' => 'Wssof_Ws_AcoesDto',
            'localizadoresDTO' => 'Wssof_Ws_LocalizadoresDto',
            'planosOrcamentariosDTO' => 'Wssof_Ws_PlanosOrcamentariosDto',

            'unidadesOrcamentariasDto' => 'Wssof_Ws_UnidadesOrcamentariasDto',
            'programasDto' => 'Wssof_Ws_ProgramasDto',
            'indicadoresDto' => 'Wssof_Ws_IndicadoresDto',
            'objetivosDto' => 'Wssof_Ws_ObjetivosDto',
        ];

        $siopQualitativo = new SiopWebServiceQualitativo();

        foreach($dadosImportar->return as $tabela => $aRegistros){
            if(key_exists($tabela, $aTabelasCarga)){

                $arquivo = str_replace('Wssof_', '', $aTabelasCarga[$tabela]);
                if(file_exists(APPRAIZ . "wssof/classes/{$arquivo}.inc")){
                    include_once APPRAIZ . "wssof/classes/{$arquivo}.inc";
                }

                // Excluindo registros antigos
                $model = new $aTabelasCarga[$tabela];
                switch ($tabela) {
                    case 'acoesDTO':
                    case 'localizadoresDTO':
                    case 'planosOrcamentariosDTO':
                        $model->excluirVarios("exercicio = '{$exercicio}' and codigomomento = '{$codigoMomento}'");
                        break;
                    case 'orgaosDTO':
                    case 'unidadesOrcamentariasDto':
                    case 'programasDto':
                    case 'indicadoresDto':
                        $model->excluirVarios("exercicio = '{$exercicio}'");
                        break;
                    default:
                        $model->excluirVarios(" true ");
                        break;
                }
                unset($model);

                foreach($aRegistros as $count => $dados){
                    $model = new $aTabelasCarga[$tabela];
                    $model->realizarCarga($dados, true);
                    $this->commit();
                    unset($model);
                }
            }
        }
    }

    public function importatUnidadeOrcamentaria($exercicio, $codigoUnidade)
    {
        // @TODO retirar linha abaixo após realizar todos os testes
        $exercicio = 2019;
        $anoAnterior = $exercicio - 1;

        // Inserindo subunidades
        $dadosBancoSubUnidade = (new SubUnidadeOrcamentaria())->recuperarTodos('*', array("uno.prsano = '$anoAnterior'", "unostatus = 'A'", "suostatus = 'A'"));

//        // Inserindo unidades de acordo com ano anterior
        foreach($dadosBancoSubUnidade as $dados){

            $modelSubUnidadeOrcamentaria = new SubUnidadeOrcamentaria();
            if(empty($aUnoid[$dados['unocod']])){

                $modelUnidadeOrcamentaria = new UnidadeOrcamentaria();
                $modelUnidadeOrcamentaria->popularDadosObjeto($dados);

                $modelUnidadeOrcamentaria->unoid = null;
                $modelUnidadeOrcamentaria->prsano = $exercicio;

                $modelUnidadeOrcamentaria->salvar();
                $unoid = $aUnoid[$dados['unocod']] = $modelUnidadeOrcamentaria->unoid;

                unset($modelUnidadeOrcamentaria);
            }

            $modelSubUnidadeOrcamentaria->unoid = $modelUnidadeOrcamentaria->unoid;
            $modelSubUnidadeOrcamentaria->popularDadosObjeto($dados);

            $modelSubUnidadeOrcamentaria->suoid = null;
            $modelSubUnidadeOrcamentaria->unoid = $aUnoid[$dados['unocod']];

            $modelSubUnidadeOrcamentaria->salvar();
            $modelSubUnidadeOrcamentaria->commit();
            unset($modelUnidadeOrcamentaria);
        }
    }

    public function importatUsuarioResponsabilidade($exercicio)
    {
        // @TODO retirar linha abaixo após realizar todos os testes
        $exercicio = 2019;
        $anoAnterior = $exercicio - 1;

        $sql = "INSERT INTO planointerno.usuarioresponsabilidade ( pflcod, usucpf,  suoid )
                SELECT DISTINCT
                    ur{$anoAnterior}.pflcod,
                    usuario.usucpf,
                --	,ur{$anoAnterior}.suoid AS suoid_{$anoAnterior}
                    (
                            SELECT DISTINCT
                                suo.suoid
                            FROM planointerno.subunidadeorcamentaria suo 
                                JOIN planointerno.unidadeorcamentaria uno ON uno.unoid = suo.unoid
                            WHERE
                                uno.prsano = '{$exercicio}'
                                AND suo.suocod = suo{$anoAnterior}.suocod
                            LIMIT 1
                    ) AS suoid_{$exercicio}
                FROM (
                    SELECT
                        unicod,
                        unidsc
                    FROM
                        public.unidade
                    WHERE
                        unitpocod = 'U') AS unidadex
                    RIGHT JOIN seguranca.usuario usuario ON usuario.unicod = unidadex.unicod
                    LEFT JOIN territorios.municipio municipio ON municipio.muncod = usuario.muncod
                    INNER JOIN seguranca.usuario_sistema usuariosistema ON usuario.usucpf = usuariosistema.usucpf
                    LEFT JOIN entidade.entidade entidade ON usuario.entid = entidade.entid
                    LEFT JOIN planointerno.usuarioresponsabilidade ur{$anoAnterior} ON(
                        usuario.usucpf = ur{$anoAnterior}.usucpf
                        AND ur{$anoAnterior}.rpustatus = 'A'
                        AND ur{$anoAnterior}.suoid IN(
                            SELECT DISTINCT
                                suo.suoid
                            FROM planointerno.subunidadeorcamentaria suo
                                JOIN planointerno.unidadeorcamentaria uno ON uno.unoid = suo.unoid
                            WHERE
                                uno.prsano = '{$anoAnterior}'
                        )
                    )
                    LEFT JOIN planointerno.subunidadeorcamentaria suo{$anoAnterior} ON(ur{$anoAnterior}.suoid = suo{$anoAnterior}.suoid)
                    LEFT JOIN planointerno.usuarioresponsabilidade ur{$exercicio} ON(
                        usuario.usucpf = ur{$exercicio}.usucpf
                        AND ur{$exercicio}.rpustatus = 'A'
                        AND ur{$exercicio}.suoid IN(
                            SELECT DISTINCT
                                suo.suoid
                            FROM planointerno.subunidadeorcamentaria suo
                                JOIN planointerno.unidadeorcamentaria uno ON uno.unoid = suo.unoid
                            WHERE
                                uno.prsano = '{$exercicio}'
                        )
                    )
                WHERE
                    usunome IS NOT NULL
                    AND usuariosistema.suscod = 'A'
                    AND usuariosistema.sisid = '100'
                    AND ur{$anoAnterior}.usucpf IS NOT NULL
                    AND ur{$exercicio}.usucpf IS NULL -- 304 Com Unidades de {$anoAnterior} vinculadas e sem Unidades {$exercicio} Corrigidas
                ";

        $this->carregar($sql);
        $this->commit();
    }

    public function importatUnidadeProgramas($exercicio, $codigoMomento, $codigoUnidade)
    {
        // @TODO retirar linha abaixo após realizar todos os testes
        $exercicio = 2019;

        // Importando dados do WS e salvando na tabela temporária
        $siopQualitativo = new SiopWebServiceQualitativo();
        $dadosImportar = $siopQualitativo->obterProgramasPorOrgao($exercicio, $codigoMomento, $codigoUnidade);
        $dadosImportar->return->programasDto = $dadosImportar->return->registros;
        unset($dadosImportar->return->registros);
        $this->importarTabelasSof($exercicio, $dadosImportar);

        // Recuperando dados da tabela temporária para inserir na do sistema
        $modelWs_ProgramasDto = new Wssof_Ws_ProgramasDto();
        $dadosWs = $modelWs_ProgramasDto->recuperarTodos('*', array("exercicio = '$exercicio'"));

        // Inserindo unidades de acordo com carga do WS
        foreach($dadosWs as $dados){
            $modelPpaPrograma = new PpaPrograma();

            $dadosGravar = [
                'prsano' => $dados['exercicio'],
                'prgcod' => $dados['codigoprograma'],
                'prgnome' => $dados['titulo'],
                'prgdescricao' => substr($dados['problema'], 0, 500),
                'prgstatus' => 'A',
            ];

            $modelPpaPrograma->popularDadosObjeto($dadosGravar);
            $modelPpaPrograma->salvar();
            $modelPpaPrograma->commit();
            unset($modelPpaPrograma);
        }
    }

    public function importarIndicadores($exercicio, $codigoMomento)
    {
        // @TODO retirar linha abaixo após realizar todos os testes
        $exercicio = 2019;

        // Importando dados do WS e salvando na tabela temporária
        $siopQualitativo = new SiopWebServiceQualitativo();
        $dadosImportar = $siopQualitativo->obterIndicadoresPorPrograma($exercicio, $codigoMomento);
        $dadosImportar->return->indicadoresDto = $dadosImportar->return->registros;
        unset($dadosImportar->return->registros);
        $this->importarTabelasSof($exercicio, $dadosImportar);
    }

    public function importarTabelasApoio($exercicio)
    {
        // @TODO retirar linha abaixo após realizar todos os testes
        $exercicio = 2019;

        // Importando dados do WS e salvando na tabela temporária
        $siopQualitativo = new SiopWebServiceQualitativo();
        $dadosImportar = $siopQualitativo->obterTabelasApoio($exercicio);
        $this->importarTabelasSof($exercicio, $dadosImportar);
    }

    public function importatDadosAnoAnterior($exercicio)
    {
        // @TODO retirar linha abaixo após realizar todos os testes
        $exercicio = 2019;

        $anoAnterior = $exercicio - 1;

        // Cadastrando Objetivos
        $sql = "insert into planointerno.ppaobjetivo (prsano, objcod, objnome, objdescricao, objstatus)
                select distinct '{$exercicio}', o.objcod, o.objnome, o.objdescricao, o.objstatus
                from planointerno.ppaobjetivo o
                where prsano = '$anoAnterior' 
                and o.objstatus = 'A'";
        $this->executar($sql);

        // Cadastrando Iniciativas
        $sql = "insert into planointerno.ppainiciativa (prsano, inicod, ininome, inidescricao, inistatus)
                select distinct '{$exercicio}', i.inicod, i.ininome, i.inidescricao, i.inistatus
                from planointerno.ppainiciativa i
                where prsano = '$anoAnterior' 
                and i.inistatus = 'A'";
        $this->executar($sql);

        // Unidades Medida PO
        $sql = "insert into planointerno.unidademedidapo (prsano, uponome, upodescricao, upostatus, usucpf)
                select distinct '{$exercicio}', upo.uponome, upo.upodescricao, upo.upostatus, upo.usucpf
                from planointerno.unidademedidapo upo
                where prsano = '$anoAnterior' 
                and upostatus = 'A'";
        $this->executar($sql);

        // Produtos PO
        $sql = "insert into planointerno.produtopo (prsano, prpnome, prpdescricao, prpstatus, usucpf)
                select distinct '{$exercicio}', prp.prpnome, prp.prpdescricao, prp.prpstatus, prp.usucpf
                from planointerno.produtopo prp
                where prsano = '$anoAnterior' 
                and prpstatus = 'A'";
        $this->executar($sql);

        // Unidades de Medida PI
        $sql = "insert into planointerno.unidademedidapi (prsano,umpnome,umpdescricao,umpstatus)
                select distinct '{$exercicio}',umpnome,umpdescricao,umpstatus
                from planointerno.unidademedidapi
                where prsano = '$anoAnterior' 
                and umpstatus='A'";
        $this->executar($sql);

        // Produtos PI
        $sql = "insert into planointerno.produtopi (prsano,prinome,pridescricao,pristatus)
                select  distinct '{$exercicio}', prinome, pridescricao, pristatus
                from planointerno.produtopi
                where prsano = '$anoAnterior' 
                and pristatus='A'";
        $this->executar($sql);

        // Tipo Detalhamento PLOA
        $sql = "insert into planointerno.tipodetalhamentoploa (prsano, tpdcodigo,tpdnome,tpdstatus)
                select distinct '{$exercicio}', tpdcodigo, tpdnome, tpdstatus
                from planointerno.tipodetalhamentoploa
                where prsano = '$anoAnterior' 
                and tpdstatus='A'";
        $this->executar($sql);

        // Enquadramento de Despesa
        $sql = "insert into planointerno.enquadramentodespesa (prsano,eqdsigla,eqdnome,eqddescricao,eqdstatus,eqdliberado,eqddataprogincio,eqddataprogtermino,eqdcodigorp,tdpid)
                select '{$exercicio}', e.eqdsigla, e.eqdnome, e.eqddescricao, e.eqdstatus, e.eqdliberado, e.eqddataprogincio, e.eqddataprogtermino, e.eqdcodigorp, tatual.tdpid
                from planointerno.enquadramentodespesa e
                        left join planointerno.tipodetalhamentoploa tanterior on e.tdpid = tanterior.tdpid
                        left join planointerno.tipodetalhamentoploa tatual on tanterior.tpdcodigo = tatual.tpdcodigo and tatual.prsano = '{$exercicio}'
                where e.prsano = '$anoAnterior'";
        $this->executar($sql);

        // Área Cultural
        $sql = "insert into planointerno.areacultural (prsano,aresigla,arenome,aredescricao,arestatus)
                select distinct '{$exercicio}', aresigla, arenome, aredescricao, arestatus
                from planointerno.areacultural
                where prsano = '$anoAnterior' 
                and arestatus='A'";
        $this->executar($sql);

        // Item Manutenção
        $sql = "insert into planointerno.manutencaoitem (prsano,mainome,maidescricao,maistatus,eqdid)
                select '{$exercicio}', mai.mainome, mai.maidescricao, mai.maistatus, eqdatual.eqdid
                from planointerno.manutencaoitem mai
                        left join planointerno.enquadramentodespesa eqd on eqd.eqdid = mai.eqdid
                        left join planointerno.enquadramentodespesa eqdatual on eqdatual.eqdsigla = eqd.eqdsigla and eqdatual.prsano = '{$exercicio}'
                where mai.prsano = '$anoAnterior'";
        $this->executar($sql);

        // Sub Item Manutenção
        $sql = "insert into planointerno.manutencaosubitem (maiid, prsano, masnome, masdescricao, masstatus)
                select maiatual.maiid, '{$exercicio}', mas.masnome, mas.masdescricao, mas.masstatus
                from planointerno.manutencaosubitem mas
                        inner join planointerno.manutencaoitem mai on mai.maiid = mas.maiid
                        inner join planointerno.manutencaoitem maiatual on maiatual.mainome = mai.mainome and maiatual.prsano = '{$exercicio}'
                where mas.prsano = '$anoAnterior'";
        $this->executar($sql);

        // Segmento
        $sql = "insert into planointerno.segmento (segdsc,segstatus,areid)
                select seg.segdsc, seg.segstatus, areatual.areid
                from planointerno.segmento seg
                        inner join planointerno.areacultural are on are.areid = seg.areid and are.prsano = '$anoAnterior'
                        inner join planointerno.areacultural areatual on areatual.aresigla = are.aresigla and areatual.prsano = '{$exercicio}'";
        $this->executar($sql);

        // Modalidade de Pactuação (Tipo Instrumento)
        $sql = "insert into planointerno.tipoinstrumento (prsano, tpidsc,tpistatus)
                select distinct '{$exercicio}', tpidsc,tpistatus
                from planointerno.tipoinstrumento
                where prsano = '$anoAnterior' 
                and tpistatus='A'";
        $this->executar($sql);

        // Prioridade Ministerial
        $sql = "insert into planointerno.prioridadeministra (prsano, prmdsc,prmstatus,prmcodigo)
                select distinct '{$exercicio}', prmdsc,prmstatus,prmcodigo
                from planointerno.prioridadeministra
                where prsano = '$anoAnterior' 
                and prmstatus='A'";
        $this->executar($sql);

        // Iniciativa Estratégica (Meta Minc)
        $sql = "insert into planointerno.iniciativaestrategica (prsano,inecod,inenome,inedescricao,inestatus)
                select distinct '{$exercicio}', inecod,inenome,inedescricao,inestatus
                from planointerno.iniciativaestrategica
                where prsano = '$anoAnterior' 
                and inestatus='A'";
        $this->executar($sql);

        // Meta PPA
        $sql = "insert into planointerno.metappa (prsano ,mppcod ,mppnome ,mppdescricao ,mppstatus ,mppquantificavel ,mppregionalizado ,mppacumulativo)
                select distinct '{$exercicio}', mppcod ,mppnome ,mppdescricao ,mppstatus ,mppquantificavel ,mppregionalizado ,mppacumulativo
                from planointerno.metappa 
                where prsano='$anoAnterior' 
                and mppstatus='A'";
        $this->executar($sql);

        // Meta PNC
        $sql = "insert into planointerno.metapnc (prsano,mpncod,mpnnome,mpndescricao,mpnstatus,mpnquantificavel,mpnregionalizado,mpnacumulativo)
                select distinct '{$exercicio}', mpncod, mpnnome, mpndescricao, mpnstatus, mpnquantificavel, mpnregionalizado, mpnacumulativo
                from planointerno.metapnc
                where prsano = '$anoAnterior' 
                and mpnstatus='A'";
        $this->executar($sql);

        // Programa MinC
        $sql = "insert into planointerno.programaprioritario (prsano ,pprnome ,pprdescricao ,pprstatus)
                select distinct '{$exercicio}', pprnome ,pprdescricao ,pprstatus 
                from planointerno.programaprioritario 
                where prsano='$anoAnterior' 
                and pprstatus='A'";
        $this->executar($sql);

        // Vínculo de Meta com Objetivo
        $sql = "insert into planointerno.metappaobjetivo(mppid,objid)
                select mppatual.mppid, objatual.objid
                from planointerno.metappaobjetivo mpo
                        inner join planointerno.metappa mpp on mpp.mppid = mpo.mppid and mpp.prsano = '$anoAnterior'
                        inner join planointerno.metappa mppatual on mppatual.mppcod = mpp.mppcod and mppatual.prsano = '{$exercicio}'
                        inner join planointerno.ppaobjetivo obj on obj.objid = mpo.objid and obj.prsano = '$anoAnterior'
                        inner join planointerno.ppaobjetivo objatual on objatual.objcod = obj.objcod and objatual.prsano = '{$exercicio}'";
        $this->executar($sql);

        // Vínculo de Meta com Unidade
        $sql = "insert into planointerno.metappaunidade (mppid, suoid, mpuvalor, mpustatus, mpuvalorprevisto)
                select mppatual.mppid, suoatual.suoid, mpu.mpuvalor, mpu.mpustatus, mpu.mpuvalorprevisto 
                from planointerno.metappaunidade mpu
                        inner join planointerno.metappa mpp on mpp.mppid = mpu.mppid and mpp.prsano = '$anoAnterior'
                        inner join planointerno.metappa mppatual on mppatual.mppcod = mpp.mppcod and mppatual.prsano = '{$exercicio}'
                        inner join (
                                select * from planointerno.subunidadeorcamentaria su1 inner join planointerno.unidadeorcamentaria uno on uno.unoid = su1.unoid and uno.prsano = '$anoAnterior'
                        ) suo on suo.suoid = mpu.suoid 
                        inner join (
                                select * from planointerno.subunidadeorcamentaria su2 inner join planointerno.unidadeorcamentaria uno on uno.unoid = su2.unoid and uno.prsano = '{$exercicio}'
                        ) suoatual on suoatual.suocod = suo.suocod and suoatual.unofundo = suo.unofundo";
        $this->executar($sql);
        $this->commit();
    }

    public function importarProgramacaoCompleta($exercicio, $codigoMomento)
    {
        // @TODO retirar linha abaixo após realizar todos os testes
        $exercicio = $exercicioFixo = 2018;

        // Importando dados do WS e salvando na tabela temporária
        $siopQualitativo = new SiopWebServiceQualitativo();
        $dadosImportar = $siopQualitativo->obterProgramacaoCompleta($exercicio, $codigoMomento);
        $this->importarTabelasSof($exercicio, $dadosImportar, $codigoMomento);

        // @TODO retirar exercícioFixo e exercício. Está desta forma para simular anos anteriores
        // @TODO depois basta substuituir $exercícioFixo por $exercício
        $exercicio = 2019;

        // Cadastrando Ações
        $sql = "insert into planointerno.ppaacao (prsano, acacod, acanome, acadescricao, acastatus ) 
                select distinct '{$exercicio}', a.codigoacao, a.titulo , SUBSTRING(a.descricao, 1, 500), 'A'
                from wssof.ws_acoesdto a
                where exercicio = '$exercicioFixo'";
        $this->executar($sql);

        // Cadastrando Programas
        $sql = "insert into planointerno.ppaprograma(prsano, prgcod, prgnome, prgdescricao, prgstatus)
                select distinct '{$exercicio}', p.codigoprograma, p.titulo,  SUBSTRING(p.problema, 1, 500), 'A'
                from wssof.ws_programasdto p
                where exercicio = '$exercicioFixo'";
        $this->executar($sql);

        // Cadastrando Localizadores
        $sql = "insert into planointerno.ppalocalizador (prsano, loccod, locnome, locdescricao, locstatus)
                select distinct '{$exercicio}', l.codigolocalizador, l.descricao, l.descricao, 'A'
                from wssof.ws_localizadoresdto l
                where exercicio = '$exercicioFixo'";
        $this->executar($sql);

        // Plano Orçamentário
        $sql = "insert into planointerno.ppaplanoorcamentario (prsano, plocod, plonome, plodescricao, plostatus, prpid, upoid, acaid)
                select distinct '{$exercicio}', po.planoorcamentario, po.titulo, po.detalhamento, 'A', pr.prpid , un.upoid, aca.acaid
                from wssof.ws_planosorcamentariosdto po
                        left join (
                                select prp.prpid, p.codigoproduto
                                from planointerno.produtopo prp
                                        inner join wssof.ws_produtosdto p on LOWER(prp.prpnome) = lower(p.descricao)
                                where prsano = '{$exercicio}'        
                        ) pr on pr.codigoproduto = po.codigoproduto::int
                        left join (
                                select upo.upoid, u.codigounidademedida
                                from planointerno.unidademedidapo upo
                                        inner join wssof.ws_unidadesmedidadto u on LOWER(uponome) = lower(u.descricao)
                                where prsano = '{$exercicio}'        
                        ) un on un.codigounidademedida = po.codigounidademedida
                        left join wssof.ws_acoesdto a on a.identificadorunico = po.identificadorunicoacao and a.exercicio = '$exercicioFixo'
                        inner join planointerno.ppaacao aca on aca.acacod = a.codigoacao and aca.prsano = '{$exercicio}'
                where po.exercicio = '$exercicioFixo'";
        $this->executar($sql);

        // Selecionando Momento
        $momid = $this->pegaUm("select momid from planointerno.momento where prsano = '{$exercicio}' and momstatus = 'A' order by momid limit 1");

        // Momento Unidade
        $sql = "insert into planointerno.ppamomentounidade (suoid, prsano, momid, prgid, acaid, locid, objid, iniid, ploid, pmustatus)
                select suo.suoid, '{$exercicio}' prsano, $momid momid, prg.prgid, aca.acaid, loc.locid, obj.objid, ini.iniid, plo.ploid, 'A' pmustatus
                from wssof.ws_acoesdto a
                        left join wssof.ws_localizadoresdto l on l.identificadorunicoacao = a.identificadorunico and l.exercicio = '$exercicioFixo' and l.codigomomento = '$codigoMomento'
                        left join wssof.ws_planosorcamentariosdto po on po.identificadorunicoacao = a.identificadorunico and po.exercicio = '$exercicioFixo' and po.codigomomento = '$codigoMomento'
                        inner join planointerno.unidadeorcamentaria uno on uno.unocod = a.codigoorgao and uno.prsano = '{$exercicio}' and unostatus = 'A'
                        inner join planointerno.subunidadeorcamentaria suo on suo.unoid = uno.unoid
                        inner join planointerno.ppaprograma prg on prg.prgcod = a.codigoprograma and prg.prsano = '{$exercicio}'
                        inner join planointerno.ppaacao aca on aca.acacod = a.codigoacao and aca.prsano = '{$exercicio}'
                        inner join planointerno.ppalocalizador loc on loc.loccod = l.codigolocalizador and loc.prsano = '{$exercicio}'        
                        left join planointerno.ppaobjetivo obj on obj.objcod = a.codigoobjetivo and obj.prsano = '{$exercicio}'
                        left join planointerno.ppainiciativa ini on ini.inicod = a.codigoiniciativa and ini.prsano = '{$exercicio}'
                        inner join planointerno.ppaplanoorcamentario plo on plo.plocod = po.planoorcamentario and plo.acaid = aca.acaid and plo.prsano = '{$exercicio}'        
                where a.exercicio = '$exercicioFixo'
                and a.codigomomento = '$codigoMomento'";
        $this->executar($sql);

        $this->commit();
    }

    public function atualizarDadosSof($exercicio, $codigoMomento, $item)
    {
        $acao = $item == 1;
        $localizador = $item == 2;
        $po = $item == 3;

        $siopQualitativo = new SiopWebServiceQualitativo();
        $dadosImportar = $siopQualitativo->obterProgramacaoCompleta($exercicio, $codigoMomento, false, false, false, false, false, $acao, $localizador, false, false, $po, false, false);

        $this->importarTabelasSof($exercicio, $dadosImportar, $codigoMomento);
    }

    public function resgatarDadosAnoAnterior()
    {
        $sql = "
                insert into public.metappa (mppdsc, mppcod, mppnome, mppstatus, prsano, mppquantificavel, mppquantidade, mppanualizavel)
                select t.mppdsc, t.mppcod, t.mppnome, t.mppstatus, '2019', t.mppquantificavel, t.mppquantidade, t.mppanualizavel
                from public.metappa t
                where prsano = '2018';
                
                insert into public.metapnc (mpndsc, mpnstatus, mpncod, mpnnome, prsano)
                select t.mpndsc, t.mpnstatus, t.mpncod, t.mpnnome, '2019'
                from public.metapnc t
                where prsano = '2018';
                
                insert into planejamento.enquadramento_despesa (eqdcod, eqddsc, eqdano, eqdstatus, mpneid, eqdstreduzido, eqdrp)
                select t.eqdcod, t.eqddsc, 2019, t.eqdstatus, t.mpneid, t.eqdstreduzido, t.eqdrp
                from planejamento.enquadramento_despesa t
                where eqdano = '2018';
                
                insert into monitora.pi_produto (prsano, pprnome, pprdescricao, pprstatus, tipo)
                select '2019', t.pprnome, t.pprdescricao, t.pprstatus, t.tipo
                from monitora.pi_produto t
                where t.prsano = '2018';
                
                insert into monitora.pi_unidade_medida (prsano, pumnome, pumdescricao, pumstatus)
                select '2019', t.pumnome, t.pumdescricao, t.pumstatus
                from monitora.pi_unidade_medida t
                where t.prsano = '2018';
                
                insert into planejamento.area_cultural (arccod, arcdsc, arcano, arcstatus)
                select t.arccod, t.arcdsc, '2019', t.arcstatus
                from planejamento.area_cultural t
                where t.arcano = '2018';
                
                insert into planejamento.segmento_cultural (needsc
                , neeano, neestatus, arceid, neecod)
                select t.needsc, '2019', t.neestatus, novo.arceid, t.neecod
                from planejamento.segmento_cultural t
                        inner join planejamento.area_cultural old on old.arceid = t.arceid and old.arcstatus = 'A'
                        inner join planejamento.area_cultural novo on novo.arccod = old.arccod and novo.arcano = '2019' and novo.arcstatus = 'A'
                where t.neeano = '2018'
                and t.neestatus = 'A';
                
                insert into planacomorc.manutencaoitem (prsano,mainome,maidescricao,maistatus,eqdid)
                select '2019', t.mainome, t.maidescricao, t.maistatus, novo.eqdid
                from planacomorc.manutencaoitem t
                        inner join planejamento.enquadramento_despesa old on old.eqdid = t.eqdid and old.eqdstatus = 'A'
                        inner join planejamento.enquadramento_despesa novo on novo.eqdcod = old.eqdcod and novo.eqdano = '2019' and novo.eqdstatus = 'A'
                where t.prsano = '2018'
                and t.maistatus = 'A'
                ;
                
                insert into planacomorc.manutencaosubitem (maiid, prsano, masnome, masdescricao, masstatus)
                select novo.maiid, '2019', t.masnome, t.masdescricao, t.masstatus
                from planacomorc.manutencaosubitem t
                        inner join planacomorc.manutencaoitem old on old.maiid = t.maiid and old.maistatus = 'A'
                        inner join planacomorc.manutencaoitem novo on novo.mainome = old.mainome and novo.prsano = '2019' and novo.maistatus = 'A'
                where t.prsano = '2018'
                and t.masstatus = 'A'
                ;
                
                insert into public.objetivoppa (prsano, oppdsc, oppnome, oppcod, oppstatus)
                select '2019', t.oppdsc, t.oppnome, t.oppcod, t.oppstatus
                from public.objetivoppa t
                where t.prsano = '2018';
                
                
                insert into public.iniciativappa (oppid, ippdsc, ippnome, ippcod, ippstatus, prsano)
                select novo.oppid, t.ippdsc, t.ippnome, t.ippcod, t.ippstatus, '2019'
                from public.iniciativappa t
                        inner join public.objetivoppa old on old.oppid = t.oppid and old.oppstatus = 'A'
                        inner join public.objetivoppa novo on novo.oppcod = old.oppcod and novo.prsano = '2019' and novo.oppstatus = 'A'
                where t.prsano = '2018'
                and t.ippstatus = 'A';
                
                insert into public.indicadorpnc (mpnid, ipndsc, ipnstatus, prsano, ipncod)
                select novo.mpnid, t.ipndsc, t.ipnstatus, '2019', t.ipncod
                from public.indicadorpnc t
                        inner join public.metapnc old on old.mpnid = t.mpnid and old.mpnstatus = 'A'
                        inner join public.metapnc novo on novo.mpncod = old.mpncod and novo.prsano = '2019' and novo.mpnstatus = 'A'
                where t.prsano = '2018'
                and t.ipnstatus  = 'A';
                ;
                
                insert into planejamento.categoria_apropriacao (capcod, capdsc, capano, capstatus, capsiconv)
                select t.capcod, t.capdsc, '2019', t.capstatus, t.capsiconv
                from planejamento.categoria_apropriacao t
                where t.capano = '2018'
                and t.capstatus = 'A'
                ;
                
                
                insert into public.unidadeorcamentaria (prsano, unocod, unonome, unostatus, unocodigopi, unofundo, unosigla, unocodorcamento)
                select '2019', t.unocod, t.unonome, t.unostatus, t.unocodigopi, t.unofundo, t.unosigla, t.unocodorcamento
                from public.unidadeorcamentaria t
                where t.prsano = '2018'
                and t.unostatus = 'A'
                -- and not (unocod = '42902' and unofundo = 'f')
                ;
                
                insert into public.subunidadeorcamentaria (unoid, suocod, suosigla, suonome, suostatus, suoflaglimiteliberado, suocodigopi)
                select novo.unoid, t.suocod, t.suosigla, t.suonome, t.suostatus, t.suoflaglimiteliberado, t.suocodigopi
                from public.subunidadeorcamentaria t
                        inner join public.unidadeorcamentaria old on old.unoid = t.unoid and old.unostatus = 'A' and old.prsano = '2018'
                        inner join public.unidadeorcamentaria novo on novo.unocod = old.unocod and novo.unofundo = old.unofundo and novo.prsano = '2019' and novo.unostatus = 'A'
                where t.suostatus = 'A'
                ;
                
                insert into planacomorc.unidadegestora_limite (lmuvlr, lmudtcadastro, usucpf, lmustatus, lmuflgliberado, prsano, ungcod, unoid)
                select t.lmuvlr, '2018-05-05 17:17:17', '00704963159', t.lmustatus, t.lmuflgliberado, '2019', t.ungcod, t.unoid
                from planacomorc.unidadegestora_limite t
                where t.prsano = '2018'
                ;
                
                insert into spo.subunidademetappa (suoid, mppid)
                select novo.suoid, novo1.mppid 
                from spo.subunidademetappa t
                        inner join public.vw_subunidadeorcamentaria old on old.suoid = t.suoid and old.prsano = '2018'
                        inner join public.vw_subunidadeorcamentaria novo on novo.suocod = old.suocod and novo.unofundo = old.unofundo  and novo.prsano = '2019'
                        
                        inner join public.metappa old1 on old1.mppid = t.mppid and old1.prsano = '2018'
                        inner join public.metappa novo1 on novo1.mppcod = old1.mppcod and novo1.prsano = '2019'
                ;
                
                insert into spo.subunidademetapnc (suoid, mpnid)
                select novo.suoid, novo1.mpnid
                from spo.subunidademetapnc t
                        inner join public.vw_subunidadeorcamentaria old on old.suoid = t.suoid and old.prsano = '2018'
                        inner join public.vw_subunidadeorcamentaria novo on novo.suocod = old.suocod and novo.unofundo = old.unofundo and novo.prsano = '2019'
                        
                        inner join public.metapnc old1 on old1.mpnid = t.mpnid and old1.prsano = '2018'
                        inner join public.metapnc novo1 on novo1.mpncod = old1.mpncod and novo1.prsano = '2019'
                ;
                
                insert into spo.subunidadeindicadorpnc (suoid, ipnid)
                select novo.suoid, novo1.ipnid
                from spo.subunidadeindicadorpnc t
                        inner join public.vw_subunidadeorcamentaria old on old.suoid = t.suoid and old.prsano = '2018'
                        inner join public.vw_subunidadeorcamentaria novo on novo.suocod = old.suocod and novo.unofundo = old.unofundo and novo.prsano = '2019'
                        
                        inner join public.indicadorpnc old1 on old1.ipnid = t.ipnid and old1.prsano = '2018'
                        inner join public.indicadorpnc novo1 on novo1.ipncod = old1.ipncod and novo1.prsano = '2019'
                ;        

                insert into spo.ptressubunidade (ptrid, suoid, ptrdotacaocapital, ptrdotacaocusteio)
                select novo1.ptrid, novo.suoid, t.ptrdotacaocapital, t.ptrdotacaocusteio
                from spo.ptressubunidade t
                        inner join public.vw_subunidadeorcamentaria old on old.suoid = t.suoid and old.prsano = '2018'
                        inner join public.vw_subunidadeorcamentaria novo on novo.suocod = old.suocod and novo.unofundo = old.unofundo and novo.prsano = '2019'
                
                        inner join monitora.vw_ptres old1 on old1.ptrid = t.ptrid and old1.ptrano = '2018'
                        inner join monitora.vw_ptres novo1 
                                on  novo1.acacod = old1.acacod 
                                and novo1.loccod = old1.loccod 
                                and novo1.prgcod = old1.prgcod 
                                -- and novo1.acaobjetivocod = old1.acaobjetivocod 
                                and novo1.unicod = old1.unicod 
                                and novo1.plocod = old1.plocod 
                                and novo1.ptrano = '2019';                

                insert into public.objetivometappa (oppid, mppid, mpodata)
                select novo.oppid, novo1.mppid, '2018-05-06 13:31:13'
                from public.objetivometappa t
                        inner join public.objetivoppa old on old.oppid = t.oppid and old.prsano = '2018'
                        inner join public.objetivoppa novo on novo.oppcod = old.oppcod and novo.prsano = '2019'
                
                        inner join public.metappa old1 on old1.mppid = t.mppid and old1.prsano = '2018'
                        inner join public.metappa novo1  on  novo1.mppcod = old1.mppcod and novo1.prsano = '2019'
                ;  
                
                insert into public.naturezadespesa (gndcod, ndpano, ndpcod, ndpdsc, ndpstatus, edpcod, mapcod, ctecod, sbecod, cagcod)
                select t.gndcod, '2019', t.ndpcod, t.ndpdsc, t.ndpstatus, t.edpcod, t.mapcod, t.ctecod, t.sbecod, t.cagcod
                from public.naturezadespesa t
                where t.ndpano = '2018'
                ;      
        ";

        $updateRpPtres = "  select 'update monitora.ptres set irpcod = '''|| t.irpcod ||''' where acacod = '''|| t.acacod ||''' and  loccod = '''|| t.loccod ||''' and  prgcod = '''|| t.prgcod ||''' and  unicod = '''|| t.unicod ||''' and  plocod = '''|| t.plocod ||''' and ptrano = ''2019'';  ' rp, t.irpcod
                            from monitora.ptres t
                            where t.ptrano = '2018'
                            and coalesce(irpcod, '6') != '6'";
    }

}