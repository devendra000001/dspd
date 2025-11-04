#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "db.h"

short giveWriteIndexForDateOfCollection(short firstSeen, short firstOccurrenceOfMinusOne, short areaId, char dateOfCollection[], struct wasteRecord *wasteRecordArray);
short checkIfAreaIdExist(short areaId, struct area *areaArray, struct metaData *metaDataRecord);
void calculateMonthlySummary(char monthYear[], struct monthlySummary *monthlySummaryRecord, struct wasteRecord *wasteRecordArray);
void calculateMonthlyReport(char monthYear[], struct monthlyReport *monthlyReportRecord, struct wasteRecord *wasteRecordArray);
short removeAssociatedWasteRecordsWithArrayId(short areaId, struct wasteRecord *wasteRecordArray);

short checkForDuplicatesAndOrderedIndex(struct metaData *metaDataRecord, short areaId, short addEnable, struct area *areaArray)
{
    short midIndex, low = 0, high = 0;
    short firstOccurrenceOfMinusOne = 0;

    // while (high < areaArrayLength && areaArray[high].areaId != -1)
    // {
    //     printf("arr value : %hd \n", areaArray[high].areaId);
    //     high++;
    // }
    firstOccurrenceOfMinusOne = metaDataRecord->areaArrayFilled;
    high = firstOccurrenceOfMinusOne - 1;

    printf("first run : %hd ---- %hd ---- %hd\n", low, high, firstOccurrenceOfMinusOne);

    while (low <= high)
    {
        midIndex = (high - low) / 2 + low;
        if (areaArray[midIndex].areaId == areaId)
        {
            printf("Duplicate Found !\n");
            return midIndex;
        }
        else if (areaArray[midIndex].areaId > areaId)
        {
            high = midIndex - 1;
        }
        else
        {
            low = midIndex + 1;
        }
        printf("%hd----%hd----%hd\n", low, high, midIndex);
    }

    if (!addEnable)
    {
        return -1;
    }

    if (firstOccurrenceOfMinusOne < areaArrayLength)
    {
        while (firstOccurrenceOfMinusOne > low)
        {
            areaArray[firstOccurrenceOfMinusOne] = areaArray[firstOccurrenceOfMinusOne - 1];
            firstOccurrenceOfMinusOne--;
        }
    }
    else
    {
        printf("Area array is full (dbOperations) \n");
        return -1;
    }
    return low;
}

short giveWriteIndexForWasteRecords(struct metaData *metaDataRecord, short areaId, short addEnable, char dateOfCollection[], struct wasteRecord *wasteRecordArray)
{
    short midIndex = -1, low = 0, high = 0;
    short firstOccurrenceOfMinusOne = 0;

    // while (high < wasteRecordArrayLength && wasteRecordArray[high].areaIdForeignKey != -1)
    // {
    //     printf("arr value : %hd \n", wasteRecordArray[high].areaIdForeignKey);
    //     high++;
    // }
    firstOccurrenceOfMinusOne = metaDataRecord->wasteArrayFilled;
    high = firstOccurrenceOfMinusOne - 1;

    printf("first run : %hd ---- %hd ---- %hd\n", low, high, firstOccurrenceOfMinusOne);

    while (low <= high)
    {
        midIndex = (high - low) / 2 + low;
        if (wasteRecordArray[midIndex].areaIdForeignKey == areaId)
        {
            return giveWriteIndexForDateOfCollection(midIndex, firstOccurrenceOfMinusOne, areaId, dateOfCollection, wasteRecordArray);
        }
        else if (wasteRecordArray[midIndex].areaIdForeignKey > areaId)
        {
            high = midIndex - 1;
        }
        else
        {
            low = midIndex + 1;
        }
        printf("%hd----%hd----%hd\n", low, high, midIndex);
    }

    if (!addEnable)
    {
        return -1; // if code reach this place means
    }

    if (low == firstOccurrenceOfMinusOne)
    {
        if (wasteRecordArray[low].areaIdForeignKey == -1)
        {
            return low;
        }
        else
        {
            printf("Error: never suppose to happen check dbOperations-giveWriteIndexForWasteRecords");
            return -1;
        }
    }
    else
    {
        return giveWriteIndexForDateOfCollection(low, firstOccurrenceOfMinusOne, areaId, dateOfCollection, wasteRecordArray);
    }

    return -1;
}

