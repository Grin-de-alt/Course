#include "stdafx.h"
#include "GameBoard.h"
#include <assert.h>
#include <string>
#include <iostream>
#include "Wall.h"
#include "Food.h"
#include <time.h>
#include "Snake.h"
#include <math.h>
#include <windows.h>
#include "Consts.h"
#include <vector>
#include "Point.h"

using namespace SnakeGame;
using namespace std;

/*
	������ � ��������.

	��������� ��������� �������� ���� ���������� �� ���������� ����������� � �������.
*/
namespace Console
{
	// ����������� ������ � ������� �� �����������
	void Gotoxy(int x, int y)
	{
		COORD coord;
		coord.X = x;
		coord.Y = y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	}

	// ���������� ������ �� ����������� � �������
	void PrintToCoordinates(int x, int y, char ch)
	{
		Gotoxy(x, y);
		std::cout << ch;
	}
}


GameBoard::GameBoard()
{
	srand(time(nullptr));
	
	CreateGameBoard();						// ��������� ���� ��������� ������� �������. ���������� ����� ��������� �������� ����.
	InitializeGameBoard();					// ������������� ����� �� ����		
}

void GameBoard::CleanObjectFromMatrix(Object * obj, int x, int y)
{
	/*
		�������� ����� ��������� �������� �� �������.
		- �������� ���������� ����������
		- ���������� ������ �� null
		- ������ � ������� �� ���������� ����������� �� null
		- ������ � ������� ������������ ����������� �������

		���� ��� �������� ��������, ������ ����� ������ ������ � ������� ������� �� �����������.
	*/
	if (IsPositionCorrect(x, y) && obj && _gameMatrix[x][y] && _gameMatrix[x][y] == obj)
	{
		delete obj;
		_gameMatrix[x][y] = nullptr;
	}
	else
	{
		assert(false, "������ ��� �������� �������� �� �������.");
	}
}

void GameBoard::CreateGameBoard()
{
	for (int i = 0; i < GAME_BOARD_WIDTH; i++)
		for (int j = 0; j < GAME_BOARD_HEIGHT; j++)
			_gameMatrix[i][j] = nullptr;
}

void GameBoard::CleanGameBoard()
{
	for (int i = 0; i < GAME_BOARD_WIDTH; i++)
	{
		for (int j = 0; j < GAME_BOARD_HEIGHT; j++)
		{
			auto * obj = _gameMatrix[i][j];

			if (obj)
			{
				CleanObjectFromMatrix(obj, i, j);
			}
			else
			{
				_gameMatrix[i][j] = nullptr;
			}
		}
	}

	CopyMatrixDataToTempMatrix();
}

void SnakeGame::GameBoard::CopyMatrixDataToTempMatrix()
{
	for (int i = 0; i < GAME_BOARD_WIDTH; i++)
	{
		for (int j = 0; j < GAME_BOARD_HEIGHT; j++)
		{
			auto * obj = _gameMatrix[i][j];

			if (obj)
			{
				_gameMatrixPrevStep[i][j] = obj->GetSymbolForDisplay();
			}
			else
			{
				_gameMatrixPrevStep[i][j] = ' ';
			}
		}
	}
}

int SnakeGame::GameBoard::GetNumberFreeCells()
{
	return ((GAME_BOARD_WIDTH * GAME_BOARD_HEIGHT) - (GAME_BOARD_WIDTH * 2) - (GAME_BOARD_HEIGHT * 2)) + 4;
}

int SnakeGame::GameBoard::GetSnakeSize()
{
	auto * Element = FindHeadSnake();

	int counter = 0;

	while (Element != nullptr)
	{
		Element = Element->Prev;

		counter++;
	}

	return counter;
}

