#include <iostream>
#include<time.h> //for rand 
#include <windows.h> //for go_to_xy
#include <conio.h> //for getch and khbit
#include <fstream> //for filehandling
using namespace std;

//User handling 
//**********************************************************************************************************
struct UserData
{
    string username;
    string password;
    int highscore;
    int currentScore;
    int currentLevel;
};

//class to manage user functions:
class UserSystem 
{
    public:
    UserData users[100];
    int usercount;
    const char* filename;

    UserSystem()
    {
        filename = "userdata.txt";
        usercount = 0;
        loadUsers();
    } 

    void loadUsers()
    {
        usercount = 0;
        ifstream file;
        file.open(filename);
        if (!file){return;}
        else
        {
            while
            (
                file >> users[usercount].username 
                >> users[usercount].password
                >> users[usercount].highscore
                >> users[usercount].currentScore
                >> users[usercount].currentLevel
            )
            {
                usercount++;
            }
            file.close();
        }
    }

    void saveUsers()
    {
        ofstream file;
        file.open(filename);
        if (!file){return;}
        else
        {
             for (int i = 0; i < usercount; i++)
        {
            file << users[i].username << " " 
            << users[i].password << " "
            << users[i].highscore << " " 
            << users[i].currentScore << " " 
            << users[i].currentLevel << "\n";
        }
        file.close();
        }
    }

    int findUser(const string &username)
    {
        for (int i = 0; i < usercount; i++)
        {
            if (users[i].username == username)
                return i;
        }
        return -1; // User not found
    }

     void registerUser()
    {
        string username, password;
        cout << "Enter a username: ";
        cin >> username;

        if (findUser(username) != -1)
        {
            cout << "Username already exists. Please Login.\n";
            return;
        }

        cout << "Enter a password: ";
        cin >> password;

        users[usercount].username =username;
        users[usercount].password = password;
        users[usercount].highscore = 0;
        users[usercount].currentScore = 0;
        users[usercount].currentLevel = 1;
        usercount++;
        saveUsers();
        cout << "Registration successful!\n";
    }

      int loginUser()
    {
        string username, password;
        cout << "Enter your username: ";
        cin >> username;

        int userIndex = findUser(username);
        if (userIndex == -1)
        {
            cout << "Username not found. Please register first.\n";
            return -1;
        }

        cout << "Enter your password: ";
        cin >> password;

        if (users[userIndex].password != password)
        {
            cout << "Incorrect password. Please try again.\n";
            return -1;
        }

        cout << "Login successful! Welcome, " << username << "!\n";
        cout << "Score: " << users[userIndex].currentScore
        << ", Current Level: " << users[userIndex].currentLevel << ", High Score: " 
        << users[userIndex].highscore<<"\n";
        return userIndex;
    }
    void updateUser(int userIndex, int score, int level)
    {
        if (userIndex < 0 || userIndex >= usercount)
            return;

        if (score >= users[userIndex].highscore)
            users[userIndex].highscore = score;

        users[userIndex].currentScore = score;
        users[userIndex].currentLevel = level;

        saveUsers();
    }

    UserData getUserData(int userIndex)
    {
        return users[userIndex];
    }
    };
//**********************************************************************************************************

//Game Handling:
//creating Node struct for body
struct Node
{
    int cordX; //to store x cordinate position of snake
    int cordY; //to store x cordinate position of snake
    Node* next;
};

//creating class for the Snake body
class SnakeBody
{
    public:
    Node* head;
    Node* tail;

    SnakeBody(): head(NULL), tail(NULL){};

    //creating the snake body:
    void createBody(int x, int y)
    {
        Node* newnode = new Node;
        newnode -> cordX = x;
        newnode -> cordY = y;
        newnode->next = NULL;

        if (head == NULL)
        {
            head = newnode;
            tail = newnode;
        }
        else
        {
            tail->next = newnode;
            tail = newnode;
        }
    }

    //funcion to move snake
    void move(int dx, int dy, bool& grow)
    {
        int prevX = head->cordX;
        int prevY = head->cordY;

        head->cordX += dx; //update the head cordinates accoring to the difference of x and y
        head->cordY += dy;

        Node* current = head->next;
        while(current != NULL)
        {
            int tempX = current->cordX;
            int tempY = current->cordY;

            current->cordX = prevX;
            current->cordY = prevY;

            prevX = tempX;
            prevY = tempY;

            current = current->next;
        }

        if (grow == true)
        {
            createBody(prevX,prevY); //ellongate the snake by adding a new node
            grow = false;
        }
    }

};

