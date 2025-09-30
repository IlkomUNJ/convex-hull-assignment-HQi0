#include "mainwindow.h"
#include "drawingcanvas.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle("Computer Graphics Simulation");

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    canvas = new DrawingCanvas(this);
    mainLayout->addWidget(canvas);

    QHBoxLayout *buttonLayout = new QHBoxLayout();

    QPushButton *runButton = new QPushButton("Run Convex Hull Algorithms", this);
    QPushButton *clearButton = new QPushButton("Clear Canvas", this);

    buttonLayout->addWidget(runButton);
    buttonLayout->addWidget(clearButton);

    mainLayout->addLayout(buttonLayout);

    setCentralWidget(centralWidget);

    connect(clearButton, &QPushButton::clicked, canvas, &DrawingCanvas::clearCanvas);
    connect(runButton, &QPushButton::clicked, canvas, &DrawingCanvas::runAlgorithms);
}

MainWindow::~MainWindow()
{
}
