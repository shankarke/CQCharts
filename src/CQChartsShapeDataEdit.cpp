#include <CQChartsShapeDataEdit.h>

#include <CQChartsStrokeDataEdit.h>
#include <CQChartsFillDataEdit.h>
#include <CQChartsView.h>
#include <CQChartsPlot.h>
#include <CQCharts.h>
#include <CQChartsRoundedPolygon.h>

#include <CQPropertyView.h>
#include <CQWidgetMenu.h>
#include <CQUtil.h>

#include <QLabel>
#include <QVBoxLayout>
#include <QPainter>

CQChartsShapeDataLineEdit::
CQChartsShapeDataLineEdit(QWidget *parent) :
 CQChartsLineEditBase(parent)
{
  setObjectName("shapeDataLineEdit");

  //---

  menuEdit_ = dataEdit_ = new CQChartsShapeDataEdit;

  menu_->setWidget(dataEdit_);

  //---

  connectSlots(true);

  shapeDataToWidgets();
}

const CQChartsShapeData &
CQChartsShapeDataLineEdit::
shapeData() const
{
  return dataEdit_->data();
}

void
CQChartsShapeDataLineEdit::
setShapeData(const CQChartsShapeData &shapeData)
{
  updateShapeData(shapeData, /*updateText*/ true);
}

void
CQChartsShapeDataLineEdit::
updateShapeData(const CQChartsShapeData &shapeData, bool updateText)
{
  connectSlots(false);

  dataEdit_->setData(shapeData);

  connectSlots(true);

  if (updateText)
    shapeDataToWidgets();

  emit shapeDataChanged();
}

void
CQChartsShapeDataLineEdit::
textChanged()
{
  CQChartsShapeData shapeData(edit_->text());

  if (! shapeData.isValid())
    return;

  updateShapeData(shapeData, /*updateText*/ false);
}

void
CQChartsShapeDataLineEdit::
shapeDataToWidgets()
{
  connectSlots(false);

  if (shapeData().isValid())
    edit_->setText(shapeData().toString());
  else
    edit_->setText("");

  setToolTip(shapeData().toString());

  connectSlots(true);
}

void
CQChartsShapeDataLineEdit::
menuEditChanged()
{
  shapeDataToWidgets();

  emit shapeDataChanged();
}

void
CQChartsShapeDataLineEdit::
connectSlots(bool b)
{
  CQChartsLineEditBase::connectSlots(b);

  if (b)
    connect(dataEdit_, SIGNAL(shapeDataChanged()), this, SLOT(menuEditChanged()));
  else
    disconnect(dataEdit_, SIGNAL(shapeDataChanged()), this, SLOT(menuEditChanged()));
}

void
CQChartsShapeDataLineEdit::
drawPreview(QPainter *painter, const QRect &rect)
{
  CQChartsShapeDataEditPreview::draw(painter, shapeData(), rect, plot(), view());
}

//------

CQPropertyViewEditorFactory *
CQChartsShapeDataPropertyViewType::
getEditor() const
{
  return new CQChartsShapeDataPropertyViewEditor;
}

void
CQChartsShapeDataPropertyViewType::
drawPreview(QPainter *painter, const QRect &rect, const QVariant &value,
            CQChartsPlot *plot, CQChartsView *view)
{
  CQChartsShapeData data = value.value<CQChartsShapeData>();

  CQChartsShapeDataEditPreview::draw(painter, data, rect, plot, view);
}

QString
CQChartsShapeDataPropertyViewType::
tip(const QVariant &value) const
{
  QString str = value.value<CQChartsShapeData>().toString();

  return str;
}

//------

CQChartsLineEditBase *
CQChartsShapeDataPropertyViewEditor::
createPropertyEdit(QWidget *parent)
{
  return new CQChartsShapeDataLineEdit(parent);
}

void
CQChartsShapeDataPropertyViewEditor::
connect(QWidget *w, QObject *obj, const char *method)
{
  CQChartsShapeDataLineEdit *edit = qobject_cast<CQChartsShapeDataLineEdit *>(w);
  assert(edit);

  QObject::connect(edit, SIGNAL(shapeDataChanged()), obj, method);
}

QVariant
CQChartsShapeDataPropertyViewEditor::
getValue(QWidget *w)
{
  CQChartsShapeDataLineEdit *edit = qobject_cast<CQChartsShapeDataLineEdit *>(w);
  assert(edit);

  return QVariant::fromValue(edit->shapeData());
}

void
CQChartsShapeDataPropertyViewEditor::
setValue(QWidget *w, const QVariant &var)
{
  CQChartsShapeDataLineEdit *edit = qobject_cast<CQChartsShapeDataLineEdit *>(w);
  assert(edit);

  CQChartsShapeData data = var.value<CQChartsShapeData>();

  edit->setShapeData(data);
}

//------

