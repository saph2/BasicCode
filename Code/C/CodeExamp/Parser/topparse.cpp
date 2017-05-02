/*
 * test.cpp
 *
 *  Created on: 21 באפר 2017
 *      Author: saph1
 */

#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <sstream>
#include <string>
#include <string.h>
#include <fstream>

#define nopos std::string::npos

class MAP_DS{
	char firstname[200];
	char lastname[200];
	char nickname[200];
	int salary;
public:
	char* get_firstname(){ return firstname; }
	void set_firstname(char* str){ snprintf(firstname,sizeof(firstname)-1,"%s",str);}
	char* get_lastname(){ return lastname; }
	void set_lastname(char* str){ snprintf(lastname,sizeof(lastname)-1,"%s",str); }
	char* get_nickname(){ return nickname; }
	void set_nickname(char* str){ snprintf(nickname,sizeof(nickname)-1,"%s",str); }
	int get_salary(){ return salary; }
	void set_salary(int num){ salary=num; }
};

std::map<char* ,MAP_DS> staff_map;

void print_map(){
	std::map<char*,MAP_DS>::iterator it;
	for (it=staff_map.begin(); it!=staff_map.end(); ++it){
		printf("staff name: %s\n",it->first);
		printf("\tfirstname: %s\n",it->second.get_firstname());
		printf("\tlastname: %s\n",it->second.get_lastname());
		printf("\tnickname: %s\n",it->second.get_nickname());
		printf("\tsalary: %d\n",it->second.get_salary());
	}
}

int get_line(std::ifstream * infile,char* tag_begin, char* tag_end, char* field, std::string *current_line){
	std::string line;
	std::size_t pos_begin, pos_end;
	if(!std::getline(*infile, line))
		return -1;
	pos_begin = line.find(tag_begin)+strlen(tag_begin);
	if (pos_begin==nopos) return -1;
	pos_end = line.find(tag_end);
	if (pos_end==nopos) return -1;
	char* line_pt = (char*) line.substr(pos_begin).c_str();
	memcpy(field, line_pt, pos_end-pos_begin);
	field[pos_end-pos_begin]='\0';
	printf("%s\n",field);
	*current_line=line;
	return 0;
}

int update_map(std::ifstream * infile, char* name, std::string *current_line){
	staff_map[name];
	char field[200];
	MAP_DS map_pt = (MAP_DS) staff_map[name];
	std::string line;
	int rc;
	rc = get_line(infile, "<firatname>","</firstname>",field, current_line);
	printf("%s\n",field);
	if (rc<0) return -1;
	map_pt.set_firstname(field);
	rc = get_line(infile, "<lastname>","</lastname>",field, current_line);
	if (rc<0) return -1;
	map_pt.set_lastname(field);
	rc = get_line(infile, "<nickname>","</nickname>",field, current_line);
	if (rc<0) return -1;
	map_pt.set_nickname(field);
	rc = get_line(infile, "<salary>","</salary>",field, current_line);
	if (rc<0) return -1;
	map_pt.set_salary(atoi(field));
	*current_line=line;
	return 0;
}

int update_staff_map_rec(std::ifstream * infile, std::string *current_line){
	std::string line;
	std::size_t pos_begin, pos_end;
	char *line_pt;
	while (std::getline(*infile, line) && line.find("<staff name=")!=nopos){
		int rc;
		char* name = (char*) calloc(200,sizeof(char));
		pos_begin = line.find("=")+2;
		if (pos_begin==nopos) return -1;
		pos_end = line.find(">")-1;
		if (pos_end==nopos) return -1;
		line_pt = (char*) line.substr(pos_begin).c_str();
		memcpy(name, line_pt, pos_end-pos_begin);
		name[pos_end-pos_begin]='\0';
		printf("%s\n",name);
		rc = update_map(infile, name, current_line);
		if (rc<0) return -1;
		if (!std::getline(*infile, line) ||  line.find("</staff>")==nopos) return -1;
	}
	*current_line=line;
	return 0;
}

int update_staff_map(){
	std::ifstream infile("C:\\Users\\saph1\\Desktop\\text.xml");
	std::string line;
	if (!std::getline(infile, line)){
		printf("Illegal xml format!\n");
		return -1;
	}
	if (line.find("<?xml version=\"1.0\"?>")==nopos){
		printf("Illegal xml format!\n");
		return -1;
	}
	int rc = -1;
	if (std::getline(infile, line) && line.find("<company>")!=nopos){
		rc = update_staff_map_rec(&infile,&line);
		if (rc<0){
			printf("Illegal xml format!\n");
			return -1;
		}
		if (line.find("</company>")==nopos){
			printf("%s\n",line.c_str());
			printf("Illegal xml format!\n");
			return -1;
		}
	}
	return 0;
}

int main() {
	if(update_staff_map()==0){
		print_map();
	}
	printf("end!\n");
	return 0;
}
