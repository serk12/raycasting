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
        setZero = 30;
        this->setTitle("Alpha");
        break;
    }
    this->setAxisMaxMajor(Axis::yLeft, 4);
    this->setAxisScale(Axis::yLeft, 0.0, PlotX::resolution, 25);
    this->setAxisAutoScale(Axis::yLeft, false);
    this->setAxisMaxMajor(Axis::xBottom, 4);
    this->setAxisScale(Axis::xBottom, 0.0, PlotX::resolution, 25);
    this->setAxisAutoScale(Axis::xBottom, false);
    curve = new QwtPlotCurve;
    y = QVector<double>(PlotX::resolution / PlotX::step + 1);
    x = QVector<double>(PlotX::resolution / PlotX::step + 1, defValue);
    for (int i = 0; i <= PlotX::resolution / PlotX::step; ++i) {
        y[i] = i*PlotX::step;
    }
    for (int i = 0; i < setZero*getSize() / 100; ++i) {
        x[i] = 0;
    }
    curve->setSamples(y, x);
    curve->attach(this);
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
