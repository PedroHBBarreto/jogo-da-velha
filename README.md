# 📚 Jogo da Velha na linguagem C

Este repositório contém uma recriação do `Jogo da Velha`, que consiste em um programa completo, com menu, instruções e um placar de líderes. Seu objetivo é integrar conceitos e funcionalidades da `linguagem C`, como alocação dinâmica de memória e persistência de dados, aplicando-os em um projeto real.

## 🎯 Sobre o Projeto

> - Dominar funcionalidades da linguagem C, bem como consolidar conhecimentos em alocação dinâmica e persistência de dados.
> - Criar um projeto real que sirva para integrar e incentivar os estudos por meio da programação de uma aplicação prática e divertida.
> - Diversos cuidados foram tomados nas escolhas da interface, visando uma melhor experiência do usuário. Por exemplo, ao colorir os nomes e símbolos no tabuleiro, optou-se por `ciano` e `magenta` por não causarem conflitos visuais para daltônicos.
> - As cores podem ser alteradas a qualquer momento, bem como outras macros úteis, no início do código.
> - Para permitir testes, o arquivo do placar `"placar_jdv.bin"` disponível no repositório contém 3 jogadores cadastrados: Primeiro, Segundo e Terceiro, com 3, 2 e 1 vitória, respectivamente.
> - Depois de verificar o funcionamento, para o uso real, basta excluir o arquivo de testes, pois o programa irá criar um novo, se necessário.

## ⚙️ Funcionalidades

> - O código contém, no menu inicial, as opções de `jogar`, ver `placar`, `instruções` e `sair`.
> - O placar sempre imprimirá até os 10 melhores colocados, mas pode ser atualizado caso algum jogador faça novas pontuações.
> - O placar é registrado em um arquivo `.bin`, mais difícil de ser alterado acidentalmente. Além disso, em caso de erro ou exclusão, o jogo gerará outro arquivo, sendo capaz de manter os dados, caso isso ocorra durante o uso.
> - O jogo permite a reutilização do usuário cadastrado para que ele possa continuar pontuando, bem como pode ser jogado várias vezes seguidas com a mesma dupla, que pode ser trocada voltando ao menu.

## ⚠️ Atenção
> - O código pode conter comentários indicando alterações de caminhos, macros, funções ou adaptações locais. Antes de executar, verifique se o estado atual se adequa ao seu objetivo.

## 🛠️ Ferramentas e Ambiente

> - **Linguagem C.**
> - **Compilador:** GCC - versão 15.2.0.
> - **IDE / Editor:** Visual Studio Code.
> - **Extensões Úteis:** C/C++ (Microsoft) e C/C++ Compile Run.

## 💻 Como Executar
Instruções para rodar o código localmente:

1. Clone o repositório:
    >   ```bash
    >   git clone https://github.com/PedroHBBarreto/jogo-da-velha.git
    >   ```

2. Acesse o repositório clonado, compile e execute manualmente ou por comandos.
    >   ```bash
    >   cd jogo-da-velha
    >   g++ jogo_da_velha.cpp -o jogo_da_velha.exe
    >   .\jogo_da_velha.exe
    >   ```

3. Caso deseje editar o código:
    >   Selecione o arquivo e abra com o editor de texto de sua escolha ou digite no terminal algum dos comandos a seguir:
    - Abrir no **Notepad**:
        >   ```bash
        >   notepad jogo_da_velha.cpp
        >   ```
    - Abrir no **Visual Studio Code**:
        >   ```bash
        >   code jogo_da_velha.cpp
        >   ```