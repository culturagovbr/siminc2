<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Description of relatorio
 *
 * @author rafael
 */
class relatorioSimple {
    
    public $db;
    private $colunas;
    private $colunasSelecionadas;
    private $colunasNaoSelecionadas;
    private $dados;
    private $listaColunas;
    private $listaJoins;
    private $tabelaBase;
    private $filtros;
    private $totalizador = array();
    private $ordenacao;
    private $tipoRelatorio;
    private $negrito=false;
    
    const STRING = 'string';
    const NUMERIC = 'numeric';
    const CURRENCY = 'currency';
    
    Public Function __construct() {
//        if (class_exists('Banco')) {
//            $this->db = new Banco;
//        } else {
//            $this->db = (object) '';
//        }
    }
    /**
     * @param type $colunasSelecionadas
     * @param type $colunasNaoSelecionadas
     */
    Public Function setColunas($colunasSelecionadas, $colunasNaoSelecionadas){
        $this->colunasNaoSelecionadas = $colunasNaoSelecionadas;
        $this->colunasSelecionadas = $colunas = $colunasSelecionadas;
//        ver(count($colunasNaoSelecionadas),d);
        if (count($colunasNaoSelecionadas)>0){
            foreach($colunasNaoSelecionadas as $value){
                $colunas[] = $value;
            }
        }
        $this->colunas = $colunas;
    }
    
    Public Function setDados($dados){
        $this->dados = $dados;
    }
    
    Public Function setListaColunas($lista){
        $this->listaColunas = $lista;
    }
    
    Public Function setListaJoins($lista){
        $this->listaJoins = $lista;
    }
    
    Public Function setTabelaBase($tabela){
        $this->tabelaBase = $tabela;
    }
    
    Public Function setFiltros($filtros){
        $this->filtros = $filtros;
    }
    
    Public Function setTotalizador($campo, $valor){
        if (!array_key_exists($campo,$this->totalizador)){
            $this->totalizador[$campo] = $valor;
        }else{
            $this->totalizador[$campo] += $valor;
        }
    }
    
    Public Function setOrdenacao($campo){
        if ($campo){
            $this->ordenacao = " ORDER BY ".$campo;
        }
    }
    
    Public Function setTipoRelatorio($tipoRelatorio){
        $this->tipoRelatorio = $tipoRelatorio;
    }
    
    Public Function MontaSelect(){
        $colunas = $this->montaColunas();
        $joins = $this->montaJoin();
        $where = $this->montaWhere();
        if ($where){
            $where = " AND ".implode(' AND ', $where);
        }        
        $sql = "select ".implode(', ', $colunas)." from ".$this->tabelaBase." ".implode(' ', $joins)." where 1=1 ".$where.$this->ordenacao;
//        ver($sql,d);
        $this->dados = $this->db->carregar($sql);
    }
    
    Public Function getRelatorio(){
        $table = "<form name='formRelatorio' method='POST'>";
        $table = $this->adicionaCampos($table);
        $table .= "<div class='table-responsive' style='height: 90%; width:100%'>";
        $table .= "<table width=\"100%\" cellspacing=\"1\" cellpadding=\"5\" border=\"0\" align=\"center\" class=\"table tabela\">";
        $table = $this->montaCabecalho($table);
        $table = $this->montaCorpo($table);
        $table = $this->montaRodape($table);
        $table .= "</table>";
        $table .= "</div>";
        $table = $this->adicionaScript($table);
        $table .= "</form>";
        echo $table;
    }
    
    Public Function getRelatorioXls(){
        header( 'Content-Type: application/vnd.ms-excel' );
        header( 'Expires: 0' );
        header( 'Cache-Control: must-revalidate, post-check=0, pre-check=0' );
        header( 'Content-Disposition: attachment; filename="relatorio_geral' . '.xls"' );        
        $table .= "<table width=\"100%\" cellspacing=\"1\" cellpadding=\"5\" border=\"0\" align=\"center\" class=\"table tabela\">";
        $table = $this->montaCabecalho($table);
        $table = $this->montaCorpo($table);
        $table = $this->montaRodape($table);
        $table .= "</table>";
        echo $table;
    }
    
    Public Function montaCabecalho($table){
        $table .= "<tr>";
        foreach($this->colunasSelecionadas as $value){
            foreach($this->listaColunas as $key => $colunas){
                if ($key===$value){
                    switch ($colunas['type']) {
                        case self::STRING:
                            $table .= "<td align='left'>";
                            $table .= "<div><a href=javascript:ordena('".$colunas['alias'].".".$key."');>".$colunas['label']."</a></div>";
                            $table .= "</td>";
                            break;
                        case self::NUMERIC:
                            $table .= "<td align='right'>";
                            $table .= "<div><a href=javascript:ordena('".$colunas['alias'].".".$key."');>".$colunas['label']."</a></div>";
                            $table .= "</td>";
                            break;
                        case self::CURRENCY:
                            $table .= "<td align='right'>";
                            $table .= "<div><a href=javascript:ordena('".$colunas['alias'].".".$key."');>".$colunas['label']."</a></div>";
                            $table .= "</td>";             
                            break;

                        default:
                            break;
                    }
                }
            }
        }
        $table .= "</td></tr>";
        return $table;
    }
    
