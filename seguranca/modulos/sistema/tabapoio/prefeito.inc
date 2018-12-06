<?php
include_once APPRAIZ . "includes/classes/dateTime.inc";
$data = new Data();

if ($_REQUEST['opt'] && $_REQUEST['opt'] == 'buscarCpf') {
    require_once APPRAIZ . "adodb/adodb.inc.php";
    require_once APPRAIZ . "includes/entidades.php";
    require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";

    $entidade = Entidade::carregarEntidadePorCnpjCpf($_REQUEST['entnumcpfcnpj']);

    if ($entidade->getPrimaryKey() !== null) {
        die($entidade->getPrimaryKey());
    } else {
        die('0');
    }
}


if ($_REQUEST['opt'] && $_REQUEST['opt'] == 'salvarRegistro') {
    require_once APPRAIZ . "adodb/adodb.inc.php";
    require_once APPRAIZ . "includes/entidades.php";
    require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
    require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";
    require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
    require_once APPRAIZ . "includes/ActiveRecord/classes/EntidadeEndereco.php";

    //echo "<pre>\n" , print_r($_REQUEST) , "</pre>\n";
    //die();

    $entidade = new Entidade();

    
    if ($_REQUEST['entid'] != '')
        $entidade->carregar((integer) $_REQUEST['entid']);

    $_REQUEST['entnumcpfcnpj'] = str_replace(array('.', '/', '-'), '', $_REQUEST['entnumcpfcnpj']);

    try {
        $entidade->BeginTransaction();

        $entidade->entnumcpfcnpj        = str_replace(array('.', '/'), '', $_REQUEST['entnumcpfcnpj']);
        $entidade->entnome              = $_REQUEST['entnome'];
        $entidade->entemail             = $_REQUEST['entemail'];
        $entidade->entnumrg             = $_REQUEST['entnumrg'];
        $entidade->entorgaoexpedidor    = $_REQUEST['entorgaoexpedidor'];
        $entidade->entsexo              = $_REQUEST['entsexo'];
        $entidade->entnumdddresidencial = $_REQUEST['entnumdddresidencial'];
        $entidade->entnumresidencial    = $_REQUEST['entnumresidencial'];
        $entidade->entnumdddcomercial   = $_REQUEST['entnumdddcomercial'];
        $entidade->entnumcomercial      = $_REQUEST['entnumcomercial'];
        $entidade->entnumramalcomercial = $_REQUEST['entnumramalcomercial'];
        $entidade->entnumdddfax         = $_REQUEST['entnumdddfax'];
        $entidade->entnumfax            = $_REQUEST['entnumfax'];
        $entidade->entnumramalfax       = $_REQUEST['entnumramalfax'];
        $entidade->entobs               = $_REQUEST['entobs'];
        $entidade->no_entobs            = $_REQUEST['no_entobs'];
        $entidade->entstatus            = $_REQUEST['entstatus'];

        $entidade->entdatanasc          = trim($_REQUEST['entdatanasc'])   != '' ? ajusta_data($_REQUEST['entdatanasc'])   : 'null';
        $entidade->entdatainiass        = trim($_REQUEST['entdatainiass']) != '' ? ajusta_data($_REQUEST['entdatainiass']) : 'null';
        $entidade->entdatafimass        = trim($_REQUEST['entdatafimass']) != '' ? ajusta_data($_REQUEST['entdatafimass']) : 'null';
        $entidade->funid                = 2;
        
        if(!$entidade->entdatainclusao){
        	$entidade->entdatainclusao = NULL;
        }else{
        	$entidade->entdatainclusao = $data->formataData($entidade->entdatainclusao, "YYYY-MM-DD");
        }

        $entidade->entdatanasc	= $entidade->entdatanasc == null ? $data->formataData($entidade->entdatanasc) :
        
		
        //$endereco->endzoom = 0;

        $entidade->save();
        $entidade->Commit();

        $endereco = new Endereco();
        $endereco->BeginTransaction();

        if ($_REQUEST['endid'] != '')
            $endereco->carregar((integer) $_REQUEST['endid']);

        $endereco->endcep     = str_replace(array('.', '-'), '', $_REQUEST['endereco']['endcep']);
        $endereco->endnum     = $_REQUEST['endereco']['endnum'];
        $endereco->endcom     = $_REQUEST['endereco']['endcom'];
        $endereco->endlog     = $_REQUEST['endereco']['endlog'];
        $endereco->endbai     = $_REQUEST['endereco']['endbai'];
        $endereco->muncod     = $_REQUEST['endereco']['muncod'];
        $endereco->estuf      = $_REQUEST['endereco']['estuf'];
        $endereco->entid      = 'null';
        $endereco->endstatus  = 'A';
        $endereco->tpeid      = 1;

        $endereco->save();
        $endereco->Commit();

        EntidadeEndereco::adicionar($entidade, $endereco);

        echo '<script type="text/javascript">alert("Registro salvo com sucesso!");self.close();</script>';
    } catch (Exception $e) {
        $entidade->Rollback();
    }
}


