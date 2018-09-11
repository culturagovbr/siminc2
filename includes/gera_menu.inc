<!-- MENU -->
<!-- INCLUSÃO DO CSS PARA O MENU DO NOVO LAYOUT DO SIMEC - 15/05/10 -->
<script type="text/javascript" language="javascript" src="../includes/layout/<? echo $theme ?>/js/menu.js"></script>
<?php
 /*
   Sistema Sistema Simec
   Setor responsável: SPO/MEC
   Analista: Cristiano Cabral
   Programador: Cristiano Cabral (e-mail: cristiano.cabral@gmail.com)
   Módulo: gera_menu.inc
   Finalidade: montagem dinâmica do menu do sistema baseado em perfil de acesso
   Data de criação: 09/08/2005 - Atualizado: 07/04/2009
*/

if (!$_SESSION['menu'][$_SESSION['usucpf']][$_SESSION['sisid']] || $db->testa_superuser()) {
	$premenu = '<script>function DoPopup(url,name,width,height){ args = "width=" + width + "," + "height=" + height + "," + "location=0," + "menubar=0," + "resizable=0," + "scrollbars=1"; popupWin = window.open(url, name, args);} isNS4 = (document.layers) ? true : false;NS4 = (document.layers);IE4 = (document.all);ver4 = (NS4 || IE4);isMac = (navigator.appVersion.indexOf("Mac") != -1);isMenu = (NS4 || (IE4 && !isMac));function popUp(){return};function popDown(){return};function startIt(){return};var platform = navigator.appVersion;isUNIX = (platform.indexOf("X11") != -1) ||  (platform.indexOf("Linux") != -1) ||  (platform.indexOf("SunOS") != -1) ||  (platform.indexOf("IRIX") != -1) ||   (platform.indexOf("HP-UX") != -1);if (!ver4) event = null;browser = (((navigator.appName == "Netscape") && (parseInt(navigator.appVersion) >= 3 )) || ((navigator.appName == "Microsoft Internet Explorer") && (parseInt(navigator.appVersion) >= 4 )));function countries(){var location = document.country_dropdown.select1.options[document.country_dropdown.select1.selectedIndex].value;document.location = location;}if (NS4) {document.write(\'<layer name="searchBox2" left="20px" top="59px" width="117px" height="20px" visibility="hidden" id="searchBox2" z-index="0"><table cellpadding="0" cellspacing="0" border="0"><tr><td><a href="#" onMouseOver="javascript:restoreSearch();"><img src="stylesheet/ns4search.gif" vspace="0" border="0" alt=""><\/a><\/td><\/tr><\/table><\/layer>\');}menunum=0;menus=new Array();_d=document;function addmenu(){menunum++;menus[menunum]=menu;}function dumpmenus(){mt="<script language=javascript>";for(a=1;a<menus.length;a++){mt+=" menu"+a+"=menus["+a+"];"}mt+="<\/script>";_d.write(mt)}timegap=1000;followspeed=2;followrate=5;suboffset_top=0;suboffset_left=0;if(navigator.appVersion.indexOf("MSIE 6.0")>0){	effect = ""}else{effect = ""}';
	// Primeiro campo numerico é a distancia do top
    $premenu .= 'addmenu(menu=["mainmenu",85,10,140,0,,style2,1,"left",,1,1,1,,1,,0,,,,,';

	$lista_cpf="'".$_SESSION['usucpf']."'";
	$sql = "select usucpf_delegante as usu_superior from seguranca.delegacao_competencia where sisid=".$_SESSION['sisid']." and usucpf_delegado='".$_SESSION['usucpf']."' and dcpstatus='A' and dcpdata_ini <='".date('Y-m-d')."' and dcpdata_fim >='".date('Y-m-d')."'";

	$rs = @$db->carregar( $sql );
	if (  $rs && count($rs) > 0 )
		{
		 $i=0;
		 foreach ( $rs as $linha )
			{
			 foreach($linha as $k=>$v) ${$k}=$v;
	       	 $lista_cpf .=",'".$usu_superior."'";
	  		 $i++;
			}
		}

	//Menu Principal (Superior Horizontal)
	 $sql= "select distinct mnu.mnucod,mnu.mnuid , mnu.mnuidpai , mnu.mnudsc , mnu.mnustatus , mnu.mnulink ,
	 		mnu.mnutipo , mnu.mnustile , mnu.mnuhtml , mnu.mnusnsubmenu , mnu.mnutransacao , mnu.mnushow , mnu.abacod
	 		from seguranca.menu mnu, seguranca.perfilmenu pmn, seguranca.perfil pfl, seguranca.perfilusuario pfu
	 		where mnu.mnuid=pmn.mnuid and pmn.pflcod=pfl.pflcod and pfl.pflcod=pfu.pflcod
	 		and pfu.usucpf in (".$lista_cpf.") and mnu.mnutipo=1 and mnu.mnushow='t' and mnu.mnustatus='A'
	 		and mnu.sisid=".$_SESSION['sisid']."
	 		order by mnu.mnucod,mnu.mnuid , mnu.mnuidpai , mnu.mnudsc";

	$rs = @$db->carregar( $sql );
	
	if (  $rs && count($rs) > 0 ) {
		 $i=0;
		 foreach ( $rs as $linha ) {
			 foreach($linha as $k=>$v) ${$k}=$v;
			 if ($mnusnsubmenu == "f") {
			 	$premenu .=',"' . $mnuhtml . '","# ';
			 	if ($mnustile <> "")
			 		$premenu .= $mnustile; $premenu .= '","' . $mnulink . '","' . str_replace("/","\/",$mnutransacao); $premenu .= '",1';
			 }elseif ($mnulink=="") {
			 	$premenu .= ',"';
			 	if ($mnuhtml <> "") {
			 		$premenu .= $mnuhtml;
			 	} else {
			 		$premenu .= str_replace("/","\/",$mnudsc);
			 	}
			 	$premenu .= '","show-menu=' . str_replace("/","\/",$mnudsc);
			 	$premenu .= '","# ';
			 	if ($mnustile <> "") {
			 		$premenu .= $mnustile;
			 	}
			 	$premenu .= '","' . str_replace("/","\/",$mnutransacao);
			 	$premenu .= '",1';
			 	//ver($premenu);
			 } else {
			 	$premenu .= ',"';
			 	if ($mnuhtml <> "") {
			 		$premenu .= $mnuhtml;
			 	} else {
			 		$premenu .= str_replace("/","\/",$mnudsc);
			 	}
			 	$premenu .='","' . $mnulink . '","# ","' . str_replace("/","\/",$mnutransacao);
			 	$premenu .= '",1';
			}
			$i++;
		}
	}
	$premenu .= ',"&nbsp;&nbsp;<img src=../imagens/menu/bt_menu_close.png align=top border=0>&nbsp;&nbsp;Sair&nbsp;&nbsp;","../logout.php", " ", ,1]);';



	//ZSub-Menus Verticais
	$sql2= "select distinct  mnu.mnuid , mnu.mnuidpai , mnu.mnudsc , mnu.mnustatus , mnu.mnulink , mnu.mnutipo , mnu.mnustile ,
			mnu.mnuhtml , mnu.mnusnsubmenu , mnu.mnutransacao , mnu.mnushow , mnu.abacod, mnu2.mnudsc as mnudscpai
			from seguranca.menu mnu, seguranca.menu mnu2, seguranca.perfilmenu pmn, seguranca.perfil pfl, seguranca.perfilusuario pfu
			where mnu2.mnuid=mnu.mnuidpai and mnu.sisid=mnu2.sisid and mnu.mnuid=pmn.mnuid and pmn.pflcod=pfl.pflcod and pfl.pflcod=pfu.pflcod
			and pfu.usucpf in (".$lista_cpf.") and mnu.mnuidpai is not null and mnu.mnushow= 't' and mnu.mnustatus='A'
			and mnu.sisid=".$_SESSION['sisid']."
			order by mnu.mnutipo, mnu.mnuidpai, mnu.mnuid,mnu.mnudsc";
	
	$rs = @$db->carregar( $sql2 );
	
	if (  $rs && count($rs) > 0 ) {
		$i=0;
		foreach ( $rs as $linha ) {
			foreach($linha as $k=>$v) ${$k}=$v;
	       	if ($submenu<>$mnudscpai){
				if ($i>0) $premenu .=  "]);";
				$premenu .= chr(10);
				$premenu .= 'addmenu(menu=["' . $mnudscpai . '","offset=3",,150,1,"",style1,,,effect,,,,,,,,,,,,';
				$submenu=$mnudscpai;
			}
			if ($mnulink==""){
					$premenu .= ',"';
					if ($mnuhtml <> "") {
						$premenu .=  $mnuhtml;
					} else {
						$premenu .=  str_replace("/","\/",$mnudsc);
					}
					$premenu .= '","show-menu=' . str_replace("/","\/",$mnudsc);
					$premenu .= '","# ';
					if ($mnustile <> "") {
						$premenu .=  $mnustile;
					}
					$premenu .= '","' . str_replace("/","\/",$mnutransacao);
					$premenu .= '",1';
			}else{
				$premenu .= ',"';
				if ($mnuhtml <> "") {
					$premenu .=  $mnuhtml;
				} else {
					$premenu .=  str_replace("/","\/",$mnudsc);
				}
				$premenu .= '","' . $mnulink . '","# ","' . str_replace("/","\/",$mnutransacao); $premenu .= '",1';
			}
			$i++;
		}
		$premenu .= ']); ';
	}
	$premenu .= chr(10);
	$premenu .= 'dumpmenus();</script>';
	$_SESSION['menu'][$_SESSION['usucpf']][$_SESSION['sisid']] = $premenu;
}
print $_SESSION['menu'][$_SESSION['usucpf']][$_SESSION['sisid']];
?>
<script type="text/javascript" language="javascript" src="../includes/mmenu.js"></script>
<? 
unset($mnucod , $mnucodpai , $mnudsc , $mnustatus , $mnulink , $mnutipo , $mnustile , $mnuhtml , $mnusnsubmenu , $mnutransacao , $mnushow , $abacod , $mnudscpai, $premenu);
?>