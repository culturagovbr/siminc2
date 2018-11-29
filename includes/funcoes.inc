<?php

/**
 * #functionName substituir_char_especiais_word
 *
 * @author Luciano F. Ribeiro
 *
 * @param string $_string é o "conjunto de palavras" que será rastreado símbolos ou caracteres especiais, como o dash do WORK, e substituido pelo seu respectivo. Para ser usada é nescessário passar o paramentro que é uma string.
 * @return string $_string é o "conjunto de palavras" já mapeada e com as alterações nescessária.
 *
 * @version v1
*/
function substituir_char_especiais_word( $_string ){

	#CASO HAJA NESCESSIDADE DE ADICIONAR NOVOS "SÍMBOLOS" PARA A CONVERSÃO, ABAIXO SE ENCONNTRA ALGUNS, OS MAIS "COMUNS".
	/*
	'?'=>'S', '?'=>'s', '?'=>'Z', '?'=>'z', 'À'=>'A', 'Á'=>'A', 'Â'=>'A', 'Ã'=>'A', 'Ä'=>'A', 'Å'=>'A', 'Æ'=>'A', 'Ç'=>'C', 'È'=>'E', 'É'=>'E',
	'Ê'=>'E', 'Ë'=>'E', 'Ì'=>'I', 'Í'=>'I', 'Î'=>'I', 'Ï'=>'I', 'Ñ'=>'N', 'Ò'=>'O', 'Ó'=>'O', 'Ô'=>'O', 'Õ'=>'O', 'Ö'=>'O', 'Ø'=>'O', 'Ù'=>'U',
	'Ú'=>'U', 'Û'=>'U', 'Ü'=>'U', 'Ý'=>'Y', 'Þ'=>'B', 'ß'=>'Ss', 'à'=>'a', 'á'=>'a', 'â'=>'a', 'ã'=>'a', 'ä'=>'a', 'å'=>'a', 'æ'=>'a', 'ç'=>'c',
	'è'=>'e', 'é'=>'e', 'ê'=>'e', 'ë'=>'e', 'ì'=>'i', 'í'=>'i', 'î'=>'i', 'ï'=>'i', 'ð'=>'o', 'ñ'=>'n', 'ò'=>'o', 'ó'=>'o', 'ô'=>'o', 'õ'=>'o',
	'ö'=>'o', 'ø'=>'o', 'ù'=>'u', 'ú'=>'u', 'û'=>'u', 'ý'=>'y', 'ý'=>'y', 'þ'=>'b', 'ÿ'=>'y'
	*/

	#SUBSTITUIR POR CARACTERES CORRETOS.
	$nao_desejado_array = array(
		chr(96) => "'", chr(130) => ",", chr(133) => "...", chr(145) => "'", chr(146) => "'",  chr(147) => "\"",
		chr(148) => "\"", chr(151) => "-", chr(180) => "'", chr(171) => "&laquo;", chr(182) => "", chr(187) => "&raquo;"
	);

	$_string = strtr( $_string, $nao_desejado_array );

	return $_string;
}

/* Array unique para arrays multidimencionais
 * */
function arrayUnique($array, $preserveKeys = false)
{
	// Unique Array for return
	$arrayRewrite = array();
	// Array with the md5 hashes
	$arrayHashes = array();
	foreach($array as $key => $item) {
		// Serialize the current element and create a md5 hash
		$hash = md5(serialize($item));
		// If the md5 didn't come up yet, add the element to
		// to arrayRewrite, otherwise drop it
		if (!isset($arrayHashes[$hash])) {
			// Save the current element hash
			$arrayHashes[$hash] = $hash;
			// Add element to the unique Array
			if ($preserveKeys) {
				$arrayRewrite[$key] = $item;
			} else {
				$arrayRewrite[] = $item;
			}
		}
	}
	return $arrayRewrite;
}

function somenteNumeros( $string ){
	return preg_replace('/[^0-9]/', '',$string, -1);
}

function MoedaToBd($vlr){
   $string = str_replace(".","",$vlr);
   $string = str_replace(",",".",$string);

   return $string;
}

function resumo($texto,$qnt){
    $resumo=substr($texto,'0',$qnt);
    $last=strrpos($resumo," ");
    $resumo=substr($resumo,0,$last);
    return $resumo."...";
}

/**
 * Componente para um popup com uma lista ordenada.
 * @param $params array
 *
 * O parâmetros do componentes são passados por um array, sendo
 * que cada chave do array representa uma funcionalidade específica.
 * Se o array for vazio não imprime nenhum conteúdo.
 *
 * Abaixo a lista com as chaves do array que podem ser utilizadas:
 * 		-> 'nome':
 * 		-> 'valueButton': o texto(value) do botão que abre a popup. Se não for informado o padrão do texto é "Abrir".
 * 		-> 'sql': a query para os dados da lista. Deve ser informado os alias "codigo" e "descricao", para definição do
 * 				  valor(value) do checkbox e do texto que se segue, respectivamente.
 * 		-> 'titulo': título usado na popup para descrever os itens da lista. Se não for informado o padrão é "Selecione o(s) item(ns) desejado(s):".
 */
function popLista( $params = array() )
{
	/*** Se algum parâmetro foi criado... ***/
	if( !empty($params) )
	{
		/*** ***/
		if( $params['nome'] )
		{
			/*** Se o SQL foi definido... ***/
			if( $params['sql'] )
			{
				/*** Joga o SQL na sessão para uso no arquivo popLista.php ***/
				$_SESSION['sql_pop_lista'][$params['nome']] = $params['sql'];
			}

			/*** Cria a variável com o value do botão. Se não houver sido informada, usa-se o padrão ***/
			$valueButton = ( $params['valueButton'] ) ? $params['valueButton'] : 'Abrir';
			/*** Cria variável com o título da popup. Se não houver sido informada, usa-se o padrão ***/
			$titulo = ( $params['titulo'] ) ? $params['titulo'] : 'Selecione o(s) item(ns) desejado(s):';
			/*** Imprime o botão que abrir a pop-up. (Arquivo: www/geral/popLista.php) ***/
			echo '<input type="button" value="'.$valueButton.'" onclick="abrePopLista(\''.urlencode($params['nome']).'\', \''.urlencode($titulo).'\');" />';
		}
	}
}

/*
 Sistema Sistema Simec
 Setor responsável: SPO/MEC
 Desenvolvedor: Desenvolvedores Simec
 Analista: Gilberto Arruda Cerqueira Xavier
 Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
 Módulo: funcoes.inc
 Finalidade: reunião de várias funcoes que não dependem de banco
 Data de criação: 24/06/2005
 */

function emailPara($cpf){
	$html = "<img src='../imagens/email.gif' title='Enviar e-mail' border='0' onclick='emailPara(\"{$cpf}\");' style='cursor:pointer;'>&nbsp;";

	return $html;
}

/**
 * Converte Inteiros em Algarismos Romanos
 * por: Werter Dias Almeida em 11/03/2009
 * @param integer valor
 * @return string
 */
function decimal2romano($decimal)
{
	$romano = "";
	while ($decimal/1000 >= 1) {$romano .= "M"; $decimal = decimal-1000;}
		if ($decimal/900 >= 1) {$romano .= "CM"; $decimal=$decimal-900;}
		if ($decimal/500 >= 1) {$romano .= "D"; $decimal=$decimal-500;}
		if ($decimal/400 >= 1) {$romano .= "CD"; $decimal=$decimal-400;}
	while ($decimal/100 >= 1) {$romano .= "C"; $decimal = $decimal-100;}
		if ($decimal/90 >= 1) {$romano .= "XC"; $decimal=$decimal-90;}
		if ($decimal/50 >= 1) {$romano .= "L"; $decimal=$decimal-50;}
		if ($decimal/40 >= 1) {$romano .= "XL"; $decimal=$decimal-40;}
	while ($decimal/10 >= 1) {$romano .= "X"; $decimal = $decimal-10;}
		if ($decimal/9 >= 1) {$romano .= "IX"; $decimal=$decimal-9;}
		if ($decimal/5 >= 1) {$romano .= "V"; $decimal=$decimal-5;}
		if ($decimal/4 >= 1) {$romano .= "IV"; $decimal=$decimal-4;}
	while ($decimal >= 1) {$romano .= "I"; $decimal = $decimal-1;}

	return $romano;
}

/*
 * função que faz a verificação do CPF
 *
 * @param (string) $cpf
 * @return (true) caso o CPF seja válido
 * @return (false) caso inválido.
 * @author: FelipeChiavicatti;
 */
function validaCPF($cpf) {
	/*
	*/
	$nulos = array("12345678909","11111111111","22222222222","33333333333",
	               "44444444444","55555555555","66666666666","77777777777",
	               "88888888888","99999999999","00000000000","12345678909",
	               "00000000191","22525837541","23111478203","28075746708",
				   "91087662826");

	/* Retira todos os caracteres que nao sejam 0-9 */
	$cpf = preg_replace('/[^0-9]/', '',$cpf, -1);
//ver($cpf);
	/*Retorna falso se houver letras no cpf */
//	if (!(preg_match("/[0-9]/", $cpf)))
//ver('Contém letras no cpf', d);
//	    return 0;
//ver('NÃO Contém letras no cpf', d);
	/* Retorna falso se o cpf for nulo */
	if( in_array($cpf, $nulos) )
	    return 0;

	if (strlen($cpf) > 11)
		return 0;

	/*Calcula o penúltimo dígito verificador*/
	$acum=0;
	for($i=0; $i<9; $i++) {
	  $acum+= $cpf[$i]*(10-$i);
	}

	$x=$acum % 11;
	$acum = ($x>1) ? (11 - $x) : 0;

	/* Retorna falso se o digito calculado eh diferente do passado na string */
	if ($acum != $cpf[9]){
	  return 0;
	}

	/*Calcula o último dígito verificador*/
	$acum=0;
	for ($i=0; $i<10; $i++){
	  $acum+= $cpf[$i]*(11-$i);
	}

	$x=$acum % 11;
	$acum = ($x > 1) ? (11-$x) : 0;

	/* Retorna falso se o digito calculado eh diferente do passado na string */
	if ( $acum != $cpf[10]){
	  return 0;
	}

	/* Retorna verdadeiro se o cpf eh valido */
	return 1;
}

function validarCnpj($cnpj){
  $cnpj = str_pad(str_replace(array('.','-','/'),'',$cnpj),14,'0',STR_PAD_LEFT);
  if (strlen($cnpj) != 14){
    return false;
  }else{
    for($t = 12; $t < 14; $t++){
      for($d = 0, $p = $t - 7, $c = 0; $c < $t; $c++){
        $d += $cnpj{$c} * $p;
        $p  = ($p < 3) ? 9 : --$p;
      }
      $d = ((10 * $d) % 11) % 10;
      if($cnpj{$c} != $d){
        return false;
      }
    }
    return true;
  }
}

function get_rnd_iv($iv_len)
{
	// gera um caracter aleatório em função de um número aleatório gerado pela função mt_rand
	$iv = '';
	while ($iv_len-- > 0) {
		$iv .= chr(mt_rand() & 0xff);
	}
	return $iv;
}

//Função que retorna true caso contenha a valor procurado em um array ou false caso não encontre
function in($valor, $vetor)
{
	$key = array_search($valor, $vetor);
	if ($key==null and $vetor[0]<>$valor) return false; else return true;
}

function formata_numero($nm) {
	for ($done=strlen($nm); $done > 3;$done -= 3) {
		$returnNum = ".".substr($nm,$done-3,3).$returnNum;
	}
	return substr($nm,0,$done).$returnNum;
}

function formata_numero_monetario($nm) {
	return number_format($nm, 2, ',', '.');
}

function aleatorio() {
	$aleatorio = '';
	while (strlen($aleatorio) < 15)
	$aleatorio = $aleatorio.mt_rand(2000,40000);
	Return $aleatorio;
}
function verifica_datas($dtmaior,$dtmenor,$dif) {
	if (
	mktime (0,0,0,substr($dtmaior,3,2),substr($dtmaior,0,2),substr($dtmaior,6,4)) -
	mktime (0,0,0,substr($dtmenor,3,2), substr($dtmenor,0,2), substr($dtmenor,6,4))
	> $dif
	)
	{
		return false;
	}
	else {
		return true;
	}
}

function ajusta_data($data)
{
	// retorna a data no formato yyyy-mm-dd
	return substr($data,6,4).'-'.substr($data,3,2).'-'.substr($data,0,2);
}

function updateArquivo($nome,$unicod,$codResposta){

	global  $db;
	$sql = "update reuni.resposta set rspdsc = $nome where unicod = '$unicod' and rspcod = $codResposta ";
	$db->executar($sql);
	$db->commit();
	return;
}

function formata_data($data, $formato = "%d/%m/%Y")
{
	// retorna a data no formato yyyy-mm-dd
	//if ($data) return substr($data,0,2).'/'.substr($data,3,2).'/'.substr($data,6,4);

	if ($data) return strftime($formato, strtotime($data));

	else return '';
}

function formata_data_sql($data)
{
	if ($data) return trim(substr($data,6,4)).'-'.trim(substr($data,3,2)).'-'.trim(substr($data,0,2));
	else return '';
}


function controla_navegacao($nl)
{
	if ($_POST['navega'] == 0) $_SESSION['registro'] = 0;
	else if ($_POST['navega'] == -2) $_SESSION['registro'] = $nl;
	else
	{
		$_SESSION['registro'] = $_SESSION['registro'] + $_POST['navega'];
		if ($_SESSION['registro'] < 0) $_SESSION['registro'] = 0;
		if ($_SESSION['registro'] > $nl) $_SESSION['registro'] = $nl;
	}
	if (! $_SESSION['registro'] ) $_SESSION['registro'] = 0;
}
function md5_encrypt_senha($plain_text, $password, $iv_len = 16)
{
	$plain_text .= "\x13";
	$n = strlen($plain_text);
	if ($n % 16) $plain_text .= str_repeat("\0", 16 - ($n % 16));
	$i = 0;
	$enc_text = get_rnd_iv($iv_len);
	$iv = substr($password ^ $enc_text, 0, 512);
	while ($i < $n) {
		$block = substr($plain_text, $i, 16) ^ pack('H*', md5($iv));
		$enc_text .= $block;
		$iv = substr($block . $iv, 0, 512) ^ $password;
		$i += 16;
	}
	return base64_encode($enc_text);
	// return base64_encode(trim($plain_text));
}
function md5_encrypt($plain_text, $password='', $iv_len = 16)
{
	//$plain_text .= "\x13";
	$n = strlen($plain_text);
	if ($n % 16) $plain_text .= str_repeat("\0", 16 - ($n % 16));
	$i = 0;
	$enc_text = get_rnd_iv($iv_len);
	$iv = substr($password ^ $enc_text, 0, 512);
	while ($i < $n) {
		$block = substr($plain_text, $i, 16) ^ pack('H*', md5($iv));
		$enc_text .= $block;
		$iv = substr($block . $iv, 0, 512) ^ $password;
		$i += 16;
	}
	//return base64_encode($enc_text);
	return base64_encode(trim($plain_text));
}
Function obrigatorio()
{
	$obrigatorio = " <img border='0' src='". URL_SISTEMA. "imagens/obrig.gif' title='Indica campo obrigatório.' />";
	return $obrigatorio;
}
function md5_decrypt($enc_text, $password='', $iv_len = 16)
{
	$enc_text = base64_decode($enc_text);
	$n = strlen($enc_text);
	$i = $iv_len;
	$plain_text = '';
	$iv = substr($password ^ substr($enc_text, 0, $iv_len), 0, 512);
	while ($i < $n) {
		$block = substr($enc_text, $i, 16);
		$plain_text .= $block ^ pack('H*', md5($iv));
		$iv = substr($block . $iv, 0, 512) ^ $password;
		$i += 16;
	}
	return $enc_text;
	//return preg_replace('/\\x13\\x00*$/', '', $plain_text);
}

function md5_decrypt_senha($enc_text, $password, $iv_len = 16)
{
	$enc_text = base64_decode($enc_text);
	$n = strlen($enc_text);
	$i = $iv_len;
	$plain_text = '';
	$iv = substr($password ^ substr($enc_text, 0, $iv_len), 0, 512);
	while ($i < $n) {
		$block = substr($enc_text, $i, 16);
		$plain_text .= $block ^ pack('H*', md5($iv));
		$iv = substr($block . $iv, 0, 512) ^ $password;
		$i += 16;
	}
	return preg_replace('/\\x13\\x00*$/', '', $plain_text);

}

function monta_titulo2($linha1,$linha2)
{
	//print '<center><div id="cabecalho" align="center" style="background-color:#DCDCDC;padding-top:3px;padding-bottom:3px;width:95%;" class="TituloTela">'.$linha1.'</div></center>';
	$retorno = '<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;">';
	$retorno .= '<tr><td width="100%" align="center"><label class="TituloTela" style="color:#000000;">'.$linha1.'</label></td></tr><tr>';
	$retorno .= '<td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" >'.$linha2.'</td></tr></table>';
	return $retorno;
}

function monta_titulo($linha1,$linha2)
{
    if($linha2){
        $subtitulo = ' <small>' . $linha2 . '</small>';
    } else {
        $subtitulo = '';
    }
//    style="text-align: center;"
//    $html = '
//        <div class="row">
//            <div class="col-md-12">
//                <div class="page-header" style="text-align: center">
//                    <h3 contenteditable="true" >' . $linha1 . $subtitulo . '</h3>
//                </div>
//            </div>
//        </div>
//        <br />
//    ';
//
//    echo $html;

//	print '<center><div id="cabecalho" align="center" style="background-color:#DCDCDC;padding-top:3px;padding-bottom:3px;width:95%;" class="TituloTela">'.$linha1.'</div></center>';



//    $linha2 = '<small>Testando</small>';

	print '<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;">';
	print '<tr><td width="100%" align="center"><label class="TituloTela" style="color:#000000;">'.$linha1.'</label></td></tr>';
    if($linha2){
        print '<tr><td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" >'.$linha2.'</td></tr>';
    }

    print '</table>';
}


function sql_vincula_arquivo ($campo,$item,$chave=0)
{
	//esta funcao tem por finalidade montar a sql que permitirá listar os arquivos que
	// poderão ser listados para download
	$usucpf=$_SESSION['usucpf'];
	$sql = "select d.usucpf as donoarquivo, d.tpdcod, d.docid,d.docdsc as descricao, d.docnomefisico, case when d.docsigilo='O' then 'Ostensivo' when d.docsigilo='R' then 'Reservado' else 'Confidencial' end as sigilo from public.documento d inner join public.documento_processo dp on d.docid=dp.docid and dp.tabela_processo='$campo' inner join $campo cc on cc.$chave=$item and cc.$chave=dp.proid order by d.docsigilo,d.docdsc asc";
	return $sql;


}


function formatar_cpf($cpf) {
	if ( empty($cpf) ) return ;
	else return substr($cpf,0,3).".".substr($cpf,3,3).".".substr($cpf,6,3)."-".substr($cpf,9,2);
}
function formatar_cnpj($cnpj) {
	if ( empty($cnpj) ) return ;
	return substr($cnpj,0,2).".".substr($cnpj,2,3).".".substr($cnpj,5,3)."/".substr($cnpj,8,4)."-".substr($cnpj,12,2);
}
function pega_numero($cnpjcpf) {
	if ( empty($cnpjcpf) ) return ;
	return preg_replace( "/[^\d]/", "", $cnpjcpf );
}

function formatar_cpf_cnpj($cpfcnpj) {
	if ( strlen($cpfcnpj) == 11 )
	return substr($cpfcnpj,0,3).".".substr($cpfcnpj,3,3).".".substr($cpfcnpj,6,3)."-".substr($cpfcnpj,9,2);
	else if ( strlen($cpfcnpj) == 14 )
	return substr($cpfcnpj,0,2).".".substr($cpfcnpj,2,3).".".substr($cpfcnpj,5,3)."/".substr($cpfcnpj,8,4)."-".substr($cpfcnpj,12,2);
	else return;
}
function make_seed() {
	list($usec, $sec) = explode(' ', microtime());
	return (float) $sec + ((float) $usec * 100000);
}
function senha() {
	mt_srand(make_seed());
	for($i=0;$i<8;$i++) {
		$n = (integer)mt_rand(0,35);
		$c = ($n<10) ? ($n+48) : ($n+55);
		$senha.= pack ("c",$c);
	}
	return $senha;
}
function email($paraquem, $paraonde, $assunto, $message, $cc='',$cco='', $from='')
{
	global $db;
	require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
	require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';

	//trata mensagem
	if($message) $message = str_replace(chr(13), '<br>', $message);

	$mensagem = new PHPMailer();
	$mensagem->persistencia = $db;
	$mensagem->Host         = "localhost";
	$mensagem->Mailer       = "smtp";
	$mensagem->FromName		= SIGLA_SISTEMA. " - ".strtoupper($_SESSION['sisdiretorio'])." - " . $_SESSION['usunome'] . " - " . $_SESSION['usuorgao'];

	//$mensagem->AddReplyTo($_SESSION['email_sistema']);
	//$mensagem->AddReplyTo($_SESSION["usuemail"]);
	//$mensagem->From = $_SESSION['email_sistema'];
	$mensagem->AddReplyTo('no-reply@mec.gov.br');
	if($from)
		{ $mensagem->From = $from; }
	else
		{ $mensagem->From = $_SESSION["usuemail"]; }

	$mensagem->AddAddress( $paraonde, $paraquem );
	$mensagem->Subject = $assunto;
	$mensagem->Body = $message;
	$mensagem->IsHTML( true );
	foreach ( explode( ";", $cc ) as $end )
	{
		$end = trim( $end );
		if( $end )
		{
			$mensagem->AddCC( $end );
		}
	}
	foreach ( explode( ";", $cco ) as $end )
	{
		$end = trim( $end );
		if( $end )
		{
			$mensagem->AddBCC( $end );
		}
	}

	return $mensagem->Send();


	/*

	global $db;
	// esta função está sendo alterada para incluir sempre a origem e para enviar um email para a mesma origem
	$sql = "select u.usunome, u.usuemail, un.unidsc,o.orgdsc from seguranca.usuario u left join unidade un on un.unicod=u.unicod left join orgao o on o.orgcod=u.orgcod where u.usucpf='".$_SESSION['usucpf']."' limit 1";
	$res=$db->pegalinha($sql);
	if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
	//$message=strip_tags($message);
	$dequem = SIGLA_SISTEMA. " - $usunome - $unidsc - $orgdsc"; //trim($_SESSION['sigla']);
	$deonde = "$usuemail;simec@mec.gov.br";//$_SESSION['ittemail'];
	$headers = "Return-Path: simec@mec.gov.br\n";
	$headers .= "Reply-To: simec@mec.gov.br\n";
	$headers .= "X-Sender: simec@mec.gov.br\n";
	$headers .= "Content-type: text/html;\n";
	$headers .= "From: \"".$dequem."\" <".$deonde.">\n";
	$headers .= "X-Mailer: PHP 5.1\n";
	//$headers .= "Reply-To:gilbertoxavier@gmail.com\n";
	if ($cc) $headers .= "Cc:$cc\n";
	if ($cco) $headers .= "Bcc:$cco\n";
	$headers .= "MIME-Version: 1.0\n";
	//dbg($headers,1);
	return mail($paraonde, $assunto, $message, $headers);
	*/
}

function email_pessoal($dequem,$deonde, $paraonde, $assunto, $message, $cc='',$cco='')
{

	global $db;
	require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
	require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
	$mensagem = new PHPMailer();
	$mensagem->persistencia = $db;
	$mensagem->Host         = "localhost";
	$mensagem->Mailer       = "smtp";
	$mensagem->FromName		= SIGLA_SISTEMA. " - " . $dequem;
	$mensagem->AddReplyTo(EMAIL_SISTEMA_NOREPLY);
	$mensagem->From = $deonde;
	$mensagem->AddAddress( $paraonde );
	$mensagem->Subject = $assunto;
	$mensagem->Body = $message;
	$mensagem->IsHTML( true );
	foreach ( explode( ";", $cc ) as $end )
	{
		$end = trim( $end );
		if( $end )
		{
			$mensagem->AddCC( $end );
		}
	}
	foreach ( explode( ";", $cco ) as $end )
	{
		$end = trim( $end );
		if( $end )
		{
			$mensagem->AddBCC( $end );
		}
	}
	return $mensagem->Send();
	/*
	 // $dequem = SIGLA_SISTEMA; //trim($_SESSION['sigla']);
	 // $deonde = $_SESSION['email_sistema'];//$_SESSION['ittemail'];
	 $headers = "Return-Path: simec@mec.gov.br\n";
	 $headers .= "X-Sender: simec@mec.gov.br\n";
	 $headers .= "Reply-To: simec@mec.gov.br\n";
	 $headers .= "Content-type: text/html;\n";
	 $headers .= "From: \"".SIGLA_SISTEMA. ' - '.$dequem."\" <".$deonde.">\n";
	 $headers .= "X-Mailer: PHP 5.1\n";
	 if ($cc) $headers .= "Cc:$cc\n";
	 if ($cco) $headers .= "Bcc:$cco\n";
	 $headers .= "MIME-Version: 1.0\n";
	 return mail($paraonde, $assunto, $message, $headers);
	 */
}

function enviar_email( $remetente, $destinatario, $assunto, $conteudo, $cc='',$cco='', $arquivos = array()){
	require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
	require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
	$mensagem = new PHPMailer();
	global $db;
	$mensagem->persistencia = $db;
//	$mensagem->Host         = "localhost";
	$mensagem->Host         = $_SESSION['config_smtp'];
	$mensagem->Mailer       = "smtp";

	/* Regras definidas pelo Cristiano (11/11/2008)*/
	//$mensagem->From = $remetente['email'];

	/* Regras redefinidas pelo Cristiano (08/10/2009)*/
	$mensagem->From = $_SESSION['email_sistema'];

	if(count($remetente)==2) {
		$mensagem->AddReplyTo($remetente['email'],$remetente['nome']);
		$mensagem->FromName = $remetente['nome'];
	} elseif($remetente != '') {
		$mensagem->FromName = is_array($remetente) ? $remetente['nome'] : "Administrador ". SIGLA_SISTEMA;
	} else {
		$mensagem->FromName = "Administrador ". SIGLA_SISTEMA;
	}
	/* FIM regras*/

	if( is_array( $destinatario ) ){
		foreach( $destinatario as $email ){
            if(is_array($email)){
                if(isset($email['usuemail']) && isset($email['usunome'])){
                    $mensagem->AddAddress( $email['usuemail'], $email['usunome']);
                } else {
                    $mensagem->AddAddress($email);
                }
            } else {
                $mensagem->AddAddress($email);
            }
		}
	}
	else{
		$mensagem->AddAddress( $destinatario );
	}

	/*
	 * Atualizado pelo Alexandre Dourado
	 * recebe um array de email
	 */
	if(is_array($cc) || count($cc) > 1) {
		foreach($cc as $email) {
			$mensagem->AddCC( $email );
		}
	} else {
		$mensagem->AddCC( $cc );
	}
	/*
	 * FIM Atualizado pelo Alexandre Dourado
	 */

        /*
         * Atualizado pelo Ruy Ferreira
         * copia oculta
         */
        if(is_array($cco)){
            foreach($cco as $valueCco){
                $valueCco = trim( $valueCco );
                if( $valueCco ) $mensagem->AddBCC( $valueCco );
            }
        } else {
             $valueCco = trim( $cco );
             if( $valueCco ) $mensagem->AddBCC( $valueCco );
        }

	/*
	 * FIM Atualizado pelo Ruy Ferreira
	 */



	# Atualizado pelo Wesley Romualdo
	//anexa os arquivos
	foreach ( $arquivos as $arquivo ){
		if ( !file_exists( $arquivo['arquivo'] ) ) {
			continue;
		}
                $arqtipo = isset($arquivo['arqtipo']) ? $arquivo['arqtipo'] : 'application/octet-stream';
		$mensagem->AddAttachment($arquivo['arquivo'], basename($arquivo['nome']) , 'base64', $arqtipo);
	}
	# Fim Atualizado pelo Wesley Romualdo

	$mensagem->Subject = $assunto;
	$mensagem->Body = $conteudo;
	$mensagem->IsHTML( true );
    if($mensagem->Host){
        return $mensagem->Send();
    } else {
        return TRUE;
    }
}

