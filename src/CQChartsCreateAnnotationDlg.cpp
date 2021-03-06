#include <CQChartsCreateAnnotationDlg.h>
#include <CQChartsAnnotation.h>
#include <CQChartsArrow.h>
#include <CQChartsView.h>
#include <CQChartsPlot.h>
#include <CQChartsPositionEdit.h>
#include <CQChartsRectEdit.h>
#include <CQChartsLengthEdit.h>
#include <CQChartsMarginEdit.h>
#include <CQChartsSymbolDataEdit.h>
#include <CQChartsTextDataEdit.h>
#include <CQChartsSidesEdit.h>
#include <CQChartsPolygonEdit.h>
#include <CQChartsFillDataEdit.h>
#include <CQChartsStrokeDataEdit.h>
#include <CQChartsArrowDataEdit.h>
#include <CQChartsColorEdit.h>
#include <CQChartsWidgetUtil.h>
#include <CQChartsLineEdit.h>

#include <CQRealSpin.h>
#include <CQCheckBox.h>
#include <CQGroupBox.h>
#include <CQUtil.h>

#include <QFrame>
#include <QStackedWidget>
#include <QComboBox>
#include <QRadioButton>
#include <QLabel>
#include <QVBoxLayout>

CQChartsCreateAnnotationDlg::
CQChartsCreateAnnotationDlg(QWidget *parent, CQChartsView *view) :
 QDialog(parent), view_(view)
{
  initWidgets();
}

CQChartsCreateAnnotationDlg::
CQChartsCreateAnnotationDlg(QWidget *parent, CQChartsPlot *plot) :
 QDialog(parent), plot_(plot)
{
  initWidgets();
}

void
CQChartsCreateAnnotationDlg::
initWidgets()
{
  setObjectName("createAnnotationDlg");

  setWindowTitle("Create Annotation");

  //---

  auto *layout = CQUtil::makeLayout<QVBoxLayout>(this, 2, 2);

  //----

  // type combo
  typeCombo_ = CQUtil::makeWidget<QComboBox>("type");

  typeCombo_->addItems(typeNames());

  connect(typeCombo_, SIGNAL(currentIndexChanged(int)), this, SLOT(typeSlot(int)));

  addLabelWidget(layout, "Type", typeCombo_);

  //----

  auto *gridLayout = CQUtil::makeLayout<QGridLayout>(2, 2);

  layout->addLayout(gridLayout);

  int row = 0;

  //--

  // id, tip edits
  idEdit_  = CQUtil::makeWidget<CQChartsLineEdit>("id");
  tipEdit_ = CQUtil::makeWidget<CQChartsLineEdit>("tip");

  idEdit_ ->setToolTip("Annotation Id");
  tipEdit_->setToolTip("Annotation Tooltip");

  CQChartsWidgetUtil::addGridLabelWidget(gridLayout, "Id" , idEdit_ , row);
  CQChartsWidgetUtil::addGridLabelWidget(gridLayout, "Tip", tipEdit_, row);

  //----

  // per type widgets stack
  typeStack_ = CQUtil::makeWidget<QStackedWidget>("typeStack");

  layout->addWidget(typeStack_);

  createRectFrame    ();
  createEllipseFrame ();
  createPolygonFrame ();
  createPolyLineFrame();
  createTextFrame    ();
  createArrowFrame   ();
  createPointFrame   ();

  typeStack_->addWidget(rectWidgets_    .frame);
  typeStack_->addWidget(ellipseWidgets_ .frame);
  typeStack_->addWidget(polygonWidgets_ .frame);
  typeStack_->addWidget(polylineWidgets_.frame);
  typeStack_->addWidget(textWidgets_    .frame);
  typeStack_->addWidget(arrowWidgets_   .frame);
  typeStack_->addWidget(pointWidgets_   .frame);

  //---

  msgLabel_ = CQUtil::makeWidget<QLabel>("msgLabel");

  CQChartsWidgetUtil::setTextColor(msgLabel_, Qt::red);

  layout->addWidget(msgLabel_);

  //---

  // OK, Apply, Cancel Buttons
  auto *buttons = CQUtil::makeWidget<CQChartsDialogButtons>("buttons");

  buttons->connect(this, SLOT(okSlot()), SLOT(applySlot()), SLOT(cancelSlot()));

  layout->addWidget(buttons);
}

