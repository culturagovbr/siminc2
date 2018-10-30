<?php

# carrega as bibliotecas internas do sistema
require_once 'config.inc';
require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
require_once APPRAIZ . "includes/library/simec/funcoes.inc";

# Valida o CPF, vindo do post
if($_POST['usucpf'] && !validaCPF($_POST['usucpf'])) {
    die('<script>alert(\'CPF inválido!\');history.go(-1);</script>');
}

# Executa a rotina de autenticação quando o formulário for submetido
if($_POST['usucpf']){
    # Abre conexão com o servidor de banco de dados
    $db = new cls_banco();
    
    if(AUTHSSD) {
        include_once APPRAIZ . "includes/autenticarssd.inc";
    } else {
        include_once APPRAIZ . "includes/autenticar.inc";
    }
}

if ( $_REQUEST['expirou'] ) {
    $_SESSION['MSG_AVISO'][] = "Sua conexão expirou por tempo de inatividade. Para entrar no sistema efetue login novamente.";
}

?>
<!doctype html>
<html lang="pt-BR">
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
        <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
        <link rel="profile" href="http://gmpg.org/xfn/11">
        <link href="https://fonts.googleapis.com/css?family=Open+Sans:400,800" rel="stylesheet">

        <script>var et_site_url = 'http://gestaoestrategica.cultura.gov.br'; var et_post_id = '39'; function et_core_page_resource_fallback(a, b){"undefined" === typeof b && (b = a.sheet.cssRules && 0 === a.sheet.cssRules.length); b && (a.onerror = null, a.onload = null, a.href?a.href = et_site_url + "/?et_core_page_resource=" + a.id + et_post_id:a.src && (a.src = et_site_url + "/?et_core_page_resource=" + a.id + et_post_id))}
        </script><title>Gestão Estratégica &#8211; Acompanhar e avaliar a execução das ações pertinentes ao MinC</title>
        <link rel='dns-prefetch' href='//base-wp.cultura.gov.br' />
        <link rel='dns-prefetch' href='//barra.brasil.gov.br' />
        <link rel='dns-prefetch' href='//s.w.org' />
        <link rel="alternate" type="application/rss+xml" title="Feed para Gestão Estratégica &raquo;" href="http://gestaoestrategica.cultura.gov.br/feed/" />
        <link rel="alternate" type="application/rss+xml" title="Feed de comentários para Gestão Estratégica &raquo;" href="http://gestaoestrategica.cultura.gov.br/comments/feed/" />
        <script type="text/javascript">
            window._wpemojiSettings = {"baseUrl":"https:\/\/s.w.org\/images\/core\/emoji\/11\/72x72\/", "ext":".png", "svgUrl":"https:\/\/s.w.org\/images\/core\/emoji\/11\/svg\/", "svgExt":".svg", "source":{"concatemoji":"http:\/\/gestaoestrategica.cultura.gov.br\/wp-includes\/js\/wp-emoji-release.min.js?ver=4.9.8"}};
            !function(a, b, c){function d(a, b){var c = String.fromCharCode; l.clearRect(0, 0, k.width, k.height), l.fillText(c.apply(this, a), 0, 0); var d = k.toDataURL(); l.clearRect(0, 0, k.width, k.height), l.fillText(c.apply(this, b), 0, 0); var e = k.toDataURL(); return d === e}function e(a){var b; if (!l || !l.fillText)return!1; switch (l.textBaseline = "top", l.font = "600 32px Arial", a){case"flag":return!(b = d([55356, 56826, 55356, 56819], [55356, 56826, 8203, 55356, 56819])) && (b = d([55356, 57332, 56128, 56423, 56128, 56418, 56128, 56421, 56128, 56430, 56128, 56423, 56128, 56447], [55356, 57332, 8203, 56128, 56423, 8203, 56128, 56418, 8203, 56128, 56421, 8203, 56128, 56430, 8203, 56128, 56423, 8203, 56128, 56447]), !b); case"emoji":return b = d([55358, 56760, 9792, 65039], [55358, 56760, 8203, 9792, 65039]), !b}return!1}function f(a){var c = b.createElement("script"); c.src = a, c.defer = c.type = "text/javascript", b.getElementsByTagName("head")[0].appendChild(c)}var g, h, i, j, k = b.createElement("canvas"), l = k.getContext && k.getContext("2d"); for (j = Array("flag", "emoji"), c.supports = {everything:!0, everythingExceptFlag:!0}, i = 0; i < j.length; i++)c.supports[j[i]] = e(j[i]), c.supports.everything = c.supports.everything && c.supports[j[i]], "flag" !== j[i] && (c.supports.everythingExceptFlag = c.supports.everythingExceptFlag && c.supports[j[i]]); c.supports.everythingExceptFlag = c.supports.everythingExceptFlag && !c.supports.flag, c.DOMReady = !1, c.readyCallback = function(){c.DOMReady = !0}, c.supports.everything || (h = function(){c.readyCallback()}, b.addEventListener?(b.addEventListener("DOMContentLoaded", h, !1), a.addEventListener("load", h, !1)):(a.attachEvent("onload", h), b.attachEvent("onreadystatechange", function(){"complete" === b.readyState && c.readyCallback()})), g = c.source || {}, g.concatemoji?f(g.concatemoji):g.wpemoji && g.twemoji && (f(g.twemoji), f(g.wpemoji)))}(window, document, window._wpemojiSettings);
        </script>
        <style type="text/css">
            img.wp-smiley,
            img.emoji {
                display: inline !important;
                border: none !important;
                box-shadow: none !important;
                height: 1em !important;
                width: 1em !important;
                margin: 0 .07em !important;
                vertical-align: -0.1em !important;
                background: none !important;
                padding: 0 !important;
            }
        </style>
        <link rel='stylesheet' id='twitter-track-fix-css'  href='http://gestaoestrategica.cultura.gov.br/wp-content/mu-plugins/css/twitter-tracker.css?ver=4.9.8' type='text/css' media='all' />
        <link rel='stylesheet' id='contact-form-7-css'  href='http://gestaoestrategica.cultura.gov.br/wp-content/plugins/contact-form-7/includes/css/styles.css?ver=4.9.1' type='text/css' media='all' />
        <link rel='stylesheet' id='WpBarraBrasil-css'  href='http://gestaoestrategica.cultura.gov.br/wp-content/plugins/wp-barra-brasil//frontend/css/WpBarraBrasil.css?ver=4.9.8' type='text/css' media='all' />
        <link rel='stylesheet' id='gewp-styles-css'  href='http://gestaoestrategica.cultura.gov.br/wp-content/plugins/ge-wp/assets/gewp-styles.css?ver=4.9.8' type='text/css' media='all' />
        <link rel='stylesheet' id='pp-wp-style-css'  href='http://gestaoestrategica.cultura.gov.br/wp-content/themes/pp-wp/assets/stylesheets/dist/bundle.min.css?ver=4.9.8' type='text/css' media='all' />
        <link rel='stylesheet' id='et-builder-modules-style-css'  href='http://gestaoestrategica.cultura.gov.br/wp-content/plugins/divi-builder/includes/builder/styles/frontend-builder-plugin-style.min.css?ver=2.0.67' type='text/css' media='all' />
        <link rel='stylesheet' id='dashicons-css'  href='http://gestaoestrategica.cultura.gov.br/wp-includes/css/dashicons.min.css?ver=4.9.8' type='text/css' media='all' />
        <script type='text/javascript' src='http://gestaoestrategica.cultura.gov.br/wp-includes/js/jquery/jquery.js?ver=1.12.4'></script>
        <script type='text/javascript' src='http://gestaoestrategica.cultura.gov.br/wp-includes/js/jquery/jquery-migrate.min.js?ver=1.4.1'></script>
        <script type='text/javascript' src='http://base-wp.cultura.gov.br/wp-content/mu-plugins/includes/widgets/js/facebook-like.js?ver=4.9.8'></script>
        <script type='text/javascript' src='http://gestaoestrategica.cultura.gov.br/wp-content/plugins/ge-wp/assets/masonry.pkgd.min.js?ver=4.9.8'></script>
        <script type='text/javascript' src='http://gestaoestrategica.cultura.gov.br/wp-content/plugins/ge-wp/assets/gewp-scripts.js?ver=4.9.8'></script>
        <script type='text/javascript'>
                            /* <![CDATA[ */
                            var campaign_common = {"label":{"MeusProjetos":"Nome de listagem dos blogs por usu\u00e1rio na barra superior administrativa", "AdministrarProjetos":""}, "value":{"MeusProjetos":"Meus projetos", "AdministrarProjetos":"Administrar projetos"}};
                            /* ]]> */
        </script>
        <script type='text/javascript' src='http://gestaoestrategica.cultura.gov.br/wp-content/mu-plugins/js/campaign_common.js?ver=4.9.8'></script>
        <link rel='https://api.w.org/' href='http://gestaoestrategica.cultura.gov.br/wp-json/' />
        <link rel="EditURI" type="application/rsd+xml" title="RSD" href="http://gestaoestrategica.cultura.gov.br/xmlrpc.php?rsd" />
        <link rel="wlwmanifest" type="application/wlwmanifest+xml" href="http://gestaoestrategica.cultura.gov.br/wp-includes/wlwmanifest.xml" /> 
        <link rel="canonical" href="http://gestaoestrategica.cultura.gov.br/" />
        <link rel='shortlink' href='http://gestaoestrategica.cultura.gov.br/' />
        <link rel="alternate" type="application/json+oembed" href="http://gestaoestrategica.cultura.gov.br/wp-json/oembed/1.0/embed?url=http%3A%2F%2Fgestaoestrategica.cultura.gov.br%2F" />
        <link rel="alternate" type="text/xml+oembed" href="http://gestaoestrategica.cultura.gov.br/wp-json/oembed/1.0/embed?url=http%3A%2F%2Fgestaoestrategica.cultura.gov.br%2F&#038;format=xml" />
        <script src='http://base-wp.cultura.gov.br/?dm=523d84ef05dfbcc2d4b9334fd6576d26&amp;action=load&amp;blogid=33&amp;siteid=1&amp;t=170047204&amp;back=http%3A%2F%2Fgestaoestrategica.cultura.gov.br%2F' type='text/javascript'></script>		<!-- Piwik -->
        <script type="text/javascript">
            var _paq = _paq || [];
            _paq.push(['trackPageView']);
            _paq.push(['enableLinkTracking']);
            (function() {
            var u = "//analise.cultura.gov.br/";
            _paq.push(['setTrackerUrl', u + 'piwik.php']);
            _paq.push(['setSiteId', 36]);
            var d = document, g = d.createElement('script'), s = d.getElementsByTagName('script')[0];
            g.type = 'text/javascript'; g.async = true; g.defer = true; g.src = u + 'piwik.js'; s.parentNode.insertBefore(g, s);
            })();
        </script>
        
	<!-- Custom Scripts -->
        <script type="text/javascript" src="../includes/funcoes.js?v=1"></script>
        
    <noscript><p><img src="//analise.cultura.gov.br/piwik.php?idsite=36" style="border:0;" alt="" /></p></noscript>
    <!-- End Piwik Code -->
    <link rel="icon" href="http://gestaoestrategica.cultura.gov.br/wp-content/uploads/sites/33/2018/02/cropped-strategy-32x32.png" sizes="32x32" />
    <link rel="icon" href="http://gestaoestrategica.cultura.gov.br/wp-content/uploads/sites/33/2018/02/cropped-strategy-192x192.png" sizes="192x192" />
    <link rel="apple-touch-icon-precomposed" href="http://gestaoestrategica.cultura.gov.br/wp-content/uploads/sites/33/2018/02/cropped-strategy-180x180.png" />
    <meta name="msapplication-TileImage" content="http://gestaoestrategica.cultura.gov.br/wp-content/uploads/sites/33/2018/02/cropped-strategy-270x270.png" />
    <style type="text/css" id="wp-custom-css">
        .site-title {
            font-family: 'Open Sans', sans-serif;
        }
        header.site-header .custom-header-bg img {
            display: none;
        }
        article .entry-footer {
            border-top: 0;
        }
        button.navbar-toggler {
            background: #243850;
        }
        #footer-brasil.verde {
            background: none repeat scroll 0% 0% #0A1524;
            padding: 1em 0px;
            max-width: 100%;
        }
        @media (max-width: 700px) {
            header.site-header>div.container {
                position: absolute;
            }
            header.site-header a {
                font-size: 27px !important;
                top: -18px !important;
                position: relative !important;
                left: -13%;
            }
            header.site-header .site-description {
                font-size: 14px;
                text-transform: uppercase;
                display: none;
            }
            .card {
                margin-bottom: 55px !important;
            }
            footer.site-footer {
                display: none;
            }
        }
        @media (min-width: 1200px) {
            .container {
                margin: 0 auto !important;
            }
            .et_divi_builder #et_builder_outer_content .et_pb_image img {
                position: relative;
                margin-left: 24px;
            }
            header.site-header .custom-header-bg img {
                height: 101px;
                display: block;
                width: auto;
                right: 20%;
                top: 20px;
                position: absolute;
            }
            .col-md-5th-1 {
                width: 20% ;
                flex: 0 0 20%;
                max-width: 20%;
            }
            .et_pb_row {
                max-width: 1200px !important;
                margin: 0 auto;
            }
            header.site-header, header.site-header .site-title {
                color: #243850;
                position: relative;
                top: -22px;
            }
            header.site-header .site-description {
                position: relative;
                top: -19px;
            }
        }
        .arrow-down-blue:after {
            width: 0;
            height: 0;
            border-left: 32px solid transparent !important;
            display: block;
            border-right: 32px solid transparent;
            content: "";
            text-align: center;
            border-top: solid 25px rgb(36, 56, 80);
            position: absolute;
            margin: 17px auto;
            bottom: 0;
            left: 50%;
        }
        .et_pb_section.content-home.arrow-down-blue.et_pb_section_1.et_pb_with_background.et_section_regular {
            padding: 0 !important;
            margin-top: 50px !important;
        }
        .row-container {
            display: flex;
            flex-wrap: wrap;
            margin-right: -15px;
            margin-left: -15px;
        }
        .arrow-down:before {
            width: 0;
            height: 0;
            border-left: 20px solid transparent !important;
            display: block;
            border-right: 20px solid transparent;
            content: "";
            text-align: center;
            border-top: solid 20px rgb(202, 202, 202);
            position: absolute;
            margin: 0 auto;
            bottom: 0px;
            left: 50%;
        }
        .arrow-down:after {
            width: 0;
            height: 0;
            border-left: 20px solid transparent !important;
            display: block;
            border-right: 20px solid transparent;
            content: "";
            text-align: center;
            border-top: solid 20px rgb(246, 246, 247);
            position: absolute;
            margin: 0 auto;
            bottom: 1px;
            left: 50%;
        }
        .et_pb_section.secao-home.arrow-down > div:last-child {
            border-bottom: #e0e0e0 1px solid !important;
            box-shadow: 0px 1px 0px #e0e0e0 !important;
        }
        /**/
        header#header {
            top: 0px;
            background: linear-gradient(0deg, #FFFFFF 16.48%, #D5D9DD 100%);
            box-shadow: 0px 4px 4px rgba(0, 0, 0, 0.25);
            position: relative;
            z-index: 1;
        }
        header.site-header .service-bar-container {
            background: transparent;
        }
        header.site-header, header.site-header .site-title {
            color: #205065;
        }
        ul#shortcut-bar {
            opacity: 0;
        }
        #accessibility {
            opacity: 0;
        }
        header.site-header .service-bar-container .service-bar li a {
            color: #4F4F4F;
            padding: 0 0 0 7px;
            border-left: 1px dotted #fff;
        }
        .navbar-dark .navbar-nav .active>.nav-link, .navbar-dark .navbar-nav .nav-link.active, .navbar-dark .navbar-nav .nav-link.show, .navbar-dark .navbar-nav .show>.nav-link {
            color: #243850;
        }
        header.site-header .service-bar-container .service-bar li {
            font-family: 'Open Sans', sans-serif;
            font-style: normal;
            font-weight: bold;
            line-height: normal;
            font-size: 14px;
            text-transform: uppercase;
        }
        ul#menu-menu-principal {
            position: relative;
            z-index: 1;
            margin: 0 !important;
        }
        main#main .row {
            margin-right: -15px;
            margin-left: -15px;
        }
        div#banner-home {
            margin-left: -15px;
            margin-right: -15px;
        }
        footer.site-footer .row {
            margin-right: -45px;
            margin-left: -45px;
        }
        footer.site-footer {
            background-color: #112640;
        }
        #footer-brasil #wrapper-footer-brasil {
            max-width: 1140px;
            padding: 0;
        }
        .et_divi_builder #et_builder_outer_content .et_pb_row {
            width: 100%;
            max-width: inherit;
        }
        brasil.verde {
            background: none repeat scroll 0% 0% #0A1524;
        }
        #banner-home .et_pb_container.clearfix {
            height: 459px !important;
        }
        #banner-home .et_pb_slide {
            padding: 0 !important;
        }
        div#banner-home {
            background: url(http://gestaoestrategica.cultura.gov.br/wp-content/uploads/sites/33/2018/02/Rectangle-2.png);
            background-attachment: fixed;
        }
        .et_divi_builder #et_builder_outer_content .et_pb_slider .et_pb_slide_0 {
            background-image: url(http://gestaoestrategica.cultura.gov.br/wp-content/uploads/sites/33/2018/02/Rectangle-8.png);
            background-size: 100% 77%;
        }
        header.site-header a {
            color: #243850 !important;
        }
        .et_pb_slide_image:after {
            display: block;
            position: absolute;
            width: 213%;
            z-index: 13;
            height: 547px;
            content: "";
            margin-left: 81%;
            top: -110px;
            background-image: url(http://gestaoestrategica.cultura.gov.br/wp-content/uploads/sites/33/2018/02/Rectangle-8.2.png);
            background-repeat: no-repeat;
            background-size: 100%;
        }
        .titulo-home {
            background: #243850 !important;
            font-size: 28px;
            padding: 20px !important;
            font-weight: 100;
            text-transform: uppercase;
            color: #fff !important;
            margin-top: -41px !important;
        }
        .et_divi_builder .et_pb_section.secao-home > div {
            background-color: #F6F6F7 !important;
            display: block;
        }
        .et_divi_builder #et_builder_outer_content .et_pb_section.secao-home {
            padding: 20px 0;
        }
        .et_divi_builder .et_pb_section.secao-home > div:first-child {
            border-top: 2px solid #243850 !important;
            padding-top: 12px !important;
            font-size: 24px;
            text-transform: uppercase;
            padding-left: 12px !important;
            color: #243850 !important;
            padding-right: 12px !important;
        }
        .card strong {
            font-size: 24px;
            text-transform: uppercase;
            margin-left: 20px !important;
            color: #243850 !important;
        }
        .card {
            text-align: center !important;
            background: white !important;
            margin-top: -63px !important;
            margin-left: 15px !important;
            padding-top: 20px !important;
            border-radius: 4px !important;
            padding-bottom: 20px !important;
            display: block !important;
            box-shadow: 0px 4px 4px rgba(0, 0, 0, 0.25) !important;
            margin-right: 15px !important;
            background-image: url(http://gestaoestrategica.cultura.gov.br/wp-content/uploads/sites/33/2018/02/card-background.png) !important;
            background-repeat: no-repeat !important;
            background-position: center 18px !important;
            background-size: 92% !important;
        }
        .card .card-body a {
            color: #293d54 !important;
            padding-top: 14px !important;
            display: block;
            text-transform: uppercase !important;
            min-height: 64px !important;
        }
        .card img.card-img-top {
            height: 90px !important;
            width: auto !important;
            padding-top: 20px !important;
        }
        article {
            margin-bottom: 0 !important;
        }
        .et_divi_builder #et_builder_outer_content .et_pb_section_1.et_pb_section {
            background-color: #ffffff!important;
            z-index: 0;
        }
        .et_divi_builder #et_builder_outer_content .et_pb_slider .et_pb_slide:first-child {
            display: list-item;
            z-index: 0;
        }
        #breadcrumb {
            background: 0 0;
            text-transform: uppercase;
            padding-left: 0;
            padding-right: 0;
            margin-top: 15px;
            margin-bottom: 0;
            color: #828282;
            cursor: default;
            opacity: 0.6;
        }
        .site-footer {
            background-color: #112640;
            text-transform: uppercase;
        }
        .site-footer .widget {
            border-left: none;
        }
        .widget ul {
            margin-left: 9px;
        }
        a {
            color: #092352;
        }
        .card {
            margin-bottom:20px;
        }
        #acoes-estrategicas .card-wrapper {
            float: none;
            clear: both;
            width: inherit;
            max-height: inherit;
            overflow-y: auto;
            overflow-x: hidden;
        }
        #acoes-estrategicas .ge-card .card-header .headline {
            font-size: 12px;
            color: #0000008b;
            font-weight: bold;
            width: 155px;
            overflow: hidden;
            display: block;
        }
        div#card-2 {
            position: relative;
        }
        #acoes-estrategicas .ge-card {
            margin: 15px 15px 0 15px;
            background: #fff;
            position: relative;
            box-shadow: 0px 2px 3px 0px #ccc;
        }
        #acoes-estrategicas .ge-card .card-header .meta {
            float: right;
            font-size: 18px;
            position: absolute;
            right: 17px;
            top: 17px;
        }
        #acoes-estrategicas .ge-card .card-desc .card-media {
            position: absolute;
            right: 15px;
            top: 0;
            background-color: #243850;
            width: 70px;
            text-align: center;
            text-transform: uppercase;
            font-size: 12px;
            padding: 3px;
            overflow: hidden;
            height: 46px;
            border-radius: 4px;
            color: #ffffff;
            line-height: 39px;
            font-weight: bold;
        }
        .current-menu-item a.nav-link {
            color: #3a82bb !important;
            box-shadow: 0px 2px 0px;
            text-align: center;
            padding: 0px !important;
        }
        #acoes-estrategicas .ge-card .card-header .headline .img {
            display: none !important;
        }
        #acoes-estrategicas .ge-card .card-desc .text h4 {
            font-size: 16px;
            font-weight: bold;
            margin-top: 18px;
        }
        .site-footer .widget ul.sub-menu li {
            border-bottom: none;
            color: #486487;
            list-style: disc;
            margin-left: 11px;
        }
        #acoes-estrategicas .ge-card .card-header {
            background: transparent;
        }
        #acoes-estrategicas .ge-card:hover, #acoes-estrategicas .ge-card:hover a {
            box-shadow: 0px 2px 3px 0px #ccc inset;
            background: #f1f9ff;
            color: #4689bf;
        }
        h1.entry-title {
            background: #243850 !important;
            font-size: 28px;
            padding: 20px !important;
            font-weight: 100;
            text-transform: uppercase;
            color: #fff !important;
            margin-bottom: 20px;
        }
        li#menu-item-493 a {
            float: right;
            display: block;
            text-align: right;
            right: -278px;
            background: #e1e2e3;
            color: #243850 !important;
            position: absolute;
            padding: 9px;
            top: -2px;
            border: 0;
            display: none;
        }
        li#menu-item-493 a:hover {
            background: #243850;
            color: #fff !important;
        }
        a.nav-link-nova-acao {
            padding: 8px;
            position: absolute;
            right: 29px;
            border-radius: 4px;
            top: 30px;
            border: 2px solid #fff;
            background: transparent;
            color: #fff;
        }
        ul.acoes-estrategicas-objetivos {
            position: absolute;
            top: -385px;
            width: 841px;
            left: 240px;
        }
        ul.acoes-estrategicas-objetivos li {
            overflow: hidden;
            width: 202px;
            float: left;
            /* min-height: 144px; */
            display: block;
            border-radius: 4px !important;
            margin: 4px !important;
            text-align: center;
        }
        ul.acoes-estrategicas-objetivos li a {
            font-size: 13px;
            background: #486487 !important;
            display: block;
            overflow: hidden !important;
            padding: 11px !important;
            height: 95px;
            color: #fff !important;
            border-radius: 4px !important;
            line-height: 16px !important;
            font-size: 13px;
            text-align: center !important;
            color: #fff !important;
            text-transform: uppercase !important;
        }

        ul.acoes-estrategicas-objetivos li {
            display: inline-block;
            float: none;
            clear: both;
        }
        ul.acoes-estrategicas-objetivos li a:hover {
            background: #ffffff !important;
            color: #3a82bb !important 
        }
        header.site-header .service-bar-container .navbar{
            padding: 5px 0;
        }
        ul.acoes-estrategicas-objetivos li:last-child {
            width: 99%;
        }

        .et_divi_builder #et_builder_outer_content .et_pb_row {
            width: 100%;
            max-width: inherit;
            z-index: 0;
        }

        .et_divi_builder #et_builder_outer_content .et_pb_section {
            padding: 0; 
        }

        @media (max-width: 1300px) {
            ul.acoes-estrategicas-objetivos {
                left: 205px;
                top: -347px;
                width: 79%;
            }
            ul.acoes-estrategicas-objetivos li {
                overflow: hidden;
                width: 170px;
                display: inline-block;
                /* min-height: 144px; */
                border-radius: 4px !important;
                margin: 4px !important;
                text-align: center;
                margin-bottom: 0 !important;
                margin-top: 0 !important;
            }
            ul.acoes-estrategicas-objetivos li a {
                background: #486487 !important;
                display: inline-block;
                overflow: hidden !important;
                padding: 8px !important;
                height: 85px;
                border-radius: 4px !important;
                line-height: 16px !important;
                font-size: 11px;
                text-align: center !important;
                color: #fff !important;
                text-transform: uppercase !important;
            }
            .card {
                margin-top: -42px !important;
            }
            ul.acoes-estrategicas-objetivos li:last-child a {
                width: 93.6%;
            }
        }
        .home .card a {
            transition: all 0.3s ease 0.3s !important;
            min-height: 86px !important;
        }
        /*
        .home .card a:hover {
            background: #dddde47a !important;
            margin-top: -118px !important;
            padding-top: 136px !important;
            min-height: 86px !important;
            margin-bottom: -20px !important;
        }
        */

        @media (max-width: 500px) {
            ul.acoes-estrategicas-objetivos {
                width: 100%;
                position: inherit !important;
                top: 0;
                left: 0;
                margin: 0 !important;
                padding: 0 !important;
            }
            ul.acoes-estrategicas-objetivos li {
                width: 100%;
            }
            .card img {
                width: 53px;
            }
            ul.acoes-estrategicas-objetivos li a {
                background: #2c3847 !important;
            }
            .col-lg-12 {
                padding: 0;
            }
            button.navbar-toggler {
                position: absolute;
                right: 0;
                top: 12px;
                z-index: 9;
            }
            header.site-header .service-bar-container .service-bar li a {
                font-size: 11px !important;
                margin: 0 !important;
                text-align: left;
                float: left;
                width: 100%;
            }
            header.site-header .service-bar-container .service-bar li {
                font-family: 'Open Sans', sans-serif;
                font-style: normal;
                font-weight: bold;
                line-height: normal;
                text-transform: uppercase;
                padding: 7px;
            }
            ul#menu-menu-principal {
                position: relative;
                z-index: 1;
                top: 19px;
                right: -37px;
                margin: 0 !important;
            }
            a.nav-link-nova-acao {
                position: inherit;
                color: #fff;
                background: #243850;
                border: #243850 1px solid;
                margin: 0 auto;
                box-shadow: 1px 3px 4px #868585, 1px 3px 4px #b3b3b3;
            }
            #accessibility {
                display: none;
            }
        }

        #acoes-estrategicas.subacoes .ge-card .card-desc .text {
            padding: 0 !important;
        }
        div#acoes-estrategicas.subacoes {
            margin-left: -30px;
            margin-right: -30px;
            margin-bottom: 56px;
        }
        a.nav-link-nova-acao {
            top: -103px;
            color: #fff !important;
            border: 1px solid !important;
            padding: 15px !important;
            right: 11px;
        }
        .acao h3 a {
            color: #fff !important;
            font-weight: 100 !important;
            margin-left: 11px !important;
            line-height: 28px !important;
            top: -12px;
            position: relative;
            text-transform: uppercase !important;
        }
        .acao {
            margin: 10px !important;
        }
        .dropdown-menu.show {
            background: #fff !important;
            left: 0 !important;
            position: absolute !important;
            color: #243850 !important;
        }
        .dropdown.d-inline.show {
            position: relative !important;
        }
        .et_divi_builder #et_builder_outer_content .et_pb_module a {
            color: #23282d;
        }
        .dropdown-menu.show {
            box-shadow: 1px 1px 1px #ccc !important;
            padding: 20px !important;
        }

        .et_divi_builder #et_builder_outer_content a.et_pb_more_button.et_pb_button {
            color: white;
        }
        .arrow-right-plano a {
            background: url(http://gestaoestrategica.cultura.gov.br/wp-content/uploads/sites/33/2018/07/right-arrow.png) !important;
            display: block;
            width: 100%;
            color: #486487 !important;
            background-repeat: no-repeat !important;
            background-position: right !important;
            padding-bottom: 0 !important;
        }
        .no-padding{
            padding: 14px !important;
        }
        .arrow-right-plano a:hover {
            color: #5d9be8 !important;
        }
        .acoes-estrategicas-resultados {
            position: relative;
            text-align: center;
            min-height: 660px;
            margin: 29px 0 45px;
        }
        .acoes-estrategicas-resultados .medal-1 h3 {
            top: -78px;
            padding: 0;
        }

        .acoes-estrategicas-resultados-list a.list-group-item {
            background: #F6F6F7 !important;
            border-bottom: 1px solid #243850 !important;
            margin-bottom: 1px !important;
            margin-top: -1px !important;
            text-transform: uppercase !important;
            font-weight: bold !important;
        }
        
        #login-form-page h2{
            background-color: rgb(36, 56, 80) !important;
            color: #fff;
            padding: 20px;
            margin-bottom: 30px;
        }
        
        #login-form-page .box-area{
            border-top: 2px solid #243850 !important;
            background-color: #f6f6f7;
            padding: 15px;
            margin-bottom: 30px;
        }
        
        #login-form-page .box-area-inner{
            padding: 15px;
            background-color: #fff;
            margin-top: 15px;
            margin-bottom: 15px;
        }
        
        #login-form-page h3{
            font-size: 24px;
            color: #243850;
            text-transform: uppercase;
        }
        
        #login-form-page input[type="checkbox"]{
            position: static;
            margin: auto;
        }
        
        #login-form-page label.form-check-label{
            padding: 0;
        }
        
        #login-form-page .btn.btn-primary{
            background-color: #243850;
            border: transparent;
        }
        
        .lnkSolicitarAcesso{
            color: #3a82bb;
            font-weight: bold;
        }
        
        .lnkLembrarSenha{
            cursor: hand;
            cursor: pointer;
        }

        .btn-acessar{
            cursor: hand;
            cursor: pointer;
        }
    </style>