function enviar_email_usuario( $remetente, $destinatario, $assunto, $conteudo, $cc='',$cco='' ){
	require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
	require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
	$mensagem = new PHPMailer();
	global $db;
	$mensagem->persistencia = $db;
	$mensagem->Host         = "localhost";
	$mensagem->Mailer       = "smtp";
	$mensagem->From 		= $remetente;
	$mensagem->FromName 	= SIGLA_SISTEMA. " - ".strtoupper($_SESSION['sisdiretorio'])." - " . $_SESSION['usunome'];
	$mensagem->AddAddress( $destinatario );
	$mensagem->Subject 		= $assunto;
	$mensagem->Body 		= $conteudo;
	$mensagem->IsHTML( true );

	//ver($mensagem);
	foreach ( explode( ";", $cc ) as $end )
	{
		$end = trim( $end );
		if( $end )
		{
			$mensagem->AddCC( $end );
		}
	}
	foreach ( explode( ";", $cco ) as $end )
	{
		$end = trim( $end );
		if( $end )
		{
			$mensagem->AddBCC( $end );
		}
	}

	return $mensagem->Send();
}


function ulcase($variavel) {
	// altera o primeiro caracter de cada palavra de uma expresaão, transformando em maiúsculo, com exceções de artigos, pronomes, etc.
	setlocale("LC_CTYPE","iso_8859_1");
	$nom=explode(" ",$variavel);
	$nom[0]=ucfirst(strtolower($nom[0]));
	for ($j=1;$nom[$j];$j++) {
		$nom[$j]=strtolower($nom[$j]);
		$nom[$j]=ucfirst($nom[$j]);
		if ( ereg("^(Com|Sem|Para|Por|Dos|Das|Ou)$",$nom[$j],$temp ) ) {
			$nom[$j]=strtolower($nom[$j]);
		}
		if ( ereg("^[DN].$",$nom[$j],$temp ) ) {
			$nom[$j]=strtolower($nom[$j]);
		}
		if ( ereg("^[AEO]$",$nom[$j],$temp ) ) {
			$nom[$j]=strtolower($nom[$j]);
		}
	}
	$nom_int = implode($nom," ");
	if ( substr($nom_int,-3,1) == "/" )
	$nom_int = substr($nom_int,0,strlen($nom_int)-3)." - "
	.strtoupper(substr($nom_int,-2));
	return $nom_int;
}
function tirar_acentos($variavel) {
	$busca= array("'Á'","'À'","'Ã'","'Ä'","'Â'","'É'","'È'","'Ê'","'Ë'",
	"'Í'","'Ì'","'Ï'","'Î'","'Ñ'",
	"'Ó'","'Ò'","'Ô'","'Õ'","'Ö'","'Ú'","'Ù'","'Ü'","'Û'",
	"'Ý'","'Ç'",
	"'á'","'à'","'ã'","'ä'","'â'","'é'","'è'","'ê'","'ë'",
	"'í'","'ì'","'ï'","'î'","'ñ'",
	"'ó'","'ò'","'ô'","'õ'","'ö'","'ú'","'ù'","'ü'","'û'",
	"'ý'","'ç'");
	$subst= array("A","A","A","A","A","E","E","E","E","I","I","I","I","N",
	"O","O","O","O","O","U","U","U","U","Y","C",
	"a","a","a","a","a","e","e","e","e","i","i","i","i","n",
	"o","o","o","o","o","u","u","u","u","y","c");
	$result = preg_replace($busca,$subst,$variavel);
	return $result;
}
function dv_cpf_cnpj_ok($cpfcnpj) {
	$dv = false;
	$cpfcnpj = preg_replace('/[^0-9]/', '', $cpfcnpj, -1);
	if ( strlen($cpfcnpj) == 14 ) {
		$cnpj_dv = substr($cpfcnpj,-2);
		for ( $i = 0; $i < 2; $i++ ) {
			$soma = 0;
			for ( $j = 0; $j < 12; $j++ )
			$soma += substr($cpfcnpj,$j,1)*((11+$i-$j)%8+2);
			if ( $i == 1 ) $soma += $digito * 2;
			$digito = 11 - $soma  % 11;
			if ( $digito > 9 ) $digito = 0;
			$controle .= $digito;
		}
		if ( $controle == $cnpj_dv )
		$dv = true;
	}
	if ( strlen($cpfcnpj) == 11 ) {
		$cpf_dv = substr($cpfcnpj,-2);
		for ( $i = 0; $i < 2; $i++ ) {
			$soma = 0;
			for ( $j = 0; $j < 9; $j++ )
			$soma += substr($cpfcnpj,$j,1)*(10+$i-$j);
			if ( $i == 1 ) $soma += $digito * 2;
			$digito = ($soma * 10) % 11;
			if ( $digito == 10 ) $digito = 0;
			$controle .= $digito;
		}
		if ( $controle == $cpf_dv )
		$dv = true;
	}
	return $dv;
}
function formata_fone_fax($numero) {
	$numero = preg_replace('/[^0-9]/', '',$numero, -1);
	$numero = preg_replace('/^0+/', '',$numero, -1);
	if ( strlen($numero) == 14 or strlen($numero) == 12 )
	$numero = substr($numero,-10);
	if ( strlen($numero) == 13 )
	$numero = substr($numero,-9);
	$res = substr($numero,-4);
	if ( strlen($numero) > 4  and strlen($numero) < 9 )
	$res = substr($numero,0,strlen($numero)-4)."-".$res;
	if ( strlen($numero) > 8 )
	$res = "(0XX".substr($numero,0,2).") ". substr($numero,2,strlen($numero)-6)."-".$res;
        
	return $res;
}
function formata_cep($num) {
	$num = preg_replace('/[^0-9]/', '', $num, -1);
	$res = substr($num,0,5);
	if ( strlen($num) > 5 ) $res.="-".substr($num,5,3);
	return $res;
}
function verifica_email ($email) {
	return(preg_match("'^[a-z0-9_.=-]+@(?:[a-z0-9-]+\.)+([a-z]{2,3})\$'i",
	$email));
}
function verifica_cep ($cep) {
	$res=false;
	$cep = preg_replace('/[^0-9]/', '', $cep, -1);
	if ( strlen($cep) == 8 ) $res=true;
	return $res;
}
function verifica_data($data) {
	$dv = false;
	$data = preg_replace('/[^0-9]/', '', $data, -1);
	$tam = strlen($data);
	if ( $tam == 8 ) {
		$dia = substr($data,0,2);
		$mes = substr($data,2,2);
		$ano = substr($data,4,4);
		if ( $dia > 0 && $dia < 32 &&
		$mes > 0 && $mes < 13 &&
		$ano > 1900 && $ano < 2100 ) $dv=true;
	}
	return $dv;
}

/**
 * Retorna SQL referente a consulta de perfis por sistema de um determinado sistema
 *
 * @param integer $sisid Codigo do sistema
 * @global object $db objeto de conexão
 * @return Retorna SQL da consulta de perfil
 */
function retornarSqlBuscarPerfil($sisid){
    $sql = "";
    if(!empty($sisid)){
        $sql .= "
            SELECT
                t_p.pflcod AS codigo,
                t_p.pfldsc AS descricao
            FROM
                seguranca.perfil t_p
            WHERE
                t_p.pflstatus = 'A'
                AND t_p.sisid = $sisid
        ";
    }
    return $sql;
}

/**
 * Busca perfis de um determinado sistema
 *
 * @param integer $sisid Codigo do sistema
 * @global object $db objeto de conexão
 * @return Retorna lista de perfil
 */
function buscarPerfis($sisid){
    global $db;
    $listaPerfil = array();

    if(!empty($sisid)){
        $listaPerfil = $db->carregar(retornarSqlBuscarPerfil($sisid));
    }
    return $listaPerfil;
}

function verificaPerfil( $pflcods )
{
	global $db;
	if ( is_array( $pflcods ) )
	{
		$pflcods = array_map( "intval", $pflcods );
		$pflcods = array_unique( $pflcods );
	}
	else
	{
		$pflcods = array( (integer) $pflcods );
	}
	if ( count( $pflcods ) == 0 )
	{
		return false;
	}
	$sql = "
		select
			count(*)
		from seguranca.perfilusuario pu
		inner join seguranca.usuario_sistema as us on us.usucpf = pu.usucpf
		where
			pu.usucpf = '" . $_SESSION['usucpf'] . "' and
			pu.pflcod in ( " . implode( ",", $pflcods ) . " ) and
			us.sisid = ".$_SESSION['sisid']."
		";
	return $db->pegaUm( $sql ) > 0;
}

/*
 function dv_banco_11_cb($numero) {
 $numero = preg_replace('/[^0-9]/', '', $numero, -1);
 $len = strlen($numero);
 for ( $i = 0; $i<$len; $i++ )
 $soma+= (($len-$i-1)%8+2) * intval(substr($numero,$i,1));
 $dv = 11 - $soma%11;
 if ( $dv > 9 ) {
 $dv="1";
 }
 return $dv;
 }
 function dv_banco_11($numero,$x=false) {
 $numero = preg_replace('/[^0-9]/', '', $numero, -1);
 $len = strlen($numero);
 for ( $i = 0; $i<$len; $i++ )
 $soma+= (9-($len-$i-1)%8) * intval(substr($numero,$i,1));
 $dv = $soma%11;
 if ( $dv == 10 ) {
 if ( $x ) $dv="X";
 else $dv="0";
 }
 return $dv;
 }
 function dv_banco_10($numero) {
 $numero = preg_replace('/[^0-9]/', '', $numero, -1);
 $len = strlen($numero);
 for ( $i = 0; $i<$len; $i++ ) {
 $s = (2-($len-$i-1)%2) * intval(substr($numero,$i,1));
 if ( $s > 9 ) $s = intval(substr($s,0,1)) + intval(substr($s,1,1));
 $soma+=$s;
 }
 $dv = 10-$soma%10;
 if ( $dv == 10 ) $dv = 0;
 return $dv;
 }
 */
function corrige_cpf($cpf)
{
    return preg_replace('/[^0-9]/', '', $cpf, -1);

}

function exibir_erros($erros) {
	$num = count($erros);
	if(!$num) return 0;
	echo "Foram encontrado(s) o(s) seguinte(s) erro(s) no seu formulário:<br>\n";
	for($x=0;$x<$num;$x++) {
		echo "<br>&nbsp;&nbsp;&nbsp;&nbsp;"."<font color=red><b>".$erros[$x]."</b></font>"."\n";
	}
	return 1;
}
function fator_vencimento($data) {
	$dat = explode("/",$data);
	return intval((mktime(0,0,0,$dat[1],$dat[0],$dat[2])-mktime(0,0,0,10,7,1997))/86400);
}
function limpar_numero($num) {
	for($x=0;$x<strlen($num);$x++) {
		if(ereg("[0-9]",$num[$x])) $saida .= $num[$x];
	}
	return $saida;
}
function md5_senha($senha) {
	return "{md5}".base64_encode(pack("H*",md5($senha)));
}

/**
 * Monta campo de texto com o formato de data.
 *
 * @global type $dt
 * @param type $dt
 * @param type $obrig
 * @param type $habil
 * @param type $label
 * @param type $formata
 * @param type $txtdica
 * @param type $txtOnChange
 * @param type $value
 * @param type $formulario
 * @return string
 */
function campo_data($dt,$obrig,$habil,$label,$formata,$txtdica='',$txtOnChange='', $value = null, $formulario = null)
{
	global ${$dt};
	$txtdica = $txtdica ? "return escape('". $txtdica ."');" : null;
	$formulario = $formulario ? $formulario : 'formulario';

	$title = "";

	if($label)
		$title = ' title="'.$label.'" ';

	if ($habil=='S') {
		//dbg(${$dt},0,'37423428787');
		//dbg('rrr'.formata_data(${$dt}),0,'37423428787');

		$data = '<input '.$title.' type="text" id="'.$dt.'" name="'.$dt;
		if ($formata=='S') $data.='" value="'.formata_data(($value) ? $value : ${$dt}); else $data.='" value="'.${$dt};
		$data.='" size="12" style="text-align: right;" maxlength="10" value="" class="normal"
				  onKeyUp="this.value=mascaraglobal('."'##/##/####',this.value);".'"
				  onchange="' . $txtOnChange . '" onmouseover="MouseOver(this);'. $txtdica .'"
				  onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);"
				  onblur="MouseBlur(this);this.value=mascaraglobal('."'##/##/####',this.value);".'VerificaData(this, this.value);">
				  <a href="javascript:show_calendar('."'".$formulario.".".$dt."');\">".
		'<img src="../imagens/calendario.gif" width="16" height="15" border="0" align="absmiddle" alt=""></a>';
	}
	else
	{
		$data = '<input '.$title.' type="text" id="'.$dt.'" name="'.$dt.
		'" readonly class="normal"value="'.formata_data(($value) ? $value : ${$dt}).
		'" size="12" style="text-align:right;BORDER-LEFT:#888888 3px solid; COLOR:#808080;" maxlength="10">';
	}
	if ($obrig == 'S')
	$data = $data . obrigatorio();

	return $data;
}

/**
 * Cria um campo data2 no formulário
 *
 * @param string  $dt - data
 * @param string  $obrig - se é obrigatório
 * @param string  $habil - se está habilitado
 * @param string  $label
 * @param string  $formata - mascara
 * @param string  $txtdica - Title
 * @param string  $txtOnBlur - funções do onblur
 * @param string  $value - valor inicial
 * @param string  $txtOnChange - funções do onchange
 * @param string  $classe - classe do input
 * @param string  $id - id do input
 * @return mixed
 */

function campo_data2($dt,$obrig,$habil,$label,$formata,$txtdica='',$txtOnBlur='', $value = null,$txtOnChange='', $classe='', $id='' )
{
	global ${$dt};
	$txtdica = $txtdica ? "return escape('". $txtdica ."');" : null;

	$title = "";

	if(!$id){
		$id = $dt;
	}

	if ($obrig == 'S'){
		$clsObrig = " obrigatorio ";
	}

	if($label)
		$title = ' title="'.$label.'" ';

	if ($formata=='S')
		$dataVal =' value="'.formata_data(($value) ? $value : ${$dt}).'"';
	else
		$dataVal =' value="'.( ($value) ? $value : ${$dt} ).'"';

	if ($habil=='S') {
		$data = '<input '.$title.' type="text" id="'.$id.'" name="'.$dt.'" '.$dataVal.'
				  size="12" style="text-align: right;" maxlength="10" value="" class="normal '.$clsObrig.$classe.'"
				  onKeyUp="this.value=mascaraglobal('."'##/##/####',this.value);".'"
				  onchange="' . $txtOnChange . '" onmouseover="MouseOver(this);'. $txtdica .'"
				  onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);"
				  onblur="' . $txtOnBlur . 'validando_data(this);MouseBlur(this);this.value=mascaraglobal(\'##/##/####\',this.value)">';
		$data.= "&nbsp;<img src=\"../includes/JsLibrary/date/displaycalendar/images/calendario.gif\" align=\"absmiddle\" border=\"0\" style=\"cursor:pointer\" title=\"Escolha uma Data\" onclick=\"displayCalendar(document.getElementById('$id'),'dd/mm/yyyy',this)\">";
	} else {
		$data = '<input '.$title.' type="text" id="'.$dt.'" name="'.$dt. '"
				  readonly class="normal '.$classe.'" '.$dataVal.'
				  size="12" style="text-align:right;BORDER-LEFT:#888888 3px solid; COLOR:#808080;" maxlength="10">';
	}

	if ($obrig == 'S')
		$data = $data . obrigatorio();

	return $data;
}

/**
 * Cria um campo texto no formulário
 *
 * @param string $var
 * @param string $obrig
 * @param string $habil
 * @param string $label
 * @param integer $size
 * @param integer $max
 * @param string $masc
 * @param string $hid
 * @param string $align
 * @param string $txtdica
 * @param integer $acao
 * @param string $complemento
 * @param string $evtkeyup
 * @param string $value
 * @param string $evtblur
 * @param array  $arrStyle: Variavel onde podem ser passados parametros de estilo, sendo o índice a propriedade. ex: array('width' =>'100%')
 * @return mixed
 */
function campo_texto($var,$obrig=false,$habil=true,$label='',$size='',$max='',$masc='',$hid='', $align='left', $txtdica='',$acao=0, $complemento='',$evtkeyup='', $value = null, $evtblur='', $arrStyle = null )
{
	global ${$var};

	if (is_array($var)) {
		extract($var);
		$var = $name;
	}

	$value = $value != '' ? $value : ${$var};

	if ( $obrig == 'S' ) $class = 'obrigatorio';

	$arrStyle['text-align'] = $align;

	if(is_array($arrStyle)){
		$sty = 'style="';
		foreach($arrStyle as $chaves=>$dados){
			$sty .= $chaves.':'.$dados.';';
		}
		$sty .= '"';
	}

	if ($hid=='S') $dif = '1';
	$texto = '<input type="text" '.$sty.' name="'.$var.$dif.'" size="'.($size+1).'" maxlength="'.$max. '" value="'.$value.'"';
	if ($masc !="")
	{
		if($evtkeyup != "")
		{
			$texto = $texto. ' onKeyUp= "this.value=mascaraglobal(\'' . $masc . '\',this.value);' . $evtkeyup . '"';
		}
		else
		{
			$texto = $texto. ' onKeyUp= "this.value=mascaraglobal(\'' . $masc . '\',this.value);"';
		}
	}
	else
	{
		if($evtkeyup != "")
		$texto = $texto. ' onKeyUp= "' . $evtkeyup . '"';
	}
	$habil != 'N' ? $class.=" normal"  :   $class="disabled";

	if ( $habil == 'N' )
	{
		if ( !$complemento )
		$texto = $texto.' readonly="readonly" style="width:'.($size+3).'ex;text-align : '.$align.';" ';
		else
		$texto = $texto.' ' . $complemento .' readonly="readonly" style="width:'.($size+3).'ex;text-align : '.$align.';" ';
		//if ($habil == 'N') $texto = $texto.' readonly ';
	}
	else
	{
		$texto .= ' onmouseover="MouseOver(this);';
		if ( $txtdica ){
			$texto .= 'return escape(\''.$txtdica.'\');';
		}
		if ( !$complemento ){
			if( $arrStyle ){
				$style = 'style="text-align : '.$align.'; width:'.($size+3).'ex;"';
			} else {
				$style = $sty;
			}
			$texto .= '" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);'.($masc != '' ? 'this.value=mascaraglobal(\'' . $masc . '\',this.value);' : '') . $evtblur . '" '.$style.' ';
		} else {
			$texto .= '" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);'.($masc != '' ? 'this.value=mascaraglobal(\'' . $masc . '\',this.value);' : '') . $evtblur . '" ' . $complemento . ' '.$style.' ';
		}
	}


	$texto = $texto . " title='$label' class='$class' />";

	if ($obrig == 'S')
	$texto = $texto . obrigatorio();
	if ($hid == 'S'){
		$texto = $texto."<input type='hidden' name='".$var."' value ='".$value."'>";
	}
	if ($acao)
	{
		//  	unidade,unicod,uni,combo,ppaacao,Unidade Orçamentária Responsável, and substr(unicod,1,2) in '26'
		$partes = explode(";", $acao);
		$alvo=$partes[0];
		$campo=$partes[1];
		$padrao=$partes[2];
		$tipo=$partes[3];
		$origem=$partes[4];
		$nome_campo=$partes[5];
		$especial=$partes[6];
		$especial2=$partes[7];

		$texto .= '&nbsp;<img border="0" src="../imagens/alterar.gif" title="Editar o campo." onclick='.'"edita_campo('."'$alvo','$campo','$padrao','$tipo','$origem','$nome_campo'";
		//if ($especial)
		$texto .= ",'$especial'";
		//if ($especial2)
		$texto .= ",'$especial2'";
		$texto .= ')">';


	}
	return $texto;
}
function campo_bool($var,$obrig,$habil,$label,$size,$max,$masc,$hid, $align='left', $txtdica='',$acao=0)
{
	global ${$var};
	if (${$var}=='t') ${$var}=$txtdica.'-> Sim'; else ${$var}=$txtdica.'-> Não';
	if ($hid=='S') $dif = '1';
	$texto = '<input type="text" name="'.$var.$dif.
	'" size="'.($size+1).'" maxlength="'.$max. '" value="'.${$var}.'" class="normal"';
	if ($masc <>"")
	$texto = $texto. ' onKeyUp= "this.value=mascaraglobal('."'".$masc."',this.value);".'"';

	if ($habil == 'N') $texto = $texto.' readonly style="BORDER:none;BORDER-LEFT:#888888 3px solid;COLOR:#404040;width:'.$size.'ex;text-align : '.$align.';" ';
	else {$texto .= ' onmouseover="MouseOver(this);';

	if($txtdica) $texto.= 'return escape(\''.$txtdica.'\');'; $texto.= '" onfocus="MouseClick(this);" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" style="text-align : '.$align.'; width:'.($size+3).'ex;"';}

	$texto = $texto . " title='$label'>";

	if ($obrig == 'S')
	$texto = $texto . obrigatorio();
	if ($hid == 'S'){
		$texto = $texto."<input type='hidden' name='".$var."' value ='".${$var}."'>";
	}
	if ($acao)
	{
		//  	unidade,unicod,uni,combo,ppaacao,Unidade Orçamentária Responsável, and substr(unicod,1,2) in '26'
		$partes = explode(";", $acao);
		$alvo=$partes[0];
		$campo=$partes[1];
		$padrao=$partes[2];
		$tipo=$partes[3];
		$origem=$partes[4];
		$nome_campo=$partes[5];
		$especial=$partes[6];
		$especial2=$partes[7];

		$texto .= '&nbsp;<img border="0" src="../imagens/alterar.gif" title="Editar o campo." onclick='.'"edita_campo('."'$alvo','$campo','$padrao','$tipo','$origem','$nome_campo'";
		//if ($especial)
		$texto .= ",'$especial'";
		//if ($especial2)
		$texto .= ",'$especial2'";
		$texto .= ')">';


	}
	return $texto;
}


/**
 * Faz um input de texto com busca de dados no servidor.
 * A consulta sql precisa ter um %s com o valor que será buscado
 *
 * @param string $nome
 * @param string $label
 * @param mixed $valor
 * @param integer $size
 * @param integer $maxlength
 * @param string $mascara
 * @param string $align
 * @param boolean $obrigatorio
 * @param boolean $habilitado
 * @param boolean $hidden
 * @param string $complemento conteúdo que vai dentro da tag input, os eventos onfocus, onkeyup e onkeydown não podem ser sobreescritos
 * @param string[] $parametros_extra envia na requisição ajax os valores dos campos com os nomes indicados
 * @param boolean $valida
 * @return string
 *
 * @author Adonias Malosso <malosso@gmail.com>
 * @version 1.0
 */
function campo_texto_ajax( $sql, $id, $nome, $label='', $valor='', $size='', $maxlength='', $mascara='', $align='left', $obrigatorio=false, $habilitado=true, $hidden=false, $label_externo = false, $complemento = '', $parametros_extra = array(), $valor_label = '', $valida = false )
{
	// é preciso passar o comando SQL a ser executado com '%s' no lugar do valor do cmapo
	if(strpos($sql, "%s") === false ) {
		return false;
	}

	$jsCssCampo = $extraCampo = '';

	$_SESSION["SQLAJAX"][$id] = $sql;

	$size += $habilitado ? 1 : 0;
	// se usar o campo hidden, altera o nome do campo normal
	$idCampo = $id;
	$nomeCampo	 = $hidden ? $nome . "1" :  $nome;
	// se campo obrigatorio, apresentar indicador
	if ( $label_externo )
	{
		$extraCampo .= '<span id="LSLabel_' . $idCampo . '">' . $valor_label . '</span>';
	}
	$extraCampo .= $obrigatorio ? "&nbsp;<img src=\"". URL_SISTEMA. "imagens/obrig.gif\" title=\"Campo $label obrigatório.\" border=\"0\">" : "";
	// colocar campo hidden se necessario
	$extraCampo .= $hidden ? "<input type=\"hidden\" name=\"" . $nome . "\" value=\"" . $valor ."\">" : "";
	// adiciona label ao campo


	// acrescenta javascript para a mascara do campo
	if ($mascara <> "")
	$jsCssCampo .= " onKeyUp=\"this.value=mascaraglobal('" . $mascara . "',this.value);\"";

	if(!$habilitado)
	$jsCssCampo .= " readonly style=\"border: none; border-left: #888888 3px solid; color: #404040; width: " . $size . "ex; text-align: ". $align . ";\"";
	else
	$jsCssCampo .= " style=\"text-align: " . $align . "; width:" . $size . "ex;\"";
	//		$jsCssCampo .= " onmouseover=\"MouseOver(this)\" onfocus=\"MouseClick(this);alert(123);liveSearchStart(this);\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" style=\"text-align: " . $align . "; width:" . $size . "ex;\"";

	$campos_extra = '';
	if ( count( $parametros_extra ) )
	{
		$campos_extra .= " '" . implode( "','", $parametros_extra ) . "' ";
	}
	$extra .= 'new Array(' . $campos_extra . ')';
	$valida_valor = $valida ? 'true' : 'false' ;
	$jsImg = <<<EOS
	var campo = document.getElementById( '$idCampo' );
	 if ( liveSearchStart( campo, $extra ) )
	 {
	 	campo.value = '';
	 	lsFazerBuscar( campo );
	 }
	 else
	 {
	 	MouseBlur( this );
	 }
EOS;
	return <<<EOS
	<div class="divcampoajax">
		<input type="text" name="$nomeCampo" id="$idCampo" size="$size" maxlength="$maxlength" value="$valor" class="normal" title="$label" $jsCssCampo onmouseover="MouseOver(this);" onmouseout="MouseOut(this);" onfocus="if ( liveSearchStart( this, $extra ) ) MouseClick( this ); else MouseBlur( this );" $complemento /><img id="lsImagem_$idCampo" src="/imagens/seta_combo.gif" width="14" height="16" style="margin:0;padding:0;" align="absmiddle" onclick="$jsImg" />
		$extraCampo
	</div>
	<div id="LSResult_$idCampo" class="LSResult" style="display: none;"></div>
	<input type="hidden" id="hidden_$nome" name="hidden_$nome" value="$valor_label"/>
	<script language="javascript">
		document.getElementById( '$idCampo' ).valida = $valida_valor;
	</script>
EOS;
}

/**
 * Monta um campo texto ajax em cascata. Caso o campo que ele é
 * dependente seja edita o valor deste é apagado. As dependencias
 * devem ser passadas na ordem de modo que somente o último é verificado.
 *
 * @param string $sql
 * @param string $nome
 * @param string $descricao
 * @param string[] $dependencias
 * @param integer $size
 * @param integer $maxlength
 * @param string $mascara
 * @param string $align
 * @param boolean $obrigatorio
 * @return string
 */
function campo_texto_ajax_cascata( $sql, $nome, $descricao, $dependencias, $size = '', $maxlength = '', $mascara = '', $align = 'left', $obrigatorio = false, $valor = '', $valor_label = '' )
{
	$ultima_dependencia = end( $dependencias );
	// adiciona valor do campo caso ele já tenha iniciado com algum valor
	$jsLabel = $valor ? " lsLabel[$nome] = '$valor_label'; " : '' ;
	$jsLabel = '';
	$javascript = <<<EOS
		<script type="text/javascript">
			var tamanho = lista_campo_cascata.length;
			lista_campo_cascata[tamanho] = new Array();
			lista_campo_cascata[tamanho]['nome'] = '$nome';
			lista_campo_cascata[tamanho]['dependencia'] = '$ultima_dependencia';
			lista_campo_cascata[tamanho]['valor'] = '${$nome}';
			$jsLabel
		</script>
EOS;
			return $javascript . campo_texto_ajax( $sql, $nome, $nome, $descricao, $valor, $size, $maxlength, $mascara, $align, $obrigatorio, true, false, true, '', $dependencias, $valor_label, true );
}


/**
 * Monta campo textarea.
 *
 * Opções implementadas em $opcoes:
 * info: Mostra um texto acima
 *
 * @param string $var nome do campo
 * @param string $obrig obrigatório ou não S|N
 * @param string $habil habilitado ou não S|N
 * @param string $label ???
 * @param string $cols quantidade de coluna
 * @param string $rows quantidade de linhas
 * @param string $max tamanho máximo de caracteres
 * @param string $funcao ???
 * @param string $acao ???
 * @param string $txtdica texto de dica, exibido quando o mouse fica sob o textarea
 * @param string $tab habilita digitação do tab ou não true|false
 * @param string $width define o tamanho do campo podendo ser px ou %
 * @param string $id define o id do elemento, usado em casos que é necessário identificar o elemento de forma "particular" diferenciada.
 * @param array $opcoes Contém opções diversas para o textarea
 * @return string
 */
