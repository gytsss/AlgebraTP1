#include <iostream>
#include "raylib.h"


struct Vector
{
    Vector2 from;
    Vector2 to;
};

const int maxVecQty=4;


Vector2 vectorIntersection(Vector vector1, Vector vector2);
void DrawVector(Vector v1, Color color);
void CheckAngles(Vector vectors[], float intersectionAngles[maxVecQty][maxVecQty]);
Vector2 NextValue(int id);
void CheckForQuadrilater(Vector vectors[],float intersectionAngles[maxVecQty][maxVecQty] );
void SortVector(Vector& vector);
static Vector2 intersectionPoints[maxVecQty][maxVecQty];
int main()
{
    
    Vector vectors[maxVecQty];
    vectors[0]={{150.0f, 500.0f}, {170.0f, 75.0f}};
    vectors[1]={{100.0f, 200.0f}, {500.0f, 200.0f}};
    vectors[2]={{340.0f, 70.0f }, {450.0f, 500.0f}};
    vectors[3]={{100.0f, 450.0f}, {500.0f, 450.0f}};

    Color colors[maxVecQty]={WHITE, RED, GREEN, BLUE};
    
    
    float intersectionAngles[maxVecQty][maxVecQty];
    
    
    for (int i = 0; i < maxVecQty; ++i)
    {
        for (int j = 0; j < maxVecQty; ++j)
        {
            intersectionPoints[i][j] = {-1,-1};
        }
    }
    
    InitWindow(720*1.5f,480*1.5f,"algebra");

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        
        for (int i = 0; i < maxVecQty; ++i)
        {
            SortVector(vectors[i]);
        }
        //CHEQUEA TODOS LOS PUNTOS DE COLISION
        for (int i = 0; i < maxVecQty; ++i)
        {
            //j=i para que no vuelva a hacer chequeos que ya se hicieron
            for (int j = 0; j < maxVecQty; ++j)
            {
                if (i != j)
                {
                    intersectionPoints[i][j] = vectorIntersection(vectors[i], vectors[j]);
                }
            }
        }
        CheckAngles(vectors, intersectionAngles);
        CheckForQuadrilater(vectors, intersectionAngles);
        for (int i = 0; i < maxVecQty; ++i)
        {
            DrawVector(vectors[i], colors[i]);
        }
        
        EndDrawing();
    }
    CloseWindow();
    return 0;
}

Vector2 NextValue(int id)
{
    for (int i = id; i < maxVecQty; ++i)
    {
        for (int j = i; j < maxVecQty; ++j)
        {
            if (intersectionPoints[i][j].x >= 0)
                return {(float)i,(float)j};
        }
    }
    return {-1,-1};
}

void CheckForQuadrilater(Vector vectors[], float intersectionAngles[maxVecQty][maxVecQty] )
{
    float angle=0;
    int count1=0;
    Vector2 points[4];
    
    for (int j = 0; j < maxVecQty; ++j)
    {
        count1 = 0;
        angle = 0;
        
        for (int i = 0; i < maxVecQty; ++i)
        {
            points[i] = {-1, -1};
        }
        
        for (int m = 0; m < 4; ++m)
        {
            points[count1] = NextValue(j + m);
            if (points[count1].x > -1)
                count1++;
            if (count1 == 4)
                break;
        }
        
        for (int i = 0; i < maxVecQty; ++i)
        {
            if (intersectionAngles[(int)points[i].x][(int)points[i].y]<180)
                angle += intersectionAngles[(int)points[i].x][(int)points[i].y];
            if ((int)angle==360)
                break;
        }
    }
    
}

void DrawVector(Vector v1, Color color)
{
    DrawLine(static_cast<int>(v1.from.x),static_cast<int>(v1.from.y), static_cast<int>(v1.to.x), static_cast<int>(v1.to.y), color);
}

