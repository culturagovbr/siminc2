<?php
/**
 * $Id: LoggerDb.php 77781 2014-03-24 19:10:23Z maykelbraz $
 */

/**
 * @see Simec_SoapClient_Log_LoggerDb
 */
require_once(dirname(__FILE__) . '/Logger.php');

/**
 * Classe de grava��o de log de chamadas webservice no banco de dados.
 * @see Simec_SoapClient_Log_Logger
 * @see Simec_BasicWS
 */
class Simec_SoapClient_Log_LoggerDb extends Simec_SoapClient_Log_Logger
{

    /**
     * Link com a base de dados para execu��o da query grava��o do log.
     * @var resource|cls_banco
     */
    protected $db;

    /**
     * Nome (e esquema) da tabela utilizada para gravar o log.
     * @var string
     */
    private $tableName;

    /**
     * String de inser��o de log na base de dados.
     * @var string
     */
    private $insertStmt;

    /**
     * Identifica��o do tipo de log desta classe.
     * @var string
     */
    protected $loggerType = 'db';

    /**
     * Executa a grava��o do log na base de dados.
     * A execu��o da query pode ser feita tanto com a cls_banco do simec, ou com
     * chamadas nativas do postgres.
     *
     * @param array $data Campos e valores mapeados de acordo com a defini��o de
     */
    public function writeLog(array $data)
    {
        $insStmt = sprintf(
            $this->insertStmt,
            str_replace("'", "''", $data['requestContent']),
            str_replace("'", "''", $data['requestHeader']),
            $data['requestTimestamp'],
            str_replace("'", "''", $data['responseContent']),
            str_replace("'", "''", $data['responseHeader']),
            $data['responseTimestamp'],
            $data['url'],
            $data['method'],
            $data['ehErro']
        );

        if ($this->db instanceof cls_banco) {
            $this->db->executar($insStmt);
            $this->db->commit();
        } else {
            pg_query($this->db, $insStmt);
        }
    }

    protected function createInsertStmt()
    {
        $this->insertStmt = <<<DML
INSERT INTO {$this->tableName}(
  {$this->fieldMap['requestContent']},
  {$this->fieldMap['requestHeader']},
  {$this->fieldMap['requestTimestamp']},
  {$this->fieldMap['responseContent']},
  {$this->fieldMap['responseHeader']},
  {$this->fieldMap['responseTimestamp']},
  {$this->fieldMap['url']},
  {$this->fieldMap['method']},
  {$this->fieldMap['ehErro']}
) VALUES ('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', %s)
DML;
    }

    /**
     * Faz a configura��o da classe de log.
     *
     * @param array $configData Parametros obrigat�rios: 'dbConnection', 'tableName' e 'fieldMap'.
     */
    public function setConfig(array $configData)
    {
        if (!isset($configData['dbConnection']) || empty($configData['dbConnection'])) {
            throw new Exception('Para utilizar LoggerDb, voc� precisa definir a conex�o que ser� utilizada.');
        }

        if (!isset($configData['tableName']) || empty($configData['tableName'])) {
            throw new Exception('Para utilizar LoggerDb, voc� precisa definir a entidade em que ser� feita a inser��o.');
        }

        if (!isset($configData['fieldMap']) || empty($configData['fieldMap'])) {
            throw new Exception('Para utilizar LoggerDb, voc� precisa definir o mapeamento de campos para inser��o.');
        }

        $this->db = $configData['dbConnection'];
        $this->tableName = $configData['tableName'];
        foreach ($configData['fieldMap'] as $field => $map) {
            $this->addField($field, $map);
        }

        $this->createInsertStmt();
    }
}
