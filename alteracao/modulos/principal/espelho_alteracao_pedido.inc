<h2 style="color: #1AB394">1. Dados do Pedido
    <a href="javascript:window.location.href='?modulo=principal/cadastro_alteracoes&acao=C&pedid=<?=$_REQUEST['pedid']?>&tpdid=<?=$_REQUEST['tpdid']?>'" tittle="Editar">
        <span class="btn btn-primary btn-sm glyphicon glyphicon-pencil"></span>
    </a>
</h2>

<div class="row">
    <div class="col-md-4">
        <div class="row">
            <div class="col-md-12">
                <ul><h3>Nº do pedido: <?= $arrPedidos['pedid'];?></h3></ul>
            </div>
            <div class="col-md-12">
                <ul><h3>Janela: <?= $arrPedidos['jannome'];?></h3></ul>
            </div>
            <div class="col-md-12">
                <ul><h3>Titulo: <?= $arrPedidos['pedtitulo'];?></h3></ul>
            </div>
        </div>
    </div>
    
    <div class="col-sm-1">
        <ul><h3>Tipo: <?= $arrPedidos['tpacod'];?></h3></ul>
    </div>
    
    <div class="col-md-4">
        <h3>CADASTRADO POR<h3>
        <hr>
        
        <div class="row">
            <div class="col-md-6">
                <ul><h3>Nome: </h3></ul>
            </div>
            <div class="col-md-6">
                <ul><h3>Email: </h3></ul>
            </div>
        </div>
        
        <ul><h3>Telefone: </h3></ul>
    <div>
</div>
