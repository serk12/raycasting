#ifndef PLOTSRGBA_H
#define PLOTSRGBA_H

#include <QVBoxLayout>
#include "plotx.h"

class PlotsRGBA
{
public:
    PlotsRGBA(QVBoxLayout& layer);
    glm::vec4* getData();
    int getSize() const;
private:
    PlotX *r, *g, *b, *a;
    glm::vec4* data;
};

#endif // PLOTSRGBA_H
