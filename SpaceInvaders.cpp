//Program: The arcade game Space Invaders
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <iomanip>
#include <thread>
#include<fstream>
#include<string>
#include<ctime>
using namespace std;
using namespace std::this_thread;
using namespace std::chrono;

/*
easy mode:
5 bunkers
5 rows of aliens
11 aliens in each row
...
hard mode:
3 bunkers
7 rows of aliens
15 aliens in each row
*/

//fills the screen array with default values
void defaultEasyMode();
//fills the screen array with default values
void defaultHardMode();
//border of the game
void defaultScreen();
//main easy mode game
void easyMode();
//main hard mode game
void hardMode();
//changes the text and background color
void setColor(int, int);
//prints the screen
void printScreen();
//delays the game
void sleepScreen();
//clears the screen array and fills it with the new values
void fillScreen(int, int, int);
//update the array's info
void updateScreen(int, int);
//
void movePlayer(char, int, int, int);
//
void moveAliens(int, int);
//
void checkHits(int, int, int);
//
void generateAliensBullets();
//
void Leaderboard();
//checks if the player has lost
void checkIfGameOver(int);
//displays a message and end the game
void endGame();
//
void goToNextLevel(int, int);
//
void pause(int, int, int);
//load the game
void load();
struct playerInfo{
    bool gameover = false;
    int x, y, score {0}, health {100},
        time, speed, level {1};
    string name;
};

//increase the capacity of the dynamic array
void resize(playerInfo*&, int&, int);
//add player information based on scores to the array
void append(playerInfo*&, int&, int&, int, playerInfo);
//save the game
void save(int rowAlien, int rowBunker, int colAlien);

struct screenInfo{
    int color;
    string character;
};
struct alienInfo{
    bool alive = true;
    int x, y, score, damage, type;
};
struct bunkerInfo{
    bool status = true;
    int x, y;
};
struct playerBulletInfo{
    bool hit = false, used = false;
    int x, y;
};
struct alienBulletInfo{
    bool used = false;
    int x, y, damage, index;
};
struct bulletHelp{
    bool used = false;
    int i, j, row;
};
struct spaceship{
    bool used = false, inside = false;
    int x, y, score {100};
};
const int rowScr = 36, colScr = 97;
//if true, aliens are moving to the right
//if false, aliens are moving to the left
bool alienDirection = true;
//determines the shape of the alien's legs
bool shape = true, endgame = false,
     easy, newGame = false, loadGame = false;
int playerCount = 1, capacity = 1, randInterval;
playerInfo* ranking = new playerInfo[capacity];
playerInfo player;
screenInfo screen[rowScr][colScr];
alienInfo** alien;
bunkerInfo* bunker;
playerBulletInfo pBullet;
alienBulletInfo* aBullet;
bulletHelp* bHelp;
spaceship redSpcShip;
clock_t start, finish, startLevel, timePassed = 0;

int main() 
{
    srand(static_cast<unsigned int>(time(nullptr)));
    while(true){
        system("cls");
        cout << "Space Invaders\n"
            << "1. New Game\n"
            << "2. Load Game\n"
            << "3. How to Play\n"
            << "4. Leaderboard\n"
            << "0. Exit\n";
        char choice;
        choice = getch();
        switch (choice)
        {
        //New Game
        case '1':
            newGame = false;
            defaultScreen();
            system("cls");
            cout << "Name: \n";
            cin >> player.name;
            system("cls");
            cout << "Choose difficulty\n";
            cout << "1. Easy\n";
            cout << "2. Hard\n";
            char level;
            level = getch();
            switch (level)
            {
            case '1':
                easyMode();
                break;
            case '2':
                hardMode();
                break;
            }
            break;
        //load game
        case '2':
            load();
            loadGame = true;
            if(easy){
                easyMode();
            }
            else{
                hardMode();
            }
            break;
        case '3':
            break;
        case '4':
            Leaderboard();
            char choice2;
            cout << "1. Back \n";
            cout << "2.Exit \n";
            choice2 = getch();
            switch (choice2)
            {
                case '1':
                    break;
                case '2':
                    return 0;
            }
            break;
        case '0':
            return 0;
        }
    }
  return 0;
}

void defaultScreen(){
    for(int i = 0; i < 36; i++){
        for(int j = 0; j < 97; j++){
            screen[i][j].character = ' ';
        }
    }
    //the corners
    screen[35][0].character = "╰";
    screen[35][96].character = "╯";
    //the border
    for(int i = 0; i < 35; i++)
    {
        screen[i][0].character = "│";
    }
    for(int i = 0; i < 35; i++)
    {
        screen[i][96].character = "│";
    }
    for(int i = 1; i < 96; i++){
        screen[35][i].character = "─";
    }
    //writes the player's spaceship coordinates in player array
    player.x = 48;
    player.y = 33;
    endgame = false;
    player.gameover = false;
    loadGame = false;
    newGame = false;
}

