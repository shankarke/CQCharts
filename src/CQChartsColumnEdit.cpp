#include <CQChartsColumnEdit.h>
#include <CQChartsColumnCombo.h>
#include <CQChartsLineEdit.h>
#include <CQChartsPlot.h>
#include <CQChartsModelUtil.h>
#include <CQChartsVariant.h>
#include <CQChartsWidgetUtil.h>

#include <CQPropertyView.h>
#include <CQWidgetMenu.h>

#include <QComboBox>
#include <QLabel>
#include <QGridLayout>
#include <QAbstractItemModel>
#include <QPainter>

CQChartsColumnLineEdit::
CQChartsColumnLineEdit(QWidget *parent) :
 CQChartsLineEditBase(parent)
{
  setObjectName("columnLineEdit");

  setToolTip("Optional Column Number or Name (use empty string to unset)");

  //---

  menuEdit_ = dataEdit_ = new CQChartsColumnEdit;

  menu_->setWidget(dataEdit_);

  //---

  connectSlots(true);
}

void
CQChartsColumnLineEdit::
setPlot(CQChartsPlot *plot)
{
  CQChartsLineEditBase::setPlot(plot);

  if (plot)
    setModel(plot->model().data());
}

QAbstractItemModel *
CQChartsColumnLineEdit::
model() const
{
  return dataEdit_->model();
}

void
CQChartsColumnLineEdit::
setModel(QAbstractItemModel *model)
{
  dataEdit_->setModel(model);
}

const CQChartsColumn &
CQChartsColumnLineEdit::
column() const
{
  return dataEdit_->column();
}

void
CQChartsColumnLineEdit::
setColumn(const CQChartsColumn &column)
{
  updateColumn(column, /*updateText*/true);
}

void
CQChartsColumnLineEdit::
updateColumn(const CQChartsColumn &column, bool updateText)
{
  connectSlots(false);

  dataEdit_->setColumn(column);

  connectSlots(true);

  if (updateText)
    columnToWidgets();

  emit columnChanged();
}

void
CQChartsColumnLineEdit::
textChanged()
{
  CQChartsColumn column;

  QString text = edit_->text();

  if (text.simplified() == "") {
    column = CQChartsColumn();
  }
  else {
    if (model()) {
      if (! CQChartsModelUtil::stringToColumn(model(), text, column))
        return;
    }
    else {
      column = CQChartsColumn(text);
    }
  }

  updateColumn(column, /*updateText*/ false);
}

void
CQChartsColumnLineEdit::
columnToWidgets()
{
  connectSlots(false);

  if (column().isValid())
    edit_->setText(column().toString());
  else
    edit_->setText("");

  setToolTip(column().toString());

  connectSlots(true);
}

void
CQChartsColumnLineEdit::
menuEditChanged()
{
  columnToWidgets();

  emit columnChanged();
}

void
CQChartsColumnLineEdit::
connectSlots(bool b)
{
  connectBaseSlots(b);

  CQChartsWidgetUtil::connectDisconnect(b,
    dataEdit_, SIGNAL(columnChanged()), this, SLOT(menuEditChanged()));
}

void
CQChartsColumnLineEdit::
drawPreview(QPainter *painter, const QRect &)
{
  QString str = (column().isValid() ? column().toString() : "<none>");

  drawCenteredText(painter, str);
}

//------

#include <CQPropertyViewItem.h>
#include <CQPropertyViewDelegate.h>

CQChartsColumnPropertyViewType::
CQChartsColumnPropertyViewType()
{
}

CQPropertyViewEditorFactory *
CQChartsColumnPropertyViewType::
getEditor() const
{
  return new CQChartsColumnPropertyViewEditor;
}

bool
CQChartsColumnPropertyViewType::
setEditorData(CQPropertyViewItem *item, const QVariant &value)
{
  return item->setData(value);
}

void
CQChartsColumnPropertyViewType::
draw(CQPropertyViewItem *item, const CQPropertyViewDelegate *delegate, QPainter *painter,
     const QStyleOptionViewItem &option, const QModelIndex &ind,
     const QVariant &value, bool inside)
{
  delegate->drawBackground(painter, option, ind, inside);

  //---

  bool ok;

  QString str = valueString(item, value, ok);

  QFont font = option.font;

  if (! ok)
    font.setItalic(true);

  //---

  QFontMetrics fm(font);

  int w = fm.width(str);

  QStyleOptionViewItem option1 = option;

  option1.rect.setRight(option1.rect.left() + w + 8);

  option1.font = font;

  delegate->drawString(painter, option1, str, ind, inside);
}

QString
CQChartsColumnPropertyViewType::
tip(const QVariant &value) const
{
  bool ok;

  QString str = valueString(nullptr, value, ok);

  return str;
}

QString
CQChartsColumnPropertyViewType::
valueString(CQPropertyViewItem *item, const QVariant &value, bool &ok) const
{
  CQChartsColumn column = value.value<CQChartsColumn>();

  QString str;

  if (column.isValid()) {
    str = column.toString();

    auto *plot = (item ? qobject_cast<CQChartsPlot *>(item->object()) : nullptr);

    if (plot) {
      QString str1 = plot->columnHeaderName(column);

      if (str1.length())
        str += " (" + str1 + ")";
    }

    ok  = true;
  }
  else {
    str = "<none>";
    ok  = false;
  }

  return str;
}

