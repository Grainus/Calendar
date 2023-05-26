SELECT STRING_AGG(num, ' ') Calendar
FROM (
    SELECT
        monthlen,
        beg,
        (
            CASE
                WHEN iter BETWEEN 1 AND monthlen THEN LPAD(iter::TEXT, 2, '0')
                ELSE '  '
            END
        ) AS num
    FROM (
        SELECT
            monthlen,
            beg,
            GENERATE_SERIES(beg, beg + 6) iter
        FROM (
            SELECT
                monthlen,
                GENERATE_SERIES(
                    (EXTRACT(DAY FROM mon) - EXTRACT(DOW FROM mon)) % 7,
                    monthlen,
                    7
                ) beg
                FROM (
                    SELECT
                        mon,
                        EXTRACT(DAY FROM (
                            DATE_TRUNC('MONTH', mon + INTERVAL '1 MONTH')
                            - DATE_TRUNC('MONTH', mon)
                        )) monthlen
                    FROM (
                        SELECT NOW() mon -- OR '2020-02-05'::DATE
                    ) date
                ) month_date
            ) week
        ) iter
    ) res
GROUP BY res.beg
ORDER BY beg;
