--Outputs table with actors who starred with Kevin Bacon (b. 1958), excluding Kevin Bacon
SELECT DISTINCT(name)
FROM people JOIN stars ON people.id = stars.person_id
JOIN movies ON movies.id = stars.movie_id
WHERE movies.id
IN
(SELECT movies.id
FROM movies JOIN stars ON movies.id = stars.movie_id
JOIN people ON people.id = stars.person_id
WHERE people.name = "Kevin Bacon" AND people.birth = 1958)
AND people.name != "Kevin Bacon";