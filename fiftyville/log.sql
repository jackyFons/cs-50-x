-- See crime report from day theft happened
SELECT description FROM crime_scene_reports
        WHERE month = 7
        AND day = 28
        AND year = 2021
--DISCOVERED: took place at 10:15am @ Humphrey Str. There are transcripts from 3 people mentioning a bakery


-- Get interview transcripts from people who saw the theft
SELECT name, transcript FROM interviews
        WHERE month = 7
        AND day = 28
        AND year = 2021
        AND transcript LIKE "%bakery%"
-- DISCOVERED: the bakery's parking lot security footage may have filmed the getaway car within 10 minutes of theft
-- someone saw the thief at the ATM in Leggett Str earlier in the morning WITHDRAWING
-- the thief made a 30 min call asking the other person to buy the first flight out of fiftyville


-- get 'people' info from cars exiting bakery 10 min within theft
SELECT id, name, phone_number FROM people
        WHERE license_plate IN
            (SELECT license_plate FROM bakery_security_logs
                    WHERE month = 7
                    AND day = 28
                    AND year = 2021
                    AND hour = 10
                    AND (minute > 14 AND minute < 26)
                    AND activity = "exit")
--DISCOVERED: Names are Vanessa, Barry, Iman, Sofia, Luca, Diana, Kelsey, and Bruce

-- Check if each of the 8 people made phone calls lasting ~30 min
SELECT caller, receiver, duration FROM phone_calls
    WHERE caller IN
        (SELECT phone_number FROM people
            WHERE license_plate IN
                (SELECT license_plate FROM bakery_security_logs
                    WHERE month = 7
                    AND day = 28
                    AND year = 2021
                    AND hour = 10
                    AND (minute > 14 AND minute < 26)
                    AND activity = "exit"))
        AND (duration < 60)
-- DISCOVERED 4 people made a phone call for less than 1 minute
-- Names are Sofia, Kelsey, Bruce, Diana

-- Check if any of the 4 remaining people withdrew money at the ATM
SELECT name FROM people
    WHERE id IN
        (SELECT person_id FROM bank_accounts
            WHERE account_number IN
                (SELECT account_number FROM atm_transactions
                    WHERE month = 7
                    AND day = 28
                    AND year = 2021
                    AND transaction_type = "withdraw"
                    AND atm_location LIKE "leggett street"))
-- DISCOVERED: From the 4, Bruce withdrew money
-- BRUCE IS THE CULPRIT

-- Checks what flights Bruce has been on
SELECT flight_id FROM passengers
    WHERE passport_number IN
        (SELECT passport_number FROM people
            WHERE name = "Bruce")
--Discovered: Flight 36


-- Determine where Flight 36 went
SELECT city FROM airports
    WHERE id IN
        (SELECT destination_airport_id FROM flights
            WHERE id = 36)
--Discovered: New York City

--Get accomplice name
-- Accomplice is Robin
SELECT name FROM people
    WHERE phone_number IN
        (SELECT receiver FROM phone_calls
            WHERE caller IN
                (SELECT phone_number FROM people
                    WHERE name = "Bruce")
                    AND duration < 60
                    AND month = 7
                    AND day = 28
                    AND year = 2021);