<?php 
require("class.xml.inc"); ?>
<?php
/**
* class of manipulation of files xml and sgbd
*
* @author    			olavo alexandrino <oalexandrino@yahoo.com.br> - 2004
* @originalauthor		ricardo costa <ricardo.community@globo.com>   - 2002
* @based				mysql to xml - xml to mysql - <http://www.phpclasses.org/browse/package/782.html>
* @require				class xmlfile <olavo alexandrino version> - <orignal version: http://www.phpclasses.org/browse/package/79.html>
		- function commentary to strtoupper of the methods:
			1. add_attribute
			2. set_name
* @require				adodb database library for php <http://php.weblogs.com/adodb#downloads>
*/

class adodb_xml 
{
	/**
	*	object representation:  file xml
	*
	*	@type		objcet
	*	@access		public
	*/
	//$xml = null;

	/**
	*	creating the members
	*
	*	@param		string		version of file xml
	*	@param		string		codification to be used	
	*	@access		public
	*/
	function adodb_xml($version = "1.0", $encoding = "UTF-8") //version="1.0" encoding="utf-8"
	{
	  $this->xml = new xmlfile($version, $encoding);
	}
	
   function ImportaXML($dbconnection, $filename, $tablename,$abrev,$base)
	{
           // verificar se o registro já existe na tabela. Se existir, então não faz nada. Se não existir, então altera
      ini_set("memory_limit","512M");
      set_time_limit(0);
	  //$xml_file = fopen(APPRAIZ."xml/importacao/base".$base.'/'.$_SESSION['exercicio'].'/'.$filename, "r" );
	$xml_file = fopen(APPRAIZ."www/xml/importacao/base".$base.'/'.$_SESSION['exercicio'].'/'.$filename, "r" );	

	  $this->xml->read_file_handle($xml_file);
	  $numrows = $this->xml->roottag->num_subtags();
	  //	print 'registros='.$numrows.'<br>';
	  		  
      $resultado = pg_query('begin transaction; ');
	  for ($i = 0; $i < $numrows; $i++)
	  {
        //set_time_limit(5);
		$ok = true;
		$arrfields = null;
		$arrvalues = null;
        $row = $this->xml->roottag->tags[$i];
		$numfields = $row->num_subtags();
        for ($ii = 0; $ii < $numfields; $ii++)
		{
            $field = $row->tags[$ii];
		    $arrfields[] = $field->name;
		    if ("'".$field->cdata."'"=="''") $arrvalues[]="null" ;
			  else $arrvalues[] = "'".addslashes(utf8_decode($field->cdata))."'";
        }

        $sql="select * from $tablename where ".$abrev."cod=".$arrvalues[0];

        if ($tablename=='ppalocalizador')
             $sql = "select * from ppalocalizador where loccodppa=".$arrvalues[2];

        if ($tablename=='orgao' and $base=='ppa')
        {
          $sql = "select * from orgao where organo='".$_SESSION['exercicio']."' and orgcod=".$arrvalues[0];
          $arrfields[] = 'organo';
          $arrvalues[] =$_SESSION['exercicio'];
        }
        //if ($tablename=='orgao' and $base=='sigplan') $sql = "select * from orgao where organo=".$arrvalues[0]." and orgcod=".$arrvalues[1];
        if ($tablename=='orgao' and $base=='sigplan') $sql = "select * from orgao where orgcod=".$arrvalues[1];        
        if ($tablename=='unidade' and $base=='sigplan') $sql= "select * from unidade where unicod=".$arrvalues[1]." and unitpocod=".$arrvalues[2];        
        // if ($tablename=='unidade' and $base=='sigplan') $sql= "select * from unidade where unicod=".$arrvalues[1]." and uniano=".$arrvalues[0]." and unitpocod=".$arrvalues[2];        
        if ($tablename=='programa' and $base=='sigplan') $sql= "select * from programa where prgcod=".$arrvalues[1]." and prgano=".$arrvalues[0];

        //if ($tablename=='ppaprograma_orcamento' and $base=='ppa') $sql= "select * from elabrev.ppaprograma_orcamento where prgcod=".$arrvalues[1]." and prgano=".$arrvalues[0];
        
        
        if ($tablename=='indicador' and $base=='sigplan') $sql= "select * from indicador where prgano=".$arrvalues[0]." and prgcod=".$arrvalues[1]." and indnum=".$arrvalues[2];
        if ($tablename=='restricaoprograma' and $base=='sigplan') $sql= "select * from restricaoprograma where prgano=".$arrvalues[0]." and prgcod=".$arrvalues[1]." and rspnum=".$arrvalues[2];

        if ($tablename=='planejamento.acao' and $base=='sigplan') {
          $sql= "select acacod from planejamento.acao where prgano=".$arrvalues[0]." and prgcod=".$arrvalues[1]." and acacod=".$arrvalues[2]." and saccod=".$arrvalues[3]." and loccod=".$arrvalues[4];
        }
        if ($tablename=='elabrev.ppaindicador' and $base=='ppa') $sql = "select * from elabrev.ppaindicador where indnum=".$arrvalues[1];
        
        if ($tablename=='elabrev.ppaindicador_orcamento' and $base=='ppa') $sql = "select * from elabrev.ppaindicador_orcamento where indnum=".$arrvalues[1];        
           
        if ($tablename=='elabrev.ppaacao' and $base=='ppa') $sql = "select * from elabrev.ppaacao where acacodppa=".$arrvalues[1]." and prgcod=".$arrvalues[0]." and prsano='".$_SESSION['exercicio']."'"; 

        if ($tablename=='elabrev.ppaacao_orcamento' and $base=='ppa') $sql = "select * from elabrev.ppaacao_orcamento where acacodppa=".$arrvalues[1]." and prgcod=".$arrvalues[0]." and prgano='".$_SESSION['exercicio']."'";        
		if ($tablename=='elabrev.ppaacao_orcamento' and $base=='ppa') $sql = "select * from elabrev.ppaacao_orcamento where acacodppa=".$arrvalues[1]." and prgcod=".$arrvalues[0]." and prgano='".$_SESSION['exercicio']."'";        
        
        if ($tablename=='elabrev.ppalocalizador' and $base=='ppa') $sql = "select * from elabrev.ppalocalizador where acacodppa=".$arrvalues[1]." and prgcod=".$arrvalues[0]." and  loccodppa=".$arrvalues[2]." and prsano='".$_SESSION['exercicio']."'";            
       
   		$fields = join($arrfields, ", ");
		$values = join($arrvalues, ", ");
		unset($res);
        if ($tablename=='monitora.dadofisico')
		  {
             $sql = "select * from monitora.dadofisico where prgano='".$_SESSION['exercicio']."' and prgcod=".$arrvalues[1]." and acacod=".$arrvalues[2]." and saccod=".$arrvalues[3]." and regcod=".$arrvalues[4];
		  }
        if ($tablename=='monitora.dadofinanceiro')
		  {
             $sql = "select * from monitora.dadofinanceiro where prgano='".$_SESSION['exercicio']."' and prgcod=".$arrvalues[1]." and acacod=".$arrvalues[2]." and saccod=".$arrvalues[3]." and fppcod=".$arrvalues[4]." and regcod=".$arrvalues[5];	
		  }
		  
        if ($tablename=='elabrev.dadofinanceiro')
		  {
             $sql = "select * from elabrev.dadofinanceiro where prsano='".$_SESSION['exercicio']."' and prgcod=".$arrvalues[0]." and acacodPPA=".$arrvalues[1]." and LOCCodPPA=".$arrvalues[2]." and fppcod=".$arrvalues[3]." and NATCod=".$arrvalues[4]. " and tdpcod=".$arrvalues[5];
		  }	
		  
        if ($tablename=='elabrev.dadofisico')
		  {
             $sql = "select * from elabrev.dadofisico where prsano='".$_SESSION['exercicio']."' and prgcod=".$arrvalues[0]." and acacodPPA=".$arrvalues[1]." and LOCCodPPA=".$arrvalues[2];
          //  print "estou no registro=$i<br>$sql<br>";
		  }			  
		  
        if ($tablename=='elabrev.dadofinanceiro_orcamento')
		  {
             $sql = "select * from elabrev.dadofinanceiro_orcamento where prgano='".$_SESSION['exercicio']."' and prgcod=".$arrvalues[0]." and acacodPPA=".$arrvalues[1]." and LOCCodPPA=".$arrvalues[2]." and fppcod=".$arrvalues[3]." and NATCod=".$arrvalues[4]. " and tdpcod=".$arrvalues[5];
		  }	
		  
        if ($tablename=='elabrev.dadofisico_orcamento')
		  {
             $sql = "select * from elabrev.dadofisico_orcamento where prgano='".$_SESSION['exercicio']."' and prgcod=".$arrvalues[0]." and acacodPPA=".$arrvalues[1]." and LOCCodPPA=".$arrvalues[2];
          //  print "estou no registro=$i<br>$sql<br>";
		  }		  

        if ($tablename=='elabrev.ppalocalizador_orcamento')
		  {
             $sql = "select * from elabrev.ppalocalizador_orcamento where prgano='".$_SESSION['exercicio']."' and prgcod=".$arrvalues[0]." and acacodPPA=".$arrvalues[1]." and LOCCodPPA=".$arrvalues[2]." and regcod=".$arrvalues[4]." and loccod=".$arrvalues[3];
           
             //  print "estou no registro=$i<br>$sql<br>";
		  }	
        if ($tablename=='elabrev.ppalocalizador_orcamento2')
		  {
             $sql = "select * from elabrev.ppalocalizador_orcamento2 where prgano='".$_SESSION['exercicio']."' and prgcod=".$arrvalues[0]." and acacodPPA=".$arrvalues[1]." and LOCCodPPA=".$arrvalues[2]." and regcod=".$arrvalues[4]." and loccod=".$arrvalues[3];
           
             //  print "estou no registro=$i<br>$sql<br>";
		  }		  		  
        if ($tablename=='elabrev.ppaacao_orcamento2' and $base=='ppa') $sql = "select * from elabrev.ppaacao_orcamento2 where acacodppa=".$arrvalues[1]." and prgcod=".$arrvalues[0]." and prgano='".$_SESSION['exercicio']."'";
		  
  
          $RS = pg_query($sql);
          $res=pg_fetch_array($RS);
          if (! is_array($res))
         {
            // não existe, logo tem que inserir
            if ($tablename=='planejamento.acao' and $base=='sigplan')
            {
              if ($arrvalues[3]<>'0000' and $arrvalues[3]<>'' and $arrvalues[4]<>'' )
                 {
                   $strsql = "insert into $tablename ($fields) values ($values)";
                   pg_query($strsql);
                  }
            }
            elseif ($tablename=='elabrev.ppaprograma_orcamento' and $base=='ppa')
            {
                $fields .=',PRGANO';
                $values .=','.$_SESSION['exercicio'];
            	$strsql = "insert into $tablename ($fields) values ($values)";
                 pg_query($strsql);
   
            }  
            elseif ($tablename=='elabrev.ppaacao_orcamento2' and $base=='ppa')
            {
                $fields .=',PRGANO';
                $values .=','.$_SESSION['exercicio'];
            	$strsql = "insert into $tablename ($fields) values ($values)";
                 pg_query($strsql);
   
            } 
            elseif ($tablename=='elabrev.ppalocalizador_orcamento2' and $base=='ppa')
            {
                $fields .=',PRGANO';
                $values .=','.$_SESSION['exercicio'];
            	$strsql = "insert into $tablename ($fields) values ($values)";
                 pg_query($strsql);
   
            }            
            elseif ($tablename=='elabrev.dadofinanceiro_orcamento' and $base=='ppa')
            {
                $fields .=',PRGANO';
                $values .=','.$_SESSION['exercicio'];
            	$strsql = "insert into $tablename ($fields) values ($values)";
                 pg_query($strsql);
   
            }  
            elseif ($tablename=='elabrev.ppalocalizador_orcamento' and $base=='ppa')
            {
                $fields .=',PRGANO';
                $values .=','.$_SESSION['exercicio'];
            	$strsql = "insert into $tablename ($fields) values ($values)";
                 pg_query($strsql);
   
            }
            else
            {
                 $strsql = "insert into $tablename ($fields) values ($values)";
                // print $strsql.'<br><br>';
                 pg_query($strsql);
            }
         }
       }
       pg_query('commit; ');
       $this->xml->cleanup();
	}
    	
