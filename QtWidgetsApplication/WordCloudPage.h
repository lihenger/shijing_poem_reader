#pragma once
#include <QWidget>
#include <QPushButton>
#include <QLabel>

class WordCloudWidget;

class WordCloudPage : public QWidget {
    Q_OBJECT

public:
    explicit WordCloudPage(QWidget* parent = nullptr);
    void setWordFrequency(const QMap<QString, int>& frequency,
        int totalPoems, int totalWords);

signals:
    void backClicked();

private slots:
    void onBackClicked();
    void onSaveClicked();
    void onZoomInClicked();
    void onZoomOutClicked();
    void onResetClicked();

private:
    WordCloudWidget* m_wordCloudWidget;
    QPushButton* m_btnBack;
    QPushButton* m_btnSave;
    QPushButton* m_btnZoomIn;
    QPushButton* m_btnZoomOut;
    QPushButton* m_btnReset;
    QLabel* m_infoLabel;
};