function campo_textarea( $var, $obrig, $habil, $label, $cols, $rows, $max, $funcao = '', $acao = 0, $txtdica = '', $tab = false, $title = NULL, $value = null, $width=null, $id=null, $opcoes = null){
	global ${$var};

	$value = $value != "" ? $value : ${$var};

	$texto = '';

	if ( $obrig == 'S' ) $class = 'obrigatorio ';

	if($title){
		$title = 'title="'.$title.'"';
	}
	// verifica se há texto de dica e cria span onde o texto ficará visível
	// ao final da função a mesma verificação é realizada para fechar o tag
	if ( $txtdica )
	{
		$texto .= '<span onmouseover="return escape( \'' . $txtdica . '\' );">';
	}

	// inicia o campo textarea
        if( $id != ''){
            $texto .= '<textarea id="' . $id . '" name="' . $var . '" cols="' . $cols . '" rows="' . $rows . '" ' . $funcao . ' '  . $title;
        }else{
            $texto .= '<textarea id="' . $var . '" name="' . $var . '" cols="' . $cols . '" rows="' . $rows . '" ' . $funcao . ' '  . $title;
        }

	$style = ''; // contém os estilos a serem aplicados ao textarea

	// monta atributos do textarea de acordo com a habilitação ou não do campo
	if ( $habil == 'N' ){
		$texto .= ' readonly="readonly" disabled="disabled" ';

                if($width){
                    $style .= 'border:none;border-left:#888888 3px solid;color:#404040;width:'.$width.'; text-align:' . $align . ';';
                } else {
                    $style .= 'border:none;border-left:#888888 3px solid;color:#404040;width:' . $cols . 'ex;text-align:' . $align . ';';
                }
	}else{
		$txtdica = $txtdica ? 'return escape( \'' . simec_htmlentities( $txtdica ) . '\' );' : '' ;
		$texto .= ' onmouseover="MouseOver( this );' . $txtdica . '"';
		$texto .= ' onfocus="MouseClick( this );" ';
		$texto .= ' onmouseout="MouseOut( this );" ';
		if ($max){
                    if($id != ''){
                        $texto .= ' onblur="MouseBlur( this ); textCounter( document.getElementById(\'' . $id . '\'), document.getElementById(\'no_' . $id . '\'), ' . $max . ');" ';
                    }else{
			$texto .= ' onblur="MouseBlur( this ); textCounter( this.form.' . $var . ', this.form.no_' . $var . ', ' . $max . ');" ';
                    }
		}else {
			$texto .= ' onblur="MouseBlur( this );" ';
		}

                if($width){
                    $style .= 'width:' . $width . '; ';
                } else {
                    $style .= 'width:' . $cols . 'ex;';
                }
	}
	// habilita tab, quando o tab é habilitado o estilo é alterado para
	// uma classe onde a fonte é monoespeçada
	$keydown = '';
	if ( $tab == true )
	{
		//$keydown = 'return readTabChar( event, this );';
		$style .= 'font-family:monospace;font-size:8pt;';
	}
	// aplica estilos
	$texto .= ' style="' . $style . '" ';
	// verifica se é para habilitar checagem de tamanho do campo
	if ( $max && $habil == 'S' ){
            if( $id != ''){
		$texto .= ' onkeydown=" textCounter( document.getElementById(\''.$id.'\'), document.getElementById(\'no_'.$id.'\'), '.$max.');' . $keydown . '" ';
		$texto .= ' onkeyup=" textCounter( document.getElementById(\'' . $id . '\'), document.getElementById(\'no_' . $id . '\'), ' . $max . ');" ';
            }else{
                $texto .= ' onkeydown="textCounter( this.form.' . $var . ', this.form.no_' . $var . ', ' . $max . ' );' . $keydown . '" ';
		$texto .= ' onkeyup="textCounter( this.form.' . $var . ', this.form.no_' . $var . ', ' . $max . ');" ';
            }
	}
	// finaliza criação do campo textarea
	$texto .= ' class="'.$class.'txareanormal" >' . str_replace( '&nbsp;', '&amp;nbsp;', ($value != "") ? $value : ${$var} ) . '</textarea>';
	// mostra indicador de obrigatoriedade do campo

	if ( $obrig == 'S' )$texto = $texto . obrigatorio();
	// mostra tamanho máximo do conteúdo caso haja tamanho máximo
	if ( $max && $habil=='S' ){
            if( $id != ''){
                $texto .= "<br><input readonly style=\"text-align:right;border-left:#888888 3px solid;color:#808080;\" type=\"text\" name=\"no_" . $var . "\" id=\"no_" . $id . "\" size=\"6\" maxlength=\"6\" value=\"". ( $max - strlen( $value ) ) . "\" class=\"CampoEstilo\"><font color=\"red\" size=\"1\" face=\"Verdana\"> máximo de caracteres</font>";
            }else{
                $texto .= "<br><input readonly style=\"text-align:right;border-left:#888888 3px solid;color:#808080;\" type=\"text\" name=\"no_" . $var . "\" size=\"6\" maxlength=\"6\" value=\"". ( $max - strlen( $value ) ) . "\" class=\"CampoEstilo\"><font color=\"red\" size=\"1\" face=\"Verdana\"> máximo de caracteres</font>";
            }
	}

	if ( $acao ){
		$partes = explode( ';', $acao);
		$alvo       = $partes[0];
		$campo      = $partes[1];
		$padrao     = $partes[2];
		$tipo       = $partes[3];
		$origem     = $partes[4];
		$nome_campo = $partes[5];
		$especial   = $partes[6];
		$texto = $texto . '&nbsp;<img border="0" src="../imagens/alterar.gif" title="Editar o campo." onclick="edita_campo( ' . "'$alvo', '$campo', '$padrao', '$tipo', '$origem', '$nome_campo', '$especial'" . ' )">';
	}
	// fecha tag span do texto dica
	// olhar comentário no início da função que iniciar o tag
	if ( $txtdica )
	{
		$texto .= "</span>";
	}

    $info = (is_array($opcoes) && isset($opcoes['info']))?$opcoes['info']:'';
	return '<div class="notprint">' . $info .
	$texto .
		'</div>' .
		'<div id="print_' . $var . '" class="notscreen" style="text-align: left;">' .
	${$var} .
		'</div>';
}

function procura_array($search_value, $the_array)
{
	if (is_array($the_array))
	{
		foreach ($the_array as $key => $value)
		{
			$result = procura_array($search_value, $value);
			if (is_array($result))
			{
				$return = $result;
				array_unshift($return, $key);
				return $return;
			}
			elseif ($result == true)
			{
				$return[] = $key;
				return $return;
			}
		}
		return false;
	}
	else
	{
		if ($search_value == $the_array)
		{
			return true;
		}
		else return false;
	}
}

function monta_arvore_menu($cod)
{
	$sql = 'select * from menu where mnucodpai = '.$cod;
	$saida = $db->recuperar($sql);
	if(is_array($saida)) foreach($saida as $k=>$v) ${$k}=$v;
	if ($mnusnsubmenu == 't') monta_arvore_menu($mnucod);
	else print "<td><input type='checkbox' name='cb'$mnucod</td><td>$mnucod</td><td>$mnudsc</td>";
}


function mes_extenso($mes)
{
	if (strval($mes) == 1) return 'JANEIRO';
	else   if (strval($mes) == 2) return 'FEVEREIRO';
	else   if (strval($mes) == 3) return 'MARÇO';
	else   if (strval($mes) == 4) return 'ABRIL';
	else   if (strval($mes) == 5) return 'MAIO';
	else   if (strval($mes) == 6) return 'JUNHO';
	else   if (strval($mes) == 7) return 'JULHO';
	else   if (strval($mes) == 8) return 'AGOSTO';
	else   if (strval($mes) == 9) return 'SETEMBRO';
	else   if (strval($mes) == 10) return 'OUTUBRO';
	else   if (strval($mes) == 11) return 'NOVEMBRO';
	else   if (strval($mes) == 12) return 'DEZEMBRO';
}

function str_to_upper($str){
	return strtr($str,
	"abcdefghijklmnopqrstuvwxyz".
	"\x9C\x9A\xE0\xE1\xE2\xE3".
	"\xE4\xE5\xE6\xE7\xE8\xE9".
	"\xEA\xEB\xEC\xED\xEE\xEF".
	"\xF0\xF1\xF2\xF3\xF4\xF5".
	"\xF6\xF8\xF9\xFA\xFB\xFC".
	"\xFD\xFE\xFF",
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ".
	"\x8C\x8A\xC0\xC1\xC2\xC3\xC4".
	"\xC5\xC6\xC7\xC8\xC9\xCA\xCB".
	"\xCC\xCD\xCE\xCF\xD0\xD1\xD2".
	"\xD3\xD4\xD5\xD6\xD8\xD9\xDA".
	"\xDB\xDC\xDD\xDE\x9F");
}

function grid($name, &$data){

	$row_count = @pg_num_rows($data);
	$column_count = @pg_numfields($data);


	$columns = "var ".$name."_columns = [\n";
	for ($i=0; $i < $column_count; $i++) {
		$columns .= "\"".@pg_field_name($data, $i)."\", ";
	}
	$columns .= "\n];\n";


	$rows = "var ".$name."_data = [\n";
	while ($result = @pg_fetch_array($data)) {
		$rows .= "[";
		for ($i=0; $i < $column_count; $i++) {
			$rows .= "\"".grid_html($result[$i])."\", ";
		}
		$rows .= "],\n";
	}
	$rows .= "];\n";

	$html = "<"."script".">\n";
	$html .= $columns;
	$html .= $rows;
	$html .= "try {\n";
	$html .= "  var $name = new Active.Controls.Grid;\n";
	$html .= "  $name.setId(\"grid1\");";
	$html .= "  $name.setRowCount($row_count);\n";
	$html .= "  $name.setColumnCount($column_count);\n";
	$html .= "  $name.setDataText(function(i, j){return ".$name."_data[i][j]});\n";
	$html .= "  $name.setColumnText(function(i){return ".$name."_columns[i]});\n";
	$html .= "  document.write($name);\n";
	$html .= "}\n";
	$html .= "catch (error){\n";
	$html .= "  document.write(error.description);\n";
	$html .= "}\n";
	$html .= "</"."script".">\n";

	print $html;
}

function grid_html($msg){

	$msg = addslashes($msg);
	$msg = str_replace("\n", "\\n", $msg);
	$msg = str_replace("\r", "\\r", $msg);
	$msg = simec_htmlspecialchars($msg);

	return $msg;
}

function dbg($var=NULL, $morte=FALSE, $usucpf=''){
	if ( $usucpf AND $usucpf != $_SESSION['usucpforigem'] )	return;
	print "/*<center>-- ----- ----- ----- ----- ----- ----- ----- ----- -----\n ";
	print "<strong>DEBUG INICIO</strong>";
	print " ----- ----- ----- ----- ----- ----- ----- ----- ----- --</center>\n";
	$array = debug_backtrace();
	print("<center>linha '". $array[0]['line'] ."' do arquivo '". $array[0]['file'] ."'</center>\n");
	print("<pre><font size='3'>\n");
	var_dump($var);
	print("</font></pre>\n");
	print "<center>---- ----- ----- ----- ----- ----- ----- ----- ----- ----- \n";
	print "<strong>DEBUG FIM</strong>";
	print " ----- ----- ----- ----- ----- ----- ----- ----- ----- ----</center>\n";
	if( $morte )
	die("<center><font color=\"#ff0000\"><strong>D I E</strong></font></center>\n*/");
}

define( "d", "die" );

function ver(){

    // Somente será exibido se não estiver logado ou o usuário for super-usuário no módulo SIS
    $exibirVer = true;
    if($_SESSION['usucpforigem']){
        $perfis = pegaPerfilGeral($_SESSION['usucpforigem'], 4); // Módulo SIS (4)
        if(!is_array($perfis) || !in_array(25, $perfis)){ //SUPER-USER do SIS (25)
            $exibirVer = false;
        }
    }

    if($exibirVer){

		$parar = false;
                $backstrace = debug_backtrace();
		$caminho = array_shift($backstrace);
		echo '
			<div style="background-color: #ddd; color: #000; font-size: 14px;">
			<div>
				<label>
					Linha   --> <label class="valorCaminho">'.$caminho['line']	  .'</label><br />
					Caminho --> <label class="valorCaminho">'.$caminho['file']	  .'</label><br />
				</label>
			</div>';

		echo '<div><pre>';


		foreach( $caminho['args'] as $indice => $valor ){

			if(  $valor == 'die' ) $parar = true;
			echo '<label style="color:red; font-weight: bold;">Argumento '.( $indice + 1 ).'</label><br />';
			print_r( $valor );
			echo '<br /><br />';

		}

		echo '</div></pre></div>';


		// Verifica se é para dar o die no final
		if( $parar ){ die; }
	}
}

function monta_grafico_validacao($vl0,$vl1,$vl2,$vl3,$vltotal)
{
	$vl0p = number_format($vl0*100/$vltotal,2,'.',',');
	$vl1p = number_format($vl1*100/$vltotal,2,'.',',');
	$vl2p = number_format($vl2*100/$vltotal,2,'.',',');
	$vl3p = number_format($vl3*100/$vltotal,2,'.',',');
	$txtgrafico = "$vltotal Ações no Total:<br>$vl1 <img src=\'../imagens/cor1.gif\' style=\'height:7;width:7;border:1px solid #888888;\'> Concordância ($vl1p%)<br>$vl2 <img src=\'../imagens/cor2.gif\' style=\'height:7;width:7;border:1px solid #888888;\'> Pendente ($vl2p%)<br>$vl3 <img src=\'../imagens/cor3.gif\' style=\'height:7;width:7;border:1px solid #888888;\'> Discordância ($vl3p%)<br>$vl0 <img src=\'../imagens/cor0.gif\' style=\'height:7;width:7;border:1px solid #888888;\'> Não Validado ($vl0p%)";
	$imggrafico = "<span onmouseover=\"return escape('$txtgrafico')\"><img src='../imagens/cor1.gif' style='height:10;width:".($vl1p/2).";border:1px solid #888888;border-right:0'><img src='../imagens/cor2.gif' style='height:10;width:".($vl2p/2).";border:1px solid #888888;border-right:0;border-left:0;border-right:0;border-left:0'><img src='../imagens/cor3.gif' style='height:10;width:".($vl3p/2).";border:1px solid #888888;border-left:0;border-right:0'><img src='../imagens/cor0.gif' style='height:10;width:".($vl0p/2).";border:1px solid #888888;border-left:0'></span>";
	return $imggrafico;
}


function monta_grafico_avaliacao( $vl0, $vl1, $vl2, $vl3, $vltotal, $mes, $percent )
{
	$vl0p = number_format($vl0*100/$vltotal,0,'.',',');
	$vl1p = number_format($vl1*100/$vltotal,0,'.',',');
	$vl2p = number_format($vl2*100/$vltotal,0,'.',',');
	$vl3p = number_format($vl3*100/$vltotal,0,'.',',');
	/*
	 $txtgrafico = "<b>$mes</b> - Total: $vltotal Ações<br> ". ( $vl1+$vl2+$vl3 ) ." Avaliada(s) / Preenchida(s) $percent<br><table border=0><tr><td>$vl1</td><td><img src=\'../imagens/cor1.gif\' style=\'height:7;width:$vl1p;border:1px solid #888888;\'> Estável ($vl1p%)</td></tr><tr><td>$vl2</td><td><img src=\'../imagens/cor2.gif\' style=\'height:7;width:$vl2p;border:1px solid #888888;\'> Merece Atenção ($vl2p%)</td></tr><tr><td>$vl3</td><td><img src=\'../imagens/cor3.gif\' style=\'height:7;width:$vl3p;border:1px solid #888888;\'> Crítico ($vl3p%)</td></tr><tr><td>$vl0</td><td><img src=\'../imagens/cor0.gif\' style=\'height:7;width:$vl0p;border:1px solid #888888;\'> Não Avaliada ($vl0p%)</td></tr></table>";
	 $imggrafico = "<span onmouseover=\"return escape('$txtgrafico')\"><img src='../imagens/cor1.gif' style='height:10;width:".($vl1p/3).";border:1px solid #888888;border-right:0'><img src='../imagens/cor2.gif' style='height:10;width:".($vl2p/3).";border:1px solid #888888;border-right:0;border-left:0;border-right:0;border-left:0'><img src='../imagens/cor3.gif' style='height:10;width:".($vl3p/3).";border:1px solid #888888;border-left:0;border-right:0'><img src='../imagens/cor0.gif' style='height:10;width:".($vl0p/3).";border:1px solid #888888;border-left:0'></span>";
	 return $imggrafico;
	 */
	$url = '../' . $_SESSION['sisdiretorio'] . '/geral/infoMonitora.php' .
	'?vl0=' . $vl0 .
	'&vl1=' . $vl1 .
	'&vl2=' . $vl2 .
	'&vl3=' . $vl3 .
	'&vltotal=' . $vltotal .
	'&mes=' . $mes .
	'&percent=' . $percent;
	return "
		<span	onmousemove=\"SuperTitleAjax( '" . $url . "', this );\"
				onmouseout=\"SuperTitleOff( this );\">
			<img src='../imagens/cor1.gif' style='height:10;width:".($vl1p/3).";border:1px solid #888888;border-right:0'><img src='../imagens/cor2.gif' style='height:10;width:".($vl2p/3).";border:1px solid #888888;border-right:0;border-left:0;border-right:0;border-left:0'><img src='../imagens/cor3.gif' style='height:10;width:".($vl3p/3).";border:1px solid #888888;border-left:0;border-right:0'><img src='../imagens/cor0.gif' style='height:10;width:".($vl0p/3).";border:1px solid #888888;border-left:0'>
		</span>";
}

/*
 function monta_grafico_avaliacao_resultado( $vl0,$vl1,$vl2,$vl3,$vltotal,$mes,$percent )
 {
 $vl0p = number_format( $vl0 * 100 / $vltotal, 0, '.', ',' );
 $vl1p = number_format( $vl1 * 100 / $vltotal, 0, '.', ',' );
 $vl2p = number_format( $vl2 * 100 / $vltotal, 0, '.', ',' );
 $vl3p = number_format( $vl3 * 100 / $vltotal, 0, '.', ',' );
 return "<b>$mes</b> - Total: $vltotal Ações<br>$percent  Avaliada(s) / Preenchida(s)<br><table border=0><tr><td>$vl1</td><td><img src=\'../imagens/cor1.gif\' style=\'height:7;width:$vl1p;border:1px solid #888888;\'> Estável ($vl1p%)</td></tr><tr><td>$vl2</td><td><img src=\'../imagens/cor2.gif\' style=\'height:7;width:$vl2p;border:1px solid #888888;\'> Merece Atenção ($vl2p%)</td></tr><tr><td>$vl3</td><td><img src=\'../imagens/cor3.gif\' style=\'height:7;width:$vl3p;border:1px solid #888888;\'> Crítico ($vl3p%)</td></tr><tr><td>$vl0</td><td><img src=\'../imagens/cor0.gif\' style=\'height:7;width:$vl0p;border:1px solid #888888;\'> Não Avaliada ($vl0p%)</td></tr></table>";
 }

 function monta_grafico_avaliacao( $vl0, $vl1, $vl2, $vl3, $vltotal, $mes, $percent )
 {
 $vl0p = number_format( $vl0 * 100 / $vltotal, 0, '.', ',' );
 $vl1p = number_format( $vl1 * 100 / $vltotal, 0, '.', ',' );
 $vl2p = number_format( $vl2 * 100 / $vltotal, 0, '.', ',' );
 $vl3p = number_format( $vl3 * 100 / $vltotal, 0, '.', ',' );
 //$onmouseover = "monta_grafico_avaliacao( '$vl0', '$vl1', '$vl2', '$vl3', '$vltotal', '$mes', '$percent' );";
 //$onmouseover = "alert( 4 );";
 $imggrafico = "<span onmouseover=\"" . $onmouseover . "\"><img src='../imagens/cor1.gif' style='height:10;width:" . ( $vl1p / 3 ) . ";border:1px solid #888888;border-right:0'><img src='../imagens/cor2.gif' style='height:10;width:" . ( $vl2p / 3 ) . ";border:1px solid #888888;border-right:0;border-left:0;border-right:0;border-left:0'><img src='../imagens/cor3.gif' style='height:10;width:".($vl3p/3).";border:1px solid #888888;border-left:0;border-right:0'><img src='../imagens/cor0.gif' style='height:10;width:" . ( $vl0p / 3 ) . ";border:1px solid #888888;border-left:0'></span>";
 return $imggrafico;
 }
 */


/**
 * Autor: Luciano Fernandes Ribeiro.
 * Criado em: 13/05/2014.
 * Monta um combo especial com seleção em popups. No popup é selecionado apartir de 2 combobox, o primeiro filtra o segundo.
 *
 * Para exemplificação e conhecimento do componete siga as instruções e tenho como exemplo o já feito no arquivo:
 * rcs/principal/saberes/cad_inscricao_prof.inc
 * No arquivo especificado a 3 objetos feitos com esse componentes.
 *
 * @param string $nome nome atribuido ao campo no formulário * OBRIGATORIO
 * @param string $sql consulta a ser realizada no banco * OBRIGATORIO
        VARIAVEL COM A SQL QUE SERA O PRIMEIRO COMBOBOX
    *   $sql_1 = "
            SELECT  arcid as codigo,
                    arcdsc as descricao
            FROM rcs.areacurso
            ORDER BY arcdsc
        ";
        VARIAVEL COM A SQL QUE SERA O SEGUNDO COMBOBOX
        $sql_2 = "
            SELECT  sbcid as codigo,
                    sbcdsc as descricao
            FROM rcs.subareacurso
        ";
        VARIAVEL $sql É PASSADA PARA A FUNÇÃO OS PARAMETROS:
            * COMBO - 1
            - titulo_combo: TITULO QUE DO COMBO.
            - sql_combo: SQL QUE É USADA PARA GERAR O COMBOBOX. (PARA ESSE CASO ESTA SENDO USADO A VARIAVEL "$sql_???")
            - name_combo: NAME QUE SERA DADO AO COMBO.
            - id_combo: ID QUE SERA DADO AO COMBO.
            * COMBO - 2 (TEM OS MESMOS PARAMETROS DO COMBO - 1 E COMPLEMENTADO POR MAIS 2 DESCRITOS ABAIXO)
            - where: CLAUSA WHERE QUE SERA USADA PARA FILTAR O COMBOBOX DE ACORDCO COM O PARAMENTRO DO PROMEIRO. EX: WHERE arcid = %s'
            - orderby: ORDEM QUE SERA APRESENTADO O COMBOBOX. EX: ORDER BY sbcdsc;
        $sql = array(
            'combo_1'=>
                array(
                    'titulo_combo' => 'Área - Cursos',
                    'sql_combo' => $sql_1,
                    'name_combo' => 'arcid',
                    'id_combo' => 'arcid'
               ),
            'combo_2'=>
                array(
                    'titulo_combo' => 'Sub área - Cursos',
                    'sql_combo' => $sql_2,
                    'where'=> 'WHERE arcid = %s',
                    'orderby'=> 'ORDER BY sbcdsc;',
                    'name_combo' => 'sbcid',
                    'id_combo' => 'sbcid'
                )
        );
 * @param string $titulo titulo que deverá aparecer no popup, nome da janela.
 * @param string $tamanho_janela tamanho da janela popup no formato HEIGHTxWIDTH
 * @param string $habilitado indica se o campo está aberto para edição ( S | N )
 * @param string $size tamanho do objeto select
 * @param string $width largura do objeto select
 * @param string $maximo_itens maximo de itens a ser exibido no objeto select.
 * @param $resp os valores que é usado para preencher o "objeto" com as respostas.
 * @param string $funcao Função Javascript a ser chamada. É uma array e deve ser definido os eventos da funcao. Abaixo segue um exemplo.
    * Ex: Deve ser criado um array com os seguintes indices.
    * $funcao = array(
            'funcao_usar'   => 'S',
            'onclick'       => 'nomefuncao_onclick_aserchamada()',
            'ondblclick'    => 'nomefuncao_ondblclick_aserchamada()',
            'ondelete'      => 'nomefuncao_ondelete_aserchamada()'
        );
 * @return void
 */
    function select_popup( $nome, $sql, $titulo, $tamanho_janela = '280x600', $habilitado = 'S', $size = 10, $width = 400 , $maximo_itens=null, $resp = null, $funcao=false ){
	global ${$nome};

	unset($dados_sessao);

        $titulo_combo_1 = $sql['combo_1']['titulo_combo'];
        $sql_combo_1    = $sql['combo_1']['sql_combo'];
        $name_combo_1   = $sql['combo_1']['name_combo'];
        $id_combo_1     = $sql['combo_1']['id_combo'];

        $titulo_combo_2 = $sql['combo_2']['titulo_combo'];
        $sql_combo_2    = $sql['combo_2']['sql_combo'];
        $name_combo_2   = $sql['combo_2']['name_combo'];
        $id_combo_2     = $sql['combo_2']['id_combo'];
        $where_combo    = $sql['combo_2']['where'];
        $orderby        = $sql['combo_2']['orderby'];

	#PREPARA SESSÃO.
	$dados_sessao = array(
            'titulo'            => $titulo,
            'maximo'            => $maximo_itens,
            'where'             => $where,

            'titulo_combo_1'    => $titulo_combo_1,
            'sql_combo_1'       => $sql_combo_1,
            'name_combo_1'      => $name_combo_1,
            'id_combo_1'        => $id_combo_1,

            'titulo_combo_2'    => $titulo_combo_2,
            'sql_combo_2'       => $sql_combo_2,
            'name_combo_2'      => $name_combo_2,
            'id_combo_2'        => $id_combo_2,
            'where_combo'       => $where_combo,
            'orderby'           => $orderby
	);

	if ( !isset( $_SESSION['indice_sessao_select_popup'] ) ){
            $_SESSION['indice_sessao_select_popup'] = array();
	}

	unset($_SESSION['indice_sessao_select_popup'][$nome]);
	$_SESSION['indice_sessao_select_popup'][$nome] = $dados_sessao;

	#MONTAS HTML PARA O FORMULARIO.
	$tamanho    = explode( 'x', $tamanho_janela );

        if( $funcao['funcao_usar'] == 'S' ){
            #RECEBE VALORES DO ARRAY PASSADO PELA FUNÇÃO.
            $func_onclick    = $funcao['onclick'];
            $func_ondblclick = $funcao['ondblclick'];
            $func_ondelete   = $funcao['ondelete'];

            $onclick    = " onclick=\"{$func_onclick}\" ";
            $ondblclick = " ondblclick=\"$func_ondblclick\"; ";
            $ondelete   = " onkeydown=\"{$func_ondelete}\" ";
        }else{
            $onclick    = " onclick=\"combo_popup_alterar_campo_busca( this );\" ";
            $ondblclick = " ondblclick=\"select_popup_abre_janela( '{$nome}', '{$tamanho[0]}', '{$tamanho[1]}', '' )\"; ";
            $ondelete   = " onkeydown=\"combo_popup_remove_selecionados( event, '{$nome}' );\" ";
        }

	$habilitado_select = $habilitado == 'S' ? '' : ' disabled="disabled" ' ;

        $select = "<select maximo=\"{$maximo_itens}\" tipo=\"combo_popup\" multiple=\"multiple\" size=\"{$size}\" name=\"{$nome}[]\" id=\"{$nome}\" {$onclick} {$ondblclick} {$ondelete} class=\"CampoEstilo\" style=\"width:{$width}px;\" {$habilitado_select}>";

	if($resp && count( $resp ) > 0){
            $itens_criados = 0;
            foreach ( $resp as $item ){
                $select .= '<option value="' . $item['codigo'] . '">' . simec_htmlentities( $item['descricao'] ) . '</option>';
                $itens_criados++;

                if ( $maximo_itens != 0 && $itens_criados >= $maximo_itens ){
                    break;
                }
            }
	} elseif ( ${$nome} && count( ${$nome} ) > 0 ) {
            $itens_criados = 0;
            if( is_array(${$nome}) ){
                foreach ( ${$nome} as $item ){
                    $select .= '<option value="' . $item['codigo'] . '">' . simec_htmlentities( $item['descricao'] ) . '</option>';
                    $itens_criados++;
                    if ( $maximo_itens != 0 && $itens_criados >= $maximo_itens ){
                            break;
                    }
                }
            }
	} else if ( $habilitado == 'S' ) {
            $select .= '<option value="">Duplo clique para selecionar da lista</option>';
	} else {
            $select .= '<option value="">Nenhum</option>';
	}
	$select .= '</select>';
	$buscaCodigo = '';

	$cabecalho = '';
	if ( $buscaCodigo != '' || $flagContem != '' ){
            $cabecalho .= '<table width="' . $width . '" border="0" cellspacing="0" cellpadding="0"><tr>';
            $cabecalho .= '<td align="left">' . $buscaCodigo . '</td>';
            $cabecalho .= '<td align="right">' . $flagContem . '</td>';
            $cabecalho .= '</tr></table>';
	}
	echo $cabecalho . $select  . " <img src=\"../imagens/pop_p.gif\" style=\"cursor:pointer;\" align=\"absmiddle\" onclick=\"select_popup_abre_janela( '{$nome}', '{$tamanho[0]}', '{$tamanho[1]}', '');\" >";
}