	function insertintodb($dbconnection, $filename, $tablename,$abrev) 
	{
	  $xml_file = fopen($filename, "r");
	  $this->xml->read_file_handle($xml_file);
	  $numrows = $this->xml->roottag->num_subtags();
	 
	  for ($i = 0; $i < $numrows; $i++) 
	  {
		$ok = true;
		$arrfields = null;
		$arrvalues = null; 
		$row = $this->xml->roottag->tags[$i];
		$numfields = $row->num_subtags();
        unset($ano);
        unset($acaid);
	    for ($ii = 0; $ii < $numfields; $ii++)
		{
		    unset($saccod);
		    unset($prgprgcod);

		    $field = $row->tags[$ii];
		    $arrfields[] = $field->name;

		    if ("'".$field->cdata."'"=="''") $arrvalues[]="null" ;
			  else $arrvalues[] = "'".$field->cdata."'";

                   if (($arrfields[$ii] == 'muncod' ) and (strval($arrvalues[$ii]) == 0)) {$arrvalues[$ii]='null'; }
                   if (($arrfields[$ii] == 'tpocod' ) and (strval($arrvalues[$ii]) == 0) and ($tablename=='orgaosiorg')) {$arrvalues[$ii]='null'; }

		   if (($arrfields[$ii] == 'saccod' ) and ($arrvalues[$ii] == "'0000'")) $ok = false;
		   if (($arrfields[$ii] == 'loccod' ) and ($arrvalues[$ii] == "null")) $ok = false;

		    $fields = join($arrfields, ", ");
		    $values = join(mb_convert_encoding($arrvalues,"iso-8859-1","utf-8"), ", ");
		    //$values = join($arrvalues, ", ");
            $strsql = "insert into $tablename ($fields) values ($values)";
	    	print '<br>'.$strsql;
   	        if ($ok) pg_query($strsql);
       }
	  } 
	}


