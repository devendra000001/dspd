#include <stdio.h>
#include <string.h>

#include "db.h"
#include "dbOperations.h"

struct area areaArray[areaArrayLength];
struct wasteRecord wasteRecordArray[wasteRecordArrayLength];
struct monthlySummary monthlySummaryRecord;
struct monthlyReport monthlyReportRecord;
struct metaData metaDataRecord;
/*
 init db initialises db with -1 as default value

*/
void initDataBase()
{

    metaDataRecord.areaArrayFilled = 0;
    metaDataRecord.wasteArrayFilled = 0;

    monthlySummaryRecord.totalRecycledWaste = -1;
    monthlySummaryRecord.totalWasteCollected = -1;
    monthlySummaryRecord.recyclingRate = -1.0;

    monthlyReportRecord.areaId = -1;
    monthlyReportRecord.areaRecyclingRate = -1.0;
    monthlyReportRecord.recyclingRate = -1.0;
    monthlyReportRecord.totalRecycledWaste = -1;
    monthlyReportRecord.totalWasteCollected = -1;

    for (int i = 0; i < areaArrayLength; i++)
    {
        areaArray[i].areaId = -1;
        areaArray[i].areaName[0] = '\0';
        areaArray[i].population = -1;
        areaArray[i].avgWastePerPerson = -1;
    }
    for (int i = 0; i < wasteRecordArrayLength; i++)
    {
        wasteRecordArray[i].recordId = -1;
        wasteRecordArray[i].areaIdForeignKey = -1;
        wasteRecordArray[i].collectionAgencyName[0] = '\0';
        wasteRecordArray[i].dateOfCollection[0] = '\0';
        wasteRecordArray[i].recycledWaste = -1;
        wasteRecordArray[i].totalWasteCollected = -1;
    }
}

void debugAreaDetails()
{
    for (int i = 0; i < 10; i++)
    {
        printf(":: %d :: %s :: %d ::  %.2f :: \n", areaArray[i].areaId, areaArray[i].areaName, areaArray[i].population, areaArray[i].avgWastePerPerson);
    }
}

void addArea(short addEnable)
{
    short areaIdTempHolder;
    char areaNameTempHolder[areaNamelength];
    int populationTempHolder;
    float avgWastePerPersonTempHolder;

    printf("========================================================================\n ");

    printf("Enter Area ID: ");
    scanf("%hd", &areaIdTempHolder);

    printf("Enter Area Name: ");
    scanf("%50s", areaNameTempHolder);

    printf("Enter Population: ");
    scanf("%d", &populationTempHolder);

    printf("Enter Average Waste Per Person (kg/day): ");
    scanf("%f", &avgWastePerPersonTempHolder);

    short writeIndex = checkForDuplicatesAndOrderedIndex(&metaDataRecord, areaIdTempHolder, addEnable, areaArray);
    printf("%hd\n", writeIndex);

    if (writeIndex != -1)
    {
        if (addEnable)
        {
            metaDataRecord.areaArrayFilled += 1;
        }
        areaArray[writeIndex].areaId = areaIdTempHolder;
        strcpy(areaArray[writeIndex].areaName, areaNameTempHolder);
        areaArray[writeIndex].population = populationTempHolder;
        areaArray[writeIndex].avgWastePerPerson = avgWastePerPersonTempHolder;
    }
    else
    {
        printf("Area Array Full (dbManager) \n");
    }
}

void printMetaData()
{
    printf("meta data is : %hd --- %hd", metaDataRecord.areaArrayFilled, metaDataRecord.wasteArrayFilled);
}

void addWasteRecord(short addEnable)
{
    short recordIdHolder;
    short areaIdForeignKeyHolder;
    char dateOfCollectionHolder[dateOfCollectionLength];
    int totalWasteCollectedHolder;
    int recycledWasteHolder;
    char collectionAgencyNameHolder[collectionAgencyNameLength];

    printf("========================================================================\n ");

    printf("Enter WaterRecord ID: ");
    scanf("%hd", &recordIdHolder);

    printf("Enter Area ID: ");
    scanf("%hd", &areaIdForeignKeyHolder);

    if (!checkIfAreaIdExist(areaIdForeignKeyHolder, areaArray, &metaDataRecord))
    {
        printf("Entered Area ID Dose Not exist in Records");
        return;
    }

    printf("Enter DOC Name: ");
    scanf("%10s", dateOfCollectionHolder);

    printf("Enter Total Waste Collected: ");
    scanf("%d", &totalWasteCollectedHolder);

    printf("Enter Recycle dWaste: ");
    scanf("%d", &recycledWasteHolder);

    printf("Enter Collection Agency Name: ");
    scanf("%50s", collectionAgencyNameHolder);

    short writeIndex = giveWriteIndexForWasteRecords(&metaDataRecord, areaIdForeignKeyHolder, addEnable, dateOfCollectionHolder, wasteRecordArray);
    printf(":: writing to : %d ::::::: \n", writeIndex);
    if (writeIndex != -1)
    {
        if (addEnable)
        {
            metaDataRecord.wasteArrayFilled += 1;
        }
        wasteRecordArray[writeIndex].recordId = recordIdHolder;
        wasteRecordArray[writeIndex].areaIdForeignKey = areaIdForeignKeyHolder;
        strcpy(wasteRecordArray[writeIndex].dateOfCollection, dateOfCollectionHolder);
        strcpy(wasteRecordArray[writeIndex].collectionAgencyName, collectionAgencyNameHolder);
        wasteRecordArray[writeIndex].recycledWaste = recycledWasteHolder;
        wasteRecordArray[writeIndex].totalWasteCollected = totalWasteCollectedHolder;
    }
    else
    {
        printf("Waste Record may be Full (dbManager) \n");
    }
}

