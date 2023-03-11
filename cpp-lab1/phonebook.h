//
// Created by Safina on 24.09.2022.
//

#ifndef PHONEBOOK_H
#define PHONEBOOK_H(args)
#include <stdio.h>

typedef class phonebook
{
	char surname[25];
	char name[25];
	char patronymic[25];
	long long number;
	friend bool operator==(const phonebook &member1, const phonebook &member2);
	friend bool operator<=(const phonebook &member1, const phonebook &member2);
	friend bool operator<(const phonebook &member1, const phonebook &member2);
	friend bool operator>=(const phonebook &member1, const phonebook &member2);
	friend bool operator>(const phonebook &member1, const phonebook &member2);

  public:
	void get_surname(char *ans) const;
	void get_name(char *ans) const;
	void get_patronymic(char *ans) const;
	long long get_number() const;
	void set_surname(char *ans);
	void set_name(char *ans);
	void set_number(long long int ans);
	void set_patronymic(char *ans);

} phonebook;

#endif
