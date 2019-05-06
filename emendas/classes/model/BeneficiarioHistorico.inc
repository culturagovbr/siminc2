<?php
/**
 * Classe de mapeamento da entidade emendas.beneficiario_historico
 *
 * @category Class
 * @package  A1
 * @author   DOUGLAS SANTANA FONTES <douglas.fontes@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 2018-08-23
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Emendas_Model_BeneficiarioObservacao
 *
 * @category Class
 * @package  A1
 * @author   DOUGLAS SANTANA FONTES <douglas.fontes@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 2018-08-23
 * @link     no link
 */
class Emendas_Model_BeneficiarioHistorico extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'emendas.beneficiario_historico';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'behid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'usucpf' => array('tabela' => 'seguranca.usuario', 'pk' => 'usucpf'),
        'benid' => array('tabela' => 'emendas.beneficiario', 'pk' => 'benid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'behid' => null,
        'benid' => null,
        'usucpf' => null,
        'behdata' => null,
    );

    /**
     * Retorna lista com usuários que fizeram alterações no cadastro de beneficiário.
     *
     * @return mixed
     */
    public function recuperarHistorico($benid)
    {
        global $db;

        $sql = "SELECT
                    beh.behid,
                    to_char(beh.behdata, 'DD/MM/YYYY HH24:MI:SS') as behdata,
                    seg.usucpf,
                    seg.usunome,
                    seg.usuemail,
                    '(' || seg.usufoneddd || ') ' || seg.usufonenum as telefone
                FROM emendas.beneficiario_historico beh
                JOIN seguranca.usuario seg ON ( seg.usucpf = beh.usucpf )
                WHERE
                    beh.benid = '".$benid ."'
                ORDER BY
                    beh.behdata desc
        ";

        return $db->carregar($sql);
    }
    
    /**
     * Grava histórico de alteração de um campo especifico do formulário.
     * 
     * @param string $campo
     * @param string $valorAnterior
     * @param string $valorNovo
     */
    public function gravarHistoricoCampo($campo, $valorAnterior, $valorNovo){
        $beneficiarioHistoricoAlteracao = new Emendas_Model_BeneficiarioHistoricoAlteracao();
        $beneficiarioHistoricoAlteracao->behid = $this->behid;
        $beneficiarioHistoricoAlteracao->bahcampo = $campo;
        $beneficiarioHistoricoAlteracao->bahanterior = $valorAnterior? $valorAnterior: 'Nenhuma';
        $beneficiarioHistoricoAlteracao->bahnovo = $valorNovo? $valorNovo: 'Nenhuma';
        $beneficiarioHistoricoAlteracao->salvar();
    }
    
    /**
     * Grava historico de alteração da opção SubUnidade
     * 
     * @param array $post
     * @param Emendas_Model_Beneficiario $beneficiario
     */
    public function gravarHistoricoSubUnidade(array $post, Emendas_Model_Beneficiario $beneficiario){
        if($post['suoid'] != $beneficiario->suoid){
            $subunidadeAnterior = new Public_Model_SubUnidadeOrcamentaria($beneficiario->suoid);
            $subunidadeNova = new Public_Model_SubUnidadeOrcamentaria($post['suoid']);
            
            $this->gravarHistoricoCampo(
                'SubUnidade',
                $subunidadeAnterior->suocod. '-'. $subunidadeAnterior->suonome,
                $subunidadeNova->suocod. '-'. $subunidadeNova->suonome
            );
        }
    }
    
    /**
     * Grava historico de alteração da opção Proponente
     * 
     * @param array $post
     * @param Emendas_Model_Beneficiario $beneficiario
     */
    public function gravarHistoricoProponente(array $post, Emendas_Model_Beneficiario $beneficiario){
        if($post['proid'] != $beneficiario->proid){
            $proponenteAnterior = new Emendas_Model_Proponente($beneficiario->proid);
            $proponenteNova = new Emendas_Model_Proponente($post['proid']);
            
            $this->gravarHistoricoCampo(
                'Proponente',
                formatar_cnpj($proponenteAnterior->procnpj). '-'. $proponenteAnterior->pronome,
                formatar_cnpj($proponenteNova->procnpj). '-'. $proponenteNova->pronome
            );
        }
    }
    
    /**
     * Grava historico de alteração da opção Localização
     * 
     * @param array $post
     * @param Emendas_Model_Beneficiario $beneficiario
     */
    public function gravarHistoricoLocalizacao(array $post, Emendas_Model_Beneficiario $beneficiario){
        if($post['esfid'] != $beneficiario->esfid){
            $localizacaoAnterior = new Territorios_Model_Esfera($beneficiario->esfid);
            $localizacaoNova = new Territorios_Model_Esfera($post['esfid']);

            $this->gravarHistoricoCampo(
                'Localização',
                $localizacaoAnterior->esfdsc,
                $localizacaoNova->esfdsc
            );
        }
    }
    
    /**
     * Grava historico de alteração da opção País
     * 
     * @param array $post
     * @param Emendas_Model_Beneficiario $beneficiario
     */
    public function gravarHistoricoPais(array $post, Emendas_Model_Beneficiario $beneficiario){
        $listaLocalizacaoBeneficiario = (new Emendas_Model_BeneficiarioLocalizacao())->recuperarPorBeneficiario($beneficiario->benid);
        $listaPaisBeneficiario = $listaLocalizacaoBeneficiario['paiid'];
        $listaPaisPost = retirarValorVazioArray($post['paiid']);
        if(
            array_diff($listaPaisPost, $listaPaisBeneficiario)
            ||
            array_diff($listaPaisBeneficiario, $listaPaisPost)
        ){
            $listaAnteriores = self::formatarListaPais($listaPaisBeneficiario);
            $listaNovas = self::formatarListaPais($listaPaisPost);
            $this->gravarHistoricoCampo(
                'País',
                join($listaAnteriores, '<br />'),
                join($listaNovas, '<br />')
            );
        }
    }
    
    /**
     * Grava historico de alteração da opção UF
     * 
     * @param array $post
     * @param Emendas_Model_Beneficiario $beneficiario
     */
    public function gravarHistoricoUf(array $post, Emendas_Model_Beneficiario $beneficiario){
        $listaLocalizacaoBeneficiario = (new Emendas_Model_BeneficiarioLocalizacao())->recuperarPorBeneficiario($beneficiario->benid);
        $listaUfBeneficiario = $listaLocalizacaoBeneficiario['estuf'];
        $listaUfPost = retirarValorVazioArray($post['estuf']);
        if(
            array_diff($listaUfPost, $listaUfBeneficiario)
            ||
            array_diff($listaUfBeneficiario, $listaUfPost)
        ){
            $listaAnteriores = self::formatarListaUf($listaUfBeneficiario);
            $listaNovas = self::formatarListaUf($listaUfPost);
            $this->gravarHistoricoCampo(
                'UF',
                join($listaAnteriores, '<br />'),
                join($listaNovas, '<br />')
            );
        }
    }
    
    /**
     * Grava historico de alteração da opção Município
     * 
     * @param array $post
     * @param Emendas_Model_Beneficiario $beneficiario
     */
    public function gravarHistoricoMunicipio(array $post, Emendas_Model_Beneficiario $beneficiario){
        $listaLocalizacaoBeneficiario = (new Emendas_Model_BeneficiarioLocalizacao())->recuperarPorBeneficiario($beneficiario->benid);
        $listaMunicipioBeneficiario = $listaLocalizacaoBeneficiario['muncod'];
        $listaMunicipioPost = retirarValorVazioArray($post['muncod']);
        if(
            array_diff($listaMunicipioPost, $listaMunicipioBeneficiario)
            ||
            array_diff($listaMunicipioBeneficiario, $listaMunicipioPost)
        ){
            $listaAnteriores = self::formatarListaMunicipio($listaMunicipioBeneficiario);
            $listaNovas = self::formatarListaMunicipio($listaMunicipioPost);
            $this->gravarHistoricoCampo(
                'Município',
                join($listaAnteriores, '<br />'),
                join($listaNovas, '<br />')
            );
        }
    }
    
    /**
     * Grava historico de alteração da opção Delegação(SubUnidade Delegada)
     * 
     * @param array $post
     * @param Emendas_Model_Beneficiario $beneficiario
     */
    public function gravarHistoricoDelegacao(array $post, Emendas_Model_Beneficiario $beneficiario){
        $listaSubUnidadesDelegadasBeneficiario = $beneficiario->recuperarDelegacao();
        $listaSubUnidadesDelegadasPost = retirarValorVazioArray($post['delegacao']);
        if(
            array_diff($listaSubUnidadesDelegadasPost, $listaSubUnidadesDelegadasBeneficiario)
            ||
            array_diff($listaSubUnidadesDelegadasBeneficiario, $listaSubUnidadesDelegadasPost)
        ){
            $listaAnteriores = self::formatarListaSubunidade($listaSubUnidadesDelegadasBeneficiario);
            $listaNovas = self::formatarListaSubunidade($listaSubUnidadesDelegadasPost);
            $this->gravarHistoricoCampo(
                'Delegação(Subunidade Delegada)',
                join($listaAnteriores, '<br />'),
                join($listaNovas, '<br />')
            );
        }
    }
    
    /**
     * Formata a lista de códigos de Subunidade pra retornar uma lista com a 
     * descrição contendo Número SIOP e nome da Subunidade pra ser exibido ao usuário.
     * 
     * @param array $listaCodigo
     * @return array
     */
    public static function formatarListaSubunidade($listaCodigo = array()){
        $listaDescricao = array();
        foreach($listaCodigo as $codigo){
            $subunidade = new Public_Model_SubUnidadeOrcamentaria($codigo);
            $listaDescricao[] = $subunidade->suocod. '-'. $subunidade->suonome;
        }
        
        return $listaDescricao;
    }
    
    /**
     * Formata a lista de códigos de País pra retornar uma lista com a 
     * descrição contendo nome do País pra ser exibido ao usuário.
     * 
     * @param array $listaCodigo
     * @return array
     */
    public static function formatarListaPais($listaCodigo = array()){
        $listaDescricao = array();
        foreach($listaCodigo as $codigo){
            $pais = new Territorios_Model_Pais($codigo);
            $listaDescricao[] = $pais->paidescricao;
        }
        
        return $listaDescricao;
    }
    
    /**
     * Formata a lista de códigos de UF pra retornar uma lista com a 
     * descrição contendo nome do UF pra ser exibido ao usuário.
     * 
     * @param array $listaCodigo
     * @return array
     */
    public static function formatarListaUf($listaCodigo = array()){
        $listaDescricao = array();
        foreach($listaCodigo as $codigo){
            $uf = new Territorios_Model_Estado($codigo);
            $listaDescricao[] = $uf->estuf. '-'. $uf->estdescricao;
        }
        
        return $listaDescricao;
    }
    
    /**
     * Formata a lista de códigos de Município pra retornar uma lista com a 
     * descrição contendo nome do Município pra ser exibido ao usuário.
     * 
     * @param array $listaCodigo
     * @return array
     */
    public static function formatarListaMunicipio($listaCodigo = array()){
        $listaDescricao = array();
        foreach($listaCodigo as $codigo){
            $municipio = new Territorios_Model_Municipio($codigo);
            $listaDescricao[] = $municipio->estuf. '-'. $municipio->mundescricao;
        }
        
        return $listaDescricao;
    }

    /**
     * Grava historico de alteração da opção Número do Processo
     * 
     * @param array $post
     * @param Emendas_Model_Beneficiario $beneficiario
     */
    public function gravarHistoricoProcesso(array $post, Emendas_Model_Beneficiario $beneficiario){
        if($post['bennumeroprocesso'] != $beneficiario->bennumeroprocesso){
            $this->gravarHistoricoCampo(
                'Número do Processo',
                $beneficiario->bennumeroprocesso,
                $post['bennumeroprocesso']
            );
        }
    }
    
    /**
     * Grava historico de alteração da opção Data de Início da Vigência
     * 
     * @param array $post
     * @param Emendas_Model_Beneficiario $beneficiario
     */
    public function gravarHistoricoVigencia(array $post, Emendas_Model_Beneficiario $beneficiario){
        if($post['beninicio'] != formata_data($beneficiario->beninicio)){
            $this->gravarHistoricoCampo(
                'Data de Início da Vigência',
                formata_data($beneficiario->beninicio),
                $post['beninicio']
            );
        }
    }
    
    /**
     * Grava historico de alteração da opção Modalidade de Pactuação
     * 
     * @param array $post
     * @param Emendas_Model_Beneficiario $beneficiario
     */
    public function gravarHistoricoModalidade(array $post, Emendas_Model_Beneficiario $beneficiario){
        if($post['capid'] != $beneficiario->capid){
            $modalidadeAnterior = new Planacomorc_Model_CategoriaApropriacao($beneficiario->capid);
            $modalidadeNova = new Planacomorc_Model_CategoriaApropriacao($post['capid']);
            
            $this->gravarHistoricoCampo(
                'Modalidade de Pactuação',
                $modalidadeAnterior->capdsc,
                $modalidadeNova->capdsc
            );
        }
    }
    
    /**
     * Grava historico de alteração da opção TED
     * 
     * @param array $post
     * @param Emendas_Model_Beneficiario $beneficiario
     */
    public function gravarHistoricoTed(array $post, Emendas_Model_Beneficiario $beneficiario){
        $tedAnterior = $beneficiario->bented == 't'?'Sim': 'Não';
        $tedNovo = $post['bented']?'Sim': 'Não';
        if($tedAnterior != $tedNovo){
            $this->gravarHistoricoCampo(
                'TED',
                $tedAnterior,
                $tedNovo
            );
        }
    }

}
