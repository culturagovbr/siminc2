function excluirCurso( curid ){
	
	
	if(confirm('CUIDADO! Deletando este curso voc� estar� deletando todos os outros dados vinculados � este. Voc� tem certeza que dseja excluir este curso?')){
		jQuery.ajax({
			type: "POST",
			url: window.location.href,
			data: "req=excluirCurso&curid="+curid,
			async: false,
			success: function(msg){
				alert('Curso exclu�do.');
				window.location = window.location;
			}
		});
	}
	
}


$(document).ready(function() {
	
	$('#pesquisar').click(function(){
		
		$('#frmCatalogo').submit();
	});
});
