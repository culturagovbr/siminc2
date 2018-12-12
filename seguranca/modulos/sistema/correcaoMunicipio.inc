<?php
if($_REQUEST['requisicao']){
	$_REQUEST['requisicao']();
}

function carregaPoligonos()
{
	global $db;
	
	if($_REQUEST['uf2']) {
		$sql = "select ST_asGeoJSON(ST_transform(ST_simplify(ST_transform(estpoligono, 2249), 10000),4291),2, 0) as poli, REPLACE(REPLACE(st_astext(st_centroid(estpoligono)),'POINT(',''),')','') as centro, estuf from territoriosgeo.estado where estuf='".$_REQUEST['uf2']."'";
	} else {
		$sql = "SELECT ST_asGeoJSON(ST_transform(ST_simplify(ST_transform(munpoligono, 2249), 10000),4291),2, 0) as poli, REPLACE(REPLACE(st_astext(st_centroid(munpoligono)),'POINT(',''),')','') as centro, muncod, removeacento(mundescricao) as mundescricao, estuf, '#f6ead9' as cor
		from territoriosgeo.municipio mun
		where munpoligono is not null
		".(($_REQUEST['uf'])?" and mun.estuf in ('".$_REQUEST['uf']."')":" ")."
		".(($_REQUEST['muncod'])?" and mun.muncod in ('".$_REQUEST['muncod']."') ":" ");
	}
	$dados = $db->carregar($sql); 
	//dbg($dados,1);
	echo JSON_encode($dados);
	die;
}

