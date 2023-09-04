/*func print_arq --> local para printar arquivos e alocar dinamicamente um vetor img dependendo do arq recebido
  func escrita --> local onde acontece o printf dos vetores que contem a historia do game, utilizando um laço de repeticao para
                    escrever caracter por caracter com um sleep e um beep;
  func open --> local exclusivo para abertura e verificao de arquivos;
  func inicio --> menu inicial do jogo, pedindo para o usuario se ele quer jogar ou fechar a aplicacao;
  func hero --> local onde é coletado as informacoes do usuario e aplicado na struct heroi;
  func dungeon --> local aonde é escolhido aleatoriamente um arquivo monster e coletado as informaçoes desse arquivo para mudar as
                    variaveis gold,vida do usuario;
  func loja --> local aonde as variaveis gold,vida,poder sao alteradas conforme as escolhas do usuario;
  func status --> local aonde o usuario pode verificar como estao suas variaveis vida,gold,poder;
  func fim --> local aonde o programa sera encerrado dependendo das acoes do usuario;
  func chefe --> laço de repeticao usuando as variaveis vida,poder até quem a vida do usuario ou a vida_boss se torne <= 0;
  func main --> local principal do codigo, aonde possui o menu principal do jogo juntamento com o chamamento das outras funcs conforme 
                escolha do usuario; */

#include<stdio.h>
#include<Windows.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<conio.h>
struct heroi{
    char nome[50];
    char classe[50];
    int idade;
};
int delay=1000; //delay da funcao sleep

