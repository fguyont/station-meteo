#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private slots:
    void on_cboChoixVille_currentTextChanged(const QString &arg1);

    void on_rbH12_clicked();

    void on_rbH24_clicked();

    void on_rbCel_clicked();

    void on_rbFah_clicked();

    void on_cboPolice_currentFontChanged(const QFont &f);


    void on_rbJour_clicked();

    void on_rbNuit_clicked();

    void on_rbEng_clicked();

    void on_rbFra_clicked();

    void setFrench();

    void setEnglish();

    void update();

    void setJour();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
