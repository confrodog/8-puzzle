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
#include <cstdlib>

using namespace std;

#define MISPLACED 0
#define MANHATTAN 1
#define UP 0
#define DOWN 1
#define RIGHT 2
#define LEFT 3
#define DEBUG_INIT "3 6 4\n_ 1 2\n8 7 5\n"
#define DEBUG_GOAL "1 2 3\n8 _ 4\n7 6 5\n"

// Globals

int debug = 0;
int mode;
int num_nodes_explored = 0;
bool found = false;
vector<int> curr_puzzle;
vector<int> goal_puzzle;
vector<string> database;

// Functions
int solve(int num_moves, int last_direction);
int MisplacedTiles(vector<int> puzzle);
int ManhattanDistance(vector<int> puzzle);
//int compare_puzzles();

vector<int> process_input(string type);
void print_puzzle(vector<int> &puzzle);
void swap(vector<int> &puzzle,int first, int second);
bool check_legal_move(int direction);
vector<int> expand_node(int direction);
int find_blank();
void mindex(int f[],bool min_f[], int size);
void print_choice(int direction);
void store_puzzle(vector<int> puzzle);
bool check_db(vector<int> possible_puzzle);

void print_min_f(bool* minf);

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
            {
                if(debug) 
                {
                    int num_misplaced = MisplacedTiles(curr_puzzle);
                    cout << "DEBUG: There are " << num_misplaced << " tiles misplaced."<<endl;
                }
                mode = MISPLACED;
            }
            break;
        case 'b':
            {
                mode = MANHATTAN;
            }
            break;
        default:
            cout << "That is not a valid command, goodbye."<<endl;
    }
    solve(0, -1);
    return 0;
}

// solves given 8puzzle with input heuristic
int solve(int num_moves, int last_direction)
{
    if(debug) cout << "\tMove #" << num_moves<<endl;
    if((mode == MISPLACED && MisplacedTiles(curr_puzzle) == 0) || (mode == MANHATTAN && ManhattanDistance(curr_puzzle) == 0))
    {
        cout << "\nGiven the selected heuristic, the solution required " << num_moves << " moves."<<endl;
        cout << "The A* explored "<< num_nodes_explored << " nodes to find this solution."<<endl;
        found = true;
        return num_moves;
    }
    // Possible nodes
    vector<int> possible_puzzle[4];
    int fscore[4] = {2147483647,2147483647,2147483647,2147483647};
    // Check moving up
    if(check_legal_move(UP) && last_direction != DOWN)
    {
        possible_puzzle[UP] = expand_node(UP);
        // Check db if already visited
        if(check_db(possible_puzzle[UP]) == false)
        {
            num_nodes_explored++;
            if(mode == MISPLACED)
            {
                fscore[UP] = MisplacedTiles(possible_puzzle[UP]);// + num_moves;
            }
            else if(mode == MANHATTAN)
            {
                fscore[UP] = ManhattanDistance(possible_puzzle[UP]);// + num_moves;
            }

            if(debug)
            {
                if(debug) cout<< "fscore up: "<<fscore[UP]<< endl;
                //print_puzzle(possible_puzzle[UP]);
            }
        } 
    }
    // Check moving down
    if(check_legal_move(DOWN) && last_direction != UP)
    {
        possible_puzzle[DOWN] = expand_node(DOWN);
        if(check_db(possible_puzzle[DOWN])==false)
        {
            num_nodes_explored++;
            if(mode == MISPLACED)
            {
                fscore[DOWN] = MisplacedTiles(possible_puzzle[DOWN]);// + num_moves;
            }
            else if(mode == MANHATTAN)
            {
                fscore[DOWN] = ManhattanDistance(possible_puzzle[DOWN]);// + num_moves;
            }
            
            if(debug)
            {
                if(debug) cout<< "fscore down: "<<fscore[DOWN]<< endl;
                //print_puzzle(possible_puzzle[DOWN]);
            }
        }
    }
    // Check moving right
    if(check_legal_move(RIGHT) && last_direction != LEFT)
    {
        possible_puzzle[RIGHT] = expand_node(RIGHT);
        // check db if already visited
        if(check_db(possible_puzzle[RIGHT])==false)
        {
            num_nodes_explored++;
            if(mode == MISPLACED)
            {
                fscore[RIGHT] = MisplacedTiles(possible_puzzle[RIGHT]);// + num_moves;
            }
            else if(mode == MANHATTAN)
            {
                fscore[RIGHT] = ManhattanDistance(possible_puzzle[RIGHT]);// + num_moves;
            }

            if(debug)
            {
                if(debug) cout<< "fscore right: "<<fscore[RIGHT]<< endl;
                //print_puzzle(possible_puzzle[RIGHT]);
            }
        }
    }
    // Check moving left
    if(check_legal_move(LEFT) && last_direction != RIGHT)
    {
        possible_puzzle[LEFT] = expand_node(LEFT);
        if(check_db(possible_puzzle[LEFT]) == false)
        {
            num_nodes_explored++;
            if(mode == MISPLACED)
            {
                fscore[LEFT] = MisplacedTiles(possible_puzzle[LEFT]);// + num_moves;
            }
            else if(mode == MANHATTAN)
            {
                fscore[LEFT] = ManhattanDistance(possible_puzzle[LEFT]);// + num_moves;
            }
            if(debug)
            {
                if(debug) cout<< "fscore left: "<<fscore[LEFT]<< endl;
                //print_puzzle(possible_puzzle[LEFT]);
            }
        }
    }
    // Find all minimum, branch down all paths
    bool min_f[4] = {false, false, false, false};
    mindex(fscore, min_f, 4);
    if (debug) print_min_f(min_f);
    if(min_f[UP] == true && found == false)
    {
        curr_puzzle = possible_puzzle[UP];
        store_puzzle(possible_puzzle[UP]);
        print_choice(UP);
        solve(num_moves+1, UP);
    }
    if(min_f[DOWN] == true && found == false)
    {
        curr_puzzle = possible_puzzle[DOWN];
        store_puzzle(possible_puzzle[DOWN]);
        print_choice(DOWN);
        solve(num_moves+1, DOWN);
    }
    if(min_f[RIGHT] == true && found == false)
    {
        curr_puzzle = possible_puzzle[RIGHT];
        store_puzzle(possible_puzzle[RIGHT]);
        print_choice(RIGHT);
        solve(num_moves+1, RIGHT);
    }
    if(min_f[LEFT] == true && found == false)
    {
        curr_puzzle = possible_puzzle[LEFT];
        store_puzzle(possible_puzzle[LEFT]);
        print_choice(LEFT);
        solve(num_moves+1, LEFT);
    }
    return 0;
}

