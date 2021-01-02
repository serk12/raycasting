#include "plotrgb.h"

PlotRGB::PlotRGB() : PlotRGB(Alpha) {}


PlotRGB::PlotRGB(const RGBaType type)
{
    this->setMaximumSize(200, 200);
    this->type = type;
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
        this->setTitle("Alpha");
        break;
    }
    this->setAxisMaxMajor(Axis::yLeft, 4);
    this->setAxisScale(Axis::yLeft, 0.0, PlotRGB::resolution, 25);
    this->setAxisAutoScale(Axis::yLeft, false);
    this->setAxisMaxMajor(Axis::xBottom, 4);
    this->setAxisScale(Axis::xBottom, 0.0, PlotRGB::resolution, 25);
    this->setAxisAutoScale(Axis::xBottom, false);
    curve = new QwtPlotCurve;
    y = QVector<double>(PlotRGB::resolution / PlotRGB::step + 1);
    x = QVector<double>(PlotRGB::resolution / PlotRGB::step + 1, 50);
    for (int i = 0; i <= PlotRGB::resolution / PlotRGB::step; ++i) {
        y[i] = i*PlotRGB::step;
    }
    curve->setSamples(y, x);
    curve->attach(this);
    this->replot();
}

void PlotRGB::getData(glm::vec4* colors)
{
    for (int i = 0; i < PlotRGB::resolution; ++i) {
        switch (type) {
        case RGBaType::Red:
            colors[i][0] = x[i];
            break;
        case RGBaType::Green:
            colors[i][1] = x[i];
            break;
        case RGBaType::Blue:
            colors[i][2] = x[i];
            break;
        case RGBaType::Alpha:
            colors[i][3] = x[i];
            break;
        }
    }
}

void PlotRGB::mousePressEvent(QMouseEvent* event) {

    QWidget::mousePressEvent(event);
    auto canvasPos = this->canvas()->mapFrom(this, event->pos());
    double i_x = curve->plot()->invTransform(Axis::xBottom, canvasPos.x());
    double i_y = curve->plot()->invTransform(Axis::yLeft, canvasPos.y());
    double _x = (i_y <= 0? 0 : (i_y >= 100? 100 : i_y));
    double _y = (i_x <= 0? 0 : (i_x >= 100? 100 : i_x));
    _y = int(_y / PlotRGB::step);
    _x = int(_x);

    x[_y] = _x;
    curve->setSamples(y, x);
    this->replot();
}
