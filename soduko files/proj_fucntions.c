#include "Header.h"


// this funcntion puts -1 in every cell in the sudoko board
void initialBoard(short board[][COL])
{
    int i, j;
    for (i = 0; i < ROW; i++)
    {
        for (j = 0; j < COL; j++)
        {
            board[i][j] = -1;
        }
    }
}

// this function create the array which contain the possibillities for every cell in the board
Array*** PossibleDigits(short sudokuBoard[][9])
{
    int i, j;
    Array*** possible_solutions;


    possible_solutions = (Array***)malloc(sizeof(Array**) * 9);
    checkMemoryAllocation(possible_solutions);


    for (i = 0; i < 9; i++)
    {
        possible_solutions[i] = (Array**)malloc(sizeof(Array*) * 9);
        checkMemoryAllocation(possible_solutions[i]);



    }

    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            if (sudokuBoard[i][j] == -1)
            {
                possible_solutions[i][j] = Findpos(sudokuBoard, i, j);
            }
            else
            {
                possible_solutions[i][j] = (Array*)malloc(sizeof(Array));
                checkMemoryAllocation(possible_solutions[i][j]);
                (possible_solutions[i][j])->arr = NULL;
                (possible_solutions[i][j])->size = 0;
            }
        }
    }

    return possible_solutions;
}

// create the array of the posibble digits for a cell in the board
Array* Findpos(short sudokuBoard[][9], int row, int coll)
{
    int arrCount[10] = { 0 };
    findRow(sudokuBoard, row, arrCount);
    findCol(sudokuBoard, coll, arrCount);
    findCube(sudokuBoard, row, coll, arrCount);
    int i, j = 0, count = 0;
    Array* res;
    res = (Array*)malloc(sizeof(Array));
    checkMemoryAllocation(res);
    if (res == NULL)
    {
        printf("failed ");
        exit(1);
    }

    // counting the size of the not existing values.
    for (i = 1; i < 10; i++)
    {
        if (arrCount[i] == 0)
        {
            count++;
        }

    }

    res->arr = (short*)malloc(sizeof(short) * count);
    checkMemoryAllocation(res->arr);
    if (res->arr == NULL)
    {
        printf("failed ");
        exit(1);
    }

    //inserting the pos values to res

    for (i = 1; i < 10; i++)
    {
        if (arrCount[i] == 0)
        {
            (res->arr)[j] = (short)i;
            j++;
        }
    }

    res->size = count;
    return res;
}

// these three functions together helps to create the counting sort arr 
void findRow(short sudokuBoard[][9], int row, int countarr[])
{
    int i;

    for (i = 0; i < 9; i++)
    {
        if (sudokuBoard[row][i] != -1)
        {
            countarr[sudokuBoard[row][i]]++;

        }
    }

}
void findCol(short sudokuBoard[][9], int coll, int countarr[])
{
    int i;

    for (i = 0; i < 9; i++)
    {
        if (sudokuBoard[i][coll] != -1)
        {
            countarr[sudokuBoard[i][coll]]++;
        }
    }

}
void findCube(short sudokuBoard[][9], int row, int coll, int countarr[])
{
    int startCol;
    int startRow;
    startRow = startPoint(row, coll, &startCol);
    int i, j;


    for (i = startRow; i <= startRow + 2; i++)
    {
        for (j = startCol; j <= startCol + 2; j++)
        {
            if (sudokuBoard[i][j] != -1)
            {
                countarr[sudokuBoard[i][j]]++;
            }
        }
    }
}
int startPoint(int row, int coll, int* collStart)
{
    if (row >= 0 && row <= 2)
    {
        if (coll >= 0 && coll <= 2)
        {
            *collStart = 0;
            return 0;
        }
        else if (coll >= 3 && coll <= 5)
        {
            *collStart = 3;
            return 0;
        }
        else if (coll >= 6 && coll <= 8)
        {
            *collStart = 6;
            return 0;
        }
    }
    else if (row >= 3 && row <= 5)
    {
        if (coll >= 0 && coll <= 2)
        {
            *collStart = 0;
            return 3;
        }
        else if (coll >= 3 && coll <= 5)
        {
            *collStart = 3;
            return 3;
        }
        else if (coll >= 6 && coll <= 8)
        {
            *collStart = 6;
            return 3;
        }
    }
    if (row >= 6 && row <= 8)
    {
        if (coll >= 0 && coll <= 2)
        {
            *collStart = 0;
            return 6;
        }
        else if (coll >= 3 && coll <= 5)
        {
            *collStart = 3;
            return 6;
        }
        else if (coll >= 6 && coll <= 8)
        {
            *collStart = 6;
            return 6;
        }
    }
}



