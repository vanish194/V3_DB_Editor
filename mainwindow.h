// MainWindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QStandardItemModel>
#include "DatabaseManager.h"
#include "Storage.h"
#include "customtreeview.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onOpenDatabase();

private:
    Ui::MainWindow *ui;
    DatabaseManager dbManager;
    Storage *storage;

    void populateTable();
    void populateTree();
};

#endif // MAINWINDOW_H
