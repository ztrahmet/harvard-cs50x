-- Write a SQL query to determine the number of movies with an IMDb rating of 10.0.
SELECT COUNT(*) AS n
FROM ratings
WHERE rating = 10.0;