	function upgradedb_ppa($dbconnection, $filename, $tablename,$abrev,$base)
	{
      ini_set("memory_limit","256M");
      $xml_file = fopen("/var/www/html/simec/xml/importacao/base".$base.'/'.$_SESSION['exercicio'].'/'.$filename, "r" );
	  $this->xml->read_file_handle($xml_file);
	  $numrows = $this->xml->roottag->num_subtags();
	  for ($i = 0; $i < $numrows; $i++)
	  {
	    unset($prgcod,$codppa,$acacod,$unicod);
		$ok = true;
		$arrfields = null;
		$arrvalues = null; 
		$row = $this->xml->roottag->tags[$i];
		$numfields = $row->num_subtags();
	    for ($ii = 0; $ii < $numfields; $ii++)
		{
		    $field = $row->tags[$ii];
		    $arrfields[] = $field->name;
		    if ("'".$field->cdata."'"=="''") $arrvalues[]="null" ;
			  else $arrvalues[] = "'".addslashes(utf8_decode($field->cdata))."'";


		 //  if ($arrfields[$ii] == 'PRGANO' ) $prgano = $arrvalues[$ii] ;
		   if ($arrfields[$ii] == 'PRGCOD' ) $prgcod = $arrvalues[$ii] ;
		   if ($arrfields[$ii] == 'UNICOD' ) $unicod = $arrvalues[$ii];
		   if ($arrfields[$ii] == 'ACACOD' ) $acacod = $arrvalues[$ii];


		   if ($arrfields[$ii] == 'ACAMESINICIO' ) $acamesinicio = $arrvalues[$ii];
		   if ($arrfields[$ii] == 'ACAANOINICIO' ) $acaanoinicio = $arrvalues[$ii] ;
		   if ($arrfields[$ii] == 'ACAMESTERMINO' ) $acamestermino = $arrvalues[$ii];
		   if ($arrfields[$ii] == 'ACAANOTERMINO' ) $acaanotermino = $arrvalues[$ii];
		   if ($arrfields[$ii] == 'FUNCOD' ) $funcod = $arrvalues[$ii];
		   if ($arrfields[$ii] == 'SFUCOD' ) $sfucod = $arrvalues[$ii] ;
		   if ($arrfields[$ii] == 'TINCOD' ) $tincod = $arrvalues[$ii];
		   if ($arrfields[$ii] == 'ESFCOD' ) $esfcod = $arrvalues[$ii];
		   if ($arrfields[$ii] == 'OSICOD' ) $osicod = $arrvalues[$ii] ;
		   if ($arrfields[$ii] == 'ACACODPPA' ) $acacodppa = $arrvalues[$ii] ;
		   if ($arrfields[$ii] == 'ACAFINALIDADE' ) $acafinalidade = $arrvalues[$ii];
		   if ($arrfields[$ii] == 'ACABASELEGAL' ) $acabaselegal = $arrvalues[$ii];
		   if ($arrfields[$ii] == 'ACAREPERCFINANCEIRA' ) $acarepercfinanceira = $arrvalues[$ii] ;
		   if ($arrfields[$ii] == 'ACAVLRREPERCFINANCEIRA' ) $acavlrrepercfinanceira = $arrvalues[$ii];
		   if ($arrfields[$ii] == 'ACASNPADRONIZADA' ) $acasnpadronizada = $arrvalues[$ii];
		   if ($arrfields[$ii] == 'ACASNSETPADRONIZADA' ) $acasnsetpadronizada = $arrvalues[$ii] ;
		   if ($arrfields[$ii] == 'ACASNMEDIRETA' ) $acasnmedireta = $arrvalues[$ii];
		   if ($arrfields[$ii] == 'ACASNMEDESC' ) $acasnmedesc = $arrvalues[$ii];
		   if ($arrfields[$ii] == 'ACASNMELINCRED' ) $acasnmelincred = $arrvalues[$ii] ;
		   if ($arrfields[$ii] == 'ACASNTRANSFOBRIGATORIA' ) $acasntransfobrigatoria = $arrvalues[$ii];
		   if ($arrfields[$ii] == 'ACASNTRANSFVOLUNTARIA' ) $acasntransfvoluntaria = $arrvalues[$ii];
		   if ($arrfields[$ii] == 'ACASNTRANSFOUTRAS' ) $acasntransfoutras = $arrvalues[$ii];
		   if ($arrfields[$ii] == 'ACASNDESPESAOBRIGATORIA' ) $acasndespesaobrigatoria = $arrvalues[$ii];
		   if ($arrfields[$ii] == 'ACASNBLOQUEIOPROGRAMACAO' ) $acasnbloqueioprogramacao = $arrvalues[$ii] ;
		   if ($arrfields[$ii] == 'ACADETALHAMENTO' ) $acadetalhamento = $arrvalues[$ii];
		   if ($arrfields[$ii] == 'ACAORDEMPRIORIDADE' ) $acaordemprioridade = $arrvalues[$ii];
		   if ($arrfields[$ii] == 'ACAOBS' ) $acaobs = $arrvalues[$ii];
		   if ($arrfields[$ii] == 'ACACODSOF' ) $acacodsof = $arrvalues[$ii] ;
		   if ($arrfields[$ii] == 'ACAQTDCUSTOTOTAL' ) $acaqtdcustototal = $arrvalues[$ii];
		   if ($arrfields[$ii] == 'ACACODREFERENCIASOF' ) $acacodreferenciasof = $arrvalues[$ii];
//		   if ($arrfields[$ii] == 'ACAVLRREALATEANOANTERIOR' ) $acavlrrealateanoanterior = $arrvalues[$ii];
		   if ($arrfields[$ii] == 'ACADSCPRODUTO' ) $acadscproduto = $arrvalues[$ii] ;
		   if ($arrfields[$ii] == 'ACADESCRICAO' ) $acadescricao = $arrvalues[$ii];
		   if ($arrfields[$ii] == 'ACAVLRCUSTOTOTAL' ) $acavlrcustototal = $arrvalues[$ii];
		   if ($arrfields[$ii] == 'ACAQTDATEANOANTERIOR' ) $acaqtdateanoanterior = $arrvalues[$ii];
		   if ($arrfields[$ii] == 'ACAVLRCUSTOPREVISTOANOCORRENTE' ) $acavlrcustoprevistoanocorrente = $arrvalues[$ii];
		   if ($arrfields[$ii] == 'ACAQTDPREVISTOANOCORRENTE' ) $acaqtdprevistoanocorrente = $arrvalues[$ii] ;

		  }

	        if (($prgcod ) and ($acacod) and ($unicod) )
		{
		   $sql = "update acao set acacodppa=$acacodppa, funcod=$funcod,sfucod=$sfucod,tincod=$tincod, esfcod=$esfcod,osicod=$osicod,		   acafinalidade=$acafinalidade,  acabaselegal=$acabaselegal,  acarepercfinanceira=$acarepercfinanceira,  acavlrrepercfinanceira=$acavlrrepercfinanceira,		   acasnpadronizada=$acasnpadronizada,  acasnsetpadronizada=$acasnsetpadronizada,   acasnmedireta=$acasnmedireta, acasnmedesc=$acasnmedesc,		   acasnmelincred=$acasnmelincred,  acasntransfobrigatoria=$acasntransfobrigatoria,  acasntransfvoluntaria=$acasntransfvoluntaria,		   acasntransfoutras=$acasntransfoutras,  acasndespesaobrigatoria=$acasndespesaobrigatoria, acasnbloqueioprogramacao=$acasnbloqueioprogramacao,		   acadetalhamento=$acadetalhamento, acaordemprioridade=$acaordemprioridade,acaobs=$acaobs,  acacodsof=$acacodsof, acaqtdcustototal=$acaqtdcustototal,acamesinicio=$acamesinicio,acaanoinicio=$acaanoinicio,acamestermino=$acamestermino,acaanotermino=$acaanotermino,acadscproduto=$acadscproduto,acadescricao=$acadescricao,acavlrcustototal=$acavlrcustototal,acaqtdateanoanterior=$acaqtdateanoanterior,acavlrcustoprevistoanocorrente=$acavlrcustoprevistoanocorrente,acaqtdprevistoanocorrente=$acaqtdprevistoanocorrente, acacodreferenciasof=$acacodreferenciasof where prgcod = $prgcod  and acacod=$acacod and unicod=$unicod ";
		 //  print $sql;
		}

		///$fields = join($arrfields, ", ");
		//$values = join($arrvalues, ", ");
        pg_query($sql);

	  } 
	 ini_set("memory_limit","64M");
	}

	
	function upgradedb_ppa_programa($dbconnection, $filename, $tablename,$abrev,$base)
	{
      ini_set("memory_limit","256M");
      $xml_file = fopen("/var/www/html/simec/xml/importacao/base".$base.'/'.$_SESSION['exercicio'].'/'.$filename, "r" );
	  $this->xml->read_file_handle($xml_file);
	  $numrows = $this->xml->roottag->num_subtags();
	  for ($i = 0; $i < $numrows; $i++)
	  {
	    unset($prgcod);
		$ok = true;
		$arrfields = null;
		$arrvalues = null; 
		$row = $this->xml->roottag->tags[$i];
		$numfields = $row->num_subtags();
	    for ($ii = 0; $ii < $numfields; $ii++)
		{
		    $field = $row->tags[$ii];
		    $arrfields[] = $field->name;
		    if ("'".$field->cdata."'"=="''") $arrvalues[]="null" ;
			  else $arrvalues[] = "'".addslashes(utf8_decode($field->cdata))."'";

		   if ($arrfields[$ii] == 'PRGCOD' ) $prgcod = $arrvalues[$ii] ;
		   if ($arrfields[$ii] == 'PRGMESINICIO' ) $prgmesinicio = $arrvalues[$ii];
		   if ($arrfields[$ii] == 'PRGANOINICIO' ) $prganoinicio = $arrvalues[$ii] ;
		   if ($arrfields[$ii] == 'PRGMESTERMINO' ) $prgmestermino = $arrvalues[$ii];
		   if ($arrfields[$ii] == 'PRGANOTERMINO' ) $prganotermino = $arrvalues[$ii];
		   if ($arrfields[$ii] == 'PRGDSCOBJETIVO' ) $prgdscobjetivo = $arrvalues[$ii];
		   if ($arrfields[$ii] == 'PRGDSCPUBLICOALVO' ) $prgdscpublicoalvo = $arrvalues[$ii] ;
		   if ($arrfields[$ii] == 'PRGDSCJUSTIFICATIVA' ) $prgdscjustificativa = $arrvalues[$ii];
		   if ($arrfields[$ii] == 'PRGDSCESTRATEGIA' ) $prgdscestrategia = $arrvalues[$ii];
		   if ($arrfields[$ii] == 'PRGSNPADRONIZADO' ) $prgsnpadronizado = $arrvalues[$ii] ;
		   if ($arrfields[$ii] == 'PRGVLRPROGRAMA' ) $prgvlrprograma = $arrvalues[$ii] ;
		   if ($arrfields[$ii] == 'PRGSNTEMPORARIO' ) $prgsntemporario = $arrvalues[$ii];
		   if ($arrfields[$ii] == 'PRGOBS' ) $prgobs = $arrvalues[$ii];		   
		  }

	        if ($prgcod )
		{
		   $sql = "update programa set prgmesinicio=$prgmesinicio, prganoinicio=$prganoinicio,prgmestermino=$prgmestermino,prganotermino=$prganotermino, prgdscobjetivo=$prgdscobjetivo,		   prgdscpublicoalvo=$prgdscpublicoalvo,		   prgdscjustificativa=$prgdscjustificativa,  prgdscestrategia=$prgdscestrategia,  prgsnpadronizado=$prgsnpadronizado,  prgvlrprograma=$prgvlrprograma,		   prgsntemporario=$prgsntemporario,  prgobs=$prgobs where prgcod = $prgcod   ";
//		   print $sql;
		}

		///$fields = join($arrfields, ", ");
		//$values = join($arrvalues, ", ");
        pg_query($sql);

	  } 

	}	

	
   function upgradedb2($dbconnection, $filename, $tablename,$abrev) 
	{
	
	  $xml_file = fopen($filename, "r"); 
	  $this->xml->read_file_handle($xml_file);
	  $numrows = $this->xml->roottag->num_subtags();
	  for ($i = 0; $i < $numrows; $i++) 
	  {
	       unset($prgcod,$codppa,$acacod);
		$ok = true;
		$arrfields = null;
		$arrvalues = null; 
		$row = $this->xml->roottag->tags[$i];
		$numfields = $row->num_subtags();
	        for ($ii = 0; $ii < $numfields; $ii++) 
		{
                      

		    $field = $row->tags[$ii];
		    $arrfields[] = $field->name;

		    if ("'".$field->cdata."'"=="''") $arrvalues[]="null" ;
			  else $arrvalues[] = "'".$field->cdata."'";

  

		   if ($arrfields[$ii] == 'prgcod' ) $prgcod = $arrvalues[$ii] ;
		   if ($arrfields[$ii] == 'acacod' ) $acacod = $arrvalues[$ii];

		   if ($arrfields[$ii] == 'acadscproduto' ) $acadscproduto = $arrvalues[$ii] ;
		   
                   }
	        if (($prgcod ) and ($acacod))
		{
		   $sql = "update acao set acadscproduto=$acadscproduto where prgcod = $prgcod  and acacod=$acacod";
		   print $sql;
		}
                
		$fields = join($arrfields, ", ");
		$values = join($arrvalues, ", ");

               // $strsql = "insert into $tablename ($fields) values ($values)";

                if (pg_query($sql)) print "ok.$i<br>";	 
		   
	  } 
		  
	}
	
