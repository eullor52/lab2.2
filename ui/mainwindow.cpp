#include "mainwindow.h"
#include "resources.hpp"
#include "../headers/arraysequence.hpp"
#include "../headers/listsequence.hpp"
#include "../headers/bitsequence.hpp"
#include "../headers/algorithms.hpp"
#include "../headers/exceptions.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QRegularExpression>
#include <type_traits>

template <typename T>
T double_val(T x) { return x * static_cast<T>(2); }

template <typename T>
bool is_big(T x) { return x > static_cast<T>(60); }

template <typename T>
T sum_func(T acc, T x) { return acc + x; }

template <typename T>
T add_index(T x, size_t idx) { return x + static_cast<T>(idx); }

template <typename T>
T MainWindow::parseVal(const QString& str) {
    if constexpr (std::is_same_v<T, int>)
        return str.toInt();
    else
        return str.toDouble();
}

template <typename T, template <typename> class Container>
Container<T>* MainWindow::createSeq(const QString& str) {
    auto* seq = new Container<T>();
    if (!str.trimmed().isEmpty()) {
        QStringList parts = str.split(QRegularExpression("[\\s,]+"), Qt::SkipEmptyParts);
        for (const QString& part : parts)
            seq->Append(parseVal<T>(part));
    }
    return seq;
}

template <typename T>
BitSequence<T>* MainWindow::createBitSeq(const QString& str) {
    auto* seq = new MutableBitSequence<T>();
    if (!str.trimmed().isEmpty()) {
        QStringList parts = str.split(QRegularExpression("[\\s,]+"), Qt::SkipEmptyParts);
        for (const QString& part : parts) {
            bool bit = (part.toInt() != 0);
            seq->Append(Bit<T>(bit ? 1 : 0));
        }
    }
    return seq;
}

template <typename T>
QString MainWindow::formatSeq(const Sequence<T>* seq) const {
    QStringList lst;
    for (size_t i = 0; i < seq->GetLength(); ++i) {
        if constexpr (std::is_same_v<T, double>)
            lst << QString::number(seq->Get(i), 'f', 4);
        else
            lst << QString::number(seq->Get(i));
    }
    return lst.join(" ");
}

template <typename T>
QString MainWindow::formatBitSeq(const BitSequence<T>* seq) const {
    QStringList lst;
    for (size_t i = 0; i < seq->GetLength(); ++i) {
        lst << QString::number(static_cast<bool>(seq->Get(i)) ? 1 : 0);
    }
    return lst.join(" ");
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle(APP_TITLE);
    setMinimumSize(600, 400);
    setStyleSheet(mainStyle());

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(12);
    mainLayout->setContentsMargins(16, 16, 16, 16);

    setupOperationsTab();
    updateOperations();
}

MainWindow::~MainWindow() {}

QString MainWindow::mainStyle() {
    return R"(
        QMainWindow, QWidget {
            background-color: #1e1e1e;
            color: #ffffff;
        }
        QGroupBox {
            color: #ffffff;
            border: 1px solid #555555;
            border-radius: 4px;
            margin-top: 10px;
            padding-top: 10px;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 3px 0 3px;
        }
        QLineEdit, QTextEdit, QComboBox {
            background-color: #2d2d2d;
            color: #ffffff;
            border: 1px solid #555555;
            border-radius: 3px;
            padding: 4px;
        }
        QPushButton {
            background-color: #0d6efd;
            color: #ffffff;
            border: none;
            border-radius: 4px;
            padding: 8px 16px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #0b5ed7;
        }
        QPushButton:pressed {
            background-color: #0a58ca;
        }
        QComboBox::drop-down {
            border: 0px;
        }
        QComboBox QAbstractItemView {
            background-color: #2d2d2d;
            color: #ffffff;
            selection-background-color: #0d6efd;
        }
    )";
}

