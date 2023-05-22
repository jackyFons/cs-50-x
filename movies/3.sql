--Outputs table of movie titles released 2018 and after
SELECT title FROM movies WHERE year > 2017 ORDER BY title ASC;