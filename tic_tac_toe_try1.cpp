#include <bits/stdc++.h>
#include <windows.h>
#include <conio.h>
using namespace std;

// Utility / UI --X--X--

// Pause with optional early keypress
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

// Show example grid with indices
void sample_grid() {
    cout << " 0 | 1 | 2 \n";
    cout << "---|---|---\n";
    cout << " 3 | 4 | 5 \n";
    cout << "---|---|---\n";
    cout << " 6 | 7 | 8 \n";
}

// Show actual grid with grey indices
void grid_bound() {
    cout << " \033[90m0\033[0m | \033[90m1\033[0m | \033[90m2\033[0m \n";
    cout << "---|---|---\n";
    cout << " \033[90m3\033[0m | \033[90m4\033[0m | \033[90m5\033[0m \n";
    cout << "---|---|---\n";
    cout << " \033[90m6\033[0m | \033[90m7\033[0m | \033[90m8\033[0m \n";
}

// Place a move
int update_grid(vector<vector<int>>& grid, int move, int turn) {
    int row = move/3, col = move%3;
    if (grid[row][col] != 0) return 1;
    grid[row][col] = (turn == 0 ? 2 : 3);  // 2 = X, 3 = O
    return 0;
}

// Render and print grid
void print_grid(vector<vector<int>>& grid, int turn) {
    char move = 'X';
    int update = 2;
    if (turn) {
        move = 'O';
        update = 3;
    }

    int grid_no = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (grid[i][j] == 0) {
                if (j < 2) cout << " \033[90m" << grid_no << "\033[0m |";
                else cout << " \033[90m" << grid_no << "\033[0m";
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
            grid_no++;
        }

        if (i < 2) cout << "\n---|---|---\n";
    }
    cout << "\n";
}

// Prompt to continue or break
int conti_or_break() {
    system("cls");
    cout << "Press 'Esc' to quit, any other key to continue: ";

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

// to display the options in mod
void show_mode_menu(int selected) {
    system("cls");
    cout << "Select game mode using arrow keys and press Enter:\n";
    
    string options[2] = {"Play with another player", "Play with bot"};

    if (selected == 0) {
        cout << "\033[34m--> " << options[0] << "\033[0m\n   " << options[1] << "\n";
    } else {
        cout << "  " << options[0] << "\n\033[34m--> " << options[1] << "\033[0m\n";
    }
}

// to get the mod
int select_mode() {
    int selected = 0;
    const int numOptions = 2;
    show_mode_menu(selected);

    while (true) {
        int key = _getch();
        if (key == 224) { // Arrow key prefix
            int arrow = _getch();
            if (arrow == 72) { // Up arrow
                selected = (selected - 1 + numOptions) % numOptions;
            } else if (arrow == 80) { // Down arrow
                selected = (selected + 1) % numOptions;
            }
            show_mode_menu(selected);
        } else if (key == 13) { // Enter key
            return selected;
        }
    }
}

// to display the options in move while playing with bot
void show_first_move_menu(int selected) {
    system("cls");
    cout << "Select to whom you want to take the first move\nusing arrow keys and press Enter:\n";
    
    string options[2] = {"If you want to play first", "If you want bot play first"};

    if (selected == 0) {
        cout << "\033[34m--> " << options[0] << "\033[0m\n   " << options[1] << "\n";
    } else {
        cout << "  " << options[0] << "\n\033[34m--> " << options[1] << "\033[0m\n";
    }
}

// to get the move
int select_first_move() {
    int selected = 0;
    const int numOptions = 2;
    show_first_move_menu(selected);

    while (true) {
        int key = _getch();
        if (key == 224) { // Arrow key prefix
            int arrow = _getch();
            if (arrow == 72) { // Up arrow
                selected = (selected - 1 + numOptions) % numOptions;
            } else if (arrow == 80) { // Down arrow
                selected = (selected + 1) % numOptions;
            }
            show_first_move_menu(selected);
        } else if (key == 13) { // Enter key
            return selected;
        }
    }
}


// Unified Win Logic --X--X--

// returns +10 if O wins, -10 if X wins, 0 otherwise
int gameResult(const vector<vector<int>>& grid) {
    // hr/vr lines check
    for (int i = 0; i < 3; i++) {
        if (grid[i][0] && grid[i][0] == grid[i][1] && grid[i][1] == grid[i][2])
            return (grid[i][0] == 3? +10 : -10);
        if (grid[0][i] && grid[0][i] == grid[1][i] && grid[1][i] == grid[2][i])
            return (grid[0][i] == 3? +10 : -10);
    }

    //diagonal lines check
    if (grid[0][0] && grid[0][0] == grid[1][1] && grid[1][1] == grid[2][2])
        return (grid[0][0] == 3? +10 : -10);
    if (grid[0][2] && grid[0][2] == grid[1][1] && grid[1][1] == grid[2][0])
        return (grid[0][2] == 3? +10 : -10);

    //no match
    return 0;
}

// check if some one won
bool isWin(const vector<vector<int>>& grid) {
    return gameResult(grid) != 0;
}


int minimax(vector<vector<int>>& grid, bool isBot) {
    int score = gameResult(grid);
    if (score == 10 || score == -10) return score;
    bool movesLeft = false;
    for (auto& row: grid) {
        for (int c: row) {
            if (c == 0) { 
                movesLeft = true;
                break;
            }
        }
    }
    if (!movesLeft) return 0;

    int best = isBot? -1000: +1000;
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        if (grid[i][j] == 0) {
          grid[i][j] = isBot? 3: 2;
          int val = minimax(grid, !isBot);
          grid[i][j] = 0;
          best = isBot? max(best,val): min(best,val);
        }
      }
    }
    return best;
}

