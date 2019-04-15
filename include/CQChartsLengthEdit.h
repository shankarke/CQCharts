#ifndef CQChartsLengthEdit_H
#define CQChartsLengthEdit_H

#include <CQChartsLength.h>
#include <QFrame>

class CQChartsUnitsEdit;
class CQRealSpin;

//! \brief length edit
class CQChartsLengthEdit : public QFrame {
  Q_OBJECT

 public:
  CQChartsLengthEdit(QWidget *parent=nullptr);

  const CQChartsLength &length() const;
  void setLength(const CQChartsLength &pos);

 signals:
  void lengthChanged();

 private slots:
  void editChanged();
  void unitsChanged();

 private:
  void widgetsToLength();
  void lengthToWidgets();

  void connectSlots(bool b);

 private:
  CQChartsLength     length_;                //!< length data
  CQRealSpin*        edit_      { nullptr }; //!< length edit
  CQChartsUnitsEdit* unitsEdit_ { nullptr }; //!< units edit
  bool               connected_ { false };   //!< is connected
};

//------

#include <CQPropertyViewType.h>

//! \brief type for CQChartsLength
class CQChartsLengthPropertyViewType : public CQPropertyViewType {
 public:
  CQChartsLengthPropertyViewType();

  CQPropertyViewEditorFactory *getEditor() const override;

  bool setEditorData(CQPropertyViewItem *item, const QVariant &value) override;

  void draw(const CQPropertyViewDelegate *delegate, QPainter *painter,
            const QStyleOptionViewItem &option, const QModelIndex &index,
            const QVariant &value, bool inside) override;

  QString tip(const QVariant &value) const override;
};

//---

#include <CQPropertyViewEditor.h>

//! \brief editor factory for CQChartsLength
class CQChartsLengthPropertyViewEditor : public CQPropertyViewEditorFactory {
 public:
  CQChartsLengthPropertyViewEditor();

  QWidget *createEdit(QWidget *parent);

  void connect(QWidget *w, QObject *obj, const char *method);

  QVariant getValue(QWidget *w);

  void setValue(QWidget *w, const QVariant &var);
};

#endif
