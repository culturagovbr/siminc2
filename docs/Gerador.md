# Usando o Gerador
**Etapa 1** Selecionar um esquema.<br>
**Etapa 2** Selecionar as tabelas.<br>
**Etapa 3** Confirmar as informações adicionais: caminho, extensão dos arquivos e prefixo da classe.

# Erro no gerador

Caso ocorrer o seguinte erro no gerador, você deverá criar um caminho de acordo com o diretório anexado no erro:

Warning - Ambiente de Desenvolvimento

fopen(/var/www/siminc2/www/gerador/arquivos_gerados/model/Acao.inc): failed to open stream: No such file or directory


Esse erro acontece pois não encontra um diretório para gerar os novos arquivos.

## Comandos
Caso não tenha as pastas em seu diretório, você tera que executar os seguintes comandos:
```
	mkdir arquivos_gerados
	cd arquivos_gerados
	mkdir form
	mkdir lista
	mkdir controller
	mkdir model
```
Esses comandos deverão ser executados dentro do diretório simin2/www/gerador/