//function to move the cursor to specific cordinates on the terminal, using windows library
void go_to_xy(int x,int y)
{
    COORD c = {x,y};
    {
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),c);
    }
}

class Map
{
    public:

    int mapGrid[40][120];//grid for the whole map;
    int dimensionX; //x, y dimensions of the playabale area
    int dimensionY;
    int score; //for scoring the players score
    int FoodCordX; //x,y cordinates for the cordinates where food will be placed
    int FoodCordY;

    //constructor to initiazlie these data
    Map():dimensionX(90), dimensionY(20),score(0),FoodCordX(2),FoodCordY(2){};

    //function for creating the playing grid
    void displayWalls()
    {
        for (int i = 0; i < dimensionY; i++)
        {
            for (int j = 0; j < dimensionX; j++)
            {
                if (i == 0 || i == dimensionY - 1 || j == 0 || j == dimensionX-1)
                {
                    go_to_xy(j,i);
                    cout<<char(177); //block characters for wall
                }
            }
        }
    }

    //function for creating random food cordinatates:
    void createFood()
    {
        srand(time(NULL));
        mapGrid [FoodCordY] [FoodCordX] = 0; //making current food cordinate empty

        do
        {
            FoodCordX = rand() % (dimensionX - 2) + 1;
            FoodCordY = rand() % (dimensionY - 2) + 1;
        } while (mapGrid[FoodCordY][FoodCordX] != 0);

        mapGrid [FoodCordY] [FoodCordX] = 3;
        go_to_xy (FoodCordX , FoodCordY); //move cursor to cordinates
        cout<<"F";
    } 

    //function for creating obstacles (displayed as block characters)
    void createObstacles(int count)
    {
        //making current obstacle cordinates empty
        for (int i = 0; i < dimensionY - 1; i++)
        {
            for (int j = 0; j < dimensionY - 1; j++)
            {
                if (mapGrid[i][j]==4)
                {
                    mapGrid[i][j]=0;
                    go_to_xy(j,i); 
                    cout<<" ";
                }
            }
        }

        //creating new obstacles:
        srand(time(NULL));
        //The number of obstacles generated depends on the level
        for (int i = 0; i < count; i++)
        {
            int obsCordX, obsCordY;

            do {
            obsCordX = rand() % (dimensionX - 2) + 1;
            obsCordY = rand() % (dimensionY - 2) + 1;
            } while(mapGrid[obsCordY][obsCordX]!= 0);

            mapGrid[obsCordY][obsCordX] = 4;
            go_to_xy(obsCordX,obsCordY);
            cout<<char(176);

        }
    }

    bool isObstacle(int x, int y)
    {
        return mapGrid[y][x]==4;
    }
};


class Game
{
    public:
    Map map;
    SnakeBody snake;
    int dx; //for chnage in x cordinate
    int dy; //for chnage in y cordinate
    bool grow;
    int level; 

    Game(int startScore, int startLevel):dx(1), dy(0), level(1), grow(false){map.score = startScore;} //for moving right as soon as game starts

    //function for determining dy and dx based on user input
    void input()
    {
        if (_kbhit()) //checking if any key is pressed
        {
            switch(_getch())
            {
                case 'w':
                {
                    if (dy == 0)
                    {
                        dx = 0;
                        dy = -1;
                    }
                    break;
                }
                case 's':
                {
                    if (dy == 0)
                    {
                        dx = 0;
                        dy = 1;
                    }
                    break;
                }
                case 'a':
                {
                    if (dx == 0)
                    {
                        dx = -1;
                        dy = 0;
                    }
                    break;
                }
                case 'd':
                {
                    if (dx == 0)
                    {
                        dx = 1;
                        dy = 0;
                    }
                    break;
                }
            }
        }
    }

    void updateLevel()
    {
        int newlevel;
        newlevel = (map.score / 1) +1;

        if (newlevel > level)
        {
            level = newlevel;
            map.createObstacles(level * 2 - 2); //increase obstacles with each level
        }
    }

    int getSleeptime()
    {
        if (level == 1){return 100;}
        if (level == 2){return 70;}
        if (level == 3){return 50;}
        if (level == 4){return 30;}
        if (level == 5){return 25;}
        if (level == 6){return 20;}
        if (level > 6){return 10;}
    }

