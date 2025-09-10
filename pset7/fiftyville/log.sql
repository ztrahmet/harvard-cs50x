-- CS50x Fiftyville
.schema

-- Get the crime description
SELECT description
FROM crime_scene_reports
WHERE year = 2024
  AND month = 7
  AND day = 28
  AND street = 'Humphrey Street';

-- Gather witness interviews
SELECT transcript
FROM interviews
WHERE year = 2024
  AND month = 7
  AND day = 28;

-- Check bakery logs between 10.15 AM and 10.25 AM
SELECT *
FROM people AS p
JOIN bakery_security_logs AS b
  ON p.license_plate = b.license_plate
WHERE b.year = 2024
  AND b.month = 7
  AND b.day = 28
  AND hour = 10
  AND minute BETWEEN 15 AND 25
  AND activity = 'exit';

-- Check who withdrew money before crime
SELECT name
FROM people AS p
JOIN bank_accounts AS b
  ON p.id = b.person_id
JOIN atm_transactions AS a
  ON b.account_number = a.account_number
WHERE a.year = 2024
  AND a.month = 7
  AND a.day = 28
  AND a.atm_location = 'Leggett Street'
  AND a.transaction_type = 'withdraw';

-- Check phone calls
SELECT name
FROM people AS p1
JOIN phone_calls AS p2
  ON p1.phone_number = p2.receiver
WHERE p2.year = 2024
  AND p2.month = 7
  AND p2.day = 28
  AND duration < 60;

SELECT name
FROM people AS p1
JOIN phone_calls AS p2
  ON p1.phone_number = p2.caller
WHERE p2.year = 2024
  AND p2.month = 7
  AND p2.day = 28
  AND duration < 60;

-- Bruce called and Robin was the reciever
SELECT name
FROM people AS p1
JOIN phone_calls AS p2
  ON p1.phone_number = p2.receiver
WHERE p2.year = 2024
  AND p2.month = 7
  AND p2.day = 28
  AND duration < 60
  AND caller = (SELECT phone_number FROM people WHERE name = 'Bruce');

-- Find the flight
SELECT id, hour, minute
FROM flights
WHERE year = 2024
  AND month = 7
  AND day = 29
ORDER BY hour, minute
LIMIT 1;

-- Find the destination city of flight, id = 36
SELECT city
FROM airports
WHERE id = (
    SELECT destination_airport_id
    FROM flights
    WHERE id = 36
);