void MainWindow::setupOperationsTab() {
    groupSettings = new QGroupBox(TEXT_SETTINGS);
    QFormLayout *setLay = new QFormLayout(groupSettings);
    comboDataType = new QComboBox();
    comboDataType->addItems({TYPE_INT, TYPE_DOUBLE});
    comboContainer = new QComboBox();
    comboContainer->addItems({CONT_ARRAY, CONT_LIST, CONT_BIT});
    comboOperation = new QComboBox();
    setLay->addRow(TEXT_DATA_TYPE, comboDataType);
    setLay->addRow(TEXT_CONTAINER, comboContainer);
    setLay->addRow(TEXT_OPERATION, comboOperation);
    mainLayout->addWidget(groupSettings);

    groupSeqA = new QGroupBox(TEXT_SEQ_A);
    QVBoxLayout *aLay = new QVBoxLayout(groupSeqA);
    lineSeqA = new QLineEdit();
    lineSeqA->setPlaceholderText("Пример: 1 2 3 4 5");
    aLay->addWidget(lineSeqA);
    mainLayout->addWidget(groupSeqA);

    groupSeqB = new QGroupBox(TEXT_SEQ_B);
    QVBoxLayout *bLay = new QVBoxLayout(groupSeqB);
    lineSeqB = new QLineEdit();
    lineSeqB->setPlaceholderText("Пример: 6 7 8");
    bLay->addWidget(lineSeqB);
    mainLayout->addWidget(groupSeqB);

    groupArgs = new QGroupBox(TEXT_ARGS);
    QFormLayout *argsLay = new QFormLayout(groupArgs);
    labelValue = new QLabel(TEXT_VALUE);
    lineValue = new QLineEdit();
    argsLay->addRow(labelValue, lineValue);
    labelIndex = new QLabel(TEXT_INDEX);
    lineIndex = new QLineEdit();
    argsLay->addRow(labelIndex, lineIndex);
    labelStart = new QLabel(TEXT_START);
    lineStart = new QLineEdit();
    argsLay->addRow(labelStart, lineStart);
    labelEnd = new QLabel(TEXT_END);
    lineEnd = new QLineEdit();
    argsLay->addRow(labelEnd, lineEnd);
    mainLayout->addWidget(groupArgs);

    btnRun = new QPushButton(TEXT_RUN);
    mainLayout->addWidget(btnRun);

    groupResult = new QGroupBox(TEXT_GROUP_RESULT);
    QVBoxLayout *resLay = new QVBoxLayout(groupResult);
    textResult = new QTextEdit();
    textResult->setReadOnly(true);
    resLay->addWidget(textResult);
    mainLayout->addWidget(groupResult);

    connect(comboContainer, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onContainerChanged);
    connect(comboOperation, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onOperationChanged);
    connect(btnRun, &QPushButton::clicked, this, &MainWindow::onRun);
}

void MainWindow::updateOperations() {
    comboOperation->blockSignals(true);
    comboOperation->clear();
    comboOperation->addItem("GetFirst()", 0);
    comboOperation->addItem("GetLast()", 1);
    comboOperation->addItem("Get(index)", 2);
    comboOperation->addItem("GetLength()", 3);
    comboOperation->addItem("GetSubsequence(start, end)", 4);
    comboOperation->addItem("Append(item)", 5);
    comboOperation->addItem("Prepend(item)", 6);
    comboOperation->addItem("InsertAt(item, index)", 7);
    comboOperation->addItem("Concat(list)", 8);

    if (comboContainer->currentText() == CONT_BIT) {
        comboOperation->addItem("AND (&)", 10);
        comboOperation->addItem("OR (|)", 11);
        comboOperation->addItem("XOR (^)", 12);
        comboOperation->addItem("NOT (~)", 13);
    } else {
        comboOperation->addItem("Map (x * 2)", 14);
        comboOperation->addItem("Reduce (sum)", 15);
        comboOperation->addItem("Where (x > 60)", 16);
        comboOperation->addItem("MapIndexed (x + index)", 17);
        comboOperation->addItem("MinMaxAvg", 18);
        comboOperation->addItem("MovingAvg3 ((a[i-1]+a[i]+a[i+1])/3)", 19);
        comboOperation->addItem("ReflectSum (a[i] + a[n-1-i])", 20);
        comboOperation->addItem("Prefixes", 21);
        comboOperation->addItem("Postfixes", 22);
        comboOperation->addItem("operator[] (index)", 23);
    }
    comboOperation->blockSignals(false);
    onOperationChanged();
}

void MainWindow::onContainerChanged() {
    if (comboContainer->currentText() == CONT_BIT) {
        comboDataType->setCurrentIndex(0);
        comboDataType->setEnabled(false);
    } else {
        comboDataType->setEnabled(true);
    }
    updateOperations();
}