//the function filling the cell in the board which have only one option digit. 
int OneStage(short board[][9], Array*** possibilities, int* x, int* y)
{
    int i, j, num, count = 0;
    int res;

    while (count < 81)// this while stops when the all the cells in the board have size != 1
    {
        for (i = 0; i < 9; i++)
        {
            for (j = 0; j < 9; j++)
            {
                if (board[i][j] == -1)
                {

                    if (possibilities[i][j]->size == 1)
                    {
                        board[i][j] = (possibilities[i][j]->arr)[0];
                        free(possibilities[i][j]->arr);
                        possibilities[i][j]->arr = NULL;
                        possibilities[i][j]->size = 0;
                        num = board[i][j];
                        updatePos(board, possibilities, num, i, j);
                        count = 0;
                    }

                }
                if ((possibilities[i][j]->size != 1))
                {
                    count++;




                    if (count == 81)
                    {
                        break;
                    }
                }
            }
        }



    }
    *x = minCell(board, possibilities, y);
    res = checkDuplications(board);
    if (res == FINISH_FAILURE)
    {
        return res;
    }
    else
    {
        res = checkFailure(board, possibilities);
        return res;
    }
}

// checks if there are any duplications digits in the same row/coll/cube
int checkDuplications(short board[][9])
{

    int i, j, k = 0;
    int res1, res2, res3;
    int num;

    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            res1 = DupInRow(board, board[i][j], i);
            res2 = DupInColl(board, board[i][j], j);
            res3 = DupInCell(board, board[i][j], ((i / 3) * 3), ((j / 3) * 3));
        }
    }
    if (res1 == 1 || res2 == 1 || res3 == 1)
        return FINISH_FAILURE;
    else
        return 0;
}

// after any insert to the borad, this function updating the possible digit array for every cel in the row/coll/cube.
void updatePos(short board[][9], Array*** possibilities, int num, int row, int col)
{
    int i, j;
    int startRow, startCol;


    for (j = 0; j < 9; j++)
    {
        if (board[row][j] == -1)
        {
            if (checkInPosArr(num, possibilities[row][j]->arr, possibilities[row][j]->size))
            {
                possibilities[row][j]->arr = removeCell(possibilities[row][j]->arr, possibilities[row][j]->size, num);
                (possibilities[row][j]->size)--;

            }
        }
        if (board[j][col] == -1)
        {
            if (checkInPosArr(num, possibilities[j][col]->arr, possibilities[j][col]->size))
            {
                possibilities[j][col]->arr = removeCell(possibilities[j][col]->arr, possibilities[j][col]->size, num);
                (possibilities[j][col]->size)--;
            }
        }

    }

    startRow = ((row / 3) * 3);
    startCol = ((col / 3) * 3);

    for (i = startRow; i < startRow + 3; i++)
    {
        for (j = startCol; j < startCol + 3; j++)
        {

            if (board[i][j] == -1)
            {
                if (checkInPosArr(num, possibilities[i][j]->arr, possibilities[i][j]->size))
                {
                    possibilities[i][j]->arr = removeCell(possibilities[i][j]->arr, possibilities[i][j]->size, num);
                    (possibilities[i][j]->size)--;
                }
            }


        }
    }
}
// cheking if num exist in the poss arr
bool checkInPosArr(int num, short* arr, int size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        if (arr[i] == num)
            return true;

    }
    return false;
}