QStringList
CQChartsCreateAnnotationDlg::
typeNames() const
{
  static QStringList strs;

  if (strs.length() == 0)
    strs << "Rectangle" << "Ellipse" << "Polygon" << "Polyline" << "Text" << "Arrow" << "Point";

  return strs;
}

void
CQChartsCreateAnnotationDlg::
createRectFrame()
{
  rectWidgets_.frame = CQUtil::makeWidget<QFrame>("rectFrame");

  auto *frameLayout = CQUtil::makeLayout<QVBoxLayout>(rectWidgets_.frame, 2, 2);

  //----

  auto *gridLayout = CQUtil::makeLayout<QGridLayout>(2, 2);

  frameLayout->addLayout(gridLayout);

  int row = 0;

  //--

  // rect
  rectWidgets_.rectEdit = CQUtil::makeWidget<CQChartsRectEdit>("rectEdit");

  CQChartsRect rect = CQChartsViewPlotObj::makeRect(view(), plot(), 0, 0, 1, 1);

  rectWidgets_.rectEdit->setRect(rect);

  CQChartsWidgetUtil::addGridLabelWidget(gridLayout, "Rect", rectWidgets_.rectEdit, row);

  //--

  // outer margin
  rectWidgets_.marginEdit  = CQUtil::makeWidget<CQChartsMarginEdit>("marginEdit");

  CQChartsWidgetUtil::addGridLabelWidget(gridLayout, "Margin", rectWidgets_.marginEdit, row);

  //--

  // inner padding
  rectWidgets_.paddingEdit = CQUtil::makeWidget<CQChartsMarginEdit>("paddingEdit");

  CQChartsWidgetUtil::addGridLabelWidget(gridLayout, "Padding", rectWidgets_.paddingEdit, row);

  //---

  addFillWidgets  (rectWidgets_, frameLayout);
  addStrokeWidgets(rectWidgets_, frameLayout, /*cornerSize*/true);

  addSidesWidget(rectWidgets_, frameLayout);

  //---

  frameLayout->addStretch(1);
}

void
CQChartsCreateAnnotationDlg::
createEllipseFrame()
{
  ellipseWidgets_.frame = CQUtil::makeWidget<QFrame>("ellipseFrame");

  auto *frameLayout = CQUtil::makeLayout<QVBoxLayout>(ellipseWidgets_.frame, 2, 2);

  //---

  ellipseWidgets_.centerEdit = new CQChartsPositionEdit;
  ellipseWidgets_.rxEdit     = new CQChartsLengthEdit;
  ellipseWidgets_.ryEdit     = new CQChartsLengthEdit;

  ellipseWidgets_.centerEdit->setToolTip("Ellipse Center Poistion");
  ellipseWidgets_.rxEdit    ->setToolTip("Ellipse X Radius Length");
  ellipseWidgets_.rxEdit    ->setToolTip("Ellipse Y Radius Length");

  CQChartsPosition pos = CQChartsViewPlotObj::makePosition(view(), plot(), 0.0, 0.0);
  CQChartsLength   ul  = CQChartsViewPlotObj::makeLength(view(), plot(), 1.0);

  ellipseWidgets_.centerEdit->setPosition(pos);
  ellipseWidgets_.rxEdit    ->setLength(ul);
  ellipseWidgets_.ryEdit    ->setLength(ul);

  //---

  auto *gridLayout = CQUtil::makeLayout<QGridLayout>(2, 2);

  frameLayout->addLayout(gridLayout);

  int row = 0;

  //--

  CQChartsWidgetUtil::addGridLabelWidget(gridLayout, "Center"  , ellipseWidgets_.centerEdit, row);
  CQChartsWidgetUtil::addGridLabelWidget(gridLayout, "Radius X", ellipseWidgets_.rxEdit    , row);
  CQChartsWidgetUtil::addGridLabelWidget(gridLayout, "Radius Y", ellipseWidgets_.ryEdit    , row);

  //---

  addFillWidgets  (ellipseWidgets_, frameLayout);
  addStrokeWidgets(ellipseWidgets_, frameLayout, /*cornerSize*/true);

  addSidesWidget(ellipseWidgets_, frameLayout);

  //---

  frameLayout->addStretch(1);
}