void defaultEasyMode(){
    //writes the aliens coordinates in alien array
    //the row the alien is in (the y coordinate)
    for(int i = 6, p = 0; i <= 14;i += 2, p++){
        //the column the alien is in (the x coordinate)
        for(int j = 6, q = 0; j <= 46; j += 4, q++){
            switch (i){
            //row 6 is type 3 alien
            case 6:
                alien[p][q].type = 3;
                alien[p][q].score = 40;
                alien[p][q].damage = 10;
                break;
            //rows 8 and 10 are type 2 aliens
            case 8:
            case 10:
                alien[p][q].type = 2;
                alien[p][q].score = 20;
                alien[p][q].damage = 5;
                break;
            //rows 12 and 14 are type 1 aliens
            case 12:
            case 14:
                alien[p][q].type = 1;
                alien[p][q].score = 10;
                alien[p][q].damage = 1;
                break;
            }
            alien[p][q].x = j;
            alien[p][q].y = i;
        }
    }
    //writes the bunkers coordinates in bunker array
    for(int i = 0, p = 8; i < 5; i++, p += 20){
        bunker[i].x = p;
        bunker[i].y = 29;
    }
}

void defaultHardMode(){
    for(int i = 6, p = 0; i <= 18;i += 2, p++){
        //the column the alien is in (the x coordinate)
        for(int j = 6, q = 0; j <= 62; j += 4, q++){
            switch (i){
            //row 6 and 8 are type 3 aliens
            case 6:
            case 8:
                alien[p][q].type = 3;
                alien[p][q].score = 40;
                alien[p][q].damage = 10;
                break;
            //rows 10 and 12 are type 2 aliens
            case 10:
            case 12:
                alien[p][q].type = 2;
                alien[p][q].score = 20;
                alien[p][q].damage = 5;
                break;
            //rows 14, 16 and 18 are type 1 aliens
            case 14:
            case 16:
            case 18:
                alien[p][q].type = 1;
                alien[p][q].score = 10;
                alien[p][q].damage = 1;
                break;
            }
            alien[p][q].x = j;
            alien[p][q].y = i;
        }
    }
    //writes the bunkers coordinates in bunker array
    for(int i = 0, p = 7; i < 3; i++, p += 34){
        bunker[i].x = p;
        bunker[i].y = 29;
    }
}

void easyMode(){
    int rowAlien = 5, colAlien = 11, rowBunker = 5;
    easy = true;
    if(!loadGame){
        //create a two dimensional alien array for easy mode
        alien = new alienInfo* [rowAlien];
        for(int i = 0; i < rowAlien; i++){
            alien[i] = new alienInfo[colAlien];
        }
        //create a two dimensional bunker array for easy mode
        bunker = new bunkerInfo[rowBunker];
        //creates an array for aliens bullets
        aBullet = new alienBulletInfo[colAlien];
        bHelp = new bulletHelp[colAlien];
        for(int i = 0; i < colAlien; i++){
            bHelp[i].i = rowAlien - 1;
            bHelp[i].j = i;
            bHelp[i].row = rowAlien - 1;
        }
        defaultEasyMode();
    }
    fillScreen(rowAlien, colAlien, rowBunker);
    printScreen();
    start = clock();
    //beginning of level 1
    startLevel = clock();
    player.speed = 7;
    while(!endgame){
        //if the player has hit a key
        if(kbhit()){
            char key = getch();
            movePlayer(key, rowAlien, colAlien, rowBunker);
        }
        if(!easy){
            rowAlien = 7;
            colAlien = 15;
            rowBunker = 3;
        }
        if(newGame){
            break;
        }
        generateAliensBullets();
        updateScreen(rowAlien, colAlien);
        checkHits(rowAlien, colAlien, rowBunker);
        fillScreen(rowAlien, colAlien, rowBunker);
        printScreen();
        sleep_for(milliseconds(200));
        checkIfGameOver(rowAlien);
        goToNextLevel(rowAlien, colAlien);
        //red spaceship
        if(!redSpcShip.used){
            //at least every 15 and at most every 45 seconds
            randInterval = rand() % 15 + 30;
            redSpcShip.used = true;
        }
        //if it has left the screen
        if(!redSpcShip.inside){
            if((clock() / CLOCKS_PER_SEC) % randInterval == 0){
                redSpcShip.x = 100;
                redSpcShip.y = 2;
                redSpcShip.inside = true;
            }
        }
    }
    if(newGame){
        return;
    }
    endGame();
    cout << "Score: " << player.score << endl;
    cout << "Time: " << player.time << endl; 
}

