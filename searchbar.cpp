#include "searchbar.h"
#include "./ui_searchbar.h"

SearchBar::SearchBar(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SearchBar)
{
    ui->setupUi(this);
}

SearchBar::~SearchBar()
{
    delete ui;
}
