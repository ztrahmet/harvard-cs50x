-- Write a SQL query to list the titles of all movies in which both Bradley Cooper and Jennifer Lawrence starred.
SELECT m.title
FROM movies AS m
JOIN stars AS s
  ON s.movie_id = m.id
JOIN people AS p
  ON s.person_id = p.id
WHERE p.name IN ('Bradley Cooper', 'Jennifer Lawrence')
GROUP BY m.title
HAVING COUNT(DISTINCT p.name) = 2;
