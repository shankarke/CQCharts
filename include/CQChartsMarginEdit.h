#ifndef CQChartsMarginEdit_H
#define CQChartsMarginEdit_H

#include <CQChartsMargin.h>
#include <QFrame>

class CQChartsLineEdit;

/*!
 * \brief margin edit
 * \ingroup Charts
 */
class CQChartsMarginEdit : public QFrame {
  Q_OBJECT

  Q_PROPERTY(CQChartsMargin margin READ margin WRITE setMargin)

 public:
  CQChartsMarginEdit(QWidget *parent=nullptr);

  const CQChartsMargin &margin() const { return margin_; }
  void setMargin(const CQChartsMargin &margin);

 signals:
  void marginChanged();

 private slots:
  void editChanged();

 private:
  void widgetsToMargin();
  void marginToWidgets();

  void connectSlots(bool b);

 private:
  CQChartsMargin    margin_;                //!< margin data
  CQChartsLineEdit* edit_      { nullptr }; //!< line edit
  bool              connected_ { false };   //!< is connected
};

//------

#include <CQPropertyViewType.h>

/*!
 * \brief type for CQChartsMargin
 * \ingroup Charts
 */
class CQChartsMarginPropertyViewType : public CQPropertyViewType {
 public:
  CQChartsMarginPropertyViewType() { }

  CQPropertyViewEditorFactory *getEditor() const override;

  bool setEditorData(CQPropertyViewItem *item, const QVariant &value) override;

  void draw(CQPropertyViewItem *item, const CQPropertyViewDelegate *delegate, QPainter *painter,
            const QStyleOptionViewItem &option, const QModelIndex &index,
            const QVariant &value, bool inside) override;

  QString tip(const QVariant &value) const override;

  QString userName() const override { return "margin"; }
};

//---

#include <CQPropertyViewEditor.h>

/*!
 * \brief editor factory for CQChartsMargin
 * \ingroup Charts
 */
class CQChartsMarginPropertyViewEditor : public CQPropertyViewEditorFactory {
 public:
  CQChartsMarginPropertyViewEditor() { }

  QWidget *createEdit(QWidget *parent);

  void connect(QWidget *w, QObject *obj, const char *method);

  QVariant getValue(QWidget *w);

  void setValue(QWidget *w, const QVariant &var);
};

#endif