void
CQChartsCreateAnnotationDlg::
createPolygonFrame()
{
  polygonWidgets_.frame = CQUtil::makeWidget<QFrame>("polygonFrame");

  auto *frameLayout = CQUtil::makeLayout<QVBoxLayout>(polygonWidgets_.frame, 2, 2);

  //---

  polygonWidgets_.pointsEdit = new CQChartsPolygonEdit;

  if      (view()) polygonWidgets_.pointsEdit->setUnits(CQChartsUnits::VIEW);
  else if (plot()) polygonWidgets_.pointsEdit->setUnits(CQChartsUnits::PLOT);

  addLabelWidget(frameLayout, "Points", polygonWidgets_.pointsEdit);

  //---

  addFillWidgets  (polygonWidgets_, frameLayout);
  addStrokeWidgets(polygonWidgets_, frameLayout, /*cornerSize*/false);

  //---

  frameLayout->addStretch(1);
}

void
CQChartsCreateAnnotationDlg::
createPolyLineFrame()
{
  polylineWidgets_.frame = CQUtil::makeWidget<QFrame>("polylineFrame");

  auto *frameLayout = CQUtil::makeLayout<QVBoxLayout>(polylineWidgets_.frame, 2, 2);

  //---

  polylineWidgets_.pointsEdit = new CQChartsPolygonEdit;

  if      (view()) polylineWidgets_.pointsEdit->setUnits(CQChartsUnits::VIEW);
  else if (plot()) polylineWidgets_.pointsEdit->setUnits(CQChartsUnits::PLOT);

  addLabelWidget(frameLayout, "Points", polylineWidgets_.pointsEdit);

  //---

  addFillWidgets  (polylineWidgets_, frameLayout);
  addStrokeWidgets(polylineWidgets_, frameLayout, /*cornerSize*/false);

  //---

  frameLayout->addStretch(1);
}

