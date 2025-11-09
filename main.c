#include <stdio.h>
#include "dbManager.h"
#define DB_FILENAME "waste_db.txt"

int main(void)
{
    initDataBase();

    int choice;
    while (1)
    {
        printf("\n===== MENU =====\n");
        printf("1) Load database from disk into memory (reads '%s')\n", DB_FILENAME);
        printf("2) Save database to disk and exit (writes '%s')\n", DB_FILENAME);
        printf("3) Add area (interactive)\n");
        printf("4) Add waste record (interactive)\n");
        printf("5) Debug: print area details\n");
        printf("6) Debug: print waste records\n");
        printf("7) GenerateMonthlySummary\n");
        printf("8) getCityMonthlyReport\n");
        printf("9 -- 10 ) update - delete\n");
        printf("11) getAreaPerformance()\n");
        printf("0) Exit without saving\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1)
        {
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
            {
            }
            printf("Invalid input. Try again.\n");
            continue;
        }

        switch (choice)
        {
        case 1:
        {
            int r = loadDatabase(DB_FILENAME);
            if (r == 0)
            {
                printf("Database loaded from '%s'.\n", DB_FILENAME);
            }
            else if (r == 1)
            {
                printf("No DB file found. Starting with empty database.\n");
            }
            else
            {
                printf("Error loading DB (code %d). See stderr for details.\n", r);
            }
            break;
            printMetaData();
        }

        case 2:
        {
            int r = saveDatabase(DB_FILENAME);
            if (r == 0)
            {
                printf("Database saved to '%s'. Exiting.\n", DB_FILENAME);
                return 0;
            }
            else
            {
                printf("Failed to save DB (code %d). Exiting anyway.\n", r);
                return r;
            }
        }

        case 3:
            addArea(1);
            debugAreaDetails();
            printMetaData();
            break;

        case 4:
            addWasteRecord(1);
            debugPrintWasteRecords();
            printMetaData();
            break;

        case 5:
            debugAreaDetails();
            printMetaData();
            break;

        case 6:
            debugPrintWasteRecords();
            printMetaData();
            break;

        case 7:
            generateMonthlySummary();
            break;
        case 8:
            getCityMonthlyReport();
            break;
        case 9:
            update_Area_or_Record();
            printMetaData();
            break;
        case 10:
            delete_Area_or_Record();
            printMetaData();
            break;
        case 11:
            getAreaPerformance();
            break;
        case 0:
            printf("Exiting without saving.\n");
            return 0;

        default:
            printf("Unknown choice.\n");
            break;
        }
    }

    return 0;
}
