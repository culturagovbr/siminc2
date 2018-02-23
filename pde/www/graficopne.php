<?php
include_once('funcoesgraficopne.php');
?>
<html>
<head>
    <meta http-equiv="X-UA-Compatible" content="IE=7" />
    <meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1">
    <title>Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle</title>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.5.1.min.js"></script>
    <script type="text/javascript" src="../includes/jquery-jqplot-1.0.0/jquery.jqplot.min.js"></script>
    <script type="text/javascript" src="../includes/jquery-jqplot-1.0.0/plugins/jqplot.pieRenderer.min.js"></script>
    <script type="text/javascript" src="../includes/jquery-jqplot-1.0.0/plugins/jqplot.donutRenderer.min.js"></script>

    <!--    <script src="../includes/Highcharts-3.0.0/js/highcharts.js"></script>-->
    <!--	<script src="../includes/Highcharts-3.0.0/js/modules/exporting.js"></script>-->

    <script language="javascript" src="../includes/Highcharts-4.0.3/js/highcharts.js"></script>
    <script language="javascript" src="../includes/Highcharts-4.0.3/js/highcharts-more.js"></script>
    <script language="javascript" src="../includes/Highcharts-4.0.3/js/modules/solid-gauge.src.js"></script>


    <!--	<script type="text/javascript" src="js/estrategico.js"></script>-->
    <script src="../includes/funcoes.js"></script>
    <script language="javascript" src="/estrutura/js/funcoes.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
    <link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
    <link rel="stylesheet" type="text/css" href="../includes/jquery-jqplot-1.0.0/jquery.jqplot.min.css" />

    <script type="text/javascript" 			src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>
    <link rel="stylesheet" type="text/css" href="css/stylegrafico.css"/>
    <script language="javascript" src="js/funcoesgraficopne.js"></script>
</head>
<body>

<section id="ti" style="display: none;margin-top: 30px; width: 400px;">
    <p style="font-size: 22px;font-weight: bold; margin:0 0 5px 0;">Minist�rio da Educa��o</p>
    <p style="font-size: 16px;">
        Secretaria de Articula��o com os Sistemas de Ensino <br/>
        Diretoria de Coopera��o e Planos de Educa��o <br/>
        Proje��o da Contribui��o para a Meta Nacional
    </p>
</section>
<section id="barra-brasil">
    <section id="wrapper-barra-brasil">
        <section class="brasil-flag">
            <a href="http://brasil.gov.br" class="link-barra">
                Brasil
            </a>
        </section>
        <span class="acesso-info">
				<a href="http://brasil.gov.br/barra#acesso-informacao" class="link-barra">
                    Acesso � informa��o
                </a>
			</span>
        <ul class="list">
            <li class="list-item first">
                <a href="http://brasil.gov.br/barra#participe" class="link-barra">
                    Participe
                </a>
            </li>
            <li class="list-item">
                <a href="http://www.servicos.gov.br/" class="link-barra">
                    Servi�os
                </a>
            </li>
            <li class="list-item">
                <a href="http://www.planalto.gov.br/legislacao" class="link-barra">
                    Legisla��o
                </a>
            </li>
            <li class="list-item last last-item">
                <a href="http://brasil.gov.br/barra#orgaos-atuacao-canais" class="link-barra">
                    Canais
                </a>
            </li>
        </ul>
    </section>
</section>
<section id="header">
    <article class="header-content">
        <section id="bg-logo">
            <a href="http://webdes.mec.gov.br/acoespdemunicipio/2013/site/?pagina=inicial" id="logo1"></a>
            <!--            <section id="texto-topo">Construindo as Metas</section>-->
        </section>
        <section id="social-icons">
            <ul class="pull-right">
                <li class="portalredes-item">
                    <a title="Twitter" href="http://twitter.com/mec_comunicacao" target="blank"><img src="/pde/cockpit/images/pne/twitter.png" border="0"></a>
                </li>
                <li class="portalredes-item">
                    <a title="YouTube" href="http://youtube.com/ministeriodaeducacao" target="blank"><img src="/pde/cockpit/images/pne/youtube.png" border="0"></a>
                </li>
                <li class="portalredes-item">
                    <a title="Facebook" href="http://www.facebook.com/pages/Minist%C3%A9rio-da-Educa%C3%A7%C3%A3o/188209857893503" target="blank"><img src="/pde/cockpit/images/pne/facebook.png" border="0"></a>
                </li>
            </ul>
        </section>
    </article>