void
CQChartsCreateAnnotationDlg::
createTextFrame()
{
  textWidgets_.frame = CQUtil::makeWidget<QFrame>("textFrame");

  auto *frameLayout = CQUtil::makeLayout<QVBoxLayout>(textWidgets_.frame, 2, 2);

  //---

  auto *positionRectFrame  = CQUtil::makeWidget<QFrame>("positionRectFrame");
  auto *positionRectLayout = CQUtil::makeLayout<QHBoxLayout>(positionRectFrame, 2, 2);

  textWidgets_.positionRadio = CQUtil::makeLabelWidget<QRadioButton>("Position", "positionRadio");
  textWidgets_.rectRadio     = CQUtil::makeLabelWidget<QRadioButton>("Rect", "rectRadio");

  textWidgets_.positionRadio->setToolTip("Position text at point");
  textWidgets_.rectRadio    ->setToolTip("Place text in rectangle");

  positionRectLayout->addWidget(textWidgets_.positionRadio);
  positionRectLayout->addWidget(textWidgets_.rectRadio);
  positionRectLayout->addStretch(1);

  connect(textWidgets_.positionRadio, SIGNAL(toggled(bool)), this, SLOT(textPositionSlot(bool)));

  textWidgets_.positionEdit = new CQChartsPositionEdit;
  textWidgets_.rectEdit     = new CQChartsRectEdit;

  CQChartsPosition pos  = CQChartsViewPlotObj::makePosition(view(), plot(), 0.0, 0.0);
  CQChartsRect     rect = CQChartsViewPlotObj::makeRect(view(), plot(), 0, 0, 1, 1);

  textWidgets_.positionEdit->setPosition(pos);
  textWidgets_.rectEdit    ->setRect(rect);

  textWidgets_.positionRadio->setChecked(true);

  frameLayout->addWidget(positionRectFrame);

  textPositionSlot(true);

  //---

  auto *gridLayout1 = CQUtil::makeLayout<QGridLayout>(2, 2);

  frameLayout->addLayout(gridLayout1);

  int row1 = 0;

  //--

  CQChartsWidgetUtil::addGridLabelWidget(gridLayout1, "Position", textWidgets_.positionEdit, row1);
  CQChartsWidgetUtil::addGridLabelWidget(gridLayout1, "Rect"    , textWidgets_.rectEdit    , row1);

  //---

  textWidgets_.textEdit = CQUtil::makeWidget<CQChartsLineEdit>("edit");

  textWidgets_.textEdit->setToolTip("Text String");

  //---

  auto *gridLayout = CQUtil::makeLayout<QGridLayout>(2, 2);

  frameLayout->addLayout(gridLayout);

  int row = 0;

  //--

  CQChartsWidgetUtil::addGridLabelWidget(gridLayout, "Text", textWidgets_.textEdit, row);

  //---

  textWidgets_.dataEdit = new CQChartsTextDataEdit(nullptr, /*optional*/false);

  textWidgets_.dataEdit->setPlot(plot());
  textWidgets_.dataEdit->setView(view());

  //--

  frameLayout->addWidget(textWidgets_.dataEdit);

  //---

  addFillWidgets  (textWidgets_, frameLayout);
  addStrokeWidgets(textWidgets_, frameLayout, /*cornerSize*/true);

  addSidesWidget(textWidgets_, frameLayout);

  //---

  frameLayout->addStretch(1);
}

void
CQChartsCreateAnnotationDlg::
createArrowFrame()
{
  arrowWidgets_.frame = CQUtil::makeWidget<QFrame>("arrowFrame");

  auto *frameLayout = CQUtil::makeLayout<QVBoxLayout>(arrowWidgets_.frame, 2, 2);

  //---

  auto *gridLayout = CQUtil::makeLayout<QGridLayout>(2, 2);

  frameLayout->addLayout(gridLayout);

  int row = 0;

  //--

  // start, end point
  arrowWidgets_.startEdit = new CQChartsPositionEdit;
  arrowWidgets_.endEdit   = new CQChartsPositionEdit;

  CQChartsPosition startPos = CQChartsViewPlotObj::makePosition(view(), plot(), 0, 0);
  CQChartsPosition endPos   = CQChartsViewPlotObj::makePosition(view(), plot(), 1, 1);

  arrowWidgets_.startEdit->setPosition(startPos);
  arrowWidgets_.endEdit  ->setPosition(endPos);

  CQChartsWidgetUtil::addGridLabelWidget(gridLayout, "Start", arrowWidgets_.startEdit, row);
  CQChartsWidgetUtil::addGridLabelWidget(gridLayout, "End"  , arrowWidgets_.endEdit  , row);

  //---

  // arrow data edit
  arrowWidgets_.dataEdit = new CQChartsArrowDataEdit;

  arrowWidgets_.dataEdit->setPlot(plot());
  arrowWidgets_.dataEdit->setView(view());

  CQChartsLength len = CQChartsViewPlotObj::makeLength(view(), plot(), 1);

  arrowWidgets_.dataEdit->lengthEdit()->setLength(len);

  frameLayout->addWidget(arrowWidgets_.dataEdit);

  //---

  auto *gridLayout1 = CQUtil::makeLayout<QGridLayout>(2, 2);

  frameLayout->addLayout(gridLayout1);

  int row1 = 0;

  //--

  // stroke width, stroke color, filled and fill color
  arrowWidgets_.strokeWidthEdit = new CQChartsLengthEdit;
  arrowWidgets_.strokeColorEdit = new CQChartsColorLineEdit;
  arrowWidgets_.filledCheck     = CQUtil::makeWidget<CQCheckBox>("filledCheck");
  arrowWidgets_.fillColorEdit   = new CQChartsColorLineEdit;

  arrowWidgets_.filledCheck->setToolTip("Is Filled");

  arrowWidgets_.strokeWidthEdit->setLength(CQChartsLength(0, CQChartsUnits::PIXEL));

  CQChartsWidgetUtil::addGridLabelWidget(gridLayout1, "Stroke Width",
    arrowWidgets_.strokeWidthEdit, row1);
  CQChartsWidgetUtil::addGridLabelWidget(gridLayout1, "Stroke Color",
    arrowWidgets_.strokeColorEdit, row1);
  CQChartsWidgetUtil::addGridLabelWidget(gridLayout1, "Filled"      ,
    arrowWidgets_.filledCheck    , row1);
  CQChartsWidgetUtil::addGridLabelWidget(gridLayout1, "Fill Color"  ,
    arrowWidgets_.fillColorEdit  , row1);

  //---

  frameLayout->addStretch(1);
}

