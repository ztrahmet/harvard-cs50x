-- Write a SQL query to determine the average rating of all movies released in 2012.
SELECT AVG(r.rating) AS avg_rating
FROM ratings AS r
JOIN movies AS m
  ON r.movie_id = m.id
WHERE m.year = 2012;