CQChartsShapeDataEdit::
CQChartsShapeDataEdit(QWidget *parent, bool tabbed) :
 CQChartsEditBase(parent), tabbed_(tabbed)
{
  setObjectName("shapeDataEdit");

  QGridLayout *layout = new QGridLayout(this);
  layout->setMargin(0); layout->setSpacing(2);

  int row = 0;

  //---

  if (tabbed_) {
    QTabWidget *tab = CQUtil::makeWidget<QTabWidget>("tab");

    layout->addWidget(tab, row, 0, 1, 2); ++row;

    //----

    // fill frame
    QFrame *fillFrame = CQUtil::makeWidget<QFrame>("fillFrame");

    QVBoxLayout *fillFrameLayout = new QVBoxLayout(fillFrame);
    fillFrameLayout->setMargin(0); fillFrameLayout->setSpacing(2);

    tab->addTab(fillFrame, "Fill");

    //--

    // background
    fillEdit_ = new CQChartsFillDataEdit;

    fillEdit_->setPreview(false);

    fillFrameLayout->addWidget(fillEdit_);

    //----

    // stroke frame
    QFrame *strokeFrame = CQUtil::makeWidget<QFrame>("strokeFrame");

    QVBoxLayout *strokeFrameLayout = new QVBoxLayout(strokeFrame);
    strokeFrameLayout->setMargin(0); strokeFrameLayout->setSpacing(2);

    tab->addTab(strokeFrame, "Stroke");

    //--

    // border
    strokeEdit_ = new CQChartsStrokeDataEdit;

    strokeEdit_->setPreview(false);

    strokeFrameLayout->addWidget(strokeEdit_);
  }
  else {
    // background
    fillEdit_ = new CQChartsFillDataEdit;

    fillEdit_->setTitle("Fill");
    fillEdit_->setPreview(false);

    layout->addWidget(fillEdit_, row, 0, 1, 2); ++row;

    //--

    // border
    strokeEdit_ = new CQChartsStrokeDataEdit;

    strokeEdit_->setTitle("Stroke");
    strokeEdit_->setPreview(false);

    layout->addWidget(strokeEdit_, row, 0, 1, 2); ++row;
  }

  //---

  // preview
  preview_ = new CQChartsShapeDataEditPreview(this);

  layout->addWidget(preview_, row, 1);

  //---

  layout->setRowStretch(row, 1);

  //---

  connectSlots(true);

  dataToWidgets();
}

void
CQChartsShapeDataEdit::
setData(const CQChartsShapeData &d)
{
  data_ = d;

  dataToWidgets();
}

void
CQChartsShapeDataEdit::
setPlot(CQChartsPlot *plot)
{
  CQChartsEditBase::setPlot(plot);

  fillEdit_  ->setPlot(plot);
  strokeEdit_->setPlot(plot);
}

void
CQChartsShapeDataEdit::
setView(CQChartsView *view)
{
  CQChartsEditBase::setView(view);

  fillEdit_  ->setView(view);
  strokeEdit_->setView(view);
}

void
CQChartsShapeDataEdit::
setTitle(const QString &)
{
  if (! tabbed_) {
    fillEdit_  ->setTitle("Fill");
    strokeEdit_->setTitle("Stroke");
  }
}

void
CQChartsShapeDataEdit::
setPreview(bool b)
{
  fillEdit_  ->setPreview(b);
  strokeEdit_->setPreview(b);

  preview_->setVisible(b);
}

void
CQChartsShapeDataEdit::
connectSlots(bool b)
{
  auto connectDisconnect = [&](bool b, QWidget *w, const char *from, const char *to) {
    if (b)
      QObject::connect(w, from, this, to);
    else
      QObject::disconnect(w, from, this, to);
  };

  connectDisconnect(b, fillEdit_, SIGNAL(fillDataChanged()), SLOT(widgetsToData()));
  connectDisconnect(b, strokeEdit_, SIGNAL(strokeDataChanged()), SLOT(widgetsToData()));
}

void
CQChartsShapeDataEdit::
dataToWidgets()
{
  connectSlots(false);

  fillEdit_  ->setData(data_.background());
  strokeEdit_->setData(data_.border());

  preview_->update();

  connectSlots(true);

}

void
CQChartsShapeDataEdit::
widgetsToData()
{
  data_.setBackground(fillEdit_  ->data());
  data_.setBorder    (strokeEdit_->data());

  preview_->update();

  emit shapeDataChanged();
}

//------

CQChartsShapeDataEditPreview::
CQChartsShapeDataEditPreview(CQChartsShapeDataEdit *edit) :
 CQChartsEditPreview(edit), edit_(edit)
{
}

void
CQChartsShapeDataEditPreview::
draw(QPainter *painter)
{
  const CQChartsShapeData &data = edit_->data();

  draw(painter, data, rect(), edit_->plot(), edit_->view());
}

void
CQChartsShapeDataEditPreview::
draw(QPainter *painter, const CQChartsShapeData &data, const QRect &rect,
     CQChartsPlot *plot, CQChartsView *view)
{
  // set pen and brush
  QColor pc = interpColor(plot, view, data.border    ().color());
  QColor fc = interpColor(plot, view, data.background().color());

  double width = CQChartsUtil::limitLineWidth(data.border().width().value());

  QPen pen;

  CQChartsUtil::setPen(pen, data.border().isVisible(), pc, data.border().alpha(),
                       width, data.border().dash());

  QBrush brush;

  CQChartsUtil::setBrush(brush, data.background().isVisible(), fc, data.background().alpha(),
                         data.background().pattern());

  painter->setPen  (pen);
  painter->setBrush(brush);

  //---

  // draw shape
  double cxs = data.border().cornerSize().value();
  double cys = data.border().cornerSize().value();

  CQChartsRoundedPolygon::draw(painter, rect, cxs, cys);
}
