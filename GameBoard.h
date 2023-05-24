#pragma once

/*
	������ �����. 2017 ���. 

	skype : nestaby
*/

#include "Object.h"
#include "Consts.h"
#include <stdlib.h>
#include <vector>

/*
	������ ����� ������� ������ �� ��������. ������ ���������� ������ ����� ������� ������ 2 �������� - x , y. 
	����� �������������� ��� ���������� ���� �� ������ ������ �� ��������� ���.
*/
typedef std::vector<std::vector<int>> PointsToFood;

class Snake;

/*
	GameBoard - ������� ���� �������������, ��������� �������, �������.

	���� ������ � ���� ������� - �����, ���, ����. 

	�������������� ��������� ����, �������� ��� � ������ ����, ������� ������������� ���� ����
	�� ��������� ��� ��� ������ ��������� ���������.
*/

namespace SnakeGame 
{
	class GameBoard
	{
	private:
		/*
			������� ����. 

			���� ����� � ���� ������� �������� �������. ���� ������ ������ nullptr - ������ � ��� ������ ���.
			���� ��� ���������� Object, ������ ��� ��� Wall ��� Snake ��� Food. ��� ��� ������� ������������ �� Object.
		*/
		Object * _gameMatrix[GAME_BOARD_WIDTH][GAME_BOARD_HEIGHT];

		/*
			����� ������ ����� ������ ��������� ���������� ��������� ���� ��� ����, ����� ��� ������ ������ ��������
			������ �� ������� ����, ������� ����������. �����, ����� ������� �� ������� ��� ���������� ������.
		*/
		char _gameMatrixPrevStep[GAME_BOARD_WIDTH][GAME_BOARD_HEIGHT];

	public:
		GameBoard();

	// ������ � ���������� �� ����, ������ �������������, �������� ����, ���, ������ ���� � �.�
	private:
		void CreateGameBoard();											// ������ �������� �������� ����
		void CleanGameBoard();											// ������� �������� ����
		void InitializeGameBoard();										// ������������� �������� ���� (��������� ���, ������, �����)
		void CreateWallsAroundPerimeter();								// ������� ����� �� ���������
		void CreateFoodsAroundPerimeter();								// ���������� ��� �� ������� ����
		void CreateFoodOnRandomPosition();												// ������� ���� ��� ���� ��� ��������
		void CopyMatrixDataToTempMatrix();

	// ������ � ��������� �� ����, ��������, ��������, ������ ��������
	private:
		Object * CreateObject(ObjectType);								// ������� � ���������� ������ �� ����������� ����
		Object * FindGameObject(int x, int y);							// ������� � ���������� ������ �� ������� �� �����������
		bool IsPositionCorrect(int x, int y);							// ��������, ������ �� ���������� ���������� � �������� �������. false ���� < 0 ��� > ������� ����
		void CleanObjectFromMatrix(Object *, int x, int y);				// ������� ������� �� ������� � ������� ������ �� ������. ���������� ��� ������ � ��� ���������� � �������.
		int GetNumberFreeCells();

	// ������ �� �������
	private:
		void CreateSnakeHead(int x, int y);								// ��������� ������ ��� ���� �� �������� �����������
		void CreateSnakeHead();											// ��������� ������ ��� ���� � ��������� �����������
		Snake * FindHeadSnake();										// ������� � ���������� ������ ���� � �������. ���� �� ���, ���������� nullptr
		bool IsInteraction(int x, int y, ObjectType &);					// ����������� �� ������ ���� � ���-�� �� �����������, ���������� true ���� ����������� � ������� �� ������ ��� �����������
		bool AttemptMoveOrEat(int x, int y);							// �������� �� ����������� � ����, ���� �����������, ��������� ����� ������, ���� ���, �� ���������� ������ � ������ ������
		bool EatFoodAndCreatePart(int x, int y);						// ������ ��� � �������� ������� � ����
		void AddSnakePart(int x, int y);								// �������� � ���� �������. ���������� ���� ���� ��� ���� ���.
		void MoveSnakeTo(int x, int y);									// ����������� ���� � �������� ����������
		bool IsPositionsNear(int cX, int cY, int nX, int nY);			// ���������, ����� �� ����������. ��������, ������� ����� ��� ����������� ������ ������. ������ ������ ����� ������������ ������ �� 1 ������
		void FindAndMakeNextSteps();									// ������ ������� ���� �� ��������� ���. ������������ ��������� ������� ��� ������, ����������� �� ���� ����. ���������� � �.�.
		Snake * FindLastPart();											// ����� ���������� �������� ������. ���� ���������� ���, �� ��������� ����� �������.
		int GetSnakeSize();

	// ����� ��������� �� ��������� ���
	private:
		PointsToFood SearchPathToNearestFood();							// ��� ������ ������������ �������� �������� ������ ����.

	// ������ � �������� ����� ������
	public:
		void MakeMovement();											// ������ ��� �������� ������� (� ����� �� �����), ������ ���
		bool IsExit();													// ����� �� ���� ��� ��������� �����. ���� �� �������������.

	// ������ � ��������
	private:
		void PrintGameBoardToConsole();									// ����� ������� ���� ������ ����������� ������ � �������. ������������ ������ �� ������, ������� ���� ���������, ����� �� ��������� ������� � ������ �� �������.
		void ClearGameBoardConsole();
	};

}