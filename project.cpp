#include <iostream>
#include<time.h> //for rand 
#include <windows.h>
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

        if (head == NULL)
        {
            head = newnode;
        }
        else
        {
            tail->next = newnode;
            tail = newnode;
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
    void createMap()
    {
        for (int i = 0; i < dimensionY; i++)
        {
            for (int j = 0; j < dimensionX; j++)
            {
                if (i == 0 || i == dimensionY - 1 || j == 0 || j == dimensionX-1)
                {
                    mapGrid[i][j] = 1; //make grid boarders = 1;
                }
                else
                {
                    mapGrid[i][j] = 0; //make empty spaces = 0;
                }
            }
        }
        mapGrid[FoodCordY][FoodCordX] = 3; //make cordinate where there is food = 3
    }

    //function for creating random food cordinatates:
    void createFood()
    {
        srand(time(NULL));
        mapGrid [FoodCordY] [FoodCordX] = 0; //making current food cordinate empty

        do
        {
            FoodCordX = rand() % (dimensionX - 1) + 1;
            FoodCordY = rand() % (dimensionY - 1) + 1;
        } while (mapGrid[FoodCordY][FoodCordX] != 0);

        mapGrid [FoodCordY] [FoodCordX] = 3;
        //go_to_xy (FoodCordX , FoodCordY); //move cursor to cordinates
        //cout<<"F";
    }

    void displayMap()
    {
        for (int i = 0; i < dimensionY; i++)
        {
            for (int j = 0; j < dimensionX; j++)
            {
                if (mapGrid[i][j] == 1)
                {
                    cout<<char(177); //block character for wall
                }
                else if (mapGrid[i][j] == 3)
                {
                    cout<<"F"; //F for food
                }
                else
                {
                    cout<<" "; //Space for empty
                }
            }
            cout<<endl;
        }
    }

};

class Game
{
    public:
    Map map;
    SnakeBody snake;
};

int main()
{
    Map m;
    cout<<endl;
    m.createMap();
    m.createFood();
    m.displayMap();

}