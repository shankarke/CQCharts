#include <CQChartsPlotObj.h>

CQChartsPlotObj::
CQChartsPlotObj(CQChartsPlot *plot, const CQChartsGeom::BBox &rect) :
 plot_(plot), rect_(rect)
{
}

const QString &
CQChartsPlotObj::
id() const
{
  if (! id_)
    const_cast<CQChartsPlotObj*>(this)->id_ = calcId();

  return *id_;
}

const QString &
CQChartsPlotObj::
tipId() const
{
  if (! tipId_)
    const_cast<CQChartsPlotObj*>(this)->tipId_ = calcTipId();

  return *tipId_;
}

//------

CQChartsGroupObj::
CQChartsGroupObj(CQChartsPlot *plot) :
 CQChartsPlotObj(plot, CQChartsGeom::BBox(0, 0, 1, 1))
{
}
