#include <CQGradientControlPlot.h>
#include <QPainter>

namespace Util {
  inline double norm(double x, double low, double high) {
    return (x - low)/(high - low);
  }

  inline double lerp(double value1, double value2, double amt) {
    return value1 + (value2 - value1)*amt;
  }

  inline double map(double value, double low1, double high1, double low2, double high2) {
    return lerp(low2, high2, norm(value, low1, high1));
  }
}

//------

#ifdef CGRADIENT_EXPR
CQGradientControlPlot::
CQGradientControlPlot(QWidget *parent, CExpr *expr) :
 QFrame(parent), expr_(expr)
{
  init();
}

CQGradientControlPlot::
CQGradientControlPlot(CExpr *expr, QWidget *parent) :
 QFrame(parent), expr_(expr)
{
  init();
}
#else
CQGradientControlPlot::
CQGradientControlPlot(QWidget *parent) :
 QFrame(parent)
{
  init();
}
#endif

void
CQGradientControlPlot::
init()
{
  setObjectName("palette");

  if (! pal_) {
#ifdef CGRADIENT_EXPR
    pal_ = new CGradientPalette(expr_);
#else
    pal_ = new CGradientPalette();
#endif
  }

  pal_->addDefinedColor(0, QColor(0,0,0));
  pal_->addDefinedColor(1, QColor(255,255,255));
}

void
CQGradientControlPlot::
setGradientPalette(CGradientPalette *pal)
{
  delete pal_;

  pal_ = pal;
}

void
CQGradientControlPlot::
paintEvent(QPaintEvent *)
{
  QPainter painter(this);

  painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

  painter.fillRect(rect(), Qt::white);

  //---

  drawAxis(&painter);

  //---

  // draw graph
  QPen redPen  (Qt::red  ); redPen  .setWidth(0);
  QPen greenPen(Qt::green); greenPen.setWidth(0);
  QPen bluePen (Qt::blue ); bluePen .setWidth(0);
  QPen blackPen(Qt::black); blackPen.setWidth(0);

  QPainterPath redPath, greenPath, bluePath, blackPath;

  double px1, py1, px2, py2;

  windowToPixel(0.0, 0.0, px1, py1);
  windowToPixel(1.0, 1.0, px2, py2);

  bool   first = true;
  double r1 = 0.0, g1 = 0.0, b1 = 0.0, m1 = 0.0, x1 = 0.0;

  for (double x = px1; x <= px2; x += 1.0) {
    double wx, wy;

    pixelToWindow(x, 0, wx, wy);

    QColor c = pal_->getColor(wx);

    double x2 = wx;
    double r2 = c.redF  ();
    double g2 = c.greenF();
    double b2 = c.blueF ();
    double m2 = qGray(r2*255, g2*255, b2*255)/255.0;

    double px, py;

    if (first) {
      windowToPixel(x1, r1, px, py); redPath  .moveTo(px, py);
      windowToPixel(x1, g1, px, py); greenPath.moveTo(px, py);
      windowToPixel(x1, b1, px, py); bluePath .moveTo(px, py);
      windowToPixel(x1, m1, px, py); blackPath.moveTo(px, py);
    }

    windowToPixel(x2, r2, px, py); redPath  .lineTo(px, py);
    windowToPixel(x2, g2, px, py); greenPath.lineTo(px, py);
    windowToPixel(x2, b2, px, py); bluePath .lineTo(px, py);
    windowToPixel(x2, m2, px, py); blackPath.lineTo(px, py);

    //if (! first) {
    //  drawLine(&painter, x1, r1, x2, r2, redPen  );
    //  drawLine(&painter, x1, g1, x2, g2, greenPen);
    //  drawLine(&painter, x1, b1, x2, b2, bluePen );
    //  drawLine(&painter, x1, m1, x2, m2, blackPen);
   // }

    x1 = x2;
    r1 = r2;
    g1 = g2;
    b1 = b2;
    m1 = m2;

    first = false;
  }

  painter.strokePath(redPath  , redPen  );
  painter.strokePath(greenPath, greenPen);
  painter.strokePath(bluePath , bluePen );
  painter.strokePath(blackPath, blackPen);

  //---

  // draw color bar
  double xp1 = 1.05;
  double xp2 = 1.15;

  double pxp1, pxp2;

  windowToPixel(xp1, 0.0, pxp1, py1);
  windowToPixel(xp2, 1.0, pxp2, py2);

  for (double y = py2; y <= py1; y += 1.0) {
    double wx, wy;

    pixelToWindow(0, y, wx, wy);

    QColor c = pal_->getColor(wy);

    QPen pen(c); pen.setWidth(0);

    painter.setPen(pen);

    painter.drawLine(QPointF(pxp1, y), QPointF(pxp2, y));
  }

  painter.setPen(blackPen);

  painter.drawLine(QPointF(pxp1, py1), QPointF(pxp2, py1));
  painter.drawLine(QPointF(pxp2, py1), QPointF(pxp2, py2));
  painter.drawLine(QPointF(pxp2, py2), QPointF(pxp1, py2));
  painter.drawLine(QPointF(pxp1, py2), QPointF(pxp1, py1));

  //---

  if (pal_->colorType() == CGradientPalette::ColorType::DEFINED) {
    for (const auto &c : pal_->colors()) {
      double x  = c.first;
      QColor c1 = c.second;

      drawSymbol(&painter, x, c1.redF  (), redPen  );
      drawSymbol(&painter, x, c1.greenF(), greenPen);
      drawSymbol(&painter, x, c1.blueF (), bluePen );
    }
  }
}

