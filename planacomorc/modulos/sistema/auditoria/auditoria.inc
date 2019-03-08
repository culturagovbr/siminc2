<?php

# Recupera tudo que foi passado por REQUEST e transforma os valores em variáveis.
foreach($_REQUEST as $k=>$v) ${$k}=$v;

# Montando a cláusula WHERE
if ($usucpf) $where .= " and usu.usucpf like '%".$usucpf."%' ";
if ($usunome) $where .= " and usu.usunome like '%".str_to_upper($usunome)."%' ";
if ($audtipo) $where .= " and aud.audtipo = '".$audtipo."' ";
if ($audtabela) $where .= " and aud.audtabela like '%".$audtabela."%' ";
if ($audsql) $where .= " and aud.audsql like '%".$audsql."%' ";
if ($auddata_ini and $auddata_fim) $where .=  " and aud.auddata <= '".formata_data_sql($auddata_fim)." 23:59:59' and aud.auddata >= '".formata_data_sql($auddata_ini)." 00:00:00' ";
if ($where) $where = " WHERE ".substr($where,4,strlen($where)); else $where = "";

# Opções da compo de Operações.
$operações = array(
        "U" =>"Alteração",
        "X" =>"Erros",
        "D" =>"Exclusão",
        "I" =>"Inclusão",
    );
if ($where){
    $sql = "
                SELECT
                    usu.usucpf AS cpf,
                    usu.usunome AS nome,
                    usu.usuemail AS email,
                    usu.regcod AS uf,
                    usu.usufoneddd AS ddd,
                    usu.usufonenum AS telefone,
                    usu.usufuncao AS funcao,
                    CASE aud.audtipo
                        WHEN 'I' THEN 'Inclução'
                        WHEN 'U' THEN 'Alteração'
                        WHEN 'D' THEN 'Exclusão'
                        WHEN 'X' THEN 'Erro do Sistema'
                    END AS tipo_acao,
                    to_char(aud.auddata, 'dd/MM/yyyy HH:MM:SS') as data_evento,
                    aba.abadsc AS menuutilizado
                FROM seguranca.usuario usu
                JOIN auditoria aud ON usu.usucpf = aud.usucpf
                JOIN seguranca.aba aba ON aud.sisid = aba.sisid
                    " . $where . "
            ";
    global $db;
    $auditorias = $db->carregar($sql);
}
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
                    <form name="formulario" method="post" role="form" class="form-horizontal">
                        <?php
                            echo $simec->input('usucpf', 'CPF', $filtroauditoria['usucpf'], ['class' => 'inteiro', 'maxlength' => 11]);
                            echo $simec->input('usunome', 'Nome', $filtroauditoria['usunome'], ['maxlength' => 50]);
                            echo $simec->select('audtipo', 'Operação', $filtroauditoria['audtipo'], $operações);
                            echo $simec->input('audtabela', 'Tabela', $filtroauditoria['audtabela'], ['maxlength' => 50]);
                            echo $simec->input('audsql', 'Expressão na Query', $filtroauditoria['audsql'], ['maxlength' => 50]);
                            echo $simec->data('auddata_ini', 'Data', $mJanela->auddata_ini);
                            echo $simec->data('auddata_fim', 'Até', $mJanela->janfim);
                        ?>
                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-primary" type="submit"><i class="fa fa-check"></i>&nbsp;Consultar</button>
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
                                    <td><?= "( ".$auditoria['ddd']." ) ".$auditoria['telefone']; ?></td>
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
