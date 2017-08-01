#ifndef CQChartsColumn_H
#define CQChartsColumn_H

#include <CQChartsUtil.h>
#include <QString>
#include <CStrUtil.h>

typedef std::map<QString,QString> CQChartsNameValues;

class CQChartsColumnType {
 public:
  CQChartsColumnType(const QString &name) :
   name_(name) {
  }

  const QString &name() const { return name_; }

  virtual ~CQChartsColumnType() { }

  virtual QString userData(const QString &data, const CQChartsNameValues &nameValues) const = 0;

  virtual QString dataName(double pos, const CQChartsNameValues &nameValues) const = 0;

 private:
  QString name_;
};

//---

class CQChartsColumnStringType : public CQChartsColumnType {
 public:
  CQChartsColumnStringType() :
   CQChartsColumnType("string") {
  }

  QString userData(const QString &data, const CQChartsNameValues &) const override {
    return data;
  }

  QString dataName(double pos, const CQChartsNameValues &) const override {
    return CStrUtil::toString(pos).c_str();
  }
};

//---

class CQChartsColumnRealType : public CQChartsColumnType {
 public:
  CQChartsColumnRealType() :
   CQChartsColumnType("real") {
  }

  QString userData(const QString &data, const CQChartsNameValues &) const override {
    return data;
  }

  QString dataName(double pos, const CQChartsNameValues &) const override {
    return CStrUtil::toString(pos).c_str();
  }
};

//---

class CQChartsColumnTimeType : public CQChartsColumnType {
 public:
  CQChartsColumnTimeType() :
   CQChartsColumnType("time") {
  }

  QString userData(const QString &data, const CQChartsNameValues &nameValues) const override {
    auto p = nameValues.find("format");

    if (p == nameValues.end())
      return data;

    double t;

    if (! stringToTime((*p).second, data, t))
      return data;

    return QString("%1").arg(t);
  }

  QString dataName(double pos, const CQChartsNameValues &nameValues) const override {
    auto p = nameValues.find("format");

    if (p == nameValues.end())
      return CStrUtil::toString(pos).c_str();

    return timeToString((*p).second, pos);
  }

 private:
  static QString timeToString(const QString &fmt, double r) {
    static char buffer[512];

    time_t t(r);

    struct tm *tm1 = localtime(&t);

    (void) strftime(buffer, 512, fmt.toLatin1().constData(), tm1);

    return buffer;
  }

  static bool stringToTime(const QString &fmt, const QString &str, double &t) {
    struct tm tm1; memset(&tm1, 0, sizeof(tm));

    char *p = strptime(str.toLatin1().constData(), fmt.toLatin1().constData(), &tm1);

    if (! p)
      return false;

    t = mktime(&tm1);

    return true;
  }

#if 0
  QString calcTimeFmt() const {
    if (timeFmt_ == "")
      return "%d/%m/%y,%H:%M";

    return timeFmt_;
  }
#endif
};

//---

#define CQChartsColumnTypeMgrInst CQChartsColumnTypeMgr::instance()

class CQChartsColumnTypeMgr {
 public:
  static CQChartsColumnTypeMgr *instance() {
    static CQChartsColumnTypeMgr *inst;

    if (! inst)
      inst = new CQChartsColumnTypeMgr;

    return inst;
  }

  ~CQChartsColumnTypeMgr() { }

  void addType(const QString &name, CQChartsColumnType *type) {
    nameType_[name] = type;
  }

  CQChartsColumnType *getType(const QString &name) const {
    auto p = nameType_.find(name);

    if (p == nameType_.end())
      return nullptr;

    return (*p).second;
  }

 private:
  CQChartsColumnTypeMgr() { }

 private:
  typedef std::map<QString,CQChartsColumnType *> NameType;

  NameType nameType_;
};

//---

class CQChartsColumn {
 public:
  struct NameValue {
    NameValue(const QString &name, const QString &value) :
     name(name), value(value) {
    }

    QString name;
    QString value;
  };

 public:
  CQChartsColumn(const QString &name=QString()) :
   name_(name) {
  }

  const QString &name() const { return name_; }
  void setName(const QString &v) { name_ = v; }

  const QString &type() const { return type_; }
  void setType(const QString &v) { type_ = v; }

  bool decodeType(QString &baseType, CQChartsNameValues &nameValues) const {
    return decodeType(type_, baseType, nameValues);
  }

  static bool decodeType(const QString &type, QString &baseType, CQChartsNameValues &nameValues) {
    int pos = type.indexOf(":");

    if (pos < 0) {
      baseType = type;

      return true;
    }

    baseType = type.mid(0, pos);

    QString rhs = type.mid(pos + 1);

    QStringList strs = rhs.split(",", QString::SkipEmptyParts);

    for (int i = 0; i < strs.length(); ++i) {
      int pos1 = strs[i].indexOf("=");

      if (pos1 < 1) {
        nameValues[strs[i]] = "1";
      }
      else {
        QString name  = strs[i].mid(0, pos1 ).simplified();
        QString value = strs[i].mid(pos1 + 1).simplified();

        nameValues[name] = value;
      }
    }

    return true;
  }

 private:
  QString name_;
  QString type_ { "string" };
};

#endif