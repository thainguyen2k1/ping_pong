#include <gui/gamescreen_screen/GameScreenView.hpp>
#include <cmsis_os2.h>
#include <algorithm>
#include <cstdlib> // Để dùng rand()
#include <cmath>   // Để dùng sinf, cosf

extern osMessageQueueId_t joystickQueueHandle;
extern int winner;
typedef struct {
    uint16_t x1;
    uint16_t y1;
    uint16_t x2;
    uint16_t y2;
} JoystickData;

GameScreenView::GameScreenView()
{
    score1 = 0;
    score2 = 0;
    isBallResetting = false;
    resetStartTime = 0;
    winner = 0;
}

void GameScreenView::setupScreen()
{
    GameScreenViewBase::setupScreen();
    // Khởi tạo seed cho rand()

}

void GameScreenView::tearDownScreen()
{
    GameScreenViewBase::tearDownScreen();
}


void GameScreenView::resetBall()
{
	isBallResetting = true;
	resetStartTime = osKernelGetTickCount();
	ballX = 110; // Giữa màn hình (240/2)
	ballY = 150; // Giữa màn hình (320/2)
	    //3.14159265f / 4.0f((float)rand() / RAND_MAX) * 2.0f * 3.14159265f
	float angle; // Góc ngẫu nhiên 0-2π
	if(caseReset == 1){
		angle = angles[caseReset];
		caseReset = 2;
	} else if (caseReset == 2) {
		angle = angles[caseReset];
		caseReset = 3;
	} else if (caseReset == 2) {
		angle = angles[caseReset];
		caseReset = 0;
	} else {
		angle = angles[caseReset];
		caseReset = 1;
	}
	float speed = 2.0f; // Tốc độ cố định
    ballVx = speed * cosf(angle);
    ballVy = speed * sinf(angle);
    circle1.moveTo((int)ballX, (int)ballY);
    circle1.invalidate();
}