// deleting the unnecessery num
short* removeCell(short* arr, int size, int num)
{
    int i, flag = 1, k = 0;

    short* res = (short*)malloc(sizeof(short) * (size - 1));
    checkMemoryAllocation(res);

    for (i = 0; i < size; i++)
    {
        if (arr[i] != num)
        {

            res[k] = arr[i];
            k++;
        }
    }
    free(arr);

    return res;
}

// checks if the player won/lost or not finished the game.
int checkFailure(short sudokuBoard[][9], Array*** possibilities) // checks if the player won/lost or not finished the game.
{
    int i, j, count = 0;
    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            if (sudokuBoard[i][j] == -1 && possibilities[i][j]->size == 0)
                return FINISH_FAILURE;
            else if (sudokuBoard[i][j] != -1)
                count++;
        }
    }

    if (count == 81)
    {
        return SUCCESS_FINISH;
    }
    else
    {
        return NOT_FINISH;
    }
}


// check the first cell in board which have the minimum possbilities
int minCell(short sudokuBoard[][9], Array*** possibilities, int* y)
{
    int sizeMin;
    int i, j, XSaver, YSaver;

    XSaver = 0;
    YSaver = 0;
    sizeMin = 10;

    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            if (sudokuBoard[i][j] == -1 && possibilities[i][j]->size < sizeMin)
            {
                sizeMin = possibilities[i][j]->size;
                XSaver = i;
                YSaver = j;
            }
        }
    }
    *y = YSaver;
    return XSaver;

}


// if all the cells in player's board have more than one possbilities, this function offers him the cordinate of the first cell with the least pssbilities.
int FillBoard(short board[][9], Array*** possibilities)
{
    int x, y, i, options, choose, res;
    res = OneStage(board, possibilities, &x, &y);

    while (res == NOT_FINISH)
    {
        sudokoPrintBoard(board);

        printf("\nCell number[%d, %d] currently holds the minimum number of possible values, select one of the below :\n", x, y);
        options = possibilities[x][y]->size;
        for (i = 0; i < options; i++)
        {
            printf("%d. %d\n", i + 1, possibilities[x][y]->arr[i]);
        }
        scanf("%d", &choose);
        board[x][y] = choose;
        printf("Updating cell %d with value %d\n", x * 9 + y, choose);
        free(possibilities[x][y]->arr);
        possibilities[x][y]->arr = NULL;
        possibilities[x][y]->size = 0;
        updatePos(board, possibilities, choose, x, y);

        res = OneStage(board, possibilities, &x, &y);

    }


    if (res == SUCCESS_FINISH)
    {

        return FILLED;
    }
    else
    {

        return FINISH_FAILURE;

    }

}

// these three function check if there are any duplication in the relavant coll/row/cell.
int DupInRow(short board[][9], int num, int row)
{
    int i, count = 0;
    for (i = 0; i < 9; i++)
    {
        if (num != -1)
        {
            if (board[row][i] == num)
                count++;
        }
    }
    if (count >= 2)
        return 1;
    else
        return 0;
}
int DupInColl(short board[][9], int num, int coll)
{
    int i, count = 0;
    for (i = 0; i < 9; i++)
    {
        if (num != -1)
        {
            if (board[i][coll] == num)
                count++;
        }
    }
    if (count >= 2)
        return 1;
    else
        return 0;
}
int DupInCell(short board[][9], int num, int row, int coll)
{
    int i, j, count = 0;

    for (i = row; i < row + 3; i++)
    {
        for (j = coll; j < coll + 3; j++)
        {
            if (num != -1)
            {
                if (board[i][coll] == num)
                    count++;
            }
        }
        if (count >= 2)
            return 1;
        else
            return 0;
    }
}

