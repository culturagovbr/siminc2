<?php 
	include  APPRAIZ."includes/cabecalho.inc"; 
	
	if($_POST['enviar_email'] == ''){

?>

<form name="form" id="form" method="post" action="" >
	<input type="hidden" name="enviar_email" id="enviar_email" value="S">
	<input type="submit" name="enviar" id="enviar" value="Enviar e-mail">
</form>

<?php 
	}
	
	elseif($_POST['enviar_email'] == 'S'){
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
			From temporario.email_pde_interativo_est
			Where lower(trim(usuemail)) ilike '%@%' and email_enviado = 'N' 
			order by 2
			limit 5000
		";
		
		$us = $db->carregar($sql);
		
		$us[] = array("usuemail"=>"luciano.fr.ribeiro@gmail.com","usunome"=>"luciano");
		//$us[] = array("usuemail"=>"missaomip@hotmail.com","usunome"=>"Luciano");
		$us[] = array("usuemail"=>"wallcp@gmail.com","usunome"=>"Wallace");
		
		
		$path_0 = '/var/www/simec/simec_dev/simec/www/anexo_email_wallace/despacho.pdf';
		$path_1 = '/var/www/simec/simec_dev/simec/www/anexo_email_wallace/mensagem_diretores.pdf';
		
	
		if($us[0]){
			foreach($us as $u){
				$mensagem = new PHPMailer();
				$mensagem->persistencia = $db;
				$mensagem->Host         = "localhost";
				$mensagem->Mailer       = "smtp";
				$mensagem->FromName		= "Carta do Senhor Secret�rio de Educa��o B�sica - PACTO";
				$mensagem->From 		= $_SESSION['email_sistema'];
				$mensagem->AddAddress( $u['usuemail'], $u['usunome'] ); 
				
				echo $i.' - '.$u['usuemail'].' - '.$u['usunome'].'<br>';
				
				$mensagem->AddAttachment($path_0);
				$mensagem->AddAttachment($path_1);
				$mensagem->Subject = "Assunto: Carta do Senhor Secret�rio de Educa��o B�sica - PACTO";
				$mensagem->Body = "
						<p><b>Carta do Senhor Secret�rio de Educa��o B�sica - PACTO</b></p>
						<p>O minist�rio da Educa��o na pessoa do Senhor Secret�rio de Educa��o B�sica, encaminha em anexo, carta informativa com orienta��es sobre o Pacto Nacional pela Alfabetiza��o na Idade Certa, voltadas para os diretores de escolas p�blicas estaduais e municipais.</p>
						
						<p>
						Bras�lia, 22 de outubro de 2012.<br>
						".$_SESSION['email_sistema']."<br>
						MINIST�RIO DA EDUCA��O
						</p>
				";
			
				$mensagem->IsHTML( true );
				$mensagem->Send();
				
				$i = $i+1;
				
				if($i <= 5000){ 
					$sql = "UPDATE temporario.email_pde_interativo_est SET email_enviado = 'S' WHERE lstid = ".$u['lstid'].";";
					$db->executar( $sql );
					$db->commit();
				}			
			}
		}
	
		$sql = "Select count(lstid) as qtd From temporario.email_pde_interativo_est Where email_enviado='S';";
		$qtd = $db->pegaUm($sql);
		
		if($qtd == 21384){
			echo 'Foi enviado!';
		}else{
			echo "<script>window.location='pdeinterativo.php?modulo=sistema/geral/envia_email_sis&acao=A&repetir=S';</script>";
		}
	}