#include "recommendationengine.h"
#include <QRegularExpression>
#include <QSet>
#include <QtMath>
#include <algorithm>
#include <QDebug>


RecommendationEngine::RecommendationEngine(QObject *parent)
    : QObject(parent)
{
}

float RecommendationEngine::calculateScore(const QSqlRecord &base, const QSqlRecord &other)
{
    float score = 0.0f;

    QStringList baseGenres = base.value("Genre").toString().split(",", Qt::SkipEmptyParts);
    QStringList otherGenres = other.value("Genre").toString().split(",", Qt::SkipEmptyParts);

    for (const QString &genre : baseGenres) {
        if (otherGenres.contains(genre.trimmed(), Qt::CaseInsensitive)) {
            score += 1.0f;
        }
    }

    int baseYear = base.value("Released_Year").toInt();
    int otherYear = other.value("Released_Year").toInt();
    if (baseYear == otherYear)
        score += 0.5f;
    else if (std::abs(baseYear - otherYear) <= 3)
        score += 0.25f;

    if (base.value("Director").toString().compare(other.value("Director").toString(), Qt::CaseInsensitive) == 0)
        score += 0.5f;

    QStringList baseStars = {
        base.value("Star1").toString(),
        base.value("Star2").toString(),
        base.value("Star3").toString(),
        base.value("Star4").toString()
    };

    for (const QString &star : baseStars) {
        if (star.isEmpty()) continue;

        for (int i = 1; i <= 4; ++i) {
            if (other.value(QString("Star%1").arg(i)).toString().compare(star, Qt::CaseInsensitive) == 0) {
                score += 0.25f;
                break;
            }
        }
    }

    float rating = other.value("IMDB_Rating").toFloat();
    if (rating >= 8.0f) score += 1.0f;
    else if (rating >= 7.0f) score += 0.5f;
    else if (rating >= 6.0f) score += 0.25f;

    return score;
}

QMap<QString, int> RecommendationEngine::textToVector(const QString &text)
{
    QMap<QString, int> freq;
    if (text.isEmpty())
        return freq;

    QStringList words = text.toLower().split(QRegularExpression("\\W+"), Qt::SkipEmptyParts);
    for (const QString &word : words) {
        freq[word]++;
    }
    return freq;
}
float RecommendationEngine::cosineSimilarity(const QMap<QString, int> &v1, const QMap<QString, int> &v2)
{
    // Create a QSet to hold all unique words from both v1 and v2
    QSet<QString> allWords;

    // Insert keys from v1 into the QSet
    for (const QString &word : v1.keys()) {
        allWords.insert(word);
    }

    // Insert keys from v2 into the QSet
    for (const QString &word : v2.keys()) {
        allWords.insert(word);
    }

    float dot = 0, normA = 0, normB = 0;

    // Loop over all unique words from both v1 and v2
    for (const QString &word : allWords) {
        int a = v1.value(word, 0);  // Default to 0 if the word doesn't exist in v1
        int b = v2.value(word, 0);  // Default to 0 if the word doesn't exist in v2
        dot += a * b;
        normA += a * a;
        normB += b * b;
    }

    // If either vector is empty (normA or normB is 0), similarity is 0
    if (normA == 0 || normB == 0)
        return 0.0f;

    return dot / (qSqrt(normA) * qSqrt(normB));
}



QVector<MovieWithScore> RecommendationEngine::recommendByOverview(const QString &baseOverview, const QVector<MovieWithScore> &topMovies)
{
    QVector<MovieWithScore> results;
    QMap<QString, int> baseVec = textToVector(baseOverview);

    for (const MovieWithScore &m : topMovies) {
        if (m.overview.isEmpty()) {
            qDebug() << "Skipping empty overview for movie:" << m.title;
            continue;
        }

        float sim = cosineSimilarity(baseVec, textToVector(m.overview));

        // Ensure movie title and overview are not empty
        if (!m.title.isEmpty() && !m.overview.isEmpty()) {
            results.append(MovieWithScore(m.title, sim, m.overview));
        }
    }

    // Ensure results vector is not empty before sorting or resizing
    if (results.isEmpty()) {
        qDebug() << "No valid results found.";
        return results;
    }

    std::sort(results.begin(), results.end(), [](const auto &a, const auto &b) {
        return a.score > b.score;
    });

    if (results.size() > 100)
        results.resize(100);

    return results;
}

