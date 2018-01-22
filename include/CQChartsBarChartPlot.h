#ifndef CQChartsBarChartPlot_H
#define CQChartsBarChartPlot_H

#include <CQChartsPlot.h>
#include <CQChartsPlotObj.h>
#include <CQChartsColorSet.h>
#include <CQChartsDataLabel.h>
#include <CQChartsPaletteColor.h>

class CQChartsBarChartPlot;
class CQChartsBoxObj;
class CQChartsFillObj;

// bar value
class CQChartsBarChartValue {
 public:
  using NameValues = std::map<QString,QString>;

 public:
  CQChartsBarChartValue() = default;

  CQChartsBarChartValue(double value, const QModelIndex &ind) :
   value_(value), ind_(ind) {
  }

  double value() const { return value_; }
  void setValue(double r) { value_ = r; }

  const QModelIndex &ind() const { return ind_; }
  void setInd(const QModelIndex &v) { ind_ = v; }

  const QString &groupName() const { return groupName_; }
  void setGroupName(const QString &v) { groupName_ = v; }

  const QString &valueName() const { return valueName_; }
  void setValueName(const QString &v) { valueName_ = v; }

  const NameValues &nameValues() const { return nameValues_; }
  void setNameValues(const NameValues &v) { nameValues_ = v; }

  void setNameValue(const QString &name, QString &value) {
    nameValues_[name] = value;
  }

  QString getNameValue(const QString &name) const {
    auto p = nameValues_.find(name);
    if (p == nameValues_.end()) return "";

    return (*p).second;
  }

 private:
  double      value_ { 0.0 };
  QModelIndex ind_;
  QString     groupName_;
  QString     valueName_;
  NameValues  nameValues_;
};

class CQChartsBarChartValueSet {
 public:
  using Values = std::vector<CQChartsBarChartValue>;

 public:
  CQChartsBarChartValueSet() = default;

  CQChartsBarChartValueSet(const QString &name, int ind) :
   name_(name), ind_(ind) {
  }

  const QString &name() const { return name_; }

  int ind() const { return ind_; }

  int numValues() const { return values_.size(); }

  const Values &values() const { return values_; }

  void addValue(const CQChartsBarChartValue &value) {
    values_.push_back(value);
  }

  const CQChartsBarChartValue &value(int i) const {
    assert(i >= 0 && i < int(values_.size()));

    return values_[i];
  }

  void calcSums(double &posSum, double &negSum) const {
    posSum = 0.0;
    negSum = 0.0;

    for (auto &v : values_) {
      double value = v.value();

      if (value >= 0) posSum += value;
      else            negSum += value;
    }
  }

 private:
  QString name_;
  int     ind_ { 0 };
  Values  values_;
};

//------

// bar object
class CQChartsBarChartObj : public CQChartsPlotObj {
  Q_OBJECT

 public:
  using OptColor = boost::optional<CQChartsPaletteColor>;

 public:
  CQChartsBarChartObj(CQChartsBarChartPlot *plot, const CQChartsGeom::BBox &rect,
                      int iset, int nset, int ival, int nval, int isval, int nsval,
                      const CQChartsBarChartValue *value, const QModelIndex &ind);

  QString calcId() const override;

  QString calcTipId() const override;

  void setColor(const CQChartsPaletteColor &color) { color_ = color; }

  CQChartsGeom::BBox dataLabelRect() const;

  void addSelectIndex() override;

  bool isIndex(const QModelIndex &) const override;

  void draw(QPainter *painter, const CQChartsPlot::Layer &) override;

 private:
  CQChartsBarChartPlot*        plot_  { nullptr }; // parent plot
  int                          iset_  { -1 };      // set number
  int                          nset_  { -1 };      // number of sets
  int                          ival_  { -1 };      // value number
  int                          nval_  { -1 };      // number of values
  int                          isval_ { -1 };      // sub set number
  int                          nsval_ { -1 };      // number of sub sets
  const CQChartsBarChartValue* value_ { nullptr }; // value data
  QModelIndex                  ind_;               // model index
  OptColor                     color_;             // custom color
};

//---

#include <CQChartsKey.h>

// key color box
class CQChartsBarKeyColor : public CQChartsKeyColorBox {
  Q_OBJECT

 public:
  using OptColor = boost::optional<CQChartsPaletteColor>;

 public:
  CQChartsBarKeyColor(CQChartsBarChartPlot *plot, int i, int n);

  void setColor(const CQChartsPaletteColor &color) { color_ = color; }

  bool mousePress(const CQChartsGeom::Point &p) override;

  QBrush fillBrush() const override;

