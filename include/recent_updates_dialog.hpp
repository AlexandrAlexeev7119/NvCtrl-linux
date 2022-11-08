#pragma once

#include <QDialog>
#include <QThread>
#include <QProcess>

namespace Ui { class RecentUpdatesDialog; }

class RecentUpdatesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RecentUpdatesDialog(QWidget* parent = nullptr);
    ~RecentUpdatesDialog();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_label_changelog_text_linkActivated(const QString& link);

private:
    Ui::RecentUpdatesDialog* ui;
    QProcess retrieve_changelog_process_;

    void showEvent(QShowEvent* show_event);
};
