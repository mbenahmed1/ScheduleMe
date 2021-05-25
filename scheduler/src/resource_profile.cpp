//
// Created by tim on 25.05.21.
//

#include "../include/resource_profile.hpp"

namespace ScheduleMe
{

ResourceProfile::ResourceProfile(Instance &instance) : instance(instance)
{
    profiles.resize(instance.r());
}

unsigned int ResourceProfile::get_available_capacity(unsigned int time, unsigned int resource) const
{
    if (time >= profiles.at(resource).size())
    {
        return instance.resources.at(resource);
    }
    unsigned int max_capacity = instance.resources.at(resource);
    unsigned int current_capacity = profiles.at(resource).at(time);

    return max_capacity - current_capacity;
}

bool ResourceProfile::is_schedulable(unsigned int start_time, unsigned int activity) const
{
    if (start_time >= profiles.at(0).size())
    {
        return true;
    }
    unsigned int processing_time = instance.processing_time.at(activity);
    for (unsigned int r = 0; r < instance.r(); r++)
    {
        unsigned int activity_demands = instance.demands.at(activity).at(r);
        for (unsigned int t = start_time; t < start_time + processing_time; t++)
        {
            if (get_available_capacity(t, r) < activity_demands)
            {
                return false;
            }
        }
    }
    return true;
}

void ResourceProfile::schedule_at(unsigned int start_time, unsigned int activity)
{

    for (unsigned int r = 0; r < profiles.size(); r++)
    {
        std::vector<unsigned int> &current_profile = profiles.at(r);
        unsigned int processing_time = instance.processing_time.at(activity);
        unsigned int activity_demands = instance.demands.at(activity).at(r);
        unsigned int completion_time = start_time + processing_time;

        if (completion_time > current_profile.size())
        {
            current_profile.resize(completion_time, 0);
        }

        for (unsigned int i = start_time; i < completion_time; i++)
        {
            current_profile.at(i) = activity_demands;
        }

    }
}

void ResourceProfile::print_resource_profiles() const
{
    using namespace std;
    for (unsigned int r = 0; r < profiles.size(); r++)
    {
        unsigned int capacity = instance.resources.at(r);
        unsigned int makespan = profiles.at(r).size();
        cout << "Resource: " << r << " Capacity: " << capacity << endl;

        std::vector<std::vector<unsigned int>> draw_matrix;
        draw_matrix.resize(capacity);
        for (auto &vec : draw_matrix)
        {
            vec.resize(makespan, 0);
        }

        // fill matrix

        for (unsigned int i = 0; i < draw_matrix.size(); i++)
        {
            // fill up to current capacity

            for (unsigned int j = 0; j < draw_matrix.at(i).size(); j++)
            {
                unsigned int fill = profiles.at(r).at(j);
                if (i >= draw_matrix.size() - fill)
                {
                    draw_matrix[i][j] = 1;
                }

            }
        }

        // draw
        for (unsigned int i = 0; i < draw_matrix.size(); i++)
        {
            if (capacity - i < 10)
            {
                cout << " ";
            }
            cout << capacity - i << "\u2595 ";
            for (unsigned int j = 0; j < draw_matrix.at(i).size(); j++)
            {
                if (draw_matrix[i][j] == 0)
                {
                    cout << "  ";
                }
                else
                {
                    cout << "\u25A8 ";
                }

            }
            cout << endl;
        }
        for (unsigned int i = 0; i < makespan; i++)
        {
            cout << "\u2F00\u2F00";
        }
        cout << ">" << endl << "  \u2595 ";
        for (unsigned int i = 0; i < makespan; i++)
        {
            cout << i % 10 << " ";
        }

        cout << endl;
    }

}

unsigned int ResourceProfile::get_used_capacity(unsigned int time, unsigned int resource) const
{
    if (time >= profiles.at(0).size())
    {
        return 0;
    }
    return profiles.at(resource).at(time);
}


}