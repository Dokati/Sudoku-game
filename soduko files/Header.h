#ifndef PROJ_S
#define PROJ_S
//316411719
//318867272
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include <time.h>
#include <math.h>

#define FINISH_FAILURE 1
#define SUCCESS_FINISH 2
#define NOT_FINISH 3
#define FILLED 4
#define FAIL 5
#define FALSE 0
#define TRUE 1
#define SUDOKO_SIZE 81
#define EMPTY_CELL -1
#define ROW 9
#define COL 9
#define PLAYER_NAME 100

typedef struct _Array
{
    short* arr;
    unsigned short size;
} Array;

typedef struct player
{
    char Name[PLAYER_NAME];
    short board[9][9];
    Array*** possible_solutions;

}PLAYER;


typedef struct playernode
{
    PLAYER data;
    struct playernode* next;
    struct playernode* prev;
}PlayerNode;

typedef struct playerlist
{
    PlayerNode* head;
    PlayerNode* tail;
}PlayerList;

typedef struct Tnode
{
    PlayerNode** data;
    struct Tnode* right;
    struct Tnode* left;
}TreeNode;

typedef struct tree
{
    TreeNode* root;

}Tree;

typedef struct CELL
{
    int row;
    int col;

}CELL;

typedef struct Node {
    CELL* data;
    struct Node* next;
}Node;

typedef struct list {
    Node* head;
    Node* tail;
}List;

void initialBoard(short board[][COL]);
Array*** PossibleDigits(short sudokuBoard[][9]);
Array* Findpos(short sudokuBoard[][9], int row, int coll);
void findRow(short sudokuBoard[][9], int row, int countarr[]);
void findCol(short sudokuBoard[][9], int coll, int countarr[]);
void findCube(short sudokuBoard[][9], int row, int coll, int countarr[]);
int startPoint(int row, int coll, int* collStart);
int OneStage(short board[][9], Array*** possibilities, int* x, int* y);
int checkDuplications(short board[][9]);
void updatePos(short board[][9], Array*** possibilities, int num, int row, int col);
bool checkInPosArr(int num, short* arr, int size);
short* removeCell(short* arr, int size, int num);
int checkFailure(short sudokuBoard[][9], Array*** possibilities);
int minCell(short sudokuBoard[][9], Array*** possibilities, int* y);
int FillBoard(short board[][9], Array*** possibilities);
int DupInRow(short board[][9], int num, int row);
int DupInColl(short board[][9], int num, int coll);
int DupInCell(short board[][9], int num, int row, int coll);
void sudokoPrintBoard(short board[][9]);
List getList();
void insertDataToEndList(List* lst, int i, int j);
Node* createNewNode(CELL* q);
void makeEmptyList(List* res);
int isEmptyList(List* lst);
Node* createNewListNode(int row, int col, Node* next);
void insertNodeToEndList(List* lst, Node* tail);
void freeList(List* lst);
void makeBoard(short sudoko_board[][9], List* cellsList, Array*** possibilities);
Node* FindCellToRandom(List* lst, int i);
PlayerList makePlayersList(int* size);
void makeEmptyPlayerList(PlayerList* lst);
bool isEmptyPlayerList(PlayerList* lst);
void insertDataToEndPlayerList(PlayerList* lst, PLAYER data);
PlayerNode* createNewPlayerListNode(PLAYER data, PlayerNode* next, PlayerNode* prev);
void insertNodeToEndPlayerList(PlayerList* lst, PlayerNode* tail);
PlayerNode** playerNodePArr(PlayerList lst, int lstsize);
int countFilledCells(short board[][9]);
void sortPlayerNodeArr(PlayerNode** arr, int size);
void mergeSort(PlayerNode** arr, int size);
PlayerNode** merge(PlayerNode** a1, int size1, PlayerNode** a2, int size2);
void copyArr(PlayerNode** dest, PlayerNode** src, int size);
PlayerNode** increaseSize(PlayerNode** arr, int size, int* retSize);
Tree buildTreeFromPointersArray(PlayerNode** arr, int size);
TreeNode* BuildTreeFromArrayHelper(PlayerNode** arr, int  size);
TreeNode* createNewTNode(PlayerNode** data, TreeNode* left, TreeNode* right);
void gameRunner(Tree tr, PlayerList* players, PlayerList* winners);
void gameRunnerHelper(TreeNode* root, PlayerList* players, PlayerList* winners);
void deleteFromPlayersList(PlayerList* players, TreeNode* root, int isWinnerList);
void printWinners(PlayerList* winners);
void myFree(PlayerList* winners, Tree* tr, PlayerNode** arrPointers);
void freePossibilities(Array*** poss);
void freePlayerList(PlayerList* players);
void freeTreeHelper(TreeNode* root);
void checkMemoryAllocation(void* ptr);
void playGame();

#endif
