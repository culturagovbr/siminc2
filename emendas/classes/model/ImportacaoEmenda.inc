<?php

require_once APPRAIZ. 'includes/classes/Modelo.class.inc';

/**
 * Emendas_Model_ImportacaoEmenda
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release:
 * @link     no link
 */
class Emendas_Model_ImportacaoEmenda extends Emendas_Model_Emenda
{
    protected $linha = 0;

    protected $exercicio = NULL;

    protected $listaEmendasArquivo = array();

    public function getLinha() {
        return $this->linha;
    }

    public function getExercicio() {
        return $this->exercicio;
    }

    public function getListaEmendasArquivo() {
        return $this->listaEmendasArquivo;
    }

    public function setLinha($linha) {
        $this->linha = $linha;
        return $this;
    }

    public function setExercicio($exercicio) {
        $this->exercicio = $exercicio;
        return $this;
    }

    public function setListaEmendasArquivo($listaEmendasArquivo) {
        $this->listaEmendasArquivo = $listaEmendasArquivo;
        return $this;
    }
    
    /**
     * Atualiza/importa os dados de emendas e beneficiários da Planilha para o Sistema
     * 
     * @return array
     */
    public function importar()
    {
        foreach($this->listaEmendasArquivo as $linha => $emenda){
            $this->linha = $linha;
            $this->listaEmendasArquivo[$this->linha]['listaDiagnostico'] = NULL;

            $emenumero = (int)trim($emenda['emenda']);
            /**
             * Caso o registro da emenda não tenha número de emenda, o sistema 
             * pula a importação desse registro e continua a partir do próximo.
             */
            if(!$emenumero){
                $this->listaEmendasArquivo[$this->linha]['listaDiagnostico'][] = 'Nenhuma informação dessa linha foi importada porque não existe número de emenda.';
                continue;
            }
            
            $sql = "SELECT emeid FROM emendas.emenda WHERE emenumero = ". (int)$emenumero;
            $emeid = $this->pegaUm($sql);

            if($emeid) {
                $this->carregarPorId($emeid);
            } else {
                # Unidade Orçamentária
                $unocod = substr(trim($emenda['unidadeOrcamentaria']), 0, 5);
                $unoid = $this->_buscarUOporUnocodExercicio($unocod);

                # Autor
                $autid = $this->_importarAutor($emenda['autor'], $emenda['partido']);

                # Funcional
                $ptrid = $this->_buscarFuncionalImportacao(
                    $unocod,
                    $emenda['funcao'],
                    $emenda['subFuncao'],
                    $emenda['programa'],
                    $emenda['acao'],
                    $emenda['localizador']
                );

                /**
                 * Caso o registro da emenda não tenha funcional ou autor, o sistema 
                 * pula a importação desse registro e continua a partir do próximo.
                 */
                if(!$ptrid || !$autid){
                    if(!$ptrid) $this->listaEmendasArquivo[$this->linha]['listaDiagnostico'][] = 'Nenhuma informação dessa linha foi importada porque não existe Funcional relacionada no sistema.';
                    if(!$autid) $this->listaEmendasArquivo[$this->linha]['listaDiagnostico'][] = 'Nenhuma informação dessa linha foi importada porque não existe número de Autor válido para cadastrar no sistema.';
                    continue;
                }

                $this->emenumero = $emenumero;
                $this->unoid = $unoid;
                $this->ptrid = $ptrid;
                $this->autid = $autid;
                $this->emeimpositiva = 't';
                $this->prsano = $this->exercicio;
                $this->salvar();
            }

            if(is_numeric($emenda['beneficiario'])){
                $this->_importarProponenteBeneficiario($emenda);
            } else {
                $this->_importarFonteEmendaDetalhes($emenda);
            }
            $this->emeid = null;
        }
    }

    private function _buscarUOporUnocodExercicio($unocod)
    {
        $sql = "
            SELECT DISTINCT
                unoid,
                unonome
            FROM public.vw_subunidadeorcamentaria
            WHERE
		unostatus = 'A'
                AND prsano = '". (int)$this->exercicio. "'
                AND unocod = '". (int)$unocod. "'
            ORDER BY
		unonome
        ";

        return $this->pegaUm($sql);
    }

    private function _importarAutor($autor, $partido)
    {
        $listaAutor = explode('-', $autor);
        $autcod = (int)trim($listaAutor[0]);
        $autnome = trim($listaAutor[1]);
        
        /**
         * Caso não tenha número de autor válido o sistema não realiza 
         * modificação ou inserção de dados.
         */
        if(!$autcod > 0){
            $this->listaEmendasArquivo[$this->linha]['listaDiagnostico'][] = 'Não foi importado nenhuma informação de Autor ou Partido porque não existe número do parlamentar.';
            return NULL;
        }
        
        # Buscando código interno de Autor
        $sql = "SELECT autid FROM emendas.autor WHERE autstatus = 'A' AND autcod = '". (int)$autcod. "'";
        $autid = $this->pegaUm($sql);

        # Buscando código interno de Partido
        $sql = "SELECT parid FROM emendas.partido WHERE parstatus = 'A' AND parsigla = '". $partido. "'";
        $parid = $this->pegaUm($sql);

        if(!$parid){
            # Inserindo Partido novo
            $mPartido = new Emendas_Model_Partido();
            $mPartido->pardsc = $partido;
            $mPartido->parsigla = $partido;
            $mPartido->salvar();
            $parid = $mPartido->parid;
        }

        # Atualizando/inserindo nome e partido do autor
        $mAutor = new Emendas_Model_Autor($autid);
        $mAutor->autcod = $autcod;
        $mAutor->autnome = $autnome;
        $mAutor->parid = $parid;
        $mAutor->salvar();
        $mAutor->autid;

        return $mAutor->autid;
    }