void
CQChartsCreateAnnotationDlg::
createPointFrame()
{
  pointWidgets_.frame = CQUtil::makeWidget<QFrame>("pointFrame");

  auto *frameLayout = CQUtil::makeLayout<QVBoxLayout>(pointWidgets_.frame, 2, 2);

  //---

  auto *gridLayout = CQUtil::makeLayout<QGridLayout>(2, 2);

  frameLayout->addLayout(gridLayout);

  int row = 0;

  //---

  pointWidgets_.positionEdit = new CQChartsPositionEdit;

  CQChartsPosition pos = CQChartsViewPlotObj::makePosition(view(), plot(), 0, 0);

  pointWidgets_.positionEdit->setPosition(pos);

  CQChartsWidgetUtil::addGridLabelWidget(gridLayout, "Position", pointWidgets_.positionEdit, row);

  //---

  pointWidgets_.dataEdit = new CQChartsSymbolDataEdit(nullptr, /*optional*/false);

  pointWidgets_.dataEdit->setPlot(plot());
  pointWidgets_.dataEdit->setView(view());

  frameLayout->addWidget(pointWidgets_.dataEdit);

  //---

  frameLayout->addStretch(1);
}

void
CQChartsCreateAnnotationDlg::
addFillWidgets(Widgets &widgets, QBoxLayout *playout)
{
  widgets.backgroundDataEdit = new CQChartsFillDataEdit;

  widgets.backgroundDataEdit->setTitle("Fill");
  widgets.backgroundDataEdit->setToolTip("Enable Fill");

  widgets.backgroundDataEdit->setPlot(plot());
  widgets.backgroundDataEdit->setView(view());

  playout->addWidget(widgets.backgroundDataEdit);
}

void
CQChartsCreateAnnotationDlg::
addStrokeWidgets(Widgets &widgets, QBoxLayout *playout, bool cornerSize)
{
  widgets.strokeDataEdit =
    new CQChartsStrokeDataEdit(nullptr,
          CQChartsStrokeDataEditConfig().setCornerSize(cornerSize));

  widgets.strokeDataEdit->setTitle("Stroke");
  widgets.strokeDataEdit->setToolTip("Enable Stroke");

  widgets.strokeDataEdit->setPlot(plot());
  widgets.strokeDataEdit->setView(view());

  playout->addWidget(widgets.strokeDataEdit);
}

void
CQChartsCreateAnnotationDlg::
addSidesWidget(Widgets &widgets, QBoxLayout *playout)
{
  auto *gridLayout = CQUtil::makeLayout<QGridLayout>(2, 2);

  int row = 0;

  //--

  widgets.borderSidesEdit = CQUtil::makeWidget<CQChartsSidesEdit>("borderSidesEdit");

  widgets.borderSidesEdit->setSides(CQChartsSides::Side::ALL);

  CQChartsWidgetUtil::addGridLabelWidget(gridLayout, "Border Sides", widgets.borderSidesEdit, row);

  //--

  playout->addLayout(gridLayout);
}

