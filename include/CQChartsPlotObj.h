#ifndef CQChartsPlotObj_H
#define CQChartsPlotObj_H

#include <CQChartsObj.h>
#include <CQChartsGeom.h>
#include <set>

class CQChartsPlot;
class CQPropertyViewModel;

/*!
 * \brief Plot Object base class
 */
class CQChartsPlotObj : public CQChartsObj {
  Q_OBJECT

  Q_PROPERTY(QString typeName READ typeName)
  Q_PROPERTY(bool    visible  READ isVisible  WRITE setVisible )

 public:
  using Indices = std::set<QModelIndex>;

 public:
  CQChartsPlotObj(CQChartsPlot *plot, const CQChartsGeom::BBox &rect=CQChartsGeom::BBox());

  virtual ~CQChartsPlotObj() { }

  //---

  //! get parent plot
  CQChartsPlot *plot() const { return plot_; }

  //---

  //! get type name
  virtual QString typeName() const = 0;

  //---

  //! get id from idColumn for index (if defined)
  bool calcColumnId(const QModelIndex &ind, QString &str) const;

  //---

  //! get/set visible
  bool isVisible() const { return visible_; }
  void setVisible(bool b) { visible_ = b; }

  //---

  virtual bool visible() const { return isVisible(); }

  //---

  // is point inside (override if not simple rect shape)
  virtual bool inside(const CQChartsGeom::Point &p) const {
    if (! isVisible()) return false;
    return rect_.inside(p);
  }

  // is rect inside/touching (override if not simple rect shape)
  virtual bool rectIntersect(const CQChartsGeom::BBox &r, bool inside) const {
    if (! isVisible()) return false;

    if (inside)
      return r.inside(rect_);
    else
      return r.overlaps(rect_);
  }

  //virtual void postResize() { }

  virtual void selectPress() { }

  //---

  //! get property path
  virtual QString propertyId() const;

  //! add properties
  virtual void addProperties(CQPropertyViewModel *model, const QString &path);

  //---

  // select
  bool isSelectIndex(const QModelIndex &ind) const;

  void addSelectIndices();

  void getHierSelectIndices(Indices &inds) const;

  virtual void getSelectIndices(Indices &inds) const = 0;

  virtual void addColumnSelectIndex(Indices &inds, const CQChartsColumn &column) const = 0;

  void addSelectIndex(Indices &inds, const CQChartsModelIndex &ind) const;
  void addSelectIndex(Indices &inds, int row, const CQChartsColumn &column,
                      const QModelIndex &parent=QModelIndex()) const;
  void addSelectIndex(Indices &inds, const QModelIndex &ind) const;

  //---

  // draw
  virtual void drawBg(QPainter *) const { }
  virtual void drawFg(QPainter *) const { }

  virtual void draw(QPainter *) = 0;

 protected:
  CQChartsPlot* plot_     { nullptr }; //! parent plot
  bool          visible_  { true };    //! is visible
};

//------

/*!
 * \brief Group Plot object
 */
class CQChartsGroupObj : public CQChartsPlotObj {
  Q_OBJECT

 public:
  CQChartsGroupObj(CQChartsPlot *plot, const CQChartsGeom::BBox &bbox=CQChartsGeom::BBox());
};

#endif