// Solve by using misplaced tiles heuristic
int MisplacedTiles(vector<int> puzzle)
{
    int num_misplaced = 0;
    for(int i = 0; i < 9; i++)
    {
        if(puzzle.at(i)!=goal_puzzle.at(i))
            num_misplaced++;
    }
    return num_misplaced;
}

int ManhattanDistance(vector<int> puzzle)
{
    int distance = 0, i = 0, j = 0;
    for(i = 0; i < goal_puzzle.size(); i++)
    {
        for(j = 0; j < puzzle.size(); j++)
        {
            if(goal_puzzle.at(i) == puzzle.at(j))
            {
                break;
            }
        }
        int difference = abs(i-j);
        if(difference >= 6)
        {
            difference = 2 + (difference - 6);
            distance += difference;
        }
        else if (difference >= 3)
        {
            difference = 1 + (difference - 3); 
            distance += difference;
        }
        else
        {
            distance += difference;
        }
        
        if(debug) cout << difference << " + ";
    }
    if (debug) cout << " = " << distance << endl;
    return distance;
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

    for(int i = 0; i < puzzle_str.size(); i++)
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

// returns index of smallest f value and value
void mindex(int f[], bool min_f[], int size)
{
    int* _min = min_element(f,f+size);
    if(debug) cout<<"mindex(): "<< *_min<<endl;
    for (int i = 0; i < size; i++)
    {
        if(f[i] <= *_min)
        {
            min_f[i] = true;
        }
    }
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
    print_puzzle(curr_puzzle);
}

// turns vector<int> -> string and stores it
void store_puzzle(vector<int> puzzle)
{
    
        stringstream result;
        copy(puzzle.begin(), puzzle.end(), ostream_iterator<int>(result, " "));
        string puzzle_str = result.str();
        if(debug) cout << "Stored current puzzle into db."<<endl;
        database.push_back(puzzle_str);
}

// returns true if puzzle was already visited
bool check_db(vector<int> possible_puzzle)
{
    stringstream result;
    copy(possible_puzzle.begin(), possible_puzzle.end(), ostream_iterator<int>(result, " "));
    string pos_puzzle_str = result.str();
    for(vector<string>::iterator i = database.begin(); i != database.end(); i++)
    {
        if((*i).compare(pos_puzzle_str)==0)
        {
            if(debug) cout << "db: '" << (*i) << "', current: '"<< pos_puzzle_str <<"'."<<endl;
            return true;
        }
    }
    return false;
}

void print_min_f(bool* minf)
{
    for(int i = 0; i<4;i++)
    {
        cout << "min_f: " << minf[i] << ", ";
    }
    cout << endl;
}