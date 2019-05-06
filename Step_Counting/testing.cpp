#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

int step_counting(vector<float> &array, int size);
void read_csv(const char *filename, vector<float> &array);
void add(float *array, float);
int main()
{
    //read csv to vector
    vector<float> array;
    char filename[30] = "foo.csv";
    read_csv(filename, array);
    //step coutning
    int count;
    count = step_counting(array, array.size());
    cout << count << endl;
    return 0;
}

int step_counting(vector<float> &array, int size)
{
    int count = 0;
    int *peaks;

    peaks = (int *)malloc(size * sizeof(int));
    for (int i = 0; i < size; i++)
    {
        if (i == 0)
            peaks[i] = 0;
        else if (i == size - 1)
            peaks[i] = 0;
        else
        {
            if (array[i - 1] < array[i] && array[i] > array[i + 1])
                peaks[i] = 1;
            else
                peaks[i] = 0;
        }
    }
    int k = 0, D = 0, j = 0;
    for (j = 0; j < size; j++)
    {
        if (peaks[j] == 1)
        {
            if (k != 0)
            {
                D = j - k - 1;
                // cout << "D= " << D << endl;
                count = D > 2 ? count + 1 : count;
            }
            k = j;
        }
    }
    if (j == size)
    {
        D = size - k;
        count = D > 2 ? count + 1 : count;
    }
    free(peaks);
    return count;
}

void read_csv(const char *filename, vector<float> &array)
{
    std::ifstream file(filename, ifstream::in);
    if (!file)
    {
        string error_message = "File name error.";
        return;
    }
    string line;
    while (getline(file, line))
    {
        float temp = atof(line.c_str());
        array.push_back(temp);
        cout << temp << endl;
    }
}