//prints the board.
void sudokoPrintBoard(short board[][9])
{
    int i, j;
    printf("\n");

    printf("  | 0 1 2| 3 4 5| 6 7 8\n");
    printf("-----------------------");


    for (i = 0; i < 9; i++)
    {
        printf("\n %d| ", i);
        for (j = 0; j < 9; j++)
        {
            if (j % 3 == 1)
            {
                printf(" ");
            }
            if (board[i][j] != -1)
            {
                printf("%d", board[i][j]);
            }
            else
            {
                printf(" ");
            }
            if (j % 3 == 2 && j != 8)
            {
                printf("| ");
            }
            if (j % 3 == 1)
            {
                printf(" ");
            }

        }
        if (i % 3 == 2)
        {
            printf("\n-----------------------");
        }
    }

    printf("\n");
}


// this function creates linked list of all the cells in our sudoko board .
List getList()
{
    unsigned int N = rand() % 20;
    N += 1;
    List resList;
    int i, j;
    makeEmptyList(&resList);

    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            insertDataToEndList(&resList, i, j);
        }
    }
    return resList;
}

void insertDataToEndList(List* lst, int i, int j)
{
    Node* newTail;
    newTail = createNewListNode(i, j, NULL);
    insertNodeToEndList(lst, newTail);
}

Node* createNewNode(CELL* q)
{
    Node* newNode = (Node*)malloc(sizeof(Node));
    checkMemoryAllocation(newNode);
    newNode->data->row = q->row;
    newNode->data->col = q->col;
}

void makeEmptyList(List* res)
{
    res->head = NULL;
    res->tail = NULL;
}
int isEmptyList(List* lst)
{
    return lst->head == NULL;

}


Node* createNewListNode(int row, int col, Node* next)
{
    Node* output;
    output = (Node*)malloc(sizeof(Node));
    checkMemoryAllocation(output);


    output->data = (CELL*)malloc(sizeof(CELL));
    checkMemoryAllocation(output->data);

    output->data->row = row;
    output->data->col = col;
    output->next = next;
    return output;
}
void insertNodeToEndList(List* lst, Node* tail)
{
    if (isEmptyList(lst))
    {
        lst->head = tail;
        lst->tail = tail;
    }
    else
    {
        lst->tail->next = tail;

        lst->tail = tail;
    }
}
void freeList(List* lst)
{
    Node* current = lst->head;
    Node* next = lst->head;
    while (current != NULL)
    {
        next = current->next;
        free(current->data);

        free(current);
        current = next;
    }
}

//This function puts random values in N cells in our sudoko board.
void makeBoard(short sudoko_board[][9], List* cellsList, Array*** possibilities)
{
    int i;
    short tempSize;
    short indexValue;
    srand(time(NULL));
    unsigned int N = (rand() % 20) + 1;
    Node* resNode;
    int randIndex;

    for (i = 0; i < N; i++)
    {

        resNode = FindCellToRandom(cellsList, i);
        tempSize = (possibilities[resNode->data->row][resNode->data->col])->size;
        if (tempSize != 0)
        {
            randIndex = (rand() % tempSize);

        }
        else
        {
            randIndex = 0;
        }
        if (randIndex == 9)
        {
            randIndex = 8;
        }


        indexValue = (possibilities[resNode->data->row][resNode->data->col])->arr[randIndex];
        sudoko_board[resNode->data->row][resNode->data->col] = indexValue;
        possibilities = PossibleDigits(sudoko_board);


    }


}

