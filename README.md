# calculator

This experiment has realized four arithmetic topic generators, which support mode selection and parameter setting.

Instructions:

Several formulas will be generated, including integers in the range [0,20) 
and operators: addition[+]/subtraction[-]/multiplication[*]/division[/]/power[**|^].
Brackets['(' and ')'] is also supported.
Note that the number of operators cannot exceed 10.
And the answer and intermediate calculation result won't be larger than 1e10 or smaller than 1e-8.
There is no decimals or root symbols, such as "0.123" or "√2" [2^(0.5)].
Give your answer in the format including no more than one "+" operator with integers and fractions.
Such as 123/45, 1+78/45, 2+33/45.
After the program exits, you can see the log file in folder "./logs".
Start the program with "start 10" and enjoy!
=============================commands=============================================================================================
It's not case sensitive.
enter "exit" or "quit" to force exit.
enter "setting" to view settings in any time.
To change power type, enter "setpower"+[number], e.g. "setpower 1".
To change operation numbers, enter "setopnum"+[number], e.g. "setopnum 5".
If you are ready, enter "start"+[number of formulas], e.g. "start 10".
To submit an answer, enter "answer"+[your answer], e.g. "answer 5/2".
To submit an custom formula and ask for an answer, enter "calc"+[your formula], e.g "calc 2^(-3)**3".

