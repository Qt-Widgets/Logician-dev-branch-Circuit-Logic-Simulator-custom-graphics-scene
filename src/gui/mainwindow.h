#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDockWidget>
#include <QMainWindow>

#include "../controllers/circuit_view/circuitviewcontroller.h"
#include "../widgets/assetbrowser.h"
#include "../widgets/mainwindowmenubar.h"
#include "../widgets/mainwindowtoolbar.h"
#include "../widgets/mainwindowui.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);

    void setCircuitViewController(CircuitViewController *controller);

private:
    AssetBrowser *browser_;
    MainWindowMenuBar *menuBar_;
    MainWindowToolBar *toolBar_;
    MainWindowUi *ui_;

signals:

public slots:
};

#endif // MAINWINDOW_H
