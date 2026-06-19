-- ============================================================
-- Books Dataset Analysis Queries
-- Database: bookanalysis | Table: books
-- ============================================================

USE bookanalysis;

-- ------------------------------------------------------------
-- 1. OVERVIEW
-- ------------------------------------------------------------
SELECT
    COUNT(*)                       AS total_books,
    MIN(`Year`)                    AS earliest_year,
    MAX(`Year`)                    AS latest_year,
    ROUND(AVG(`User Rating`), 2)   AS avg_rating,
    ROUND(AVG(`Price`), 2)         AS avg_price
FROM books;

-- ------------------------------------------------------------
-- 2. TOP 5 HIGHEST RATED BOOKS
-- ------------------------------------------------------------
SELECT `Name`, `User Rating`, `Year`
FROM books
ORDER BY `User Rating` DESC
LIMIT 5;

-- ------------------------------------------------------------
-- 3. TOP 5 MOST REVIEWED BOOKS
-- ------------------------------------------------------------
SELECT `Name`, `Reviews`
FROM books
ORDER BY `Reviews` DESC
LIMIT 5;

-- ------------------------------------------------------------
-- 4. AVERAGE RATING BY GENRE
-- ------------------------------------------------------------
SELECT `Genre`, ROUND(AVG(`User Rating`), 2) AS avg_rating
FROM books
GROUP BY `Genre`;

-- ------------------------------------------------------------
-- 5. BOOKS PUBLISHED IN 2024
-- ------------------------------------------------------------
SELECT `Name`, `Author`, `User Rating`
FROM books
WHERE `Year` = 2024;

-- ------------------------------------------------------------
-- 6. BOOKS PUBLISHED IN 2025
-- ------------------------------------------------------------
SELECT `Name`, `Author`, `User Rating`
FROM books
WHERE `Year` = 2025;

-- ------------------------------------------------------------
-- BONUS QUERIES (not in the Python script, but nice extras
-- to show off more SQL skills if you want them)
-- ------------------------------------------------------------

-- Number of books per year
SELECT `Year`, COUNT(*) AS book_count
FROM books
GROUP BY `Year`
ORDER BY `Year`;

-- Average rating per year (trend)
SELECT `Year`, ROUND(AVG(`User Rating`), 2) AS avg_rating
FROM books
GROUP BY `Year`
ORDER BY `Year`;

-- Fiction vs Non Fiction count
SELECT `Genre`, COUNT(*) AS book_count
FROM books
GROUP BY `Genre`;