void GameScreenView::handleTick()
{
    GameScreenViewBase::handleTick();
    JoystickData data;

    if (firstTick)
    {
    	resetBall(); // không chờ 3 giây
        firstTick = false;
    }

    // Kiểm tra trạng thái reset
    if (isBallResetting)
    {
        if (osKernelGetTickCount() - resetStartTime >= 1000)
        {
            isBallResetting = false;
            circle1.moveTo((int)ballX, (int)ballY);
            circle1.invalidate();
        }
        return;
    }

    // Cập nhật vị trí bóng
    ballX = circle1.getX();
    ballY = circle1.getY();

    ballX += ballVx;
    ballY += ballVy;

    int currentX1 = line1_1.getX();
    int currentY1 = line1_1.getY();
    int currentX2 = line1_2.getX();
    int currentY2 = line1_2.getY();

    // Xử lý va chạm với thanh chắn
    if (ballX + 20 >= currentX1 &&
        ballX <= currentX1 + 70 &&
        ballY >= currentY1 &&
        ballY <= currentY1 + 10)
    {
        ballVy = -ballVy;
        ballY = currentY1 + 10;
    }

    if (ballX + 20 >= currentX2 &&
        ballX <= currentX2 + 70 &&
        ballY + 20 >= currentY2 &&
        ballY + 20 <= currentY2 + 10)
    {
        ballVy = -ballVy;
        ballY = currentY2 - 20;
    }

    // Xử lý va chạm với biên
    if (ballX <= 0) {
        ballX = 0;
        ballVx = -ballVx;
    }
    else if (ballX + 20 >= 240) {
        ballX = 240 - 20;
        ballVx = -ballVx;
    }

    if (ballY <= 0) {
        ballY = 0;
        ballVy = -ballVy;
        score2++;
        updateScoreDisplays2();
        if (score2 >= 10) {
        	presenter->onGameOver(2);
        	static_cast<FrontendApplication*>(Application::getInstance())->gotoOverScreenScreenCoverTransitionEast();
            return;
        }
		resetBall();
    }
    else if (ballY + 20 >= 320) {
        ballY = 320 - 20;
        ballVy = -ballVy;
        score1++;
        updateScoreDisplays1();
        if (score1 >= 10) {
        	presenter->onGameOver(1);
        	static_cast<FrontendApplication*>(Application::getInstance())->gotoOverScreenScreenCoverTransitionEast();
            return;
        }
		resetBall();
    }

    circle1.moveTo((int)ballX, (int)ballY);

    // Xử lý joystick
    if (osMessageQueueGet(joystickQueueHandle, &data, NULL, 0) == osOK)
    {
        const int16_t xCenter = 2048;
        const int16_t deadZone = 200;
        const float maxSpeed = 3.0f;

        float deltaX1 = 0.0f, deltaX2 = 0.0f;

        float normX1 = (float)(data.x1 - xCenter) / (float)(xCenter);
        if (normX1 > deadZone / (float)xCenter) {
            deltaX1 = (normX1 - deadZone / (float)xCenter) * maxSpeed;
        }
        else if (normX1 < -deadZone / (float)xCenter) {
            deltaX1 = (normX1 + deadZone / (float)xCenter) * maxSpeed;
        }

        float normX2 = (float)(data.x2 - xCenter) / (float)(xCenter);
        if (normX2 > deadZone / (float)xCenter) {
            deltaX2 = (normX2 - deadZone / (float)xCenter) * maxSpeed;
        }
        else if (normX2 < -deadZone / (float)xCenter) {
            deltaX2 = (normX2 + deadZone / (float)xCenter) * maxSpeed;
        }

        int newX1 = currentX1 + (int)deltaX1;
        int newY1 = currentY1;
        int newX2 = currentX2 + (int)deltaX2;
        int newY2 = currentY2;

        const int screenWidth = 240;
        const int lineWidth = 70;

        if (newX1 < 0) newX1 = 0;
        if (newX1 + lineWidth > screenWidth) newX1 = screenWidth - lineWidth;
        if (newX2 < 0) newX2 = 0;
        if (newX2 + lineWidth > screenWidth) newX2 = screenWidth - lineWidth;

        int padding = 2;
        int invalidateX1 = std::min(currentX1, newX1) - padding;
        int invalidateX2 = std::min(currentX2, newX2) - padding;

        int invalidateWidth1 = std::max(currentX1 + lineWidth, newX1 + lineWidth) - invalidateX1 + 2 * padding;
        int invalidateWidth2 = std::max(currentX2 + lineWidth, newX2 + lineWidth) - invalidateX2 + 2 * padding;

        int invalidateY1 = currentY1 - padding;
        int invalidateY2 = currentY2 - padding;

        int invalidateHeight = 10 + 2 * padding;

        if (invalidateX1 < 0) invalidateX1 = 0;
        if (invalidateX1 + invalidateWidth1 > screenWidth) invalidateWidth1 = screenWidth - invalidateX1;
        if (invalidateX2 < 0) invalidateX2 = 0;
        if (invalidateX2 + invalidateWidth2 > screenWidth) invalidateWidth2 = screenWidth - invalidateX2;

        line1_1.setPosition(newX1, newY1, lineWidth, 10);
        line1_2.setPosition(newX2, newY2, lineWidth, 10);

        Rect invalidRect1(invalidateX1, invalidateY1, invalidateWidth1, invalidateHeight);
        Rect invalidRect2(invalidateX2, invalidateY2, invalidateWidth2, invalidateHeight);

        invalidateRect(invalidRect1);
        invalidateRect(invalidRect2);
    }
}

void GameScreenView::updateScoreDisplays1()
{
    Unicode::snprintf(textArea2Buffer, TEXTAREA2_SIZE, "%u", score1);
    textArea2.invalidate();
}

void GameScreenView::updateScoreDisplays2()
{
    Unicode::snprintf(textArea2_1Buffer, TEXTAREA2_1_SIZE, "%u", score2);
    textArea2_1.invalidate();
}
