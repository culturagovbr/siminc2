<?

 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Simec
   Analista: Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Cristiano Cabral (cristiano.cabral@gmail.com)
   Módulo:incusuario.inc
   Finalidade: include de dados especificos para cadastro de usuarios, , usado no modulo de segurança
 */

?>
	<script>
		//Armazena em um array as responsabilidades dos perfis
		var arrPerfis = new Array();

		function addPerfil( pflcod, tprcod )
		{
			arrPerfis.push( {pflcod: pflcod, tprcod: tprcod} );
		}

	</script>
  	<tr>
        <td align='right' class="SubTituloDireita" >Perfil desejado:</td>
        <td >
        <?
        $sql = "select pflcod from usuario_sistema where usucpf = '".corrige_cpf($_REQUEST['usucpf'])."' and sisid = ".$codigo;
        $pflcod = $db->pegaUm($sql);
        //$sql="select pflcod as codigo, pfldsc as descricao from seguranca.perfil where pflstatus='A' and pflnivel >=".$_SESSION['usunivel']." and sisid=".$_SESSION['sisid']."order by 2";
        $queryResp = "select seguranca.perfil.pflcod as codigo, monitora.tprperfil.tprcod as tprcod from seguranca.perfil inner join monitora.tprperfil on seguranca.perfil.pflcod = monitora.tprperfil.pflcod where pflstatus='A' and seguranca.perfil.pflnivel > 1 and sisid=". $codigo ." order by codigo;";
        $resultado = $db->carregar( $queryResp );
        echo "<script>";
        foreach( $resultado as $linha => $dado )
        {
        	echo "addPerfil(".$dado[ "codigo" ].",".$dado[ "tprcod" ].");\n";
        } 
        echo "</script>"; 	
        $sql="select pflcod as codigo, pfldsc as descricao from seguranca.perfil where pflstatus='A'  and seguranca.perfil.pflnivel > 1 and sisid=".$codigo." order by descricao";
     
        $db->monta_combo("pflcod",$sql,($pflcod == ''?'S':'N'),"Selecione o perfil desejado",'mostraResponsabilidade','');?>
        </td>
      </tr>
      <tr id="prg">
        <td align='right' class="subtitulodireita">Programa(s) desejado(s):</td>
        <td>
        <?php
		//busca os programas ja propostos caso já exista 
			$sql = "select pap.prgid, p.prgcod||' - '||p.prgdsc as dscprg from monitora.progacaoproposto as pap left join planejamento.programa as p on pap.prgid = p.prgid where pap.acacod = '' and pap.usucpf = '".corrige_cpf($_REQUEST['usucpf'])."'";
			$RS1 = $db->carregar($sql);
			if (! is_array($RS1) or ($_SESSION['sisid']==4))
			{
		?>
			<select multiple size="5" name="usuprgproposto[]" id="usuprgproposto" style="width:500px;" onclick="seleciona_prg();"  class="CampoEstilo">
			<option value="">Clique Aqui para Selecionar o(s) Programa(s)</option></select><?=obrigatorio();?>
			<br>
			<font color="#006666">Indique acima o(s) Programa(s) sob sua responsabilidade como <b id="prgresp"></b>.</font>
	    <?	
		}else
		{
		?>
			<select multiple size="5" name="usuprgproposto[]" id="usuprgproposto" style="width:500px;" onclick=""  class="CampoEstilo">
		<?
				$nlinhas = count($RS1)-1;	
		  		for ($i=0; $i<=$nlinhas;$i++)
			 	{
					foreach($RS1[$i] as $k=>$v) ${$k}=$v;
					//print 'valor de i:'.$i;
					print '<option value='.$prgid.'>'.$dscprg.'</option>';
				
			 	}
		?>
			</select><?=obrigatorio();?>
				
		<?
		}
		?>
		</td>
      </tr>
      <tr id="aca">
        <td align='right' class="subtitulodireita">Ação(ões) desejada(s):</td>
        <td>
        <?php
		//busca os programas ja propostos caso já exista 
			$sql = "select distinct pap.acacod, a.prgcod||'.'||pap.acacod||'.'||pap.unicod||' - '||a.acadsc as dscacao from monitora.progacaoproposto as pap inner join monitora.acao as a on pap.acacod = a.acacod and pap.prgid = a.prgid and pap.unicod = a.unicod  where a.acasnrap = false and pap.usucpf =  '".corrige_cpf($_REQUEST['usucpf'])."'";

			$RS2 = $db->carregar($sql);
			if (! is_array($RS2) or ($_SESSION['sisid']==4))
			{
		?>
		<select multiple size="5" name="usuacaproposto[]" id="usuacaproposto" style="width:500px;" onclick="seleciona_aca();"  class="CampoEstilo">
		<option value="">Clique Aqui para Selecionar a(s) Ação(ões)</option></select><?=obrigatorio();?><br>
		<font color="#006666">Indique acima a(s) Ação(ões) sob sua responsabilidade como <b id="acaresp"></b>.</font>
	    	    <?	
		}else
		{
		?>
			<select multiple size="5" name="usuacaproposto[]" id="usuacaproposto" style="width:500px;" onclick=""  class="CampoEstilo">
		<?
				$nlinhas = count($RS2)-1;	
		  		for ($i=0; $i<=$nlinhas;$i++)
			 	{
					foreach($RS2[$i] as $k=>$v) ${$k}=$v;
					//print 'valor de i:'.$i;
					print '<option value='.$acacod.'>'.$dscacao.'</option>';
				
			 	}
		?>
			</select><?=obrigatorio();?>
				
		<?
		}
		?>
		</td>
