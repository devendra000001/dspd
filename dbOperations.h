short checkForDuplicatesAndOrderedIndex(struct metaData *metaDataRecord,short areaId, short addEnable, struct area *areaPointer);
short giveWriteIndexForWasteRecords(struct metaData *metaDataRecord,short areaId, short addEnable, char dateOfCollection[], struct wasteRecord *wasteRecordArray);
short checkIfAreaIdExist(short areaId, struct area *areaArray,struct metaData *metaDataRecord);
void calculateMonthlySummary(char monthYear[], struct monthlySummary *monthlySummaryRecord, struct wasteRecord *wasteRecordArray);
void calculateMonthlyReport(char monthYear[], struct monthlyReport *monthlyReportRecord, struct wasteRecord *wasteRecordArray);
short deleteWasteRecord(short recordId, struct wasteRecord *wasteRecordArray);
short deleteAreaRecord(struct metaData *metaDataRecord,short areaId, struct area *areaArray,struct wasteRecord *wasteRecordArray);
short calculateAreaPerformance( struct monthlyReport *monthlyReportRecord,short areaId,char month[],struct wasteRecord *wasteRecordArray);
void  calculateGetHighestWasteDay(char date[]);