short giveWriteIndexForDateOfCollection(short firstSeen, short firstOccurrenceOfMinusOne, short areaId, char dateOfCollection[], struct wasteRecord *wasteRecordArray)
{
    short i = firstSeen, j = firstOccurrenceOfMinusOne;

    printf("[DEBUG] Start: areaId=%hd, date=%s, firstSeen=%hd, firstEmpty=%hd\n", areaId, dateOfCollection, firstSeen, firstOccurrenceOfMinusOne);

    if (strcmp(wasteRecordArray[i].dateOfCollection, dateOfCollection) > 0)
    {
        while (i > -1 && wasteRecordArray[i].areaIdForeignKey == areaId && strcmp(wasteRecordArray[i].dateOfCollection, dateOfCollection) >= 0)
        {
            i--;
            printf("[DEBUG]---------- new i=%hd\n", i);
        }
        i++;
        printf("[DEBUG] Moved backward, new i=%hd\n", i);
    }
    else
    {
        while (i < wasteRecordArrayLength && wasteRecordArray[i].areaIdForeignKey == areaId && strcmp(wasteRecordArray[i].dateOfCollection, dateOfCollection) < 0)
        {
            i++;
        }
        printf("[DEBUG] Moved forward, new i=%hd\n", i);
    }

    if (i > -1)
    {
        if (wasteRecordArray[i].areaIdForeignKey == areaId && strcmp(wasteRecordArray[i].dateOfCollection, dateOfCollection) == 0)
        {
            return i;
        }
        else
        {
            printf("[DEBUG] Shifting records: targetPos=%hd <<<< empty=%hd\n", i, j);
            while (j > i)
            {
                wasteRecordArray[j] = wasteRecordArray[j - 1];
                j--;
            }
            printf("[DEBUG] final  insert index = %hd\n", i);
            return i;
        }
    }
    else
    {
        printf("[DEBUG] issue  insert index = %hd\n", i);
        return -1;
    }

    return -1;
}

short checkIfAreaIdExist(short areaId, struct area *areaArray, struct metaData *metaDataRecord)
{
    short midIndex, low = 0, high = 0;
    short firstOccurrenceOfMinusOne = 0;

    firstOccurrenceOfMinusOne = metaDataRecord->areaArrayFilled;
    high = firstOccurrenceOfMinusOne - 1;

    while (low <= high)
    {
        midIndex = (high - low) / 2 + low;
        if (areaArray[midIndex].areaId == areaId)
        {
            return 1;
        }
        else if (areaArray[midIndex].areaId > areaId)
        {
            high = midIndex - 1;
        }
        else
        {
            low = midIndex + 1;
        }
    }
    return 0;
}

void calculateMonthlySummary(char monthYear[], struct monthlySummary *monthlySummaryRecord, struct wasteRecord *wasteRecordArray)
{
    monthlySummaryRecord->totalRecycledWaste = 0;
    monthlySummaryRecord->totalWasteCollected = 0;
    monthlySummaryRecord->recyclingRate = 0;
    short i = 0;

    while (wasteRecordArray[i].areaIdForeignKey != -1)
    {
        if (strncmp(wasteRecordArray[i].dateOfCollection, monthYear, 7) == 0)
        {
            monthlySummaryRecord->totalRecycledWaste += wasteRecordArray[i].recycledWaste;
            monthlySummaryRecord->totalWasteCollected += wasteRecordArray[i].totalWasteCollected;
        }
        i++;
    }
    if (monthlySummaryRecord->totalWasteCollected > 0)
    {
        monthlySummaryRecord->recyclingRate = (monthlySummaryRecord->totalRecycledWaste * 100.0) / monthlySummaryRecord->totalWasteCollected;
    }
}