void sleepScreen(){
    if(clock() / CLOCKS_PER_SEC < 10){
        sleep_for(milliseconds(500));
    }
}

void updateScreen(int rowAln, int colAln){
    //for the first 6 seconds, move aliens every 0.7 seconds
    if((clock() - timePassed / CLOCKS_PER_SEC) <= 10 && (clock() / 100) % 8 == 0){
        moveAliens(rowAln, colAln);
    }
    //for the second 6 seconds, move aliens every 0.5 seconds
    else if((clock() - timePassed / CLOCKS_PER_SEC) <= 20 && (clock() / CLOCKS_PER_SEC) >= 10 && (clock() / 100) % 6 == 0){
        moveAliens(rowAln, colAln);
    }
    else if((clock() - timePassed / CLOCKS_PER_SEC) <= 30 && (clock() / CLOCKS_PER_SEC) >= 20 &&(clock() / 100) % 4 == 0){
        moveAliens(rowAln, colAln);
    }
    else if((clock() - timePassed / CLOCKS_PER_SEC) <= 40 && (clock() / CLOCKS_PER_SEC) >= 30 && (clock() / 100) % 3 == 0){
        moveAliens(rowAln, colAln);
    }
    else if((clock() - timePassed / CLOCKS_PER_SEC) >= 40 && (clock() / 100) % 2 == 0){
        moveAliens(rowAln, colAln);
    }
    //moves the player's bullet
    if(pBullet.used){
        pBullet.y--;
    }
    //if the player's bullet leaves the screen
    if(pBullet.y == 0){
        pBullet.used = false;
    }
    //moves the alien's bullet / level 1
    for(int i = 0; i < 2 * player.level + 1; i++){
        //if the alien's bullet leaves the screen
        if(aBullet[i].y == 33){
            aBullet[i].used = false;
            bHelp[aBullet[i].index].used = false;
        }
        aBullet[i].y++;
    }
    //moves the red spaceship
    if(redSpcShip.used == true && redSpcShip.inside == true){
        redSpcShip.x--;
    }
}


void checkHits(int rowAln, int colAln, int rowBnk){
    //the player's bullet
    //checks aliens
    for(int i = 0; i < rowAln; i++){
        for(int j = 0; j < colAln; j++){
            if((pBullet.used == true)&&(alien[i][j].alive == true)&&(pBullet.x == alien[i][j].x || pBullet.x == alien[i][j].x - 1
                || pBullet.x == alien[i][j].x + 1)&&(pBullet.y == alien[i][j].y + 1 || pBullet.y == alien[i][j].y)){
                pBullet.hit = true;
                alien[i][j].alive = false;
                player.score += alien[i][j].score;
                pBullet.used = false;
                //chooses the above alive alien
                for(int k = 0; k < colAln; k++){
                    if(bHelp[k].i == i && bHelp[k].j == j){
                        bHelp[k].i--;
                    }
                }
            }
        }
    }
    //checks bunkers
    for(int i = 0; i < rowBnk; i++){
        //the below loop is for checking the x coordinate of the bunker
        for(int k = -3; k < 4; k++){
            if((bunker[i].status == true) && (pBullet.x == bunker[i].x + k)
                && (pBullet.y == bunker[i].y || pBullet.y == bunker[i].y + 1)){
                bunker[i].status = false;
                pBullet.hit = true;
                pBullet.used = false;
                break;
            }
        }
    }
    //checks red spaceship
    if(redSpcShip.inside == true && pBullet.x == redSpcShip.x && pBullet.y == redSpcShip.y){
        player.score += redSpcShip.score;
        pBullet.hit = true;
        pBullet.used = false;
        redSpcShip.used = false;
    }
    //the alien's bullets
    for(int i = 0; i < 2 * player.level + 1; i++){
        //checks bunkers
        for(int j = 0; j < rowBnk; j++){
            //the below loop is for checking the x coordinate of the bunker
            for(int k = -3; k < 4; k++){
                if((bunker[j].status == true)&&(aBullet[i].x == bunker[j].x + k)
                    && (aBullet[i].y == bunker[j].y || aBullet[i].y == bunker[j].y + 1)){
                    bunker[j].status = false;
                    aBullet[i].used = false;
                    bHelp[aBullet[i].index].used = false;
                    break;
                }
            }
        }
        //checks player's spaceship
        if(((aBullet[i].x == player.x - 2 || aBullet[i].x == player.x + 2 ) && (aBullet[i].y == player.y)) ||
           ((aBullet[i].x == player.x - 1 || aBullet[i].x == player.x || aBullet[i].x == player.x + 1)&&(aBullet[i].y = player.y - 1))){
            player.health -= aBullet[i].damage;
            aBullet[i].used = false;
            bHelp[aBullet[i].index].used = false;
        }
    }
}