void MainWindow::onOperationChanged() {
    int op = comboOperation->currentData().toInt();
    groupSeqB->setVisible(op == 8 || op == 10 || op == 11 || op == 12);
    bool showVal = (op == 5 || op == 6 || op == 7);
    bool showIdx = (op == 2 || op == 7 || op == 23);
    bool showBounds = (op == 4);
    lineValue->setVisible(showVal);
    labelValue->setVisible(showVal);
    lineIndex->setVisible(showIdx);
    labelIndex->setVisible(showIdx);
    lineStart->setVisible(showBounds);
    labelStart->setVisible(showBounds);
    lineEnd->setVisible(showBounds);
    labelEnd->setVisible(showBounds);
    groupArgs->setVisible(showVal || showIdx || showBounds);
    textResult->clear();
}

template <typename T, template <typename> class Container>
void MainWindow::executeStandard() {
    Container<T>* seqA = createSeq<T, Container>(lineSeqA->text());

    int op = comboOperation->currentData().toInt();
    QString res;
    bool updateA = false;

    switch (op) {
    case 0:
        res = QString::number(seqA->GetFirst());
        break;
    case 1:
        res = QString::number(seqA->GetLast());
        break;
    case 2:
        res = QString::number(seqA->Get(lineIndex->text().toULongLong()));
        break;
    case 3:
        res = QString::number(seqA->GetLength());
        break;
    case 4: {
        auto* sub = seqA->GetSubsequence(lineStart->text().toULongLong(),
                                         lineEnd->text().toULongLong());
        res = formatSeq<T>(sub);
        delete sub;
        break;
    }
    case 5:
        seqA->Append(parseVal<T>(lineValue->text()));
        updateA = true;
        break;
    case 6:
        seqA->Prepend(parseVal<T>(lineValue->text()));
        updateA = true;
        break;
    case 7:
        seqA->InsertAt(parseVal<T>(lineValue->text()), lineIndex->text().toULongLong());
        updateA = true;
        break;
    case 8: {
        Container<T>* seqB = createSeq<T, Container>(lineSeqB->text());
        Sequence<T>* concatenated = seqA->Concat(*seqB);
        delete seqB;
        res = formatSeq<T>(concatenated);
        updateA = true;
        delete concatenated;
        break;
    }
    case 14: {
        auto* mapped = Map<T>(*seqA, double_val<T>);
        res = formatSeq<T>(mapped);
        delete mapped;
        break;
    }
    case 15: {
        T val = Reduce<T, T>(*seqA, sum_func<T>, static_cast<T>(0));
        if constexpr (std::is_same_v<T, double>)
            res = QString::number(val, 'f', 4);
        else
            res = QString::number(val);
        break;
    }
    case 16: {
        auto* filtered = Where<T>(*seqA, is_big<T>);
        res = formatSeq<T>(filtered);
        delete filtered;
        break;
    }
    case 17: {
        auto* mapped = MapIndexed<T>(*seqA, add_index<T>);
        res = formatSeq<T>(mapped);
        delete mapped;
        break;
    }
    case 18: {
        auto stats = ComputeMinMaxAvg<T>(*seqA);
        QString minStr, maxStr;
        if constexpr (std::is_same_v<T, double>) {
            minStr = QString::number(stats.min, 'f', 4);
            maxStr = QString::number(stats.max, 'f', 4);
        } else {
            minStr = QString::number(stats.min);
            maxStr = QString::number(stats.max);
        }
        res = QString("min=%1 max=%2 avg=%3")
                  .arg(minStr).arg(maxStr).arg(QString::number(stats.avg, 'f', 4));
        break;
    }
    case 19: {
        auto* avg3 = MovingAverage3<T>(*seqA);
        res = formatSeq<double>(avg3);
        delete avg3;
        break;
    }
    case 20: {
        auto* reflected = ReflectSum<T>(*seqA);
        res = formatSeq<T>(reflected);
        delete reflected;
        break;
    }
    case 21: {
        auto* prefixes = Prefixes<T>(*seqA);
        QStringList lines;
        for (size_t i = 0; i < prefixes->GetLength(); ++i) {
            Sequence<T>* p = prefixes->Get(i);
            lines << formatSeq<T>(p);
            delete p;
        }
        delete prefixes;
        res = lines.join("\n");
        break;
    }
    case 22: {
        auto* postfixes = Postfixes<T>(*seqA);
        QStringList lines;
        for (size_t i = 0; i < postfixes->GetLength(); ++i) {
            Sequence<T>* p = postfixes->Get(i);
            lines << formatSeq<T>(p);
            delete p;
        }
        delete postfixes;
        res = lines.join("\n");
        break;
    }
    case 23: {
        T val = (*seqA)[lineIndex->text().toULongLong()];
        if constexpr (std::is_same_v<T, double>)
            res = QString::number(val, 'f', 4);
        else
            res = QString::number(val);
        break;
    }
    default:
        res = "Неизвестная операция";
    }

    if (updateA && res.isEmpty())
        res = formatSeq<T>(seqA);

    textResult->setPlainText(res);
    if (updateA)
        lineSeqA->setText(res);

    delete seqA;
}

