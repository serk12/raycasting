#ifndef PLOTSRGBA_H
#define PLOTSRGBA_H

#include <QVBoxLayout>
#include <QObject>
#include "plotx.h"

class PlotsRGBA : public QObject {
    Q_OBJECT
public:
    PlotsRGBA(QVBoxLayout& layer);
    glm::vec4* getData();
    int getSize() const;
private:
    PlotX *r, *g, *b, *a;
    glm::vec4* data;
protected slots:
    void updates();
signals:
    void updateFather();
};

#endif // PLOTSRGBA_H
