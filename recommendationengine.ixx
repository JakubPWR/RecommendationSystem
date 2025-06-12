export module RecommendationEngine;

import <QObject>;
import <QSqlRecord>;
import <QMap>;
import <QVector>;
import <QStringList>;
import <QSet>;
import <QtMath>;
import <QRegularExpression>;
import <QDebug>;
import <algorithm>;
import <concepts>

export struct MovieWithScore {
    QString title;
    float score;
    QString overview;

    MovieWithScore(QString t, float s, QString o)
        : title(std::move(t)), score(s), overview(std::move(o)) {
    }
};

export class RecommendationEngine : public QObject {
    Q_OBJECT

public:
    explicit RecommendationEngine(QObject* parent = nullptr);

    float calculateScore(const QSqlRecord& base, const QSqlRecord& other);
    QMap<QString, int> textToVector(const QString& text);
    float cosineSimilarity(const QMap<QString, int>& v1, const QMap<QString, int>& v2);
    QVector<MovieWithScore> recommendByOverview(const QString& baseOverview, const QVector<MovieWithScore>& topMovies);
};

RecommendationEngine::RecommendationEngine(QObject* parent)
    : QObject(parent) {
}

float RecommendationEngine::calculateScore(const QSqlRecord& base, const QSqlRecord& other) {
    float score = 0.0f;
    QStringList baseGenres = base.value("Genre").toString().split(",", Qt::SkipEmptyParts);
    QStringList otherGenres = other.value("Genre").toString().split(",", Qt::SkipEmptyParts);

    for (const QString& genre : baseGenres) {
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

    for (const QString& star : baseStars) {
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

QMap<QString, int> RecommendationEngine::textToVector(const QString& text) {
    QMap<QString, int> freq;
    if (text.isEmpty()) return freq;

    QStringList words = text.toLower().split(QRegularExpression("\\W+"), Qt::SkipEmptyParts);
    for (const QString& word : words) {
        freq[word]++;
    }
    return freq;
}

template <typename Map>
concept WordFrequencyMap = requires(Map m, typename Map::key_type key) {
    { m.value(key, 0) } -> std::convertible_to<int>;
    { m.keys() } -> std::convertible_to<QList<typename Map::key_type>>;
};

template <WordFrequencyMap MapType>
float RecommendationEngine::cosineSimilarity(const MapType& v1, const MapType& v2) {
    QSet<QString> allWords = QSet<QString>::fromList(v1.keys()) | QSet<QString>::fromList(v2.keys());

    float dot = 0, normA = 0, normB = 0;
    for (const QString& word : allWords) {
        int a = v1.value(word, 0);
        int b = v2.value(word, 0);
        dot += a * b;
        normA += a * a;
        normB += b * b;
    }

    if (normA == 0 || normB == 0)
        return 0.0f;

    return dot / (qSqrt(normA) * qSqrt(normB));
}

QVector<MovieWithScore> RecommendationEngine::recommendByOverview(const QString& baseOverview, const QVector<MovieWithScore>& topMovies) {
    QVector<MovieWithScore> results;
    QMap<QString, int> baseVec = textToVector(baseOverview);

    for (const MovieWithScore& m : topMovies) {
        if (m.overview.isEmpty()) continue;
        float sim = cosineSimilarity(baseVec, textToVector(m.overview));
        if (!m.title.isEmpty()) {
            results.append(MovieWithScore(m.title, sim, m.overview));
        }
    }

    std::sort(results.begin(), results.end(), [](const auto& a, const auto& b) {
        return a.score > b.score;
        });

    if (results.size() > 100)
        results.resize(100);

    return results;
}