   function upgradedb_($dbconnection, $filename, $tablename,$abrev)
	{

	  $xml_file = fopen($filename, "r");
	  $this->xml->read_file_handle($xml_file);
	  $numrows = $this->xml->roottag->num_subtags();
	  for ($i = 0; $i < $numrows; $i++)
	  {
	       unset($prgcod,$codppa,$acacod);
		$ok = true;
		$arrfields = null;
		$arrvalues = null;
		$row = $this->xml->roottag->tags[$i];
		$numfields = $row->num_subtags();
	        for ($ii = 0; $ii < $numfields; $ii++)
		{
		    $field = $row->tags[$ii];
		    $arrfields[] = $field->name;

		    if ("'".$field->cdata."'"=="''") $arrvalues[]="null" ;
			  else $arrvalues[] = "'".$field->cdata."'";

		   if ($arrfields[$ii] == 'PRGCOD' ) $prgcod = $arrvalues[$ii] ;
		   if ($arrfields[$ii] == 'acacodppa' ) $acacodppa = $arrvalues[$ii];
		   if ($arrfields[$ii] == 'ACACOD' ) $acacod = $arrvalues[$ii] ;
		   if ($arrfields[$ii] == 'unicod' ) $unicod = $arrvalues[$ii];
		   if ($arrfields[$ii] == 'funcod') $funcod = $arrvalues[$ii] ;
		   if ($arrfields[$ii] == 'sfucod' ) $sfucod = $arrvalues[$ii];
		   
		   
		   $sql = "update acao set funcod=$funcod, sfucod=$sfucod where prgcod = $prgcod  and acacod=$acacod and unicod=$unicod";
		   print $sql;
		}

		$fields = join($arrfields, ", ");
		$values = join($arrvalues, ", ");

               // $strsql = "insert into $tablename ($fields) values ($values)";

                if (pg_query($sql)) print "ok.$i<br>";

	  }
}


