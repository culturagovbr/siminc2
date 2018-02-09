<?php 
	include  APPRAIZ."includes/cabecalho.inc"; 
	
	if($_POST['enviar_email'] == ''){

?>

<form name="form" id="form" method="post" action="" >
	<input type="hidden" name="enviar_email" id="enviar_email" value="S">
	<input type="submit" name="enviar" id="enviar" value="Enviar e-mail Web Conf">
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
			select	est.estdescricao || ' - ' || pcu.pcunome as usunome,
					pcu.pcuemail as usuemail
			from territorios.estado est
			left join par.instrumentounidade inu on inu.estuf = est.estuf
			left join par.pfadesaoprograma adp on adp.inuid = inu.inuid and adp.tapid in (13,14)
			left join workflow.documento doc on doc.docid = adp.docid
			left join workflow.estadodocumento esd on esd.esdid = doc.esdid
			left join par.pftermoadesaoprograma tap on adp.tapid = tap.tapid and tap.prgid in (157)
			left join par.pfcurso pfc on pfc.prgid = tap.prgid and pfcstatus = 'A'
			left join par.pfcursista pcu on pcu.adpid = adp.adpid and pcu.pfcid = pfc.pfcid
			left join public.tipoformacao tfo on tfo.tfoid = pcu.tfoid
			left join public.tipovinculoprofissional tvp on tvp.tvpid = pcu.tvpid
			left join par.pffuncao pff on pff.pffid = pcu.pffid  
		";
			
			$us = $db->carregar($sql);
			
			$us[] = array("usuemail"=>"luciano.fr.ribeiro@gmail.com","usunome"=>"luciano");
			//$us[] = array("usuemail"=>"missaomip@hotmail.com","usunome"=>"Luciano");
			$us[] = array("usuemail"=>"wallcp@gmail.com","usunome"=>"Wallace");
		
		//$path_0 = '/var/www/simec/simec_dev/simec/www/anexo_email_wallace/SisPacto_Manual_Orientacoes.pdf';	
	
		if($us[0]){
			foreach($us as $u){
				
				$mensagem = new PHPMailer();
				$mensagem->persistencia = $db;
				$mensagem->Host         = "localhost";
				$mensagem->Mailer       = "smtp";
				$mensagem->FromName		= "IV Webconfer�ncia sobre o Pacto Nacional pela Alfabetiza��o na Idade Certa";
				$mensagem->From 		= $_SESSION['email_sistema'];
				$mensagem->AddAddress( $u['usuemail'], $u['usunome'] ); 
				
				echo $i.' - '.$u['usuemail'].' - '.$u['usunome'].'<br>';
				
				//$mensagem->AddAttachment($path_0);
				
				$mensagem->Subject = "Pacto Nacional pela Alfabetiza��o na Idade Certa - cadastramento dos Orientadores de Estudo";
				$mensagem->Body = "
						<p><b>Pacto Nacional pela Alfabetiza��o na Idade Certa - cadastramento dos Orientadores de Estudo</b></p>
						<p>Prezado(a) coordenador(a)</p>  
						<p>Convidamos voc� para a IV Webconfer�ncia sobre o Pacto Nacional pela Alfabetiza��o na Idade Certa que se realizar� no <b>dia 30 de outubro</b>, a partir das 10:00. Para assisti-la em tempo real, voc� deve digitar no seu computador http://portal.mec.gov.br/seb/transmissao, no dia e hora indicados.</p> 
						<p>Aproveitamos este e-mail para as seguintes informa��es:</p>
						<p>- o prazo para indica��o dos Orientadores de Estudo � 16 de novembro. Para isso entre no site http://simec.mec.gov.br/, digite seu CPF e sua senha e acesse o SisPACTO, que est� aberto desde 19/10;</p>
						<p>- a senha de acesso, para quem nunca teve acesso ao SIMEC � simecdti, em letras min�sculas. Para quem j� tinha acesso ao SIMEC, utilize a senha que possui. Caso n�o lembre desta senha, utilize a fun��o \"Esqueceu a senha?\", que aparece na p�gina de entrada do SIMEC, conforme endere�o acima;</p>
						<p>- o coordenador n�o pode indicar a si mesmo para a fun��o de orientador de estudos. Caso a Secretaria deseje fazer esta altera��o, antes � necess�rio que o Dirigente Municipal de Educa��o substitua o coordenador. Isso � feito no pr�prio SisPACTO, conforme o manual j� enviado a voc�s.</p>
						<p>
						Atenciosamente,<br>
						MEC/Secretaria de Educa��o B�sica
						</p>
				";
				
				$mensagem->IsHTML( true );
				$mensagem->Send();
				
				$i = $i+1;
				
				//if($i <= 5000){
					//$sql = "UPDATE pdeinterativo.listapdeinterativo SET email_enviado='S' WHERE lower(trim(usuemail)) = lower(trim('".$u['usuemail']."'));";
					//$db->executar( $sql );
					//$db->commit();
				//}		
			}
		}
	
		if($i >= 27){
			echo 'Foi enviado!';
		}
	}
		//$sql = "Select count(lstid) as qtd From temporario.email_pde_interativo_est Where email_enviado='S';";
		//$qtd = $db->pegaUm($sql);
		
		//if($qtd == 21384){
			//echo 'Foi enviado!';
		//}else{
			//echo "<script>window.location='pdeinterativo.php?modulo=sistema/geral/envia_email_sis&acao=A&repetir=S';</script>";
		//}
		
