<?php

    /**
     * M�todo respons�vel por redirecionar para p�gina solicitada e exibir uma mensagem passada como par�metro
     *
     * @name direcionar
     * @author
     * @access public
     * @return mensagem do sucesso ou fracasso
     */
    function direcionar($url, $msg=null){
        if($msg){
            echo "<script>
                    alert('$msg');
                    window.location='$url';
                  </script>";
        } else{
            echo "<script>
                    window.location='$url';
                  </script>";
        }
        exit;
    }

    /**
     * M�todo respons�vel por exibir alertas com a mensagem passada como par�metro
     *
     * @name exibeAlerta
     * @author C�zar Cirqueira
     * @access public
     * @return alert
     */
    function exibeAlerta($msg){
        echo "<script>
                  alert('$msg');
              </script>";
    }

    /**
     * M�todo respons�vel por executar scripts da tela pai partindo da popup
     *
     * @name executarScriptPai
     * @author C�zar Cirqueira
     * @access public
     * @return
     */
    function executarScriptPai($funcao){
        echo "<script>
                  executarScriptPai('$funcao');
              </script>";
    }

    /**
     * M�todo respons�vel por fechar popups
     *
     * @name fecharPopup
     * @author C�zar Cirqueira
     * @access public
     * @return
     */
    function fecharPopup(){
        echo "<script>
                  self.close();
              </script>";
    }

    /**
     * M�todo respons�vel por Consultar o t�tulo da tela
     *
     * @name consultarTituloTela
     * @author C�zar Cirqueira
     * @return t�tulo
     */
    function consultarTituloTela($abacod, $url){
        global $db;

        $sql = "select m.mnudsc
                  from seguranca.menu m
                 where m.mnulink = '$url'";

        return $db->pegaUm($sql);
    }

    /**
     * M�todo respons�vel por Montar o grupo de radio
     *
     * @name montaRadio
     * @author C�zar Cirqueira
     * @return radios
     */
    function montaRadio($nome,$arrayId,$arrayValor,$arrayRotulos,$valorSelecionado,$espacoEntreLinhas){
        $erroConstrucao = false;
        if((count($arrayId)!=count($arrayValor)) || (count($arrayId)!=count($arrayRotulos))){
            echo "O n�mero de Id's e Valores e R�tulos devem se iguais.";
            $erroConstrucao = true;
        }else if($nome==null || empty($nome)){
            echo "O Nome do componente deve ser informado.";
            $erroConstrucao = true;
        }
        if(!$erroConstrucao){
            for($i=0;$i<count($arrayId);$i++){
                $selecaoCampo = ($valorSelecionado==$arrayValor[$i])?"CHECKED":"";
                echo "<input type='radio' id='".$arrayId[$i]."' name='".$nome."' value='".$arrayValor[$i]."' ".$selecaoCampo." >";
                echo "<span onclick='selecionaCampoPorID(\"".$arrayId[$i]."\")'>".$arrayRotulos[$i]."</span>";
                for($j=0;$j<$espacoEntreLinhas;$j++){
                    echo "<br/>";
                }
            }
        }
    }

    function exibeVariaveis($listaVariaveis, $finalizar){
        echo "<pre>";
        foreach ($listaVariaveis as $value) {
            var_dump($value);
        }
        if($finalizar){
            exit;
        }
    }
?>