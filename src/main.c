//Made by ~Dots
//This has a bunch of hardcoded stuff don't mind the bad code :^)
//No comments explaining code because I don't need any B^)


#include "raylib.h"
#include "stdlib.h"
#include "time.h"

typedef struct Vector2i{
    int x;
    int y;
}Vector2i;


//Pot
typedef struct FlowerPot{
    Texture2D* potTexture;
    Rectangle sourceRec;
    Rectangle destRec;
    Vector2 potOrigin;
    Color potColor;
    bool isPlanted;
} FlowerPot;

FlowerPot InitFlowerPot(Color, Texture2D*);

//

//Plant

typedef struct Plant{
    Texture2D* plantTexture;
    Texture2D* flowerTexture;
    Vector2i* plantParts;
    Color plantColor;
    Color flowerColor;
    unsigned char maxPlantSize;
    unsigned char maxPlantGrowth;
    unsigned char currentSize;
    unsigned char growthCount;
    unsigned char plantIterator;
    unsigned char flowerType;
} Plant;

Plant InitPlant();
void EmptyPlant(Plant* plant);
void CreatePlant(Plant* plant);

void DestroyPlant(Plant*);
void GrowPlants(Plant*, int, int);
void DrawPlants(Plant*);
void DrawFlower(Plant*);

void GrowLeft(Plant*, int);
void GrowRight(Plant*, int);
void GrowUp(Plant*, int);
int SellPlant(Plant*);

Color GetRandomPlantColor(int);
Color GetRandomFlowerColor(int);
Texture2D* GetRandomFlowerType(int);

//


typedef enum STAGES{
    IDLE,
    GROWING,
    FULL_GROWN,
}STAGES;



void Delay(int);

Texture2D plantTexture;
Texture2D potTexture;
Texture2D wateringCan;
Texture2D seed;
Texture2D money;
Texture2D flower1;
Texture2D flower2;
Texture2D flower3;
Texture2D flower4;
Texture2D flower5;
Texture2D flower6;
Texture2D flower7;
Texture2D flower8;
Texture2D flower9;



const int WIDTH = 1280;
const int HEIGHT = 1024;

int main(){
    InitWindow(WIDTH, HEIGHT, "Idle Botanist");

    
    //Init all textures
    plantTexture = LoadTexture("resources/plant.png");
    potTexture = LoadTexture("resources/plant_pot.png");
    wateringCan = LoadTexture("resources/watering_can.png");
    seed = LoadTexture("resources/seed.png");
    money = LoadTexture("resources/money.png");
    flower1 = LoadTexture("resources/flower_1.png");
    flower2 = LoadTexture("resources/flower_2.png");
    flower3 = LoadTexture("resources/flower_3.png");
    flower4 = LoadTexture("resources/flower_4.png");
    flower5 = LoadTexture("resources/flower_5.png");
    flower6 = LoadTexture("resources/flower_6.png");
    flower7 = LoadTexture("resources/flower_7.png");
    flower8 = LoadTexture("resources/flower_8.png");
    flower9 = LoadTexture("resources/flower_9.png");


    //Init game objects
    FlowerPot pot = InitFlowerPot(WHITE, &potTexture);
    Plant plant = InitPlant();



    STAGES plantStage = GROWING;


    int plantTimer = 0;
    int moneyAmmount = 0;
    SetTargetFPS(60);

    while(!WindowShouldClose()){
        
        //Begin state machine
        
        switch (plantStage)
        
        {
        
            case IDLE:
                if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                    if(GetMousePosition().x > 30 && GetMousePosition().x < 30 + seed.width/10 && GetMousePosition().y > 300 && GetMousePosition().y < 300 + seed.height/10){   
                        CreatePlant(&plant);
                        plantStage = GROWING;
                    }
                }
        
        
                break;
        
            case GROWING:
                if(plant.currentSize ==  plant.maxPlantSize){
                    plantStage = FULL_GROWN;
                }

                if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                    if(GetMousePosition().x > 10 && GetMousePosition().x < 10 + wateringCan.width && GetMousePosition().y > 150 && GetMousePosition().y < 150 + wateringCan.height){   
                        if(plant.currentSize < plant.maxPlantSize){
                            srand(clock());
                            int growthDirection = rand() %3;
                            int distance = rand()% 5 + 4;
                            GrowPlants(&plant, growthDirection, distance);
                            plant.currentSize += 1;
                        }
                        
                }
                }       

                break;
        
            case FULL_GROWN:
                if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                    if(GetMousePosition().x > 1100 && GetMousePosition().x < 1100 + 170 && GetMousePosition().y > 10 && GetMousePosition().y < 10 + 50){   
                        moneyAmmount += SellPlant(&plant);
                        EmptyPlant(&plant);
                        plantStage = IDLE;
                    }
                }
        
        
                break;
        
            default:
        
                break;
        
        }



        //Draw
        BeginDrawing();
                ClearBackground(BROWN);

                DrawTexturePro(*(pot.potTexture), pot.sourceRec, pot.destRec, pot.potOrigin, 0, pot.potColor);
                DrawPlants(&plant);
                if(plant.currentSize == plant.maxPlantSize){
                     DrawFlower(&plant);
                }
                DrawTexture(wateringCan, 10, 150, WHITE);
                DrawTextureEx(seed, (Vector2){30, 300},0.0f, 0.1f, DARKBROWN);
                DrawTextureEx(money, (Vector2){10,10}, 0.0f, 0.5f, WHITE);
                DrawRectangle(1100, 10, 170, 50, DARKBROWN);
                DrawText(TextFormat("$%04i", moneyAmmount), 80, 10, 60, DARKGREEN);
                DrawText("SELL", 1135, 16, 40, RAYWHITE);

        EndDrawing();

    }

    DestroyPlant(&plant);
    UnloadTexture(plantTexture);
    UnloadTexture(flower1);
    UnloadTexture(flower2);
    UnloadTexture(flower3);
    UnloadTexture(flower4);
    UnloadTexture(flower5);
    UnloadTexture(flower6);
    UnloadTexture(flower7);
    UnloadTexture(flower8);
    UnloadTexture(flower9);
    UnloadTexture(flower7);
    UnloadTexture(seed);
    UnloadTexture(wateringCan);
    CloseWindow();
}


