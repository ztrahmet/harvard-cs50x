-- Write a SQL query to list the names of all people who have directed a movie that received a rating of at least 9.0.
SELECT DISTINCT p.name
FROM directors AS d
JOIN people AS p
  ON d.person_id = p.id
JOIN movies AS m
  ON d.movie_id = m.id
JOIN ratings AS r
  ON m.id = r.movie_id
WHERE r.rating >= 9.0;
