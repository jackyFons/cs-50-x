--Outputs table with all Harry Potter movie titles ordered by year
SELECT title, year FROM movies WHERE title LIKE "Harry Potter%" ORDER BY year ASC;