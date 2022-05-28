SELECT
    movies.title,
    ratings.rating
FROM
    movies
LEFT JOIN
    ratings
    ON movies.id = ratings.movie_id
WHERE
    movies.year = 2010
    AND ratings.rating IS NOT NULL
ORDER BY
    ratings.rating DESC,
    movies.title ASC;