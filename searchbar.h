#ifndef SEARCHBAR_H
#define SEARCHBAR_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class SearchBar;
}
QT_END_NAMESPACE

class SearchBar : public QMainWindow
{
    Q_OBJECT

public:
    SearchBar(QWidget *parent = nullptr);
    ~SearchBar();

private:
    Ui::SearchBar *ui;
};
#endif // SEARCHBAR_H
