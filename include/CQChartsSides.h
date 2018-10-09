#ifndef CQChartsSide_H
#define CQChartsSide_H

#include <QString>
#include <iostream>

class CQChartsSides {
 public:
  enum class Side {
    NONE   = 0,
    LEFT   = (1<<0),
    RIGHT  = (1<<1),
    TOP    = (1<<2),
    BOTTOM = (1<<3)
  };

  using Sides = unsigned int;

 public:
  static void registerMetaType();

 public:
  CQChartsSides() { }

  CQChartsSides(const QString &s) {
    setValue(s);
  }

  CQChartsSides(const CQChartsSides &rhs) :
    sides_(rhs.sides_) {
  }

  CQChartsSides &operator=(const CQChartsSides &rhs) {
    sides_ = rhs.sides_;

    return *this;
  }

  const Sides &sides() const { return sides_; }

  bool setValue(const QString &str) {
    Sides sides;

    if (! decodeString(str, sides))
      return false;

    sides_ = sides;

    return true;
  }

  //---

  bool isLeft  () const { return (sides_ & int(Side::LEFT  )); }
  bool isRight () const { return (sides_ & int(Side::RIGHT )); }
  bool isTop   () const { return (sides_ & int(Side::TOP   )); }
  bool isBottom() const { return (sides_ & int(Side::BOTTOM)); }

  //---

  QString toString() const {
    QString str;

    if (isLeft  ()) str += "l";
    if (isRight ()) str += "r";
    if (isTop   ()) str += "t";
    if (isBottom()) str += "b";

    return str;
  }

  void fromString(const QString &s) {
    setValue(s);
  }

  //---

  friend bool operator==(const CQChartsSides &lhs, const CQChartsSides &rhs) {
    if (lhs.sides_ != rhs.sides_) return false;

    return true;
  }

  friend bool operator!=(const CQChartsSides &lhs, const CQChartsSides &rhs) {
    return ! operator==(lhs, rhs);
  }

  //---

  void print(std::ostream &os) const {
    os << toString().toStdString();
  }

  friend std::ostream &operator<<(std::ostream &os, const CQChartsSides &l) {
    l.print(os);

    return os;
  }

  //---

 private:
  bool decodeString(const QString &str, Sides &sides);

 private:
  Sides sides_ { 0 };
};

//---

#include <CQUtilMeta.h>

CQUTIL_DCL_META_TYPE(CQChartsSides)

#endif