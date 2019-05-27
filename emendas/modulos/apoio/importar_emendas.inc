<?php

if(!empty($_FILES['arquivo'])){
    $controleImportar = new Emendas_Controller_Importacao();
    $listaEmendasArquivo = $controleImportar->importar($_FILES['arquivo']);
}

include APPRAIZ . "includes/cabecalho.inc";
?>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-12">
        <h2><?php echo $titulo_modulo; ?></h2>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <div class="col-md-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Envio de arquivo de Planilha pra importação de Emendas, Beneficíarios, Pronponentes para o <span style="color: red; font-weight: bold;">Ano de <?= $_SESSION['exercicio']; ?></span></h5>
                </div>
                <div class="ibox-title">
                    <h7>
                        Atenção! Para que as informações da planilha sejam 
                        corretamente importadas a planilha deverá ter as seguintes 
                        colunas e exatamente na mesma ordem:
                        <?= join(Emendas_Controller_Importacao::listarColunasArquivoPlanilha(), ', '); ?>
                        <br />
                        <br />
                        Exemplo de arquivo válido de Planilha pra importação: <a style="font-weight: bold;" href="<?= URL_SISTEMA. 'emendas/planilha-importacao-emendas-beneficiarios-exemplo.xlsx'?>" >Planilha com carga de emendas.xlsx</a>
                    </h7>
                </div>
                <div class="ibox-content">

                    <form class="form-horizontal" enctype="multipart/form-data" method="post">
                        <div class="form-group">
                            <label for="inputEmail3" class="col-sm-2 control-label">Planilha SIOP(.XLSX): <span class="campo-obrigatorio" title="Campo obrigatório">*</span></label>
                            <div class="col-sm-8">
                                <input type="file" name="arquivo" class="form-control" required>
                            </div>
                            <div class="col-sm-2">
                                <button type="submit" class="btn btn-success">Importar Arquivo</button>
                            </div>
                        </div>
                    </form>

                </div>
                
                <?php if($listaEmendasArquivo): ?>
                    <div class="ibox-title">
                        <h5>Informações do Arquivo da Planilha que foi importado para o Sistema</h5>
                    </div>
                    <div class="ibox-content">
                        <div class="table-responsive">
                            <table class="table table-striped table-bordered table-hover dataTables">
                                <thead>
                                <tr class="text-center">
                                    <?php foreach(Emendas_Controller_Importacao::listarColunasArquivoPlanilha() as $coluna): ?>
                                        <th><?=$coluna; ?></th>
                                    <?php endforeach; ?>
                                    <th>
                                        Resultado
                                    </th>
                                </tr>
                                </thead>
                                <tbody>
                                    <?php foreach($listaEmendasArquivo AS $emendaArquivo):
                                        $corDestaque = $emendaArquivo['listaDiagnostico']? 'color: red;': NULL;
                                    ?>
                                        <tr style="<?= $corDestaque; ?>">
                                            <?php foreach($emendaArquivo as $coluna => $valor): ?>
                                                <td>
                                                    <?php
                                                        if($coluna == 'listaDiagnostico'):
                                                            echo $valor? join($valor, '<br />'): 'OK!';
                                                        else:
                                                            echo trim($valor);
                                                        endif;
                                                    ?>
                                                </td>
                                            <?php endforeach; ?>
                                        </tr>
                                    <?php endforeach; ?>
                                </tbody>
                            </table>
                        </div>
                    </div>
                <?php endif; ?>

            </div>
        </div>
    </div>
</div>
