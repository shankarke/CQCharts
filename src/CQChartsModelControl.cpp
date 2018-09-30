#include <CQChartsModelControl.h>
#include <CQChartsModelList.h>
#include <CQChartsModelData.h>
#include <CQChartsTree.h>
#include <CQChartsTable.h>
#include <CQChartsUtil.h>
#include <CQCharts.h>
#include <CQUtil.h>

#include <QVBoxLayout>
#include <QTabWidget>
#include <QStackedWidget>
#include <QRadioButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>

CQChartsModelControl::
CQChartsModelControl(CQCharts *charts) :
 charts_(charts)
{
  QVBoxLayout *layout = new QVBoxLayout(this);

  //---

  QTabWidget *controlTab = CQUtil::makeWidget<QTabWidget>("controlTab");

  controlTab->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

  layout->addWidget(controlTab);

  //------

  QFrame *exprFrame = CQUtil::makeWidget<QFrame>("exprFrame");

  controlTab->addTab(exprFrame, "Expression");

  QVBoxLayout *exprFrameLayout = new QVBoxLayout(exprFrame);

  //--

  QFrame *exprModeFrame = CQUtil::makeWidget<QFrame>("exprMode");

  QHBoxLayout *exprModeLayout = new QHBoxLayout(exprModeFrame);

  exprFrameLayout->addWidget(exprModeFrame);

  exprAddRadio_    = CQUtil::makeWidget<QRadioButton>("add"   );
  exprRemoveRadio_ = CQUtil::makeWidget<QRadioButton>("remove");
  exprModifyRadio_ = CQUtil::makeWidget<QRadioButton>("modify");

  exprAddRadio_   ->setText("Add");
  exprRemoveRadio_->setText("Remove");
  exprModifyRadio_->setText("Modify");

  exprAddRadio_->setChecked(true);

  exprModeLayout->addWidget(exprAddRadio_);
  exprModeLayout->addWidget(exprRemoveRadio_);
  exprModeLayout->addWidget(exprModifyRadio_);
  exprModeLayout->addStretch(1);

  connect(exprAddRadio_   , SIGNAL(toggled(bool)), this, SLOT(expressionModeSlot()));
  connect(exprRemoveRadio_, SIGNAL(toggled(bool)), this, SLOT(expressionModeSlot()));
  connect(exprModifyRadio_, SIGNAL(toggled(bool)), this, SLOT(expressionModeSlot()));

  //--

  QGridLayout *exprGridLayout = new QGridLayout;

  exprFrameLayout->addLayout(exprGridLayout);

  int row = 0;

  //---

  exprValueLabel_ = new QLabel("Expression");
  exprValueLabel_->setObjectName("exprValueLabel");

  exprValueEdit_ = CQUtil::makeWidget<QLineEdit>("exprValueEdit");

  exprValueEdit_->setToolTip("+<expr> OR -<column> OR =<column>:<expr>\n"
                             "Use: @<number> as shorthand for column(<number>)\n"
                             "Functions: column, row, cell, setColumn, setRow, setCell\n"
                             " header, setHeader, type, setType, map, bucket, norm, key, rand");

  exprGridLayout->addWidget(exprValueLabel_, row, 0);
  exprGridLayout->addWidget(exprValueEdit_ , row, 1);

  ++row;

  //----

  exprColumnLabel_ = new QLabel("Column");
  exprColumnLabel_->setObjectName("exprEditLabel");

  exprColumnEdit_ = CQUtil::makeWidget<QLineEdit>("exprColumnEdit");

  exprColumnEdit_->setToolTip("Column to Modify");

  exprGridLayout->addWidget(exprColumnLabel_, row, 0);
  exprGridLayout->addWidget(exprColumnEdit_ , row, 1);

  ++row;

  //----

  exprNameLabel_ = new QLabel("Name");
  exprNameLabel_->setObjectName("exprNameLabel");

  exprNameEdit_ = CQUtil::makeWidget<QLineEdit>("exprNameEdit");

  exprNameEdit_->setToolTip("Column Name");

  exprGridLayout->addWidget(exprNameLabel_, row, 0);
  exprGridLayout->addWidget(exprNameEdit_ , row, 1);

  ++row;

  //--

  exprTypeLabel_ = new QLabel("Type");
  exprTypeLabel_->setObjectName("exprTypeLabel");

  exprTypeEdit_ = CQUtil::makeWidget<QLineEdit>("exprTypeEdit");

  exprTypeEdit_->setToolTip("Column Type");

  exprGridLayout->addWidget(exprTypeLabel_, row, 0);
  exprGridLayout->addWidget(exprTypeEdit_ , row, 1);

  ++row;

  //--

  exprFrameLayout->addStretch(1);

  //--

  QHBoxLayout *exprButtonLayout = new QHBoxLayout;

  exprFrameLayout->addLayout(exprButtonLayout);

  QPushButton *exprApplyButton = CQUtil::makeWidget<QPushButton>("exprApply");

  exprApplyButton->setText("Apply");

  connect(exprApplyButton, SIGNAL(clicked()), this, SLOT(exprSlot()));

  exprButtonLayout->addWidget(exprApplyButton);
  exprButtonLayout->addStretch(1);

  //------

#ifdef CQCHARTS_FOLDED_MODEL
  QFrame *foldFrame = CQUtil::makeWidget<QFrame>("foldFrame");

  controlTab->addTab(foldFrame, "Fold");

  QVBoxLayout *foldFrameLayout = new QVBoxLayout(foldFrame);

  //---

  QGridLayout *foldWidgetsLayout = new QGridLayout;

  foldFrameLayout->addLayout(foldWidgetsLayout);

  int foldRow = 0;

  foldColumnEdit_ = addLineEdit(foldWidgetsLayout, foldRow, "Column", "column");

  foldAutoCheck_ = CQUtil::makeWidget<QCheckBox>("foldAuto");

  foldAutoCheck_->setText("Auto");
  foldAutoCheck_->setChecked(true);

  foldWidgetsLayout->addWidget(foldAutoCheck_, foldRow, 0, 1, 1); ++foldRow;

  foldDeltaEdit_ = addLineEdit(foldWidgetsLayout, foldRow, "Delta", "delta");

  foldDeltaEdit_->setText("1.0");

  foldCountEdit_ = addLineEdit(foldWidgetsLayout, foldRow, "Count", "count");

  foldCountEdit_->setText("20");

  foldWidgetsLayout->setRowStretch(foldRow, 1); ++foldRow;

  //--

  QHBoxLayout *foldButtonLayout = new QHBoxLayout;

  foldFrameLayout->addLayout(foldButtonLayout);

  QPushButton *foldApplyButton = CQUtil::makeWidget<QPushButton>("foldApply");

  foldApplyButton->setText("Apply");

  connect(foldApplyButton, SIGNAL(clicked()), this, SLOT(foldApplySlot()));

  QPushButton *foldClearButton = CQUtil::makeWidget<QPushButton>("foldClear");

  foldClearButton->setText("Clear");

  connect(foldClearButton, SIGNAL(clicked()), this, SLOT(foldClearSlot()));

  foldButtonLayout->addWidget(foldApplyButton);
  foldButtonLayout->addWidget(foldClearButton);
  foldButtonLayout->addStretch(1);
#endif

  //------

  QFrame *columnDataFrame = CQUtil::makeWidget<QFrame>("columnDataFrame");

  controlTab->addTab(columnDataFrame, "Column Data");

  QGridLayout *columnDataFrameLayout = new QGridLayout(columnDataFrame);

  int columnDataRow = 0;

  columnNumEdit_  = addLineEdit(columnDataFrameLayout, columnDataRow, "Number", "number");
  columnNameEdit_ = addLineEdit(columnDataFrameLayout, columnDataRow, "Name"  , "name"  );
  columnTypeEdit_ = addLineEdit(columnDataFrameLayout, columnDataRow, "Type"  , "type"  );

  columnDataFrameLayout->setRowStretch(columnDataRow, 1); ++columnDataRow;

  QPushButton *typeSetButton = CQUtil::makeWidget<QPushButton>("typeSet");

  typeSetButton->setText("Apply");

  connect(typeSetButton, SIGNAL(clicked()), this, SLOT(typeSetSlot()));

  columnDataFrameLayout->addWidget(typeSetButton, columnDataRow, 0);

  //---

  expressionModeSlot();
}

