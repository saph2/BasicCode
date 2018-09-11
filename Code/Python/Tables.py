import csv
import re


class Table_Parser():

    def __init__(self):
        self.table = dict()

    def parse_table(self, input_file, table_location):
        # Parse lines into table dictionary <row_title, [value1,...]> #
        self.input_file = input_file
        location = 0
        for line in self.input_file:
            if (location < table_location):
                location = location + 1
                continue
            if ('--' in line):
                return
            line = line.strip('\n')
            row = re.split(r'\s{2,}', line)
            if ((len(row) > 1 and row[1] == " ") or row[0]==''):
                continue
            key = row[0]
            self.table[key] = row[1:]

    def get_table(self):
        return self.table

#######################################################################################

class Table():

    def __init__(self, table_name, table_headline, table_location, table_parser=Table_Parser()):
        self.table_name = table_name
        self.table_headline = table_headline
        self.table_location = table_location
        self.table_parser = table_parser
        self.table = dict()

    def set_section_counters(self, total_counter, current_counter):
        self.total_counter = total_counter
        self.current_counter = current_counter

    def reset_table(self):
        return
        
    def parse_table(self, input_file, table_name=None):
        self.table_parser.parse_table(input_file, self.table_location)
        self.table = self.table_parser.get_table()

    def get_table(self):
        return self.table
    
    def get_table_name(self):
        return self.table_name

    def updated_table_values(self, new_table):
       return

class Live_Table(Table):

    def updated_table_values(self, new_table):
        new_table = new_table.get_table()
        for row in new_table:
            if row in self.table:
                for i in range (0, len(self.table[row])):
                    self.table[row][i] = float(self.table[row][i]) + float(new_table[row][i])
        return self.table

class Static_Table(Table):

    def updated_table_values(self, new_table):
        new_table = new_table.get_table()
        for row in new_table:
            if row in self.table:
                for i in range (0, len(self.table[row])):
                    self.table[row][i] = float(new_table[row][i]) - float(self.table[row][i])
        return self.table

#######################################################################################

class Joined_Tables(Table):
    
    def __init__(self):
        self.tables_to_parse = dict()
        self.table = dict()
        self.table_name = None
    
    def reset_table(self):
        self.table = dict()

    def add_table(self, table):
        self.tables_to_parse[table.get_table_name()] = table
        if (not self.table_name):
            self.table_name = table.get_table_name()
        else:
            self.table_name = "{0}_{1}".format(self.table_name, table.get_table_name())

    def update_joined_table(self, new_table):
        for row in new_table:
            if row not in self.table:
                self.table[row] = []
            self.table[row] = self.table[row] + new_table[row]

    def parse_table(self, input_file, table_name=None):
        if (table_name and table_name in self.tables_to_parse):
            self.tables_to_parse[table_name].parse_table(input_file)
            self.update_joined_table(self.tables_to_parse[table_name].get_table()) 

class Joined_Live_Tables(Joined_Tables):
    
    def updated_table_values(self, new_table):
        new_table = new_table.get_table()
        for row in new_table:
            if row in self.table:
                for i in range (0, len(self.table[row])):
                    self.table[row][i] = float(self.table[row][i]) + float(new_table[row][i])
        return self.table


class Joined_Static_Tables(Joined_Tables):

    def parse_table(self, input_file, table_name=None):
        # Only parse first and last table #
        if (self.current_counter == 1):
            super().parse_table(input_file, table_name)
        if (self.current_counter == self.total_counter):
            super().parse_table(input_file, table_name)
    
    def updated_table_values(self, new_table):
        new_table = new_table.get_table()
        for row in new_table:
            if row in self.table:
                for i in range (0, len(self.table[row])):
                    self.table[row][i] = float(new_table[row][i]) - float(self.table[row][i])
        return self.table

#######################################################################################