Vector2 vectorIntersection(Vector vector1, Vector vector2)
{
    
    // linea del vector1 de from a to representado como a1x + b1y = c1
    float a1 = vector1.to.y - vector1.from.y;
    float b1 = vector1.from.x - vector1.to.x;
    float c1 = a1*(vector1.from.x) + b1*(vector1.from.y);
 
    // Line CD represented as a2x + b2y = c2
    float a2 = vector2.to.y - vector2.from.y;
    float b2 = vector2.from.x - vector2.to.x;
    float c2 = a2*(vector2.from.x)+ b2*(vector2.from.y);
 
    float determinant = a1*b2 - a2*b1;
 
    if (determinant == 0)
    {
        // RETORNA -2 -2 CUANDO SON PARALELAS
        return {-2, -2};
    }
    else
    {
        float x = (b2*c1 - b1*c2)/determinant;
        float y = (a1*c2 - a2*c1)/determinant;
        float a,b,c,d;
        if(vector1.from.x < vector1.to.x)
        {
            a = vector1.from.x;
            b = vector1.to.x;
        }
        else
        {
            a = vector1.to.x;
            b = vector1.from.x;
        }
        if(vector1.from.y < vector1.to.y)
        {
            c = vector1.from.y;
            d = vector1.to.y;
        }
        else
        {
            c = vector1.to.y;
            d = vector1.from.y;
        }
        if(a <= x && x <= b && c <= y && y <= d)
        {
            return {x, y};
            
        }
        else
        {
            //INTERSECTION OUT OF BOUNDS
            return{-3,-3};
        }
        
    }/*
    
    Vector2 result;
    // punto de interseccion en X
    result.x = ((vector1.from.x * vector1.to.y - vector1.from.y * vector1.to.x) * (vector2.from.x - vector2.to.x) - (vector1.from.x - vector1.to.x) * (vector2.from.x * vector2.to.y - vector2.from.y * vector2.to.x)) / ((vector1.from.x - vector1.to.x) * (vector2.from.y - vector2.to.y) - (vector1.from.y - vector1.to.y) * (vector2.from.x - vector2.to.x));

    // punto de interseccion en Y
    result.y = ((vector1.from.x * vector1.to.y - vector1.from.y * vector1.to.x) * (vector2.from.y - vector2.to.y) - (vector1.from.y - vector1.to.y) * (vector2.from.x * vector2.to.y - vector2.from.y * vector2.to.x)) / ((vector1.from.x - vector1.to.x) * (vector2.from.y - vector2.to.y) - (vector1.from.y - vector1.to.y) * (vector2.from.x - vector2.to.x));
    return result;*/
}

void CheckAngles(Vector vectors[],float intersectionAngles[maxVecQty][maxVecQty] )
{
    for (int i = 0; i < maxVecQty; ++i)
    {
        for (int j = 0; j < maxVecQty; ++j)
        {
            if(intersectionPoints[i][j].x >= 0)
            {
                /*
                float m1 = (vectors[i].to.y - vectors[i].from.y)/(vectors[i].to.x - vectors[i].from.x);
                float m2 = (vectors[j].to.y - vectors[j].from.y)/(vectors[j].to.x - vectors[j].from.x);
                intersectionAngles[i][j] = atan(abs((m2-m1)/(1+m2*m1)));
                intersectionAngles[i][j] *= 180 / acos(-1.0f);
                */
               
                double a = (vectors[i].to.x - vectors[i].from.x) * (vectors[j].to.x - vectors[j].from.x);
                double b = (vectors[i].to.y - vectors[i].from.y) * (vectors[j].to.y - vectors[j].from.y);
                double c = sqrt((vectors[i].to.x-vectors[i].from.x)*(vectors[i].to.x-vectors[i].from.x)+(vectors[i].to.x-vectors[i].from.x)*(vectors[i].to.x-vectors[i].from.x));
                double d = sqrt((vectors[j].to.x-vectors[j].from.x)*(vectors[j].to.x-vectors[j].from.x)+(vectors[j].to.x-vectors[j].from.x)*(vectors[j].to.x-vectors[j].from.x));
                
                intersectionAngles[i][j] = ((acos((float)((a + b) / (c * d))))) * 180 / PI;
                
            }
        }
    }
}

void SortVector(Vector& vector)
{
    if (vector.to.x < vector.from.x)
    {
        float savex = vector.from.x;
        vector.from.x = vector.to.x;
        vector.to.x = savex;
        float savey = vector.from.y;
        vector.from.y = vector.to.y;
        vector.to.y = savey;
    }
    else if (vector.from.x == vector.to.x)
    {
        if (vector.to.y < vector.from.y)
        {
            float savey = vector.from.y;
            vector.from.y = vector.to.y;
            vector.to.y = savey;
        }
    }
}
