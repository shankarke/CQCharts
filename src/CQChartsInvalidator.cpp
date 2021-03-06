#include <CQChartsInvalidator.h>
#include <CQChartsPlot.h>
#include <CQChartsView.h>
#include <CQChartsAxis.h>
#include <CQChartsKey.h>
#include <CQChartsBoxObj.h>
#include <CQChartsArrow.h>
#include <CQCharts.h>

namespace {

void
getObjPlotViewChart(QObject *obj, CQChartsPlot* &plot, CQChartsView* &view, CQCharts* &charts) {
  view   = nullptr;
  charts = nullptr;

  plot = qobject_cast<CQChartsPlot *>(obj);
  if (plot) return;

  view = qobject_cast<CQChartsView *>(obj);
  if (view) return;

  auto *axis = qobject_cast<CQChartsAxis *>(obj);

  if (axis) {
    plot = axis->plot();
    if (plot) return;

    charts = axis->charts();

    return;
  }

  auto *key = qobject_cast<CQChartsKey *>(obj);

  if (key) {
    plot = key->plot();
    if (plot) return;

    view = key->view();
    if (view) return;

    charts = key->charts();

    return;
  }

  auto *boxObj = qobject_cast<CQChartsBoxObj *>(obj);

  if (boxObj) {
    plot = boxObj->plot();
    if (plot) return;

    view = boxObj->view();
    if (view) return;

    charts = boxObj->charts();

    return;
  }

  auto *arrow = qobject_cast<CQChartsArrow *>(obj);

  if (arrow) {
    plot = arrow->plot();
    if (plot) return;

    view = arrow->view();
    if (view) return;

    charts = arrow->charts();

    return;
  }

  assert(false);
}

}

//---

void
CQChartsInvalidator::
invalidate(bool reload)
{
  auto *axis = qobject_cast<CQChartsAxis *>(obj_);

  if (axis) {
    if (axis->isUpdatesEnabled())
      axis->redraw();

    return;
  }

  //---

  CQChartsPlot *plot   = nullptr;
  CQChartsView *view   = nullptr;
  CQCharts     *charts = nullptr;

  getObjPlotViewChart(obj_, plot, view, charts);

  if      (plot) {
    if (reload)
      plot->updateRangeAndObjs();
    else
      plot->drawObjs();

    auto *key = qobject_cast<CQChartsPlotKey *>(obj_);

    if (key)
      key->updatePositionAndLayout();
  }
  else if (view) {
    view->invalidateObjects();

    view->update();
  }
}

//---

#if 0
QColor
CQChartsInterpolator::
interpColor(const CQChartsColor &c, int i, int n) const
{
  CQChartsUtil::ColorInd ic(i, n);

  return interpColor(c, ic);
}

QColor
CQChartsInterpolator::
interpColor(const CQChartsColor &c, double r) const
{
  CQChartsUtil::ColorInd ic(r);

  return interpColor(c, ic);
}
#endif

QColor
CQChartsInterpolator::
interpColor(const CQChartsColor &c, const CQChartsUtil::ColorInd &ind) const
{
  CQChartsPlot *plot   = nullptr;
  CQChartsView *view   = nullptr;
  CQCharts     *charts = nullptr;

  getObjPlotViewChart(obj_, plot, view, charts);

  if      (plot  ) return plot  ->interpColor(c, ind);
  else if (view  ) return view  ->interpColor(c, ind);
  else if (charts) return charts->interpColor(c, ind);

  return QColor();
}
