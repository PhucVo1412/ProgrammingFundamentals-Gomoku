#include <iostream>
#include <cstdlib>
#include <iomanip>

using namespace std;

#define MAX_SIZE 15

const char HORZ = 196;
const char VERT = 179;
const char TL   = 218;
const char TM   = 194;
const char TR   = 191;
const char BL   = 192;
const char BM   = 193;
const char BR   = 217;
const int numW  = 3;
const int charW = 1;
const int numPerLine = 15;
const char VERT_START = 195;
const char VERT_END = 180;
const char VERT_MID = 197;
const string inputCommand = "Previous move/Next move/Stop [p/n/s]: ";
const string endOfHistory = "This is the end of the match.\nPrevious move/Next move/Stop [p/n/s]: ";
const string startOfGame = "This is the start of the match.\nPrevious move/Next move/Stop [p/n/s]: ";
const string invalidInput = "Illegal input, please try again: ";

enum Stone {NA, X, O};

string history_state = "ff";

string lineString( char left, char mid, char right, char horz, int wcell, int ncell )
{
    string result = "", cell = string( wcell, horz );
    result += left;
    for ( int i = 0; i < ncell - 1; i++ ) result += cell + mid;
    result += cell + right;
    return result;
}

void displayBoard(Stone arr[][MAX_SIZE], int size) {
    //UPPERLINE
    cout << "   ";
    for(int i = 0; i < size; i++) {
        cout << "  " << (char)(i + 'a') << " ";
    }
    cout << endl;
    cout << "   " << lineString( TL, TM, TR, HORZ, 3, numPerLine ) << "\n";

    //MIDDLE
    for(int i = 0; i < size; i++) {
        cout << setw(2) << size - i << " " << VERT;
        for(int j = 0; j < size; j++) {
            if(arr[i][j] == NA) cout << "   " << VERT;
            if(arr[i][j] == X) cout << " X " << VERT;
            if(arr[i][j] == O) cout << " O " << VERT;
        }
        cout << endl;
        if(i != size - 1) cout << "   " << lineString( VERT_START, VERT_MID, VERT_END, HORZ, 3, numPerLine ) << "\n";
    }

    //UNDERLINE
    cout << "   " << lineString( BL, BM, BR, HORZ, 3, numPerLine ) << "\n";
}

void displayBoardSimple(Stone arr[][MAX_SIZE], int size) {
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++) {
            cout << arr[i][j];
        }
    }
    cout << endl;
}

bool makeMove(Stone board[][MAX_SIZE], int size, string playerMove, bool isFirstPlayerTurn){ //quite safe
    //BEGIN TODO
    if (playerMove == "") return false; // ""
    if (playerMove[0] < '0' || playerMove[0] > '9') return false; // a13
    if (playerMove[0] < '0' || playerMove[0] > '9' || !isalpha(playerMove.back())) return false; //1a^

    int row = 0, col = 0;
    string row1 ="";
    char col1 ;
    for (int i = 0; i < playerMove.length();++i){
        if (playerMove[i]>='0' && playerMove[i]<='9') row1 = row1 + playerMove[i];
        else {
            col1 = playerMove[i];
            if (i != playerMove.length() - 1) return false; //13aa, 1a3a
            break;
        }
    }

    //if (col1 >= 'A' && col1 <= 'Z') col1 = col1 + ('a' - 'A');

    int n = 0;
    /*while (n != row1.length()){
        row = row * 10 + (row1[n++] - '0');
    }*/

    row = stoi(row1);
    if (row >MAX_SIZE || row <1) return false;
    if (col1 <'a' || col1 > 'o') return false;

    int rowindex = MAX_SIZE - row;
    int colindex = col1 - 'a';
    switch (board[rowindex][colindex]) {
        case NA:
            board[rowindex][colindex] = isFirstPlayerTurn ? X : O;
            history_state = history_state + " " + playerMove  ;
            return true;;
        case X:
        case O:
            return false;
    }
    return false;
    //END TODO
}
bool makeRemove(Stone board[][MAX_SIZE], int size, string playerMove, bool isFirstPlayerTurn){ //ff
    if (playerMove.empty()) return false;
    if (playerMove[0] < '0' || playerMove[0] > '9' || !isalpha(playerMove.back())) return false;

    int row = 0, col = 0;
    string row1 ="";
    char col1 ;
    for (int i = 0; i < playerMove.length();++i){
        if (playerMove[i]>='0' && playerMove[i]<='9') row1 = row1 + playerMove[i];
        else {col1 = playerMove[i]; break;}
    }


    int n = 0;
    /*while (n != row1.length()){
        row = row * 10 + (row1[n++] - '0');
    }*/
    row = stoi(row1);

    if (row >MAX_SIZE || row <1) return false;
    if (col1 <'a' || col1 > 'o') return false;

    if (board[MAX_SIZE-row][col1 - 'a'] != NA) board[MAX_SIZE-row][col1 - 'a'] = NA;
    else return false;

    return true;
}