pair<int,int> findBestMove(vector<vector<int>>& grid) {
    int bestVal = -1000;
    pair<int,int> move{-1, -1};
    for (int i = 0; i < 3; i++){
      for (int j = 0; j < 3; j++){
        if (grid[i][j] == 0) {
          grid[i][j] = 3;
          int val = minimax(grid, false);
          grid[i][j] = 0;
          if (val > bestVal) {
            bestVal = val;
            move = {i, j};
          }
        }
      }
    }
    return move;
}


// Game Modes --X--X--

void players_vs_player() {
    // how to play
    system("cls");
    cout << "How to play.\n";
    cout << "-> Grid positions will be shown in grey;\n   pick a number (0-8) on your turn.\n";
    sample_grid();
    cout.flush();
    the_transition();

    // initialize
    vector<vector<int>> grid(3, vector<int>(3, 0));
    string result = "Draw";
    int turn = 0;
    system("cls"); 
    grid_bound();

    // moves (in motion)
    while (turn < 9) {
        int move; 
        cout << "\nTurn of "<< (turn % 2 == 0? "User1": "User2") <<"\nEnter (0-8): ";
        cin >> move;

        // Check for invalid input and update the grid
        if (cin.fail() || (move < 0|| move > 8) || update_grid(grid, move, turn % 2)) {
            cin.clear(); // clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // remove bad input
            cout << "Invalid move! Try again.\n";
            the_transition();

            // print grid
            system("cls");
            print_grid(grid, turn % 2);
            continue;
        }

        // print grid
        system("cls");
        print_grid(grid, turn % 2);

        // check for winner
        if (isWin(grid)) {
            result = (turn % 2 == 0? "User1 wins!": "User2 wins!");
            break;
        }

        turn++;
    }

    // result
    system("cls");
    print_grid(grid, 0);
    cout << "Result: " << result << "\n";
    the_transition();
}

void player_vs_bot() {
    // how to play
    system("cls");
    cout << "How to play.\n";
    cout << "-> Grid positions will be shown in grey;\n   pick a number (0-8) on your turn.\n";
    sample_grid();
    cout.flush();
    the_transition();

    // first move/ initialize
    int first = select_first_move();
    vector<vector<int>> grid(3, vector<int>(3, 0));
    string result = "Draw";
    int turn = 0;
    system("cls");
    grid_bound();

    // move (in motion)
    while (turn < 9) {
        bool userTurn = ((turn % 2) == first);
        if (userTurn) {
            int move;
            cout << "\nYour turn.\nEnter (0-8): ";
            cin >> move;

            // Check for invalid input and update the grid
            if (cin.fail() || (move < 0 || move > 8) || update_grid(grid, move, 0)) {
                cin.clear(); // clear error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // remove bad input
                cout << "Invalid move! Try again.\n";
                the_transition();

                // print grid
                system("cls");
                print_grid(grid, turn % 2);
                continue;
            }
        } else {
            pair<int, int> p = findBestMove(grid);
            int r = p.first;
            int c = p.second;
            grid[r][c] = 3;
        }

        // print grid
        system("cls");
        print_grid(grid, turn % 2);

        // check for result
        if (isWin(grid)) {
            result = userTurn? "You win!": "Bot wins!";
            break;
        }

        turn++;
    }

    // result
    system("cls");
    print_grid(grid, 0);
    cout << "Result: " << result << "\n";
    the_transition();
}


// main --X--X--

int main() {
    // initial welcome msg
    system("cls");
    cout << "Welcome to the tic tac toe game";
    cout.flush();
    the_transition();;

    while (true) {
        // mode -> user or bot
        int mode = select_mode();

        // user vs user
        if (mode == 0) players_vs_player();
        // user vs bot
        else player_vs_bot();

        // ask to conti or break
        system("cls");
        if (conti_or_break()) continue;
        else break;
    }

    return 0;
}

