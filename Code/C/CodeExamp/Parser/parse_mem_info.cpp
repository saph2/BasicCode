/*
 * parse_mem_info.cpp
 *
 *  Created on: 1 ·Ó‡È 2017
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

#define g_system_mem_capasity_limit 0.8

int get_line(const char* filename, const char* titlename,const char* tag_begin,const char* tag_end){
	std::ifstream infile(filename);
	std::string line;
	while (std::getline(infile, line) && line.find(titlename)==nopos){
		continue;
	}
	if (!line || line.find(titlename)==nopos) return -1;
	line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
	pos_begin = line.find(tag_begin)+strlen(tag_begin);
	if (pos_begin==nopos) return -1;
	pos_end = line.find(tag_end);
	if (pos_end==nopos) return -1;
	char* line_pt = (char*) line.substr(pos_begin).c_str();
	char value[32];
	memcpy(value, line_pt, pos_end-pos_begin);
	value[pos_end-pos_begin]='\0';
	return atoi(val);
}


int get_memory_status(){
	const char* path = "/proc/meminfo";
	const char* tag_begin = ":";
	const char* tag_end = "kB";
	int memTotal = get_line(path,"MemTotal",tag_begin,tag_end);
	if (memTotal<0)return -1;
	int memFree = get_line(path,"MemFree",tag_begin,tag_end);
	if (memFree<0)return -1;
	float result = (float) (memFree*1.0)/memTotal;
	if (result>g_system_mem_capasity_limit){
		return 1;
	}
	return 0;
}

int take_top_proc_snapshot(){
	int rc = system("top -n 1 | grep -A 10 PID > /tmp/memspy_topinfo ; cat /tmp/memspy_topinfo | awk '{print $2}' > /tmp/memspy_toppid");
	if (rc<0) return -1;
	const char* path = "/tmp/topÙid";
	std::ifstream infile(path);
	std::string line;
	if (!std::getline(infile, line)) return -1;
	if(line.find("PID")==nopos) return -1;
	int i;
	for (i=0; i<10; i++){
		if (!std::getline(infile, line)) return -1;
		line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
		int pid = atoi(line.c_str());
		printf("pid: %d\n",pid);
	}
	return 0;
}

int check_system_state(){
	int rc = get_memory_status();
	if (rc<0) return -1;
	if (rc == 0) return 0;
	if (rc == 1){
		rc = take_top_proc_snapshot();
		if (rc<0) return -1;
	}
	return 0;
}




