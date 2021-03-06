#ifndef CQChartsRotatedTextBoxObj_H
#define CQChartsRotatedTextBoxObj_H

#include <CQChartsTextBoxObj.h>

/*!
 * \brief Charts Rotated Text Box Object
 * \ingroup Charts
 */
class CQChartsRotatedTextBoxObj : public CQChartsTextBoxObj {
 public:
  CQChartsRotatedTextBoxObj(CQChartsPlot *plot);

  void draw(CQChartsPaintDevice *device, const CQChartsGeom::Point &c, const QString &text,
            double angle=0.0, Qt::Alignment align=Qt::AlignHCenter | Qt::AlignVCenter) const;

  CQChartsGeom::BBox bbox(const CQChartsGeom::Point &center, const QString &text, double angle=0.0,
                          Qt::Alignment align=Qt::AlignHCenter | Qt::AlignVCenter) const;

  void drawConnectedRadialText(CQChartsPaintDevice *device, const CQChartsGeom::Point &center,
                               double ro, double lr, double ta, const QString &text,
                               const QPen &lpen, bool isRotated);

  void calcConnectedRadialTextBBox(const CQChartsGeom::Point &center, double ro, double lr,
                                   double ta, const QString &text, bool isRotated,
                                   CQChartsGeom::BBox &tbbox);

 private:
  void drawCalcConnectedRadialText(CQChartsPaintDevice *device, const CQChartsGeom::Point &center,
                                   double ro, double lr, double ta, const QString &text,
                                   const QPen &lpen, bool isRotated, CQChartsGeom::BBox &tbbox);

 private:
  mutable CQChartsGeom::BBox bbox_;
};

#endif