void GameBoard::InitializeGameBoard()
{
	ClearGameBoardConsole();				// ������� �������
	CleanGameBoard();						// ������� ��� ������� � ���� ���� ��� ����
	CreateWallsAroundPerimeter();			// ������� �����
	CreateSnakeHead();					// ������� ������
	//CreateSnakeHead(1,1);
	CreateFoodOnRandomPosition();			// ������� �������� ������ ���
	//CreateFoodsAroundPerimeter();
	PrintGameBoardToConsole();				// ��������� ������� ����
}

void GameBoard::CreateWallsAroundPerimeter()
{
	for (int i = 0; i < GAME_BOARD_WIDTH; i++)
	{
		for (int j = 0; j < GAME_BOARD_HEIGHT; j++)
		{
			// ������� ���������� ���� � �� ��� ������� �������� ������� ����.
			if (i == 0 || j == 0 || i == GAME_BOARD_WIDTH-1 || j == GAME_BOARD_HEIGHT-1)
			{
				auto * obj = _gameMatrix[i][j];

				// ���� ���-�� ���� ������ ������� �� ���� �����������, ������� � ����������� �����.
				if (obj != nullptr)
				{
					delete obj;
					_gameMatrix[i][j] = nullptr;
				}

				// ������� ������ �����, � ���������� � ������� �������
				_gameMatrix[i][j] = CreateObject(ObjectType::Wall);
			}
		}
	}
}

void SnakeGame::GameBoard::CreateFoodOnRandomPosition()
{
	int maxFreeCells = GetNumberFreeCells() - GetSnakeSize();

	if (maxFreeCells > 0 )
	{
		int FoodLoopCounter = 0;

		do 
		{
			int iIndex = rand() % GAME_BOARD_WIDTH;
			int jIndex = rand() % GAME_BOARD_HEIGHT;

			auto * obj = _gameMatrix[iIndex][jIndex];

			if (!obj)
			{
				_gameMatrix[iIndex][jIndex] = CreateObject(ObjectType::Food);

				break;
			}

			FoodLoopCounter++;

		} 
		while (FoodLoopCounter <= maxFreeCells);
	}
}

void GameBoard::CreateFoodsAroundPerimeter()
{
	int maxFreeCells = GetNumberFreeCells();					// ���������� ��������� ������
	float coefficient = 0.2f;
	int shouldCreateFood = (maxFreeCells * coefficient) - 1;	// ������� ����� ������� ��� (����������). -1 ��� ������ ������.

	// ������� ��� ��� �������, �������
	int createdFoodCounter = 0;									

	// ���� ���� ������ ��������� ��� ����� � �� ���������
	shouldCreateFood = shouldCreateFood == 0 ? 1 : shouldCreateFood;

	assert(shouldCreateFood < maxFreeCells, "���������� ����������� ��� �� ������ ���� ������ ��������� ������ �� ����.");

	/*
		��������� ������� ���� ���� ���� createdFoodCounter < shouldCreateFood. 

		��������� ��������� ���������� � ������� �������� ���� ���, ���� ��� ��� ���-�� ����, ������ �����.
	*/
	do 
	{
		int iIndex = rand() % GAME_BOARD_WIDTH;
		int jIndex = rand() % GAME_BOARD_HEIGHT;

		auto * obj = _gameMatrix[iIndex][jIndex];

		if (!obj)
		{
			_gameMatrix[iIndex][jIndex] = CreateObject(ObjectType::Food);

			createdFoodCounter++;
		}

	} while (createdFoodCounter < shouldCreateFood);
}

// ����� ��� �������� � �������
WORD GetColorByObjectType(ObjectType type)
{
	switch (type)
	{
	case ObjectType::None:
		return 15;
	case ObjectType::Wall:
		return FOREGROUND_BLUE;
	case ObjectType::Food:
		return 10;
	case ObjectType::Snake:
		return 12;
	default:
		return 15;
	}
}