if (array_key_exists('entid', $_REQUEST)) {
    require_once APPRAIZ . "includes/entidades.php";
    //require_once APPRAIZ . "adodb/adodb.inc.php";
    //require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
    //require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
    //require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";
?>
<html>
  <head>
    <meta http-equiv="Cache-Control" content="no-cache">
    <meta http-equiv="Pragma" content="no-cache">
    <meta http-equiv="Connection" content="Keep-Alive">
    <meta http-equiv="Expires" content="Mon, 26 Jul 1997 05:00:00 GMT">
    <title><?= $titulo ?></title>

    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script type="text/javascript" src="../includes/prototype.js"></script>
    <script type="text/javascript" src="../includes/entidades.js"></script>
    <script type="text/javascript" src="/includes/estouvivo.js"></script>

    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <script type="text/javascript">
      this._closeWindows = false;
    </script>
  </head>
  <body style="margin:10px; padding:0; background-color: #fff; background-image: url(../imagens/fundo.gif); background-repeat: repeat-y;">
    <div>
      <h3 class="TituloTela" style="color:#000000; text-align: center"><?php echo $titulo_modulo; ?></h3>
<?php
    $ent = new Entidade();

    if ($_REQUEST['entid'] != '')
        $ent->carregar((integer) $_REQUEST['entid']);

    $prefeitura = new Entidade();

    if ($ent->entidassociado != null)
        $prefeitura->carregar($ent->entidassociado);

    if (!$_REQUEST['opt'] || $_REQUEST['opt'] != 'vinculacaoPrefeitura') {

        echo formEntidade($ent,
                          'seguranca.php?modulo=sistema/tabapoio/prefeito&acao=A&opt=salvarRegistro',
                          PESSOA_FISICA);

    $ent->carregarEnderecos();
    if (count($ent->enderecos) > 0) {
        $sql = "SELECT
                    e.entnome,
                    p.entnome,
                    p.entid,
                    m.mundescricao
                FROM
                    entidade.entidade e
    INNER JOIN 
    	entidade.funcaoentidade fe1 ON fe1.entid = e.entid
    INNER JOIN 
	entidade.funentassoc fea ON fea.fueid = fe1.fueid
    INNER JOIN
        entidade.entidade p on fea.entid = p.entid
    INNER JOIN 
    	entidade.funcaoentidade fe2 ON fe2.entid = p.entid
    INNER JOIN
        entidade.endereco entd on entd.entid = p.entid
    INNER JOIN
        territorios.municipio m on entd.muncod = m.muncod
                WHERE
                    e.entstatus = 'A'
                AND
                	m.muncod = ";
    }
?>

    <script type="text/javascript">
        var newTr  = null;
        var inline = null;

        newTr  = $('tableEntidade').insertRow($('tableEntidade').numRows);
        inline = '<td style="font-weight: bold" colspan="2">Prefeitura</td>';

        Element.insert(newTr,           {bottom: inline});
        Element.insert('tableEntidade', {bottom: newTr });


        newTr  = $('tableEntidade').insertRow($('tableEntidade').numRows);
        inline = '<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"></td>'
               + '<td>'
               + '  <span id="entnomepref"><?php echo addslashes($prefeitura->entnome); ?></span>'
               + '  <a href="javascript:void(0);" onclick="editarPrefeitura(<?php echo $prefeitura->getPrimaryKey(); ?>);">(Alterar)</a>'
               + '  <input type="hidden" name="prefid" id="prefid" value="<?php echo $prefeitura->getPrimaryKey(); ?>" />'
               + '</td>';

        Element.insert(newTr,           {bottom: inline});
        Element.insert('tableEntidade', {bottom: newTr });
        //                                                                  */

        $('frmEntidade').onsubmit  = function(e)
        {
            if (Entidade.validateForm(this, ['entnome'])) {
                /*!@
                 * SEMPRE deve-se remover os atributos 'disabled' ao submeter
                 * o formulï¿½rio
                 */
                $('muncod').removeAttribute('disabled');
                $('endbai').removeAttribute('disabled');
                $('estuf').removeAttribute('disabled');

                $('frmEntidade').submit();
            } else {
                return false;
            }
        }

        $('entnumcpfcnpj').onblur  = function (e)
        {
            if ($('entid').value != '' || this.value == '' || this.value.length != 14)
                return false;

            var req = new Ajax.Request('seguranca.php?modulo=sistema/tabapoio/prefeito&acao=A', {
                                       method: 'post',
                                       parameters: '&opt=buscarCpf&entnumcpfcnpj=' + this.value,
                                       onComplete: function (res)
                                       {
                                           if (res.responseText != 0) {
                                               if (confirm('O CPF informado jï¿½ se encontra cadastrado.\n' 
                                                          +'Deseja carregar o registro?'))
                                               {
                                                   window.location.href = 'seguranca.php?modulo=sistema/tabapoio/prefeito&acao=A&entid=' + res.responseText;
                                               } else {
                                                   $('entnumcpfcnpj').value = '';
                                                   $('entnumcpfcnpj').activate();
                                               }
                                           }
                                       }
            });
        } // !$('entnumcpfcnpj').onblur

        if ($('muncod') && $('estuf')) {
            Entidade.carregarMunicipio();

            $('muncod').setAttribute('disabled', 'disabled');
        }

        function editarPrefeitura(entid)
        {
            windowOpen('seguranca.php?modulo=sistema/tabapoio/prefeito&acao=A&entid=' + entid, 'vinculacaoPrefeitura', 'height=750,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
        }

    </script>

<?php
    } else { // !if (!$_REQUEST['opt'] || $_REQUEST['opt'] != 'vinculacaoPrefeitura')
    }

    die();
}





if (array_key_exists('entbusca', $_REQUEST) || $_REQUEST['entbusca'] == '1') {
		
    if ($_REQUEST['estuf'] != '' && $_REQUEST['estuf'] != 'null') {
        $sql_estado = '
            INNER JOIN
                territorios.estado
            on
                ee.estuf = es.estuf
                AND es.estuf = \'' . $_REQUEST['estuf'] . '\'';

    }

    if ($_REQUEST['muncod'] != '' && $_REQUEST['muncod'] != 'null') {
        $sql_municipio = '
            INNER JOIN
                territorios.municipio m
            ON
                m.muncod = ee.muncod
                AND m.muncod = \'' . $_REQUEST['muncod'] . '\'';
    }
    
    $sql_busca_prefeitos = "
            SELECT DISTINCT
                '<a href=\"javascript:editarEntidade(''' || e.entid || ''');\" title=\"Editar cadastro\"><img border=\"0\" src=\"../imagens/alterar.gif\" title=\"Alterar cadastro\" alt=\"Alterar cadastro\" /></a>' as edicao,
                substring(e.entnumcpfcnpj from 1 for 3) || '.' || substring(e.entnumcpfcnpj from 4 for 3) || '.' || substring(e.entnumcpfcnpj from 7 for 3) || '-' || substring(e.entnumcpfcnpj from 10 for 2) as entnumcpfcnpj,
                e.entnome as entidade,
                pre.entnome as orgao
            FROM entidade.entidade e
            	inner join
					entidade.funcaoentidade fe ON fe.entid = e.entid
				inner join
					entidade.endereco ee on e.entid = ee.entid
				left join
					entidade.funentassoc fea on fe.fueid = fea.fueid
				left join
					entidade.entidade pre on pre.entid = fea.entid
				left join
					territorios.estado es on ee.estuf = es.estuf
            $sql_estado
            $sql_municipio
            WHERE e.entstatus = 'A' 
            AND ";

    	if ( $_REQUEST['funid'] == 2 ){
    		$sql_busca_prefeitos .= ' fe.funid = 2 ';
    	} else if ($_REQUEST['funid'] == 15 ){
    		$sql_busca_prefeitos .= ' fe.funid = 15 ';
        }

    $and = ' AND ';
    if (trim($_REQUEST['entnumcpfcnpj']) != '') {
        $sql_busca_prefeitos .= ' AND e.entnumcpfcnpj = \'' . str_replace(array('.', '/'), '', $_REQUEST['entnumcpfcnpj']) . '\'';
        $and  = ' OR ';
    }

    if (trim($_REQUEST['entnome']) != '') {
            $sql_busca_prefeitos .= $and . ' UPPER(e.entnome) LIKE UPPER(\'%' . str_replace(' ', '%', $_REQUEST['entnome']) . '%\')';
    }
    
    
    //ver( $sql_busca_prefeitos );

    //$res_busca = (array) $db->carregar($sql);

    /*!@
    foreach ($res_busca as $i => &$entidade)
        $entidade['cpf']    = formatar_cpf($entidade['cpf']);
    //                                                                      */
}

require_once APPRAIZ . 'includes/cabecalho.inc';

echo '<script type="text/javascript" src="../includes/prototype.js"></script><br />';

$db->cria_aba($abacod_tela, $url, $parametros);
$titulo_modulo = 'Manutenção de entidades';
monta_titulo($titulo_modulo, '');
?>
<script type="text/javascript">
    function validarFrmBusca()
    {
		if( document.formulario.funid.value == '' ){
			alert('Escolha uma funcao.');
			return false;
		} else {
			return true;
		}
    }

  function editarEntidade(entid)
  {
      if (!entid)
          return false;

      windowOpen('seguranca.php?modulo=sistema/tabapoio/prefeito&acao=A&entid=' + entid, 'edicao', 'height=750,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
  }

  function incluirEntidade()
  {
      windowOpen('seguranca.php?modulo=sistema/tabapoio/prefeito&acao=A&entid=', 'incluir', 'height=750,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
  }

  function carregarMunicipios(estuf)
  {
    var req   = new Ajax.Request('/geral/dne.php?opt=municipio&regcod=' + estuf, {
                                    method: 'post',
                                    onComplete: function (res)
                                    {
                                        while ($('muncod').options[1]) {
                                            $('muncod').options[1] = null;
                                        }

                                        eval(res.responseText);

                                        for (var i = 1; i < listaMunicipios[estuf].length; i++) {
                                            $('muncod').options[i] = new Option(listaMunicipios[estuf][i][1],
                                                                                listaMunicipios[estuf][i][0],
                                                                                false,
                                                                                false);
                                        }
                                    }
    })
  }

</script>


<div align="center">

<form method="post" name="formulario" id="frmbusca" onsubmit="return validarFrmBusca()">
  <center>
    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
      <tr>
        <td align="right" class="SubTituloDireita">Função:</td>
        <td><?php echo $db->monta_combo('funid',
                                        'select funid as codigo, fundsc as descricao from entidade.funcao where funid=2 OR funid=15 order by funid asc',
                                        'S',
                                        'Função',
                                        '',
                                        '',
                                        '',
                                        '',
                                        '',
                                        '',
                                        'funid',
        								$_REQUEST['funid']); ?>
      </tr>
      <tr>
        <td align="right" class="SubTituloDireita">CPF:</td>
        <td><?php echo campo_texto('entnumcpfcnpj', 'N', 'S', 'CPF', '20 ', '14', '###.###.###/##', '', 'left', '', 0, 'id="entnumcpfcnpj" onblur="MouseBlur(this);"'); ?></td>
      </tr>

      <tr>
        <td align="right" class="SubTituloDireita">Nome (ou parte do nome):</td>
        <td><?php echo campo_texto('entnome', 'N', 'S', 'Nome', '50', '150', '', '', 'left', '', 0, 'id="entnome" onblur="MouseBlur(this);"'); ?></td>
      </tr>

      <tr>
        <td align="right" class="SubTituloDireita">UF:</td>
        <td><?php echo $db->monta_combo('estuf',
                                        'select estuf as codigo, estdescricao as descricao from territorios.estado order by estuf asc',
                                        'S',
                                        'UF',
                                        'carregarMunicipios',
                                        '',
                                        '',
                                        '',
                                        '',
                                        '',
                                        'estuf',
        								$_REQUEST['estuf']); ?>
        </td>
      </tr>

      <tr>
        <td align="right" class="SubTituloDireita">Município:</td>
        <td><select name="muncod" class="CampoEstilo" id="muncod"><option value="null">Selecione a UF</option></select></td>
      </tr>

      <tr>
        <td colspan="2" align="center">
          <input type="hidden" name="entbusca" value="1" />
          <input type="submit" value="Buscar" />
          <input type="button" value="Inclcuir" onclick="incluirEntidade()" />
        </td>
      </tr>

    </table>
  </center>
</form>

<center>
<?php

if (array_key_exists('entbusca', $_REQUEST) || $_REQUEST['entbusca'] == '1') {
    $db->monta_lista($sql_busca_prefeitos, array('Ação', 'CPF', 'Nome', 'Órgão'), 10, 50, 'N', '', '');
}

?>
</center>
</div>
