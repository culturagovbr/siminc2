<?php

/**
 * Classe de mapeamento da entidade public.arquivo_modulo
 *
 * @category Class
 * @package  A1
 * @link     no link
 */
require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Public_Model_ArquivoModulo
 *
 * @category Class
 * @package  A1
 * @link     no link
 */
class Public_Model_Arquivo extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'public.arquivo';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'arqid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'arqid' => null,
        'arqnome' => null,
        'arqdescricao' => null,
        'arqextensao' => null,
        'arqtipo' => null,
        'arqtamanho' => null,
        'arqdata' => null,
        'arqhora' => null,
        'arqstatus' => null,
        'usucpf' => null,
        'sisid' => null
    );
    
}