//This function finds the number of the link in our list that holds the cell that we want to put value inside , and delete it from the list and return it.
Node* FindCellToRandom(List* lst, int i)
{
    Node* resNode;
    Node* current;
    Node* temp;
    current = lst->head;
    int j;
    unsigned int K;


    K = (rand() % (SUDOKO_SIZE - i)) + 1;


    if (K == 1)
    {
        lst->head = current->next;
        resNode = current;
        current = NULL;
        free(current);
        return resNode;
    }
    for (j = 0; j < K - 2; j++)
    {
        current = current->next;
    }
    temp = current->next;
    current->next = temp->next;
    resNode = temp;
    temp = NULL;
    free(temp);
    return resNode;
}


//Playerlist functions:
//These functions creating the player list.
PlayerList makePlayersList(int* size)
{
    int x, i;
    PlayerList res;
    makeEmptyPlayerList(&res);
    PLAYER playerInfo;
    List cellsList;
    printf("please insert numbers of players\n");
    scanf("%d", &x);
    printf("Please insert the names of players\n");
    for (i = 0; i < x; i++)
    {
        scanf("%s", playerInfo.Name);
        initialBoard(playerInfo.board);
        cellsList = getList();
        playerInfo.possible_solutions = PossibleDigits(playerInfo.board);
        makeBoard(playerInfo.board, &cellsList, playerInfo.possible_solutions);
        freeList(&cellsList);
        playerInfo.possible_solutions = PossibleDigits(playerInfo.board);
        insertDataToEndPlayerList(&res, playerInfo);
    }
    *size = x;
    return res;
}
void makeEmptyPlayerList(PlayerList* lst)
{
    lst->head = lst->tail = NULL;
}
bool isEmptyPlayerList(PlayerList* lst)
{

    if (lst->head == NULL)
        return true;
    else
        return false;


}
void insertDataToEndPlayerList(PlayerList* lst, PLAYER data)
{
    PlayerNode* newHead;
    newHead = createNewPlayerListNode(data, NULL, NULL);
    insertNodeToEndPlayerList(lst, newHead);
}
PlayerNode* createNewPlayerListNode(PLAYER data, PlayerNode* next, PlayerNode* prev)
{
    PlayerNode* res;
    res = (PlayerNode*)malloc(sizeof(PlayerNode));
    checkMemoryAllocation(res);
    if (res)
    {
        res->data = data;
        res->next = next;
        res->prev = prev;
    }
    

    return res;
}
void insertNodeToEndPlayerList(PlayerList* lst, PlayerNode* tail)
{
    if (isEmptyPlayerList(lst) == true)
    {
        tail->next = tail->prev = NULL;
        lst->head = lst->tail = tail;
    }
    else
    {
        tail->prev = lst->tail;
        tail->next = NULL;
        lst->tail->next = tail;
        lst->tail = tail;
    }
}


// Create the array which every cell in this array point on speciefic node in the player list.
PlayerNode** playerNodePArr(PlayerList lst, int lstsize)
{
    PlayerNode* curr;
    curr = lst.head;
    PlayerNode** arr;
    arr = (PlayerNode**)malloc(sizeof(PlayerNode*) * lstsize);
    checkMemoryAllocation(arr);

    int i;
    for (i = 0; i < lstsize && curr != NULL; i++)
    {
        arr[i] = curr;
        curr = curr->next;
    }
    return arr;

}


//counting filled cells in the board.
int countFilledCells(short board[][9])
{
    int count = 0, i, j;
    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            if (board[i][j] != -1)
                count++;
        }
    }
    return count;
}


//sorting the  arr getting the arr and the size of the arr

void sortPlayerNodeArr(PlayerNode** arr, int size) {
    mergeSort(arr, size);
}