void printScreen(){
    system("cls");
    cout << "╭";
    for (int i = 0; i < 95; i++){
        cout << "─";
    }
    cout << "╮" << endl;
    cout << "│";
    for (int i = 0; i < 95; i++){
        cout << " ";
    }
    cout << "│" << endl;
    cout << "│  "; 
    cout << "SCORE " << setw(6) << player.score;
    for (int i = 0; i < 30; i++){
        cout << " ";
    }
    cout << "LEVEL " << player.level;
    for (int i = 0; i < 30; i++){
        cout << " ";
    }
    finish = clock() - start;
    player.time = (int)finish / CLOCKS_PER_SEC;
    cout << "TIME " << setw(7) << player.time << "  │" << endl;
    //edit later
    cout << "│  HI-SCORE " << setw(6) ;
    for (int i = 0; i < 65; i++){
        cout << " ";
    }
    cout << "HEALTH " << setw(3) << player.health << "    │" << endl;
    for(int j = 0 ; j < 2 ; j++){
        cout << "│";
        for (int i = 0; i < 95; i++){
            cout << " ";
        }
        cout << "│" << endl;
    }
    //prints the screen array
    for(int i = 0; i < 36; i++){
        for(int j = 0; j < 97; j++){
            cout << screen[i][j].character;
        }
        cout << endl;
    }
}

