-- Write a SQL query to list the names of all people who starred in a movie in which Kevin Bacon also starred.
SELECT p.name
FROM stars AS s
JOIN people AS p
  ON s.person_id = p.id
WHERE s.movie_id IN (
    SELECT movie_id
    FROM stars AS s
    JOIN people as p
      ON s.person_id = p.id
    WHERE p.name = 'Kevin Bacon'
)
AND p.name != 'Kevin Bacon';