QHBoxLayout *
CQChartsCreateAnnotationDlg::
addLabelWidget(QBoxLayout *playout, const QString &label, QWidget *widget)
{
  auto *layout = CQUtil::makeLayout<QHBoxLayout>(0, 2);

  auto *qlabel = CQUtil::makeLabelWidget<QLabel>(label, "label" + label);

  layout->addWidget (qlabel);
  layout->addWidget (widget);
  layout->addStretch(1);

  playout->addLayout(layout);

  return layout;
}

void
CQChartsCreateAnnotationDlg::
typeSlot(int ind)
{
  if      (view())
    setWindowTitle(QString("Create View %1 Annotation").
      arg(typeCombo_->currentText()));
  else if (plot())
    setWindowTitle(QString("Create Plot %1 Annotation (%2)").
      arg(typeCombo_->currentText()).arg(plot()->id()));

  typeStack_->setCurrentIndex(ind);

  clearErrorMsg();
}

void
CQChartsCreateAnnotationDlg::
textPositionSlot(bool)
{
  if (textWidgets_.positionRadio->isChecked()) {
    textWidgets_.positionEdit->setEnabled(true);
    textWidgets_.rectEdit    ->setEnabled(false);
  }
  else {
    textWidgets_.positionEdit->setEnabled(false);
    textWidgets_.rectEdit    ->setEnabled(true);
  }
}

void
CQChartsCreateAnnotationDlg::
okSlot()
{
  if (applySlot())
    cancelSlot();
}

bool
CQChartsCreateAnnotationDlg::
applySlot()
{
  int ind = typeStack_->currentIndex();

  bool rc = false;

  if      (ind == 0) rc = createRectangleAnnotation();
  else if (ind == 1) rc = createEllipseAnnotation();
  else if (ind == 2) rc = createPolygonAnnotation();
  else if (ind == 3) rc = createPolylineAnnotation();
  else if (ind == 4) rc = createTextAnnotation();
  else if (ind == 5) rc = createArrowAnnotation();
  else if (ind == 6) rc = createPointAnnotation();

  return rc;
}

bool
CQChartsCreateAnnotationDlg::
createRectangleAnnotation()
{
  CQChartsBoxData boxData;

  CQChartsShapeData &shapeData = boxData.shape();

  //---

  auto id    = idEdit_ ->text();
  auto tipId = tipEdit_->text();

  auto rect = rectWidgets_.rectEdit->rect();

  if (! rect.isValid())
    return setErrorMsg("Invalid rectangle geometry");

  boxData.setMargin (rectWidgets_.marginEdit ->margin());
  boxData.setPadding(rectWidgets_.paddingEdit->margin());

  auto fill   = rectWidgets_.backgroundDataEdit->data();
  auto stroke = rectWidgets_.strokeDataEdit    ->data();

  shapeData.setFill  (fill);
  shapeData.setStroke(stroke);

  boxData.setBorderSides(rectWidgets_.borderSidesEdit->sides());

  //---

  CQChartsRectangleAnnotation *annotation = nullptr;

  if      (view())
    annotation = view()->addRectangleAnnotation(rect);
  else if (plot())
    annotation = plot()->addRectangleAnnotation(rect);
  else
    return false;

  if (id != "")
    annotation->setId(id);

  annotation->setTipId(tipId);

  annotation->setBoxData(boxData);

  return true;
}

