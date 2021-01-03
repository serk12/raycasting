#include "plotx.h"

PlotX::PlotX() : PlotX(Alpha) {}


PlotX::PlotX(const RGBaType type) : QwtPlot()
{
    this->setMaximumSize(200, 200);
    this->type = type;
    int defValue = 50;
    int setZero = 0;
    switch (type) {
    case RGBaType::Red:
        this->setTitle("Red");
        break;
    case RGBaType::Green:
        this->setTitle("Green");
        break;
    case RGBaType::Blue:
        this->setTitle("Blue");
        break;
    case RGBaType::Alpha:
        defValue = 100;
        setZero = 15;
        this->setTitle("Alpha");
        break;
    }
    this->setAxisMaxMajor(Axis::yLeft, 4);
    this->setAxisScale(Axis::yLeft, 0.0, PlotX::resolution, 25);
    this->setAxisAutoScale(Axis::yLeft, false);
    this->setAxisMaxMajor(Axis::xBottom, 4);
    this->setAxisScale(Axis::xBottom, 0.0, PlotX::resolution, 25);
    this->setAxisAutoScale(Axis::xBottom, false);

    y = QVector<double>(PlotX::resolution / PlotX::step + 1);
    x = QVector<double>(PlotX::resolution / PlotX::step + 1, defValue);
    for (int i = 0; i <= PlotX::resolution / PlotX::step; ++i) {
        y[i] = i*PlotX::step;
    }
    for (int i = 0; i < setZero*getSize() / 100; ++i) {
        x[i] = 0;
    }
    curve = new QwtPlotCurve;
    curve->setSamples(y, x);
    curve->attach(this);

    this->replot();
}

void PlotX::setReference(const QVector<double>& ref)
{
    reference = new QwtPlotCurve;
    QVector<double> ref_y = QVector<double>(ref.size());
    double max = 0, max2 = 0;
    for (int i = 0; i < ref.size(); ++i) {
        ref_y[i] = (i*100)/(ref.size()-1);
        if (ref[i] > max) max = ref[i];
        if (ref[i] > max2 && ref[i] < max) max2 = ref[i];
    }
    QVector<double> ref_x(ref.size());
    if (max2*20 < max) max = max2;
    for (int i = 0; i < ref.size(); ++i) {
        double aux = (ref[i]*100)/max;
        aux = aux > 100.0f? 100.0f : (aux < 0.0f? 0.0f : aux);
        ref_x[i] = aux;
    }
    reference->setSamples(ref_y, ref_x);
    reference->attach(this);
    QBrush b;
    b.setStyle(Qt::SolidPattern);
    switch (type) {
    case RGBaType::Red:
        b.setColor(QColor(255,0,0,64));
        break;
    case RGBaType::Green:
        b.setColor(QColor(0,255,0,64));
        break;
    case RGBaType::Blue:
        b.setColor(QColor(0,0,255,64));
        break;
    case RGBaType::Alpha:
        b.setColor(QColor(64,64,64,64));
        break;
    }
    reference->setBrush(b);

    this->replot();
}
void PlotX::getData(glm::vec4* colors)
{
    for (int i = 0; i < getSize(); ++i) {
        switch (type) {
        case RGBaType::Red:
            colors[i].r = x[i];
            break;
        case RGBaType::Green:
            colors[i].g = x[i];
            break;
        case RGBaType::Blue:
            colors[i].b = x[i];
            break;
        case RGBaType::Alpha:
            colors[i].a = x[i];
            break;
        }
    }
}

void PlotX::mousePressEvent(QMouseEvent* event) {

    QWidget::mousePressEvent(event);
    auto canvasPos = this->canvas()->mapFrom(this, event->pos());
    double i_x = curve->plot()->invTransform(Axis::xBottom, canvasPos.x());
    double i_y = curve->plot()->invTransform(Axis::yLeft, canvasPos.y());
    double _x = (i_y <= 0? 0 : (i_y >= 100? 100 : i_y));
    double _y = (i_x <= 0? 0 : (i_x >= 100? 100 : i_x));
    _y = int(_y / PlotX::step);
    _x = int(_x);

    x[_y] = _x;
    curve->setSamples(y, x);
    this->replot();
    emit updateFather();
}

int PlotX::getSize() const
{
    return y.size();
}
