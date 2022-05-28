SELECT
    DISTINCT people.name
FROM
    movies
LEFT JOIN
    directors
    ON movies.id = directors.movie_id
LEFT JOIN
    ratings
    ON movies.id = ratings.movie_id
LEFT JOIN
    people
    ON directors.person_id = people.id
WHERE
    ratings.rating >= 9.0
    AND people.name IS NOT NULL;