bool
CQChartsCreateAnnotationDlg::
createEllipseAnnotation()
{
  CQChartsBoxData boxData;

  CQChartsShapeData &shapeData = boxData.shape();

  //---

  auto id    = idEdit_ ->text();
  auto tipId = tipEdit_->text();

  auto center = ellipseWidgets_.centerEdit->position();
  auto rx     = ellipseWidgets_.rxEdit->length();
  auto ry     = ellipseWidgets_.ryEdit->length();

  if (rx.value() <= 0.0 || ry.value() <= 0.0)
    return setErrorMsg("Invalid ellipse radius");

  auto fill   = ellipseWidgets_.backgroundDataEdit->data();
  auto stroke = ellipseWidgets_.strokeDataEdit    ->data();

  shapeData.setFill  (fill);
  shapeData.setStroke(stroke);

  boxData.setBorderSides(ellipseWidgets_.borderSidesEdit->sides());

  //---

  CQChartsEllipseAnnotation *annotation = nullptr;

  if      (view())
    annotation = view()->addEllipseAnnotation(center, rx, ry);
  else if (plot())
    annotation = plot()->addEllipseAnnotation(center, rx, ry);
  else
    return false;

  if (id != "")
    annotation->setId(id);

  annotation->setTipId(tipId);

  annotation->setBoxData(boxData);

  return true;
}

bool
CQChartsCreateAnnotationDlg::
createPolygonAnnotation()
{
  CQChartsBoxData boxData;

  CQChartsShapeData &shapeData = boxData.shape();

  //---

  auto id    = idEdit_ ->text();
  auto tipId = tipEdit_->text();

  auto polygon = polygonWidgets_.pointsEdit->polygon();

  if (polygon.units() == CQChartsUnits::NONE) {
    if      (view())
      polygon.setUnits(CQChartsUnits::VIEW);
    else if (plot())
      polygon.setUnits(CQChartsUnits::PLOT);
  }

  if (! polygon.isValid(/*closed*/true))
    return setErrorMsg("Not enough polygon points");

  auto fill   = polygonWidgets_.backgroundDataEdit->data();
  auto stroke = polygonWidgets_.strokeDataEdit    ->data();

  shapeData.setFill  (fill);
  shapeData.setStroke(stroke);

  //---

  CQChartsPolygonAnnotation *annotation = nullptr;

  if      (view())
    annotation = view()->addPolygonAnnotation(polygon);
  else if (plot())
    annotation = plot()->addPolygonAnnotation(polygon);
  else
    return false;

  if (id != "")
    annotation->setId(id);

  annotation->setTipId(tipId);

  annotation->setBoxData(boxData);

  return true;
}

bool
CQChartsCreateAnnotationDlg::
createPolylineAnnotation()
{
  CQChartsBoxData boxData;

  CQChartsShapeData &shapeData = boxData.shape();

  //---

  auto id    = idEdit_ ->text();
  auto tipId = tipEdit_->text();

  auto polygon = polylineWidgets_.pointsEdit->polygon();

  if (polygon.units() == CQChartsUnits::NONE) {
    if      (view())
      polygon.setUnits(CQChartsUnits::VIEW);
    else if (plot())
      polygon.setUnits(CQChartsUnits::PLOT);
  }

  if (! polygon.isValid(/*closed*/false))
    return setErrorMsg("Not enough polyline points");

  auto fill   = polylineWidgets_.backgroundDataEdit->data();
  auto stroke = polylineWidgets_.strokeDataEdit    ->data();

  shapeData.setFill  (fill);
  shapeData.setStroke(stroke);

  //---

  CQChartsPolylineAnnotation *annotation = nullptr;

  if      (view())
    annotation = view()->addPolylineAnnotation(polygon);
  else if (plot())
    annotation = plot()->addPolylineAnnotation(polygon);
  else
    return false;

  if (id != "")
    annotation->setId(id);

  annotation->setTipId(tipId);

  annotation->setBoxData(boxData);

  return true;
}

