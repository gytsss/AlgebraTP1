//Tp 1 Algebra
//Barra Santiago
//Godoy Tobias
//Monti Matias 

#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include "cmath"
#include <vector>

using namespace std;

struct Vector
{
	Vector2 from;
	Vector2 to;
	int id;
};

struct Angle
{
	float angle;
	int id1; //angle point 1
	int id2; //angle point 2
};

constexpr int maxVectors = 4;

//vector para todos los angulos encontrados
vector<Angle> angles;

//array para los puntos de interseccion
static Vector2 intersectionPoints[maxVectors][maxVectors];

//array para todas las combinaciones de angulos posibles
static Angle pointCombinations[126][maxVectors];

//array para calcular que 4 angulos suman 360 grados
Angle anglesUsed[maxVectors];

//Id de las diferentes formas
int figureID[maxVectors];

//Id de las difentes formas pero ordenado
int arrangedID[maxVectors];

//Calcula el punto de interseccion entre 2 vectores
Vector2 vectorIntersection(Vector vector1, Vector vector2);

//Dibuja los vectores
void DrawVector(Vector v1, Color color);

//Calcula los angulos teniendo en cuenta el punto de interseccion
void CheckAngles(Vector vectors[], float intersectionAngles[maxVectors][maxVectors]);

//Ordena de menor a mayor un vector
void SortVector(Vector& vector);

// "Crea todas las combinaciones posibles entre angulos"
void CreateCombinations(int offset, int k);

//Condicion para la combinacion de puntos, si el id ya esta en la combinacion retorna false
bool isUnique(Angle anglesI[][maxVectors], Angle angle);

//"Usa la combinacion de angulos para chequear si suman 360"
void SumOfPoint(Angle anglesCombination[][maxVectors]);

//Calcula la distancia entre 2 puntos
float GetDistance(Vector2 v1, Vector2 v2);

//Pasa la informacion de un array a otro
void CopyArray(int oldArr[], int newArr[]);

//Ordena un array
void SortArray(int arr[]);

//Compara dos id para saber si son la misma figura
bool CompareID(int id1[], int id2[]);

//Chequea si es un triangulo dependiendo del id
bool CheckTriangle(int id[]);

//Chequea si es un cuadrilatero dependiendo del id
bool CheckQuadrilateral(int id[]);

//Calcula el area de un triangulo
void TriangleArea();

//Calcula el area de un cuadrilatero cortandolo a la mitad
void QuadrilateralArea();


