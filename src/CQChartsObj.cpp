#include <CQChartsObj.h>
#include <CQChartsUtil.h>

CQChartsObj::
CQChartsObj(QObject *parent, const CQChartsGeom::BBox &rect) :
 QObject(parent), rect_(rect)
{
}

const QString &
CQChartsObj::
id() const
{
  if (! id_) {
    const_cast<CQChartsObj*>(this)->id_ = calcId();

    assert((*id_).length());
  }

  return *id_;
}

void
CQChartsObj::
setId(const QString &s)
{
  id_ = s;

  emit idChanged();
}

const QString &
CQChartsObj::
tipId() const
{
  if (! tipId_) {
    const_cast<CQChartsObj*>(this)->tipId_ = calcTipId();

    assert((*tipId_).length());
  }

  return *tipId_;
}
