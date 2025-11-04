
void initDataBase();
void debugAreaDetails();
void addArea(short addEnable);
void addWasteRecord(short addEnable);
void debugPrintWasteRecords();
void generateMonthlySummary();
void getCityMonthlyReport();
void update_Area_or_Record();
void delete_Area_or_Record();
void printMetaData();
void getAreaPerformance();

int saveDatabase(const char *filename);
int loadDatabase(const char *filename);