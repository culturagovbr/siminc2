<div class="row">
	<div class="col-lg-12" id="div_msg_ordenacao" style="display:none">
		<div class="alert alert-dismissable alert-success">
			<strong>Sucesso! </strong><span id="msg_retorno"></span>
		</div>
	</div>
</div>

<input name="grpid" id="grpid" type="hidden" value="<?= $this->view->idGrupo; ?>">

<div class="row">
	<div class="col-lg-12">

		<div class="alert alert-info" role="alert">
			<h4><?= $this->grupoInfos['nomegrupo'] ?></h4>
		</div>

		<ul class="nav nav-pills nav-justified">
			<li class="active">
				<a data-toggle="tab" href="#tabidentificacao" id="lk_identificacao"> <span class="glyphicon glyphicon-asterisk" aria-hidden="true"></span> Identificação </a>
			</li>
			<li>
				<a data-toggle="tab" href="#tabavaliacao" id="lk_avaliacao"> <span class="glyphicon glyphicon-tasks" aria-hidden="true"></span> Avaliação </a>
			</li>
		</ul>

		<hr>

		<div id="tabidentificacao" class="tab-pane fade active in">
			<?php require_once('_identificacao.php'); ?>
		</div>

		<div id="tabavaliacao" class="tab-pane fade" style="display: none;">
			<?php require_once(APPRAIZ_VIEW . 'avaliacao/_index.php'); ?>
		</div>
	</div>
</div>

<script>
	$(function () {
		$('#lk_identificacao').on('click', function () {
			$('#tabavaliacao').hide();
			$('#tabidentificacao').show()
		});
		$('#lk_avaliacao').on('click', function () {
			$('#tabidentificacao').hide();
			$('#tabavaliacao').show();
		});
	});
</script>