QLineEdit *
CQChartsModelControl::
addLineEdit(QGridLayout *grid, int &row, const QString &name, const QString &objName) const
{
  QLabel    *label = CQUtil::makeWidget<QLabel   >(objName + "Label");
  QLineEdit *edit  = CQUtil::makeWidget<QLineEdit>(objName + "Edit" );

  label->setText(name);

  grid->addWidget(label, row, 0);
  grid->addWidget(edit , row, 1);

  ++row;

  return edit;
}

void
CQChartsModelControl::
expressionModeSlot()
{
  mode_ = Mode::ADD;

  if      (exprAddRadio_   ->isChecked()) mode_ = Mode::ADD;
  else if (exprRemoveRadio_->isChecked()) mode_ = Mode::REMOVE;
  else if (exprModifyRadio_->isChecked()) mode_ = Mode::MODIFY;

  exprColumnLabel_->setEnabled(mode_ == Mode::MODIFY);
  exprColumnEdit_ ->setEnabled(mode_ == Mode::MODIFY);

  exprTypeLabel_->setEnabled(mode_ != Mode::REMOVE);
  exprNameEdit_ ->setEnabled(mode_ != Mode::REMOVE);
  exprTypeEdit_ ->setEnabled(mode_ != Mode::REMOVE);
}