function validarlatlongmunicipio()
{
	global $db;
	
	$sql = "SELECT distanciaPontosGPS(
										CASE WHEN (length (mun.munmedlat)=8) THEN 
											CASE WHEN length(REPLACE('0' || mun.munmedlat,'S','')) = 8 THEN
												((SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),1,2)::double precision))*(-1)
											ELSE
												(SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),1,2)::double precision)
											END
										ELSE
											CASE WHEN length(REPLACE(mun.munmedlat,'S','')) = 8 THEN
												((SUBSTR(REPLACE(mun.munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(mun.munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE(mun.munmedlat,'S',''),1,2)::double precision))*(-1)
											ELSE
												0--((SUBSTR(REPLACE(mun.munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(mun.munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE(mun.munmedlat,'N',''),1,2)::double precision))
											END
										END,
										
										(SUBSTR(REPLACE(mun.munmedlog,'W',''),1,2)::double precision + (SUBSTR(REPLACE(mun.munmedlog,'W',''),3,2)::double precision/60)) *(-1),
										'".$_REQUEST['lat']."',
										'".$_REQUEST['lng']."') as distancia,
										
				CAST(munmedraio as integer)*1000 as munmedraio
			FROM territorios.municipio mun
			WHERE mun.muncod='".$_REQUEST['muncod']."'";
	
	$dados = $db->pegaLinha($sql);
	
	if($dados['distancia'] > $dados['munmedraio']) {
		echo "TRUE";
	} else {
		echo "TRUE";
	}
	exit;
}

function corrigeMunicipio()
{
	global $db;
	
	$muncod = $_REQUEST['muncod'];
	$lat = $_REQUEST['lat'];
	$lng = $_REQUEST['lng'];
	
	if(strstr($lat,"-")){
		$polo = "S";
	}else{
		$polo = "N";
	}
	$lat = str_replace("-","",$lat);
	$inicio = strpos($lat,".");
	$grau = substr($lat,0,$inicio);
	if(strlen($grau) == 1){
		$grau = "0".$grau;
	}elseif(strlen($grau) == 2){
		$grau = $grau;
	}else{
		$grau = substr($grau,0,2);
	}
	$minutos = (float)$lat - (int)$grau;
	$minutos = (float)$minutos*60;
	$segundos = $minutos;
	$minutos = (int)$minutos;
	$minutos = str_replace(".","",$minutos);
	if(strlen($minutos) == 1){
		$minutos = "0".$minutos;
	}elseif(strlen($minutos) == 2){
		$minutos = $minutos;
	}else{
		$minutos = substr($minutos,0,2);
	}
	
	$segundos = $segundos - $minutos;
	$segundos = $segundos*60;
	$segundos = str_replace(".","",$segundos);
	if(strlen($segundos) == 1){
		$segundos = "000".$segundos;
	}elseif(strlen($segundos) == 2){
		$segundos = "00".$segundos;
	}elseif(strlen($segundos) == 3){
		$segundos = "0".$segundos;
	}else{
		$segundos = substr($segundos,0,4);
	}
	$medlatitude = $grau.$polo.$minutos.$segundos;
	
	
	$polo = "W";
	$lng = str_replace("-","",$lng);
	$inicio = strpos($lng,".");
	$grau = substr($lng,0,$inicio);
	if(strlen($grau) == 1){
		$grau = "0".$grau;
	}elseif(strlen($grau) == 2){
		$grau = $grau;
	}else{
		$grau = substr($grau,0,2);
	}
	$minutos = (float)$lng - (int)$grau;
	$minutos = (float)$minutos*60;
	$segundos = $minutos;
	$minutos = (int)$minutos;
	$minutos = str_replace(".","",$minutos);
	if(strlen($minutos) == 1){
		$minutos = "0".$minutos;
	}elseif(strlen($minutos) == 2){
		$minutos = $minutos;
	}else{
		$minutos = substr($minutos,0,2);
	}
	$segundos = $segundos - $minutos;
	$segundos = $segundos*60;
	$segundos = str_replace(".","",$segundos);
	if(strlen($segundos) == 1){
		$segundos = "000".$segundos;
	}elseif(strlen($segundos) == 2){
		$segundos = "00".$segundos;
	}elseif(strlen($segundos) == 3){
		$segundos = "0".$segundos;
	}else{
		$segundos = substr($segundos,0,4);
	}
	$medlongitude = $grau.$polo.$minutos.$segundos;
	
	
	if(!$muncod || $muncod == ""){
		echo "Município não encontrado!";
		exit;
	}
	
	if(!$medlatitude || $medlatitude == ""){
		echo "Latitude inválida!";
		exit;
	}
	
	if(!$medlongitude || $medlongitude == ""){
		echo "Longitude inválida!";
		exit;
	}
	
	$sql = "update 
				territoriosgeo.municipio 
			set 
				munmedlat = '$medlatitude',
				munmedlog = '$medlongitude',
				munlatlong = (select ST_GeomFromText('POINT({$_REQUEST['lng']} {$_REQUEST['lat']})',4291) )
			where 
				muncod = '$muncod';
			update 
				territorios.municipio 
			set 
				munmedlat = '$medlatitude',
				munmedlog = '$medlongitude' 
			where 
				muncod = '$muncod';";
	
	$db->executar($sql);
	
	if($db->commit($sql)){
		echo "Município atualizado com sucesso!";
	}else{
		echo "Não foi possível atualizar o município!";
	}
	exit;
}

function arrMunicipios()
{
	global $db;
	
	header('content-type: text/html; charset=ISO-8859-1');
	
	if($_REQUEST['muncod'] && $_REQUEST['muncod'] != ""){
		$arrWhere[] = "muncod = '{$_REQUEST['muncod']}'";
	}

	$sql = "select latitude,longitude,muncod,mundescricao,estuf from(
				select muncod,mundescricao,estuf,
				ST_X(munlatlong) as longitude,
   				 ST_Y(munlatlong) as latitude,
				
				munpoligono as poli from territoriosgeo.municipio ".($arrWhere ? " where ".implode(" and ",$arrWhere) : "")." 
				)as foo where 1=1 ".(!$_REQUEST['muncod'] ? " and ST_Contains( poli,ST_GeomFromText('POINT('||longitude||' '||latitude||')',4291)) = 'f' " : "")." order by mundescricao,estuf";
 	
	$dados = $db->carregar($sql);
	
	if($dados):
		
		$conteudo .= "<markers> "; // inicia o XML
		
		foreach($dados as $d):
										
			$conteudo .= "<marker "; //inicia um ponto no mapa
			$conteudo .= "mundescricao=\"". $d['mundescricao'] ."\" "; // adiciona a descrição do município;
			$conteudo .= "muncod=\"". $d['muncod'] ."\" "; // adiciona a descrição do município;
			$conteudo .= "estuf=\"". $d['estuf'] ."\" "; // adiciona UF;
			$conteudo .= "lat='{$d['latitude']}' "; // adiciona a latitude;
			$conteudo .= "lng='{$d['longitude']}' "; //adiciona a longitude;
			$conteudo .= "/> ";
		
		endforeach;
		
		$conteudo .= "</markers> ";
		print $conteudo;
		
	endif;
	exit;
	
}

function exibeMapaGeo()
{
	
	?>
	<html>
		<head>
			<meta name="viewport" content="initial-scale=1.0, user-scalable=no" />
			<script type="text/javascript" src="http://maps.google.com/maps/api/js?sensor=false"></script>
			<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
			<script type="text/javascript">
				var map;
				var infowindow = '';
				var draw_circle = null;  // object of google maps polygon for redrawing the circle
				var shape = {
				    coord: [1, 1, 1, 20, 18, 20, 18 , 1],
				    type: 'poly'
				};
				var nomePoli        = new Array();
				var nomePoli2       = new Array();
				var nomePont        = new Array();
				var corPoli         = new Array();
				var arrMuncod		= new Array();
				
				var estadoPoligono  = new Array();
				var estadoPoligono2 = new Array();
				var centroPoligono2 = new Array();
				var estadoPonto     = new Array();
				
				function initialize() {
					var latlng = new google.maps.LatLng(-14.689881, -52.373047);
					var myOptions = {
						zoom: 4,
						center: latlng,
						mapTypeId: google.maps.MapTypeId.TERRAIN
					};
					map = new google.maps.Map(document.getElementById("map_canvas"), myOptions);
					
					marcarPontos("seguranca.php?modulo=sistema/correcaoMunicipio&acao=A&requisicao=arrMunicipios&muncod=<?php echo $_REQUEST['muncod'] ?>");
					
					<?php if($_REQUEST['muncod']): ?>
						jQuery.ajax({
							type: "POST",
							url: "seguranca.php?modulo=sistema/correcaoMunicipio&acao=A&requisicao=carregaPoligonos&muncod=<?php echo $_REQUEST['muncod'] ?>",
							async: false,
							dataType:'JSON',
							success: function(response){
								montarPoligonos(response);
							}
						});
					<?php endif; ?>
								
				}
				 
				function salvarGeoMunicipio(muncod)
				{
					var lat = $('#latitude_' + muncod).val();
					var lng = $('#longitude_'+ muncod).val();
					
					var validadistancia = true;
					
					if(!lat || !lng) {
						alert("Favor informar as coordenadas!");
						return false;
					}
					
					jQuery.ajax({
				   		type: "POST",
				   		url: 'seguranca.php?modulo=sistema/correcaoMunicipio&acao=A&requisicao=validarlatlongmunicipio&lat='+lat+'&lng='+lng+'&muncod='+muncod,
				   		async: false,
				   		success: function(data) {
				   			if(data == "FALSE"){
								validadistancia = false;
							}
						}
				   	}); 
					
					if(!validadistancia) {
						alert("As coordenadas não correspondem aos limites do munícipio!");
						return false;
					}
					$("[name='btn_salvar_" + muncod + "']").attr("disabled","disabled");
					$("[name='btn_salvar_" + muncod + "']").val("Carregando...");
					
					jQuery.ajax({
				   		type: "POST",
				   		url: "seguranca.php?modulo=sistema/correcaoMunicipio&acao=A&requisicao=corrigeMunicipio&muncod=" + muncod + "&lat=" + lat + "&lng=" + lng,
				   		async: false,
				   		success: function(data) {
				   			alert(data);
				   			$("[name='btn_salvar_" + muncod + "']").attr("disabled","");
							$("[name='btn_salvar_" + muncod + "']").val("Salvar");
				   		}
				   	}); 
				}
				  
				function marcarPontos(xml_filtro) {

					jQuery.ajax({
				   		type: "POST",
				   		url: xml_filtro,
				   		async: false,
				   		success: function(data) {
						      jQuery(data).find("marker").each(function() {
						      
						        var marker = jQuery(this);
						        var latlng = new google.maps.LatLng(parseFloat(marker.attr("lat")),
						                                    		parseFloat(marker.attr("lng")));
						                                    		
								var html = '<div style="font-size:12px;font-family:verdana" ><b>Município:</b> ' + marker.attr("mundescricao") + ' / ' + marker.attr("estuf") + '<br><table style="font-size:12px;font-family:verdana" ><tr><td colspan="2" >Informe a nova latitude e longitude</td></tr><tr><td>Latitude:</td><td><input type="text" name="latitude[' + marker.attr("muncod") + ']" size="30" id="latitude_' + marker.attr("muncod") + '" /></td></tr><tr><td>Longitude:</td><td><input type="text" name="longitude[' + marker.attr("muncod") + ']" size="30" id="longitude_' + marker.attr("muncod") + '" /></td></tr><tr><td></td><td><input type="button" name="btn_salvar_' + marker.attr("muncod") + '" value="Salvar" onclick="salvarGeoMunicipio(\'' + marker.attr("muncod") + '\')" /></td></tr></table><span id="msg_' + marker.attr("muncod") + '"></span></div>';
						                                    		
						        var ponto = new google.maps.Marker({
								      position: latlng, 
								      map: map, 
								      title: marker.attr("mundescricao"),
								      draggable: true
								  });   
															    
								google.maps.event.addListener(ponto, "dblclick", function() {
									if (infowindow) infowindow.close();
									var lat = ponto.getPosition().lat();
								    var lng = ponto.getPosition().lng();
									var html = '<div style="font-size:12px;font-family:verdana" ><b>Município:</b> ' + marker.attr("mundescricao") + ' / ' + marker.attr("estuf") + '<br><table style="font-size:12px;font-family:verdana" ><tr><td colspan="2" >Informe a nova latitude e longitude</td></tr><tr><td>Latitude:</td><td><input type="text" name="latitude[' + marker.attr("muncod") + ']" size="30" value="' + lat + '" id="latitude_' + marker.attr("muncod") + '" /></td></tr><tr><td>Longitude:</td><td><input type="text" name="longitude[' + marker.attr("muncod") + ']" size="30" value="' + lng + '" id="longitude_' + marker.attr("muncod") + '" /></td></tr><tr><td></td><td><input type="button" name="btn_salvar_' + marker.attr("muncod") + '" value="Salvar" onclick="salvarGeoMunicipio(\'' + marker.attr("muncod") + '\')" /></td></tr></table><span id="msg_' + marker.attr("muncod") + '"></span></div>';
									infowindow = new google.maps.InfoWindow({content: html});
									infowindow.open(map, ponto);
								});
								
								 google.maps.event.addListener(ponto, 'dragend', function() {
								    var lat = ponto.getPosition().lat();
								    var lng = ponto.getPosition().lng();
								    jQuery('#latitude_' + marker.attr("muncod")).val(lat);
								    jQuery('#longitude_'+ marker.attr("muncod")).val(lng);
								  });
								
								map.setZoom(9);
								map.setCenter(new google.maps.LatLng(ponto.getPosition().lat(),ponto.getPosition().lng()));
								

							});
				   		}
				 		});
				
				}
				
				
				function montarPoligonos(response,carregaEstado){
					response = jQuery.parseJSON(response);
					jQuery.each(response,function(index,item){
						var corpolyd = "#d82b40";
						var corpoly = "#f6ead9";
						var GeoJSON = jQuery.parseJSON(item.poli);
						var coords = GeoJSON.coordinates;
				         var paths = [];
				            for (var i = 0; i < coords.length; i++) {
				                for (var j = 0; j < coords[i].length; j++) {
				                    var path = [];
				                    for (var k = 0; k < coords[i][j].length; k++) {
				                        var ll = new google.maps.LatLng(coords[i][j][k][1],coords[i][j][k][0]);
				                        path.push(ll);
				                    }
				                    paths.push(path);
				                }
				            } 
				
					corPoli[item.estuf+item.muncod] = corpoly;
					
					if(!arrMuncod[item.muncod]){
							
							arrMuncod[item.muncod] = item.estuf;
							
							if(nomePoli[item.estuf+item.muncod]){
								nomePoli[item.estuf+item.muncod].setMap(null);
							}
							
							nomePoli[item.estuf+item.muncod] = new google.maps.Polygon({
						      paths: paths, 
						      strokeColor: '#000000',
						      strokeOpacity: 0.6,
						      strokeWeight: 0.5,
						      fillColor: item.cor,
						      fillOpacity: 0.5,
						      zIndex: 0
						    });
						    
							nomePoli[item.estuf+item.muncod].setMap(map);
							
						}
					
					
					if(!estadoPoligono[item.estuf]) {
						estadoPoligono[item.estuf]='';
					}
					
					if(carregaEstado != 1){
						estadoPoligono[item.estuf] = estadoPoligono[item.estuf]+item.estuf+item.muncod+",";
					}
						
					google.maps.event.addListener(nomePoli[item.estuf+item.muncod], 'mouseover', function(event){f_mouseover(item.estuf+item.muncod,corpolyd,item.mundescricao+'/'+item.estuf);});
					google.maps.event.addListener(nomePoli[item.estuf+item.muncod], 'mouseout', function(event){f_mouseout(item.estuf+item.muncod,item.cor);});
					
					var centro = item.centro;
					var arrCoord = centro.split(" ");
					map.setZoom(9);
					map.setCenter(new google.maps.LatLng(arrCoord[1],arrCoord[0]));
					
				});
					
					
				}
				
				 function f_mouseover(obj,cor,mundescricao)
				  {
				  if (nomePoli[obj]){
					nomePoli[obj].setOptions( {fillColor: cor} );
					//document.getElementById('nome_mun').innerHTML = mundescricao;
					}
				  }
				
				 function f_mouseout(obj,cor)
				  {
				  if (nomePoli[obj]){
					f_mudacor(obj,corPoli[obj]); 
					//document.getElementById('nome_mun').innerHTML = '&nbsp;';
					}
				  }
				
				 function f_mudacor(obj,cor){
					corPoli[obj] = cor;
				    nomePoli[obj].setOptions( {fillColor: cor} );
				  }
				
			</script>
		</head>
		<body onload="initialize()">
		  <div id="map_canvas" style="width:100%; height:100%"></div>
		</body>
	</html>
<?php exit;}

ini_set("memory_limit", "1024M");
set_time_limit(0);

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$titulo = !$titulo ? str_replace("...","",$_SESSION['mnudsc']) : $titulo;
monta_titulo( $titulo, '&nbsp' );
?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript">
function pesquisarMunicipio(){
	$("#formulario_geo").submit();
}

function corrigirGeo(muncod)
{
	janela("seguranca.php?modulo=sistema/correcaoMunicipio&acao=A&requisicao=exibeMapaGeo&muncod=" + muncod,"600","500","Mapa");
}

</script>
<style>
	.SubtituloTabela{background-color:#cccccc}
	.negrito{font-weight:bold}
	.bold{font-weight:bold}
	.normal{font-weight:normal}
	.center{text-align: center;}
	.direita{text-align: right;}
	.esquerda{text-align: left;}
	.msg_erro{color:#990000}
	.link{cursor: pointer}
	.mini{width:12px;height:12px}
	.sucess_msg{color: blue;}
	.img_middle{vertical-align:middle}
	.hidden{display:none}
	.absolute{position:absolute;padding-top:5px;padding-bottom:5px;padding-left:5px;margin-top:5px;margin-left:50px;border:solid 1px black;background-color: #FFFFFF}
	.fechar{position:relative;right:-5px;top:-26px;}
	.img{background-color:#FFFFFF}
	.red{color:#990000}
</style>
<form name="formulario_geo" id="formulario_geo"  method="post" action="" >
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td width="25%" class="SubtituloDireita" >Município:</td>
			<td><?php echo campo_texto("mundescricao","N","S","Município",60,100,"","","","","","","",$_POST['mundescricao']) ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Estado:</td>
			<td>
				<?php $sql = "select estuf as codigo, estdescricao as descricao from territorios.estado order by estuf" ?>
				<?php $db->monta_combo("estuf",$sql,"S","Selecione...","","","","","","","",$_POST['estuf']) ?>
			</td>
		</tr>
		<tr class="SubtituloTabela">
			<td></td>
			<td>
				<input type="button" name="btn_pesquisar" onclick="pesquisarMunicipio()" value="Pesquisar" />
				<input type="button" name="btn_ver_todos" onclick="window.location.href=window.location" value="Ver Todos" />
			</td>
		</tr>
	</table>
</form>
<?php 

if($_POST['mundescricao']){
	$arrrWhere[] = "removeacento(mundescricao) ilike (removeacento('%{$_POST['mundescricao']}%'))";
}
if($_POST['estuf']){
	$arrrWhere[] = "estuf = '{$_POST['estuf']}'";
}

$sql = "select '<center><img src=\"../imagens/globo_terrestre.png\" class=\"img_middle link\" onclick=\"corrigirGeo(\'' || muncod || '\')\" /> </center>',muncod, mundescricao,estuf from(
select muncod, mundescricao,estuf,

--############### LATITUDE ###################### --
						CASE WHEN (SPLIT_PART(munmedlat, '.', 1) <>'' AND SPLIT_PART(munmedlat, '.', 2) <>'' AND split_part(munmedlat, '.', 3) <>'') THEN
			               CASE WHEN split_part(munmedlat, '.', 4) <>'N' THEN
			                   (((split_part(munmedlat, '.', 3)::double precision / 3600) +(SPLIT_PART(munmedlat, '.', 2)::double precision / 60) + (SPLIT_PART(munmedlat, '.', 1)::int)))*(-1)
			                ELSE
			                   ((SPLIT_PART(munmedlat, '.', 3)::double precision / 3600) +(SPLIT_PART(munmedlat, '.', 2)::double precision / 60) + (SPLIT_PART(munmedlat, '.', 1)::int))
			               END
			            ELSE
			            -- Valores do IBGE convertidos em  decimal
			            CASE WHEN (length (munmedlat)=8) THEN
			                CASE WHEN length(REPLACE('0' || munmedlat,'S','')) = 8 THEN
			                    ((SUBSTR(REPLACE('0' || munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || munmedlat,'S',''),1,2)::double precision))*(-1)
			                ELSE
			                    (SUBSTR(REPLACE('0' || munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || munmedlat,'N',''),1,2)::double precision)
			                END
			            ELSE
			                CASE WHEN length(REPLACE(munmedlat,'S','')) = 8 THEN
			                   ((SUBSTR(REPLACE(munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE(munmedlat,'S',''),1,2)::double precision))*(-1)
			                ELSE
			                  0--((SUBSTR(REPLACE(munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE(munmedlat,'N',''),1,2)::double precision))
			                END
			            END
			            END as latitude,
			            --############### FIM LATITUDE ###################### --

			            --############### LONGITUDE ###################### --
			            CASE WHEN (SPLIT_PART(munmedlog, '.', 1) <>'' AND SPLIT_PART(munmedlog, '.', 2) <>'' AND split_part(munmedlog, '.', 3) <>'') THEN
			               ((split_part(munmedlog, '.', 3)::double precision / 3600) +(SPLIT_PART(munmedlog, '.', 2)::double precision / 60) + (SPLIT_PART(munmedlog, '.', 1)::int))*(-1)
			            ELSE
			                -- Valores do IBGE convertidos em  decimal
			               (SUBSTR(REPLACE(munmedlog,'W',''),1,2)::double precision + (SUBSTR(REPLACE(munmedlog,'W',''),3,2)::double precision/60)) *(-1)
			            END as longitude,
			            --############### FIM LONGITUDE ###################### --

munpoligono as poli from territoriosgeo.municipio ".($arrrWhere ? " where ".implode(" and ",$arrrWhere) : "")." 
)as foo where ST_Contains( poli,ST_GeomFromText('POINT('||longitude||' '||latitude||')',4291)) = 'f' order by mundescricao,estuf"; 

$arrCabecalho = array("Ação","Muncod","Município","UF");

//$db->monta_lista($sql,$arrCabecalho,50,10,"N","center","N");?>