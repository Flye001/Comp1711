#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// declaring this value so I can use it in different places
#define buffer_size 100

/**
 * @brief Struct to hold each daily reading, contains the date as a string
 *        and the bloodIron as a floating point number.
 *
 */
typedef struct
{
    char date[20];
    float bloodIron;
} reading;

/**
 * @brief Adapted version of the tokeniseRecord function which you should now be familiar with - this one is adapted for this data file
 *        as it has fewer outputs and gives you the bloodIron as a float
 *
 * @param input the line of the file to be split
 * @param delimiter what character it should split on
 * @param date the place where the date will be stored
 * @param bloodIron the place where the bloodIron will be stored.
 */
void tokeniseRecord(const char *input, const char *delimiter,
                    char *date, float *bloodIron)
{
    // Create a copy of the input string as strtok modifies the string
    char *inputCopy = strdup(input);

    // Tokenize the copied string
    char *token = strtok(inputCopy, delimiter);
    if (token != NULL)
    {
        strcpy(date, token);
    }

    token = strtok(NULL, delimiter);
    if (token != NULL)
    {
        // turns the blood iron into a float - similar to atoi().
        // we have to tell C that bloodIron is a pointer so it stores it.
        *bloodIron = atof(token);
    }

    // Free the duplicated string
    free(inputCopy);
}

// SUGGESTED FUNCTIONS

/**
 * @brief Opens the file in the correct mode
 *
 * @param filename the name of the file to open
 * @param mode the mode (r/w/a/r+/w+/a+)
 * @return FILE* The file object to store the opened file in.
 */
FILE *open_file(char *filename, char *mode)
{
    FILE *input = fopen(filename, mode);
    if (!input)
    {
        printf("Error: File could not be opened\n");
        exit(1);
    }
    return input;
}

/**
 * @brief Reads the data from the input file into an array of structs
 *
 * @param inputFile the open file object
 * @param dataArray the array of readings
 * @param counter a counter that will return number of items
 * @return int Returns the number of readings from the file
 */
int read_file(FILE *inputFile, reading *dataArray, int *counter)
{
    char line[buffer_size];
    while (fgets(line, buffer_size, inputFile))
    {
        // split up the line and store it in the right place
        // using the & operator to pass in a pointer to the bloodIron so it stores it
        tokeniseRecord(line, ",", dataArray[*counter].date, &dataArray[*counter].bloodIron);
        *counter += 1;
    }
}

/**
 * @brief Checks that there was data for each part of each reading in the file
 *
 * @param dataArray The array of data from the file
 * @param numReadings The number of readings in the array
 * @return int Return 0 if there are no errors, 1 if you find an error.
 */
int data_checker(reading *dataArray, int numReadings)
{
    // to do
}

/**
 * @brief Display all data records
 * @param dataArray The array of data from the file
 * @param numReadings The number of readings in the array
 */
void display_records(reading *dataArray, int numReadings)
{
    for (int i = 0; i < numReadings; i++)
    {
        printf("%s - Blood iron: %.1f\n", dataArray[i].date, dataArray[i].bloodIron);
    }
}

/**
 * @brief Calculates and returns the mean of the readings in the array
 *
 * @param dataArray The array of data from the file
 * @param numReadings The number of readings in the array
 * @return float The mean of the readings.
 */
float find_mean(reading *dataArray, int numReadings)
{
    float sum = 0;
    for (int i = 0; i < numReadings; i++)
    {
        sum += dataArray[i].bloodIron;
    }
    return sum / numReadings;
}

/**
 * @brief Finds and returns the highest blood iron reading
 *
 * @param dataArray The array of data from the file
 * @param numReadings The number of readings in the array
 * @return float The highest blood iron reading
 */
float find_highest(reading *dataArray, int numReadings)
{
    float highest = 0;
    for (int i = 0; i < numReadings; i++)
    {
        if (dataArray[i].bloodIron > highest)
            highest = dataArray[i].bloodIron;
    }
    return highest;
}

/**
 * @brief Finds and returns the lowest blood iron reading
 *
 * @param dataArray The array of data from the file
 * @param numReadings The number of readings in the array
 * @return float The lowest blood iron reading
 */
float find_lowest(reading *dataArray, int numReadings)
{
    float lowest = 100000;
    for (int i = 0; i < numReadings; i++)
    {
        if (dataArray[i].bloodIron < lowest)
            lowest = dataArray[i].bloodIron;
    }
    return lowest;
}

/**
 * @brief Ask the user for the month to find, and then print out all readings containing that month.
 *
 * @param dataArray The array of data from the file
 * @param numReadings The number of readings in the array
 */
void monthly_iron(reading *dataArray, int numReadings)
{
    char month[4];
    printf("Enter a month (e.g. SEP, OCT): ");
    scanf("%s", month);

    char *strPtr;
    for (int i = 0; i < numReadings; i++)
    {
        strPtr = strstr(dataArray[i].date, month);
        if (strPtr != NULL)
        {
            printf("%s - Blood iron: %.1f\n", dataArray[i].date, dataArray[i].bloodIron);
        }
    }
}

/**
 * @brief Generate additional statistics about the data
 *
 * @param dataArray The array of data from the file
 * @param numReadings The number of readings in the array
 */
void additional_stats(reading *dataArray, int numReadings)
{
    float range = find_highest(dataArray, numReadings) - find_lowest(dataArray, numReadings);
    float mean = find_mean(dataArray, numReadings);
    float variance_sum = 0;
    for (int i = 0; i < numReadings; i++) {
        variance_sum += powf((dataArray[i].bloodIron - mean), 2);
    }
    float varience = variance_sum / numReadings;
    float stdDev = sqrtf(varience);
    float median = dataArray[numReadings / 2].bloodIron;

    printf("Range: %.2f\n", range);
    printf("Mean: %.2f\n", mean);
    printf("Standard Deviation: %.2f\n", stdDev);
    printf("Median: %.2f\n", median);
}