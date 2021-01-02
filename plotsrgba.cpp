#include "plotsrgba.h"

PlotsRGBA::PlotsRGBA(QVBoxLayout& layer)
{
    r = new PlotX(RGBaType::Red);
    layer.addWidget(r);

    g = new PlotX(RGBaType::Green);
    layer.addWidget(g);

    b = new PlotX(RGBaType::Blue);
    layer.addWidget(b);

    a = new PlotX(RGBaType::Alpha);
    layer.addWidget(a);
}

glm::vec4* PlotsRGBA::getData()
{
    data = new glm::vec4[r->getSize()];
    for (int i = 0; i < r->getSize(); ++i) {
        data[i] = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    }
    r->getData(data);
    g->getData(data);
    b->getData(data);
    a->getData(data);
    return data;
}

int PlotsRGBA::getSize() const
{
    return r->getSize();
}
