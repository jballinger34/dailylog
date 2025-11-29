#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h> 


typedef struct {
	//date
	int day;
	int month;
	int year;

	//categories
	int water;
	int protein;
} DailyLog;

#define LOG_FILE "daily.log"

DailyLog *logs = NULL;
int logCount = 0;

char *getDateString() {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    char *datebuffer = malloc(11);

    snprintf(datebuffer, 11, "%d/%d/%d",
             t->tm_mday,
             t->tm_mon + 1,
             t->tm_year + 1900);

    return datebuffer;
}

int countLines(char *fileName){
	FILE *fp = fopen(fileName, "r");
	if(!fp)	return 0;
	int lines = 0;
	char buffer[255];
	while(fgets(buffer, 255, fp)){
		lines++;
	}

	fclose(fp);
	return lines;
}

void loadLogsToMemory(){
	logCount = countLines(LOG_FILE);
	logs = calloc(logCount == 0 ? 1 : logCount, sizeof(DailyLog));

	FILE *fp = fopen(LOG_FILE, "r");
	if(!fp){	
		fp = fopen(LOG_FILE, "w");	
	}
	int i = 0;
	while(fscanf(fp, "%d/%d/%d water=%d protein=%d", &logs[i].day, 
				&logs[i].month, &logs[i].year,
				&logs[i].water, &logs[i].protein) == 5) {
		i++;
	}
	fclose(fp);
}

DailyLog *getTodaysLog(){
	char *today = getDateString();

	int day, month, year;
	scanf(today, "%d/%d/%d", day, month, year);
	free(today);
	for(int i = 0; i < logCount; i++){
		if(logs[i].day == day && logs[i].month == month && logs[i].year == year){
			return &logs[i];		
		}
	}
	logs = realloc(logs, sizeof(DailyLog) * (logCount + 1));
	logs[logCount].day = day;
	logs[logCount].month = month;
	logs[logCount].year = year;
	
	logCount++;
	return &logs[logCount - 1];

}


void addWater(int amount){
	DailyLog *today = getTodaysLog();
	today->water += amount;
}
void addProtein(int amount){
	DailyLog *today = getTodaysLog();
	today->protein += amount;
}


void saveLogs(){
	FILE *fp = fopen(LOG_FILE, "w");

	if(!fp) return;

	for(int i = 0; i < logCount; i++){
		fprintf(fp,"%d/%d/%d water=%d protein=%d\n", logs[i].day, logs[i].month, logs[i].year, logs[i].water, logs[i].protein);
	}

	fclose(fp);

}

void printUsage(){
	printf("Usage:\n");
	printf("dailylog log <category> <amount> <unit>\n");
	printf("dailylog summary <category>\n");
}

int printSummary(char *category){
	//atm prints a monthly average for the logged value!
	
	char* today = getDateString();
	int day, month, year;
	sscanf(today, "%d/%d/%d", &day, &month, &year);
	free(today);
	
	int monthlySum = 0;

	if(strcmp("water", category) == 0){	
		for(int i = 0; i < logCount; i++){
			if(logs[i].month == month && logs[i].year == year){
				monthlySum += logs[i].water;
			}
		}
		int monthlyAvg = monthlySum / day;
		printf("This months averaage daily water intake: %dml\n", monthlyAvg);
		return 0;
	}	
	if(strcmp("protein", category) == 0){
	
		for(int i = 0; i < logCount; i++){
			if(logs[i].month == month && logs[i].year == year){
				monthlySum += logs[i].protein;
			}
		}
		int monthlyAvg = monthlySum / day;
		printf("This months averaage daily protein intake: %dg\n", monthlyAvg);
		return 0;
	
	}
	return 1;
}

int handleArgs(int argc, char **argv){
	if(argc < 2){
		return 1;
	}
	
	char* option = argv[1];
	if(strcmp("log", option) == 0){
		
		if(argc < 4){
			return 1;
		}
		char* category = argv[2];
		char* amount = argv[3];

		if(strcmp("water", category) == 0){
			int amt = atoi(amount);
			if(argc >= 5){
				char* unit = argv[3];
				if(strcmp("l", unit) == 0){
					amt = amt*1000;
				}
				if(strcmp("pint", unit) == 0){
					amt = amt*568;
				}
			}
			addWater(amt);
		}

	
		if(strcmp("protein", category) == 0 ){
			int amt = atoi(amount);
			addProtein(amt);
		}
	}
	if(strcmp("summary",option) == 0){
		if(argc < 3){
			return 1;
		}
		char* category = argv[2];
		return printSummary(category);
	}	
	
	return 0;
}

int main(int argc, char **argv){
	loadLogsToMemory();
	
	if(handleArgs(argc, argv) == 1){
		printUsage();
	}
	
	saveLogs();
	free(logs);
	return 0;
}
