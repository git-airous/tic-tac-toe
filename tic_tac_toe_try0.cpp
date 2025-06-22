#include <bits/stdc++.h>
#include <windows.h>
#include <conio.h>
using namespace std;

void the_transition() {
    int wait = 600; // 60 seconds (1 minute)
    int count = 0;

    // Clear the input buffer before starting (else both the transition where triggering at the same time)
    while (_kbhit()) {
        _getch();  // Consume any pending key presses to reset the buffer
    }

    while (count < wait) {
        if (_kbhit()) {
            _getch();  // Consume the keypress and break out of the loop
            break;
        }
        count++;
        Sleep(100);  // Sleep for 100ms (to be a bit more accurate)
    }
}

void sample_grid() {
    cout << " 0 | 1 | 2 \n";
    cout << "---|---|---\n";
    cout << " 3 | 4 | 5 \n";
    cout << "---|---|---\n";
    cout << " 6 | 7 | 8 \n";
}

void grid_bound() {
    cout << "   |   |   \n";
    cout << "---|---|---\n";
    cout << "   |   |   \n";
    cout << "---|---|---\n";
    cout << "   |   |   \n";
}

int update_grid(vector<vector<int>>& grid, int move) {
    int row = move / 3, col = move % 3;
    if (grid[row][col] == 0) grid[row][col] = 1;
    else return 1;

    return 0;
}

void print_grid(vector<vector<int>>& grid, int turn) {
    char move = 'X';
    int update = 2;
    if (turn) {
        move = 'O';
        update = 3;
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (grid[i][j] == 0) {
                if (j < 2) cout << "   |";
            }
            else if (grid[i][j] == 1) {
                if (j < 2) cout << " " << move << " |";
                else cout << " " << move;
                grid[i][j] = update;
            }
            else if (grid[i][j] == 2) {
                if (j < 2) cout << " X |";
                else cout << " X";
            }
            else if (grid[i][j] == 3) {
                if (j < 2) cout << " O |";
                else cout << " O";
            }
        }

        if (i < 2) cout << "\n---|---|---\n";
    } cout << "\n";
}

int checker(vector<vector<int>> grid) {
    //hr/vr lines check
    for (int i = 0; i < 3; i++) {
        if (grid[i][0] != 0 && grid[i][0] == grid[i][1] && grid[i][1] == grid[i][2]) return 1;
        if (grid[0][i] != 0 && grid[0][i] == grid[1][i] && grid[1][i] == grid[2][i]) return 1;
    }

    //diagonal lines check
    if (grid[0][0] != 0 && grid[0][0] == grid[1][1] && grid[1][1] == grid[2][2]) return 1;
    if (grid[0][2] != 0 && grid[0][2] == grid[1][1] && grid[1][1] == grid[2][0]) return 1;

    //no match
    return 0;
}

int conti_or_break() {
    system("cls");
    cout << "Press 'esc key' to end the game\nelse press any other key: ";

    int wait = 600; // 60 seconds
    int count = 0;

    while (!_kbhit()) {
        count++;
        Sleep(100); // 0.1 second = 100 ms

        if (count > wait) {
            system("cls");
            cout << "It's been long enough\nSo we are ending the game\n";
            Sleep(5000);
            system("cls");
            cout << "Thank you for playing!\n";
            return 0;
        }
    }

    int ch = _getch();  // read the pressed key
    if (ch == 27) {     // ASCII 27 = Esc key
        system("cls");
        cout << "Thank you for playing!\n";
        return 0;
    }
    return 1;
}

int main() {
    //transition 0
    system("cls");
    cout << "You need 2 players for this.";
    cout.flush();
    the_transition();

    //transition 1
    system("cls");
    cout << "How to play.\n";
    cout << "-> Look at the grid and accordingly enter\n   the number when your turn comes.\n";
    sample_grid();
    cout.flush();
    the_transition();

    while (true) {
        string result = "Draw";
        int turn = 0;
        vector<vector<int>> grid(3, vector<int> (3, 0));

        // initial print of the grid
        system("cls");
        grid_bound();

        // game
        while (turn < 9) {
            if (turn % 2 == 0) {
                int move = 0;

                // user1's move
                cout << "Turn of user1\n";
                cout << "Enter the number where you want to move (number only (0 - 8)): ";
                cin >> move;

                // Check for invalid input and update the grid
                if (cin.fail()  || (move < 0 || move > 8) || update_grid(grid, move)) {  //checks if move == int (the correct datatype that we declared)
                    cin.clear(); // clear error flag
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // remove bad input
                    cout << "Invalid move! Try again." << endl;
                    the_transition();

                    // print grid
                    system("cls");
                    print_grid(grid, turn % 2);
                    continue;
                }
            }
            else {
                int move = 0;

                // user2's move
                cout << "Turn of user2\n";
                cout << "Enter the number where you want to move (number only (0 - 8)): ";
                cin >> move;

                // Check for invalid input and update the grid
                if (cin.fail()  || (move < 0 || move > 8) || update_grid(grid, move)) {  //checks if move == int (the correct datatype that we declared)
                    cin.clear(); // clear error flag
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // remove bad input
                    cout << "Invalid move! Try again." << endl;
                    the_transition();

                    // print grid
                    system("cls");
                    print_grid(grid, turn % 2);     
                    continue;
                }
            }

            // print grid
            system("cls");
            print_grid(grid, turn % 2);
            
            // check for winner
            if (checker(grid)) {
                if (turn % 2) result = "user2";
                else result = "user1";
                break;
            }

            turn++;
        }

        // result
        system("cls");
        print_grid(grid, 0);
        cout << "result: " << result;
        the_transition();

        //ask to conti or break
        system("cls");
        if (conti_or_break()) continue;
        else break;
    }

    return 0;
}