</section>

<div id="dialog-detalhe"></div>

<div id="obs" style="text-align:left ;margin-top:6px;font-size:11px; width:250px; padding-left:1%;"><p>Recomendamos a utiliza��o dos navegadores Google Chrome ou Mozilla Firefox.</p></div>
<div style="width: 70%;margin:0 auto; padding: 10px 0 0 0;">
    <h2 id="h1contribuicao" style="display:none; margin:15px auto 0 auto; text-align: center;">Situa��o das metas dos planos</h2>
</div>
<div class="container">
    <form name='formulario' action='graficopne_new.php' method='post' style="width:909px;display:table;align:center;border:0; margin: 0 auto;"  >
        <input type='hidden' name='metid' value = ''  id = "metid" >
        <table style="width: 100%!important;">
            <tr>
                <td colspan="2">
                    <?php
                    
                    echo montarAbasArrayLocal( criarAbasMetasPNE() , "" );
                    ?>
                </td>
            </tr>
            <tr>
                <td  id="esconder" class="" width="20%" valign="top">
                    <div style="margin-top:20px;" id="pesquisa">
                        <div style="float:left;" class="titulo_box" >
                            Pesquisa<br/><br/>
                        </div>
                    </div>

                    <!--Ciclo-->
                    <table  cellpadding="5" cellspacing="1" width="100%" id="tabelaCiclo">
                        <?php
                        echo '<td width="22%" class="fundo_td" valign="top">Ciclo
                                <td class="fundo_td">
                                <select name="pneciclo" id="pneciclo"
                                style="height:20px;width: 242px;font-size:12px;border-radius: 0px;
                                border: 1px solid #555555;!important;border-top:2px solid #555555; border-left:2px solid #555555;"
                                onchange="atualizarRelacionadosRegiao(5);">';
                                    $sql = "SELECT * FROM sase.pneciclo ORDER BY ciclo DESC";
                                    $pneciclo= $db->carregar($sql);
                                    foreach ($pneciclo as $ciclo) { ?>
                                        <option 
                                            <?= ($ciclo['descricao'] == $_POST['descricao']) ? 'selected' : '' ?> value="<?= $ciclo['ciclo']?>"><?= $ciclo['descricao'] ?>      
                                        </option>
                                    <?php } ?>
                                </select>
                    </table>


                    <table  cellpadding="5" cellspacing="1" width="100%" id="tabelaRegioes">
                        <?php
                        #Regi�o
                        $sql = " Select	regcod AS codigo, regdescricao AS descricao From territorios.regiao order by regdescricao";
                   //     ($stDescricao, $stNomeCampo, $sql_combo, $sql_carregados, $stTextoSelecao, Array $where = null, $funcaoJS = null, $semTR = false, $intervalo = false, $arrVisivel = null, $arrOrdem = null, $obrig = false, $campoContem = true, $onpush = 'S') {
                        mostrarComboPopupLocal( 'Regi�o', 'slRegiao',  $sql, "", 'Selecione as Regi�es', null,'atualizarRelacionadosRegiao(1)',false);
                        ?>
                    </table>
                    <table  cellpadding="5" cellspacing="1" width="100%" id = "tabelaEstados" class="filtro_combo">
                        <?php
                        listarEstados();
                        ?>
                    </table><!--
-->                 <table  cellpadding="5" cellspacing="1" width="100%" id = "tabelaMesoregioes" class="filtro_combo">
                        <?php
                        listarMesoregioes();
                        ?>
                    </table>
                    <table  cellpadding="5" cellspacing="1" width="100%" id ="tabelaMunicipios" class="filtro_combo">
                        <?php
                        listarMunicipios();
                        ?>
                    </table>
                </td>
                <td class="" width="80%" id="divListagem" valign="top">
                </td>
            </tr>
        </table>
    </form>
</div>

<script>
    <?php if(!empty($_REQUEST['metid'])){ ?>
    $(function(){
        $('#abametaf' + <?php echo $_REQUEST['metid']; ?>).click();
    });
    <?php } ?>
</script>

</body>
</html>