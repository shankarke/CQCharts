#ifndef CQChartsLoader_H
#define CQChartsLoader_H

#include <QFrame>

class CQCharts;
class CQFilename;
class QComboBox;
class QCheckBox;

class CQChartsLoader : public QFrame {
  Q_OBJECT

 public:
  CQChartsLoader(CQCharts *charts);
 ~CQChartsLoader();

  bool isCommentHeader() const;
  bool isFirstLineHeader() const;

 signals:
  void loadFile(const QString &type, const QString &filename);

 private slots:
  void okSlot();
  void cancelSlot();

 private:
  CQCharts*   charts_               { nullptr };
  QComboBox*  typeCombo_            { nullptr };
  CQFilename* fileEdit_             { nullptr };
  QCheckBox*  commentHeaderCheck_   { nullptr };
  QCheckBox*  firstLineHeaderCheck_ { nullptr };
};

#endif