  function upgradedb_dadofis($dbconnection, $filename, $tablename,$abrev)
	{
// esta função atualiza a tabela de dados físicos proveniente do sigplan em duas fases:
// fase 1 - apaga todos os registros de um específico ano
// fase 2 - cria todos os registros novos.

	  $xml_file = fopen($filename, "r");
	  $this->xml->read_file_handle($xml_file);
	  $numrows = $this->xml->roottag->num_subtags();
	  print 'Trabalho iniciado<br>';
      for ($i = 0; $i < 1; $i++)
	  {
	    unset($prgano);
		$ok = true;
		$arrfields = null;
		$arrvalues = null;
		$row = $this->xml->roottag->tags[0];
		$numfields = $row->num_subtags();
	    for ($ii = 0; $ii < $numfields; $ii++)
		{
		    $field = $row->tags[$ii];
		    $arrfields[] = $field->name;

		    if ("'".$field->cdata."'"=="''") $arrvalues[]="null" ;
			  else $arrvalues[] = $field->cdata;

		   if ($arrfields[$ii] == 'PRGANO' ) $prgano = $arrvalues[$ii] ;
           if ($prgcod) break;
		}
	  }
	  	  print "Ano = $prgano<br>";
	  // com a sequencia acima temos o ano
	  $sql ="delete from dadofisico where prgano='$prgano'";
      // com o ano apagamos o conteudo seletivo da tabela
       $RS = pg_query($sql);
     // fim da fase 1
     print 'Fim da Fase 1 - Exclusão dos registros.<br>';
     print 'Início da Fase 2 - Inclusão dos registros.<br>';
     //     fase 2
	  for ($i = 0; $i < $numrows; $i++)
	  {
		$arrfields = null;
		$arrvalues = null;
		$row = $this->xml->roottag->tags[$i];
		$numfields = $row->num_subtags();
	    for ($ii = 0; $ii < $numfields; $ii++)
		{
		    $field = $row->tags[$ii];
		    $arrfields[] = $field->name;

		    if ("'".$field->cdata."'"=="''") $arrvalues[]="null" ;
			  else if (! is_numeric($field->cdata)) $arrvalues[] = "'".$field->cdata."'"; else $arrvalues[] = $field->cdata;

		   if ($arrfields[$ii] == 'PRGCOD' and is_numeric($arrvalues[$ii])) $arrvalues[$ii]= "'".$arrvalues[$ii]."'";
		   if ($arrfields[$ii] == 'PRGANO' and is_numeric($arrvalues[$ii]) )  $arrvalues[$ii] = "'".$arrvalues[$ii]."'";
		   if ($arrfields[$ii] == 'ACACOD'  and is_numeric($arrvalues[$ii]))  $arrvalues[$ii] = "'".$arrvalues[$ii]."'";
		   if ($arrfields[$ii] == 'SACCOD' and is_numeric($arrvalues[$ii]))  $arrvalues[$ii] = "'".$arrvalues[$ii]."'";
		}

		$fields = join($arrfields, ", ");
		$values = join($arrvalues, ", ");
        $sql = "insert into $tablename ($fields) values ($values)";
        pg_query($sql);
	  }
     print 'Fim da Fase 2 - Inclusão dos registros.<br>';
     $this->dadofis();

}



}// end class

?>
