	<!-- Navigation Modulos -->
	<?php
            // abre conexão com o servidor de banco de dados
//            $db = new cls_banco();
//            
//	    $sql = "
//                SELECT
//                    sisid,
//                    sisabrev,
//                    sisdsc,
//                    sisfinalidade,
//                    sispublico,
//                    sisrelacionado
//                FROM seguranca.sistema
//                WHERE
//                    sisstatus='A'
//                    AND sismostra IS TRUE
//                ORDER BY
//                    sisid
//            ";
//
//	    $sistemas = $db->carregar($sql);
        $sistemas = array();
        $sistemas[] = array('sisid' => 157, 'sisdsc' => 'Planejamento Orçamentário');
        $sistemas[] = array('sisid' => 251, 'sisdsc' => 'Emendas Parlamentares');
        $sistemas[] = array('sisid' => 255, 'sisdsc' => 'Monitoramento');
        $sistemas[] = array('sisid' => 255, 'sisdsc' => 'Proposta Orçamentária');
        $sistemas[] = array('sisid' => 48, 'sisdsc' => 'Gestão Estratégica');
    ?>
    <a id="menu-toggle" data-toggle="sidebar-wrapper-modulos" href="#" class="btn btn-circle btn-dark btn-modulos btn-lg menu-toggle toggle hidden-xs" data-tooltip="left" data-placement="top" data-original-title="Modulos"><i class="fa fa-th"></i></a>
    <nav id="sidebar-wrapper" class="sidebar-wrapper sidebar-wrapper-modulos">
        <ul class="sidebar-nav sidebar-modulos">
            <a id="menu-close" data-toggle="sidebar-wrapper-modulos" href="#" class="btn btn-light btn-lg pull-right menu-close toggle"><i class="fa fa-times"></i></a>
            <li class="sidebar-brand">
                <i class="fa menu-icon fa-th"></i> Modulos
            </li>
                <?php if($sistemas) { ?>
                    <?php foreach ($sistemas as $count => $sistemaItem) : ?>
                        <li class="item">
                            <div class="item-hover">
                                <div class="item-text"><?php echo $sistemaItem['sisdsc']; ?></div>
                                <div class="item-menu">
                                    <a class="btn-item" href="javascript:janela('/geral/fale_conosco.php?sisid=<?php echo $sistemaItem['sisid']; ?>',850,550)"><i class="fa fa-info-circle"></i></a>
                                    <a class="btn-item" href="cadastrar_usuario.php?sisid=<?php echo $sistemaItem['sisid']; ?>"><i class="fa fa-clipboard" title="Solicitar acesso"></i></a>
                                </div>
                            <div>
                        </li>
                    <?php endforeach; ?>
                <?php } ?>
        </ul>
    </nav>

	<!-- Navigation Prêmios -->
	<?php
            $premios = array
            (
                'Prêmio CGU 2014 - Fortalecimento dos Controles Internos Administrativos' => 'estrutura/temas/default/img/premios/premio-cgu.jpg',
                'E-Gov 2013' => 'estrutura/temas/default/img/premios/egov2013.jpg',
                'E-Gov 2012' => 'estrutura/temas/default/img/premios/premioe-gov2012.jpg',
                'E-Gov 2011' => 'estrutura/temas/default/img/premios/premiogovernoti2011.jpg',
                'Excelência em Inovação na Gestão Pública - 2010' => 'estrutura/temas/default/img/premios/conip.jpg',
                'E-Gov 2009' => 'estrutura/temas/default/img/premios/premioe-gov.jpg',
                'Selo Inovação' => 'estrutura/temas/default/img/premios/selo-inovacao.jpg',
            );
	?>
    <a id="menu-toggle" data-toggle="sidebar-wrapper-premios" href="#" class="btn btn-circle btn-dark btn-premios btn-lg menu-toggle toggle hidden-xs" data-tooltip="left" data-placement="top" data-original-title="Prêmios"><i class="fa fa-trophy"></i></a>
    <nav id="sidebar-wrapper" class="sidebar-wrapper sidebar-wrapper-premios">
        <ul class="sidebar-nav sidebar-premios">
            <a id="menu-close" href="#" data-toggle="sidebar-wrapper-premios" class="btn btn-light btn-lg pull-right menu-close toggle"><i class="fa fa-times"></i></a>
            <li class="sidebar-brand">
                <i class="fa menu-icon fa-trophy"></i> Prêmios
            </li>
            <?php foreach($premios as $premioItem => $premioImagem) : ?>
                <li class="">
                    <img class="img-responsive img-thumbnail" src="<?php echo $premioImagem; ?>" alt="<?php echo $premioItem; ?>" title="<?php echo $premioItem; ?>">
                </li>
            <?php endforeach; ?>
        </ul>
    </nav>
	
	<!-- Navigation Avisos -->
	<?php 