// these two function sortinh the pointer array.
void mergeSort(PlayerNode** arr, int size)
{
    PlayerNode** tmpArr = NULL;
    if (size <= 1)
    {
        return;
    }
    mergeSort(arr, size / 2);
    mergeSort(arr + size / 2, size - size / 2);
    tmpArr = merge(arr, size / 2, arr + size / 2, size - size / 2);
    if (tmpArr)
    {
        copyArr(arr, tmpArr, size); // copy values from tmpArr to arr
        free(tmpArr);
    }
    else
    {
        printf("Memory allocation failure!!!\n");
        exit(1);	// end program immediately with code 1 (indicating an error)
    }
}
PlayerNode** merge(PlayerNode** a1, int size1, PlayerNode** a2, int size2) {
    int ind1, ind2, resInd;
    PlayerNode** res = (PlayerNode**)malloc((size1 + size2) * sizeof(PlayerNode*));
    checkMemoryAllocation(res);

    if (res)
    {

        ind1 = ind2 = resInd = 0;


        while ((ind1 < size1) && (ind2 < size2))
        {
            if (countFilledCells(a1[ind1]->data.board) < countFilledCells(a2[ind2]->data.board))
            {
                res[resInd] = a1[ind1];
                ind1++;
            }
            else if (countFilledCells(a1[ind1]->data.board) > countFilledCells(a2[ind2]->data.board))
            {
                res[resInd] = a2[ind2];
                ind2++;
            }

            else
            {
                if (strcmp(a1[ind1]->data.Name, a2[ind2]->data.Name) < 0)
                {
                    res[resInd] = a1[ind1];
                    ind1++;
                }
                else
                {
                    res[resInd] = a2[ind2];
                    ind2++;
                }
            }
            resInd++;
        }

        while (ind1 < size1) {
            res[resInd] = a1[ind1];
            ind1++;
            resInd++;
        }
        while (ind2 < size2) {
            res[resInd] = a2[ind2];
            ind2++;
            resInd++;
        }
    }
    return res;
}
void copyArr(PlayerNode** dest, PlayerNode** src, int size)
{
    int i;

    for (i = 0; i < size; i++)
        dest[i] = src[i];
}


// these function increasing the size of the pointer array.
PlayerNode** increaseSize(PlayerNode** arr, int size, int* retSize)
{
    int i;
    int newSize;
    newSize = ceil(log2(size + 1));
    newSize = pow(2, newSize) - 1;
    if (newSize != size)
    {
        arr = (PlayerNode**)realloc(arr, newSize * sizeof(PlayerNode*));
        for (i = size; i < newSize; i++)
        {
            arr[i] = NULL;
        }
    }

    *retSize = newSize;
    return arr;
}


// makes tree based on the pointer array.
Tree buildTreeFromPointersArray(PlayerNode** arr, int size)
{
    Tree tr;
    tr.root = BuildTreeFromArrayHelper(arr, size);
    return tr;
}
TreeNode* BuildTreeFromArrayHelper(PlayerNode** arr, int  size)
{
    TreeNode* left;
    TreeNode* right;
    if (size == 0)
    {
        return NULL;
    }
    else
    {
        left = BuildTreeFromArrayHelper(arr, size / 2);
        right = BuildTreeFromArrayHelper(arr + (size / 2) + 1, (size / 2));

        return createNewTNode(&arr[size / 2], left, right);

    }
}
TreeNode* createNewTNode(PlayerNode** data, TreeNode* left, TreeNode* right)
{

    TreeNode* res;
    res = (TreeNode*)malloc(sizeof(TreeNode));
    checkMemoryAllocation(res);

    (res->data) = data;
    res->left = left;
    res->right = right;
    return res;
}

