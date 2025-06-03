#include <gui/gamescreen_screen/GameScreenView.hpp>
#include <cmsis_os2.h>
#include <algorithm>

extern osMessageQueueId_t joystickQueueHandle;
typedef struct {
    uint16_t x;
    uint16_t y;
} JoystickData;
GameScreenView::GameScreenView()
{
	tickCount = 0;
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

	if (osMessageQueueGet(joystickQueueHandle, &data, NULL, 0) == osOK)
	    {
			const int16_t xCenter = 2048;
	        const int16_t deadZone = 200;  // Vùng chết tránh jitter
	        const float maxSpeed = 3.0f;   // Tốc độ di chuyển tối đa (pixel/tick)

	        // Lấy vị trí hiện tại của line1_1
	        int currentX = line1_1.getX();
	        int currentY = line1_1.getY();  // Giữ nguyên Y

	        float deltaX = 0.0f;

	        // Tính deltaX theo joystick
	        if (data.x > xCenter + deadZone) {
	            float val = (float)(data.x - (xCenter + deadZone)) / (4095 - (xCenter + deadZone));
	            deltaX = val * maxSpeed;
	        }
	        else if (data.x < xCenter - deadZone) {
	            float val = (float)((xCenter - deadZone) - data.x) / (xCenter - deadZone);
	            deltaX = -val * maxSpeed;
	        }

	        // Cập nhật vị trí mới
	        int newX = currentX + (int)deltaX;
	        int newY = currentY; // Giữ nguyên Y

	        // Giới hạn vùng di chuyển
	        const int screenWidth = 240;
	        const int lineWidth = 70;

	        if (newX < 0) newX = 0;
	        if (newX + lineWidth > screenWidth) newX = screenWidth - lineWidth;

	        // Tính vùng invalidate bao gồm cả vị trí cũ và mới, cộng thêm padding để đảm bảo xóa tàn ảnh
	        int padding = 2; // Thêm padding để đảm bảo vùng làm mới đủ lớn
	        int invalidateX = std::min(currentX, newX) - padding;
	        int invalidateWidth = std::max(currentX + lineWidth, newX + lineWidth) - invalidateX + 2 * padding;
	        int invalidateY = currentY - padding;
	        int invalidateHeight = 10 + 2 * padding; // Chiều cao của line1_1 là 10

	        // Đảm bảo vùng invalidate nằm trong màn hình
	        if (invalidateX < 0) invalidateX = 0;
	        if (invalidateX + invalidateWidth > screenWidth) invalidateWidth = screenWidth - invalidateX;

	        // Cập nhật vị trí line1_1
	        line1_1.setPosition(newX, newY, lineWidth, 10);

	        // Làm mới vùng lớn hơn để xóa tàn ảnh
	        Rect invalidRect(invalidateX, invalidateY, invalidateWidth, invalidateHeight);
	        invalidateRect(invalidRect);

	        // Nếu vẫn còn tàn ảnh, thử làm mới toàn bộ màn hình (bỏ comment dòng dưới nếu cần)
	        // invalidate();
	    }



}