void calculateMonthlyReport(char monthYear[], struct monthlyReport *monthlyReportRecord, struct wasteRecord *wasteRecordArray)
{
    monthlyReportRecord->totalRecycledWaste = 0;
    monthlyReportRecord->totalWasteCollected = 0;
    monthlyReportRecord->recyclingRate = 0.0;
    monthlyReportRecord->areaRecyclingRate = -1.0;
    monthlyReportRecord->areaId = -1;

    short i = 0;
    short arrayIdHolder = wasteRecordArray[0].areaIdForeignKey;
    int areaRecycledWaste = 0, areaTotalWaste = 0;
    float tempFloatValue = 0.0;

    while (i < wasteRecordArrayLength && wasteRecordArray[i].areaIdForeignKey != -1)
    {
        areaRecycledWaste = 0;
        areaTotalWaste = 0;

        while (i < wasteRecordArrayLength && arrayIdHolder == wasteRecordArray[i].areaIdForeignKey)
        {
            if (strncmp(wasteRecordArray[i].dateOfCollection, monthYear, 7) == 0)
            {
                areaTotalWaste += wasteRecordArray[i].totalWasteCollected;
                areaRecycledWaste += wasteRecordArray[i].recycledWaste;
            }
            i++;
        }

        monthlyReportRecord->totalWasteCollected += areaTotalWaste;
        monthlyReportRecord->totalRecycledWaste += areaRecycledWaste;

        if (areaTotalWaste > 0)
        {
            tempFloatValue = (areaRecycledWaste * 100.0) / areaTotalWaste;
            if (tempFloatValue > monthlyReportRecord->areaRecyclingRate)
            {
                monthlyReportRecord->areaRecyclingRate = tempFloatValue;
                monthlyReportRecord->areaId = arrayIdHolder;
            }
        }

        if (i < wasteRecordArrayLength)
        {
            arrayIdHolder = wasteRecordArray[i].areaIdForeignKey;
        }
    }

    if (monthlyReportRecord->totalWasteCollected > 0)
    {
        monthlyReportRecord->recyclingRate = (monthlyReportRecord->totalRecycledWaste * 100.0) / monthlyReportRecord->totalWasteCollected;
    }
    else
    {
        monthlyReportRecord->recyclingRate = 0.0;
    }
}

short deleteWasteRecord(short recordId, struct wasteRecord *wasteRecordArray)
{
    int i = 0;
    while (i < wasteRecordArrayLength && wasteRecordArray[i].recordId != recordId)
    {
        i++;
    }

    if (i < wasteRecordArrayLength)
    {
        struct wasteRecord wasteRecordHolder = wasteRecordArray[i];
        wasteRecordHolder.recordId = -1;
        wasteRecordHolder.areaIdForeignKey = -1;

        i++;

        while (i < wasteRecordArrayLength && wasteRecordArray[i].recordId != -1)
        {
            wasteRecordArray[i - 1] = wasteRecordArray[i];
            i++;
        }

        wasteRecordArray[i - 1] = wasteRecordHolder;

        return 1;
    }

    return -1;
}

short deleteAreaRecord(struct metaData *metaDataRecord, short areaId, struct area *areaArray, struct wasteRecord *wasteRecordArray)
{
    short seenIndex = checkForDuplicatesAndOrderedIndex(metaDataRecord, areaId, 0, areaArray);

    if (seenIndex != -1)
    {
        short i = seenIndex + 1;
        struct area areaElementHolder = areaArray[seenIndex];
        while (i < areaArrayLength && areaArray[i].areaId != -1)
        {
            areaArray[i - 1] = areaArray[i];
            i++;
        }
        i--;
        areaArray[i] = areaElementHolder;
        if (removeAssociatedWasteRecordsWithArrayId(areaId, wasteRecordArray) == -1)
        {
            return -1;
        }
        else
        {
            areaArray[i].areaId = -1;
            return 1;
        }
    }
    return -1;
}

