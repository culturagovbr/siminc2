<?php 
	include  APPRAIZ."includes/cabecalho.inc"; 
	
	if($_POST['enviar_email'] == ''){

?>

<form name="form" id="form" method="post" action="" >
	<input type="hidden" name="enviar_email" id="enviar_email" value="S">
	<input type="submit" name="enviar" id="enviar" value="Enviar e-mail">
</form>

<?php 
	} elseif($_POST['enviar_email'] == 'S') {
		include  APPRAIZ."includes/cabecalho.inc";
		/* configura��es */
		ini_set("memory_limit", "3000M");
		set_time_limit(0);
		
		require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
		require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
			
		global $db;
		
		$sql = "
			Select	lstid, 
					lower(trim(usuemail)) as usuemail, 
					Initcap(trim(usunome)) as usunome, 
					email_enviado
			From temporario.email_pde_interativo
			where lower(trim(usuemail)) ilike '%@%' and email_enviado = 'N' 
			order by 2
			limit 3000
		";
		
		$us = $db->carregar($sql);
		
		$us[] = array("usuemail"=>$_SESSION['email_sistema'],"usunome"=>SIGLA_SISTEMA);
		
		$path_0 = '/var/www/simec/simec_dev/simec/www/anexo_email_wallace/premio_professores_email_marketing.jpg';
		$path_1 = '/var/www/simec/simec_dev/simec/www/anexo_email_wallace/premio_professores.mp3';
		
		$i=0;
	
		if($us[0]){
			foreach($us as $u){
				
				$mensagem = new PHPMailer();
				$mensagem->persistencia = $db;
				$mensagem->Host         = "localhost";
				$mensagem->Mailer       = "smtp";
				$mensagem->FromName		= "PRORROGA��O - Inscri��o para Pr�mio Professores do Brasil";
				$mensagem->From 		= $_SESSION['email_sistema'];
				$mensagem->AddAddress( $u['usuemail'], $u['usunome'] ); 
				
				echo $i.' - '.$u['usuemail'].' - '.$u['usunome'].'<br>';
				
				$mensagem->AddAttachment($path_0);
				$mensagem->AddAttachment($path_1);
				
				$mensagem->Subject = "Assunto: PRORROGA��O - Inscri��o para Pr�mio Professores do Brasil";
				$mensagem->Body = "
						<p><b>Inscri��es do Pr�mio Professores do Brasil prorrogadas at� 10 de novembro de 2012.</b></p>
						<p>Professores de todo o pa�s podem se inscrever e concorrer a pr�mio por iniciativas de ensino bem-sucedidas</p>  
						<p>Est�o prorrogadas, at� o dia 10 de novembro de 2012, as inscri��es para o 6� Pr�mio Professores do Brasil. A iniciativa do Minist�rio da Educa��o foi institu�da pela Secretaria de Educa��o B�sica (SEB) para valorizar pr�ticas pedag�gicas bem-sucedidas, criativas e inovadoras nas redes p�blicas de ensino.</p> 
						<p>Este ano, foi criada uma segunda categoria, sobre temas espec�ficos, al�m da j� conhecida, de temas livres. Esta � subdividida nas �reas de educa��o infantil, anos iniciais do ensino fundamental, anos finais e ensino m�dio. O novo m�dulo conter� projetos de educa��o integral ou integrada, ci�ncias para os anos iniciais, alfabetiza��o nos anos iniciais e educa��o digital articulada ao desenvolvimento do curr�culo.</p>
						<p>Cada categoria ter� at� quatro professores premiados em cada uma das subcategorias, um por regi�o do pa�s. Os autores das experi�ncias selecionadas pela comiss�o julgadora nacional, independentemente de regi�o e da categoria, receber�o R$ 7 mil, al�m de trof�u e certificados expedidos pelas institui��es parceiras.</p>
						<p>As inscri��es para a sexta edi��o devem ser feitas na p�gina do pr�mio na internet - http://www.premioprofessoresdobrasil.mec.gov.br/. Nela, o professor tamb�m encontra informa��es relevantes e o regulamento do 6� Pr�mio Professores do Brasil.</p>
						<p>Diretor, convoque os professores de sua escola para participar do 6� Pr�mio Professores do Brasil.</p>
						<p>
						Bras�lia, 29 de novembro de 2012.<br>
						Secretaria de Educa��o B�sica<br>
						MINIST�RIO DA EDUCA��O
						</p>
				";
			
				$mensagem->IsHTML( true );
				$mensagem->Send();
				
				$i = $i+1;
				
				if($i <= 3000){
					$sql = "UPDATE temporario.email_pde_interativo SET email_enviado = 'S' WHERE lstid = ".$u['lstid'].";";
					$db->executar( $sql );
					$db->commit();
				}
						
			}
		}
	
		$sql = "Select count(lstid) as qtd From temporario.email_pde_interativo Where email_enviado='S';";
		$qtd = $db->pegaUm($sql);
		
		//if($i >= 1 ){
		if($qtd == 107879 ){
			echo 'foi enviado!';
		}else{
			echo "<script>window.location='pdeinterativo.php?modulo=sistema/geral/envia_email_sis&acao=A&enviar_email=S';</script>";
		}
	}