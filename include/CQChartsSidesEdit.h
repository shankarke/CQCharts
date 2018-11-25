#ifndef CQChartsSidesEdit_H
#define CQChartsSidesEdit_H

#include <CQChartsSides.h>
#include <QFrame>

class QStyleOptionComboBox;

class CQChartsSidesEdit : public QFrame {
  Q_OBJECT

 public:
  CQChartsSidesEdit(QWidget *parent=nullptr);
 ~CQChartsSidesEdit();

  const CQChartsSides &sides() const;
  void setSides(const CQChartsSides &side);

  QSize sizeHint() const;

 signals:
  void sidesChanged();

 private:
  void paintEvent(QPaintEvent *);

  void mousePressEvent(QMouseEvent *);

  void initStyleOption(QStyleOptionComboBox &opt) const;

 private:
  CQChartsSides sides_;
};

//------

class CQChartsSidesEditMenuWidget : public QFrame {
  Q_OBJECT

 public:
  CQChartsSidesEditMenuWidget(CQChartsSidesEdit *edit);

  QSize sizeHint() const;

 private:
  void resizeEvent(QResizeEvent *);

  void paintEvent(QPaintEvent *);

  void mouseMoveEvent(QMouseEvent *);

  void mousePressEvent(QMouseEvent *);

  void drawSideRect(QPainter *p, CQChartsSides::Side side, bool on);

 private:
  struct Rect {
    QRect r;
    bool  inside { false };

    Rect(const QRect &r=QRect()) :
     r(r) {
    }
  };

  typedef std::map<CQChartsSides::Side,Rect> SideRect;

  CQChartsSidesEdit* edit_ { nullptr };
  SideRect           sideRect_;
};

//------

#include <CQPropertyViewType.h>

// type for CQChartsLength
class CQChartsSidesPropertyViewType : public CQPropertyViewType {
 public:
  CQChartsSidesPropertyViewType();

  CQPropertyViewEditorFactory *getEditor() const override;

  bool setEditorData(CQPropertyViewItem *item, const QVariant &value) override;

  void draw(const CQPropertyViewDelegate *delegate, QPainter *painter,
            const QStyleOptionViewItem &option, const QModelIndex &index,
            const QVariant &value, bool inside) override;

  QString tip(const QVariant &value) const override;
};

//---

#include <CQPropertyViewEditor.h>

// editor factory for CQChartsLength
class CQChartsSidesPropertyViewEditor : public CQPropertyViewEditorFactory {
 public:
  CQChartsSidesPropertyViewEditor();

  QWidget *createEdit(QWidget *parent);

  void connect(QWidget *w, QObject *obj, const char *method);

  QVariant getValue(QWidget *w);

  void setValue(QWidget *w, const QVariant &var);
};

#endif
