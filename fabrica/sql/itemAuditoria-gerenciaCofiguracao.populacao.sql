BEGIN TRANSACTION;
insert into fabrica.itemauditoria (itemnome, itemdsc, itemsituacao) VALUES
('Aderente ao template?', 'Aderente ao template?', true),
('Atualizado?', 'Artefato Atualizado?', true),
('Hist�rico atualizado?', 'Hist�rico atualizado?', true),
('Referencia ao reposit�rio?', 'A SS faz referencia ao reposit�rio correto?', true),
('Encontra-se no reposit�rio?', 'Foi criado e se encontra no reposit�rio?', true);
COMMIT;