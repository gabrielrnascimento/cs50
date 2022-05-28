SELECT
    DISTINCT (people.name)
FROM
    movies
LEFT JOIN
    stars
    ON movies.id = stars.movie_id
LEFT JOIN
    people
    ON stars.person_id = people.id
WHERE
    movies.year = 2004
ORDER BY
    people.birth ASC;
