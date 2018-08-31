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
     * Grava historico de alteração da opção Delegação(SubUnidade Delegada)
     * 
     * @param array $post
     * @param Emendas_Model_Beneficiario $beneficiario
     */
    public function gravarHistoricoDelegacao(array $post, Emendas_Model_Beneficiario $beneficiario){
        $listaSubUnidadesDelegadasBeneficiario = $beneficiario->recuperarDelegacao();

        $listaSubUnidadesDelegadasPost = $post['delegacao']? $post['delegacao']: array();
        
        if(
            array_diff($listaSubUnidadesDelegadasPost, $listaSubUnidadesDelegadasBeneficiario)
            ||
            array_diff($listaSubUnidadesDelegadasBeneficiario, $listaSubUnidadesDelegadasPost)
        ){
            $listaSubunidadesAnteriores = self::formatarListaSubunidade($listaSubUnidadesDelegadasBeneficiario);
            $listaSubunidadesNovas = self::formatarListaSubunidade($listaSubUnidadesDelegadasPost);
            $this->gravarHistoricoCampo(
                'Delegação(Subunidade Delegada)',
                join($listaSubunidadesAnteriores, '<br />'),
                join($listaSubunidadesNovas, '<br />')
            );
        }
    }
    
    /**
     * Formata a lista de códigos de Subunidade pra retornar uma lista com a 
     * descrição contendo Número SIOP e nome da Subunidade pra ser exibido ao usuário.
     * 
     * @param array $listaCodigoSubunidade
     * @return array
     */
    public static function formatarListaSubunidade($listaCodigoSubunidade = array()){
        $listaSubunidade = array();
        foreach($listaCodigoSubunidade as $codigo){
            $subunidade = new Public_Model_SubUnidadeOrcamentaria($codigo);
            if($subunidade->suoid){
                $listaSubunidade[] = $subunidade->suocod. '-'. $subunidade->suonome;
            }
        }
        
        return $listaSubunidade;
    }
    
}
