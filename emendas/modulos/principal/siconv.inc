<?php if(count($aSiconv)){ ?>

    <?php if(count($aSiconv) > 1){ ?>
        <div class="table-responsive">
            <table class="table table-striped table-bordered table-hover" >
                <thead>
                <tr class="text-center">
                    <th colspan="2">Programa</th>
                    <th colspan="2">Proposta</th>
                    <th rowspan="2">Sequencial</th>
                    <th rowspan="2">N� Conv�nio</th>
                    <th rowspan="2">Situa��o</th>
                </tr>
                <tr class="text-center">
                    <th>T�tulo</th>
                    <th>Objeto</th>
                    <th>T�tulo</th>
                    <th>Objeto</th>
                </tr>
                </thead>
                <tbody>

                <?php foreach($aSiconv as $dados){
                    if(empty($sicid)){
                        $sicid = $dados['sicid'];
                    }
                    ?>
                    <tr>
                        <td></td>
                        <td></td>
                        <td></td>
                        <td><?php echo $dados['sicobjeto']; ?></td>
                        <td class="text-center"><?php echo $dados['sicsequencial']; ?></td>
                        <td class="text-center"><?php echo $dados['numeroconvenio']; ?></td>
                        <td><?php echo $dados['sitdsc']; ?></td>
                    </tr>
                <?php } ?>
                </tbody>
            </table>
        </div>
    <?php } else {
        $sicid = $aSiconv[0]['sicid'];
    } ?>

    <div id="detalhe-siconv"><?php echo $this->montarDetalheSiconv($sicid); ?></div>

    <div class="clearfix"></div>

<?php } else {
    echo '<div class="alert alert-danger">Nenhum registro encontrado no SICONV</div>';
} ?>