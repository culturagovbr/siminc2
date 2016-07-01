<?php

include APPRAIZ . 'includes/classes/EmailAgendado.class.inc';
require_once APPRAIZ . 'includes/library/simec/Listagem.php';
include_once APPRAIZ . "includes/funcoesspo.php";

class Model_Usuario extends Abstract_Model
{

	public $entity = array();
	protected $_schema = 'seguranca';
	protected $_name = 'usuario';

	public function __construct($commit = true)
	{
		parent::__construct($commit);

		$this->perfilUsuario = new Model_PerfilUsuario();

		$this->entity['usucpf'] = array('value' => '', 'type' => 'character', 'is_null' => 'NO', 'maximum' => '11', 'contraint' => 'pk', 'label' => 'CPF');
		$this->entity['regcod'] = array('value' => '', 'type' => 'character', 'is_null' => 'NO', 'maximum' => '2', 'contraint' => '', 'label' => 'UF');
		$this->entity['usunome'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'NO', 'maximum' => '100', 'contraint' => '', 'label' => 'Nome');
		$this->entity['usuemail'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'NO', 'maximum' => '50', 'contraint' => '', 'label' => 'E-mail');
		$this->entity['usustatus'] = array('value' => 'A', 'type' => 'character', 'is_null' => 'YES', 'maximum' => '1', 'contraint' => '', 'label' => 'Status');
		$this->entity['usufoneddd'] = array('value' => '', 'type' => 'character', 'is_null' => 'NO', 'maximum' => '2', 'contraint' => '', 'label' => 'DDD');
		$this->entity['usufonenum'] = array('value' => '', 'type' => 'character', 'is_null' => 'NO', 'maximum' => '10', 'contraint' => '', 'label' => 'Telefone');
		$this->entity['ususenha'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'NO', 'maximum' => '100', 'contraint' => '', 'label' => 'Senha');
		$this->entity['usudataultacesso'] = array('value' => '', 'type' => 'date', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '', 'label' => 'Data Ultimo Acesso');
		$this->entity['usunivel'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '', 'label' => 'Nivel');
		$this->entity['usufuncao'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '100', 'contraint' => '', 'label' => 'Fun��o');
		$this->entity['ususexo'] = array('value' => '', 'type' => 'character', 'is_null' => 'YES', 'maximum' => '1', 'contraint' => '', 'label' => 'Sexo');
		$this->entity['orgcod'] = array('value' => '', 'type' => 'character', 'is_null' => 'YES', 'maximum' => '5', 'contraint' => '', 'label' => 'Cod Org�o');
		$this->entity['unicod'] = array('value' => '', 'type' => 'character', 'is_null' => 'YES', 'maximum' => '5', 'contraint' => '', 'label' => 'Unidade Or�ament�ria');
		$this->entity['usuchaveativacao'] = array('value' => '', 'type' => 'boolean', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '', 'label' => 'dddd');
		$this->entity['usutentativas'] = array('value' => '', 'type' => 'smallint', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '', 'label' => 'dddd');
		$this->entity['usuprgproposto'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '1000', 'contraint' => '', 'label' => 'dddd');
		$this->entity['usuacaproposto'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '1000', 'contraint' => '', 'label' => 'dddd');
		$this->entity['usuobs'] = array('value' => '', 'type' => 'text', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '', 'label' => 'Observa��o');
		$this->entity['ungcod'] = array('value' => '', 'type' => 'character', 'is_null' => 'YES', 'maximum' => '6', 'contraint' => '', 'label' => 'dddd');
		$this->entity['usudatainc'] = array('value' => '', 'type' => 'timestamp without time zone', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '', 'label' => 'dddd');
		$this->entity['usuconectado'] = array('value' => '', 'type' => 'boolean', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '', 'label' => 'dddd');
		$this->entity['pflcod'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '', 'label' => 'dddd');
		$this->entity['suscod'] = array('value' => 'A', 'type' => 'character', 'is_null' => 'YES', 'maximum' => '1', 'contraint' => '', 'label' => 'dddd');
		$this->entity['usunomeguerra'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '20', 'contraint' => '', 'label' => 'dddd');
		$this->entity['orgao'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '100', 'contraint' => '', 'label' => '�rgao');
		$this->entity['muncod'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'NO', 'maximum' => '7', 'contraint' => '', 'label' => 'Munic�pio');
		$this->entity['usudatanascimento'] = array('value' => '', 'type' => 'date', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '', 'label' => 'Data de Nascimento');
		$this->entity['usudataatualizacao'] = array('value' => '', 'type' => 'timestamp without time zone', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '', 'label' => 'dddd');
		$this->entity['entid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'fk', 'label' => '�rg�o');
		$this->entity['tpocod'] = array('value' => '', 'type' => 'character', 'is_null' => 'NO', 'maximum' => '1', 'contraint' => '', 'label' => 'Tipo �rg�o');
		$this->entity['carid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => 'fk', 'label' => 'dddd');
	}

	public function isValid()
	{
		parent::isValid();

		foreach ($this->entity as $nameColumn => $column) {

			if ($nameColumn === 'usucpf' && empty($column['value'])) {
				$validate = new Zend_Validate_NotEmpty();
				$validate->isValid($column['value']);
				$this->error[] = array("name" => $nameColumn, "msg" => (reset($validate->getMessages())));
			}

			if ($nameColumn === 'usucpf' && !empty($column['value'])) {
				if (!validaCPF($column['value'])) {
					$this->error[] = array("name" => $nameColumn, "msg" => ('CPF Inv�lido'));
				}
			}

			if ($nameColumn === 'usuemail' && !empty($column['value'])) {
				if (!$this->validarEmail($column['value'])) {
					$this->error[] = array("name" => $nameColumn, "msg" => ('E-mail Inv�lido'));
				}
			}
		}


		if ($this->error) return false; else
			return true;
	}

	public function treatEntity()
	{
		parent::treatEntity();
		foreach ($this->entity as $columnName => &$column) {
			if ($columnName === 'usucpf') {
				$column['value'] = $this->removeMaskCpf($column['value']);
			}
		}
	}

	public function removeMaskCpf($cpf)
	{
		return str_replace('.', '', str_replace('-', '', $cpf));
	}

	public function getDadosUsuarioFatorAvaliado($dados)
	{
		$retorno = array();

		if (!empty($dados)) {
			$retorno = array('usucpf' => $this->mask($this->getAttributeValue('usucpf'), '###.###.###-##'), 'usunome' => ($this->getAttributeValue('usunome')), 'usuemail' => $this->getAttributeValue('usuemail'), 'usufoneddd' => $this->getAttributeValue('usufoneddd'), 'usufonenum' => $this->getAttributeValue('usufonenum'), 'ususexo' => $this->getAttributeValue('ususexo'), 'usuobs' => ($this->getAttributeValue('usuobs')), 'usudatanascimento' => $this->getAttributeValue('usudatanascimento'), 'regcod' => $dados['regcod'], 'muncod' => $dados['muncod'], 'tpocod' => $dados['tpocod'], 'entid' => $dados['entid'],);
		}
		return $retorno;
	}

	public function getUsuarioByCpf($cpf)
	{
		$cpf = str_replace('.', '', $cpf = str_replace('-', '', $cpf));
		return $this->getAllByValues(array('usucpf' => $cpf));
	}

	public function getDadosUsuarioFatorAvaliadoReceitaFederal($cpf)
	{
		$usu_receita = recuperarUsuarioReceita($cpf);
		$retorno = array();
		if ($usu_receita['usuarioexiste']) {
			$dados = $usu_receita['dados'];
			$usudatanascimento = date('d/m/Y', strtotime($dados['dt_nascimento_rf']));
			list($ddd, $telefone) = explode('-', $dados['ds_contato_pessoa']);
			$retorno = array('usunome' => ($dados['no_pessoa_rf']), 'usufoneddd' => $ddd, 'usufonenum' => $telefone, 'ususexo' => $dados['sg_sexo_rf'], 'usudatanascimento' => $usudatanascimento, 'regcod' => $dados['regcod'], 'muncod' => $dados['muncod'], 'tpocod' => $dados['tpocod'], 'entid' => $dados['entid'],);
		}
		return $retorno;
	}

	function salvar($cpf, $etapa)
	{
		$dados = $this->getAllByValues(array('usucpf' => $cpf));

		if (empty($dados)) {
			$senha = strtoupper(senha());
			$this->setAttributeValue('ususenha', $senha);
			$this->setAttributeValue('usuchaveativacao', 'f');
			$this->setAttributeValue('ususenha', md5_encrypt_senha($senha, ''));

			if (IS_PRODUCAO) {
				$this->enviarEmail($etapa);
			}
			return $this->insert(true, true);
		} else {
			$this->setAttributeValue('suscod', 'A');
			$this->treatEntityToUser();
			return $this->update();
		}
	}

	function enviarEmail($etapa)
	{
		$assunto = 'Inscri��o no Cadastro do SIMEC - Planos T�ticos Operacionais';
		$comprimento = 'Prezado Sr.  ';

		if (str_replace('\'', '', $this->getAttributeValue('ususexo')) === 'F') {
			$comprimento = 'Prezada Sra. ';
		}

		$ususenha = md5_decrypt_senha($this->getAttributeValue('ususenha'), '');
		$usunome = str_replace('\'', '', $this->getAttributeValue('usunome'));
		$mensagem = $comprimento . $usunome . ',';

		$mensagem .= "<br><br>Voc� foi cadastrado(a) no SIMEC como Executor, no sistema de Planos T�ticos Operacionais - PTO.";
		$mensagem .= "<br><br> <b>Sua Senha � {$ususenha}.</b> <br><br>Ao se conectar, altere esta senha para a sua senha preferida.<br><br> Para maiores informa��es entre em contato conosco:";
		$mensagem .= "<br><b>Telefone:</b> xxx xxxx-xxxx";
		$mensagem .= "<br><b>E-mail:</b> simec@teste.com.br";

		$e = new EmailAgendado();
		$e->setTitle($assunto);
		$e->setText($mensagem);
		$e->setName($this->getAttributeValue('usunome'));
		$e->setEmailOrigem("no-reply@mec.gov.br");
		$e->setEmailsDestino($this->getAttributeValue('usuemail'));
		$e->enviarEmails();
	}

	public function getComboUfs()
	{
		$sql = "SELECT regcod AS codigo, regcod||' - '||descricaouf AS descricao FROM uf WHERE codigoibgeuf IS NOT NULL ORDER BY 2";
		$dados = $this->_db->carregar($sql);
		return $this->getOptions($dados, array('prompt' => ' Selecione '), 'regcod');
	}

	public function getOptions(array $dados, array $htmlOptions = array(), $idCampo = null, $descricaoCampo = null)
	{
		$html = '';
		$selected = '';

		if (isset($htmlOptions['prompt'])) {
			$html .= '<option value="">' . strtr($htmlOptions['prompt'], array('<' => '&lt;', '>' => '&gt;')) . "</option>\n";
		}

		if ($dados) {
			foreach ($dados as $data) {
				if ($idCampo) {
					$selected = ($data['codigo'] === $this->getAttributeValue($idCampo) ? "selected='true' " : "");
				}
				$html .= "<option {$selected}  title=\"{$data['descricao']}\" value= " . $data['codigo'] . ">  " . simec_htmlentities($data['descricao']) . " </option> ";
			}
		}
		return $html;
	}

	public function getComboMunicipios($estuf)
	{
		$sql = "SELECT muncod AS codigo, mundescricao AS descricao  FROM territorios.municipio WHERE estuf = '" . $estuf . "' ORDER BY descricao";
		$dados = $this->_db->carregar($sql);
		return $this->getOptions($dados, array('prompt' => ' Selecione '), 'muncod');
	}

	public function getComboTipoOrgao()
	{
		$sql = "SELECT tpocod as codigo, tpodsc as descricao FROM public.tipoorgao WHERE tpostatus='A' ";
		$dados = $this->_db->carregar($sql);
		return $this->getOptions($dados, array('prompt' => ' Selecione '), 'tpocod');
	}

	public function getComboOrgaos($tpocod, $regcod, $muncod)
	{
		$inner = ($tpocod == 3 || $tpocod == 2) ? ' INNER JOIN entidade.endereco eed ON eed.entid = ee.entid ' : '';
		$uniao = ($tpocod == 3 || $tpocod == 2) ? " UNION ALL ( SELECT 999999 AS codigo, 'OUTROS' AS descricao )" : '';

		if ($tpocod == 2) {
			$clausula = " AND eed.estuf = '{$regcod}' ";
		} elseif ($tpocod == 3) {
			$clausula = " AND eed.muncod = '{$muncod}' ";
		}

		$sql = "(SELECT
                        ee.entid AS codigo,
                        CASE WHEN ee.entorgcod is not null THEN ee.entorgcod ||' - '|| ee.entnome
                        ELSE ee.entnome END AS descricao
                FROM
                        entidade.entidade ee
                INNER JOIN entidade.funcaoentidade ef ON ef.entid = ee.entid
                INNER JOIN public.tipoorgaofuncao tpf ON ef.funid = tpf.funid
                        " . $inner . "
                WHERE
                    ee.entstatus = 'A' and
                        tpf.tpocod = '{$tpocod}'
                        " . $clausula . " AND
                        ( ee.entorgcod is null or ee.entorgcod <> '73000' )

                ORDER BY
                        ee.entnome)" . $uniao;
		$dados = $this->_db->carregar($sql);
		return $this->getOptions($dados, array('prompt' => ' Selecione '), 'entid');
	}

	public function getListing( $parans = array() )
	{
		$sql = $this->getDados($parans);

		$list = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);

		$list->setQuery($sql)
			->setCabecalho(array('CPF ', 'Nome', 'Telefone', 'E-mail', 'UF', 'Munic�pio'))
			->addCallbackDeCampo('usunome', 'alinhaParaEsquerda')
			->addCallbackDeCampo('usuemail', 'alinhaParaEsquerda')
			->addCallbackDeCampo('cpf', 'formatar_cpf')
			->addCallbackDeCampo('mundescricao', 'alinhaParaEsquerda')
		    ->setAcoes(array('edit' => array('func' => 'vincularSistema','extra-params' => array('usunome'))));
			//->setTamanhoPagina(15);

		$list->render();
	}

	public function getDados( $parans = array()  ){
		$where = '';
		if(isset($parans['nome']) and !empty($parans['nome']) ){
			$nome =  addslashes($parans['nome']);
			$where .= " AND (usuario.usunome) ilike ('%{$nome}%')";
		}

		if(isset($parans['cpf']) and !empty($parans['cpf']) ){
			$usucpf =  addslashes( pega_numero($parans['cpf']) );
			$where .= " AND usuario.usucpf = '{$usucpf}' ";
		}
		
		$sql = "
        	SELECT
				usuario.usucpf,
				usuario.usucpf as cpf,
				usuario.usunome,
                '(' || usuario.usufoneddd || ') ' || usuario.usufonenum as fone ,
                usuario.usuemail,
                usuario.regcod,
                municipio.mundescricao

			FROM  seguranca.usuario usuario
			LEFT JOIN  territorios.municipio municipio on municipio.muncod = usuario.muncod

			WHERE usunome IS NOT NULL and usuario.suscod != 'B' {$where}
			LIMIT 15
         ";
		return $sql;
	}
}