//------

CQChartsColumnPropertyViewEditor::
CQChartsColumnPropertyViewEditor()
{
}

QWidget *
CQChartsColumnPropertyViewEditor::
createEdit(QWidget *parent)
{
  auto *item = CQPropertyViewMgrInst->editItem();

  auto *obj = (item ? item->object() : nullptr);

  auto *plot = qobject_cast<CQChartsPlot *>(obj);

  //---

  auto *edit = new CQChartsColumnLineEdit(parent);

  if (plot)
    edit->setModel(plot->model().data());

  return edit;
}

void
CQChartsColumnPropertyViewEditor::
connect(QWidget *w, QObject *obj, const char *method)
{
  auto *edit = qobject_cast<CQChartsColumnLineEdit *>(w);
  assert(edit);

  QObject::connect(edit, SIGNAL(columnChanged()), obj, method);
}

QVariant
CQChartsColumnPropertyViewEditor::
getValue(QWidget *w)
{
  auto *edit = qobject_cast<CQChartsColumnLineEdit *>(w);
  assert(edit);

  return QVariant::fromValue(edit->column());
}

void
CQChartsColumnPropertyViewEditor::
setValue(QWidget *w, const QVariant &var)
{
  auto *edit = qobject_cast<CQChartsColumnLineEdit *>(w);
  assert(edit);

  CQChartsColumn column = var.value<CQChartsColumn>();

  edit->setColumn(column);
}

//------

CQChartsColumnEdit::
CQChartsColumnEdit(QWidget *parent) :
 CQChartsEditBase(parent)
{
  setObjectName("columnEdit");

  auto *layout = CQUtil::makeLayout<QGridLayout>(this, 2, 2);

  //---

  int row = 0;

  auto addLabelWidget = [&](const QString &id, const QString &label, QWidget *edit) {
    auto *labelW = CQUtil::makeLabelWidget<QLabel>(label, id + "Label");

    layout->addWidget(labelW, row, 0);
    layout->addWidget(edit  , row, 1);

    widgetLabels_[edit] = labelW;

    ++row;
  };

  auto createLabelCombo = [&](const QString &id, const QString &label, const QString &tipStr) {
    auto *combo = CQUtil::makeWidget<QComboBox>(id + "Combo");

    combo->setToolTip(tipStr);

    addLabelWidget(id, label, combo);

    return combo;
  };

  auto createLabelEdit = [&](const QString &id, const QString &label, const QString &tipStr) {
    auto *edit = CQUtil::makeWidget<CQChartsLineEdit>(id + "Edit");

    edit->setToolTip(tipStr);

    addLabelWidget(id, label, edit);

    return edit;
  };

  auto createColumnEdit = [&](const QString &id, const QString &label, const QString &tipStr) {
    auto *combo = CQUtil::makeWidget<CQChartsColumnCombo>(id + "Combo");

    combo->setToolTip(tipStr);

    addLabelWidget(id, label, combo);

    return combo;
  };

  //---

  // column custom name
  nameEdit_ = createLabelEdit("name", "Name", "Column Custom Name");

  //---

  // type combo
  typeCombo_ = createLabelCombo("type", "Type", "Column Type");

  typeCombo_->addItems(QStringList() <<
    "Column" << "Expression" << "Row Number" << "Vertical Header" << "Group");

  //---

  // column name/number
  columnCombo_ = createColumnEdit("column", "Column", "Column Name or Number");

  // column role
  roleEdit_ = createLabelEdit("role", "Role", "Column Role");

  // column index
  indexEdit_ = createLabelEdit("index", "Index", "Column Index");

  //---

  // column expression
  expressionEdit_ = createLabelEdit("expr", "Expr", "Column Expression");

  //---

  layout->setRowStretch(row, 1);

  layout->setColumnStretch(1, 1);

  //---

  connectSlots(true);

  widgetHeight_ = CQChartsColumnEdit::minimumSizeHint().height();

  setFixedHeight(widgetHeight_);

  updateState();
}

void
CQChartsColumnEdit::
setModel(QAbstractItemModel *model)
{
  model_ = model;

  updateColumnsFromModel();

  columnToWidgets();

  updateState();
}

const CQChartsColumn &
CQChartsColumnEdit::
column() const
{
  return column_;
}

void
CQChartsColumnEdit::
setColumn(const CQChartsColumn &column)
{
  column_ = column;

  columnToWidgets();

  updateState();

  emit columnChanged();
}

