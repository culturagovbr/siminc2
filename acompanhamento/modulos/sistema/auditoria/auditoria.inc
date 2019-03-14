<?php

include_once APPRAIZ . 'includes/classes/modelo/seguranca/Usuario.class.inc';
$mUsuario = new Usuario;

# Persiste o Filtro na sessão.
$filtroAud = $_SESSION['auditoria'] = ($_POST?$_POST:$_SESSION['auditoria']);

switch ($_POST['requisicao']){
    case 'limpar':
        unset($filtroAud);
        break;
    case 'buscar-auditoria':
        foreach ($_POST as $key => $val){
            $params[$key] = $val;
        }
        $where = $mUsuario::montaWhere((object)$params);
        if($where) $auditorias = $mUsuario->recuperarAuditoria($where);
        break;
}

# Opções da compo de Operações.
$operações = array(
    " " => "Selecione",
    "U" => "Alteração",
    "X" => "Erros",
    "D" => "Exclusão",
    "I" => "Inclusão",
);

include APPRAIZ."includes/cabecalho.inc";
?>
<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <div class="col-md-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5><?= $titulo_modulo; ?></h5>
                </div>
                <div class="ibox-content">
                    <form name="formulario" id="filtro-aud" method="post" role="form" class="form-horizontal">
                        <input name="requisicao" id="requisicao" value="buscar-auditoria" type="hidden">
                        <?php
                        echo $simec->input('usucpf', 'CPF', $filtroAud['usucpf'], ['class' => 'inteiro', 'maxlength' => 11]);
                        echo $simec->input('usunome', 'Nome', $filtroAud['usunome'], ['maxlength' => 50]);
                        echo $simec->select('audtipo', 'Operação', $filtroAud['audtipo'], $operações);
                        echo $simec->input('audtabela', 'Tabela', $filtroAud['audtabela'], ['maxlength' => 50]);
                        echo $simec->input('audsql', 'Expressão na Query', $filtroAud['audsql'], ['maxlength' => 50]);
                        echo $simec->data('auddata_ini', 'Data', $filtroAud['auddata_ini']);
                        echo $simec->data('auddata_fim', 'Até', $filtroAud['auddata_fim']);
                        ?>
                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-primary" type="submit">
                                    <i class="fa fa-check"></i>&nbsp;Consultar
                                </button>
                                <button class="btn btn-warning btn-limpar" type="reset">
                                    <span class="glyphicon glyphicon-remove-circle"></span> Limpar
                                </button>
                            </div>
                        </div>
                    </form>
                </div>
            </div>
        </div>
    </div>
    <div class="row">
        <div class="col-md-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Dados Gerais</h5>
                </div>
                <div class="ibox-content">
                    <?php if($auditorias): ?>
                        <div class="table-responsive">
                            <table class="table table-striped table-bordered table-hover dataTables" >
                                <thead>
                                <tr class="text-center">
                                    <th>CPF</th>
                                    <th>Nome</th>
                                    <th>E-mail</th>
                                    <th>UF de lotação</th>
                                    <th>Telefone</th>
                                    <th>Função</th>
                                    <th>Tipo de Evento</th>
                                    <th>Data</th>
                                    <th>Menu Utilizado</th>
                                </tr>
                                </thead>
                                <tbody>
                                <?php foreach($auditorias as $auditoria): ?>
                                    <tr>
                                        <td><?= $auditoria['cpf']; ?></td>
                                        <td><?= $auditoria['nome']; ?></td>
                                        <td><?= $auditoria['email']; ?></td>
                                        <td><?= $auditoria['uf']; ?></td>
                                        <td><?= $auditoria['telefone']; ?></td>
                                        <td><?= $auditoria['funcao']; ?></td>
                                        <td><?= $auditoria['tipo_acao']; ?></td>
                                        <td><?= $auditoria['data_evento']; ?></td>
                                        <td><?= $auditoria['menuutilizado']; ?></td>
                                    </tr>
                                <?php endforeach; ?>
                                </tbody>
                            </table>
                        </div>
                    <?php else: ?>
                        <div style="text-align: center">
                            <span class="alert-danger">Não há registros disponíveis!</span>
                        </div>
                    <?php endif; ?>
                </div>
            </div>
        </div>
    </div>
</div>
<script>
    $(document).ready(function () {
        $('.btn-limpar').click(function(){
            $('#requisicao').val('limpar');
            $('#filtro-aud').submit();
        });
    })
</script>