</tr>
<?
//mostra dados quando somente é leitura
 if (! is_array($RS1) or ($_SESSION['sisid']==4) )
 {?>
	<script>
	document.getElementById("prg").style.visibility = "hidden";
	document.getElementById("prg").style.display = "none";
 	</script>
 
 <?
 }
  if (! is_array($RS2) or ($_SESSION['sisid']==4))
 {?>
	<script>
	document.getElementById("aca").style.visibility = "hidden";
	document.getElementById("aca").style.display = "none";
 	</script>
 
 <?
 }
?>


<script>

var selecionaprg = null;


function mostraResponsabilidade( pflcod )
{
	
	document.getElementById("prg").style.visibility = "hidden";
	document.getElementById("prg").style.display = "none";
	document.getElementById("aca").style.visibility = "hidden";
	document.getElementById("aca").style.display = "none";

	//alert( arrPerfis.length );
	for( var i = 0 ; i < arrPerfis.length ; i++ )
	{
			if( arrPerfis[ i ].pflcod == pflcod )
			{
				if (arrPerfis[ i ].tprcod == 6)
				{
					prg_obrg = 1;
					if(document.all){
						document.getElementById("prgresp").innerText = "Programas";}
					else {
						document.getElementById("prgresp").textContent = "Programas";}
					
					document.getElementById("prg").style.visibility = "visible";
					document.getElementById("prg").style.display = "";
				}
				
				if (arrPerfis[ i ].tprcod == 8)
				{
					aca_obrg = 1;
					if(document.all){
						document.getElementById("acaresp").innerText = "Coordenador de Ação";}
					else {
						document.getElementById("acaresp").textContent = "Coordenador de Ação";}
					
					document.getElementById("aca").style.visibility = "visible";
					document.getElementById("aca").style.display = "";
				}			

		}
	}
	return;
}


function retorno(prgaca)
{
j = 0;
if (prgaca=="prg"){campoSelect = document.getElementById("usuprgproposto");} else {campoSelect = document.getElementById("usuacaproposto");}
	
tamanho = campoSelect.options.length-1;
	
	for(var i=tamanho; i>=0; i--)
		{
		campoSelect.options[i] = null;
		}

	for(var i=0; i<selecionaprg.document.formulario.prgid.length; i++)
		{if(selecionaprg.document.formulario.prgid[i].checked == true)
			{campoSelect.options[j] = new Option(selecionaprg.document.formulario.prgdsc[i].value, selecionaprg.document.formulario.prgid[i].value, false, false);
			j++;}
		}
	if (j == 0)
		{ if (prgaca=="prg") {campoSelect.options[j] = new Option('Clique Aqui para Selecionar o(s) Programa(s)', '', false, false);} else {campoSelect.options[j] = new Option('Clique Aqui para Selecionar a(s) Ação(ões)', '', false, false);} }
}

function retorna(objeto,prgaca)
{
	if (prgaca=="prg"){campoSelect = document.getElementById("usuprgproposto");} else {campoSelect = document.getElementById("usuacaproposto");}
	tamanho = campoSelect.options.length;
	if (campoSelect.options[0].value=='') {tamanho--;}
	if (selecionaprg.document.formulario.prgid[objeto].checked == true){
		campoSelect.options[tamanho] = new Option(selecionaprg.document.formulario.prgdsc[objeto].value, selecionaprg.document.formulario.prgid[objeto].value, false, false);
		sortSelect(campoSelect);
	}
	else {
		for(var i=0; i<=campoSelect.length-1; i++){
			if (selecionaprg.document.formulario.prgid[objeto].value == campoSelect.options[i].value)
				{campoSelect.options[i] = null;}
			}
			if (!campoSelect.options[0]){if (prgaca=="prg") {campoSelect.options[0] = new Option('Clique Aqui para Selecionar o(s) Programa(s)', '', false, false);} else {campoSelect.options[0] = new Option('Clique Aqui para Selecionar a(s) Ação(ões)', '', false, false);}}
			sortSelect(campoSelect);
	}
}	

function seleciona_prg()
{
	document.getElementById("usuprgproposto").selectedIndex = -1;
	selecionaprg = window.open("../monitora/geral/seleciona_prg.php?campo=usuprgproposto", "selecionaprg","menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=500,height=480");
}

function seleciona_aca()
{
	document.getElementById("usuacaproposto").selectedIndex = -1;
	selecionaprg = window.open("../monitora/geral/seleciona_aca.php?campo=usuacaproposto", "selecionaprg","menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=500,height=480");
}


function sortSelect(obj) {
			var o = new Array();
			if (obj.options==null) { return; }
			for (var i=0; i<obj.options.length; i++) {
				o[o.length] = new Option( obj.options[i].text, obj.options[i].value, obj.options[i].defaultSelected, obj.options[i].selected) ;
				}
			if (o.length==0) { return; }
			o = o.sort( 
				function(a,b) { 
					if ((a.text+"") < (b.text+"")) { return -1; }
					if ((a.text+"") > (b.text+"")) { return 1; }
					return 0;
					} 
				);
		
			for (var i=0; i<o.length; i++) {
				obj.options[i] = new Option(o[i].text, o[i].value, o[i].defaultSelected, o[i].selected);
				}
	}
	
	function selectAllOptions(obj) {
	for (var i=0; i<obj.options.length; i++) {
		obj.options[i].selected = true;
		}
	}



</script>