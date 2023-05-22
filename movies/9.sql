--Outputs table with all people who starred in a movie in 2004, ordered by birth year
SELECT DISTINCT(people.name)
FROM stars JOIN people ON stars.person_id = people.id
JOIN movies ON movies.id = stars.movie_id
WHERE movies.year = 2004
ORDER BY people.birth;