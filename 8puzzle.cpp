#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Globals

int debug = 1;

// Functions
vector<int> process_input();
void print_puzzle(vector<int> puzzle);

// MAIN
int main()
{
    vector<int> init_puzzle = process_input();
    //print_puzzle(init_puzzle);
    return 0;
}

vector<int> process_input()
{
    int num_rows = 3;
    vector<int> init;
    string puzzle_str;
    string buffer;
    bool has_underscore = false;
    bool has_correct_num_num = false;

    cout << "Enter the initial state" << endl;
    cout << "Format: " << endl
         << "\t1 2 3" << endl
         << "\t4 _ 6" << endl
         << "\t7 8 9" << endl;
    int i = 0;
    while(getline(cin, buffer))
    {
        i++;
        puzzle_str += buffer + " ";
        if(i == 3)
            break;
    }
    
    if(debug)
        cout << "Puzzle: \n" << puzzle_str << endl;

    for(string::iterator i = puzzle_str.begin(); i != puzzle_str.end(); i++)
    {
        cout << i << ' ';
    }
    return init;
}

// void print_puzzle(vector<int> puzzle)
// {
//     int count = 0;
//     for(vector<int>::iterator i = puzzle.begin(); i != puzzle.end(); i++)
//     {
//         cout << i;
//         if(count == 2)
//         {
//             cout << endl;
//             count = 0;
//         }
//         count++;
//     }
// }