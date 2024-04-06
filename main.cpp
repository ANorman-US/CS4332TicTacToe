//Alexander Norman
#include <vector>
#include <string>
#include <iostream>
#include <cmath>
#include <climits>

using namespace std;

void nextMove(const char &, vector<vector<char>>&);
bool isTerminal(const vector<vector<char>>&);//return true if game is over (board full or somebody has won)
int utilFunction(const char &, const vector<vector<char>>&);//1 if player has won, 0 if draw, -1 if loss
int alphaBetaPruning(const char&, vector<vector<char>>&, bool , int, int);

int main()
{

    string input;
    cin >> input;
    char player = input[0];
    vector<vector<char>> array (3, vector<char> (3));//vector to hold board

    for(int i=0;i<3;i++)
    {
        cin >> input;
        for(int j=0;j<3;j++)
            array[i][j] = input[j];
    }
    nextMove(player, array);

    return 0;
}

void nextMove(const char &player, vector<vector<char>> &array)
{
    int maxScore = INT_MIN;
    int r, c;
    //prioritize middle spot
    //also check for win in one moves
    if(array[1][1] == '_')
    {
        array[1][1] = player;
        if(isTerminal(array))//if win in one
        {
            cout << 1 << " " << 1;
            return;
        }
        int current = alphaBetaPruning(player, array, false, INT_MIN, INT_MAX);//init alpha, beta as -inf, +inf
        array[1][1] = '_';
        if(current > maxScore)
        {
            maxScore = current;
            r = 1;
            c = 1;
        }
    }

    //then rest. 
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            if(array[i][j] == '_')
            {
                array[i][j] = player;
                if(isTerminal(array))//if win in one
                {
                    cout << i << " " << j;
                    return;
                }
                int current = alphaBetaPruning(player, array, false, INT_MIN, INT_MAX);//init alpha, beta as -inf, +inf
                array[i][j] = '_';
                if(current > maxScore)
                {
                    maxScore = current;
                    r = i;
                    c = j;
                }
            }
        }
    }
    cout << r << " " << c;
}

int alphaBetaPruning(const char& player, vector<vector<char>> &array, bool isPlayerMax, int alpha, int beta)
{
    char opponent;
    if(player == 'X')
        opponent = 'O';
    else
        opponent = 'X';

    if(isTerminal(array))//if leaf node we return value of node
        return utilFunction(player, array);

    if (isPlayerMax)
    {
        int bestVal = INT_MIN;
        for(int i=0;i<3;i++)
        {
            for(int j=0;j<3;j++)
            {
                if(array[i][j] == '_')//generate children nodes
                {
                    array[i][j] = player;
                    int value = alphaBetaPruning(player, array, false, alpha, beta);
                    array[i][j] = '_';//undo change
                    bestVal = max(bestVal, value);
                    alpha = max(alpha, bestVal);
                    if(beta <= alpha)
                        break;
                }
            }
        }
        return bestVal;
    }
    else
    {
        int bestVal = INT_MAX;
        for(int i=0;i<3;i++)
        {
            for(int j=0;j<3;j++)
            {
                if(array[i][j] == '_')//generate children nodes
                {
                    array[i][j] = opponent;
                    int value = alphaBetaPruning(player, array, true, alpha, beta);
                    array[i][j] = '_';
                    bestVal = min(bestVal, value);
                    beta = min(beta, bestVal);
                    if (beta <= alpha)
                        break;
                }
            }
        }
        return bestVal;
    }
}

int utilFunction(const char &player, const vector<vector<char>>& array)//maybe clean this up later
{
    char opponent;
    if(player == 'X')
        opponent = 'O';
    else
        opponent = 'X';
        
    for(int i=0;i<3;i++)
    {
        if(array[i][0] == player && array[i][1] == player && array[i][2] == player)
            return 1;
        if(array[i][0] == opponent && array[i][1] == opponent && array[i][2] == opponent)
            return -1;
        if(array[0][i] == player && array[1][i] == player && array[2][i] == player)
            return 1;
        if(array[0][i] == opponent && array[1][i] == opponent && array[2][i] == opponent)
            return -1;
    }
    //diagonals
    if(array[0][0] == player && array[1][1] == player && array[2][2] == player)
        return 1;
    if (array[0][2] == player && array[1][1] == player && array[2][0] == player)
        return 1;
    if(array[0][0] == opponent && array[1][1] == opponent && array[2][2] == opponent)
        return -1;
    if (array[0][2] == opponent && array[1][1] == opponent && array[2][0] == opponent)
        return -1;
    return 0;
}

bool isTerminal(const vector<vector<char>> &array)
{
    //check if a player has won first
    for(int i=0;i<3;i++)
    {
        if(array [i][0] == array[i][1] && array[i][0] == array[i][2] && array[i][0] != '_')//rows
            return true;
        if(array [0][i] == array[1][i] && array[0][i] == array[2][i] && array[0][i] != '_')//columns
            return true;
    }
    //diagonals
    if(array[0][0] == array[1][1] && array[0][0] == array[2][2] && array[0][0] != '_')
        return true;
    if (array[0][2] == array[1][1] && array[0][2] == array[2][0] && array[0][2] != '_')
        return true;

    //check if board is full
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            if(array[i][j] == '_')//board not full
                return false;
        }
    }
    //board is full and no victor yet
    return true;
}
