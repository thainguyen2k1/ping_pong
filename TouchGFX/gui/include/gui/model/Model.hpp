#ifndef MODEL_HPP
#define MODEL_HPP

#include <cstdint>

class ModelListener;

class Model
{
public:
    Model();

    void setWinner(uint32_t w) { winner = w; }
    uint32_t getWinner() const { return winner; }

    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    void tick();
protected:
    ModelListener* modelListener;
    uint32_t winner = 0;
};

#endif // MODEL_HPP