FlowerPot InitFlowerPot(Color potColor, Texture2D* potTexture){
    FlowerPot tempPot;
    tempPot.potTexture = potTexture;
    
    tempPot.sourceRec = (Rectangle){0.0f, 0.0f, (float)tempPot.potTexture->width, (float)tempPot.potTexture->height};

    tempPot.destRec = (Rectangle){(WIDTH/2) - 300, (HEIGHT/2) + 200, 600.0f, 250.0f};

    tempPot.potOrigin = (Vector2){tempPot.potTexture->width/2.0f, tempPot.potTexture->height/2.0f};

    tempPot.potColor = potColor;

    tempPot.isPlanted = false;


    return tempPot;
}

Plant InitPlant(){
    Plant tempPlant;
    tempPlant.plantTexture = &plantTexture;
    
    srand(time(NULL));
    tempPlant.plantColor = GetRandomPlantColor(rand()%30);
    tempPlant.flowerColor = GetRandomFlowerColor(rand()%8);

    tempPlant.flowerType = rand()%9;
    tempPlant.flowerTexture = GetRandomFlowerType(tempPlant.flowerType);
    tempPlant.maxPlantGrowth = (rand() % 8) + 3;
    tempPlant.maxPlantSize = (rand()% 5) + 2;

    tempPlant.plantParts = (Vector2i*)malloc(sizeof(Vector2i) * tempPlant.maxPlantSize * tempPlant.maxPlantGrowth);
    tempPlant.plantParts[0] = (Vector2i){WIDTH/2 - tempPlant.plantTexture->width, 700};
    tempPlant.growthCount = 1;
    tempPlant.plantIterator = 1;
    tempPlant.currentSize = 0;

    return tempPlant;
}

void EmptyPlant(Plant* plant){
    for(int i = 0; i < plant->plantIterator; i++){
        plant->plantParts[i].x = 0;
        plant->plantParts[i].x = 0;
    }
    plant->plantIterator = 0;
    plant->maxPlantGrowth = 0;
    plant->maxPlantSize = 0;
    plant->maxPlantGrowth = 0;
    plant->growthCount = 0;
    plant->plantIterator = 0;
    plant->currentSize = 0;
}

void CreatePlant(Plant* plant){
    
    srand(time(NULL));
    plant->plantColor = GetRandomPlantColor(rand()%30);
    plant->flowerColor = GetRandomFlowerColor(rand()%8);
    plant->flowerType = rand()%9;
    plant->flowerTexture = GetRandomFlowerType(plant->flowerType);
    plant->maxPlantGrowth = (rand() % 8) + 3;
    plant->maxPlantSize = (rand()% 5) + 2;

    plant->plantParts[0] = (Vector2i){WIDTH/2 - plant->plantTexture->width, 700};
    plant->growthCount = 1;
    plant->plantIterator = 1;
    plant->currentSize = 0;
}

void DestroyPlant(Plant* plant){
    free(plant->plantParts);
}



void GrowPlants(Plant* plant, int growthDirection, int distance){
    switch (growthDirection)
    {
    case 0:
        GrowLeft(plant, distance);
        break;
    case 1:
        GrowRight(plant,distance);
        break;
    case 2:
        GrowUp(plant,distance);
        break;
    default:
        GrowLeft(plant,distance);
        break;
    }
    
}

