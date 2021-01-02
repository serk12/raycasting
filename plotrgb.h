#ifndef PLOTRGB_H
#define PLOTRGB_H

#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_curve.h>
#include <glm/vec4.hpp>
#include <QMouseEvent>
#include <iostream>

enum RGBaType {Red, Green, Blue, Alpha};

class PlotRGB : public QwtPlot
{
public:
    PlotRGB();
    PlotRGB(const RGBaType type);
    void getData(glm::vec4* colors);

protected:
    void mousePressEvent(QMouseEvent* event) override;
private:
    RGBaType type;
    QwtPlotCurve* curve;
    inline static const int resolution = 100;
    inline static const int step = 10;

    QVector<double> y, x;
};

#endif // PLOTRGB_H