void debugPrintWasteRecords()
{
    printf("\n-------------------------------------------------\n");
    printf(" DEBUG: FIRST 10 WASTE RECORDS\n");
    printf("-------------------------------------------------\n");

    for (int i = 0; i < 20; i++)
    {
        printf("::: %hd ::: %hd ::: %s ::: %d ::: %d ::: %s \n",
               wasteRecordArray[i].recordId,
               wasteRecordArray[i].areaIdForeignKey,
               wasteRecordArray[i].dateOfCollection,
               wasteRecordArray[i].recycledWaste,
               wasteRecordArray[i].totalWasteCollected,
               wasteRecordArray[i].collectionAgencyName);
    }

    printf("-------------------------------------------------\n");
}

void update_Area_or_Record()
{
    printf("Update Record of choose 1 : Area and 2 : WasteRecord\n");
    int choice;
    if (scanf("%d", &choice) != 1)
    {
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
        {
        }
        printf("Invalid input. Try again.\n");
    }
    else
    {
        switch (choice)
        {
        case 1:
            addArea(0);
            break;
        case 2:
            addWasteRecord(0);
            break;
        }
    }
}

void delete_Area_or_Record()
{
    printf("Delete Record of choose 1 : Area and 2 : WasteRecord\n");
    int choice;
    if (scanf("%d", &choice) != 1)
    {
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
        {
        }
        printf("Invalid input. Try again.\n");
    }
    else
    {
        switch (choice)
        {
        case 1:
            short areaId;
            printf("Enter Area ID: ");
            scanf("%hd", &areaId);

            if (deleteAreaRecord(&metaDataRecord, areaId, areaArray, wasteRecordArray) == -1)
            {
                printf("Error deleting \n");
            }
            else
            {
                metaDataRecord.areaArrayFilled -= 1;
                printf("Area Deleted \n");
            }
            break;
        case 2:
            short recordId;
            printf("Enter WaterRecord ID: ");
            scanf("%hd", &recordId);

            if (deleteWasteRecord(recordId, wasteRecordArray) == -1)
            {
                printf("Error deleting \n");
            }
            else
            {
                metaDataRecord.wasteArrayFilled -= 1;
                printf("Waste Deleted \n");
            }
            break;
        }
    }
}

void generateMonthlySummary()
{
    char monthYear[monthYearArrayLength]; // yyyy-mm format
    printf("Enter Month and Year (YYYY-MM) : ");
    scanf("%7s", monthYear);

    calculateMonthlySummary(monthYear, &monthlySummaryRecord, wasteRecordArray);

    printf("Total Waste Collected: %d \n", monthlySummaryRecord.totalWasteCollected);
    printf("Total Recycled Waste: %d \n", monthlySummaryRecord.totalRecycledWaste);
    printf("Recycling Rate: %f \n", monthlySummaryRecord.recyclingRate);
}

void getCityMonthlyReport()
{
    char monthYear[monthYearArrayLength]; // yyyy-mm format
    printf("Enter Month and Year (YYYY-MM) : ");
    scanf("%7s", monthYear);

    calculateMonthlyReport(monthYear, &monthlyReportRecord, wasteRecordArray);

    printf("Total Waste Collected: %d \n", monthlyReportRecord.totalWasteCollected);
    printf("Total Recycled Waste: %d \n", monthlyReportRecord.totalRecycledWaste);
    printf("Recycled Rate: %f \n", monthlyReportRecord.recyclingRate);
    printf("Highest Recycling Rate : Area ID : %hd || RR : %f", monthlyReportRecord.areaId, monthlyReportRecord.areaRecyclingRate);
}