/**
 * Monta um combo especial com seleção em popups.
 *
 * Monta apenas o campo do formulário. Consulte as observações indicada
 * para cada parâmetro. É necessário que o campo identificador seja
 * renomeado para 'codigo' e a descrição para 'descricao'. Veja restante
 * dos comentário do arquivo www/geral/combopopup.php
 *
 * @param string $nome nome atribuido ao campo no formulário
 * @param string $sql consulta a ser realizada no banco
 * @param string $titulo titulo que deverá aparecer no popup
 * @param string $tamanho_janela tamanho da janela popup no formato HEIGHTxWIDTH
 * @param integer $maximo_itens máximo de itens que podem ser selecionados ( se igual a 0 quantidade é ilimitada )
 * @param string[] $codigos_fixos códigos que o usuário não pode remover do combo
 * @param string $mensagem_fixo mensagem que será exibida caso o usuário tente remover um item fixo
 * @param string $habilitado indica se o campo está aberto para edição ( S | N )
 * @param boolean $campo_busca_codigo
 * @param boolean $campo_flag_contem
 * @param integer $size
 * @param integer $width
 * @param string $funcaoJS Função Javascript a ser chamada quando se escolhe uma opção da popup(onclick). É passado por parâmetro para esta função js o objeto(this) (Adicionado por: Felipe Carvalho)
 * @return void
 */
function combo_popup( $nome, $sql, $titulo, $tamanho_janela = '400x400', $maximo_itens = 0,
$codigos_fixos = array(), $mensagem_fixo = '', $habilitado = 'S', $campo_busca_codigo = false,
$campo_flag_contem = false, $size = 10, $width = 400 , $onpop = null, $onpush = null, $param_conexao = false, $where=null, $value = null, $mostraPesquisa = true, $campo_busca_descricao = false, $funcaoJS=null, $intervalo=false, $arrVisivel = null , $arrOrdem = null)
{
	global ${$nome};
	unset($dados_sessao);
	// prepara parametros
	$maximo_itens = abs( (integer) $maximo_itens );
	$codigos_fixos = $codigos_fixos ? $codigos_fixos : array();
	// prepara sessão
	$dados_sessao = array(
	'sql' => (string) $sql, // o sql é armazenado para ser executado posteriormente pela janela popup
	'titulo' => $titulo,
	'indice' => $indice_visivel,
	'maximo' => $maximo_itens,
	'codigos_fixos' => $codigos_fixos,
	'mensagem_fixo' => $mensagem_fixo,
	'param_conexao' => $param_conexao,
	'where'			=> $where,
	'mostraPesquisa'=> $mostraPesquisa,
	'intervalo'     => $intervalo,
	'arrVisivel'    => $arrVisivel,
	'arrOrdem'     => $arrOrdem
	);

	if ( !isset( $_SESSION['indice_sessao_combo_popup'] ) )
	{
		$_SESSION['indice_sessao_combo_popup'] = array();
	}
	unset($_SESSION['indice_sessao_combo_popup'][$nome]);
	$_SESSION['indice_sessao_combo_popup'][$nome] = $dados_sessao;

	// monta html para formulario
	$tamanho    = explode( 'x', $tamanho_janela );
	$onclick    = ' onclick="javascript:combo_popup_alterar_campo_busca( this );" ';

	/*** Adiciona a função Javascript ***/
	$funcaoJS = (is_null($funcaoJS)) ? 'false' : "'" . $funcaoJS . "'";

	$ondblclick = ' ondblclick="javascript:combo_popup_abre_janela( \'' . $nome . '\', ' . $tamanho[0] . ', ' . $tamanho[1] . ', '.$funcaoJS.' );" ';
	$ondelete   = ' onkeydown="javascript:combo_popup_remove_selecionados( event, \'' . $nome . '\' );" ';
	$onpop		= ( $onpop == null ) ? $onpop = '' : ' onpop="' . $onpop . '"';
	$onpush		= ( $onpush == null ) ? $onpush = '' : ' onpush="' . $onpush . '"';
	$habilitado_select = $habilitado == 'S' ? '' : ' disabled="disabled" ' ;
	$select =
	'<select ' .
	'maximo="'. $maximo_itens .'" tipo="combo_popup" title="'. $titulo .'" ' .
	'multiple="multiple" size="' . $size . '" ' .
	'name="' . $nome . '[]" id="' . $nome . '" '.
	$onclick . $ondblclick . $ondelete . $onpop . $onpush  .
	'class="CampoEstilo" style="width:' . $width . 'px;" ' .
	$habilitado_select .
	'>';

	if($value && count( $value ) > 0){
		$itens_criados = 0;
		foreach ( $value as $item )
		{
			if (is_array($item))
			{
				$select .= '<option value="' . $item['codigo'] . '">' . simec_htmlentities( $item['descricao'] ) . '</option>';
				$itens_criados++;
				if ( $maximo_itens != 0 && $itens_criados >= $maximo_itens )
				{
					break;
				}
			}
		}
	} elseif ( ${$nome} && count( ${$nome} ) > 0 ) {
		$itens_criados = 0;
		if( is_array(${$nome}) ){
			foreach ( ${$nome} as $item )
			{
				if (is_array($item))
				{
					$select .= '<option value="' . $item['codigo'] . '">' . simec_htmlentities( $item['descricao'] ) . '</option>';
					$itens_criados++;
					if ( $maximo_itens != 0 && $itens_criados >= $maximo_itens )
					{
						break;
					}
				}
			}
		}
	}
	else if ( $habilitado == 'S' )
	{
		$select .= '<option value="">Duplo clique para selecionar da lista</option>';
	}
	else
	{
		$select .= '<option value="">Nenhum</option>';
	}
	$select .= '</select>';
	$buscaCodigo = '';

	#Alteração feita por wesley romualdo
	#caso a consulta não seja por descrição e sim por codigo, não permitir digitar string no campo de consulta.
	if($campo_busca_descricao == true ){
		$paramentro = "";
		$complOnblur = "";
	} else {
		$paramentro = "onkeyup=\"this.value=mascaraglobal('[#]',this.value);\"";
		$complOnblur = "this.value=mascaraglobal('[#]',this.value);";
	}

	if ( $campo_busca_codigo == true && $habilitado == 'S' )
	{
		$buscaCodigo .= '<input type="text" id="combopopup_campo_busca_' . $nome . '" onkeypress="combo_popup_keypress_buscar_codigo( event, \'' . $nome . '\', this.value );" '.$paramentro.' onmouseover="MouseOver( this );" onfocus="MouseClick(this);" onmouseout="MouseOut(this);" onblur="MouseBlur(this); '.$complOnblur.'" class="normal" style="margin: 2px 0;" />';
		$buscaCodigo .= '&nbsp;<img title="adicionar" align="absmiddle" src="/imagens/check_p.gif" onclick="combo_popup_buscar_codigo( \'' . $nome . '\', document.getElementById( \'combopopup_campo_busca_' . $nome . '\' ).value );"/>';
		$buscaCodigo .= '&nbsp;<img title="remover" align="absmiddle" src="/imagens/exclui_p.gif" onclick="combo_popup_remover_item( \'' . $nome . '\', document.getElementById( \'combopopup_campo_busca_' . $nome . '\' ).value, true );"/>';
		$buscaCodigo .= '&nbsp;<img title="abrir lista" align="absmiddle" src="/imagens/pop_p.gif" onclick="combo_popup_abre_janela( \'' . $nome . '\', ' . $tamanho[0] . ', ' . $tamanho[1] . ' );"/>';
		$buscaCodigo .= '<br/>';
	}
	#Fim da alteração realizada por wesley romualdo

	$flagContem = '';
	if ( $campo_flag_contem == true )
	{
		$nomeFlagContemGlobal = $nome . '_campo_excludente';
		global ${$nomeFlagContemGlobal};
		$flagContem .= '<input type="checkbox" id="' . $nome . '_campo_excludente" name="' . $nome . '_campo_excludente" value="1" ' . ( ${$nomeFlagContemGlobal} ? 'checked="checked"' : '' ) . ' style="margin:0;" />';
		$flagContem .= '&nbsp;<label for="' . $nome . '_campo_excludente">Não contém</label>';
	}
	$cabecalho = '';
	if ( $buscaCodigo != '' || $flagContem != '' )
	{
		$cabecalho .= '<table width="' . $width . '" border="0" cellspacing="0" cellpadding="0"><tr>';
		$cabecalho .= '<td align="left">' . $buscaCodigo . '</td>';
		$cabecalho .= '<td align="right">' . $flagContem . '</td>';
		$cabecalho .= '</tr></table>';
	}
	print $cabecalho . $select  . ' <img src="../imagens/pop_p.gif" style="cursor:pointer;" align="absmiddle" onclick="javascript:combo_popup_abre_janela( \'' . $nome . '\', ' . $tamanho[0] . ', ' . $tamanho[1] . ', '.$funcaoJS.' );">';
}

/**
 * Monta o HTTML do combo_popup
 *
 * @param string $stDescricao (1) Descrição do campo
 * @param string $stNomeCampo (2) Nome do campo que será criado HTML/PHP
 * @param string $sql_combo (3) SQL para carregar as opções
 * @param string $sql_carregados (4) SQL para mostrar campos selecionados
 * @param string $stTextoSelecao (5) Texto para aparecer no popup - Hint
 * @param array $where (6) ???
 * @param string $funcaoJS (7) Função Javascript a ser chamada quando se escolhe uma opção da popup(onclick). É passado por parâmetro para esta função js o objeto(this) (Adicionado por: Felipe Carvalho)
 * @param boolean $semTR (8) Se true, não coloca TR, o que deve ser feito manualmente. Usado para casos em que se deve setar o innerHTML ou outro atributo DOM da TR. (Adicionado por: Felipe Carvalho)
 * @param string $intervalo (9) Se "S" será adicionado um campo chamado Selecionar intervalo onde, depois que dois registros forem clicados e acionado o botão de intervalo, todos os registros entre esse intervalo serão selecionados. ( Adicionado por: Victor Benzi )
 * @param array $arrVisivel (10) ???
 * @param array $arrOrdem (11) Indica a ordenação dos items exibidos na popup
 * @param bool $obrig (12) Exibe imagem de obrigatoriedade
 * @param bool $campoContem (13) ???
 *
 * @author Cristiano Teles
 * @since 09/02/2009
 */
function mostrarComboPopup( $stDescricao, $stNomeCampo, $sql_combo, $sql_carregados, $stTextoSelecao, Array $where=null, $funcaoJS=null, $semTR=false, $intervalo=false , $arrVisivel = null , $arrOrdem = null, $obrig = false, $campoContem = true ){
	global $db, $$stNomeCampo;
	if ( $_REQUEST[$stNomeCampo] && $_REQUEST[$stNomeCampo][0] && !empty( $sql_carregados ) ) {
		if(!is_array($_REQUEST[$stNomeCampo])){
			$_REQUEST[$stNomeCampo][0] = $_REQUEST[$stNomeCampo];
		}
		if(is_array($_REQUEST[$stNomeCampo])){
			$sql_carregados = sprintf( $sql_carregados, "'" . implode( "','", $_REQUEST[$stNomeCampo] ) . "'" );
			$$stNomeCampo = $db->carregar( sprintf( $sql_combo, $sql_carregados ) );
		}
	}
	if( !empty($sql_carregados) ){
		$$stNomeCampo = $db->carregar($sql_carregados);
	}

	if(!$semTR)
	{
		echo '<tr id="tr_'.$stNomeCampo.'">';
	}

	echo '<td width="195" class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( \'' . $stNomeCampo . '\' );">
			' . $stDescricao . '
			<input type="hidden" id="' . $stNomeCampo . '_campo_flag" name="' . $stNomeCampo . '_campo_flag" value="' . ( empty( $$stNomeCampo ) ? '0' : '1' ) . '"/>
		</td>
		<td>';

	echo '<div id="' . $stNomeCampo . '_campo_off" style="color:#a0a0a0;';
	echo !empty( $$stNomeCampo ) ? 'display:none;' : '';
// 	echo '" onclick="javascript:onOffCampo( \'' . $stNomeCampo . '\' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>';
	echo '" onclick="javascript:onOffCampo( \'' . $stNomeCampo . '\' );">
				<select style="width:400px;" class="CampoEstilo obrigatorio" >
					<option selected="selected" value="">Todos</option>
				</select>
			</div>';
	echo '<div id="' . $stNomeCampo . '_campo_on" ';
	echo empty( $$stNomeCampo ) ? 'style="display:none;"' : '';
	echo '>';
	combo_popup( $stNomeCampo, sprintf( $sql_combo, '' ), $stTextoSelecao, '400x400', 0, array(), '', 'S', true, $campoContem, 10, 400, null, null, '', $where, null, true, false, $funcaoJS, $intervalo , $arrVisivel, $arrOrdem);

	if($obrig){
            echo '<img border="0" title="Indica campo obrigatório." src="'. URL_SISTEMA. 'imagens/obrig.gif">';
	}

	echo '</div>
            </td>';

	if(!$semTR) echo '</tr>';
}


/**
 * Monta um combo especial com seleção em popups.
 *
 * Monta apenas o campo do formulário. Consulte as observações indicada
 * para cada parâmetro. É necessário que o campo identificador seja
 * renomeado para 'codigo' e a descrição para 'descricao'. Veja restante
 * dos comentário do arquivo www/geral/combopopup.php
 *
 * @param string  $nome nome atribuido ao campo no formulário
 * @param string  $sql consulta a ser realizada no banco, sendo os álias (codigo,
 * 																		  descricao,
 * 																		  value)
 * 				  OBS.: O "value" é opcional, caso não seja passado o return será o "codigo".
 * 						o return fica setado em um campo "hidden", portanto o campo visto denomina-se por: "nome do campo"_dsc.
 *
 * @param string  $titulo titulo que deverá aparecer no popup
 * @param string  $tamanho_janela tamanho da janela popup no formato HEIGHTxWIDTH
 * @param integer $width
 * @param array   $where ex: array( array("codigo" 	  => nomeCampoBanco,
 * 								 		  "descricao" => labelCampoFormulario,
 * 										  "tipo" 	  => 1 = inteiro ou 0 = string),
 * 						   		    array("codigo" 	  => nomeCampoBanco2,
 * 								 		  "descricao" => labelCampoFormulario2
 * 										  "tipo" 	  => 1 = inteiro ou  0 = string),
 * 						 )
 *
 * @param array   $complemento
 * 				  Como ativar o autocomplete(exemplo: Solicitação de Diárias => evento/modulos/principal/solicitacaoDiarias.inc):
 * 				  - $complemento['class'] = 'campo_popup_autocomplete' e $complemento['whereAuto'] com o campo a ser filtrado
 * 				  Ter incluido o modulo do jQuery + Autocomplete
 * 				  - <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
 * 				  - <script type='text/javascript' src='../includes/jquery-autocomplete/jquery.autocomplete.js'></script>
 * 				  - <link rel="stylesheet" type="text/css" href="../includes/jquery-autocomplete/jquery.autocomplete.css" />
 * 				  Colocar os seguintes script jQuery:
 * 				  - jQuery('.campo_popup_autocomplete').autocomplete("/geral/campopopup.php?nome=origem&autocomplete=1", {
 *					  	cacheLength:50,
 *						width: 440,
 *						scrollHeight: 220,
 *						delay: 1000,
 *						selectFirst: true,
 *						autoFill: false
 *					});
 *
 *					jQuery('.campo_popup_autocomplete').result(function(event, data, formatted) {
 *					     if (data) {
 *
 *					     	// Extract the data values
 *					     	var campoHidden = jQuery(this).attr('id').replace('_dsc','');
 *
 *							var descricao = data[0];
 *							var id = data[1];
 *
 *							if( id == '' ){
 *						    	jQuery(this).val('');
 *						    	return false;
 *							}
 *
 *					     	jQuery('#'+campoHidden).val( id );
 *						  	jQuery('#'+campoHidden+'_retorno').val( descricao );
 *						}
 *					});
 *
 *					jQuery('.campo_popup_autocomplete').blur(function (){
 *						jQuery(this).val(jQuery(this).val());
 *					  	var campoHidden = jQuery(this).attr('id').replace('_dsc','');
 *					  	if( jQuery(this).val() != '' && jQuery('#'+campoHidden+'_retorno').val() != '' ){
 *							if( jQuery(this).val() != jQuery('#'+campoHidden+'_retorno').val() ){
 *								jQuery(this).val('Selecione...')
 *								jQuery('#'+campoHidden).val('')
 *								jQuery('#'+campoHidden+'_retorno').val('')
 *							}
 *						}else{
 *							jQuery(this).val('Selecione...')
 *						}
 *					});
 *
 * @return void
 */
function campo_popup( $nome, $sql, $titulo, $func=null, $tamanho_janela = '400x400', $width = 30, $where=null, $typeCampo=1, $limpar = true,$disabled = false, $return = false, $obrigatorio = "N", $complemento = Array() )
{
	global $db,${$nome};

	unset($dados_sessao);

	// prepara parametros
	$maximo_itens  = abs( (integer) $maximo_itens );
	$codigos_fixos = $codigos_fixos ? $codigos_fixos : array();
	$where = $where ? $where : "";

	$ClassObrigatorio = $obrigatorio == "S" ? " obrigatorio" : "";

	// prepara sessão
	$dados_sessao = array(
							'sql'    => (string) $sql, // o sql é armazenado para ser executado posteriormente pela janela popup
							'titulo' => $titulo,
							'indice' => $indice_visivel,
							'func'	 => $func,
//							'maximo' => $maximo_itens,
//							'codigos_fixos' => $codigos_fixos,
//							'mensagem_fixo' => $mensagem_fixo,
							'where'			=> $where,
							'whereAuto'     => $complemento['whereAuto']
						  );
	if ( !isset( $_SESSION['indice_sessao_campo_popup'] ) )
	{
		$_SESSION['indice_sessao_campo_popup'] = array();
	}
	$_SESSION['indice_sessao_campo_popup'][$nome] = $dados_sessao;
	// monta html para formulario
	$tamanho    = explode( 'x', $tamanho_janela );
	$onclick    = $disabled ? ' ' : 'onclick="javascript:campo_popup_abre_janela( \'' . $nome . '\', ' . $tamanho[0] . ', ' . $tamanho[1] . ' );" ';
//	$ondblclick = ' ondblclick="javascript:campo_popup_abre_janela( \'' . $nome . '\', ' . $tamanho[0] . ', ' . $tamanho[1] . ' );" ';
    $onchange   = ' onchange="javascript:'.$func.'(this.value)" ';
	$habilitado = 'S' ;
	$buscaCodigo = '';
	$desabilitado = $disabled ? ' disabled="disabled" ' : '';
	$class = $complemento['class'];
	$readonly   = $complemento['class'] == 'campo_popup_autocomplete' ? '' : 'readonly="readonly"';
	$ondblclick = ($complemento['class'] == 'campo_popup_autocomplete') && ($onclick != '')  ? 'onclick="if(this.value==\'Selecione...\'){this.value=\'\'}"' : $onclick;
	if ($typeCampo == 1){
		$campo =
				'<input ' .
				'name="' . $nome.'_dsc' . '" ' .
				'id="' . $nome.'_dsc' . '" ' .
				'type = "text" ' .
				'value = "' . ( ${$nome}['descricao'] ? ${$nome}['descricao'] : 'Selecione...') . '" ' .
				$ondblclick.
				'class="CampoEstilo'.$ClassObrigatorio.' '.$class.'" '.
				'size="' . $width . '" ' .
				$readonly .
				$desabilitado .
				'/>';
		if( $complemento['class'] == 'campo_popup_autocomplete' ){
			$campo .=
					'<input '.
					'name="' . $nome .'_retorno' . '" ' .
					'id="' . $nome .'_retorno' . '" ' .
					'type = "hidden"' .
					'value = "' . ${$nome}['value'] . '" ' .
					'class="CampoEstilo" '.
					'size="' . $width . '" ' .
					$habilitado_select .
					'/>';
		}

		$campo .=
				'<input '.
				'name="' . $nome . '" ' .
				'id="' . $nome . '" ' .
				'type = "hidden"' .
				'value = "' . ${$nome}['value'] . '" ' .
				'class="CampoEstilo" '.
				'size="' . $width . '" ' .
				$habilitado_select .
				'/>';
		if(!$disabled)
			$buscaCodigo .= '<img title="abrir lista" id="' . $nome . '_img" align="absmiddle" src="/imagens/seta_combo.gif" onclick="campo_popup_abre_janela( \'' . $nome . '\', ' . $tamanho[0] . ', ' . $tamanho[1] . ' );"/>'.($obrigatorio == "S" ? '&nbsp;'.obrigatorio() : "");
		if  ($limpar && !$disabled){
			$buscaCodigo .= '&nbsp;<img title="remover" align="absmiddle" src="/imagens/exclui_p.gif" onclick="campo_popup_remover_item( \'' . $nome . '\');"/>'.($obrigatorio == "S" ? '&nbsp;'.obrigatorio() : "");
		}
	}else{
		$campo ='<select ' .
				'maximo="'. $maximo_itens .'" tipo="combo_popup" ' .
				'size="1" ' .
				'name="' . $nome . '" id="' . $nome . '" '.
				$onchange .
				'class="CampoEstilo'.$ClassObrigatorio.'" style="width:' . $width . 'px;" ' .
				$habilitado_select .
				'>';
		if ($sql){
			$dados = $db->carregar($sql);
			$itens_criados = 0;

			$campo .= '<option value="" >Selecione...</option>';

			foreach ( $dados as $item ){
				$select = ${$nome} ? ${$nome} : '';
				$campo .= '<option value="' . ($item['value'] ? $item['value'] : $item['codigo']) . '">' . simec_htmlentities( $item['descricao'] ) . '</option>';
				$itens_criados++;
			}
		}else{
			$campo .= '<option value="">Nenhum</option>';
		}
		$campo .= '</select>';
		$buscaCodigo .= '&nbsp;<img title="abrir lista" align="absmiddle" src="/imagens/pop_p.gif" onclick="campo_popup_abre_janela( \'' . $nome . '\', ' . $tamanho[0] . ', ' . $tamanho[1] . ' );"/> '.($obrigatorio == "S" ? '&nbsp;'.obrigatorio() : "");

	}

//	$cabecalho .= '<table width="100%" border="0" cellspacing="0" cellpadding="0"><tr>';
//	$cabecalho .= '<td align="left">' . $campo . $buscaCodigo . '</td>';
//	$cabecalho .= '<td align="right">' . $flagContem . '</td>';
//	$cabecalho .= '</tr></table>';
	$cabecalho = "<span style=\"white-space: no-wrap\">".$campo.$buscaCodigo.$flagContem."</span>";

	if($return){
		return $cabecalho;
	}else{
		print $cabecalho;
	}
}

/**
 * Mona um campo texto que pode ser preenchido com valores listados em um popup.
 *
 * Monta apenas o campo do formulário. Consulte as observações indicada
 * para cada parâmetro. É necessário que o campo identificador seja
 * renomeado para 'codigo' e a dhttp://www.maujor.com/tutorial/haslayout.phpescrição para 'descricao'. Veja restante
 * dos comentário do arquivo www/geral/textopopup.php
 *
 * @param string $nome nome atribuido ao campo no formulário
 * @param string $sql consulta a ser realizada no banco
 * @param string $titulo titulo que deverá aparecer no popup
 * @param string $maxlength quantidade máxima de caracteres
 * @param string $tamanho_campo tamanho do campo texto
 * @param string $mascara máscara do campo texto
 * @param string $tamanho_janela tamanho da janela popup no formato HEIGHTxWIDTH
 * @param string $complemento complemento para o input
 * @return void
 */
function texto_popup( $nome, $sql, $titulo, $maxlength = '', $tamanho_campo = 20, $mascara = '', $tamanho_janela = '400x400', $complemento = '' , $onblur = '')
{
	global ${$nome};
	// prepara sessão
	$dados_sessao = array(
	'sql' => (string) $sql, // o sql é armazenado para ser executado posteriormente pela janela popup
	'titulo' => $titulo,
	);
	$_SESSION['indice_sessao_texto_popup'][$nome] = $dados_sessao;
	if ( !$tamanho_janela )
	{
		$tamanho_janela = '400x400';
	}
	$tamanho = explode( 'x', $tamanho_janela );
	$onclick = ' onclick="javascript:texto_popup_abre_janela( \'' . $nome . '\', ' . $tamanho[0] . ', ' . $tamanho[1] . ', \'&mostraCodigo=1&label=0\' );" ';
	$mascara = $mascara == '' ? '' : ' onKeyUp= "if(event.keyCode!=9&&event.keyCode!=13)this.value=mascaraglobal( \'' . $mascara . '\',this.value);" ' ;
	//$mascara = '';
	$maxlength = $maxlength == '' ? '' : ' maxlength=' . $maxlength . ' ';
	print '<input ' . $complemento . ' ' . $mascara . $maxlength . ' onmouseover="MouseOver(this);" onfocus="MouseClick(this);" onmouseout="MouseOut(this);" onblur="MouseBlur(this);' . $onblur . '" size="' . $tamanho_campo . '" type="text" name="' . $nome . '" id="' . $nome . '" class="normal" value="' . ${$nome} . '">';
	print '<img id="img_texto_popup_' . $nome . '" src="/imagens/seta_combo.gif" width="14" height="16" style="margin:0;padding:0;" align="absmiddle" ' . $onclick . ' />';
}
function hidden_popup( $nome, $sql, $titulo, $tamanho_janela = '400x400',$explica='' )
{
	global ${$nome};
	// prepara sessão
	$dados_sessao = array(
	'sql' => (string) $sql, // o sql é armazenado para ser executado posteriormente pela janela popup
	'titulo' => $titulo,
	);
	$_SESSION['indice_sessao_texto_popup'][$nome] = $dados_sessao;
	$tamanho = explode( 'x', $tamanho_janela );
	$onclick = ' onclick="javascript:texto_popup_abre_janela( \'' . $nome . '\', ' . $tamanho[0] . ', ' . $tamanho[1] . ', \'&mostraCodigo=0&label=1\' );" ';
	print '<input type="hidden" name="' . $nome . '" id="' . $nome . '" class="normal" value="' . ${$nome} . '">';
	print '<span id="label_popup_'.$nome.'"></span><img src="/imagens/preview.gif"  style="margin:0 0 0 5px;padding:0;cursor:pointer;" align="absmiddle" ' . $onclick . ' />'.$explica;
}

/**
 * Cria os botões de listagem e inclusão de arquivos.
 *
 * @param string $nome Nome para controle dos dados da sessao
 * @param string $sqlListagem Se o valor for diferente de '', insere o botão para listar os arquivos de acordo com a query passada
 * @param string $campo_id_vinculo Nome do campo que será usado para a tabela de assiciação documento_processo (pjeid, ptoid, acaid, etc.)
 * @param integer $inserir Variável para verificação de inclusão ou não do botão de inserção de novos arquivos
 * @param integar $width largura da janela a ser aberta
 * @param integer $height altura da janela a ser aberta
 */
function popup_arquivo( $nome, $sqlListagem = '', $campo_id_vinculo, $valor_id_vinculo, $inserir = 1, $width, $height,$tabela_vinculo='' )
{

	$dados_sessao = array(
	'nome' => $nome,
	'campo_id_vinculo' => $campo_id_vinculo,
	'valor_id_vinculo' => $valor_id_vinculo,
	'tabela_vinculo' => $tabela_vinculo
	);
	if( $sqlListagem )
	{
		print '<input type="button" value="Arquivos associados" onclick="popup_arquivo( \'L\', \'' . $nome .'\', '. $width .', ' . $height . ' )" />&nbsp;&nbsp;';
		$dados_sessao[ 'sql' ] = $sqlListagem;
	}

	if( $inserir )
	{
		print '<input type="button" value="Inserir arquivo" onclick="popup_arquivo( \'I\', \'' . $nome .'\', '. $width .', ' . $height . ' )" />';
	}

	$_SESSION[ 'indice_sessao_popup_arquivo' ][ $nome ] = $dados_sessao;
}

/**
 * Formata um numero real com a mascara ###.###.###,##
 * O segundo parâmetro indica a quantidade de casas decimais
 *
 * @param float $valor
 * @param integer $casas_decimais
 * @return string
 */
