#include <iostream>
#include <ctime>
#include <iomanip>
#include <string>
#include <sstream>

bool is_correct_input_timer(const std::string& timer)
{
	int count = 0, minutes, second, temp1 = 0, temp2;

	if (timer.empty()) return false;

	for (int i = 0; i < timer.length(); ++i)
	{
		if ((timer[i] == ':' && i == 0) ||
			(timer[i] == ':' && i == (timer.length() - 1))) return false;
		if (timer[i] == ':')
		{
			temp2 = i;
			++count;
		}
		if ((timer[i] < '0' || timer[i] > '9') &&
			(timer[i] != ':')) return false;
	}

	if (count > 1 || count < 1) return false;

	minutes = std::stoi(timer.substr(temp1, temp2));
	second = std::stoi(timer.substr(temp2 + 1, timer.length()));

	if (minutes < 0 || second < 0 || 
		minutes > 59 || second > 59) return false;
	if (minutes == 0 && second == 0) return false;

	return true;
}

void setup_timer(std::tm& timer, const int& minutes, const int& seconds)
{
	if (timer.tm_min + minutes == 60)
	{
		if (timer.tm_hour == 23)
		{
			timer.tm_wday++;
			timer.tm_mday++;
			timer.tm_hour = 0;
			timer.tm_min = 0;
		}
		else
		{
			timer.tm_hour++;
			timer.tm_min = 0;
		}
	}
	else if (timer.tm_min + minutes < 60)
		timer.tm_min += minutes;
	else 
	{
		if (timer.tm_hour == 23) 
		{
			timer.tm_wday++;
			timer.tm_mday++;
			timer.tm_hour = 0;
			timer.tm_min = (timer.tm_min + minutes) % 60;
		}
		else
		{
			timer.tm_hour++;
			timer.tm_min = (timer.tm_min + minutes) % 60;
		}
	}

	if (timer.tm_sec + seconds == 60)
	{
		if (timer.tm_min == 59)
		{
			if (timer.tm_hour == 23) // 23:59 | 33 + 27 | 00:00:00
			{
				timer.tm_wday++;
				timer.tm_mday++;
				timer.tm_hour = 0;
				timer.tm_min = 0;
				timer.tm_sec = 0;
			}
			else // 22:59 | 33 + 27 | 23:00:00
			{
				timer.tm_hour = 0;
				timer.tm_min = 0;
				timer.tm_sec = 0;
			}
		}
		else // 22:57 | 33 + 27 | 22:58:00
		{
			timer.tm_min++;
			timer.tm_sec = 0;
		}
	}
	else if (timer.tm_sec + seconds < 60)
		timer.tm_sec += seconds;
	else // timer.tm_sec + seconds > 60
	{
		if (timer.tm_min == 59)
		{
			if (timer.tm_hour == 23) // 23:59:40 | 40 + 40 | 00:00:20
			{
				timer.tm_wday++;
				timer.tm_mday++;
				timer.tm_hour = 0;
				timer.tm_min = 0;
				timer.tm_sec = (timer.tm_sec + seconds) % 60;
			}
			else // 22:59:40 | 40 + 40 | 23:00:20
			{
				timer.tm_hour++;
				timer.tm_min = 0;
				timer.tm_sec = (timer.tm_sec + seconds) % 60;
			}
		}
		else // 22:58:40 | 40 + 40 | 22:59:20
		{
			timer.tm_min++;
			timer.tm_sec = (timer.tm_sec + seconds) % 60;
		}
	}
}

int main()
{
	std::string SetTimer;
	std::time_t currentTimeInSecond;
	std::tm localTime;

	std::tm timer;

	std::cout << "Set a timer(format: 33:33 (Max - 59:59. Min - 00:01)): ";
	std::cin >> SetTimer;
	if (!is_correct_input_timer(SetTimer))
	{
		std::cerr << "Error! Incorrect input data." << std::endl;
		return 0;
	}

	currentTimeInSecond = std::time(nullptr);
	timer = *std::localtime(&currentTimeInSecond);

	int minutes = timer.tm_min;
	int seconds = timer.tm_sec;

	std::stringstream str(SetTimer);
	str >> std::get_time(&timer, "%M:%S");
	setup_timer(timer, minutes, seconds);

	localTime = *std::localtime(&currentTimeInSecond);

	std::time_t timerInSecond = std::mktime(&timer);

	int temp = currentTimeInSecond;

	std::cout << "Current time: " << std::asctime(&localTime);
	std::cout << "The timer will sound in: " << std::asctime(&timer);
	while (true)
	{
		if (currentTimeInSecond > temp)
		{
			system("cls");
			temp = currentTimeInSecond;
			std::cout << "Current time: " << std::asctime(&localTime);
			std::cout << "The timer will sound in: " << std::asctime(&timer);
		}
		if (currentTimeInSecond == timerInSecond)
		{
			std::cout << "DING! DING! DING! " << std::endl;
			return 0;
		}
		currentTimeInSecond = std::time(nullptr);
		localTime = *std::localtime(&currentTimeInSecond);
	}
}
