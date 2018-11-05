#include <CQChartsViewToolBar.h>
#include <CQChartsWindow.h>
#include <CQChartsView.h>
#include <CQChartsPlot.h>
#include <CQChartsModelDlg.h>
#include <CQChartsPlotDlg.h>
#include <CQChartsModelData.h>
#include <CQPixmapCache.h>
#include <CQIconCombo.h>
#include <CQUtil.h>

#include <svg/select_light_svg.h>
#include <svg/select_dark_svg.h>
#include <svg/zoom_light_svg.h>
#include <svg/zoom_dark_svg.h>
#include <svg/pan_light_svg.h>
#include <svg/pan_dark_svg.h>
#include <svg/probe_light_svg.h>
#include <svg/probe_dark_svg.h>
#include <svg/edit_light_svg.h>
#include <svg/edit_dark_svg.h>
#include <svg/zoom_fit_light_svg.h>
#include <svg/zoom_fit_dark_svg.h>
#include <svg/left_light_svg.h>
#include <svg/left_dark_svg.h>
#include <svg/right_light_svg.h>
#include <svg/right_dark_svg.h>
#include <svg/charts_light_svg.h>
#include <svg/charts_dark_svg.h>
#include <svg/models_light_svg.h>
#include <svg/models_dark_svg.h>

#include <QStackedWidget>
#include <QToolButton>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>
#include <QHBoxLayout>

CQChartsViewToolBar::
CQChartsViewToolBar(CQChartsWindow *window) :
 QFrame(window), window_(window)
{
  setObjectName("toolbar");

  setAutoFillBackground(true);

  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

  //---

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setMargin(0); layout->setSpacing(2);

  //---

  auto createButton = [&](const QString &name, const QString &iconName, const QString &tip,
                          const char *receiver, bool checkable=true) -> QToolButton * {
    QToolButton *button = CQUtil::makeWidget<QToolButton>(name);

    button->setIcon(CQPixmapCacheInst->getIcon(iconName + "_LIGHT", iconName + "_DARK"));
    button->setCheckable(checkable);

    button->setFocusPolicy(Qt::NoFocus);

    connect(button, SIGNAL(clicked(bool)), this, receiver);

    button->setToolTip(tip);

    button->setFocusPolicy(Qt::NoFocus);

    return button;
  };

  //---

  modeCombo_ = new CQIconCombo;

  modeCombo_->setObjectName("modeCombo");

  modeCombo_->addItem(CQPixmapCacheInst->getIcon("SELECT_LIGHT", "SELECT_DARK"), "Select");
  modeCombo_->addItem(CQPixmapCacheInst->getIcon("ZOOM_LIGHT"  , "ZOOM_DARK"  ), "Zoom"  );
  modeCombo_->addItem(CQPixmapCacheInst->getIcon("PAN_LIGHT"   , "PAN_DARK"   ), "Pan"   );
  modeCombo_->addItem(CQPixmapCacheInst->getIcon("PROBE_LIGHT" , "PROBE_DARK" ), "Probe" );
  modeCombo_->addItem(CQPixmapCacheInst->getIcon("EDIT_LIGHT"  , "EDIT_DARK"  ), "Edit"  );

  modeCombo_->setFocusPolicy(Qt::NoFocus);

  connect(modeCombo_, SIGNAL(currentIndexChanged(int)), this, SLOT(modeSlot(int)));

  layout->addWidget(modeCombo_);

  //---

  controlsStack_ = CQUtil::makeWidget<QStackedWidget>("controls");

  controlsStack_->layout()->setMargin(0); controlsStack_->layout()->setSpacing(0);

  layout->addWidget(controlsStack_);

  QFrame *selectControls = CQUtil::makeWidget<QFrame>("select");
  QFrame *zoomControls   = CQUtil::makeWidget<QFrame>("zoom");
  QFrame *panControls    = CQUtil::makeWidget<QFrame>("pan");
  QFrame *probeControls  = CQUtil::makeWidget<QFrame>("probe");
  QFrame *editControls   = CQUtil::makeWidget<QFrame>("edit");

  controlsStack_->addWidget(selectControls);
  controlsStack_->addWidget(zoomControls);
  controlsStack_->addWidget(panControls);
  controlsStack_->addWidget(probeControls);
  controlsStack_->addWidget(editControls);

  layout->addStretch(1);

  //---

  QHBoxLayout *selectControlsLayout = new QHBoxLayout(selectControls);
  selectControlsLayout->setMargin(0); selectControlsLayout->setSpacing(2);

  QButtonGroup *selectButtonGroup = new QButtonGroup(this);

  selectPointButton_ = new QRadioButton("Point");
  selectRectButton_  = new QRadioButton("Rect");

  selectPointButton_->setFocusPolicy(Qt::NoFocus);
  selectRectButton_ ->setFocusPolicy(Qt::NoFocus);

  selectPointButton_->setChecked(true);

  selectButtonGroup->addButton(selectPointButton_, 0);
  selectButtonGroup->addButton(selectRectButton_ , 1);

  selectControlsLayout->addWidget(selectPointButton_);
  selectControlsLayout->addWidget(selectRectButton_ );

  connect(selectButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(selectButtonClicked(int)));

  //---

  QHBoxLayout *zoomControlsLayout = new QHBoxLayout(zoomControls);
  zoomControlsLayout->setMargin(0); zoomControlsLayout->setSpacing(2);

  QPushButton *zoomButton = CQUtil::makeWidget<QPushButton>("reset");

  zoomButton->setText("Reset");
  zoomButton->setFocusPolicy(Qt::NoFocus);

  connect(zoomButton, SIGNAL(clicked()), this, SLOT(zoomFullSlot()));

  zoomControlsLayout->addWidget(zoomButton);

  //---

  QHBoxLayout *panControlsLayout = new QHBoxLayout(panControls);
  panControlsLayout->setMargin(0); panControlsLayout->setSpacing(2);

  QPushButton *panButton = CQUtil::makeWidget<QPushButton>("reset");

  panButton->setText("Reset");
  panButton->setFocusPolicy(Qt::NoFocus);

  connect(panButton, SIGNAL(clicked()), this, SLOT(panResetSlot()));

  panControlsLayout->addWidget(panButton);

  //---

  modelDlgButton_ = createButton("moldeDel", "MODELS", "Manage Models",
                                 SLOT(manageModelsSlot()), false);
  plotDlgButton_  = createButton("plotDlg" , "CHARTS", "Add Plot",
                                 SLOT(addPlotSlot()), false);

  autoFitButton_ = createButton("fit"  , "ZOOM_FIT", "Zoom Fit"    , SLOT(autoFitSlot()), false);
  leftButton_    = createButton("left" , "LEFT"    , "Scroll Left" , SLOT(leftSlot()));
  rightButton_   = createButton("right", "RIGHT"   , "Scroll Right", SLOT(rightSlot()));

  layout->addWidget(modelDlgButton_);
  layout->addWidget(plotDlgButton_);
  layout->addWidget(autoFitButton_);
  layout->addWidget(leftButton_);
  layout->addWidget(rightButton_);
}