//            $sql = "
//                SELECT
//                    ifmtitulo as titulo, ifmtexto as texto, arqid
//                FROM seguranca.informes
//                WHERE
//                    ifmstatus = 'A'
//                    AND ifmmodal IS FALSE
//                    AND (
//                        (CURRENT_TIMESTAMP >=ifmdatainicio and  ifmdatafim is null) OR
//                        (CURRENT_TIMESTAMP between ifmdatainicio and ifmdatafim)
//                    )
//                ORDER BY
//                    ifmid
//            ";
//
//            $informes = $db->carregar($sql);
	?>
    <a id="menu-toggle" data-toggle="sidebar-wrapper-avisos" href="#" class="btn btn-circle btn-dark btn-avisos btn-lg menu-toggle toggle hidden-xs" data-tooltip="left" data-placement="top" data-original-title="Avisos"><i class="fa fa-comments"></i></a>
    <nav id="sidebar-wrapper" class="sidebar-wrapper sidebar-wrapper-avisos">
        <ul class="sidebar-nav sidebar-avisos">
            <a id="menu-close" data-toggle="sidebar-wrapper-avisos" href="#" class="btn btn-light btn-lg pull-right menu-close toggle"><i class="fa fa-times"></i></a>
            <li class="sidebar-brand">
                <i class="fa menu-icon fa-comments"></i> Avisos
            </li>
            <?php if($informes): ?>
                <?php foreach($informes as $informeItem) : ?>
                    <li class="news">
                        <h6><?php echo $informeItem['titulo']; ?></h6>
                        <p>
                        <?php 
                            if($informeItem['arqid']){
                                $link = '<a href="javascript:dinfo('.$informeItem['arqid'].')">Clique Aqui</a>';
                                echo str_replace("[LINK]", $link, $informeItem['texto']); 
                            }else{
                                echo $informeItem['texto']; 
                            }
                        ?>
                        </p>
                    </li>
                <?php endforeach; ?>
            <?php endif; ?>
        </ul>
    </nav>
	
	<!-- Navigation -->
    <a id="menu-toggle" data-toggle="sidebar-wrapper-documentos" href="#" class="btn btn-circle btn-dark btn-documentos btn-lg menu-toggle toggle hidden-xs" data-tooltip="left" data-placement="top" data-original-title="Validação de documentos"><i class="fa fa-check-square"></i></a>
    <nav id="sidebar-wrapper" class="sidebar-wrapper sidebar-wrapper-documentos">
        <ul class="sidebar-nav sidebar-documentos">
            <a id="menu-close" data-toggle="sidebar-wrapper-documentos" href="#" class="btn btn-light btn-lg pull-right menu-close toggle"><i class="fa fa-times"></i></a>
            <li class="sidebar-brand">
                <i class="fa menu-icon fa-check-square"></i> Documentos
            </li>
            <li class="">
                <div class="form-group">
                    <div class="col-sm-12">
                            <input type="text" class="form-control" name="nudoc" id="nudoc" placeHolder="Número do documento" required="">
                    </div>
                </div>
                <div class="form-group">
                    <div class="col-sm-12"  style="margin-left: -20px;">
                            <button type="" class="btn btn-success"><span class="glyphicon glyphicon-ok"></span> Validar documento</button>
                    </div>
                </div>
            </li>
        </ul>
    </nav>
	
    <div id="barra-identidade">
        <div id="barra-brasil">
            <div id="wrapper-barra-brasil">
                <div class="brasil-flag"><a class="link-barra" href="http://brasil.gov.br">Brasil</a></div>
                <span class="acesso-info"><a class="link-barra" href="http://brasil.gov.br/barra#acesso-informacao">Acesso à informação</a></span>
                <nav><a id="menu-icon" href="#"></a>
                    <ul class="list"><a class="link-barra" href="http://brasil.gov.br/barra#participe">
                            <li class="list-item first last-item">Participe</li>
                        </a><a class="link-barra" href="http://www.servicos.gov.br/?pk_campaign=barrabrasil">
                            <li class="list-item last-item">Serviços</li>
                        </a><a class="link-barra" href="http://www.planalto.gov.br/legislacao">
                            <li class="list-item last-item">Legislação</li>
                        </a><a class="link-barra" href="http://brasil.gov.br/barra#orgaos-atuacao-canais">
                            <li class="list-item last last-item">Canais</li>
                        </a></ul>
                </nav>
            </div>
        </div>
        <script async="" defer="" type="text/javascript" src="http://barra.brasil.gov.br/barra.js"></script>
    </div>

    <!-- Header -->
    <header id="top" class="header">
        <div class="row">
            <div class="col-lg-6 col-xs-6 col-sm-6" style="margin-top: 5px;">
                <div class="text-left">
                    <img src="estrutura/temas/default/img/logo-simec.png" class="img-responsive" width="200">
                </div>
            </div>

            <div class="col-lg-6 col-xs-6 col-sm-6 pull-right" style="margin-top: 5px;">
                <a href="http://www.brasil.gov.br/" class="brasil pull-right"> 
                    <img style="margin-right: 10px;" src="/estrutura/temas/default/img/brasil.png" alt="Brasil - Governo Federal" class="img-responsive">
                </a>
            </div>
        </div>
    </header>