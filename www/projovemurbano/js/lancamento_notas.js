var TrabalhoMensal    = {
    
    init : function(){
    	$(this).attr('disabled', true);
        $('#nucid').change( TrabalhoMensal.nucleoChangeHandler );
        $('#btnVisualizarDiario').click( TrabalhoMensal.visualizarTrabalhoHandler )
                                 .attr('disabled', true);
        jQuery.ajaxSetup({
            beforeSend: function(){
                $("#dialogAjax").show();
            },
            complete: function(){
                $("#dialogAjax").hide();
            }
        });
        
        TrabalhoMensal.verificaCamposPreenchidos();
    },
    
    verificaCamposPreenchidos : function(){
        if( jQuery.trim($('#perid').val()) != '' && 
                jQuery.trim($('#turid').val() != '') ){
                TrabalhoMensal.visualizarTrabalhoHandler();
                $('#btnVisualizarDiario').attr('disabled', false);
        }
    },
    
    nucleoChangeHandler : function(){
        var nucid   = $('#nucid').val(), 
            params  = {};
         $('#container-trabalho-mensal').html('');
            
        if( nucid == '' )
        {
            return false;
        }
        
        params['nucid'] = nucid;
        params['acao']  = 'listarTurma';
        
        $.post( 'geral/ajax.php', params, function(response){
            $('#container-turma').html( response );
            $('#turid').change( TrabalhoMensal.turmaChangeHandler );
        }, 'html' );
            
            
        return true;
    },
    
    turmaChangeHandler : function(){

        var turid   = $('#turid').val(),
            params  = {};
        
        $('#container-trabalho-mensal').html('');
        
        if( turid == '' )
        {
        	$('#perid option:[value!=""]').remove();
        	$('#perid option:first').html("Selecione uma turma");
        	
            return false;
        }
        
        params['turid'] = turid;
        params['acao']  = 'listarPeriodos';
        
        $.post( 'geral/ajax.php', params, function(response){
            $('#container-diario').html( response );
             $('#btnVisualizarDiario').attr('disabled', false);
        }, 'html' );
            
        return true;
    },
    visualizarTrabalhoHandler : function()
    {
        var perId   = $('#perid').val(),
            turId   = $('#turid').val(),
            params  = {};
            
        if( perId == '' )
        {
            alert(' Selecione um per�odo para visualiza��o do(s) di�rio(s) de trabalho.');
            return false;
        }
        
        params['perid'] = perId;
        params['turid'] = turId;
        params['acao']  = 'visualizarDiarioLancamentoNotas';
        
        
        $.post( 'geral/ajax.php', params, function(response){
            $('#container-trabalho-mensal').html( response );
            $('#btnSalvarTrabalho').click( TrabalhoMensal.salvarDiarioTrabalhoHandler );
            $('#btnFecharTrabalho').click(  TrabalhoMensal.fecharDiario );
        }, 'html' );
            
        return true;
    },
    fecharDiario :function()
    {
        if( confirm('Deseja realmente fechar esse di�rio?\nAp�s o seu fechamento o mesmo n�o poder� ser mais editado.') )
            {
                var params;
        
                $('#acao').val('fecharDiario');
                
                params = $('#frmTrabalho').serialize();
                
                params['diaid'] = $('#diaid').val();
                
                $.post( 'geral/ajax.php', params, function(response){
                    
                    alert(response.retorno);

                    if( response.status == true  )
                    {
                        TrabalhoMensal.visualizarTrabalhoHandler();
                    }
                    
                }, 'json' );
            }
    }
    
};