class Section_Parser():

    def __init__(self):
        self.next_line = None
        self.section_parsed_tables = dict()

    def update_parsed_tables_data(self, section_tables, instance):
        new_parsed_tables = dict()
        new_parsed_tables[instance] = dict()
        for table in section_tables:
            table_name = section_tables[table].get_table_name()
            new_parsed_tables[instance][table_name] = section_tables[table]
        if instance not in self.section_parsed_tables:
            self.section_parsed_tables[instance] = new_parsed_tables[instance]
            return
        self.section_parsed_tables[instance].update(new_parsed_tables[instance])

    def parse_section(self, input_file, section_tables, instance):
        # Parse all selected tables under current section #
        self.input_file = input_file
        
        for table in section_tables:
            section_tables[table].reset_table()
        
        for line in self.input_file:
            self.next_line = line
            if ('.Key' in line):
                return
            for table in section_tables:
                if table in line:
                    section_tables[table].parse_table(input_file, table)
    
    def get_next_line(self):
        return self.next_line
    
    def get_parsed_tables_data(self):
        return self.section_parsed_tables

    def print_tables(self):
        for instance in self.section_parsed_tables:
            print (instance)
            for table in self.section_parsed_tables[instance]:
                current_table = self.section_parsed_tables[instance][table].get_table()
                print (current_table)

#######################################################################################             
            
class Section():

    def __init__(self, section_name):
        self.section_name = section_name
        self.section_tables = dict()
        self.section_parser = Section_Parser()
    
    def set_section_counters(self, total_counter, current_counter):
        self.total_counter = total_counter
        self.current_counter = current_counter
        for table in self.section_tables:
           self.section_tables[table].set_section_counters(total_counter, current_counter)

    def add_table(self, table_name, table):
        # Add table parser and metadata to section #
        self.section_tables[table_name] = table
        
    def parse_section(self, input_file, instance):
        # Parse all tables under the corrent section and instance #
        self.section_parser.parse_section(input_file, self.section_tables, instance)
        self.section_parser.update_parsed_tables_data(self.section_tables, instance)
        
    def get_section_name(self):
        return self.section_name

    def get_next_line(self):
        return self.section_parser.get_next_line()

    def print_tables(self):
       return# self.section_parser.print_tables()

#######################################################################################

class Parser():

    def __init__(self):
        self.sections_list = dict()
        self.total_counter = 0
        self.current_counter = 0

    def add_section(self, section):
        self.sections_list[section.get_section_name()] = section

    def get_instnace(self, section, line):
        return line.split(section)[-1]

    def parse_file(self, input_file):
        # Parse all sections in the sections list by their tables and parsers #
        self.input_file = input_file
        for line in self.input_file:
            if ('.Key.#' in line):
                self.total_counter = self.total_counter + 1
        input_file.seek(0)
        line = self.input_file.readline()
        while (line):
            scetion_found = False
            if ('.Key.#' in line):
                self.current_counter = self.current_counter + 1
                line = self.input_file.readline()
                continue
            if ('.Key' in line):
                for section in self.sections_list:
                    if section in line:
                        instance = self.get_instnace(section, line)
                        self.sections_list[section].set_section_counters(self.total_counter, self.current_counter)
                        self.sections_list[section].parse_section(self.input_file, instance)
                        line = self.sections_list[section].get_next_line()
                        scetion_found = True
                        break
            if (not scetion_found):
                line = self.input_file.readline()
    
    def print_tables(self):
        for section in self.sections_list:
            self.sections_list[section].print_tables()

#######################################################################################
#######################################################################################
#######################################################################################              

section = Section('.Key.Section1.Tab1.')
table1 = Table('Table1', ['Col1', 'Col2', 'Col3'], 2, Table_Parser())
#section.add_table(table1)
table2 = Table('Table2', ['Col1', 'Col2', 'Col3'], 2, Table_Parser())
#section.add_table(table2)
table3 = Table('Table3', ['Col1', 'Col2', 'Col3'], 4, Table_Parser())
#section.add_table(table3)

joined_tables = Joined_Live_Tables()
joined_tables.add_table(table1)
joined_tables.add_table(table2)
section.add_table("Table1", joined_tables)
section.add_table("Table2", joined_tables)

parser = Parser()
parser.add_section(section)

with open ('s_tables.elg', 'r') as input_file:
    parser.parse_file(input_file)
    parser.print_tables()
    input_file.close()
    