void GameBoard::PrintGameBoardToConsole()
{
	for (int i = 0; i < GAME_BOARD_WIDTH; i++)
	{
		for (int j = 0; j < GAME_BOARD_HEIGHT; j++)
		{
			auto * obj = _gameMatrix[i][j];

			char symbolToConsole = ' ';
				
			if (obj != NULL)
			{
				symbolToConsole = obj->GetSymbolForDisplay();
			}

			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, GetColorByObjectType(obj ? obj->ObjectType : ObjectType::None));

			Snake * SnakePart = nullptr;

			if (obj && obj->ObjectType == ObjectType::Snake)
			{
				SnakePart = dynamic_cast<Snake *>(obj);
			}

			bool forceUpdateCell = false;

			if (SnakePart && SnakePart->Head && SnakePart->Prev)
			{
				forceUpdateCell = true;

				if (SnakePart->Head == SnakePart)
				{
					HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
					SetConsoleTextAttribute(hConsole, 12);
				}
				else
				{
					HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
					SetConsoleTextAttribute(hConsole, 14);
				}
			}

			if (forceUpdateCell || _gameMatrixPrevStep[i][j] != symbolToConsole)
			{
				Console::PrintToCoordinates(j, i, symbolToConsole);
			}
		}
	}
}

void SnakeGame::GameBoard::ClearGameBoardConsole()
{
	for (int i = 0; i < GAME_BOARD_WIDTH; i++)
	{
		for (int j = 0; j < GAME_BOARD_HEIGHT; j++)
		{
			Console::PrintToCoordinates(j, i, ' ');
		}
	}
}

Object * GameBoard::CreateObject(ObjectType CreatedType)
{
	Object * newObject = nullptr;

	switch (CreatedType)
	{

	case ObjectType::Wall :
		newObject = new Wall();
		break;

	case ObjectType::Food:
		newObject = new Food();
		break;

	case ObjectType::Snake :
		newObject = new Snake();
		break;

	default:
		assert(false, "������� ������� ������, ��� �������� ��� �����.");
	}

	return newObject;
}

Object * GameBoard::FindGameObject(int x, int y)
{
	Object * returnObj = nullptr;

	if (IsPositionCorrect(x, y))
	{
		returnObj = _gameMatrix[x][y];
	}

	return returnObj;
}

bool GameBoard::IsPositionCorrect(int x, int y)
{
	return x >= 0 && y >= 0 && x < GAME_BOARD_WIDTH && y < GAME_BOARD_HEIGHT;
}

void GameBoard::MakeMovement()
{
	FindAndMakeNextSteps();
}

bool GameBoard::IsExit()
{
	// ���� �� ������������. �������� �� ���� ����� ��� ���������. 
	return false;
}

void GameBoard::CreateSnakeHead()
{
	auto * SnakeHead = FindHeadSnake();

	assert(!SnakeHead, "������ ��� �������. ������ ������� ��� ���� ������ ���� �.� �� ���� ��� ���� ����������.");

	/*
		��������� ��������� ���������� � ������� ������� ��� ������. 
		������ ���� �� ���������.
	*/
	if (!SnakeHead)
	{
		do
		{
			int iIndex = rand() % GAME_BOARD_WIDTH;
			int jIndex = rand() % GAME_BOARD_HEIGHT;

			auto * obj = _gameMatrix[iIndex][jIndex];

			if (!obj)
			{
				CreateSnakeHead(iIndex, jIndex);
			}

		// ����� ���� �� ��� ��� ���� ������ �� ����� ������� � �������� � ������ _gameMatrix
		} while (!FindHeadSnake());
	}
}

void GameBoard::CreateSnakeHead(int x, int y)
{
	assert(IsPositionCorrect(x, y), "������ ������� ���� �� ������������ �����������.");
	assert(!FindHeadSnake(), "������ ��� ���� �������!");

	auto * SnakePart = new Snake;
	SnakePart->X = x;
	SnakePart->Y = y;
	SnakePart->Head = SnakePart;

	_gameMatrix[x][y] = (Object*)SnakePart;
}