void fillScreen(int rowAln, int colAln, int rowBnk){
    //clears the screen array except the border
    for(int i = 0; i < 35; i++){
        for(int j = 1; j < 96; j++){
            screen[i][j].character = ' ';
        }
    }
    //fills screen array with the head of the aliens
    for(int i = 0; i < rowAln; i++){
        switch(alien[i][0].type){
        //type 1 alien
        case 1:
            for(int j = 0; j < colAln; j++){
                if(alien[i][j].alive){
                    //the head
                    for(int k = -1; k < 2; k++){
                        screen[alien[i][j].y][alien[i][j].x + k].character = "▄";
                    }
                }
            }
            break;
        //type 2 alien
        case 2:
            for(int j = 0; j < colAln; j++){
                if(alien[i][j].alive){
                    //the head
                    for(int k = -1; k < 2; k += 2){
                        screen[alien[i][j].y][alien[i][j].x + k].character = "▄";
                    }
                    screen[alien[i][j].y][alien[i][j].x].character = "█";
                }
            }
            break;
        //type 3 alien
        case 3:
            for(int j = 0; j < colAln; j++){
                if(alien[i][j].alive){
                    //the head
                    for(int k = -1; k < 2; k += 2){
                        screen[alien[i][j].y][alien[i][j].x + k].character = "█";
                    }
                    screen[alien[i][j].y][alien[i][j].x].character = "▄";
                }
            }
            break;
        }
    }
    //fills screen array with the legs of the aliens
    for(int i = 0; i < rowAln; i++){
        for(int j = 0; j < colAln; j++){
            if(alien[i][j].alive){
                //the legs are ┗ ┛
                if(shape){
                    screen[alien[i][j].y + 1][alien[i][j].x - 1].character = "┗";
                    screen[alien[i][j].y + 1][alien[i][j].x + 1].character = "┛";
                }
                //the legs are ┛ ┗
                else{
                    screen[alien[i][j].y + 1][alien[i][j].x - 1].character = "┛";
                    screen[alien[i][j].y + 1][alien[i][j].x + 1].character = "┗";
                }
            }
        }
    }
    //fills screen array with bunkers
    for(int i = 0; i < rowBnk; i++){
        if(bunker[i].status){
            //the top
            for(int k = -3 ; k < 4; k += 6){
                screen[bunker[i].y][bunker[i].x + k].character = "▄";
            }
            for(int k = -2; k < 3; k++){
                screen[bunker[i].y][bunker[i].x + k].character = "█";
            }
            //the bottom
            for(int k = -3; k < 3; k += 5){
                screen[bunker[i].y + 1][bunker[i].x + k].character = "█";
                screen[bunker[i].y + 1][bunker[i].x + k + 1].character = "█";
            }
        }
    }
    //fills screen array with player's spaceship
    ////the top
    screen[player.y - 1][player.x].character = "█";
    for(int i = -1; i < 2; i += 2){
        screen[player.y - 1][player.x + i].character = "▄";
    }
    ////the bottom
    for(int i = -2; i < 3; i++){
        screen[player.y][player.x + i].character = "█";
    }
    //puts player's bullet in the screen array
    if(pBullet.used == true){
        screen[pBullet.y][pBullet.x].character = "🠅";
    }
    //puts alien's bullet in the screen array
    for(int i = 0; i < 2 * player.level + 1; i++){
        if(aBullet[i].used){
            screen[aBullet[i].y][aBullet[i].x].character = "🠇";
        }
    }
    //puts the red spaceship in the screen array
    if(redSpcShip.used == true && redSpcShip.inside == true){
        switch(redSpcShip.x)
        {
        //entering the screen
        case 91:
            screen[redSpcShip.y][redSpcShip.x + 4].character = "▄";
            screen[redSpcShip.y + 1][redSpcShip.x + 4].character = "▄";
        case 92:
            screen[redSpcShip.y][redSpcShip.x + 3].character = "█";
        case 93:
            screen[redSpcShip.y][redSpcShip.x + 2].character = "▄";
            screen[redSpcShip.y - 1][redSpcShip.x + 2].character = "▄";
        case 94:
            screen[redSpcShip.y][redSpcShip.x + 1].character = "█";
            screen[redSpcShip.y - 1][redSpcShip.x + 1].character = "▄";
        case 95:
            screen[redSpcShip.y][redSpcShip.x].character = "▄";
            screen[redSpcShip.y - 1][redSpcShip.x].character = "▄";
            screen[redSpcShip.y + 1][redSpcShip.x].character = "▄";
        case 96:
            screen[redSpcShip.y][redSpcShip.x - 1].character = "█";
            screen[redSpcShip.y - 1][redSpcShip.x - 1].character = "▄";
        case 97:
            screen[redSpcShip.y][redSpcShip.x - 2].character = "▄";
            screen[redSpcShip.y - 1][redSpcShip.x - 2].character = "▄";
        case 98:
            screen[redSpcShip.y][redSpcShip.x - 3].character = "█";
        case 99:
            screen[redSpcShip.y][redSpcShip.x - 4].character = "▄";
            screen[redSpcShip.y + 1][redSpcShip.x - 4].character = "▄";
            break;
        //exiting the screen█
        case 4:
            screen[redSpcShip.y][redSpcShip.x - 3].character = "█";
        case 3:
            screen[redSpcShip.y][redSpcShip.x - 2].character = "▄";
            screen[redSpcShip.y - 1][redSpcShip.x - 2].character = "▄";
        case 2:
            screen[redSpcShip.y][redSpcShip.x - 1].character = "█";
            screen[redSpcShip.y - 1][redSpcShip.x - 1].character = "▄";
        case 1:
            screen[redSpcShip.y][redSpcShip.x].character = "▄";
            screen[redSpcShip.y - 1][redSpcShip.x].character = "▄";
            screen[redSpcShip.y + 1][redSpcShip.x].character = "▄";
        case 0:
            screen[redSpcShip.y][redSpcShip.x + 1].character = "█";
            screen[redSpcShip.y - 1][redSpcShip.x + 1].character = "▄";
        case -1:
            screen[redSpcShip.y][redSpcShip.x + 2].character = "▄";
            screen[redSpcShip.y - 1][redSpcShip.x + 2].character = "▄";
        case -2:
            screen[redSpcShip.y][redSpcShip.x + 3].character = "█";
        case -3:
            screen[redSpcShip.y][redSpcShip.x + 4].character = "▄";
            screen[redSpcShip.y][redSpcShip.x + 4].character = "█";
            break;
        //of screen
        case -4:
            redSpcShip.used = false;
            redSpcShip.inside = false;
            break;
        default:
            screen[redSpcShip.y][redSpcShip.x - 4].character = "▄";
            screen[redSpcShip.y + 1][redSpcShip.x - 4].character = "▄";
            screen[redSpcShip.y][redSpcShip.x - 3].character = "█";
            screen[redSpcShip.y][redSpcShip.x - 2].character = "▄";
            screen[redSpcShip.y - 1][redSpcShip.x - 2].character = "▄";
            screen[redSpcShip.y][redSpcShip.x - 1].character = "█";
            screen[redSpcShip.y - 1][redSpcShip.x - 1].character = "▄";
            screen[redSpcShip.y][redSpcShip.x].character = "▄";
            screen[redSpcShip.y - 1][redSpcShip.x].character = "▄";
            screen[redSpcShip.y + 1][redSpcShip.x].character = "▄";
            screen[redSpcShip.y][redSpcShip.x + 4].character = "▄";
            screen[redSpcShip.y + 1][redSpcShip.x + 4].character = "▄";
            screen[redSpcShip.y][redSpcShip.x + 3].character = "█";
            screen[redSpcShip.y][redSpcShip.x + 2].character = "▄";
            screen[redSpcShip.y - 1][redSpcShip.x + 2].character = "▄";
            screen[redSpcShip.y][redSpcShip.x + 1].character = "█";
            screen[redSpcShip.y - 1][redSpcShip.x + 1].character = "▄";
        }
    }
}

