#ifndef OVERSCREENPRESENTER_HPP
#define OVERSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class OverScreenView;

class OverScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    OverScreenPresenter(OverScreenView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~OverScreenPresenter() {}
    uint32_t getWinner() const;


private:
    OverScreenPresenter();

    OverScreenView& view;
};

#endif // OVERSCREENPRESENTER_HPP
