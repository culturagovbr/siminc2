
<div class="col-md-12">
    <div class="ibox float-e-margins">
        <div class="ibox-title">
            <h5>Acompanhamento de Plano Interno</h5>
        </div>
        <div class="ibox-content">
            <div class="table-responsive">
                <table id="tabelaAcompanhamento" class="table table-bordered table-hover" >
                    <thead>
                    <tr class="text-center">
                        <th rowspan="2" style="border-right: 1px black solid;">
                            <i class="fa fa-plus detalhar-todos link"></i>&nbsp;&nbsp;Unidade
                        </th>
                        <th colspan="2" style="border-right: 1px black solid;">Custeio</th>
                        <th colspan="2" style="border-right: 1px black solid;">Capital</th>
                        <th colspan="2" style="border-right: 1px black solid;">Total</th>
                        <th rowspan="2" style="border-right: 1px black solid;">Limite</th>
                    </tr>
                    <tr class="text-center">
                        <th>Pr�-PI</th>
                        <th style="border-right: 1px black solid;">Detalhado</th>
                        <th>Pr�-PI</th>
                        <th style="border-right: 1px black solid;">Detalhado</th>
                        <th>Pr�-PI</th>
                        <th style="border-right: 1px black solid;">Detalhado</th>
                    </tr>
                    </thead>
                    <tbody>
                        <?php foreach($aPropostas as $dados){
                            $dado = $dados['detalhe'][0];
                        ?>
                            <tr class="linha_subunidade">
                                <td style="border-right: 1px black solid;">
                                    <i class="fa fa-plus detalhar-linha link" data-suoid="<?php echo $dado['suoid']; ?>"></i>&nbsp;&nbsp;
                                    <span class="detalhe-subunidade" data-suoid="<?php echo $dado['suoid']; ?>" title="Visualizar PIs dessa Unidade">
                                        <?php echo $dado['unosigla'] . ' - ' . $dado['suonome'] . ' (' . $dado['suocod'] . ')'; ?>
                                    </span>
                                </td>

                                <td class="text-right"><?php echo number_format($dados['dotacaoCusteio'], 0, ',', '.'); ?></td>
                                <?php $disponivel = $dados['dotacaoCusteio'] - $dados['planejadoCusteio']; ?>
                                <td style="color: <?php echo controlarCorPorValorDisponivel($disponivel); ?>; border-right: 1px black solid;" class="text-right title-disponivel" title="Dispon�vel: <?php echo number_format($disponivel, 0, ',', '.'); ?>">
                                    <?php echo number_format($dados['planejadoCusteio'], 0, ',', '.'); ?>
                                </td>

                                <td class="text-right"><?php echo number_format($dados['dotacaoCapital'], 0, ',', '.'); ?></td>
                                <?php $disponivel = $dados['dotacaoCapital'] - $dados['planejadoCapital']; ?>
                                <td style="color: <?php echo controlarCorPorValorDisponivel($disponivel); ?>; border-right: 1px black solid;" class="text-right title-disponivel" title="Dispon�vel: <?php echo number_format($disponivel, 0, ',', '.'); ?>">
                                    <?php echo number_format($dados['planejadoCapital'], 0, ',', '.'); ?>
                                </td>

                                <td class="text-right"><?php echo number_format(($dados['dotacaoCusteio'] + $dados['dotacaoCapital']), 0, ',', '.'); ?></td>
                                <?php $disponivel = ($dados['dotacaoCusteio'] + $dados['dotacaoCapital']) - $dados['totalPlanejado']; ?>
                                <td style="color: <?php echo controlarCorPorValorDisponivel($disponivel); ?>; border-right: 1px black solid;" class="text-right title-disponivel" title="Dispon�vel: <?php echo number_format($disponivel, 0, ',', '.'); ?>">
                                    <?php echo number_format($dados['totalPlanejado'], 0, ',', '.'); ?>
                                </td>

                                <?php $disponivel = $dado['limite_subunidade'] - $dados['totalPlanejado']; ?>
                                <td style="color: <?php echo controlarCorPorValorDisponivel($disponivel); ?>; border-right: 1px black solid;" class="text-right title-disponivel" title="Dispon�vel: <?php echo number_format($disponivel, 0, ',', '.'); ?>">
                                    <?php echo number_format($dado['limite_subunidade'], 0, ',', '.'); ?>
                                </td>
                            </tr>
                            <?php
                            foreach($dados['detalhe'] as $dado){ ?>
                                <tr class="tr_<?php echo $dado['suoid']; ?> tr_detalhe" style="display: none;">
                                    <td style="padding-left: 30px; border-right: 1px black solid;" >
                                        <span class="detalhe-funcional" data-ptrid="<?php echo $dado['ptrid']; ?>" data-suoid="<?php echo $dado['suoid']; ?>" title="Visualizar lista de PIs com essa funcional">
                                            <?php echo $dado['funcional'] . ' - ' . $dado['acatitulo'] . ': ' . $dado['plodsc'] . ' (RP ' . $dado['irpcod'] . ')'; ?>
                                        </span>
                                        <?php if($dado['compartilhada'] > 1): ?>
                                            &nbsp;<i class="fa fa-handshake-o link" title="Funcional Compartilhada com outras Subunidades."></i>
                                        <?php endif; ?>
                                    </td>

                                    <td class="text-right"><?php echo number_format($dado['ptrdotacao_custeio'], 0, ',', '.'); ?></td>
                                    <?php $disponivel = $dado['ptrdotacao_custeio'] - $dado['picvalorcusteio']; ?>
                                    <td style="color: <?php echo controlarCorPorValorDisponivel($disponivel); ?>; border-right: 1px black solid;" class="text-right title-disponivel" title="Dispon�vel: <?php echo number_format($disponivel, 0, ',', '.'); ?>">
                                        <?php echo number_format($dado['picvalorcusteio'], 0, ',', '.'); ?>
                                    </td>

                                    <td class="text-right"><?php echo number_format($dado['ptrdotacao_capital'], 0, ',', '.'); ?></td>
                                    <?php $disponivel = $dado['ptrdotacao_capital'] - $dado['picvalorcapital']; ?>
                                    <td style="color: <?php echo controlarCorPorValorDisponivel($disponivel); ?>; border-right: 1px black solid;" class="text-right title-disponivel" title="Dispon�vel: <?php echo number_format($disponivel, 0, ',', '.'); ?>">
                                        <?php echo number_format($dado['picvalorcapital'], 0, ',', '.'); ?>
                                    </td>

                                    <td class="text-right"><?php echo number_format(($dado['ptrdotacao_custeio'] + $dado['ptrdotacao_capital']), 0, ',', '.'); ?></td>
                                    <?php $disponivel = ($dado['ptrdotacao_custeio'] + $dado['ptrdotacao_capital']) - ($dado['picvalorcusteio'] + $dado['picvalorcapital']); ?>
                                    <td style="color: <?php echo controlarCorPorValorDisponivel($disponivel); ?>; border-right: 1px black solid;" class="text-right title-disponivel" title="Dispon�vel: <?php echo number_format($disponivel, 0, ',', '.'); ?>">
                                        <?php echo number_format(($dado['picvalorcusteio'] + $dado['picvalorcapital']), 0, ',', '.'); ?>
                                    </td>
                                    <td class="text-center" style="border-right: 1px black solid;"> - </td>
                                </tr>
                            <?php } ?>
                        <?php } ?>
                        </tbody>                    
                </table>
            </div>
        </div>
    </div>
