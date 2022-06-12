SELECT
    people.name
FROM
    movies
LEFT JOIN
    stars
    ON movies.id = stars.movie_id
LEFT JOIN
    people
    ON stars.person_id = people.id
WHERE
    movies.title = "Toy Story";