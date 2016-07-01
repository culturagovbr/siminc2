<form class="form-horizontal"
      name="<?=$this->element->getName(); ?>"
      id="<?=$this->element->getId(); ?>"
      action="<?= $this->element->getAction(); ?>"
      method="<?= $this->element->getMethod(); ?>" role="form">

    <?= $this->element->action; ?>
    <?= $this->element->docid; ?>

    <div class="form-group">
        <label class="control-label col-md-4" for="esdid">Situa��o da Solicita��o:</label>
        <div class="col-md-8">
            <?= $this->element->esdid; ?>
        </div>
    </div>

    <div class="form-group">
        <label class="control-label col-md-4" for="aedid">A��es de Tramita��o:</label>
        <div class="col-md-8 aedid-container">
            <?= $this->element->aedid; ?>
        </div>
    </div>

    <div class="form-group comment hide">
        <label class="control-label col-md-4" for="cmddsc">Coment�rio:</label>
        <div class="col-md-8">
            <?= $this->element->cmddsc; ?>
            <div id="counter-cmddsc" class=""></div>
        </div>
    </div>

    <div class="form-group">

    </div>

    <div class="form-group">
        <div class="col-md-offset-2">
            <button type="submit" class="btn btn-primary" name="tramita" id="tramita">Tramitar</button>
        </div>
    </div>
</form>