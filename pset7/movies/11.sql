-- Write a SQL query to list the titles of the five highest rated movies (in order) that Chadwick Boseman starred in, starting with the highest rated.
SELECT DISTINCT m.title
FROM stars AS s
JOIN movies AS m
  ON s.movie_id = m.id
JOIN people AS p
  ON s.person_id = p.id
JOIN ratings AS r
  ON m.id = r.movie_id
WHERE p.name = 'Chadwick Boseman'
ORDER BY r.rating DESC
LIMIT 5;
