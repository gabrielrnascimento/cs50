WITH
johnny_movies AS (
    SELECT
        movies.id,
        movies.title
    FROM
        movies, stars, people
    WHERE
        movies.id = stars.movie_id
        AND stars.person_id = people.id
        AND people.name = "Johnny Depp"),

helena_movies AS (
    SELECT
        movies.id,
        movies.title
    FROM
        movies, stars, people
    WHERE
        movies.id = stars.movie_id
        AND stars.person_id = people.id
        AND people.name = "Helena Bonham Carter")

SELECT johnny_movies.title FROM johnny_movies INNER JOIN helena_movies USING (id);