Snake * GameBoard::FindHeadSnake()
{
	Snake * returnHead = nullptr;

	for (int i = 0; i < GAME_BOARD_WIDTH; i++)
	{
		for (int j = 0; j < GAME_BOARD_HEIGHT; j++)
		{
			auto * obj = _gameMatrix[i][j];

			if (obj && obj->ObjectType == ObjectType::Snake)
			{
				auto * SnakeHead = dynamic_cast<Snake*>(obj);

				if (SnakeHead)
				{
					returnHead = SnakeHead->Head;

					break;
				}
			}
		}
	}

	return returnHead;
}

bool GameBoard::IsInteraction(int x, int y, ObjectType & outType)
{
	auto * head = FindHeadSnake();

	bool foundInteraction = false;

	if (head)
	{
		/*
			���� ����������, ������� ���� ��������� �� ����������� ��������� ������� ������ �� ������, �� ��� ������. 
			���� ����� ������ ������ ��, ��� � ����� ������ �� ��� - ��� � ��������. ��������� �� ������ � ��������������� �������� �� ������ ����?
		*/
		bool isNear = IsPositionsNear(head->X, head->Y, x, y);

		assert(isNear);

		if (isNear)
		{
			auto * obj = FindGameObject(x, y);

			if (obj)
			{
				outType = obj->ObjectType;

				foundInteraction = true;
			}
			else
			{
				outType = ObjectType::None;
			}
		}
	}

	return foundInteraction;
}

bool GameBoard::AttemptMoveOrEat(int x, int y)
{
	/*	
		��������� ����������� �� ���������� �����������
		����������� ������ ���� ����� ���
		���������� � ����� ����������
	*/

	// � ���������� interactionType ���������� ��� � ������� ������������ ����
	ObjectType interactionType = ObjectType::None;
	bool isInteraction = IsInteraction(x, y, interactionType);

	bool allowMove = false;

	bool bEatedFood = false;

	/*
		������� ��� � ����������� ������.
	*/
	if (isInteraction && interactionType == ObjectType::Food)
	{
		/*
			�� ����� ����������� ��������� ����� ������.
		*/
		bEatedFood = EatFoodAndCreatePart(x, y);
	}
	/*
		����������� �� ����������, ������ ���������� ������ �� ����� �����������.
	*/
	else if (!isInteraction)
	{
		allowMove = true;
	}
	else
	{
		assert(false, "�����-�� ��������� ������, �������� ������� :)");

		return false;
	}

	// ����������� � ���� �� ����, ���������� ���� �� ���������� �����������
	if (allowMove)
	{
		MoveSnakeTo(x, y);
	}

	return bEatedFood;
}

void GameBoard::MoveSnakeTo(int x, int y)
{
	auto * objByPosition = FindGameObject(x, y);	// ������� ������ �� ���������� �����������, ���� ������ ������, ������ � ��� ������ ����� ����������� ����
	auto * headSnake = FindHeadSnake();

	/*
		- ������� ���������
		- ������� �� ����� ����������� ���, ������ ���� ����� ����������� ������
		- ������ ������ ����������
		- ����� ������� � ������ ������ �����
	*/
	if (IsPositionCorrect(x, y) && !objByPosition && headSnake && IsPositionsNear(headSnake->X, headSnake->Y, x, y))
	{
		/*
			����������� ��������� ����. �������:

			- ������� ��������� ������� ������
			- �� ����� ����������� ������� ������, �����
		*/

		auto * lastPart  = FindLastPart();

		// � �������������� �������� ���� ����� ������� ���� � ��������� �����
		if (lastPart && lastPart->Next && lastPart->Next->Prev)
		{
			// lastPart->Next - Next � ���� ������ ������������� �������. 
			// Prev = nullptr - �������� ��������� �� ���������� (���������) �������
			lastPart->Next->Prev = nullptr;
		}

		// ���� ��������� ������� ����������, ������� ��� �� �������
		if (lastPart)
		{
			CleanObjectFromMatrix(lastPart, lastPart->X, lastPart->Y);
		}

		assert(lastPart, "��������� ������� �� ����� �� ������������. ����� �������, ���� ���� �� 1 - ��� � ���� ���������.");

		AddSnakePart(x, y);
	}
	else
	{
		assert(false, "�������� �������, ������, �� ���� ����������� ����!");
	}
}

