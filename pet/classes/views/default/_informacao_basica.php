<div>
	<p><strong>IES:</strong> <?= $this->grupoInfos['instuicaoEnsinoSuperior'] ?> </p>

	<p><strong>Caracteriza��o da Abrang�ncia do Grupo:</strong> <?= $this->grupoInfos['abrangencia'] ?></p>

	<p><strong>CPF/Tutor:</strong> <?= $this->grupoInfos['cpftutor'] ?> - <?= $this->grupoInfos['nometutor'] ?></p>

	<p><strong>In�cio da Tutoria:</strong> <?= $this->grupoInfos['datainiciotutoria'] ?> </p>

	<p>
		<strong>Curso(s) que � (s�o) atendido(s) pelo grupo PET objeto da avalia��o:</strong>
	<ul>
		<?php
		if (!empty($this->grupoInfos['nomecurso'])):
			foreach ($this->grupoInfos['nomecurso'] as $curso): ?>
				<li><?= $curso ?></li>
			<?php
			endforeach;
		endif; ?>
	</ul>
	</p>

	<div class="well">
		<div class="text-center">
			<h5> Lista de Estudantes que integram ou integraram o grupo PET no per�odo de Avalia��o </h5>
		</div>

		<?= $this->discente->getListaDiscentes($this->view->idGrupo); ?>
	</div>

</div>