# SIMEC

[![GitHub Issues Abertas](https://img.shields.io/github/issues/culturagovbr/siminc2.svg?maxAge=2592000)]() 
[![GitHub Issues Fechas](https://img.shields.io/github/issues-closed-raw/culturagovbr/siminc2.svg?maxAge=2592000)]()
<a href="https://app.zenhub.com/workspace/o/culturagovbr/siminc2/boards" target="_blank">
    <img src="https://img.shields.io/badge/Managed_with-ZenHub-5e60ba.svg" alt="zenhub">
</a>

O [SIMEC](https://softwarepublico.gov.br/social/simec/) é uma ferramenta web escrita em linguagem PHP e com servidor de banco de dados PostgreSQL. O sistema em PHP é responsável pela lógica do servidor com interfaces do lado do cliente escritas em Javascript, enquanto o PostgreSQL faz o papel de repositório de dados.

Estas são documentações sobre o processo de desenvolvimento do SIMEC, versionameno e publicação:

* [Roteiro de publicação de releases](docs/Roteiro_de_publicacao_de_releases.md)
* [Regras de versionamento](docs/Regras_versionamento.md)
* [Guia de operação e desenvolvimento](docs/Guia_de_operacao-desenvolvimento.md)
* [Guia de fluxo de demandas do Kanban](docs/Fluxo_Kanban.md)

## Docker
Utilizamos o Docker como plataforma de desenvolvimento com o intuito de garantir o mesmo ambiente de desenvolvimento 
independentemente do Sistema Operacional(SO) utilizado. Informaçoes mais detalhadas sobre a utilização do docker clique
[aqui](docs/Guia_utilizacao_docker.md).

Para criar um ambiente para trabalhar com o SIMEC basta executar o comando abaixo:
```
  docker-compose up
```

Para visualizar os container
```
  docker-compose ps
```

Após finalizado os procedimentos de configuração do ambiente acesse o sistema pela porta 8082 por exemplo:
```
http://localhost:8083
```
Na tela de login utilize o CPF de demonstração e senha abaixo:
```
CPF: 86274565426
Senha: 123456
```

## Tecnologias
* [PHP](http://php.net/)
* [Zend Framework 1](https://framework.zend.com/manual/1.12/en/learning.quickstart.html) 
* [Docker](https://www.docker.com)
* [jQuery](https://jquery.com/)
* [Bootstrap](https://getbootstrap.com/)
* [PostgreSQL](https://www.postgresql.org/)

## Autores
Várias pessoas colaboraram com o desenvimento do projeto SIMEC e decidimos centralizar em um único local todos os que participaram com o desenvolvimento do projeto.
  
Clique [aqui](docs/Autores.md) para visualizar.
