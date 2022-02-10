#include "settingswindow.hpp"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(QWidget* parent)
    : QDialog{parent}
    , ui{new Ui::SettingsWindow}
{
    ui->setupUi(this);
    setMinimumSize(size());
    setMaximumSize(size() * 1.5);
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}
