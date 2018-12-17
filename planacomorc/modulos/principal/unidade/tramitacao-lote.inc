<?php

    include_once '../planacomorc/_funcoespi.php';
    require_once APPRAIZ . 'includes/workflow.php';
    include_once APPRAIZ . "monitora/classes/Pi_PlanoInterno.class.inc";

    $mPlanoInterno = new Pi_PlanoInterno();

    $listaSubUnidadeUsuario = buscarSubUnidadeUsuario((object) array('usucpf' => $_SESSION['usucpf']));

    $dadosListagem = [];
    if(!empty($_REQUEST['esdid']) || !empty($_REQUEST['unoid']) || !empty($_REQUEST['suoid']) ){
        $filtro = $_REQUEST;
        $filtro['exercicio'] = $_SESSION['exercicio'];
        $filtro['suoid'] = $_REQUEST['suoid'];
        $dadosListagem = $mPlanoInterno->recuperarPis($filtro);

    }
    if(isset($_REQUEST['btn-aplicar'])){
        $mPlanoInterno->tramitarLote($_REQUEST['aedid'], $_REQUEST['aDocid'], $_REQUEST['cmddsc']);
        $filtro = $_REQUEST;
        $filtro['exercicio'] = $_SESSION['exercicio'];
        $filtro['suoid'] = $_REQUEST['suoid'];
        $dadosListagem = $mPlanoInterno->recuperarPis($filtro);        
    }
    include APPRAIZ . "includes/cabecalho.inc";
?>

<script src="js/unidade/lista-pi-manter.js?v=1"></script>
<style>
/*    .tabela-listagem {
        font-size: 1em !important;
    }*/