Snake * GameBoard::FindLastPart()
{
	Snake * LastPart = FindHeadSnake();

	/*
		���������� �� ���� ���������� � ������ ���������.
	*/
	while (LastPart->Prev != nullptr)
	{
		LastPart = LastPart->Prev;
	}

	return LastPart;
}

PointsToFood GameBoard::SearchPathToNearestFood()
{
	/*
		��� ������ ���� ������� ����� ������� ���� �� ��������� ���.

		������ ������ �� ������ ������ �� ��� ����� �������� ��������� ����� (����� �������� ��������).
	*/
	int matrix [GAME_BOARD_WIDTH][GAME_BOARD_HEIGHT];

	// � ��� ���������� ����� �������� ������� ����� �� ��������� ���
	PointsToFood foundedPath;

	// �������������� ������� �������������� ����������, -1 ������ ��� ������ ��� �� ���� ��������� �� ������������
	for (int i = 0; i < GAME_BOARD_WIDTH; i++)
		for (int j = 0; j < GAME_BOARD_HEIGHT; j++)
			matrix[i][j] = -1;

	int dx[4] = { 1, 0, -1, 0 };   // ��������, ��������������� ������� ������ �� x �����������
	int dy[4] = { 0, 1, 0, -1 };   // ������, �����, ����� � ������

	auto * headSnake = FindHeadSnake();
	if (!headSnake)
	{
		assert(false, "������ ���� ������ ����!");
		return foundedPath;
	}

	// ���������� ��� �������, ���������� �� ���������� ����������� �� �������� ���� � ����� ���������� � ���������
	auto EqualsObjectByPositionAndType = [&](int x, int y, ObjectType type) -> bool
	{
		auto * obj = FindGameObject(x, y);

		if (obj)
		{
			return obj->ObjectType == type;
		}
		else
		{
			return ObjectType::None == type;
		}
	};

	int maxFreeCells = GetNumberFreeCells() - GetSnakeSize();


	//////////////////////////////////////////////////////////////////////////
	struct Vertex
	{
		Point	Position;					// ������� �� ������ ����
		int		Weight;						// ��� ������� (�� ������ ����� �� ���� �������)
		Vertex * Root;						// �������� ���� �������
		std::vector<Vertex *> Neighbors;	// ������ ���� ������� (���� ����� ����� �� ���� �������)

		Vertex(const Point& Position)
		{
			Weight = 0;
			Root = nullptr;
			this->Position = Position;
		}
	};

	// ���� ������� ��� ��������� ����� 
	std::vector<Vertex *> Vertexes;
	//////////////////////////////////////////////////////////////////////////


	Vertex * FoodVertex = nullptr;											// ��� ������ ��� ����� ������� �������� ��������� �� �� �������.
	Vertexes.push_back(new Vertex(Point(headSnake->X, headSnake->Y)));		// �������� ������ ������� �� ������� ������ ������� ����
	matrix[headSnake->X][headSnake->Y] = 0;									// ������� ������� � ������� ������ �� �����
	int CountFindNextPosition = 0;											// ���������� ������ ���-�� ��������� ������� ��������� �� 1 ������

	// ������ ��� ���������� ������ �������� � ����
	auto CreateNewVertex = [&](Vertex * Parent, const Point & Position) -> Vertex*
	{
		Vertex * NewNeighbor = new Vertex(Position);	// ������� ����� ������� �� ���������� �������
		NewNeighbor->Weight  = Parent->Weight + 1;		// ������������� �� ��� + 1 �� ��������
		NewNeighbor->Root    = Parent;					// ������� ������� ��� �� ��������

		Parent->Neighbors.push_back(NewNeighbor);		// ����� ������� ��������� � ������� ��������
		Vertexes.push_back(NewNeighbor);				// ��� �� ����� ������� ��������� � ����� ������ ���� ������

		matrix[NewNeighbor->Position.X][NewNeighbor->Position.Y] = NewNeighbor->Weight;	// �������� � ������� ��������, ��� ����� �� ��� ����

		CountFindNextPosition++;	// ����������� ���������� ��������� ������ �� 1 ������

		return NewNeighbor;
	};

	do 
	{
		int InitialSize = Vertexes.size();
		int StartIndex = InitialSize - CountFindNextPosition - (CountFindNextPosition == 0 ? 1 : 0);
		CountFindNextPosition = 0;

		for (int i = StartIndex; i < InitialSize; i++)
		{
			if (FoodVertex)
				break;

			Vertex * CurrentVertex = Vertexes[i];

			Point Position = CurrentVertex->Position;

			for (int k = 0; k < 4; ++k)
			{
				Point FindPosition(Position.X + dx[k], Position.Y + dy[k]);
				bool IsFreeSpace = matrix[FindPosition.X][FindPosition.Y] == -1;

				if (IsFreeSpace && IsPositionCorrect(FindPosition.X, FindPosition.Y))
				{
					bool IsWall  = EqualsObjectByPositionAndType(FindPosition.X, FindPosition.Y, ObjectType::Wall);
					bool IsSnake = EqualsObjectByPositionAndType(FindPosition.X, FindPosition.Y, ObjectType::Snake);

					if (!IsWall && !IsSnake)
					{
						Vertex * NewestPart = CreateNewVertex(CurrentVertex, FindPosition);

						bool IsFood = EqualsObjectByPositionAndType(FindPosition.X, FindPosition.Y, ObjectType::Food);

						if (IsFood)
						{
							if (!FoodVertex)
							{
								FoodVertex = NewestPart;
								break;
							}
						}
					}
				}
			}
		}

	} while (!FoodVertex && CountFindNextPosition > 0);

	// ��������� � ������ foundedPath ����� ������ �� ���� ���������� x � y.
	// ������ ������ ��� ����� ���� �� ��������� ���.
	auto AddPositionToPath = [&](int x, int y)
	{
		foundedPath.push_back(std::vector<int>{x, y});
	};

	Vertex * Root = FoodVertex;
	while (Root != nullptr && Vertexes.size() != 0 && Vertexes[0] != Root)
	{
		AddPositionToPath(Root->Position.X, Root->Position.Y);

		Root = Root->Root;
	}

	for (Vertex * vertex : Vertexes)
	{
		delete vertex;
	}
	Vertexes.clear();

	return foundedPath;
}

