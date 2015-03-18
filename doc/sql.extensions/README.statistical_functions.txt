---------------------
Statistical Functions
---------------------

By the SQL specification, some statistical functions are defined.
Function about variance and standard deviation are bellow.

VAR_POP: return the population variance.
VAR_SAMP: return  the sample variance.
STDDEV_SAMP: return the sample standard deviation .
STDDEV_POP: return the population standard deviation.

VAR_POP(<expr>) is equivalent to (SUM(<expr> ^ 2) - SUM(<expr>) ^ 2 / COUNT(<expr>)) / COUNT(<expr>).
VAR_SAMP(<expr>) is equivalent to (SUM(<expr> ^ 2) - SUM(<expr>) ^ 2 / COUNT(<expr>)) / (COUNT(<expr>) - 1).
STDDEV_POP(<expr>) is equivalent to SQRT(VAR_POP(<expr>)).
STDDEV_SAMP(<expr>) is equivalent to SQRT(VAR_SAMP(<expr)).

Author:
    Hajime Nakagami <nakagami@gmail.com>

Syntax:
    <statistical function> ::= <statistical function name>(<expr>)
    <statistical function name> := { VAR_POP | VAR_SAMP | STDDEV_POP | STDDEV_SAMP }

Example:
    SELECT STDDEV_SAMP(salary) FROM employees;
