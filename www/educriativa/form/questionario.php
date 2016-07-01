<div class="card" style="margin-top: 30px">
	<div class="card-body ">

        <div class="alert alert-warning">
            <p><i class="fa fa-bell-o"></i> Esta <b>chamada p�blica</b> busca encontrar organiza��es que desenvolvem ou pretendem desenvolver estrat�gias de articula��o inovadoras e criativas. Sugerimos que preenchimento deste formul�rio seja feito como um processo de autorreflex�o, envolvendo os diversos segmentos que comp�em a organiza��o.</p>
        </div>

		<?php $perguntas = $pergunta->lista(
				array('p.perid', 'p.pertexto', 'p.perresumo', 'r.restexto'), null,
				array('left' => array('criatividadeeducacao.resposta r' => "r.perid = p.perid AND r.queid = {$_SESSION['queid']}")),
				array('order' => 'p.perordem', 'alias' => 'p')); ?>
		<?php foreach ($perguntas as $data) : ?>
		<div class="col-sm-12">
			<label for="perid[<?php echo $data['perid']; ?>]" class="control-label" style="line-height: 18px">
				<span class="campo_obrigatorio">*</span> 
				<?php echo $data['pertexto']; ?>
			</label>
			<p>
				<small style="color: #6f7676; font-size: 13px; padding-top: 10px;"><?php echo nl2br($data['perresumo']); ?></small>
			</p>
			<div class="form-group" style="padding-top: 0px;">
				<textarea style="border: 1px solid black; padding: 4px 10px 4px 10px;" required data-perid="<?php echo $data['perid']; ?>" name="perid[<?php echo $data['perid']; ?>]" id="perid_<?php echo $data['perid']; ?>" class="form-control question" rows="6" required><?php echo $data['restexto']; ?></textarea>
				<em class="help-block countdown" data-input="#perid_<?php echo $data['perid']; ?>" data-max-lenght="3000"></em>
			</div>
		</div>
		<?php endforeach; ?>
		<div class="col-sm-12">
		<div class="alert alert-callout alert-success">
	        <p>Estudante: A palavra refere-se a todas as pessoas que participam como p�blico das organiza��es educativas, abrangendo termos como alunos, aprendizes, educandos, entre outros.</p>
	        <p>Agradecemos sua participa��o.</p>
	    </div>
	    </div>
	</div>
</div>