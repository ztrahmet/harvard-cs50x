-- Write a SQL query to list the names of all people who starred in Toy Story.
SELECT p.name
FROM
    people AS p
  , stars AS S
  , movies AS m
WHERE s.person_id = p.id
  AND s.movie_id = m.id
  AND m.title = 'Toy Story';