void
CQGradientControlPlot::
drawAxis(QPainter *painter)
{
  QPen blackPen(Qt::black); blackPen.setWidth(0);

  double px1, py1, px2, py2;

  windowToPixel(0.0, 0.0, px1, py1);
  windowToPixel(1.0, 1.0, px2, py2);

  drawLine(painter, 0, 0, 1, 0, blackPen);
  drawLine(painter, 0, 0, 0, 1, blackPen);

  painter->setPen(blackPen);

  painter->drawLine(QPointF(px1, py1), QPointF(px1, py1 + 4));
  painter->drawLine(QPointF(px2, py1), QPointF(px2, py1 + 4));

  painter->drawLine(QPointF(px1, py1), QPointF(px1 - 4, py1));
  painter->drawLine(QPointF(px1, py2), QPointF(px1 - 4, py2));
}

void
CQGradientControlPlot::
drawLine(QPainter *painter, double x1, double y1, double x2, double y2, const QPen &pen)
{
  painter->setPen(pen);

  double px1, py1, px2, py2;

  windowToPixel(x1, y1, px1, py1);
  windowToPixel(x2, y2, px2, py2);

  painter->drawLine(QPointF(px1, py1), QPointF(px2, py2));
}

void
CQGradientControlPlot::
drawSymbol(QPainter *painter, double x, double y, const QPen &pen)
{
  painter->setPen(pen);

  double px, py;

  windowToPixel(x, y, px, py);

  painter->drawLine(QPointF(px - 4, py), QPointF(px + 4, py));
  painter->drawLine(QPointF(px, py - 4), QPointF(px, py + 4));
}

void
CQGradientControlPlot::
windowToPixel(double wx, double wy, double &px, double &py) const
{
  px = Util::map(wx, -margin_.left  , 1 + margin_.right, 0, width () - 1);
  py = Util::map(wy, -margin_.bottom, 1 + margin_.top  , height() - 1, 0);
}

void
CQGradientControlPlot::
pixelToWindow(double px, double py, double &wx, double &wy) const
{
  wx = Util::map(px, 0, width () - 1, -margin_.left  , 1 + margin_.right);
  wy = Util::map(py, height() - 1, 0, -margin_.bottom, 1 + margin_.top  );
}

QSize
CQGradientControlPlot::
sizeHint() const
{
  return QSize(600, 600);
}
