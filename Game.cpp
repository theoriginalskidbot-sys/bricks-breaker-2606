#include "stdafx.h"
#include "Game.h"

Game::Game()
{
	Reset();
}

void Game::Reset()
{
	gameWon = false;
	gameLost = false;

	Console::SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	Console::CursorVisible(false);
	paddle.width = 12;
	paddle.height = 2;
	paddle.x_position = 32;
	paddle.y_position = 30;

	ball.visage = 'O';
	ball.color = ConsoleColor::Cyan;
	ResetBall();

	// - Add this brick and 4 more bricks to the vector

	bricks.clear();
	bricksHits.clear();


	const int space = (WINDOW_WIDTH - 5 * 10) / 6;

	for (int i = 0; i < 5; i++)
	{
		Box brick;

		brick.width = 10;
		brick.height = 2;
		brick.x_position = space + i * (10 + space);
		brick.y_position = 5;
		brick.doubleThick = true;
		brick.color = ConsoleColor::DarkGreen;

		bricks.push_back(brick);
		bricksHits.push_back(0);
	}
}


void Game::ResetBall()
{
	ball.x_position = paddle.x_position + paddle.width / 2;
	ball.y_position = paddle.y_position - 1;
	ball.x_velocity = rand() % 2 ? 1 : -1;
	ball.y_velocity = -1;
	ball.moving = false;
}

bool Game::Update()
{
	if (GetAsyncKeyState(VK_ESCAPE) & 0x1)
		return false;

	if (GetAsyncKeyState(VK_RIGHT) && paddle.x_position < WINDOW_WIDTH - paddle.width)
		paddle.x_position += 2;

	if (GetAsyncKeyState(VK_LEFT) && paddle.x_position > 0)
		paddle.x_position -= 2;

	if (GetAsyncKeyState(VK_SPACE) & 0x1)
		ball.moving = !ball.moving;

	if (GetAsyncKeyState('R') & 0x1)
		Reset();

	ball.Update();
	CheckCollision();
	return true;
}

//  All rendering, including text, should occur in the Render function
void Game::Render() const
{
	Console::Lock(true);
	Console::Clear();

	paddle.Draw();
	ball.Draw();

	// #3 - Update render to render all bricks
	for (const Box& brick : bricks)
	{
		brick.Draw();
	}

	if (gameWon)
	{
		Console::ForegroundColor(ConsoleColor::White);
		Console::WordWrap((WINDOW_WIDTH - 30) / 2, WINDOW_HEIGHT / 2, 30, "You win! Press R to play again.");

		Console::ResetColor();
	}
	if (gameLost)
	{
		Console::ForegroundColor(ConsoleColor::White);
		Console::WordWrap((WINDOW_WIDTH - 30) / 2, WINDOW_HEIGHT / 2, 30, "You lose. Press R to play again.");

		Console::ResetColor();
	}


	Console::Lock(false);
}

void Game::CheckCollision()
{
	if (gameWon || gameLost)
	{
		ball.moving = false;
		return;
	}

	//  #4 - Update collision to check all bricks
	for (std::size_t i = 0; i < bricks.size(); ++i)
	{
		if (bricks[i].Contains(ball.x_position + ball.x_velocity, ball.y_position + ball.y_velocity))
		{
			ball.y_velocity *= -1;
			++bricksHits[i];

			//  #5 - If the ball hits the same brick 3 times (color == black), remove it from the vector
			if (bricksHits[i] >= 3)
			{
				bricks.erase(bricks.begin() + i);
				bricksHits.erase(bricksHits.begin() + i);
			}

			break;
		}
	}


//  #6 - If no bricks remain, pause ball and display (render) victory text with R to reset

if (bricks.empty())
{
	gameWon = true;
	ball.moving = false;
	return;
}
if (paddle.Contains(ball.x_position + ball.x_velocity, ball.y_velocity + ball.y_position))
{
	ball.y_velocity *= -1;
}

//  #7 - If ball touches bottom of window, pause ball and display (render) defeat text with R to reset
if (ball.y_position >= WINDOW_HEIGHT - 1)
{
	gameLost = true;
	ball.moving = false;
	return;
}

}