short removeAssociatedWasteRecordsWithArrayId(short areaId, struct wasteRecord *wasteRecordArray)
{
    short midIndex = -1, low = 0, high = 0, existingFlag = -1;
    short firstOccurrenceOfMinusOne = 0;

    printf("\n=== removeAssociatedWasteRecordsWithArrayId DEBUG START ===\n");
    printf("Searching for areaId = %hd\n", areaId);

    while (high < wasteRecordArrayLength && wasteRecordArray[high].areaIdForeignKey != -1)
    {
        printf("arr value : %hd \n", wasteRecordArray[high].areaIdForeignKey);
        high++;
    }
    if (high == wasteRecordArrayLength)
    {
        return -1;
    }
    firstOccurrenceOfMinusOne = high;
    high--;

    printf("[INFO] First -1 found at index=%hd (arrayLen=%hd)\n", firstOccurrenceOfMinusOne, wasteRecordArrayLength);
    printf("[INFO] Binary search range: low=%hd, high=%hd\n", low, high);

    while (low <= high && existingFlag != 1)
    {
        midIndex = (high - low) / 2 + low;
        if (wasteRecordArray[midIndex].areaIdForeignKey == areaId)
        {
            existingFlag = 1;
        }
        else if (wasteRecordArray[midIndex].areaIdForeignKey > areaId)
        {
            high = midIndex - 1;
        }
        else
        {
            low = midIndex + 1;
        }
    }
    if (midIndex != -1)
    {
        printf("[POST-SEARCH] midIndex=%hd, existingFlag=%hd\n", midIndex, existingFlag);

        short lowIndex = midIndex, highIndex = midIndex;
        while (lowIndex > 0 && wasteRecordArray[lowIndex - 1].areaIdForeignKey == areaId)
        {
            lowIndex--;
        }
        while (highIndex < firstOccurrenceOfMinusOne && wasteRecordArray[highIndex].areaIdForeignKey == areaId)
        {
            highIndex++;
        }

        printf("[RANGE] lowIndex=%hd, highIndex=%hd (exclusive), count=%hd\n", lowIndex, highIndex, highIndex - lowIndex);

        short wasteWasterRecordHolderArrayLength = highIndex - lowIndex;
        struct wasteRecord *wasteWasterRecordHolderArray = malloc((wasteWasterRecordHolderArrayLength) * sizeof(struct wasteRecord));
        if (wasteWasterRecordHolderArray == NULL)
        {
            printf("Memory allocation issue dbOperations \n");
            return -1;
        }
        printf("[ALLOC] Temp buffer of %hd records allocated\n", wasteWasterRecordHolderArrayLength);

        short i = lowIndex, k = 0;
        while (i < highIndex)
        {

            printf("--:");
            printf("%hd \n", k);
            wasteRecordArray[i].areaIdForeignKey = -1;
            wasteRecordArray[i].recordId = -1;
            wasteWasterRecordHolderArray[k] = wasteRecordArray[i];
            i++;
            k++;
        }
        while (highIndex < firstOccurrenceOfMinusOne)
        {
            printf("=======\n");
            wasteRecordArray[lowIndex] = wasteRecordArray[highIndex];
            lowIndex++;
            highIndex++;
        }
        k = 0;
        while (lowIndex < firstOccurrenceOfMinusOne)
        {
            printf("++:");
            printf("%hd \n", k);
            wasteRecordArray[lowIndex] = wasteWasterRecordHolderArray[k];
            k++;
            lowIndex++;
        }
        free(wasteWasterRecordHolderArray);
        return 1;
    }
    return -1;
}

short calculateAreaPerformance(struct monthlyReport *monthlyReportRecord, short areaId, char monthYear[], struct wasteRecord *wasteRecordArray)
{
    monthlyReportRecord->totalRecycledWaste = 0;
    monthlyReportRecord->totalWasteCollected = 0;
    monthlyReportRecord->recyclingRate = 0;
    short i = 0;
    int areaTotalWaste = 0, areaRecycledWaste = 0;

    while (wasteRecordArray[i].areaIdForeignKey != -1)
    {

        if (strncmp(wasteRecordArray[i].dateOfCollection, monthYear, 7) == 0)
        {
            monthlyReportRecord->totalRecycledWaste += wasteRecordArray[i].recycledWaste;
            monthlyReportRecord->totalWasteCollected += wasteRecordArray[i].totalWasteCollected;
            if (wasteRecordArray[i].areaIdForeignKey == areaId)
            {
                areaRecycledWaste += wasteRecordArray[i].recycledWaste;
                areaTotalWaste += wasteRecordArray[i].totalWasteCollected;
            }
        }

        i++;
    }
    if (monthlyReportRecord->totalWasteCollected > 0)
    {
        monthlyReportRecord->recyclingRate = (monthlyReportRecord->totalRecycledWaste * 100.0) / monthlyReportRecord->totalWasteCollected;
        monthlyReportRecord->areaRecyclingRate = (areaRecycledWaste * 100.0) / areaTotalWaste;
        return 1;
    }
    return -1;
}



void calculateGetHighestWasteDay(char date[]){

}