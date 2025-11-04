#define areaNamelength 51
#define dateOfCollectionLength 11
#define collectionAgencyNameLength 51
#define monthYearArrayLength 8
#define monthsInYear 12
// note all char name  = name length + /0 for null terminator
#define areaArrayLength 50
#define wasteRecordArrayLength 1550
#define monthArrayLength 3
#define yearArrayLength 5

struct area
{
    short areaId;
    char areaName[areaNamelength];
    int population;
    float avgWastePerPerson; // (kg/day)
};

struct wasteRecord
{
    short recordId;
    short areaIdForeignKey;
    char dateOfCollection[dateOfCollectionLength];
    int totalWasteCollected;
    int recycledWaste;
    char collectionAgencyName[collectionAgencyNameLength];
};

struct monthlySummary
{
    char month[monthArrayLength];
    char year[yearArrayLength];
    int totalWasteCollected;
    int totalRecycledWaste;
    float recyclingRate;
};

struct monthlyReport
{
    int totalWasteCollected;
    int totalRecycledWaste;
    float recyclingRate;
    short areaId;
    float areaRecyclingRate;
};

struct metaData
{
    short areaArrayFilled;
    short wasteArrayFilled;
};