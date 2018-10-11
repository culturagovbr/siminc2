<?php

/* MECANISMO DE ESPELHO DOS PERFIS - Removendo todos os perfis slaves dos outros sistemas	 */

function removerPerfisSlaves($usucpf, $sisid) {
    global $db;

    $sql = "SELECT DISTINCT p.pflcodslave, p.servidorslave FROM seguranca.espelhoperfil p WHERE p.sisidmaster = '" . $sisid . "'";
    $arrslaves = $db->carregar($sql);

    if ($arrslaves[0]) {
        foreach ($arrslaves as $arrs) {
            if ($arrs['servidorslave'] == 'EXTERNO') {
                $sql = "DELETE FROM seguranca.perfilusuario WHERE usucpf = '" . $usucpf . "' AND pflcod='" . $arrs['pflcodslave'] . "'";
                adapterConnection::siminc1()->executar($sql);
            } else {
                $sql = "DELETE FROM seguranca.perfilusuario WHERE usucpf = '" . $usucpf . "' AND pflcod='" . $arrs['pflcodslave'] . "'";
                $db->executar($sql);
                $db->commit();
            }
        }
    }
}

/* MECANISMO DE ESPELHO DOS PERFIS - Inserindo todos os perfis slaves dos outros sistemas	 
  Mudança para atender o EXTERNO 2013 - 20/03/2013 - solicitado pelo Daniel Areas
 * */