void
CQChartsColumnEdit::
columnToWidgets()
{
  connectSlots(false);

  nameEdit_      ->setText("");
  columnCombo_   ->setCurrentIndex(0);
  roleEdit_      ->setText("");
  indexEdit_     ->setText("");
  expressionEdit_->setText("");

  if (column_.isValid()) {
    nameEdit_->setText(column_.name());

    if      (column_.type() == CQChartsColumn::Type::DATA ||
             column_.type() == CQChartsColumn::Type::DATA_INDEX) {
      typeCombo_->setCurrentIndex(0);

      columnCombo_->setColumn(column_);

      if (column_.role() >= 0)
        roleEdit_->setText(QString("%1").arg(column_.role()));

      if (column_.type() == CQChartsColumn::Type::DATA_INDEX)
        indexEdit_->setText(column_.expr());
    }
    else if (column_.type() == CQChartsColumn::Type::EXPR) {
      typeCombo_->setCurrentIndex(1);

      expressionEdit_->setText(column_.expr());
    }
    else if (column_.type() == CQChartsColumn::Type::ROW) {
      typeCombo_->setCurrentIndex(2);
    }
    else if (column_.type() == CQChartsColumn::Type::VHEADER) {
      typeCombo_->setCurrentIndex(3);
    }
    else if (column_.type() == CQChartsColumn::Type::GROUP) {
      typeCombo_->setCurrentIndex(4);
    }
  }
  else {
    typeCombo_->setCurrentIndex(0);

    columnCombo_->setColumn(column_);

    expressionEdit_->setText("");
  }

  connectSlots(true);
}

void
CQChartsColumnEdit::
widgetsToColumn()
{
  CQChartsColumn column;

  if      (typeCombo_->currentIndex() == 0) {
    CQChartsColumn icolumn = columnCombo_->getColumn();

    bool ok;

    long role = CQChartsUtil::toInt(roleEdit_->text(), ok);

    if (! ok)
      role = -1;

    if (indexEdit_->text().simplified().length())
      column = CQChartsColumn(CQChartsColumn::Type::DATA_INDEX, icolumn.column(),
                              indexEdit_->text().simplified(), int(role));
    else
      column = CQChartsColumn(CQChartsColumn::Type::DATA, icolumn.column(),
                              "", int(role));
  }
  else if (typeCombo_->currentIndex() == 1) {
    QString str;

    if (expressionEdit_->text().simplified().length())
      str = QString("%1").arg(expressionEdit_->text());

    column = CQChartsColumn(CQChartsColumn::Type::EXPR, -1, str, -1);
  }
  else if (typeCombo_->currentIndex() == 2) {
    column = CQChartsColumn(CQChartsColumn::Type::ROW, -1, "", -1);
  }
  else if (typeCombo_->currentIndex() == 3) {
    column = CQChartsColumn(CQChartsColumn::Type::VHEADER, -1, "", -1);
  }
  else if (typeCombo_->currentIndex() == 4) {
    column = CQChartsColumn(CQChartsColumn::Type::GROUP, -1, "", -1);
  }

  if (nameEdit_->text().simplified().length())
    column.setName(nameEdit_->text().simplified());

  column_ = column;

  //---

  updateState();

  emit columnChanged();
}

void
CQChartsColumnEdit::
connectSlots(bool b)
{
  assert(b != connected_);

  connected_ = b;

  //---

  auto connectDisconnect = [&](bool b, QWidget *w, const char *from, const char *to) {
    CQChartsWidgetUtil::connectDisconnect(b, w, from, this, to);
  };

  connectDisconnect(b, typeCombo_, SIGNAL(currentIndexChanged(int)),
                    SLOT(widgetsToColumnSlot()));

  connectDisconnect(b, nameEdit_, SIGNAL(textChanged(const QString &)),
                    SLOT(widgetsToColumnSlot()));

  connectDisconnect(b, columnCombo_, SIGNAL(columnChanged()),
                    SLOT(widgetsToColumnSlot()));
  connectDisconnect(b, roleEdit_, SIGNAL(textChanged(const QString &)),
                    SLOT(widgetsToColumnSlot()));
  connectDisconnect(b, indexEdit_, SIGNAL(textChanged(const QString &)),
                    SLOT(widgetsToColumnSlot()));

  connectDisconnect(b, expressionEdit_, SIGNAL(textChanged(const QString &)),
                    SLOT(widgetsToColumnSlot()));
}

void
CQChartsColumnEdit::
widgetsToColumnSlot()
{
  connectSlots(false);

  widgetsToColumn();

  updateState();

  connectSlots(true);

  emit columnChanged();
}

void
CQChartsColumnEdit::
updateColumnsFromModel()
{
  columnCombo_->setModel(model());
}

void
CQChartsColumnEdit::
updateState()
{
  auto setEditVisible = [&](QWidget *w, bool visible) {
    w->setVisible(visible);

    widgetLabels_[w]->setVisible(visible);
  };

  setEditVisible(columnCombo_   , typeCombo_->currentIndex() == 0);
  setEditVisible(roleEdit_      , typeCombo_->currentIndex() == 0);
  setEditVisible(indexEdit_     , typeCombo_->currentIndex() == 0);
  setEditVisible(expressionEdit_, typeCombo_->currentIndex() == 1);
}

QSize
CQChartsColumnEdit::
sizeHint() const
{
  int w = width();

  return QSize(w, widgetHeight_);
}