bool
CQChartsCreateAnnotationDlg::
createTextAnnotation()
{
  CQChartsBoxData boxData;

  CQChartsShapeData &shapeData = boxData.shape();

  //---

  auto id    = idEdit_ ->text();
  auto tipId = tipEdit_->text();

  auto pos  = textWidgets_.positionEdit->position();
  auto rect = textWidgets_.rectEdit->rect();
  auto text = textWidgets_.textEdit->text();

  if (! textWidgets_.positionRadio->isChecked()) {
    if (! rect.isValid())
      return setErrorMsg("Invalid text rectangle");
  }

  if (text.simplified().length() == 0)
    return setErrorMsg("Text string is empty");

  auto textData = textWidgets_.dataEdit->data();

  auto fill   = textWidgets_.backgroundDataEdit->data();
  auto stroke = textWidgets_.strokeDataEdit    ->data();

  shapeData.setFill  (fill);
  shapeData.setStroke(stroke);

  boxData.setBorderSides(textWidgets_.borderSidesEdit->sides());

  //---

  CQChartsTextAnnotation *annotation = nullptr;

  if      (view()) {
    if (textWidgets_.positionRadio->isChecked())
      annotation = view()->addTextAnnotation(pos, text);
    else
      annotation = view()->addTextAnnotation(rect, text);
  }
  else if (plot()) {
    if (textWidgets_.positionRadio->isChecked())
      annotation = plot()->addTextAnnotation(pos, text);
    else
      annotation = plot()->addTextAnnotation(rect, text);
  }
  else
    return false;

  if (id != "")
    annotation->setId(id);

  annotation->setTipId(tipId);

  annotation->setTextData(textData);
  annotation->setBoxData (boxData );

  return true;
}

bool
CQChartsCreateAnnotationDlg::
createArrowAnnotation()
{
  CQChartsShapeData shapeData;

  CQChartsStrokeData &stroke = shapeData.stroke();
  CQChartsFillData   &fill   = shapeData.fill();

  //---

  auto id    = idEdit_ ->text();
  auto tipId = tipEdit_->text();

  auto start = arrowWidgets_.startEdit->position();
  auto end   = arrowWidgets_.endEdit  ->position();

  if (start == end)
    return setErrorMsg("Arrow is zero length");

  auto arrowData = arrowWidgets_.dataEdit->data();

  stroke.setWidth(arrowWidgets_.strokeWidthEdit->length());
  stroke.setColor(arrowWidgets_.strokeColorEdit->color());

  fill.setVisible(arrowWidgets_.filledCheck->isChecked());
  fill.setColor  (arrowWidgets_.fillColorEdit->color());

  //---

  CQChartsArrowAnnotation *annotation = nullptr;

  if      (view())
    annotation = view()->addArrowAnnotation(start, end);
  else if (plot())
    annotation = plot()->addArrowAnnotation(start, end);
  else
    return false;

  if (id != "")
    annotation->setId(id);

  annotation->setTipId(tipId);

  annotation->setArrowData(arrowData);

  annotation->arrow()->setShapeData(shapeData);

  return true;
}

bool
CQChartsCreateAnnotationDlg::
createPointAnnotation()
{
  auto id    = idEdit_ ->text();
  auto tipId = tipEdit_->text();

  auto pos        = pointWidgets_.positionEdit->position();
  auto symbolData = pointWidgets_.dataEdit->data();

  //---

  CQChartsPointAnnotation *annotation = nullptr;

  if      (view())
    annotation = view()->addPointAnnotation(pos, symbolData.type());
  else if (plot())
    annotation = plot()->addPointAnnotation(pos, symbolData.type());
  else
    return false;

  if (id != "")
    annotation->setId(id);

  annotation->setTipId(tipId);

  annotation->setSymbolData(symbolData);

  return true;
}

void
CQChartsCreateAnnotationDlg::
cancelSlot()
{
  close();
}

bool
CQChartsCreateAnnotationDlg::
setErrorMsg(const QString &msg)
{
  msgLabel_->setText   (msg);
  msgLabel_->setToolTip(msg);

  return false;
}

void
CQChartsCreateAnnotationDlg::
clearErrorMsg()
{
  msgLabel_->setText   ("");
  msgLabel_->setToolTip("");
}