int main()
{
	InitWindow(1080, 720, "Algebra TP1");

	Vector vectors[maxVectors];

	//Creacion de vectores
	vectors[0] = { {150.0f, 500.0f}, {170.0f, 75.0f}, 1 };
	vectors[1] = { {100.0f, 200.0f}, {500.0f, 200.0f}, 2 };
	vectors[2] = { {340.0f, 70.0f}, {450.0f, 500.0f}, 3 };

	vectors[3] = { {100.0f, 450.0f}, {500.0f, 450.0f}, 4 };  //vector para formar el cuadrilatero
	//vectors[3] = { {100.0f, 450.0f}, {250.0f, 150}, 4 };       //vector para formar el triangulo

	Color colors[maxVectors] = { WHITE, RED, GREEN, BLUE };

	float intersectionAngles[maxVectors][maxVectors];

	for (int i = 0; i < maxVectors; ++i)
	{
		for (int j = 0; j < maxVectors; ++j)
		{
			intersectionPoints[i][j] = { -1, -1 };  //el -1 declara que esta vacio
			intersectionAngles[i][j] = -1;
		}
	}


	for (int i = 0; i < maxVectors; ++i)
	{
		SortVector(vectors[i]);
	}

	//Chequea todos los puntos de interseccion
	for (int i = 0; i < maxVectors; ++i)
	{
		for (int j = i + 1; j < maxVectors; ++j)
		{
			if (i != j)
			{
				intersectionPoints[i][j] = vectorIntersection(vectors[i], vectors[j]);

				if (intersectionPoints[i][j].x >= 0 && intersectionPoints[i][j].y >= 0)
				{
					figureID[i]++;
					figureID[j]++;
				}
			}
		}
	}



	CheckAngles(vectors, intersectionAngles);

	for (int i = 0; i < maxVectors; ++i)
	{
		for (int j = 0; j < maxVectors; ++j)
		{
			if (intersectionAngles[i][j] >= 0 && intersectionAngles[i][j] < 180)
			{
				angles.push_back({ intersectionAngles[i][j], i, j }); //No pueden ser del mismo id

				if ((180 - intersectionAngles[i][j]) >= 1)
				{
					angles.push_back({ (180 - intersectionAngles[i][j]), i, j }); //Sacar el segundo angulo
				}
			}
		}
	}

	CreateCombinations(0, maxVectors);

	SumOfPoint(pointCombinations);




	CopyArray(figureID, arrangedID);

	SortArray(arrangedID);

	cout << "Figure intersections: ";

	for (int i = 0; i < maxVectors; i++)
	{
		cout << arrangedID[i] << " ";
	}

	cout << "\n";


	if (CheckQuadrilateral(arrangedID))
	{
		cout << "The figure is a quadrilateral\n";
		QuadrilateralArea();
	}

	if (CheckTriangle(arrangedID))
	{
		cout << "The figure is a triangle\n";
		TriangleArea();
	}



	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);

		for (int i = 0; i < maxVectors; ++i)
		{
			DrawVector(vectors[i], colors[i]);
		}
		for (int i = 0; i < maxVectors; ++i)
		{
			for (int j = 0; j < maxVectors; ++j)
			{
				if (intersectionPoints[i][j].x > 0)
				{
					DrawCircle(static_cast<int>(intersectionPoints[i][j].x),
						static_cast<int>(intersectionPoints[i][j].y), 5.0f, YELLOW);
				}
			}
		}
		EndDrawing();
	}
	CloseWindow();
	return 0;
}

Vector2 NextValue(int id)
{
	for (int i = id; i < maxVectors; ++i)
	{
		for (int j = i; j < maxVectors; ++j)
		{
			if (intersectionPoints[i][j].x >= 0)
				return { static_cast<float>(i), static_cast<float>(j) };
		}
	}
	return { -1, -1 };
}

void DrawVector(Vector v1, Color color)
{
	DrawLine(static_cast<int>(v1.from.x), static_cast<int>(v1.from.y), static_cast<int>(v1.to.x),
		static_cast<int>(v1.to.y), color);
	DrawLineEx(v1.from, v1.to, 0.5, color);
	DrawLine(static_cast<int>(v1.from.x), static_cast<int>(v1.from.y), static_cast<int>(v1.to.x),
		static_cast<int>(v1.to.y), color);
}

Vector2 vectorIntersection(Vector vector1, Vector vector2)
{
	//linea del vector1 de from a to representado como a1x + b1y = c1
	float a1 = vector1.to.y - vector1.from.y;
	float b1 = vector1.from.x - vector1.to.x;
	float c1 = a1 * (vector1.from.x) + b1 * (vector1.from.y);


	// linea del vector2 representado como a2x + b2y = c2
	float a2 = vector2.to.y - vector2.from.y;
	float b2 = vector2.from.x - vector2.to.x;
	float c2 = a2 * (vector2.from.x) + b2 * (vector2.from.y);

	float determinant = a1 * b2 - a2 * b1;

	if (determinant == 0)
	{
		// RETORNA -2 -2 CUANDO SON PARALELAS
		return { -2, -2 };
	}
	else
	{
		float x = (b2 * c1 - b1 * c2) / determinant;
		float y = (a1 * c2 - a2 * c1) / determinant;
		float a, b, c, d;


		if (vector1.from.x < vector1.to.x)
		{
			a = vector1.from.x;
			b = vector1.to.x;
		}
		else
		{
			a = vector1.to.x;
			b = vector1.from.x;
		}

		if (vector1.from.y < vector1.to.y)
		{
			c = vector1.from.y;
			d = vector1.to.y;
		}
		else
		{
			c = vector1.to.y;
			d = vector1.from.y;
		}

		if (a <= x && x <= b && c <= y && y <= d)
		{

			cout << "Intersection found: (" << x << "," << y << ") Vectors: [" << vector1.id << " & " << vector2.id << "]\n";

			return { x, y }; //Devuelve las coords del punto de interseccion
		}
		else
		{
			//out of bounds
			return { -3, -3 };
		}
	}
}

