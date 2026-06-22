#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QRadioButton>
#include <QSpinBox>
#include <QMessageBox>
#include <QString>
#include <sstream>
#include <vector>

#include "headers/arraysequence.hpp"
#include "headers/listsequence.hpp"
#include "headers/exceptions.h"

// Вспомогательная функция: разбор строки с числами, разделёнными пробелами
static std::vector<int> parseNumbers(const QString& text) {
    std::vector<int> result;
    std::istringstream iss(text.toStdString());
    int num;
    while (iss >> num) {
        result.push_back(num);
    }
    return result;
}

// Вспомогательная функция: отображение последовательности в QString
template <typename T>
static QString sequenceToString(const Sequence<T>* seq) {
    if (!seq || seq->GetLength() == 0) return "(empty)";
    std::ostringstream oss;
    for (size_t i = 0; i < seq->GetLength(); ++i) {
        if (i > 0) oss << ' ';
        oss << seq->Get(i);
    }
    return QString::fromStdString(oss.str());
}

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    Sequence<int>* currentSeq = nullptr;

    QRadioButton* radioArray;
    QRadioButton* radioList;
    QLineEdit* inputElements;
    QPushButton* createButton;
    QTextEdit* display;

    QLineEdit* valueInput;
    QSpinBox* indexSpin;
    QPushButton* appendButton;
    QPushButton* prependButton;
    QPushButton* insertButton;

    QSpinBox* startSpin;
    QSpinBox* endSpin;
    QPushButton* subseqButton;

    QPushButton* concatButton;

public:
    MainWindow(QWidget* parent = nullptr) : QMainWindow(parent) {
        setWindowTitle("Sequence Tester (Qt)");
        resize(600, 500);

        QWidget* central = new QWidget(this);
        setCentralWidget(central);
        QVBoxLayout* mainLayout = new QVBoxLayout(central);

        // ---- Выбор типа последовательности ----
        QHBoxLayout* typeLayout = new QHBoxLayout;
        radioArray = new QRadioButton("ArraySequence", this);
        radioList = new QRadioButton("ListSequence", this);
        radioArray->setChecked(true);
        typeLayout->addWidget(new QLabel("Type:"));
        typeLayout->addWidget(radioArray);
        typeLayout->addWidget(radioList);
        typeLayout->addStretch();
        mainLayout->addLayout(typeLayout);

        // ---- Создание последовательности ----
        QHBoxLayout* createLayout = new QHBoxLayout;
        createLayout->addWidget(new QLabel("Elements (space separated):"));
        inputElements = new QLineEdit(this);
        createLayout->addWidget(inputElements);
        createButton = new QPushButton("Create", this);
        createLayout->addWidget(createButton);
        mainLayout->addLayout(createLayout);

        // ---- Отображение ----
        display = new QTextEdit(this);
        display->setReadOnly(true);
        display->setFontFamily("monospace");
        mainLayout->addWidget(display);

        // ---- Операции добавления ----
        QHBoxLayout* opLayout = new QHBoxLayout;
        opLayout->addWidget(new QLabel("Value:"));
        valueInput = new QLineEdit(this);
        valueInput->setFixedWidth(60);
        opLayout->addWidget(valueInput);
        opLayout->addWidget(new QLabel("Index:"));
        indexSpin = new QSpinBox(this);
        indexSpin->setRange(0, 999);
        opLayout->addWidget(indexSpin);
        appendButton = new QPushButton("Append", this);
        prependButton = new QPushButton("Prepend", this);
        insertButton = new QPushButton("InsertAt", this);
        opLayout->addWidget(appendButton);
        opLayout->addWidget(prependButton);
        opLayout->addWidget(insertButton);
        opLayout->addStretch();
        mainLayout->addLayout(opLayout);

        // ---- GetSubsequence ----
        QHBoxLayout* subLayout = new QHBoxLayout;
        subLayout->addWidget(new QLabel("Subsequence: start"));
        startSpin = new QSpinBox(this);
        startSpin->setRange(0, 999);
        subLayout->addWidget(startSpin);
        subLayout->addWidget(new QLabel("end"));
        endSpin = new QSpinBox(this);
        endSpin->setRange(0, 999);
        subLayout->addWidget(endSpin);
        subseqButton = new QPushButton("GetSubsequence", this);
        subLayout->addWidget(subseqButton);
        subLayout->addStretch();
        mainLayout->addLayout(subLayout);

        // ---- Concat ----
        concatButton = new QPushButton("Concat with new sequence from input", this);
        mainLayout->addWidget(concatButton);

        // ---- Подключение сигналов ----
        connect(createButton, &QPushButton::clicked, this, &MainWindow::onCreate);
        connect(appendButton, &QPushButton::clicked, this, &MainWindow::onAppend);
        connect(prependButton, &QPushButton::clicked, this, &MainWindow::onPrepend);
        connect(insertButton, &QPushButton::clicked, this, &MainWindow::onInsert);
        connect(subseqButton, &QPushButton::clicked, this, &MainWindow::onSubsequence);
        connect(concatButton, &QPushButton::clicked, this, &MainWindow::onConcat);

        updateDisplay();
    }

    ~MainWindow() {
        delete currentSeq;
    }

