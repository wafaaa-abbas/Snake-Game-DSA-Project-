#include <iostream>
#include<time.h> //for rand 
#include <windows.h> //for go_to_xy
#include <conio.h> //for getch and khbit
using namespace std;

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
};


class Game
{
    public:
    Map map;
    SnakeBody snake;
    int dx; //for chnage in x cordinate
    int dy; //for chnage in y cordinate

    Game():dx(1), dy(0){}; //for moving right as soon as game starts

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

    void playGame()
    {
        //setting up the game:
        map.displayWalls();
        map.createFood();
        snake.createBody(10,10);
        bool grow = false;

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
            }

            //checking for collission
            if (snake.head->cordX == 0 || snake.head->cordY == 0 || snake.head->cordX == map.dimensionX-1 ||
              snake.head->cordY == map.dimensionY-1 )
              {
                cout<<"\n ---GAME OVER---\n";
                break; //exit the loop
              }

              Sleep (100);
        } 
    }
};

int main()
{
    Game g;
    system("cls");
    g.playGame();

}