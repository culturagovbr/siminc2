
-- APAGANDO DE EXECUÇÃO ORÇAMENTÁRIA DO WEBSERVICE DO SIOP
DELETE FROM wssof.ws_execucaoorcamentariadto;
DELETE FROM spo.siopexecucao;

VACUUM FULL VERBOSE wssof.ws_execucaoorcamentariadto;
VACUUM FULL VERBOSE spo.siopexecucao;

-- APAGANDO ARQUIVOS DE AUDITORIA DE USO DA BASE DE DADOS
DELETE FROM auditoria.auditoria;
VACUUM FULL VERBOSE auditoria.auditoria;

