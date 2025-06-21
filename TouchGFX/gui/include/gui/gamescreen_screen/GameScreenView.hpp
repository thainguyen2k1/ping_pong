#ifndef GAMESCREENVIEW_HPP
#define GAMESCREENVIEW_HPP

#include <gui_generated/gamescreen_screen/GameScreenViewBase.hpp>
#include <gui/gamescreen_screen/GameScreenPresenter.hpp>

class GameScreenView : public GameScreenViewBase
{
public:
    GameScreenView();
    virtual ~GameScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    void handleTick();
    void updateScoreDisplays1();
    void updateScoreDisplays2();
    void resetBall();
    void gotoOverScreen();

protected:
    float ballX, ballY;
    float ballVx, ballVy;
    int score1;
    int score2;
    bool isBallResetting; // Trạng thái reset bóng
    uint32_t resetStartTime; // Thời gian bắt đầu reset
    bool firstTick = true;
    float angles[4] = {
        3.14159265f / 4,    // 45°
        -3.14159265f / 4,   // -45°
        3.14159265f * 3 / 4,   // 135°
        -3.14159265f * 3 / 4   // -135°
    };
    int caseReset = 1;
    uint32_t winner = 0;
};

#endif // GAMESCREENVIEW_HPP
