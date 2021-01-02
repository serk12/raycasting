#include "plotsrgba.h"

PlotsRGBA::PlotsRGBA(QVBoxLayout& layer) : QObject()
{
    r = new PlotX(RGBaType::Red);
    layer.addWidget(r);
    connect(r, SIGNAL(updateFather()), this, SLOT(updates()));

    g = new PlotX(RGBaType::Green);
    layer.addWidget(g);
    connect(g, SIGNAL(updateFather()), this, SLOT(updates()));

    b = new PlotX(RGBaType::Blue);
    layer.addWidget(b);
    connect(b, SIGNAL(updateFather()), this, SLOT(updates()));

    a = new PlotX(RGBaType::Alpha);
    layer.addWidget(a);
    connect(a, SIGNAL(updateFather()), this, SLOT(updates()));
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

void PlotsRGBA::updates()
{
    emit updateFather();
}
