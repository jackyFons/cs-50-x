--Outputs table of directors with a movies that has a rating >= 9.0
SELECT DISTINCT(people.name)
FROM people JOIN directors ON people.id = directors.person_id
JOIN ratings ON directors.movie_id = ratings.movie_id
WHERE ratings.rating >= 9;