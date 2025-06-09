#include "movie.h"

// Constructor
Movie::Movie()
    : posterLink(""), seriesTitle(""), releasedYear(0), certificate(""),
    runtime(""), genre(""), imdbRating(0.0f), overview(""), metaScore(0),
    director(""), star1(""), star2(""), star3(""), star4(""),
    noOfVotes(0), gross(0.0) {}

// Getters
std::string Movie::getPosterLink() const { return posterLink; }
std::string Movie::getSeriesTitle() const { return seriesTitle; }
int Movie::getReleasedYear() const { return releasedYear; }
std::string Movie::getCertificate() const { return certificate; }
std::string Movie::getRuntime() const { return runtime; }
std::string Movie::getGenre() const { return genre; }
float Movie::getImdbRating() const { return imdbRating; }
std::string Movie::getOverview() const { return overview; }
int Movie::getMetaScore() const { return metaScore; }
std::string Movie::getDirector() const { return director; }
std::string Movie::getStar1() const { return star1; }
std::string Movie::getStar2() const { return star2; }
std::string Movie::getStar3() const { return star3; }
std::string Movie::getStar4() const { return star4; }
int Movie::getNoOfVotes() const { return noOfVotes; }
double Movie::getGross() const { return gross; }

// Setters
void Movie::setPosterLink(const std::string& value) { posterLink = value; }
void Movie::setSeriesTitle(const std::string& value) { seriesTitle = value; }
void Movie::setReleasedYear(int value) { releasedYear = value; }
void Movie::setCertificate(const std::string& value) { certificate = value; }
void Movie::setRuntime(const std::string& value) { runtime = value; }
void Movie::setGenre(const std::string& value) { genre = value; }
void Movie::setImdbRating(float value) { imdbRating = value; }
void Movie::setOverview(const std::string& value) { overview = value; }
void Movie::setMetaScore(int value) { metaScore = value; }
void Movie::setDirector(const std::string& value) { director = value; }
void Movie::setStar1(const std::string& value) { star1 = value; }
void Movie::setStar2(const std::string& value) { star2 = value; }
void Movie::setStar3(const std::string& value) { star3 = value; }
void Movie::setStar4(const std::string& value) { star4 = value; }
void Movie::setNoOfVotes(int value) { noOfVotes = value; }
void Movie::setGross(double value) { gross = value; }
