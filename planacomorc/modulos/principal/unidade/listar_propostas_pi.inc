<?php

include_once '_funcoespi.php';

require_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . "monitora/classes/Pi_PlanoInterno.class.inc";

$oPlanoInterno = new Pi_PlanoInterno();
$listaPropostas = $oPlanoInterno->recuperarPropostasSiminc($_SESSION['exercicio'], $aFiltro);
/**
 * Cabeçalho padrão do sistema.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";
?>


<div class="ibox float-e-margins">
    <div class="ibox-title">
            <h5>Listagem de Propostas Orçamentárias</h5>
    </div>
    <div class="ibox-content">

        <div class="table-responsive">
            <table class="table table-striped table-bordered table-hover dataTables-example" >
                <thead>
                <tr>
                    <th>Ações</th>
                    <th>Funcional</th>
                    <th>Título</th>
                    <th>UO</th>
                    <th>Unidade</th>
                    <th>Enq. Despesa</th>
                    <th>Qtd. Física</th>
                    <th>Vl. Custeio</th>
                    <th>Vl. Capital</th>
                    <th>Vl. Total</th>
                </tr>
                </thead>
                <tbody>
                    <?php foreach($listaPropostas as $proposta){ ?>
                        <tr>
                            <td>
                                <?php if(!array_intersect($perfis, [PFL_CONSULTA, PFL_CONSULTA_UNIDADE])){ ?>
                                    <a href="planacomorc.php?modulo=principal/unidade/cadastro_pi&acao=A&replicarProposta=<?php echo $proposta['ppiid']; ?>"><span class="fa fa-plus-circle" title="Criar PI"></span></a>
                                <?php } ?>
                            </td>
                            <td><?php echo substr($proposta['funcionalcampobusca'], 0, strpos($proposta['funcionalcampobusca'], ' - ')); ?></td>
                            <td><?php echo $proposta['ppinome']; ?></td>
                            <td><?php echo $proposta['unonome']; ?></td>
                            <td><?php echo $proposta['suonome']; ?></td>
                            <td><?php echo $proposta['eqdnome']; ?></td>
                            <td><?php echo number_format($proposta['ppiqtdmeta'], 2, ',', '.'); ?></td>
                            <td><?php echo number_format($proposta['ppivlrcusteio'], 2, ',', '.'); ?></td>
                            <td><?php echo number_format($proposta['ppivlrcapital'], 2, ',', '.'); ?></td>
                            <td><?php echo number_format($proposta['total'], 2, ',', '.'); ?></td>
                        </tr>
                    <?php } ?>
                </tbody>
            </table>
        </div>

    </div>
</div>

<script>
    $(document).ready(function(){
        $('.dataTables-example').DataTable({
            pageLength: 100,
            responsive: true,
            dom: '<"html5buttons"B>lTfgitp',
            "language": {
                "url": "/zimec/public/temas/simec/js/plugins/dataTables/Portuguese-Brasil.json"
            }
        });
    });

</script>
