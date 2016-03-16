<div class="modal-dialog" style="width:60%;">
    <div class="modal-content">
        <div class="modal-header">
            <button type="button" class="close" data-dismiss="modal">
                <span aria-hidden="true">&times;</span>
                <span class="sr-only">Close</span>
            </button>
            <h4 class="modal-title">Cadastrar Nota de Cr�dito (NC)</h4>
        </div>
        <div class="modal-body">
            <div class="row">
                <div class="col-md-12">
                    <div class="alert alert-danger" role="alert">Todos os campos s�o de preenchimento obrigat�rios</div>
                </div>
            </div>
            <form class="form-horizontal"
                  name="<?=$this->element->getName(); ?>"
                  id="<?=$this->element->getId(); ?>"
                  action="<?= $this->element->getAction(); ?>"
                  method="<?= $this->element->getMethod(); ?>"
                  role="form">
                <?= $this->element->_proid; ?>
                <div class="row">
                    <div class="form-group">
                        <label class="control-label col-md-3" for="tcpnumtransfsiafi">N� de Transferencia Siafi:</label>
                        <div class="col-md-8">
                            <?= $this->element->tcpnumtransfsiafi; ?>
                        </div>
                    </div>
                    <div class="form-group">
                        <label class="control-label col-md-3" for="codncsiafi">Nota de Cr�dito:</label>
                        <div class="col-md-8">
                            <?= $this->element->codncsiafi; ?>
                        </div>
                    </div>
                </div>
            </form>
        </div>
        <div class="modal-footer">
            <button type="button" class="btn btn-default hide-modal-nc" data-dismiss="modal">Fechar</button>
            <button type="button" class="btn btn-primary" id="btn-Salva-NC">Salvar</button>
        </div>
    </div>
</div>