void GrowLeft(Plant* plant, int distance){
    while(plant->growthCount < plant->maxPlantGrowth){
        plant->plantParts[plant->plantIterator] = (Vector2i){plant->plantParts[plant->plantIterator-1].x - plant->plantTexture->width/distance, plant->plantParts[plant->plantIterator-1].y - plant->plantTexture->height/distance};
        plant->plantIterator += 1;
        plant->growthCount += 1;
    }
    plant->growthCount = 0;
}

void GrowRight(Plant* plant, int distance){
    while(plant->growthCount < plant->maxPlantGrowth){
        plant->plantParts[plant->plantIterator] = (Vector2i){plant->plantParts[plant->plantIterator-1].x + plant->plantTexture->width/distance, plant->plantParts[plant->plantIterator-1].y - plant->plantTexture->height/distance};
        plant->plantIterator += 1;
        plant->growthCount += 1;
    }
    plant->growthCount = 0;
}
void GrowUp(Plant* plant, int distance){
    while(plant->growthCount < plant->maxPlantGrowth){
        plant->plantParts[plant->plantIterator] = (Vector2i){plant->plantParts[plant->plantIterator-1].x, plant->plantParts[plant->plantIterator-1].y - plant->plantTexture->height/distance};
        plant->plantIterator += 1;
        plant->growthCount += 1;
    }
    plant->growthCount = 0;
}

void DrawPlants(Plant* plant){
    if(plant->plantIterator == 0){
        return;
    }
    else{
        for(int i = 0; i < plant->plantIterator; i++){
            DrawTexture(*(plant->plantTexture), plant->plantParts[i].x, plant->plantParts[i].y, plant->plantColor);
        }
    }
}

void DrawFlower(Plant* plant){
    DrawTextureEx(*(plant->flowerTexture), (Vector2){(float)(plant->plantParts[plant->plantIterator-1].x-plant->flowerTexture->width*1.5), (float)(plant->plantParts[plant->plantIterator-1].y-plant->flowerTexture->height*1.5)}, 0.0f, 4.0f, plant->flowerColor);
}


Color GetRandomFlowerColor(int rand){
    switch (rand)
    {   case 0:
            return MAROON;
            break;
        case 1:
            return YELLOW;
            break;
        case 2:
            return BLUE;
            break;
        case 3:
            return RED;
            break;
        case 4:
            return VIOLET;
            break;
        case 5:
            return LIME;
            break;
        case 6:
            return RAYWHITE;
            break;
        case 7:
            return SKYBLUE;
            break;
        default:
            return YELLOW;
            break;
    }
}

Color GetRandomPlantColor(int rand){
    switch (rand)
    {
        case 10:
            return GREEN;
            break;
        case 11: 
            return MAROON;
            break;
        case 12:
            return MAGENTA;
            break;
        case 13:
            return LIME;
            break;
        case 14:
            return YELLOW;
            break;
        case 15:
            return DARKBLUE;
            break;
        case 16:
            return BLUE;
            break;
        case 17:
            return ORANGE;
            break;
        case 18:
            return ORANGE;
            break;
        case 19:
            return RED;
            break;
        case 20:
            return ORANGE;
            break;
        case 21:
            return ORANGE;
            break;
        case 22:
            return ORANGE;
            break;
        case 23:
            return ORANGE;
            break;
        case 24:
            return ORANGE;
            break;
        case 25:
            return ORANGE;
            break;
        case 26:
            return ORANGE;
            break;
        default:
            return DARKGREEN;
            break;
    }
}   

Texture2D* GetRandomFlowerType(int rand){
        switch (rand)
        {
            case 0:
                return &flower1;
                break;

            case 1:
                return &flower2;
                break;

            case 2:
                return &flower3;
                break;

            case 3:
                return &flower4;
                break;

            case 4:
                return &flower5;
                break;

            case 5:
                return &flower6;
                break;

            case 6:
                return &flower7;
                break;

            case 7:
                return &flower8;
                break;

            case 8:
                return &flower9;
                break;
            default:
                return &flower1;
                break;
    }
}

int SellPlant(Plant* plant){
    int temp;
    int temp2;
    switch (plant->flowerType)
        {
            case 0:
                return temp = 1;
                break;

            case 1:
                return temp = 2;
                break;

            case 2:
                return temp = 3;
                break;

            case 3:
                return temp = 4;
                break;

            case 4:
                return temp = 5;
                break;

            case 5:
                return temp = 6;
                break;

            case 6:
                return temp = 7;
                break;

            case 7:
                return temp = 8;
                break;

            case 8:
                return temp = 9;
                break;
            default:
                return temp = 1;
                break;
    }
    temp2 = plant->maxPlantGrowth * 0.1f, plant->maxPlantSize * 1.5f + temp;


    return temp2;
}

void Delay(int seconds){
    int milliSeconds = 1000* seconds;

    clock_t startTime = clock();

    if(clock() < startTime + milliSeconds){
        PollInputEvents();
    }
}
