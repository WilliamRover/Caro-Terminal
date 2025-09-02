#include <iostream>
#include <conio.h>
#include <cstring>
using namespace std;

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_ENTER 13
#define KEY_ESC 27

char **board;
int board_size;
bool is_playing = true;
int locationX, locationY;
int win_locationX, win_locationY, which_direction = 0;

// Color character: \u001b[(numcode)m
// (numcode):
// Black: 30
// Red: 31
// Green: 32
// Yellow: 33
// Blue: 34
// Magenta: 35
// Cyan: 36
// White: 37

string color(char turn, string color) {
    string color_str = "\033[" + color + "m";
    color_str = color_str + string(1, turn);
    color_str = color_str + "\033[0m";

    return color_str;
}

void init_board() {
    cout << "Input board size: ";
    cin >> board_size;
    while(board_size < 5) {
        cout << "Board size must be larger than 4: ";
        cin >> board_size;
    }

    // Cap phat bo nho cho 2d pointer
    board = new char*[board_size];
    for (int i = 0; i < board_size; i++) {
        board[i] = new char[board_size];
    }

    for (int i = 0; i < board_size; i++) {
        for (int j = 0; j < board_size; j++) {
            board[i][j] = '-';
        }
    }
}


void printInstructions() {
    cout << "Press arrow keys to move \t Press [Enter] to confirm placement \t Press [Esc] to exit the game" << endl;
}

void print_ui() {
    cout << "\033[2J\033[1;1H";
    for (int i = 0; i < board_size; i++) {
        for (int j = 0; j < board_size; j++) {
            cout << board[i][j] << "      ";
        }
        cout << endl << endl;
    }
}

int get_user_input() {
    int c = 0;
    c = getch();
    return c;
}

char turn(bool is_x_turn) {
    char current_turn;
    if (is_x_turn == true) {
        current_turn = 'x';
    } else {
        current_turn = 'o';
    }
    return current_turn;
}

void get_location(bool is_x_turn) {
    int x = 0;
    int y = 0;
    
    while (true) {
        // Update UI
        cout << "\033[2J\033[1;1H";
        printInstructions();
        for (int i = 0; i < board_size; i++) {
            for (int j = 0; j < board_size; j++) {
                if (i == x && j == y) {
                    cout << color((char)toupper(turn(is_x_turn)), "31") << "      ";
                } else {
                    cout << color(board[i][j], "36") << "      ";
                }
            }
            cout << endl << endl;
        }   

        int userInput = get_user_input();
        if (userInput == KEY_DOWN) {
            if (x != board_size - 1) {
                x = x + 1;
            }
        } else if (userInput == KEY_UP) {
            if (x != 0) {
                x = x - 1;
            }
        } else if (userInput == KEY_RIGHT) {
            if (y != board_size - 1) {
                y = y + 1;
            }
        } else if (userInput == KEY_LEFT) {
            if (y != 0) {
                y = y - 1;
            }
        } else if (userInput == KEY_ENTER) {
            if (board[x][y] != '-') {
                continue;
            }
            locationX = x;
            locationY = y;
            return;
        } else if (userInput == KEY_ESC) {
            is_playing = false;
            return;
        }
    }
}

bool check_win(bool is_x_turn) {
    int x = locationX;
    int y = locationY;
    bool win = false;

    int score = 0;

    int dx_s[4] = {0, 1, 1, -1};
    int dy_s[4] = {1, 0, 1, 1};

    // General
    for (int k = 0; k < 4; k++) {
        int dx = dx_s[k];
        int dy = dy_s[k];

        for (int i = -3; i < 4; i++) {
            int cur_x = x + i * dx;
            int cur_y = y + i * dy;

            if (cur_x < 0 || cur_y < 0) {
                continue;
            } else if (cur_x > board_size - 1 || cur_y > board_size - 1) {
                break;
            }

            if (board[cur_x][cur_y] != turn(is_x_turn)) {
                score = 0;
                continue;
            }
            score += 1;
            if (score == 4) {
                win = true;
                break;
            }            
        }
    }
    
    return win;
}

// bool check_win(bool is_x_turn) {
//     int x = locationX;
//     int y = locationY;
//     bool win = false;

//     int score = 0;
//     // horizontal
//     for (int i = -3; i < 4; i++) {
//         if (y + i < 0) {
//             continue;
//         } else if (y + i > board_size - 1) {
//             break;
//         }
//         if (board[x][y + i] != turn(is_x_turn)) {
//             score = 0;
//             continue;
//         }
//         score += 1;
//         if (score == 4) {
//             win = true;
//         }
//     }
//     score = 0;
//     //vertical
//     for (int i = -3; i < 4; i++) {
//         if (x + i < 0) {
//             continue;
//         } else if (x + i > board_size - 1) {
//             break;
//         }
//         if (board[x + i][y] != turn(is_x_turn)) {
//             score = 0;
//             continue;
//         }
//         score += 1;
//         if (score == 4) {
//             win = true;
//         }
//     }
//     score = 0;
//     //dialogal 1
//     for (int i = -3; i < 4; i++) {
//         if (x + i < 0 || y + i < 0) {
//             continue;
//         } else if (x + i > board_size - 1 || y + i > board_size - 1) {
//             break;
//         }
//         if (board[x + i][y + i] != turn(is_x_turn)) {
//             score = 0;
//             continue;
//         }
//         score += 1;
//         if (score == 4) {
//             win = true;
//         }
//     }
//     score = 0;
//     //dialogal 2
//     for (int i = -3; i < 4; i++) {
//         if (x - i < 0 || y + i < 0) {
//             continue;
//         } else if (x - i > board_size - 1 || y + i > board_size - 1) {
//             break;
//         }
//         if (board[x - i][y + i] != turn(is_x_turn)) {
//             score = 0;
//             continue;
//         }
//         score += 1;
//         if (score == 4) {
//             win = true;
//         }
//     }
//     score = 0;
    
//     return win;
// }

bool play_again() {
    bool left = true;
    string yes_or_no[] = {"Play again", "Exit"};
    cout << "\033[33m" << yes_or_no[0] << "\033[0m" << " | " << yes_or_no[1];
    while (true) {
        int userInput = get_user_input();
        cout << "\033[2J\033[1;1H";

        if (userInput == KEY_LEFT) {
            left = true;
            cout << endl << "\033[33m" << yes_or_no[0] << "\033[0m" << " | " << yes_or_no[1];
        } else if (userInput == KEY_RIGHT) {
            left = false;
            cout << endl << yes_or_no[0] << " | " << "\033[33m" << yes_or_no[1] << "\033[0m";
        } else if (userInput == KEY_ENTER) {
            if (left == true) {
                return true;
            } else {
                return false;
            }
        }
    }
}

int main() {
    init_board();
    bool is_x_turn = true;
    
    while(is_playing) {
        print_ui();
        get_location(is_x_turn);
        board[locationX][locationY] = turn(is_x_turn);
        if (check_win(is_x_turn) == true) {
            cout << color(char(toupper(turn(is_x_turn))), "33") << " win the game!" << endl;
            is_playing = false;
            cout << "Press <- or -> to choose" << endl;
            if (play_again() == true) {
                is_playing = true;
                cout << endl;
                init_board();
            }
        }
        is_x_turn = !is_x_turn;
    }
}