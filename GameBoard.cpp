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
	Работа с консолью.

	Отрисовка элементов игрового поля происходит по переданным координатам и символу.
*/
namespace Console
{
	// переместить курсор в консоли по координатам
	void Gotoxy(int x, int y)
	{
		COORD coord;
		coord.X = x;
		coord.Y = y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	}

	// отрисовать символ по координатам в консоли
	void PrintToCoordinates(int x, int y, char ch)
	{
		Gotoxy(x, y);
		std::cout << ch;
	}
}


GameBoard::GameBoard()
{
	srand(time(nullptr));
	
	CreateGameBoard();						// обнуление всех элементов игровой матрицы. подготовка перед созданием игрового поля.
	InitializeGameBoard();					// инициализация всего на поле		
}

void GameBoard::CleanObjectFromMatrix(Object * obj, int x, int y)
{
	/*
		Проверка перед удалением элемента из матрицы.
		- передали правильные координаты
		- переданный объект не null
		- объект в матрице по переденным координатам не null
		- объект в матрице соответстует переданному объекту

		Если все проверки пройдены, значит можно удалть объект и очищать матрицу по координатам.
	*/
	if (IsPositionCorrect(x, y) && obj && _gameMatrix[x][y] && _gameMatrix[x][y] == obj)
	{
		delete obj;
		_gameMatrix[x][y] = nullptr;
	}
	else
	{
		assert(false, "Ошибка при удалении элемента из матрицы.");
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
	ClearGameBoardConsole();				// очищает консоль
	CleanGameBoard();						// очищаем все объекты с поля если они были
	CreateWallsAroundPerimeter();			// создаем стены
	CreateSnakeHead();					// создаем змейку
	//CreateSnakeHead(1,1);
	CreateFoodOnRandomPosition();			// создаем рандомно первую еду
	//CreateFoodsAroundPerimeter();
	PrintGameBoardToConsole();				// отрисовка игровго поля
}

void GameBoard::CreateWallsAroundPerimeter()
{
	for (int i = 0; i < GAME_BOARD_WIDTH; i++)
	{
		for (int j = 0; j < GAME_BOARD_HEIGHT; j++)
		{
			// Находим координаты стен и по ним создаем реальные объекты стен.
			if (i == 0 || j == 0 || i == GAME_BOARD_WIDTH-1 || j == GAME_BOARD_HEIGHT-1)
			{
				auto * obj = _gameMatrix[i][j];

				// Если что-то есть внутри массива по этим поординатам, удаляем и пересоздаем стену.
				if (obj != nullptr)
				{
					delete obj;
					_gameMatrix[i][j] = nullptr;
				}

				// создаем объект стена, и записываем в игровую матрицу
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
	int maxFreeCells = GetNumberFreeCells();					// количество свободных клеток
	float coefficient = 0.2f;
	int shouldCreateFood = (maxFreeCells * coefficient) - 1;	// Сколько можно создать еды (количество). -1 для головы змейки.

	// сколько еды уже создано, счетчик
	int createdFoodCounter = 0;									

	// если поле совсем маленькое еда может и не создаться
	shouldCreateFood = shouldCreateFood == 0 ? 1 : shouldCreateFood;

	assert(shouldCreateFood < maxFreeCells, "Количество создаваемйе еды не должно быть больше свободных клеток на поле.");

	/*
		Заполняем игровое поле едой пока createdFoodCounter < shouldCreateFood. 

		Генерирую рандомные координаты и пытаюсь вставить туда еду, если там уже что-то есть, пробую снова.
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

// цвета для объектов в консоли
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
		assert(false, "Попытка создать объект, для которого нет кейса.");
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
	// пока не используется. выходить из игры когда она закончена. 
	return false;
}

void GameBoard::CreateSnakeHead()
{
	auto * SnakeHead = FindHeadSnake();

	assert(!SnakeHead, "Змейка уже создана. Нельзя создать еще одну голову змеи т.к на поле уже одна существует.");

	/*
		Генерирую рандомные координаты и пытаюсь создать там змейку. 
		Пробую пока не получится.
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

		// кручу цикл до тех пор пока змейка не будет создана и записана в массив _gameMatrix
		} while (!FindHeadSnake());
	}
}

void GameBoard::CreateSnakeHead(int x, int y)
{
	assert(IsPositionCorrect(x, y), "Нельзя создать змею по некорректным координатам.");
	assert(!FindHeadSnake(), "Голова уже была создана!");

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
			Если координаты, которые надо проверить на пересечение находятся слишком далеко от головы, то это ошибка. 
			Змея может съесть только то, что в однйо ктетке от нее - это и проверим. Находится ли объект в непосредстенной близости от головы змеи?
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
		Проверяем пересечение по переданным координатам
		Увеличиваем змейку если съели еду
		Перемещаем в новые координаты
	*/

	// в переменную interactionType записываем тип с которым пересекается змея
	ObjectType interactionType = ObjectType::None;
	bool isInteraction = IsInteraction(x, y, interactionType);

	bool allowMove = false;

	bool bEatedFood = false;

	/*
		Съедаем еду и увеличиваем змейку.
	*/
	if (isInteraction && interactionType == ObjectType::Food)
	{
		/*
			По новым координатам добавляем новую голову.
		*/
		bEatedFood = EatFoodAndCreatePart(x, y);
	}
	/*
		Пересечение не обнаружено, значит перемещаем змейку по новым координатам.
	*/
	else if (!isInteraction)
	{
		allowMove = true;
	}
	else
	{
		assert(false, "Какая-то неведомая ошибка, проверяй условия :)");

		return false;
	}

	// пересечения с едой не было, перемещаем змею по переданным координатам
	if (allowMove)
	{
		MoveSnakeTo(x, y);
	}

	return bEatedFood;
}

void GameBoard::MoveSnakeTo(int x, int y)
{
	auto * objByPosition = FindGameObject(x, y);	// достаем объект по переданным координатам, если объект пустой, значит в эту клетку можно переместить змею
	auto * headSnake = FindHeadSnake();

	/*
		- позиция коррекнта
		- объекта по новым координатам нет, значит туда можно передвинуть змейку
		- голова змейки существует
		- новая позиция и голова змейки рядом
	*/
	if (IsPositionCorrect(x, y) && !objByPosition && headSnake && IsPositionsNear(headSnake->X, headSnake->Y, x, y))
	{
		/*
			Перемещение реализуем след. образом:

			- удаляем последний элемент змейки
			- по новым координатам создаем голову, вуаля
		*/

		auto * lastPart  = FindLastPart();

		// у предпоследнего элемента змеи нужно удалить свзь с последней змеей
		if (lastPart && lastPart->Next && lastPart->Next->Prev)
		{
			// lastPart->Next - Next в этом случае предпоследний элемент. 
			// Prev = nullptr - обнуляем указатель на предыдущий (последний) элемент
			lastPart->Next->Prev = nullptr;
		}

		// если последний элемент существует, удаляем его из матрицы
		if (lastPart)
		{
			CleanObjectFromMatrix(lastPart, lastPart->X, lastPart->Y);
		}

		assert(lastPart, "Последний элемент не может не существовать. Любой элемент, даже если он 1 - это и есть последний.");

		AddSnakePart(x, y);
	}
	else
	{
		assert(false, "Проверяй условия, ошибка, не могу передвинуть змею!");
	}
}

Snake * GameBoard::FindLastPart()
{
	Snake * LastPart = FindHeadSnake();

	/*
		Пробежимся по всем предыдущим и найдем последний.
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
		При помощи этой матрицы будут найдены пути до ближайшей еды.

		Каждая клетка от головы змейки до еды будет помочена счетчиком волны (читай волновой алгоритм).
	*/
	int matrix [GAME_BOARD_WIDTH][GAME_BOARD_HEIGHT];

	// в эту переменную будут записаны вектора путей до ближайшей еды
	PointsToFood foundedPath;

	// инициализируем матрицу пероначальными значениями, -1 значит что клетка еще не была проверена на проходимость
	for (int i = 0; i < GAME_BOARD_WIDTH; i++)
		for (int j = 0; j < GAME_BOARD_HEIGHT; j++)
			matrix[i][j] = -1;

	int dx[4] = { 1, 0, -1, 0 };   // смещения, соответствующие соседям ячейки по x координатам
	int dy[4] = { 0, 1, 0, -1 };   // справа, снизу, слева и сверху

	auto * headSnake = FindHeadSnake();
	if (!headSnake)
	{
		assert(false, "Голова змеи должна быть!");
		return foundedPath;
	}

	// сравнивает тип объекта, полученный по переданным координатам из игрового поля с типом переданным в параметры
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
		Point	Position;					// позиция на игрвом поле
		int		Weight;						// вес вершины (от начала графа до этой вершины)
		Vertex * Root;						// родитель этой вершины
		std::vector<Vertex *> Neighbors;	// соседи этой вершины (куда можно пойти из этой вершины)

		Vertex(const Point& Position)
		{
			Weight = 0;
			Root = nullptr;
			this->Position = Position;
		}
	};

	// сюда запишем все найденные точки 
	std::vector<Vertex *> Vertexes;
	//////////////////////////////////////////////////////////////////////////


	Vertex * FoodVertex = nullptr;											// как только еда будет найдена сохраним указатель на ее Вершину.
	Vertexes.push_back(new Vertex(Point(headSnake->X, headSnake->Y)));		// создадим первую вершину от которой пойдем строить граф
	matrix[headSnake->X][headSnake->Y] = 0;									// пометим позицию в которой искать не нужно
	int CountFindNextPosition = 0;											// переменная хранит кол-во элементов соседей найденных за 1 проход

	// лямбда для добавления нового элемента в граф
	auto CreateNewVertex = [&](Vertex * Parent, const Point & Position) -> Vertex*
	{
		Vertex * NewNeighbor = new Vertex(Position);	// создаем новую вершину по переданной позиции
		NewNeighbor->Weight  = Parent->Weight + 1;		// устанавливаем ей вес + 1 от родителя
		NewNeighbor->Root    = Parent;					// говорим вершине кто ее родитель

		Parent->Neighbors.push_back(NewNeighbor);		// новую вершину добавляем к соседям родителя
		Vertexes.push_back(NewNeighbor);				// так же новую вершину добавляем в общий вектор всех вершин

		matrix[NewNeighbor->Position.X][NewNeighbor->Position.Y] = NewNeighbor->Weight;	// помечаем в матрице проходов, что здесь мы уже были

		CountFindNextPosition++;	// увеличиваем количество найденных вершин за 1 проход

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

	// добавляем в вектор foundedPath новый вектор из двух переменных x и y.
	// каждый объект это часть пути до ближайшей еды.
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
		assert(false, "Объект, который змея пытается съесть не еда или не существует.");
	}

	return false;
}

void GameBoard::AddSnakePart(int x, int y)
{
	// получаем текущую голову змеи
	// prev - потому, что это скоро будет предыдущая голова т.к мы сейчас создадим новую
	auto * prevHead = FindHeadSnake();

	/*
		- существует ли голова змейки (может не существовать в случае если голова пока и есть одна часть змейки, которая каждый ход удаляется и создается заного по новым координатам)
		- верные ли координаты по которым нужно создать новую часть змейки
	*/
	if (prevHead && IsPositionCorrect(x, y) && IsPositionsNear(prevHead->X, prevHead->Y, x, y))
	{
		// создаем новую часть змеи, она будет новой головой
		auto * newHead = dynamic_cast<Snake*>(CreateObject(ObjectType::Snake));

		// вставляем в матрицу новую голову
		_gameMatrix[x][y] = newHead;

		// записываем для головы ее новые координаты
		newHead->X = x;
		newHead->Y = y;

		/*
			Устанавливаем новой голове:
			- теперь она и есть голова змеи
			- следующего элемента нет т.к голова первый элемент
			- предыдущим элементом становится предыдущая голова
		*/
		newHead->Head = newHead;
		newHead->Next = nullptr;
		newHead->Prev = prevHead;

		prevHead->Next = newHead;	// у прошлой головы следующим элементом становится новая голова

		/*
			Далее пробегаемся по всем имеющимся предыдущим элементам
			и обновляем у всех указатель на новую голову.
		*/
		Snake * SnakePrev = newHead->Prev;

		while (SnakePrev != nullptr)
		{
			SnakePrev->Head = newHead;

			SnakePrev = SnakePrev->Prev;
		}
	}
	/*
		- если головы нет, значит ее удалили и нужно заного создать по новым координатам
	*/
	else if (!prevHead)
	{
		CreateSnakeHead(x, y);
	}
	else
	{
		assert(false, "Проверяй условия.");
	}
}

bool GameBoard::IsPositionsNear(int cX, int cY, int nX, int nY)
{
	// с - current/текущий, n - next/следующий
	int difference = abs(cX - nX) + abs(cY - nY);

	// допустимая разница между головой змеи и едой только в одну клетку, 1 - значит позиция рядом и с ней можно работать.
	return difference > 0 && difference < 2;
}

void GameBoard::FindAndMakeNextSteps()
{
	// поиск пути до блидайщей еды
	PointsToFood points = SearchPathToNearestFood();

	// это значит нет пути от головы змеи до ближайшей еды
	// змея все скушала или ошибка в алгоритме :(
	if (points.size() == 0)
	{
		CleanGameBoard();
		InitializeGameBoard();

		return;
	}

	// после того как нашли путь до ближайшей еды пробешаемся по каждому элементу пути и перемещаем по нему змейку
	for(int i = points.size()-1; i >= 0 ; --i)
	{
		/*
			points - это вектор, который в себе содержил вектора. 
			Каждый внутренний вектор содержит в себе 2 инта - x и y.
		*/

		auto & vector = points[i];

		assert(vector.size() == 2, "Вектор может хранить только 2 значения 'x' и 'y'. Здесь явно ошибка. Смотри каким образом вектор хранит не 2.");	

		// снимаем копию с текущего состояния игрового поля
		CopyMatrixDataToTempMatrix();

		// достаем координаты, по которым нужно переместить змею
		int x = vector[0];
		int y = vector[1];

		// достаем текущую голову змеи
		auto * SnakeHead = FindHeadSnake();

		// проверяем, - голова и новые координаты рядом? . - правильные ли координаты по которым нужно переместить змейку.
		bool isPositionNear		 = IsPositionsNear(SnakeHead->X, SnakeHead->Y, x, y);
		bool isPositionCorrected = IsPositionCorrect(x, y);

		// если все верно, то пробуем переместить змейку или съесть еду
		if (isPositionNear && isPositionCorrected)
		{
			// если скушали еду
			if (AttemptMoveOrEat(x, y))
			{
				CreateFoodOnRandomPosition();
			}
		}
		else
		{
			assert(false, "В этом блоке не должно быть логических ошибок. Проверяй, что за координаты пришли для новой позиции змейки.");
		}

		// отрисовываем в консоль состояние игрового поля
		PrintGameBoardToConsole();

		// останавливаем основной поток, чтобы змейка ела еду не так быстро
		//Sleep(0);
	}
}