void
CQChartsModelControl::
exprSlot()
{
  QString expr = exprValueEdit_->text().simplified();

  //---

  CQChartsModelData *modelData = charts_->currentModelData();

  if (! modelData) {
    charts_->errorMsg("No model data");
    return;
  }

  CQChartsExprModel::Function function { CQChartsExprModel::Function::EVAL };

  switch (mode_) {
    case Mode::ADD   : function = CQChartsExprModel::Function::ADD   ; break;
    case Mode::REMOVE: function = CQChartsExprModel::Function::DELETE; break;
    case Mode::MODIFY: function = CQChartsExprModel::Function::ASSIGN; break;
    default:                                                           break;
  }

  ModelP model = modelData->currentModel();

  QString columnStr = exprColumnEdit_->text();

  CQChartsColumn column;

  if (! CQChartsUtil::stringToColumn(model.data(), columnStr, column)) {
    bool ok;

    int icolumn = columnStr.toInt(&ok);

    if (ok)
      column = CQChartsColumn(icolumn);
  }

  int column1 = CQChartsUtil::processExpression(model.data(), function, column, expr);

  if (function == CQChartsExprModel::Function::ADD ||
      function == CQChartsExprModel::Function::ASSIGN) {
    QString nameStr = exprNameEdit_->text();
    QString typeStr = exprTypeEdit_->text();

    if (column1 < 0) {
      charts_->errorMsg("Invalid column");
      return;
    }

    if (nameStr.length())
      model->setHeaderData(column1, Qt::Horizontal, nameStr, Qt::DisplayRole);

    if (typeStr.length()) {
      if (! CQChartsUtil::setColumnTypeStr(charts_, model.data(), column1, typeStr)) {
        charts_->errorMsg("Invalid type '" + typeStr + "'");
        return;
      }
    }
  }
}

void
CQChartsModelControl::
foldApplySlot()
{
#ifdef CQCHARTS_FOLDED_MODEL
  CQChartsModelData *modelData = charts_->currentModelData();

  if (! modelData)
    return;

  CQChartsModelData::FoldData foldData;

  foldData.columnsStr = foldColumnEdit_->text();
  foldData.isAuto     = foldAutoCheck_->isChecked();
  foldData.delta      = foldDeltaEdit_->text().toDouble();
  foldData.count      = foldCountEdit_->text().toInt();

  modelData->foldModel(foldData);

  updateModel(modelData);

  updateModelDetails(modelData);
#endif
}

void
CQChartsModelControl::
foldClearSlot()
{
#ifdef CQCHARTS_FOLDED_MODEL
  CQChartsModelData *modelData = charts_->currentModelData();

  if (! modelData)
    return;

  modelData->foldClear();

  updateModel(modelData);

  updateModelDetails(modelData);
#endif
}

void
CQChartsModelControl::
updateModel(CQChartsModelData *modelData)
{
  if (modelList_)
    modelList_->updateModel(modelData);
}

void
CQChartsModelControl::
updateModelDetails(const CQChartsModelData *modelData)
{
  if (modelList_)
    modelList_->setDetails(modelData);
}

void
CQChartsModelControl::
typeSetSlot()
{
  CQChartsModelData *modelData = charts_->currentModelData();

  if (! modelData)
    return;

  ModelP model = modelData->currentModel();

  //---

  QString numStr = columnNumEdit_->text();

  bool ok;

  int column = numStr.toInt(&ok);

  if (! ok) {
    charts_->errorMsg("Invalid column number '" + numStr + "'");
    return;
  }

  //--

  QString nameStr = columnNameEdit_->text();

  if (nameStr.length())
    model->setHeaderData(column, Qt::Horizontal, nameStr, Qt::DisplayRole);

  //---

  QString typeStr = columnTypeEdit_->text();

  if (! CQChartsUtil::setColumnTypeStr(charts_, model.data(), column, typeStr)) {
    charts_->errorMsg("Invalid type '" + typeStr + "'");
    return;
  }

  //---

  if (modelList_)
    modelList_->redrawView(modelData);
}

void
CQChartsModelControl::
setColumnData(int num, const QString &headerStr, const QString &typeStr)
{
  columnNumEdit_->setText(QString("%1").arg(num));

  columnNameEdit_->setText(headerStr);

  columnTypeEdit_->setText(typeStr);
}