bool checkwinning(Stone board[][MAX_SIZE],string cor, Stone currStone){      // ff
    int row = 0, col = 0;
    string row1 ="";
    char col1 ;
    for (int i = 0; i < cor.length();++i){
        if (cor[i]>='0' && cor[i]<='9') row1 = row1 + cor[i];
        else {col1 = cor[i]; break;}
    }
    int n = 0;
    while (n != row1.length()){
        row = row * 10 + (row1[n++] - '0');
    }
    row = MAX_SIZE - row;
    col = col1 - 'a';

    int consecutive = 1;
    int head = col, tail = col;
    // check horizontal
    for (int j = col-1; j >=0 ; --j){
        if (board[row][j] ==  currStone) {
            consecutive ++;
            head = j;
        }
        else if (board[row][j] !=  currStone) break;

    }
    for (int j = col+1; j <= MAX_SIZE - 1 ; ++j){
        if (board[row][j] ==  currStone) {
            consecutive ++;
            tail = j;
        }
        else if (board[row][j] !=  currStone) break;

    }
    if (consecutive == 5){
        if ((head > 0 && board[row][head - 1] == NA) ||
            (tail < MAX_SIZE - 1 && board[row][tail + 1] == NA)) {
            return true;
        }
        else if (head == 0
            //&& tail <MAX_SIZE - 1 && board[row][tail + 1] == NA
                )   return true;

        else if (tail == MAX_SIZE - 1
            //&& head > 0 && board[row][head - 1] == NA
                )  return true;

    }

    // check vertical
    consecutive = 1;
    head = row ; tail = row;

    for (int i = row-1; i >=0  ; --i){
        if (board[i][col] ==  currStone) {
            consecutive ++;
            head = i;
        }
        else if (board[i][col] !=  currStone) break;

    }
    for (int i = row+1; i <= MAX_SIZE - 1 ; ++i){
        if (board[i][col] ==  currStone) {
            consecutive ++;
            tail = i;
        }
        else if (board[i][col] !=  currStone) break;
    }

    if (consecutive == 5){
        if ((head > 0 && board[head - 1][col] == NA) ||
            (tail < MAX_SIZE - 1 && board[tail + 1][col] == NA)) {
            return true;
        }
        else if (head == 0
            //&& tail < MAX_SIZE - 1 && board[tail + 1][col] == NA
                )   return true;
        else if (tail == MAX_SIZE - 1
            //&& head > 0 && board[head - 1][col]  == NA
                )  return true;
    }

    // check diagonally dấu huyền

    consecutive = 1;
    int rowhead = row, colhead = col;
    int rowtail = row, coltail = col;
    for (int i = row-1,  j = col -1 ; i >=0 && j>=0 ; --i,--j){
        if (board[i][j] ==  currStone) {
            consecutive ++;
            rowhead = i;
            colhead = j;
        }
        else if (board[i][j] !=  currStone) break;

    }
    for (int i = row + 1, j = col + 1; i <= MAX_SIZE - 1 && j <= MAX_SIZE - 1 ; ++i, ++j){
        if (board[i][j] ==  currStone) {
            consecutive ++;
            rowtail = i;
            coltail = j;
        }
        else if (board[i][j] !=  currStone) break;
    }

    if (consecutive == 5){
        if ((rowhead > 0 && colhead > 0 && board[rowhead - 1][colhead - 1] == NA) ||
            (rowtail < MAX_SIZE - 1 && coltail < MAX_SIZE - 1 && board[rowtail + 1][coltail + 1] == NA)) {
            return true;
        }
        else if ((rowhead == 0 || colhead == 0)
            //&& board[rowtail + 1][coltail + 1] == NA
                ) return true;
        else if ((rowtail == MAX_SIZE-1 || coltail == MAX_SIZE - 1)
            //&& board[rowhead - 1][colhead - 1] == NA
                ) return true;

    }
    // check diagonally dấu sắc
    consecutive = 1;
    rowhead = row; colhead = col;
    rowtail = row; coltail = col;

    for (int i = row - 1, j = col + 1; i >= 0 && j <= MAX_SIZE - 1 ; --i, ++j) {
        if (board[i][j] == currStone) {
            consecutive++;
            rowhead = i;
            colhead = j;
        }
        else break;
        ;
    }

    for (int i = row + 1, j = col - 1; i <= MAX_SIZE - 1 && j >= 0 ; ++i, --j) {
        if (board[i][j] == currStone) {
            consecutive++;
            rowtail = i;
            coltail = j;
        }
        else break;
    }

    if (consecutive == 5) {
        if ((rowhead > 0 && colhead < MAX_SIZE - 1 && board[rowhead - 1][colhead + 1] == NA) ||
            (rowtail < MAX_SIZE - 1 && coltail > 0 && board[rowtail + 1][coltail - 1] == NA)) {
            return true;
        }
        else if ((rowhead == 0 || colhead == MAX_SIZE - 1)
            //&& board[rowtail + 1][coltail - 1] == NA
                ) return true;
        else if ((rowtail == MAX_SIZE-1 || coltail == 0)
            //&& board[rowhead - 1][colhead + 1] == NA
                ) return true;
    }


    return false;
}