void movePlayer(char key, int rowAln, int colAln, int rowBnk){
    switch(key){
    //moves left
    case 'a':
        player.x--;
        break;
    //moves right
    case 'd':
        player.x++;
        break;
    //pause
    case 'p':
        pause(rowAln, colAln, rowBnk);
        break;
    //shoots
    case ' ':
        if(!pBullet.used){
            pBullet.x = player.x;
            pBullet.y = player.y - 2;
            pBullet.used = true;
        }
    }
}

void generateAliensBullets(){
    int len;
    switch(player.level){
    case 1:
        len = 3;
        break;
    case 2:
        len = 5;
        break;
    case 3:
        len = 7;
        break;
    case 4:
        len = 9;
        break;
    case 5:
        len = 11;
    }
    int counter = 0;
    while(counter < len){
        while(aBullet[counter].used){
            counter++;
            if(counter == len){
            break;
            }
        }
        if(counter == len){
            break;
        }
        int randNum = rand() % 11;
        if(bHelp[randNum].used){
            continue;
        }
        if(bHelp[randNum].row == -1){
            continue;
        }
        aBullet[counter].x = alien[bHelp[randNum].i][bHelp[randNum].j].x;
        aBullet[counter].y = alien[bHelp[randNum].i][bHelp[randNum].j].y;
        aBullet[counter].damage = alien[bHelp[randNum].i][bHelp[randNum].j].damage;
        aBullet[counter].index = randNum;
        aBullet[counter].used = true;
        bHelp[randNum].used = true;
        counter++;
    }
}

void hardMode(){
    int rowAlien = 7, colAlien = 15, rowBunker = 3;
    easy = false;
    if(!loadGame){
        //create a two dimensional alien array for hard mode
        alien = new alienInfo* [rowAlien];
        for(int i = 0; i < rowAlien; i++){
            alien[i] = new alienInfo[colAlien];
        }
        //create a two dimensional bunker array for hard mode
        bunker = new bunkerInfo[rowBunker];
        //creates an array for aliens bullets
        aBullet = new alienBulletInfo[colAlien];
        defaultHardMode();
    }
    bHelp = new bulletHelp[colAlien];
    for(int i = 0; i < colAlien; i++){
        bHelp[i].i = rowAlien - 1;
        bHelp[i].j = i;
        bHelp[i].row = rowAlien - 1;
    }
    fillScreen(rowAlien, colAlien, rowBunker);
    printScreen();
    start = clock();
    //beginning of level 1
    startLevel = clock();
    player.speed = 7;
    while(!endgame){
        //the red spaceship
        
        //if the player has hit a key
        if(kbhit()){
            char key = getch();
            movePlayer(key, rowAlien, colAlien, rowBunker);
        }
        if(easy){
            rowAlien = 5;
            colAlien = 11;
            rowBunker = 5;
        }
        if(newGame){
            break;
        }
        generateAliensBullets();
        updateScreen(rowAlien, colAlien);
        checkHits(rowAlien, colAlien, rowBunker);
        fillScreen(rowAlien, colAlien, rowBunker);
        printScreen();
        sleep_for(milliseconds(200));
        checkIfGameOver(rowAlien);
        goToNextLevel(rowAlien, colAlien);
        //red spaceship
        if(!redSpcShip.used){
            //at least every 15 and atmost every 45 seconds
            randInterval = rand() % 15 + 30;
            redSpcShip.used = true;
        }
        //if it has left the screen
        if(!redSpcShip.inside){
            if((clock() / CLOCKS_PER_SEC) % randInterval == 0){
                redSpcShip.x = 100;
                redSpcShip.y = 2;
                redSpcShip.inside = true;
            }
        }
    }
    if(newGame){
        return;
    }
    endGame();
    cout << "Score: " << player.score << endl;
    cout << "Time: " << player.time << endl;
}

void goToNextLevel(int rowAln, int colAln){
    int counter = 0, numAlns = rowAln * colAln;
    for(int i = 0; i < rowAln; i++){
        for(int j = 0; j < colAln; j++){
            if(alien[i][j].alive){
                continue;
            }
            counter++;
        }
    }
    //if all the aliens are dead
    if(counter == numAlns){
        if(player.level == 5){
            endgame = true;
        }
        else{
            timePassed += clock() - startLevel;
            startLevel = clock();
            player.level++;
        }
    }
}

void checkIfGameOver(int rowAln){
    if(player.health == 0){
        player.gameover = true;
        endgame = true;
    }
    //if the aliens have reached the bottom
    if(alien[rowAln - 1][0].y == 32){
        player.gameover = true;
        endgame = true;
    }
}

