// Sucker Punch.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<iostream>
#include<conio.h>
#include<fstream>
#include<string>
#include<cstdlib>
#include<utility>
#include<vector>
#include<functional>
#include<algorithm>
#include<numeric>
#include<stdexcept>
#include<set>
#include<sstream>
#include<memory>

using namespace std;

bool pairCompare(const pair<int,int>& firstElem, const pair<int, int>& secondElem)
{
	return firstElem.first < secondElem.first;
}


struct pair_sum
{
	std::pair<int, int> operator()(const std::pair<int, int>& lhs, const std::pair<int, int>& rhs)
	{
		return std::pair<int, int>(lhs.first + rhs.first, lhs.second+rhs.second);
	}
};

struct attack_sum 
{
	std::pair<int, int> operator()(const std::pair<int, int>& lhs, const std::pair<int, int>& rhs)
	{
		return std::pair<int, int>(lhs.first + rhs.first, 0);
	}
};

struct defense_sum
{
	std::pair<int,int> operator()(const std::pair<int,int>& lhs, const std::pair<int,int>& rhs)
	{
		return std::pair<int,int>(0, lhs.second + rhs.second);
	}
};

struct Team
{

	vector<int> indices;
	vector<pair<int, int>>resources;
	pair<int, int>power{0,0};
	pair<int, int>team_threshold;

	~Team()
	{
		
	}

};

vector<pair<int, int>> num_units;
vector<pair<int, int>> sorted_num_units;
set<int>checker;

int checkindex(int startindex)
{

int index;
auto iter = find(num_units.begin(), num_units.end(), make_pair(sorted_num_units[startindex].first, sorted_num_units[startindex].second));

index = distance(num_units.begin(), iter);
if (checker.empty())
{
	checker.insert(index);
}
else
{
	if (checker.count(index) >= 1)
	{
		iter = find(iter + 1, num_units.end(), make_pair(sorted_num_units[startindex].first, sorted_num_units[startindex].second));
		index = distance(num_units.begin(), iter);
	}
}

return index;
}


