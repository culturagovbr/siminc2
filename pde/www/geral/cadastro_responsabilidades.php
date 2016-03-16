<?
/*
  Sistema Simec
  Setor respons�vel: SPO-MEC
  Desenvolvedor: Equipe Consultores Simec
  Analista: Gilberto Arruda Cerqueira Xavier
  Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
  M�dulo:cadastro_usuario_elaboracao_responsabilidades.php

 */
    include "config.inc";
    header('Content-Type: text/html; charset=iso-8859-1');
    include APPRAIZ . "includes/classes_simec.inc";
    include APPRAIZ . "includes/funcoes.inc";
    $db = new cls_banco();

    $usucpf = $_REQUEST["usucpf"];
    $pflcod = $_REQUEST["pflcod"];

    if (!$pflcod && !$usucpf) {

?>
    <font color="red">Requisi��o inv�lida</font>
    
<?PHP
        eixt();
    }

    $sqlResponsabilidadesPerfil = "
        SELECT tr.*
        FROM pde.tprperfil p
        INNER JOIN pde.tiporesponsabilidade tr ON p.tprcod = tr.tprcod
        WHERE tprsnvisivelperfil = TRUE AND p.pflcod = '%s'
        ORDER BY tr.tprdsc
    ";
    $query = sprintf($sqlResponsabilidadesPerfil, $pflcod);

    $responsabilidadesPerfil = $db->carregar($query);

    if (!$responsabilidadesPerfil || @count($responsabilidadesPerfil) < 1) {
        print "<font color='red'>N�o foram encontrados registros</font>";
    } else {
        foreach ($responsabilidadesPerfil as $rp) {
            // monta o select com codigo, descricao e status de acordo com o tipo de responsabilidade (a��o, programas, etc)
            $sqlRespUsuario = "";
            switch ($rp["tprsigla"]) {
                case "E": #Empresas
                    $aca_prg = "Empresas";
                    $sqlRespUsuario = "
                        SELECT  DISTINCT e.entid AS codigo, 
                                e.entnome AS descricao, 
                                ur.rpustatus AS status
                        FROM pde.usuarioresponsabilidade ur 

                        INNER JOIN entidade.entidade e ON e.entid = ur.entid

                        WHERE ur.usucpf = '%s' AND ur.pflcod = '%s' AND  ur.rpustatus='A'
                    ";
                    break;
                case "C": #COCKPIT
                    $aca_prg = "Cockpit";
                    $sqlRespUsuario = "

                        SELECT  c.cocid AS codigo,
                                c.cocnome AS descricao
                        FROM pde.usuarioresponsabilidade ur

                        JOIN pde.cockpit c ON c.cocid = ur.cocid

                        WHERE ur.usucpf = '%s' AND ur.pflcod = %s AND ur.rpustatus = 'A'
                    ";
                    break;
                default:
                    break;
            }

            if (!$sqlRespUsuario)
                continue;

            $query = vsprintf($sqlRespUsuario, array($usucpf, $pflcod));
            $respUsuario = $db->carregar($query);

            if (!$respUsuario || @count($respUsuario) < 1) {
?>
                <table width="100%" border="0" cellspacing="0" cellpadding="0" align="center" style="width:100%; border: 0px; color:#006600;">
                    <tr>
                        <td style="text-align: center;"><font color='red'>N�o existem atribu��es a este Perfil.</font></td>
                    </tr>
                </table>
<?PHP
            } else {
?>
            <table width="100%" border="0" cellspacing="0" cellpadding="0" align="center" style="width:100%; border: 0px; color:#006600;">
                <tr>
                    <td colspan="3"><?= $rp["tprdsc"] ?></td>
                </tr>
                <tr style="color:#000000;">
                    <td valign="top" width="12">&nbsp;</td>
                    <td valign="top">C�digo</td>
                    <td valign="top">Descri��o</td>
                </tr>
<?PHP
                foreach ($respUsuario as $ru) {
?>
                    <tr onmouseover="this.bgColor = '#ffffcc';" onmouseout="this.bgColor = 'F7F7F7';" bgcolor="F7F7F7">
                        <td valign="top" width="12" style="padding:2px;"><img src="../imagens/seta_filho.gif" width="12" height="13" alt="" border="0"></td>
                        <td valign="top" width="90" style="border-top: 1px solid #cccccc; padding:2px; color:#003366;" nowrap>
<?PHP
                        if ($rp["tprsigla"] == 'A') { 
?>
                            <a href="simec_er.php?modulo=principal/acao/cadacao&acao=C&acaid=<?= $ru["acaid"] ?>&prgid=<?= $ru["prgid"] ?>"><?= $ru["codigo"] ?></a>
<?PHP
                        } else {
                            echo $ru["codigo"];
                        } 
?>
                        </td>
                        <td valign="top" width="290" style="border-top: 1px solid #cccccc; padding:2px; color:#006600;"><?= $ru["descricao"] ?></td>
                    </tr>
<?PHP
                }
?>
                <tr>
                    <td colspan="4" align="right" style="color:000000;border-top: 2px solid #000000;">
                        Total: (<?= @count($respUsuario) ?>)
                    </td>
                </tr>
            </table>
<?PHP
        }
    }
}

$db->close();
exit();

?>