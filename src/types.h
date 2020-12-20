#ifndef TYPES_H
#define TYPES_H

#include <complex>
#include <QList>
#include <QVector>
#include <QPointF>

#include <EndpointTargetDetection.h>

using DataPoints_t = QList<QPointF>;
using Targets_t = QVector<Target_Info_t>;

using DoubleVec_t = std::vector<double>;
using Complex_t = std::complex<double>;
using ComplexVec_t = std::vector<Complex_t>;

enum class EndpointType_t
{
    Calibration,
    AdcXmc,
    Base,
    Doppler,
    Fmcw,
    Industrial,
    P2G,
    TargetDetection
};

#endif // TYPES_H