void getAreaPerformance()
{
    short areaId;
    printf("Enter Area Id : ");
    scanf("%hd", &areaId);

    if (!checkIfAreaIdExist(areaId, areaArray, &metaDataRecord))
    {
        printf("Entered Area ID Dose Not exist in Records");
        return;
    }

    char monthYear[monthYearArrayLength]; // yyyy-mm format
    printf("Enter Month and Year (YYYY-MM) : ");
    scanf("%7s", monthYear);

    if (calculateAreaPerformance(&monthlyReportRecord, areaId, monthYear, wasteRecordArray) == -1)
    {
        printf("Error encountered \n");
    }
    else
    {
        printf("areaId : %hd \n", areaId);
        if (monthlyReportRecord.areaRecyclingRate > monthlyReportRecord.recyclingRate)
        {
            printf("Performance: Above Average (Rate: %.2f%% vs Avg: %.2f%%)\n", monthlyReportRecord.areaRecyclingRate, monthlyReportRecord.recyclingRate);
        }
        else if (monthlyReportRecord.areaRecyclingRate < monthlyReportRecord.recyclingRate)
        {
            printf("Performance: Below Average (Rate: %.2f%% vs Avg: %.2f%%)\n", monthlyReportRecord.areaRecyclingRate, monthlyReportRecord.recyclingRate);
        }
        else
        {
            printf("Performance: Average (Rate: %.2f%% vs Avg: %.2f%%)\n", monthlyReportRecord.areaRecyclingRate, monthlyReportRecord.recyclingRate);
        }
    }
}

void getHighestWasteDay(){
    char date[dateOfCollectionLength];
    calculateGetHighestWasteDay(date);
}


int saveDatabase(const char *filename)
{
    FILE *fp = fopen(filename, "wb");
    if (!fp)
    {
        perror("Failed to open file for writing");
        return -1;
    }

    size_t written = fwrite(areaArray, sizeof(areaArray[0]), (size_t)areaArrayLength, fp);
    if (written != (size_t)areaArrayLength)
    {
        fprintf(stderr, "Failed to write all area records (wrote %zu of %d)\n", written, areaArrayLength);
        fclose(fp);
        return -2;
    }

    written = fwrite(wasteRecordArray, sizeof(wasteRecordArray[0]), (size_t)wasteRecordArrayLength, fp);
    if (written != (size_t)wasteRecordArrayLength)
    {
        fprintf(stderr, "Failed to write all waste records (wrote %zu of %d)\n", written, wasteRecordArrayLength);
        fclose(fp);
        return -3;
    }

    written = fwrite(&metaDataRecord, sizeof(struct metaData), (size_t)1, fp);
    if (written != (size_t)1)
    {
        fprintf(stderr, "Failed to write (wrote %zu of %d)\n", written, wasteRecordArrayLength);
        fclose(fp);
        return -4;
    }

    fclose(fp);
    return 0;
}

int loadDatabase(const char *filename)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp)
    {
        return 1;
    }

    // Read area array
    size_t read = fread(areaArray, sizeof(areaArray[0]), (size_t)areaArrayLength, fp);
    if (read != (size_t)areaArrayLength)
    {
        fprintf(stderr, "Warning: area section reads %zu of %d entries. DB file may be corrupted/partial.\n", read, areaArrayLength);
        for (size_t i = read; i < (size_t)areaArrayLength; ++i)
        {
            areaArray[i].areaId = -1;
            areaArray[i].areaName[0] = '\0';
            areaArray[i].population = -1;
            areaArray[i].avgWastePerPerson = -1;
        }
    }

    // Read wasteRecord array
    read = fread(wasteRecordArray, sizeof(wasteRecordArray[0]), (size_t)wasteRecordArrayLength, fp);
    if (read != (size_t)wasteRecordArrayLength)
    {
        fprintf(stderr, "Warning: wasteRecord section reads %zu of %d entries. DB file may be corrupted/partial.\n", read, wasteRecordArrayLength);
        for (size_t i = read; i < (size_t)wasteRecordArrayLength; ++i)
        {
            wasteRecordArray[i].recordId = -1;
            wasteRecordArray[i].areaIdForeignKey = -1;
            wasteRecordArray[i].collectionAgencyName[0] = '\0';
            wasteRecordArray[i].dateOfCollection[0] = '\0';
            wasteRecordArray[i].recycledWaste = -1;
            wasteRecordArray[i].totalWasteCollected = -1;
        }
    }

    read = fread(&metaDataRecord, sizeof(struct metaData), (size_t)1, fp);
    if (read != (size_t)1)
    {
        metaDataRecord.areaArrayFilled = 0;
        metaDataRecord.wasteArrayFilled = 0;
    }

    fclose(fp);
    return 0;
}