</style>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <div class="col-md-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Tramitação em lote</h5>
                </div>
                <div class="ibox-content">
                    <form id="filtropi" name="filtropi" method="POST" role="form" class="form-horizontal">
                        <input name="" id="filtropi_requisicao" value="" type="hidden">

                        <?php
                        echo $simec->select('esdid', 'Situação Origem', $_REQUEST['esdid'], Spo_Model_Unidade::queryComboEstados((object) array('tpdid' => WF_TPDID_PLANEJAMENTO_PI)), array('required'));
                        echo $simec->select('eqdid[]', 'Enquadramento da Despesa', $_REQUEST['eqdid'], Spo_Model_Ptres::queryComboEnquadramento((object) array('exercicio' => $_SESSION['exercicio'])));

                        // if (!$listaSubUnidadeUsuario) {
                        //     echo $simec->select('unicod', 'Unidade', $filtropi['unicod'], Spo_Model_Unidade::queryCombo((object)array(
                        //         'prsano::INTEGER = '. (int)$_SESSION['exercicio'],
                        //         'suo.unofundo = FALSE')), array('multiple' => 'true'));
                        // }
                        echo '<div id=div_ungcod >';
                        echo $simec->select('suoid', 'SubUnidade', $_REQUEST['suoid'], Public_Model_SubUnidadeOrcamentaria::queryCombo((object) array(
                            'unofundo' => 'FALSE',
                            'exercicio' => $_SESSION['exercicio'],
                            'listaSubUnidadeUsuario' => $listaSubUnidadeUsuario)), array('multiple' => 'true'));
                        echo '</div>';
                        // ver($dadosListagem,d);
                        ?>

                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-primary btn-buscar" name="btn-buscar" type="submit"><span class="glyphicon glyphicon-search"></span> Buscar</button>
                            </div>
                        </div>

                    </form>
                </div>
            </div>
        </div>
        <div class="col-md-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Listagem de PIs</h5>
                </div>
                <div class="ibox-content" id="listagem">

                    <style>
                        .marcado{background-color:#C1FFC1!important}
                        .remover{display:none}
                    </style>

                    <script type="text/javascript" lang="JavaScript">
                        jQuery(document).ready(function(){
                            jQuery.expr[':'].contains = function(a, i, m) {
                                return jQuery(a).text().toUpperCase().indexOf(m[3].toUpperCase()) >= 0;
                            };

                            jQuery("#textFind").keyup(function()
                            {
                                jQuery('#listagem table tbody tr td').removeClass('marcado');
                                jQuery('#listagem table tbody tr').removeClass('remover');
                                stringPesquisa = jQuery("#textFind").val();

                                console.log(stringPesquisa)

                                if (stringPesquisa) {
                                    jQuery('#listagem table tbody tr td:contains(' + stringPesquisa + ')').addClass('marcado');
                                    jQuery('#listagem table tbody tr:not(:contains(' + stringPesquisa + '))').addClass('remover');
                                }
                            });
                        });
                    </script>

                    <?php if(count($dadosListagem)){
                        $dados = current($dadosListagem);

                        $estados = wf_pegarProximosEstados($dados['docid']);
                        $aEstados = [];
                        foreach($estados as $estado){
                            $aEstados[$estado['aedid']] = $estado['aeddscrealizar'];
                        }
                        ?>
                        <div class="table-responsive">
                            <form id="form-tramitacao" name="form-tramitacao" method="post" role="form" class="form-horizontal">
                                <input name="esdid" value="<?php echo $_REQUEST['esdid']; ?>" type="hidden">
                                <div class="form-group ">
                                    <div class="col-md-9">
                                        <?php echo $simec->select('aedid', 'Tramitar Para', null, $aEstados, array('required')); ?>
                                    </div>
                                    <div class="col-md-2 div_motivo">
                                      <?php if(!array_intersect($perfis, [PFL_CONSULTA, PFL_CONSULTA_UNIDADE])){ ?>
                                        <button class="btn btn-danger" name="btn-aplicar" type="submit"><span class="glyphicon glyphicon-transfer"></span> Tramitar</button>
                                      <?php }?>
                                    </div>
                                </div>
                                <div class="form-group ">
                                    <div class="col-md-9">
                                        <?php
                                        $cmddsc = $_REQUEST['cmddsc'] ? $_REQUEST['cmddsc'] : 'Tramitado em lote';
                                        echo $simec->textarea('cmddsc', 'Comentário', $cmddsc);
                                        ?>
                                    </div>
                                </div>

                                <div class="col-md-12">
                                    <input class="normal form-control" type="text" id="textFind" placeholder="Digite aqui o que procura" style="width: 100%; margin-bottom: 10px;" />
                                </div>

                                <table class="table table-striped table-bordered table-hover bedidTables" >
                                    <thead>
                                    <tr class="text-center">
                                        <th width="5%">
                                            <input type="checkbox" class="marcar-todos-docs" marcar="check_pi">
                                        </th>
                                        <th>ID</th>
                                        <th>Código</th>
                                        <th>PTRES</th>
                                        <th>Título</th>
                                        <th>Funcional</th>
                                        <th>SubUnidade</th>
                                    </tr>
                                    </thead>
                                    <tbody>
                                    <?php foreach($dadosListagem as $dados){ ?>
                                        <tr>
                                            <td class="text-center">
                                                <input type="checkbox" name="aDocid[<?php echo $dados['pliid']; ?>]" class="vincular check_pi" value="<?php echo $dados['docid']; ?>"/>
                                            </td>
                                            <td>
                                                <?php $url = $dados['unofundo'] == 't' ? 'cadastro_pi_fnc' : 'cadastro_pi'; ?>
                                                <a target="_blank" href="planacomorc.php?modulo=principal/unidade/<?php echo $url; ?>&acao=A&pliid=<?php echo $dados['pliid']; ?>"><?php echo $dados['pliid']; ?></a>
                                            </td>
                                            <td><?php echo $dados['plicod']; ?></td>
                                            <td><?php echo $dados['ptres']; ?></td>
                                            <td><?php echo $dados['plititulo']; ?></td>
                                            <td><?php echo $dados['funcional'] . ' - ' . $dados['acatitulo'] . ': ' . $dados['plodsc'] . ' (RP ' . $dados['irpcod'] . ')'; ?></td>
                                            <td><?php echo $dados['unosigla'] . ' - ' . $dados['suonome'] . ' (' . $dados['suocod'] . ')'; ?></td>
                                        </tr>
                                    <?php } ?>
                                    </tbody>
                                </table>
                            </form>
                        </div>
                    <?php } else {
                        $texto = isset($_REQUEST['requisicao']) ? 'Nenhum Registro Encontrado' : 'Favor filtrar por Estado de Origem';
                        echo "<div class='alert alert-danger'>{$texto}</div>";
                    } ?>
                </div>
            </div>
        </div>
    </div>
</div>


<script type="text/javascript">
    $(document).ready(function(){
        initListaPiManter();

        jQuery('.marcar-todos-docs').click(function () {
            var marcado = jQuery(this).prop('checked');
            jQuery('tr:visible .' + jQuery(this).attr('marcar')).each(function(index, obj){
                if($(obj).prop('checked') != marcado){
                    $(obj).click();
                }
            });
        });
    });
</script>
