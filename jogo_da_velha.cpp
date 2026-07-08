/*  Jogo da velha com placar

    Implementação de jogo da velha em linguagem C com placar dos 10 melhores,
    garantindo persistência dos dados. O código tem o propósito de servir como
    um consolidado para os estudos da linguagem, aplicando conceitos e funções.
    Os dados são lidos do arquivo de placar para um vetor criado dinamicamente
    durante o uso, que é ordenado ao final de cada sessão e atualizado no placar.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <ctype.h>
#define ARQUIVO_PLACAR "placar_jdv.bin"
// Macros para alterar limites de modo simplificado.
#define MAX_NOME 50
#define MAX_PLACAR 10
#define TAM_TABELA 3
// Macros para cores:
// Ciano.
#define COR_X "\033[96;1m"
// Magenta.
#define COR_O "\033[95;1m"
#define RESET "\033[0m"

// Estrutura para guardar as informações do jogador.
typedef struct{
    char nome[MAX_NOME];
    int vitorias;
}Jogador;
//--------------------------------------------------------------------------------
// Para mais detalhes, verificar o código comentado das funções.
// Ordenação:
void swap(Jogador *valor1, Jogador *valor2);
int partition(Jogador placar[], int inicio, int fim);
void quickSort(Jogador placar[], int inicio, int fim);
//--------------------------------------------------------------------------------
// Funções gerais:
int menu();
void esperarEnter();
void instrucoes();
void cadastrarNome(char nome[], int tamanho);
//--------------------------------------------------------------------------------
// Funções específicas:
FILE* abrirArquivo(const char *nome, const char *modo);
void imprimirPlacar(Jogador placar[], int qtd);
int qtdRegistrados(FILE *arquivo);
void inicializarTabela(char tabela[][TAM_TABELA]);
int verificarCadastro(Jogador placar[], Jogador *jogadores, int *indices, int qtd);
void copiarCadastro(Jogador *placar, Jogador *jogadores);
int partida(char tabela[][TAM_TABELA], Jogador jogadores[]);
void imprimirTabela(char tabela[][TAM_TABELA]);
void imprimirJogada(char simbolo);
char jogadorVez(int rodada);
void realizarJogada(char tabela[][TAM_TABELA], int rodada);
int verificarVitoria(char tabela[][TAM_TABELA]);
int verificarLinhas(char tabela[][TAM_TABELA]);
int verificarColunas(char tabela[][TAM_TABELA]);
int verificarDiagonais(char tabela[][TAM_TABELA]);
void declararVencedor(char vencedor, Jogador jogadores[]);
void atualizarPlacar(Jogador placar[], Jogador jogadores[2], int indices[]);
//--------------------------------------------------------------------------------
int main(){
    // Garante que os caracteres especiais do tabuleiro sejam exibidos corretamente.
    SetConsoleOutputCP(CP_UTF8);
    system("title Jogo da Velha");
    // Abre o arquivo, com tratamento para o caso de ainda não existir.
    FILE *arquivo = abrirArquivo(ARQUIVO_PLACAR, "rb");
    // Retorna a quantidade de jogadores cadastrados (no primeiro uso retorna 0).
    int qtd = qtdRegistrados(arquivo);
    // Cria o vetor do placar durante o uso como ponteiro para alocar espaço dinamicamente;
    // Atribui-se o valor nulo para identificar o primeiro uso.
    Jogador *placar = NULL;
    // Se existem jogadores cadastrados, aloca o devido espaço e copia os dados.
    if(qtd > 0){
        placar = (Jogador*)malloc(qtd * sizeof(Jogador));
        fread(placar, sizeof(Jogador), qtd, arquivo);
    }
    fclose(arquivo);
    int operacao = 1;
    while(operacao == 1){
        int opcaoMenu = menu();
        switch(opcaoMenu){
            case 1:{
                int jogar = 1;
                while(jogar == 1){
                    // Cria e inicializa os dois jogadores atuais sem nome e sem vitórias.
                    Jogador jogadores[2] = {{"\0", 0},{"\0", 0}};
                    // Esse vetor guardará os índices do placar dos novos jogadores, já existentes ou não.
                    int indices[2];
                    int novosJogadores = 0;
                    char tabela[TAM_TABELA][TAM_TABELA];
                    int jogando = 0;
                    // O cadastro será feito até que dois jogadores confirmem seus nomes.
                    while(jogando < 2){
                        system("cls");
                        printf("\t╔═════════════════╗\n");
                        printf("\t║    CADASTRO     ║\n");
                        printf("\t╚═════════════════╝\n\n\n");
                        // Separa a impressão dos jogadores e símbolos com cada cor.
                        if(jogadorVez(jogando + 1) == 'X'){
                            printf(COR_X "Jogador X" RESET " - ");
                        }
                        if(jogadorVez(jogando + 1) == 'O'){
                            printf(COR_O "Jogador O" RESET " - ");
                        }
                        cadastrarNome(jogadores[jogando].nome, MAX_NOME);
                        int cadastrado = 0;
                        // Para todos os casos, exceto o primeiro uso, verificar se os jogadores já estão cadastrados.
                        if(placar != NULL){
                            cadastrado = verificarCadastro(placar, &jogadores[jogando], &indices[jogando], qtd);
                        }
                        // Se ainda não for cadastrado, grava o nome e atualiza os índices e a quantidade de jogadores.
                        if(cadastrado == 0){
                            indices[jogando] = qtd;
                            qtd++;
                            novosJogadores++;
                            jogando++;      
                        }
                        // Se cadastrado, pede a confirmação de identidade.
                        else{
                            printf("\n\nJogador ja cadastrado:");
                            printf("\n\t1) Confirmar identidade");
                            printf("\n\t0) Recadastrar");
                            printf("\n\nOpcao: ");
                            int confirmar;
                            // Proteção e validação padrão de entrada (verifique a função "menu").
                            int leitura = scanf("%i", &confirmar);
                            while(leitura != 1 || (confirmar != 0 && confirmar != 1)){
                                while(getchar() != '\n');
                                printf("\n\nInsira uma opcao valida: ");
                                leitura = scanf("%i", &confirmar);
                            }
                            // Ao confirmar sua identidade, o cadastro será copiado do placar para o jogo atual.
                            if(confirmar == 1){
                                copiarCadastro(&placar[indices[jogando]], &jogadores[jogando]);
                                // Apenas aumenta a quantidade de jogadores se confirmado.
                                jogando++;
                            }
                        }
                    }
                    int iniciarPartida = 1;
                    while(iniciarPartida == 1){
                        // O retorno da função "partida" permite jogar várias vezes com a mesma dupla.
                        iniciarPartida = partida(tabela, jogadores);
                    }
                    // Ao sair, volta ao menu para permitir que outra dupla jogue ou para encerrar o jogo.
                    jogar = iniciarPartida;
                    // Caso existam novos cadastros, o placar será realocado e aumentado.
                    if(novosJogadores > 0){
                        placar = (Jogador*)realloc(placar, qtd * sizeof(Jogador));
                    }
                    // Atualizar e ordenar o placar a partir dos melhores jogadores, até o décimo colocado.
                    atualizarPlacar(placar, jogadores, indices);
                    quickSort(placar, 0, qtd - 1);
                    // Abre o arquivo e grava logo depois cada sessão, garantindo maior integridade dos dados.
                    arquivo = abrirArquivo(ARQUIVO_PLACAR, "wb");
                    fwrite(placar, sizeof(Jogador), qtd, arquivo);
                    fclose(arquivo);
                }
                break;
            }
            case 2:{
                system("cls");
                printf("\t╔═════════════════╗\n");
                printf("\t║     PLACAR      ║\n");
                printf("\t╚═════════════════╝\n\n\n");
                if(qtd == 0){
                    printf("Ainda nao ha jogadores cadastrados");
                }
                // Operador ternário retorna a quantidade de linhas do placar;
                // Isso garante a impressão adequada até o limite dos 10 melhores jogadores.
                else{
                    int qtdPlacar = (qtd >= MAX_PLACAR) ? MAX_PLACAR : qtd;
                    imprimirPlacar(placar, qtdPlacar);
                }
                esperarEnter();
                break;
            }
            case 3:{
                instrucoes();
                esperarEnter();
                break;
            }
            case 4:{
                // Libera a memória alocada dinamicamente para encerrar o programa.
                free(placar);
                system("cls");
                printf("\t╔════════════════════╗\n");
                printf("\t║ SISTEMA ENCERRADO  ║\n");
                printf("\t╚════════════════════╝\n");
                operacao = 0;
                esperarEnter();
                break;
            }
        }
    }
    return 0;
}
//--------------------------------------------------------------------------------
// Função de troca simples, passando o valor por referência.
void swap(Jogador *valor1, Jogador *valor2){
    Jogador temp = *valor1;
    *valor1 = *valor2;
    *valor2 = temp;
}
//--------------------------------------------------------------------------------
// Move o elemento analisado até sua posição correta;
// Garante que os elementos maiores e menores estejam dos lados corretos.
int partition(Jogador placar[], int inicio, int fim){
    // Pressupõe-se que a posição correta possa ser a primeira;
    int posicaoCorreta = inicio;
    // Guarda o valor a comparar.
    int valorPivot = placar[fim].vitorias;
    // Percorre todos os elementos e verifica a ordem;
    // Caso o elemento atual seja maior, a posição correta aumenta até a ideal.
    for(int i = inicio; i < fim; i++){
        if(placar[i].vitorias > valorPivot){
            swap(&placar[i], &placar[posicaoCorreta]);
            posicaoCorreta++;
        }
    }
    // Troca final para garantir a posição do elemento avaliado.
    swap(&placar[fim], &placar[posicaoCorreta]);
    return posicaoCorreta;
}
//--------------------------------------------------------------------------------
// Função de Quick Sort recursiva para a ordenação.
void quickSort(Jogador placar[], int inicio, int fim){
    // Condição de parada só permite que a ordenação seja feita com dois ou mais elementos;
    // Chama recursivamente a ordenação para os dois lados.
    if(inicio < fim){
        int posicaoPivot = partition(placar, inicio, fim);
        // Ordenação para a esquerda (maiores).
        quickSort(placar, inicio, posicaoPivot - 1);
        // Ordenação para a direita (menores).
        quickSort(placar, posicaoPivot + 1, fim);
    }
}
//--------------------------------------------------------------------------------
int menu(){
    system("cls");
    printf("\t╔═════════════════╗\n");
    printf("\t║  JOGO DA VELHA  ║\n");
    printf("\t╚═════════════════╝\n\n");
    
    printf(COR_X "\n░██    ░██" RESET "               " COR_O "  ░██████  " RESET);
    printf(COR_X "\n ░██  ░██ " RESET "               " COR_O " ░██   ░██ " RESET);
    printf(COR_X "\n  ░██░██  " RESET "               " COR_O "░██     ░██" RESET);
    printf(COR_X "\n   ░███   " RESET "    ░██████    " COR_O "░██     ░██" RESET);
    printf(COR_X "\n  ░██░██  " RESET "               " COR_O "░██     ░██" RESET);
    printf(COR_X "\n ░██  ░██ " RESET "               " COR_O " ░██   ░██ " RESET);
    printf(COR_X "\n░██    ░██" RESET "               " COR_O "  ░██████  " RESET);
    printf("\n\n\n");
    printf("1) Jogar\n");
    printf("2) Placar (10 melhores)\n");
    printf("3) Instrucoes\n");
    printf("4) Sair\n");
    printf("\n\nOpcao: ");
    int opcao;
    // Garante que a leitura foi efetuada e não reutilize o valor da opção anterior.
    int leitura = scanf("%i", &opcao);
    // Verificação para opções inválidas.
    while(leitura != 1 || (opcao < 1 || opcao > 4)){
        // O getchar consome o buffer e impede falha em caso de digitar letras.
		while(getchar() != '\n');
        printf("\nInsira uma opcao valida: ");
        leitura = scanf("%i", &opcao);
    }
    // Opção retornada que será usada no switch case do MENU.
    return opcao;
}
//--------------------------------------------------------------------------------
void esperarEnter(){
	// A função limpa o \n do último Enter e espera outro para continuar;
	// Permite a espera em uma tela, sem exigir entradas específicas.
    printf("\n\nAperte Enter para continuar...");
    while (getchar() != '\n');
    while (getchar() != '\n');
}
//--------------------------------------------------------------------------------
void instrucoes(){
    system("cls");
    printf("\t╔═════════════════╗\n");
    printf("\t║   INSTRUCOES    ║\n");
    printf("\t╚═════════════════╝\n");
    printf("\n\n");
    printf("- Ao iniciar o jogo, sera possivel cadastrar dois jogadores;\n");
    printf("- Os nomes serao tratados sem diferenca de caracteres;\n");
    printf("- Caso ja registrados, pedira confirmacao da identidade;\n");
    printf("- O placar sempre exibira os 10 melhores colocados;\n");
    printf("- Nao ha limite de jogadores cadastrados;\n");
}
//--------------------------------------------------------------------------------
void cadastrarNome(char nome[], int tamanho){
	printf("Insira seu nome: ");
	fgets(nome, tamanho, stdin);
	// Em caso de Enter acidental ou no buffer, escaneia a string novamente.
	while(nome[0] == '\n'){
		fgets(nome, tamanho, stdin);
	}
	// Verifica a posição do Enter, indicando que a string tem o tamanho adequado.
	int posicaoEnter = strcspn(nome, "\n");
	// String tem o tamanho previsto, considerando o Enter (com \0 no final):
	if(nome[posicaoEnter] == '\n'){
		// Garante que o Enter escaneado seja eliminado da string.
		nome[posicaoEnter] = '\0';
	}
	// Caso seja maior que o tamanho reservado:
	else{
		// Limpa o buffer com a parte cortada da string até o \n.
		while(getchar() != '\n');
	}
}
//--------------------------------------------------------------------------------
// Função para centralizar a abertura do arquivo e leitura mais fluida do main.
FILE* abrirArquivo(const char *nome, const char *modo){
    FILE *arquivo = fopen(nome, modo);
    // Para o caso de erro ou arquivo inexistente, cria e permite leitura (vazio: qtd == 0)
    if(arquivo == NULL && strcmp(modo, "rb") == 0){
        arquivo = fopen(nome, "wb+");
    }
    // Retorna o ponteiro do arquivo.
    return arquivo;
}
//--------------------------------------------------------------------------------
void imprimirPlacar(Jogador placar[], int qtd){
    printf("| Vitorias | Nomes\n");
    for(int i = 0; i < qtd; i++){
        printf("| %8i | %s\n", placar[i].vitorias , placar[i].nome);
    }
}
//--------------------------------------------------------------------------------
// Lê o tamanho do arquivo, calcula e retorna a quantidade de jogadores.
int qtdRegistrados(FILE *arquivo){
    int qtd = 0;
    fseek(arquivo, 0, SEEK_END);
    long tamanho = ftell(arquivo);
    rewind(arquivo);
    qtd = tamanho / sizeof(Jogador) ;
    return qtd;
}
//--------------------------------------------------------------------------------
// Inicializa a tabela com os valores de 1 a 9.
void inicializarTabela(char tabela[][TAM_TABELA]){
    int posicao = 1;
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            tabela[i][j] = '0' + posicao;
            posicao++;
        }
    }
}
//--------------------------------------------------------------------------------
int verificarCadastro(Jogador placar[], Jogador *jogadores, int *indices, int qtd){
    // Strings temporárias para o nome atual e o nome a ser comparado.
    char nomePlacar[MAX_NOME];
    char nome[MAX_NOME];
    // Normaliza o nome totalmente em maiúsculo para comparar com o placar.
    strcpy(nome, jogadores->nome);
    for(int i = 0; nome[i] != '\0'; i++){
            nome[i] = toupper(nome[i]);
    }
    // Percorre todo o placar para efetuar a comparação.
    for(int i = 0; i < qtd; i++){
        // Normaliza o nome a ser comparado no placar.
        strcpy(nomePlacar, placar[i].nome);
        for(int j = 0; nomePlacar[j] != '\0'; j++){
            nomePlacar[j] = toupper(nomePlacar[j]);
        }
        // Caso exista uma correspondência, retorna 1 e atualiza o vetor de índices.
        if(strcmp(nomePlacar, nome) == 0){
            *indices = i;
            return 1;
        }
    }
    // Se não encontrar o nome:
    return 0;
}
//--------------------------------------------------------------------------------
// Função criada para simplificar a leitura no main.
void copiarCadastro(Jogador *placar, Jogador *jogadores){
    *jogadores = *placar;
}
//--------------------------------------------------------------------------------
// Centraliza o fluxo da partida e retorna 1 ou 0 para permitir jogar novamente.
int partida(char tabela[][TAM_TABELA], Jogador jogadores[]){
    // Inicializa as principais variáveis e tabela.
    int vitoria = 0;
    int rodada = 1;
    inicializarTabela(tabela);
    char vencedor = ' ';
    // Ciclo do jogo, até a vitória ou rodada máxima.
    while(vitoria != 1 && rodada <=9 ){
        system("cls");
        printf("\t╔═════════════════╗\n");
        printf("\t║     Rodada %i    ║\n", rodada);
        printf("\t╚═════════════════╝\n");
        imprimirTabela(tabela);
        // Separa a impressão dos nomes e símbolos com cada cor.
        if(jogadorVez(rodada) == 'X'){
            printf("\n\n\tVez de " COR_X "%s - %c" RESET "\n", jogadores[(rodada + 1) % 2].nome, jogadorVez(rodada));
        }
        if(jogadorVez(rodada) == 'O'){
            printf("\n\n\tVez de " COR_O "%s - %c" RESET "\n", jogadores[(rodada + 1) % 2].nome, jogadorVez(rodada));
        }
        realizarJogada(tabela, rodada);
        vitoria = verificarVitoria(tabela);
        // Para o caso de vitória, grava o símbolo do vencedor.
        if(vitoria == 1){
            vencedor = jogadorVez(rodada);
            jogadores[(rodada + 1) % 2].vitorias++;
        }
        // Contador da rodada.
        rodada++;
    }
    system("cls");
    printf("\t╔═════════════════╗\n");
    printf("\t║   FIM DE JOGO   ║\n");
    printf("\t╚═════════════════╝\n\n\n");
    // Imprime a tabela final e declara o vencedor.
    imprimirTabela(tabela);
    declararVencedor(vencedor, jogadores);
    printf("\n\n\nDeseja jogar novamente?");
    printf("\n\n1) Sim");
    printf("\n0) Nao (retornar ao menu)");
    printf("\n\nOpcao: ");
    int jogar;
    // Proteção e validação padrão de entrada (verifique a função "menu").
    int leitura = scanf("%i", &jogar);
    while(leitura != 1 || (jogar != 0 && jogar != 1)){
        while(getchar() != '\n');
        printf("\n\nInsira uma opcao valida: ");
        leitura = scanf("%i", &jogar);
    }
    // Permite jogar novamente com a mesma dupla
    return jogar;
}
//--------------------------------------------------------------------------------
void imprimirTabela(char tabela[][TAM_TABELA]){
    printf("\t╔═════╦═════╦═════╗\n");
    printf("\t║  ");
    imprimirJogada(tabela[0][0]);
    printf("  ║  ");
    imprimirJogada(tabela[0][1]);
    printf("  ║  ");
    imprimirJogada(tabela[0][2]);
    printf("  ║\n");
    printf("\t╠═════╬═════╬═════╣\n");
    printf("\t║  ");
    imprimirJogada(tabela[1][0]);
    printf("  ║  ");
    imprimirJogada(tabela[1][1]);
    printf("  ║  ");
    imprimirJogada(tabela[1][2]);
    printf("  ║\n");
    printf("\t╠═════╬═════╬═════╣\n");
    printf("\t║  ");
    imprimirJogada(tabela[2][0]);
    printf("  ║  ");
    imprimirJogada(tabela[2][1]);
    printf("  ║  ");
    imprimirJogada(tabela[2][2]);
    printf("  ║\n");
    printf("\t╚═════╩═════╩═════╝\n");
    /*  Tabela base (Local da variável: "_")
    
        printf("\t╔═════╦═════╦═════╗\n");
        printf("\t║  _  ║  _  ║  _  ║\n");
        printf("\t╠═════╬═════╬═════╣\n");
        printf("\t║  _  ║  _  ║  _  ║\n");
        printf("\t╠═════╬═════╬═════╣\n");
        printf("\t║  _  ║  _  ║  _  ║\n");
        printf("\t╚═════╩═════╩═════╝\n");
    */
}
//--------------------------------------------------------------------------------
// Função criada para imprimir as variáveis do placar com a cor adequada.
void imprimirJogada(char simbolo){
    if(simbolo == 'X'){
        printf(COR_X "%c" RESET, simbolo);
    }
    else if(simbolo == 'O'){
        printf(COR_O "%c" RESET, simbolo);
    }
    else{
        printf("%c", simbolo);
    }
}
//--------------------------------------------------------------------------------
// Garante que o X seja o primeiro jogador (rodadas ímpares).
char jogadorVez(int rodada){
    if(rodada % 2 == 1){
        return 'X';
    }
    else{
        return 'O';
    }
}
//--------------------------------------------------------------------------------
// Pede a jogada e atualiza a matriz.
void realizarJogada(char tabela[][TAM_TABELA], int rodada){
    printf("\n\nEscolha a posicao desejada (1-9): ");
    int jogada;
    // Proteção e validação padrão de entrada (verifique a função "menu").
    int leitura = scanf("%i", &jogada);
    int linha = (jogada - 1) / 3;
    int coluna = (jogada - 1) % 3;
    while(leitura != 1 || ((jogada < 1 || jogada > 9) || (tabela[linha][coluna] == 'X' || tabela[linha][coluna] == 'O'))){
        while(getchar() != '\n');
        printf("\nInsira uma posicao valida: ");
        leitura = scanf("%i", &jogada);
        linha = (jogada - 1) / 3;
        coluna = (jogada - 1) % 3;
    }
    // Grava a jogada na tabela.
    tabela[linha][coluna] = jogadorVez(rodada);
}
//--------------------------------------------------------------------------------
// Centraliza a verificação da vitória, presente nas funções a seguir;
// Retorna a expressão booleana, onde qualquer positivo garante a vitória;
int verificarVitoria(char tabela[][TAM_TABELA]){
    int linha = verificarLinhas(tabela);
    int coluna = verificarColunas(tabela);
    int diagonal = verificarDiagonais(tabela);
    return linha || coluna || diagonal;
}
//--------------------------------------------------------------------------------
// Percorre as linhas da tabela e retorna se há vitória.
int verificarLinhas(char tabela[][TAM_TABELA]){
    for(int i = 0; i < TAM_TABELA; i++){
        int vitoria = 1;
        for(int j = 1; j < TAM_TABELA; j++){
            if(tabela[i][0] != tabela[i][j] || (tabela[i][0] != 'X' && tabela[i][0] != 'O')){
                // Para cada inconsistência, cancela a vitória.
                vitoria = 0;
            }
        }
        // Caso contrário, retorna a vitória (1).
        if(vitoria == 1){
            return 1;
        }
    }
    // Se não foi constatada vitória.
    return 0;
}
//--------------------------------------------------------------------------------
// Percorre as colunas da tabela e retorna se há vitória.
int verificarColunas(char tabela[][TAM_TABELA]){
    for(int i = 0; i < TAM_TABELA; i++){
        int vitoria = 1;
        for(int j = 1; j < TAM_TABELA; j++){
            if(tabela[0][i] != tabela[j][i] || (tabela[0][i] != 'X' && tabela[0][i] != 'O')){
                // Para cada inconsistência, cancela a vitória.
                vitoria = 0;
            }
        }
        // Caso contrário, retorna a vitória (1).
        if(vitoria == 1){
            return 1;
        }
    }
    // Se não foi constatada vitória.
    return 0;
}
//--------------------------------------------------------------------------------
// Percorre as diagonais da tabela e retorna se há vitória.
int verificarDiagonais(char tabela[][TAM_TABELA]){
    int vitoria = 1;
    // Diagonal principal.
    for(int i = 1; i < TAM_TABELA; i++){
        if(tabela[0][0] != tabela[i][i] || (tabela [0][0] != 'X' && tabela [0][0] != 'O')){
            // Para cada inconsistência, cancela a vitória.
            vitoria = 0;
        }
    }
    // Caso contrário, retorna a vitória (1).
    if(vitoria == 1){
        return 1;
    }
    // Reinicializa a variável e verifica a diagonal secundária.
    vitoria = 1;
    for(int i = 1; i < TAM_TABELA; i++){
        if(tabela[0][TAM_TABELA - 1] != tabela[i][TAM_TABELA - 1 - i] || (tabela [0][TAM_TABELA - 1] != 'X' && tabela [0][TAM_TABELA - 1] != 'O')){
            // Para cada inconsistência, cancela a vitória.
            vitoria = 0;
        }
    }
    // Caso contrário, retorna a vitória (1).
    if(vitoria == 1){
        return 1;
    }
    // Se não foi constatada vitória.
    return 0;
}
//--------------------------------------------------------------------------------
// Simplifica a leitura da partida;
// Imprime o vencedor com a cor definida ou declara empate.
void declararVencedor(char vencedor, Jogador jogadores[]){
    if(vencedor == 'X'){
        printf("\n\nO vencedor foi: " COR_X "%s - %c" RESET "!", jogadores[0].nome, vencedor);
    }
    else if(vencedor == 'O'){
        printf("\n\nO vencedor foi: " COR_O "%s - %c" RESET "!", jogadores[1].nome, vencedor);
    }
    else{
        printf("\n\nHouve um empate!");
    }
}
//--------------------------------------------------------------------------------
// Atualiza a pontuação dos dois jogadores no placa, novos ou cadastrados;
// Utiliza os índices para encontrar imediatamente a posição correta.
void atualizarPlacar(Jogador placar[], Jogador jogadores[2], int indices[]){
    for(int i = 0; i < 2; i++){
        placar[indices[i]] = jogadores[i];
    }
}
//--------------------------------------------------------------------------------