void print_arq(FILE *nome_arquivo){
    char *img;
    int tam_img;

    fscanf(nome_arquivo,"%d",&tam_img);
    img = (char*) malloc(tam_img*sizeof(char)); //malloc img
    if(img == NULL){ //verifica se a memoria foi alocada corretamente
        printf("Erro ao Alocar a memoria.Fim do programa\n");
        exit(0);
    }
    while(fgets(img,tam_img,nome_arquivo)!= NULL){
        printf("%s",img);
    }
    free(img);
    return;
}
void escrita(char escrevendo[]){
    for(int i=0; escrevendo[i] != '\0'; i++){
        printf("%c",escrevendo[i]);
        Beep(550,30);
        Sleep(40);
    }
}
FILE *open(char nome_arquivo[]){
    FILE *abertura = fopen(nome_arquivo,"r");
    if(abertura==NULL){
        printf("Erro ao Abrir o programa.Fim do programa\n");
        exit(0);
    }
    return abertura;
}
void inicio(){
    int acao;
    char iniciar[]="\n1-Iniciar\n2-Sair\nOque deseja Fazer: ";

    while(1){
        FILE *inicial = open("inicio.txt");
        print_arq(inicial);
        fclose(inicial);
        Sleep(delay),escrita(iniciar);
        scanf("%d",&acao);
        switch(acao){
            case 1: return;
            case 2: exit(0); //encerra o programa
            default: system("cls"); //limpa o terminal
        }
    }
    return;
}
void hero(struct heroi* h){
    char nome_heroi[]="Qual seu nome heroi: ",classe_heroi[]="Qual sua Classe heroi: ",idade_heroi[]="Qual sua idade heroi: ";
    char historia[]="inicia sua jornada de heroi em busca de riquezas e poder. Mas sua jornada sera dificil e longa e precisara derrotar o Chefe dos Monstros...\nQue a Jornada....Comece...";
    system("cls");
    Sleep(delay),escrita(nome_heroi);
    //scanf's nas variaveis da struct
    scanf(" %[^\n]",h->nome);
    Sleep(delay),escrita(classe_heroi);
    scanf(" %[^\n]",h->classe);
    Sleep(delay),escrita(idade_heroi);
    scanf("%d",&h->idade);
    Sleep(delay),system("cls");
    printf("%s,o(a) %s ",h->nome,h->classe);
    escrita(historia);
    system("pause");
    return;
}
void dungeon(int *vida,int *gold){
    int aleat_monster,reward_monster,qtde_monster;
    char monster_choice[10],tipo_monstro[10];

    FILE *name_monster = open("nomes_monstros.txt");
    fscanf(name_monster,"%d",&qtde_monster);
    srand(time(NULL));
    aleat_monster = 1 + rand() % qtde_monster; //sorteia um numero aleatorio 
    for(int i=0;i<aleat_monster;i++){
        fscanf(name_monster,"%s",monster_choice); //escolhe um arquivo aleatorio dentro de nomes_monstro.txt
    }
    fclose(name_monster);
    FILE *monstro = open(monster_choice); //aplica o arquivo escolhido no FILE monster
    //fscanf do arquivo .txt escolhido em variaveis declaradas
    fscanf(monstro,"%s",tipo_monstro);
    fscanf(monstro,"%d",&reward_monster);
    system("cls");
    print_arq(monstro);
    Sleep(delay),printf("\nVoce encontrou um %s...\n", tipo_monstro);
    Sleep(delay),printf("+%d de gold...-%d de vida....Voce retorna para casa...\n",reward_monster,reward_monster);
    //modifica os conteudos dos ponteiros declarados
    *vida=*vida-reward_monster;
    *gold=*gold+reward_monster;
    fclose(monstro);
    system("pause");
    return;
}
void loja(int *vida,int *gold,int *poder){
    char loja_escrita[]="\n1-Comprar Item\n2-Sair\nOque desejar Fazer: ",loja_escrita2[]="\nOque desejar comprar: ",img[1000];
    int acao,acao_2,custo,poder_loja,vida_loja;
    system("cls");
    FILE *tabela = open("tabela.txt");
    print_arq(tabela);
    fclose(tabela);
    while(1){
        Sleep(delay),escrita(loja_escrita);
        scanf("%d",&acao);
        switch(acao){
        case 1:
            escrita(loja_escrita2);
            scanf("%d",&acao_2);
            if(acao_2 <1 || acao_2 > 4){ //verifica se o usuario inseriu um codg de compra valido
                Sleep(delay),printf("Voce ficou indeciso do que comprar...Voltando para Casa...\n");
                system("pause");
                return;
            }
            FILE *preco = open("preco_tabela.txt");
            for(int i=0;i<acao_2;i++){
                fscanf(preco,"%d %d %d",&custo,&poder_loja,&vida_loja);
            }
            fclose(preco);
            if(*gold>=custo){ //verificacao para saber se o jogador possui gold suficiente para comprar o item
                *gold=*gold-custo,*poder=*poder+poder_loja,*vida=*vida+vida_loja;
                Sleep(delay),printf("Compra realizada +%d Poder... +%d Vida... -%d Gold...\n",poder_loja,vida_loja,custo);
                system("pause");
                return;
            }else{
                Sleep(delay),printf("Gold insuficiente... Voltando para casa...\n");
                system("pause");
                return;
            }
        case 2: return;
        default:;
        }
    }
}
void status(int vida,int gold,int poder,struct heroi* h){
    system("Cls");
    Sleep(delay),printf("Nome: %s | Classe: %s | Idade: %d\n",h->nome,h->classe,h->idade);
    Sleep(delay),printf("Vida: %d",vida);
    Sleep(delay),printf("\nGold: %d",gold);
    Sleep(delay),printf("\nPoder: %d\n",poder);
    system("pause");
    return;

}
void fim(int x){
    char conclusao[][70]={"O heroi nao aguenta a dura jorna e rende-se ao Chefe dos Monstros...","O heroi morre na tentativa de se tornar uma lenda...","O heroi derrota o Chefe dos monstros e se torna uma lenda..."};
        system("cls");
        for(int i=0;conclusao[x][i]!= '\0';i++){ //executa a linha da matriz conclusao conferme o valor de x
            printf("%c",conclusao[x][i]);
            Beep(550,30);
            Sleep(40);
        }
    system("pause");
    system("cls");
    FILE *final = open("final.txt");
    print_arq(final);
    fclose(final);
    exit(0); //encerra o programa
}
void chefe(int *vida,int *poder){
    FILE *Boss;
    char comeco_luta[]="O heroi parte bravamente para enfrentar seu ultimo desafio...O Chefe aparace em frente de seus olhos...Boa sorte...";
    int vida_boss=200,poder_boss=20;
    system("cls");
    escrita(comeco_luta),Sleep(delay);
    system("cls");
    Boss = open("boss.txt");
    print_arq(Boss);
    Sleep(delay),printf("\nVida Boss: %d\nSua vida: %d",vida_boss,*vida);
    while(1){
        //serie de comandos ate alguma das variaveis vida ser <= 0
        vida_boss=vida_boss-*poder;
        Sleep(delay),printf("\nVoce golpeia...Vida boss: %d",vida_boss);
        if(vida_boss<=0){
            system("pause");
            fim(2);
        }
        *vida=*vida-poder_boss;
        Sleep(delay),printf("\nO chefe ataca voce...Sua vida: %d",*vida);
        if(*vida<=0){
            system("pause");
            fim(1);
        }
    }

}
int main(){
    char pergunta[]="Oque deseja fazer: ";
    int acao,vida=100,gold=0,poder=10;
    struct heroi h;
    inicio();
    hero(&h);
    while(1){
        if(vida<=0){ //sempre verifica se a vida nao esta <= 0
            fim(1);
        }
        system("cls");
        FILE *menu = open("menu_p.txt");
        print_arq(menu);
        Sleep(delay),escrita(pergunta);
        scanf("%d",&acao);
        switch(acao){
            case 1: dungeon(&vida,&gold);break;
            case 2: loja(&vida,&gold,&poder);break;
            case 3: status(vida,gold,poder,&h);break;
            case 4: chefe(&vida,&poder);
            case 5: fim(0);
            default:;
        }
    }
}
