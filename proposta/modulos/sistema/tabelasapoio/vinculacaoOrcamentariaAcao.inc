<?php
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include_once APPRAIZ . "includes/library/simec/Helper/FlashMessage.php";
require_once(APPRAIZ . 'includes/library/simec/Listagem.php');

$fm = new Simec_Helper_FlashMessage('planacomorc/tc/vinculacaoOrcamentaria');

if (isset($_GET['del_exercicio']) && isset($_GET['id'])) {

    $sql_del = "DELETE FROM planacomorc.vinculacaoacaoestrategica WHERE vacid = {$_GET['id']}";
    if ($db->executar($sql_del)) {
        $db->commit();
        alertlocation(array(
            'alert' => 'Registro apagado com sucesso!'
          , 'location' => 'planacomorc.php?modulo=sistema/tabelasapoio/vinculacaoOrcamentaria&acao=A&listar=true'
        ));
    } else {
        alertlocation(array(
            'alert' => 'Erro ao tentar apagar!'
          , 'location' => 'planacomorc.php?modulo=sistema/tabelasapoio/vinculacaoOrcamentaria&acao=A&listar=true'
        ));
    }
}

/**
 * Rotina para deletar
 * @return Redirect
 */
if (isset($_GET['section']) && isset($_GET['id']) && isset($_GET['vaeid'])) {

    switch ($_GET['section']) {
        case 'acao':
            $strSQL = "DELETE FROM planacomorc.vinculacaoestrategicaacoes WHERE veaid=%d";
            break;
        case 'subacao':
            $sbacod = $db->pegaUm("SELECT sbacod FROM planacomorc.vinculacaoestrategicasubacoes WHERE vesid={$_GET['id']}");
            $strSQL = "DELETE FROM planacomorc.vinculacaoestrategicasubacoes WHERE sbacod='%s' AND vaeid='{$_GET['vaeid']}'";
            break;
        case 'pi';
            $strSQL = "DELETE FROM planacomorc.vinculcaoestrategicapis WHERE vepid=%d AND vaeid='{$_GET['vaeid']}'";
            break;
        case 'po';
            $strSQL = "DELETE FROM planacomorc.vinculacaoestrategicapos WHERE vpoid=%d AND vaeid='{$_GET['vaeid']}'";
            break;
        default:
            throw new Exception('nenhuma ação foi passada via requisição');
    }

    $_GET['id'] = ($_GET['section'] == 'subacao') ? $sbacod : $_GET['id'];
    $stmt = sprintf($strSQL, $_GET['id']);
    $db->executar($stmt);
    $db->commit();
    alertlocation(array(
        'alert' => 'Registro foi apagado com sucesso!'
      , 'location' => 'planacomorc.php?modulo=sistema/tabelasapoio/vinculacaoOrcamentaria&acao=A&vaeid='.$_GET['vaeid']
    ));
}

/**
 * Carrega registro ativo para vinculo
 */