function inserirPerfisSlaves($usucpf, $pflcod) {
    global $db;

    $sql = "SELECT DISTINCT p.pflcodslave, p.servidorslave FROM seguranca.espelhoperfil p WHERE p.pflcodmaster = '" . $pflcod . "'";
    $pfls = $db->carregar($sql);

    if ($pfls[0]) {

        foreach ($pfls as $pfl) {

            if ($pfl['servidorslave'] == 'EXTERNO') {

                $existe_us = adapterConnection::siminc1()->pegaUm("SELECT usucpf FROM seguranca.usuario WHERE usucpf='". $usucpf. "'");

                if (!$existe_us) {

                    $dados_us = $db->pegaLinha("SELECT * FROM seguranca.usuario WHERE usucpf='" . $usucpf . "'");

                    $sql = "
                        INSERT INTO seguranca.usuario(
                            usucpf,
                            regcod,
                            usunome,
                            usuemail,
                            usustatus,
                            usufoneddd,
                            usufonenum,
                            ususenha,
                            usudataultacesso,
                            usunivel,usufuncao, 
                            ususexo, 
                            orgcod, 
                            unicod, 
                            usuchaveativacao, 
                            usutentativas, 
                            usuprgproposto, 
                            usuacaproposto, 
                            usuobs, 
                            ungcod, 
                            usudatainc, 
                            usuconectado, 
                            pflcod, 
                            suscod, 
                            usunomeguerra, 
                            orgao, muncod, usudatanascimento, usudataatualizacao,						             
                            tpocod, carid)
                    VALUES (
                        '" . $dados_us['usucpf'] . "', 
                        " . (($dados_us['regcod']) ? "'" . $dados_us['regcod'] . "'" : "NULL") . ", 
                        " . (($dados_us['usunome']) ? "'" . str_replace(array("'"), array(""), $dados_us['usunome']) . "'" : "NULL") . ", 
                        " . (($dados_us['usuemail']) ? "'" . $dados_us['usuemail'] . "'" : "NULL") . ", 
                        " . (($dados_us['usustatus']) ? "'" . $dados_us['usustatus'] . "'" : "NULL") . ", 
                        " . (($dados_us['usufoneddd']) ? "'" . $dados_us['usufoneddd'] . "'" : "NULL") . ", 
                        " . (($dados_us['usufonenum']) ? "'" . $dados_us['usufonenum'] . "'" : "NULL") . ", 
                        " . (($dados_us['ususenha']) ? "'" . $dados_us['ususenha'] . "'" : "NULL") . ", 
                        " . (($dados_us['usudataultacesso']) ? "'" . $dados_us['usudataultacesso'] . "'" : "NULL") . ", 
                        " . (($dados_us['usunivel']) ? "'" . $dados_us['usunivel'] . "'" : "NULL") . ", 
                        " . (($dados_us['usufuncao']) ? "'" . $dados_us['usufuncao'] . "'" : "NULL") . ", 
                        " . (($dados_us['ususexo']) ? "'" . $dados_us['ususexo'] . "'" : "NULL") . ", 
                        " . (($dados_us['orgcod']) ? "'" . $dados_us['orgcod'] . "'" : "NULL") . ", 
                        " . (($dados_us['unicod']) ? "'" . $dados_us['unicod'] . "'" : "NULL") . ", 
                        " . (($dados_us['usuchaveativacao']) ? "'" . $dados_us['usuchaveativacao'] . "'" : "NULL") . ", 
                        " . (($dados_us['usutentativas']) ? "'" . $dados_us['usutentativas'] . "'" : "NULL") . ", 
                        " . (($dados_us['usuprgproposto']) ? "'" . $dados_us['usuprgproposto'] . "'" : "NULL") . ", 
                        " . (($dados_us['usuacaproposto']) ? "'" . $dados_us['usuacaproposto'] . "'" : "NULL") . ", 
                        " . (($dados_us['usuobs']) ? "'" . $dados_us['usuobs'] . "'" : "NULL") . ", 
                        " . (($dados_us['ungcod']) ? "'" . $dados_us['ungcod'] . "'" : "NULL") . ", 
                        " . (($dados_us['usudatainc']) ? "'" . $dados_us['usudatainc'] . "'" : "NULL") . ", 
                        " . (($dados_us['usuconectado']) ? "'" . $dados_us['usuconectado'] . "'" : "NULL") . ", 
                        " . (($dados_us['pflcod']) ? "'" . $dados_us['pflcod'] . "'" : "NULL") . ", 
                        " . (($dados_us['suscod']) ? "'" . $dados_us['suscod'] . "'" : "NULL") . ", 
                        " . (($dados_us['usunomeguerra']) ? "'" . $dados_us['usunomeguerra'] . "'" : "NULL") . ", 
                        " . (($dados_us['orgao']) ? "'" . $dados_us['orgao'] . "'" : "NULL") . ", 
                        " . (($dados_us['muncod']) ? "'" . $dados_us['muncod'] . "'" : "NULL") . ", 
                        " . (($dados_us['usudatanascimento']) ? "'" . $dados_us['usudatanascimento'] . "'" : "NULL") . ", 
                        " . (($dados_us['usudataatualizacao']) ? "'" . $dados_us['usudataatualizacao'] . "'" : "NULL") . ",
                        " . (($dados_us['tpocod']) ? "'" . $dados_us['tpocod'] . "'" : "NULL") . ", 
                        " . (($dados_us['carid']) ? "'" . $dados_us['carid'] . "'" : "NULL") . ");";

                    adapterConnection::siminc1()->executar($sql);
                } else {

                    $dados_us = $db->pegaLinha("SELECT * FROM seguranca.usuario WHERE usucpf='" . $usucpf . "'");

                    $sql = "update seguranca.usuario set suscod = '{$dados_us['suscod']}' where usucpf = '{$usucpf}'";
                    adapterConnection::siminc1()->executar($sql);
                }

                $existe = adapterConnection::siminc1()->pegaUm("SELECT usucpf from seguranca.perfilusuario where pflcod='" . $pfl['pflcodslave'] . "' and usucpf='" . $usucpf . "'");

                if (!$existe) {
                    $sql = "INSERT INTO seguranca.perfilusuario ( usucpf, pflcod ) VALUES ( '" . $usucpf . "', '" . $pfl['pflcodslave'] . "');";
                    adapterConnection::siminc1()->executar($sql);
                }
            } else {

                $existe = $db->pegaUm("SELECT usucpf from seguranca.perfilusuario where pflcod='" . $pfl['pflcodslave'] . "' and usucpf='" . $usucpf . "'");

                if (!$existe) {
                    $sql = "INSERT INTO seguranca.perfilusuario ( usucpf, pflcod ) VALUES ( '" . $usucpf . "', '" . $pfl['pflcodslave'] . "');";
                    $db->executar($sql);
                    $db->commit();
                }
            }
        }
    }

    $sql = "SELECT DISTINCT p.sisidslave, p.servidorslave FROM seguranca.espelhoperfil p WHERE p.pflcodmaster = '" . $pflcod . "'";
    $siss = $db->carregar($sql);

    if ($siss[0]) {

        foreach ($siss as $sis) {

            $suscod = $db->pegaUm("(SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf='" . $usucpf . "' AND sisid IN (SELECT DISTINCT sisidmaster FROM seguranca.espelhoperfil p WHERE p.pflcodmaster = " . $pflcod . "))");

            if ($sis['servidorslave'] == 'EXTERNO') {

                $existe = adapterConnection::siminc1()->pegaUm("SELECT usucpf from seguranca.usuario_sistema where sisid='" . $sis['sisidslave'] . "' and usucpf='" . $usucpf . "'");

                if (!$existe) {
                    $sql = "INSERT INTO seguranca.usuario_sistema(usucpf, sisid, susstatus, pflcod, susdataultacesso, suscod) VALUES ('" . $usucpf . "', '" . $sis['sisidslave'] . "', 'A', NULL::integer, NOW(), '" . $suscod . "')";
                    adapterConnection::siminc1()->executar($sql);
                }

                $sql = "UPDATE seguranca.usuario_sistema SET suscod='" . $suscod . "' WHERE usucpf='" . $usucpf . "' AND sisid='" . $sis['sisidslave'] . "'";
                adapterConnection::siminc1()->executar($sql);
            } else {

                $existe = $db->pegaUm("SELECT usucpf from seguranca.usuario_sistema where sisid='" . $sis['sisidslave'] . "' and usucpf='" . $usucpf . "'");

                if (!$existe) {
                    $sql = "INSERT INTO seguranca.usuario_sistema(usucpf, sisid, susstatus, pflcod, susdataultacesso, suscod) VALUES ('" . $usucpf . "', '" . $sis['sisidslave'] . "', 'A', NULL::integer, NOW(), '" . $suscod . "')";
                    $db->executar($sql);
                    $db->commit();
                }

                $sql = "UPDATE seguranca.usuario_sistema SET suscod='" . $suscod . "' WHERE usucpf='" . $usucpf . "' AND sisid='" . $sis['sisidslave'] . "'";
                $db->executar($sql);
                $db->commit();
            }
        }
    }
}