  bool tipText(const CQChartsGeom::Point &p, QString &tip) const override;

  bool isSetHidden() const;

  void setSetHidden(bool b);

 private:
  CQChartsBarChartPlot *plot_;  // plot
  OptColor              color_; // custom color
};

// key text
class CQChartsBarKeyText : public CQChartsKeyText {
  Q_OBJECT

 public:
  CQChartsBarKeyText(CQChartsBarChartPlot *plot, int i, const QString &text);

  QColor interpTextColor(int i, int n) const override;

 private:
  int i_ { 0 }; // set id
};

//---

// bar chart plot type
class CQChartsBarChartPlotType : public CQChartsPlotType {
 public:
  CQChartsBarChartPlotType();

  QString name() const override { return "barchart"; }
  QString desc() const override { return "BarChart"; }

  void addParameters() override;

  CQChartsPlot *create(CQChartsView *view, const ModelP &model) const override;
};

//---

// bar chart plot
//  x   : category, name
//  y   : value, values
//  bar : custom color, stacked, horizontal, margin, border, fill
class CQChartsBarChartPlot : public CQChartsPlot {
  Q_OBJECT

  Q_PROPERTY(int     categoryColumn   READ categoryColumn    WRITE setCategoryColumn  )
  Q_PROPERTY(int     valueColumn      READ valueColumn       WRITE setValueColumn     )
  Q_PROPERTY(QString valueColumns     READ valueColumnsStr   WRITE setValueColumnsStr )
  Q_PROPERTY(int     nameColumn       READ nameColumn        WRITE setNameColumn      )
  Q_PROPERTY(int     labelColumn      READ labelColumn       WRITE setLabelColumn     )
  Q_PROPERTY(bool    rowGrouping      READ isRowGrouping     WRITE setRowGrouping     )
  Q_PROPERTY(bool    colorBySet       READ isColorBySet      WRITE setColorBySet      )
  Q_PROPERTY(int     colorColumn      READ colorColumn       WRITE setColorColumn     )
  Q_PROPERTY(bool    stacked          READ isStacked         WRITE setStacked         )
  Q_PROPERTY(bool    horizontal       READ isHorizontal      WRITE setHorizontal      )
  Q_PROPERTY(double  margin           READ margin            WRITE setMargin          )
  Q_PROPERTY(bool    border           READ isBorder          WRITE setBorder          )
  Q_PROPERTY(QString borderColor      READ borderColorStr    WRITE setBorderColorStr  )
  Q_PROPERTY(double  borderAlpha      READ borderAlpha       WRITE setBorderAlpha     )
  Q_PROPERTY(double  borderWidth      READ borderWidth       WRITE setBorderWidth     )
  Q_PROPERTY(double  borderCornerSize READ borderCornerSize  WRITE setBorderCornerSize)
  Q_PROPERTY(bool    barFill          READ isBarFill         WRITE setBarFill         )
  Q_PROPERTY(QString barColor         READ barColorStr       WRITE setBarColorStr     )
  Q_PROPERTY(double  barAlpha         READ barAlpha          WRITE setBarAlpha        )
  Q_PROPERTY(Pattern barPattern       READ barPattern        WRITE setBarPattern      )
  Q_PROPERTY(bool    colorMapEnabled  READ isColorMapEnabled WRITE setColorMapEnabled )
  Q_PROPERTY(double  colorMapMin      READ colorMapMin       WRITE setColorMapMin     )
  Q_PROPERTY(double  colorMapMax      READ colorMapMax       WRITE setColorMapMax     )

  Q_ENUMS(Pattern)

 public:
  enum class Pattern {
    SOLID,
    HATCH,
    DENSE,
    HORIZ,
    VERT,
    FDIAG,
    BDIAG
  };

 public:
  CQChartsBarChartPlot(CQChartsView *view, const ModelP &model);
 ~CQChartsBarChartPlot();

  //---

  int categoryColumn() const { return categoryColumn_; }
  void setCategoryColumn(int i);

  int valueColumn() const { return valueColumn_; }
  void setValueColumn(int i);

  const Columns &valueColumns() const { return valueColumns_; }
  void setValueColumns(const Columns &valueColumns);

  QString valueColumnsStr() const;
  bool setValueColumnsStr(const QString &s);

  int valueColumnAt(int i) {
    assert(i >= 0 && i < int(valueColumns_.size()));

    return valueColumns_[i];
  }

  int numValueColumns() const { return std::max(int(valueColumns_.size()), 1); }

  int nameColumn() const { return nameColumn_; }
  void setNameColumn(int i);

