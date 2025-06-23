#ifndef OVERSCREENVIEW_HPP
#define OVERSCREENVIEW_HPP

#include <gui_generated/overscreen_screen/OverScreenViewBase.hpp>
#include <gui/overscreen_screen/OverScreenPresenter.hpp>

class OverScreenView : public OverScreenViewBase
{
public:
    OverScreenView();
    virtual ~OverScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();


protected:
    void updateWinner();
};

#endif // OVERSCREENVIEW_HPP
