import RecommendationEngine;
import Logger;

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QtConcurrent>
#include <QFutureWatcher>
#include <QThread>
#include <QListWidgetItem>
#include <ranges>
#include <algorithm>
#include <QDir>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString dbPath = QCoreApplication::applicationDirPath() + "/movie.db";
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);

    if (!db.open()) {
        QMessageBox::critical(this, "Database Error", "Failed to connect to database: " + db.lastError().text());
        Logger::log("Database connection FAILED: " + db.lastError().text());
    }
    else {
        Logger::log("Database connected successfully.");
    }

    completerModel = new QStringListModel(this);
    completer = new QCompleter(completerModel, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains);
    ui->lineEdit->setCompleter(completer);
    ui->movieListWidget->setVisible(false);
    ui->movieListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &MainWindow::onMovieTitleChanged);
    connect(ui->movieListWidget, &QListWidget::itemClicked, this, &MainWindow::onSuggestionClicked);
    ui->movieListWidget->clear();

    connect(ui->lineEdit, &QLineEdit::textEdited, this, &MainWindow::onMovieTitleChanged);
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::on_pushButton_clicked);
    connect(ui->exportButton, &QPushButton::clicked, this, &MainWindow::on_exportButton_clicked);
}

MainWindow::~MainWindow()
{
    db.close();
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString searchText = ui->lineEdit->text().trimmed();
    QString movieNumber = ui->movieNumber->text();

    if (searchText.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a movie title.");
        Logger::log("Recommendation attempt with empty movie title.");
        return;
    }

    Logger::log("User requested recommendations for: " + searchText);

    ui->pushButton->setEnabled(false);
    ui->plainTextEdit->setPlainText("Generating recommendations, please wait...");

    auto future = QtConcurrent::run([=]() -> QString {
        QString connectionName = QString("thread_%1").arg((quintptr)QThread::currentThreadId());
        QSqlDatabase threadDb = QSqlDatabase::contains(connectionName)
            ? QSqlDatabase::database(connectionName)
            : QSqlDatabase::addDatabase("QSQLITE", connectionName);

        threadDb.setDatabaseName("C:/Users/MrPie/OneDrive/Desktop/PK4/Recommendations/movie.db");
        if (!threadDb.open()) {
            Logger::log("Thread DB open FAILED: " + threadDb.lastError().text());
            return "Error: Failed to open DB in thread: " + threadDb.lastError().text();
        }

        QSqlQuery query("SELECT * FROM movies", threadDb);
        QSqlRecord targetMovie;
        QString baseOverview;
        bool found = false;

        while (query.next()) {
            QString title = query.value("Series_Title").toString().trimmed();
            if (QString::compare(title, searchText, Qt::CaseInsensitive) == 0) {
                targetMovie = query.record();
                baseOverview = targetMovie.value("Overview").toString();
                found = true;
                break;
            }
        }

        if (!found) {
            Logger::log("Movie not found in DB: " + searchText);
            return "Movie not found.";
        }

        if (baseOverview.isEmpty()) {
            Logger::log("Movie found but has no overview: " + searchText);
            return "Selected movie has no overview.";
        }

        RecommendationEngine engine;
        QVector<MovieWithScore> candidates;

        query.exec("SELECT * FROM movies");
        while (query.next()) {
            QSqlRecord other = query.record();
            QString otherTitle = other.value("Series_Title").toString().trimmed();

            if (QString::compare(otherTitle, searchText, Qt::CaseInsensitive) == 0)
                continue;

            float score = engine.calculateScore(targetMovie, other);
            QString otherOverview = other.value("Overview").toString();

            if (score > 0.0f && !otherOverview.isEmpty()) {
                candidates.append({ otherTitle, score, otherOverview });
            }
        }

        std::ranges::sort(candidates, {}, &MovieWithScore::score);
        std::reverse(candidates.begin(), candidates.end());

        if (candidates.size() > 100)
            candidates.resize(100);

        QVector<MovieWithScore> finalRecs = engine.recommendByOverview(baseOverview, candidates);

        int desiredCount = movieNumber.toInt();
        if (desiredCount <= 0) desiredCount = 10;
        if (desiredCount > 100) desiredCount = 100;
        if (finalRecs.size() > desiredCount)
            finalRecs.resize(desiredCount);

        QString result = "Top Recommendations by Semantic Similarity:\n";
        for (int i = 0; i < finalRecs.size(); ++i) {
            result += QString("%1. %2 (Similarity Score: %3)\n")
                .arg(i + 1)
                .arg(finalRecs[i].title)
                .arg(finalRecs[i].score, 0, 'f', 2);
        }

        Logger::log("Recommendations generated successfully for: " + searchText);
        QSqlDatabase::removeDatabase(connectionName);

        return result;
        });

    auto* watcher = new QFutureWatcher<QString>(this);
    connect(watcher, &QFutureWatcher<QString>::finished, this, [=]() {
        QString result = watcher->result();
        ui->plainTextEdit->setPlainText(result);
        ui->pushButton->setEnabled(true);
        watcher->deleteLater();
        });

    watcher->setFuture(future);
}

void MainWindow::onMovieTitleChanged(const QString& text)
{
    ui->movieListWidget->clear();

    if (text.length() < 2 || !db.isOpen()) {
        ui->movieListWidget->setVisible(false);
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT Series_Title FROM movies WHERE Series_Title LIKE :pattern LIMIT 10");
    query.bindValue(":pattern", "%" + text + "%");

    if (!query.exec()) {
        qDebug() << "Suggestion query failed:" << query.lastError().text();
        ui->movieListWidget->setVisible(false);
        return;
    }

    while (query.next()) {
        QString title = query.value(0).toString().trimmed();
        if (!title.isEmpty())
            ui->movieListWidget->addItem(title);
    }

    ui->movieListWidget->setVisible(ui->movieListWidget->count() > 0);
}

void MainWindow::onSuggestionClicked(QListWidgetItem* item)
{
    ui->lineEdit->setText(item->text());
    ui->movieListWidget->setVisible(false);
}

void MainWindow::on_exportButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save Recommendations"), "",
        tr("Text Files (*.txt);;All Files (*)"));

    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("File Error"),
            tr("Cannot write to file: %1").arg(file.errorString()));
        Logger::log("Failed to export recommendations to file: " + file.errorString());
        return;
    }

    QTextStream out(&file);
    out << ui->plainTextEdit->toPlainText();
    file.close();

    Logger::log("Exported recommendations to file: " + fileName);
    QMessageBox::information(this, tr("Success"), tr("Recommendations exported successfully."));
}
