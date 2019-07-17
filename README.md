# SIMINC2

[![GitHub Issues Abertas](https://img.shields.io/github/issues/culturagovbr/siminc2.svg?maxAge=2592000)]() 
[![GitHub Issues Fechas](https://img.shields.io/github/issues-closed-raw/culturagovbr/siminc2.svg?maxAge=2592000)]()
<a href="https://app.zenhub.com/workspace/o/culturagovbr/siminc2/boards" target="_blank">
    <img src="https://img.shields.io/badge/Managed_with-ZenHub-5e60ba.svg" alt="zenhub">
</a>

O SIMINC2 é uma customização do Sistema Integrado de Planejamento Orçamento e Finanças - [SIMEC](https://softwarepublico.gov.br/social/simec/), solução desenvolvida inicialmente pelo Ministério da Educação e compartilhada como Software Público com diversos órgãos públicos.

Em 2012 o Ministério da Cidadania implantou o SIMEC e realizou diversas customizações, batizando-o de SIMINC. Em 2015 o MC viu a necessidade de atualizar a versão do software e de qualificar o processo de desenvolvimento, evitando futuros conflitos de versões. Para isso o MEC forneceu uma nova cópia do código mais atual e o MC iniciou um novo ciclo de customizações de maneira a possibilitar o retorno do código novo ao MEC e reduzir o esforço de outros órgãos que também desejarem utilizá-lo. Esta versão foi batizada de [SIMINC2](http://siminc2.cultura.gov.br/) e desde então vem sendo desenvolvida abertamente aqui no Github.

Estas são documentações sobre o processo de desenvolvimento do SIMINC2, versionameno e publicação:

* [Roteiro de publicação de releases](docs/Roteiro_de_publicacao_de_releases.md)
* [Regras de versionamento](docs/Regras_versionamento.md)
* [Guia de operação e desenvolvimento](docs/Guia_de_operacao-desenvolvimento.md)
* [Guia de fluxo de demandas do Kanban](docs/Fluxo_Kanban.md)

Estes são os documentos de Histórias de Usuários dos principais módulos do SIMINC2:
#### Módulo SIS
* [[HU001]Solicitar Acesso](https://drive.google.com/file/d/1aKV2XY5jOnirdWT-2066fEecS6jNvZfw/view?usp=sharing)
* [[HU002]Acessar Sistema](https://drive.google.com/file/d/1f0ed_Ttl-v1LLmrehS9oNzD5xDLNszZq/view?usp=sharing)
#### Módulo Planejamento Orçamentário

#### Módulo Monitoramento

Estes são os documentos de Caso de Uso do SIMINC2:
#### Módulo Planejamento Orçamentário
* [[UC001]Listar PI](https://docs.google.com/document/d/1C__jUY_Sd2e34Q98I_vRXDoljyNQlJBy8myWyE2HWyE/edit?usp=sharing)
* [[UC002]Cadastrar PI](https://docs.google.com/document/d/1Tjv5MKW66fER0rpa6S28d94PDYkJCffmr7CB26uW3us/edit?usp=sharing)
* [[UC003]Tramitar PI](https://docs.google.com/document/d/1R3NiUYxq_WB6mOB7UGw73nzgG1FMUzve0JI5rGv4_1Q/edit?usp=sharing)
* [[UC004]Excluir PI](https://docs.google.com/document/d/14xLg2ZQDtsSzeSmM-S9e7BJoF09PDam_hB5Eo4hRzeM/edit?usp=sharing)

#### Módulo Emendas Parlamentares
* [[UC001]Listar Emenda](https://docs.google.com/document/d/1YYDxSPJ-QHQ_Iibv_n0pIqEXzOnzvVm7C0QYGXBHRDQ/edit?usp=sharing)
* [[UC002]Cadastrar Emenda](https://docs.google.com/document/d/1CX6VK1uuI2112cwqQ7L6OK-BMIkiHtgzymbZTRBQ4BQ/edit?usp=sharing)
* [[UC003]Cadastrar Beneficiário](https://docs.google.com/document/d/1ZpcmLDE6sdwCoHg_NUIhSEWUIqs15eHSIw9g8sjQVZ0/edit?usp=sharing)
* [[UC004]Tramitar Beneficiário](https://docs.google.com/document/d/1RZFbgFfyAbz90ksIVT7RNkSQgj7vziR5l2h3m1ULi_I/edit?usp=sharing)

## Docker
Utilizamos o Docker como plataforma de desenvolvimento com o intuito de garantir o mesmo ambiente de desenvolvimento 
independentemente do Sistema Operacional(SO) utilizado. Informaçoes mais detalhadas sobre a utilização do docker clique
[aqui](docs/Guia_utilizacao_docker.md).

Para criar um ambiente para trabalhar com o SIMINC2 basta executar o comando abaixo:
```
  docker-compose up
```

Para visualizar os container
```
  docker-compose ps
```

Após finalizado os procedimentos de configuração do ambiente acesse o sistema pela porta 8082 por exemplo:
```
http://localhost:8082
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
Várias pessoas colaboraram com o desenvimento do projeto SIMINC2 e decidimos centralizar em um único local todos os que participaram com o desenvolvimento do projeto.
  
Clique [aqui](docs/Autores.md) para visualizar.