if (isset($_GET['clicouexercicio'])) {
    if (!empty($_GET['vacid'])) {
        $resultSet = $db->pegaLinha("
            SELECT  
                v.vacid,
                vaeid,
                v.acaid,
                exercicio,
                v.vaetituloorcamentario,
                ve.vaedescricao
            FROM planacomorc.vinculacaoacaoestrategicaexercicio ve
            JOIN planacomorc.vinculacaoacaoestrategica v using(vacid)
            WHERE exercicio = {$_GET['clicouexercicio']}
            AND vacid = {$_GET['vacid']}");
        if (empty($resultSet)) {
            $vet = $db->pegaLinha("insert into planacomorc.vinculacaoacaoestrategicaexercicio (exercicio, vacid) values ('{$_GET['clicouexercicio']}', '{$_GET['vacid']}') returning vaeid ");
            $resultSet = $db->pegaLinha("SELECT * FROM planacomorc.vinculacaoacaoestrategicaexercicio join planacomorc.vinculacaoacaoestrategica using(vacid)  WHERE vaeid = " . $vet['vaeid']);
            $_GET['vacid'] = $resultSet['vacid'];
        }
    }
}
if (empty($resultSet)){
    if (isset($_GET['vaeid'])&& !empty($_GET['vaeid'])) {
   // ver($_GET['vacid'],d); 
        $resultSet = $db->pegaLinha("SELECT * FROM planacomorc.vinculacaoacaoestrategicaexercicio join planacomorc.vinculacaoacaoestrategica using(vacid)  WHERE vaeid = {$_GET['vaeid']}");
    } elseif (!empty($_GET['vacid'])) {
        // tratando erro pg_query() [function.pg-query]: Query failed: ERROR:  syntax error at end of input LINE 1: ...oestrategica using(vacid) WHERE exercicio='2013' and vacid = ^
        if(!empty($vet['vaeid']))
        {
            $recVacid = $db->pegaLinha("SELECT vacid FROM planacomorc.vinculacaoacaoestrategicaexercicio join planacomorc.vinculacaoacaoestrategica using(vacid) WHERE vaeid = " . $_GET['vaeid']);
                        //ver($recVacid,d);//caso perder o $_GET['vacid'] recupera via select.
            $vacid = $_GET['vacid'] ? $_GET['vacid'] : $recVacid;
        }
        $resultSet = $db->pegaLinha("SELECT * FROM planacomorc.vinculacaoacaoestrategicaexercicio join planacomorc.vinculacaoacaoestrategica using(vacid) WHERE exercicio='2013' and vacid = {$_GET['vacid']}");
    }
}

/**
 * Checa se a ação na foi adicionada para cada categoria de Ação Orçamentária
 */
if (isAjax() && isset($_GET['verify'])) {

    header('Content-Type: application/json');

    switch ($_GET['section']) {
        case 'acao':
            if (isset($_GET['a']) && isset($_GET['vaeid'])) {
                $wheres = select_values_acoes($_GET['a'], $_GET['vaeid']);
                $sql = "SELECT * FROM planacomorc.vinculacaoestrategicaacoes WHERE %s";
                foreach ($wheres as $where) {
                    $instruction = sprintf($sql, $where);
                    if ($rs = $db->carregar($instruction)) {
                        echo simec_json_encode(current($rs));
                    }
                }
            }
        break;
        case 'subacao':
            if (isset($_GET['uos']) && isset($_GET['a']) && isset($_GET['vaeid'])) {
                $wheres = select_values_subacoes($_GET['uos'], $_GET['a'], $_GET['vaeid']);
                $sql = "SELECT * FROM planacomorc.vinculacaoestrategicasubacoes WHERE %s";
                foreach ($wheres as $where) {
                    $instruction = sprintf($sql, $where);
                    if ($rs = $db->carregar($instruction)) {
                        echo simec_json_encode(current($rs));
                    }
                }
            }
        break;
        case 'pi':
            if (isset($_GET['uos']) && isset($_GET['a']) && isset($_GET['vaeid'])) {

                $subacao = substr($_GET['a'], 1, 4);
                $wheres = select_values_subacoes($_GET['uos'], $subacao, $_GET['vaeid']);
                $sql = "SELECT * FROM planacomorc.vinculacaoestrategicasubacoes WHERE %s";
                foreach ($wheres as $where) {
                    $instruction = sprintf($sql, $where);
                    if ($rs = $db->carregar($instruction)) {
                        echo simec_json_encode(current($rs));
                    }
                }

                $where = select_values_pi($_GET['a'], $_GET['vaeid']);
                $sql = "SELECT * FROM planacomorc.vinculcaoestrategicapis WHERE %s";
                $instruction = sprintf($sql, $where);
                if ($rs = $db->carregar($instruction)) {
                    echo simec_json_encode(current($rs));
                }
            }
        break;
        case 'po':
            if (isset($_GET['a']) && isset($_GET['vaeid'])) {
                $wheres = select_values_po($_GET['a'], $_GET['vaeid']);
                $sql = "SELECT * FROM planacomorc.vinculacaoestrategicapos WHERE %s";
                foreach ($wheres as $where) {
                    $instruction = sprintf($sql, $where);
                    if ($rs = $db->carregar($instruction)) {
                        echo simec_json_encode(current($rs));
                    }
                }
            }
        break;
    }

    exit;
}

/**
 * Rotina que faz o vínculo das ações
 * alimenta o crud dos vinculos
 */
if (isset($_GET['section'])) {
    switch ($_GET['section']) {
        case 'acao':
            if (isset($_GET['acoes']) && isset($_GET['vaeid'])) {
                $inserts = insert_values_acoes($_GET['acoes'], $_GET['vaeid']);
                $SQLInsert = "INSERT INTO planacomorc.vinculacaoestrategicaacoes(acacod, vaeid, unicod) VALUES {$inserts}";
                $db->executar($SQLInsert);
                $db->commit();                    
                
            }
            break;

        case 'subacao':
            if (isset($_GET['acoes']) && isset($_GET['vaeid'])) {
                $inserts = insert_values_subacoes($_GET['uos'], $_GET['acoes'], $_GET['vaeid']);
                $SQLInsert = "INSERT INTO planacomorc.vinculacaoestrategicasubacoes(vaeid, sbacod, unicod) VALUES {$inserts}";
                $db->executar($SQLInsert);
                $db->commit();
                
            }
            break;

        case 'pi':
            if (isset($_GET['acoes']) && isset($_GET['vaeid'])) {
                $inserts = insert_values_pis($_GET['uos'],$_GET['acoes'], $_GET['vaeid']);
                $SQLInsert = "INSERT INTO planacomorc.vinculcaoestrategicapis(vaeid, plicod,unicod) VALUES {$inserts}";
                $db->executar($SQLInsert);
                $db->commit();
            }
            break;

        case 'po':
            if (isset($_GET['acoes']) && isset($_GET['vaeid'])) {
                $inserts = insert_values_pos($_GET['acoes'], $_GET['vaeid']);
                $SQLInsert = "INSERT INTO planacomorc.vinculacaoestrategicapos(ptres, plocod, plodsc, vaeid, unicod) VALUES {$inserts}";
                $db->executar($SQLInsert);
                $db->commit();
            }
            break;
    }

    $fm->addMensagem(
            'Registro atualizado com sucesso.', Simec_Helper_FlashMessage::SUCESSO
    );

    $exercicio = $_REQUEST['clicouexercicio'];
    echo "<script>window.location = 'http:planacomorc.php?modulo=sistema/tabelasapoio/cadastrarVinculacaoOrcamentaria&acao=A&clicouexercicio=" . $_REQUEST['clicouexercicio'] . "&vaeid=" . $_REQUEST['vaeid'] . "'</script>";

}

/**
 * Rotina que receber o POST
 * e cadastra a Ação estratégica
 */
if (count($_POST) > 0) {
    $resultSet = salvar($_POST);
}

/**
 * Rotina para alimentar modal com dados
 * Primeiro passo com as Unidades Orçamentárias
 * @return void
 */
if (isAjax() && isset($_GET['selecionaUo'])) {
    $sql = <<<DML
        SELECT DISTINCT 
            a.unicod AS codigo, 
            u.unidsc AS descricao 
        FROM monitora.acao a 
        JOIN unidade u ON (u.unicod = a.unicod) 
        WHERE a.prgano='{$_REQUEST["clicouexercicio"]}'
        ORDER BY u.unidsc
DML;
    $unidades = $db->carregar($sql);
    
    /** Ações, Subações e Planos Orçamentárioas */
    if ($_GET['selecionaUo'] != 'pi') {
        if ($unidades) {
            scriptAcoes();
            echo <<<HTML
            <div class="step1">
HTML;
            echo inputSearch('unicodSeach','Pesquisa');
            echo <<<HTML
                <table class="tabela unicodSeach table table-striped table-bordered table-hover" border="1" style="border-collapse:collapse;" bgcolor="#f5f5f5" cellSpacing="4" cellPadding="4" align="center">
                    <thead>
                        <tr>
                            <th style="width:5%;"> 
                                <input type="checkbox" class="superCheck" data-target="uoCheck" />
                            </th>
                            <th style="width:10%;" class="text-center" style="color:black;">
                                Código
                            </th>
                            <th style="width:85%;" class="text-center" style="color:black;">
                                Descrição
                            </th>
                        </tr>
                    </thead>
                    <tbody>
HTML;
            foreach ($unidades as $row) {
                echo <<<HTML
                        <tr>
                            <td class="text-center">
                                <input class="uoCheck" name="uo[]" value="{$row['codigo']}" id="uo{$row['codigo']}" type="checkbox" />
                            </td>
                            <td class="text-center">{$row['codigo']}</td>
                            <td>{$row['descricao']}</td>
                        </tr>
HTML;
            }
            echo <<<HTML
                    </tbody>
                </table>            
            </div>
            <div class="step2" style="display:none;"/>
HTML;
            echo <<<JAVASCRIPT
            <script>
                $('#modal-confirm .btn-primary').html('Avançar');
            </script>
JAVASCRIPT;
        }
    }
    /** Planos Internos */
    else {
        if ($unidades) {
            scriptAcoes();
            echo <<<HTML
            <div class="row">
                <div class="step1 col-md-6">
HTML;
            echo inputSearch('uoFilter','Pesquisa');            
            echo <<<HTML
                    <table class="tabela uoFilter table table-striped table-bordered table-hover" border="1" style="border-collapse:collapse;" align="center">
                        <thead>
                            <tr>
                                <th colspan="3">Unidades</th>
                            </tr>
                            <tr>
                                <th style="width:5%;">
                                    <input type="checkbox" class="superCheck pi_uo" data-target="piUoCheck" />
                                </th>
                                <th style="width:10%;" class="text-center" style="color:black;">Código</th>
                                <th style="width:85%;" class="text-center" style="color:black;">Título</th>
                            </tr>
                        </thead>
                        <tbody>
HTML;
            foreach ($unidades as $row) {
                echo <<<HTML
                            <tr>
                                <td class="text-center">
                                    <input class="piUoCheck" name="pi_uo[]" value="{$row['codigo']}" id="pi_uo{$row['codigo']}" type="checkbox" />
                                </td>
                                <td class="text-center">{$row['codigo']}</td>
                                <td>{$row['descricao']}</td>
                            </tr>                    
HTML;
            }
            echo <<<HTML
                        </tbody>
                    </table>            
                </div>
HTML;
        }

        $strSQL_pi_subacoes = <<<DML
            SELECT DISTINCT 
                sd.sbaid, 
                sbacod, 
                sbasigla, 
                sbatitulo
            FROM monitora.pi_subacao sb
            JOIN monitora.pi_subacaodotacao sd ON sd.sbaid = sb.sbaid
            JOIN monitora.ptres pt ON pt.ptrid = sd.ptrid
            WHERE pt.ptrano = '{$_REQUEST["clicouexercicio"]}'
                AND sb.sbaano = '{$_REQUEST["clicouexercicio"]}' 
DML;
        $subacoes = $db->carregar($strSQL_pi_subacoes);
        if ($subacoes) {
            echo <<<HTML
                <div class="filter-right col-md-6">
HTML;
            echo inputSearch('sbaFilter');            
            echo <<<HTML
                    <table class="tabela sbaFilter table table-striped table-bordered table-hover" border="1" style="border-collapse:collapse;" bgcolor="#f5f5f5" cellSpacing="4" cellPadding="4" align="center">
                        <thead>
                            <tr>
                                <th colspan="3">Subações</th>
                            </tr>                    
                            <tr class="text-center">
                                <th style="width:5%;">
                                    <input type="checkbox" class="superCheck pi_sba" data-target="piSbaCheck" />
                                </th>
                                <th style="width:10%;" class="text-center">Código</th>
                                <th style="width:85%;">Título</th>
                            </tr>
                        </thead>
                        <tbody>
HTML;
            foreach ($subacoes as $row) {
                $sbasigla = $row['sbasigla'] ? $row['sbasigla'] : 'N/A';
                echo <<<HTML
                            <tr>
                                <td class="text-center">
                                    <input class="piSbaCheck" name="pi_sba[]" value="{$row['sbaid']}" id="pi_sba{$row['sbaid']}" type="checkbox" />
                                </td>
                                <td>{$row['sbacod']}</td>
                                <td><span class="text-primary">$sbasigla</span> - {$row['sbatitulo']}</td>
                            </tr>
HTML;
            }
            echo <<<HTML
                        </tbody>
                    </table>
                </div>
            </div>
HTML;
        }
        echo <<<JAVASCRIPT
            <script>
                $('#modal-confirm .btn-primary').html('Carregar PI\'s');
            </script>
JAVASCRIPT;
        echo <<<HTML
            <div class="pi-container"></div>
HTML;
    }
    exit;
}

/**
 * Ação para modal com dados de Ações
 * @return void
 */
if (isAjax() && isset($_GET['getGrid'])) {
    scriptAcoes();
    switch ($_GET['getGrid']) {
        case 'acao':
            $arrValues = getRequestValues();
            $cabecalho = array(superCheck('acaoUoCheck'), 'Unidade', 'Código', 'Descrição da ação');
            $strSQLacao = <<<DML
                SELECT DISTINCT
                    '<input type=\"checkbox\" name=\"acao[]\" class=\"acaoUoCheck\" value=\"'||u.unicod||'-'||aca.acacod||'\"/>' AS acao,
                    u.unidsc, acacod,
                    (SELECT acatitulo FROM monitora.acao aca2 WHERE aca2.acacod = aca.acacod AND TRIM(acatitulo) <> '' ORDER BY acaid DESC LIMIT 1) acadsc
                FROM monitora.acao aca
                JOIN unidade u ON u.unicod=aca.unicod
                WHERE prgano='{$_REQUEST["clicouexercicio"]}'
                    AND u.unicod IN ({$arrValues})
                    AND aca.acacod NOT IN (
			SELECT acacod
			FROM planacomorc.vinculacaoestrategicaacoes 
                        WHERE vaeid = '{$_REQUEST['id']}' AND unicod IN ({$arrValues}) 
		    )
                ORDER BY u.unidsc, acacod
DML;
            #ver($strSQLacao);
            $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
            $listagem->setQuery($strSQLacao);
            $listagem->addCallbackDeCampo('acadsc', 'alinhaParaEsquerda');
            $listagem->setId('table_aca');
            $listagem->turnOnPesquisator();
            $listagem->setCabecalho($cabecalho);
            $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);            
            echo <<<JAVASCRIPT
            <script>
                $('#modal-confirm .btn-primary').html('Salvar Ação(ões)');
            </script>
JAVASCRIPT;
            break;

        case 'subacao':
            //Era o do Lucas -> $strSQL_subacao = retornaConsultaSubacao(array('SELECT' => "SELECT '<input type=\"checkbox\" name=\"subacao[]\" class=\"subacaoCheck\" value=\"'||sbacod||'\"/>' AS subacao, sbacod, sbatitulo"));
            $arrValues = getRequestValues();
            $strSQL_subacao = retornaSubacoesPeloAnoPtres($_REQUEST["clicouexercicio"], true, $arrValues);
            $cabecalho = array(superCheck('subacaoCheck'), 'Código', 'Descrição da Subação');
            #_monta_lista($strSQL_subacao,$cabecalho,100000,20,'','','','', array(),array(),null,array('classTable' => 'subacaoCheck', 'ordena' => false));
            $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
            $listagem->setDados($strSQL_subacao);
            $listagem->setId('table_sba');
            $listagem->addCallbackDeCampo('descricao', 'alinhaParaEsquerda');
            $listagem->turnOnPesquisator();
            $listagem->setCabecalho($cabecalho);
            $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
            echo <<<JAVASCRIPT
            <script>
                $('#modal-confirm .btn-primary').html('Salvar Subação(ões)');
            </script>
JAVASCRIPT;
            break;

        case 'pi':
            if (isset($_GET['uos']) && $_GET['uos']!='false') {
                $uos = addSlash($_GET['uos']);
            }
            if (isset($_GET['sba']) && $_GET['sba']!='false') {
                $sba = addSlash($_GET['sba']);
            }
            if ($uos && $sba) {
                $strSQL_pis = <<<DML
                    SELECT DISTINCT
                        '<input type=\"checkbox\" name=\"plicod[]\" class=\"plicodCheck\" value=\"'||plicod||'\"/>' AS acao,
                        plicod, 
                        plititulo 
                    FROM monitora.pi_planointerno pli
                    JOIN monitora.pi_planointernoptres ppt ON pli.pliid=ppt.pliid
                    JOIN monitora.ptres pt ON pt.ptrid = ppt.ptrid
                    WHERE plistatus = 'A' 
                        AND pliano = '{$_REQUEST["clicouexercicio"]}' 
                        AND pt.ptrano = '{$_REQUEST["clicouexercicio"]}'
                        AND pt.unicod IN ({$uos})
                        AND sbaid IN ({$sba})
DML;
            } elseif($uos && !$sba) {
                $strSQL_pis = <<<DML
                    SELECT DISTINCT
                        '<input type=\"checkbox\" name=\"plicod[]\" class=\"plicodCheck\" value=\"'||plicod||'\"/>' AS acao,
                        plicod, 
                        plititulo 
                    FROM monitora.pi_planointerno pli
                    JOIN monitora.pi_planointernoptres ppt on pli.pliid = ppt.pliid
                    JOIN monitora.ptres pt on pt.ptrid = ppt.ptrid
                    WHERE plistatus = 'A' 
                        AND pliano = '{$_REQUEST["clicouexercicio"]}' 
                        AND pt.ptrano = '{$_REQUEST["clicouexercicio"]}'
                        AND pt.unicod IN ({$uos})
DML;
            } else {
                $strSQL_pis = <<<DML
                    SELECT
                        '<input type=\"checkbox\" name=\"plicod[]\" class=\"plicodCheck\" value=\"'||plicod||'\"/>' AS acao, 
                        plicod, 
                        plititulo 
                    FROM monitora.pi_planointerno 
                    WHERE pliano = '{$_REQUEST["clicouexercicio"]}' 
                        AND plistatus = 'A' 
                        AND sbaid IN ({$sba})
DML;
            }
            
            $strSQL_pis .= "
                AND plicod NOT IN (
                    SELECT plicod
                    FROM planacomorc.vinculcaoestrategicapis 
                    WHERE vaeid = {$_REQUEST['id']} AND unicod IN ({$uos}) 
                )
                ";
            #$cabecalho = array(superCheck('plicodCheck'), 'Plicod', 'Título');
            $cabecalho = array(superCheck('plicodCheck'), 'Código', 'Título');
            $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
            $listagem->setQuery($strSQL_pis);
            $listagem->addCallbackDeCampo('plititulo', 'alinhaParaEsquerda');
            $listagem->turnOnPesquisator();
            $listagem->setId('table_pi');
            $listagem->setCabecalho($cabecalho);
            $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
            
            #_monta_lista($strSQL_pis,$cabecalho,100000,20,'','','','', array(),array(),null,array('classTable' => 'plicodCheck', 'ordena' => false));
            echo <<<JAVASCRIPT
            <script>
                $('#modal-confirm .btn-primary').html('Salvar PI\'s');
            </script>
JAVASCRIPT;
            break;

        case 'po':
            $arrValues = getRequestValues();
            $strSQLpo = <<<DML
                SELECT DISTINCT
                    '<input type=\"checkbox\" name=\"ptres[]\" class=\"ptresCheck\" value=\"'||pt.ptres||'-'||pt.plocod||'-'||aca.unicod||'\"/>' AS acao,
                    aca.unicod,
                    '<span class="label label-info">' || aca.esfcod || '.' || aca.unicod || '.' || aca.funcod || '.' || aca.sfucod || '.' || aca.prgcod || '.' || aca.acacod || '.' || aca.loccod || '</span>' AS funcional,
                    pt.plocod, 
                    plotitulo
                FROM monitora.acao aca
                JOIN monitora.ptres pt ON aca.acaid = pt.acaid AND pt.ptrano = aca.prgano
                LEFT JOIN monitora.planoorcamentario po ON po.acacod = aca.acacod 
                    AND po.unicod = aca.unicod 
                    AND po.plocodigo = pt.plocod
                    AND po.exercicio = aca.prgano
                WHERE prgano = '{$_REQUEST["clicouexercicio"]}' 
                    AND aca.unicod IN ({$arrValues})
                    AND pt.ptres NOT IN (
                        SELECT ptres
                        FROM planacomorc.vinculacaoestrategicapos 
                        WHERE vaeid = {$_REQUEST['id']} AND unicod IN ({$arrValues}) 
                    )
                        
DML;
            #ver($strSQLpo);
            $cabecalho = array(superCheck('ptresCheck'), 'Id', 'Funcional', 'Código', 'Título');
            $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
            $listagem->setQuery($strSQLpo);
            $listagem->setId('table_po');
            $listagem->addCallbackDeCampo('plotitulo', 'alinhaParaEsquerda');
            $listagem->turnOnPesquisator();
            $listagem->setCabecalho($cabecalho);
            $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
            #_monta_lista($strSQLpo,$cabecalho,100000,20,'','','','', array(),array(),null,array('classTable' => 'ptresCheck', 'ordena' => false));
            echo <<<JAVASCRIPT
            <script>
                $('#modal-confirm .btn-primary').html('Salvar PO\'s');
            </script>
JAVASCRIPT;
            break;
    }
    echo <<<JAVASCRIPT
        <script>
            $('#modal-confirm .btn-primary').attr('class','btn btn-primary btnFinish {$_GET['getGrid']}');
            $('#modal-confirm .btn-primary').attr('onclick','return confirmaSelecaoFinal(this);');
        </script>
JAVASCRIPT;

    exit;
}