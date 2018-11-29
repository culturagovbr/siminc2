<style>
    .wizard > .content {
        overflow: visible;
    }

    .wizard .chosen-container .chosen-results {
        height: 80px;
        overflow-y: auto;
        overflow-x: hidden;
    }

    .wizard .chosen-container .chosen-drop {
        height: 120px;
    }

</style>

<div class="row">
    <div class="col-lg-12">
        <div style="text-align: center; color: #1AB394;">
            <h3>PLANOS INTERNOS</h3>
        </div>
        <div class="ibox-title">
            <?php include APPRAIZ.'alteracao/modulos/principal/lista_pi_loa.inc';?>
        </div>
        <button class="btn btn-primary" type="button" id="btnloa" name="btnloa">
            <span class="glyphicon glyphicon-plus"></span> Adicionar
        </button>
    </div>
    <div class="col-lg-12">
        <div style="text-align: center; color: #1AB394;">
            <h3>LOA</h3>
        </div>
        <div class="ibox-title">
            <?php include APPRAIZ.'alteracao/modulos/principal/lista_loa.inc';?>
        </div>
    </div>
</div>