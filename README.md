# dailylog
A terminal tool allowing you to log nutritional macros

## Usage
    dailylog log <category> <amount> <?unit>
    dailylog summary <category> 
2 base commands, log and summary

### log
Used to when you want to store data. At the moment you can only log 2 categories, water and protein, you specify the amount and (optionally) a unit, and the program will log that to the daily.log file, automatically converting the amount using the unit provied (or g and ml as a default). Multiple logs on the same day combine so it tracks your macros for the day.

Note: I'm still unsure whether to develop the categories to be flexible and user defined, or pre-set (as they are currently).

### summary
Used to review your progress, currently just gets you average daily for the category provided, but in the future will display a wide range of information, i.e. longest streak*, days missed, total, summary statistics and potentially display it visually.

*will add a command to set a streak target

