# Compilador MEPA
Trabalho da Disciplina [CI1211 - Construção de Compiladores](https://www.inf.ufpr.br/bmuller/#/ci1211) do curso de Ciência da Computação - Bacharelado da UFPR, 2º semestre de 2023. Ministrada pelo prof. Dr. Bruno Müller Junior.

O objetivo deste trabalho é criar um compilador simplificado da linguagem Pascal que gera código MEPA (Máquina Virtual de Execução Pascal) conforme definido no livro do prof. Tomasz Kowaltowski, [Implementação de Linguagens de Programação](https://www.ic.unicamp.br/~tomasz/ilp/ilp.pdf).

Todo este trabalho foi baseado no conteúdo do [Projeto Base](https://www.inf.ufpr.br/bmuller/assets/ci1211/Recursos/ProjetoBase.tar.bz2) fornecido para a disciplina, com todos os créditos mantidos em arquivos que os continham. A maior diferença estrutural entre o projeto original e este vem da criação de um diretório próprio [`testes/`](./testes) para o qual foram movidos os arquivos `.pas` de entrada para os testes.

Abaixo encontram-se as instruções de uso básicas deste repositório, modificadas do arquivo `USAGE` presente no Projeto Base fornecido pelo prof. Bruno.

## Conteúdo do arquivo `USAGE`
1. Só para gerar código MEPA:
```bash
make
```

2. Executar o programa
```bash
./compilador testes/teste1.pas
```

3. Ver código MEPA
```bash
cat MEPA
```

Para aqueles mais curiosos, verifiquem o arquivo `lex.yy.c` . Este arquivo é a implementação em "C" do autômato gerado pelo analisador léxico ao ler o arquivo [`compilador.l`](./compilador.l). De forma análoga, `compilador.tab.c` é o autômato a pilha gerado pelo `bison` ao ler o arquivo [`compilador.y`](./compilador.y).

Na segunda parte da disciplina, será mostrado como gerar um autômato a pilha (tabela de ações e de desvios) para uma classe de gramáticas mais restritas do que esta, porém que segue o mesmo "esquema".