bool hasWon(Stone board[][MAX_SIZE], int size, bool isFirstPlayerTurn) { //quite safe
    //BEGIN TODO
    int n = history_state.length()-1;
    string cor = "";
    while (history_state[n] != ' ') cor = history_state[n--] + cor;
    Stone CurrStone = isFirstPlayerTurn? X:O ;
    return checkwinning(board,cor,CurrStone);
    //END TODO
}

void displayHistory(string history, int numOfMoves) { //quite safe
    //BEGIN TODO
    Stone game[MAX_SIZE][MAX_SIZE]={{NA}};
    for(int i = 0; i < MAX_SIZE; i++) {
        for(int j = 0; j < MAX_SIZE; j++) {
            game[i][j] = NA;
        }
    }
    displayBoard(game, MAX_SIZE);
    string *cor = new string[numOfMoves];
    for (int i = 0; i<numOfMoves;++i) cor[i] = "";
    string history2 = "";
    for (auto x : history) {
        if (x >= 'a' && x <= 'o') history2 = history2 + x + " " ;
        else history2 = history2 + x;
    }
    int n=0;
    for (auto x : history2){
        if (x != ' ') cor[n] = cor[n] + x;
        else n++;
    }

    //for (int i =0; i<numOfMoves;++i) cout<<cor[i]<<" ";
    n = 0;
    char c;
    bool isFirstPlayerTurn = true;
    cout<<inputCommand;
    while (cin>>c){
        if (c == 's') break;
        else if (c == 'n'){
            if (n >= numOfMoves ) {
                cout<<endOfHistory;
                continue;
            }
            if (cor[n] =="ff"){
                cout<<endOfHistory;
                continue;
            }
            makeMove(game, MAX_SIZE, cor[n], isFirstPlayerTurn);
            isFirstPlayerTurn = !isFirstPlayerTurn;
            n++;
            displayBoard(game, MAX_SIZE);
            cin.ignore();
            cout<<inputCommand;
        }
        else if (c == 'p'){
            if (n==0) {
                cout<<startOfGame;
                continue;
            }
            n--;
            makeRemove(game, MAX_SIZE, cor[n], isFirstPlayerTurn);
            isFirstPlayerTurn = !isFirstPlayerTurn;
            displayBoard(game, MAX_SIZE);
            cin.ignore();
            cout<<inputCommand;
        }
        else {
            cin.ignore();
            cout<<invalidInput;
        }

    }
    delete []cor;
    return ;
    //END TODO
}

void startGame() {
    Stone game[15][15];
    for(int i = 0; i < MAX_SIZE; i++) {
        for(int j = 0; j < MAX_SIZE; j++) {
            game[i][j] = NA;
        }
    }
    displayBoard(game, MAX_SIZE);
    string playerMove;
    bool player1Turn = true;
    cout << "Player 1 turn: ";
    while(cin >> playerMove) {
        if(playerMove == "ff") {
            cout << ((player1Turn) ? "Player 2 won " : "Player 1 won ");
            break;
        }
        else if(makeMove(game, MAX_SIZE, playerMove, player1Turn)){
            if(hasWon(game, MAX_SIZE, player1Turn)) {
                displayBoard(game, MAX_SIZE);
                cout << ((player1Turn) ? "Player 1 won " : "Player 2 won ");
                return;
            }
            player1Turn = !player1Turn;
            displayBoard(game, MAX_SIZE);
            cout << ((player1Turn) ? "Player 1 turn: " : "Player 2 turn: ");
        } else {
            cout << "Illegal move, please try again: ";
        }
    }
}

int main()
{
    cout << "Welcome to Gomoku!" << endl;
    cout << "1. Play game" << endl;
    cout << "2. History" << endl;
    cout << "3. Exit" << endl;
    cout << "Please select mode [1/2/3]: ";
    int mode;
    while(cin >> mode){
        if(mode == 1) {
            startGame();
            return 0;
        } else if (mode == 2) {
            int numOfMoves;
            cout << "Please enter number of moves: ";
            cin >> numOfMoves;
            string history;
            cout << "Please enter history: ";
            cin >> history;
            displayHistory(history, numOfMoves);
            return 0;
        } else if (mode == 3) {
            return 0;
        } else {
            cout << "Invalid mode, please try again: ";
        }
    }


    return 0;
}
