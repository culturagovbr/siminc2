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
    private $agrupadores;
    private $agrupadoresSelecionados;
    private $dados;
    private $dadosAgrupados;
    private $listaColunas;
    private $listaAgrupadores;
    private $listaJoins;
    private $tabelaBase;
    
    
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
        $this->colunasSelecionadas = $colunas = $colunasSelecionadas;
        foreach($colunasNaoSelecionadas as $value){
            $colunas[] = $value;
        }
        $this->colunas = $colunas;
    }
    
    Public Function setAgrupadores($agpSelecionados, $agpNaoSelecionados){
        $this->agrupadoresSelecionados = $agp = $agpSelecionados;
        if (!empty($agpNaoSelecionados)){
            foreach($agpNaoSelecionados as $value){
                $agp[] = $value;
            }        
        }
        $this->agrupadores = $agp;
    }
    
    Public Function setDados($dados){
        $this->dados = $dados;
    }
    
    Public Function setListaColunas($lista){
        $this->listaColunas = $lista;
    }
    
    Public Function setListaAgrupadores($lista){
        $this->listaAgrupadores = $lista;
    }
    
    Public Function setListaJoins($lista){
        $this->listaJoins = $lista;
    }
    
    Public Function setTabelaBase($tabela){
        $this->tabelaBase = $tabela;
    }
    
    Public Function MontaSelect(){
        $colunas = $this->montaColunas();
        $joins = $this->montaJoin();
        $where = $this->montaWhere();
        $sql = "select ".implode(', ', $colunas)." from ".$this->tabelaBase." ".implode(' ', $joins)." where 1=1 ".implode(' ', $where);
        $this->dados = $this->db->carregar($sql);
        $this->Agrupar();
    }
    
    Public Function getRelatorio(){
        $table = "<table width=\"100%\" cellspacing=\"1\" cellpadding=\"5\" border=\"0\" align=\"center\" class=\"tabela\">";
        $table = $this->montaCabecalho($table);
        $table = $this->montaCorpo($table);
        $table .= "</table>";
        echo $table;
    }
    
    Public Function montaCabecalho($table){
        $i=0;
        $table .= "<tr>";
        $table .= "<td>";
        foreach($this->listaAgrupadores as $colunas){
            foreach($this->agrupadoresSelecionados as $value){
                if ($colunas['campo']===$value){
                    if ($i>0){
                        $table .= "<div style=\"padding-left:" . (20 * ($i - 1)) . "px;\"><img src=\"../imagens/seta_filho.gif\" align=\"absmiddle\"/>".$colunas['label']."</div>";
                    }else{
                        $table .= "<div>".$colunas['label']."</div>";
                    }
                    $i++;
                }
            }
        }     
        $table .= "</td>";
        foreach($this->listaColunas as $colunas){
            foreach($this->colunasSelecionadas as $value){
                if ($colunas['campo']===$value){
                    $table .= "<td>";
                    $table .= "<div>".$colunas['label']."</div>";
                    $table .= "</td>";
                }
            }
        }
        $table .= "</td></tr>";
        return $table;
    }
    
    Public Function Agrupar(){
        $vlrAnterior = "";
        $retorno="";
        $valorNivel="";
        $i=-1;
        foreach($this->agrupadoresSelecionados as $chaveagp => $vlragp){
            if($i==-1){
                $vlrAnterior=$vlragp;
                $i++;
                $retorno[$i]['campo']=$vlragp;
            }
            foreach($this->dados as $dado){
                foreach($dado as $key=>$value){
                    if (!in_array($key, $this->agrupadoresSelecionados)){
                        switch ($this->listaColunas[$key]['type']) {
                            case 'numeric':
                                $retorno[$i][$dado[$vlragp]][$key]+=$value;
                                break;
                            default:
                                break;
                        }
                    }
                    if ($vlragp!=$vlrAnterior){
                        $i++;                    
                        $retorno[$i]['campo']=$vlragp;
                    }
                    $vlrAnterior=$vlragp;
                }
            }
        }   
        $qtdAgrupadores = array();
        $keyAnterior="";
        foreach($this->agrupadoresSelecionados as $chaveagp => $vlragp){
            $array = array($vlragp=>'');
            if ($keyAnterior!="")
                array_push($qtdAgrupadores,$array);
            else{
                array_push($qtdAgrupadores,$array);
            }
            $keyAnterior = $vlragp;
        }
        ver($qtdAgrupadores,d);
//                                ver($dado, $valueAgp, $key, $value,$this->agrupadoresSelecionados,d);
//        foreach($this->dados as $dado){
//                ver($dado,d);
//            foreach($dado as $key=>$value){
//                foreach($this->agrupadoresSelecionados as $keyAgp=>$valueAgp){
//                    if (!in_array($key, $this->agrupadoresSelecionados)){
//                        switch ($this->listaColunas[$key]['type']) {
//                            case 'numeric':
//                                $retorno[$dado[$valueAgp]][$key]+=$value;
//                                break;
//                            default:
//                                break;
//                        }
//                    }
//                }
//            }
//        }
        ver($retorno,d);
        $this->dadosAgrupados = $retorno;
    }
    
    Public Function montaCorpo($table){
        $agrupadorAnterior="";
        foreach($this->dados as $chave => $dado){
            $table .= "<tr>";
            $table .= "<td>";
            foreach($this->agrupadoresSelecionados as $key=>$vlrAgrupadores){
                
                $dadosAgrupadosSemCampo = $this->dadosAgrupados[$key];
                unset($dadosAgrupadosSemCampo['campo']);
                $campo=$this->dadosAgrupados[$key]['campo'];
                ver($this->dadosAgrupados);
                if ($key>0){
                    $table .= "<div style=\"padding-left:" . (20 * ($i - 1)) . "px;\"><img src=\"../imagens/seta_filho.gif\" align=\"absmiddle\"/>".$dado[$campo]."</div>";
                }else{
                    $table .= "<div>".$dado[$campo]."</div>";
                }
            }
                $table .= "</td>";
//                foreach($dadosAgrupadosSemCampo as $chaveAgrupador=>$valorAgrupador){
//                    foreach($valorAgrupador as $chave => $valorCampo){
//                        if($dado[$campo]==$chaveAgrupador){
//                            foreach($valorAgrupador as $value){
//                                $table .= "<td>";
//                                $table .= "<div>".$value."</div>";
//                                $table .= "</td>";
//                            }
//                        }
//                    }
//                }
//            $table .= "</tr>";
//            $table .= "<tr>";
//            foreach($this->colunasSelecionadas as $value){
//                $table .= "<td>";
//                $table .= "<div>".$dado[$campo]."</div>";
//                $table .= "</td>";
//            }
            $table .= "</tr>";
                 
        }
        return $table;
    }
    
    Public Function montaColunas(){
        $retorno = array();
        foreach($this->listaColunas as $colunas){
            foreach($this->colunasSelecionadas as $value){
                if ($colunas['campo']===$value){
                    $retorno[] = $colunas['alias'].".".$colunas['campo'];
                }
            }
        }
        foreach($this->listaAgrupadores as $colunas){
            foreach($this->agrupadoresSelecionados as $value){
                if ($colunas['campo']===$value){
                    $retorno[] = $colunas['alias'].".".$colunas['campo'];
                }
            }
        }        
        return $retorno;
    }
    
    Public Function montaJoin(){
        $retorno = array();
        foreach($this->listaJoins as $joins){
            foreach($this->agrupadoresSelecionados as $value){
                if ($joins['campo']===$value){
                    foreach($joins['join'] as $joinValue){
                        if ($this->verificaJoin($retorno, $joinValue)){
                            $retorno[] = $joinValue;
                        }
                    }
                }
            }
            foreach($this->colunasSelecionadas as $value){
                if ($joins['campo']===$value){
                    foreach($joins['join'] as $joinValue){
                        if ($this->verificaJoin($retorno, $joinValue)){
                            $retorno[] = $joinValue;
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
            foreach($this->agrupadoresSelecionados as $value){
                if ($wheres['campo']===$value){
                    if (array_key_exists('where', $wheres)){
                        foreach($wheres['where'] as $joinValue){
                            if ($this->verificaJoin($retorno, $joinValue)){
                                $retorno[] = $joinValue;
                            }
                        }
                    }
                }
            }
            foreach($this->colunasSelecionadas as $value){
                if ($wheres['campo']===$value){
                    if (array_key_exists('where', $wheres)){
                        foreach($wheres['where'] as $joinValue){
                            if ($this->verificaJoin($retorno, $joinValue)){
                                $retorno[] = $joinValue;
                            }
                        }
                    }
                }
            }           
        }
        return $retorno;          
    }
    
    Public Function verificaJoin($join, $novoJoin){
        foreach($join as $key=> $value){
            if ($value==$novoJoin){
                return false;
            }
        }
        return true;
    }
}
