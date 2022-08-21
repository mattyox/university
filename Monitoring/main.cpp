#include <iostream>
#include <cstring>
#include <vector>
#include <memory>
#include <ctime>
#include <thread>
#include <chrono>
#include <fstream>
#include <cstdlib>

#define basic_temperature 20
#define basic_noise 60
#define basic_pressure 1000

using namespace std;

class Sensor
{
protected:
    string coordinates;
    string quantity;
    string unit;

public:
    Sensor(string c = "Unknown", string q = "Unknown", string u = "Unknown")
    {
        coordinates = c;
        quantity = q;
        unit = u;
    }

    ~Sensor()
    {
        ;
    }

    string get_coordinates()
    {
        return coordinates;
    }

    string get_quantity()
    {
        return quantity;
    }

    string get_unit()
    {
        return unit;
    }

    virtual string get_measurement() = 0;
};

class Temperature: public Sensor
{
public:
    Temperature(string c, string q = "Temperature", string u = "\370""C"):Sensor(c, q, u)
    {
        coordinates = c;
        quantity = q;
        unit = u;
    }

    ~Temperature()
    {
        ;
    }

    virtual string get_measurement()
    {
        int measurement_int = basic_temperature;

        srand((int)time(NULL));
        measurement_int += ((rand() % 2));
        srand((int)time(NULL));
        measurement_int -= ((rand() % 3));

        string measurement_string = to_string(measurement_int);

        return measurement_string;
    }
};

class Noise: public Sensor
{
public:
    Noise(string c, string q = "Noise level", string u = "dB"):Sensor(c, q, u)
    {
        coordinates = c;
        quantity = q;
        unit = u;
    }

    ~Noise()
    {
        ;
    }

    virtual string get_measurement()
    {
        int measurement_int = basic_noise;

        srand((int)time(NULL));
        measurement_int += (rand() % 6);
        srand((int)time(NULL));
        measurement_int -= (rand() % 11);

        string measurement_string = to_string(measurement_int);

        return measurement_string;
    }
};

class Pressure: public Sensor
{
public:
    Pressure(string c, string q = "Pressure", string u = "hPa"):Sensor(c, q, u)
    {
        coordinates = c;
        quantity = q;
        unit = u;
    }

    ~Pressure()
    {
        ;
    }

    virtual string get_measurement()
    {
        int measurement_int = basic_pressure;

        srand((int)time(NULL));
        measurement_int += (rand() % 26);
        srand((int)time(NULL));
        measurement_int -= (rand() % 51);

        string measurement_string = to_string(measurement_int);

        return measurement_string;
    }
};

class Panel
{
    vector <shared_ptr <Sensor>> sensors;
    int number_of_sensors;
    shared_ptr <string> readings;

public:
    Panel()
    {
        ;
    }

    Panel(vector <shared_ptr <Sensor>> s, int n = 1)
    {
        sensors = s;
        number_of_sensors = n;
        readings = make_shared <string> ();
    }

    ~Panel()
    {
        ;
    }

    string get_time()
    {
        time_t now = time(NULL);
        struct tm tstruct;
        char time_string[9];
        tstruct = *localtime(&now);
        strftime(time_string, sizeof(time_string), "%X", &tstruct);
        return time_string;
    }

    string get_readings()
    {
        string buffer;

        for (int i = 0; i < number_of_sensors; i++)
        {
            buffer += get_time() + " " + sensors[i]->get_coordinates() + " " + sensors[i]->get_quantity() + " = " + sensors[i]->get_measurement() + " ""[" + sensors[i]->get_unit() + "]""\n";
        }

        *readings = buffer;

        return *readings;
    }
};

class Concentrator
{
    vector <shared_ptr <Panel>> panels;
    int number_of_panels;
    shared_ptr <string> data;

public:
    Concentrator()
    {
        ;
    }

    Concentrator(vector <shared_ptr <Panel>> p, int n = 1)
    {
        panels = p;
        number_of_panels = n;
        data = make_shared <string> ();
    }

    ~Concentrator()
    {
        ;
    }

    void get_data()
    {
        for (int i = 0; i < number_of_panels; i++)
        {
            *data += "Measuring Panel: " + to_string(i + 1) + "\n" + panels[i]->get_readings();
        }
    }

    void print_data()
    {
        cout << *data;
    }

