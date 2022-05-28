WITH
kevin_movies AS (
    SELECT
        movies.id
    FROM
        movies, stars, people
    WHERE
        movies.id = stars.movie_id
        AND stars.person_id = people.id
        AND (people.birth = 1958 AND people.name = "Kevin Bacon"))

SELECT
    people.name
FROM
    movies, stars, people
INNER JOIN
    kevin_movies
    ON movies.id = kevin_movies.id
WHERE
    movies.id = stars.movie_id
    AND stars.person_id = people.id
    AND (people.birth != 1958 AND people.name != "Kevin Bacon");