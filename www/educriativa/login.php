<?php 
include "config.inc";
include_once APPRAIZ . "educriativa/autoload.php";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";

$configuracao = new Educriativa_Model_Configuracao();

$sql = "select to_char(dataexpiracao, 'YYYY-MM-DD') data_termino,
               to_char(dataexpiracao, 'YYYYMMDDHH24MISS') data_expiracao,
               to_char(dataexpiracao, 'DD/MM/YYYY \�\s HH24:MI:SS') as data_formatada
          from criatividadeeducacao.configuracao";

$dados = $configuracao->pegaLinha($sql);

$dataTermino = $dados['data_termino'];
$dataExpiracao = $dados['data_expiracao'];
$dataFormatada = $dados['data_formatada'];
$dias = floor((strtotime($dataTermino) - strtotime(date('Y-m-d'))) / ((60*60*24)+1));
?>
<!DOCTYPE html>
<html lang="pt-BR">
<?php require "header.php"; ?>
<body class="menubar-hoverable header-fixed ">

	<!-- BEGIN HEADER-->
	<!-- barra do governo -->
	<div id="barra-brasil">
	  <a href="http://brasil.gov.br" class="barraGoverno">Portal do Governo Brasileiro</a>
	</div>
	<!-- fim barra do governo -->

	<div class="container">
	  <div id="topo">
	    <div class="row">
	      <div class="col-md-3 col-sm-6 col-xs-6 logo">
	      	<img src="img/logo.png" alt="">
	       </div>
	    </div> <!--  row -->
	  </div> <!--  topo -->
	</div> <!--  container -->
	<!-- END HEADER-->

	<!-- BEGIN BASE-->
	<div id="base">
	
		<!-- BEGIN CONTENT-->
		<div id="content" class="section-body contain-lg shadow">
			<section>
				<!-- BEGIN VALIDATION FORM WIZARD -->
				<div class="row">
					<?php if ($_GET['sucesso']): ?>
		                <div class="col-lg-12 col-sm-12 col-xs-12">
		                    <div class="alert alert-success" role="alert">
		                        <p>Sua inscri��o na Chamada P�blica Inova��o e Criatividade na Educa��o B�sica foi finalizada corretamente.</p>
		                        <p><strong>Obrigado por sua participa��o.</strong></p>
		                    </div>
		                </div>
		            <?php endif; ?>
		
		            <?php if ($_GET['finalizado']): ?>
		                <div class="col-lg-12 col-sm-12 col-xs-12">
		                    <div class="alert alert-warning" role="alert">
		                        <p>Este formul�rio j� foi finalizado!</p>
		                        <p>Agrade�emos sua participa��o na pesquisa.</p>
		                    </div>
		                </div>
		            <?php endif; ?>
					
					<?php if(date('YmdHis') >= $dataExpiracao) : ?>
		                <div class="col-lg-12 col-sm-12 col-xs-12">
		                    <div class="alert alert-success" role="alert">
		                        <p>Envio do questionario encerrado em <?php echo $dataFormatada; ?>.</p>
		                        <p>Obrigado pela colabora��o!</p>
		                    </div>
		                </div>
		            <?php else : ?>
					<div class="col-sm-6" style="text-align: justify;">
						<div class="panel-group" id="accordion3">
							<div class="card panel expanded">
								<div class="card-head card-head-sm collapsed link" data-toggle="collapse" data-parent="#accordion3" data-target="#accordion3-1" aria-expanded="true">
									<header>O que � o projeto Inova��o e Criatividade na Educa��o B�sica?</header>
									<div class="tools">
										<a class="btn btn-icon-toggle"><i class="fa fa-angle-down"></i></a>
									</div>
								</div>
								<div id="accordion3-1" class="collapse in">
									<div class="card-body">						
										<p>O s�culo 21 j� inicia com uma revolu��o na comunica��o, que traz mudan�as no mundo da educa��o e do trabalho. As novas tecnologias facilitam o autoaprendizado, a forma��o de comunidades de aprendizagem e de redes e a produ��o de conhecimento em diversos suportes.  Tamb�m no mundo do trabalho as rela��es s�o mais fluidas, menos regulamentadas, carreiras e caminhos profissionais surgem de um dia para o outro e a dura��o da vida ativa se prolonga para al�m dos convencionais 60, 65 anos. � medida que ocorrem as transforma��es, aumenta o compromisso �tico com as gera��es do futuro, exigindo prud�ncia e criatividade para encontrar novas formas sustent�veis de lidar com os recursos ambientais. </p>
										<p>Diante deste quadro, o projeto visa criar as bases para uma pol�tica p�blica de fomento � inova��o e � criatividade na educa��o b�sica, estimulando as escolas, institui��es e organiza��es que ousaram romper com os padr�es educacionais tradicionais para criar uma nova escola que forme cidad�os integrais, felizes, produtores de conhecimento e cultura e que se relacionem com o planeta de modo respons�vel, sustent�vel e respeitoso. Pretende fortalecer as experi�ncias inovadoras para que elas superem o isolamento, a fragmenta��o, a descontinuidade no tempo e a depend�ncia de voluntarismo; levantar refer�ncias de inova��o para uma efetiva mudan�a da educa��o b�sica e apoiar os processos que ampliam o impacto das experi�ncias inovadoras relevantes para al�m de seu polo inicial.</p>
									</div>
								</div>
							</div>
							<div class="card panel">
								<div class="card-head card-head-sm collapsed link" data-toggle="collapse" data-parent="#accordion3" data-target="#accordion3-2" aria-expanded="false">
									<header>Quais as caracter�sticas de uma organiza��o inovadora e criativa?</header>
									<div class="tools">
										<a class="btn btn-icon-toggle"><i class="fa fa-angle-down"></i></a>
									</div>
								</div>
								<div id="accordion3-2" class="collapse">
									<div class="card-body">						
				                        <p>1.	GEST�O: Corresponsabiliza��o na constru��o e gest�o do projeto pol�tico pedag�gico: Estrutura��o do trabalho da equipe, da organiza��o do espa�o, do tempo e do percurso do estudante com base em um sentido compartilhado de educa��o, que orienta a cultura institucional e os processos de aprendizagem e de tomada de decis�o, garantindo-se que os crit�rios de natureza pedag�gica sejam sempre preponderantes.</p>
				                        <p>2.	CURR�CULO: Desenvolvimento integral: Estrutura��o de um curr�culo voltado para a forma��o integral, que reconhece a multidimensionalidade da experi�ncia humana - afetiva, �tica, social, cultural e intelectual.</p>
				                        <p>Produ��o de conhecimento e cultura: Estrat�gias voltadas para tornar a institui��o educativa espa�o de produ��o de conhecimento e cultura, que conecta os interesses dos estudantes, os saberes comunit�rios e os conhecimentos acad�micos para transformar o contexto socioambiental.</p>
				                        <p>Sustentabilidade (social, econ�mica, ecol�gica e cultural): Estrat�gias pedag�gicas que levem a uma nova forma de rela��o do ser humano com o contexto planet�rio.</p>
				                        <p>3.	AMBIENTE: Ambiente f�sico que manifeste a inten��o de educa��o humanizada, potencializadora da criatividade, com os recursos dispon�veis para explora��o e a conviv�ncia enriquecedora nas diferen�a. Estrat�gias que estimulam o di�logo entre os diversos segmentos da comunidade, a media��o de conflitos por pares, o bem-estar de todos, a valoriza��o da diversidade e das diferen�as e a promo��o da equidade.</p>
				                        <p>4.	M�TODOS: Protagonismo: Estrat�gias pedag�gicas que reconhecem o estudante como protagonista de sua pr�pria aprendizagem; que reconhecem e permitem ao estudante expressar sua singularidade e desenvolver projetos de seu interesse que impactem a comunidade e que contribuam para a sua futura forma��o profissional.</p>
				                        <p>5.	ARTICULA��O COM OUTROS AGENTES: Rede de direitos: Estrat�gias intersetoriais e em rede, envolvendo a comunidade, para a garantia dos direitos fundamentais dos estudantes, reconhecendo-se que o direito � educa��o � indissoci�vel dos demais.</p>
									</div>
								</div>
							</div>
							<div class="card panel">
								<div class="card-head card-head-sm collapsed link" data-toggle="collapse" data-parent="#accordion3" data-target="#accordion3-3" aria-expanded="false">
									<header>Qual o objetivo da chamada p�blica?</header>
									<div class="tools">
										<a class="btn btn-icon-toggle"><i class="fa fa-angle-down"></i></a>
									</div>
								</div>
								<div id="accordion3-3" class="collapse">
									<div class="card-body">						
			                        	<p>O objetivo da chamada p�blica � mapear e caracterizar as interven��es inovadoras que ocorrem em n�vel local, por iniciativa de escolas, comunidades ou outras organiza��es educativas. As organiza��es que apresentarem as caracter�sticas de inova��o e criatividade e aquelas que demonstrarem estrat�gias claras para desenvolver estas caracter�sticas ser�o reconhecidas e divulgadas pelo MEC e, em fase posterior, poder�o se inscrever em programas voltados para o seu fortalecimento.</p>
									</div>
								</div>
							</div>
							<div class="card panel">
								<div class="card-head card-head-sm collapsed link" data-toggle="collapse" data-parent="#accordion3" data-target="#accordion3-4" aria-expanded="false">
									<header>Quem pode participar?</header>
									<div class="tools">
										<a class="btn btn-icon-toggle"><i class="fa fa-angle-down"></i></a>
									</div>
								</div>
								<div id="accordion3-4" class="collapse">
									<div class="card-body">
				                        <ol type="I">
                                            <li>Escolas p�blicas de educa��o b�sica (educa��o infantil, ensino fundamental, ensino m�dio, ensino t�cnico e EJA) das redes p�blicas federal, estaduais/distrital e municipais.</li>
											<li>Escolas privadas de educa��o b�sica (educa��o infantil, ensino fundamental, ensino m�dio e/ou ensino m�dio integrado e EJA).</li>
											<li>Associa��es, organiza��es sociais (OS) e organiza��es da sociedade civil que atuam no campo da educa��o com crian�as, adolescentes e/ou jovens.</li>
											<li>Institui��es educacionais comunit�rias, filantr�picas e confessionais que atuam com crian�as, adolescentes e/ou jovens.</li>
				                        </ol>
									</div>
								</div>
							</div>
						</div>
                    </div>
					<div class="col-sm-6">
						<br>
						<div class="card">
							<div class="card-head card-head-sm style-primary">
								<header><i class="fa fa-user"></i> LOGIN</header>
							</div>
							<div class="card-body">
								<form action="controlador.php" class="form floating-label form-validation" role="form" novalidate="novalidate" accept-charset="utf-8" method="post">
									<div class="col-sm-12">
										<div class="form-group">
											<input type="text" required class="form-control" data-inputmask="'mask': '999.999.999-99', 'showMaskOnHover': false" id="usucpf" name="usucpf">
											<label for="usucpf">CPF do participante</label>
											<?php echo ( (isset($_GET['cpf']) and $_GET['cpf'] == 1) ? ' <p class="help-block has-error login-error">Campo Obrigat�rio</p>' : ''); ?>
		                            		<?php echo ( (isset($_GET['cpf']) and $_GET['cpf'] == 2) ? ' <p class="help-block has-error login-error">CPF Incorreto!</p>' : ''); ?>
										</div>
									</div>
									<div class="col-sm-9">
										<div class="form-group <?php echo ( isset($_GET['captcha']) ? ' has-error' : ''); ?>">
			                            	<input type="text" required class="form-control" name="captcha" id="captcha" maxlength="4">
			                            	<label for="captcha">Digite os caracteres da imagem ao lado</label>
			                            	<?php echo ( (isset($_GET['captcha']) and $_GET['captcha'] == 1) ? ' <p class="help-block has-error login-error">Campo Obrigat�rio</p>' : ''); ?>
				                            <?php echo ( (isset($_GET['captcha']) and $_GET['captcha'] == 2) ? ' <p class="help-block has-error login-error">Caracteres Incorretos - digite novamente!</p>' : ''); ?>
										</div>
									</div>
									<div class="col-sm-3" style="padding: 0px">
										<img src="captcha.php" width="113" height="49">
									</div>
									<br>
									<div class="row">
										<div class="col-xs-12 text-center">
											<button class="btn btn-primary btn-raised" type="submit">Entrar</button>
										</div><!--end .col -->
									</div><!--end .row -->
								</form>
							</div>
							<div class="card-footer text-center">
								<h4 class="text-default-light">Navegadores compativeis</h4>
								<img src="img/navegadores.png" />
							</div>
						</div>
					</div>
					<?php endif; ?>
				</div><!--end .row -->
				<!-- END VALIDATION FORM WIZARD -->
			</section>
		</div><!--end #content-->
		<!-- END CONTENT -->

	</div><!--end #base-->
	<!-- END BASE -->

    <!-- Modal -->
    <div class="modal fade" id="myModal" tabindex="-1" role="dialog" aria-labelledby="myModalLabel">
        <div class="modal-dialog" role="document">
            <div class="modal-content">
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
                    <h4 class="modal-title" id="myModalLabel">Aten��o!</h4>
                </div>
                <div class="modal-body">

                    <?php if ($_GET['sucesso']){ ?>
                        <p>Sua inscri��o na Chamada P�blica Inova��o e Criatividade na Educa��o B�sica foi finalizada corretamente.</p>
                        <p><strong>Obrigado por sua participa��o.</strong></p>
                    <?php } else {
                        echo 'Leia as informa��es ao lado para saber se voc� pode se inscrever.';
                    }?>

                </div>
                <div class="modal-footer">
                    <button type="button" class="btn btn-default" data-dismiss="modal">OK</button>
                </div>
            </div>
        </div>
    </div>

	<?php require_once "footer.php"; ?>

</body>
