/*
    Connor Musick
    Miranda Hampton
    8-puzzle A* heuristic solver
*/
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <ctype.h>
#include <algorithm>
#include <sstream>
#include <iterator>
#include <cstdio>

using namespace std;

#define UP 0
#define DOWN 1
#define RIGHT 2
#define LEFT 3
#define DEBUG_INIT "1 3 4\n8 6 2\n_ 7 5"
#define DEBUG_GOAL "1 2 3\n8 _ 4\n7 6 5\n"

// Globals

int debug = 1;
vector<int> curr_puzzle;
vector<int> goal_puzzle;
vector<string> database;

// Functions
int MisplacedTiles(int f, int num_moves, int last_direction);
int ManhattanDistance();

vector<int> process_input(string type);
void print_puzzle(vector<int> &puzzle);
int compare_puzzles(vector<int> puzzle);
void swap(vector<int> &puzzle,int first, int second);
bool check_legal_move(int direction);
vector<int> expand_node(int direction);
int find_blank();
int mindex(int f[], int size);
void print_choice(int direction);
void store_puzzle(vector<int> &puzzle);
bool check_db(vector<int> possible_puzzle);

// MAIN
int main()
{
    curr_puzzle = process_input("initial");
    goal_puzzle = process_input("goal");
    char heur = '\0';
    //select hueristic
    cout << "Select the heuristic" << endl
        << "a) Number of misplaced tiles" << endl
        << "b) Manhattan distance" << endl;
        // add more if want to
    cin >> heur;
    cout << "Solution:" <<endl<<endl;
    switch(heur)
    {
        case 'a':
            int num_misplaced = compare_puzzles(curr_puzzle);
            if(debug)
                cout << "DEBUG: There are " << num_misplaced << " tiles misplaced."<<endl;
            int status = MisplacedTiles(num_misplaced, 0, -1);
            if(status == -1) cout << "Encountered a problem... exiting"<<endl;
            break;
    }
    return 0;
}

int MisplacedTiles(int f, int num_moves, int last_direction)
{
    bool problem = true;
    if(compare_puzzles(curr_puzzle) == 0)
    {
        cout << "\nGiven the selected heuristic, the solution required " << num_moves << " moves."<<endl;
        // number of nodes?
        return num_moves;
    }
    // Possible nodes
    vector<int> possible_puzzle[4];
    int fscore[4] = {2147483647,2147483647,2147483647,2147483647};
    // Check moving up
    if(check_legal_move(UP) && last_direction != DOWN)
    {
        possible_puzzle[UP] = expand_node(UP);
        if(!check_db(possible_puzzle[UP]))
        {
            problem = false;
            fscore[UP] = compare_puzzles(possible_puzzle[UP]);// + num_moves;
            if(debug)
            {
                cout<< "fscore up: "<<fscore[UP]<< endl;
                print_puzzle(possible_puzzle[UP]);
            }
        } 
    }
    // Check moving down
    if(check_legal_move(DOWN) && last_direction != UP)
    {
        possible_puzzle[DOWN] = expand_node(DOWN);
        if(!check_db(possible_puzzle[DOWN]))
        {
            problem = false;
            fscore[DOWN] = compare_puzzles(possible_puzzle[DOWN]);// + num_moves;
            if(debug)
            {
                cout<< "fscore down: "<<fscore[DOWN]<< endl;
                print_puzzle(possible_puzzle[DOWN]);
            }
        }
    }
    // Check moving right
    if(check_legal_move(RIGHT) && last_direction != LEFT)
    {
        possible_puzzle[RIGHT] = expand_node(RIGHT);
        // check db if already visited
        if(!check_db(possible_puzzle[RIGHT]))
        {
            problem = false;
            fscore[RIGHT] = compare_puzzles(possible_puzzle[RIGHT]);// + num_moves;
            if(debug)
            {
                cout<< "fscore right: "<<fscore[RIGHT]<< endl;
                print_puzzle(possible_puzzle[RIGHT]);
            }
        }
    }
    // Check moving left
    if(check_legal_move(LEFT) && last_direction != RIGHT)
    {
        possible_puzzle[LEFT] = expand_node(LEFT);
        if(!check_db(possible_puzzle[LEFT]))
        {
            problem = false;
            fscore[LEFT] = compare_puzzles(possible_puzzle[LEFT]);// + num_moves;
            if(debug)
            {
                cout<< "fscore left: "<<fscore[LEFT]<< endl;
                print_puzzle(possible_puzzle[LEFT]);
            }
        }
    }
    if(problem) return -1;
    // Find minimum, take that path
    int min_f = mindex(fscore, 4);
    if(debug)
        cout << "Minimum f index is " << min_f << endl <<endl;
    curr_puzzle = possible_puzzle[min_f];
    store_puzzle(curr_puzzle);
    print_choice(min_f);
    // return MisplacedTiles with updated values
    return MisplacedTiles(fscore[min_f], num_moves+1, min_f);
}

int ManhattanDistance()
{
    return 0;
}

