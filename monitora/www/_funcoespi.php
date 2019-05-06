<?php

/* Função para recuperar o nome e a cor da situação, passada por parâmetro (código) */

function carregardadosplanotrabalhoUN_raiz() {

    arConfiguracoesPerfis($arPerfilUnidadeOrcamento, $arPerfilUnidadePlanejamento);
    $boPerfilSomenteLeitura = boPerfilSomenteLeitura();
    $boNaoVePlanoInterno = boNaoVePlanoInterno();

    if (possui_perfil(PERFIL_MONITORA_SUPERUSUARIO) || possui_perfil(PERFIL_UNIDMONITORAAVALIA) || possui_perfil(PERFIL_CGP) ) {
        $menu = array(0 => array("id" => 1, "descricao" => "Lista de unidades", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/listaUN&acao=A"),
            1 => array("id" => 2, "descricao" => "Plano de trabalho", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/subatividadesUN&acao=A&unicod=" . $_SESSION['monitora_var']['unicod']),
            2 => array("id" => 3, "descricao" => "Subação", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/cadsubacaoUN&acao=A"),
            //3 => array("id" => 4, "descricao" => "Plano Interno", 	  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/cadastro_piUN&acao=A")
            3 => array("id" => 4, "descricao" => "Pesquisa Plano Interno", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/pesquisa_piUN&acao=A"),
            4 => array("id" => 5, "descricao" => "Cadastro Plano Interno", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/cadastro_piUN&acao=A")
        );
    } elseif (possui_perfil($arPerfilUnidadeOrcamento) && !possui_perfil($arPerfilUnidadePlanejamento)) {
        if (!$boPerfilSomenteLeitura) {
            // monta menu padrão contendo informações sobre as entidades
            if ($_SESSION['monitora_var']['boMostraAbaListaUnidadeUN']) {
                $menu = array(0 => array("id" => 1, "descricao" => "Lista de unidades", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/listaUN&acao=A"),
                    1 => array("id" => 2, "descricao" => "Plano de trabalho", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/subatividadesUN&acao=A&unicod=" . $_SESSION['monitora_var']['unicod']),
                    2 => array("id" => 3, "descricao" => "Subação", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/cadsubacaoUN&acao=A"),
                    //3 => array("id" => 4, "descricao" => "Plano Interno", 	  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/cadastro_piUN&acao=A")
                    3 => array("id" => 4, "descricao" => "Pesquisa Plano Interno", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/pesquisa_piUN&acao=A"),
                    4 => array("id" => 5, "descricao" => "Cadastro Plano Interno", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/cadastro_piUN&acao=A")
                );
            } else {
                $menu = array(0 => array("id" => 1, "descricao" => "Plano de trabalho", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/subatividadesUN&acao=A&unicod=" . $_SESSION['monitora_var']['unicod']),
                    1 => array("id" => 2, "descricao" => "Subação", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/cadsubacaoUN&acao=A"),
                    //2 => array("id" => 3, "descricao" => "Plano Interno", 	  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/cadastro_piUN&acao=A")
                    2 => array("id" => 3, "descricao" => "Pesquisa Plano Interno", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/pesquisa_piUN&acao=A"),
                    3 => array("id" => 4, "descricao" => "Cadastro Plano Interno", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/cadastro_piUN&acao=A")
                );
            }
        } else {
            // monta menu padrão contendo informações sobre as entidades
            if ($_SESSION['monitora_var']['boMostraAbaListaUnidadeUN']) {
                $menu = array(0 => array("id" => 1, "descricao" => "Lista de unidades", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/listaUN&acao=A"),
                    1 => array("id" => 2, "descricao" => "Plano de trabalho", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/subatividadesUN&acao=A&unicod=" . $_SESSION['monitora_var']['unicod']),
                    //2 => array("id" => 3, "descricao" => "Plano Interno", 	  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/cadastro_piUN&acao=A")
                    2 => array("id" => 3, "descricao" => "Pesquisa Plano Interno", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/pesquisa_piUN&acao=A"),
                    3 => array("id" => 4, "descricao" => "Cadastro Plano Interno", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/cadastro_piUN&acao=A")
                );
            } else {
                $menu = array(0 => array("id" => 1, "descricao" => "Plano de trabalho", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/subatividadesUN&acao=A&unicod=" . $_SESSION['monitora_var']['unicod']),
                    //1 => array("id" => 2, "descricao" => "Plano Interno", 	  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/cadastro_piUN&acao=A")
                    1 => array("id" => 2, "descricao" => "Pesquisa Plano Interno", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/pesquisa_piUN&acao=A"),
                    2 => array("id" => 3, "descricao" => "Cadastro Plano Interno", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/cadastro_piUN&acao=A")
                );
            }
        }
    } elseif (!possui_perfil($arPerfilUnidadeOrcamento) && possui_perfil($arPerfilUnidadePlanejamento)) {
        if (!$boNaoVePlanoInterno) {
            // monta menu padrão contendo informações sobre as entidades
            if ($_SESSION['monitora_var']['boMostraAbaListaUnidadeUN']) {
                $menu = array(0 => array("id" => 1, "descricao" => "Lista de unidades", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/listaUN&acao=A"),
                    1 => array("id" => 2, "descricao" => "Plano de trabalho", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/subatividadesUN&acao=A&unicod=" . $_SESSION['monitora_var']['unicod']),
                    2 => array("id" => 3, "descricao" => "Subação", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/cadsubacaoUN&acao=A"),
                    //3 => array("id" => 4, "descricao" => "Plano Interno", 	  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/cadastro_piUN&acao=A")
                    3 => array("id" => 4, "descricao" => "Pesquisa Plano Interno", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/pesquisa_piUN&acao=A"),
                    4 => array("id" => 5, "descricao" => "Cadastro Plano Interno", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/cadastro_piUN&acao=A")
                );
            } else {
                $menu = array(0 => array("id" => 1, "descricao" => "Plano de trabalho", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/subatividadesUN&acao=A&unicod=" . $_SESSION['monitora_var']['unicod']),
                    1 => array("id" => 2, "descricao" => "Subação", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/cadsubacaoUN&acao=A"),
                    //2 => array("id" => 3, "descricao" => "Plano Interno", 	  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/cadastro_piUN&acao=A")
                    2 => array("id" => 3, "descricao" => "Pesquisa Plano Interno", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/pesquisa_piUN&acao=A"),
                    3 => array("id" => 4, "descricao" => "Cadastro Plano Interno", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/cadastro_piUN&acao=A")
                );
            }
        } else {
            // monta menu padrão contendo informações sobre as entidades
            if ($_SESSION['monitora_var']['boMostraAbaListaUnidadeUN']) {
                $menu = array(0 => array("id" => 1, "descricao" => "Lista de unidades", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/listaUN&acao=A"),
                    1 => array("id" => 2, "descricao" => "Plano de trabalho", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/subatividadesUN&acao=A&unicod=" . $_SESSION['monitora_var']['unicod']),
                    2 => array("id" => 3, "descricao" => "Subação", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/cadsubacaoUN&acao=A")
                );
            } else {
                $menu = array(0 => array("id" => 1, "descricao" => "Plano de trabalho", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/subatividadesUN&acao=A&unicod=" . $_SESSION['monitora_var']['unicod']),
                    1 => array("id" => 2, "descricao" => "Subação", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/cadsubacaoUN&acao=A")
                );
            }
        }
    } elseif (possui_perfil($arPerfilUnidadeOrcamento) && possui_perfil($arPerfilUnidadePlanejamento)) {
        // monta menu padrão contendo informações sobre as entidades
        if ($_SESSION['monitora_var']['boMostraAbaListaUnidadeUN']) {
            $menu = array(0 => array("id" => 1, "descricao" => "Lista de unidades", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/listaUN&acao=A"),
                1 => array("id" => 2, "descricao" => "Plano de trabalho", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/subatividadesUN&acao=A&unicod=" . $_SESSION['monitora_var']['unicod']),
                2 => array("id" => 3, "descricao" => "Subação", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/cadsubacaoUN&acao=A"),
                3 => array("id" => 4, "descricao" => "Plano Interno", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/cadastro_piUN&acao=A"),
                4 => array("id" => 5, "descricao" => "Pesquisa Plano Interno", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/pesquisa_piUN&acao=A"),
                5 => array("id" => 6, "descricao" => "Cadastro Plano Interno", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/cadastro_piUN&acao=A")
            );
        } else {
            $menu = array(0 => array("id" => 1, "descricao" => "Plano de trabalho", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/subatividadesUN&acao=A&unicod=" . $_SESSION['monitora_var']['unicod']),
                1 => array("id" => 2, "descricao" => "Subação", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/cadsubacaoUN&acao=A"),
                2 => array("id" => 3, "descricao" => "Plano Interno", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/cadastro_piUN&acao=A"),
                3 => array("id" => 4, "descricao" => "Pesquisa Plano Interno", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/pesquisa_piUN&acao=A"),
                4 => array("id" => 5, "descricao" => "Cadastro Plano Interno", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/cadastro_piUN&acao=A")
            );
        }
    }

    return $menu;
}

function carregardadosplanotrabalhoUG_raiz() {
	// monta menu padrão contendo informações sobre as entidades
	if($_SESSION['monitora_var']['boMostraAbaListaUnidadeUG']){
		$menu = array(0 => array("id" => 1, "descricao" => "Lista de unidades",   "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUG/listaUG&acao=A"),
					  1 => array("id" => 2, "descricao" => "Plano de trabalho",	  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUG/subatividadesUG&acao=A&ungcod=".$_SESSION['monitora_var']['ungcod']),
					  //2 => array("id" => 3, "descricao" => "Plano Interno", 	  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUG/cadastro_piUG&acao=A")
					  2 => array("id" => 3, "descricao" => "Pesquisa Plano Interno","link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUG/pesquisa_piUG&acao=A"),
					  3 => array("id" => 4, "descricao" => "Cadastro Plano Interno", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUG/cadastro_piUG&acao=A")
				  	  );		
	} else {
		$menu = array(0 => array("id" => 1, "descricao" => "Plano de trabalho",	  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUG/subatividadesUG&acao=A&ungcod=".$_SESSION['monitora_var']['ungcod']),
					  //1 => array("id" => 2, "descricao" => "Plano Interno", 	  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUG/cadastro_piUG&acao=A")
					  1 => array("id" => 2, "descricao" => "Pesquisa Plano Interno","link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUG/pesquisa_piUG&acao=A"),
					  2 => array("id" => 3, "descricao" => "Cadastro Plano Interno", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUG/cadastro_piUG&acao=A")
				  	  );
	}
	return $menu;
}

function carregardadosplanotrabalhoUN_sub() {
	$boNaoVePlanoInterno = boNaoVePlanoInterno();
	
	if(!$boNaoVePlanoInterno){
		// monta menu padrão contendo informações sobre as entidades
		if($_SESSION['monitora_var']['boMostraAbaListaUnidadeUN']){
			$menu = array(0 => array("id" => 1, "descricao" => "Lista de unidades",   "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/listaUN&acao=A"),
						  1 => array("id" => 2, "descricao" => "Plano de trabalho",	  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/subatividadesUN&acao=A&atiid=".$_REQUEST['atiid']),
						  2 => array("id" => 3, "descricao" => "Informações Gerais",  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/atividadeUN&acao=A&atiid=".$_REQUEST['atiid']),
						  3 => array("id" => 4, "descricao" => "Restrições",    	  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/controleUN&acao=A&atiid=".$_REQUEST['atiid']),
						  4 => array("id" => 5, "descricao" => "Documentos",   		  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/instrumentoUN&acao=A&atiid=".$_REQUEST['atiid']),
						  5 => array("id" => 6, "descricao" => "Observações",  		  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/observacaoUN&acao=A&atiid=".$_REQUEST['atiid']),
						  6 => array("id" => 7, "descricao" => "Financeiro",  		  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/planoInternoUN&acao=R&atiid=".$_REQUEST['atiid']),
						  7 => array("id" => 8, "descricao" => "Equipe",	 		  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/equipeUN&acao=A&atiid=".$_REQUEST['atiid']),
						  //8 => array("id" => 9, "descricao" => "Plano Interno",		  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/cadastro_piUN&acao=A&atiid=".$_REQUEST['atiid'])
						  8 => array("id" => 9, "descricao" => "Pesquisa Plano Interno", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/pesquisa_piUN&acao=A&atiid=".$_REQUEST['atiid']),
					  	  9 => array("id" => 10, "descricao" => "Cadastro Plano Interno", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/cadastro_piUN&acao=A&atiid=".$_REQUEST['atiid'])
					  	  );
		} else {
			$menu = array(0 => array("id" => 1, "descricao" => "Plano de trabalho",	  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/subatividadesUN&acao=A&atiid=".$_REQUEST['atiid']),
						  1 => array("id" => 2, "descricao" => "Informações Gerais",  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/atividadeUN&acao=A&atiid=".$_REQUEST['atiid']),
						  2 => array("id" => 3, "descricao" => "Restrições",    	  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/controleUN&acao=A&atiid=".$_REQUEST['atiid']),
						  3 => array("id" => 4, "descricao" => "Documentos",   		  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/instrumentoUN&acao=A&atiid=".$_REQUEST['atiid']),
						  4 => array("id" => 5, "descricao" => "Observações",  		  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/observacaoUN&acao=A&atiid=".$_REQUEST['atiid']),
						  5 => array("id" => 6, "descricao" => "Financeiro",  		  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/planoInternoUN&acao=R&atiid=".$_REQUEST['atiid']),
						  6 => array("id" => 7, "descricao" => "Equipe",	 		  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/equipeUN&acao=A&atiid=".$_REQUEST['atiid']),
						  //7 => array("id" => 8, "descricao" => "Plano Interno",		  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/cadastro_piUN&acao=A&atiid=".$_REQUEST['atiid'])
						  7 => array("id" => 8, "descricao" => "Pesquisa Plano Interno", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/pesquisa_piUN&acao=A&atiid=".$_REQUEST['atiid']),
					  	  8 => array("id" => 9, "descricao" => "Cadastro Plano Interno", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/cadastro_piUN&acao=A&atiid=".$_REQUEST['atiid'])
		  	  			 );
		}
	} else {
		// monta menu padrão contendo informações sobre as entidades
		if($_SESSION['monitora_var']['boMostraAbaListaUnidadeUN']){
			$menu = array(0 => array("id" => 1, "descricao" => "Lista de unidades",   "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/listaUN&acao=A"),
						  1 => array("id" => 2, "descricao" => "Plano de trabalho",	  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/subatividadesUN&acao=A&atiid=".$_REQUEST['atiid']),
						  2 => array("id" => 3, "descricao" => "Informações Gerais",  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/atividadeUN&acao=A&atiid=".$_REQUEST['atiid']),
						  3 => array("id" => 4, "descricao" => "Restrições",    	  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/controleUN&acao=A&atiid=".$_REQUEST['atiid']),
						  4 => array("id" => 5, "descricao" => "Documentos",   		  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/instrumentoUN&acao=A&atiid=".$_REQUEST['atiid']),
						  5 => array("id" => 6, "descricao" => "Observações",  		  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/observacaoUN&acao=A&atiid=".$_REQUEST['atiid']),
						  6 => array("id" => 7, "descricao" => "Financeiro",  		  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/planoInternoUN&acao=R&atiid=".$_REQUEST['atiid']),
						  7 => array("id" => 8, "descricao" => "Equipe",	 		  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/equipeUN&acao=A&atiid=".$_REQUEST['atiid'])
					  	  );
		} else {
			$menu = array(0 => array("id" => 1, "descricao" => "Plano de trabalho",	  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/subatividadesUN&acao=A&atiid=".$_REQUEST['atiid']),
						  1 => array("id" => 2, "descricao" => "Informações Gerais",  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/atividadeUN&acao=A&atiid=".$_REQUEST['atiid']),
						  2 => array("id" => 3, "descricao" => "Restrições",    	  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/controleUN&acao=A&atiid=".$_REQUEST['atiid']),
						  3 => array("id" => 4, "descricao" => "Documentos",   		  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/instrumentoUN&acao=A&atiid=".$_REQUEST['atiid']),
						  4 => array("id" => 5, "descricao" => "Observações",  		  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/observacaoUN&acao=A&atiid=".$_REQUEST['atiid']),
						  5 => array("id" => 6, "descricao" => "Financeiro",  		  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/planoInternoUN&acao=R&atiid=".$_REQUEST['atiid']),
						  6 => array("id" => 7, "descricao" => "Equipe",	 		  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUN/equipeUN&acao=A&atiid=".$_REQUEST['atiid'])
					  	  );
		}
	}
	
	return $menu;
	
}

function carregardadosplanotrabalhoUG_sub() {
	// monta menu padrão contendo informações sobre as entidades
	if($_SESSION['monitora_var']['boMostraAbaListaUnidadeUG']){
		$menu = array(0 => array("id" => 1, "descricao" => "Lista de unidades",   "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUG/listaUG&acao=A"),
					  1 => array("id" => 2, "descricao" => "Plano de trabalho",	  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUG/subatividadesUG&acao=A&atiid=".$_REQUEST['atiid']),
					  2 => array("id" => 3, "descricao" => "Informações Gerais",  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUG/atividadeUG&acao=A&atiid=".$_REQUEST['atiid']),
					  3 => array("id" => 4, "descricao" => "Restrições",    	  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUG/controleUG&acao=A&atiid=".$_REQUEST['atiid']),
					  4 => array("id" => 5, "descricao" => "Documentos",   		  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUG/instrumentoUG&acao=A&atiid=".$_REQUEST['atiid']),
					  5 => array("id" => 6, "descricao" => "Observações",  		  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUG/observacaoUG&acao=A&atiid=".$_REQUEST['atiid']),
					  6 => array("id" => 7, "descricao" => "Financeiro",  		  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUG/planoInternoUG&acao=A&atiid=".$_REQUEST['atiid']),
					  7 => array("id" => 8, "descricao" => "Equipe",	 		  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUG/equipeUG&acao=A&atiid=".$_REQUEST['atiid']),
					  //8 => array("id" => 9, "descricao" => "Plano Interno",  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUG/cadastro_piUG&acao=A&atiid=".$_REQUEST['atiid'])
					  8 => array("id" => 9, "descricao" => "Pesquisa Plano Interno", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUG/pesquisa_piUG&acao=A&atiid=".$_REQUEST['atiid']),
					  9 => array("id" => 10, "descricao" => "Cadastro Plano Interno",  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUG/cadastro_piUG&acao=A&atiid=".$_REQUEST['atiid'])
				  	  );
	} else {
		$menu = array(0 => array("id" => 1, "descricao" => "Plano de trabalho",	  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUG/subatividadesUG&acao=A&atiid=".$_REQUEST['atiid']),
					  1 => array("id" => 2, "descricao" => "Informações Gerais",  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUG/atividadeUG&acao=A&atiid=".$_REQUEST['atiid']),
					  2 => array("id" => 3, "descricao" => "Restrições",    	  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUG/controleUG&acao=A&atiid=".$_REQUEST['atiid']),
					  3 => array("id" => 4, "descricao" => "Documentos",   		  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUG/instrumentoUG&acao=A&atiid=".$_REQUEST['atiid']),
					  4 => array("id" => 5, "descricao" => "Observações",  		  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUG/observacaoUG&acao=A&atiid=".$_REQUEST['atiid']),
					  5 => array("id" => 6, "descricao" => "Financeiro",  		  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUG/planoInternoUG&acao=A&atiid=".$_REQUEST['atiid']),
					  6 => array("id" => 7, "descricao" => "Equipe",	 		  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUG/equipeUG&acao=A&atiid=".$_REQUEST['atiid']),
					  //7 => array("id" => 8, "descricao" => "Plano Interno",  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUG/cadastro_piUG&acao=A&atiid=".$_REQUEST['atiid'])
					  7 => array("id" => 8, "descricao" => "Pesquisa Plano Interno", "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUG/pesquisa_piUG&acao=A&atiid=".$_REQUEST['atiid']),
					  8 => array("id" => 9, "descricao" => "Cadastro Plano Interno",  "link" => "/monitora/monitora.php?modulo=principal/planotrabalhoUG/cadastro_piUG&acao=A&atiid=".$_REQUEST['atiid'])
				  	  );
	}
	return $menu;
	
}



function validaCodPi($pi, $pliid = false){
	global $db;
	$sql = "SELECT plicod FROM monitora.pi_planointerno WHERE plistatus='A' AND date_part('Y', plidata) = '{$_SESSION['exercicio']}' AND plicod = '{$pi}'".(($pliid)?" AND pliid != '".$pliid."'":"");
	// ver($sql,d);
	$plicod = $db->PegaUm($sql);
	
	if(!$plicod) {
		# Comentando a pedido do Henrique, no dia 04/03/2010
		/*$piaux = substr($pi, 0, 6);
		$sql = "SELECT DISTINCT plicod, plititulo FROM monitora.pi_planointerno WHERE plistatus='A' AND plicod like '%{$piaux}%' ".(($pliid)?" AND pliid != '".$pliid."'":"")." ORDER BY plititulo";
		$dados = (array) $db->carregar($sql);
		if($dados[0]){
			$cabecalho = array('Cód PI','Título');
			$retorno = $db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '' );
			echo $retorno;
			exit;
		}
		else{*/
			$retorno = "";
			echo $retorno;
			exit;
		//}
	} else {
		$retorno = "pijaexiste";
		echo $retorno;
		$sql = "SELECT p.plicod as plicod, coalesce(p.plititulo,'Não preenchido') as titulo, 
				coalesce(SUM(pp.pipvalor),0) as total, 
				CASE WHEN p.plisituacao = 'P' THEN ' Pendente ' WHEN p.plisituacao = 'C' THEN ' Aprovado ' 
				     WHEN p.plisituacao = 'H' THEN ' Homologado ' WHEN p.plisituacao = 'V' THEN ' Revisado ' 
				     WHEN p.plisituacao = 'S' THEN ' Cadastrado no SIAFI ' WHEN p.plisituacao = 'R' THEN ' Enviado para Revisão ' END as situacao, 
				u.usunome ||' por '||to_char(p.plidata, 'dd/mm/YYYY hh24:mi'), 
				COALESCE(a._atinumero||' - '||a.atidescricao, 'Não atribuido')as atividade
				FROM monitora.pi_planointerno p 
				LEFT JOIN monitora.pi_planointernoptres pp ON  pp.pliid=p.pliid
				LEFT JOIN seguranca.usuario u ON u.usucpf = p.usucpf 
				LEFT JOIN monitora.pi_planointernoatividade pa on pa.pliid = p.pliid 
				LEFT JOIN pde.atividade a on a.atiid = pa.atiid 
				WHERE p.plicod='".$plicod."' AND p.plistatus = 'A' 
				GROUP BY p.plicod,p.plititulo,u.usunome,p.plidata,p.plisituacao,atividade 
				ORDER BY p.plidata DESC";
		$cabecalho = array("Código PI","Título","Total PI","Situação","Dados inserção","Atividade");
		$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '' );
		exit;
	}
}


function buscaDadosSubacao($sbaid, $capid = "") {
	global $db;
	
	$sql = "SELECT * FROM monitora.pi_subacao WHERE sbaid='".$sbaid."'";
	$subacao = $db->pegaLinha($sql);
	
	$categoria = "";
	if($capid){
		$sql = "SELECT capdsc FROM monitora.pi_categoriaapropriacao WHERE capid='".$capid."'";
		$categoria = $db->pegaUm($sql);	
	}
	
	/* 
	if( trim( $subacao['sbasigla'] ) != '' ){
		echo $subacao['sbacod']."!@#".$subacao['sbasigla']."!@#".$categoria;
	}else{
		echo $subacao['sbacod']."!@#".$subacao['sbatitulo']."!@#".$categoria;
	}	
	*/
	echo $subacao['sbacod']."!@#".$subacao['sbasigla']."!@#".$categoria;
}


function carregarComboEnquadramentoPorSubacao($sbaid) {
	global $db;
	
	if($sbaid){
		$sql = "SELECT ed.eqdid as codigo, ed.eqdcod ||' - '|| ed.eqddsc as descricao
		    FROM planejamento.enquadramento_despesa ed
		    	INNER JOIN monitora.pi_subacaoenquadramento se on ed.eqdid = se.eqdid 
		    WHERE ed.eqdano='".$_SESSION['exercicio']."' and ed.eqdstatus='A' and se.sbaid=$sbaid
		    ORDER BY ed.eqdcod";
		$arDados = $db->carregar($sql);
		if(count($arDados) && $arDados[0]){
			$eqdid = $arDados[0]['codigo'];
		}
		die($db->monta_combo('eqdid', $sql, 'S', 'Selecione', 'atualizarPrevisaoPI', '', '', '240', 'S', 'eqdid', false, $eqdid));
	}	
}


/**
 * @return boolean
 */
function usuario_possui_perfil2( $perfil, $usuario = null ){
	global $db;
//	if ( $db->testa_superuser() ) {
//		return true;
//	}
	$usuario = $usuario ? $usuario : $_SESSION['usucpf'];
	$sql = sprintf(
		"select count( * )
		from seguranca.perfilusuario
		where
			usucpf = '%s' and
			pflcod = %d",
		$usuario,
		$perfil
	);
	return (boolean) $db->pegaUm( $sql );
}

?>