void MainWindow::executeBit(int op) {
    BitSequence<int>* seqA = createBitSeq<int>(lineSeqA->text());
    QString res;
    bool updateA = false;

    switch (op) {
    case 0:
        res = QString::number(static_cast<bool>(seqA->GetFirst()));
        break;
    case 1:
        res = QString::number(static_cast<bool>(seqA->GetLast()));
        break;
    case 2:
        res = QString::number(static_cast<bool>(seqA->Get(lineIndex->text().toULongLong())));
        break;
    case 3:
        res = QString::number(seqA->GetLength());
        break;
    case 4: {
        auto* sub = seqA->GetSubsequence(lineStart->text().toULongLong(),
                                         lineEnd->text().toULongLong());
        res = formatBitSeq<int>(sub);
        delete sub;
        break;
    }
    case 5: {
        int val = lineValue->text().toInt();
        seqA->Append(Bit<int>(val != 0 ? 1 : 0));
        updateA = true;
        break;
    }
    case 6: {
        int val = lineValue->text().toInt();
        seqA->Prepend(Bit<int>(val != 0 ? 1 : 0));
        updateA = true;
        break;
    }
    case 7: {
        int val = lineValue->text().toInt();
        seqA->InsertAt(Bit<int>(val != 0 ? 1 : 0), lineIndex->text().toULongLong());
        updateA = true;
        break;
    }
    case 8: {
        auto* seqB = createBitSeq<int>(lineSeqB->text());
        auto* concatenated = seqA->Concat(*seqB);
        delete seqB;
        res = formatBitSeq<int>(concatenated);
        updateA = true;
        delete concatenated;
        break;
    }
    case 10: {
        auto* seqB = createBitSeq<int>(lineSeqB->text());
        BitSequence<int> result = *seqA & *seqB;
        res = formatBitSeq<int>(&result);
        updateA = true;
        delete seqB;
        break;
    }
    case 11: {
        auto* seqB = createBitSeq<int>(lineSeqB->text());
        BitSequence<int> result = *seqA | *seqB;
        res = formatBitSeq<int>(&result);
        updateA = true;
        delete seqB;
        break;
    }
    case 12: {
        auto* seqB = createBitSeq<int>(lineSeqB->text());
        BitSequence<int> result = *seqA ^ *seqB;
        res = formatBitSeq<int>(&result);
        updateA = true;
        delete seqB;
        break;
    }
    case 13: {
        BitSequence<int> result = ~(*seqA);
        res = formatBitSeq<int>(&result);
        updateA = true;
        break;
    }
    default:
        res = "Неизвестная операция";
    }

    if (updateA && res.isEmpty())
        res = formatBitSeq<int>(seqA);

    textResult->setPlainText(res);
    if (updateA)
        lineSeqA->setText(res);

    delete seqA;
}

void MainWindow::onRun() {
    try {
        QString container = comboContainer->currentText();
        QString dataType = comboDataType->currentText();
        int op = comboOperation->currentData().toInt();

        if (container == CONT_BIT) {
            executeBit(op);
        } else {
            if (container == CONT_ARRAY) {
                if (dataType == TYPE_INT) {
                    executeStandard<int, MutableArraySequence>();
                } else {
                    executeStandard<double, MutableArraySequence>();
                }
            } else if (container == CONT_LIST) {
                if (dataType == TYPE_INT) {
                    executeStandard<int, MutableListSequence>();
                } else {
                    executeStandard<double, MutableListSequence>();
                }
            }
        }
    } catch (const Exception& e) {
        QMessageBox::warning(this, ERR_TITLE, e.what());
    } catch (const std::exception& e) {
        QMessageBox::warning(this, ERR_TITLE, e.what());
    } catch (...) {
        QMessageBox::warning(this, ERR_TITLE, ERR_INVALID_INPUT);
    }
}