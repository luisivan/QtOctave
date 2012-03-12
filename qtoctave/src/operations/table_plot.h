#ifndef __TABLE_PLOT_H__
#define __TABLE_PLOT_H__
#include <QString>
#include <QDialog>
#include <QLineEdit>
#include <QRadioButton>
#include <QComboBox>
#include <QTableView>

class ComplexNumberTableModel;

class TablePlot: public QDialog
{
  Q_OBJECT
 public:
  enum Type {PLOT, SEMILOGY, SEMILOGX, LOGLOG, POLAR, BAR};
  TablePlot(QWidget *parent, QTableView *table, ComplexNumberTableModel *model, Type type = PLOT);

  QString command();

 private:
  Type type;

  QRadioButton *use_rows;
  QRadioButton *use_cols;

  QLineEdit *x_lineedit, *y_lineedit, *x_error_lineedit, *y_error_lineedit;
  QLineEdit *title_lineedit;

  QComboBox *style_combo, *color_combo, *point_combo;

  QTableView *table;
  ComplexNumberTableModel *model;
};

#endif 
