#ifndef MOVIE_H
#define MOVIE_H

#include <string>

class Movie {
public:
    // Constructor
    Movie();

    // Getters
    std::string getPosterLink() const;
    std::string getSeriesTitle() const;
    int getReleasedYear() const;
    std::string getCertificate() const;
    std::string getRuntime() const;
    std::string getGenre() const;
    float getImdbRating() const;
    std::string getOverview() const;
    int getMetaScore() const;
    std::string getDirector() const;
    std::string getStar1() const;
    std::string getStar2() const;
    std::string getStar3() const;
    std::string getStar4() const;
    int getNoOfVotes() const;
    double getGross() const;

    // Setters
    void setPosterLink(const std::string& value);
    void setSeriesTitle(const std::string& value);
    void setReleasedYear(int value);
    void setCertificate(const std::string& value);
    void setRuntime(const std::string& value);
    void setGenre(const std::string& value);
    void setImdbRating(float value);
    void setOverview(const std::string& value);
    void setMetaScore(int value);
    void setDirector(const std::string& value);
    void setStar1(const std::string& value);
    void setStar2(const std::string& value);
    void setStar3(const std::string& value);
    void setStar4(const std::string& value);
    void setNoOfVotes(int value);
    void setGross(double value);

private:
    std::string posterLink;
    std::string seriesTitle;
    int releasedYear;
    std::string certificate;
    std::string runtime;
    std::string genre;
    float imdbRating;
    std::string overview;
    int metaScore;
    std::string director;
    std::string star1;
    std::string star2;
    std::string star3;
    std::string star4;
    int noOfVotes;
    double gross;
};

#endif // MOVIE_H