function formata_valor( $valor, $casas_decimais = 2, $adicionar_zeros_direita = true )
{
	$valor = sprintf( '%.' . $casas_decimais . 'f', (float) $valor );
	$positivo = $valor > 0;
	$valores = explode( '.', $valor );
	$parte_inteira = @$valores[0];
	$parte_decimal = @$valores[1];
	while ( $parte_inteira{0} == '0' )
	{
		$parte_inteira = substr( $parte_inteira, 1 );
	}
	if ( strlen( $parte_inteira ) == 0 )
	{
		$parte_inteira = '0';
	}
	$sobra = '';
	if ( strlen( $parte_inteira ) % 3 == 2 )
	{
		$sobra .= $parte_inteira{0};
		$parte_inteira = substr( $parte_inteira, 1 );
	}
	if ( strlen( $parte_inteira ) % 3 == 1 )
	{
		$sobra .= $parte_inteira{0};
		$parte_inteira = substr( $parte_inteira, 1 );
	}
	if ( strlen( $sobra ) && $positivo && strlen( $parte_inteira ) )
	{
		$sobra .= '.';
	}
	$parte_inteira = implode( '.', str_split( $parte_inteira, 3 ) );
	if ( $adicionar_zeros_direita == false )
	{
		while ( $parte_decimal[strlen($parte_decimal)-1] == '0' )
		{
			$parte_decimal = substr( $parte_decimal, 0, -1 );
		}
	}
	if ( strlen( $parte_decimal ) > 0 )
	{
		$parte_inteira .= ',';
	}
	$retorna = $sobra . $parte_inteira . $parte_decimal;
	return $retorna;
}

/**
 * Formata um string com a mascara ###.###.###,## para um
 * ponto flutuante.
 *
 * @param float $valor
 * @param integer $casas_decimais
 * @return string
 */
function desformata_valor( $valor )
{
	$valor = str_replace( '.', '', $valor );
	$valor = str_replace( ',', '.', $valor );
	return (float) $valor;
}

function eof()
{
	?>
<html>
<head>
<title>Registro não encontrado</title>
</head>
<body>
<script language="Javascript">
				alert('Erro: Registro não encontrado!');
				history.back();
				</script>
</body>
</html>
	<?php
	exit();
}
function mostra_num($num,$dec=0,$simb='')
{
	if ($num==0) return '-' ; else
	return $simb.' '.number_format($num,$dec,',','.');
}

/**
 * Monta campo radio com as opções determinadas pelo segundo parâmetros.
 * O formato do segundo parÂmetro deve ser
 * $opcoes = array(
 * 		"label1" => array(
 * 			"valor" => "valor1",
 * 			"id" => "id1"
 * 			"callbak" => "nome_da_funcao"
 * );
 * Caso queira que algum valor seja por default, selecionado
 *
 * @param string $nome
 * @param array $opcoes
 * @param string $alinhamento vertical ou horizontal ( 'v'|'h' )
 * @param boolean $return
 */
function campo_radio( $nome, $opcoes, $alinhamento, $return = false )
{
	global ${$nome};
	$valor_selecionado = ${$nome};
	if ( !is_array( $opcoes ) || !count( $opcoes ) )
	{
		return;
	}

    $container  = '';
	$container .= '<ul style="margin: 0; padding: 0;">';
	$alinhamento = strtolower( $alinhamento ) == 'h' ? ' float:left; ' : '' ;
	$nome = simec_htmlentities( $nome );
	foreach ( $opcoes as $label => $dados )
	{
		$valor = $dados['valor'];
		$id = $dados['id'] ? $dados['id'] : 'radio_opcao_' . $nome . '_' . $valor ;
		$checked = $valor_selecionado == $valor  ? ' checked="checked" ' : $dados[ 'default' ] ? ' checked="checked" ' : '' ;
		$valor = simec_htmlentities( $valor );
		$onfocus = '';
		if ( $dados['callback'] )
		{
			$onfocus = ' onfocus="this.checked = true; ' . $dados['callback'] . '( this );" ';
		}
		$container .= '<li style="margin:0;width:80px; list-style-type: none;' . $alinhamento . '">';
		$container .= '<input type="radio" name="' . $nome . '" value="' . $valor . '" id="' . $id . '" ' . $checked . ' ' . $onfocus . '/>';
		$container .= '<label for="' . $id . '">' . $label . '</label>';
		$container .= '</li>';
	}
	$container .= '</ul>';

    if ($return)
       return $container;

    echo $container;
}

/**
 * Monta campo radio com opções booleana. Os valores dos campos serão '0' ou '1'.
 *
 * @param string $nome
 * @param string $alinhamento vertical ou horizontal ( 'v'|'h' )
 */
function campo_radio_sim_nao( $nome, $alinhamento = 'v' )
{
	global ${$nome};
	$valor_antigo = ${$nome};
	${$nome} = ${$nome} ? '1' : '0' ; // altera de booleano para string '0' ou '1'
	$opcoes = array(
	'Sim' =>
	array(
	'valor' => '1',
	'id' => null
	),
	'Não' =>
	array(
	'valor' => '0',
	'id' => null
	)
	);
	campo_radio( $nome, $opcoes, $alinhamento );
	${$nome} = $valor_antigo; // volta ao valor original
}
function tradutor_att($valor)
{
	// esta função traduz um nome de um atributo para o seu nome fantasia
	if (trim($valor)=='acafinalidade') return 'Finalidade';
	if (trim($valor)=='funcod') return 'Função';
	if (trim($valor)=='unicod') return 'Unidade';
	if (trim($valor)=='sfucod') return 'Subfunção';
	if (trim($valor)=='acadescricao') return 'Descrição';
	if (trim($valor)=='procod') return 'Produto';
	if (trim($valor)=='acadscproduto') return 'Descrição do Produto';
	if (trim($valor)=='unmcod') return 'Unidade de Medida';
	if (trim($valor)=='taccod') return 'Tipo de Ação';
	if (trim($valor)=='esfcod') return 'Esfera';
	if (trim($valor)=='acadetalhamento') return 'Detalhamento';
	if (trim($valor)=='acabaselegal') return 'Base legal';
	// programas
	if (trim($valor)=='tprcod') return 'Tipo do Programa';
	if (trim($valor)=='mobcod') return 'Macro objetivo';
	if (trim($valor)=='prgdscobjetivo') return 'Descrição do objetivo';
	if (trim($valor)=='prgdscpublicoalvo') return 'Público alvo';
	if (trim($valor)=='prgdscjustificativa') return 'Justificativa';
	if (trim($valor)=='prgdscestrategia') return 'Estratégia de implementação';
	if (trim($valor)=='prgdscproblema') return 'Problema';
	// indicador
	if (trim($valor)=='percod') return 'Periodicidade';
	if (trim($valor)=='bsgcod') return 'Base geográfica';
	if (trim($valor)=='inddscfonte') return 'Fonte';
	if (trim($valor)=='inddscformula') return 'Fórmula';
	if (trim($valor)=='indvlrapurado') return 'Valor apurado';
	if (trim($valor)=='inddataapuracao') return 'Data de apuração';
	if (trim($valor)=='indvlrapurado') return 'Valor apurado';

	if (trim($valor)=='indvlrfinalppa') return 'Valor ao final do PPA';
	if (trim($valor)=='indvlrfinalprg') return 'Valor final do programa';
	if (trim($valor)=='indvlrreferencia') return 'Valor de referência';
	if (trim($valor)=='indvlrapurado') return 'Valor apurado';

}
function busca_filho($id)
{
	global $db;
	$sql = "select ptoid from monitora.planotrabalho where ptostatus='A'  and ptoid_pai=$id limit 1";
	//dbg($sql,1);
	$filho=$db->pegaUm($sql);
	if ($filho) return true;
	else return false;
}

function monta_link( $modulo, $acao, $parametros = '' )
{
	return '/' . $_SESSION['sisdiretorio'] . '/' . $_SESSION['sisdiretorio'] . '.php?modulo=' . $modulo . '&acao=' . urlencode( $acao ) . '&' . $parametros;
}

function mostracod($id)
{
	global $db;
	// monta um código com pontos
	$sql = "select ptoid_pai, ptoordemprov from monitora.planotrabalho where ptoid=$id";
	$cod= $db->pegalinha($sql);
	if ($cod['ptoid_pai'])
	{
		// tem pai
		return mostracod($cod['ptoid_pai']).'.'.$cod['ptoordemprov'];
	}
	else
	return $cod['ptoordemprov'];

}

function nomeUnidade($cod){
	global  $db;
	$sql = "select unidsc from public.unidade where unicod = '$cod' and unitpocod = 'U'";
	return $db->pegaUm($sql);

}

function verficaPerfil($cpf,$sisid){
	global $db;
	$sql = "select  trim(p.pflcod) as cod from seguranca.perfilusuario pu inner join seguranca.perfil p on pu.pflcod = p.pflcod where pu.usucpf = '$cpf' and p.pflstatus = 'A' and p.sisid = $sisid;";
	return $db->carregarColuna($sql);
}

function verficaSuperUsuario($sisid){
	global $db;
	$cpf = $_SESSION['usucpf'];
	$sql = "select count(*) from seguranca.perfilusuario pu inner join seguranca.perfil p on pu.pflcod = p.pflcod where pu.usucpf = '$cpf' and p.pflstatus = 'A' and p.sisid = $sisid and p.pflnivel = 1;";

	if($db->pegaUm($sql) > 0) return true;
	else
	return false;
}


function permissaoUsuario($cod){


	$perfil =  verficaPerfil($_SESSION['usucpf'],$_SESSION['sisid']);

	if(in_array(SESU_ADMINISTRADOR,$perfil)) return true;

	return in_array($cod,$perfil);
}

function permissaoReuni($arrayPerfil,$unicod){
	global $db;

	$sisid = $_SESSION['sisid'];

	if(verficaSuperUsuario($sisid)) return true;

	$fase = statusFase($unicod);


	switch ($fase){
		case 'N' :
			$permissao = array(IFES_APROVACAO,IFES_CADASTRO,IFES_CONSULTA);
			break;
		case 'E' :
			$permissao = array(IFES_APROVACAO,IFES_CADASTRO,IFES_CONSULTA);
			break;
		case 'G' :
			$permissao = array(SESU_APROVACAO,SESU_CONSULTAGERAL,SESU_PARECER);
			break;
		default:
			$permissao = array();
			break;

	}

	$arrayPerfil = array_intersect($permissao,$arrayPerfil);

	foreach ($arrayPerfil as $perfil){

		if(permissaoUsuario($perfil) === true){
			return true;
			break;
		}
	}

	return false;

}
function statusFase($unicod){
	global $db;
	$sql = "select coalesce(u.stscod,'N') as status  from reuni.unidadepropostastatus u inner join reuni.statusproposta s on s.stscod = u.stscod where unicod = '$unicod' order by u.upscod desc" ;
	if(($dado = $db->pegaUm($sql))!=''){
		return $dado;
	}else return 'N';
}
function mudaStatus($projeto,$codUnidade,$tipo,$status){
	global $db;
	$sql = "insert into reuni.unidadepropostastatus (prjcod,unicod,unitpocod,stscod) values ($projeto,'$codUnidade','$tipo','$status')";
	$db->executar($sql);
	$db->commit();
}

function montaLinkManual( $intSisId )
{
	global $db;

	require_once APPRAIZ . "includes/arquivo.inc";

	if (isset($_SESSION['manuais'][$intSisId]))
	{
		$arrDadosManual = $_SESSION['manuais'][$intSisId];
	}
	else
	{
		$strSql = "select arqid from public.manual as manual WHERE manual.sisid = {$intSisId}";
		$arrDadosManual = $db->pegaLinha( $strSql );
		$_SESSION['manuais'][$intSisId] = $arrDadosManual;
	}

	$booArquivoExiste = existeArquivoFisico( $arrDadosManual["arqid"] );

	if( $booArquivoExiste && sizeof( $arrDadosManual ) > 0 && $arrDadosManual["arqid"] > 0 )
	{
		$intArqId = $arrDadosManual["arqid"];
		$linkmanual = '<a href="../mostra_arquivo.php?id='.$intArqId.'" style="color: #ffffff; margin-right: 10px; margin-bottom: 5px;">Manual</a>';
	}
	else
	{
		$linkmanual = '<script type="text/javascript">function alertMe( objTag )
						{
							alert( objTag.title );
							return false;
						}
						</script>
						<a href="#"
							title="Manual não cadastrado para este módulo, encontra-se em elaboração."
							onclick="return alertMe( this )"
							style="color: #ffffff; margin-right: 10px; margin-bottom: 5px;">Manual</a>';

	}

	return $linkmanual;
}

function montaLinkManual2( $intSisId )
{
	global $db;
	require_once APPRAIZ . "includes/arquivo.inc";
	$strSql = "	 select
					 arqid
				 from
					 public.manual as manual
				 WHERE
					 manual.sisid = {$intSisId}
				";
	$arrDadosManual = $db->pegaLinha( $strSql );
	$booArquivoExiste = existeArquivoFisico( $arrDadosManual["arqid"] );
	if( $booArquivoExiste && sizeof( $arrDadosManual ) > 0 && $arrDadosManual["arqid"] > 0 )
	{
		$intArqId = $arrDadosManual["arqid"];

	$linkmanual = '<a href="../mostra_arquivo.php?id='.$intArqId.'&tela_login=1"
	style="color: #555555; margin-right: 10px; margin-bottom: 5px;">Manual do Sistema</a>';

}
else
{
	$linkmanual = false;

}
	return $linkmanual;
}

/**
 * Monta abas no padrao do SIMEC
 *
 * @param array Um array no seguinte formato:
 *  Array
 *  (
 *      [0] => Array
 *      (
 *          ['descricao'] => TITULO DA ABA 1
 *          ['link']      => LINK DA ABA 1
 *      )
 *
 *      [1] => Array (
 *          ['descricao'] => TITULO DA ABA 1
 *          ['link']      => LINK DA ABA 1
 *          )
 *      )
 *  )
 *
 * @param $boOpenWin Boleano. Quando true esse parametro indica que as abas devem ser abertas em uma nova janela. (09-11-2010)
 * @param bool $incBootstrapCSS Indica se deve ou não incluir o CSS do bootstrap. Por padrão, sempre era exibido.
 */
function montarAbasArray($itensMenu, $url = false, $boOpenWin = false, $incBootstrapCSS = true)
{
    if ($incBootstrapCSS) {
        echo '<link href="../library/bootstrap-3.0.0/css/bootstrap.min-simec.css" rel="stylesheet" media="screen">';
    }

    $url = $url ? $url : $_SERVER['REQUEST_URI'];

    if (is_array($itensMenu)) {
        $rs = $itensMenu;
    } else {
        global $db;
        $rs = $db->carregar($itensMenu);
    }


    $menu = '';
    $menu .= '<br />';
    $menu .= '<div class="row notprint">';
    $menu .= '<div class="col-md-12">';
    $menu .= '<ul class="nav nav-tabs">';

    foreach($rs as $tab){
    	if (is_array($tab)) {
			$active = ($tab['link'] == $url)? 'class="active"' : '';
        	$menu .= '<li ' . $active . '><a href="' . $tab['link'] . '">' . $tab['descricao'] . '</a></li>';
    	}
    }

    $menu .= '</ul>';
    $menu .= '</div>';
    $menu .= '</div>';
    $menu .= '<br />';

    return $menu;
}


function alert($str, $close = false, $escape = true)
{
    if ($escape) {
        $str = str_replace(array("'", "\"", "\\"),
                           array("\\'", "\\\"", "\\\\"), $str);
    }

    $close = $close ? 'self.close();' : '';

    printf('<script type="text/javascript">alert("%s");%s</script>', $str, $close);
}

function removeAcentos($str){
	 $arAcentos = array(
				"/[ÂÀÁÄÃ]/"=>"A",
				"/[âãàáä]/"=>"a",
				"/[ÊÈÉË]/"=>"E",
				"/[êèéë]/"=>"e",
				"/[ÎÍÌÏ]/"=>"I",
				"/[îíìï]/"=>"i",
				"/[ÔÕÒÓÖ]/"=>"O",
				"/[ôõòóö]/"=>"o",
				"/[ÛÙÚÜ]/"=>"U",
				"/[ûúùü]/"=>"u",
				"/ç/"=>"c",
				"/Ç/"=> "C");
	// Tira o acento pela chave do array
	return preg_replace(array_keys($arAcentos), array_values($arAcentos), $str);

}


/**
 * Função que carrega os orgãos de acordo com o tipo
 * e com o município informado.
 *
 * @author Fernando A. Bagno da Silva
 * @param string $editavel -> se os campos estao habilitados ou não
 * @param string $usucpf -> cpf do usuário cadastrado, caso exista
 * /
 */
function carrega_orgao($editavel, $usucpf){


	global $muncod, $regcod, $tpocod, $entid, $db;

	if ($editavel == null){
		$editavel = 'S';
	}

	if ($editavel == 'N'){

		// Se o usuário já está cadastrado no SIMEC, busca o orgão
		$sql = "SELECT
					u.entid as codigo,
					CASE WHEN ee.entorgcod is not null THEN ee.entorgcod ||' - '|| ee.entnome
					ELSE ee.entnome END AS descricao
				FROM
					seguranca.usuario u
				INNER JOIN
					entidade.entidade ee ON
					ee.entid = u.entid
				WHERE
					u.usucpf = '{$usucpf}' AND
					ee.entorgcod <> '73000'";

		$db->monta_combo("entid",$sql,$editavel,"&nbsp;",'ajax_carrega_unidade','','','350','S', 'entid');

	}else {
		// Se for Estadual ou Outros, o usuário deverá informar o orgão
		if ( /*$tpocod == 2 ||*/ $tpocod == 4){
			echo campo_texto( 'orgao', '', $editavel, '', 50, 50, '', '', 'left', '', 0, 'id="orgao"' );

		}else{

			// Caso o tipo seja municipal, verifica o municipio escolhido
			$inner = ( $tpocod == 3 || $tpocod == 2 ) ? 'INNER JOIN
																entidade.endereco eed ON
																eed.entid = ee.entid
															 ': '';

			$uniao = ( $tpocod == 3 || $tpocod == 2 ) ? " UNION ALL (
																  SELECT
																  	999999 AS codigo,
																  	'OUTROS' AS descricao
																)" : '';

			if ( $tpocod == 2 ){
				$regcod = ($_REQUEST['regcod']) ? $_REQUEST['regcod'] : $regcod;
				$clausula = "AND eed.estuf = '" . $regcod . "'";
			}elseif ( $tpocod == 3 ){
				$clausula = "AND eed.muncod = '" . $muncod . "'";
			}
//				$clausula = $tpocod == 3 ? "AND eed.muncod = '" . $muncod . "'" : '';

			$sql = "(SELECT
						ee.entid AS codigo,
						CASE WHEN ee.entorgcod is not null THEN ee.entorgcod ||' - '|| ee.entnome
						ELSE ee.entnome END AS descricao
					FROM
						entidade.entidade ee
					INNER JOIN entidade.funcaoentidade ef ON ef.entid = ee.entid
					INNER JOIN public.tipoorgaofuncao tpf ON ef.funid = tpf.funid
						" . $inner . "
					WHERE
					    ee.entstatus = 'A' and
						tpf.tpocod = '{$tpocod}'
						" . $clausula . " AND
						( ee.entorgcod is null or ee.entorgcod <> '73000' )

					ORDER BY
						ee.entnome)" . $uniao;

			$db->monta_combo("entid",$sql,$editavel,"&nbsp;",'ajax_carrega_unidade','','','350','S', 'entid');

		}

	}

}

/**
 * Função que carrega as unidades daquele órgão (caso
 * existam)
 *
 * @author Fernando A. Bagno da Silva
 * @param integer $entid -> ID do orgão selecionado
 * @param string $editavel -> se os campos estao habilitados ou não
 * @param string $usucpf -> cpf do usuário cadastrado, caso exista
 */
