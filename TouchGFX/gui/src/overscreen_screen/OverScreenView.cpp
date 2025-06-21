#include <gui/overscreen_screen/OverScreenView.hpp>

void OverScreenView::updateWinner()
{
	uint32_t winner = presenter->getWinner();
	Unicode::snprintf(textArea1Buffer, TEXTAREA1_SIZE, "%u", winner);
	textArea1.invalidate();
}

OverScreenView::OverScreenView()
{
	updateWinner();
}

void OverScreenView::setupScreen()
{
    OverScreenViewBase::setupScreen();
    textArea1.setWildcard(textArea1Buffer);
    updateWinner();
}

void OverScreenView::tearDownScreen()
{
    OverScreenViewBase::tearDownScreen();
}
