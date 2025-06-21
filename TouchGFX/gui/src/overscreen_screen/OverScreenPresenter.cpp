#include <gui/overscreen_screen/OverScreenView.hpp>
#include <gui/overscreen_screen/OverScreenPresenter.hpp>
#include <gui/model/Model.hpp>

OverScreenPresenter::OverScreenPresenter(OverScreenView& v)
    : view(v)
{

}

uint32_t OverScreenPresenter::getWinner() const
{
    return model->getWinner();
}

void OverScreenPresenter::activate()
{

}

void OverScreenPresenter::deactivate()
{

}
