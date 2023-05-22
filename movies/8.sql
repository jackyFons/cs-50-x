--Outputs table with all people in movie "Toy Story"
SELECT name FROM people
WHERE id IN (SELECT person_id FROM stars
WHERE movie_id IN (SELECT id FROM movies WHERE title LIKE "TOY STORY"));