    void save_data(string output_filepath)
    {
        ofstream output_file;
        output_file.open(output_filepath);

        if (!output_file)
        {
            cout << "\n";
            cerr << "Unable to create the output file" << endl;
            exit(0);
        }

        output_file << *data;
        cout << "\nReport successfully created\n\n" << endl;
        output_file.close();
    }
};

class Utilities
{
public:
    void monitoring(vector <shared_ptr <Concentrator>> concentrators, int number_of_concentrators, int duration, int interval, string output_filepath[])
    {
        for (int i = 1; i <= (duration / interval); i++)
        {
            for (int j = 0; j < number_of_concentrators; j++)
            {
                concentrators[j]->get_data();
            }

            this_thread::sleep_for(chrono::seconds(interval));
        }

        for (int i = 0; i < number_of_concentrators; i++)
        {
            cout << "CONCENTRATOR " << (i + 1) << endl;
            concentrators[i]->print_data();
            concentrators[i]->save_data(output_filepath[i]);
        }
    }

    void print_help()
    {
        cout << "The format of passing arguments:" << endl;
        cout << "Number X of concentrators: -c X or -C X" << endl;
        cout << "Can not be less than 1" << endl;
        cout << "Number of Y panels of concentrator X: -pX Y or -PX Y" << endl;
        cout << "Can not be less than 1" << endl;
        cout << "Number of Z sensors (-t or -T for temperature, -n or -N for noise level, -p or -P for pressure) of panel Y of concentrator X and appropriated coordinates W V for the n-th sensor: -sXY Z Wn Vn" << endl;
        cout << "There must be at least one sensor of any type, while non of them can not be less than 0" << endl;
        cout << "Duration of measurement of the system (in hours, minutes and seconds): -d HH MM SS or -D HH MM SS" << endl;
        cout << "Can not be less than 0" << endl;
        cout << "Time interval of measurement of the system (in hours, minutes and seconds): -i HH MM SS or -I HH MM SS" << endl;
        cout << "Can not be less than 0" << endl;
        cout << "Output files path O for appropriated n-th concentrator: -f On or -F On" << endl;
        cout << "Each concentrator must have an output file path" << endl;
    }

    void get_help(int counter, char *argument[])
    {
        if (string(argument[1]) == "-h" || string(argument[1]) == "-H")
        {
            print_help();
            exit(0);
        }
    }

    int get_number_of_concentrators(int counter, char *argument[])
    {
        int number_of_concentrators;

        for(int i = 1; i < counter; i++)
        {
            if (string(argument[i]) == "-c" || string(argument[i]) == "-C")
            {
                number_of_concentrators = atoi(argument[i + 1]);

                if (number_of_concentrators <= 0)
                {
                    print_help();
                    exit(0);
                }
            }
        }

        return number_of_concentrators;
    }

    int get_number_of_panels(int i, int counter, char *argument[])
    {
        int number_of_panels;

        for(int a = 1; a < counter; a++)
            {
                if (string(argument[a]) == ("-p" + to_string(i + 1)) || string(argument[a]) == ("-P" + to_string(i + 1)))
                {
                    number_of_panels = atoi(argument[a + 1]);

                    if (number_of_panels <= 0)
                    {
                        print_help();
                        exit(0);
                    }
                }
            }

            return number_of_panels;
    }

    int get_sensor_type_number(string type_lower, string type_upper, int i, int j, int counter, char *argument[])
    {
        int sensor_type_number;

        for (int a = 1; a < counter; a++)
        {
            if (string(argument[a]) == (type_lower + to_string(i + 1) + to_string(j + 1)) || string(argument[a]) == (type_upper + to_string(i + 1) + to_string(j + 1)))
            {
                sensor_type_number = atoi(argument[a + 1]);

                if (sensor_type_number < 0)
                {
                    print_help();
                    exit(0);
                }
            }
        }

        return sensor_type_number;
    }

    string get_coordinates(string type_lower, string type_upper, int i, int j, int n, int counter, char *argument[])
    {
        string coordinates;

        for (int a = 1; a < counter; a++)
        {
            if (string(argument[a]) == (type_lower + to_string(i + 1) + to_string(j + 1)) || string(argument[a]) == (type_upper + to_string(i + 1) + to_string(j + 1)))
            {
                coordinates = string(argument[a + n + 1]) + " " + string(argument[a + n + 2]);
            }
        }
        //n += 2;

        return coordinates;
    }