int _tmain(int argc, _TCHAR* argv[])
{
	ifstream myfile;
	string line;
	bool flag = true;
	float units_per_team = 0.0;
	vector<vector<int>> teams;
	vector<int> units_index;
	pair<int, int>total_attack ;
	pair<int, int> total_defense;
	int attack_threshold = 0;
	int defense_threshold = 0;
	int num_teams;
	int startindex=0, startindexoffset=0, endindex=0;
	int team_index = -1;
	int index;
	pair<int, int> threshold;
	


	myfile.open("C:\\Users\\prvn\\Documents\\Visual Studio 2013\\Projects\\Sucker Punch\\Sucker Punch\\Input.txt", ios::in);

	if (myfile.is_open())
	{
		std::cout << "Opened file for processing..." << endl;
		stringstream ss;
		int k = 0;
		string s, s1;
		while (getline(myfile,line))
		{
			if (flag == true)
			{
				num_teams = atoi(line.c_str());
				std::cout << "Number of Teams: "<<num_teams << endl;
				flag = false;
			}
			else
			{
				for (int i = 0; i <=line.length(); i++)
				{
					if ((i != (line.length()))&&(line[i] != ','))
					{
						ss << line[i];
				
					}
					else
					{
						if (k == 0)
						{
							s = ss.str();
							k++;
							ss.str("");
						}
						else
						{
							s1 = ss.str();
							k = 0;
							ss.str("");
						}
						
					}

				}
				num_units.push_back(make_pair(atoi(s.c_str()),atoi(s1.c_str())));

			}
			
		}
		myfile.close();
		Team *teams = new Team[num_teams];
		
		sorted_num_units=num_units;

		units_per_team = (float)num_units.size() / (float)num_teams;

		std::cout << "units_per_team: "<<units_per_team << endl;

		if (units_per_team < 1)
		{
			std::cout << "Not enough units per team.." << endl;
			return -1;
		}
		else if (units_per_team >= 1 && units_per_team <2)
		{
			sort(sorted_num_units.begin(), sorted_num_units.end(), pairCompare);
			if (((num_units.size()) % num_teams) != 0)
			{
				flag = true;
				startindexoffset = (num_units.size() % num_teams);
			}

			for (int i = startindexoffset; i < (num_teams+startindexoffset); i++)
			{
				team_index++;
				index = checkindex(i);
				teams[team_index].indices.push_back(index);
				teams[team_index].resources.push_back(make_pair(sorted_num_units[i].first, sorted_num_units[i].second));
				teams[team_index].power.first = teams[team_index].power.first + sorted_num_units[i].first;
				teams[team_index].power.second = teams[team_index].power.second + sorted_num_units[i].second;
				
			}

			for (int i = 0; i < startindexoffset; i++)
			{
				index = checkindex(i);
				teams[i].indices.push_back(index);
				teams[i].resources.push_back(make_pair(sorted_num_units[i].first, sorted_num_units[i].second));
				teams[i].power.first = teams[i].power.first + sorted_num_units[i].first;
				teams[i].power.second = teams[i].power.second + sorted_num_units[i].second;
				
				
			}


			

		}
		else if (units_per_team >=2)
		{

			sort(sorted_num_units.begin(), sorted_num_units.end(), pairCompare);

			total_attack = (accumulate(sorted_num_units.begin(), sorted_num_units.end(), make_pair(0, 0), attack_sum()));
			total_defense = (accumulate(sorted_num_units.begin(), sorted_num_units.end(), make_pair(0, 0), defense_sum()));

			attack_threshold = (total_attack.first) / num_teams;
			defense_threshold = (total_defense.second) / num_teams;

			threshold = make_pair(attack_threshold, defense_threshold);
			std::cout << "Threshold: " << threshold.first << "," << threshold.second<< endl;
		
				
				if (((num_units.size()) % num_teams) == 0)
				{
					
					flag = false;
					startindex = 0;
					endindex = sorted_num_units.size() - 1;
				}
				else
				{
					flag = true;
					startindexoffset = (num_units.size()%num_teams);
					units_per_team = (num_units.size() - startindexoffset) / num_teams;
				}
				
				int j = 0;
				for (int i = 0; i < num_teams; i++)
				{

					j = 0;
					team_index++;

					if (flag == true)
					{
						startindex = i+startindexoffset;
						endindex = (sorted_num_units.size() - 1) - i;
					}
					else
					{
						startindex = i;
						endindex = (sorted_num_units.size() - 1) - i;
					}
					if ((((int)units_per_team) % 2) == 0)
					{
						while (j < units_per_team)
						{
							j = j + 2;
							index = checkindex(startindex);

							teams[team_index].indices.push_back(index);
							teams[team_index].resources.push_back(make_pair(sorted_num_units[startindex].first, sorted_num_units[startindex].second));
							teams[team_index].power.first = teams[team_index].power.first + sorted_num_units[startindex].first;
							teams[team_index].power.second = teams[team_index].power.second + sorted_num_units[startindex].second;
							

							index = checkindex(endindex);
							teams[team_index].indices.push_back(index);
							teams[team_index].resources.push_back(make_pair(sorted_num_units[endindex].first, sorted_num_units[endindex].second));
							teams[team_index].power.first = teams[team_index].power.first + sorted_num_units[endindex].first;
							teams[team_index].power.second = teams[team_index].power.second + sorted_num_units[endindex].second;
													
							startindex = startindex + 2;
							endindex = endindex - 2;
						}
						
					}
					else
					{
						while (j < units_per_team)
						{
							if ((units_per_team - j) != 1)
							{

								j = j + 2;
								
								index = checkindex(startindex);
								teams[team_index].indices.push_back(index);
								teams[team_index].resources.push_back(make_pair(sorted_num_units[startindex].first, sorted_num_units[startindex].second));
								teams[team_index].power.first = teams[team_index].power.first + sorted_num_units[startindex].first;
								teams[team_index].power.second = teams[team_index].power.second + sorted_num_units[startindex].second;

								index = checkindex(endindex);
								teams[team_index].indices.push_back(index);
								teams[team_index].resources.push_back(make_pair(sorted_num_units[endindex].first, sorted_num_units[endindex].second));
								teams[team_index].power.first = teams[team_index].power.first + sorted_num_units[endindex].first;
								teams[team_index].power.second = teams[team_index].power.second + sorted_num_units[endindex].second;


								if (((units_per_team - j) == 1))
								{
								
									index = checkindex((startindex+num_teams));
									teams[team_index].indices.push_back(index);
									teams[team_index].resources.push_back(make_pair(sorted_num_units[(startindex + num_teams)].first, sorted_num_units[(startindex + num_teams)].second));
									teams[team_index].power.first = teams[team_index].power.first + sorted_num_units[(startindex + num_teams)].first;
									teams[team_index].power.second = teams[team_index].power.second + sorted_num_units[(startindex + num_teams)].second;


								}
								startindex = startindex + 2;
								endindex = endindex - 2;

							}
							else
							{
								j = j + 2;
							}

						}
				

					}
				}

				if (flag == true)
				for (int i = 0; i < startindexoffset; i++)
				{
					index = checkindex(i);
					teams[i].indices.push_back(index);
					teams[i].resources.push_back(make_pair(sorted_num_units[i].first, sorted_num_units[i].second));
					teams[i].power.first = teams[i].power.first + sorted_num_units[i].first;
					teams[i].power.second = teams[i].power.second + sorted_num_units[i].second;
				}


			

		}


		for (int i = 0; i < num_teams; i++)
		{
			std::cout << "Team: " << i << endl;

			std::cout << "Team indices: " << endl;
			for (int j = 0; j < teams[i].indices.size(); j++)
			{
				std::cout << teams[i].indices[j] << ",";
			}
			//std::cout << endl << "Team resources: " << endl;
			for (int j = 0; j < teams[i].resources.size(); j++)
			{

				//std::cout << teams[i].resources[j].first << "," << teams[i].resources[j].second << endl;
			}

			std::cout << "Team Power: " << teams[i].power.first << ", " << teams[i].power.second << endl;

		}







		for (int i = 0; i<num_teams; i++)
		{
			teams[i].team_threshold.first = teams[i].power.first - threshold.first;
			teams[i].team_threshold.second = teams[i].power.second - threshold.second;
		}

		checker.erase(checker.begin(), checker.end());
		pair<int, int>swapper;
		bool resource_flag = true;
		for (int i = 0; i < num_teams; i++)
		{
			if (checker.count(i)<1)
			{
				checker.insert(i);

				if (((teams[i].team_threshold.first)>1) || ((teams[i].team_threshold.second)>1) || ((teams[i].team_threshold.first)<(-1)) || ((teams[i].team_threshold.second)<(-1)))
				{
					
					for (int j = 0; j < teams[i].resources.size(); j++)
					{
						
						swapper.first = (teams[i].resources[j].first - teams[i].team_threshold.first);
						swapper.second = (teams[i].resources[j].second - teams[i].team_threshold.second);
						flag = true;
												
						for (int k = i+1; k < num_teams; k++)
						{
							if ((!(checker.count(k)>1))&&(resource_flag == true))
							{
								for (int h = 0; h < teams[k].resources.size(); h++)
								{
									
									
									if (abs(teams[i].team_threshold.first) > (abs(teams[i].team_threshold.second)))
									{
					
										if (teams[k].resources[h].first == swapper.first)
										{
											if ( abs(teams[k].resources[h].second - swapper.second) <= 2)
											{
												swap(teams[i].resources[j].first, teams[k].resources[h].first);
												swap(teams[i].resources[j].second, teams[k].resources[h].second);
												swap(teams[i].indices[j], teams[k].indices[h]);
												teams[i].power = make_pair(0, 0);
												resource_flag = false;
												break;
											}
										}
										else if (teams[k].resources[h].second == swapper.second)
										{
											if ( abs(teams[k].resources[h].first - swapper.first) <= 2)
											{
												swap(teams[i].resources[j].first, teams[k].resources[h].first);
												swap(teams[i].resources[j].second, teams[k].resources[h].second);
												swap(teams[i].indices[j], teams[k].indices[h]);
												teams[i].power = make_pair(0, 0);
												resource_flag = false;
												break;
											}
										}
									}
									else
									{
										if (teams[k].resources[h].second == swapper.second)
										{
											if ( abs(teams[k].resources[h].first - swapper.first) <= 2)
											{
												swap(teams[i].resources[j].first, teams[k].resources[h].first);
												swap(teams[i].resources[j].second, teams[k].resources[h].second);
												swap(teams[i].indices[j], teams[k].indices[h]);
												teams[i].power = make_pair(0,0);
												resource_flag = false;
												break;
											}
										}
										else if (teams[k].resources[h].first == swapper.first)
										{
											if ( abs(teams[k].resources[h].second - swapper.second) <= 2)
											{
												swap(teams[i].resources[j].first, teams[k].resources[h].first);
												swap(teams[i].resources[j].second, teams[k].resources[h].second);
												swap(teams[i].indices[j], teams[k].indices[h]);
												teams[i].power = make_pair(0, 0);
												resource_flag = false;
												break;
											}
										}

									}
									}
								}
							}
					
						}

					}
				}
		}



		for (int i = 0; i < num_teams; i++)
		{
			teams[i].power = make_pair(0,0);	
		}


		for (int i = 0; i < num_teams; i++)
		{
			for (int j = 0; j < teams[i].indices.size(); j++)
			{
				teams[i].power.first = teams[i].power.first + teams[i].resources[j].first;
				teams[i].power.second = teams[i].power.second + teams[i].resources[j].second;
			}
			
		}



		cout << "----------------------------------------------" << endl;
		for (int i = 0; i < num_teams; i++)
		{
			std::cout << "Team: " << i << endl;

			std::cout << "Team indices: " << endl;
			for (int j = 0; j < teams[i].indices.size(); j++)
			{
				std::cout << teams[i].indices[j] << ",";
			}
			std::cout << endl << "Team resources: " << endl;
			for (int j = 0; j < teams[i].resources.size(); j++)
			{

				std::cout << teams[i].resources[j].first << "," << teams[i].resources[j].second << endl;
			}

			std::cout << "Team Power: " << teams[i].power.first << ", " << teams[i].power.second << endl;

		}


		delete[] teams;

	}
	else
	{
		std::cout << "Error opening file" << endl;
	}



	_getch();
	return 0;
}