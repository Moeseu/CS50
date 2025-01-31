-- Keep a log of any SQL queries you execute as you solve the mystery.
--read reports
SELECT * FROM crime_scene_reports WHERE day = 28 AND year = 2023 AND month = 7;

--read interviews
SELECT * FROM interviews WHERE transcript LIKE '%bakery%' AND month = 7;

--find the id of people who withdrew cash
SELECT ac.person_id FROM atm_transactions AS tr JOIN bank_accounts AS ac ON tr.account_number = ac.account_number WHERE tr.day = 28 AND tr.year = 2023 AND tr.month = 7 AND atm_location LIKE '%Leggett%';


SELECT * FROM flights WHERE origin_airport_id IN (SELECT id FROM airports WHERE city LIKE '%Fiftyville%');

SELECT * FROM flights AS fl JOIN passengers AS ps ON ps.flight_id = fl.id WHERE fl.origin_airport_id IN (SELECT id FROM airports WHERE city LIKE '%Fiftyville%') AND fl.day = 29 AND fl.year = 2023 AND fl.month = 7;

--List of unique phone numbers of people who
--flew on July 29, 2023 from an airport in Fiftyville.
--Made a bank transaction on July 28, 2023 at Leggett.
SELECT DISTINCT pe.license_plate
FROM flights AS fl
JOIN passengers AS ps ON ps.flight_id = fl.id
JOIN people AS pe ON pe.passport_number = ps.passport_number
JOIN airports AS ap ON fl.origin_airport_id = ap.id
JOIN atm_transactions AS tr ON tr.account_number IN (
    SELECT ac.account_number
    FROM bank_accounts AS ac
    WHERE ac.person_id = pe.id
)
WHERE ap.city LIKE '%Fiftyville%'
  AND fl.day = 29
  AND fl.year = 2023
  AND fl.month = 7
  AND tr.day = 28
  AND tr.year = 2023
  AND tr.month = 7
  AND tr.atm_location LIKE '%Leggett%';

--car = name
SELECT *
FROM bakery_security_logs
WHERE day = 28
  AND month = 7
  AND year = 2023
  AND activity = 'exit'
  AND license_plate IN (
      SELECT DISTINCT pe.license_plate
      FROM flights AS fl
      JOIN passengers AS ps ON ps.flight_id = fl.id
      JOIN people AS pe ON pe.passport_number = ps.passport_number
      JOIN airports AS ap ON fl.origin_airport_id = ap.id
      JOIN atm_transactions AS tr ON tr.account_number IN (
          SELECT ac.account_number
          FROM bank_accounts AS ac
          WHERE ac.person_id = pe.id
      )
      WHERE ap.city LIKE '%Fiftyville%'
        AND fl.day = 29
        AND fl.year = 2023
        AND fl.month = 7
        AND tr.day = 28
        AND tr.year = 2023
        AND tr.month = 7
        AND tr.atm_location LIKE '%Leggett%'
  );

--combine everything for now + time
SELECT pe.name
FROM bakery_security_logs AS bsl
JOIN people AS pe ON bsl.license_plate = pe.license_plate
JOIN bank_accounts AS ba ON ba.person_id = pe.id
JOIN atm_transactions AS tr ON tr.account_number = ba.account_number
JOIN passengers AS ps ON ps.passport_number = pe.passport_number
JOIN flights AS fl ON ps.flight_id = fl.id
JOIN airports AS ap ON fl.origin_airport_id = ap.id
JOIN phone_calls AS pc ON pc.caller = pe.phone_number
WHERE bsl.activity = 'exit'
  AND bsl.day = 28
  AND bsl.month = 7
  AND bsl.year = 2023
  AND ap.city LIKE '%Fiftyville%'
  AND fl.day = 29
  AND fl.year = 2023
  AND fl.month = 7
  AND tr.day = 28
  AND tr.year = 2023
  AND tr.month = 7
  AND tr.atm_location LIKE '%Leggett%'
  AND pc.year = 2023
  AND pc.month = 7
  AND pc.day = 28
  AND pc.duration < 60
  ORDER BY fl.hour, fl.minute LIMIT 1;

SELECT city FROM airports WHERE id = 4;

SELECT p.name
FROM phone_calls AS ph
JOIN people AS p ON p.phone_number = ph.receiver
WHERE ph.caller = (
    SELECT p.phone_number
    FROM people AS p
    WHERE p.name = 'Bruce'
)
AND ph.year = 2023
AND ph.month = 7
AND ph.day = 28
AND ph.duration < 60;

