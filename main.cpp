//------------------------INCLUDE------------------------//
#include <iostream>
//-----------------------NAMESPACE-----------------------//
using namespace std;
//------------------------DEFINE------------------------//
#define MEMORY_ERROR 2
#define FILE_ERROR 3
#define SCAN_ERROR 4
#define path R"(...\queens\number.txt)"
#define output R"(...\queens\output.txt)"
//-------------------ERROR--FUNCTIONS-------------------//
void memoryError(){
    std::cerr<<"Memory error\n";
    exit(MEMORY_ERROR);
}
void scanError(){
    std::cerr<<"Scan error\n";
    exit(SCAN_ERROR);
}
void fileError(){
    std::cerr<<"File error\n";
    exit(FILE_ERROR);
}
//------------------------STRUCT------------------------//
struct board{
    int* column;
    int numberOfSpareSquares;
};
//------------------------------------------------------//
void recount(board* board, const int size){
    for (int i = 0; i < size; i++){
        int count = 0;
        for (int j = 0; j < size; j++) {
            if(board[i].column[j] == 0) count++;
        }
        board[i].numberOfSpareSquares = count;
    }
}
void setOrRemoveQueen(const int size, const int x, const int y, board* board,
                      bool mode){
    int a = mode ? 1 : -1;
    for (int i = 0; i < x; i++){
        board[i].column[y] += a;
    }
    for (int i = x + 1; i < size; i++){
        board[i].column[y] += a;
    }
    for (int i = 0; i < y; i++){
        board[x].column[i] += a;
    }
    for (int i = y + 1; i < size; i++){
        board[x].column[i] += a;
    }
    int i = x + 1, j = y + 1;
    while (i < size && j < size){
        board[i].column[j] += a;
        i++; j++;
    }
    i = x - 1, j = y - 1;
    while (i >= 0 && j >= 0){
        board[i].column[j] += a;
        i--; j--;
    }
    i = x + 1, j = y - 1;
    while (i < size && j >= 0){
        board[i].column[j] += a;
        i++; j--;
    }
    i = x - 1, j = y + 1;
    while (i >= 0 && j < size){
        board[i].column[j] += a;
        i--; j++;
    }
    board[x].column[y] += a;
    recount(board, size);
}
void reset(board* board, const int size){
    for(int i = 0; i < size; i++){
        for (int j = 0; j < size ; j++){
            board[i].column[j] = false;
        }
        board[i].numberOfSpareSquares = size;
    }
}
bool doesCapture(const int x, const int y, const board* board){
    if (board[x].column[y]) return true;
    return false;
}
int min(const board* board, const int size){
    int min = INT_MAX;
    int index = -1;
    for (int i = 0; i < size; i++) {
        if (board[i].numberOfSpareSquares < min &&
            board[i].numberOfSpareSquares!= 0) {
            min = board[i].numberOfSpareSquares;
            index = i;
        }
    }
    return index;
}
void setAllQueens(const int size, board* board, int* queen){
    bool initialized = true;
    for (int i = 0; i < size; ++i) if (queen[i] == -1) initialized = false;
    int minColumn = min(board, size);
    if (minColumn == -1 && initialized){
        FILE *file;
        if( (file = fopen( output, "w" )) == nullptr ) fileError();
        for (int i = 0; i < size; i++){
            fprintf(file, "%i%c", i, ' ');
            fprintf(file, "%i%c", queen[i], '\n');
        }
        exit(0);
    } else{
        if(minColumn != -1) {
            for (int i = 0; i < size; i++) {
                if (!doesCapture(minColumn, i, board)) {
                    queen[minColumn] = i;
                    setOrRemoveQueen(size, minColumn, i, board, true);
                    setAllQueens(size, board, queen);
                    queen[minColumn] = -1;
                    setOrRemoveQueen(size, minColumn, i, board, false);
                }
            }
        }
    }
}
void resetArray(int* array, int length){
    for (int i = 0; i < length; i++) {
        array[i] = -1;
    }
}
int main() {
    FILE *file;
    if( (file = fopen( path, "r" )) == nullptr ) fileError();
    int size;
    if(fscanf(file, "%i", &size) == EOF) scanError();
    board board[size];
    for (int i = 0; i < size; i++) {
        board[i].column = (int*)malloc(size * sizeof (int));
        if (!board[i].column) memoryError();
    }
    reset(board, size);
    int queens[size];
    resetArray(queens, size);
    setAllQueens(size, board, queens);
    return 0;
}
