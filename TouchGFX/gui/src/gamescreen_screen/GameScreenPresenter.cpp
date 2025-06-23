#include <gui/gamescreen_screen/GameScreenView.hpp>
#include <gui/gamescreen_screen/GameScreenPresenter.hpp>
#include <gui/common/FrontendApplication.hpp>

GameScreenPresenter::GameScreenPresenter(GameScreenView& v)
    : view(v)
{

}

void GameScreenPresenter::onGameOver(uint32_t finalWinner)
{
    model->setWinner(finalWinner); // Gửi dữ liệu sang model
    static_cast<FrontendApplication*>(Application::getInstance())
        ->gotoOverScreenScreenCoverTransitionEast(); // Chuyển màn
}

void GameScreenPresenter::activate()
{

}

void GameScreenPresenter::deactivate()
{

}
