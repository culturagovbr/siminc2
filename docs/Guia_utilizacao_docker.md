# SIMEC - Guia de utilização do Docker

Para facilitar o desenvolvimento e escabilidade da aplicação trabalhamos a plataforma Docker. Com essa plataforma temos a possibilidade de isolar serviços permitindo várias possibilidades de integrações, manutenções e melhorias.

Caso queira conhecer um pouco mais sobre a plataforma Docker é possível acessar uma apresentação criada pela equipe da UFABC que é simples e objetiva fornecendo o que você precisa saber para iniciar no mundo Docker, clicando [aqui](http://pt.slideshare.net/vinnyfs89/docker-essa-baleia-vai-te-conquistar?qid=aed7b752-f313-4515-badd-f3bf811c8a35&v=&b=&from_search=1).

## Docker Images

O Docker fornece "Images" que são imagens de sistemas operacionais, o qual podemos costumizar para que esteja de acordo com um cenário mais próximo possível do ambiente de produção, criando nossas próprias imagens ou receitas para que sejam geradas dinâmicamente.

As "Images" são como "templates". Com elas também é possível versionar estados do sistema operacional e gerar "Containers". 

## Docker Containers

Um container é a implementação de uma Imagem, quando ele é criado, é possível definir configurações para acesso e compartilhamento de conteúdo. 

## Docker Commands

Para executarmos ações na plataforma docker precisamos executar alguns comandos. Clicando [aqui](https://github.com/vinnyfs89/dockerCommands) você pode obter uma lista dos comandos mais utilizados do Docker.

## Imagem de Ambiente

A principal imagem utilizadas pelo SIMEC podem ser encontradas [aqui](https://hub.docker.com/r/culturagovbr/simec) .

## Dockerfile

O SIMEC possui um Dockerfile contendo todas as informações necessárias para que tenhamos um ambiente para o funcionamento
da aplicação. Para acessa-lo, clique [aqui](https://github.com/culturagovbr/docker-simec) .
