//
// Created by Matthew McCall on 1/29/22.
//

#include "york/Layer.hpp"

namespace york {

Layer::Layer(Registry& registry)
    : EventHandler(registry)
{
}

bool Layer::getExit() const
{
    return m_exit;
}

void Layer::requestExit()
{
    m_exit = true;
}

}