void endGame(){
    if(player.gameover == true){
        cout << "You Lost\n";
    }
    else{
        cout << "You Win!\n";
    }
    system("pause");
}

void moveAliens(int rowAln, int colAln){
    //if aliens are moving to the right
    if(alienDirection){
        //if the aliens haven't reached the last column
        if(alien[0][colAln - 1].x != 94){
            for(int i = 0; i < rowAln; i++){
                for(int j = 0; j < colAln; j++){
                    alien[i][j].x += 4;
                }
            }
        }
        //if the aliens have reached the last column
        else{
            for(int i = 0; i < rowAln; i++){
                for(int j = 0; j < colAln; j++){
                    alien[i][j].y += 2;
                }
            }
            alienDirection = false;
        }
    }
    //if aliens are moving to the left
    else{
        //if the aliens haven't reached the first column
        if(alien[0][0].x != 2){
            for(int i = 0; i < rowAln; i++){
                for(int j = 0; j < colAln; j++){
                    alien[i][j].x -= 4;
                }
            }
        }
        //if the aliens have reached the first column
        else{
            for(int i = 0; i < rowAln; i++){
                for(int j = 0; j < colAln; j++){
                    alien[i][j].y += 2;
                }
            }
            alienDirection = true;
        }
    }
    shape = !shape;
}

void pause(int rowAln, int colAln, int rowBnk){
    system("cls");
    cout << "1. Save and Exit\n";
    cout << "2. New Game\n";
    cout << "3. Load Game\n";
    char choicePause = getch();
    switch (choicePause)
    {
        case '1':
            save(rowAln, colAln, rowBnk);
            exit(0);
        case '2':
            newGame = true;
            break;
        case '3':
            load();
    }
}

void Leaderboard(){
    cout << setw(10) << "NAME" << "|" << setw(6) << "TIME" << "|" << setw(6) << "SCORE" << endl;
    for(int i = 0; i < playerCount - 1; i++){
        cout << setw(10) << ranking[i].name << "|";
        cout << setw(6) << ranking[i].time << "|" << setw(6) << ranking[i].score << endl;
    }
}

void resize(playerInfo* &ranking, int &capacity, int newCapacity)
{
	playerInfo* newArr = new playerInfo[newCapacity];
	for (int i = 0; i < capacity; i++)
        newArr[i] = ranking[i];

	capacity = newCapacity;
	delete[] ranking;
	ranking = newArr;
}


void append(playerInfo* &ranking, int &size, int &capacity, int i, playerInfo player)
{
    if(size >= capacity) 
		resize(ranking, capacity, capacity * 2);
    for(i = 0; i < size; i++){
        if (ranking[i].name == player.name){
            if (ranking[i].score > player.score){
                ranking[i].score = player.score;
                ranking[i].health = player.health;
                ranking[i].time = player.time;
                ranking[i].level = player.level;
                size++;
                return;
            }
            if ((ranking[i].score == player.score) && (ranking[i].time > player.time)){
                ranking[i].health = player.health;
                ranking[i].level = player.level;
                size++;
                return;
            }
        }
    }
    for(i = 0; i < size; i++){
        if (player.score > ranking[i].score){
            for (int j = size; j > i; j--){
                ranking[j] = ranking[j - 1];
            }
            ranking[i].score = player.score;
            ranking[i].health = player.health;
            ranking[i].time = player.time;
            ranking[i].level = player.level;
            return;
        }
        if (player.score == ranking[i].score && player.time < ranking[i].time){
            for (int j = size; j > i; j--){
                ranking[j] = ranking[j - 1];
            }
            ranking[i].score = player.score;
            ranking[i].health = player.health;
            ranking[i].time = player.time;
            ranking[i].level = player.level;
            return;
        }
    }
    ranking[size].score = player.score;
    ranking[size].health = player.health;
    ranking[size].time = player.time;
    ranking[size].level = player.level;
    size++;
}

void save(int rowAlien, int rowBunker, int colAlien){
    ofstream file1("screen.txt", ios :: trunc);
    if (file1.is_open()) {
        for(int i = 0; i < 36; i++){
            for(int j = 0; j < 97; j++){
                file1 << screen[i][j].character;
            }
            file1 << endl;
        }
        file1.close();
    }
    ofstream file2("player.txt", ios :: trunc); 
    if (file2.is_open()){
        file2 << player.x << endl;
        file2 << player.y << endl;
        file2 << player.score << endl;
        file2 << player.health << endl;
        file2 << player.time << endl;
        file2 << player.level << endl;
        file2 << player.speed << endl;
        file2 << player.name << endl;
        file2 << easy << endl;
        file2 << alienDirection << endl;
        file2.close();
    }
    ofstream file3("alien.txt", ios :: trunc); 
    if (file3.is_open()){
        for(int i = 0; i < rowAlien; i++){
            for(int j = 0; j < colAlien; j++){
                file3 << setw(6) << alien[i][j].alive << setw(6) << alien[i][j].x << setw(6) << alien[i][j].y;
                file3 << setw(6) << alien[i][j].score << setw(6) << alien[i][j].damage << setw(6) << alien[i][j].type << endl;
            }
        }
        file3.close();
    }
    ofstream file4("bunker.txt", ios :: trunc); 
    if (file4.is_open()){
       for(int i = 0; i < rowBunker; i++){
                file4 << setw(6) << bunker[i].status << setw(6) << bunker[i].x << setw(6) << bunker[i].y << endl;
        }
        file4.close();
    }
}

