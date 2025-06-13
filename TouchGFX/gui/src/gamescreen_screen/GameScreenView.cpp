#include <gui/gamescreen_screen/GameScreenView.hpp>
#include <cmsis_os2.h>
#include <algorithm>

extern osMessageQueueId_t joystickQueueHandle;
typedef struct {
    uint16_t x1;
    uint16_t y1;
    uint16_t x2;
    uint16_t y2;
} JoystickData;


GameScreenView::GameScreenView()
{
	tickCount = 0;
	ballVx = 2.0f, ballVy = 2.0f;
	score1=0;
	score2=0;
}

void GameScreenView::setupScreen()
{
    GameScreenViewBase::setupScreen();

}

void GameScreenView::tearDownScreen()
{
    GameScreenViewBase::tearDownScreen();
}




void GameScreenView::handleTick(){
	GameScreenViewBase::handleTick();
	JoystickData data;

	ballX = circle1.getX();
	ballY = circle1.getY();

	ballX += ballVx;
	ballY += ballVy;

	int currentX1 = line1_1.getX();
	int currentY1 = line1_1.getY();
	int currentX2 = line1_2.getX();
	int currentY2 = line1_2.getY();

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
	}
	else if (ballY + 20 >= 320) {
	    ballY = 320 - 20;
	    ballVy = -ballVy;
	    score1++;
	    updateScoreDisplays1();
	}

	circle1.moveTo((int)ballX, (int)ballY);


	if (osMessageQueueGet(joystickQueueHandle, &data, NULL, 0) == osOK)
	    {
			const int16_t xCenter = 2048;
	        const int16_t deadZone = 200;  // Vùng chết tránh jitter
	        const float maxSpeed = 3.0f;   // Tốc độ di chuyển tối đa (pixel/tick)

	        // Lấy vị trí hiện tại của line1_1


	        float deltaX1 = 0.0f, deltaX2 = 0.0f;

	        // Tính deltaX theo joystick
	        float normX1 = (float)(data.x1 - xCenter) / (float)(xCenter); // Chuẩn hóa về [-1, 1]
	        if (normX1 > deadZone / (float)xCenter) {
	            deltaX1 = (normX1 - deadZone / (float)xCenter) * maxSpeed * 1.5;
	        }
	        else if (normX1 < -deadZone / (float)xCenter) {
	            deltaX1 = (normX1 + deadZone / (float)xCenter) * maxSpeed * 1.5;
	        }

	        float normX2 = (float)(data.x2 - xCenter) / (float)(xCenter); // Chuẩn hóa về [-1, 1]
	        if (normX2 > deadZone / (float)xCenter) {
	        	deltaX2 = (normX2 - deadZone / (float)xCenter) * maxSpeed * 1.5;
	        }
	        else if (normX2 < -deadZone / (float)xCenter) {
	        	deltaX2 = (normX2 + deadZone / (float)xCenter) * maxSpeed * 1.5;
	        }

	        // Cập nhật vị trí mới
	        int newX1 = currentX1 + (int)deltaX1;
	        int newY1 = currentY1;
	        int newX2 = currentX2 + (int)deltaX2;
	        int newY2 = currentY2;

	        // Giới hạn vùng di chuyển
	        const int screenWidth = 240;
	        const int lineWidth = 70;

	        if (newX1 < 0) newX1 = 0;
	        if (newX1 + lineWidth > screenWidth) newX1 = screenWidth - lineWidth;
	        if (newX2 < 0) newX2 = 0;
	        if (newX2 + lineWidth > screenWidth) newX2 = screenWidth - lineWidth;

	        // Tính vùng invalidate bao gồm cả vị trí cũ và mới, cộng thêm padding để đảm bảo xóa tàn ảnh
	        int padding = 2; // Thêm padding để đảm bảo vùng làm mới đủ lớn
	        int invalidateX1 = std::min(currentX1, newX1) - padding;
	        int invalidateX2 = std::min(currentX2, newX2) - padding;

	        int invalidateWidth1 = std::max(currentX1 + lineWidth, newX1 + lineWidth) - invalidateX1 + 2 * padding;
	        int invalidateWidth2 = std::max(currentX2 + lineWidth, newX2 + lineWidth) - invalidateX2 + 2 * padding;

	        int invalidateY1 = currentY1 - padding;
	        int invalidateY2 = currentY2 - padding;

	        int invalidateHeight = 10 + 2 * padding; // Chiều cao của line1_1 là 10

	        // Đảm bảo vùng invalidate nằm trong màn hình
	        if (invalidateX1 < 0) invalidateX1 = 0;
	        if (invalidateX1 + invalidateWidth1 > screenWidth) invalidateWidth1 = screenWidth - invalidateX1;
	        if (invalidateX2 < 0) invalidateX2 = 0;
	        if (invalidateX2 + invalidateWidth2 > screenWidth) invalidateWidth2 = screenWidth - invalidateX2;
	        // Cập nhật vị trí line1_1

	        line1_1.setPosition(newX1, newY1, lineWidth, 10);
	        line1_2.setPosition(newX2, newY2, lineWidth, 10);
	        // Làm mới vùng lớn hơn để xóa tàn ảnh
	        Rect invalidRect1(invalidateX1, invalidateY1, invalidateWidth1, invalidateHeight);
	        Rect invalidRect2(invalidateX2, invalidateY2, invalidateWidth2, invalidateHeight);

	        invalidateRect(invalidRect1);
	        invalidateRect(invalidRect2);

	        // Nếu vẫn còn tàn ảnh, thử làm mới toàn bộ màn hình (bỏ comment dòng dưới nếu cần)
	        // invalidate();
	    }
}

void GameScreenView::updateScoreDisplays1() {
    Unicode::snprintf(textArea2Buffer,TEXTAREA2_SIZE , "%u", score1);
    textArea2.invalidate();
}

void GameScreenView::updateScoreDisplays2() {
    Unicode::snprintf(textArea2_1Buffer, TEXTAREA2_1_SIZE, "%u", score2);
	textArea2_1.invalidate();
}
