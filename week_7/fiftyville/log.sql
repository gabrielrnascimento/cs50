-- Keep a log of any SQL queries you execute as you solve the mystery.
WITH
-- ? Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery. Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery.
-- ! It was at 10:15 AM. 3 witnesses each of them mentions the bakery.

-- * Query about the crime scene report about the CS50 duck theft.
theft_report AS (
    SELECT
        *
    FROM
        crime_scene_reports
    WHERE
        year = 2021
        AND month = 7
        AND day = 28
        AND description LIKE "%CS50%"
),

-- * Query for witnesses` transcripts
transcripts AS (
    SELECT
        *
    FROM
        interviews
    WHERE
        year = 2021
        AND month = 7
        AND day = 28
        AND transcript LIKE "%bakery%"
),

-- ? Transcript 1
-- ? Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away.
-- ? If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.
-- * Query for cars` license plates that left the parking lot of the bakery within 10 minutes of the theft
car_name AS (
    SELECT
        people.id,
        people.name
    FROM
        bakery_security_logs
    LEFT JOIN
        people USING (license_plate)
    WHERE
        activity = "exit"
        AND year = 2021
        AND month = 7
        AND day = 28
        AND hour = 10
        AND minute BETWEEN 15 AND 25
),

-- ? Transcript 2
-- ? I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money
-- * Query for getting ATM transactions from a suspect before the theft
atm_name AS (
    SELECT
        people.id,
        people.name
    FROM
        atm_transactions
    LEFT JOIN
        bank_accounts
        USING (account_number)
    LEFT JOIN
        people
        ON bank_accounts.person_id = people.id
    WHERE
        transaction_type = "withdraw"
        AND atm_location = "Leggett Street"
        AND year = 2021
        AND month = 7
        AND day = 28
),


-- ? Transcript 3
-- ? As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow.
-- ? The thief then asked the person on the other end of the phone to purchase the flight ticket.
-- * Query to find the call a suspect made to his accomplice
caller_name AS (
    SELECT
        people.id,
        people.name
    FROM
        phone_calls
    LEFT JOIN
        people
        ON phone_calls.caller = people.phone_number
    WHERE
        duration < 60
        AND year = 2021
        AND month = 7
        AND day = 28
),

receiver AS (
    SELECT
        *
    FROM
        phone_calls
    LEFT JOIN
        people
        ON phone_calls.receiver = people.phone_number
    WHERE
        duration < 60
        AND year = 2021
        AND month = 7
        AND day = 28
),

earliest_flight_id AS (
    SELECT
        id
    FROM
        flights
    WHERE
        year = 2021
        AND month = 7
        AND day = 29
    ORDER BY
        hour ASC,
        minute ASC
    LIMIT 1
),

passenger_name AS (
    SELECT
        people.id,
        people.name
    FROM
        passengers
    LEFT JOIN
        people
        USING (passport_number)
    WHERE
        flight_id = (SELECT * FROM earliest_flight_id)
),

-- * Find the thief
thief_info AS (
    SELECT
        people.*
    FROM
        car_name
    INNER JOIN
        atm_name
        USING (id)
    INNER JOIN
        caller_name
        USING (id)
    INNER JOIN
        passenger_name
        USING (id)
    INNER JOIN
        people
        USING (id)
),

thief_name AS (
    SELECT
        name AS thief
    FROM
        thief_info
),

-- * Find the city the thief escaped to
destination_city AS (
    SELECT
        airports.city
    FROM
        thief_info
    LEFT JOIN
        passengers
        ON thief_info.passport_number = passengers.passport_number
    LEFT JOIN
        flights
        ON passengers.flight_id = flights.id
    LEFT JOIN
        airports
        ON flights.destination_airport_id = airports.id
),

-- * Find the accomplice
accomplice_name AS (
    SELECT
        people.name AS accomplice
    FROM
        thief_info
    INNER JOIN
        receiver
        ON thief_info.phone_number = receiver.caller
    LEFT JOIN
        people
        ON receiver.receiver = people.phone_number
)


SELECT
    *
FROM
    thief_name, destination_city, accomplice_name

