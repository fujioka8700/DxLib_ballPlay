//************************************************
// DXライブラリ マウスで玉遊び(基本)
// 作成日：2024/04/04
// 作成者：fujioka8700
// Copyright (c) fujioka8700 All rights reserved.
//************************************************

#include <math.h>
#include <Dxlib.h>

typedef struct {
	float X, Y;
} CRD; // coordinate

typedef struct {
	CRD	  POS;
	CRD   SPEED;
	float RADIUS;
} BALL;

//================================================
// グローバル変数宣言
//================================================
BALL     Ball;
LONGLONG fpsTimer, deltaTimer;
bool     MouseLeftPress, MouseLeftRelease;
CRD      MousePressPos, MouseReleasePos;

//================================================
// FPSの計測と描画
//================================================
void FpsDraw(LONGLONG* p)
{
#ifdef _DEBUG
	static int Fps = 0, FpsCnt = 0;
	LONGLONG now = GetNowHiPerformanceCount();

	FpsCnt++;
	if (now - *p > 1000000)
	{
		Fps = FpsCnt;
		FpsCnt = 0;
		*p = now;
	}
	DrawFormatString(0, 0, GetColor(255, 255, 255), "FPS: %d", Fps);
#endif // !_DEBUG
}

//================================================
// デルタタイムの計測
//================================================
float getDeltaTime(LONGLONG* p)
{
	LONGLONG now = GetNowHiPerformanceCount();
	float    deltaTime = (float)(now - *p) / 1000000.0f;

	*p = now;
	return deltaTime;
}

//================================================
// 初期処理
//================================================
void Init(void)
{
	fpsTimer = deltaTimer = GetNowHiPerformanceCount();

	Ball.POS.X = 320, Ball.POS.Y = 240;
	Ball.SPEED.X = 0, Ball.SPEED.Y = 0;
	Ball.RADIUS = 20;

	SetDrawScreen(DX_SCREEN_BACK);
}

//================================================
// 終了処理
//================================================
void End(void)
{

}

//================================================
// 入力処理
//================================================
void Input(void)
{
	int button, x, y, type;

	MouseLeftPress = MouseLeftRelease = false;

	if (GetMouseInputLog2(&button, &x, &y, &type, TRUE) == 0)
	{
		if ((button & MOUSE_INPUT_LEFT) != 0)
		{
			if (type == MOUSE_INPUT_LOG_DOWN)
			{
				MouseLeftPress = true;
				MousePressPos.X = (float)x, MousePressPos.Y = (float)y;
			}
			else if (type == MOUSE_INPUT_LOG_UP)
			{
				MouseLeftRelease = true;
				MouseReleasePos.X = (float)x, MouseReleasePos.Y = (float)y;
			}
		}
	}
}

//================================================
// 更新処理
//================================================
void Update(void)
{
	float deltaTime = getDeltaTime(&deltaTimer);

	Ball.POS.X += Ball.SPEED.X * deltaTime;
	Ball.POS.Y += Ball.SPEED.Y * deltaTime;

	if (Ball.POS.X + Ball.RADIUS >= 640.0f)
	{
		Ball.POS.X = 640.0f - Ball.RADIUS;
		Ball.SPEED.X *= -1;
	}
	else if (Ball.POS.X - Ball.RADIUS < 0.0f) {
		Ball.POS.X = Ball.RADIUS;
		Ball.SPEED.X *= -1;
	}

	if (Ball.POS.Y + Ball.RADIUS >= 480.0f)
	{
		Ball.POS.Y = 480.0f - Ball.RADIUS;
		Ball.SPEED.Y *= -1;
	}
	else if (Ball.POS.Y - Ball.RADIUS < 0.0f) {
		Ball.POS.Y = Ball.RADIUS;
		Ball.SPEED.Y *= -1;
	}

	double d = pow(Ball.POS.X - MousePressPos.X, 2) + pow(Ball.POS.Y - MousePressPos.Y, 2);
	static int pressTime = 0;

	if (MouseLeftPress && d <= pow(Ball.RADIUS, 2))
	{
		Ball.SPEED.X = Ball.SPEED.Y = 0;
		pressTime = GetNowCount();
	}
	else if (MouseLeftRelease && pressTime > 0)
	{
		int now = GetNowCount();
		Ball.SPEED.X = (MouseReleasePos.X - MousePressPos.X) * 1000.0f / (now - pressTime);
		Ball.SPEED.Y = (MouseReleasePos.Y - MousePressPos.Y) * 1000.0f / (now - pressTime);

		pressTime = 0;
	}
}

//================================================
// 描画処理
//================================================
void Draw(void)
{
	ClearDrawScreen();
	DrawCircleAA(Ball.POS.X, Ball.POS.Y, Ball.RADIUS, 32,
		GetColor(255, 255, 0), TRUE);
	FpsDraw(&fpsTimer);
}

//================================================
// 主処理
//================================================
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)
{
#ifndef _DEBUG
	SetOutApplicationLogValidFlag(FALSE);
#endif // !_DEBUG

	ChangeWindowMode(TRUE);

	if (DxLib_Init() == -1) return -1;

	Init();

	while (ProcessMessage() == 0)
	{
		Input();
		Update();
		Draw();

		ScreenFlip();
	}

	End();

	DxLib_End();
	return 0;
}