vector<int> process_input(string type)
{
    int num_rows = 3;
    vector<int> inp;
    string puzzle_str;
    string buffer;
    bool has_underscore = false;
    bool has_correct_num_num = false;

    if(type.compare("initial") == 0)
    {
        cout << "Enter the initial state" << endl;
        cout << "Format: " << endl
            << "\t1 2 3" << endl
            << "\t4 _ 5" << endl
            << "\t6 7 8" << endl;
        if(debug)
        {
            cout << "DEBUG init state used"<<endl;
            puzzle_str = DEBUG_INIT;
        }
        else
        {
            // Grab input
            int i = 0;
            while(getline(cin, buffer))
            {
                i++;
                puzzle_str += buffer + " ";
                if(i == 3)
                    break;
            }
        }
        
    }
       
    else if (type.compare("goal") == 0)
    {
        cout << "Enter the goal state" << endl;
        if(debug)
        {
            cout << "DEBUG goal state used" << endl;
            puzzle_str = DEBUG_GOAL;
        }
        else
        {
            // Grab input
            int i = 0;
            while(getline(cin, buffer))
            {
                i++;
                puzzle_str += buffer + " ";
                if(i == 3)
                    break;
            }
        }
    }
    if(debug)
        cout << "Puzzle input: \n" << puzzle_str << endl;

    for(string::size_type i = 0; i < puzzle_str.size(); i++)
    {
        if(!isspace(puzzle_str[i]))
        {
            if(puzzle_str[i] == '_')
            {
                inp.push_back(0);
            }
            else
            {
               inp.push_back(atoi(&puzzle_str[i]));
            }
        }        
    }
    
    return inp;
}

void print_puzzle(vector<int> &puzzle)
{
    int count = 0;

    for(vector<int>::iterator i = puzzle.begin(); i != puzzle.end(); i++)
    {
        cout << *i << ' ';
        count++;
        if(count == 3)
        {
            cout << endl;
            count = 0;
        }
    }
}

// Helper func to return number of misplaced tiles
int compare_puzzles(vector<int> puzzle)
{
    int num_misplaced = 0;
    for(int i = 0; i < 9; i++)
    {
        if(puzzle.at(i)!=goal_puzzle.at(i))
            num_misplaced++;
    }
    return num_misplaced;
}

void swap(vector<int>& puzzle,int first, int second)
{
    int temp = puzzle.at(second);
    puzzle.at(second) = puzzle.at(first);
    puzzle.at(first) = temp;
}

bool check_legal_move(int direction)
{
    switch(direction)
    {
        case UP:
            return curr_puzzle.at(0) != 0 && curr_puzzle.at(1) != 0 && curr_puzzle.at(2) != 0;
        break;
        case DOWN:
            return curr_puzzle.at(6) != 0 && curr_puzzle.at(7) != 0 && curr_puzzle.at(8) != 0;
        break;
        case RIGHT:
            return curr_puzzle.at(2) != 0 && curr_puzzle.at(5) != 0 && curr_puzzle.at(8) != 0;
        break;
        case LEFT:
            return curr_puzzle.at(0) != 0 && curr_puzzle.at(3) != 0 && curr_puzzle.at(6) != 0;
        break;
        default:
            cout << "check_legal_move(): should not see this..."<<endl;
    }
    return false;
}

// expand curr puzzle in the direction and return it
vector<int> expand_node(int direction)
{
    vector<int> expanded_node = curr_puzzle;
    int blank = find_blank();
    switch(direction)
    {
        case UP:
            swap(expanded_node, blank, blank-3);
            break;
        case DOWN:
            swap(expanded_node, blank, blank+3);
            break;
        case RIGHT:
            swap(expanded_node, blank, blank+1);
            break;
        case LEFT:
            swap(expanded_node, blank, blank-1);
            break;
    }
    return expanded_node;
}

// finds blank tile in curr_puzzle
int find_blank()
{
    for(int i = 0; i != curr_puzzle.size(); i++)
    {
        if(curr_puzzle.at(i) == 0)
        {
            return i;
        }
            
    }
    return -1;
}

// returns index of smallest f value
int mindex(int f[], int size)
{
    int min = 999;
    int mindex = 0;
    for (int i = 0; i < size; i++)
    {
        if(f[i] <= min)
        {
            mindex = i;
            min = f[i];
        }
    }
    return mindex;
}

void print_choice(int direction)
{
    switch(direction)
    {
        case UP:
            cout << "Move blank up" <<endl;
        break;
        case DOWN:
            cout << "Move blank down" <<endl;
        break;
        case RIGHT:
            cout << "Move blank right" <<endl;
        break;
        case LEFT:
            cout << "Move blank left" <<endl;
        break;
        default:
            cout << "print_choice(): should not see this..."<<endl;
    }
    if(debug) print_puzzle(curr_puzzle);
}

// turns vector<int> -> string and stores it
void store_puzzle(vector<int> &puzzle)
{
    stringstream result;
    copy(puzzle.begin(), puzzle.end(), ostream_iterator<int>(result, " "));
    string puzzle_str = result.str();
    //cout << "Stored:\n\t"<< puzzle_str << endl;
    database.push_back(puzzle_str);
}

// returns true if puzzle was already visited
bool check_db(vector<int> possible_puzzle)
{
    stringstream result;
    //ostream_iterator<int> os(result, " ");
    copy(possible_puzzle.begin(), possible_puzzle.end(), ostream_iterator<int>(result, " "));
    string pos_puzzle_str = result.str();
    for(vector<string>::iterator i = database.begin(); i != database.end(); i++)
    {
        if((*i).compare(pos_puzzle_str)==0)
        {
            return true;
        }
    }
    return false;
}