    void GameOver()
    {
        for (int i = 0; i < 4; i++)
        {
            system("cls");
            Sleep(300);
            cout << "----------------------------\n";
            cout << "        GAME OVER!          \n";
            cout << "----------------------------\n";
            Sleep(300);
        }
        cout << "Final Score: " << map.score << "\n";
        cout << "Level: " << level << "\n";
    }
    void initializeLevel()
    {
        if (level == 1){return;}
        map.createObstacles(level * 2 - 2); // Generate obstacles for the starting level
    }

    void playGame()
    {
        //setting up the game:
        map.displayWalls();
        map.createFood();
        initializeLevel();
        snake.createBody(10,10);

        while(true) //runs until wall is hit
        {
            input(); //takes input from the user and modifies dx or dy

            //moving the snake
            go_to_xy(snake.tail->cordX, snake.tail->cordY);
            cout<<" "; //remove old tail;
            snake.move(dx,dy,grow);

            //showing snake head
            go_to_xy(snake.head->cordX, snake.head->cordY);
            cout<<"O"; //represents snakes head;

            //checking if food is eaten
            if (snake.head->cordX == map.FoodCordX && snake.head->cordY == map.FoodCordY)
            {
                grow = true;
                map.score++;
                map.createFood();
                updateLevel();
            }

            //checking for collission
            if (snake.head->cordX == 0 || snake.head->cordY == 0 || snake.head->cordX == map.dimensionX-1 ||
              snake.head->cordY == map.dimensionY-1 || map.isObstacle(snake.head->cordX,snake.head->cordY))
              {
                GameOver();
                break; //exit the loop
              }
              
                Sleep (getSleeptime());
        } 
    }
};

int main()
{
    system("cls");
    
    int choice = 0;
    while (choice != 4)
    {

        cout<<"SNAKE GAME"<<endl;
        cout<<"1. Register\n";
        cout<<"2. Login\n";
        cout<<"3. How to play\n";
        cout<<"4. Exit\n";
        cin>>choice;

        UserSystem userSystem;
        int userIndex = -1;

        switch(choice)
        {
            case 1:
            {
                system("cls");
                userSystem.registerUser();
                break;
            }
            case 2:
            {
                system("cls");
            userIndex = userSystem.loginUser();
            if (userIndex != -1)
            {
                UserData userData = userSystem.getUserData(userIndex);
                bool keepPlaying = true;

                while (keepPlaying) // Replay loop
                {
                    int playChoice;
                    cout << "1. Restart\n2. Continue\n";
                    cin >> playChoice;

                    if (playChoice == 1) // Restart the game
                    {
                        system("cls");
                        Game g(0, 1);
                        g.playGame();
                        userSystem.updateUser(userIndex, g.map.score, g.level);
                    }
                    else if (playChoice == 2) // Continue from saved state
                    {
                        system("cls");
                        Game g(userData.currentScore, userData.currentLevel);
                        g.playGame();
                        userSystem.updateUser(userIndex, g.map.score, g.level);
                    }

                    // Ask user after game over
                    int postGameChoice;
                    cout << "\n1. Replay\n2. Log Out\n3. Return to Main Menu\n";
                    cin >> postGameChoice;

                    if (postGameChoice == 2) // Log out
                    {
                        cout << "Logging out...\n";
                        userIndex = -1;
                        keepPlaying = false;
                    }
                    else if (postGameChoice == 3) // Return to Main Menu
                    {
                        keepPlaying = false;
                    }
                    else
                    {
                        system("cls");
                        userData = userSystem.getUserData(userIndex); // Reload updated user data
                    }
                }
            }
            break;
            }
            case 3:
            {
                cout << "\nHOW TO PLAY:\n";
                cout << "1. Use 'W', 'A', 'S', 'D' to move the snake.\n";
                cout << "2. Avoid running into walls or the snake's own body.\n";
                cout << "3. Collect food ('F') to grow and increase your score.\n";
                cout << "4. Progress through levels by reaching specific scores.\n";
                cout << "5. Obstacles" <<char(176)<<" appear as levels increase; avoid them!\n";
                cout << "6. Have fun!\n\n";
                cout << "Press any key to return to the main menu...\n";
                _getch();
                break;
            }
            case 4:
            {
                cout<<"Exiting...";
                break;
            }
            default:
            {
                cout<<"Invalid Entry";
                break;
            }
        }
    }
}