function carrega_unidade($entid, $editavel, $usucpf){

	global $db;

	if ($editavel == null){
		$editavel = 'S';
	}

	if ( $entid == 999999 ){

		echo campo_texto( 'orgao', '', $editavel, '', 50, 50, '', '', 'left', '', 0, 'id="orgao"' );

	} else {

		if ( $editavel == 'N' ){

			$sql = "SELECT
						u.unicod AS codigo,
						u.unicod||' - '||u.unidsc AS descricao
					FROM
						unidade u
					INNER JOIN
						seguranca.usuario su ON
						u.unicod = su.unicod
					WHERE
						usucpf = '{$usucpf}'";

			$db->monta_combo("unicod",$sql,$editavel,"",'selecionar_unidade_orcamentaria', '', '','','S','unicod');

		}else {

			if ( $entid ){

				$uo_total = $db->pegaUm( "SELECT
											count(u.unicod)
										  FROM
										  	unidade u
										  INNER JOIN
										  	entidade.entidade e ON
										  	u.orgcod = e.entorgcod
										  WHERE
										  	unistatus='A' AND
										  	unitpocod='U' AND
										  	e.entid ='{$entid}'" );

			}

			if ( $uo_total > 0 ){

				$sql = "SELECT DISTINCT
							u.unicod AS codigo,
							u.unicod||' - '||unidsc AS descricao
						FROM
							unidade u
						INNER JOIN
							entidade.entidade e ON
							orgcod = entorgcod
						WHERE
							unistatus='A' AND
							u.unitpocod='U' AND
							entid = '{$entid}'
						ORDER BY
							u.unicod";

				$db->monta_combo("unicod",$sql,$editavel,"&nbsp;",'ajax_unidade_gestora', '', '','','S','unicod');

			} else {
				echo '<font style="color:#909090;">Este órgão não possui uma unidade.</font>';
			}
		}
	}
}

/**
 * Função que carrega a unidade gestora de uma unidade orçamentária,
 * quando existir
 *
 * @author Fernando A. Bagno da Silva
 * @param integer $unicod -> ID da unidade selecionada
 * @param string $editavel -> se os campos estao habilitados ou não
 * @param string $usucpf -> cpf do usuário cadastrado, caso exista
 */
function carrega_unidade_gestora($unicod, $editavel, $usucpf){

	global $db;

	if ($editavel == null){
		$editavel = 'S';
	}

	if ( $editavel == 'N' ){

		$sql = "SELECT
					ug.ungcod AS codigo,
					ug.ungcod||' - '||ug.ungdsc as descricao
				FROM
					unidadegestora ug
				INNER JOIN
					seguranca.usuario su ON
					ug.ungcod = su.ungcod
				WHERE
					ungstatus = 'A' AND
					unitpocod = 'U' AND
					su.unicod = '".$unicod."' AND
					usucpf = '".$usucpf."'
				ORDER BY
					ungdsc";

		$db->monta_combo("ungcod",$sql,$editavel,"",'','', '', '', 'S', 'ungcod');

	}else {

		/*
		 * FOI REMOVIDO PORQUE É NECESSÁRIO CADASTRAR USUÁRIOS DE UOs QUE NÃO SÃO DO MEC
		 * ESCOLHENDO A UNIDADE GESTORA, PROBLEMA OCORRIDO NA PROGRAMAÇÃO ORÇAMENTÁRIA
		 * TERMO DE COOPERAÇÃO, REQUISITANTE GUILHERME BORGES
		 * DATA: 10/10/2013
		 */
// 		if ( $unicod == '26101' || $unicod == CODIGO_ORGAO_SISTEMA ){

			$sql = "SELECT
						ungcod AS codigo,
						ungcod||' - '||ungdsc as descricao
					FROM
						unidadegestora
					WHERE
						ungstatus = 'A' AND
						unitpocod = 'U' AND
						unicod = '".$unicod."'
					ORDER BY
						ungdsc";

// 		}else {

			if($db->carregar($sql)){
				$db->monta_combo("ungcod",$sql,$editavel,"&nbsp;",'','','', '', 'S', 'ungcod');
			}else{
				echo '<font style="color: #909090;">Esta unidade não possui uma Unidade Gestora.</font>';
			}

// 		}
	}
}

function carrega_subunidade_orcamentaria($unicod, $exercicio){

	global $db;

	if($unicod){

        $sql = "SELECT suocod AS codigo, suocod ||' - '|| suonome as descricao
                FROM public.vw_subunidadeorcamentaria
                WHERE unocod = '".$unicod."'
                and prsano = '$exercicio'
                ORDER BY suonome";

        $dados = $db->carregar($sql);
        $dados = $dados ? $dados : [];

        $ungcod = 1 == count($dados) ? $dados[0]['codigo'] : null;

        if(count($dados)){
            $db->monta_combo("ungcod",$sql,$editavel,"&nbsp;",'','','', '', 'S', 'ungcod', '', $ungcod);
        }else{
            echo '<font style="color: #909090;">Esta unidade não possui uma Unidade Orçamentária.</font>';
        }
    }
}

/**
 * Função que monta o cabecalho padrão com o brasão brasileiro
 * utilizado nos relatórios do SIMEC
 *
 * @param string $largura
 * @author Fernando A. Bagno da Silva
 * @since 20/02/2009
 */
function monta_cabecalho_relatorio( $largura ){

	global $db;

	$cabecalho = '<table width="'.$largura.'%" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug"  style="border-bottom: 1px solid;">'
				.'	<tr bgcolor="#ffffff">'
				.'		<td valign="top" width="50" rowspan="2"><img src="../imagens/brasao.gif" width="45" height="45" border="0"></td>'
				.'		<td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;">'
				.'			'. NOME_SISTEMA. '<br/>'
//				.'			Acompanhamento da Execução Orçamentária<br/>'
				.'			MEC / SE - Secretaria Executiva <br />'
				.'		</td>'
				.'		<td align="right" valign="middle" height="1" style="padding:5px 0 0 0;">'
				.'			Impresso por: <b>' . $_SESSION['usunome'] . '</b><br/>'
				.'			Hora da Impressão:' . date( 'd/m/Y - H:i:s' ) . '<br />'
				.'		</td>'
				.'	</tr><tr bgcolor="#ffffff">'
				.'		<td colspan="2" align="center" valign="top" style="padding:5 0 5px 0;">'
				.'			<b><font style="font-size:14px;">' . $_REQUEST["titulo"] . '</font></b>'
				.'		</td>'
				.'	</tr>'
				.'</table>';

		return $cabecalho;

}


/**
 * Função que retorna o próximo valor à partir do parâmetro recebido.
 * O padrão para os valores é o seguinte: '0' à '9', e depois de 'A' à 'Z', dependendo
 * do número de caracteres do parâmetro.
 *
 * Exemplos:
 * 			valor passado por parâmetro: '99'   --->  valor retornado: 'AA'
 * 			valor passado por parâmetro: 'ABA'  --->  valor retornado: 'ABC'
 * 			valor passado por parâmetro: '677'  --->  valor retornado: '678'
 * 			valor passado por parâmetro: 'AMZZ' --->  valor retornado: 'ANAA'
 *
 * @param string $num
 * @author Cristiano Cabral
 * @since 07/04/2009
 */
function retornaseq($num) {
	$qtddig = strlen($num);

    if(is_numeric($num)) {
    	$num = (integer) $num;
        $num = $num+1;

        if(strlen($num)>$qtddig) {
        	$num = str_pad('',$qtddig,'A');
            return $num;
        } else {
           	return str_pad($num,$qtddig,'0',0);
        }
	}
    else {
		//valida se é uma sequencia de caracteres permitida
        foreach (range(0,$qtddig-1) as $i){ validachar(substr($num,$i,1)); }

        $numnovo = '';
        //$num = strrev($numnovo);

        foreach (range(1,$qtddig) as $i) {
        	$ascii = ord(substr($num,$qtddig-$i,1));

            if($ascii==90 && !$para) {
            	$numnovo .= 'A';
                $para=false;
           	}
           	else {
            	if(!$para) {
                	$numnovo .= chr($ascii+1);
                    $para=true;
                } else {
                	$numnovo .= chr($ascii);
                }
            }

            try{if (strrev($numnovo)==str_pad('',$qtddig,'A')) throw new Exception('Fim da Sequencia.');} catch (Exception $e){die('Caught exception: '.  $e->getMessage() . "\n");}
		}

		return strrev($numnovo);
	}
}

/**
 * Função que valida se o caracter é permitido.
 *
 * @param char $char
 * @author Cristiano Cabral
 * @since 07/04/2009
 */
function validachar($char) {
      if(in_array(ord($char),range(65,90))) {return true;} else {try{if (strrev($numnovo)==str_pad('',$qtddig,'A')) throw new Exception('Formato Inválido.');} catch (Exception $e){die('Caught exception: '.  $e->getMessage() . "\n");}}
}

/**
 * Função que coloca em extenso valores monetários.
 *
 * @param float $valor
 * @param boolean $maiusculas
 * @author Felipe de Oliveira Carvalho
 * @since 22/12/2009
 */
function valorMonetarioExtenso($valor=0, $maiusculas=false) {

    $singular 	= array("centavo", "real", "mil", "milhão", "bilhão", "trilhão", "quatrilhão");
    $plural 	= array("centavos", "reais", "mil", "milhões", "bilhões", "trilhões", "quatrilhões");

    $c = array("", "cem", "duzentos", "trezentos", "quatrocentos", "quinhentos", "seiscentos", "setecentos", "oitocentos", "novecentos");
    $d = array("", "dez", "vinte", "trinta", "quarenta", "cinquenta", "sessenta", "setenta", "oitenta", "noventa");
    $d10 = array("dez", "onze", "doze", "treze", "quatorze", "quinze", "dezesseis", "dezesete", "dezoito", "dezenove");
    $u = array("", "um", "dois", "três", "quatro", "cinco", "seis", "sete", "oito", "nove");

    $z=0;

    $valor = number_format($valor, 2, ".", ".");
    $inteiro = explode(".", $valor);

    for($i=0;$i<count($inteiro);$i++)
        for($ii=strlen($inteiro[$i]);$ii<3;$ii++)
            $inteiro[$i] = "0".$inteiro[$i];

    $fim = count($inteiro) - ($inteiro[count($inteiro)-1] > 0 ? 1 : 2);

    for ($i=0;$i<count($inteiro);$i++) {
        $valor = $inteiro[$i];
        $rc = (($valor > 100) && ($valor < 200)) ? "cento" : $c[$valor[0]];
        $rd = ($valor[1] < 2) ? "" : $d[$valor[1]];
        $ru = ($valor > 0) ? (($valor[1] == 1) ? $d10[$valor[2]] : $u[$valor[2]]) : "";

        $r = $rc.(($rc && ($rd || $ru)) ? " e " : "").$rd.(($rd && $ru) ? " e " : "").$ru;
        $t = count($inteiro)-1-$i;
        $r .= $r ? " ".($valor > 1 ? $plural[$t] : $singular[$t]) : "";
        if ($valor == "000")$z++; elseif ($z > 0) $z--;
        if (($t==1) && ($z>0) && ($inteiro[0] > 0)) $r .= (($z>1) ? " de " : "").$plural[$t];
        if ($r) $rt = $rt . ((($i > 0) && ($i <= $fim) && ($inteiro[0] > 0) && ($z < 1)) ? ( ($i < $fim) ? ", " : " e ") : " ") . $r;
    }

    if(!$maiusculas) {
    	return($rt ? trim($rt) : "zero");
    } else {
    	return (ucwords($rt) ? ucwords(trim($rt)) : "Zero");
	}

}

/**
 * @description : Função para gravar o tema escolhido pelo usuário
 * @category Function
 * @version 1.0
 * @param string $theme		tema do usuário
 * @param string $usucpf	cpf do usuário
 * @author Juliano Meinen
 * @since 01/06/2010
 * @example : gravaThemaUsuario($_POST['theme]);
 */
function gravaThemaUsuario($theme = "verde",$usucpf = null){
	global $db;

	$usucpf = !$usucpf ? $_SESSION['usucpf'] : $usucpf;

	if(!$usucpf)
		return false;

	if(!$theme)
		return false;

	$sql = "SELECT
				pruvalor
			FROM
				seguranca.parametro_usuario
			WHERE
				usucpf = '$usucpf'
			AND
				prunome = 'tema'
			AND
				prustatus = 'A'";

	$tema = $db->pegaUm($sql);

	if(!$tema){

		$sql = "INSERT
					INTO
				seguranca.parametro_usuario
					(usucpf,prunome,pruvalor,prudesc,prustatus)
				VALUES
					('$usucpf','tema','$theme','Tema do SIMEC selecionado pelo usuário.','A')";

	}else{

		$sql = "UPDATE
					seguranca.parametro_usuario
				SET
					pruvalor = '$theme'
				WHERE
					usucpf = '$usucpf'
				AND
					prunome = 'tema'
				AND
					prustatus = 'A'";
	}

	if($db->executar($sql)){
		$db->commit($sql);
		return true;
	}


}


/**
 * @description : Função que recupera o tema escolhido pelo usuário
 * @category Function
 * @version 1.0
 * @param string $usucpf	cpf do usuário
 * @author Juliano Meinen
 * @since 02/06/2010
 * @return string ( nome do tema)
 * @example : recuperaThemaUsuario();
 */
function recuperaThemaUsuario($usucpf = null){
	global $db;

	$usucpf = !$usucpf ? $_SESSION['usucpf'] : $usucpf;

	if(!$usucpf)
		return false;

	$sql = "SELECT
				pruvalor
			FROM
				seguranca.parametro_usuario
			WHERE
				usucpf = '$usucpf'
			AND
				prunome = 'tema'
			AND
				prustatus = 'A'";

	$tema = $db->pegaUm($sql);

	if($tema)
		return $tema;
	else
		return false;

}

/**
 * @description : Função valida um xml com a referencia de um xsd
 * @category Function
 * @version 1.0
 * @param string $arqXml	pode ser um arquivo ou um string xml
 * @author Wesley Romualdo da Silva
 * @since 10/06/2010
 * @return array com mensagem de erro, caso ocorra.
 * @example :
 	1 - Exemplo 1
 		$arqXml = <<<XML
 		<?xml version="1.0" encoding="iso-8859-1"?>
		<catalogo>
		    <livro>
		        <isbn>8504006115</isbn>
		        <autor>George Orwell</autor>
		        <titulo>1984</titulo>
		        <paginas>302</paginas>
		    </livro>
		</catalogo>
		XML;
		$xsd = 'catalogo.xsd';
		validaXML($arqXml, $xsd);

	2 - Exemplo 2
		$arqXml = 'catalogo.xml';
		$xsd = 'catalogo.xsd';
		validaXML($arqXml, $xsd);

 */
function validaXML($arqXml, $xsd){
	$dom = new DOMDocument('1.0', 'iso-8859-1');

	if( is_file( $arqXml ) ){
		$dom->load($arqXml);
	} else {
		$dom->loadXML($arqXml);
	}

    //Enable user error handling
    libxml_use_internal_errors(true);
    $arMessage = array();
    if(!$dom->schemaValidate( $xsd )) {
    	$errors = libxml_get_errors();

        foreach ($errors as $error) {
        	$arMessage[] = array('code' => $error->code,
                                 'text' => $error->message );
        }
    }
    return $arMessage;
}


/**
 * @description : Função para associar perfil ao usuário e caso esteja pendente, ativá-lo. (Descontinuada)
 * @category Function
 * @version 1.0
 * @param string/array $usucpf	cpf do usuário
 * @param string/array $perfil	perfil do usuário (normalmente é uma constante com valor numérico)
 * @param string	$sisid	(opcional) identificador do sistema
 * @param array	$arrUsuResp	(opcional) colunas e valores para inclusão na tabela de usuarioresponsabilidade do schema
 * @author Juliano Meinen
 * @since 28/07/2010
 * @return boolean ( true se tudo ocorrer normalmente / falso se houver erros )
 * @example : associaPerfilUsuarioPendente($_SESSION['usucpf'],PERFIL_COD_GESTOR,11,array("atiid" => 1);
 */
function associaPerfilUsuarioPendente($usucpf,$perfil,$sisid = null,$arrUsuResp = null){
	global $db;

	if(!$sisid)
		$sisid = $_SESSION['sisid'];

	if(!$usucpf)
		return false;

	if(!$perfil)
		return false;

	if(!is_array($usucpf))
		$arrUsucpf = array($usucpf);
	else
		$arrUsucpf = $usucpf;

	if(!is_array($perfil))
		$arrPerfil = array($perfil);
	else
		$arrPerfil = $perfil;

	foreach($arrUsucpf as $usucpf):
		if(strlen(limpar_numero($usucpf)) == 11)
			$arrUsucpfTratado[] = limpar_numero($usucpf);
		else
			return false;
	endforeach;

	foreach($arrPerfil as $perfil2):
		$arrPerfilTratado[] = $perfil2;
	endforeach;

	//Passo 1 - Selecionar os Usuários Pendentes (suscod = 'A')
	$sql = "select
				usunome,
				usucpf,
				usuemail,
				ususexo,
				ususenha
			from
				seguranca.usuario
			where
				usucpf in ('".implode("','",$arrUsucpfTratado)."')
			and
				suscod = 'P'";
	$arrDadosUsuario = $db->carregar($sql);

	if(is_array($arrDadosUsuario)):
		foreach($arrDadosUsuario as $dadoUsuario):
			$remetente = array("nome" => $_SESSION['usunome'],"email" => $_SESSION['usuemail']);
			$destinatario = $dadoUsuario['usuemail'];
            $assunto = "Aprovação do Cadastro no ".SIGLA_SISTEMA;
			$conteudo = "
				<br/>
				<span style='background-color: red;'><b>Esta é uma mensagem gerada automaticamente pelo sistema. </b></span>
				<br/>
				<span style='background-color: red;'><b>Por favor, não responda. Pois, neste caso, a mesma será descartada.</b></span>
				<br/>
				";
			$conteudo .= sprintf(
									"%s %s<p>Sua conta está ativa.</p>",
									$dadoUsuario['ususexo'] == 'M' ? 'Prezado Sr.' : 'Prezada Sra.',
									$dadoUsuario['usunome']
								);
			//Passo 2 - Atualizar os status dos usuários Pendentes (suscod = 'A')
			alterar_status_usuario( $dadoUsuario['usucpf'], 'A', "Atribuição de responsabilidade em atividade ou projeto.", $sisid );
			//Passo 3 - Enviar e-mails para usuários Pendentes de Ativação
			//enviar_email( $remetente, $destinatario, $assunto, $conteudo );
			$arrUsucpfPendentes[] = $dadoUsuario['usucpf'];
		endforeach;
	endif;

	//Passo 4 - Coletar dados do esquema por meio do sisid
	$sqlSchema = "select
				sisdiretorio
			from
				seguranca.sistema
			where
				sisid = $sisid";
	$schema = $db->pegaUm($sqlSchema);

	if(is_array($arrUsuResp)):
		foreach($arrUsuResp as $campo => $valor):
			$arrAnd[] = "$campo = '$valor'";
			$arrInsert[] = $campo;
			$arrValorInsert[] = "'$valor'";
		endforeach;
	endif;

	//Passo 5 - Desativar todos os usuários que não fazem parte dos usuários passados por parâmetro
	if( count($arrUsucpfTratado) == count($arrPerfilTratado) ):
		$n = 0;
		foreach($arrUsucpfTratado as $usuCpf):
			$sqlClear = "update
							$schema.usuarioresponsabilidade
						set
							rpustatus = 'I'
						where
							usucpf != '$usuCpf'
						and
							pflcod = {$arrPerfilTratado[$n]}
						".(count($arrAnd) ? " AND ".implode(" AND ", $arrAnd) : "")."";
			dbg($sqlClear);//$db->executar($sqlClear);
		$n++;
		endforeach;
	else:
		$sqlClear = "update
						$schema.usuarioresponsabilidade
					set
						rpustatus = 'I'
					where
						usucpf not in ('".implode("','", $arrUsucpfTratado)."')
					and
						pflcod = {$arrPerfilTratado[0]}
					".(count($arrAnd) ? " AND ".implode(" AND ", $arrAnd) : "")."";
		dbg($sqlClear);//$db->executar($sqlClear);
	endif;


	$n = 0;
	foreach($arrUsucpfTratado as $usuCpf):

		$pflcod = count($arrUsucpfTratado) == count($arrPerfilTratado) ? $arrPerfilTratado[$n] : $arrPerfilTratado[0];

		$sqlConsulta = "select
							rpuid
						from
							$schema.usuarioresponsabilidade
						where
							usucpf = '$usuCpf'
						and
							rpustatus = 'I'
						and
							pflcod = $pflcod
						".(count($arrAnd) ? " AND ".implode(" AND ", $arrAnd) : "")."";
		$rpuid = $db->pegaUm($sqlConsulta);
		if($rpuid):
			$arrSql .= "update
							$schema.usuarioresponsabilidade
						set
							rpustatus = 'A'
						where
							rpuid = $rpuid;";
		else:
			$arrSql .= "insert into $schema.usuarioresponsabilidade
							(usucpf,rpustatus,rpudata_inc,pflcod".(count($arrInsert) ? ",".implode(",",$arrInsert) : "").")
						values
							('$usuCpf','A',now(),'$pflcod'".(count($arrValorInsert) ? ",".implode(",",$arrValorInsert) : "").");";
		endif;
		$n++;
	endforeach;
}

function somar_dias_uteis($str_data, $int_qtd_dias_somar = 7) {

    // Caso seja informado uma data do tipo DATETIME - aaaa-mm-dd 00:00:00
    // Transforma para DATE - aaaa-mm-dd
    $str_data = substr($str_data,0,10);

    // Se a data estiver no formato brasileiro: dd/mm/aaaa
    // Converte-a para o padrão americano: aaaa-mm-dd
    if ( preg_match("@/@",$str_data) == 1 ) {
        $str_data = implode("-", array_reverse(explode("/",$str_data)));
    }

    $array_data = explode('-', $str_data);
    $count_days = 0;
    $int_qtd_dias_uteis = 0;
    while ( $int_qtd_dias_uteis < $int_qtd_dias_somar ) {
        $count_days++;
                if ( ( $dias_da_semana = gmdate('w', strtotime('+'.$count_days.' day', mktime(0, 0, 0, $array_data[1], $array_data[2], $array_data[0]))) ) != '0' && $dias_da_semana != '6' ) {
            $int_qtd_dias_uteis++;
        }
    }
    return gmdate('Y-m-d',strtotime('+'.$count_days.' day',strtotime($str_data)));
}

/**
 * Retorna uma lista de perfis com descrição e código do usuário no módulo 
 * especificado pelo parametro. sisid
 * 
 * @global cls_banco $db
 * @param string $usucpf
 * @param string $sisid
 * @return array/boolean Retorna lista de códigos de perfis do usuário logado.
 */
function pegaPerfilGeral($usucpf = null, $sisid = null){
	global $db;

	$usucpf = !$usucpf ? $_SESSION['usucpf'] : $usucpf;
	$sisid  = !$sisid  ? $_SESSION['sisid']  : $sisid;

	if (isset($_SESSION['perfil'][$usucpf][$sisid]))
	{
		$arrPerfil = $_SESSION['perfil'][$usucpf][$sisid];
	}
	else
	{
		$sql = "SELECT
                    pu.pflcod
                FROM seguranca.perfilusuario pu
                    JOIN seguranca.perfil p ON p.pflcod = pu.pflcod
                        AND pu.usucpf = '$usucpf'
                        AND p.sisid = '$sisid'
                        AND pflstatus = 'A'
        ";

		$resultado = $db->carregar($sql);
        if (!$resultado) return false;

		$arrPerfil = array();

		foreach ($resultado as $perfil)
		{
			$arrPerfil[] = $perfil['pflcod'];
		}

		$_SESSION['perfil'][$usucpf][$sisid] = $arrPerfil;
	}

	return $arrPerfil ? $arrPerfil : false;
}

function pegaPerfilPorUsuario($usucpf = null, $sisid = null){
	global $db;

	$usucpf = !$usucpf ? $_SESSION['usucpf'] : $usucpf;
	$sisid  = !$sisid  ? $_SESSION['sisid']  : $sisid;

	if (isset($_SESSION['perfilusuario'][$usucpf][$sisid]))
	{
		$arrPerfil = $_SESSION['perfilusuario'][$usucpf][$sisid];
	}
	else
	{
		$sql = "select pu.pflcod, p.pfldsc
		from seguranca.perfilusuario pu
		inner join seguranca.perfil p on p.pflcod = pu.pflcod and pu.usucpf = '$usucpf' and p.sisid = $sisid and pflstatus = 'A'";

		$arrPerfil = $db->carregar($sql);

		$_SESSION['perfilusuario'][$usucpf][$sisid] = $arrPerfil;
	}

	return $arrPerfil ? $arrPerfil : false;
}

function carregarPerfils($sisid = null){
	global $db;

	if (isset($_SESSION['perfils'][$sisid]))
	{
		$arrPerfils = $_SESSION['perfils'][$sisid];
	}
	else
	{
		$sql = "select p.pflcod as codigo, p.pfldsc as descricao from seguranca.perfil p where p.sisid = {$sisid} and pflstatus = 'A'";

		$arrPerfils = $db->carregar($sql);

		$_SESSION['perfils'][$sisid] = $arrPerfils;
	}

	return $arrPerfils;
}

function carregarTipoDocumentos($sisid = null){
	global $db;

	if (isset($_SESSION['tipodocumentos']['geral'][$sisid]))
	{
		$tipoDocumentos = $_SESSION['tipodocumentos']['geral'][$sisid];
	}
	else
	{
		$sql = "select tpdid as codigo, tpddsc as descricao from workflow.tipodocumento where sisid = '{$sisid}' order by descricao";

		$tipoDocumentos = (array) $db->carregar($sql);

		$_SESSION['tipodocumentos']['geral'][$sisid] = $tipoDocumentos;
	}

	return $tipoDocumentos;
}

function carregarTipoDocumentosPainel($sisid = null){
	global $db;

	if (isset($_SESSION['tipodocumentos']['painel'][$sisid]))
	{
		$tipoDocumentos = $_SESSION['tipodocumentos']['painel'][$sisid];
	}
	else
	{
		$sql = "select tpdid as codigo, tpddsc as descricao from workflow.tipodocumento where sisid = '{$sisid}' and painel = 'A' order by descricao";

		$tipoDocumentos = (array) $db->carregar($sql);

		$_SESSION['tipodocumentos']['painel'][$sisid] = $tipoDocumentos;
	}

	return $tipoDocumentos;
}

function popupAlertaGeral($texto = "Informe o texto.", $largura = "400px", $altura = "200px", $id = "id_popup_alerta", $classeCSS = null){
?>
    <style>
        .popup_alerta{
            width:<?php echo $largura ?>;
            height:<?php echo $altura+5; ?>;
            position:absolute;
            z-index:999;
            top:50%;
            left:50%;
            margin-top:-<?php echo $altura/2 ?>;
            margin-left:-<?php echo $largura/2 ?>;
            border: solid 2px black;
            background-color:#FFFFFF;
        }
    </style>


    <div id="<?php echo $id ?>" class="popup_alerta <?php echo $classeCSS ?>" >
        <div style="width:100%; text-align:right">
            <img src="../imagens/fechar.jpeg" title="Fechar" style="margin-top:5px; margin-right:5px; cursor:pointer" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" />
        </div>
        <div style="padding:5px;text-align:justify;">
            <?php echo $texto ?>
            <center>
                <input type="button" id="ok" name="ok" value="OK" onclick="document.getElementById('<?php echo $id ?>').style.display='none'">
            </center>
        </div>
    </div>

<?PHP

}

function verificaExistenciaArquivo($arqid,$nomeEsquema = null)
{
	$nomeEsquema = !$nomeEsquema ? $_SESSION['sisdiretorio'] : $nomeEsquema;
	$caminho = APPRAIZ."arquivos/".$nomeEsquema.'/'.floor($arqid/1000).'/'.$arqid;
	if(is_file($caminho)){
		return true;
	}else{
		return false;
	}
}

function verificaArquivoUsuarioTabela($schema,$tbl,$usucpf = null)
{
	global $db;

	$usucpf = !$usucpf ? $_SESSION['usucpf'] : $usucpf;

	$arrWhere[] = "a.usucpf = '$usucpf'";
	$arrWhere[] = "(a.arqid / 1000) between 647 and 725";
	$arrWhere[] = "a.arqid not in(select arqid from public.arquivo_recuperado)";

	$sql = "select
				a.arqid
			from
				public.arquivo a
			inner join
				$schema.$tbl tbl ON tbl.arqid = a.arqid
			and
				(a.arqstatus = 'A'::bpchar or a.arqstatus = '1'::bpchar)
			".($arrWhere ? " and ".implode(" and ",$arrWhere) : "");

	if($db->pegaUm($sql)){
		return true;
	}else{
		return false;
	}

}

function extenso($valor = 0, $maiusculas = false, $masc = true) {

	$singular  = array("", "", "mil", "milhão", "bilhão", "trilhão", "quatrilhão");
	$plural    = array("", "", "mil", "milhões", "bilhões", "trilhões", "quatrilhões");

	$c = array("", "cem", "duzentos", "trezentos", "quatrocentos", "quinhentos", "seiscentos", "setecentos", "oitocentos", "novecentos");
	$d = array("", "dez", "vinte", "trinta", "quarenta", "cinquenta", "sessenta", "setenta", "oitenta", "noventa");
	$d10 = array("dez", "onze", "doze", "treze", "quatorze", "quinze", "dezesseis", "dezesete", "dezoito", "dezenove");
	if($masc){
		$u = array("", "um", "dois", "três", "quatro", "cinco", "seis", "sete", "oito", "nove");
	}else{
		$u = array("", "uma", "duas", "três", "quatro", "cinco", "seis", "sete", "oito", "nove");
	}

	$z = 0;
	$rt = "";

	$valor = number_format($valor, 2, ".", ".");
	$inteiro = explode(".", $valor);
	for($i=0;$i<count($inteiro);$i++)
		for($ii=strlen($inteiro[$i]);$ii<3;$ii++)
			$inteiro[$i] = "0".$inteiro[$i];

	$fim = count($inteiro) - ($inteiro[count($inteiro)-1] > 0 ? 1 : 2);
	for ($i=0;$i<count($inteiro);$i++) {
		$valor = $inteiro[$i];
		$rc = (($valor > 100) && ($valor < 200)) ? "cento" : $c[$valor[0]];
		$rd = ($valor[1] < 2) ? "" : $d[$valor[1]];
		$ru = ($valor > 0) ? (($valor[1] == 1) ? $d10[$valor[2]] : $u[$valor[2]]) : "";

		$r = $rc.(($rc && ($rd || $ru)) ? " e " : "").$rd.(($rd &&
		$ru) ? " e " : "").$ru;
		$t = count($inteiro)-1-$i;
		$r .= $r ? " ".($valor > 1 ? $plural[$t] : $singular[$t]) : "";
		if ($valor == "000")$z++; elseif ($z > 0) $z--;
		if (($t==1) && ($z>0) && ($inteiro[0] > 0)) $r .= (($z>1) ? " de " : "").$plural[$t];
		if ($r) $rt = $rt . ((($i > 0) && ($i <= $fim) && ($inteiro[0] > 0) && ($z < 1)) ? ( ($i < $fim) ? ", " : " e ") : " ") . $r;
	}

	if(!$maiusculas){
		return($rt ? $rt : "zero");
	} else {
// 		if ($rt) $rt=preg_replace(" E "," e ",ucwords($rt));
		if ($rt) $rt=str_replace(" E "," e ",ucwords($rt));
		return (($rt) ? ($rt) : "Zero");
	}

}

function retornaPflcodFilhos($arrPerfil){
	global $db;
	$sql = "select
				pflcodfilho
			from
				seguranca.perfilpermissao
			where
				pflcodpai in (".implode(",",$arrPerfil).")
			and
				pflcodfilho not in (".implode(",",$arrPerfil).")";
	$arrFilhos = $db->carregarColuna($sql);
	if($arrFilhos){
		foreach($arrFilhos as $f){
			$arrPerfil[] = $f;
		}
		$arrPerfil = retornaPflcodFilhos($arrPerfil);
	}
	return array_unique($arrPerfil);
}
function montaLinkRegra(){
	global $db;

	if( $db->testa_superuser() ){
		$html = '<br><table class="rodape" align=\'center\' border="0" cellspacing="0" cellpadding="2" class="notprint" style="width:100%; color: #ffffff;" bgcolor="#E6AC10">
						<tr>
							<td colspan="2" height="2"></td>
						</tr>
						<tr>
							<td style="text-align: center;" valing="top"><span style="cursor:pointer;" onclick="abrirRegras( '.$_SESSION['mnuid'].' );"><b>Visualizar Regras</b></span></td>
						</tr>
					</table>';
	}
	echo $html;
}

function formata_string_sql( $string ){
	if( is_array( $string ) ){
		$arString = array();
		foreach ($string as $v) {
			$v = trim($v);
			$v = !empty($v) ? "'".$v."'" : 'null';
			array_push( $arString, $v);
		}
		$string = array();
		$string = $arString;
	} else {
		$string = trim($string);
		$string = !empty($string) ? "'".$string."'" : 'null';
	}

	return $string;
}


/**
 * Monta o HTML da combo de municipios
 *
 * @param string $stNome Nome do campo que será criado HTML/PHP
 * @param string $stLabel Descrição do campo
 * @param string $habilitado habilita/desabilita combo
 * @param string $campo_busca_codigo mostra/oculta pesquisa rápida
 * @param string $campo_flag_contem mostra/oculta checkbox contem/não contem
 * @param string $stWidth Largura da combo
 * @param string $boConflict ativa/desativa noConclict jQuery
 * @author Wescley Guedes Lima
 * @since 13/07/2012
 */
function mostrarComboPopupMunicipios( $stNome = 'muncod', $stLabel = 'Municípios', $habilitado = 'S', $campo_busca_codigo = true, $campo_flag_contem = true, $stWidth='400px', $boConflict = false ){

	global $$stNome;

	echo "<script>
			if(typeof jQuery == 'undefined'){
				document.writeln('<script src=\"../includes/JQuery/jquery-1.4.2.js\" type=\"text/javascript\"></sc'+'ript>');
			}
			document.writeln('<script src=\"../includes/jquery-autocomplete/jquery.autocomplete.js\" type=\"text/javascript\"></sc'+'ript>');
			document.writeln('<link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/jquery-autocomplete/jquery.autocomplete.css\" />');
		  </script>";

	if($boConflict){
		echo "<script>jQuery.noConflict();</script>";
		$stConflict = "jQuery";
	}else{
		$stConflict = "$";
	}

	// Funções JS do componente
	echo "<script>

			{$stConflict}(function(){

				{$stConflict}('.mostrarCombo').click(function(){
					{$stConflict}('#{$stNome}_campo_off, #{$stNome}_campo_on').toggle();
				});

				{$stConflict}('.mostrarModalMunicipios').dblclick(function(){

					var janela = window.open('../geral/popup_municipios.php?nome={$stNome}', 'popListaMunicipios', 'height=500,width=400,scrollbars=yes,top=50,left=200');
					janela.focus();

				});

				{$stConflict}('.mostrarModalMunicipiosClick').click(function(){

					var janela = window.open('../geral/popup_municipios.php?nome={$stNome}', 'popListaMunicipios', 'height=500,width=400,scrollbars=yes,top=50,left=200');
					janela.focus();

				});

				{$stConflict}('.mostraMunicipios').live('click',function(){
					{$stConflict}('#tr_'+this.id+', #img_mais_'+this.id+', #img_menos_'+this.id).toggle();
				});

				var dominio = location.href.split('/')[2];

				{$stConflict}('#combopopup_campo_busca_{$stNome}').autocomplete('http://'+dominio+'/geral/popup_municipios.php?requisicao=buscarMunicipios', {
					cacheLength:0,
					width: 440,
					scrollHeight: 220,
					selectFirst: true,
					autoFill: true,
					extraParams: { muncod: function(){

											var dados = new Array();
											{$stConflict}('#{$stNome} option').each(function(){
												if({$stConflict}(this).val()){
											   		dados.push({$stConflict}(this).val());
											   	}
											});

											return dados;
									},
								   remover: function(){
								   		return {$stConflict}('#remover_valor_{$stNome}').attr('checked');
									}
								}

				}).result(function(event, data, formatted) {

					var codigo = data[1];
					var descricao = data[0].split(' - ')[1].split('/')[0];

		   			if({$stConflict}('#remover_valor_{$stNome}').attr('checked')){
						combo_popup_remover_item( '{$stNome}', codigo, true );
					}else{
						combo_popup_adicionar_item( '{$stNome}', codigo, descricao, true );
					}

					{$stConflict}('#combopopup_campo_busca_{$stNome}').val('');

			    });

			});

		  </script>";

	$buscaCodigo = '';
	if ( $campo_busca_codigo == true && $habilitado == 'S' )
	{
		$buscaCodigo .= '<input type="text" id="combopopup_campo_busca_' . $stNome . '" onmouseover="MouseOver( this );" onfocus="MouseClick(this);" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" class="normal" style="margin: 2px 0;" />';
//		$buscaCodigo .= '&nbsp;<img title="adicionar" align="absmiddle" src="/imagens/check_p.gif" onclick="combo_popup_buscar_codigo( \'' . $nome . '\', document.getElementById( \'combopopup_campo_busca_' . $nome . '\' ).value );"/>';
//		$buscaCodigo .= '&nbsp;<img title="remover" align="absmiddle" src="/imagens/exclui_p.gif" onclick="combo_popup_remover_item( \'' . $nome . '\', document.getElementById( \'combopopup_campo_busca_' . $nome . '\' ).value, true );"/>';
		$buscaCodigo .= '&nbsp;<input type="checkbox" name="remover_valor_'.$stNome.'" id="remover_valor_'.$stNome.'" />&nbsp;Remover';
		$buscaCodigo .= '&nbsp;<img title="abrir lista" align="absmiddle" src="/imagens/pop_p.gif" class="mostrarModalMunicipiosClick"/>';
	}

	$flagContem = '';
	if ( $campo_flag_contem == true )
	{
		$nomeFlagContemGlobal = $stNome . '_campo_excludente';
		global ${$nomeFlagContemGlobal};
		$flagContem .= '<input type="checkbox" id="' . $stNome . '_campo_excludente" name="' . $stNome . '_campo_excludente" value="1" ' . ( ${$nomeFlagContemGlobal} ? 'checked="checked"' : '' ) . ' style="margin:0;" />';
		$flagContem .= '&nbsp;<label for="' . $stNome . '_campo_excludente">Não contém</label>';
	}

	// HTML da tr do filtro
	echo '<tr>
			<td class="subtituloDireita mostrarCombo" valign="top">'.$stLabel.'</td>
			<td>
				<input type="hidden" id="' . $stNome . '_campo_flag" name="' . $stNome . '_campo_flag" value="1"/>
				<div id="'.$stNome.'_campo_off">
					<img border="0" align="middle" src="../imagens/combo-todos.gif" class="mostrarCombo">
				</div>
				<div id="'.$stNome.'_campo_on" style="display:none;">
					<table width="'.$stWidth.'" cellpadding="0" cellmarging="0">
						<tr>
							<td align="left">'.$buscaCodigo.'</td>
							<td align="right">'.$flagContem.'</td>
						</tr>
					</table>
					<select style="width:'.$stWidth.';" class="mostrarModalMunicipios" id="'.$stNome.'" name="'.$stNome.'[]" size="10" multiple="multiple">
						<option value="">Duplo clique para selecionar da lista</option>
					</select>
				</div>
			</td>
		  </tr>';



}

/*
 * Função para montar combo com estado,mesoregião,micriregião e municipio
 * passando os parametros do nome do campo,obrigatório,disabled,value
 *
 * Deverá utilizar jQuery
 * montaComboTerritorio("obra",array("estuf"=>array("obrigatorio"=>"S"),"mescod"=>array("obrigatorio"=>"N","disabled"=>"S"),"miccod"=>array("disabled"=>"S"),"muncod"=>array("disabled"=>"S")));
 * Ná página colocar:
 * if($_POST['requisicaoAjax']){
	$_POST['requisicaoAjax']();
	die;
	}
 */
function montaComboTerritorio($alias,$arrParametros = array()){
	header("Content-Type: text/html; charset=ISO-8859-1",true);
	if($arrParametros){
		$_SESSION['combo_territorio']['arrParametros'][$alias] = $arrParametros;
	}
	$arrParametros['estuf']['obrigatorio'] 	= $arrParametros['estuf']['obrigatorio']  ? $arrParametros['estuf']['obrigatorio']  : "N";
	$arrParametros['estuf']['disabled']    	= $arrParametros['estuf']['disabled'] 	  ? $arrParametros['estuf']['disabled'] 	: "S";
	$arrParametros['estuf']['value'] 	  	= $arrParametros['estuf']['value'] 		  ? $arrParametros['estuf']['value'] 		: "";
	?>
	<script type="text/javascript">
	function <?php echo $alias ?>montaComboFake()
	{
		return true;
	}

	function <?php echo $alias ?>montaComboMunicipio(filtrameso){
		if($("#<?php echo $alias ?>estuf").val())
			var estuf = $("#<?php echo $alias ?>estuf").val();
		if($("#<?php echo $alias ?>mescod").val())
			var mescod = $("#<?php echo $alias ?>mescod").val();
		if($("#<?php echo $alias ?>miccod").val())
			var miccod = $("#<?php echo $alias ?>miccod").val();
		var erro = 0;
		if(!estuf){
			$("#<?php echo $alias ?>td_muncod").html("Selecione o Estado.");
			$("#<?php echo $alias ?>td_meso").html("Selecione o Estado.");
			$("#<?php echo $alias ?>td_micro").html("Selecione a Mesorregião.");
			erro = 1;
		}
		if(!mescod){
			$("#<?php echo $alias ?>td_micro").html("Selecione a Mesorregião.");
		}

		if(erro == 0){
			$.ajax({
					   type: "POST",
					   url: window.location,
					   data: 'requisicaoAjax=montaComboMunicipio&<?php echo $alias ?>estuf= ' +estuf + '&<?php echo $alias ?>mescod= ' +mescod + '&<?php echo $alias ?>miccod= ' +miccod + '&alias=<?php echo $alias ?>',
					   success: function(msg){
							$('#<?php echo $alias ?>td_muncod').html( msg );
							if(filtrameso){
								<?php echo $alias ?>montaComboMesorregiao();
							}
					   }
			 });
		}
	}

	function <?php echo $alias ?>montaComboMesorregiao(filtrameso){
		if($("#<?php echo $alias ?>estuf").val())
			var estuf = $("#<?php echo $alias ?>estuf").val();
		$.ajax({
				   type: "POST",
				   url: window.location,
				   data: "requisicaoAjax=montaComboMesorregiao&<?php echo $alias ?>estuf= " +estuf + "&alias=<?php echo $alias ?>",
				   success: function(msg){
						$('#<?php echo $alias ?>td_meso').html( msg );
						<?php echo $alias ?>montaComboMunicipio(filtrameso);

				   }
		 });
	}

	function <?php echo $alias ?>montaComboMicrorregiao(filtrameso){
		if($("#<?php echo $alias ?>mescod").val())
			var mescod = $("#<?php echo $alias ?>mescod").val();
		$.ajax({
				   type: "POST",
				   url: window.location,
				   data: "requisicaoAjax=montaComboMicrorregiao&<?php echo $alias ?>mescod= " +mescod + "&alias=<?php echo $alias ?>",
				   success: function(msg){
						 $('#<?php echo $alias ?>td_micro').html( msg );
						<?php echo $alias ?>montaComboMunicipio(filtrameso);

				   }
		 });
	}
	</script>
	<?php
	global $db;

		$texto = '<tr>
					<td class="SubTituloDireita">Estado:</td>
					<td>';
                if(isset($arrParametros['sgrid'])){
                    $sql = "

                        SELECT DISTINCT (e.estcod), e.estuf AS codigo, e.estdescricao AS descricao
                        FROM territorios.estado e
                        JOIN territorios.mesoregiao m
                                ON e.estuf = m.estuf
                        JOIN obras2.supervisao_grupo_mesoregiao sgm
                                ON sgm.mescod = m.mescod
                        WHERE sgrid = ".$arrParametros['sgrid']." AND sgm.sgmstatus = 'A'
                        ORDER BY e.estdescricao ASC

                    ";
                } else{
                    $sql = "select e.estuf as codigo, e.estdescricao as descricao from territorios.estado e order by e.estdescricao asc";
                }

		$texto .=		$db->monta_combo("{$alias}estuf", $sql, $arrParametros['estuf']['disabled'], "Selecione...", $alias.'montaComboMunicipio(true);'.$alias.'montaComboFake', '', '', '',$arrParametros['estuf']['obrigatorio'], $alias."estuf",true,$arrParametros['estuf']['value']);
		$texto .= 	'</td>
				   </tr>
				   <tr>
						<td class="SubTituloDireita">Mesorregião:</td>
						<td id="'.$alias.'td_meso" >'.($arrParametros['estuf']['value'] ? montaComboMesorregiao(true,$alias) : "Selecione o Estado.").'</td>
					</tr>
				   <tr>
						<td class="SubTituloDireita">Microrregião:</td>
						<td id="'.$alias.'td_micro" >'.($arrParametros['mescod']['value'] ? montaComboMicrorregiao(true,$alias) : "Selecione a Mesorregião.").'</td>
					</tr>
					 <tr>
						<td class="SubTituloDireita">Município:</td>
						<td id="'.$alias.'td_muncod" >'.($arrParametros['estuf']['value'] ? montaComboMunicipio(true,$alias) : "Selecione o Estado.").'</td>
					</tr>';
		echo $texto;
}

function montaComboMesorregiao($retorno = false, $alias = null){
	global $db;
	header("Content-Type: text/html; charset=ISO-8859-1",true);
	$alias = $_POST['alias'] ? $_POST['alias'] : $alias;
	$arrParametros = $_SESSION['combo_territorio']['arrParametros'][$alias];
	$arrParametros['mescod']['obrigatorio'] = $arrParametros['mescod']['obrigatorio'] ? $arrParametros['mescod']['obrigatorio'] 	: "N";
	$arrParametros['mescod']['disabled'] 	= $arrParametros['mescod']['disabled']    ? $arrParametros['mescod']['disabled'] 		: "S";
	$arrParametros['mescod']['value'] 		= $arrParametros['mescod']['value'] 	  ? $arrParametros['mescod']['value'] 			: "";
	$_POST[$alias.'estuf'] = $_POST[$alias.'estuf'] ? $_POST[$alias.'estuf'] : $arrParametros['estuf']['value'];
	$sql = "select mescod as codigo,mesdsc as descricao from territorios.mesoregiao where estuf ='".trim($_POST[$alias.'estuf'])."' order by mesdsc";
	$texto .=	$db->monta_combo($alias."mescod", $sql, $arrParametros['mescod']['disabled'],'Selecione...',$alias.'montaComboMicrorregiao(false);'.$alias.'montaComboFake', '', '','',$arrParametros['mescod']['obrigatorio'],$alias.'mescod',true,$arrParametros['mescod']['value']);
	if($retorno){
		return $texto;
	}else{
		echo $texto;
	}
}

function montaComboMicrorregiao($retorno = false, $alias = null){
	global $db;
	header("Content-Type: text/html; charset=ISO-8859-1",true);
	$alias = $_POST['alias'] ? $_POST['alias'] : $alias;
	$arrParametros = $_SESSION['combo_territorio']['arrParametros'][$alias];
	$arrParametros['miccod']['obrigatorio'] = $arrParametros['miccod']['obrigatorio'] ? $arrParametros['miccod']['obrigatorio'] 	: "N";
	$arrParametros['miccod']['disabled'] 	= $arrParametros['miccod']['disabled'] 	  ? $arrParametros['miccod']['disabled'] 		: "S";
	$arrParametros['miccod']['value'] 		= $arrParametros['miccod']['value'] 	  ? $arrParametros['miccod']['value'] 			: "";
	$_POST[$alias.'mescod'] = $_POST[$alias.'mescod'] ? $_POST[$alias.'mescod'] : $arrParametros['mescod']['value'];
	$sql = "select miccod as codigo ,micdsc as descricao from territorios.microregiao where mescod ='".trim($_POST[$alias.'mescod'])."' order by micdsc";
	$texto .=	$db->monta_combo($alias."miccod", $sql, $arrParametros['miccod']['disabled'],'Selecione...',$alias.'montaComboMunicipio(false);'.$alias.'montaComboFake', '', '','',$arrParametros['miccod']['obrigatorio'],$alias.'miccod',true,$arrParametros['miccod']['value']);
	if($retorno){
		return $texto;
	}else{
		echo $texto;
	}
}

function montaComboMunicipio($retorno = false, $alias = null){
	global $db;
	header("Content-Type: text/html; charset=ISO-8859-1",true);
	$alias = $_POST['alias'] ? $_POST['alias'] : $alias;
	$arrParametros = $_SESSION['combo_territorio']['arrParametros'][$alias];
	$arrParametros['muncod']['obrigatorio'] = $arrParametros['muncod']['obrigatorio']  ? $arrParametros['muncod']['obrigatorio']  : "N";
	$arrParametros['muncod']['disabled']    = $arrParametros['muncod']['disabled']     ? $arrParametros['muncod']['disabled'] 	  : "S";
	$arrParametros['muncod']['value']       = $arrParametros['muncod']['value'] 	   ? $arrParametros['muncod']['value'] 		  : "";
	$_POST[$alias.'mescod'] = $_POST[$alias.'mescod'] ? $_POST[$alias.'mescod'] : $arrParametros['mescod']['value'];
	$_POST[$alias.'miccod'] = $_POST[$alias.'miccod'] ? $_POST[$alias.'miccod'] : $arrParametros['miccod']['value'];
	$_POST[$alias.'estuf'] = $_POST[$alias.'estuf'] ? $_POST[$alias.'estuf'] : $arrParametros['estuf']['value'];
	if(trim($_POST[$alias.'mescod']) && trim($_POST[$alias.'mescod']) != "undefined"){
		$and.= " and mescod = '" .trim($_POST[$alias.'mescod'])."' ";
	}
	if(trim($_POST[$alias.'miccod']) && trim($_POST[$alias.'miccod']) != "undefined"){
		$and.= " and miccod = '" .trim($_POST[$alias.'miccod'])."' ";
	}
	if(trim($_POST[$alias.'estuf']) && trim($_POST[$alias.'estuf']) != "undefined"){
		$and.= " and estuf = '" .trim($_POST[$alias.'estuf'])."' ";
	}
	$sql = "select muncod as codigo ,mundescricao as descricao from territorios.municipio where 1=1 " .$and. " order by mundescricao";
	$texto .=	$db->monta_combo($alias."muncod", $sql, $arrParametros['muncod']['disabled'],'Selecione...','', '', '','',$arrParametros['muncod']['obrigatorio'],$alias.'muncod',true,$arrParametros['muncod']['value']);
	if($retorno){
		return $texto;
	}else{
		echo $texto;
	}
}

/*
* Incluir:
* <script type="text/javascript" src="../includes/JQuery/carouFredSel-6.2.1/jquery.carouFredSel-6.2.1.js"></script>
*
* Parametros:
*
* ['tamanho_icone'] 	define width do icone em pixels;
* ['qtd_itens'] 		define quantidade de icones;
* ['background-color']  define cor de fundo;
*
*/
function desenha_slideShow( $param = Array() ){

	$tamanho_icone  = $param['tamanho_icone'] 	? $param['tamanho_icone'] 	: 90;
	$id			 	= $param['id'] 				? $param['id'] 				: 'foo';
	$qtd_itens		= $param['qtd_itens'] 		? $param['qtd_itens'] 		: '';

	$tamanho 			= $qtd_itens ? ($qtd_itens*($tamanho_icone+35))+30 : '';
	$tamanho_div_menu 	= $tamanho_icone*1.355;

	$includes = '';

	$includes .= '<link href="../includes/css/barra_ferramentas.css" type="text/css" rel="stylesheet">';

	$conf_tamanho = $tamanho != '' ? 'width	: '.$tamanho.',' : '';

	$scripts = '<script>
					$(document).ready(function() {
					// Using custom configuration
					   	$("#'.$id.'").carouFredSel({
					   		//items: 3,
						    circular: false,
						    infinite: false,
						    auto    : false,
							'.$conf_tamanho.'
						    prev    : {
						        button  : "#'.$id.'_prev",
						        key     : "left"
						    },
						    next    : {
						        button  : "#'.$id.'_next",
						        key     : "right"
						    }
						});

						$(\'#'.$id.'_externo\').css(\'height\',\''.$tamanho_div_menu.'px\');

						// acao para evitar problema para caso de lista vazia
						if( $("#extra_vazio") ){
							$("#extra_vazio").remove();
						}
					});
				</script>';

	$html = '<div id="carousel_div">';

	$bckground = $param['background-color'] != '' ? "background-color: {$param['background-color']};" : "";

	$html .= '<div class="div_barra_menu image_carousel" id='.$id.'_externo
				   style="'.$bckground.' width:'.($tamanho).'; height: '.round($tamanho_icone).'px padding: '.round($tamanho_icone/6).'px '.(round($tamanho_icone/2)+round($tamanho_icone/10)).'px '.round($tamanho_icone/6).'px '.(round($tamanho_icone/2)+round($tamanho_icone/10)).'px;">
				<div id="'.$id.'" />';
	if( is_array($param['icones']) ){
		foreach( $param['icones'] as $icone ){


				$html .= '<img src="'.$icone['url'].'"
							   title="'.$icone['titulo'].'"
							   class="'.$icone['class'].'"
							   id="'.$icone['id'].'"';

			if ( $icone['height'] )
			{
						$html .='
							   height="'.$icone['height'].'"';
			}
							$html .='
							   width="'.$tamanho_icone.'"
							   mnuid="'.$icone['mnuid'].'"
							   sisid="'.$icone['sisid'].'"
							   usucpf="'.$icone['usucpf'].'" />';

		}
	}else{ // acao para evitar problema para caso de lista vazia
		$html .= '<img id="extra_vazio" width="'.$tamanho_icone.'" height="'.$tamanho_icone.'"/>';
	}
	$var_iconTam = $tamanho_icone < 70 ? 70 : ( $tamanho_icone > 120 ? 120 : $tamanho_icone);
	$html .=   '</div>
				<div class="clearfix"></div>
				<a class="prev" id="'.$id.'_prev" href="#" style="width: '.round($var_iconTam/4).'px; height: '.round($var_iconTam/4).'px; margin-top: '.((round(($tamanho_icone*30)/45)-(75))).'px;"><span>prev</span></a>
				<a class="next" id="'.$id.'_next" href="#" style="width: '.round($var_iconTam/4).'px; height: '.round($var_iconTam/4).'px; margin-top: '.((round(($tamanho_icone*30)/45)-(75))).'px;"><span>next</span></a>
				<div class="pagination" id="'.$id.'_pag"></div>
			</div>';

	$html .= "</div>";

	echo $includes.$scripts.$html;

}


/**
 *
 */
function chamaAreaDeTrabalho( $params ){

	extract($params);

	$tamanhoIcone = ( $tamanhoIcone )? $tamanhoIcone : 90 ;

	$BarraFeramentas = new BarraFerramentas();

	// personalizado, a ser buscado de seguranca.barraferamentas
	echo '<div id="draggable_area">';

		// TODO: adicionar nessa busca as infromacoes (que serao adicionadas como atributos aos tags): usucpf, mnuid
		$param = Array('id'=>'pessoal');
		$param['icones'] = $BarraFeramentas->resgataListaDeIconesDaAba( $usucpf, $mnuid, $sisid );
		$param['elementosPorPagina'] = ( $elementosPorPagina )? $elementosPorPagina : 10 ;

		echo disponibilizaElementosEditavel( $param );

		$param = Array('id'=>'geral');
		$param['icones'] = $BarraFeramentas->resgataListaDeTodosIconesDaAba( $mnuid );

		echo disponibilizaTodosElementosEditavel( $param );

	echo '</div>';

	$arrayEditabilidade = array(
							'mnuid' => $mnuid,
							'usucpf' => $usucpf,
							'idDraggable' => 'geral',
							'idDropabble' => 'pessoal',
							'tamanho_icone' => $tamanhoIcone,
							'slideShows' => array('geral','pessoal'),
							'sisid' => $sisid,
							'elementosPorPagina' => $elementosPorPagina );
	aplicaEditabilidadeDragDropSlideShow( 'build', $arrayEditabilidade );
}

/**
 * Função que disponibiliza items de usuario - pessoal
 */
function disponibilizaElementosEditavel( $params ){

	$tamanho_icone  = $params['tamanho_icone'] 	? $params['tamanho_icone'] 	: 90;

	$html = "";

	$altura = ( (($params['elementosPorPagina']/10) * 110) < 110 )? 110 : (($params['elementosPorPagina']/10) * 110) ;
	$html .= "<div style='width:100%;height:".$altura."px !important;top:5%;' id='".$params['id']."'>";

	$paginaLoop = 1;
	$html .= "<div class='bloco' style='padding-top:10px' id='container_Pag_{$paginaLoop}' pagina='{$paginaLoop}'>";
	$contador = 0;
	if( $params['icones'][0]['url'] )
	foreach ($params['icones'] as $key => $value) {

		// paginacao
		if(
			$contador == $params['elementosPorPagina']
			&& count($params['icones']) >= $contador ){
			$paginaLoop++;
			$html .= '</div>';
			$html .= "<div class='bloco' id='container_Pag_{$paginaLoop}' pagina='{$paginaLoop}' style='display:none;padding-top:10px'>";
			$contador = 0;
		}

		$html .= "<div style='width:10%;height:150px;float:left;margin:0;padding:0;text-align:center;'>";
		$html .= '<img src="'.$value['url'].'"
					   title="'.$value['titulo'].'"
					   class="'.$value['class'].'"
					   id="'.$value['id'].'"
					   width="'.$tamanho_icone.'"
					   mnuid="'.$value['mnuid'].'"
					   sisid="'.$value['sisid'].'"
					   usucpf="'.$value['usucpf'].'" />';
		$html .= "</div>";

		// paginacao
		if( count($params['icones']) == $key+1 )
			$html .= '</div>';

		$contador++;
	}

	$html .= "</div>";
	echo $html;
}

/**
 * Função que disponibiliza todos items - geral
 */
function disponibilizaTodosElementosEditavel( $params ){

	$tamanho_icone  = $params['tamanho_icone'] 	? $params['tamanho_icone'] 	: 90;

	$html = "";

	$html .= "<div style='position:absolute;display:none;width:100%;height:100%;top:5%;' id='".$params['id']."'>";

	foreach ($params['icones'] as $key => $value) {

		$html .= "<div style='width:25%;height:150px;float:left;margin:0;padding:0;text-align:center;'>";
		$html .= '<img src="'.$value['url'].'"
						   title="'.$value['titulo'].'"
						   class="'.$value['class'].'"
						   id="'.$value['id'].'"
						   width="'.$tamanho_icone.'"
						   mnuid="'.$value['mnuid'].'"
						   sisid="'.$value['sisid'].'"
						   usucpf="'.$value['usucpf'].'" />';
		$html .= "</div>";

	}

	$html .= "</div>";
	echo $html;
}

/**
 * Função que inicia objeto de editabilidade de personalização do slideShow
 *
 * Parametros (params):
 *  mnuid
 *  usucpf
 *  idDraggable
 *  idDropabble
 *  tamanho_icone = 90
 *  slideShows
 *  sisid
 */
function aplicaEditabilidadeDragDropSlideShow( $method, $params ){

	if( $method == 'build' ){
		extract($params);
		echo "
			<script type=\"text/javascript\" src=\"../includes/jquery-ui/jquery-ui-1.8.20.custom.min.js\"></script>
			<script type=\"text/javascript\" src=\"../includes/JQuery/jquery.ba-outside-events.js\"></script>
			<script>
				$(document).ready(function(){
					var slideShows = ['".implode("','",$slideShows)."'];
					AreaDeTrabalho.init( '{$mnuid}', '{$usucpf}', '{$idDraggable}', '{$idDropabble}', '{$tamanho_icone}', slideShows, '{$sisid}', '{$elementosPorPagina}' );
				});
			</script>";
	}else{
		$BarraFerramentas = new BarraFerramentas();
		$BarraFerramentas->{$method}( $params );
	}
}

/**
 * Função que verifica se CPF que está operando o sistema é vávilo perante a receita
 *
 * @param string: $cpf
 */
function verificaUsuarioReceita($cpf)
{
    if (IS_PRODUCAO && $cpf) {
        $aDadosUsuario = recuperarUsuarioReceita($cpf);

        if(!$aDadosUsuario['usuarioexiste']){
            enviarEmailUsuarioInvalido($cpf, $aDadosUsuario['dados']);
            $_SESSION['MSG_AVISO'][] = 'O CPF do usuário não é válido';
            header('Location: /logout.php');
            die;
        }
    }
}

/**
 * Função que recupera os dados da receita de um CPF
 *
 * @param string: $cpf
 * @return array: array com 2 índices: 'usuarioexiste' (bool) e 'dados' (array com os dados)
 */
function recuperarUsuarioReceita($cpf)
{
    include_once APPRAIZ. 'www/includes/webservice/cpf.php';
    $objPessoaFisica = new PessoaFisicaClient("http://ws.mec.gov.br/PessoaFisica/wsdl");
    $xml = $objPessoaFisica->solicitarDadosPessoaFisicaPorCpf($cpf);

    $obj = (array) simplexml_load_string($xml);

    $pessoa   = (array) $obj['PESSOA'];
    $endereco = (array) $obj['PESSOA']->ENDERECOS->ENDERECO;
    $contato  = (array) $obj['PESSOA']->CONTATOS->CONTATO;

    if(array_key_exists('ERRO', $pessoa)){
        return array('usuarioexiste'=>false, 'dados'=>$pessoa['ERRO']);
    }

    foreach($pessoa as $k =>$val):
        if (ctype_upper($k)){continue;}
        $return[$k] = $val;
    endforeach;

    foreach($endereco as $k =>$val):
        if (ctype_upper($k)){continue;}
        $return[$k] = $val;
    endforeach;

    foreach($contato as $k =>$val):
        if (ctype_upper($k)){continue;}
        $return[$k] = $val;
    endforeach;

    return array('usuarioexiste'=>true, 'dados'=>$return);
}

function enviarEmailUsuarioInvalido($cpf, $mensagem = '')
{
    $remetente     = array("nome"=>SIGLA_SISTEMA, "email"=>EMAIL_SISTEMA_NOREPLY);
    $destinatarios = array($_SESSION['email_sistema']);

    $assunto = "Tentativa de entrar no sistema com CPF inválido";
    $mensagem = "<pre>Prezados,

Houve uma tentativa de entrar no sistema com CPF {$cpf}, porém o web-service com a receita rejeitou o mesmo com a mensagem: " . utf8_decode($mensagem) . "

Atenciosamente,
Equipe ". SIGLA_SISTEMA. ".
</pre>
";
    return enviar_email($remetente, $destinatarios, $assunto, $mensagem);
}

/* Função que ajusta a string baseada em um tamanho passado
 * - Se a string for maoir ela faz um substr para ficar com o tamanho correto;
 * - Se a string for do tamanho certo retorna a string;
 * - Se a string for menor retorna false;
 * */
function ajustaTamanhoString( $string, $tamanho ){

	switch (strlen($string)){
		case (strlen($string) > $tamanho):
			$retorno = substr($string, 0, $tamanho);
		break;
		case $tamanho:
			$retorno = $string;
		break;
		case (strlen($string) < $tamanho):
			$retorno = false;
		break;
	}

	return $retorno;
}

/* Função que filtra um array de strings ou uma string baseada em um tamanho
 * - Se for um array, roda a função ajustaTamanhoString para cada string e retorna um array;
 * - Se for uma string roda a função ajustaTamanhoString e retorna o resultado
 * */
function filtraTamanhoStrings( $arr, $tamanho ){

	if( is_array($arr) ){
		$temp = '';
		$retorno = Array();
		foreach( $arr as $string ){
			$temp = ajustaTamanhoString( $string, $tamanho );
			if( $temp != '' ){
				$retorno[] = $temp;
				$temp = '';
			}
		}
	}else{
		$retorno = ajustaTamanhoString( $arr, $tamanho );
	}
	return $retorno;
}


function comparaValorArray($array1,$array2, $tipoDeRetorno ){
	foreach($array1 as $chaveArr1=>$dadosArr1){
		if(in_array($dadosArr1,$array2) ){
			if($tipoDeRetorno == 'boolean'){
				return true;
			}

		}
	}
	if($tipoDeRetorno == 'boolean'){
		return false;
	}
}

function numeroCardinalOrdinal( $valor = 0, $maiusculas = false, $numerais = 'cardinal', $genero = 'mas') {

	if( $numerais == 'ordinal' ){
		if( $genero == 'fem' ){
			$c = array("", "centésima", "ducentésima", "tricentésima", "quadringentésima", "quingentésima", "sexcentésima", "septingentésima", "octingentésima", "nongentésima");
			$d = array("", "décima", "vigésima", "trigésima", "quadragésima", "quinquagésima", "sexagésima", "septuagésima", "octagésima", "nonagésima");
			$d10 = array("décima", "décimo primeira", "décimo segunda", "décimo terceira", "décimo quarta", "décimo quinta", "décimo sexta", "décimo sétima", "décimo oitava", "décimo nona");
			$u = array("", "primeira", "segunda", "terceira", "quarta", "quinta", "sexta", "sétima", "oitava", "nona");
		} else {
			$c = array("", "centésimo", "ducentésimo", "tricentésimo", "quadringentésimo", "quingentésimo", "sexcentésimo", "septingentésimo", "octingentésimo", "nongentésimo");
			$d = array("", "décimo", "vigésimo", "trigésimo", "quadragésimo", "quinquagésimo", "sexagésimo", "septuagésimo", "octagésimo", "nonagésimo");
			$d10 = array("décimo", "décimo primeiro", "décimo segundo", "décimo terceiro", "décimo quarto", "décimo quinto", "décimo sexto", "décimo sétimo", "décimo oitavo", "décimo nono");
			$u = array("", "primeiro", "segundo", "terceiro", "quarto", "quinto", "sexto", "sétimo", "oitavo", "nono");
		}

		$z = 0;
		$rt = "";

		$valor = number_format($valor, 2, ".", ".");
		$inteiro = explode(".", $valor);
		for($i=0;$i<count($inteiro);$i++)
			for($ii=strlen($inteiro[$i]);$ii<3;$ii++)
				$inteiro[$i] = "0".$inteiro[$i];

		$fim = count($inteiro) - ($inteiro[count($inteiro)-1] > 0 ? 1 : 2);
		for ($i=0;$i<count($inteiro);$i++) {
			$valor = $inteiro[$i];
			$rc = (($valor > 100) && ($valor < 200)) ? "centésimo" : $c[$valor[0]];
			$rd = ($valor[1] < 2) ? "" : $d[$valor[1]];
			$ru = ($valor > 0) ? (($valor[1] == 1) ? $d10[$valor[2]] : $u[$valor[2]]) : "";

			$r = $rc.(($rc && ($rd || $ru)) ? " " : "").$rd.(($rd &&
			$ru) ? " " : "").$ru;
			$t = count($inteiro)-1-$i;
			if ($valor == "000")$z++; elseif ($z > 0) $z--;
			if ($r) $rt = $rt . ((($i > 0) && ($i <= $fim) &&
			($inteiro[0] > 0) && ($z < 1)) ? ( ($i < $fim) ? ", " : " ") : "") . $r;
		}
	} else {
		$singular = array("centavo", "real", "mil", "milhão", "bilhão", "trilhão", "quatrilhão");
		$plural = array("centavos", "reais", "mil", "milhões", "bilhões", "trilhões", "quatrilhões");

		$c = array("", "cem", "duzentos", "trezentos", "quatrocentos", "quinhentos", "seiscentos", "setecentos", "oitocentos", "novecentos");
		$d = array("", "dez", "vinte", "trinta", "quarenta", "cinquenta", "sessenta", "setenta", "oitenta", "noventa");
		$d10 = array("dez", "onze", "doze", "treze", "quatorze", "quinze", "dezesseis", "dezesete", "dezoito", "dezenove");
		$u = array("", "um", "dois", "três", "quatro", "cinco", "seis", "sete", "oito", "nove");

		$z = 0;
		$rt = "";

		$valor = number_format($valor, 2, ".", ".");
		$inteiro = explode(".", $valor);
		for($i=0;$i<count($inteiro);$i++)
			for($ii=strlen($inteiro[$i]);$ii<3;$ii++)
				$inteiro[$i] = "0".$inteiro[$i];

		$fim = count($inteiro) - ($inteiro[count($inteiro)-1] > 0 ? 1 : 2);
		for ($i=0;$i<count($inteiro);$i++) {
			$valor = $inteiro[$i];
			$rc = (($valor > 100) && ($valor < 200)) ? "cento" : $c[$valor[0]];
			$rd = ($valor[1] < 2) ? "" : $d[$valor[1]];
			$ru = ($valor > 0) ? (($valor[1] == 1) ? $d10[$valor[2]] : $u[$valor[2]]) : "";

			$r = $rc.(($rc && ($rd || $ru)) ? " e " : "").$rd.(($rd &&
			$ru) ? " e " : "").$ru;
			$t = count($inteiro)-1-$i;
			$r .= $r ? " ".($valor > 1 ? $plural[$t] : $singular[$t]) : "";
			if ($valor == "000")$z++; elseif ($z > 0) $z--;
			if (($t==1) && ($z>0) && ($inteiro[0] > 0)) $r .= (($z>1) ? " de " : "").$plural[$t];
			if ($r) $rt = $rt . ((($i > 0) && ($i <= $fim) &&
			($inteiro[0] > 0) && ($z < 1)) ? ( ($i < $fim) ? ", " : " e ") : " ") . $r;
		}
	}
	if(!$maiusculas){
		return($rt ? $rt : "zero");
	} else {
		if ($rt) $rt = str_replace(" E ", " e ", ucwords($rt));
		return (($rt) ? ($rt) : "Zero");
	}
}

/**
 * Remove todos os em excessos entre as palavras e também espaços no começo e no final da texto.
 *
 * @param string $texto Texto informado para eliminar espaços duplos
 * @return string Texto corrigido sem espaços em excesso no meio das palavras, no começo e no fim
 */
function removerEspacoDuplicado($texto){
    # Primeiramente remove todos espaços no começo e no final da string
    $textoSemEspacoInicioFim = trim($texto);
    $textoEspacoDuplo = preg_replace('/\s(?=\s)/', '', $textoSemEspacoInicioFim);
    $textoFinal = preg_replace('/[\n\r\t]/', ' ', $textoEspacoDuplo);
    return $textoFinal;
}

function intervaloEntreDatas($dataini, $datafim) {
	# Split para dia, mes, ano, hora, minuto e segundo da data inicial
	$_split_datehour = explode(' ',$dataini);
	$_split_data = explode("/", $_split_datehour[0]);
	$_split_hour = explode(":", $_split_datehour[1]);
	# Coloquei o parse (integer) caso o timestamp nao tenha os segundos, dai ele fica como 0
	$dtini = mktime ($_split_hour[0], $_split_hour[1], (integer)$_split_hour[2], $_split_data[1], $_split_data[0], $_split_data[2]);

	# Split para dia, mes, ano, hora, minuto e segundo da data final
	$_split_datehour = explode(' ',$datafim);
	$_split_data = explode("/", $_split_datehour[0]);
	$_split_hour = explode(":", $_split_datehour[1]);
	$dtfim = mktime ($_split_hour[0], $_split_hour[1], (integer)$_split_hour[2], $_split_data[1], $_split_data[0], $_split_data[2]);

	# Diminui a datafim que é a maior com a dataini
	$time = ($dtfim - $dtini);

	# Recupera os dias
	$days  = floor($time/86400);
	# Recupera as horas
	$hours = floor(($time-($days*86400))/3600);
	# Recupera os minutos
	$mins  = floor(($time-($days*86400)-($hours*3600))/60);
	# Recupera os segundos
	$secs  = floor($time-($days*86400)-($hours*3600)-($mins*60));

	# Monta o retorno no formato
	# 5d 10h 15m 20s
	# somente se os itens forem maior que zero
	/* $retorno  = "";
	$retorno .= ($days>0)  ?  $days .' dia ' : ""  ;
	$retorno .= ($hours>0) ?  str_pad($hours, 2, '0', STR_PAD_LEFT) .':': ""  ;
	$retorno .= ($mins>0)  ?  str_pad($mins, 2, '0', STR_PAD_LEFT) .':' : ""  ;
	$retorno .= ($secs>0)  ?  str_pad($secs, 2, '0', STR_PAD_LEFT) .'' : ""  ; */

	$retorno = $days.'d '.str_pad($hours, 2, '0', STR_PAD_LEFT).':'.str_pad($mins, 2, '0', STR_PAD_LEFT).':'.str_pad($secs, 2, '0', STR_PAD_LEFT);

	return $retorno;
}

/**
 * Carrega os sistemas que os usuarios com os cpf passado tem acesso.
 *
 * @global object $db
 * @return array
 */

function carregarSistemas()
{
	global $db;

	$sql = sprintf(
		"SELECT s.sisid, trim(s.sisabrev) as sisabrev, trim(s.sisdsc) as sisdsc
			   FROM seguranca.usuario u
         INNER JOIN seguranca.perfilusuario pu USING ( usucpf )
         INNER JOIN seguranca.perfil p ON pu.pflcod = p.pflcod
         INNER JOIN seguranca.sistema s ON p.sisid = s.sisid
         INNER JOIN seguranca.usuario_sistema us ON s.sisid = us.sisid AND u.usucpf = us.usucpf
              WHERE u.usucpf = '%s'
                AND us.suscod = 'A'
            	AND p.pflstatus = 'A'
            	AND s.sisstatus = 'A'
            	AND u.suscod = 'A'
           GROUP BY s.sisid, s.sisabrev, s.sisdsc
           ORDER BY s.sisabrev", $_SESSION['usucpf']
	);

	if (isset($_SESSION['sistemas'][$_SESSION['usucpf']]))
	{
		$sistemas = $_SESSION['sistemas'][$_SESSION['usucpf']];
	}
	else
	{
		$resultado = $db->carregar($sql);

		$sistemas = array();

        if($resultado){
            foreach ($resultado as $sistema)
            {
                $sistemas[$sistema['sisid']] = $sistema;
            }

            $_SESSION['sistemas'][$_SESSION['usucpf']] = $sistemas;
        } else {
//            header('Location: /logout.php');
//            die;
        }
	}

	return $sistemas;
}

function carregarUsuariosSistema($simular = null)
{
	global $db;

	$where = '';

	if ($_POST['nomeSimularUsuario'])
	{
        if(14 == strlen($_POST['nomeSimularUsuario'])){
            $_POST['nomeSimularUsuario'] = str_replace(array('.', '-'), '', $_POST['nomeSimularUsuario']);
        }

		if (!empty($_POST['nomeSimularUsuario']) && is_numeric($_POST['nomeSimularUsuario']))
		{
			$sqlCpf = "OR u.usucpf like '%" . str_to_upper(corrige_cpf($_POST['nomeSimularUsuario'])) . "%'";
		} else {
			$sqlCpf = '';
		}

		$where .= " AND (TRANSLATE(u.usunome, 'áàãâéêíóôõúüçÁÀÃÂÉÊÍÓÔÕÚÜÇ','aaaaeeiooouucAAAAEEIOOOUUC') ilike TRANSLATE('%" . $_POST['nomeSimularUsuario'] . "%', 'áàãâéêíóôõúüçÁÀÃÂÉÊÍÓÔÕÚÜÇ','aaaaeeiooouucAAAAEEIOOOUUC')
		            OR TRANSLATE(u.usuemail, 'áàãâéêíóôõúüçÁÀÃÂÉÊÍÓÔÕÚÜÇ','aaaaeeiooouucAAAAEEIOOOUUC') ilike TRANSLATE('%" . $_POST['nomeSimularUsuario'] . "%', 'áàãâéêíóôõúüçÁÀÃÂÉÊÍÓÔÕÚÜÇ','aaaaeeiooouucAAAAEEIOOOUUC')
		            {$sqlCpf})";
	}

	if (is_array($_POST['perfisSimularUsuario']))
	{
		$where .= " AND p.pflcod IN (" . implode(',', $_POST['perfisSimularUsuario']) . ") ";
	}

	$sql = "select distinct u.usucpf as codigo, u.usucpf as cpf, u.usunome as descricao , u.usuemail as email
			  from seguranca.usuario u
		inner join seguranca.perfilusuario pf on pf.usucpf = u.usucpf
		inner join seguranca.perfil p on p.pflcod = pf.pflcod and	p.sisid = " . $_SESSION['sisid'] . "
		inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf and	us.sisid = p.sisid
		     where us.suscod = 'A' and p.pflnivel >= (
            select min(pflnivel) from seguranca.perfil
        inner join seguranca.perfilusuario on perfil.pflcod = perfilusuario.pflcod
             where perfilusuario.usucpf = '" . $_SESSION['usucpforigem'] . "'
           	   and perfil.sisid = " . $_SESSION['sisid'] . " )
               and u.usucpf not in ('{$_SESSION['usucpforigem']}')
               $where
               order by u.usunome";



	if (isset($_SESSION['sistemas']['usuario'][$_SESSION['sisid']]) && simec_trim($_SESSION['sistemas']['usuario'][$_SESSION['sisid']]) != '' && !$simular)
	{
		$usuariosSistema = $_SESSION['sistemas']['usuario'][$_SESSION['sisid']];
	}
	else
	{
		$usuariosSistema = $db->carregar($sql);
		$_SESSION['sistemas']['usuario'][$_SESSION['sisid']] = $usuariosSistema;
	}
	return $usuariosSistema;
}

/**
 * Recupera um array com os menus e seus itens de menus de maneira recursivamente.
 *
 * @global object $db - class
 * @param integer $sisId
 * @param string $cpf
 * @return array $menus
 */
function carregarMenus($sisId, $cpf)
{
	global $db;

	// Carregando o menu e itens de menu deste sistema e dos usuarios especificos.
	$lista_cpf = "'" . $cpf . "'";

	$sql = "select distinct mnu.mnucod,mnu.mnuid , mnu.mnuidpai , mnu.mnudsc , mnu.mnustatus , mnu.mnulink ,
                   mnu.mnutipo , mnu.mnustile , mnu.mnuhtml , mnu.mnusnsubmenu , mnu.mnutransacao , mnu.mnushow , mnu.abacod
                   from seguranca.menu mnu, seguranca.perfilmenu pmn, seguranca.perfil pfl, seguranca.perfilusuario pfu
             where mnu.mnuid=pmn.mnuid and pmn.pflcod=pfl.pflcod and pfl.pflcod=pfu.pflcod
               and pfu.usucpf in (" . $lista_cpf . ") and ( mnu.mnutipo=1 or mnu.mnuidpai is not null )  and mnu.mnushow='t' and mnu.mnustatus='A'
               and mnu.sisid=" . $sisId . "
          order by mnu.mnucod,mnu.mnuid , mnu.mnuidpai , mnu.mnudsc";

	if (isset($_SESSION['sistemas']['menu'][$sisId]))
	{
		$menusBd = $_SESSION['sistemas']['menu'][$sisId];
	}
	else
	{
		$menusBd = @$db->carregar($sql);
		$_SESSION['sistemas']['menu'][$sisId] = $menusBd;
	}

	$menus = array();

	if (is_array($menusBd))
	{
		foreach ($menusBd as $key => &$menuBd)
		{
			if (empty($menuBd['mnuidpai']))
			{
				$menus[$menuBd['mnuid']] = $menuBd;
				unset($menusBd[$key]);
				$menus[$menuBd['mnuid']]['itensMenu'] = tratarMenus($menusBd, $menuBd);
			}
		}
	}

	return $menus;
}

function tratarMenus($menusBd, $menuPaiBd, $key = null)
{
	$menus = array();

	foreach ($menusBd as $key => $menuBd)
	{
		if (!empty($menuBd['mnuidpai']) && $menuPaiBd['mnuid'] == $menuBd['mnuidpai'])
		{
			$menus[$menuBd['mnuid']] = $menuBd;
			unset($menusBd[$key]);
			$menus[$menuBd['mnuid']]['itensMenu'] = tratarMenus($menusBd, $menuBd, $key);
		}
	}

	return $menus;
}

/**
 * Gera o html do menu e chamando a funcao de gerar os itens menus recursivamente de maneira infitina.
 *
 * @name menuHtml
 * @return string
 *
 * @author Ruy Junior Ferreira Silva <ruy.silva@mec.com>
 * @since 02/09/2013
 */
function menuHtml()
{
	$menus = carregarMenus($_SESSION['sisid'], $_SESSION['usucpf']);
	$menusHtml = '';

	foreach ($menus as $menu)
	{
		if (empty($menu['mnuidpai']))
		{
			if ($menu['itensMenu'] && count($menu['itensMenu']) > 0)
			{
				$menusHtml .= '<li class="dropdown">';
				$menusHtml .= '<a class="dropdown-toggle" href="#" data-toggle="dropdown">';
				$menusHtml .= $menu['mnuhtml'];
				$menusHtml .= '<b class="caret"></b>';
				$menusHtml .= '</a>';
				$menusHtml .= '<ul class="dropdown-menu">';
				$menusHtml .= menuItemHtml($menu['itensMenu']);
				$menusHtml .= '</ul>';
				$menusHtml .= '</li>';
			}
			else
			{
				$menusHtml .= '<li>';
				$menusHtml .= '<a href="#">';
				$menusHtml .= $menu['mnuhtml'];
				$menusHtml .= '</a>';
				$menusHtml .= '</li>';
			}

			$menusHtml .= "";
		}
	}

	return $menusHtml;
}

/**
 * Monta o html de itens e chama ele mesmo caso tenha filhos.
 * De maneira recursivamente chamando ele mesmo e montado o html de todos os itens de menu.
 *
 * @param array $itensMenu - Array com os itens de menu.
 * @return string
 *
 * @author Ruy Junior Ferreira Silva <ruy.silva@gmail.com>
 * @since 03/09/2013
 */
function menuItemHtml($itensMenu)
{
	$menusHtml = '';

	foreach ($itensMenu as $menu)
	{
		if ($menu['itensMenu'] && count($menu['itensMenu']) > 0)
		{
			$url = ($menu['mnulink']) ? $menu['mnulink'] : '#';

			$menusHtml .= '<li class="dropdown-submenu">';
			$menusHtml .= '<a href="' . $url . '" tabindex="-1">' . $menu['mnutransacao'];
			$menusHtml .= '<ul class="dropdown-menu">';
			$menusHtml .= menuItemHtml($menu['itensMenu']);
			$menusHtml .= '</ul>';
			$menusHtml .= '</a>';
			$menusHtml .= '</li>';
		}
		else
		{
			$url = ($menu['mnulink']) ? $menu['mnulink'] : '#';
			$menusHtml .= '<li>';
			$menusHtml .= '<a href="' . $url . '">' . $menu['mnutransacao'] . '</a>';
			$menusHtml .= '</li>';
		}
	}

	return $menusHtml;
}

function montaMenuBootstrap($menus = array()) {
	$html = '<ul class="nav metismenu" id="side-menu">';

	$arrayIcons = array(
            'Principal' => 'fa fa-th-large',
            'Relatórios' => 'fa fa-line-chart',
            'Sistema' => 'fa fa-user-circle',
            'Configuração' => 'fa fa-gears',
            'Apoio' => 'fa fa-cubes'
	);

	foreach ($menus as $nivel1) {
		if (count($nivel1['itensMenu']) > 0) {
			$itens.= '	<li>';
			$itens.= '		<a href="#' . $nivel1['mnudsc'] . '">';
			$itens.= '			<i class="' . (isset($arrayIcons[$nivel1['mnudsc']]) ? $arrayIcons[$nivel1['mnudsc']] : 'fa fa-th-large') . '"></i> ';
			$itens.= '			<span class="nav-label">' . $nivel1['mnudsc'] . '</span>';
			$itens.= '			<span class="fa arrow"></span>';
			$itens.= '		</a>';
			$itens.= '		<ul class="nav nav-second-level">';

			foreach ($nivel1['itensMenu'] as $nivel2) {
				if (count($nivel2['itensMenu']) > 0) {
					$itens.= '	<li>';
					$itens.= '		<a href="#' . $nivel2['mnudsc'] . '">';
					$itens.= '			<i class="fa arrow"></i>' . $nivel2['mnudsc'];
					$itens.= '		</a>';
					$itens.= '		<ul class="nav nav-third-level">';

					foreach ($nivel2['itensMenu'] as $nivel3) {
						if (count($nivel3['itensMenu']) > 0) {
							$itens.= '	<li>';
							$itens.= '		<a href="#' . $nivel3['mnudsc'] . '">';
							$itens.= '			<i class="fa arrow"></i>' . $nivel3['mnudsc'];
							$itens.= '		</a>';
							$itens.= '		<ul class="nav nav-fourth-level">';

							foreach ($nivel3['itensMenu'] as $nivel4) {
								$itens.= '		<li>';
								$itens.= '			<a href="' . $nivel4['mnulink'] . '">' . $nivel4['mnudsc'] . '</a>';
								$itens.= '		</li>';
							}

							$itens.= '		</uL>';

						} else {
							$itens.= '		<li>';
							$itens.= '			<a href="' . $nivel3['mnulink'] . '">' . $nivel3['mnudsc'] . '</a>';
							$itens.= '		</li>';
						}
					}

					$itens.= '		</uL>';

				} else {
					$itens.= '		<li>';
					$itens.= '			<a href="' . $nivel2['mnulink'] . '">' . $nivel2['mnudsc'] . '</a>';
					$itens.= '		</li>';
				}
			}

			$itens.= '		</uL>';

		} else {
			$itens.= '		<li>';
			$itens.= '			<a href="#' . $nivel1['mnudsc'] . '">';
			$itens.= '			<i class="fa ' . (isset($arrayIcons[$nivel1['mnudsc']]) ? $arrayIcons[$nivel1['mnudsc']] : 'fa fa-th-large') . '"></i> ';
			$itens.= '			<span class="nav-label">' . $nivel1['mnudsc'] . '</span>';
			$itens.= '			</a>';
			$itens.= '		</li>';
		}
	}

	$html.= $itens;

	$html .= '</ul>';

	return $html;
}

/**
 * @param mixed $mData
 * @param boolean $bExit
 */
function dump( $mData, $bExit = false )
{
	$oTrace = new BackTrace();
	$oTrace->levelDown();
	$sTrace = $oTrace->explain();
	?>
	<fieldset>
		<legend>Dump</legend>
		<pre><?php var_dump( $mData ); ?></pre>
		<?php echo $sTrace; ?>
	</fieldset>
	<?php
	if ( $bExit === true )
	{
		echo '<pre style="color: red">Interrupted execution in file ' . __FILE__ . ' in line ' .( __LINE__ + 1 ) . '.</pre>';
		exit();
	}
}

/**
 * Busca todos os usuários de um perfil especifico ou sistema.
 * 
 * @global cls_banco $db
 * @param stdClass $filtro
 * @return boolean/array
 */
function buscarUsuarioPerfilPlanejamento(stdClass $filtro){
    global $db;
    $where = '';
//    157 SPOA - Planejamento Orçamentario
    $where .= $filtro->sisid? " \n AND s.sisid = {$filtro->sisid}": NULL;
//    956 Planejamento
    $where .= $filtro->pflcod? " \n AND pu.pflcod = {$filtro->pflcod}": NULL;
    $where .= $filtro->ungcod? " \n AND ur.ungcod = '{$filtro->ungcod}'": NULL;
    $sql = "
        SELECT DISTINCT
            u.usucpf,
            u.usunome,
            u.usuemail,
            CASE WHEN LENGTH(u.usufoneddd) > 0 THEN
                '(' || u.usufoneddd || ')' || u.usufonenum
            ELSE
                NULL
            END AS telefone
        FROM seguranca.sistema s
            JOIN seguranca.usuario_sistema us USING(sisid)
            JOIN seguranca.usuario u USING(usucpf)
            JOIN seguranca.perfilusuario pu USING(usucpf)
            JOIN seguranca.perfil p ON(pu.pflcod = p.pflcod AND p.sisid = s.sisid)
            LEFT JOIN planacomorc.usuarioresponsabilidade ur ON(u.usucpf = ur.usucpf AND ur.pflcod = p.pflcod AND ur.rpustatus = 'A')
        WHERE
            us.suscod = 'A'
            AND u.suscod = 'A'
            AND p.pflstatus = 'A' $where
    ";
//ver($sql, d);
    $listaUsuarioPlanejamento = $db->carregar($sql);
    return $listaUsuarioPlanejamento;
}

/**
 * Busca todos os usuários de um perfil especifico ou sistema.
 * 
 * @global cls_banco $db
 * @param stdClass $filtro
 * @return boolean/array
 */
function buscarUsuarioPerfilEmendas(stdClass $filtro){
    global $db;
    $where = '';
    $where .= $filtro->sisid? " \n AND s.sisid = {$filtro->sisid}": NULL;
    $where .= $filtro->pflcod? " \n AND pu.pflcod = {$filtro->pflcod}": NULL;
    $where .= $filtro->ungcod? " \n AND ur.ungcod = '{$filtro->ungcod}'": NULL;
    $sql = "
        SELECT DISTINCT
            u.usucpf,
            u.usunome,
            u.usuemail,
            CASE WHEN LENGTH(u.usufoneddd) > 0 THEN
                '(' || u.usufoneddd || ')' || u.usufonenum
            ELSE
                NULL
            END AS telefone
        FROM seguranca.sistema s
            JOIN seguranca.usuario_sistema us USING(sisid)
            JOIN seguranca.usuario u USING(usucpf)
            JOIN seguranca.perfilusuario pu USING(usucpf)
            JOIN seguranca.perfil p ON(pu.pflcod = p.pflcod AND p.sisid = s.sisid)
            LEFT JOIN emendas.usuarioresponsabilidade ur ON(
                u.usucpf = ur.usucpf
                AND ur.pflcod = p.pflcod
                AND ur.rpustatus = 'A')
        WHERE
            us.suscod = 'A'
            AND u.suscod = 'A'
            AND p.pflstatus = 'A'
            $where
    ";
//ver($sql, d);
    $listaUsuarioPlanejamento = $db->carregar($sql);
    return $listaUsuarioPlanejamento;
}

/**
 * Busca lista de ids de enquadramentos que estão configurados pra terem formulário
 * de Pi/Pre-pi reduzidos.
 * 
 * @global object $db
 * @param int $exercicio
 * @return array
 */
function buscarListaCodigoEnquadramentoReduzido($exercicio) {
    $lista = array();
    global $db;
    $sql = "
        SELECT
            eqdid
        FROM monitora.pi_enquadramentodespesa
        WHERE
            eqdstatus = 'A'
            AND eqdstreduzido = 'S'
            AND eqdano = '". (int)$exercicio. "'
    ";
    
    $listaConsulta = $db->carregar($sql);
    if($listaConsulta){
        foreach($listaConsulta as $interacao => $enquadramento){
            $lista[] = $enquadramento['eqdid'];
        }
    }
    
    return $lista;
}

function buscarCodigoProdutoNaoAplica($exercicio) {
    global $db;

    $sql = "
        SELECT
            pprid AS codigo
        FROM monitora.pi_produto
        WHERE
            prsano = '". (int)$exercicio. "'
            AND pprstatus = 'A'
            AND tipo = 'N'
    ";
    
    $codigo = $db->pegaUm($sql);
    return $codigo;
}

/**
 * Recebe o caractere que representa o sexo e retorna uma string com a descrição Masculino ou Feminino.
 * 
 * @param string $sexo 'M', 'F' ou Vazio
 * @return string
 */
function retornarDescricaoSexo($sexo){
    $descricao = '';
    if($sexo == 'M'){
        $descricao = 'Masculino';
    }
    
    if($sexo == 'F'){
        $descricao = 'Feminino';
    }
    
    return $descricao;
}

/**
 * Recebe o caractere que representa o sexo e retorna uma string com a descrição Base ou Oposição.
 * 
 * @param string $lado 'B', 'O' ou Vazio
 * @return string
 */
function retornarDescricaoLadoPartido($lado){
    $descricao = '';
    if($lado == 'B'){
        $descricao = 'Base';
    }
    
    if($lado == 'O'){
        $descricao = 'Oposição';
    }
    
    return $descricao;
}

/**
 * (PHP 4, PHP 5, PHP 7)<br/>
 * Alias of <b>implode</b>
 * @link http://php.net/manual/en/function.join.php
 * @param string $glue
 * @param array $pieces
 * @return string
 */
function join_simec($glue, $pieces){
    $resultado = '';
    if($pieces){
        foreach($pieces as $item => $valor){
            if(!empty($valor)){
                $resultado .= $valor. $glue;
            }
        }
        $resultado = substr($resultado, 0, -1);
    }

    return $resultado;
}

/**
 * Verifica se o primeiro valor é superior ao segundo e formata a cor pra vermelho.
 * 
 * @param string $valor1
 * @param string $valor2
 * @return string
 */
function marcarVermelhoValorSuperior($valor1 = NULL, $valor2 = NULL, $decimal = 0){
    $resultado = number_format($valor1, $decimal, ',', '.');

    if($valor1 > $valor2){
        $resultado = '<span class="validateRedText">'. number_format($valor1, $decimal, ',', '.'). '</span>';
    }
    
    return $resultado;
}

/**
 * Verificando IP de origem da requisição é autorizado para executar os SCRIPTS VIA PHPCLI.
 * Caso o ip não seja autorizado o sistema para a execução do arquivo.
 * 
 * return VOID
 */
function controlarExecucaoScript(){
    # Verificando IP de origem da requisição via PHPCLI
    $host = gethostname();
    $ip = gethostbyname($host);
    
    # Verificando IP de origem da requisição é autorizado
    if(IP_CLI_AUTORIZADO != $ip){
        # Finaliza a execução do SCRIPT
        die;
    }

}

/**
 * Verifica se a sessão não expirou, se tiver expirada envia pra tela de login.
 * 
 * return VOID
 */
function controlarAcessoSemAutenticacao(){
    # Verifica se o CPF do usuário autenticado foi gravado na sessao.
    if(!$_SESSION['usucpf']) {
        header( "Location: ". URL_SISTEMA);
        die();
    }
}

/**
 * Retira valores vazios do array. Correção de bug de envio do elemento do formulário pelo componente chosen
 * 
 * @param array $lista
 */
function retirarValorVazioArray($lista){
    $listaResultado = array();
    if($lista){
        foreach($lista as $valor){
            if($valor && $valor !== '0' && $valor !== 0){
                $listaResultado[] = $valor;
            }
        }
    }
    
    return $listaResultado;
}