bool GameBoard::EatFoodAndCreatePart(int x, int y)
{
	auto * obj = FindGameObject(x, y);

	if (obj && obj->ObjectType == ObjectType::Food)
	{
		CleanObjectFromMatrix(obj, x, y);
		
		AddSnakePart(x, y);

		return true;
	}
	else
	{
		assert(false, "������, ������� ���� �������� ������ �� ��� ��� �� ����������.");
	}

	return false;
}

void GameBoard::AddSnakePart(int x, int y)
{
	// �������� ������� ������ ����
	// prev - ������, ��� ��� ����� ����� ���������� ������ �.� �� ������ �������� �����
	auto * prevHead = FindHeadSnake();

	/*
		- ���������� �� ������ ������ (����� �� ������������ � ������ ���� ������ ���� � ���� ���� ����� ������, ������� ������ ��� ��������� � ��������� ������ �� ����� �����������)
		- ������ �� ���������� �� ������� ����� ������� ����� ����� ������
	*/
	if (prevHead && IsPositionCorrect(x, y) && IsPositionsNear(prevHead->X, prevHead->Y, x, y))
	{
		// ������� ����� ����� ����, ��� ����� ����� �������
		auto * newHead = dynamic_cast<Snake*>(CreateObject(ObjectType::Snake));

		// ��������� � ������� ����� ������
		_gameMatrix[x][y] = newHead;

		// ���������� ��� ������ �� ����� ����������
		newHead->X = x;
		newHead->Y = y;

		/*
			������������� ����� ������:
			- ������ ��� � ���� ������ ����
			- ���������� �������� ��� �.� ������ ������ �������
			- ���������� ��������� ���������� ���������� ������
		*/
		newHead->Head = newHead;
		newHead->Next = nullptr;
		newHead->Prev = prevHead;

		prevHead->Next = newHead;	// � ������� ������ ��������� ��������� ���������� ����� ������

		/*
			����� ����������� �� ���� ��������� ���������� ���������
			� ��������� � ���� ��������� �� ����� ������.
		*/
		Snake * SnakePrev = newHead->Prev;

		while (SnakePrev != nullptr)
		{
			SnakePrev->Head = newHead;

			SnakePrev = SnakePrev->Prev;
		}
	}
	/*
		- ���� ������ ���, ������ �� ������� � ����� ������ ������� �� ����� �����������
	*/
	else if (!prevHead)
	{
		CreateSnakeHead(x, y);
	}
	else
	{
		assert(false, "�������� �������.");
	}
}

