#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Global Variables/Data Structures
struct Player{
    char name[81];
    char symbol;
} player1,  player2;

char board[6][7];

int result; /*takes 3 states, 0, 1, 2
	      0 for continuing position
	      1 for winning position
	      2 for draw position
	    */

int max_depth; //sets difficulty 3 , 5, 7


//Function Prototypes
void init_board(void);
void display_board(char board[6][7]);
void get_player_move(struct Player);
int check(char board[6][7]);
int minimax(char board[6][7], int choice, int depth, int is_max);
int find_best_move(void);


int main() {
    //system("color 71");
    int mode;
    system("cls");
    struct Player current_player;
    printf("Lets Play Connect 4!!\n");
    printf("Choose the game mode:\n (1) for single player, (2) for double player: ");
    scanf("%d", &mode);
    while(mode<1 || mode>2){ //input validation
	printf("Invalid Input!\n Select mode 1 or 2: ");
	scanf("%d", &mode);
    }


    printf("Enter name for player1 'O': ");
    scanf("%s", player1.name);
    player1.symbol='O';
    player2.symbol='X';

    if(mode==2){
        printf("Enter name for player2 'X': ");
        scanf("%s", player2.name);
    }
    else{
        printf("You are playing against COMPUTER!\nSet your difficulty (1, 2, 3): ");
        int difficulty;
        scanf("%d", &difficulty);
	while(difficulty<1 || difficulty>3){ //input validation
	    printf("Invalid Input!\n Set difficulty within (1,2,3):");
	    scanf("%d", &difficulty);
	}
        switch(difficulty){
            case 1: max_depth=4;
                break;
            case 2: max_depth=7;
                break;
            case 3: max_depth=8;
        }
        strcpy(player2.name, "COMPUTER");
    }

    init_board();
    display_board(board);
    result=0;
    current_player=player2;
    while(!result){
        if(current_player.symbol=='X')
            current_player=player1;
        else current_player=player2;

        if(current_player.symbol=='X' && mode==1){
            printf("Computer is thinking...\n");
            int j=find_best_move();
            int i;
            for(i=1; board[i][j]==' ' && i<6; i++);
            i--;
            board[i][j]='X';
	    system("cls");
            printf("COMPUTER chose %d\n", j+1);
        }
        else get_player_move(current_player);

        display_board(board);
        result=check(board);
    }

    //announce winner or draw;
    if(result==1){
        printf("%s is the WINNER!!\n", current_player.name);
    }
    else
        printf("Its a DRAW!\n");

}





//function definitions
void init_board(void){
    printf("Initialising board...\n");
    for(int i=0; i<6; i++)
        for(int j=0; j<7; j++)
            board[i][j]=' ';
}

void display_board(char board[6][7]){
    printf("\n   1 2 3 4 5 6 7\n"); //printing column index
    for(int i=0; i<6; i++){
        printf("   _ _ _ _ _ _ _\n");
        printf("%d ", i+1); //printing row index
        for(int j=0; j<7; j++){
            printf("|%c", board[i][j]);
        }
        printf("|\n");
    }
    printf("   - - - - - - -\n\n");
}

int check(char board[6][7]){

    //horizontal check
    for(int i=0; i<6; i++)
        for(int j=0; j<4; j++){
            if(board[i][j]!=' ')
                if(board[i][j]==board[i][j+1] &&
                   board[i][j]==board[i][j+2] &&
                   board[i][j]==board[i][j+3]){
                    return 1;
                }
        }

    //vertical check
    for(int j=0; j<7; j++){
        for(int i=0; i<3; i++){
            if(board[i][j]!=' ')
                if(board[i][j]==board[i+1][j] &&
                    board[i][j]==board[i+2][j] &&
                    board[i][j]==board[i+3][j]){
                    return 1;
                }
        }
    }


    //diagonal checks
    for(int i=0; i<3; i++)
        for(int j=0; j<4; j++)
            if(board[i][j]!=' ')
                if(board[i][j]==board[i+1][j+1] &&
                    board[i][j]==board[i+2][j+2] &&
                    board[i][j]==board[i+3][j+3]){
                    return 1;
                }
    for(int i=0; i<3; i++)
        for(int j=6; j>2; j--)
            if(board[i][j]!=' ')
                if(board[i][j]==board[i+1][j-1] &&
                    board[i][j]==board[i+2][j-2] &&
                    board[i][j]==board[i+3][j-3]){
                    return 1;
                }

    //fullcheck
    int is_full=1;
        for(int j=0; j<7; j++){
            if(board[0][j]==' '){
                is_full=0;
                break;
            }
        }

    if(is_full) return 2;

    return 0;
}

void get_player_move(struct Player player){
    int i, j;

    printf("Its %s's(%c) turn\n", player.name, player.symbol);
    printf("Enter column: ");

    //getting j;
    scanf("%d", &j);
    while(j<1 || j>7 || board[0][j-1]!=' '){ //input validation
        printf("chose a valid column\n\t:");
        scanf("%d", &j);
    }

    system("cls");
    printf("%s chose %d\n", player.name, j);

    j--;

    //finding i (starting from i=1 since  i=0 is already empty for the column j);
    for(i=1; board[i][j]==' ' && i<6; i++);

    i--;

    board[i][j]=player.symbol;

}




//----the minimax algorithm----

int minimax(char board[6][7], int choice, int depth, int is_max){
    if(depth>max_depth) return 0;

    //generate altered board
    int i, j;
    //copy board
    char alt_board[6][7];
    for(i=0; i<6; i++)
        for(j=0; j<7; j++)
            alt_board[i][j]=board[i][j];

    char symbol= is_max?'X':'O';

    //finding i;
    j=choice;
    for(i=1; alt_board[i][j]==' ' && i<6; i++);

    i--;

    alt_board[i][j]=symbol;

    //check for terminal state;
    int state= check(alt_board);
    if(state==1){
        if(is_max) return 1;
        else return -1;
    }
    if(state==2){
        return 0;
    }



    //is game continuing?
    if(is_max){
        int best_value=1, current_value;
        for(int j=0; j<7; j++){
            if(alt_board[0][j]!=' ') continue;
            current_value=minimax(alt_board, j, depth+1, 0);
            if(best_value>current_value)
                best_value=current_value;
        }

        return best_value;
    }
    else{
        int best_value=-1, current_value;
        for(int j=0; j<7; j++){
            if(alt_board[0][j]!=' ') continue;
            current_value=minimax(alt_board, j, depth+1, 1);
            if(best_value<current_value)
                best_value=current_value;
        }

        return best_value;
    }
}


int find_best_move(void){
    int best_move=-1;
    int best_value=-2, current_value;
    for(int j=0; j<7; j++){
        if(board[0][j]!=' ') continue;
        current_value=minimax(board, j, 0, 1);
        if(best_value<current_value){
            best_value=current_value;
            best_move=j;
        }
    }

    return best_move;
}
