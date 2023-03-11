//
// Created by Safina on 24.09.2022.
//
#include "phonebook.h"

#include <cstring>
#include <stdio.h>

void phonebook::get_surname(char *ans) const
{
	strcpy(ans, surname);
}
void phonebook::get_name(char *ans) const
{
	strcpy(ans, name);
}
void phonebook::get_patronymic(char *ans) const
{
	strcpy(ans, patronymic);
}
long long phonebook::get_number() const
{
	return number;
}
void phonebook::set_surname(char *ans)
{
	strcpy(surname, ans);
}
void phonebook::set_name(char *ans)
{
	strcpy(name, ans);
}
void phonebook::set_patronymic(char *ans)
{
	strcpy(patronymic, ans);
}
void phonebook::set_number(long long ans)
{
	number = ans;
}

bool operator<(const phonebook &member1, const phonebook &member2)
{
	if (strcmp(member1.surname, member2.surname) <= 0)
	{
		if (strcmp(member1.surname, member2.surname) == 0)
		{
			if (strcmp(member1.name, member2.name) <= 0)
			{
				if (strcmp(member1.name, member2.name) == 0)
				{
					if (strcmp(member1.patronymic, member2.patronymic) <= 0)
					{
						if (strcmp(member1.patronymic, member2.patronymic) == 0)
						{
							return member1.number < member2.number;
						}
						return true;
					}
					return false;
				}
				return true;
			}
			return false;
		}
		return true;
	}
	return false;
}

bool operator>(const phonebook &member1, const phonebook &member2)
{
	return member2 < member1;
}

bool operator==(const phonebook &member, const phonebook &member2)
{
	return (member >= member2) && (member2 >= member);
}
bool operator<=(const phonebook &member1, const phonebook &member2)
{
	return !(member1 > member2);
}

bool operator>=(const phonebook &member1, const phonebook &member2)
{
	return !(member1 < member2);
}