    private function _buscarFuncionalImportacao($unocod, $funcao, $subfuncao, $programa, $acao, $localizador)
    {
        $acacod = substr(trim($acao), 0, 4);
        $prgcod = substr(trim($programa), 0, 4);
        $loccod = substr(trim($localizador), 0, 4);
        $sql = "
            SELECT DISTINCT
                ptrid,
                funcional
            FROM monitora.vw_ptres
            WHERE
                ptrstatus = 'A'
                AND ptrano = '$this->exercicio'
                AND unicod = '". (int)$unocod. "'
                AND funcod = '". (int)$funcao. "'
                AND sfucod = '". (int)$subfuncao. "'
                AND prgcod = '". (int)$prgcod. "'
                AND acacod = '$acacod'
                AND loccod = '$loccod'
                AND plocod = 'EIND'
        ";

        $listaFuncional = $this->carregar($sql);
        $funcional = current($listaFuncional);
        if(count($listaFuncional) > 1){
            throw new Exception('Houve um erro porque o sistema achou mais de uma funcional('. $funcional['funcional']. ') pra uma emenda existente na planilha.');
        }

        return $funcional? $funcional['ptrid']: NULL;
    }

    private function _importarFonteEmendaDetalhes($emenda)
    {
        $mFonteRecurso = new Public_Model_FonteRecurso();
        $mFonteRecurso->foncod = (int)$emenda['fonte'];
        $fonteRecurso = $mFonteRecurso->carregarPorFoncod();

        $mEmendaDetalhe = new Emendas_Model_EmendaDetalhe();
        $mEmendaDetalhe->emeid = $this->emeid;
        $mEmendaDetalhe->fonid = $fonteRecurso['fonid'];
        $mEmendaDetalhe->gndcod = $emenda['gnd'];
        $mEmendaDetalhe->mapcod = $emenda['modalidade'];
        $mEmendaDetalhe->emdvalor = str_replace(['.', ','], ['', '.'], $emenda['autorizado']) + str_replace(['.', ','], ['', '.'], $emenda['valorPriorizado']);

        return $mEmendaDetalhe->salvar();
    }

    private function _importarProponenteBeneficiario($emenda)
    {
        $proponente = explode('-', $emenda['beneficiarioNome']);

        if(count($proponente) >= 2){
            $procnpj = trim(array_shift($proponente));
            $sql = "SELECT proid FROM emendas.proponente WHERE prostatus = 'A' AND procnpj = '{$procnpj}'";
        } else {
            $this->listaEmendasArquivo[$this->linha]['listaDiagnostico'][] = 'O CNPJ não é válido e portanto o sistema relacionou as informações dessa linha ao CNPJ 00.000.000/0000-00.';
            $sql = "SELECT proid FROM emendas.proponente WHERE prostatus = 'A' AND procnpj = '00000000000000'";
        }

        $proid = $this->pegaUm($sql);

        if(!$proid){
            $mProponente = new Emendas_Model_Proponente();
            $mProponente->procnpj = $procnpj;
            $mProponente->pronome = trim(implode(' - ', $proponente));
            $mProponente->salvar();
            $proid = $mProponente->proid;
        }

        $sql = "SELECT benid FROM emendas.beneficiario WHERE benstatus = 'A' AND emeid = ". (int)$this->emeid. " AND proid = ". (int)$proid;
        $benid = $this->pegaUm($sql);

        $mBeneficiario = new Emendas_Model_Beneficiario($benid);
        if(!$benid){
            $mBeneficiario->emeid = $this->emeid;
            $mBeneficiario->proid = $proid;
            $mBeneficiario->salvar();
        }

        $this->_importarBeneficiarioDetalhe($mBeneficiario->benid, $mBeneficiario->emeid, $emenda);
    }

    private function _importarBeneficiarioDetalhe($benid, $emeid, $emenda)
    {
        $sql = "SELECT emdid FROM emendas.emendadetalhe WHERE gndcod = '". (int)$emenda['gnd']. "' AND emeid = ". (int)$emeid;
        $emdid = $this->pegaUm($sql);

        $mBeneficiarioDetalhe = new Emendas_Model_BeneficiarioDetalhe();
        $mBeneficiarioDetalhe->benid = $benid;
        $mBeneficiarioDetalhe->emdid = $emdid;
        $mBeneficiarioDetalhe->bedvalor = str_replace(['.', ','], ['', '.'], $emenda['valorIndicado']);
        $mBeneficiarioDetalhe->bedvalorpriorizado = str_replace(['.', ','], ['', '.'], $emenda['valorPriorizado']);

        return $mBeneficiarioDetalhe->salvar();
    }

}