    Public Function adicionaCampos($table){
        $table .= "<input type='hidden' id='campoOrdenacao' name='campoOrdenacao'>";
        $table .= "<input type='hidden' id='req' name='req'>";
        $table .= "<input type='hidden' id='tipo_relatorio' name='tipo_relatorio' value='".$this->tipoRelatorio."'>";
        foreach($this->filtros as $key => $value){
            if (is_array($value)){
                foreach($value as $kvalue => $vvalue){
                    $table .= "<input type='hidden' name='filtro[".$key."][]' value='".$vvalue."'>";
                }
            }else{
                $table .= "<input type='hidden' name='filtro[".$key."]' value='".$value."'>";
            }
        }
        foreach($this->colunasSelecionadas as $key => $value){
            $table .= "<input type='hidden' name='coluna[]' value='".$value."'>";
        }
        if (count($this->colunasNaoSelecionadas)>0){
            foreach($this->colunasNaoSelecionadas as $key => $value){
                $table .= "<input type='hidden' name='naoColuna[]' value='".$value."'>";
            }
        }
        return $table;
    }
    
    Public Function montaCorpo($table){
        $agrupadorAnterior="";
        for($i=0;$i<count($this->dados);$i++){
            $colorBg = $colorBg=="white"?"#F5F5F5":"white";
            $table .= "<tr bgcolor=".$colorBg.">";
            foreach($this->colunasSelecionadas as $chaveColuna => $value){
                foreach($this->listaColunas as $chaveLista => $colunas){
                    if ($chaveLista===$value){
                        $table = $this->imprimeColunaPorTipo($colunas['type'], $this->dados[$i][$chaveLista], $table, true, $chaveLista);
                    }                        

                }
            }
            $table .= "</td></tr>";
                 
        }
        return $table;
    }
        
    Public Function montaRodape($table){
        $table .= "<tr>";
        $this->negrito=true;
        foreach($this->colunasSelecionadas as $value){
            if (array_key_exists($value, $this->totalizador)){
                $table = $this->imprimeColunaPorTipo($this->listaColunas[$value]['type'], $this->totalizador[$value], $table);
            }else{
                $table = $this->imprimeColunaPorTipo($this->listaColunas[$value]['type'], ' - ', $table);
            }
        }
        $table .= "</td></tr>";
        return $table;
    }    
    
    Public Function adicionaScript($table){
        $table .= "<script>";
        $table .= "  function ordena(campo){";
        $table .= "     document.getElementById('campoOrdenacao').value=campo;";
        $table .= "     document.getElementById('req').value='exibir';";
        $table .= "     document.formRelatorio.submit();";
        $table .= "  }";
        $table .= "</script>";
        return $table;
    }
    
    Public Function imprimeColunaPorTipo($type, $valor, $table, $totaliza=false, $campo=null){
        $negritoInicio = $this->negrito?"<b>":"";
        $negritoFinal = $this->negrito?"</b>":"";
        switch ($type) {
            case self::STRING:
                $table .= "<td align='left'>";
                $table .= "<div>".$valor."</div>";
                $table .= "</td>";
                break;
            case self::NUMERIC:
                if ($valor==''){
                    $valor=0;
                }
                $table .= "<td align='right'>";
                $table .= "<div>$negritoInicio".number_format($valor,0,'.',',')."$negritoFinal</div>";
                $table .= "</td>";
                if ($totaliza){
                    $this->setTotalizador($campo,$valor);
                }
                    
                break;
            case self::CURRENCY:
                $table .= "<td align='right'>";
                $table .= "<div>$negritoInicio R$ ".number_format($valor,0,'.',',')."$negritoFinal</div>";
                $table .= "</td>";
                if ($totaliza){
                    $this->setTotalizador($campo,$valor);
                }                
                break;

            default:
                break;
        }     
        return $table;
    }
    
    Public Function montaColunas(){
        $retorno = array();
        foreach($this->listaColunas as $chaveColuna => $colunas){
            foreach($this->colunasSelecionadas as $value){
                if ($chaveColuna===$value){
                    if ($colunas['alias']!=''){
                        $retorno[] = $colunas['alias'].".".$colunas['campo']."\r\n";
                    }else{
                        $retorno[] = $colunas['campo']."\r\n";
                    }
                }
            }
        }   
        return $retorno;
    }
    
    Public Function montaJoin(){
        $retorno = array();
        foreach($this->listaJoins as $joins){
            foreach($this->colunasSelecionadas as $value){
                if ($joins['campo']===$value){
                    foreach($joins['join'] as $joinValue){
                        if ($this->verificaJoin($retorno, $joinValue)){
                            $retorno[] = $joinValue."\r\n";
                        }
                    }
                }
            }
            foreach($this->filtros as $key => $value){
                if ($key==$joins['campo']){
                    foreach($joins['join'] as $joinValue){
                        if ($this->verificaJoin($retorno, $joinValue)){
                            $retorno[] = $joinValue."\r\n";
                        }
                    }
                }
            }
        }
        return $retorno;        
    }
    
    Public Function montaWhere(){
        $retorno = array();
        foreach($this->listaJoins as $wheres){
            foreach($this->colunasSelecionadas as $value){
                if ($wheres['campo']===$value){
                    if (array_key_exists('where', $wheres)){
                        foreach($wheres['where'] as $joinValue){
                            if ($this->verificaJoin($retorno, $joinValue)){
                                $retorno[] = $joinValue."\r\n";
                            }
                        }
                    }
                }
            }           
        }
        foreach($_REQUEST['filtro'] as $key => $value){
            if ($value){
                if ($this->listaColunas[$key]['alias']!=''){
                    $alias = $this->listaColunas[$key]['alias'].".";
                }else{
                    $alias = "";
                }
                if (is_array($value)){
                    $retorno[] = $alias.$key." in ('".implode("', '", $value)."')\r\n";
                }else{
                    $retorno[] = $alias.$key." = '".$value."'\r\n";
                }
            }
        }
        if (count($retorno)==0){
            $retorno = "";
        }
        return $retorno;          
    }
    
    Public Function verificaJoin($join, $novoJoin){
        foreach($join as $value){
            
            if (trim($value)==trim($novoJoin)){
                return false;
            }
        }
        return true;
    }
}
