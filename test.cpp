#include <fstream>
#include "gomoku.h"

using namespace std;
int main(int argc, char* argv[])
{
    int numofMoves;
    string history;
    cout << "Welcome to Gomoku!" << endl;
    cout << "1. Play game" << endl;
    cout << "2. History" << endl;
    cout << "3. Exit" << endl;
    cout << "Please select mode [1/2/3]: ";
    string *input = new string[1000];
    char *input2 = new char [1000];
    int size = 0;
    ifstream file;
    int mode;
    file.open(argv[1]);
    string s;
    char temp;
    file>>mode;
    if (mode == 2){
        file>>numofMoves;
        file>>history;

        int size1 = 0;
        while (file >> temp) input2[size++] = temp;


    }
    //for (int i =0; i<size;i++) cout<<input2[i]<<" ";
    while(file>>s) {
        input[size++]=s  ;
    }
    cout<<mode<<endl;


    if (mode == 1) {
        Stone game[15][15];
        for(int i = 0; i < MAX_SIZE; i++) {
            for(int j = 0; j < MAX_SIZE; j++) {
                game[i][j] = NA;
            }
        }
        displayBoard(game, MAX_SIZE);

        string playerMove ;
        bool player1Turn = true;
        int n = 0;
        while(true) {
            playerMove = input[n++];
            cout << ((player1Turn) ? "Player 1 turn: " : "Player 2 turn: ") <<playerMove<<endl;

            if(playerMove == "ff") {
                cout << ((player1Turn) ? "Player 2 won " : "Player 1 won ");
                break;
            }
            else if(makeMove(game, MAX_SIZE, playerMove, player1Turn)){
                if(hasWon(game, MAX_SIZE, player1Turn)) {
                    displayBoard(game, MAX_SIZE);
                    cout << ((player1Turn) ? "Player 1 won " : "Player 2 won ");
                    return 0;
                }
                player1Turn = !player1Turn;
                displayBoard(game, MAX_SIZE);

            } else {
                cout << "Illegal move, please try again: "<<endl;
            }
            if (n==size) break;
        }
    }
    else if (mode == 2) {
        cout << "Please enter number of moves: "<<endl;
        cout << "Please enter history: ";
        Stone board[15][15]={{NA}};
        for(int i = 0; i < MAX_SIZE; i++) {
            for(int j = 0; j < MAX_SIZE; j++) {
                board[i][j] = NA;
            }
        }
        displayBoard(board, MAX_SIZE);
        string *cor = new string[numofMoves];
        for (int i = 0; i<numofMoves;++i) cor[i] = "";
        string history2 = "";
        for (auto x : history) {
            if (x < '0' || x > '9') history2 = history2 + x + " " ;
            else history2 = history2 + x;
        }
        int n=0;
        for (auto x : history2){
            if (x!=' ') cor[n] += x;
            else n++;
        }

        //for (int i =0; i<numofMoves;++i) cout<<cor[i]<<" ";
        n = 0;
        char c;
        bool isFirstPlayerTurn = true;
        int index = 0;

        cout<<inputCommand;
        while (true){
            c = input2[index++];
            cout<<c <<endl;
            if (index == size) break;
            if (c == 's') break;
            else if (c == 'n'){
                if (n >= numofMoves ) {
                    cout<<endOfHistory;
                    continue;
                }
                Move(board, MAX_SIZE, cor[n], isFirstPlayerTurn,1);
                isFirstPlayerTurn = !isFirstPlayerTurn;
                n++;
                displayBoard(board, MAX_SIZE);
                cout<<inputCommand;
            }
            else if (c == 'p'){
                if (n==0) {
                    cout<<startOfGame;
                    continue;
                }
                n--;
                Move(board, MAX_SIZE, cor[n], isFirstPlayerTurn,10000000);
                isFirstPlayerTurn = !isFirstPlayerTurn;
                displayBoard(board, MAX_SIZE);
                cout<<inputCommand;
            }
            else {
                cout<<invalidInput;
            }

        }
        delete []cor;
    } else if (mode == 3) {
        ;
    }
    file.close();
    delete []input;
    delete []input2;
    return 0;
}
