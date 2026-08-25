#pragma once
#include <QMainWindow>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QVBoxLayout>

#include "../headers/sequence.hpp"
#include "../headers/bitsequence.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onContainerChanged();
    void onOperationChanged();
    void onRun();

private:
    void setupOperationsTab();
    void updateOperations();
    QString mainStyle();
    template <typename T, template <typename> class Container>
    void executeStandard();
    void executeBit(int op);
    template <typename T, template <typename> class Container>
    Container<T>* createSeq(const QString& str);
    template <typename T>
    BitSequence<T>* createBitSeq(const QString& str);
    template <typename T>
    QString formatSeq(const Sequence<T>* seq) const;
    template <typename T>
    QString formatBitSeq(const BitSequence<T>* seq) const;
    template <typename T>
    T parseVal(const QString& str);
    QVBoxLayout *mainLayout;
    QGroupBox *groupSettings, *groupSeqA, *groupSeqB, *groupArgs, *groupResult;
    QComboBox *comboDataType, *comboContainer, *comboOperation;
    QLineEdit *lineSeqA, *lineSeqB;
    QLineEdit *lineValue, *lineIndex, *lineStart, *lineEnd;
    QLabel *labelValue, *labelIndex, *labelStart, *labelEnd;
    QPushButton *btnRun;
    QTextEdit *textResult;
};