void
CQChartsViewToolBar::
modeSlot(int ind)
{
  if      (ind == 0)
    window_->view()->setMode(CQChartsView::Mode::SELECT);
  else if (ind == 1)
    window_->view()->setMode(CQChartsView::Mode::ZOOM);
  else if (ind == 2)
    window_->view()->setMode(CQChartsView::Mode::PAN);
  else if (ind == 3)
    window_->view()->setMode(CQChartsView::Mode::PROBE);
  else if (ind == 4)
    window_->view()->setMode(CQChartsView::Mode::EDIT);

  updateMode();
}

void
CQChartsViewToolBar::
selectButtonClicked(int ind)
{
  if (ind == 0)
    window_->view()->setSelectMode(CQChartsView::SelectMode::POINT);
  else
    window_->view()->setSelectMode(CQChartsView::SelectMode::RECT);
}

void
CQChartsViewToolBar::
zoomFullSlot()
{
  CQChartsPlot *plot = window_->view()->currentPlot(/*remap*/true);

  if (plot)
    plot->zoomFull();
}

void
CQChartsViewToolBar::
panResetSlot()
{
}

void
CQChartsViewToolBar::
updateMode()
{
  if      (window_->view()->mode() == CQChartsView::Mode::SELECT) {
    modeCombo_    ->setCurrentIndex(0);
    controlsStack_->setCurrentIndex(0);

    if (window_->view()->selectMode() == CQChartsView::SelectMode::POINT)
      selectPointButton_->setChecked(true);
    else
      selectRectButton_->setChecked(true);
  }
  else if (window_->view()->mode() == CQChartsView::Mode::ZOOM) {
    modeCombo_    ->setCurrentIndex(1);
    controlsStack_->setCurrentIndex(1);
  }
  else if (window_->view()->mode() == CQChartsView::Mode::PAN) {
    modeCombo_    ->setCurrentIndex(2);
    controlsStack_->setCurrentIndex(2);
  }
  else if (window_->view()->mode() == CQChartsView::Mode::PROBE) {
    modeCombo_    ->setCurrentIndex(3);
    controlsStack_->setCurrentIndex(3);
  }
  else if (window_->view()->mode() == CQChartsView::Mode::EDIT) {
    modeCombo_    ->setCurrentIndex(4);
    controlsStack_->setCurrentIndex(4);
  }
}

void
CQChartsViewToolBar::
manageModelsSlot()
{
  CQCharts *charts = window_->view()->charts();

  if (modelDlg_)
    delete modelDlg_;

  modelDlg_ = new CQChartsModelDlg(charts);

  modelDlg_->show();
}

void
CQChartsViewToolBar::
addPlotSlot()
{
  CQCharts *charts = window_->view()->charts();

  CQChartsModelData *modelData = charts->currentModelData();

  if (! modelData)
    return;

  if (plotDlg_)
    delete plotDlg_;

  plotDlg_ = new CQChartsPlotDlg(charts, modelData);

  plotDlg_->setViewName(window_->view()->id());

  plotDlg_->show();
}

void
CQChartsViewToolBar::
autoFitSlot()
{
  window_->view()->fitSlot();
}

void
CQChartsViewToolBar::
leftSlot()
{
  window_->view()->scrollLeft();
}

void
CQChartsViewToolBar::
rightSlot()
{
  window_->view()->scrollRight();
}

QSize
CQChartsViewToolBar::
sizeHint() const
{
  int w = autoFitButton_->sizeHint().width ();
  int h = autoFitButton_->sizeHint().height();

  return QSize(4*w + 6, h);
}