void CheckAngles(Vector vectors[], float intersectionAngles[maxVectors][maxVectors])
{
	for (int i = 0; i < maxVectors; ++i)
	{
		for (int j = 0; j < maxVectors; ++j)
		{
			if (intersectionPoints[i][j].x >= 0)
			{
				//Se calcula el angulo con el dot product o producto punto

				float x1 = vectors[i].to.x - vectors[i].from.x;
				float x2 = vectors[j].to.x - vectors[j].from.x;
				float y1 = vectors[i].to.y - vectors[i].from.y;
				float y2 = vectors[j].to.y - vectors[j].from.y;

				float dotProduct = x1 * x2 + y1 * y2;

				float module = (sqrt(pow(x1, 2) + pow(y1, 2)) * (sqrt(pow(x2, 2) + pow(y2, 2))));

				float angle = acos(dotProduct / module) * 180 / PI;

				intersectionAngles[i][j] = angle;

				cout << "Angle " << i + 1 << " & " << j + 1 << ": " << angle << endl;
			}
		}
	}
}

void SortVector(Vector& vector)
{
	if (vector.to.x < vector.from.x)
	{
		float auxX = vector.from.x;
		vector.from.x = vector.to.x;
		vector.to.x = auxX;
		float auxY = vector.from.y;
		vector.from.y = vector.to.y;
		vector.to.y = auxY;
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

void PrintCombination(const vector<int>& v)
{
	static int count = 0;
	cout << "combination no " << (count++) << ": [ ";
	for (int i = 0; i < v.size(); ++i) { cout << v[i] << " "; }
	cout << "] " << endl;
}

int positionI = 0;
int positionJ = 0;

void CreateCombinations(int offset, int k)
{
	/*
	if (k == 0) //SI K = 0 YA NO HAY MAS COMBINACIONES PARA HACER
	{
		return;
	}
	//angles CONTIENE TODOS LOS ANGULOS ENCONTRADOS
	for (int i = offset; i <= angles.size() - k; ++i)
	{
		//isUnique -> SI NO SE REPITE EL NUMERO EN LA COMBINACION ACTUAL ES TRUE
		if (isUnique(pointCombinations, angles[i]))
		{
			//
			pointCombinations[positionI][positionJ % 4] = angles[i];

		}
		CreateCombinations(i + 1, k - 1);
	}
	*/

	for (int i = 0; i < 126; ++i)
	{
		for (int j = i + 1; j < maxVectors; ++j)
		{
			if (i != j && isUnique(pointCombinations, angles[i]))
			{
				pointCombinations[positionI][positionJ] = angles[i];
				positionJ++;
				positionI += positionJ % 4 == 0 ? 1 : 0;
			}
		}
	}
}

void SumOfPoint(Angle anglesCombination[][maxVectors])
{

	for (int i = 0; i < 126; ++i)
	{

		float totalAngle = 0;

		for (int j = 0; j < maxVectors; ++j)
		{
			anglesUsed[j] = anglesCombination[i][j];
			totalAngle += anglesCombination[i][j].angle;
			

		}

		if (static_cast<int>(totalAngle) == 360)
		{
			cout << "The sum of all angles is equal to " << totalAngle << endl;
			break;
		}
	}
}

bool isUnique(Angle anglesI[][maxVectors], Angle angle)
{
	for (int i = 0; i < maxVectors; ++i)
	{
		if (anglesI[positionI][i].id1 == angle.id1 && anglesI[positionI][i].id2 == angle.id2)
		{
			return false;
		}
	}
	return true;
}

void CopyArray(int oldArr[], int newArr[])
{
	for (int i = 0; i < maxVectors; i++)
	{
		newArr[i] = oldArr[i];
	}
}

void SortArray(int array[])
{
	int aux;

	for (int i = 0; i < maxVectors; i++)
	{
		for (int j = 0; j + 1 < maxVectors - i; j++)
		{
			if (array[j] < array[j + 1])
			{
				aux = array[j];
				array[j] = array[j + 1];
				array[j + 1] = aux;
			}
		}
	}
}

bool CheckTriangle(int id[])
{
	int triangle1[4] = { 2, 2, 2, 0 };
	int triangle2[4] = { 3, 2, 2, 1 };

	return (CompareID(id, triangle1) || CompareID(id, triangle2));
}

bool CompareID(int id1[], int id2[])
{
	for (int i = 0; i < maxVectors; i++)
	{
		if (id1[i] != id2[i])
			return false;
	}

	return true;
}

bool CheckQuadrilateral(int id[])
{
	int quadrilateral[] = { 2, 2, 2, 2 };

	return (CompareID(id, quadrilateral));
}

float GetDistance(Vector2 v1, Vector2 v2)
{
	//Teorema de pitagoras / euclidian distance

	float distX = v1.x - v2.x;
	float distY = v1.y - v2.y;

	return sqrt((distX * distX) + (distY * distY));
}

void TriangleArea()
{
	//distancia entre los puntos de collision
	float a = GetDistance(Vector2{ intersectionPoints[0][1].x,intersectionPoints[0][1].y }, Vector2{ intersectionPoints[1][3].x,intersectionPoints[1][3].y });
	float b = GetDistance(Vector2{ intersectionPoints[1][3].x,intersectionPoints[1][3].y }, Vector2{ intersectionPoints[0][3].x,intersectionPoints[0][3].y });
	float c = GetDistance(Vector2{ intersectionPoints[0][3].x,intersectionPoints[0][3].y }, Vector2{ intersectionPoints[0][1].x,intersectionPoints[0][1].y });


	float s = ((a + b + c) / 2);

	float area = sqrt(s * ((s - a) * (s - b) * (s - c)));

	cout << "The area of the triangle is: " << area << "\n";

}

void QuadrilateralArea()
{
	//distancia entre los puntos de collision

	float diagonal = GetDistance(Vector2{ intersectionPoints[0][1].x,intersectionPoints[0][1].y }, Vector2{ intersectionPoints[2][3].x,intersectionPoints[2][3].y });

	float a1 = GetDistance(Vector2{ intersectionPoints[0][1].x,intersectionPoints[0][1].y }, Vector2{ intersectionPoints[1][2].x,intersectionPoints[1][2].y });
	float b1 = GetDistance(Vector2{ intersectionPoints[1][2].x,intersectionPoints[1][2].y }, Vector2{ intersectionPoints[2][3].x,intersectionPoints[2][3].y });

	float s1 = ((a1 + b1 + diagonal) / 2);

	float areaTriangle1 = sqrt(s1 * ((s1 - a1) * (s1 - b1) * (s1 - diagonal)));


	float a2 = GetDistance(Vector2{ intersectionPoints[2][3].x,intersectionPoints[2][3].y }, Vector2{ intersectionPoints[0][3].x,intersectionPoints[0][3].y });
	float b2 = GetDistance(Vector2{ intersectionPoints[1][3].x,intersectionPoints[0][3].y }, Vector2{ intersectionPoints[0][1].x,intersectionPoints[0][1].y });

	float s2 = ((a2 + b2 + diagonal) / 2);

	float areaTriangle2 = sqrt(s2 * ((s2 - a2) * (s2 - b2) * (s2 - diagonal)));




	float area = areaTriangle1 + areaTriangle2;

	cout << "The area of the quadrilateral is: " << area << "\n";
}