bool GameBoard::IsPositionsNear(int cX, int cY, int nX, int nY)
{
	// � - current/�������, n - next/���������
	int difference = abs(cX - nX) + abs(cY - nY);

	// ���������� ������� ����� ������� ���� � ���� ������ � ���� ������, 1 - ������ ������� ����� � � ��� ����� ��������.
	return difference > 0 && difference < 2;
}

void GameBoard::FindAndMakeNextSteps()
{
	// ����� ���� �� ��������� ���
	PointsToFood points = SearchPathToNearestFood();

	// ��� ������ ��� ���� �� ������ ���� �� ��������� ���
	// ���� ��� ������� ��� ������ � ��������� :(
	if (points.size() == 0)
	{
		CleanGameBoard();
		InitializeGameBoard();

		return;
	}

	// ����� ���� ��� ����� ���� �� ��������� ��� ����������� �� ������� �������� ���� � ���������� �� ���� ������
	for(int i = points.size()-1; i >= 0 ; --i)
	{
		/*
			points - ��� ������, ������� � ���� �������� �������. 
			������ ���������� ������ �������� � ���� 2 ���� - x � y.
		*/

		auto & vector = points[i];

		assert(vector.size() == 2, "������ ����� ������� ������ 2 �������� 'x' � 'y'. ����� ���� ������. ������ ����� ������� ������ ������ �� 2.");	

		// ������� ����� � �������� ��������� �������� ����
		CopyMatrixDataToTempMatrix();

		// ������� ����������, �� ������� ����� ����������� ����
		int x = vector[0];
		int y = vector[1];

		// ������� ������� ������ ����
		auto * SnakeHead = FindHeadSnake();

		// ���������, - ������ � ����� ���������� �����? . - ���������� �� ���������� �� ������� ����� ����������� ������.
		bool isPositionNear		 = IsPositionsNear(SnakeHead->X, SnakeHead->Y, x, y);
		bool isPositionCorrected = IsPositionCorrect(x, y);

		// ���� ��� �����, �� ������� ����������� ������ ��� ������ ���
		if (isPositionNear && isPositionCorrected)
		{
			// ���� ������� ���
			if (AttemptMoveOrEat(x, y))
			{
				CreateFoodOnRandomPosition();
			}
		}
		else
		{
			assert(false, "� ���� ����� �� ������ ���� ���������� ������. ��������, ��� �� ���������� ������ ��� ����� ������� ������.");
		}

		// ������������ � ������� ��������� �������� ����
		PrintGameBoardToConsole();

		// ������������� �������� �����, ����� ������ ��� ��� �� ��� ������
		//Sleep(0);
	}
}
