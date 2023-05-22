--Outputs table with titles and their ratings from movies released in 2010
--Table is ordered in descending order by rating, if ratings are the same, sort those alphabetically
SELECT title, rating
FROM ratings JOIN movies ON ratings.movie_id = movies.id
WHERE year = 2010
ORDER BY rating DESC, title ASC;