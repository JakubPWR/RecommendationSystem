// recommendationengine.h

#ifndef RECOMMENDATIONENGINE_H
#define RECOMMENDATIONENGINE_H

#include <QObject>
#include <QMap>
#include <QVector>
#include <QSqlRecord>

class MovieWithScore {
public:
    QString title;
    float score;
    QString overview;

    // Default constructor
    MovieWithScore() : title(""), score(0.0f), overview("") {}

    // Constructor with parameters to initialize all fields
    MovieWithScore(const QString& t, float s, const QString& o)
        : title(t), score(s), overview(o) {}
};

class RecommendationEngine : public QObject
{
    Q_OBJECT
public:
    explicit RecommendationEngine(QObject *parent = nullptr);

    float calculateScore(const QSqlRecord &baseMovie, const QSqlRecord &otherMovie);
    QMap<QString, int> textToVector(const QString &text);
    float cosineSimilarity(const QMap<QString, int> &v1, const QMap<QString, int> &v2);
    QVector<MovieWithScore> recommendByOverview(const QString &baseOverview, const QVector<MovieWithScore> &topMovies);
};

#endif // RECOMMENDATIONENGINE_H