  int labelColumn() const { return labelColumn_; }
  void setLabelColumn(int i);

  //---

  bool isColorBySet() const { return colorBySet_; }
  void setColorBySet(bool b) { colorBySet_ = b; resetSetHidden(); updateRangeAndObjs(); }

  //---

  bool isStacked() const { return stacked_; }
  void setStacked(bool b) { stacked_ = b; updateRangeAndObjs(); }

  bool isHorizontal() const { return horizontal_; }
  void setHorizontal(bool b) { horizontal_ = b; updateRangeAndObjs(); }

  //---

  // bar margin
  int margin() const { return margin_; }
  void setMargin(int i) { margin_ = i; update(); }

  //---

  // bar stroke
  bool isBorder() const;
  void setBorder(bool b);

  QString borderColorStr() const;
  void setBorderColorStr(const QString &s);

  QColor interpBorderColor(int i, int n) const;

  double borderAlpha() const;
  void setBorderAlpha(double r);

  double borderWidth() const;
  void setBorderWidth(double r);

  double borderCornerSize() const;
  void setBorderCornerSize(double r);

  //---

  // bar fill
  bool isBarFill() const;
  void setBarFill(bool b);

  QString barColorStr() const;
  void setBarColorStr(const QString &str);

  double barAlpha() const;
  void setBarAlpha(double a);

  Pattern barPattern() const;
  void setBarPattern(Pattern pattern);

  //---

  int colorColumn() const { return valueSetColumn("color"); }
  void setColorColumn(int i) { setValueSetColumn("color", i); updateRangeAndObjs(); }

  bool isColorMapEnabled() const { return isValueSetMapEnabled("color"); }
  void setColorMapEnabled(bool b) { setValueSetMapEnabled("color", b); updateObjs(); }

  double colorMapMin() const { return valueSetMapMin("color"); }
  void setColorMapMin(double r) { setValueSetMapMin("color", r); updateObjs(); }

  double colorMapMax() const { return valueSetMapMax("color"); }
  void setColorMapMax(double r) { setValueSetMapMax("color", r); updateObjs(); }

  //---

  const CQChartsDataLabel &dataLabel() const { return dataLabel_; }
  CQChartsDataLabel &dataLabel() { return dataLabel_; }

  //---

  CQChartsGeom::BBox annotationBBox() const override;

  //---

  void addProperties() override;

  void updateRange(bool apply=true) override;

  void updateObjs() override;

  bool initObjs() override;

  //---

  QColor interpBarColor(int i, int n) const;

  QString valueStr(double v) const;

  void addKeyItems(CQChartsKey *key) override;

   //---

  bool isSetHidden  (int i) const;
  bool isValueHidden(int i) const;

  //---

  bool probe(ProbeData &probeData) const override;

  void draw(QPainter *) override;

 private:
  void addRow(QAbstractItemModel *model, const QModelIndex &parent, int r);

  void addRowColumn(QAbstractItemModel *model, const QModelIndex &parent, int row, int valueColumn);

 private:
  using ValueSets     = std::vector<CQChartsBarChartValueSet>;
  using ValueNames    = std::vector<QString>;
  using ValueGroupInd = std::map<int,int>;

 public:
  int numValueSets() const { return valueSets_.size(); }

  const CQChartsBarChartValueSet &valueSet(int i) const {
    assert(i >= 0 && i < int(valueSets_.size()));
    return valueSets_[i];
  }

  int numSetValues() const { return (! valueSets_.empty() ? valueSets_[0].numValues() : 0); }

 private:
  const QString &valueName(int i) const { return valueNames_[i]; }

  CQChartsBarChartValueSet *groupValueSet(int groupId);

 private:
  int               categoryColumn_ { -1 };      // category column
  int               valueColumn_    { 1 };       // value column
  Columns           valueColumns_;               // value columns
  int               nameColumn_     { -1 };      // name column
  int               labelColumn_    { -1 };      // data label column
  bool              colorBySet_     { false };   // color bars by set or value
  bool              stacked_        { false };   // stacked bars
  bool              horizontal_     { false };   // horizontal bars
  double            margin_         { 2 };       // bar margin
  CQChartsBoxObj*   borderObj_      { nullptr }; // border data
  CQChartsFillObj*  fillObj_        { nullptr }; // fill data
  CQChartsDataLabel dataLabel_;                  // data label data
  ValueSets         valueSets_;                  // value sets
  ValueNames        valueNames_;                 // value names
  ValueGroupInd     valueGroupInd_;              // group ind to value index map
  int               numVisible_     { 0 };       // number of visible bars
};

#endif
