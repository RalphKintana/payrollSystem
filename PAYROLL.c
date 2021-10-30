//Each worker works 5 days a week requirement
//Eight hours per day 08:00 - 17:00
//Each employee has P500.00  weekly allowance
/*
	Tax deduction is 10% of weekly gross salary income
	GSIS (1% for level 1, 1.5% for level 2 and 2% for level 3 of the basic salary rate of the worker)
	


	Late(in hours) = time in - 8:00 ----------------- if time in <= 8:00 = Late is 0
	Undertime (in hours) = 17:00 - time out --------- time out >= 17:00 = undertime is 0
	Daily workhours = 8hours - (late + undertime) --- if person is absent, 8 hours should be replaced by zero
	Overtime = overtime out - overtime in ----------- overtime starts at 17:300 and ends at 20:30
	
	overtime income = summation of(overtime x overtime rate per hour)
	regular income = summation of(hourly rate x daily work hours)
	Weekly Gross Salary Income (WGSI) = regular income + overtime
	Weekly Net Salary Income = (WGSI - (Tax deduction + GSIS)) + allowance
	Total number of work hours = summation of daily work hours
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define lvl 3
#define SIZE 100
#define wk 5
#define MAX 999

typedef char Week[5][10];

typedef struct{
	char employeeCode[10];
	char employeeName[32];
	int salaryLevel;
}Employee;

typedef struct{
	Employee records[SIZE];
	int count;
}EmployeeRecord;

typedef struct{
	int hour;
	int min;
}Time;

typedef struct{
	char type [30];
	Time timeIn;
	Time timeOut;
	Time overtimeIn;
	Time overtimeOut;
}DailyTime;

typedef struct{
	char employeeCode[10];
	DailyTime time[wk];
	char dateCovered[20];
}WeekTime;

typedef struct{
	WeekTime records[MAX];
	int size;
}DailyRecord;

double overtimeIncome(double salary, int overtime); // 	calculatees the overtime income of employee
void writeToFileEmp(EmployeeRecord records);  // writes a file together with sample employee
EmployeeRecord* readFromFileEmp(void); // read from existing employee.txt file
Employee* findEmp(EmployeeRecord records, char empCode[]); // find employee
WeekTime* weekTimeInOut(Week week,Employee emp); // time in and out for the whole week
void insertToRecord(WeekTime time, DailyRecord *data);  //records the time in 
void writeToFileDailyRec(DailyRecord data);  //a file of daily record of all employee
DailyRecord* readFromFileDailyRec(void); // reads the file of daily rec
void displayDailyRecord(DailyRecord data); // displays the daily record of employee
float totalWorkHours(WeekTime time); // calculate all the total working hours of an employee
int overtimeHours(WeekTime time); // calculates the overtime hours of an employee
double regIncome(float hours, double rate, WeekTime time); // calculates the regular income of an employee
void displayEmployee(Employee emp, double level[]); // display employee
void displayEmployeeRec(EmployeeRecord records, double level[]); // display employee records
void displayInfo(float hours, double regIn, char dateCov [], int overtime, double overIn, int l, double level []);

int main()
{
	int x, m, overtime = 0, choice1, choice2;
	float hours;
	double regIn, overIn = 0, level[] = {380.00,450.00,550.00};
	Week week = {"Monday","Tuesday","Wednesday","Thursday","Friday"};
	EmployeeRecord records = {{{"B51-1001","Jason Avanceña",1},
								{"A02-0002","Brant Uy",2},
								{"C03-0103","Chris Drape",3},
								{"Z02-6104","Ralph Kintana",1},
								{"O03-5555","Rayl Xylem",2}},5};
	WeekTime arr[] = {{"C03-0103",{{"Regular",{8,00},{17,01},{0,00},{0,00}},{"Regular",{8,00},{17,01},{0,00},{0,00}},{"Regular",{8,00},{17,01},{0,00},{0,00}},{"Regular",{8,00},{17,01},{0,00},{0,00}},{"Regular",{8,00},{17,01},{0,00},{0,00}}},"June 20-25, 2019"},
						{"O03-5555",{{"Regular",{8,00},{17,01},{0,00},{0,00}},{"Regular",{8,00},{17,01},{0,00},{0,00}},{"Regular",{8,00},{17,01},{0,00},{0,00}},{"Regular",{8,00},{17,01},{0,00},{0,00}},{"Regular",{8,00},{17,01},{0,00},{0,00}}},"June 20-25, 2019"},
						{"Z02-6104",{{"Regular",{8,00},{16,45},{0,00},{0,00}},{"Regular",{8,00},{17,01},{0,00},{0,00}},{"Regular",{13,00},{17,01},{0,00},{0,00}},{"Regular",{8,15},{17,01},{0,00},{0,00}},{"Regular",{7,30},{17,01},{0,00},{0,00}}},"June 20-25, 2019"}};
	
	WeekTime arr2[] = {{"B51-1001",{{"Regular",{8,00},{17,01},{17,30},{20,30}},{"Regular",{8,00},{17,01},{17,03},{18,30}},{"Regular",{8,00},{17,01},{0,00},{0,00}},{"Holiday",{8,00},{17,01},{0,00},{0,00}},{"Regular",{8,00},{17,01},{0,00},{0,00}}},"June 20-25, 2019"},
						{"C03-0103",{{"Regular",{8,00},{17,01},{17,30},{20,30}},{"Regular",{13,00},{17,01},{17,30},{18,30}},{"Regular",{8,00},{17,01},{0,00},{0,00}},{"Holiday",{8,00},{17,01},{0,00},{0,00}},{"Regular",{8,00},{17,01},{0,00},{0,00}}},"June 20-25, 2019"}};
		
	EmployeeRecord *rec;
	Employee *emp;
	char empCode[10];
	WeekTime *time;
	DailyRecord data;
	DailyRecord *dat;
	data.size = 0;
	rec = NULL;
	emp = NULL;
	time = NULL;
	dat = NULL;
	
	
//	writeToFileEmp(records); // e comment out if naa nay employee.txt file if wala kai dli e comment out
	
	
	printf("\n0: End \t 1: Run test case sample \t 2: More info\nEnter command: ");
	scanf("%d",&choice1);
	
	
	if(choice1 != 0){
		do{
			if(choice1==1){
					rec = readFromFileEmp();
					displayEmployeeRec(*rec,level);
					for(x=0; x < 2; x++){
						data.records[data.size] = arr2[x];
						emp = findEmp(*rec,arr2[x].employeeCode);
						hours = totalWorkHours(arr2[x]);
						regIn = regIncome(hours, level[emp->salaryLevel-1], arr2[x]);
						overtime = overtimeHours(arr2[x]);
						overIn = overtimeIncome(level[emp->salaryLevel-1], overtime);
						
						displayEmployee(*emp,level);
						displayInfo(hours, regIn, data.records[data.size].dateCovered, overtime, overIn, emp->salaryLevel, level);
				
						data.size++;
					}
			}else if(choice1==2){
					rec = readFromFileEmp();
					printf("\n\n0 - End \t 1: Insert Employee \t 2: Search employee \t 3: time-in time-out\nEnter command: ");
					scanf("%d",&choice2);
					do{
						if(choice2==1){
								rec = readFromFileEmp();
								displayEmployeeRec(*rec,level);
								printf("Enter Employee Code: ");
								fflush(stdin);
								gets(rec->records[rec->count].employeeCode);
								printf("Enter Employee Name: ");
								fflush(stdin);
								gets(rec->records[rec->count].employeeName);
								printf("Enter Salary Level: ");
								fflush(stdin);
								scanf("%d", &rec->records[rec->count].salaryLevel);
								
								displayEmployee(rec->records[rec->count],level);
								rec->count++;
								writeToFileEmp(*rec);
		
					}else if(choice2==2){
						
					
								rec = readFromFileEmp();
								displayEmployeeRec(*rec,level);
								printf("\nEmployee Code: ");
								fflush(stdin);
								gets(empCode);
								emp = findEmp(*rec,empCode);
								
								displayEmployee(*emp,level);
								
						}else if(choice2==3){
							
								rec = readFromFileEmp();
								displayEmployeeRec(*rec,level);
								do{
									printf("\nEmployee Code: ");
									fflush(stdin);
									gets(empCode);
									emp = findEmp(*rec,empCode);
								}while(strcmp(emp->employeeCode,"NULL") == 0);
							
								displayEmployee(*emp,level);
								time = weekTimeInOut(week,*emp);
								hours = totalWorkHours(*time);
								insertToRecord(*time, &data);
								regIn = regIncome(hours, level[emp->salaryLevel-1], *time);
								
								displayEmployee(*emp,level);
								displayInfo(hours, regIn, time->dateCovered, overtime, overIn, emp->salaryLevel, level);
								
								writeToFileDailyRec(data);		
						}
					printf("\n\n0 - End \t 1: Insert Employee \t 2: Search employee \t 3: time-in time-out\nEnter command: ");
						scanf("%d",&choice2);
						system("cls");
					}while(choice2 != 0);						
			}

			printf("\n0: End \t 1: Run test case sample \t 2: More info\nEnter command: ");
			scanf("%d",&choice1);
			system("cls");
		}while(choice1 != 0);
	}
	
	printf("\nProgram ended");
	return 0;
}
double overtimeIncome(double salary, int overtime)
{
	double rate, income, temp;
	temp = overtime;
	rate = (salary/8)*1.1;
	income = temp * rate;
	return income;
}

int overtimeHours(WeekTime time)
{
	int x, h, m, hours = 0;
	for(x=0; x < wk; x++){
		if(time.time[x].overtimeIn.hour > 0){
			h = time.time[x].overtimeOut.hour - time.time[x].overtimeIn.hour;
			m = time.time[x].overtimeOut.min - time.time[x].overtimeIn.min;
			hours += h;
		}
	}
	return hours;
}

void writeToFileEmp(EmployeeRecord records)
{
	FILE *fp;
	fp = fopen("employee.txt","w");
	if(fp != NULL){
		if(records.count != 0){
			fwrite(&records, sizeof(EmployeeRecord),1,fp);
		}
	}
	fclose(fp);
}

EmployeeRecord* readFromFileEmp(void)
{
	EmployeeRecord* records;
	FILE *fp;
	int x;
	records = (EmployeeRecord*)malloc(sizeof(EmployeeRecord));
	if(records != NULL){
		fp = fopen("employee.txt","r");
		if(fp != NULL){	
			while(fread(records, sizeof(EmployeeRecord),1,fp)){}
		}
		fclose(fp);
	}
	return records;
}

Employee* findEmp(EmployeeRecord records, char empCode[])
{
	int x;
	Employee* emp;
	emp = (Employee*)malloc(sizeof(Employee));
	if(emp != NULL){
		for(x=0; x < records.count && strcmpi(records.records[x].employeeCode,empCode) != 0; x++){}
		if(x < records.count){
			memcpy(emp,&records.records[x],sizeof(Employee));
		}else{
			strcpy(emp->employeeCode,"NULL");
		}
	}
	return emp;
}

WeekTime* weekTimeInOut(Week week, Employee emp)
{
	int x, type;
	WeekTime* time;
	time = (WeekTime*)malloc(sizeof(WeekTime));
	if(time != NULL){
		for(x = 0; x < wk; x++){
			printf("\nEnter time-in for %s: ", week[x]);
			fflush(stdin);
			scanf("%d:%d:",&time->time[x].timeIn.hour,&time->time[x].timeIn.min);
			
			printf("Enter time-out for %s: ", week[x]);
			fflush(stdin);
			scanf("%d:%d",&time->time[x].timeOut.hour,&time->time[x].timeOut.min);
			
			printf("\n\n%s \nTime-in: %02d:%02d \nTime-out: %02d:%02d\n\n", week[x], time->time[x].timeIn.hour,time->time[x].timeIn.min,time->time[x].timeOut.hour,time->time[x].timeOut.min);
			
			printf("Choice: 0 - holiday \t 1 - Regular \nChoice?");
			scanf("%d", &type);
			if(type == 0){
				strcpy(time->time[x].type,"Holiday");
			}else{
				strcpy(time->time[x].type,"Regular");
			}
			
			printf("\nEnter Overtime-in for %s: ", week[x]);
			fflush(stdin);
			scanf("%d:%d:",&time->time[x].overtimeIn.hour,&time->time[x].overtimeIn.min);
			
			printf("Enter Overtime-out for %s: ", week[x]);
			fflush(stdin);
			scanf("%d:%d",&time->time[x].overtimeOut.hour,&time->time[x].overtimeOut.min);
		}
		
		printf("Enter the coverage date of this payroll: ");
		fflush(stdin);
		gets(time->dateCovered);
		strcpy(time->employeeCode,emp.employeeCode);
		printf("\n\nDate Covered: %s \nEmployeeCode: %s\n", time->dateCovered,time->employeeCode);
	}
	return time;
}

void insertToRecord(WeekTime time, DailyRecord *data)
{
	data->records[data->size] = time;
	data->size++;
}

void writeToFileDailyRec(DailyRecord data)
{
	FILE *fp;
	fp = fopen("dtr.txt","w");
	if(fp != NULL){
		printf("\n\nThis is before writing to file -- No. of records : %d\n\n",data.size);
		if(data.size != 0){
			fwrite(&data,sizeof(DailyRecord),1,fp);
		}
	}
	fclose(fp);
}

DailyRecord* readFromFileDailyRec(void)
{
	DailyRecord* dat;
	FILE *fp;
	dat = (DailyRecord*)malloc(sizeof(DailyRecord));
	if(dat != NULL){
		fp = fopen("dtr.txt","r");
		if(fp != NULL){	
			while(fread(dat,sizeof(DailyRecord),1,fp)){}
		}
		fclose(fp);
	}
	return dat;
}


float totalWorkHours(WeekTime time)
{
	int x;
	float h, m, h2, m2, temp = 0, temp2, late, under, dw;
	for(x=0; x < wk; x++){
		h = time.time[x].timeIn.hour;
		m = time.time[x].timeIn.min;
		h2 = time.time[x].timeOut.hour;
		m2 = time.time[x].timeOut.min;
		if(h >= 13){
			temp2 = (h-8-1)*60 + m;
			late = temp2/60;
		}else if(h < 8 || h == 8 && m == 0){
			late = 0;
		}else{
			temp2 = (h-8)*60 + m;
			late = temp2/60;
		}
		if(h2>=17){
			under = 0;
		}else{
			temp2 = (17-h2)*60 - m2;
			under = temp2/60;
		}
		dw = 8 - (late + under);
		temp += dw;
	}
	return temp;
}

double regIncome(float hours, double rate, WeekTime time)
{
	int x;
	float h, m, h2, m2, late, under, dw, total = 0;
	double reg, hr, hol, income;
	char type [] = "holiday";
	for(x=0; x < wk; x++){
		if(strcmpi(time.time[x].type,type)==0){
			h = time.time[x].timeIn.hour;
			m = time.time[x].timeIn.min;
			h2 = time.time[x].timeOut.hour;
			m2 = time.time[x].timeOut.min;
			late = (h <= 8 && m == 0)? 0 : h-8;
			late += (m >= 30)? 0.5:0;
			under = (h2 >= 17 && m2 == 0)? 0: (17-h2);
			dw = 8 - (late + under);
			total += dw;
		}
	}
	if(total > 0){
		hr = (rate/8)*1.1;
		hol = hr * total;
		reg = rate/8 * (hours-total);
		income = hol + reg;
	}else{
		income = rate/8 * hours;
	}
	
	return income;
}

void displayEmployee(Employee emp, double level[])
{
	printf("\n********************************************************");
	printf("\nEmployee Information:\n\n");
	printf("Employee Name : %5s \nEmployee Code : %10s \nSalary Level: Level %2d \nSalary Rate : Php %.02f /day \n\n", emp.employeeName,emp.employeeCode,emp.salaryLevel,level[emp.salaryLevel-1]);
	printf("********************************************************");
}

void displayEmployeeRec(EmployeeRecord records, double level[])
{
	int x;
	printf("***************** Employee Records ****************\n\n");
	printf("Employee Name \t\tEmployee Code \tSalary Level \tSalary Rate\n\n");
	for(x=0; x < records.count; x++){
		printf("%-22s %10s \t Level %2d \t Php %.02f /day \n", records.records[x].employeeName,records.records[x].employeeCode,records.records[x].salaryLevel,level[records.records[x].salaryLevel-1]);
	}
}

void displayInfo(float hours, double regIn, char dateCov [],int overtime, double overIn, int l, double level [])
{
	int hour = hours;
	float d [] = {0.01,0.015,0.02};
	double tax, sss, netIn, grossIn = regIn;
	grossIn = regIn + overIn;
	printf("\nDate Covered: %s \nTotal number of Work Hours: %.01f Hrs. \nOvertime Hours: %d Hrs. \nRegular Income: Php %.02f \nOvertime Income: Php %.02f \nGross Income: Php %.02f", dateCov, hours, overtime, regIn, overIn, grossIn);
	tax = grossIn * 0.10;
	sss = d[l-1]*level[l-1];
	netIn = (grossIn - (tax + sss)) + 500.00;
	printf("\nDeductions: \n\t Tax: Php %.02f \n\t SSS: Php %.02f \n Net Income: Php %.02f", tax, sss, netIn);
}

void displayDailyRecord(DailyRecord data)
{
	int x;
	printf("\n\n\n***************** Employee TimeIn/Out Records ****************\n\n");
	printf("Employee Code \tMonday \t\tTuesday \tWednesday \tThursday \tFriday\n\n");
	printf("\t\tRegular Overtime \tRegular Overtime \tRegular Overtime \tRegular Overtime \tRegular Overtime \n");
	printf("\t\tIN OUT \tIN OUT \tIN OUT \tIN OUT \tIN OUT\n");
	for(x=0; strlen(data.records[x].employeeCode) != 0; x++){

	printf("%10s %02d:%02d %02d:%02d | %02d:%02d %02d:%02d | %02d:%02d %02d:%02d | %02d:%02d %02d:%02d | %02d:%02d %02d:%02d | %02d:%02d %02d:%02d | %02d:%02d %02d:%02d | %02d:%02d %02d:%02d | %02d:%02d %02d:%02d | %02d:%02d %02d:%02d",data.records[x].employeeCode, 
				data.records[x].time[0].timeIn.hour,data.records[x].time[0].timeIn.min,data.records[x].time[0].timeOut.hour,data.records[x].time[0].timeOut.min,
				data.records[x].time[0].overtimeIn.hour,data.records[x].time[0].overtimeIn.min,data.records[x].time[0].overtimeOut.hour,data.records[x].time[0].overtimeOut.min,
				data.records[x].time[1].timeIn.hour,data.records[x].time[1].timeIn.min,data.records[x].time[1].timeOut.hour,data.records[x].time[1].timeOut.min,
				data.records[x].time[1].overtimeIn.hour,data.records[x].time[1].overtimeIn.min,data.records[x].time[1].overtimeOut.hour,data.records[x].time[1].overtimeOut.min,
				data.records[x].time[2].timeIn.hour,data.records[x].time[2].timeIn.min,data.records[x].time[2].timeOut.hour,data.records[x].time[2].timeOut.min,
				data.records[x].time[2].overtimeIn.hour,data.records[x].time[2].overtimeIn.min,data.records[x].time[2].overtimeOut.hour,data.records[x].time[2].overtimeOut.min,
				data.records[x].time[3].timeIn.hour,data.records[x].time[3].timeIn.min,data.records[x].time[3].timeOut.hour,data.records[x].time[3].timeOut.min,
				data.records[x].time[3].overtimeIn.hour,data.records[x].time[3].overtimeIn.min,data.records[x].time[3].overtimeOut.hour,data.records[x].time[3].overtimeOut.min,
				data.records[x].time[4].timeIn.hour,data.records[x].time[4].timeIn.min,data.records[x].time[4].timeOut.hour,data.records[x].time[4].timeOut.min,
				data.records[x].time[4].overtimeIn.hour,data.records[x].time[4].overtimeIn.min,data.records[x].time[4].overtimeOut.hour,data.records[x].time[4].overtimeOut.min);
	}
}