</div> 
<script>
$(function(){
    $('.detalhe-subunidade').click(function(){
        $('#detalhe-subunidade .modal-body').load('planacomorc.php?modulo=principal/unidade/painel&acao=A&req=detalhe-subunidade&suoid='+ $(this).data('suoid'));
        $('#detalhe-subunidade').modal();
    });

    $('.detalhe-funcional').click(function(){
        $('#detalhe-funcional .modal-body').load('planacomorc.php?modulo=principal/unidade/painel&acao=A&req=detalhe-funcional&ptrid='+ $(this).data('ptrid')+ '&suoid='+ $(this).data('suoid'));
        $('#detalhe-funcional').modal();
    });

    $('.detalhar-linha').click(function(){
        var suoid = $(this).data('suoid');

        if($(this).hasClass('fa-minus')){
            $(this).removeClass('fa-minus').addClass('fa-plus');
            $('.tr_' + suoid).hide();
        } else {
            $(this).removeClass('fa-plus').addClass('fa-minus');
            $('.tr_' + suoid).show('slow');
        }
    });

    $('.detalhar-todos').click(function(){

        if($(this).hasClass('fa-minus')){
            $(this).removeClass('fa-minus').addClass('fa-plus');
            $('.detalhar-linha').removeClass('fa-minus').addClass('fa-plus');
            $('.tr_detalhe').hide();
        } else {
            $(this).removeClass('fa-plus').addClass('fa-minus');
            $('.detalhar-linha').removeClass('fa-plus').addClass('fa-minus');
            $('.tr_detalhe').show('slow');
        }
    });
    
});
</script>