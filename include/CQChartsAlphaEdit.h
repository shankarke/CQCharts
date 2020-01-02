#ifndef CQChartsAlphaEdit_H
#define CQChartsAlphaEdit_H

#include <CQChartsAlpha.h>
#include <CQRealSpin.h>

/*!
 * \brief alpha value edit
 * \ingroup Charts
 */
class CQChartsAlphaEdit : public CQRealSpin {
  Q_OBJECT

  Q_PROPERTY(CQChartsAlpha alpha READ alpha WRITE setAlpha)

 public:
  CQChartsAlphaEdit(QWidget *parent=nullptr);

  const CQChartsAlpha &alpha() const { return alpha_; }
  void setAlpha(const CQChartsAlpha &alpha);

 signals:
  void alphaChanged();

 private slots:
  void editChanged();

 private:
  void widgetsToAlpha();
  void alphaToWidgets();

  void connectSlots(bool b);

 private:
  CQChartsAlpha alpha_;               //!< alpha value
  bool          connected_ { false }; //!< is connected
};

//------

#include <CQPropertyViewType.h>

/*!
 * \brief type for CQChartsAlpha
 * \ingroup Charts
 */
class CQChartsAlphaPropertyViewType : public CQPropertyViewType {
 public:
  CQChartsAlphaPropertyViewType() { }

  CQPropertyViewEditorFactory *getEditor() const override;

  bool setEditorData(CQPropertyViewItem *item, const QVariant &value) override;

  void draw(CQPropertyViewItem *item, const CQPropertyViewDelegate *delegate, QPainter *painter,
            const QStyleOptionViewItem &option, const QModelIndex &index,
            const QVariant &value, bool inside) override;

  QString tip(const QVariant &value) const override;

  QString userName() const override { return "alpha"; }
};

//---

#include <CQPropertyViewEditor.h>

/*!
 * \brief editor factory for CQChartsAlpha
 * \ingroup Charts
 */
class CQChartsAlphaPropertyViewEditor : public CQPropertyViewEditorFactory {
 public:
  CQChartsAlphaPropertyViewEditor() { }

  QWidget *createEdit(QWidget *parent);

  void connect(QWidget *w, QObject *obj, const char *method);

  QVariant getValue(QWidget *w);

  void setValue(QWidget *w, const QVariant &var);
};

#endif