/* MECANISMO DE ESPELHO DOS PERFIS - Atualizando as responsabilidades nos outros sistemas	 */

function atualizarResponsabilidadesSlaves($usucpf, $pflcod) {
    global $db;

    $sql = "
        SELECT DISTINCT
            p.pflcodmaster,
            p.pflcodslave,
            p.urcampo,
            p.sisidslave,
            s.sisdiretorio AS sisdiretorioslave,
            s1.sisdiretorio AS sisdiretoriomaster,
            p.servidorslave
        FROM seguranca.espelhoperfil p
            LEFT JOIN seguranca.sistema s ON s.sisid = p.sisidslave
            LEFT JOIN seguranca.sistema s1 ON s1.sisid = p.sisidmaster
        WHERE
            p.pflcodmaster = ". (int)$pflcod;

    $registros = $db->carregar($sql);

    if ($registros[0]) {
        foreach ($registros as $registro) {

            if ($registro['servidorslave'] == 'EXTERNO') {
                $sql = "select sisdiretorio from seguranca.sistema where sisid = " . $registro['sisidslave'];
                $registro['sisdiretorioslave'] = adapterConnection::siminc1()->pegaUm($sql);
            }

            if ($registro['urcampo']) {

                if ($registro['servidorslave'] == 'EXTERNO') {
                    adapterConnection::siminc1()->executar("UPDATE " . $registro['sisdiretorioslave'] . ".usuarioresponsabilidade SET rpustatus='I' WHERE usucpf='" . $usucpf . "' AND pflcod='" . $registro['pflcodslave'] . "'");

                    $regs = $db->carregar("SELECT " . $registro['urcampo'] . " FROM " . $registro['sisdiretoriomaster'] . ".usuarioresponsabilidade WHERE rpustatus='A' AND usucpf='" . $usucpf . "' AND pflcod='" . $pflcod . "'");

                    if ($regs[0]) {
                        foreach ($regs as $r) {
                            $sql = "INSERT INTO " . $registro['sisdiretorioslave'] . ".usuarioresponsabilidade (pflcod, usucpf, rpustatus, rpudata_inc, " . $registro['urcampo'] . ")
                                VALUES ('" . $registro['pflcodslave'] . "', '{$usucpf}', 'A', NOW(), '" . $r[$registro['urcampo']] . "')";
                            adapterConnection::siminc1()->executar($sql);
                        }
                    }
                } else {

                    $db->executar("UPDATE " . $registro['sisdiretorioslave'] . ".usuarioresponsabilidade SET rpustatus='I' WHERE usucpf='" . $usucpf . "' AND pflcod='" . $registro['pflcodslave'] . "'");

                    $regs = $db->carregar("SELECT " . $registro['urcampo'] . " FROM " . $registro['sisdiretoriomaster'] . ".usuarioresponsabilidade WHERE rpustatus='A' AND usucpf='" . $usucpf . "' AND pflcod='" . $pflcod . "'");

                    if ($regs[0]) {
                        foreach ($regs as $r) {
                            if ($r[$registro['urcampo']] != '') {
                                $sql = "INSERT INTO " . $registro['sisdiretorioslave'] . ".usuarioresponsabilidade (pflcod, usucpf, rpustatus, rpudata_inc, " . $registro['urcampo'] . ")
                                    VALUES ('" . $registro['pflcodslave'] . "', '{$usucpf}', 'A', NOW(), '" . $r[$registro['urcampo']] . "')";

                                $db->executar($sql);
                            }
                        }
                    }
                }
            }
        }
    }
    $db->commit();
}