    void set_measuring_system(int number_of_concentrators, vector <shared_ptr <Concentrator>> &concentrators_vector, int counter, char *argument[])
    {
        for (int i = 0; i < number_of_concentrators; i++)
        {
            int number_of_panels = get_number_of_panels(i, counter, argument);

            vector <shared_ptr <Panel>> panels_vector (number_of_panels);

            for (int j = 0; j < number_of_panels; j++)
            {
                int number_of_temperature = get_sensor_type_number("-t", "-T", i, j, counter, argument);

                int number_of_noise = get_sensor_type_number("-n", "-N", i, j, counter, argument);

                int number_of_pressure = get_sensor_type_number("-p", "-P", i, j, counter, argument);

                int number_of_sensors = number_of_temperature + number_of_noise + number_of_pressure;

                if (number_of_sensors < 1)
                {
                    print_help();
                    exit(0);
                }

                vector <shared_ptr <Sensor>> sensors_vector (number_of_sensors);

                int l = 0;
                int n = 1;

                for (int k = 1; k <= number_of_temperature; k++)
                {
                    string coordinates = get_coordinates("-t", "-T", i, j, n, counter, argument);
                    auto temperature = make_shared <Temperature> (coordinates);
                    sensors_vector[l] = temperature;
                    l++;
                    n += 2;
                }

                n = 1;

                for (int k = 1; k <= number_of_noise; k++)
                {
                    string coordinates = get_coordinates("-n", "-N", i, j, n, counter, argument);
                    auto noise = make_shared <Noise> (coordinates);
                    sensors_vector[l] = noise;
                    l++;
                    n += 2;
                }

                n = 1;

                for (int k = 1; k <= number_of_pressure; k++)
                {
                    string coordinates = get_coordinates("-p", "-P", i, j, n, counter, argument);
                    auto pressure = make_shared <Pressure> (coordinates);
                    sensors_vector[l] = pressure;
                    l++;
                    n += 2;
                }

                auto panel = make_shared <Panel> (sensors_vector, number_of_sensors);
                panels_vector[j] = panel;
            }

            auto concentrator = make_shared <Concentrator> (panels_vector, number_of_panels);
            concentrators_vector[i] = concentrator;
        }
    }

    int get_duration(int counter, char *argument[])
    {
        int duration;

        for(int a = 1; a < counter; a++)
        {
            if (string(argument[a]) == "-d" || string(argument[a]) == "-D")
            {
                int hours = atoi(argument[a + 1]);
                int minutes = atoi(argument[a + 2]);
                int seconds = atoi(argument[a + 3]);

                duration = (3600 * hours) + (60 * minutes) + seconds;

                if (duration <= 0)
                {
                    print_help();
                    exit(0);
                }
            }
        }

        return duration;
    }

    int get_interval(int counter, char *argument[])
    {
        int interval;

        for(int a = 1; a < counter; a++)
        {
            if (string(argument[a]) == "-i" || string(argument[a]) == "-I")
            {
                int hours = atoi(argument[a + 1]);
                int minutes = atoi(argument[a + 2]);
                int seconds = atoi(argument[a + 3]);

                interval = (3600 * hours) + (60 * minutes) + seconds;

                if (interval <= 0)
                {
                    print_help();
                    exit(0);
                }
            }
        }

        return interval;
    }

    void get_filepaths(string output_filepaths[], int number_of_concentrators, int counter, char *argument[])
    {
        for(int a = 1; a < counter; a++)
        {
            if (string(argument[a]) == "-f" || string(argument[a]) == "-F")
            {
                for (int b = 0; b < number_of_concentrators; b++)
                {
                    output_filepaths[b] = string(argument[a + b + 1]);
                }
            }
        }
    }
};

int main(int argc, char *argv[])
{
    cout << endl;

    Utilities functions;

    functions.get_help(argc, argv);

    int number_of_concentrators = functions.get_number_of_concentrators(argc, argv);

    vector <shared_ptr <Concentrator>> concentrators_vector (number_of_concentrators);

    functions.set_measuring_system(number_of_concentrators, concentrators_vector, argc, argv);

    int duration = functions.get_duration(argc, argv);

    int interval = functions.get_interval(argc, argv);

    string output_filepaths[number_of_concentrators];

    functions.get_filepaths(output_filepaths, number_of_concentrators, argc, argv);

    functions.monitoring(concentrators_vector, number_of_concentrators, duration, interval, output_filepaths);

    return 0;
}
