function excluirCurso( curid ){
	
	
	if(confirm('Dseja excluir este curso?')){
		jQuery.ajax({
			type: "POST",
			url: window.location,
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