void load(){
    ifstream file1out("screen.txt");
    if(file1out.is_open()){
        string line;
        //file1out >> screen[i][j].character
        while(getline(file1out, line)){
            for(int i = 0; i < 36; i++){
                for(int j = 0; j < 97; j++){
                    screen[i][j].character = line[j];
                }
            }
        }
        file1out.close();
    }
    ifstream file2out("player.txt"); 
    if (file2out.is_open()){
        string line;
        int i = 0;
        while (getline(file2out, line)) {
            switch (i)
            {
                case 0:
                    player.x = stoi(line);
                    break;
                case 1:
                    player.y = stoi(line);
                    break;
                case 2:
                    player.score = stoi(line);
                    break;
                case 3 :
                    player.health = stoi(line);
                    break;
                case 4:
                    player.time = stoi(line);
                    break;
                case 5:
                    player.level = stoi(line);
                    break;
                case 6:
                    player.speed = stoi(line);
                    break;
                case 7:
                    player.name = line;
                    break;
                case 8:
                    if (line == "1"){
                        easy = true;
                    }
                    else{
                        easy = false;
                    }
                    break;
                case 9:
                    if (line == "1"){
                        alienDirection = true;
                    }
                    else{
                        alienDirection = false;
                    }
                    break;
            }
            i++;
        }
        file2out.close();
    }
    int rowAl2, rowBn2, colAl2;
    if(easy){
        alien = new alienInfo* [5];
        bunker = new bunkerInfo [5];
        rowAl2 = 5;
        rowBn2 = 5;
        colAl2 = 11;
        for(int i = 0; i < 5; i++){
            alien[i] = new alienInfo[11];
        }
    }
    else{
        alien = new alienInfo* [7];
        bunker = new bunkerInfo [3];
        rowAl2 = 7;
        rowBn2 = 3;
        colAl2 = 15;
        for(int i = 0; i < 7; i++){
            alien[i] = new alienInfo[15];
        }
    }
    ifstream file3out("alien.txt"); 
    if (file3out.is_open()){
        //reads each line
        string line;
        //reads each variable
        string l2;
        int row = 0;
        int col = 0;
        while (getline(file3out, line)) {
            //seperates the first variable
            l2 = line.substr(0, 6);
            //repeats for each variable
            for(int i = 0; i < 6; i++){
                line = line.substr(6);
                //assigns each value to its variable
                switch (i)
                {
                case 0:
                    if (l2 == "1"){
                        alien[row][col].alive = true;
                    }
                    else{
                        alien[row][col].alive = false;
                    }
                    break;
                case 1:
                    alien[row][col].x = stoi(l2);
                    break;
                case 2:
                    alien[row][col].y = stoi(l2);
                    break;
                case 3:
                    alien[row][col].score = stoi(l2);
                    break;
                case 4 :
                    alien[row][col].damage = stoi(l2);
                    break;
                case 5:
                    alien[row][col].type = stoi(l2);
                    break;
                }
                if(line.length() >= 6){
                    l2 = line.substr(0, 6);
                }
            }
            col++;
            if(col == colAl2){
                col = 0;
                row++;
            }
        }
        file3out.close();
    }
    ifstream file4out("bunker.txt"); 
    if (file4out.is_open()){
        string line;
        string l2;
        int row = 0;
        while (getline(file4out, line)) {
            l2 = line.substr(0, 6);
            for(int i = 0; i < 3; i++){
                line = line.substr(6);
                switch (i)
                {
                case 0:
                    if (l2 == "1"){
                        bunker[row].status = true;
                    }
                    else{
                        bunker[row].status = false;
                    }
                    break;
                case 1:
                    bunker[row].x = stoi(l2);
                    break;
                case 2:
                    bunker[row].y = stoi(l2);
                    break;
                if(line.length() >= 6){
                    l2 = line.substr(0, 6);
                }
                row++;
            }
        }
        }
        file4out.close();
    }

}

void setColor(int textColor, int bgColor)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (bgColor << 4) | textColor);
}