// This function runs the game in a recursive algorithem.
void gameRunner(Tree tr, PlayerList* players, PlayerList* winners)
{
    makeEmptyPlayerList(winners);

    gameRunnerHelper(tr.root, players, winners);
}
void gameRunnerHelper(TreeNode* root, PlayerList* players, PlayerList* winners)
{
    int res;

    if (root == NULL || players->head == NULL)
    {
        return;
    }

    else
    {
        gameRunnerHelper(root->left, players, winners);

        if (*(root->data) != NULL)
        {
            printf("it's now %s's turn\n", (*(root->data))->data.Name);
            res = FillBoard((*(root->data))->data.board, (*(root->data))->data.possible_solutions);
            if (res == FINISH_FAILURE)
            {
                printf("lost the game!\n");
                deleteFromPlayersList(players, root, 0); //delete from list
                root->data = NULL;
            }
            else if (res == FILLED)
            {
                printf("Board filled successefully!\n");
                deleteFromPlayersList(players, root, 1);
                insertNodeToEndPlayerList(winners, *(root->data));
                root->data = NULL;
            }
        }

        gameRunnerHelper(root->right, players, winners);
    }
}
void deleteFromPlayersList(PlayerList* players, TreeNode* root, int isWinnerList)
{
    PlayerNode* curr;
    curr = players->head;


    while (curr != *(root->data))
    {
        curr = curr->next;

    }
    if (curr->prev == NULL && curr->next == NULL)
    {

        players->head = players->tail = NULL;
    }
    else if (curr->prev == NULL)
    {
        players->head = curr->next;
        (curr->next)->prev = NULL;

    }
    else if (curr->next == NULL)
    {
        (curr->prev)->next = NULL;
        players->tail = curr->prev;

    }
    else
    {
        (curr->prev)->next = curr->next;
        (curr->next)->prev = curr->prev;


    }
    if (isWinnerList == 0)
    {
        *(root->data) = NULL;
        //freePossibilities(curr->data.possible_solutions);
        free(curr);

    }


}

void printWinners(PlayerList* winners)
{
    PlayerNode* curr;
    curr = (winners->head);
    printf("\nthe winners are:\n");
    while (curr != NULL)
    {
        printf("%s\n", curr->data.Name);
        sudokoPrintBoard(curr->data.board);
        printf("\n");
        curr = curr->next;
    }
}


// Free functions.
void myFree(PlayerList* winners, Tree* tr, PlayerNode** arrPointers)
{
    freePlayerList(winners);
    free(arrPointers);
    freeTreeHelper(tr->root);

}
void freePossibilities(Array*** poss)
{
    int i, j;
    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            if (poss[i][j]->arr != NULL)
            {
                free(poss[i][j]->arr);
            }
            free(poss[i][j]);
        }
        free(poss[i]);
    }
    free(poss);
}
void freePlayerList(PlayerList* players)
{
    PlayerNode* curr = players->head;
    PlayerNode* temp;

    while (curr != NULL)
    {
        temp = curr;
        curr = curr->next;
        freePossibilities(temp->data.possible_solutions);
        free(temp);

    }


}
void freeTreeHelper(TreeNode* root)
{
    if (root == NULL)
        return;
    else
    {
        freeTreeHelper(root->left);
        freeTreeHelper(root->right);
        free(root);
    }
}

void checkMemoryAllocation(void* ptr)
{
    if (ptr == NULL)
    {
        printf("memory allocation failed !\n");
        exit(1);
    }
}

// THIS FUNCTION RUNS ALL THE MAIN FUNCTIONS, AND BASICLLY MANAGING THE GAME.
void playGame()
{
    int size;
    int newsize;
    PlayerList players;
    players = makePlayersList(&size);
    PlayerList winners;
    PlayerNode** ptrArrToPlayerList;
    ptrArrToPlayerList = playerNodePArr(players, size);
    sortPlayerNodeArr(ptrArrToPlayerList, size);//sorting the arr getting the arr and the size of the arr
    Tree TreeOfPointersToArray;
    ptrArrToPlayerList = increaseSize(ptrArrToPlayerList, size, &newsize);
    TreeOfPointersToArray = buildTreeFromPointersArray(ptrArrToPlayerList, newsize);
    gameRunner(TreeOfPointersToArray, &players, &winners);

    if (isEmptyPlayerList(&winners) == false)
    {
        printWinners(&winners);
    }
    myFree(&winners, &TreeOfPointersToArray, ptrArrToPlayerList);
}
