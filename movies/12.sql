--Outputs table of movies that star Johnny Depp and HBC
SELECT title
FROM movies JOIN stars ON movies.id = stars.movie_id
JOIN people ON people.id = stars.person_id
WHERE people.name = "Johnny Depp" and movies.title
IN(SELECT title
FROM movies JOIN stars ON movies.id = stars.movie_id
JOIN people ON people.id = stars.person_id
WHERE people.name = "Helena Bonham Carter");