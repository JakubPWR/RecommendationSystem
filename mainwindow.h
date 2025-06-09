#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qlistwidget.h"
#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QCompleter>
#include <QStringListModel>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();  // <-- SLOT for the button click
    void onMovieTitleChanged(const QString &text);
    void onSuggestionClicked(QListWidgetItem *item);
    void on_exportButton_clicked();


private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QCompleter *completer;
    QStringListModel *completerModel;
};

#endif // MAINWINDOW_H
