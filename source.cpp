//************************************************
// DX���C�u���� �}�E�X�ŋʗV��(��{)
// �쐬���F2024/04/04
// �쐬�ҁFfujioka8700
// Copyright (c) fujioka8700 All rights reserved.
//************************************************

#include <Dxlib.h>

typedef struct {
	float X, Y;
} CRD; // coordinate

typedef struct {
	CRD POS;
	CRD SPEED;
	float RADIUS;
} BALL;

//================================================
// �O���[�o���ϐ��錾
//================================================
BALL Ball;
LONGLONG fpsTimer, deltaTimer;

//================================================
// FPS�̌v���ƕ`��
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
// �f���^�^�C���̌v��
//================================================
float getDeltaTime(LONGLONG* p)
{
	LONGLONG now = GetNowHiPerformanceCount();
	float    deltaTime = (float)(now - *p) / 1000000.0f;

	*p = now;
	return deltaTime;
}

//================================================
// ��������
//================================================
void Init(void)
{
	fpsTimer = deltaTimer = GetNowHiPerformanceCount();

	Ball.POS.X = 320, Ball.POS.Y = 240;
	Ball.SPEED.X = 200, Ball.SPEED.Y = 200;
	Ball.RADIUS = 20;

	SetDrawScreen(DX_SCREEN_BACK);
}

//================================================
// �I������
//================================================
void End(void)
{

}

//================================================
// ���͏���
//================================================
void Input(void)
{

}

//================================================
// �X�V����
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
}

//================================================
// �`�揈��
//================================================
void Draw(void)
{
	ClearDrawScreen();
	DrawCircleAA(Ball.POS.X, Ball.POS.Y, Ball.RADIUS, 32,
		GetColor(255, 255, 0), TRUE);
	FpsDraw(&fpsTimer);
}

//================================================
// �又��
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