</head>

<body class="home page-template page-template-page-fluid page-template-page-fluid-php page page-id-39 green-theme  et-pb-theme-portal padrão wp et_minified_js et_minified_css et_divi_builder">

    <div id="page" class="site">

        <a class="skip-link screen-reader-text sr-only" href="#content">Pular para o conteúdo</a>

        <header id="header" class="site-header">
            <div class="container">
                <div class="row">
                    <div class="col-md-6 col-lg-8 cf-1">

                        <ul id="shortcut-bar" class="d-none d-md-block">
                            <li>
                                <a accesskey="1" href="#main" id="main-link">
                                    Ir para o conteúdo
                                    <span>1</span>
                                </a>
                            </li>
                            <li>
                                <a accesskey="2" href="#main-navbar" id="navigation-link">
                                    Ir para o menu
                                    <span>2</span>
                                </a>
                            </li>
                            <li>
                                <a accesskey="3" href="#main-search" id="main-search-link">
                                    Ir para a busca
                                    <span>3</span>
                                </a>
                            </li>
                            <li class="last-item">
                                <a accesskey="4" href="#footer" id="footer-link">
                                    Ir para o rodapé
                                    <span>4</span>
                                </a>
                            </li>
                        </ul>

                        <h1 class="site-title">
                            <a href="http://gestaoestrategica.cultura.gov.br/" rel="home">
                                Gestão Estratégica                        </a>
                        </h1>
                        <span class="site-description">Acompanhar e avaliar a execução das ações pertinentes ao minc</span>
                    </div>

                    <div class="col-md-6 col-lg-4 cf-2">

                        <ul id="accessibility">
                            <li>
                                <a href="http://gestaoestrategica.cultura.gov.br/acessibilidade" title="Acessibilidade" accesskey="5">Acessibilidade</a>
                            </li>
                            <li>
                                <a href="#" title="Alto Contraste" accesskey="6" id="high-contrast">Alto Contraste</a>
                            </li>
                            <li>
                                <a href="http://gestaoestrategica.cultura.gov.br/mapa-do-site" title="Mapa do Site" accesskey="7">Mapa do Site</a>
                            </li>
                        </ul>

                    </div>
                </div>
            </div>

            <div class="service-bar-container">
                <div class="container">
                    <div class="row">
                        <div class="col">
                            <nav class="navbar navbar-expand-md navbar-dark">
                                <a class="navbar-brand invisible d-md-none" href="#">Menu de navegação</a>
                                <button class="navbar-toggler" type="button" data-toggle="collapse" data-target="#main-navbar" aria-controls="main-navbar" aria-expanded="false" aria-label="Toggle navigation">
                                    <span class="navbar-toggler-icon"></span>
                                </button>
                                <div id="main-navbar" class="collapse navbar-collapse"><ul id="menu-menu-principal" class="service-bar ml-auto nav navbar-nav"><li itemscope="itemscope" itemtype="https://www.schema.org/SiteNavigationElement" id="menu-item-45" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-home current-menu-item page_item page-item-39 current_page_item menu-item-45 active"><a title="Início" href="http://gestaoestrategica.cultura.gov.br/" class="nav-link">Início</a></li>
                                        <li itemscope="itemscope" itemtype="https://www.schema.org/SiteNavigationElement" id="menu-item-66" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-66"><a title="Ações Estratégicas" href="http://gestaoestrategica.cultura.gov.br/acoes-estrategicas/" class="nav-link">Ações Estratégicas</a></li>
                                        <li itemscope="itemscope" itemtype="https://www.schema.org/SiteNavigationElement" id="menu-item-65" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-has-children menu-item-65 dropdown"><a title="Gerenciamento de Processo" href="#" data-toggle="dropdown" class="dropdown-toggle" aria-haspopup="true">Gerenciamento de Processo <span class="caret"></span></a>
                                            <ul role="menu" class=" dropdown-menu" >
                                                <li itemscope="itemscope" itemtype="https://www.schema.org/SiteNavigationElement" id="menu-item-61" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-61"><a title="Modelo de Gestão" href="http://gestaoestrategica.cultura.gov.br/modelo-de-gestao/" class="nav-link">Modelo de Gestão</a></li>
                                                <li itemscope="itemscope" itemtype="https://www.schema.org/SiteNavigationElement" id="menu-item-106" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-106"><a title="Metodologia" href="http://gestaoestrategica.cultura.gov.br/metodologia/" class="nav-link">Metodologia</a></li>
                                                <li itemscope="itemscope" itemtype="https://www.schema.org/SiteNavigationElement" id="menu-item-105" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-105"><a title="Cadeia de Valor" href="http://gestaoestrategica.cultura.gov.br/cadeia-de-valor/" class="nav-link">Cadeia de Valor</a></li>
                                                <li itemscope="itemscope" itemtype="https://www.schema.org/SiteNavigationElement" id="menu-item-104" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-104"><a title="Estratégias" href="http://gestaoestrategica.cultura.gov.br/estrategias/" class="nav-link">Estratégias</a></li>
                                            </ul>
                                        </li>
                                        <li itemscope="itemscope" itemtype="https://www.schema.org/SiteNavigationElement" id="menu-item-73" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-has-children menu-item-73 dropdown"><a title="Plano de Diretrizes e Metas" href="#" data-toggle="dropdown" class="dropdown-toggle" aria-haspopup="true">Plano de Diretrizes e Metas <span class="caret"></span></a>
                                            <ul role="menu" class=" dropdown-menu" >
                                                <li itemscope="itemscope" itemtype="https://www.schema.org/SiteNavigationElement" id="menu-item-103" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-103"><a title="Mapa Estratégico" href="http://gestaoestrategica.cultura.gov.br/mapa-estrategico/" class="nav-link">Mapa Estratégico</a></li>
                                                <li itemscope="itemscope" itemtype="https://www.schema.org/SiteNavigationElement" id="menu-item-101" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-101"><a title="Fonte de recursos" href="http://gestaoestrategica.cultura.gov.br/orcamento/" class="nav-link">Fonte de recursos</a></li>
                                                <li itemscope="itemscope" itemtype="https://www.schema.org/SiteNavigationElement" id="menu-item-808" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-808"><a title="Plano de Ação" href="http://gestaoestrategica.cultura.gov.br/plano-de-acao/" class="nav-link">Plano de Ação</a></li>
                                            </ul>
                                        </li>
                                        <li itemscope="itemscope" itemtype="https://www.schema.org/SiteNavigationElement" id="menu-item-842" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-has-children menu-item-842 dropdown"><a title="Resultados" href="#" data-toggle="dropdown" class="dropdown-toggle" aria-haspopup="true">Resultados <span class="caret"></span></a>
                                            <ul role="menu" class=" dropdown-menu" >
                                                <li itemscope="itemscope" itemtype="https://www.schema.org/SiteNavigationElement" id="menu-item-102" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-102"><a title="Plano de Ação &#8211; Acompanhamento" href="http://gestaoestrategica.cultura.gov.br/resultados/" class="nav-link">Plano de Ação &#8211; Acompanhamento</a></li>
                                                <li itemscope="itemscope" itemtype="https://www.schema.org/SiteNavigationElement" id="menu-item-843" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-843"><a title="Indicadores" href="http://gestaoestrategica.cultura.gov.br/indicadores/" class="nav-link">Indicadores</a></li>
                                            </ul>
                                        </li>
                                        <li itemscope="itemscope" itemtype="https://www.schema.org/SiteNavigationElement" id="menu-item-62" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-has-children menu-item-62 dropdown"><a title="Fontes de Recursos" href="#" data-toggle="dropdown" class="dropdown-toggle" aria-haspopup="true">Fontes de Recursos <span class="caret"></span></a>
                                            <ul role="menu" class=" dropdown-menu" >
                                                <li itemscope="itemscope" itemtype="https://www.schema.org/SiteNavigationElement" id="menu-item-680" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-680"><a title="Orçamento Geral da União (OGU)" href="http://gestaoestrategica.cultura.gov.br/orcamento-e-gestao/" class="nav-link">Orçamento Geral da União (OGU)</a></li>
                                                <li itemscope="itemscope" itemtype="https://www.schema.org/SiteNavigationElement" id="menu-item-99" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-99"><a title="Lei Federal de Incentivo à Cultura" href="http://gestaoestrategica.cultura.gov.br/renuncia-lei-rouanet/" class="nav-link">Lei Federal de Incentivo à Cultura</a></li>
                                                <li itemscope="itemscope" itemtype="https://www.schema.org/SiteNavigationElement" id="menu-item-98" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-98"><a title="Fundo Setorial do Audiovisual (FSA)" href="http://gestaoestrategica.cultura.gov.br/fsa/" class="nav-link">Fundo Setorial do Audiovisual (FSA)</a></li>
                                                <li itemscope="itemscope" itemtype="https://www.schema.org/SiteNavigationElement" id="menu-item-96" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-96"><a title="Emendas Parlamentares e  PAC ? Céu das Artes" href="http://gestaoestrategica.cultura.gov.br/pac-ceu-das-artes/" class="nav-link">Emendas Parlamentares e  PAC ? Céu das Artes</a></li>
                                                <li itemscope="itemscope" itemtype="https://www.schema.org/SiteNavigationElement" id="menu-item-97" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-97"><a title="PAC ? Cidades Históricas (Iphan)" href="http://gestaoestrategica.cultura.gov.br/pac-iphan/" class="nav-link">PAC ? Cidades Históricas (Iphan)</a></li>
                                            </ul>
                                        </li>
                                        <li itemscope="itemscope" itemtype="https://www.schema.org/SiteNavigationElement" id="menu-item-493" class="menu-item menu-item-type-custom menu-item-object-custom menu-item-493"><a title="Adicionar nova ação" href="http://siminc2.cultura.gov.br/" class="nav-link">Adicionar nova ação</a></li>
                                    </ul>
                                </div>
                            </nav>
                        </div>
                    </div>
                </div>
            </div>
            <div class="custom-header-bg">
                <img src="http://gestaoestrategica.cultura.gov.br/wp-content/uploads/sites/33/2018/02/Group-2-3.png" width="556" height="168" alt="Gestão Estratégica" srcset="http://gestaoestrategica.cultura.gov.br/wp-content/uploads/sites/33/2018/02/Group-2-3.png 556w, http://gestaoestrategica.cultura.gov.br/wp-content/uploads/sites/33/2018/02/Group-2-3-300x91.png 300w" sizes="(max-width: 556px) 100vw, 556px" />
            </div>
        </header>

        <main id="main" class="site-main">

            <div class="container-fluid">
                <div class="row ">
                </div>
                <div class="row ">
                    <div class="col-lg-12">
                        <article id="login-form-page" class="hentry" style="margin: 30px 0;">
                            <div class="entry-content">
                                <div class="container">
                                    <div class="row">
                                        <div class="col-md-12">
                                            <h2 class="arrow-down-blue">SISGEST</h2>
                                        </div>
                                        <div class="col-md-12">
                                            <div class="box-area">
                                                <div class="row">
                                                    <div class="col-md-4 offset-md-4 box-area-inner">
                                                        <h3>Login</h3>
                                                        <form role="form" method="post" action="">
                                                            <input type="hidden" name="versao" value="<?php echo $_POST['versao']; ?>"/>
                                                            <input type="hidden" name="formulario" value="1"/>
                                                            
                                                            <div class="form-group">
                                                                <label for="usucpf">CPF</label>
                                                                <input type="text" maxlength="14" class="form-control" name="usucpf" id="usucpf" placeHolder="CPF" required="">
                                                            </div>
                                                            <div class="form-group">
                                                                <label for="ususenha">Senha</label>
                                                                <input type="password" class="form-control" name="ususenha" id="ususenha" placeHolder="Senha" required="">
                                                            </div>
                                                            <div class="form-group form-check">
                                                                <input type="checkbox" class="form-check-input" id="rememberme">
                                                                <label class="form-check-label lnkLembrarSenha" for="rememberme">Lembrar senha</label>
                                                            </div>
                                                            <button type="submit" class="btn btn-primary btn-acessar">Acessar</button>
                                                            
                                                            <hr>
                                                            <p class="text-center">Não tem acesso ainda? <a href="<?php echo URL_SISTEMA. 'cadastrar_usuario.php?sisid=48';?>" class="lnkSolicitarAcesso">Solicitar acesso</a></p>
                                                        </form>      
                                                    </div>
                                                </div>
                                            </div>
                                        </div>
                                    </div>
                                </div>
                            </div>
                        </article>
                    </div>
                </div>
            </div>

        </main>

        <footer id="footer" class="site-footer">
            <div class="container site-info">
                <div class="row">
                    <div class="footer-widget col 0 ">
                        <section id="nav_menu-2" class="widget widget_nav_menu">
                            <div class="menu-rodape-1-container">
                                <ul id="menu-rodape-1" class="menu">
                                    <li id="menu-item-231" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-home current-menu-item page_item page-item-39 current_page_item menu-item-231"><a href="http://gestaoestrategica.cultura.gov.br/">Início</a></li>
                                    <li id="menu-item-232" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-232"><a href="http://gestaoestrategica.cultura.gov.br/acoes-estrategicas/">Ações Estratégicas</a></li>
                                </ul>
                            </div>
                        </section>
                    </div>
                    <div class="footer-widget col 1 ">
                        <section id="nav_menu-3" class="widget widget_nav_menu">
                            <div class="menu-rodape-2-container">
                                <ul id="menu-rodape-2" class="menu">
                                    <li id="menu-item-234" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-has-children menu-item-234"><a href="http://gestaoestrategica.cultura.gov.br/gerenciamento-de-processo/">Gerenciamento de Processo</a>
                                        <ul class="sub-menu">
                                            <li id="menu-item-236" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-236"><a href="http://gestaoestrategica.cultura.gov.br/modelo-de-gestao/">Modelo de Gestão</a></li>
                                            <li id="menu-item-235" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-235"><a href="http://gestaoestrategica.cultura.gov.br/metodologia/">Metodologia</a></li>
                                            <li id="menu-item-237" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-237"><a href="http://gestaoestrategica.cultura.gov.br/cadeia-de-valor/">Cadeia de Valor</a></li>
                                            <li id="menu-item-233" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-233"><a href="http://gestaoestrategica.cultura.gov.br/estrategias/">Estratégias</a></li>
                                        </ul>
                                    </li>
                                </ul>
                            </div>
                        </section>
                    </div>
                    <div class="footer-widget col 2 ">
                        <section id="nav_menu-4" class="widget widget_nav_menu">
                            <div class="menu-rodape-3-container">
                                <ul id="menu-rodape-3" class="menu">
                                    <li id="menu-item-240" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-has-children menu-item-240"><a href="http://gestaoestrategica.cultura.gov.br/plano-de-diretrizes-e-metas/">Plano de Diretrizes e Metas</a>
                                        <ul class="sub-menu">
                                            <li id="menu-item-238" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-238"><a href="http://gestaoestrategica.cultura.gov.br/mapa-estrategico/">Mapa Estratégico</a></li>
                                            <li id="menu-item-239" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-239"><a href="http://gestaoestrategica.cultura.gov.br/orcamento/">Orçamento</a></li>
                                        </ul>
                                    </li>
                                    <li id="menu-item-869" class="menu-item menu-item-type-custom menu-item-object-custom menu-item-has-children menu-item-869"><a href="#">Resultados</a>
                                        <ul class="sub-menu">
                                            <li id="menu-item-870" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-870"><a href="http://gestaoestrategica.cultura.gov.br/indicadores/">Indicadores</a></li>
                                            <li id="menu-item-871" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-871"><a href="http://gestaoestrategica.cultura.gov.br/resultados/">Plano de Ação ? Acompanhamento</a></li>
                                        </ul>
                                    </li>
                                </ul>
                            </div>
                        </section>
                    </div>
                    <div class="footer-widget col 3 last ">
                        <section id="nav_menu-5" class="widget widget_nav_menu">
                            <div class="menu-rodape-4-container">
                                <ul id="menu-rodape-4" class="menu">
                                    <li id="menu-item-244" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-has-children menu-item-244"><a href="http://gestaoestrategica.cultura.gov.br/orcamentacao/">Orçamentação</a>
                                        <ul class="sub-menu">
                                            <li id="menu-item-243" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-243"><a href="http://gestaoestrategica.cultura.gov.br/loa/">LOA</a></li>
                                            <li id="menu-item-247" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-247"><a href="http://gestaoestrategica.cultura.gov.br/renuncia-lei-rouanet/">Lei Federal de Incentivo à Cultura (Lei Roaunet)</a></li>
                                            <li id="menu-item-242" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-242"><a href="http://gestaoestrategica.cultura.gov.br/fsa/">Fundo Setorial do Audiovisual (FSA)</a></li>
                                            <li id="menu-item-246" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-246"><a href="http://gestaoestrategica.cultura.gov.br/pac-iphan/">PAC ? Iphan</a></li>
                                            <li id="menu-item-245" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-245"><a href="http://gestaoestrategica.cultura.gov.br/pac-ceu-das-artes/">PAC ? Ceu das Artes</a></li>
                                        </ul>
                                    </li>
                                </ul>
                            </div>
                        </section>
                    </div>
                </div>
            </div>
        </footer>

    </div>
    
    <script>
        jQuery(function(){

            // Mascara de CPF
            jQuery('#usucpf').keyup(function(){
                mascara(this, mcpf);
            });

        });
    </script>

    <div id="footer-brasil" class="verde"></div>
    <script type="text/javascript">
        var et_animation_data = [];
    </script>
    <script type='text/javascript'>
        /* <![CDATA[ */
        var wpcf7 = {"apiSettings":{"root":"http:\/\/gestaoestrategica.cultura.gov.br\/wp-json\/contact-form-7\/v1", "namespace":"contact-form-7\/v1"}, "recaptcha":{"messages":{"empty":"Verifique se voc\u00ea n\u00e3o \u00e9 um rob\u00f4."}}};
        /* ]]> */
    </script>
    <script type='text/javascript' src='http://gestaoestrategica.cultura.gov.br/wp-content/plugins/contact-form-7/includes/js/scripts.js?ver=4.9.1'></script>
    <script type='text/javascript'>
        /* <![CDATA[ */
        var WpBarraBrasil = {"element_to_prepend":"BODY"};
        /* ]]> */
    </script>
    <script type='text/javascript' src='http://gestaoestrategica.cultura.gov.br/wp-content/plugins/wp-barra-brasil//frontend/js/WpBarraBrasil.js?ver=0.1.0'></script>
    <script type='text/javascript' src='//barra.brasil.gov.br/barra.js?ver=0.1.0'></script>
    <script type='text/javascript' src='http://gestaoestrategica.cultura.gov.br/wp-content/themes/pp-wp/assets/js/dist/bundle.min.js?ver=4.9.8'></script>
    <script type='text/javascript'>
        /* <![CDATA[ */
        var et_pb_custom = {"ajaxurl":"http:\/\/gestaoestrategica.cultura.gov.br\/wp-admin\/admin-ajax.php", "images_uri":"http:\/\/gestaoestrategica.cultura.gov.br\/wp-content\/themes\/pp-wp\/images", "builder_images_uri":"http:\/\/gestaoestrategica.cultura.gov.br\/wp-content\/plugins\/divi-builder\/includes\/builder\/images", "et_frontend_nonce":"d898d06b89", "subscription_failed":"Por favor, verifique os campos abaixo para verifique se voc\u00ea digitou as informa\u00e7\u00f5es corretas.", "et_ab_log_nonce":"60a1555eed", "fill_message":"Por favor, preencha os seguintes campos:", "contact_error_message":"Por favor, corrija os seguintes erros:", "invalid":"E-mail inv\u00e1lido", "captcha":"Captcha", "prev":"Anterior", "previous":"Anterior", "next":"Pr\u00f3ximo", "wrong_captcha":"Voc\u00ea digitou o n\u00famero captcha errado.", "is_builder_plugin_used":"1", "ignore_waypoints":"no", "is_divi_theme_used":"", "widget_search_selector":".widget_search", "is_ab_testing_active":"", "page_id":"39", "unique_test_id":"", "ab_bounce_rate":"5", "is_cache_plugin_active":"no", "is_shortcode_tracking":""};
        var et_pb_box_shadow_elements = [];
        /* ]]> */
    </script>
    <script type='text/javascript' src='http://gestaoestrategica.cultura.gov.br/wp-content/plugins/divi-builder/js/divi-builder.min.js?ver=2.0.67'></script>
    <script type='text/javascript' src='http://gestaoestrategica.cultura.gov.br/wp-content/plugins/divi-builder/core/admin/js/common.js?ver=3.0.105'></script>
    <script type='text/javascript'>
            var mejsL10n = {"language":"pt", "strings":{"mejs.install-flash":"Voc\u00ea est\u00e1 usando um navegador que n\u00e3o tem Flash ativo ou instalado. Ative o plugin do Flash player ou baixe a \u00faltima vers\u00e3o em https:\/\/get.adobe.com\/flashplayer\/", "mejs.fullscreen-off":"Desativar tela cheia", "mejs.fullscreen-on":"Tela cheia", "mejs.download-video":"Baixar o v\u00eddeo", "mejs.fullscreen":"Tela inteira", "mejs.time-jump-forward":["Avan\u00e7ar 1 segundo", "Avan\u00e7ar %1 segundos"], "mejs.loop":"Alternar repeti\u00e7\u00e3o", "mejs.play":"Reproduzir", "mejs.pause":"Pausar", "mejs.close":"Fechar", "mejs.time-slider":"Tempo do slider", "mejs.time-help-text":"Use as setas esquerda e direita para avan\u00e7ar um segundo. Acima e abaixo para avan\u00e7ar dez segundos.", "mejs.time-skip-back":["Voltar 1 segundo", "Retroceder %1 segundos"], "mejs.captions-subtitles":"Transcri\u00e7\u00f5es\/Legendas", "mejs.captions-chapters":"Cap\u00edtulos", "mejs.none":"Nenhum", "mejs.mute-toggle":"Alternar mudo", "mejs.volume-help-text":"Use as setas para cima ou para baixo para aumentar ou diminuir o volume.", "mejs.unmute":"Desativar mudo", "mejs.mute":"Mudo", "mejs.volume-slider":"Controle de volume", "mejs.video-player":"Tocador de v\u00eddeo", "mejs.audio-player":"Tocador de \u00e1udio", "mejs.ad-skip":"Pular an\u00fancio", "mejs.ad-skip-info":["Pular em 1 segundo", "Pular em %1 segundos"], "mejs.source-chooser":"Seletor de origem", "mejs.stop":"Parar", "mejs.speed-rate":"Taxa de velocidade", "mejs.live-broadcast":"Transmiss\u00e3o ao vivo", "mejs.afrikaans":"Afric\u00e2ner", "mejs.albanian":"Alban\u00eas", "mejs.arabic":"\u00c1rabe", "mejs.belarusian":"Bielorrusso", "mejs.bulgarian":"B\u00falgaro", "mejs.catalan":"Catal\u00e3o", "mejs.chinese":"Chin\u00eas", "mejs.chinese-simplified":"Chin\u00eas (simplificado)", "mejs.chinese-traditional":"Chin\u00eas (tradicional)", "mejs.croatian":"Croata", "mejs.czech":"Checo", "mejs.danish":"Dinamarqu\u00eas", "mejs.dutch":"Holand\u00eas", "mejs.english":"Ingl\u00eas", "mejs.estonian":"Estoniano", "mejs.filipino":"Filipino", "mejs.finnish":"Finland\u00eas", "mejs.french":"Franc\u00eas", "mejs.galician":"Galega", "mejs.german":"Alem\u00e3o", "mejs.greek":"Grego", "mejs.haitian-creole":"Crioulo haitiano", "mejs.hebrew":"Hebraico", "mejs.hindi":"Hindi", "mejs.hungarian":"H\u00fangaro", "mejs.icelandic":"Island\u00eas", "mejs.indonesian":"Indon\u00e9sio", "mejs.irish":"Irland\u00eas", "mejs.italian":"Italiano", "mejs.japanese":"Japon\u00eas", "mejs.korean":"Coreano", "mejs.latvian":"Let\u00e3o", "mejs.lithuanian":"Lituano", "mejs.macedonian":"Maced\u00f4nio", "mejs.malay":"Malaio", "mejs.maltese":"Malt\u00eas", "mejs.norwegian":"Noruegu\u00eas", "mejs.persian":"Persa", "mejs.polish":"Polon\u00eas", "mejs.portuguese":"Portugu\u00eas", "mejs.romanian":"Romeno", "mejs.russian":"Russo", "mejs.serbian":"S\u00e9rvio", "mejs.slovak":"Eslovaco", "mejs.slovenian":"Esloveno", "mejs.spanish":"Espanhol", "mejs.swahili":"Sua\u00edli", "mejs.swedish":"Sueco", "mejs.tagalog":"Tagalo", "mejs.thai":"Tailand\u00eas", "mejs.turkish":"Turco", "mejs.ukrainian":"Ucraniano", "mejs.vietnamese":"Vietnamita", "mejs.welsh":"Gal\u00eas", "mejs.yiddish":"I\u00eddiche"}};</script>
    <script type='text/javascript' src='http://gestaoestrategica.cultura.gov.br/wp-includes/js/mediaelement/mediaelement-and-player.min.js?ver=4.2.6-78496d1'></script>
    <script type='text/javascript' src='http://gestaoestrategica.cultura.gov.br/wp-includes/js/mediaelement/mediaelement-migrate.min.js?ver=4.9.8'></script>
    <script type='text/javascript'>
        /* <![CDATA[ */
        var _wpmejsSettings = {"pluginPath":"\/wp-includes\/js\/mediaelement\/", "classPrefix":"mejs-", "stretching":"responsive"};
        /* ]]> */
    </script>
    <script type='text/javascript' src='http://gestaoestrategica.cultura.gov.br/wp-includes/js/wp-embed.min.js?ver=4.9.8'></script>
    <style id="et-core-unified-cached-inline-styles-2">.et_divi_builder #et_builder_outer_content .et_pb_image_2{margin-left:0}.et_divi_builder #et_builder_outer_content .et_pb_image_1{margin-left:0}.et_divi_builder #et_builder_outer_content .et_pb_image_3{margin-left:0}.et_divi_builder #et_builder_outer_content .et_pb_image_4{margin-left:0}.et_divi_builder #et_builder_outer_content .et_pb_section_6.et_pb_section{background-color:#ffffff!important}.et_divi_builder #et_builder_outer_content .et_pb_section_5.et_pb_section{background-color:#ffffff!important}.et_divi_builder #et_builder_outer_content .et_pb_image_0{margin-left:0}.et_divi_builder #et_builder_outer_content .et_pb_section_4.et_pb_section{background-color:#ffffff!important}.et_divi_builder #et_builder_outer_content .et_pb_slide_0.et_pb_slider_with_text_overlay .et_pb_slide_content{-webkit-border-bottom-right-radius:3px;-webkit-border-bottom-left-radius:3px;-moz-border-radius-bottomright:3px;-moz-border-radius-bottomleft:3px;border-bottom-right-radius:3px;border-bottom-left-radius:3px}.et_divi_builder #et_builder_outer_content .et_pb_slide_0.et_pb_slider_with_text_overlay h2.et_pb_slide_title,.et_divi_builder #et_builder_outer_content.et_pb_slide_0.et_pb_slider_with_text_overlay .et_pb_slide_title{-webkit-border-top-left-radius:3px;-webkit-border-top-right-radius:3px;-moz-border-radius-topleft:3px;-moz-border-radius-topright:3px;border-top-left-radius:3px;border-top-right-radius:3px}.et_divi_builder #et_builder_outer_content .et_pb_section_1.et_pb_section{background-color:#ffffff!important}.et_divi_builder #et_builder_outer_content .et_pb_section_2.et_pb_section{background-color:#ffffff!important}.et_divi_builder #et_builder_outer_content .et_pb_section_3.et_pb_section{background-color:#ffffff!important}.et_divi_builder #et_builder_outer_content .et_pb_slider .et_pb_slide_0{background-image:url(http://gestaoestrategica.cultura.gov.br/wp-content/uploads/sites/33/2018/02/Rectangle-8.png)}@media only screen and (min-width:981px){.et_divi_builder #et_builder_outer_content .et_pb_image_4{display:none!important}.et_divi_builder #et_builder_outer_content .et_pb_image_0{display:none!important}}@media only screen and (min-width:768px) and (max-width:980px){.et_divi_builder #et_builder_outer_content .et_pb_image_4{display:none!important}}@media only screen and (max-width:767px){.et_divi_builder #et_builder_outer_content .et_pb_image_3{display:none!important}.et_divi_builder #et_builder_outer_content .et_pb_image_2{display:none!important}.et_divi_builder #et_builder_outer_content .et_pb_image_1{display:none!important}}</style>

</body>
</html>
