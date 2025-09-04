/*file name: mine_sweeper.cpp*/
#include <iostream>
#include <random>
using namespace std;

// 给用户一个输入列的指引
int row_instruction[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
// 初始化我们的map
int map[10][10] =
    {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

short int vis_map[10][10] =
     {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
// 这里，0表示没有雷且未被打开
// 12表示有雷
// 1表示没有雷且被打开

//这些是array的一些常量
const int MINE_NUM = 20;
const int TOTAL_ROW = 10, TOTAL_COL = 10;
const int MAX_ROW_INDEX = TOTAL_ROW - 1;
const int MAX_COL_INDEX = TOTAL_COL - 1;
bool game_finish = false;
//这些变量是给map使用的
const int HAVE_MINE = 12;
//这些变量是给vis_map使用的
const int OPEN = 1;
const int NOT_OPEN = 0;
const int MARKED = 2;

// 一些辅助函数：
int max(int x, int y)
{
    if (x > y)
        return x;
    else
        return y;
}
int min(int x, int y)
{
    if (x < y)
        return x;
    else
        return y;
}
//一个测试gen_map功能的辅助函数
void ceshi_map(int map[][10])
{
    for(int i = 0; i < TOTAL_ROW; i++)
    {
        for (int j = 0; j < TOTAL_COL; j++)
        {cout << map[i][j] << '\t';}
        cout << endl;
    }
    
}

// 输出map
// 我们需要优化这个算法，尽量让他的算法复杂度低
void print_map(void)
{
    cout << '\t';
    for (int w = 0; w < TOTAL_ROW; w++)
    {cout << w << '\t';}
    cout << endl;
    for (int i = 0; i < TOTAL_ROW; i++)
    {
        cout << row_instruction[i] << '\t';
        for(int j = 0; j < TOTAL_COL; j++)
        {
            //现在，开始print我们的map
            if(vis_map[i][j] == OPEN)
            {
                if (map[i][j] != 0){cout << map[i][j] << '\t';}     //打开格子，并且周围有雷时，标出数字
                else cout << " " << '\t';                           //打开格子，周围无雷，空格
            }
            else if(vis_map[i][j] == NOT_OPEN && vis_map[i][j] != MARKED)
            {cout << "?" << '\t';}                          //未打开、未标记的格子：？
            else 
            {cout << "*" << '\t';}                                  //被标记的格子：*
        }
        cout << endl;
    }
}

// 找到在这个打开的格子周围的雷的数量
// 我们要仔细考虑边界上的情况
short int mine_around_you(int row, int col)
{
    //如果这个格子有雷，我们无需计数
    if(map[row][col] == HAVE_MINE)
    {
        return 12;
    }
    short int count;
    count = 0;
    short int row_upbound_cond = -1, col_lebound_cond = -1; //default to be -1
    short int row_lobound_cond = 1 , col_ribound_cond = 1 ; //default to be 1

    if(row == 0){row_upbound_cond = 0;}                     //检测边界条件
    if(col == 0){col_lebound_cond = 0;}
    if(row == TOTAL_ROW - 1){row_lobound_cond = 0;}             //防止出现bug
    if(col == TOTAL_COL - 1){col_ribound_cond = 0;}

    for (int i = row_upbound_cond ; i <= row_lobound_cond ; i++)
    {
        for (int j = col_lebound_cond ; j <= col_ribound_cond ; j++)
        {if (map[row + i][col + j] == HAVE_MINE){++count;}}
    }

    return count;
}

// 生成我们的map，并埋好雷
/*我们可以在这个时候就标记好一个格子周围有多少雷，
  这样print_array的算法复杂度就会大幅下降*/
void gen_map(int map[][10])
{
    int mine_pos;
    short int count;
    for (int i = 0; i < MINE_NUM;)
    {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, 100);
        mine_pos = dis(gen);
        if (map[mine_pos / 10][mine_pos % 10] != HAVE_MINE)
        {
            map[mine_pos / 10][mine_pos % 10] = HAVE_MINE;
            i++;
        }
    }
    // for check use:
    //print_map(map);

    for (int i = 0; i < TOTAL_ROW; i++)
    {
        for (int j = 0; j < TOTAL_COL; j++)
        {
            map[i][j] = mine_around_you(i, j);
        }
    }
}

// 点开一个格子

void open_block(int chosen_row, int chosen_col)
{
    //这里使用静态转换，提高稳定性
    //int chosen_row_num = static_cast<int>(chosen_row - 65);
    //如果格子里面有雷，游戏结束
    if (map[chosen_row][chosen_col] == HAVE_MINE)
    {
        game_finish = true;
    }
    //如果格子没有打开，打开格子
    else if (vis_map[chosen_row][chosen_col] == NOT_OPEN)
    {
        vis_map[chosen_row][chosen_col] = OPEN;
    }
}

//尝试一次点开很多格子

void open_lot_block(int chosen_row, int chosen_col, int count)
{
    //cout << chosen_row <<'\t'<< chosen_col << endl;
    //int a;
    //cin >> a;

    /*
    if (count > 3)
    {
        cout << "reach max depth" << '\t' << endl;
    }
    */
    //第一次点击时，如果格子里面有雷，游戏结束
    if (count == 0 && map[chosen_row][chosen_col] == HAVE_MINE)
    {
        game_finish = true;
        return ;
    }
    
    //如果递归到了一个周围有雷的格子(未打开)，递归结束，打开这个格子
    if (vis_map[chosen_row][chosen_col] == NOT_OPEN
            && map[chosen_row][chosen_col] != HAVE_MINE 
            && map[chosen_row][chosen_col] != 0)
    {
        vis_map[chosen_row][chosen_col] = OPEN;
        return ;
    }

    //如果递归到了一个周围没有雷的格子(未打开)，继续我们的递归；并打开这个格子
    else if (vis_map[chosen_row][chosen_col] == NOT_OPEN
            && map[chosen_row][chosen_col] == 0)
    {
        vis_map[chosen_row][chosen_col] = OPEN;    //这是第三个base case

        open_lot_block(min(chosen_row + 1, MAX_ROW_INDEX), chosen_col, count++);
        open_lot_block(chosen_row, min(chosen_col + 1, MAX_COL_INDEX), count++);
        open_lot_block(max(chosen_row - 1, 0), chosen_col, count++);
        open_lot_block(chosen_row, max(chosen_col - 1, 0), count++);

        open_lot_block(min(chosen_row + 1, MAX_ROW_INDEX), min(chosen_col + 1, MAX_COL_INDEX), count++);
        open_lot_block(max(chosen_row - 1, 0), min(chosen_col + 1, MAX_COL_INDEX), count++);
        open_lot_block(max(chosen_row - 1, 0), max(chosen_col - 1, 0), count++);
        open_lot_block(min(chosen_row + 1, MAX_ROW_INDEX), max(chosen_col - 1, 0), count++);
    }

    //如果递归到一个有雷的格子，停止递归
    else if(vis_map[chosen_row][chosen_col] == NOT_OPEN     
            && map[chosen_row][chosen_col] == HAVE_MINE
            && count > 0)
    {
        //debug message:
        cout << "find a block with mine: "<< endl; 
        return ;
    }

    //如果递归到一个已经打开的格子，停止递归
    else        
    {
        cout << "have opened" << endl;
        return ;
    }
}


//标记有雷的格子
void mark(int row, int col)
{
    if(vis_map[row][col] == NOT_OPEN){vis_map[row][col] = MARKED;}
    else {cerr << "you can't mark an opened block!" << endl;}
}

//胜利条件的判定及胜利信息：
bool win(void)
{
    bool win = false;
    short int count = 0;
    for (int i = 0; i < TOTAL_ROW; i++)
    {
        for (int j = 0; j < TOTAL_COL; j++)
        {if (map[i][j] == 0 && vis_map[i][j] == OPEN){++count;}}
    }
    if (count == 100 - MINE_NUM)    //打开所有没有雷的格子
    {
        win = true;
        cout << "YOU WIN!!!" << endl;
    }
    return win;
}

// 这里是我们的主函数：
int main()
{
    int chosen_row;
    int chosen_col;
    gen_map(map);
    ceshi_map(map);
    print_map();
    
    bool game_win = false;
    while (!game_finish)
    {
        // 通过键盘输入实现交互
        char mark_or_open;
        cout << "row: 0~9 ;col: 0~9" << endl;
        cout << "enter M to mark the block; enter O to open the block" << endl;
        cout << "please enter the position of the block you want to OPEN:" << endl;
        cout << "mark or open?:" << '\t';
        cin >> mark_or_open;
        cout << "row: ";
        cin >> chosen_row;
        cout << "col: ";
        cin >> chosen_col;
        
        if (mark_or_open == 'M')
        {
            mark(chosen_row, chosen_col);
            print_map();
            continue;       //由于标记雷无需打开它，直接continue，回到开始即可。
        }
        
        open_lot_block(chosen_row, chosen_col, 0);
        print_map();
        game_win = win();
        if(game_win == true){return 0;}
    }
    cout << "OH, NO, YOU LOSE." << endl ;
    
    return 0;
}