private slots:
    void onCreate() {
        delete currentSeq;
        currentSeq = nullptr;

        std::vector<int> values = parseNumbers(inputElements->text());
        int* arr = values.empty() ? nullptr : values.data();
        size_t count = values.size();

        try {
            if (radioArray->isChecked()) {
                currentSeq = new MutableArraySequence<int>(arr, count);
            } else {
                currentSeq = new MutableListSequence<int>(arr, count);
            }
        } catch (const Exception& e) {
            QMessageBox::critical(this, "Error", QString("Exception: ") + e.what());
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Error", QString("std::exception: ") + e.what());
        }

        updateDisplay();
    }

    void onAppend() {
        if (!currentSeq) {
            QMessageBox::warning(this, "Warning", "Create a sequence first.");
            return;
        }
        bool ok;
        int val = valueInput->text().toInt(&ok);
        if (!ok) {
            QMessageBox::warning(this, "Warning", "Invalid value.");
            return;
        }
        try {
            Sequence<int>* newSeq = currentSeq->Append(val);
            if (newSeq != currentSeq) {
                delete currentSeq;   // для immutable (или если метод создаёт новый объект)
            }
            currentSeq = newSeq;
            updateDisplay();
        } catch (const Exception& e) {
            QMessageBox::critical(this, "Error", QString("Exception: ") + e.what());
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Error", QString("std::exception: ") + e.what());
        }
    }

    void onPrepend() {
        if (!currentSeq) {
            QMessageBox::warning(this, "Warning", "Create a sequence first.");
            return;
        }
        bool ok;
        int val = valueInput->text().toInt(&ok);
        if (!ok) {
            QMessageBox::warning(this, "Warning", "Invalid value.");
            return;
        }
        try {
            Sequence<int>* newSeq = currentSeq->Prepend(val);
            if (newSeq != currentSeq) {
                delete currentSeq;
            }
            currentSeq = newSeq;
            updateDisplay();
        } catch (const Exception& e) {
            QMessageBox::critical(this, "Error", QString("Exception: ") + e.what());
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Error", QString("std::exception: ") + e.what());
        }
    }

    void onInsert() {
        if (!currentSeq) {
            QMessageBox::warning(this, "Warning", "Create a sequence first.");
            return;
        }
        bool ok;
        int val = valueInput->text().toInt(&ok);
        if (!ok) {
            QMessageBox::warning(this, "Warning", "Invalid value.");
            return;
        }
        size_t idx = static_cast<size_t>(indexSpin->value());
        try {
            Sequence<int>* newSeq = currentSeq->InsertAt(val, idx);
            if (newSeq != currentSeq) {
                delete currentSeq;
            }
            currentSeq = newSeq;
            updateDisplay();
        } catch (const Exception& e) {
            QMessageBox::critical(this, "Error", QString("Exception: ") + e.what());
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Error", QString("std::exception: ") + e.what());
        }
    }

    void onSubsequence() {
        if (!currentSeq) {
            QMessageBox::warning(this, "Warning", "Create a sequence first.");
            return;
        }
        size_t start = static_cast<size_t>(startSpin->value());
        size_t end = static_cast<size_t>(endSpin->value());
        try {
            Sequence<int>* sub = currentSeq->GetSubsequence(start, end);
            QString msg = "Subsequence: " + sequenceToString(sub);
            delete sub;
            QMessageBox::information(this, "Subsequence", msg);
        } catch (const Exception& e) {
            QMessageBox::critical(this, "Error", QString("Exception: ") + e.what());
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Error", QString("std::exception: ") + e.what());
        }
    }

    void onConcat() {
        if (!currentSeq) {
            QMessageBox::warning(this, "Warning", "Create a sequence first.");
            return;
        }
        std::vector<int> values = parseNumbers(inputElements->text());
        int* arr = values.empty() ? nullptr : values.data();
        size_t count = values.size();

        Sequence<int>* other = nullptr;
        try {
            if (radioArray->isChecked()) {
                other = new MutableArraySequence<int>(arr, count);
            } else {
                other = new MutableListSequence<int>(arr, count);
            }
        } catch (const Exception& e) {
            QMessageBox::critical(this, "Error", QString("Exception while creating other: ") + e.what());
            return;
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Error", QString("std::exception: ") + e.what());
            return;
        }

        try {
            Sequence<int>* concatSeq = currentSeq->Concat(*other);
            delete currentSeq;
            currentSeq = concatSeq;
            delete other;
            updateDisplay();
        } catch (const Exception& e) {
            QMessageBox::critical(this, "Error", QString("Exception: ") + e.what());
            delete other;
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Error", QString("std::exception: ") + e.what());
            delete other;
        }
    }

    void updateDisplay() {
        if (currentSeq) {
            display->setText("Sequence: " + sequenceToString(currentSeq) +
                             "\nLength: " + QString::number(currentSeq->GetLength()));
        } else {
            display->setText("(no sequence)");
        }
    }
};

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}

#include "main.moc"
