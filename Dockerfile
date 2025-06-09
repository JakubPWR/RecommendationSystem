# Use an official Ubuntu as a base image
FROM ubuntu:20.04

# Set environment variables to prevent interactive prompts during installation
ENV DEBIAN_FRONTEND=noninteractive

# Install dependencies for Qt, SQLite, build tools, and Xvfb
RUN apt-get update && apt-get install -y \
    build-essential \
    qt5-qmake \
    qtbase5-dev \
    qtchooser \
    sqlite3 \
    libsqlite3-dev \
    cmake \
    git \
    xvfb \
    && rm -rf /var/lib/apt/lists/*

# Set up working directory for the Qt project
WORKDIR /app

# Copy the entire project to the container's /app directory
COPY . /app

# Create the SQLite database and import the CSV
RUN sqlite3 /app/movie.db <<EOF
-- Create the table structure based on the provided CSV columns
CREATE TABLE movies (
    Poster_Link TEXT,
    Series_Title TEXT,
    Released_Year INTEGER,
    Certificate TEXT,
    Runtime TEXT,
    Genre TEXT,
    IMDB_Rating REAL,
    Overview TEXT,
    Meta_score INTEGER,
    Director TEXT,
    Star1 TEXT,
    Star2 TEXT,
    Star3 TEXT,
    Star4 TEXT,
    No_of_Votes INTEGER,
    Gross REAL
);

-- Import data from the CSV file into the movies table
.mode csv
.import /app/imdb_top_1000.csv movies
EOF

# Run qmake to configure the project
RUN qmake Recommendations.pro

# Build the project using make
RUN make

# Expose a port (optional, for testing purposes if you have a GUI app running on a server)
EXPOSE 8080
#make database persistent
VOLUME ["/app"]

# Set the entry point to run the Qt application with Xvfb
CMD ["xvfb-run", "-a", "./Recommendations"]
