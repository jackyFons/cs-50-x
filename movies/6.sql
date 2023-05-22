--Outputs a table with one row containing the average IMDb ratings of all movies released in 2012
SELECT AVG(rating) FROM ratings WHERE movie_id IN (SELECT id FROM movies WHERE year = 2012);