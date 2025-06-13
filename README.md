# 🎬 Qt Movie Recommendation System

A cross-platform desktop application built using **Qt (C++/QML)** that provides intelligent movie recommendations based on a selected movie's content and metadata. The system leverages both **attribute-based scoring** (genres, cast, director, release year, IMDB rating) and **semantic similarity** of movie overviews using cosine similarity on term frequency vectors.

---

## 🚀 Features

- 🔍 **Live search & auto-suggestions** with fuzzy matching of movie titles.
- 🧠 **Hybrid recommendation engine** combining:
  - Metadata similarity (genre, director, year, cast, rating)
  - **Semantic similarity** of movie descriptions using a bag-of-words approach and cosine similarity.
- ⚡ **Asynchronous recommendation generation** via `QtConcurrent` to keep the UI responsive.
- 💾 **SQLite-based movie database** for fast local querying.
- 📝 **Export recommendations** to a text file with one click.
- 📄 **Robust logging system** for tracking usage and errors.

---

## 🧠 Recommendation Logic

1. **Pre-filter candidates** using metadata similarity:
   - Shared genres
   - Same or nearby release year
   - Common actors or director
   - IMDB rating weight
2. **Semantic ranking**:
   - Convert overviews into word-frequency vectors
   - Apply **cosine similarity** to compute text similarity
   - Rank top matches by relevance

---

## 🛠️ Technologies Used

- C++ / Qt 5 or 6
- Qt Widgets for GUI
- Qt SQL module (SQLite)
- QtConcurrent for multithreading
- STL algorithms and ranges
- C++20 modules (`RecommendationEngine`, `Logger`)

---

---

## 📦 Setup Instructions

1. **Requirements**:
   - Qt 5.15+ or Qt 6.x
   - C++20-compatible compiler
   - Qt Creator (recommended)

2. **Build**:
   - Clone this repository
   - Open the `.pro` or CMake project in Qt Creator
   - Build and run the project

3. **Run**:
   - Ensure `movie.db` is in the same directory as the built application
   - Type a movie title to get suggestions
   - Click "Recommend" to get results
   - Use "Export" to save recommendations to a file

---

## 📸 Screenshots
##1
https://github.com/user-attachments/assets/b8141826-bb03-4a3e-bd9d-c16daffdb550

##2
https://github.com/user-attachments/assets/1f9cc737-cab6-42c6-9f60-f3e0c6a8a035


---

## 🧾 License

This project is licensed under the MIT License. See `LICENSE` for details.

---

## 🤝 Contributions

Contributions, issues, and feature requests are welcome!  
Feel free to fork the repository and submit a pull request.

---

## 📬 Contact

For questions or feedback, open an issue or contact the maintainer.


